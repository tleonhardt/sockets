/*************************************************************************\
*                  Copyright (C) Todd Leonhardt, 2016.                    *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* ip6_datagram_server.c
   Adapted from The Linux Programming Interface (TLPI) Listing 59-3: i6d_ucase_sv.c

   A server that receives datagrams, converts their contents to uppercase, and
   then returns them to the senders.

   See also ip6_datagram_client.c.
*/
#include "ip6_datagram.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in6 svaddr, claddr;
    int sfd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];
    char claddrStr[INET6_ADDRSTRLEN];

    // Create a datagram socket bound to an address in the IPv6 domain
    sfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (-1 == sfd)
    {
        errExit("socket");
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_in6));
    svaddr.sin6_family = AF_INET6;
    svaddr.sin6_addr = in6addr_any;     // Wildcard address
    svaddr.sin6_port = htons(PORT_NUM);

    if (-1 == bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in6)))
    {
        errExit("bind");
    }

    // Receive messages, convert to uppercase, and return to client
    for (;;)
    {
        len = sizeof(struct sockaddr_in6);
        numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);
        if (-1 == numBytes)
        {
            errExit("recvfrom");
        }

        // Display address of client that sent the message.
        // Use inet_ntop to convert from netwrok to presentation format.
        if (NULL == inet_ntop(AF_INET6, &claddr.sin6_addr, claddrStr, INET6_ADDRSTRLEN))
        {
            printf("Couldn't convert client address to string\n");
        }
        else
        {
            printf("Server received %ld bytes from (%s, %u)\n", (long) numBytes, claddrStr, ntohs(claddr.sin6_port));
        }

        for (j = 0; j < numBytes; j++)
        {
            buf[j] = toupper((unsigned char) buf[j]);
        }

        if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) != numBytes)
        {
            fatal("sendto");
        }
    }
}
