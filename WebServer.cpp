// #include "WebServer.h"
// #include <iostream>
// #include <thread>           // for simulating the processing time
// #include <chrono>           // for simulating time in seconds

// WebServer::WebServer(int id) {
//     server_id = id;
//     is_busy = false;
// }

// void WebServer::process_request(Request req) {
//     is_busy = true;
//     std::cout << "Server " << server_id << " processing request:\n";
//     req.display();

//     // Simulate processing time (blocking thread for the time of the request)
//     std::this_thread::sleep_for(std::chrono::milliseconds(req.time_i));

//     std::cout << "Server " << server_id << " completed request\n";
//     is_busy = false;
// }

// bool WebServer::ready() const {
//     return !is_busy;
// }








// #include "WebServer.h"
// #include <iostream>
// #include <thread>
// #include <chrono>

// WebServer::WebServer(int id) : server_id(id), has_request(false), running(true) {}

// WebServer::~WebServer() {}

// void WebServer::run() {
//     while (running) {
//         std::unique_lock<std::mutex> lock(mtx);
//         cv.wait(lock, [this] { return has_request || !running; });
//         if (has_request) {
//             process_request();
//             has_request = false;
//         }
//     }
// }

// void WebServer::assign_request(Request req) {
//     {
//         std::lock_guard<std::mutex> lock(mtx);
//         current_request = req;
//         has_request = true;
//     }
//     cv.notify_one();
// }

// void WebServer::process_request() {
//     std::cout << "Server " << server_id << " processing request:\n";
//     current_request.display();

//     // Simulate processing time
//     std::this_thread::sleep_for(std::chrono::milliseconds(current_request.time_i));

//     std::cout << "Server " << server_id << " completed request\n";
// }

// bool WebServer::ready() {
//     std::lock_guard<std::mutex> lock(mtx);
//     return !has_request;
// }

// void WebServer::stop() {
//     running = false;
//     cv.notify_one();
// }














// WebServer.cpp
#include "WebServer.h"
#include <iostream>
#include <fstream> // For logging to a file
#include <thread>
#include <chrono>

WebServer::WebServer(int id) : server_id(id), has_request(false), running(true) {}

WebServer::~WebServer() {}

void WebServer::run() {
    std::ofstream log_file("loadbalancer_log.txt", std::ios_base::app); // Open log file in append mode
    if (!log_file) {
        std::cerr << "Error opening log file!\n";
        return;
    }

    while (running) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return has_request || !running; });
        if (has_request) {
            process_request(log_file); // Pass the log file to log the request
            has_request = false;
        }
    }

    log_file.close();
}

void WebServer::assign_request(Request req) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        current_request = req;
        has_request = true;
    }
    cv.notify_one();
}

void WebServer::process_request(std::ofstream& log_file) {
    std::cout << "Server " << server_id << " processing request:\n";
    current_request.display();

    // Log the request details to the file
    log_file << "Server " << server_id << " processing request:\n";
    log_file << "Request: " << current_request.ip_in << " -> " << current_request.ip_out
             << " | Time: " << current_request.time_i << " cycles | Job Type: " << current_request.job_type << '\n';

    // Simulate processing time
    std::this_thread::sleep_for(std::chrono::milliseconds(current_request.time_i));

    std::cout << "Server " << server_id << " completed request\n";

    // Log the completion of the request
    log_file << "Server " << server_id << " completed request\n";
}

bool WebServer::ready() {
    std::lock_guard<std::mutex> lock(mtx);
    return !has_request;
}

void WebServer::stop() {
    running = false;
    cv.notify_one();
}
