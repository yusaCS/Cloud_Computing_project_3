#include "Request.h"
#include <iostream>

Request::Request() {
    ip_in = "0.0.0.0";
    ip_out = "0.0.0.0";
    time_i = 0;
    job_type = 'P';
}

Request::Request(std::string ipIn, std::string ipOut, int time, char jobType) {
    ip_in = ipIn;
    ip_out = ipOut;
    time_i = time;
    job_type = jobType;
}

void Request::display() const {
    std::cout << "Request: " << ip_in << " -> " << ip_out << " | Time: " << time_i << " cycles | Job Type: " << job_type << '\n';
}