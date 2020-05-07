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
/* Cyborg Class by Zaphonite 
   Modified by Vladd
   Modified by Jobo for use as Spider Droids
 */
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"

void do_implant(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        char      buf[MAX_STRING_LENGTH];

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);

        if (IS_NPC(ch))
                return;

        if (!IS_CLASS(ch, CLASS_CYBORG))
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        if (arg1[0] == '\0' && arg2[0] == '\0')
        {
                xprintf(buf,
                        "#BCurrent implant levels#R: #wFace: #R%d#w, Legs: #R%d#w, Body: #R%d#w.#n\n\r\n\r",
                        UMIN(ch->pcdata->powers[CYBORG_FACE], 5),
                        UMIN(ch->pcdata->powers[CYBORG_LIMBS], 5),
                        UMIN(ch->pcdata->powers[CYBORG_BODY], 6));
                send_to_char(buf, ch);
                do_implant(ch, "face");
                do_implant(ch, "legs");
                do_implant(ch, "body");
                send_to_char
                        ("\n\r#BSyntax: #wimplant #0(#wplace#D) #wimprove.#n\n\r",
                         ch);
                return;
        }
        if (arg2[0] == '\0')
        {
                if (!str_cmp(arg1, "face"))
                {
                        send_to_char("#0Current Facial Implants:#n\n\r", ch);
                        if (ch->pcdata->powers[CYBORG_FACE] < 1)
                                send_to_char("None.\n\r", ch);
                        if (ch->pcdata->powers[CYBORG_FACE] > 0)
                        {
                                if (ch->pcdata->powers[CYBORG_FACE] > 3)
                                {
                                        send_to_char
                                                ("Your have bio-mechanical eyes blessed by Lloth.\n\r",
                                                 ch);
                                        send_to_char
                                                ("Your aura is shielded.\n\r",
                                                 ch);
                                        send_to_char
                                                ("Your senses are improved, allowing you to see the stats and location of others.\n\r",
                                                 ch);
                                        return;
                                }
                                else if ((ch->pcdata->powers[CYBORG_FACE] ==
                                          2))
                                        send_to_char
                                                ("You can see in the night and shadows.\n\r",
                                                 ch);
                                else if ((ch->pcdata->powers[CYBORG_FACE] ==
                                          1))
                                        send_to_char
                                                ("You can see in the night.\n\r",
                                                 ch);
                        }
                        return;
                }
                else if (!str_cmp(arg1, "legs"))
                {
                        send_to_char("#0Current Leg Implants:#n\n\r", ch);
                        if (ch->pcdata->powers[CYBORG_LIMBS] < 1)
                                send_to_char("#wNone#n.\n\r", ch);
                        if (ch->pcdata->powers[CYBORG_LIMBS] > 0)
                                send_to_char
                                        ("#wYour human legs have been replaced with stronger mechanical ones.#n\n\r",
                                         ch);
                        if (ch->pcdata->powers[CYBORG_LIMBS] > 1)
                                send_to_char
                                        ("#wDoctors have blessed you with superior speed.#n\n\r",
                                         ch);
                        if (ch->pcdata->powers[CYBORG_LIMBS] > 2)
                                send_to_char
                                        ("#wYour are one of futures champions.#n\n\r",
                                         ch);
                        if (ch->pcdata->powers[CYBORG_LIMBS] > 3)
                                send_to_char
                                        ("#wYou are a true cyborg warrior, you have supreme fighting skills.#n\n\r",
                                         ch);
                        if (ch->pcdata->powers[CYBORG_LIMBS] > 4)
                                send_to_char
                                        ("#wYour spit is now pure corrosive poison.#n\n\r",
                                         ch);
                        return;
                }
                else if (!str_cmp(arg1, "body"))
                {
                        send_to_char("#0Current Body Implants:#n\n\r", ch);
                        if (ch->pcdata->powers[CYBORG_BODY] < 1)
                                send_to_char("None.\n\r", ch);
                        if (ch->pcdata->powers[CYBORG_BODY] > 0)
                                send_to_char
                                        ("Your body's armor has been increased greatly.\n\r",
                                         ch);
                        if (ch->pcdata->powers[CYBORG_BODY] > 1)
                                send_to_char
                                        ("Your body can absorb certain attacks.\n\r",
                                         ch);
                        if (ch->pcdata->powers[CYBORG_BODY] > 2)
                                send_to_char
                                        ("You body slam your enemy when you have first attack.\n\r",
                                         ch);
                        if (ch->pcdata->powers[CYBORG_BODY] > 3)
                                send_to_char
                                        ("You have the power to cloak yourself and become invisible.\n\r",
                                         ch);
                        if (ch->pcdata->powers[CYBORG_BODY] > 4)
                                send_to_char
                                        ("You have poisoned stingers that shoot at your enemies from your body.\n\r",
                                         ch);
                        if (ch->pcdata->powers[CYBORG_BODY] > 5)
                                send_to_char("You regenerate very fast.\n\r",
                                             ch);
                        return;
                }
                xprintf(buf,
                        "#BCurrent implant levels#w: #wFace: #R%d#w, Legs: #R%d#w, Body: #R%d#w.#x\n\r\n\r",
                        UMIN(ch->pcdata->powers[CYBORG_FACE], 5),
                        UMIN(ch->pcdata->powers[CYBORG_LIMBS], 5),
                        UMIN(ch->pcdata->powers[CYBORG_BODY], 6));
                send_to_char(buf, ch);
                return;
        }
        if (!str_cmp(arg2, "improve"))
        {
                int       implant;
                int       cost;
                int       max;

                if (!str_cmp(arg1, "face"))
                {
                        implant = CYBORG_FACE;
                        max = 5;
                }
                else if (!str_cmp(arg1, "legs"))
                {
                        implant = CYBORG_LIMBS;
                        max = 5;
                }
                else if (!str_cmp(arg1, "body"))
                {
                        implant = CYBORG_BODY;
                        max = 6;
                }
                else
                {
                        send_to_char("Implants are face, legs, and body.\n\r",
                                     ch);
                        return;
                }
                if ((ch->pcdata->powers[implant] == 1))
                        cost = 25000;
                else if ((ch->pcdata->powers[implant] == 2))
                        cost = 50000;
                else if ((ch->pcdata->powers[implant] == 3))
                        cost = 100000;
                else if ((ch->pcdata->powers[implant] == 4))
                        cost = 200000;
                else if ((ch->pcdata->powers[implant] == 5))
                        cost = 400000;
                else if ((ch->pcdata->powers[implant] == 6))
                        cost = 800000;
                else
                        cost = 12500;
                arg1[0] = UPPER(arg1[0]);
                if ((ch->pcdata->powers[implant] >= max))
                {
                        xprintf(buf,
                                "You already have all implants on your %s.\n\r",
                                arg1);
                        send_to_char(buf, ch);
                        return;
                }
                if (!str_cmp(arg1, "face"))
                {
                        if ((ch->pcdata->powers[CYBORG_FACE] == 4 &&
                             ch->pcdata->powers[CYBORG_BODY] < 4))
                        {
                                send_to_char
                                        ("You need the 4th body implant first.\n\r",
                                         ch);
                                return;
                        }
                }

                if (!str_cmp(arg1, "body"))
                {
                        if (ch->pcdata->powers[CYBORG_LIMBS] < 1)
                        {
                                send_to_char
                                        ("You need at least 1 leg implant before you can get any body implants.\n\r",
                                         ch);
                                return;
                        }
                }

                if (cost > ch->pcdata->stats[CYBORG_POWER])
                {
                        xprintf(buf,
                                "Your bionic power must be greater than %d before you can improve your %s implants.\n\r",
                                cost, arg1);
                        send_to_char(buf, ch);
                        return;
                }

                ch->pcdata->powers[implant] += 1;
                ch->pcdata->stats[CYBORG_POWER] -= cost;
                xprintf(buf, "You add an implant to your %s.\n\r", arg1);
                send_to_char(buf, ch);
                return;
        }
        else
                send_to_char("Syntax: implant (place) improve.\n\r", ch);
        return;
}

