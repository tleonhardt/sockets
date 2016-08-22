C code for Internet domain (IPv4) stream sockets
================================================
This directory contains pure C code for working with Internet domain stream sockets
on a POSIX operating system.

Internet domain stream sockets are implemented on top of TCP.  They provide a
reliable, bidirectional, byte-stream communication channel.

**inet_stream_server** is the server
**inet_stream_client** is the client


Internet stream server
----------------------
The server runs in an infinite loop waiting for clients to connect.

When a client connects, it reads all of the data from the connected socket and
writes it to *stdout*.

You need to kill it manually by sending it a signal.


Internet stream client
----------------------
The client reads everything from *stdin* and sends it to the server.  Once it hits
*EOF* (end of file), it terminates.


Using the code
--------------
If the code has been build using **cmake** in directory *build*, the code can be
tested in the following manner:

    cd build
    ./inet_stream_server > server_out.txt &
    ls -lF /tmp/us*
    echo "hello world" | ./inet_stream_client
    kill %1
    cat server.out.txt

The *cat* command should echo "hello world" because that is the message that was
transmitted from the client to the server.
