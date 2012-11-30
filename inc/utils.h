/*
 * utils.h
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


 
#ifndef __UTILS_H__
#define __UTILS_H__

/** @file utils.h Utilities
 *
 * All utilities are defined and documented here
 */

/** @def WHERE 
 * Print the current position on the program 
 */

/** @def ERROR(msg) 
 * Print the current error, with the error message @a msg and exits
 */
/** @def WARNING(msg) 
 * Print the current error, with the error message @a msg and exits if the program is in debug mode.
 */

/** @def OUT(msg) 
 * Exits the program with the message @a msg
 */

/** @def FPRINT(msg) 
 * Print @a msg in stdout and flush 
 */

/* ========= Defines ==========*/
int verbose; /**< Verbosity of the program */

/* Functions */


#define WHERE		printf("In %s line %d (%d:%s)\n",__FILE__,__LINE__,getpid(),__func__)
#define ERROR(msg)	{printf("ERROR: ");WHERE; perror(msg);printf("\n");exit(EXIT_FAILURE);}
#define WARNING(msg){printf("WARNING: ");WHERE;  perror(msg);printf("\n");}
#define OUT(msg)	{WHERE; fprintf(stderr,msg);fprintf(stderr,"\n");exit(EXIT_FAILURE);}
#define FPRINT(msg)	{fprintf(stdout,msg);fflush(stdout);}
#endif /* __UTILS_H__ */