void do_stuntubes(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;

        if (IS_NPC(ch))
                return;
        if (!IS_CLASS(ch, CLASS_CYBORG))
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        if (ch->pcdata->powers[CYBORG_BODY] < 5
            || ch->pcdata->powers[CYBORG_LIMBS] < 5)
        {
                send_to_char
                        ("You need level 5 implants on your legs and body to use this.\n\r",
                         ch);
                return;
        }

        if (ch->move < 1000)
        {
                stc("You need 1000 move to do that.\n\r", ch);
                return;
        }

        if ((victim = ch->fighting) == NULL)
        {
                send_to_char("You aren't fighting anyone.\n\r", ch);
                return;
        }
        WAIT_STATE(ch, 12);
        ch->move -= 1000;

        one_hit(ch, victim, gsn_stuntubes, 1);
        one_hit(ch, victim, gsn_stuntubes, 1);
        one_hit(ch, victim, gsn_stuntubes, 1);
        one_hit(ch, victim, gsn_stuntubes, 1);
        if (!IS_AFFECTED(victim, AFF_POISON))
                SET_BIT(victim->affected_by, AFF_POISON);
        if (!IS_AFFECTED(victim, AFF_FLAMING))
                SET_BIT(victim->affected_by, AFF_FLAMING);
        return;
}

