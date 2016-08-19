/*************************************************************************\
*                  Copyright (C) Todd Leonhardt, 2016.                    *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* unix_datagram_socket.h
   Adapted from The Linux Programming Interface (TLPI) Listing 57-5: ud_ucase.h

   Header file for ud_unix_datagram_server.c and unix_datagram_client.c.

   These programs employ sockets in /tmp. This makes it easy to compile
   and run the programs. However, for a security reasons, a real-world
   application should never create sensitive files in /tmp. (As a simple of
   example of the kind of security problems that can result, a malicious
   user could create a file using the name defined in SV_SOCK_PATH, and
   thereby cause a denial of service attack against this application.
   See Section 38.7 of "The Linux Programming Interface" for more details
   on this subject.)
*/
#include <sys/un.h>     // sockaddr_un struct
#include <sys/socket.h> // socklen_t, saf_family_t, sockaddr struct, etc.
#include <ctype.h>
#include "tlpi_hdr.h"   // errExit(), fatal(), errMsg()

// Maximum size of messages exchanged between client and server
#define BUF_SIZE 10

#define SV_SOCK_PATH "/tmp/ud_ucase"
