/*************************************************************************\
*                  Copyright (C) Todd Leonhardt, 2016.                    *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Adapted from The Linux Programming Interface (TLPI) Listing 57-4: us_xfr_cl.c

   An example UNIX domain stream socket client. This client transmits contents
   of stdin to a server socket.

   See also unix_stream_sever.c.
*/
#include "unix_stream_socket.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    // Create a socket
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sfd)
    {
        errExit("socket");
    }

    // Construct the address structure for the server's socket
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    // Connect to the socket at that address
    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
    {
        errExit("connect");
    }

    // Copy stdin to socket until EOF on stdin
    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    {
        if (write(sfd, buf, numRead) != numRead)
        {
            fatal("partial/failed write");
        }
    }
    
    if (-1 == numRead)
    {
        errExit("read");
    }

    // Closes our socket; server sees EOF
    exit(EXIT_SUCCESS);
}
