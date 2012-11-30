/*
 * liblsockets.c
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

/** @file liblsockets.c Socket Library (code) */

/* ========= Defines ==========*/

int verbose; /**< Verbosity of the program */

/* ========= Functions ========*/
/** \defgroup liblsockets Little Socket Library
 * A very small lib, made mainly to avoid direct socket handling
 * 
 */
/** @{ */
 
/** Create a socket from the given informations 
 * @param name Address of the socket
 * @return The created socket
 */
lsocket* make_lsocket(char*name){
	lsocket*sck=malloc(sizeof(lsocket));
	char*addr=malloc(sizeof(char)*SIZE_ADDR);
	if (sck==NULL) ERROR("Socket malloc impossible");
	if (addr==NULL) ERROR("Socket naming impossible");
	
	/* Socket creation */
	if (verbose>1) printf("Socket created: %s\n",name);
	strcpy(addr,name); sck->addr=addr;
	sck->sendto=NULL;
	
	return sck;
}

/** Open the connection of the given socket 
 * @param sck The socket to open
 * @param type Type of the connection AF_UNIX or other
 * @param mode Mode of the connection SOCK_DGRAM or other
 */
void open_lsocket(lsocket*sck,int type,int mode){
	struct sockaddr_un*sock_un; struct sockaddr_in *sock_in; 
	char*tmp;
	struct hostent *server;
	if ((sck->file=socket(type,mode,0))<0) ERROR("Socket opening was impossible");
	
	switch (type){
		case AF_UNIX:
			sock_un=malloc(sizeof(struct sockaddr_un));
			if (sock_un==NULL) ERROR("Socket malloc");
			
			sock_un->sun_family=type;
			strcpy(sock_un->sun_path,sck->addr);
			
			sck->socket=(struct sockaddr*)sock_un;
			sck->mode=mode; sck->type=type;
			break;
		case AF_INET:
			sock_in=calloc(1,sizeof(struct sockaddr_in));
			if (sock_in==NULL) ERROR("Socket malloc");
			
			tmp=strtok(sck->addr,":");
			if (!strcmp(tmp," ")) sock_in->sin_addr.s_addr = htonl(INADDR_ANY);
			else{
				server=gethostbyname(tmp);
				if (server == NULL) OUT("Host cannot be found");
    			memcpy((char *)&(sock_in->sin_addr.s_addr),(char *)server->h_addr_list[0], server->h_length);
			}
/*			else sock_in->sin_addr.s_addr=inet_addr(tmp); */
    		
			tmp=strtok(NULL,"");
			sock_in->sin_port=htons(atoi(tmp));
			sock_in->sin_family=AF_INET;
			
			sck->socket=(struct sockaddr*)sock_in;
			sck->mode=mode; sck->type=type;
			sck->sendto=sck;
			break;
		default:
			OUT("Unhandled mode");
			break;
	}
}

/** Create a connection from the given socket and informations
 * @param sock The socket received durring incoming transmission
 * @param type Type of the connection AF_UNIX or other
 * @param mode Mode of the connection SOCK_DGRAM or other
 * @return An lsocket structure, made from the sockaddr sending
 */
 
lsocket*make_from_socket(struct sockaddr*sock,int type,int mode){
	lsocket*ret_sck=NULL;
	
	switch (type){
		case AF_UNIX:
			if(((struct sockaddr_un*)sock)->sun_path[0]==0) return NULL;
			ret_sck=make_lsocket(((struct sockaddr_un*)sock)->sun_path);
			break;
		case AF_INET:
			// OUT("AF_INET protocol doesn't needs that function");
			return NULL;
			// break;
		default:
			OUT("Unhandled mode");
			break;
	}
	open_lsocket(ret_sck,type,mode);
	return ret_sck;
}

/** Bind the sending socket for answers
 * @param send_sck The socket to bind for answers
 */