void do_cubeform(CHAR_DATA * ch, char *argument)
{
        char      buf[MAX_STRING_LENGTH];

        if (IS_NPC(ch))
                return;

        if (!IS_CLASS(ch, CLASS_CYBORG))
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        if (ch->pcdata->powers[CYBORG_BODY] < 5)
        {
                send_to_char("You must have level 5 body.\n\r", ch);
                return;
        }

        if (IS_SET(ch->newbits, NEW_CUBEFORM))
        {
                REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
                REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
                act("$n returns to a normal man.", ch, NULL, NULL, TO_ROOM);
                send_to_char("You turn into your normal form.\n\r", ch);
                ch->damroll -= 250;
                ch->hitroll -= 250;
                free_string(ch->morph);
                ch->morph = str_dup("");
                return;
        }

        if (ch->move < 2000 || ch->mana < 2000)
        {
                send_to_char
                        ("You need 2000 mana and 2000 move to make yourself an avatar of Lloth.\n\r",
                         ch);
                return;
        }

        else
        {
                act("$n eyes glow with red light as a bloodlust fills them.",
                    ch, NULL, NULL, TO_ROOM);
                send_to_char("You become a true TERMINATOR!.\n\r", ch);
                SET_BIT(ch->newbits, NEW_CUBEFORM);
                SET_BIT(ch->affected_by, AFF_POLYMORPH);
                xprintf(buf, "%s the Terminator", ch->name);
                free_string(ch->morph);
                ch->morph = str_dup(buf);
                ch->move -= 2000;
                ch->mana -= 2000;
                ch->damroll += 250;
                ch->hitroll += 250;
                return;
        }
        return;
}


