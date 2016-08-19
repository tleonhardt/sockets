/*************************************************************************\
*                  Copyright (C) Todd Leonhardt, 2016.                    *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Adapted from The Linux Programming Interface (TLPI) Listing 57-2: us_xfr.h

   Header file for unix_stream_sever.c and unix_stream_client.c.
   This is a header for a simple client-server application that uses stream
   sockets in the UNIX domain.

   These programs employ a socket in /tmp. This makes it easy to compile
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
#include "tlpi_hdr.h"   // errExit(), fatal(), errMsg()

#define SV_SOCK_PATH "/tmp/us_xfr"

#define BUF_SIZE 100
