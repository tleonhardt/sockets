/*************************************************************************\
*                  Copyright (C) Todd Leonhardt, 2016.                    *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Adapted from The Linux Programming Interface (TLPI) Listing 57-3: us_xfr_sv.c

   An example UNIX stream socket server. Accepts incoming connections
   and copies data sent from clients to stdout.

   This server is an example of an iterative server - a server that handles one
   client at a time before proceeding to the next client.

   The server has an infinite loop waiting to acccept new clients.  It must be
   terminated manually by sending it a signal.

   NOTE: After the server terminates, the socket pathname continues to exist.
   This is why the server uses remove() to remove any existing instance of the
   socket pathname before calling bind().

   See also unix_stream_client.c.
*/
#include "unix_stream_socket.h"

#define BACKLOG 5

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    // Create a socket
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sfd)
    {
        errExit("socket");
    }

    // Construct server socket address, bind socket to it, and make this a listening socket

    // For an explanation of the following check, see the errata notes for
    //    pages 1168 and 1172 at http://www.man7.org/tlpi/errata/
    if (strlen(SV_SOCK_PATH) > sizeof(addr.sun_path) - 1)
    {
        fatal("Server socket path too long: %s", SV_SOCK_PATH);
    }

    // Remove any existing file with the same pathname that we want to bind to
    if ( (-1 == remove(SV_SOCK_PATH)) && (ENOENT != errno) )
    {
        errExit("remove-'%s'.  Check file permissions.", SV_SOCK_PATH);
    }

    // Construct an address structure for the server's socket
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    // Bind the socket to that address
    if (-1 == bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)))
    {
        errExit("bind");
    }

    // Mark the socket as a listening socket
    if (-1 == listen(sfd, BACKLOG))
    {
        errExit("listen");
    }

    // Handle client connections iteratively (infinite loop)
    for (;;)
    {
        // Accept a connection. The connection is returned on a new
        //   socket, 'cfd'; the listening socket ('sfd') remains open
        //   and can be used to accept further connections.
        cfd = accept(sfd, NULL, NULL);
        if (-1 == cfd)
        {
            errExit("accept");
        }

        // Transfer data from connected socket to stdout until EOF
        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0)
        {
            if (write(STDOUT_FILENO, buf, numRead) != numRead)
            {
                fatal("partial/failed write");
            }
        }

        // There was an error, so close the connected socket and exit program
        if (-1 == numRead)
        {
            close(cfd);
            errExit("read");
        }

        // Close the connected socket 'cfd', then move on to wait for new client
        if (-1 == close(cfd))
        {
            errMsg("close");
        }
    }
}
