#ifndef REQUEST_H
#define REQUEST_H

#include <string>


/**
 * @class Request
 * @brief Represents a web request to be processed by a web server.
 *
 * The Request class contains information about the requester's IP address, 
 * the response IP address, the time required to process the request, and the type of job (processing or streaming).
 */
class Request {
public:

    /**
     * @brief IP address of the requester.
     */
    std::string ip_in;
    
    
    /**
     * @brief IP address for the server response.
     */
    std::string ip_out;
    
    
    /**
     * @brief Time taken to process the request (in clock cycles).
     */
    int time_i;
    
    
    /**
     * @brief Type of job: 'P' for processing or 'S' for streaming.
     */
    char job_type;


    /**
     * @brief Default constructor for creating an empty Request object.
     */
    Request();
    
    
    /**
     * @brief Constructs a Request object with specified values.
     * @param ipIn The IP address of the requester.
     * @param ipOut The IP address for the server response.
     * @param time The time taken to process the request (in clock cycles).
     * @param jobType The type of job: 'P' for processing or 'S' for streaming.
     */
    Request(std::string ipIn, std::string ipOut, int time, char jobType);


    /**
     * @brief Displays the details of the request.
     *
     * This function prints the IP addresses, time, and job type of the request. 
     * It can be used for debugging or logging purposes.
     */
    void display() const;
};

#endif