/*************************************************************************\
*                  Copyright (C) Todd Leonhardt, 2016.                    *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* ip6_datagram.h
   Adapted from The Linux Programming Interface (TLPI) Listing 59-2: i6d_ucase.h

   Header file for ip6_datagram_server.c and i6d_datagram_client.c.
*/
#include <netinet/in.h> // sockaddr_in struct
#include <arpa/inet.h>  // in_port_t, in_addr_t, htonl(), htons(), ntohl(), ntohs()
#include <sys/socket.h> // socklen_t, saf_family_t, sockaddr struct, etc.
#include <ctype.h>      // toupper()
#include "tlpi_hdr.h"   // errExit(), fatal(), errMsg()

// Maximum size of messages exchanged between client and server
#define BUF_SIZE 10

// Server port number
#define PORT_NUM 50002
