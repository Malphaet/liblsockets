/*
 * lpackets.c
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

#include "lpackets.h"

/**@file lpackets.c Packet Library (code) */

/* ========= Defines ==========*/

/* ========= Functions ========*/
/** \defgroup Packetlib Little Packet Library
 * Packetlib is a small lib build ontop of liblsockets.
 * 
 * It is used extensively to avoid any interference with the implementation of the sockets.
 *
 * The following will allow you to handle basic communication:
 * \li lpacket_forge()
 * \li lpacket_send()
 * \li lpacket_drop()
 *
 * Moreover, the following functions are restricted:
 * \li lpacket_message()
 * \li lpacket_receive()
 * @{
 */

/** Forge a packet with the given information.
 * For memory usage reason, the message is copied not linked, free it if needed.
 * @param type The type of sended message
 * @param message The message to send
 * @return The forged packet
 */
lpacket*lpacket_forge(msg_type type,char *message){
	lpacket*pck=malloc(sizeof(lpacket));
	char*pck_message=malloc(sizeof(char)*(strlen(message)+2));
	if (pck==NULL) ERROR("Malloc lpacket");
	if (pck==NULL) ERROR("Malloc message");
	if (!strcmp(message,"")) {pck_message[0]=42;pck_message[1]='\0';}
	else strcpy(pck_message,message);
	pck->type=type;
	pck->message=pck_message;
	return pck;
}

/** Drop a packet and free the allocated memory. 
 * @param pck The packet to drop
 */
void lpacket_drop(lpacket*pck){
	free(pck->message);free(pck);
}

/** Create a packet from the given message. 
 * @param message The message to split
 * @return The forged packet
 */
lpacket*lpacket_request(char*message){
	int type;
	char*pck_message;
	
	//if (strlen(message)){
		type=atoi(strtok(message," "));
		pck_message=strtok(NULL,"\0");
	//} else {
		 //type=msg_wtf;
		 //pck_message="";
	//}
	return lpacket_forge(type,pck_message);
}

/** Create a message from given packet. 
 * @param pck The packet to make as a message
 * @return A message made from the packet
 */
char *lpacket_message(lpacket*pck){
	char*mess=pck->message;
	int size=strlen(mess)+5;
	char*message=malloc(sizeof(char)*size);
	if (message==NULL) ERROR("lPacket malloc");
	sprintf(message,"%d %s",pck->type,pck->message); //! @todo Improve splitting mechanism
	return message;
}

/** Send a packet through the given socket.
 * Note that you will NOT receive the number of readed packets.
 * You can access that information through @ref lpacket::bytes.
 * @param sck Witch socket will receive the packet (or the sender socket if connected)
 * @param pck The packet to send
 * @return The sucess or failure of the request
 */
int lpacket_send(lsocket*sck,lpacket*pck){
	char*message=lpacket_message(pck);
	int status=lsocket_send(sck,message,strlen(message)+1)==(int)strlen(message)+1;
	free(message);
	if (status)
		return 1;
	else WARNING("Socket sending error"); 
		return 0;
}

/** Receive a packet through the given socket
 * Note that you will receive a packet not the number of readed bytes. 
 * You can access that information through @ref lpacket::bytes.
 * @param sck The socket to receive through
 * @return The received packet
 */
lpacket*lpacket_receive(lsocket*sck){
	char*message=malloc(sizeof(char)*SIZE_BUFFER);
	if (message==NULL) ERROR("lPacket malloc");
	lsocket_receive(sck,message,SIZE_BUFFER);
	return lpacket_request(message);
}
/** @} */


