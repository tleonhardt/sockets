C code for Internet domain sockets
==================================
This directory contains pure C code for working with Internet domain IPv4 and IPv6
sockets on a POSIX operating system.  There is code for both stream and datagram sockets.  
For each configuration there is code for both a client and a server.

Sockets exist in a *communication domain*, which determines:

* the method of identifying a socket (i.e. the format of a socket address)
* the range of communication (i.e. between applications on the same host or between applications on different hosts connected to a network)

Internet domain sockets allow communication between applications on different hosts
located on the same network.  Internet domain address are a combination of
an IP address and a 16-bit port number.  For IPv4 sockets the IP address is a
32-bit unsigned number, while for IPv6 sockets, the IP address ia a 128-bit number.

Stream vs Datagram sockets
--------------------------
Every sockets implementation provides at least two types of sockets: stream and
datagram.  Both of these provide a bidirectional communication channel.

Stream sockets support reliable delivery in a connection-oriented protocol which
does not preserve message boundaries, but instead has a byte-stream.  In the Internet
domain, stream sockets are provided by TCP/IP.

Datagram sockets preserve message boundaries using an unreliable and connectionless
protocol.  In the Internet domain, datagram sockets are provided by UDP/IP.


Network Byte Order
------------------
Since port numbers and IP addresses must be transmitted between, and understood by,
all hosts on a network (including both big endian and little endian hosts), a
standard ordering must be used for multibyte integers.  This ordering is called
*network byte order*, and happens to be big endian.

Since Intel processes and the vast majority of all ARM processors are little endian,
developers need to be careful to convert values to network byte order before
storing them in socket address structures.
