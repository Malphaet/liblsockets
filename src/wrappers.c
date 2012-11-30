/*
 * wrappers.c
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

#include "wrappers.h"
/** @file wrappers.c Wrappers (code) */

/* ========= Defines ==========*/

/* ========= Functions ========*/


/** \defgroup Wrappers Communication Managers
 * Handful of functions for managing communications, take care of everithing for you
 */
/** @{ */

/** Big sending function
 * @param sender_socket The receiver's socket if not connected, sender's if connected (lsockets allow to connect even UDP sockets)
 * @param type_message The type of the message (msg_type definitions are pretty clear)
 * @param msg The message itself
 * 
 * @return Number of sended bytes
 * @todo Thread-safe
 */
int message_send(lsocket*sender_socket,msg_type type_message,char*msg){
	lpacket*pck=lpacket_forge(type_message,msg);
	lpacket_send(sender_socket,pck);
	lpacket_drop(pck);
	return lpacket_snd_bytes;
}

/** Send a message through a socket specifiying a different destination for one connection.
 * @param dest The socket to send through
 * @param sender_socket The sender's socket
 * @param type_message The type of the message (msg_type definitions are pretty clear)
 * @param msg The message itself
 * Note that this function only makes sense on SOCK_DGRAM mode.
 * @return Number of sended bytes
 * @todo Thread-safe
 */
int message_send_to(lsocket*sender_socket,msg_type type_message,char*msg,lsocket*dest){
	lsocket*old_d;
	switch (sender_socket->mode){
		case SOCK_DGRAM:
			old_d=sender_socket->sendto;
			connect_lsocket(sender_socket,dest);
			message_send(sender_socket,type_message,msg);
			connect_lsocket(sender_socket,old_d);
			return lpacket_snd_bytes;
		case SOCK_STREAM:
			message_send(sender_socket,type_message,msg);
			return lpacket_snd_bytes;
		default:
			OUT("Unhandled mode");
			break;
	}
}

/** Big receiving function 
 * @param recver_socket The socket used for receiving the message
 * @param sender_socket The socket with will contain the sender's socket (if binded)
 * 
 * @return The received packet
 */
lpacket*message_receive(lsocket*recver_socket,lsocket**sender_socket){
	char*message=malloc(sizeof(char)*SIZE_BUFFER);
	lsocket*rcv; lpacket*pck;
	if (message==NULL) ERROR("lPacket malloc");
	
	rcv=lsocket_receive(recver_socket,message,SIZE_BUFFER);
	pck=lpacket_request(message);
	
	if (sender_socket!=NULL) *sender_socket=rcv;
	
	return pck;
}

/** Create new listening set (a basement: podrum in croatian) 
 * @param size Size of the future socket list
 * @return The created lpodrum
 */
lpodrum* make_lpodrum(int size){
	lsocket**sock_list=malloc(sizeof(lsocket*)*size);
	lpodrum*podr=malloc(sizeof(lpodrum));
	struct pollfd*list=malloc(sizeof(struct pollfd)*size);
	
	if (sock_list==NULL) ERROR("Socket list malloc");
	if (list==NULL) ERROR("Fd list malloc");
	if (podr==NULL) ERROR("Podrum list malloc");
	
	podr->cur_size=0;
	podr->max_size=size;
	podr->sockets=sock_list;
	podr->fd_list=list;
	podr->del_list=make_lclist();
	
	return podr;
}

/** Add a num socket to the basement 
 * @param podr The podrum to add the socket to
 * @param sock The socket to add
 * @param type The type of event expected, see poll manual for more information
 * Most common types of events:
 * \li POLLIN Data other than high-priority data may be read without blocking.
 * \li POLLPRI High-priority data may be read without blocking.
 * \li POLLOUT Normal data may be written without blocking.
 * \li POLLERR An error has occurred on the device or stream.
 * Receving only :
 * \li POLLNVAL Invalid request: fd not open (output only).
 * \li POLLHUP The device has been disconnected. This event and POLLOUT are mutually-exclusive;
 * 
 */
void add_lsocket(lpodrum*podr,lsocket*sock,int type){
	if (podr->cur_size==podr->max_size) OUT("Podrum max size reached (basement overflow)");
	podr->sockets[podr->cur_size]=sock;
	podr->fd_list[podr->cur_size].fd=sock->file;
	podr->fd_list[podr->cur_size].events=type;
	podr->cur_size++;
}

/** Get a socket number out of the listening basement 
 * @param podr The listening basement to consider
 * @param nb The index of socket to get
 * @return The retreived socket
 */
lsocket* get_lsocket(lpodrum*podr,int nb){
	if (podr->cur_size-1<nb) OUT("Index out of range");
	return podr->sockets[nb];
}

/** Delete the socket from the basement
 * @param podr The listening basement to consider
 * @param nb The index of the socket to remove
 * Note that the deletion is not immediate. 
 * It will take place when the listener is called again.
 * @return -1 in case of error, the index otherwise
 */
int del_lsocket(lpodrum*podr,int nb){
	if (podr->cur_size-1<nb) return -1;
	add_lclist(podr->del_list,nb);
	return nb;
}

/** Purge the podrum before using it 
 * @param podr The podrum to purge
 * Note that it is NOT supposed to be user-trigerred	
 */

void purge_lpodrum(lpodrum*podr){
	int todel,torep;
	while ((todel=pop_lclist(podr->del_list))!=LPOP_ERROR){
		torep=--podr->cur_size;
		if (todel==torep) torep--;

		if (verbose) printf("Killing %s\n",podr->sockets[todel]->addr);
		close_lsocket(podr->sockets[todel],1);

		if (torep<0){
			podr->sockets[todel]=NULL;
		} else {
			podr->fd_list[todel]=podr->fd_list[torep];
			podr->sockets[todel]=podr->sockets[torep];
		}
	}
}

/** List all the socket ready to communicate 
 * @param podr The lpodrum for the listening
 * @param timer Timeout before forced return (-1 for disabling)
 * @return The list of sockets's indexes ready to communicate
 */
int*listen_lpodrum(lpodrum*podr,int timer){
	int i,j=0;
	int nbs,*ret;
	
	/* Purge lpodrum */
	purge_lpodrum(podr);
	
	/* Listen for awaiting requests */
	nbs=poll(podr->fd_list,podr->cur_size,timer);
	if (nbs<0) ERROR("Podrum poll");
	
	/* Create the return list */
	ret=malloc(sizeof(int)*(nbs+1));
	if (ret==NULL) ERROR("Socket-indexes malloc");
	for (i=0;i<podr->cur_size;i+=1) if (podr->fd_list[i].revents!=0) ret[j++]=i;
	ret[j]=-1;
	
	return ret;
}

/** @} */
