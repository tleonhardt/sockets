/*************************************************************************\
*                  Copyright (C) Todd Leonhardt, 2016.                    *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* unix_datagram_server.c
   Adapted from The Linux Programming Interface (TLPI) Listing 57-6: ud_ucase_sv.c

   A server that uses a UNIX domain datagram socket to receive datagrams,
   convert their contents to uppercase, and then return them to the senders.

   See also unix_datagram_client.c.
*/
#include "unix_datagram_socket.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];

    // Create server socket
    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == sfd)
    {
        errExit("socket");
    }

    // Construct well-known address and bind server socket to it

    // For an explanation of the following check, see the erratum note for
    //   page 1168 at http://www.man7.org/tlpi/errata/.
    if (strlen(SV_SOCK_PATH) > (sizeof(svaddr.sun_path) - 1) )
    {
        fatal("Server socket path too long: %s", SV_SOCK_PATH);
    }

    // Remove any existing file with the same pathname that we want to bind to
    if ( (-1 == remove(SV_SOCK_PATH)) && (ENOENT != errno) )
    {
        errExit("remove-%s", SV_SOCK_PATH);
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    // Bind the socket to a well-known address
    if ( -1 == bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) )
    {
        errExit("bind");
    }

    // Receive messages, convert to uppercase, and return to client
    for (;;)
    {
        len = sizeof(struct sockaddr_un);
        numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);
        if (-1 == numBytes)
        {
            errExit("recvfrom");
        }

        printf("Server received %ld bytes from %s\n", (long) numBytes, claddr.sun_path);

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
