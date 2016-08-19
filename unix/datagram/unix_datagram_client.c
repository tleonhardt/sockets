/*************************************************************************\
*                  Copyright (C) Todd Leonhardt, 2016.                    *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* unix_datagram_client.c
   Adapted from The Linux Programming Interface (TLPI) Listing 57-7: ud_ucase_cl.c

   A UNIX domain client that communicates with the server in unix_datagram_server.c.
   This client sends each command-line argument as a datagram to the server,
   and then displays the contents of the server's response datagram.
*/
#include "unix_datagram_socket.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    ssize_t msgLen;
    ssize_t numBytes;
    char resp[BUF_SIZE];

    if ( (argc < 2) || (0 == strcmp(argv[1], "--help")))
    {
        usageErr("%s msg...\n", argv[0]);
    }

    // Create client socket; bind to unique pathname (based on PID)
    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == sfd)
    {
        errExit("socket");
    }

    memset(&claddr, 0, sizeof(struct sockaddr_un));
    claddr.sun_family = AF_UNIX;
    snprintf(claddr.sun_path, sizeof(claddr.sun_path),
            "/tmp/ud_ucase_cl.%ld", (long) getpid());

    if (-1 == bind(sfd, (struct sockaddr *) &claddr, sizeof(struct sockaddr_un)))
    {
        errExit("bind");
    }

    // Construct address of server
    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    // Send messages to server; echo responses on stdout
    for (j = 1; j < argc; j++)
    {
        // May be longer than BUF_SIZE
        msgLen = strlen(argv[j]);
        if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) != msgLen)
        {
            fatal("sendto");
        }

        numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
        // Or equivalently: numBytes = recv(sfd, resp, BUF_SIZE, 0);
        //              or: numBytes = read(sfd, resp, BUF_SIZE);
        if (-1 == numBytes)
        {
            errExit("recvfrom");
        }

        printf("Response %d: %.*s\n", j, (int) numBytes, resp);
    }

    // Remove client socket pathname
    remove(claddr.sun_path);
    exit(EXIT_SUCCESS);
}
