#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include "Request.h"
#include "WebServer.h"
#include <fstream>


/**
 * @class LoadBalancer
 * @brief Manages load balancing of requests to multiple web servers.
 *
 * The LoadBalancer class is responsible for generating requests, distributing them to available web servers, 
 * and managing the overall system by using threading to simulate concurrent request handling.
 */
class LoadBalancer {
public:
    /**
     * @brief Constructs a LoadBalancer object.
     * @param num_servers The number of web servers to initialize.
     */
    LoadBalancer(int num_servers);


    /**
     * @brief Destructs the LoadBalancer object and cleans up resources.
     */
    ~LoadBalancer();

    
    /**
     * @brief Starts the load balancer operation for a given runtime.
     * @param run_time The number of seconds the load balancer should run.
     */
    void start(int run_time);

private:
    std::queue<Request> request_queue;      /**< Queue of requests to be processed. */
    std::vector<WebServer*> servers;        /**< Vector of web servers managed by the load balancer. */
    std::vector<std::thread> server_threads; /**< Vector of threads representing server processing. */
    std::mutex queue_mutex;                 /**< Mutex to protect the request queue from race conditions. */
    bool running;                           /**< Flag to indicate if the load balancer is still running. */
    int total_run_time;                     /**< The total time the load balancer should run. */
    int initial_queue_size;                 /**< The size of the queue when the load balancer starts. */
    int rejected_requests;                  /**< Count of rejected requests. */
    int min_task_time;                      /**< Minimum time for a task. */
    int max_task_time;                      /**< Maximum time for a task. */

    std::ofstream log_file;                 /**< Log file to write server activities and request status. */


    /**
     * @brief Generates the initial set of requests for the servers.
     * @param num_requests The number of requests to generate initially.
     */
    void generate_initial_requests(int num_requests);

    /**
     * @brief Continuously generates new requests as the simulation runs.
     */
    void request_generator();

    /**
     * @brief Distributes requests from the queue to available servers.
     */
    void distribute_requests();

    /**
     * @brief Generates a random request with random IPs and processing times.
     * @return A randomly generated Request object.
     */
    Request generate_random_request();

    /**
     * @brief Logs the status of the load balancer, including queue size, server activity, and more.
     */
    void log_status();
};

#endif