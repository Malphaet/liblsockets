/*
 * test.c
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
#include "wrappers.h"
/** @file test.c Tests
 *
 * A little test program, uncomment the part you wanna test on the main functions, and compile it with make test
 */

#include "test_unix.c"
#include "test_inet.c"


void test_lists(){
	lclist*liste=make_lclist();
	lclist*curr;
	add_lclist(liste,12);
	add_lclist(liste,42);
	add_lclist(liste,'$');
	
	printf("Liste contains %d elts\n",len_lclist(liste));
	curr=liste; while((curr=curr->next)!=NULL) printf("%d\n",curr->data);
	
	printf("Poped %d\n",pop_lclist(liste));
	printf("Liste contains %d elts\n",len_lclist(liste));
	
	curr=liste; while((curr=curr->next)!=NULL) printf("%d\n",curr->data);
	drop_lclist(liste);
}

int main (){
	//test_lists();
	test_sockets();
	//test_network_sockets();
	return 0;
}


