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
/* Includes for board system */
/* This is version 2 of the board system, (c) 1995-96 erwin@pip.dknet.dk */


#define CON_NOTE_TO						14
#define CON_NOTE_SUBJECT				15
#define CON_NOTE_EXPIRE					16
#define CON_NOTE_TEXT					17
#define CON_NOTE_FINISH					18

#define NOTE_DIR  				"../notes/" /* set it to something you like */

#define DEF_NORMAL  0   /* No forced change, but default (any string)   */
#define DEF_INCLUDE 1   /* 'names' MUST be included (only ONE name!)    */
#define DEF_EXCLUDE 2   /* 'names' must NOT be included (one name only) */

#define MAX_BOARD 	  9

#define DEFAULT_BOARD 0 /* default board is board #0 in the boards      */
                        /*
                         * It should be readable by everyone!           
                         */

#define MAX_LINE_LENGTH 80  /* enforce a max length of 80 on text lines, reject longer lines */
                           /*
                            * This only applies in the Body of the note 
                            */

#define MAX_NOTE_TEXT (4*MAX_STRING_LENGTH - 1000)

#define BOARD_NOTFOUND -1   /* Error code from board_lookup() and board_number */

/* Data about a board */
struct board_data
{
        char     *short_name;   /* Max 8 chars */
        char     *long_name;    /* Explanatory text, should be no more than 40 ? chars */

        int       read_level;   /* minimum level to see board */
        int       write_level;  /* minimum level to post notes */

        char     *names;    /* Default recipient */
        int       force_type;   /* Default action (DEF_XXX) */

        int       purge_days;   /* Default expiration */

        /*
         * Non-constant data 
         */

        NOTE_DATA *note_first;  /* pointer to board's first note */
        bool      changed;  /* currently unused */

};

typedef struct board_data BOARD_DATA;


/* External variables */

extern BOARD_DATA boards[MAX_BOARD];    /* Declare */


/* Prototypes */

void      finish_note(BOARD_DATA * board, NOTE_DATA * note);    /* attach a note to a board */
void      free_note(NOTE_DATA * note);  /* deallocate memory used by a note */
void      load_boards(void);    /* load all boards */
int       board_lookup(const char *name);   /* Find a board with that name */
bool      is_note_to(CHAR_DATA * ch, NOTE_DATA * note); /* is tha note to ch? */
void      personal_message(const char *sender, const char *to,
                           const char *subject, const int expire_days,
                           const char *text);
void      make_note(const char *board_name, const char *sender,
                    const char *to, const char *subject,
                    const int expire_days, const char *text);
void save_notes args((void));

/* for nanny */
void      handle_con_note_to(DESCRIPTOR_DATA * d, char *argument);
void      handle_con_note_subject(DESCRIPTOR_DATA * d, char *argument);
void      handle_con_note_expire(DESCRIPTOR_DATA * d, char *argument);
void      handle_con_note_text(DESCRIPTOR_DATA * d, char *argument);
void      handle_con_note_finish(DESCRIPTOR_DATA * d, char *argument);


/* Commands */

DECLARE_DO_FUN(do_note);
DECLARE_DO_FUN(do_board);


#define BOLD "#7"
#define NO_COLOR "#n"
#define GREEN "#2"
#define RED "#1"
#define BLUE "#4"
