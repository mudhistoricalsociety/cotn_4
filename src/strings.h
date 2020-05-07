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

#ifndef _MERCSTRING_H
# define _MERCSTRING_H   1

/* This file includes very commonly used text strings from around the mud.
 * Editing this file should easily change these strings.
 * DO remember some of these are best NOT colourized, primarily the first two.
 * Not all the mud's text is in here, but I suppose it COULD be to make
 * translation easier to do. However, right now this is just for convenience
 * in things that either need to be easily changed or are used in MANY places.
 */

/*
 * Some general mud-wide stuff
 * I put parsename and namerule here, because many admins will want to change them.
 */

# define TXT_MUDNAME	"Mindcloud" // Please do change this to your mud's name.
# define TXT_MUDVERSION	"Mcloud 2.1"    // Please don't change this unless you don't like to give credit.
# define TXT_COMPILE    __DATE__ " at " __TIME__    // No need to change this.
# define TXT_MUDEMAIL   "zarius@mindcloud.com"  // Please do change this to your own email address.
# define TXT_CONTINUE	"#C=#c====#C[ #WENTER Continues #C]#c===---#C-#n"   // For the pager

/*
 * Chracter Creation Text. Put here because much of it is duplicated, and you might want to change it.
 */


/*
 * Condition text, put here for easy changing.
 */

# define TXT_COND_A	" is in {Gexcellent condition{x."
# define TXT_COND_B	" has a {Yfew scratches{x."
# define TXT_COND_C	" has some {ysmall wounds and bruises{x."
# define TXT_COND_D	" has {Wquite a few wounds{x."
# define TXT_COND_E	" has some {Mbig nasty wounds and scratches{x."
# define TXT_COND_F	" looks {rpretty hurt{x."
# define TXT_COND_G	" is in {Rawful condition{r."
# define TXT_COND_H	" is {Rbleeding to death{x."

#endif // _MERCSTRING_H