void do_infravision(CHAR_DATA * ch, char *argument)
{
        if (IS_NPC(ch))
                return;

        if (!IS_CLASS(ch, CLASS_CYBORG))
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        if (ch->pcdata->powers[CYBORG_FACE] < 1)
        {
                send_to_char
                        ("You need at least 1 implant on your face to use your unholy sight.\n\r",
                         ch);
                return;
        }
        if (ch->pcdata->powers[CYBORG_FACE] > 2)
        {
                if (IS_SET(ch->act, PLR_HOLYLIGHT))
                {
                        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
                        send_to_char("You turn off your unholy sight.\n\r",
                                     ch);
                }
                else
                {
                        SET_BIT(ch->act, PLR_HOLYLIGHT);
                        send_to_char("You turn on your unholy sight.\n\r",
                                     ch);
                }
        }
        else if (ch->pcdata->powers[CYBORG_FACE] == 2)
        {
                if (IS_SET(ch->affected_by, AFF_SHADOWSIGHT)
                    && IS_SET(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT))
                {
                        REMOVE_BIT(ch->affected_by, AFF_SHADOWSIGHT);
                        REMOVE_BIT(ch->pcdata->stats[UNI_AFF],
                                   VAM_NIGHTSIGHT);
                        send_to_char("You turn off your unholy sight.\n\r",
                                     ch);
                }
                else
                {
                        SET_BIT(ch->affected_by, AFF_SHADOWSIGHT);
                        SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
                        send_to_char("You turn on your unholy sight.\n\r",
                                     ch);
                }
        }
        else if ((ch->pcdata->powers[CYBORG_FACE] == 1))
        {
                if (IS_SET(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT))
                {
                        REMOVE_BIT(ch->pcdata->stats[UNI_AFF],
                                   VAM_NIGHTSIGHT);
                        send_to_char("You turn off your unholy sight.\n\r",
                                     ch);
                }
                else
                {
                        SET_BIT(ch->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
                        send_to_char("You turn on your unholy sight.\n\r",
                                     ch);
                }
        }
        return;
}

void do_cyborgeq(CHAR_DATA * ch, char *argument)
{
        OBJ_INDEX_DATA *pObjIndex;
        OBJ_DATA *obj;
        char      arg[MAX_INPUT_LENGTH];
        int       vnum = 0;

        argument = one_argument(argument, arg);


        if (IS_NPC(ch))
                return;

        if (!IS_CLASS(ch, CLASS_CYBORG))
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        if (arg[0] == '\0')
        {
                send_to_char
                        ("Please specify which piece of cyborg armor you wish to make: whip ring "
                         "collar armor helmet leggings boots gloves sleeves cloak belt bracer mask.\n\r",
                         ch);
                return;
        }

        if (!str_cmp(arg, "whip"))
                vnum = 33140;
        else if (!str_cmp(arg, "ring"))
                vnum = 33141;
        else if (!str_cmp(arg, "collar"))
                vnum = 33142;
        else if (!str_cmp(arg, "armor"))
                vnum = 33143;
        else if (!str_cmp(arg, "helmet"))
                vnum = 33144;
        else if (!str_cmp(arg, "leggings"))
                vnum = 33145;
        else if (!str_cmp(arg, "boots"))
                vnum = 33146;
        else if (!str_cmp(arg, "gloves"))
                vnum = 33147;
        else if (!str_cmp(arg, "sleeves"))
                vnum = 33148;
        else if (!str_cmp(arg, "cloak"))
                vnum = 33149;
        else if (!str_cmp(arg, "belt"))
                vnum = 33150;
        else if (!str_cmp(arg, "bracer"))
                vnum = 33151;
        else if (!str_cmp(arg, "mask"))
                vnum = 33152;
        else
        {
                do_cyborgeq(ch, "");
                return;
        }
        if (ch->practice < 100)
        {
                send_to_char
                        ("It costs 100 primal to create a piece of cyborg armour.\n\r",
                         ch);
                return;
        }
        if (vnum == 0 || (pObjIndex = get_obj_index(vnum)) == NULL)
        {
                send_to_char("Missing object, please inform Dracknuur.\n\r",
                             ch);
                return;
        }
        ch->practice -= 100;
        obj = create_object(pObjIndex, 50);
        obj->questowner = str_dup(ch->pcdata->switchname);
        obj->ownerid = ch->pcdata->playerid;
        obj_to_char(obj, ch);
        act("$p appears in your hands in a blast of flames.", ch, obj, NULL,
            TO_CHAR);
        act("$p appears in $n's hands in a blast of flames.", ch, obj, NULL,
            TO_ROOM);
        return;
}
