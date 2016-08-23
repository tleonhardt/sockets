/*************************************************************************\
*                  Copyright (C) Todd Leonhardt, 2016.                    *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* ip4_stream_client.c
   Adapted from The Linux Programming Interface (TLPI) Listing 59-7: is_seqnum_cl.c

   A simple Internet stream socket client. This client requests a sequence
   number from the server.

   This client accepts two arguments.  The first argument, which is the name of the host on which
   the server is running, is mandatory.  The optional second argument is the length of the sequence
   desired by the client.   The default length is 1.

   See also ip4_stream_server.c.
*/
#include <netdb.h>
#include "ip4_stream.h"

int main(int argc, char *argv[])
{
    char *reqLenStr;            // Requested length of sequence
    char seqNumStr[INT_LEN];    // Start of granted sequence
    int cfd;
    ssize_t numRead;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    if ( (argc < 2) || (0 == strcmp(argv[1], "--help")) )
    {
        usageErr("%s server-host [sequence-len]\n", argv[0]);
    }

    // Call getaddrinfo() to obtain a list of socket addresses structures suitable for connecting
    // to a TCP server bound to the specified host using the port number, PORT_NUM.
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC;    // Allows IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if (0 != getaddrinfo(argv[1], PORT_NUM, &hints, &result))
    {
        errExit("getaddrinfo");
    }

    // Walk through returned list until we find an address structure
    //   that can be used to successfully connect a socket
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        // Create a socket
        cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (-1 == cfd)
        {
        	// On error, try next address
            continue;
        }

        // Attempt to connect the socket to this server address.  Since the client has not bound
        // it's socket, the connect() call causes the kernel to assign an ephemeral port to it.
        if (-1 != connect(cfd, rp->ai_addr, rp->ai_addrlen))
        {
            // Success
            break;
        }

        // Connect failed: close this socket and try next address
        close(cfd);
    }

    if (NULL == rp)
    {
        fatal("Could not connect socket to any address");
    }

    // getaddrinfo() dynamically allocates memory for result, this frees it all
    freeaddrinfo(result);

    // Send requested sequence length, with terminating newline
    reqLenStr = (argc > 2) ? argv[2] : "1";
    if (write(cfd, reqLenStr, strlen(reqLenStr)) !=  strlen(reqLenStr))
    {
        fatal("Partial/failed write (reqLenStr)");
    }
    if (1 != write(cfd, "\n", 1))
    {
        fatal("Partial/failed write (newline)");
    }

    // Read and display sequence number returned by server
    numRead = readLine(cfd, seqNumStr, INT_LEN);
    if (-1 == numRead)
    {
        errExit("readLine");
    }
    if (0 == numRead)
    {
        fatal("Unexpected EOF from server");
    }

    // Includes '\n'
    printf("Sequence number: %s", seqNumStr);

    // Closes 'cfd'
    exit(EXIT_SUCCESS);
}
