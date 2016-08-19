C code for UNIX domain sockets
==============================
This directory contains pure C code for working with UNIX domain stream sockets
on aPOSIX operating system.

**unix_stream_server** is the server
**unix_stream_client** is the client


UNIX stream server
------------------
The server runs in an infinite loop waiting for clients to connect.

When a client connects, it reads all of the data from the connected socket and
writes it to *stdout*.

You need to kill it manually by sending it a signal.


UNIX stream client
------------------
The client reads everything from *stdin* and sends it to the server.  Once it hits
*EOF* (end of file), it terminates.


Using the code
--------------
If the code has been build using **cmake** in directory *build*, the code can be
tested in the following manner:

    cd build
    ./unix_stream_server > server_out.txt &
    ls -lF /tmp/us*
    ./unix_stream_client < "hello world"
    kill %1
    cat server.out.txt

The *cat* command should echo "hello world" because that is the message that was
transmitted from the client to the server.
