C code for Internet domain (IPv6) datagram sockets
===================================================
This directory contains pure C code for working with Internet domain (IPv6) datagram
sockets on a POSIX operating system.

Internet domain datagram sockets are implemented on top of UDP.  UDP sockets are
similar to their UNIX domain counterparts, but note the following differences

* UNIX domain datagram sockets are reliable, but UDP sockets are not - datagrams
may be lost, duplicated, or arrive in a different order from that in which they were sent
* Sending on a UNIX domain datagram socket will block if the queue of data for
the receiving socket is full.  By contrast, with UDP, if the incoming datagram
would overflow the receiver's queue, then the datagram is silently dropped.

**ip6_datagram_server** is the server
**ip6_datagram_client** is the client

This example uses IPv6 IP addresses.


Internet IPv6 datagram server
-----------------------------
The server runs in an infinite loop waiting for clients to connect.

When a client connects, it reads incoming data on the bound socket,
converts it to uppercase, and sends it back to the client.

You need to kill it manually by sending it a signal.


Internet IPV6 datagram client
-----------------------------
The client loops, sending each of its command-line arguments after the first as a separate
message to the server.  After sending each message, the client reads the server
response and displays it on standard output.  The first command-line argument is
used as the IPv6 address for the server (use ::1 for localhost).


Using the code
--------------
If the code has been build using **cmake** in directory *build*, the code can be
tested in the following manner:

    cd build
    ./unix_datagram_server &
    ./unix_datagram_client ::1 hello world
    kill %1f

The client should display two responses from the server:
Response 1:  HELLO
Response 2:  WORLD
