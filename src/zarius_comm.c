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

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "recycle.h"


void do_replay(CHAR_DATA * ch, char *argument)
{
        if (IS_NPC(ch))
        {
                send_to_char("You can't replay.\n\r", ch);
                return;
        }

        if (buf_string(ch->pcdata->buffer)[0] == '\0')
        {
                send_to_char("You have no tells to replay.\n\r", ch);
                return;
        }

        send_to_char(buf_string(ch->pcdata->buffer), ch);
        clear_buf(ch->pcdata->buffer);
        ch->tells = 0;
}

void do_tcolor(CHAR_DATA * ch, char *argument)
{
        char      buf[MSL];
        int       pcolornum;
        int       cost = 1000;

        if (IS_NPC(ch))
                return;

        if (argument[0] == '\0')
        {
                send_to_char
                        ("\n\r\n\r#n 0: None\n\r"
                         " 1: #vD#va#vr#vk #vF#vl#va#vm#ve#n\n\r"
                         " 2: #zS#zh#zi#zn#zi#zn#zg #zI#zc#ze#n\n\r"
                         " 3: #+D#+a#+r#+k #+W#+h#+i#+s#+p#+e#+r#+s#n\n\r"
                         " 4: #*M#*i#*d#*n#*i#*g#*h#*t #*S#*u#*n#n\n\r"
                         " 5: #xS#xt#xr#xa#xn#xg#xe #xF#xo#xr#xr#xe#xs#xt#n\n\r"
                         " 6: #hA#hm#hb#he#hr #hR#he#hf#hl#he#hc#ht#hi#ho#hn#hs#n\n\r"
                         " 7: #aS#ao#au#al#al#ae#as#as #aB#ar#ae#aa#at#ah#n\n\r"
                         " 8: #qA#qr#qc#qa#qn#qe #qM#qi#qs#qt#n\n\r"
                         " 9: #kW#kr#ke#kt#kc#kh#ke#kd #kB#ke#kt#kr#ka#ky#ka#kl#n\n\r"
                         "10: #eO#el#ed #eH#ee#er#eo#n\n\r"
                         "11: #tD#te#tm#to#tn#t\'s #tT#to#tn#tg#tu#te#n\n\r\n\r",
                         ch);
                xprintf(buf,
                        "Your current color pattern number is %d.\n\rTcolor skips any messages with color codes in it, like say ##nHello there!\n\r\n\r",
                        ch->talkcolor);
                send_to_char(buf, ch);
                return;
        }

        if (!is_number(argument))
        {
                send_to_char("Numbers only please.\n\r", ch);
                return;
        }

        if (ch->bones < cost)
        {
                xprintf(buf,
                        "You need %d more bones to use this command.\n\r",
                        cost - ch->bones);
                send_to_char(buf, ch);
                return;
        }

        pcolornum = atoi(argument);
        if (pcolornum >= 0 && pcolornum < 12)
        {
                ch->talkcolor = pcolornum;
                send_to_char("Your color has been set.\n\r", ch);
                ch->bones -= cost;
                return;
        }

}

char     *pcolor(CHAR_DATA * ch, const char *s, int tcolor)
{
        char     *p = new_arg;

        int       ttemp;

        tctoggle = FALSE;
        ttemp = 0;
        xprintf(new_arg, " ");

        if (ch != NULL)
                ttemp = ch->talkcolor;
        else
                ttemp = tcolor;

        for (; *s != '\0'; ++s)
                if (!isspace(*s) && *s != '#')
                {

                        if (tctoggle)
                        {
                                tctoggle = FALSE;
                        }
                        else
                        {
                                tctoggle = TRUE;
                        }

                        if (ttemp != 0 && tctoggle)
                                *p++ = '#';

                        if (ttemp == 1 && tctoggle)
                                *p++ = 'v';

                        if (ttemp == 2 && tctoggle)
                                *p++ = 'z';

                        if (ttemp == 3 && tctoggle)
                                *p++ = '+';

                        if (ttemp == 4 && tctoggle)
                                *p++ = '*';

                        if (ttemp == 5 && tctoggle)
                                *p++ = 'x';

                        if (ttemp == 6 && tctoggle)
                                *p++ = 'h';

                        if (ttemp == 7 && tctoggle)
                                *p++ = 'a';

                        if (ttemp == 8 && tctoggle)
                                *p++ = 'q';

                        if (ttemp == 9 && tctoggle)
                                *p++ = 'k';

                        if (ttemp == 10 && tctoggle)
                                *p++ = 'e';

                        if (ttemp == 11 && tctoggle)
                                *p++ = 't';

                        *p++ = *s;
                }
                else
                {
                        if (*s == '#')
                                ttemp = 0;
                        *p++ = *s;
                }
        *p = '\0';

        return new_arg;
}


void chatperform(CHAR_DATA * ch, CHAR_DATA * victim, char *msg)
{   /*ch here should be a NPC, but its checked below */
        char     *reply;

        if (!IS_NPC(ch) || (victim != NULL && IS_NPC(victim)))
                return; /*ignores ch who are PCs and victims who are NPCs */

//        reply = dochat(ch->name, msg, victim ? victim->name : "you");
        if (reply)
        {
                switch (reply[0])
                {
                case '\0': /* null msg */
                        break;
                case '"':  /*do say */
                        do_say(ch, reply + 1);
                        break;
                case ':':  /*do emote */
                        do_emote(ch, reply + 1);
                        break;
                case '!':  /*execute command thru interpreter */
                        interpret(ch, reply + 1);
                        break;
                default:   /* is a say or tell */
                        if (victim == NULL)
                                do_say(ch, reply);
                        else
                        {   /* do a tell  (this is always the case here) */
                                char      buf[MSL];

                                xprintf(buf, "$N #Rtells you '#W%s#R'#n",
                                        reply);
                                act(buf, victim, NULL, ch, TO_CHAR);
                                victim->reply = ch;
                        }
                }
        }
}


void chatperformtoroom(char *txt, CHAR_DATA * ch)
{   /*ch here is the PC saying the phrase */

        CHAR_DATA *vch;

        if (IS_NPC(ch))
                return; /*we dont want NPCs trigger'ing events */

        for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
                if (IS_NPC(vch) && !(HAS_TRIGGER_MOB(vch, TRIG_SPEECH))
                    && IS_AWAKE(vch))
                        chatperform(vch, ch, txt);
        return;

}
