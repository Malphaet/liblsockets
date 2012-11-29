/*
 * structures.h
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

/** @file structures.h Struture definitions
 *
 * All structures are defined and documented here
 */

#include "liblsockets.h"

#ifndef __LIBLSOCKETS_STRUCTURES_H__
#define __LIBLSOCKETS_STRUCTURES_H__

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
	struct sk_addr* sendto;		/**< The socket to send to */
} lsocket;


/** A raw packet, for abstraction purpose
 *
 * Note that a message MUST be newline free, as it is used to end packets 
 * @see lpacket_forge() for more detailed information about socket creation.
 */
typedef struct pk_struct {
	msg_type type;		/**< The request being done, important for having normalised communications */
	char*message;		/**< The body of the message, can be empty */
} lpacket;

/** A chained list structure
 *
 * Made for handling sockets pending removal
 */

typedef struct int_chained_list {
	int data;
	struct int_chained_list*next;
} lclist;

/** A basement made of sockets
 *
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

#endif /* __STRUCTURES_H__ */

