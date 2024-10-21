#include <iostream>
#include "Request.h"
#include "WebServer.h"
#include <vector>           // for storing multiple servers
#include <thread>
#include "LoadBalancer.h"


int main() {
    int num_servers;
    int run_time;

    std::cout << "Enter the number of servers: ";
    std::cin >> num_servers;

    std::cout << "Enter the run time in seconds: ";
    std::cin >> run_time;

    LoadBalancer lb(num_servers);
    lb.start(run_time);

    std::cout << "Load balancer simulation completed.\n";

    return 0;
}