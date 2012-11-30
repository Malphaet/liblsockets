/*
 * test_unix.c
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
#include <signal.h>

int volatile should_quit=0;
void handler(int sig){
	if (sig==SIGINT) printf("Yeah\n");
		printf("[%d] Calmly exiting\n",getpid());
		should_quit=1;
}

void child_process(){
	char name[101];
	lsocket*chld,*nserv,*serv=make_lsocket("tmp/serv");
	lpacket*pck;
	
	sprintf(name,"tmp/chld_%d",getpid());
	chld=make_lsocket(name);
	
	open_lsocket(chld,AF_UNIX,SOCK_DGRAM);
	open_lsocket(serv,AF_UNIX,SOCK_DGRAM);
	bind_lsocket(chld);
	
	/* Hardcore setup actions */
	srand(getpid());
	sleep(rand()%4);
	
	/* Handshake */
	printf("[%d] Awake, sending message\n",getpid());
	message_send_to(chld,msg_sync,"syn",serv);
	pck=message_receive(chld,&nserv);
	printf("[%d] Server answered <%i> %s\n",getpid(),pck->type,pck->message);
	close_lsocket(serv,0);
	lpacket_drop(pck);
	/* Hardcore actions again */
	sleep(2);
	
	/* Send results */
	message_send_to(chld,msg_text,"Here I am",nserv);
	
	/* Quit */
	message_send(nserv,msg_kill,"Ciao");
	printf("[%d] Exiting\n",getpid());
	
	
	close_lsocket(nserv,0);
	close_lsocket(chld,1);
	exit(EXIT_SUCCESS);
}

void father_process(){
	lsocket*serv=make_lsocket("tmp/serv");		/* Listener */
	lsocket*sndr,*clnt;							/* Replyto */
	
	int*actives,i=0;
	unsigned int nb_clients=0;
	char name[42];
	lpacket*pck;
	lpodrum*podr=make_lpodrum(20);
	
	open_lsocket(serv,AF_UNIX,SOCK_DGRAM);
	bind_lsocket(serv);

	signal(SIGTERM,handler);
	signal(SIGINT,handler);

	add_lsocket(podr,serv,POLLIN);
	while (!should_quit){
		actives=listen_lpodrum(podr,-1); 
		for(i=0;actives[i]>=0;i++) {
			/* Wait for the communication */
			printf("[Server] Waiting %s:%d\n",get_lsocket(podr,actives[i])->addr,get_lsocket(podr,actives[i])->file);
			pck=message_receive(get_lsocket(podr,actives[i]),&sndr);
			printf("[Server] (%s:%d) sended <%i> %s\n",
				sndr?sndr->addr:get_lsocket(podr,actives[i])->addr,
				sndr?(int)sndr->file:get_lsocket(podr,actives[i])->file,
				pck->type,pck->message);
			
			/* 0 is the server address: new connections comes from here */
			if (i==0 && pck->type==msg_sync) {
				/* Create particular socket for him (note that it is generally not usefull)*/
				sprintf(name,"tmp/nw_clnt_%d",nb_clients++);
				clnt=make_lsocket(name);
				open_lsocket(clnt,AF_UNIX,SOCK_DGRAM);
				bind_lsocket(clnt);
				add_lsocket(podr,clnt,POLLIN);
				
				/* Send an answer with the new connection */
				message_send_to(clnt,msg_recv,"ack",sndr);
				close_lsocket(sndr,0);
			} else if (pck->type==msg_kill){
				/* If he wants to die, well kill it */
				del_lsocket(podr,actives[i]);
			}
			lpacket_drop(pck);
		}
		free(actives);
	}
	
	close_lsocket(serv,1);
	drop_lpodrum(podr,0);
	
}

void test_sockets(){
	int p;
	p=fork();
	if (p) father_process();
	else {
		//fork();
		//fork();
		child_process();
	}
}
