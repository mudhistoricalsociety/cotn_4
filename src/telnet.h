/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Dystopia Mud improvements copyright (C) 2000, 2001 by Brian Graversen  *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*	@(#)telnet.h 1.7 88/08/19 SMI; from UCB 5.1 5/30/85	*/
/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

/*
 * Definitions for the TELNET protocol.
 */

/***************************************************************************
 *                                 _/                            _/        *
 *      _/_/_/  _/_/      _/_/_/  _/    _/_/    _/    _/    _/_/_/         *
 *     _/    _/    _/  _/        _/  _/    _/  _/    _/  _/    _/          *
 *    _/    _/    _/  _/        _/  _/    _/  _/    _/  _/    _/           *
 *   _/    _/    _/    _/_/_/  _/    _/_/      _/_/_/    _/_/_/            *
 ***************************************************************************
 * Mindcloud Copyright 2001-2003 by Jeff Boschee (Zarius),                 *
 * Additional credits are in the help file CODECREDITS                     *
 * All Rights Reserved.                                                    *
 ***************************************************************************/

#ifndef _arpa_telnet_h
#define _arpa_telnet_h

#define	IAC	255 /* interpret as command: */
#define	DONT	254 /* you are not to use option */
#define	DO	253 /* please, you use option */
#define	WONT	252 /* I won't use option */
#define	WILL	251 /* I will use option */
#define	SB	250 /* interpret as subnegotiation */
#define	GA	249 /* you may reverse the line */
#define	EL	248 /* erase the current line */
#define	EC	247 /* erase the current character */
#define	AYT	246 /* are you there */
#define	AO	245 /* abort output--but let prog finish */
#define	IP	244 /* interrupt process--permanently */
#define	BREAK	243 /* break */
#define	DM	242 /* data mark--for connect. cleaning */
#define	NOP	241 /* nop */
#define	SE	240 /* end sub negotiation */
#define EOR     239 /* end of record (transparent mode) */

#define SYNCH	242 /* for telfunc calls */

#ifdef TELCMDS
char     *telcmds[] = {
        "SE", "NOP", "DMARK", "BRK", "IP", "AO", "AYT", "EC",
        "EL", "GA", "SB", "WILL", "WONT", "DO", "DONT", "IAC",
};
#endif

/* telnet options */
#define TELOPT_BINARY	0   /* 8-bit data path */
#define TELOPT_ECHO	1   /* echo */
#define	TELOPT_RCP	2   /* prepare to reconnect */
#define	TELOPT_SGA	3   /* suppress go ahead */
#define	TELOPT_NAMS	4   /* approximate message size */
#define	TELOPT_STATUS	5   /* give status */
#define	TELOPT_TM	6   /* timing mark */
#define	TELOPT_RCTE	7   /* remote controlled transmission and echo */
#define TELOPT_NAOL 	8   /* negotiate about output line width */
#define TELOPT_NAOP 	9   /* negotiate about output page size */
#define TELOPT_NAOCRD	10  /* negotiate about CR disposition */
#define TELOPT_NAOHTS	11  /* negotiate about horizontal tabstops */
#define TELOPT_NAOHTD	12  /* negotiate about horizontal tab disposition */
#define TELOPT_NAOFFD	13  /* negotiate about formfeed disposition */
#define TELOPT_NAOVTS	14  /* negotiate about vertical tab stops */
#define TELOPT_NAOVTD	15  /* negotiate about vertical tab disposition */
#define TELOPT_NAOLFD	16  /* negotiate about output LF disposition */
#define TELOPT_XASCII	17  /* extended ascic character set */
#define	TELOPT_LOGOUT	18  /* force logout */
#define	TELOPT_BM	19  /* byte macro */
#define	TELOPT_DET	20  /* data entry terminal */
#define	TELOPT_SUPDUP	21  /* supdup protocol */
#define	TELOPT_SUPDUPOUTPUT 22  /* supdup output */
#define	TELOPT_SNDLOC	23  /* send location */
#define	TELOPT_TTYPE	24  /* terminal type */
#define	TELOPT_EOR	25  /* end or record */
#define TELOPT_EXOPL	255 /* extended-options-list */

#ifdef TELOPTS
#define	NTELOPTS	(1+TELOPT_EOR)
char     *telopts[NTELOPTS] = {
        "BINARY", "ECHO", "RCP", "SUPPRESS GO AHEAD", "NAME",
        "STATUS", "TIMING MARK", "RCTE", "NAOL", "NAOP",
        "NAOCRD", "NAOHTS", "NAOHTD", "NAOFFD", "NAOVTS",
        "NAOVTD", "NAOLFD", "EXTEND ASCII", "LOGOUT", "BYTE MACRO",
        "DATA ENTRY TERMINAL", "SUPDUP", "SUPDUP OUTPUT",
        "SEND LOCATION", "TERMINAL TYPE", "END OF RECORD",
};
#endif

/* sub-option qualifiers */
#define	TELQUAL_IS	0   /* option is... */
#define	TELQUAL_SEND	1   /* send option */

#endif /*!_arpa_telnet_h */
