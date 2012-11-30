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


#include "structures.h"
#include "wrappers.h"
#include "lpackets.h"
#include "lclists.h"

/* ========= Defines ==========*/

#define SIZE_BUFFER 1024			/**< Size of the socket carried message */
#define SIZE_ADDR 108				/**< Size of the socket address */
#define SIZE_PENDING 10			/**< Standard size of pending connections for listening socket */

/* Global variables */
volatile int lpacket_rcv_bytes;	/**< Number of received bytes */
volatile int lpacket_snd_bytes;	/**< Number of sended bytes */

/* ======== Prototype =========*/

/* Low level communication */
lsocket* make_lsocket	(char*);								/**< [Public]: Create new socket */
lsocket* make_from_socket(struct sockaddr*,int,int);			/**< [Privte] */
void 	 open_lsocket	(lsocket*,int,int);					/**< [Public]: Open the socket for communications */
void 	 connect_lsocket(lsocket*,lsocket*);					/**< [Public]: Connect the socket to another */
lsocket* listen_lsocket	(lsocket*);								/**< [Public]: Listen to new incomming transmitions */
void	 bind_lsocket	(lsocket*);								/**< [Public]: Bind the socket for answers */
void 	 close_lsocket	(lsocket*,int);							/**< [Public]: Terminate the connection */
int 	 lsocket_send	(lsocket*,char*,int);					/**< [Privte] */
lsocket* lsocket_receive(lsocket*,char*,int);					/**< [Privte] */

/* High level communication: Deprecated, use Wrappers instead */
int 	 lsocket_message_send		(lsocket*,msg_type,char*);	/**< [Public]: Deprecated - Send message */
char*	 lsocket_message_receive	(lsocket*);					/**< [Public]: Deprecated - Return a raw message */

#endif /* __liblsockets_H__ */
