C code for UNIX domain datagram sockets
=======================================
This directory contains pure C code for working with UNIX domain datagram sockets
on a POSIX operating system.

**unix_datagram_server** is the server
**unix_datagram_client** is the client

In general, datagram sockets are unreliable.  However for UNIX domain datagram
sockets, the datagram transmission is carried out within the kernel and is reliable.
Moreover, all messages are delivered in order and are unduplicated.

On Linux, we can send quite large datagrams, but portable POSIX implementations should
impose a maximum datagram size, such as 2048 bytes.


UNIX datagram server
------------------
The server runs in an infinite loop waiting for clients to connect.

When a client connects, it reads all of the data from the connected socket and
writes it to *stdout*.

You need to kill it manually by sending it a signal.


UNIX datagram client
------------------
The client reads everything from *stdin* and sends it to the server.  Once it hits
*EOF* (end of file), it terminates.


Using the code
--------------
If the code has been build using **cmake** in directory *build*, the code can be
tested in the following manner:

    cd build
    ./unix_datagram_server > server_out.txt &
    ls -lF /tmp/us*
    echo "hello world" | ./unix_datagram_client
    kill %1
    cat server.out.txt

The *cat* command should echo "hello world" because that is the message that was
transmitted from the client to the server.
