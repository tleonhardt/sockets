C socket code
=============
This repository contains pure C code for working with sockets on a POSIX operating
system.  There is code for both UNIX and Internet domain sockets using both stream
and datagram sockets.  For each configuration there is code for both a client and
a server.

In general I would prefer to write code in C++ as compared to C due primarily to
the vastly superior standard library, but by using pure C code, it can be used
with either a C or C++ baseline.

Further details are available within the subdirectory for each example in the
README.md there.


Attribution
-----------
This code is all based on code from the truly excellent book **The Linux Programming
Interface** (TLPI for short) by Michael Kerrisk.  If you haven't had the pleasure of being exposed to this book it is one of the best reference works I have ever seen.  Check it out:
[![Screenshot](screenshots/TLPI-front-cover.png)](http://man7.org/tlpi/)

I have renamed, refactored, and reformatted in various ways to arrive at code with
a coding style which I prefer.  I have also added some new comments and expanded
upon existing ones to make the code as self-documenting as possible.

But Mr. Kerrisk deserves kudos for writing very readable code with proper error handling
which is as simple as it can be while getting the job done.


Prerequisites
-------------
Building this code requires the following:

  * POSIX OS    - Linux, Mac OS X, or any of the BSD variants should all work fine (tested on Linux and Mac OS)
  * CMake       - version 2.6 or newer
  * C Compiler  - Any C compiler recognized by CMake with support for C99


Building
--------

cd into the directory for the example you wish to build and then do the following

    mkdir build
    cd build
    cmake ..
    make

Using
-----
See instructions within the README.md for the example you wish to run.


Goals
-----
* Simple working pure C code for socket programming
* Demonstrate proper error handling, setup, and teardown
* Work with any POSIX-based OS


Other Tools / Libraries to Consider
-----------------------------------

Depending on your needs, you may be a lot better off using a higher-level library
which abstracts away some of the intricacies (pains) of dealing with sockets.  In
particular, 0MQ / ZeroMQ is an excellent library which has many advantages over
direct socket programming:

* [0MQ](http://man7.org/tlpi/)
    * Connect your code in any language, on any platform.
    * Carries messages across inproc, IPC, TCP, TIPC, multicast
    * Smart patterns like pub-sub, push-pull, and router-dealer


Warnings Regarding Stream Sockets
--------------------------------
Stream sockets are wonderful in that they are reliable.  What this means is that we
are guaranteed that either the transmitted data will arrive intact at the receiving
application, exactly as it was transmitted by the sender (assuming that neither
the network link nor the receiver crashes), or that we will receive notification
of a probable failure in transmission.  

But there are a couple caveats that you need to be aware of otherwise they will bite
you very hard:

1. A stream socket may be closed by using the *close()* system call or as a consequence of
    the application terminating.  Afterward, when the peer application attempts to read
    from the other end of the connection, it receives end-of-file (once all buffered
    data has been read).  If the peer application attempts to write to its socket,
    it receives a SIGPIPE signal, and the system call fails with the error EPIPE.  Since the default behavior of a SIGPIPE is to kill the application and this signal
    is redundant due to the EPIPE error returend, the usual and recommended way
    of dealing with this is to potential fatal crash is to ignore the SIGPIPE
    signal and find out about the closed connection via the EPIPE error.
1. Stream sockets do not in general preserve message boundaries.  Practically what
    this means is your receiver code needs to deal with large messages getting fragmented
    across multiple packets and potentially with multiple smaller messages getting jammed
    together within the same packet (or a mix of the two above cases).  There are a lot of
    "edge" cases that you can screw up here.  I've seen working and tested code which has
    been deployed in the field for a decade suddenly break when the data rate doubles because
    a developer forgot to properly deal with one of these edge cases.


Warnings Regarding Datagram Sockets
-----------------------------------
Datagram sockets have the advantages of preserving message boundaries and being
connectionless.  This means you don't need to deal with message fragmentation, which
is rather swell.  However, the down side with datagrams is that they are unreliable
and you will typically need to implement an application-level protocol for dealing
with this unreliability, which can get quite complicated in it's own right.

You need to be aware of the following possibilities and
generally deal with them:

1. Messages may arrive out of order
1. Messages may be duplicated
1. Messages may not arrive at all
1. Since this is a connectionless protocol, you won't automatically get any warning or error when any of the above occurs

NOTE: UNIX domain datagram sockets are actually fully reliable, so you don't need
to worry about the points mentioned above with them.
