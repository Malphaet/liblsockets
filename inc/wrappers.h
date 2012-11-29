/*
 * wrappers.h
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

#include "liblsockets.h"

#ifndef __LSOCKETS_WRAPPERS_H__
#define __LSOCKETS_WRAPPERS_H__

/* ======== Prototype =========*/

/** @file wrappers.h Wrappers (headers)
 *
 * All the big wrapping functions user-kinda-friendly 
 */
int 	 message_send			(lsocket*,msg_type,char*);			/**< [Public]: Send a message through the socket */
int 	 message_send_to		(lsocket*,msg_type,char*,lsocket*);	/**< [Public]: Send a message through the socket */
lpacket* message_receive		(lsocket*,lsocket**);				/**< [Public]: Return the received socket, save the sending socket */

lpodrum* make_lpodrum			(int);								/**< [Public]: Create new listening basement (podrum in croatian) */
void	 add_lsocket			(lpodrum*,lsocket*,int);			/**< [Public]: Add a num socket to the basement */
lsocket* get_lsocket			(lpodrum*,int);						/**< [Public]: Get the socket number */
int		 del_lsocket			(lpodrum*,int);						/**< [Public]: Delete the socket (int) from the basement */
int* 	 listen_lpodrum			(lpodrum*,int timeout);				/**< [Public]: Return the list from all the socket ready to communicate */
void	 purge_lpodrum			(lpodrum*);							/**< [Prvate]: Purge the lpodrum before usage */

#endif /* __WRAPPERS_H__ */

