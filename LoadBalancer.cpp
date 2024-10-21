#include "LoadBalancer.h"
#include <iostream>
#include <random>
#include <chrono>
#include <fstream>

LoadBalancer::LoadBalancer(int num_servers) : running(false), rejected_requests(0), min_task_time(500), max_task_time(3000) {
    // Initialize web servers
    for (int i = 0; i < num_servers; ++i) {
        servers.push_back(new WebServer(i + 1));
    }


    // Open the log file
    log_file.open("loadbalancer_log.txt", std::ios_base::app);
    if (!log_file) {
        std::cerr << "Error opening log file!\n";
    }
}

LoadBalancer::~LoadBalancer() {
    // Clean up dynamically allocated servers
    for (auto server : servers) {
        delete server;
    }


    // Close the log file
    if (log_file.is_open()) {
        log_file.close();
    }
}

void LoadBalancer::start(int run_time) {
    running = true;
    total_run_time = run_time;

    // Generate initial requests
    int initial_queue_size = servers.size() * 100;
    generate_initial_requests(initial_queue_size);


    // Log the initial queue size and task time range
    log_file << "Initial queue size: " << request_queue.size() << '\n';
    log_file << "Task time range: " << min_task_time << " - " << max_task_time << " milliseconds\n";


    // Start server threads
    for (auto server : servers) {
        server_threads.push_back(std::thread(&WebServer::run, server));
    }

    // Start request generator thread
    std::thread request_gen_thread(&LoadBalancer::request_generator, this);

    // Start distributing requests
    distribute_requests();

    // Wait for servers to finish
    for (auto& th : server_threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    // Wait for request generator to finish
    if (request_gen_thread.joinable()) {
        request_gen_thread.join();
    }

    running = false;


    // Log the ending queue size and final server status
    log_status();
}

void LoadBalancer::generate_initial_requests(int num_requests) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    for (int i = 0; i < num_requests; ++i) {
        request_queue.push(generate_random_request());
    }
}

void LoadBalancer::request_generator() {
    // Simulate adding new requests over time
    auto start_time = std::chrono::steady_clock::now();
    while (running) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            request_queue.push(generate_random_request());
        }
        // Wait for a random interval before generating the next request
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        // Check if total run time has elapsed
        auto current_time = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() >= total_run_time) {
            running = false;
        }
    }
}

void LoadBalancer::distribute_requests() {
    while (running) {
        // Check for available servers and assign requests
        for (auto server : servers) {
            if (server->ready()) {
                std::lock_guard<std::mutex> lock(queue_mutex);
                if (!request_queue.empty()) {
                    Request req = request_queue.front();
                    request_queue.pop();
                    server->assign_request(req);
                }
            }
        }
        // Sleep briefly to prevent tight looping
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    // Signal servers to stop
    for (auto server : servers) {
        server->stop();
    }
}

Request LoadBalancer::generate_random_request() {
    // Random IP generation and time
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> ip_part(0, 255);
    std::uniform_int_distribution<int> time_dist(min_task_time, max_task_time); // milliseconds
    std::uniform_int_distribution<int> job_type_dist(0, 1);

    std::string ip_in = std::to_string(ip_part(rng)) + "." + std::to_string(ip_part(rng)) + "." +
                        std::to_string(ip_part(rng)) + "." + std::to_string(ip_part(rng));

    std::string ip_out = "10.0.0." + std::to_string(ip_part(rng));

    int time = time_dist(rng);
    char job_type = job_type_dist(rng) == 0 ? 'P' : 'S';

    return Request(ip_in, ip_out, time, job_type);
}




void LoadBalancer::log_status() {
    log_file << "Final queue size: " << request_queue.size() << '\n';
    int active_servers = 0;
    int inactive_servers = 0;

    for (auto server : servers) {
        if (server->ready()) {
            inactive_servers++;
        } else {
            active_servers++;
        }
    }

    log_file << "Active servers: " << active_servers << '\n';
    log_file << "Inactive servers: " << inactive_servers << '\n';
    log_file << "Rejected/discarded requests: " << rejected_requests << '\n';
}