/*
 * lpackets.h
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

#ifndef __LPACKETS_H__
#define __LPACKETS_H__

/* ======== Prototype =========*/

/**@file lpackets.h Packet Library (header) 
 *
 * Library for packet managing, note that all functions are not meant to be user handled
 */
 
lpacket* lpacket_forge		(msg_type,char*);					/**< [Privte] */
lpacket* lpacket_request	(char*);							/**< [Privte] */
char*	 lpacket_message	(lpacket*);							/**< [Privte] */
int 	 lpacket_send		(lsocket*,lpacket*);				/**< [Privte] */
//lpacket* lpacket_receive	(lsocket*);							/**< [Public]: Receive packet */
//void 	 lpacket_drop		(lpacket*);							/**< [Public]: Drop a packet */


#endif /* __PACKETS_H__ */

