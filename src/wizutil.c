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
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "merc.h"
#include "interp.h"
#include "recycle.h"

/*
===========================================================================
This snippet was written by Erwin S. Andreasen, erwin@pip.dknet.dk. You may 
use this code freely, as long as you retain my name in all of the files. You
also have to mail me telling that you are using it. I am giving this,
hopefully useful, piece of source code to you for free, and all I require
from you is some feedback.

Please mail me if you find any bugs or have any new ideas or just comments.

All my snippets are publically available at:

http://pip.dknet.dk/~pip1773/

If you do not have WWW access, try ftp'ing to pip.dknet.dk and examine
the /pub/pip1773 directory.

===========================================================================
*/

typedef enum
{ exit_from, exit_to, exit_both } exit_status;
const sh_int opposite_dir[6] =
        { DIR_SOUTH, DIR_WEST, DIR_NORTH, DIR_EAST, DIR_DOWN, DIR_UP };

/*
 * get the 'short' name of an area (e.g. MIDGAARD, MIRROR etc.
 * assumes that the filename saved in the AREA_DATA struct is something like midgaard.are
 */
char     *area_name(AREA_DATA * pArea)
{
        static char buffer[64];
        char     *period;

        assert(pArea != NULL);

        strncpy(buffer, pArea->filename, 64);
        period = strchr(buffer, '.');
        if (period)
                *period = '\0';
        return buffer;
}

void room_pair(ROOM_INDEX_DATA * left, ROOM_INDEX_DATA * right,
               exit_status ex, char *buffer)
{
        char     *sExit;

        switch (ex)
        {
        default:
                sExit = "??";
                break;  /* invalid usage */
        case exit_from:
                sExit = "< ";
                break;
        case exit_to:
                sExit = " >";
                break;
        case exit_both:
                sExit = "<>";
                break;
        }
        xprintf(buffer, "%5d %-26.26s %s%5d %-26.26s(%-8.8s)\n\r",
                left->vnum, left->name, sExit, right->vnum, right->name,
                area_name(right->area));
        return;
}

/* for every exit in 'room' which leads to or from pArea but NOT both, print it */
void checkexits(ROOM_INDEX_DATA * room, AREA_DATA * pArea, char *buffer)
{
        char      buf[MAX_STRING_LENGTH];
        int       i;
        EXIT_DATA *exitd;
        ROOM_INDEX_DATA *to_room;

        strcpy(buffer, "");

        for (i = 0; i < 6; i++)
        {
                exitd = room->exit[i];
                if (!exitd)
                        continue;
                else
                        to_room = exitd->to_room;
                if (to_room)
                {
                        if ((room->area == pArea) && (to_room->area != pArea))
                        {
                                if (to_room->exit[opposite_dir[i]]
                                    && to_room->exit[opposite_dir[i]]->
                                    to_room == room)
                                        room_pair(room, to_room, exit_both, buf);   /* <> */
                                else
                                        room_pair(room, to_room, exit_to, buf); /* > */
                                strcat(buffer, buf);
                        }
                        else
                        {
                                if ((room->area != pArea)
                                    && (exitd->to_room->area == pArea))
                                {   /* an exit from another area to our area */
                                        if (!
                                            (to_room->exit[opposite_dir[i]]
                                             && to_room->
                                             exit[opposite_dir[i]]->to_room ==
                                             room))
                                        {   /* two-way exits are handled in the other if */
                                                room_pair(to_room, room,
                                                          exit_from, buf);
                                                strcat(buffer, buf);
                                        }
                                }   /* if room->area */
                        }   /* for */
                }
        }
        return;
}

/* for now, no arguments, just list the current area */

/*
void do_exlist(CHAR_DATA * ch, char *argument)
{
        AREA_DATA *pArea;
        ROOM_INDEX_DATA *room;
        int       i;
        char      buf[MAX_STRING_LENGTH];
        BUFFER   *buffer;

        buffer = new_buf();
        pArea = ch->in_room->area;
        for (i = 0; i < MAX_KEY_HASH; i++)
        {
                for (room = room_index_hash[i]; room != NULL;
                     room = room->next)
                {
                        checkexits(room, pArea, buf);
                        add_buf(buffer, buf);
                }
        }
        pager_to_char(buf_string(buffer), ch);
        free_buf(buffer);
        return;
}
*/
void do_exlist (CHAR_DATA *ch, char * argument)
{
  AREA_DATA* pArea;
  ROOM_INDEX_DATA* room;
  int i;
  char buffer[MAX_STRING_LENGTH];

  pArea = ch->in_room->area;
  for (i = 0; i < MAX_KEY_HASH; i++)
  {
    for (room = room_index_hash[i]; room != NULL; room = room->next)
    {
      checkexits (room, pArea, buffer);
      send_to_char (buffer, ch);
    }
  }
  return;
}


void set_switchname(CHAR_DATA * ch, char *title)
{
        char      buf[MAX_STRING_LENGTH];

        if (IS_NPC(ch))
        {
                bug("Set_switchname: NPC.", 0);
                return;
        }
        strcpy(buf, title);
        free_string(ch->pcdata->switchname);
        ch->pcdata->switchname = str_dup(buf);
        return;
}

void set_pc_name(CHAR_DATA * ch, char *title)
{
        char      buf[MAX_STRING_LENGTH];

        if (IS_NPC(ch))
        {
                bug("Set_pc_name: NPC.", 0);
                return;
        }
        strcpy(buf, title);
        free_string(ch->name);
        ch->name = str_dup(buf);
        return;
}

