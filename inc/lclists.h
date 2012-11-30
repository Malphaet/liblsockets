/*
 * lclists.h
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

#include <stdlib.h>
#include <stdio.h>

/** @file lclists.h Chained List Library (header) */

#ifndef __LLISTS_H__
#define __LLISTS_H__

/* ======= Defines ============ */
#define DATATYPE int			/**< Data handled by the clists */
#define LPOP_ERROR -1			/**< What pop_lclist() should return. Extremely important for automation. */

/* ======= Structs ============ */
/** A chained list structure */
typedef struct chained_list {
	DATATYPE data;				/**< Data holded by the chained list */
	struct chained_list*next;	/**< Next member of the chained list */
} lclist;

/* ======== Prototype =========*/
lclist*	 make_lclist			();									/**< [Public]: Make a new chained list */
void	 drop_lclist			(lclist*);							/**< [Public]: Drop the list */
void	 add_lclist				(lclist*,DATATYPE);					/**< [Public]: Add an element to the list */
DATATYPE pop_lclist				(lclist*);							/**< [Public]: Delete the first element of the list */
int		 len_lclist				(lclist*);							/**< [Public]: Lenght of the list */

#endif /* __LLISTS_H__ */

