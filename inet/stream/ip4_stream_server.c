/*************************************************************************\
*                  Copyright (C) Todd Leonhardt, 2016.                    *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* ip4_stream_server.c
   Adapted from The Linux Programming Interface (TLPI) Listing 59-6: is_seqnum_sv.c

   A simple Internet stream socket server. Our service is to provide
   unique sequence numbers to clients.

   Usage:  is_seqnum_sv [init-seq-num]
                        (default = 0)

   See also ip4_stream_client.c.
*/
#define _BSD_SOURCE // To get definitions of NI_MAXHOST and NI_MAXSERV from <netdb.h>
#include <netdb.h>
#include "ip4_stream.h"

#define BACKLOG 50

int main(int argc, char *argv[])
{
    uint32_t seqNum;
    char reqLenStr[INT_LEN];    // Length of requested sequence
    char seqNumStr[INT_LEN];    // Start of granted sequence
    struct sockaddr_storage claddr;
    int lfd, cfd, optval, reqLen;
    socklen_t addrlen;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
    char addrStr[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if ( (argc > 1) && (0 == strcmp(argv[1], "--help")) )
    {
        usageErr("%s [init-seq-num]\n", argv[0]);
    }

    // Initialize the server's sequence number either to 0 or to the value
    // supplied in the optional command-line argument
    seqNum = (argc > 1) ? getInt(argv[1], 0, "init-seq-num") : 0;

    // Ignore the SIGPIPE signal, so that we find out about broken connection
    //   errors via a failure from write() instead of by crashing.
    if (SIG_ERR == signal(SIGPIPE, SIG_IGN))
    {
        errExit("signal");
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;    // Limit to TCP sockets
    hints.ai_family = AF_UNSPEC;        // Allows IPv4 or IPv6
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV; // Wildcard IP address; service name is numeric

    // Call getaddrinfo() to obtain a list of socket addresses for a TCP socket
    // that uses port number PORT_NUM.
    if (0 != getaddrinfo(NULL, PORT_NUM, &hints, &result))
    {
        errExit("getaddrinfo");
    }

    // Walk through returned list until we find an address structure
    //   that can be used to successfully create and bind a socket
    optval = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        // Create a socket based on this address structure
        lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (-1 == lfd)
        {
            // On error, try next address
            continue;
        }

        // Set the SO_REUSEADDR option for the socket we just created.
        // A TCP server should usually set this options on it's listening socket.
        if (-1 == setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
        {
            errExit("setsockopt");
        }

        // The loop terminates when an address structure that can bind is found
        if (0 == bind(lfd, rp->ai_addr, rp->ai_addrlen))
        {
            // Success
            break;
        }

        // bind() failed: close this socket and try next address
        close(lfd);
    }

    if (NULL == rp)
    {
        fatal("Could not bind socket to any address");
    }

    // Mark the socket as a listening socket
    if (-1 == listen(lfd, BACKLOG))
    {
        errExit("listen");
    }

    // getaddrinfo() dynamically allocates memory for result, this frees it all
    freeaddrinfo(result);

    // Handle clients iteratively in an infinite for loop.
    // Each client's request is serviced before the next client's request is accepted.
    for (;;)
    {
        // Accept a client connection, obtaining client's address
        addrlen = sizeof(struct sockaddr_storage);
        cfd = accept(lfd, (struct sockaddr *) &claddr, &addrlen);
        if (-1 == cfd)
        {
            errMsg("accept");
            continue;
        }

        // Given a sockaddr structure, get strings containing the corresponding host and service name
        if (0 == getnameinfo((struct sockaddr *) &claddr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0))
        {
            // Display the client's IP address plus port on stdout
            snprintf(addrStr, ADDRSTRLEN, "(%s, %s)", host, service);
        }
        else
        {
            snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
        }

        printf("Connection from %s\n", addrStr);

        // Read client request, which consists of a newline-terminated strings
        // specifying how many sequence numbers the client wants
        if (readLine(cfd, reqLenStr, INT_LEN) <= 0)
        {
            // Failed read; skip request
            close(cfd);
            continue;
        }

        // Convert the string to an integer
        reqLen = atoi(reqLenStr);

        // Watch for misbehaving clients
        if (reqLen <= 0)
        {
            // Bad request; skip it
            close(cfd);
            continue;
        }

        // Send the current value of the sequence number back to the client,
        // encoding it as a newline-terminated string
        snprintf(seqNumStr, INT_LEN, "%d\n", seqNum);
        if (write(cfd, seqNumStr, strlen(seqNumStr)) != strlen(seqNumStr))
        {
            fprintf(stderr, "Error on write");
        }

        // Update sequence number
        seqNum += reqLen;

        // Close connection
        if (-1 == close(cfd))
        {
            errMsg("close");
        }
    }
}
