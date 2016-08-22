/*************************************************************************\
*                  Copyright (C) Todd Leonhardt, 2016.                    *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* ip4_stream.h
   Adapted from The Linux Programming Interface (TLPI) Listing 59-6: is_seqnum.h

   Header file for ip4_stream_server.c and ip4_stream_client.c.
*/
#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include "read_line.h"  // read_line()
#include "tlpi_hdr.h"

// Port number for server
#define PORT_NUM "50000"

// Size of string able to hold largest integer (including terminating '\n')
#define INT_LEN 30
