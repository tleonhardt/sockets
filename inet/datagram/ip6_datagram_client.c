/*************************************************************************\
*                  Copyright (C) Todd Leonhardt, 2016.                    *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* ip6_datagram_client
   Adapted from The Linux Programming Interface (TLPI) Listing 59-4: i6d_ucase_cl.c

   Client for ip6_datagram_sever.c: send each command-line argument as a datagram to
   the server, and then display the contents of the server's response datagram.

   The first command-line argument is interpreted as the IPv6 address of the server.
   All subsequent command-line arguments are sent as separate messages to the server
   and a response is read for each one.

   WARNING:  A numerical IPv6 address in presentation format needs to be given.
   No DNS lookup is done if an actual hostname is given.
*/
#include "ip6_datagram.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in6 svaddr;
    int sfd, j;
    ssize_t msgLen;
    ssize_t numBytes;
    char resp[BUF_SIZE];

    if ( (argc < 3) || (0 == strcmp(argv[1], "--help")) )
    {
        usageErr("%s host-address msg...\n", argv[0]);
    }

    // Create a datagram socket; send to an address in the IPv6 domain
    sfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (-1 == sfd)
    {
        errExit("socket");
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_in6));
    svaddr.sin6_family = AF_INET6;
    svaddr.sin6_port = htons(PORT_NUM);

    // Convert the server address to binary form using inet_pton()
    if (inet_pton(AF_INET6, argv[1], &svaddr.sin6_addr) <= 0)
    {
        fatal("inet_pton failed for address '%s'", argv[1]);
    }

    // NOTE: We do NOT bind the socket to an address.  If an Internet domain socket
    // is not bound to an address ,the kernel binds the socket to an ephemeral port.

    // Send messages to server; echo responses on stdout
    for (j = 2; j < argc; j++)
    {
        msgLen = strlen(argv[j]);
        if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr,sizeof(struct sockaddr_in6)) != msgLen)
        {
            fatal("sendto");
        }

        numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
        if (-1 == numBytes)
        {
            errExit("recvfrom");
        }

        printf("Response %d: %.*s\n", j - 1, (int) numBytes, resp);
    }

    exit(EXIT_SUCCESS);
}
