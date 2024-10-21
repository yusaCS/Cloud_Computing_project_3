#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"
#include <mutex>
#include <condition_variable>


/**
 * @class WebServer
 * @brief Simulates a web server that processes requests from a load balancer.
 */
class WebServer {
public:

    /**
     * @brief Constructs a WebServer object.
     * @param id The unique identifier for the web server.
     */
    WebServer(int id);
    
    
    /**
     * @brief Destructor for WebServer.
     */
    ~WebServer();

    
    /**
     * @brief Runs the web server to process requests.
     */
    void run();


    /**
     * @brief Assigns a request to the web server for processing.
     * @param req The request to be processed.
     */
    void assign_request(Request req);

    /**
     * @brief Processes the assigned request.
     * @param log_file The log file to record the server's activity.
     */
    void process_request(std::ofstream& log_file);

    /**
     * @brief Checks if the server is ready to take a new request.
     * @return True if the server is idle, false if it's busy processing a request.
     */
    bool ready();

    /**
     * @brief Stops the server from running.
     */
    void stop();

private:
    int server_id;                      /**< Unique identifier for the server. */
    std::mutex mtx;                     /**< Mutex to protect server state. */
    std::condition_variable cv;         /**< Condition variable to manage request assignment. */
    bool has_request;                   /**< Flag indicating whether a request is being processed. */
    bool running;                       /**< Flag to indicate if the server is running. */
    Request current_request;            /**< The current request being processed. */

};

#endif