C code for UNIX domain sockets
==============================
This directory contains pure C code for working with UNIX domain sockets on a
POSIX operating system.  There is code for both stream and datagram sockets.  
For each configuration there is code for both a client and
a server.

Sockets exist in a *communication domain*, which determines:

* the method of identifying a socket (i.e. the format of a socket address)
* the range of communication (i.e. between applications on the same host or between applications on different hosts connected to a network)

UNIX (AF_UNIX) domain sockets allow communication between applications on the
same host.  Communication is performed within the OS kernel and the address is
a pathname.

Stream vs Datagram sockets
--------------------------
Every sockets implementation provides at least two types of sockets: stream and
datagram.  Both of these provide a bidirectional communication channel.

Stream sockets support reliable delivery in a connection-oriented protocol which
does not preserve message boundaries, but instead has a byte-stream.

Datagram sockets preserve message boundaries using an unreliable and connectionless
protocol.
