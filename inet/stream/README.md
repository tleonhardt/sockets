C code for Internet domain stream sockets
=========================================
This directory contains pure C code for working with Internet domain IPv4 stream sockets
on a POSIX operating system.

Internet domain stream sockets are implemented on top of TCP.  They provide a
reliable, bidirectional, byte-stream communication channel.

Here we have a simple client-server application using TCP sockets.
* **ip4_stream_server** is the server
* **ip4_stream_client** is the client

The task performed by the application is simply allocating unique sequence
numbers (or ranges of sequence numbers) to clients.

In order to handle the possibility that integers may be represented in different
formats on the server and client hosts, all transmitted integers are encoded as
strings terminated by a newline.


Internet stream server
----------------------
The server initializes the sequence number either to 0 or to the value supplied
in the optional command-line argument.  It then runs in an infinite loop waiting
for clients to connect iteratively.

When a client connects, it reads how many sequence numbers the client is requesting
from the socket, sends the current value of the sequence number back to the client,
and then increments the sequence number by the amount the client requested.

You need to kill it manually by sending it a signal.


Internet stream client
----------------------
The client accepts two arguments:
1. server hostname (mandatory)
1. length of sequence desired (optional), default is 1

The client connects to the server, sends the desired length of sequence, reads
the starting sequence number back from the server and prits it on **stdout**.


Using the code
--------------
If the code has been build using **cmake** in directory *build*, the code can be
tested in the following manner:

    cd build
    ./ip4_stream_server &
    ./ip4_stream_client localhost
    kill %1

In the above case, the client requests 1 sequence number and the server returns
sequence number 0.
