/*
 * liblsockets.h
 * This file is part of liblsockets
 *
 * Copyright (C) 2012 - Maximilien Rigaut
 *
 * liblsockets is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * liblsockets is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liblsockets. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __LIBLSOCKETS_H__
#define __LIBLSOCKETS_H__

/** @file liblsockets.h Socket Library (header)
 *
 * Little socket library, handle the basic task of creating, opening, binding, sending and receiving sockets 
 */

/* ========= Includes =========*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>

#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include <stropts.h>
#include <poll.h>

#include "utils.h"
#include "lclists.h"

/* ========= Defines ==========*/

#define SIZE_BUFFER 1024			/**< Size of the socket carried message */
#define SIZE_ADDR 108				/**< Size of the socket address */
#define SIZE_PENDING 10			/**< Standard size of pending connections for listening socket */

/* ======== Structures ========*/


/* ========= Typedef ==========*/

/** The type of message sent through the socket
 *
 * Keep in mind that, as an enum, this will be integers during runtime.
 * Beside that, don't be afraid if you see strange int in your packets.
 * 
 * Explanations of tags:
 * \li The tag [core] means it's only meant for the core program (you're the boss though)
 * \li The tag [program] means it's meant to be used by the program to cmmunicate.
 * \li The tag [msg] means the packet is carring a text-based message
 * \li The tag [nfo] means the packet is carring an information
 * \li The tag [wrn] means the packet is reporting a warning
 * \li The tag [err] means the packet is reporting an error
 */
typedef enum msg_enum{
	msg_sync,	/**< [nfo]:[program] Program seeking first connection*/
	msg_recv,	/**< [nfo]:[program] Message received */
	msg_keep,	/**< [nfo]:[core] [TBI] Keep-alive */
	msg_kill,	/**< [nfo]:[program] The connection is to be shutdown, process now */
	msg_text,	/**< [msg]:[program] String sent */
	msg_cont,	/**< [msg]:[program] Message is not finished ! */
	msg_fnsh,	/**< [msg]:[program] Message is finished */
	msg_warnings, 	/**< Not a message: Upper this value are warnings */
	msg_wait,	/**< [wrn]:[core] [NYI] Server overloaded, wait */
	msg_errors, 	/**< Not a message: Upper this value are errors */
	msg_err,	/**< [err]:[core] [NYI] (Unknown) Error in received socket */
	msg_die,	/**< [err]:[core] [NYI] Server died, will not process requests anymore */
	msg_wtf		/**< [err]:[program] Request isn't the the one expected, I don't know what to do ! Abort ! Abort !*/
} msg_type;

/**  Small wraping socket */
typedef struct sk_addr{
	char* addr;					/**< Path to the socket */
	int file;					/**< The file descriptor to use */
	int type;					/**< The type of connection (AF_UNIX)*/
	int mode;					/**< The mode of connection (SOCK_DGRAM) */
	struct sockaddr* socket;	/**< The linked socket */
	struct sk_addr* sendto;	/**< The socket to send to */
} lsocket;

/** A raw packet, for abstraction purpose
 * Note that a message MUST be newline free, as it is used to end packets 
 * @see lpacket_forge() for more detailed information about socket creation.
 */
typedef struct pk_struct {
	msg_type type;		/**< The request being done, important for having normalised communications */
	char*message;		/**< The body of the message, can be empty */
	unsigned int bytes;/**< The number of bytes processed by the last request */
} lpacket;

/** A basement made of sockets
 * The name podrum is croatian for basement.
 */
typedef struct sk_podr {
	int 		cur_size;		/**< Current size of the basement */ 
	int			max_size;		/**< Maximum size of the basement */ 
	int			type;			/**< 0 Reading fd, 1 Writing fd */
	lsocket**	sockets;		/**< Basement itself */
	struct pollfd*fd_list;		/**< List of poll file_descriptors */
	lclist*		del_list;		/**< List of elements to delete */
} lpodrum;

/* ======== Prototype =========*/

/* Low level communication */
lsocket* make_lsocket	(char*);								/**< [Public]: Create new socket */
lsocket* make_from_socket(struct sockaddr*,int,int);			/**< [Privte] */
void 	 open_lsocket	(lsocket*,int,int);					/**< [Public]: Open the socket for communications */
void 	 connect_lsocket(lsocket*,lsocket*);					/**< [Public]: Connect the socket to another */
void	 bind_lsocket	(lsocket*);								/**< [Public]: Bind the socket for answers */
void 	 close_lsocket	(lsocket*,int);							/**< [Public]: Terminate the connection */
void	 free_lsocket(lsocket*);								/**< [Public]: Free unused memory */

int 	 lsocket_send	(lsocket*,char*,int);					/**< [Privte] */
lsocket* lsocket_receive(lsocket*,char*,int);					/**< [Privte] */

/* Packet utilities */
lpacket* lpacket_receive	(lsocket*);							/**< [Public]: Receive packet */
void 	 lpacket_drop		(lpacket*);							/**< [Public]: Drop a packet */

/* All the big wrapping functions user-kinda-friendly */
int 	 message_send			(lsocket*,msg_type,char*);/**< [Public]: Send a message through the socket */
int 	 message_send_to		(lsocket*,msg_type,char*,lsocket*);	/**< [Public]: Send a message through the socket */
lpacket* message_receive		(lsocket*,lsocket**);	/**< [Public]: Return the received socket, save the sending socket */

void	 listen_lsocket	(lsocket*,int);					/**< [Public]: Listen to new incomming transmitions */
lsocket* accept_lsocket	(lsocket*);						/**< [Public]: Accept a new socket */

lpodrum* make_lpodrum			(int);					/**< [Public]: Create new listening basement (podrum in croatian) */
void	 add_lsocket			(lpodrum*,lsocket*,int);/**< [Public]: Add a num socket to the basement */
lsocket* get_lsocket			(lpodrum*,int);			/**< [Public]: Get the socket number */
int		 del_lsocket			(lpodrum*,int);			/**< [Public]: Delete the socket (int) from the basement */
int* 	 listen_lpodrum			(lpodrum*,int timeout);	/**< [Public]: Return the list from all the socket ready to communicate */
void	 purge_lpodrum			(lpodrum*);				/**< [Prvate]: Purge the lpodrum before usage */
void	 drop_lpodrum			(lpodrum*,int);			/**< [Public]: Delete the lpodrum and free the memory */
#endif /* __liblsockets_H__ */
