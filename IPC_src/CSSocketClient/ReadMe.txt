========================================================================
    CONSOLE APPLICATION : CSSocketClient Project Overview
========================================================================

/////////////////////////////////////////////////////////////////////////////
Use:

Sockets are an application programming interface (API) in an operating system,
used for in inter-process communication. Sockets constitute a mechanism for 
delivering incoming data packets to the appropriate application process or 
thread, based on a combination of local and remote IP addresses and port 
numbers. Each socket is mapped by the operational system to a communicating
application process or thread.


.NET supplies a Socket class which implements the Berkeley sockets interface.
It provides a rich set of methods and properties for network communications. 
The Socket class allows you to perform both synchronous and asynchronous data
transfer using any of the communication protocols listed in the ProtocolType
enumeration. It supplies the following types of socket:

Stream: Supports reliable, two-way, connection-based byte streams without 
the duplication of data and without preservation of boundaries.

Dgram:Supports datagrams, which are connectionless, unreliable messages of
a fixed (typically small) maximum length. 

Raw: Supports access to the underlying transport protocol.Using the 
SocketTypeRaw, you can communicate using protocols like Internet Control 
Message Protocol (Icmp) and Internet Group Management Protocol (Igmp). 

Rdm: Supports connectionless, message-oriented, reliably delivered messages, 
and preserves message boundaries in data. 

Seqpacket:Provides connection-oriented and reliable two-way transfer of 
ordered byte streams across a network.

Unknown:Specifies an unknown Socket type.


/////////////////////////////////////////////////////////////////////////////
Project Relation:

CSSocketClient <--> CSSocketServer
CSSocketServer is the server process to communicate the client process
via the Socket.


/////////////////////////////////////////////////////////////////////////////
Code Logic:

1. Create a socket to connect the TCP Socket endpoint.

2. After get the connection, send the data to the server process.

3. Receive the response of the server process.

4. Shut down the TCP connection and release the Socket resource.


/////////////////////////////////////////////////////////////////////////////
References:

Chapter4: Using Sockets of Professional .NET Network Prgromming
http://www.amazon.com/Professional-Network-Programming-Srinivasa-Sivakumar/dp/1861007353

Socket Class
http://msdn.microsoft.com/en-us/library/system.net.sockets.socket.aspx

Asynchronous Server Socket Example
http://msdn.microsoft.com/en-us/library/fx6588te.aspx


/////////////////////////////////////////////////////////////////////////////