void bind_lsocket(lsocket*send_sck){
	switch (send_sck->type){
		case AF_UNIX:
			unlink(send_sck->addr);
			if (bind(send_sck->file,(struct sockaddr*)send_sck->socket,sizeof(*send_sck->socket))<0) ERROR("Binding socket failed");
			break;
		case AF_INET:
			if (bind(send_sck->file,(struct sockaddr*)send_sck->socket,sizeof(struct sockaddr_in))<0) ERROR("Binding socket failed");
			break;
		default:
			OUT("Unhandled mode");
			break;
	}
}

/** Connect the sending socket to another socket
 * @param sck The socket to be connected
 * @param recv_sck the socket witch will be connected to, UDP only, the parameter will be ignored on tcp
 */
void connect_lsocket(lsocket*sck,lsocket*recv_sck){
	switch(sck->mode){
		case (SOCK_DGRAM):
			sck->sendto=recv_sck;
			break;
		case (SOCK_STREAM):
			if (connect(sck->file,(struct sockaddr*)sck->socket,sizeof(struct sockaddr_in))<0) ERROR("Socket connection error");
			break;
		default:
			OUT("Unhandled mode");
	}
}							

/** Close a socket connection
 * @param sck The socket to close
 * @param shutd How to shutdown, on AF_UNIX it is freeing the memory (0) or total shutdown (1) on AF_INET it is from simple shutdown (0) to total one (2);
 */
void close_lsocket(lsocket*sck,int shutd){
	switch (sck->type){
		case AF_UNIX:
			if (shutd) unlink(sck->addr);
			break;
		case AF_INET:
			shutdown(sck->file,shutd);
			break;
		default:
			OUT("Unhandled mode");
			break;
	}
	
	close(sck->file);
	free(sck->socket);
	free(sck);
}

/** Send string to the server 
 * @param sck if connect_lsocket() wasn't used it's considered to be the receiver
 * If it was, it's considered to be the sender's socket and lsocket->sendto is considered to be the destination.
 * Nb: On connected sockets, the sender is also the receiver, and sendo link to the socket.
 * @param message The message itself
 * @param bytes Length of the message (in bytes)
 * @return The number of sended bytes
 * @todo Auto-split messages
 */
int lsocket_send(lsocket*sck,char*message,int bytes){
	int write_to=sck->file;
	lsocket*recver_socket;
	if (sck->sendto!=NULL) recver_socket=sck->sendto;
	else recver_socket=sck;
	
	switch (recver_socket->mode){
		case SOCK_DGRAM:
			return sendto(write_to,message,bytes,0,
				(struct sockaddr*)recver_socket->socket,
				sizeof(((struct sockaddr_un*)recver_socket->socket)->sun_family)
				+strlen(((struct sockaddr_un*)recver_socket->socket)->sun_path));
		case SOCK_STREAM:
			return send(sck->file,message,bytes,0);
		default:
			return write(write_to,message,bytes);
	}
}

/** Receive string from the socket 
 * @param sck The socket to receive from
 * @param message The message to store the received string
 * @param bytes The number of bytes to receive
 * @return The sending socket
 * @todo Auto-split messages
 */
lsocket* lsocket_receive(lsocket*sck, char*message,int bytes){
	unsigned int bsize=sizeof(struct sockaddr_un);
	lsocket*recv_sck=NULL;
	
	struct sockaddr*sock=calloc(1,sizeof(struct sockaddr));
	
	switch (sck->mode){
		case SOCK_DGRAM:
			lpacket_rcv_bytes=recvfrom(sck->file,message,bytes,0,sock,&bsize);
			if (lpacket_rcv_bytes<0) {
				printf("Error receiving packet from %s\n",sck->addr); //! @todo make debug only
				ERROR("Reciving packet");
			}
			if (sock!=NULL) recv_sck=make_from_socket((struct sockaddr*)sock,sck->type,sck->mode);
			return recv_sck;
		case SOCK_STREAM:
			lpacket_rcv_bytes=recv(sck->file,message,bytes,0);
			if (lpacket_rcv_bytes<0) {
				printf("Error receiving packet from %s\n",sck->addr);
				ERROR("Reciving packet");
			}
			return sck->sendto;
		default:
			lpacket_rcv_bytes=read(sck->file,message,bytes);
			return NULL;
	}
}

/**@}*/
