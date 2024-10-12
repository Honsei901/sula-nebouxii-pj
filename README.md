# Learning Through Socket Communication in HTTP

This project offered me a chance to revisit socket communication, something we often overlook when dealing with HTTP communication. The project uses TCP sockets to exchange data.

## Overview of TCP Sockets

- Sockets are our basic building blocks of network communication.

- Most common networking tasks, such as web browsing, rely on the TCP protocol for communication.

- Sockets allow us to establish connection streams for data exchange across networks.

- HTTP operates on top of this TCP workflow and sends additional data specified by the protocol.

## What This Project Demonstrates

### tcp_client.cpp

Implements the client-side of TCP communication.

### tcp_server.cpp

Implements the server-side of TCP communication.

### http_server.cpp

Implements the server-side of HTTP communication.
You can see how HTTP communication relies on TCP sockets by reviewing the code. It illustrates how HTTP is transmitted over TCP. To demonstrate this, run `http_server.exe` in the dist directory and visit `http://localhost:5050`.
