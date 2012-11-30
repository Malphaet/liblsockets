A little socket library  {#mainpage}
====================

## Installation

> + `make init`
> + `make`
> + `make doc`
> + `make test`

## Introduction

This library is meant to be as simple a possible and as minimal as possible.
The main goal was to provide a litte degree of abstraction, 
hence allowing to handle @ref lpacket instead of raw text, @ref lsocket instead of structures.

Some other tools like the @ref lpodrum are meant to provide an automated version of the poll() function.
Providing a complete tool to handle all the incoming connections.

## Modules

The section [Wrappers] is containing all the high level tools for handling communication.

The section [lSockets] contains low-level tools for handling communications. Most commands are for advanced users only.

The section [lPackets] contains low-level tools for forging and unpacking packets. Advanced users only.

The section [Clists] is barely part of the project, and should NOT be used by the end-user.

@see [Modules] for a more complex doxumentation.

## Usage

The following functions are needed to manage the creation/destruction of sockets.
+ @ref make_lsocket()	Create a named socket.
+ @ref open_lsocket()	Open the socket for communications.
+ @ref bind_lsocket()	Bind the socket for answers
+ @ref close_lsocket()	Close the socket and free all memory.

The two folliwing functions handle the task of receiving and sending packets
+ @ref message_send()	 Send a message through the given socket, you can specify the sending socket (for UDP purpose).
+ @ref message_receive() Return the packet received through the socket. You can give a socket for it to store the socket of the sender.

The following function allow you to handle the task of waiting for incomming/outgoing connections:
+ @ref make_lpodrum 	Create new listening basement (podrum in croatian)
+ @ref add_lsocket 		Add a socket to the basement
+ @ref get_lsocket 		Get a socket from the basement
+ @ref del_lsocket 		Delete a socket from the basement
+ @ref listen_lpodrum 	Return the list from all the socket ready to communicate

## Todo

### Important
+ @ref todo
+ Aim at the 0 leak
+ Do the test on all other combination TCP unix, UDP inet.
+ Verbosity is not such a great idea

### Cosmetic
+ make functions thread safe

### Test:
+ listen_lsocket: Done
+ close_lsocket: Done
+ lsocket_send: Done
+ lsocket_receive: Done
+ lsocket_message_send: Done
+ lsocket_message_receive: Done

### Done
+ SOCK_INET implementation
+ make_from_lsocket corrections
+ listen_lsocket() listen for new sockets

[Modules]:  ./modules.html 			"List of all modules"
[Wrappers]: ./group___wrappers.html 	"Comunication managers"
[Clists]: 	 ./group___clists.html 		"Chained Lists"
[lSockets]:	 ./group__liblsockets.html	"Little Socket Library"
[lPackets]:  ./group___packetlib.html	"Little Packet Library"