void do_classself(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_STRING_LENGTH];
        char      arg2[MAX_STRING_LENGTH];
        char      buf[MSL];

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);


        if (IS_NPC(ch))
                return;

        if (!IS_NPC(ch) && ch->class != 0)
        {
                send_to_char("You already have a class.\n\r", ch);
                return;
        }
        if (ch->level < 3)
        {
                send_to_char("You must be avatar to selfclass.\n\r", ch);
                return;
        }
        if (arg1[0] == '\0')
        {
                send_to_char
                        ("Classes: Type selfclass <class> to get classed.\n\r\n\r",
                         ch);
                send_to_char("#W--==[    #RCotN Classes    #W]==--#n\n\r", ch);
                send_to_char("#0Demon,        Werewolf,       Drow\n\r",ch);
                send_to_char("#0Ninja,        Vampire,        Shapeshifter\n\r", ch);
                send_to_char("#0Battlemage,   Tanar'ri,       Undead Knight#n\n\r",ch);
                send_to_char("#0Cyborg,       Samurai,        Monk\n\r", ch);
                send_to_char("#0Angel,        Lich,	      Ghoul#n\n\r", ch);
                send_to_char("#0Skyblade,     Priest,         Draconian#n\n\r",ch);
                send_to_char("#0Shadow,       Jedi,           Thief#n\n\r", ch);
                send_to_char("#0Fae,          Drone,          Giant\n\r",ch);
                send_to_char("#0Hobbit,       Elemental,      Paladin #n\n\r",ch);
                send_to_char("#0Blademaster   Shinobi         Zombie#n\n\r",ch);
                send_to_char("#0Dragon#n\n\r",ch);

                return;
        }
        do_clearstats2(ch, "");
        if (!str_cmp(arg1, "demon"))
        {
                ch->class = CLASS_DEMON;
                set_learnable_disciplines(ch);
                send_to_char
                        ("You have chosen the #RDemonic#n path, may god have mercy on yer soul.\n\r",
                         ch);
                xprintf(buf, "#B%s #Chas chosen the #RDemonic#C path!#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
   else if ( !str_cmp( arg1, "dragon"      ) )
  {
    if ( !str_cmp( arg2, "red"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 1;
       send_to_char("You are now a red dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "green"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 2;
       send_to_char("You are now a green dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "blue"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 3;
       send_to_char("You are now a blue dragon!\n\r", ch);
    }

    else if ( !str_cmp( arg2, "bronze"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 4;
       send_to_char("You are now a bronze dragon!\n\r", ch);
    }

    else if ( !str_cmp( arg2, "silver"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 5;
       send_to_char("You are now a silver dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "white"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 6;
       send_to_char("You are now a white dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "black"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 7;
       send_to_char("You are now a black dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "gold"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 8;
       send_to_char("You are now a gold dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "shadow"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 9;
       send_to_char("You are now a shadow dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "purple"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 10;
       send_to_char("You are now a purple dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "chaos"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 11;
       send_to_char("You are now a chaos dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "sapphire"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 12;
       send_to_char("Your are now a Sapphire dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "emerald"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 13;
       send_to_char("You are now an Emerald dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "ruby"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 14;
       send_to_char("You are now a Ruby Dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "topaz"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 15;
       send_to_char("Your are now a Topaz dragon!\n\r", ch);
    }
    else if ( !str_cmp( arg2, "diamond"))
    {
       ch->class = CLASS_DRAGON;
       set_learnable_disciplines(ch);
       ch->pcdata->powers[dragonage] = 1;
       ch->special = 0;
       ch->pcdata->powers[DRAGON_COLOR] = 16;
      send_to_char("Your are now a Diamond dragon!\n\r", ch);
    }
	else
    {
    stc("Shadow, Gold, Black, White, Silver, Bronze, Blue, Green \n\r",ch);
    stc("Diamond, Topaz, Ruby, Emerald, Sapphire, Chaos, Purple, Red\n\r",ch);
    return;
    }

    return;
    }

	else if (!str_cmp(arg1,"zombie"))
	{
		ch->class = CLASS_ZOMBIE;
		send_to_char( "You are now a #7Zombie#n.\n\r", ch);
		sprintf(buf,"%s Has become a #7Zombie#n.", ch->name);
		do_info(ch,buf);
	}
 else if(!str_cmp(arg1,"shinobi"))
{

             if(!str_cmp(arg2,"fire")){ 
                ch->class = CLASS_SHINOBI;ch->pcdata->powers[S_ELEMENT] = S_FIRE;
		sprintf(buf,"%s Has Become a Fire Shinobi.", ch->name);
		do_info(ch,buf);stc("You are now a Fire Shinobi.\n\r",ch);}
             else if(!str_cmp(arg2,"earth")){
                ch->class = CLASS_SHINOBI; ch->pcdata->powers[S_ELEMENT] = S_EARTH;
                sprintf(buf,"%s Has Become a Earth Shinobi.", ch->name);
                do_info(ch,buf);stc("You are now an Earth Shinobi.\n\r",ch);}
             else if(!str_cmp(arg2,"wind")){ 
                ch->class = CLASS_SHINOBI;ch->pcdata->powers[S_ELEMENT] = S_WIND;
                sprintf(buf,"%s Has Become a Wind Shinobi.", ch->name);
                do_info(ch,buf);stc("You are now a Wind Shinobi.\n\r",ch);}
             else if(!str_cmp(arg2,"water")){ 
                ch->class = CLASS_SHINOBI;ch->pcdata->powers[S_ELEMENT] = S_WATER;
                sprintf(buf,"%s Has Become a Water Shinobi.", ch->name);
                do_info(ch,buf);stc("You are now a Water Shinobi.\n\r",ch);}
             else if(!str_cmp(arg2,"shadow")){ 
                ch->class = CLASS_SHINOBI;ch->pcdata->powers[S_ELEMENT] = S_SHADOW;
                sprintf(buf,"%s Has Become a Shadow Shinobi.", ch->name);
                do_info(ch,buf);stc("You are now a Shadow Shinobi.\n\r",ch);}
             else if(!str_cmp(arg2,"holy")){ 
                ch->class = CLASS_SHINOBI;ch->pcdata->powers[S_ELEMENT] = S_HOLY;
                sprintf(buf,"%s Has Become a Holy Shinobi.", ch->name);
                do_info(ch,buf);stc("You are now a Holy Shinobi.\n\r",ch);}
              else{
                stc("Select an element: Fire, Earth, Water, Wind, Holy, Shadow\n\r",ch);
                return;}
        }

        else if (!str_cmp(arg1, "elemental"))
        {
                ch->class = CLASS_ELEMENTAL;
                send_to_char
                        ("You have chosen the path of Nature.\n\r",
                         ch);
                xprintf(buf, "#B%s #0has become an Elemental#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "paladin"))
        {
                ch->class = CLASS_PALADIN;
                send_to_char
                        ("You have chosen the path of Good.\n\r",
                         ch);
                xprintf(buf, "#B%s #0has chosen to dedicate their life to the Oath and the Measure.#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "blademaster"))
        {
                ch->class = CLASS_BLADE_MASTER;
                send_to_char
                        ("You are one with a blade.\n\r",
                         ch);
                xprintf(buf, "#B%s #0has become a Blade Master.#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "thief"))
        {
                send_to_char
                        ("#0You take up the dangerous lifestyle of the thief, vowing to kill and backstab your way to the top!#n\n\r",
                         ch);
                ch->class = CLASS_THIEF;
                xprintf(buf,
                        "#R%s #0has taken up the lifestyle of the murderous thief!  Watch your possessions... and your back!#n",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "drone"))
        {
                ch->class = CLASS_DRONE;
                send_to_char
                        ("Drool pours from your mouth as you become a drone.\n\r",
                         ch);
                xprintf(buf, "#R%s #0has become a drooling drone!#n",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "fae"))
        {
                ch->class = CLASS_FAE;
                set_learnable_disciplines(ch);
                send_to_char("You become a fae, kick some butt!.\n\r", ch);
                xprintf(buf, "#R%s #0has become a fae!#n",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "hobbit"))
        {
                ch->class = CLASS_HOBBIT;
                send_to_char
                        ("Welcome to hobbithood, enjoy the stay, drinks will be served shortly.\n\r",
                         ch);
                xprintf(buf,
                        "#R%s #0shrinks hobbit size and begins blabbing about recipes!#n",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "giant"))
        {
                ch->class = CLASS_GIANT;
                send_to_char
                        ("You grow and grow, finally you stop and realise your a Giant.\n\r",
                         ch);
                xprintf(buf, "#R%s #Ggrows to enormous heights!#n",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "priest"))
        {
                send_to_char
                        ("#wYou take up the holy life of a #WPriest#n\n\r",
                         ch);
                ch->class = CLASS_PRIEST;
                xprintf(buf,
                        "#W%s #whas taken the life of a holy man as a #BPriest#n",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "shadow"))
        {
                send_to_char
                        ("#0You take up the secret lifestyle of the dark assassin!#n\n\r",
                         ch);
                ch->class = CLASS_SHADOW;
                xprintf(buf,
                        "#R%s #0has taken up the secret lifestyle of the dark assassin!#n",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "jedi"))
        {
                send_to_char
                        ("#0You learn the ways of force and begin Jedi Training!#n\n\r",
                         ch);
                ch->class = CLASS_JEDI;
                xprintf(buf,
                        "#B%s #0learns the ways of the #RForce #0and begins #BJedi #0training.#n",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "draconian"))
        {
                stc("Your features become scaly, and lizard-like... You are now a Draconian!\n\r", ch);
                xprintf(buf,
                        "#G%s #yhas begun their quest for the ancient dragon orbs!#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
                ch->class = CLASS_DRACONIAN;
        }
        else if (!str_cmp(arg1, "werewolf"))
        {
                ch->class = CLASS_WEREWOLF;
                set_learnable_disciplines(ch);
                send_to_char
                        ("You have chosen the path of the #0Garou#n, may gaia guide you.\n\r",
                         ch);
                xprintf(buf,
                        "#B%s #Chas chosen the path of the #0Lycanthrope#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }

        else if (!str_cmp(arg1, "drow"))
        {
                ch->class = CLASS_DROW;
                send_to_char
                        ("Choose your profession, and #PLloth#n will guide you.\n\r",
                         ch);
                xprintf(buf,
                        "#B%s #Chas been chosen by Lloth to join the Drow horde!#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "cyborg"))
        {
                ch->class = CLASS_CYBORG;
                send_to_char
                        ("Wires and Metal take over your body as you become a Cyborg.\n\r",
                         ch);
                xprintf(buf,
                        "#B%s #Cis ripped apart and put back together with metal and wires!#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
                SET_BIT(ch->pcdata->jflags, JFLAG_KNOWS_FORGING);
        }
        else if (!str_cmp(arg1, "lich"))
        {
                ch->class = CLASS_LICH;
                send_to_char
                        ("You rise from the dead and wage war on the living.\n\r",
                         ch);
                xprintf(buf, "#B%s #crises from hell as a #WLICH!#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "shapeshifter"))
        {
                ch->class = CLASS_SHAPESHIFTER;
                send_to_char
                        ("Your skin becomes soft as you feel at one with the world.\n\r",
                         ch);
                xprintf(buf,
                        "#B%s #Cgains the ability to reform themselves as a shapeshifter!#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "ghoul"))
        {
                send_to_char
                        ("#cYou chant the dark rights that will make you into an undead beast...#n\n\r",
                         ch);
                send_to_char("#RYou feel death overpower you!#n\n\r", ch);
                ch->hit = 1;
                ch->class = CLASS_GHOUL;
                send_to_char
                        ("#cYou are now undead, forever cursed to roam the plane of the living!#n\n\r",
                         ch);
                xprintf(buf,
                        "#G%s #chas been doomed to roam the lands as an undead apparition for all #Ceternity!#n",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "skyblade"))
        {
                send_to_char("#BThe power of the dragon fills your body.#n\n",
                             ch);
                send_to_char
                        ("#CYour blade begins to glow as the aura fills the room.#n\n",
                         ch);
                ch->class = CLASS_SKYBLADE;
                send_to_char
                        ("#BYou are now one of the mighty #CSkyblades#L.#n\n\r",
                         ch);
                xprintf(buf,
                        "#B%s #Chas been washed in the aura of the dragon and has became one of the skyblades!#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "samurai"))
        {
                ch->class = CLASS_SAMURAI;
                send_to_char
                        ("You decide to lead the solitary life of a samurai\n\r",
                         ch);
                xprintf(buf,
                        "#B%s #Chas chosen the solitary life of a #Ysamurai!#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "tanarri"))
        {
                ch->class = CLASS_TANARRI;
                send_to_char
                        ("You become a hellspawn and a warrior for EVIL\n\r",
                         ch);
                xprintf(buf,
                        "#B%s #whas been chosen to be a #RHellspawn Tanarri!#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "undead"))
        {
                ch->class = CLASS_UNDEAD_KNIGHT;
                send_to_char("You mount your steed and ride out of hell!\n\r",
                             ch);
                xprintf(buf,
                        "#B%s #cis chosen to be one of hells #BUndead Knights#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "angel"))
        {
                ch->class = CLASS_ANGEL;
                send_to_char
                        ("You become a warrior for good and all that is holy.\n\r",
                         ch);
                xprintf(buf,
                        "#B%s #Chas become a warrior for good as an #WAngel#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else if (!str_cmp(arg1, "ninja"))
        {
                ch->class = CLASS_NINJA;
                send_to_char
                        ("You have chosen a life in the #0shadows#n, assassinate at will.\n\r",
                         ch);
                xprintf(buf,
                        "#B%s #0chooses the life of shadows and #bdarkness as a #Yninja#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }

        else if (!str_cmp(arg1, "vampire"))
        {
                ch->class = CLASS_VAMPIRE;
                ch->beast = 30;
                set_learnable_disciplines(ch);
                send_to_char
                        ("Fear the #YSun#n nosferatu, God's curse lives in you.\n\r",
                         ch);
                xprintf(buf,
                        "#R%s #rbears their fangs and lusts for #Rblood!#n",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }

        else if (!str_cmp(arg1, "monk"))
        {
                ch->class = CLASS_MONK;
                ch->level = 3;
                send_to_char
                        ("You faith in God will guide you, destroy #wEVIL#n.\n\r",
                         ch);
                xprintf(buf, "#B%s #wlearns the way of the #Bmonk#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }

        else if (!str_cmp(arg1, "mage") || !str_cmp(arg1, "battlemage"))
        {
                ch->class = CLASS_MAGE;
                ch->level = 3;
                send_to_char
                        ("You start down the path of power, the #Rarcane#n is your weapon.\n\r",
                         ch);
                xprintf(buf, "#B%s #0follows a life of arcane #PMagic!#n\n\r",
                        ch->pcdata->switchname);
                do_info(ch, buf);
        }
        else
                do_classself(ch, "");
        return;
}

void do_reimb(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *vch;
        char      arg[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        int       v;

        argument = one_argument(argument, arg);
        if (arg[0] == '\0')
        {
                send_to_char("Reimburse <target> <xp|qp> <amount>\n\r", ch);
                return;
        }
        if ((vch = get_char_world(ch, arg)) == NULL)
        {
                send_to_char("They aren't logged on.\n\r", ch);
                return;
        }
        if (IS_NPC(vch))
        {
                send_to_char("That is a mob.\n\r", ch);
                return;
        }
        argument = one_argument(argument, arg);
        argument = one_argument(argument, arg2);
        v = atoi(arg2);
        if (arg[0] == '\0' || arg2[0] == '\0' || (!is_number(arg2) && v >= 0))
        {
                do_reimb(ch, "");
                return;
        }
        if (!str_cmp(arg, "xp"))
        {
                vch->exp += v;
                vch->pcdata->score[SCORE_TOTAL_XP] += v;
        }
        else if (!str_cmp(arg, "bones"))
        {
                vch->bones += v;
        }
        else
        {
                send_to_char("Please specify XP or Bones.\n\r", ch);
                return;
        }
        if (vch->mkill < 5)
        {
                vch->mkill = 5;
                do_autosave(ch, "");
        }
        xprintf(arg2, "%s reimbursed %d %s.\n\r", vch->name, v, arg);
        send_to_char(arg2, ch);
        xprintf(arg2, "%s has reimbursed you %d %s.\n\r", ch->name, v, arg);
        send_to_char(arg2, vch);
}

void do_affects(CHAR_DATA * ch, char *argument)
{
        char      buf[MAX_STRING_LENGTH];
        AFFECT_DATA *paf;
        AFFECT2_DATA *paf2;

        if (IS_NPC(ch))
                return;

        if (IS_ITEMAFF(ch, ITEMA_ICESHIELD))
                send_to_char("#CIceshield#n.\n\r", ch);
        if (IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD))
                send_to_char("#BLightningshield#n.\n\r", ch);
        if (IS_ITEMAFF(ch, ITEMA_ACIDSHIELD))
                send_to_char("#GAcidshield#n.\n\r", ch);
        if (IS_ITEMAFF(ch, ITEMA_FIRESHIELD))
                send_to_char("#RFireshield#n.\n\r", ch);
        if (IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD))
                send_to_char("#pChaosshield#n.\n\r", ch);

        //more added by Zarius
        if (IS_ITEMAFF(ch, ITEMA_SPEED))
                send_to_char("#bH#Ba#Cs#Bt#be#n.\n\r", ch);
        if (IS_ITEMAFF(ch, ITEMA_REGENERATE))
                send_to_char("#GEnhanced Regeneration#n.\n\r", ch);
        if (IS_ITEMAFF(ch, ITEMA_VISION))
                send_to_char("#WVision#n.\n\r", ch);
        if (IS_ITEMAFF(ch, ITEMA_CHAOSHANDS))
                send_to_char("#rChaos#Yhands#n.\n\r", ch);
        if (IS_SET(ch->sohbits, SOH_ACCURACY))
                stc("#gM#Gagical #gA#Gccuracy#n\n\r", ch);
        if (IS_SET(ch->sohbits, SOH_HASTE))
                stc("#lH#Laste#n\n\r", ch);
        if (IS_SET(ch->sohbits, SOH_SLOW))
                stc("#oS#ylow#n\n\r", ch);
        if (IS_SET(ch->sohbits, SOH_REGEN))
                stc("#gM#Gage #gR#Gegen#n\n\r", ch);
        if (IS_SET(ch->sohbits, SOH_MIGHT))
                stc("#rM#Ragical #rM#Right#n\n\r", ch);
        if (IS_SET(ch->sohbits, SOH_SHARDS))
                stc("#cM#Ca#7g#Cical #cS#Ch#7a#Crds#n\n\r", ch);

        if (IS_AFFECTED(ch, AFF_SANCTUARY))
                send_to_char("#wSanctuary#n.\n\r", ch);
        if (IS_AFFECTED(ch, AFF_PROTECT))
                send_to_char("#BProtection from evil#n.\n\r", ch);
        if (IS_AFFECTED(ch, AFF_PROTECT_GOOD))
                send_to_char("#BProtection from good#n.\n\r", ch);
        if (IS_AFFECTED(ch, AFF_FLYING))
                send_to_char("#cFly#n\n\r", ch);

	// more added by Xrakisis
		if(IS_ITEMAFF(ch,ITEMA_VORPAL))
		send_to_char("#RVorpal#n.\n\r",ch);
		if (IS_AFFECTED(ch, AFF_PASS_DOOR))
        send_to_char("#gPassDoor#n.\n\r",ch);
		if (IS_AFFECTED(ch, AFF_SNEAK))
		send_to_char("#gSilent Movement#n.\n\r",ch);

     if (IS_AFFECTED(ch, AFF_HIDE))
       send_to_char("#7Hiding#n.\n\r",ch);
     if (IS_CLASS(ch, CLASS_VAMPIRE) && IS_SET(ch->act, AFF_HIDE))
       send_to_char("#0Shrouded#n.\n\r",ch);
     if (IS_CLASS(ch, CLASS_WEREWOLF) && IS_SET(ch->act, AFF_HIDE))
       send_to_char("#0Vanish#n.\n\r",ch);
     if(IS_SET(ch->newbits,NEW_DROWHATE))
       send_to_char("#7Drow HATE#n.\n\r",ch);
    if (IS_SET(ch->special, SPC_WOLFMAN))
       send_to_char("#7WereWolf Form#n.\n\r",ch);

     if (IS_AFFECTED(ch, AFF_BLIND))
       send_to_char("#7Blindness#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_INVISIBLE))
       send_to_char("#7Invisibility#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_DETECT_EVIL))
       send_to_char("#7Evil Detection#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_DETECT_INVIS))
       send_to_char("#7Invisibility Detection#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_DETECT_MAGIC))
       send_to_char("#7Arcane Detection#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_DETECT_HIDDEN))
       send_to_char("#7Extra Sensory Perception#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
       send_to_char("#7ShadowPlane#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_FAERIE_FIRE))
       send_to_char("#7Faerie Fire#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_INFRARED))
       send_to_char("#7InfraRed#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_CURSE))
       send_to_char("#7Curse#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_FLAMING))
       send_to_char("#7On Fire#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_POISON))
       send_to_char("#7Poisoned#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_ETHEREAL))
       send_to_char("#7Ethereal#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_SLEEP))
       send_to_char("#7Sleeping#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_CHARM))
       send_to_char("#7Charmed#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_POLYMORPH))
       send_to_char("#7PolyMorphed#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_SHADOWSIGHT))
       send_to_char("#7ShadowSight#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_WEBBED))
       send_to_char("#7Webbed#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_DROWFIRE))
       send_to_char("#7Covered with DrowFire#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_ZULOFORM))
       send_to_char("#7Zulo-Form#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_PEACE))
       send_to_char("#7Filled with Peace#n.\n\r",ch);
     if (IS_AFFECTED(ch, AFF_INFIRMITY))
       send_to_char("#7Infirmity#n.\n\r",ch);

        send_to_char("\n\r", ch);
        if (ch->affected == NULL && ch->affected2 == NULL)
        {
                send_to_char
                        ("You have nothing affecting you at this time.\n\r",
                         ch);
                return;
        }
        if (ch->affected != NULL)
        {

                send_to_char("You are affected by:\n\r", ch);
                for (paf = ch->affected; paf != NULL; paf = paf->next)
                {
                        xprintf(buf, "#wSpell#R: #w'#B%s#w'#n",
                                skill_table[paf->type].name);
                        send_to_char(buf, ch);
                        if (ch->level >= 0)
                        {
                                xprintf(buf,
                                        " #wmodifies #G%s #wby #Y%d #wfor #R%d #whours with bits #P%s#w.#n\n\r",
                                        affect_loc_name(paf->location),
                                        paf->modifier, paf->duration,
                                        affect_bit_name(paf->bitvector));
                                send_to_char(buf, ch);
                        }

                }

        }
        if (ch->affected2 != NULL)
        {

                for (paf2 = ch->affected2; paf2 != NULL; paf2 = paf2->next)
                {
                        xprintf(buf, "#wSpell#R: #w'#B%s#w'#n",
                                skill_table[paf2->type].name);
                        send_to_char(buf, ch);
                        if (ch->level >= 0)
                        {
                                xprintf(buf,
                                        " #wmodifies #G%s #wby #Y%d #wfor #R%d #whours with bits #P%s#w.#n\n\r",
                                        affect_loc_name(paf2->location),
                                        paf2->modifier, paf2->duration,
                                        affect2_bit_name(paf2->bitvector));
                                send_to_char(buf, ch);
                        }

                }

        }

        return;

}
char     *plr_bit_name(int arg)
{
        static char buf[512];

        buf[0] = '\0';

        if (arg & PLR_IS_NPC)
                strcat(buf, " npc");
        if (arg & PLR_BRIEF)
                strcat(buf, " brief");
        if (arg & PLR_COMBINE)
                strcat(buf, " combine");
        if (arg & PLR_PROMPT)
                strcat(buf, " prompt");
        if (arg & PLR_TELNET_GA)
                strcat(buf, " telnet_ga");
        if (arg & PLR_HOLYLIGHT)
                strcat(buf, " holylight");
        if (arg & PLR_WIZINVIS)
                strcat(buf, " wizinvis");
        if (arg & PLR_ANSI)
                strcat(buf, " ansi");
        if (arg & PLR_SILENCE)
                strcat(buf, " silenced");
        if (arg & PLR_NO_TELL)
                strcat(buf, " no_tell");
        if (arg & PLR_LOG)
                strcat(buf, " log");
        if (arg & PLR_FREEZE)
                strcat(buf, " freeze");
        if (arg & PLR_BRIEF5)
                strcat(buf, " brief5");
        if (arg & PLR_BRIEF6)
                strcat(buf, " brief6");
        if (arg & PLR_BRIEF7)
                strcat(buf, " brief7");
        if (arg & PLR_BRIEF4)
                strcat(buf, " brief4");
        if (arg & PLR_BRIEF3)
                strcat(buf, " brief3");
        if (arg & PLR_BRIEF2)
                strcat(buf, " brief2");
        return (buf[0] != '\0') ? buf + 1 : "none";
}

char     *plr2_bit_name(int arg)
{
        static char buf[512];

        buf[0] = '\0';

        if (arg & PLR_AUTOEXIT)
                strcat(buf, " autoexit");
        if (arg & PLR_AUTOLOOT)
                strcat(buf, " autoloot");
        if (arg & PLR_AUTOSAC)
                strcat(buf, " autosac");
        if (arg & PLR_AUTOHEAD)
                strcat(buf, " autohead");
        if (arg & PLR_AUTOSLAB)
                strcat(buf, " autoslab");
        if (arg & PLR_AUTOBONES)
                strcat(buf, " autobones");
        if (arg & PLR_AUTOSPLIT)
                strcat(buf, " autosplit");
        if (arg & PLR_WIPED)
                strcat(buf, " wiped");
        if (arg & PLR_AUTOSIGIL)
                strcat(buf, " autosigil");
        return (buf[0] != '\0') ? buf + 1 : "none";
}

char     *extra_plr_bit_name(int arg)
{
        static char buf[512];

        buf[0] = '\0';

        if (arg & EXTRA_TRUSTED)
                strcat(buf, " q_trusted");
        if (arg & EXTRA_NEWPASS)
                strcat(buf, " newpass");
        if (arg & EXTRA_OSWITCH)
                strcat(buf, " oswitch");
        if (arg & EXTRA_SWITCH)
                strcat(buf, " switch");
        if (arg & EXTRA_FAKE_CON)
                strcat(buf, " fake_con");
        if (arg & TIED_UP)
                strcat(buf, " tied_up");
        if (arg & GAGGED)
                strcat(buf, " gagged");
        if (arg & BLINDFOLDED)
                strcat(buf, " blindfolded");
        if (arg & EXTRA_DONE)
                strcat(buf, " non_virgin");
        if (arg & EXTRA_EXP)
                strcat(buf, " got_exp");
        if (arg & EXTRA_PREGNANT)
                strcat(buf, " pregnant");
        if (arg & EXTRA_LABOUR)
                strcat(buf, " labour");
        if (arg & EXTRA_BORN)
                strcat(buf, " born");
        if (arg & EXTRA_PROMPT)
                strcat(buf, " prompt");
        if (arg & EXTRA_MARRIED)
                strcat(buf, " married");
        if (arg & EXTRA_CALL_ALL)
                strcat(buf, " call_all");
        return (buf[0] != '\0') ? buf + 1 : "none";
}
char     *get_position_name(int arg)
{
        switch (arg)
        {
        case 0:
                return "dead";
        case 1:
                return "mortal";
        case 2:
                return "incap";
        case 3:
                return "stunned";
        case 4:
                return "sleeping";
        case 5:
                return "meditating";
        case 6:
                return "sitting";
        case 7:
                return "resting";
        case 8:
                return "fighting";
        case 9:
                return "standing";
        }
        bug("Get_position_name: unknown type %d.", arg);
        return "(unknown)";
}

/*
 * Itemaffect bit names :)
 */
char     *itemaffect_bit_name(int arg)
{
        static char buf[512];

        buf[0] = '\0';

        if (arg & ITEMA_SHOCKSHIELD)
                strcat(buf, " Shockshield");
        if (arg & ITEMA_FIRESHIELD)
                strcat(buf, " Fireshield");
        if (arg & ITEMA_ICESHIELD)
                strcat(buf, " Iceshield");
        if (arg & ITEMA_ACIDSHIELD)
                strcat(buf, " Acidshield");
        if (arg & ITEMA_CHAOSSHIELD)
                strcat(buf, " Chaoshield");
        if (arg & ITEMA_ARTIFACT)
                strcat(buf, " Artifact");
        if (arg & ITEMA_REGENERATE)
                strcat(buf, " Regeneration");
        if (arg & ITEMA_SPEED)
                strcat(buf, " Speed");
        if (arg & ITEMA_VORPAL)
                strcat(buf, " Vorpal");
        if (arg & ITEMA_TATTOO)
                strcat(buf, " Tattoo");
        if (arg & ITEMA_RIGHT_SILVER)
                strcat(buf, " Right Silver");
        if (arg & ITEMA_LEFT_SILVER)
                strcat(buf, " Left Silver");
        if (arg & ITEMA_RESISTANCE)
                strcat(buf, " Resistance");
        if (arg & ITEMA_VISION)
                strcat(buf, " Vision");
        if (arg & ITEMA_STALKER)
                strcat(buf, " Stalker");
        if (arg & ITEMA_VANISH)
                strcat(buf, " Vanish");
        if (arg & ITEMA_RAGER)
                strcat(buf, " Rager");
        return (buf[0] != '\0') ? buf + 1 : "none";
}


/*  
 * Pstat code by Tijer
 */
void do_pstat(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        char      buf[MAX_STRING_LENGTH];
        CHAR_DATA *victim;

        argument = one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                send_to_char("Pstat whom?\n\r", ch);
                return;
        }

        if ((victim = get_char_world(ch, arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }

        xprintf(buf, "Name : %s.\n\r",
                IS_NPC(victim) ? victim->short_descr : victim->name);
        send_to_char(buf, ch);
        xprintf(buf,
                "Sex : %s. Room : %d. Align : %d. Primal : %d. Bones : %d.\n\r",
                victim->sex == SEX_MALE ? "Male" : victim->sex ==
                SEX_FEMALE ? "Female" : "None",
                victim->in_room == NULL ? 0 : victim->in_room->vnum,
                victim->alignment, victim->practice,
                IS_NPC(victim) ? 0 : victim->bones);
        send_to_char(buf, ch);

        xprintf(buf, "Level : %d. Trust : %d. Exp : %ld.", victim->level,
                victim->trust, victim->exp);
        send_to_char(buf, ch);
        if (!IS_NPC(victim))
        {
                xprintf(buf, " PlayerID : %d.\n\r", victim->pcdata->playerid);
                send_to_char(buf, ch);
        }
        else
                send_to_char("\n\r", ch);

        xprintf(buf, "Hit : %d. Dam : %d. AC : %d. Position : %s\n\r",
                char_hitroll(victim),
                char_damroll(victim),
                char_ac(victim),
                capitalize(get_position_name(victim->position)));
        send_to_char(buf, ch);

        xprintf(buf, "HP %d/%d. Mana %d/%d. Move %d/%d.\n\r",
                victim->hit, victim->max_hit,
                victim->mana, victim->max_mana,
                victim->move, victim->max_move);
        send_to_char(buf, ch);

        xprintf(buf, "Str: %d.  Int: %d.  Wis: %d.  Dex: %d.  Con: %d.\n\r",
                get_curr_str(victim),
                get_curr_int(victim),
                get_curr_wis(victim),
                get_curr_dex(victim), get_curr_con(victim));
        send_to_char(buf, ch);

        xprintf(buf, "Rage : %d.", ch->rage);
        send_to_char(buf, ch);

        xprintf(buf, "Fighting : %s. (%d)\n\r",
                victim->fighting ? victim->fighting->name : "(None)",
                victim->fighting ? victim->fighting->level : 0);
        send_to_char(buf, ch);

        xprintf(buf, "Pkill : %d. Pdeath : %d. Mkill : %d. Mdeath : %d.\n\r",
                IS_NPC(victim) ? 0 : victim->pkill,
                IS_NPC(victim) ? 0 : victim->pdeath,
                IS_NPC(victim) ? 0 : victim->mkill,
                IS_NPC(victim) ? 0 : victim->mdeath);
        send_to_char(buf, ch);

        xprintf(buf,
                "TotExp  : %12d. TotMobLev  : %10d. TotQuestPoints : %10d.\n\r",
                IS_NPC(victim) ? 0 : victim->pcdata->score[SCORE_TOTAL_XP],
                IS_NPC(victim) ? 0 : victim->pcdata->score[SCORE_TOTAL_LEVEL],
                IS_NPC(victim) ? 0 : victim->pcdata->score[SCORE_QUEST]);
        send_to_char(buf, ch);

        xprintf(buf,
                "HighExp : %12d. HighMobLev : %10d. Tot#Quests     : %10d.\n\r",
                IS_NPC(victim) ? 0 : victim->pcdata->score[SCORE_HIGH_XP],
                IS_NPC(victim) ? 0 : victim->pcdata->score[SCORE_HIGH_LEVEL],
                IS_NPC(victim) ? 0 : victim->pcdata->score[SCORE_NUM_QUEST]);
        send_to_char(buf, ch);

        if (!IS_NPC(victim))
        {
                xprintf(buf, "Unarmed : %4d.", victim->wpn[0]);
                send_to_char(buf, ch);
                xprintf(buf, " Slice   : %4d.", victim->wpn[1]);
                send_to_char(buf, ch);
                xprintf(buf, " Stab    : %4d.", victim->wpn[2]);
                send_to_char(buf, ch);
                xprintf(buf, " Slash   : %4d.", victim->wpn[3]);
                send_to_char(buf, ch);
                xprintf(buf, " Whip    : %4d.\n\r", victim->wpn[4]);
                send_to_char(buf, ch);
                xprintf(buf, "Claw    : %4d.", victim->wpn[5]);
                send_to_char(buf, ch);
                xprintf(buf, " Blast   : %4d.", victim->wpn[6]);
                send_to_char(buf, ch);
                xprintf(buf, " Pound   : %4d.", victim->wpn[7]);
                send_to_char(buf, ch);
                xprintf(buf, " Crush   : %4d.", victim->wpn[8]);
                send_to_char(buf, ch);
                xprintf(buf, " Grep    : %4d.\n\r", victim->wpn[9]);
                send_to_char(buf, ch);
                xprintf(buf, "Bite    : %4d.", victim->wpn[10]);
                send_to_char(buf, ch);
                xprintf(buf, " Pierce  : %4d.", victim->wpn[11]);
                send_to_char(buf, ch);
                xprintf(buf, " Suck    : %4d.\n\r", victim->wpn[12]);
                send_to_char(buf, ch);

                xprintf(buf,
                        "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d.\n\r",
                        "Purple", victim->spl[PURPLE_MAGIC], "Red",
                        victim->spl[RED_MAGIC], "Blue",
                        victim->spl[BLUE_MAGIC], "Green",
                        victim->spl[GREEN_MAGIC], "Yellow",
                        victim->spl[YELLOW_MAGIC]);
                send_to_char(buf, ch);
                xprintf(buf,
                        "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d.\n\r",
                        "Viper", victim->stance[STANCE_VIPER], "Crane",
                        victim->stance[STANCE_CRANE], "Crab",
                        victim->stance[STANCE_CRAB], "Mongoose",
                        victim->stance[STANCE_MONGOOSE], "PitBull",
                        victim->stance[STANCE_PITBULL]);
                send_to_char(buf, ch);

                xprintf(buf,
                        "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %-3d. %-8s : %3d.\n\r",
                        "Hawk", victim->stance[STANCE_HAWK], "Badger",
                        victim->stance[STANCE_BADGER], "Cougar",
                        victim->stance[STANCE_COUGAR], "Wolverine",
                        victim->stance[STANCE_WOLVERINE], "Grizzly",
                        victim->stance[STANCE_GRIZZLY]);

                send_to_char(buf, ch);
                xprintf(buf,
                        "%-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d. %-8s : %3d.\n\r",
                        "ss1", victim->stance[STANCE_SS1], "ss2",
                        victim->stance[STANCE_SS2], "ss3",
                        victim->stance[STANCE_SS3], "pizza",
                        victim->stance[STANCE_SS4], "zarius",
                        victim->stance[STANCE_SS5]);
                send_to_char(buf, ch);


                xprintf(buf, "Act         : %s\n\r",
                        plr_bit_name(victim->act));
                send_to_char(buf, ch);
                xprintf(buf, "Act2        : %s\n\r",
                        plr2_bit_name(victim->act2));
                send_to_char(buf, ch);
                xprintf(buf, "Extra       : %s\n\r",
                        victim->extra <=
                        0 ? "(None)" : extra_plr_bit_name(victim->extra));
                send_to_char(buf, ch);
                xprintf(buf, "ItemAff     : %s\n\r",
                        victim->itemaffect <=
                        0 ? "(None)" : itemaffect_bit_name(victim->
                                                           itemaffect));
                send_to_char(buf, ch);

                xprintf(buf, "Affected by : %s.\n\r",
                        affect_bit_name(victim->affected_by));
                send_to_char(buf, ch);

                return;
        }
}

/* agrr_test by blade of E, version 1.31. */
void aggr_test(CHAR_DATA * ch)
{
        char      buf[60];
        CHAR_DATA *wch;
        CHAR_DATA *wch_next;
        CHAR_DATA *victim;



        if (!IS_NPC(ch) && ch->level < 7 && ch->in_room != NULL &&
            !IS_SET(ch->in_room->room_flags, ROOM_SAFE))
        {
                for (wch = ch->in_room->people; wch != NULL; wch = wch_next)
                {
                        wch_next = wch->next_in_room;

                        if (global_chaos)
                        {
                                if (IS_NPC(wch))
                                        SET_BIT(wch->act, ACT_AGGRESSIVE);
                        }

                        if (!IS_NPC(wch) || !IS_SET(wch->act, ACT_AGGRESSIVE)
                            || wch->fighting != NULL
                            || IS_AFFECTED(wch, AFF_CHARM) || !IS_AWAKE(wch)
                            || (IS_SET(wch->act, ACT_WIMPY) && IS_AWAKE(ch))
                            || !can_see(wch, ch) || number_bits(2) == 0)
                        {
                                continue;
                        }
                        victim = wch;
                        if (victim == NULL)
                                continue;
                        xprintf(buf, "%s screams and attacks!\n\r",
                                victim->name);
                        send_to_char(buf, ch);
                        multi_hit(victim, ch, TYPE_UNDEFINED);
                }
        }
        return;
}

