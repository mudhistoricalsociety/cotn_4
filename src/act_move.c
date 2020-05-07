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
#include "merc.h"
#include "interp.h"


void horn args((CHAR_DATA * ch));

char     *const dir_name[] = {
        "north", "east", "south", "west", "up", "down"
};

const sh_int rev_dir[] = {
        2, 3, 0, 1, 5, 4
};

const sh_int movement_loss[SECT_MAX] = {
        1, 2, 2, 3, 4, 6, 4, 1, 6, 10, 6
};

/*
 * Local functions.
 */
int find_door args((CHAR_DATA * ch, char *arg));
bool has_key args((CHAR_DATA * ch, int key));
int count_imms args((CHAR_DATA * ch));
bool check_track args((CHAR_DATA * ch, int direction));
void add_tracks args((CHAR_DATA * ch, int direction));
void drow_hate args((CHAR_DATA * ch));


void move_char(CHAR_DATA * ch, int door)
{
        CHAR_DATA *fch;
        CHAR_DATA *fch_next;
        CHAR_DATA *mount;
        ROOM_INDEX_DATA *in_room;
        ROOM_INDEX_DATA *to_room;
        OBJ_DATA *obj;
        EXIT_DATA *pexit;
        DESCRIPTOR_DATA *d;
        char      buf[MAX_STRING_LENGTH];
        char      poly[MAX_STRING_LENGTH];
        char      mount2[MAX_INPUT_LENGTH];
        char      leave[20];
        int       revdoor;
        bool      bad_wall = FALSE;

        if (door < 0 || door > 5)

        {
                bug("Do_move: bad door %d.", door);
                return;
        }
        /*
         * Exit trigger, if activated, bail out. Only PCs are triggered.
         */
        if (!IS_NPC(ch)
            && (p_exit_trigger(ch, door, PRG_MPROG)
                || p_exit_trigger(ch, door, PRG_OPROG)
                || p_exit_trigger(ch, door, PRG_RPROG)))
                return;

        in_room = ch->in_room;

        if (!in_room)
        {
                bug("Move_char : ch not in any room.", 0);
                return;
        }

        if ((pexit = in_room->exit[door]) == NULL
            || (to_room = pexit->to_room) == NULL)
        {
                send_to_char("Alas, you cannot go that way.\n\r", ch);
                return;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROID)
            && IS_SET(ch->newbits, NEW_CUBEFORM))
        {
                if (!IS_SET
                    (ch->pcdata->powers[DROID_IMPLANTS], IMPLANT_LARVAFEET)
                    || ch->fighting)
                {
                        send_to_char
                                ("Battlestations don't go anywhere, they stand still and fight.\n\r",
                                 ch);
                        return;
                }
                WAIT_STATE(ch, 4);
                send_to_char("Chrss..chrss...chrrrr...shh...\n\r", ch);
        }

        if (door == DIR_NORTH
            && ((obj = get_obj_list(ch, "walln", ch->in_room->contents)) !=
                NULL
                || (obj =
                    get_obj_list(ch, "walls", to_room->contents)) != NULL))
                bad_wall = TRUE;

        if (door == DIR_SOUTH
            && ((obj = get_obj_list(ch, "walls", ch->in_room->contents)) !=
                NULL
                || (obj =
                    get_obj_list(ch, "walln", to_room->contents)) != NULL))
                bad_wall = TRUE;

        if (door == DIR_EAST
            && ((obj = get_obj_list(ch, "walle", ch->in_room->contents)) !=
                NULL
                || (obj =
                    get_obj_list(ch, "wallw", to_room->contents)) != NULL))
                bad_wall = TRUE;

        if (door == DIR_WEST
            && ((obj = get_obj_list(ch, "wallw", ch->in_room->contents)) !=
                NULL
                || (obj =
                    get_obj_list(ch, "walle", to_room->contents)) != NULL))
                bad_wall = TRUE;

        if (door == DIR_UP
            && ((obj = get_obj_list(ch, "wallu", ch->in_room->contents)) !=
                NULL
                || (obj =
                    get_obj_list(ch, "walld", to_room->contents)) != NULL))
                bad_wall = TRUE;

        if (door == DIR_DOWN
            && ((obj = get_obj_list(ch, "walld", ch->in_room->contents)) !=
                NULL
                || (obj =
                    get_obj_list(ch, "wallu", to_room->contents)) != NULL))
                bad_wall = TRUE;
        if (bad_wall)
        {
                send_to_char("You are unable to pass the wall.\n\r", ch);
                return;
        }

        if (IS_SET(pexit->exit_info, EX_CLOSED)
            && (!IS_AFFECTED(ch, AFF_PASS_DOOR)
                || IS_SET(pexit->exit_info, EX_NOPASS))
            && !IS_AFFECTED(ch, AFF_ETHEREAL) && !IS_IMMORTAL(ch)
            && !IS_AFFECTED(ch, AFF_SHADOWPLANE))

        {
                if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) &&
                    ch->power[DISC_WERE_BOAR] > 0
                    && !IS_SET(pexit->exit_info, EX_PRISMATIC_WALL))
                {
                        act("You smash open the $d.", ch, NULL,
                            pexit->keyword, TO_CHAR);
                        act("$n smashes open the $d.", ch, NULL,
                            pexit->keyword, TO_ROOM);
                        REMOVE_BIT(pexit->exit_info, EX_CLOSED);
                }
                else
                {
                        act("The $d is closed.", ch, NULL, pexit->keyword,
                            TO_CHAR);
                        return;
                }
        }

        if (IS_SET(pexit->exit_info, EX_PRISMATIC_WALL)
            && IS_SET(pexit->exit_info, EX_CLOSED))
        {
                stc("The prismatic wall prevents movement through this closed exit.\n\r", ch);
                return;
        }

        if (IS_AFFECTED(ch, AFF_CHARM)
            && ch->master != NULL && in_room == ch->master->in_room)
        {
                send_to_char("What?  And leave your beloved master?\n\r", ch);
                return;
        }

        if (IS_NPC(ch) && (mount = ch->mount) != NULL
            && IS_SET(ch->mounted, IS_MOUNT))
        {
                send_to_char
                        ("You better wait for instructions from your rider.\n\r",
                         ch);
                return;
        }

        if (room_is_private(to_room))
        {
                if (IS_NPC(ch) || ch->trust < MAX_LEVEL)
                {
                        send_to_char("That room is private right now.\n\r",
                                     ch);
                        return;
                }
                else
                        send_to_char
                                ("That room is private (Access granted).\n\r",
                                 ch);
        }

        if ((IS_LEG_L(ch, BROKEN_LEG) || IS_LEG_L(ch, LOST_LEG)) &&
            (IS_LEG_R(ch, BROKEN_LEG) || IS_LEG_R(ch, LOST_LEG)) &&
            (IS_ARM_L(ch, BROKEN_ARM) || IS_ARM_L(ch, LOST_ARM) ||
             get_eq_char(ch, WEAR_HOLD) != NULL) &&
            (IS_ARM_R(ch, BROKEN_ARM) || IS_ARM_R(ch, LOST_ARM) ||
             get_eq_char(ch, WEAR_WIELD) != NULL))
        {
                send_to_char
                        ("You need at least one free arm to drag yourself with.\n\r",
                         ch);
                return;
        }
        else if (IS_BODY(ch, BROKEN_SPINE) &&
                 (IS_ARM_L(ch, BROKEN_ARM) || IS_ARM_L(ch, LOST_ARM) ||
                  get_eq_char(ch, WEAR_HOLD) != NULL) &&
                 (IS_ARM_R(ch, BROKEN_ARM) || IS_ARM_R(ch, LOST_ARM) ||
                  get_eq_char(ch, WEAR_WIELD) != NULL))
        {
                send_to_char("You cannot move with a broken spine.\n\r", ch);
                return;
        }

        if (!IS_NPC(ch))
        {
                int       move;

                if (in_room->sector_type == SECT_AIR
                    || to_room->sector_type == SECT_AIR)
                {
                        if (!IS_AFFECTED(ch, AFF_FLYING) &&
                            (!IS_NPC(ch) && (!IS_CLASS(ch, CLASS_DROW) ||
                                             !IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION)))
                            && (!IS_NPC(ch) && !IS_VAMPAFF(ch, VAM_FLYING)
                                && (!IS_NPC(ch)
                                    && !IS_DEMAFF(ch, DEM_UNFOLDED)))
                            && !((mount = ch->mount) != NULL
                                 && IS_SET(ch->mounted, IS_RIDING)
                                 && IS_AFFECTED(mount, AFF_FLYING)))
                        {
                                send_to_char("You can't fly.\n\r", ch);
                                return;
                        }
                }

                if (in_room->sector_type == SECT_WATER_NOSWIM
                    || to_room->sector_type == SECT_WATER_NOSWIM)
                {
                        //OBJ_DATA *obj;
                        bool      found;

                        /*
                         * Look for a boat.
                         */
                        found = FALSE;
                        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
                        {
                                if (IS_VAMPAFF(ch, VAM_FLYING))
                                        found = TRUE;
                                else if (IS_POLYAFF(ch, POLY_SERPENT))
                                        found = TRUE;
                                else if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
                                        found = TRUE;
                                else if ((mount = ch->mount) != NULL
                                         && IS_SET(ch->mounted, IS_RIDING)
                                         && IS_AFFECTED(mount, AFF_FLYING))
                                        found = TRUE;
                                else
                                {
                                        send_to_char
                                                ("You are unable to cross running water.\n\r",
                                                 ch);
                                        return;
                                }
                        }
                        if (IS_AFFECTED(ch, AFF_FLYING))
                                found = TRUE;
                        else if (!IS_NPC(ch) && IS_DEMAFF(ch, DEM_UNFOLDED))
                                found = TRUE;
                        else if (!IS_NPC(ch) && (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_LEVITATION)))
                                found = TRUE;
                        if (!found)
                        {
                                for (obj = ch->carrying; obj != NULL;
                                     obj = obj->next_content)
                                {
                                        if (obj->item_type == ITEM_BOAT)
                                        {
                                                found = TRUE;
                                                break;
                                        }
                                }
                                if (!found)
                                {
                                        send_to_char
                                                ("You need a boat to go there.\n\r",
                                                 ch);
                                        return;
                                }
                        }
                }
                else if (!IS_AFFECTED(ch, AFF_FLYING)
                         && IS_POLYAFF(ch, POLY_FISH))
                {
                        bool      from_ok = FALSE;
                        bool      to_ok = FALSE;

                        if (in_room->sector_type == SECT_WATER_NOSWIM)
                                from_ok = TRUE;
                        if (in_room->sector_type == SECT_WATER_SWIM)
                                from_ok = TRUE;
                        if (to_room->sector_type == SECT_WATER_NOSWIM)
                                to_ok = TRUE;
                        if (to_room->sector_type == SECT_WATER_SWIM)
                                to_ok = TRUE;
                        if (!from_ok || !to_ok)
                        {
                                send_to_char("You cannot cross land.\n\r",
                                             ch);
                                return;
                        }
                }

                move = movement_loss[UMIN(SECT_MAX - 1, in_room->sector_type)]
                        +
                        movement_loss[UMIN
                                      (SECT_MAX - 1, to_room->sector_type)];

                if (IS_HERO(ch))
                        move = 0;

                if (ch->move <= 0)
                {
                        send_to_char("You are too Exhausted.\n\r", ch);
                        return;
                }

                if (IS_SET(ch->mounted, IS_RIDING)
                    && (ch->move < move || ch->move < 1))
                {
                        send_to_char("You are too exhausted.\n\r", ch);
                        return;
                }

                if (IS_SET(pexit->exit_info, EX_IRON_WALL)
                    && !IS_AFFECTED(ch, AFF_PASS_DOOR))
                {
                        send_to_char
                                ("A towering wall of iron blocks your path.\n\r",
                                 ch);
                        act("$n's path is blocked by the wall of iron.", ch,
                            NULL, NULL, TO_ROOM);
                        return;
                }

                if (IS_SET(pexit->exit_info, EX_MUSHROOM_WALL))
                {
                        send_to_char("The mushrooms block your path.\n\r",
                                     ch);
                        return;
                }

                if (IS_SET(pexit->exit_info, EX_ICE_WALL)
                    && ch->power[DISC_DAEM_GELU] < 5)
                {
                        send_to_char
                                ("A huge wall of ice blocks your way.\n\r",
                                 ch);
                        return;
                }

                if (IS_SET(pexit->exit_info, EX_FIRE_WALL) && ch->class == 0)
                {
                        act("$n bursts through the wall of fire.", ch, NULL,
                            NULL, TO_ROOM);
                        send_to_char
                                ("You jump through the flames and are unaffected.\n\r",
                                 ch);
                }

                else if (IS_SET(pexit->exit_info, EX_FIRE_WALL)
                         && ch->class != 0 && !IS_NPC(ch) && ch->level > 2)
                {
                        act("$n bursts through the wall of fire.", ch, NULL,
                            NULL, TO_ROOM);
                        send_to_char("You jump through the flames.\n\r", ch);
                        ch->hit -= dice(6, 50);
                        stc("The flames sear your flesh.\n\r", ch);
                }

                if (IS_SET(pexit->exit_info, EX_SWORD_WALL) && ch->class == 0)
                {
                        act("$n bursts through the wall of swords.", ch, NULL,
                            NULL, TO_ROOM);
                        send_to_char
                                ("You jump through the swords and are unaffected.\n\r",
                                 ch);
                }

                else if (IS_SET(pexit->exit_info, EX_SWORD_WALL)
                         && ch->class != 0 && !IS_NPC(ch) && ch->level > 2)
                {
                        act("$n jumps through the wall of swords.", ch, NULL,
                            NULL, TO_ROOM);
                        send_to_char("You jump through the swords.\n\r", ch);
                        send_to_char("Aaaaaaaaarghhhhhhh! That hurt!\n\r",
                                     ch);
                        ch->hit -= dice(6, 70);
                }

                if (IS_SET(pexit->exit_info, EX_ASH_WALL))
                {
                        send_to_char
                                ("You scream in agony as the wall of ash rips apart your life force.\n\r",
                                 ch);
                        act("$n screams in agony as the wall of ash rips $s life force apart.", ch, NULL, NULL, TO_ROOM);
                        ch->hit /= 2;
                        ch->move /= 2;
                }
	if(IS_SET(pexit->exit_info, EX_SHIN_WALL) && !IS_CLASS(ch,CLASS_SHINOBI)){
		stc("The shurikens tear through your flesh!\n\r",ch);
		act("$n's flesh is torn by the shuriken wall!",ch,NULL,NULL,TO_ROOM);
		ch->hit *= .9;
	}
                WAIT_STATE(ch, 1);
                if (!IS_SET(ch->mounted, IS_RIDING))
                        ch->move -= move;
        }

        /*
         * Check for mount message - KaVir 
         */
        if ((mount = ch->mount) != NULL && ch->mounted == IS_RIDING)
        {
                if (IS_NPC(mount))
                        xprintf(mount2, " on %s.", mount->short_descr);
                else
                        xprintf(mount2, " on %s.", mount->name);
        }
        else
                xprintf(mount2, ".");

        if (IS_HEAD(ch, LOST_HEAD) || IS_EXTRA(ch, EXTRA_OSWITCH))
                xprintf(leave, "rolls");
        else if (IS_AFFECTED(ch, AFF_ETHEREAL))
                xprintf(leave, "floats");
        else if (ch->in_room->sector_type == SECT_WATER_SWIM)
                xprintf(leave, "swims");
        else if (IS_SET(ch->polyaff, POLY_SERPENT))
                xprintf(leave, "slithers");
        else if (IS_SET(ch->polyaff, POLY_WOLF))
                xprintf(leave, "stalks");
        else if (IS_SET(ch->polyaff, POLY_FROG))
                xprintf(leave, "hops");
        else if (IS_SET(ch->polyaff, POLY_FISH))
                xprintf(leave, "swims");
        else if (!IS_NPC(ch) && IS_DEMAFF(ch, DEM_UNFOLDED))
                xprintf(leave, "flies");
        else if (IS_BODY(ch, BROKEN_SPINE))
                xprintf(leave, "drags $mself");
        else if (IS_LEG_L(ch, LOST_LEG) && IS_LEG_R(ch, LOST_LEG))
                xprintf(leave, "drags $mself");
        else if ((IS_LEG_L(ch, BROKEN_LEG) || IS_LEG_L(ch, LOST_LEG)
                  || IS_LEG_L(ch, LOST_FOOT)) && (IS_LEG_R(ch, BROKEN_LEG)
                                                  || IS_LEG_R(ch, LOST_LEG)
                                                  || IS_LEG_R(ch, LOST_FOOT)))
                xprintf(leave, "crawls");
        else if (ch->hit < (ch->max_hit / 4))
                xprintf(leave, "crawls");
        else if ((IS_LEG_R(ch, LOST_LEG) || IS_LEG_R(ch, LOST_FOOT)) &&
                 (!IS_LEG_L(ch, BROKEN_LEG) && !IS_LEG_L(ch, LOST_LEG) &&
                  !IS_LEG_L(ch, LOST_FOOT)))
                xprintf(leave, "hops");
        else if ((IS_LEG_L(ch, LOST_LEG) || IS_LEG_L(ch, LOST_FOOT)) &&
                 (!IS_LEG_R(ch, BROKEN_LEG) && !IS_LEG_R(ch, LOST_LEG) &&
                  !IS_LEG_R(ch, LOST_FOOT)))
                xprintf(leave, "hops");
        else if ((IS_LEG_L(ch, BROKEN_LEG) || IS_LEG_L(ch, LOST_FOOT)) &&
                 (!IS_LEG_R(ch, BROKEN_LEG) && !IS_LEG_R(ch, LOST_LEG) &&
                  !IS_LEG_R(ch, LOST_FOOT)))
                xprintf(leave, "limps");
        else if ((IS_LEG_R(ch, BROKEN_LEG) || IS_LEG_R(ch, LOST_FOOT)) &&
                 (!IS_LEG_L(ch, BROKEN_LEG) && !IS_LEG_L(ch, LOST_LEG) &&
                  !IS_LEG_L(ch, LOST_FOOT)))
                xprintf(leave, "limps");
        else if (ch->hit < (ch->max_hit / 3))
                xprintf(leave, "limps");
        else if (ch->hit < (ch->max_hit / 2))
                xprintf(leave, "staggers");
        else if (!IS_NPC(ch))
        {

                if (ch->pcdata->condition[COND_DRUNK] > 10)
                        xprintf(leave, "staggers");
                else
                        xprintf(leave, "walks");
        }
        else
                xprintf(leave, "walks");

        if (!IS_NPC(ch) && ch->stance[0] != -1)
                do_stance(ch, "");
        for (d = descriptor_list; d != NULL; d = d->next)
        {
                CHAR_DATA *victim;

                if ((victim = d->character) == NULL)
                        continue;

                if (ch->in_room == NULL || victim->in_room == NULL)
                        continue;

                if (ch == victim || ch->in_room != victim->in_room)
                        continue;

                if (d->connected != CON_PLAYING || !can_see(ch, victim))
                        continue;

                if (!IS_NPC(ch) && !IS_AFFECTED(ch, AFF_SNEAK)
                    && IS_AFFECTED(ch, AFF_POLYMORPH) && (IS_NPC(ch)
                                                          || !IS_SET(ch->act,
                                                                     PLR_WIZINVIS))
                    && can_see(victim, ch))
                {
                        if (((mount = ch->mount) != NULL
                             && ch->mounted == IS_RIDING
                             && IS_AFFECTED(mount, AFF_FLYING))
                            || IS_AFFECTED(ch, AFF_FLYING) || (!IS_NPC(ch)
                                                               &&
                                                               IS_VAMPAFF(ch,
                                                                          VAM_FLYING)))
                                xprintf(poly, "%s flies $T%s", ch->morph,
                                        mount2);
                        else if ((mount = ch->mount) != NULL
                                 && ch->mounted == IS_RIDING)
                                xprintf(poly, "%s rides $T%s", ch->morph,
                                        mount2);
                        else
                                xprintf(poly, "%s %s $T%s", ch->morph, leave,
                                        mount2);
                        act(poly, victim, NULL, dir_name[door], TO_CHAR);
                }
                else if (!IS_AFFECTED(ch, AFF_SNEAK)
                         && (IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS))
                         && can_see(victim, ch))
                {
                        if (((mount = ch->mount) != NULL
                             && ch->mounted == IS_RIDING
                             && IS_AFFECTED(mount, AFF_FLYING))
                            || IS_AFFECTED(ch, AFF_FLYING) || (!IS_NPC(ch)
                                                               &&
                                                               IS_VAMPAFF(ch,
                                                                          VAM_FLYING)))
                                xprintf(poly, "$n flies %s%s", dir_name[door],
                                        mount2);
                        else if ((mount = ch->mount) != NULL
                                 && ch->mounted == IS_RIDING)
                                xprintf(poly, "$n rides %s%s", dir_name[door],
                                        mount2);
                        else
                                xprintf(poly, "$n %s %s%s", leave,
                                        dir_name[door], mount2);
                        act(poly, ch, NULL, victim, TO_VICT);
                }
        }
        char_from_room(ch);
        char_to_room(ch, to_room);
        if (door == 0)
        {
                revdoor = 2;
                xprintf(buf, "the south");
        }
        else if (door == 1)
        {
                revdoor = 3;
                xprintf(buf, "the west");
        }
        else if (door == 2)
        {
                revdoor = 0;
                xprintf(buf, "the north");
        }
        else if (door == 3)
        {
                revdoor = 1;
                xprintf(buf, "the east");
        }
        else if (door == 4)
        {
                revdoor = 5;
                xprintf(buf, "below");
        }
        else
        {
                revdoor = 4;
                xprintf(buf, "above");
        }

        for (d = descriptor_list; d != NULL; d = d->next)
        {
                CHAR_DATA *victim;

                if ((victim = d->character) == NULL)
                        continue;

                if (ch->in_room == NULL || victim->in_room == NULL)
                        continue;

                if (ch == victim || ch->in_room != victim->in_room)
                        continue;

                if (d->connected != CON_PLAYING || !can_see(ch, victim))
                        continue;

                if (!IS_NPC(ch) && !IS_AFFECTED(ch, AFF_SNEAK)
                    && IS_AFFECTED(ch, AFF_POLYMORPH) && (IS_NPC(ch)
                                                          || !IS_SET(ch->act,
                                                                     PLR_WIZINVIS))
                    && can_see(victim, ch))
                {
                        if (((mount = ch->mount) != NULL
                             && ch->mounted == IS_RIDING
                             && IS_AFFECTED(mount, AFF_FLYING))
                            || IS_AFFECTED(ch, AFF_FLYING) || (!IS_NPC(ch)
                                                               &&
                                                               IS_VAMPAFF(ch,
                                                                          VAM_FLYING)))
                                xprintf(poly, "%s flies in from %s%s",
                                        ch->morph, buf, mount2);
                        else if ((mount = ch->mount) != NULL
                                 && ch->mounted == IS_RIDING)
                                xprintf(poly, "%s rides in from %s%s",
                                        ch->morph, buf, mount2);
                        else
                                xprintf(poly, "%s %s in from %s%s", ch->morph,
                                        leave, buf, mount2);
                        act(poly, ch, NULL, victim, TO_VICT);
                }
                else if (!IS_AFFECTED(ch, AFF_SNEAK) && can_see(victim, ch)
                         && (IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS)))
                {
                        if (((mount = ch->mount) != NULL
                             && ch->mounted == IS_RIDING
                             && IS_AFFECTED(mount, AFF_FLYING))
                            || IS_AFFECTED(ch, AFF_FLYING) || (!IS_NPC(ch)
                                                               &&
                                                               IS_VAMPAFF(ch,
                                                                          VAM_FLYING)))
                                xprintf(poly, "$n flies in from %s%s", buf,
                                        mount2);
                        else if ((mount = ch->mount) != NULL
                                 && ch->mounted == IS_RIDING)
                                xprintf(poly, "$n rides in from %s%s", buf,
                                        mount2);
                        else
                                xprintf(poly, "$n %s in from %s%s", leave,
                                        buf, mount2);
                        act(poly, ch, NULL, victim, TO_VICT);
                }
        }

        do_look(ch, "auto");

        for (fch = in_room->people; fch != NULL; fch = fch_next)
        {
                fch_next = fch->next_in_room;
                if ((mount = fch->mount) != NULL && mount == ch
                    && IS_SET(fch->mounted, IS_MOUNT))
                {
                        act("$N digs $S heels into you.", fch, NULL, ch,
                            TO_CHAR);
                        char_from_room(fch);
                        char_to_room(fch, ch->in_room);
                }

                if (fch->master == ch && fch->position == POS_STANDING
                    && fch->in_room != ch->in_room)
                {
                        act("You follow $N.", fch, NULL, ch, TO_CHAR);
                        move_char(fch, door);
                }
        }
        /*
         * If someone is following the char, these triggers get activated
         * for the followers before the char, but it's safer this way...
         */
        if (IS_NPC(ch) && HAS_TRIGGER_MOB(ch, TRIG_ENTRY))
                p_percent_trigger(ch, NULL, NULL, NULL, NULL, NULL,
                                  TRIG_ENTRY);
        if (!IS_NPC(ch))
        {
                p_greet_trigger(ch, PRG_MPROG);
                p_greet_trigger(ch, PRG_OPROG);
                p_greet_trigger(ch, PRG_RPROG);
        }
        room_text(ch, ">enter<");
        return;
}

void do_north(CHAR_DATA * ch, char *argument)
{
        ROOM_INDEX_DATA *in_room;

        if (IS_AFFECTED(ch, AFF_WEBBED))
        {
                send_to_char
                        ("You are unable to move with all this sticky webbing on.\n\r",
                         ch);
                return;
        }
        if (IS_AFFECTED2(ch, AFF2_THORNS))
        {
                send_to_char
                        ("Not while being rooted to the ground by brambles and thorns.\n\r",
                         ch);
                return;
        }

        in_room = ch->in_room;
        move_char(ch, DIR_NORTH);
        if (!IS_NPC(ch) && ch->in_room != in_room)
        {
                ROOM_INDEX_DATA *old_room;

                old_room = ch->in_room;
                char_from_room(ch);
                char_to_room(ch, in_room);
                add_tracks(ch, DIR_NORTH);
                char_from_room(ch);
                char_to_room(ch, old_room);
        }
        drow_hate(ch);

        return;
}

void do_east(CHAR_DATA * ch, char *argument)
{
        ROOM_INDEX_DATA *in_room;

        if (IS_AFFECTED(ch, AFF_WEBBED))
        {
                send_to_char
                        ("You are unable to move with all this sticky webbing on.\n\r",
                         ch);
                return;
        }
        if (IS_AFFECTED2(ch, AFF2_THORNS))
        {
                send_to_char
                        ("Not while being rooted to the ground by brambles and thorns.\n\r",
                         ch);
                return;
        }


        in_room = ch->in_room;
        move_char(ch, DIR_EAST);
        if (!IS_NPC(ch) && ch->in_room != in_room)
        {
                ROOM_INDEX_DATA *old_room;

                old_room = ch->in_room;
                char_from_room(ch);
                char_to_room(ch, in_room);
                add_tracks(ch, DIR_EAST);
                char_from_room(ch);
                char_to_room(ch, old_room);
        }
        drow_hate(ch);
        return;
}

void do_south(CHAR_DATA * ch, char *argument)
{

        ROOM_INDEX_DATA *in_room;

        if (IS_AFFECTED(ch, AFF_WEBBED))
        {
                send_to_char
                        ("You are unable to move with all this sticky webbing on.\n\r",
                         ch);
                return;
        }
        if (IS_AFFECTED2(ch, AFF2_THORNS))
        {
                send_to_char
                        ("Not while being rooted to the ground by brambles and thorns.\n\r",
                         ch);
                return;
        }


        in_room = ch->in_room;
        move_char(ch, DIR_SOUTH);
        if (!IS_NPC(ch) && ch->in_room != in_room)
        {
                ROOM_INDEX_DATA *old_room;

                old_room = ch->in_room;
                char_from_room(ch);
                char_to_room(ch, in_room);
                add_tracks(ch, DIR_SOUTH);
                char_from_room(ch);
                char_to_room(ch, old_room);
        }
        drow_hate(ch);
        return;
}

void do_west(CHAR_DATA * ch, char *argument)
{
        ROOM_INDEX_DATA *in_room;

        if (IS_AFFECTED(ch, AFF_WEBBED))
        {
                send_to_char
                        ("You are unable to move with all this sticky webbing on.\n\r",
                         ch);
                return;
        }
        if (IS_AFFECTED2(ch, AFF2_THORNS))
        {
                send_to_char
                        ("Not while being rooted to the ground by brambles and thorns.\n\r",
                         ch);
                return;
        }



        in_room = ch->in_room;
        move_char(ch, DIR_WEST);
        if (!IS_NPC(ch) && ch->in_room != in_room)
        {
                ROOM_INDEX_DATA *old_room;

                old_room = ch->in_room;
                char_from_room(ch);
                char_to_room(ch, in_room);
                add_tracks(ch, DIR_WEST);
                char_from_room(ch);
                char_to_room(ch, old_room);
        }
        drow_hate(ch);
        return;
}

void do_up(CHAR_DATA * ch, char *argument)
{
        ROOM_INDEX_DATA *in_room;

        if (IS_AFFECTED(ch, AFF_WEBBED))
        {
                send_to_char
                        ("You are unable to move with all this sticky webbing on.\n\r",
                         ch);
                return;
        }
        if (IS_AFFECTED2(ch, AFF2_THORNS))
        {
                send_to_char
                        ("Not while being rooted to the ground by brambles and thorns.\n\r",
                         ch);
                return;
        }


        in_room = ch->in_room;
        move_char(ch, DIR_UP);
        if (!IS_NPC(ch) && ch->in_room != in_room)
        {
                ROOM_INDEX_DATA *old_room;

                old_room = ch->in_room;
                char_from_room(ch);
                char_to_room(ch, in_room);
                add_tracks(ch, DIR_UP);
                char_from_room(ch);
                char_to_room(ch, old_room);
        }
/*
    if (!IS_NPC(ch) && ch->in_room != in_room) add_tracks( ch, DIR_DOWN );
*/
        drow_hate(ch);
        return;
}

void do_down(CHAR_DATA * ch, char *argument)
{
        ROOM_INDEX_DATA *in_room;

        if (IS_AFFECTED(ch, AFF_WEBBED))
        {
                send_to_char
                        ("You are unable to move with all this sticky webbing on.\n\r",
                         ch);
                return;
        }
        if (IS_AFFECTED2(ch, AFF2_THORNS))
        {
                send_to_char
                        ("Not while being rooted to the ground by brambles and thorns.\n\r",
                         ch);
                return;
        }


        in_room = ch->in_room;
        move_char(ch, DIR_DOWN);
        if (!IS_NPC(ch) && ch->in_room != in_room)
        {
                ROOM_INDEX_DATA *old_room;

                old_room = ch->in_room;
                char_from_room(ch);
                char_to_room(ch, in_room);
                add_tracks(ch, DIR_DOWN);
                char_from_room(ch);
                char_to_room(ch, old_room);
        }
/*
    if (!IS_NPC(ch) && ch->in_room != in_room) add_tracks( ch, DIR_UP );
*/
        drow_hate(ch);
        return;
}

int find_door(CHAR_DATA * ch, char *arg)
{
        EXIT_DATA *pexit;
        int       door;

        if (!str_cmp(arg, "n") || !str_cmp(arg, "north"))
                door = 0;
        else if (!str_cmp(arg, "e") || !str_cmp(arg, "east"))
                door = 1;
        else if (!str_cmp(arg, "s") || !str_cmp(arg, "south"))
                door = 2;
        else if (!str_cmp(arg, "w") || !str_cmp(arg, "west"))
                door = 3;
        else if (!str_cmp(arg, "u") || !str_cmp(arg, "up"))
                door = 4;
        else if (!str_cmp(arg, "d") || !str_cmp(arg, "down"))
                door = 5;
        else
        {
                for (door = 0; door <= 5; door++)
                {
                        if ((pexit = ch->in_room->exit[door]) != NULL
                            && IS_SET(pexit->exit_info, EX_ISDOOR)
                            && pexit->keyword != NULL
                            && is_name(arg, pexit->keyword))
                                return door;
                }
                act("I see no $T here.", ch, NULL, arg, TO_CHAR);
                return -1;
        }

        if ((pexit = ch->in_room->exit[door]) == NULL)
        {
                act("I see no door $T here.", ch, NULL, arg, TO_CHAR);
                return -1;
        }

        if (!IS_SET(pexit->exit_info, EX_ISDOOR))
        {
                send_to_char("You can't do that.\n\r", ch);
                return -1;
        }

        return door;
}

/* random room generation procedure */
ROOM_INDEX_DATA *get_random_room(CHAR_DATA * ch)
{
        ROOM_INDEX_DATA *room;

        for (;;)
        {
                room = get_room_index(number_range(0, 100000));
                if (room != NULL)
                        if (!IS_SET(room->room_flags, ROOM_PRIVATE)
                            && !IS_SET(room->room_flags, ROOM_SOLITARY)
                            && !IS_SET(room->room_flags, ROOM_SAFE))
                                break;
        }

        return room;
}

ROOM_INDEX_DATA *get_rand_room()
{
        ROOM_INDEX_DATA *room;

        for (;;)
        {
                room = get_room_index(number_range(1000, 85000));
                if (room != NULL)
                        if (!IS_SET(room->room_flags, ROOM_PRIVATE)
                            && !IS_SET(room->room_flags, ROOM_ASTRAL)
                            && !IS_SET(room->room_flags, ROOM_SOLITARY)
                            && !IS_SET(room->room_flags, ROOM_SAFE))
                                break;
        }

        return room;
}

ROOM_INDEX_DATA *get_treemeld_room()
{
        ROOM_INDEX_DATA *room;

        for (;;)
        {
                room = get_room_index(number_range(0, 100000));
                if (room != NULL && room->sector_type == SECT_FOREST)
                        break;
        }

        return room;
}

ROOM_INDEX_DATA *get_rand_room_by_sect(int sect)
{
        ROOM_INDEX_DATA *room;

        for (;;)
        {
                room = get_room_index(number_range(0, 100000));
                if (room != NULL && room->sector_type == sect)
                        break;
        }

        return room;
}

/* Designed for the portal spell, but can also have other uses...KaVir
 * V0 = Where the portal will take you.
 * V1 = Number of uses (0 is infinate).
 * V2 = if 2, cannot be entered.
 * V3 = The room the portal is currently in.
 */
void do_enter(CHAR_DATA * ch, char *argument)
{
        ROOM_INDEX_DATA *pRoomIndex;
        ROOM_INDEX_DATA *location;
        char      arg[MAX_INPUT_LENGTH];
        char      poly[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        OBJ_DATA *portal;
        OBJ_DATA *portal_next;
        CHAR_DATA *mount;
        CHAR_DATA *gch;
        CHAR_DATA *gch_next;
        bool      found;

        argument = one_argument(argument, arg);

        if (ch->in_room != NULL)
        {
                if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
                {
                        send_to_char
                                ("The portal cannot function in an astral room.\n\r",
                                 ch);
                        return;
                }
        }

        if (arg[0] == '\0')
        {
                send_to_char("Enter what?\n\r", ch);
                return;
        }
        obj = get_obj_list(ch, arg, ch->in_room->contents);
        if (obj == NULL)
        {
                act("I see no $T here.", ch, NULL, arg, TO_CHAR);
                return;
        }
        if (obj->item_type != ITEM_PORTAL && obj->item_type != ITEM_WGATE)
        {
                act("You cannot enter that.", ch, NULL, arg, TO_CHAR);
                return;
        }
        if (IS_AFFECTED(ch, AFF_SHADOWPLANE)
            && !IS_SET(obj->extra_flags, ITEM_SHADOWPLANE))
        {
                send_to_char("You are too insubstantual.\n\r", ch);
                return;
        }
        else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                 IS_SET(obj->extra_flags, ITEM_SHADOWPLANE))
        {
                send_to_char("It is too insubstantual.\n\r", ch);
                return;
        }

        if (obj->item_type == ITEM_PORTAL)
        {
                if (obj->value[2] == 2 || obj->value[2] == 3)
                {
                        act("It seems to be closed.", ch, NULL, arg, TO_CHAR);
                        return;
                }

                pRoomIndex = get_room_index(obj->value[0]);
                location = ch->in_room;
                if (pRoomIndex == NULL)
                {
                        act("You are unable to enter.", ch, NULL, arg,
                            TO_CHAR);
                        return;
                }
                act("You step into $p.", ch, obj, NULL, TO_CHAR);
                if (!IS_NPC(ch) && IS_AFFECTED(ch, AFF_POLYMORPH))
                        xprintf(poly, "%s steps into $p.", ch->morph);
                else
                        xprintf(poly, "$n steps into $p.");
                act(poly, ch, obj, NULL, TO_ROOM);
                char_from_room(ch);
                char_to_room(ch, pRoomIndex);
                if (!IS_NPC(ch) && IS_AFFECTED(ch, AFF_POLYMORPH))
                        xprintf(poly, "%s steps out of $p.", ch->morph);
                else
                        xprintf(poly, "$n steps out of $p.");
                act(poly, ch, obj, NULL, TO_ROOM);
                char_from_room(ch);
                char_to_room(ch, location);
                if (obj->value[1] != 0)
                {
                        obj->value[1] = obj->value[1] - 1;
                        if (obj->value[1] < 1)
                        {
                                act("$p vanishes.", ch, obj, NULL, TO_CHAR);
                                act("$p vanishes.", ch, obj, NULL, TO_ROOM);
                                extract_obj(obj);
                        }
                }
                char_from_room(ch);
                char_to_room(ch, pRoomIndex);
                found = FALSE;
                for (portal = ch->in_room->contents; portal != NULL;
                     portal = portal_next)
                {
                        portal_next = portal->next_content;
                        if ((obj->value[0] == portal->value[3])
                            && (obj->value[3] == portal->value[0]))
                        {
                                found = TRUE;

                                if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                                    !IS_SET(portal->extra_flags,
                                            ITEM_SHADOWPLANE))
                                {
                                        REMOVE_BIT(ch->affected_by,
                                                   AFF_SHADOWPLANE);
                                        break;
                                }
                                else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                                         IS_SET(portal->extra_flags,
                                                ITEM_SHADOWPLANE))
                                {
                                        SET_BIT(ch->affected_by,
                                                AFF_SHADOWPLANE);
                                        break;
                                }
                                if (portal->value[1] != 0)
                                {
                                        portal->value[1] =
                                                portal->value[1] - 1;
                                        if (portal->value[1] < 1)
                                        {
                                                act("$p vanishes.", ch,
                                                    portal, NULL, TO_CHAR);
                                                act("$p vanishes.", ch,
                                                    portal, NULL, TO_ROOM);
                                                extract_obj(portal);
                                        }
                                }
                        }
                }
                do_look(ch, "auto");
                if ((mount = ch->mount) != NULL)
                {
                        char_from_room(mount);
                        char_to_room(mount, ch->in_room);
                }

                for (gch = location->people; gch; gch = gch_next)
                {
                        gch_next = gch->next_in_room;

                        if (gch->master == ch && gch->position == POS_STANDING
                            && gch->in_room != ch->in_room)
                        {
                                act("You follow $N.", gch, NULL, ch, TO_CHAR);
                                do_enter(gch, arg);
                        }
                }
                return;
        }
        else
        {
                pRoomIndex = get_room_index(obj->value[0]);
                location = ch->in_room;

                if (pRoomIndex == NULL)
                {
                        act("You are unable to enter.", ch, NULL, arg,
                            TO_CHAR);
                        return;
                }
                act("You step into $p.", ch, obj, NULL, TO_CHAR);
                if (!IS_NPC(ch) && IS_AFFECTED(ch, AFF_POLYMORPH))
                        xprintf(poly, "%s steps into $p.", ch->morph);
                else
                        xprintf(poly, "$n steps into $p.");
                act(poly, ch, obj, NULL, TO_ROOM);
                char_from_room(ch);
                char_to_room(ch, pRoomIndex);
                if (!IS_NPC(ch) && IS_AFFECTED(ch, AFF_POLYMORPH))
                        xprintf(poly, "%s steps out of $p.", ch->morph);
                else
                        xprintf(poly, "$n steps out of $p.");
                act(poly, ch, obj, NULL, TO_ROOM);

                for (portal = ch->in_room->contents; portal != NULL;
                     portal = portal_next)
                {
                        portal_next = portal->next_content;
                        if ((obj->value[0] == portal->value[3])
                            && (obj->value[3] == portal->value[0]))
                        {
                                found = TRUE;

                                if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                                    !IS_SET(portal->extra_flags,
                                            ITEM_SHADOWPLANE))
                                {
                                        REMOVE_BIT(ch->affected_by,
                                                   AFF_SHADOWPLANE);
                                        break;
                                }
                                else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                                         IS_SET(portal->extra_flags,
                                                ITEM_SHADOWPLANE))
                                {
                                        SET_BIT(ch->affected_by,
                                                AFF_SHADOWPLANE);
                                        break;
                                }
                        }
                }
                do_look(ch, "auto");
                if ((mount = ch->mount) == NULL)
                        return;
                char_from_room(mount);
                char_to_room(mount, ch->in_room);
                //mob progs
                /*
                 * If someone is following the char, these triggers get activated
                 * for the followers before the char, but it's safer this way...
                 */
                if (IS_NPC(ch) && HAS_TRIGGER_MOB(ch, TRIG_ENTRY))
                        p_percent_trigger(ch, NULL, NULL, NULL, NULL, NULL,
                                          TRIG_ENTRY);
                if (!IS_NPC(ch))
                {
                        p_greet_trigger(ch, PRG_MPROG);
                        p_greet_trigger(ch, PRG_OPROG);
                        p_greet_trigger(ch, PRG_RPROG);
                }
                return;
        }
}


void do_open(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        int       door;

        one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                send_to_char("Open what?\n\r", ch);
                return;
        }

        if ((obj = get_obj_here(ch, NULL, arg)) != NULL)
        {
                /*
                 * 'open object' 
                 */
                if (obj->item_type != ITEM_CONTAINER
                    && obj->item_type != ITEM_BOOK)
                {
                        send_to_char("That's not a container.\n\r", ch);
                        return;
                }
                if (!IS_SET(obj->value[1], CONT_CLOSED))
                {
                        send_to_char("It's already open.\n\r", ch);
                        return;
                }
                if (!IS_SET(obj->value[1], CONT_CLOSEABLE)
                    && obj->item_type != ITEM_BOOK)
                {
                        send_to_char("You can't do that.\n\r", ch);
                        return;
                }
                if (IS_SET(obj->value[1], CONT_LOCKED))
                {
                        send_to_char("It's locked.\n\r", ch);
                        return;
                }

                REMOVE_BIT(obj->value[1], CONT_CLOSED);
                send_to_char("Ok.\n\r", ch);
                act("$n opens $p.", ch, obj, NULL, TO_ROOM);
                return;
        }

        if ((door = find_door(ch, arg)) >= 0)
        {
                /*
                 * 'open door' 
                 */
                ROOM_INDEX_DATA *to_room;
                EXIT_DATA *pexit;
                EXIT_DATA *pexit_rev;

                pexit = ch->in_room->exit[door];
                if (!IS_SET(pexit->exit_info, EX_CLOSED))
                {
                        send_to_char("It's already open.\n\r", ch);
                        return;
                }
                if (IS_SET(pexit->exit_info, EX_LOCKED))
                {
                        send_to_char("It's locked.\n\r", ch);
                        return;
                }

                REMOVE_BIT(pexit->exit_info, EX_CLOSED);
                act("$n opens the $d.", ch, NULL, pexit->keyword, TO_ROOM);
                send_to_char("Ok.\n\r", ch);

                /*
                 * open the other side 
                 */
                if ((to_room = pexit->to_room) != NULL
                    && (pexit_rev = to_room->exit[rev_dir[door]]) != NULL
                    && pexit_rev->to_room == ch->in_room)
                {
                        CHAR_DATA *rch;

                        REMOVE_BIT(pexit_rev->exit_info, EX_CLOSED);
                        for (rch = to_room->people; rch != NULL;
                             rch = rch->next_in_room)
                                act("The $d opens.", rch, NULL,
                                    pexit_rev->keyword, TO_CHAR);
                }
        }

        return;
}

void do_close(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        int       door;

        one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                send_to_char("Close what?\n\r", ch);
                return;
        }

        if ((obj = get_obj_here(ch, NULL, arg)) != NULL)
        {
                /*
                 * 'close object' 
                 */
                if (obj->item_type != ITEM_CONTAINER
                    && obj->item_type != ITEM_BOOK)
                {
                        send_to_char("That's not a container.\n\r", ch);
                        return;
                }
                if (IS_SET(obj->value[1], CONT_CLOSED))
                {
                        send_to_char("It's already closed.\n\r", ch);
                        return;
                }
                if (!IS_SET(obj->value[1], CONT_CLOSEABLE)
                    && obj->item_type != ITEM_BOOK)
                {
                        send_to_char("You can't do that.\n\r", ch);
                        return;
                }

                SET_BIT(obj->value[1], CONT_CLOSED);
                if (obj->item_type == ITEM_BOOK)
                        obj->value[2] = 0;
                send_to_char("Ok.\n\r", ch);
                act("$n closes $p.", ch, obj, NULL, TO_ROOM);
                return;
        }

        if ((door = find_door(ch, arg)) >= 0)
        {
                /*
                 * 'close door' 
                 */
                ROOM_INDEX_DATA *to_room;
                EXIT_DATA *pexit;
                EXIT_DATA *pexit_rev;

                pexit = ch->in_room->exit[door];
                if (IS_SET(pexit->exit_info, EX_CLOSED))
                {
                        send_to_char("It's already closed.\n\r", ch);
                        return;
                }

                SET_BIT(pexit->exit_info, EX_CLOSED);
                act("$n closes the $d.", ch, NULL, pexit->keyword, TO_ROOM);
                send_to_char("Ok.\n\r", ch);

                /*
                 * close the other side 
                 */
                if ((to_room = pexit->to_room) != NULL
                    && (pexit_rev = to_room->exit[rev_dir[door]]) != 0
                    && pexit_rev->to_room == ch->in_room)
                {
                        CHAR_DATA *rch;

                        SET_BIT(pexit_rev->exit_info, EX_CLOSED);
                        for (rch = to_room->people; rch != NULL;
                             rch = rch->next_in_room)
                                act("The $d closes.", rch, NULL,
                                    pexit_rev->keyword, TO_CHAR);
                }
        }

        return;
}



void do_turn(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        int       value = 0;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);

        if (arg1[0] == '\0' || arg2[0] == '\0')
        {
                send_to_char("Syntax: Turn <book> <forward/back>.\n\r", ch);
                return;
        }

        if (is_number(arg2))
                value = atoi(arg2);

        if ((obj = get_obj_here(ch, NULL, arg1)) == NULL)
        {
                send_to_char("You don't have that book.\n\r", ch);
                return;
        }

        if (obj->item_type != ITEM_BOOK)
        {
                send_to_char("That's not a book.\n\r", ch);
                return;
        }
        if (IS_SET(obj->value[1], CONT_CLOSED))
        {
                send_to_char("First you should open it.\n\r", ch);
                return;
        }

        if (!str_cmp(arg2, "f") || !str_cmp(arg2, "forward"))
        {
                if (obj->value[2] >= obj->value[3])
                {
                        send_to_char
                                ("But you are already at the end of the book.\n\r",
                                 ch);
                        return;
                }
                obj->value[2] += 1;
                act("You flip forward a page in $p.", ch, obj, NULL, TO_CHAR);
                act("$n flips forward a page in $p.", ch, obj, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg2, "b") || !str_cmp(arg2, "backward"))
        {
                if (obj->value[2] <= 0)
                {
                        send_to_char
                                ("But you are already at the beginning of the book.\n\r",
                                 ch);
                        return;
                }
                obj->value[2] -= 1;
                act("You flip backward a page in $p.", ch, obj, NULL,
                    TO_CHAR);
                act("$n flips backward a page in $p.", ch, obj, NULL,
                    TO_ROOM);
        }
        else if (is_number(arg2) && value >= 0 && value <= obj->value[3])
        {
                if (value == obj->value[2])
                {
                        act("$p is already open at that page.", ch, obj, NULL,
                            TO_CHAR);
                        return;
                }
                else if (value < obj->value[2])
                {
                        act("You flip backwards through $p.", ch, obj, NULL,
                            TO_CHAR);
                        act("$n flips backwards through $p.", ch, obj, NULL,
                            TO_ROOM);
                }
                else
                {
                        act("You flip forwards through $p.", ch, obj, NULL,
                            TO_CHAR);
                        act("$n flips forwards through $p.", ch, obj, NULL,
                            TO_ROOM);
                }
                obj->value[2] = value;
        }
        else
                send_to_char
                        ("Do you wish to turn forward or backward a page?\n\r",
                         ch);
        return;
}



bool has_key(CHAR_DATA * ch, int key)
{
        OBJ_DATA *obj;

        for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
        {
                if (obj->pIndexData->vnum == key)
                        return TRUE;
        }

        return FALSE;
}

void do_lock(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        int       door;

        one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                send_to_char("Lock what?\n\r", ch);
                return;
        }

        if ((obj = get_obj_here(ch, NULL, arg)) != NULL)
        {
                /*
                 * 'lock object' 
                 */
                if (obj->item_type != ITEM_CONTAINER)
                {
                        send_to_char("That's not a container.\n\r", ch);
                        return;
                }
                if (!IS_SET(obj->value[1], CONT_CLOSED))
                {
                        send_to_char("It's not closed.\n\r", ch);
                        return;
                }
                if (obj->value[2] < 0)
                {
                        send_to_char("It can't be locked.\n\r", ch);
                        return;
                }
                if (!has_key(ch, obj->value[2]))
                {
                        send_to_char("You lack the key.\n\r", ch);
                        return;
                }
                if (IS_SET(obj->value[1], CONT_LOCKED))
                {
                        send_to_char("It's already locked.\n\r", ch);
                        return;
                }

                SET_BIT(obj->value[1], CONT_LOCKED);
                send_to_char("*Click*\n\r", ch);
                act("$n locks $p.", ch, obj, NULL, TO_ROOM);
                return;
        }

        if ((door = find_door(ch, arg)) >= 0)
        {
                /*
                 * 'lock door' 
                 */
                ROOM_INDEX_DATA *to_room;
                EXIT_DATA *pexit;
                EXIT_DATA *pexit_rev;

                pexit = ch->in_room->exit[door];
                if (!IS_SET(pexit->exit_info, EX_CLOSED))
                {
                        send_to_char("It's not closed.\n\r", ch);
                        return;
                }
                if (pexit->key < 0)
                {
                        send_to_char("It can't be locked.\n\r", ch);
                        return;
                }
                if (!has_key(ch, pexit->key))
                {
                        send_to_char("You lack the key.\n\r", ch);
                        return;
                }
                if (IS_SET(pexit->exit_info, EX_LOCKED))
                {
                        send_to_char("It's already locked.\n\r", ch);
                        return;
                }

                SET_BIT(pexit->exit_info, EX_LOCKED);
                send_to_char("*Click*\n\r", ch);
                act("$n locks the $d.", ch, NULL, pexit->keyword, TO_ROOM);

                /*
                 * lock the other side 
                 */
                if ((to_room = pexit->to_room) != NULL
                    && (pexit_rev = to_room->exit[rev_dir[door]]) != 0
                    && pexit_rev->to_room == ch->in_room)
                {
                        SET_BIT(pexit_rev->exit_info, EX_LOCKED);
                }
        }

        return;
}

void do_unlock(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        int       door;

        one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                send_to_char("Unlock what?\n\r", ch);
                return;
        }

        if ((obj = get_obj_here(ch, NULL, arg)) != NULL)
        {
                /*
                 * 'unlock object' 
                 */
                if (obj->item_type != ITEM_CONTAINER)
                {
                        send_to_char("That's not a container.\n\r", ch);
                        return;
                }
                if (!IS_SET(obj->value[1], CONT_CLOSED))
                {
                        send_to_char("It's not closed.\n\r", ch);
                        return;
                }
                if (obj->value[2] < 0)
                {
                        send_to_char("It can't be unlocked.\n\r", ch);
                        return;
                }
                if (!has_key(ch, obj->value[2]))
                {
                        send_to_char("You lack the key.\n\r", ch);
                        return;
                }
                if (!IS_SET(obj->value[1], CONT_LOCKED))
                {
                        send_to_char("It's already unlocked.\n\r", ch);
                        return;
                }

                REMOVE_BIT(obj->value[1], CONT_LOCKED);
                send_to_char("*Click*\n\r", ch);
                act("$n unlocks $p.", ch, obj, NULL, TO_ROOM);
                return;
        }

        if ((door = find_door(ch, arg)) >= 0)
        {
                /*
                 * 'unlock door' 
                 */
                ROOM_INDEX_DATA *to_room;
                EXIT_DATA *pexit;
                EXIT_DATA *pexit_rev;

                pexit = ch->in_room->exit[door];
                if (!IS_SET(pexit->exit_info, EX_CLOSED))
                {
                        send_to_char("It's not closed.\n\r", ch);
                        return;
                }
                if (pexit->key < 0)
                {
                        send_to_char("It can't be unlocked.\n\r", ch);
                        return;
                }
                if (!has_key(ch, pexit->key))
                {
                        send_to_char("You lack the key.\n\r", ch);
                        return;
                }
                if (!IS_SET(pexit->exit_info, EX_LOCKED))
                {
                        send_to_char("It's already unlocked.\n\r", ch);
                        return;
                }

                REMOVE_BIT(pexit->exit_info, EX_LOCKED);
                send_to_char("*Click*\n\r", ch);
                act("$n unlocks the $d.", ch, NULL, pexit->keyword, TO_ROOM);

                /*
                 * unlock the other side 
                 */
                if ((to_room = pexit->to_room) != NULL
                    && (pexit_rev = to_room->exit[rev_dir[door]]) != NULL
                    && pexit_rev->to_room == ch->in_room)
                {
                        REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);
                }
        }

        return;
}

void do_pick(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *gch;
        OBJ_DATA *obj;
        int       door;

        one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                send_to_char("Pick what?\n\r", ch);
                return;
        }

        WAIT_STATE(ch, skill_table[gsn_pick_lock].beats);

        /*
         * look for guards 
         */
        for (gch = ch->in_room->people; gch; gch = gch->next_in_room)
        {
                if (IS_NPC(gch) && IS_AWAKE(gch)
                    && ch->level + 5 < gch->level)
                {
                        act("$N is standing too close to the lock.",
                            ch, NULL, gch, TO_CHAR);
                        return;
                }
        }

        if (!IS_NPC(ch)
            && number_percent() > ch->pcdata->learned[gsn_pick_lock])
        {
                send_to_char("You failed.\n\r", ch);
                return;
        }

        if ((obj = get_obj_here(ch, NULL, arg)) != NULL)
        {
                /*
                 * 'pick object' 
                 */
                if (obj->item_type != ITEM_CONTAINER)
                {
                        send_to_char("That's not a container.\n\r", ch);
                        return;
                }
                if (!IS_SET(obj->value[1], CONT_CLOSED))
                {
                        send_to_char("It's not closed.\n\r", ch);
                        return;
                }
                if (obj->value[2] < 0)
                {
                        send_to_char("It can't be unlocked.\n\r", ch);
                        return;
                }
                if (!IS_SET(obj->value[1], CONT_LOCKED))
                {
                        send_to_char("It's already unlocked.\n\r", ch);
                        return;
                }
                if (IS_SET(obj->value[1], CONT_PICKPROOF))
                {
                        send_to_char("You failed.\n\r", ch);
                        return;
                }

                REMOVE_BIT(obj->value[1], CONT_LOCKED);
                send_to_char("*Click*\n\r", ch);
                act("$n picks $p.", ch, obj, NULL, TO_ROOM);
                return;
        }

        if ((door = find_door(ch, arg)) >= 0)
        {
                /*
                 * 'pick door' 
                 */
                ROOM_INDEX_DATA *to_room;
                EXIT_DATA *pexit;
                EXIT_DATA *pexit_rev;

                pexit = ch->in_room->exit[door];
                if (!IS_SET(pexit->exit_info, EX_CLOSED))
                {
                        send_to_char("It's not closed.\n\r", ch);
                        return;
                }
                if (pexit->key < 0)
                {
                        send_to_char("It can't be picked.\n\r", ch);
                        return;
                }
                if (!IS_SET(pexit->exit_info, EX_LOCKED))
                {
                        send_to_char("It's already unlocked.\n\r", ch);
                        return;
                }
                if (IS_SET(pexit->exit_info, EX_PICKPROOF))
                {
                        send_to_char("You failed.\n\r", ch);
                        return;
                }

                REMOVE_BIT(pexit->exit_info, EX_LOCKED);
                send_to_char("*Click*\n\r", ch);
                act("$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM);

                /*
                 * pick the other side 
                 */
                if ((to_room = pexit->to_room) != NULL
                    && (pexit_rev = to_room->exit[rev_dir[door]]) != NULL
                    && pexit_rev->to_room == ch->in_room)
                {
                        REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);
                }
        }

        return;
}

void do_stand(CHAR_DATA * ch, char *argument)
{
        //OBJ_DATA *obj = NULL;

        switch (ch->position)
        {
        case POS_SLEEPING:
                if (IS_AFFECTED(ch, AFF_SLEEP))
                {
                        send_to_char("You can't wake up!\n\r", ch);
                        return;
                }

                send_to_char("You wake and stand up.\n\r", ch);
                act("$n wakes and stands up.", ch, NULL, NULL, TO_ROOM);
                ch->position = POS_STANDING;

                break;

        case POS_RESTING:
        case POS_SITTING:
                send_to_char("You stand up.\n\r", ch);
                act("$n stands up.", ch, NULL, NULL, TO_ROOM);
                ch->position = POS_STANDING;
                break;

        case POS_MEDITATING:
                send_to_char("You uncross your legs and stand up.\n\r", ch);
                act("$n uncrosses $s legs and stands up.", ch, NULL, NULL,
                    TO_ROOM);
                ch->position = POS_STANDING;
                break;

        case POS_STANDING:
                send_to_char("You are already standing.\n\r", ch);
                break;

        case POS_FIGHTING:
                send_to_char("You are already fighting!\n\r", ch);
                break;
        }
        //if ( HAS_TRIGGER_OBJ( obj, TRIG_SIT ) )
        //p_percent_trigger( NULL, obj, NULL, ch, NULL, NULL, TRIG_SIT );
        return;
}

/*
void do_rest( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_MEDITATING:
    case POS_SITTING:
    case POS_STANDING:
	send_to_char( "You rest.\n\r", ch );
	act( "$n rests.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_RESTING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}
*/

void do_rest(CHAR_DATA * ch, char *argument)
{
        OBJ_DATA *obj = NULL;

        if (IS_AFFECTED(ch, AFF_SLEEP)) /* magical sleep. can't rest. */
        {
                send_to_char("You are asleep and cannot wake up!\n\r", ch);
                return;
        }
        if (ch->position == POS_FIGHTING)
        {
                send_to_char("Maybe you should finish this fight first!\n\r",
                             ch);
                return;
        }

        /*
         * okay, now that we know we can rest, find an object to rest on 
         */
        if (argument[0] != '\0')
        {
                obj = get_obj_list(ch, argument, ch->in_room->contents);
                if (obj == NULL)
                {
                        send_to_char("You don't see that here.\n\r", ch);
                        return;
                }
        }
        else
                obj = ch->on;
        if (obj != NULL)
        {
                if (!IS_SET(obj->item_type, ITEM_FURNITURE)
                    || (!IS_SET(obj->value[2], REST_ON)
                        && !IS_SET(obj->value[2], REST_IN)
                        && !IS_SET(obj->value[2], REST_AT)))
                {
                        send_to_char("You can't rest on that.\n\r", ch);
                        return;
                }

                if (obj != NULL && ch->on != obj
                    && count_users(obj) >= obj->value[0])
                {
                        act("There's no more room on $p.", ch, obj, NULL,
                            TO_CHAR);
                        return;
                }
                ch->on = obj;
                if (HAS_TRIGGER_OBJ(obj, TRIG_SIT))
                        p_percent_trigger(NULL, obj, NULL, ch, NULL, NULL,
                                          TRIG_SIT);
        }

        switch (ch->position)
        {
        case POS_SLEEPING:
                send_to_char("You wake up and roll over to rest.\n\r", ch);
                act("$n wakes up, then rolls over to rest.", ch, NULL, NULL,
                    TO_ROOM);
                ch->position = POS_RESTING;
                do_look(ch, "auto");
                break;
        case POS_RESTING:
                send_to_char("You are already resting.\n\r", ch);
                break;
        case POS_STANDING:
                if (obj == NULL)
                {
                        send_to_char("You sit down and rest.\n\r", ch);
                        act("$n sits down and rests.", ch, NULL, NULL,
                            TO_ROOM);
                }
                else if (IS_SET(obj->value[2], REST_AT))
                {
                        act("You sit down at $p and rest.", ch, obj, NULL,
                            TO_CHAR);
                        act("$n sits down at $p and rests.", ch, obj, NULL,
                            TO_ROOM);
                }
                else if (IS_SET(obj->value[2], REST_ON))
                {
                        act("You sit on $p and rest.", ch, obj, NULL,
                            TO_CHAR);
                        act("$n sits on $p and rests.", ch, obj, NULL,
                            TO_ROOM);
                }
                else
                {
                        act("You rest in $p.", ch, obj, NULL, TO_CHAR);
                        act("$n rests in $p.", ch, obj, NULL, TO_ROOM);
                }
                ch->position = POS_RESTING;
                break;
        case POS_SITTING:
                if (obj == NULL)
                {
                        send_to_char("You rest.\n\r", ch);
                        act("$n rests.", ch, NULL, NULL, TO_ROOM);
                }
                else if (IS_SET(obj->value[2], REST_AT))
                {
                        act("You rest at $p.", ch, obj, NULL, TO_CHAR);
                        act("$n rests at $p.", ch, obj, NULL, TO_ROOM);
                }
                else if (IS_SET(obj->value[2], REST_ON))
                {
                        act("You rest on $p.", ch, obj, NULL, TO_CHAR);
                        act("$n rests on $p.", ch, obj, NULL, TO_ROOM);
                }
                else
                {
                        act("You rest in $p.", ch, obj, NULL, TO_CHAR);
                        act("$n rests in $p.", ch, obj, NULL, TO_ROOM);
                }
                ch->position = POS_RESTING;
                break;
        }
        return;
}

/*
void do_sit( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_MEDITATING:
	send_to_char( "You are already meditating.\n\r", ch );
	break;

    case POS_SITTING:
	send_to_char( "You are already sitting.\n\r", ch );
	break;

    case POS_STANDING:
	send_to_char( "You sit down.\n\r", ch );
	act( "$n sits down.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_SITTING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }
    //if ( HAS_TRIGGER_OBJ( obj, TRIG_SIT ) )
	//p_percent_trigger( NULL, OBJ, NULL, ch, NULL, NULL, TRIG_SIT );
    return;
}
*/

void do_sit(CHAR_DATA * ch, char *argument)
{
        OBJ_DATA *obj = NULL;

        if (IS_AFFECTED(ch, AFF_SLEEP)) /* magical sleep. can't rest. */
        {
                send_to_char("You are asleep and cannot wake up!\n\r", ch);
                return;
        }
        if (ch->position == POS_FIGHTING)
        {
                send_to_char("Maybe you should finish this fight first!\n\r",
                             ch);
                return;
        }

        /*
         * okay, now that we know we can sit, find an object to sit on 
         */
        if (argument[0] != '\0')
        {
                obj = get_obj_list(ch, argument, ch->in_room->contents);
                if (obj == NULL)
                {
                        send_to_char("You don't see that here.\n\r", ch);
                        return;
                }
        }
        else
                obj = ch->on;

        if (obj != NULL)
        {
                if (!IS_SET(obj->item_type, ITEM_FURNITURE)
                    || (!IS_SET(obj->value[2], SIT_ON)
                        && !IS_SET(obj->value[2], SIT_IN)
                        && !IS_SET(obj->value[2], SIT_AT)))
                {
                        send_to_char("You can't sit on that.\n\r", ch);
                        return;
                }

                if (obj != NULL && ch->on != obj
                    && count_users(obj) >= obj->value[0])
                {
                        act("There's no more room on $p.", ch, obj, NULL,
                            TO_CHAR);
                        return;
                }
                ch->on = obj;
                if (HAS_TRIGGER_OBJ(obj, TRIG_SIT))
                {
                        p_percent_trigger(NULL, obj, NULL, ch, NULL, NULL,
                                          TRIG_SIT);
                }
        }

        switch (ch->position)
        {
        case POS_SLEEPING:
                send_to_char("You wake and sit up.\n\r", ch);
                act("$n wakes and sits up.", ch, NULL, NULL, TO_ROOM);
                ch->position = POS_SITTING;
                do_look(ch, "auto");
                break;
        case POS_SITTING:
                send_to_char("You are already sitting.\n\r", ch);
                break;
        case POS_STANDING:
                if (obj == NULL)
                {
                        send_to_char("You sit down.\n\r", ch);
                        act("$n sits down.", ch, NULL, NULL, TO_ROOM);
                }
                else if (IS_SET(obj->value[2], SIT_AT))
                {
                        act("You sit down at $p.", ch, obj, NULL, TO_CHAR);
                        act("$n sits down at $p.", ch, obj, NULL, TO_ROOM);
                }
                else if (IS_SET(obj->value[2], SIT_ON))
                {
                        act("You sit on $p.", ch, obj, NULL, TO_CHAR);
                        act("$n sits on $p.", ch, obj, NULL, TO_ROOM);
                }
                else
                {
                        act("You sit in $p.", ch, obj, NULL, TO_CHAR);
                        act("$n sits in $p.", ch, obj, NULL, TO_ROOM);
                }
                ch->position = POS_SITTING;
                break;
        case POS_RESTING:
                if (obj == NULL)
                {
                        send_to_char("You sit up.\n\r", ch);
                        act("$n sits up.", ch, NULL, NULL, TO_ROOM);
                }
                else if (IS_SET(obj->value[2], REST_AT))
                {
                        act("You sit up at $p.", ch, obj, NULL, TO_CHAR);
                        act("$n sits up at $p.", ch, obj, NULL, TO_ROOM);
                }
                else if (IS_SET(obj->value[2], REST_ON))
                {
                        act("You rest on $p.", ch, obj, NULL, TO_CHAR);
                        act("$n rests on $p.", ch, obj, NULL, TO_ROOM);
                }
                else
                {
                        act("You rest in $p.", ch, obj, NULL, TO_CHAR);
                        act("$n rests in $p.", ch, obj, NULL, TO_ROOM);
                }
                ch->position = POS_RESTING;
                break;
        }

        return;
}

void do_sleep(CHAR_DATA * ch, char *argument)
{
        OBJ_DATA *obj = NULL;

        switch (ch->position)
        {
        case POS_MORTAL:
                send_to_char
                        ("You give up on life, and go to sleep forever.\n\r",
                         ch);
                break;
        case POS_INCAP:
        case POS_STUNNED:
        case POS_DEAD:
                send_to_char("You are hurt far too badly for that.\n\r", ch);
                break;
        case POS_SLEEPING:
                send_to_char("You are already sleeping.\n\r", ch);
                break;
        case POS_RESTING:
        case POS_SITTING:
        case POS_STANDING:
                if (argument[0] == '\0' && ch->on == NULL)
                {
                        send_to_char("You go to sleep.\n\r", ch);
                        act("$n goes to sleep.", ch, NULL, NULL, TO_ROOM);
                        ch->position = POS_SLEEPING;
                }
                else    /* find an object and sleep on it */
                {
                        if (argument[0] == '\0')
                                obj = ch->on;
                        else
                                obj = get_obj_list(ch, argument,
                                                   ch->in_room->contents);
                        if (obj == NULL)
                        {
                                send_to_char("You don't see that here.\n\r",
                                             ch);
                                return;
                        }
                        if (obj->item_type != ITEM_FURNITURE
                            || (!IS_SET(obj->value[2], SLEEP_ON)
                                && !IS_SET(obj->value[2], SLEEP_IN)
                                && !IS_SET(obj->value[2], SLEEP_AT)))
                        {
                                send_to_char("You can't sleep on that!\n\r",
                                             ch);
                                return;
                        }

                        if (ch->on != obj
                            && count_users(obj) >= obj->value[0])
                        {
                                act("There is no room on $p for you.", ch,
                                    obj, NULL, TO_CHAR);
                                return;
                        }

                        ch->on = obj;
                        if (HAS_TRIGGER_OBJ(obj, TRIG_SIT))
                                p_percent_trigger(NULL, obj, NULL, ch, NULL,
                                                  NULL, TRIG_SIT);
                        if (IS_SET(obj->value[2], SLEEP_AT))
                        {
                                act("You go to sleep at $p.", ch, obj, NULL,
                                    TO_CHAR);
                                act("$n goes to sleep at $p.", ch, obj, NULL,
                                    TO_ROOM);
                        }
                        else if (IS_SET(obj->value[2], SLEEP_ON))
                        {
                                act("You go to sleep on $p.", ch, obj, NULL,
                                    TO_CHAR);
                                act("$n goes to sleep on $p.", ch, obj, NULL,
                                    TO_ROOM);
                        }
                        else
                        {
                                act("You go to sleep in $p.", ch, obj, NULL,
                                    TO_CHAR);
                                act("$n goes to sleep in $p.", ch, obj, NULL,
                                    TO_ROOM);
                        }
                        ch->position = POS_SLEEPING;
                }
                break;
        case POS_FIGHTING:
                send_to_char("Yeah right! You'd get slaughtered!\n\r", ch);
                break;
        }

        return;
}

void do_wake(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;

        one_argument(argument, arg);
        if (arg[0] == '\0')
        {
                do_stand(ch, argument);
                return;
        }

        if (!IS_AWAKE(ch))
        {
                send_to_char("You are asleep yourself!\n\r", ch);
                return;
        }

        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }

        // Chars who are AWAKE by the macro's definition may still be unwakeable, and
        // will now get a better error msg. Someone who is mortally wounded isn't "already awake".

        if (victim->position < POS_SLEEPING)
        {
                act("$N can't be woken up.", ch, NULL, victim, TO_CHAR);
                return;
        }

        if (IS_AWAKE(victim))
        {
                act("$N is already awake.", ch, NULL, victim, TO_CHAR);
                return;
        }

        if (IS_AFFECTED(victim, AFF_SLEEP))
        {
                act("You can't wake $M!", ch, NULL, victim, TO_CHAR);
                return;
        }

        victim->position = POS_STANDING;
        victim->on = NULL;
        act("You wake $M.", ch, NULL, victim, TO_CHAR);
        act("$n wakes you.", ch, NULL, victim, TO_VICT);
        do_look(ch, "auto");
        return;
}


void do_meditate(CHAR_DATA * ch, char *argument)
{
        OBJ_DATA *obj = NULL;

        if (IS_NPC(ch)
            || (!IS_CLASS(ch, CLASS_MAGE) && !IS_CLASS(ch, CLASS_MONK)
                && !IS_CLASS(ch, CLASS_FAE) && !IS_CLASS(ch, CLASS_NINJA)
                && !IS_CLASS(ch, CLASS_DROW) && !IS_CLASS(ch, CLASS_LICH)
		&& !IS_CLASS(ch, CLASS_ZOMBIE) && !IS_CLASS(ch, CLASS_SHINOBI)))
        {
                send_to_char
                        ("You are unable to attain the correct state of mind.\n\r",
                         ch);
                return;
        }


        obj = ch->on;
        if (obj != NULL)
        {
                send_to_char("You can't mediate on furniture.\n\r", ch);
                return;
        }



        if (IS_CLASS(ch, CLASS_FAE) && ch->power[DISC_FAE_ARCANE] < 2)
        {
                send_to_char("You don't know how to do that yet.\n\r", ch);
                return;
        }

        switch (ch->position)
        {
        case POS_SLEEPING:
                send_to_char("You are already sleeping.\n\r", ch);
                break;

        case POS_RESTING:
                send_to_char("You are already resting.\n\r", ch);
                break;

        case POS_MEDITATING:
                send_to_char("You are already meditating.\n\r", ch);
                break;

        case POS_SITTING:
                send_to_char("You cross your legs and start meditating.\n\r",
                             ch);
                act("$n crosses $s legs and starts meditating.", ch, NULL,
                    NULL, TO_ROOM);
                ch->position = POS_MEDITATING;
                break;

        case POS_STANDING:
                send_to_char
                        ("You sit down, cross your legs and start meditating.\n\r",
                         ch);
                act("$n sits down, crosses $s legs and starts meditating.",
                    ch, NULL, NULL, TO_ROOM);
                ch->position = POS_MEDITATING;
                break;

        case POS_FIGHTING:
                send_to_char("You are already fighting!\n\r", ch);
                break;
        }

        return;
}

void do_healing(CHAR_DATA * ch, char *argument)
{
        if (IS_NPC(ch)
            || (!IS_CLASS(ch, CLASS_VAMPIRE)
                && ch->power[DISC_VAMP_PROT] < 8))
        {
                send_to_char
                        ("You are unable to assume a state of healing.\n\r",
                         ch);
                return;
        }

        switch (ch->position)
        {
        case POS_SLEEPING:
                send_to_char("You are already sleeping.\n\r", ch);
                break;

        case POS_RESTING:
                send_to_char("You are already resting.\n\r", ch);
                break;

        case POS_MEDITATING:
                send_to_char("You are already in a healing position.\n\r",
                             ch);
                break;

        case POS_SITTING:
                send_to_char
                        ("You close your eyes and become completely relaxed.\n\r",
                         ch);
                act("$n closes $m eyes and looks completely relaxed.", ch,
                    NULL, NULL, TO_ROOM);
                ch->position = POS_MEDITATING;
                break;

        case POS_STANDING:
                send_to_char
                        ("You close your eyes and become completely relaxed.\n\r",
                         ch);
                act("$n closes $m eyes and looks completely relaxed.", ch,
                    NULL, NULL, TO_ROOM);
                ch->position = POS_MEDITATING;
                break;

        case POS_FIGHTING:
                send_to_char("You are already fighting!\n\r", ch);
                break;
        }

        return;
}

/*
void do_sleep( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_SITTING:
    case POS_MEDITATING:
    case POS_RESTING:
    case POS_STANDING: 
	send_to_char( "You sleep.\n\r", ch );
	act( "$n sleeps.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_SLEEPING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }
    //if ( HAS_TRIGGER_OBJ( obj, TRIG_SIT ) )
	//p_percent_trigger( NULL, obj, NULL, ch, NULL, NULL, TRIG_SIT );
    return;
}
*/

/*
void do_wake( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
	{ do_stand( ch, argument ); return; }

    if ( !IS_AWAKE(ch) )
	{ send_to_char( "You are asleep yourself!\n\r",       ch ); return; }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
	{ send_to_char( "They aren't here.\n\r",              ch ); return; }

    if ( IS_AWAKE(victim) )
	{ act( "$N is already awake.", ch, NULL, victim, TO_CHAR ); return; }

    if ( IS_AFFECTED(victim, AFF_SLEEP))
	{ act( "You can't wake $M!",   ch, NULL, victim, TO_CHAR );  return; }

    if (victim->position < POS_SLEEPING)
	{ act( "$E doesn't respond!",   ch, NULL, victim, TO_CHAR );  return; }

    act( "You wake $M.", ch, NULL, victim, TO_CHAR );
    act( "$n wakes you.", ch, NULL, victim, TO_VICT );
    victim->position = POS_STANDING;
    return;
}
*/

void do_sneak(CHAR_DATA * ch, char *argument)
{
        AFFECT_DATA af;

        send_to_char("You attempt to move silently.\n\r", ch);
        affect_strip(ch, gsn_sneak);

        if (IS_NPC(ch) || number_percent() < ch->pcdata->learned[gsn_sneak])
        {
                af.type = gsn_sneak;
                af.duration = ch->level;
                af.location = APPLY_NONE;
                af.modifier = 0;
                af.bitvector = AFF_SNEAK;
                affect_to_char(ch, &af);
        }

        return;
}

void do_hide(CHAR_DATA * ch, char *argument)
{

        send_to_char("You attempt to hide.\n\r", ch);

        if (IS_AFFECTED(ch, AFF_HIDE))
                REMOVE_BIT(ch->affected_by, AFF_HIDE);

        if (IS_NPC(ch) || number_percent() < ch->pcdata->learned[gsn_hide])
                SET_BIT(ch->affected_by, AFF_HIDE);

        return;
}

/*
 * Contributed by Alander.
 */
void do_visible(CHAR_DATA * ch, char *argument)
{
        affect_strip(ch, gsn_invis);
        affect_strip(ch, gsn_mass_invis);
        affect_strip(ch, gsn_sneak);
        REMOVE_BIT(ch->affected_by, AFF_HIDE);
        REMOVE_BIT(ch->affected_by, AFF_INVISIBLE);
        REMOVE_BIT(ch->affected_by, AFF_SNEAK);
        send_to_char("Ok.\n\r", ch);
        return;
}



void do_unpolymorph(CHAR_DATA * ch, char *argument)
{
        if (!is_affected(ch, gsn_polymorph))
        {
                send_to_char("But you are not polymorphed!\n\r", ch);
                return;
        }
        act("$n's body begins to distort.", ch, NULL, NULL, TO_ROOM);
        affect_strip(ch, gsn_polymorph);
        act("$n resumes $s normal form.", ch, NULL, NULL, TO_ROOM);
        send_to_char("You resume your normal form.\n\r", ch);
        return;
}



void do_recall(CHAR_DATA * ch, char *argument)
{
        char      buf[MAX_STRING_LENGTH];
        CHAR_DATA *victim;
        CHAR_DATA *mount;
        ROOM_INDEX_DATA *location;

        if (IS_NPC(ch))
                return;

        act("$n's body flickers with green energy.", ch, NULL, NULL, TO_ROOM);
        act("Your body flickers with green energy.", ch, NULL, NULL, TO_CHAR);

        if ((location = get_room_index(ch->home)) == NULL)
        {
                send_to_char("You are completely lost.\n\r", ch);
                return;
        }

        if (ch->in_room == location)
                return;

        if (IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
            || IS_AFFECTED(ch, AFF_CURSE))
        {
                send_to_char("You are unable to recall.\n\r", ch);
                return;
        }

        if (IS_SET(ch->in_room->room_flags, ROOM_NO_CHANT))
        {
                send_to_char
                        ("You are unable to chant, its a no chant room.\n\r",
                         ch);
                return;
        }

        if (IS_SET(ch->flag2, AFF_TOTALBLIND))
        {
                stc("You are unable to recall.\n\r", ch);
                return;
        }

        if ((victim = ch->fighting) != NULL)
        {
                if (number_bits(1) == 0)
                {
                        WAIT_STATE(ch, 4);
                        xprintf(buf, "You failed!\n\r");
                        send_to_char(buf, ch);
                        return;
                }
                xprintf(buf, "You recall from combat!\n\r");
                send_to_char(buf, ch);
                stop_fighting(ch, TRUE);
        }

        act("$n disappears.", ch, NULL, NULL, TO_ROOM);
        char_from_room(ch);
        char_to_room(ch, location);
        act("$n appears in the room.", ch, NULL, NULL, TO_ROOM);
        do_look(ch, "auto");
        if ((mount = ch->mount) == NULL)
                return;
        char_from_room(mount);
        char_to_room(mount, ch->in_room);
        return;
}

void do_crecall(CHAR_DATA * ch, char *argument)
{
        char      buf[MAX_STRING_LENGTH];
        CHAR_DATA *victim;
        CHAR_DATA *mount;
        ROOM_INDEX_DATA *location;

        if (IS_NPC(ch))
                return;

        if (ch->class == 0)
        {
                stc("Think about it...\n\r", ch);
                return;
        }

        act("$n's body flickers with red energy.", ch, NULL, NULL, TO_ROOM);
        act("Your body flickers with red energy.", ch, NULL, NULL, TO_CHAR);

        if (IS_CLASS(ch, CLASS_ANGEL))
                location = get_room_index(ROOM_VNUM_ANGEL_HOME);
        else if (IS_CLASS(ch, CLASS_LICH))
                location = get_room_index(ROOM_VNUM_LICH_HOME);
        else if (IS_CLASS(ch, CLASS_TANARRI))
                location = get_room_index(ROOM_VNUM_TAN_HOME);
        else if (IS_CLASS(ch, CLASS_CYBORG))
                location = get_room_index(ROOM_VNUM_DROID_HOME);
        else if (IS_CLASS(ch, CLASS_SAMURAI))
                location = get_room_index(ROOM_VNUM_SAM_HOME);
        else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                location = get_room_index(ROOM_VNUM_UK_HOME);
        else if (IS_CLASS(ch, CLASS_VAMPIRE))
                location = get_room_index(ROOM_VNUM_VAMP_HOME);
        else if (IS_CLASS(ch, CLASS_NINJA))
                location = get_room_index(ROOM_VNUM_NINJA_HOME);
        else if (IS_CLASS(ch, CLASS_MAGE))
                location = get_room_index(ROOM_VNUM_MAGE_HOME);
        else if (IS_CLASS(ch, CLASS_WEREWOLF))
                location = get_room_index(ROOM_VNUM_WW_HOME);
        else if (IS_CLASS(ch, CLASS_MONK))
                location = get_room_index(ROOM_VNUM_MONK_HOME);
        else if (IS_CLASS(ch, CLASS_THIEF))
                location = get_room_index(ROOM_VNUM_THIEF_HOME);
        else if (IS_CLASS(ch, CLASS_DRACONIAN))
                location = get_room_index(ROOM_VNUM_DRAC_HOME);
        else if (IS_CLASS(ch, CLASS_SKYBLADE))
                location = get_room_index(ROOM_VNUM_SKY_HOME);
        else if (IS_CLASS(ch, CLASS_GHOUL))
                location = get_room_index(ROOM_VNUM_GHOUL_HOME);
        else if (IS_CLASS(ch, CLASS_DEMON))
                location = get_room_index(ROOM_VNUM_DEMON_HOME);
        else if (IS_CLASS(ch, CLASS_JEDI))
                location = get_room_index(ROOM_VNUM_JEDI_HOME);
        else if (IS_CLASS(ch, CLASS_SHADOW))
                location = get_room_index(ROOM_VNUM_SHADOW_HOME);
        else if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
                location = get_room_index(ROOM_VNUM_SHIFTER_HOME);
        else if (IS_CLASS(ch, CLASS_DROW))
                location = get_room_index(ROOM_VNUM_DROW_HOME);
        else if (IS_CLASS(ch, CLASS_PRIEST))
                location = get_room_index(ROOM_VNUM_PRIEST_HOME);
        else if (IS_CLASS(ch, CLASS_FAE))
                location = get_room_index(ROOM_VNUM_FAE_HOME);
        else if (IS_CLASS(ch, CLASS_DRONE))
                location = get_room_index(ROOM_VNUM_DRONE_HOME);
        else if (IS_CLASS(ch, CLASS_HOBBIT))
                location = get_room_index(ROOM_VNUM_HOBBIT_HOME);
        else if (IS_CLASS(ch, CLASS_GIANT))
                location = get_room_index(ROOM_VNUM_GIANT_HOME);
	else if (IS_CLASS(ch, CLASS_ELEMENTAL))
		location = get_room_index(ROOM_VNUM_ELEMENTAL_HOME);
        else if (IS_CLASS(ch, CLASS_PALADIN))
                location = get_room_index(ROOM_VNUM_PALADIN_HOME);
        else if (IS_CLASS(ch, CLASS_BLADE_MASTER))
                location = get_room_index(ROOM_VNUM_BLADE_HOME);

        else
        {
                send_to_char("Your crecall is broken, tell Zarius\n\r", ch);
                return;
        }


        if (ch->in_room == location)
                return;

        if (IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
            || IS_AFFECTED(ch, AFF_CURSE))
        {
                send_to_char("You are unable to crecall.\n\r", ch);
                return;
        }

        if ((victim = ch->fighting) != NULL)
        {
                if (number_bits(1) == 0)
                {
                        WAIT_STATE(ch, 4);
                        xprintf(buf, "You failed!\n\r");
                        send_to_char(buf, ch);
                        return;
                }
                xprintf(buf, "You crecall from combat!\n\r");
                send_to_char(buf, ch);
                stop_fighting(ch, TRUE);
        }

        act("$n disappears.", ch, NULL, NULL, TO_ROOM);
        char_from_room(ch);
        char_to_room(ch, location);
        act("$n appears in the room.", ch, NULL, NULL, TO_ROOM);
        do_look(ch, "auto");
        if ((mount = ch->mount) == NULL)
                return;
        char_from_room(mount);
        char_to_room(mount, ch->in_room);
        return;
}

void do_home(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg);

        if (IS_NPC(ch))
                return;

        if (arg[0] == '\0' || str_cmp(arg, "here"))
        {
                send_to_char
                        ("If you wish this to be your room, you must type 'home here'.\n\r",
                         ch);
                return;
        }

        if (ch->in_room->vnum == ch->home)
        {
                send_to_char("But this is already your home!\n\r", ch);
                return;
        }

        if (IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) ||
            IS_SET(ch->in_room->room_flags, ROOM_ASTRAL)
            || IS_SET(ch->in_room->area->areabits, AREA_BIT_NOHOME)
            || IS_SET(ch->in_room->room_flags, ROOM_SAFE))
        {
                send_to_char
                        ("You are unable to make this room your home.\n\r",
                         ch);
                return;
        }
        if (ch->in_room->vnum == 3001)
        {
                send_to_char
                        ("You are unable to make this room your home.\n\r",
                         ch);
                return;
        }
        ch->home = ch->in_room->vnum;
        send_to_char("This room is now your home.\n\r", ch);

        return;
}

void do_escape(CHAR_DATA * ch, char *argument)
{
        char      buf[MAX_STRING_LENGTH];
        ROOM_INDEX_DATA *location;

        if (IS_NPC(ch) || !IS_HERO(ch))
                return;

        if (ch->position >= POS_SLEEPING)
        {
                send_to_char("You can only do this if you are dying.\n\r",
                             ch);
                return;
        }
        if ((location = get_room_index(ROOM_VNUM_ESCAPE)) == NULL)
        {
                send_to_char("You are completely lost.\n\r", ch);
                return;
        }
        if (ch->in_room != NULL)
        {
                if (IS_SET(ch->in_room->room_flags, ROOM_ARENA))
                {
                        send_to_char("No silly.\n\r", ch);
                        return;
                }
        }
        if (in_fortress(ch))
        {
                send_to_char("Theres nowhere to run.\n\r", ch);
                return;
        }
        if (ch->in_room == location)
                return;
        ch->move = 0;
        ch->mana = 0;
        act("$n fades out of existance.", ch, NULL, NULL, TO_ROOM);
        char_from_room(ch);
        char_to_room(ch, location);
        act("$n fades into existance.", ch, NULL, NULL, TO_ROOM);
        do_look(ch, "auto");
        xprintf(buf,
                "%s has escaped #Gdefenceless#n, easy kill for the quick.",
                ch->pcdata->switchname);
	ch->combat = NULL;
        do_info(ch, buf);
        if (IS_SET(ch->newbits, NEW_DARKNESS))
        {
                REMOVE_BIT(ch->newbits, NEW_DARKNESS);
                if (ch->in_room != NULL)
                        if (IS_SET
                            (ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
                                REMOVE_BIT(ch->in_room->room_flags,
                                           ROOM_TOTAL_DARKNESS);
                return;
        }
        return;
}


int disc_points_needed(CHAR_DATA * ch)
{

        if (IS_NPC(ch))
                return 1;

        return (ch->power[ch->pcdata->disc_research] + 1) * 10;
}

void gain_disc_points(CHAR_DATA * ch, int points)
{
        if (IS_NPC(ch))
                return;

        if (ch->pcdata->disc_points == 666)
                return;
        if (ch->pcdata->disc_research == -1)
                return;

        ch->pcdata->disc_points += points;

        if (ch->pcdata->disc_points < 0)
        {
                ch->pcdata->disc_points = 0;
                return;
        }

        if (ch->pcdata->disc_points >= disc_points_needed(ch))
        {
                ch->pcdata->disc_points = 999;
                WAIT_STATE(ch, 6);
                stc("#BYou have finished researching your discipline.\n\r",
                    ch);
                stc("You may now use the 'train' command.#n\n\r", ch);
                return;
        }
}


void do_research(CHAR_DATA * ch, char *argument)
{
        int       i;
        int       needed;
        int       maxlevel;
        char      buf[MAX_STRING_LENGTH];

        maxlevel = 0;

        if (IS_CLASS(ch, CLASS_VAMPIRE))
        {
                if (ch->pcdata->rank == AGE_CHILDE
                    || ch->pcdata->rank == AGE_NEONATE)
                        maxlevel = 5;
                else if (ch->pcdata->rank == AGE_ANCILLA)
                        maxlevel = 7;
                else if (ch->pcdata->rank == AGE_ELDER)
                        maxlevel = 9;
                else
                        maxlevel = 10;
        }
        else if (IS_CLASS(ch, CLASS_FAE))
        {
                maxlevel = 2;
                if (ch->pcdata->powers[FAE_PLASMA] > 3)
                        maxlevel++;
                if (ch->pcdata->powers[FAE_MATTER] > 3)
                        maxlevel++;
                if (ch->pcdata->powers[FAE_ENERGY] > 3)
                        maxlevel++;
                if (ch->pcdata->powers[FAE_WILL] > 3)
                        maxlevel++;
                if (ch->pcdata->powers[FAE_PLASMA] > 7)
                        maxlevel++;
                if (ch->pcdata->powers[FAE_MATTER] > 7)
                        maxlevel++;
                if (ch->pcdata->powers[FAE_ENERGY] > 7)
                        maxlevel++;
                if (ch->pcdata->powers[FAE_WILL] > 7)
                        maxlevel++;
        }
        else
                maxlevel = 10;

        if (IS_NPC(ch))
                return;


        if (!str_cmp(argument, "cancel"))
        {
                if (ch->pcdata->disc_research == -1)
                {
                        stc("You are not currently researching a discipline.\n\r", ch);
                        return;
                }
                stc("You stop your research.\n\r", ch);
                ch->pcdata->disc_research = -1;
                ch->pcdata->disc_points = 0;
                return;
        }

        if (argument[0] == '\0')
        {
                stc("What do you wish to research.\n\r", ch);
                return;
        }

        if (ch->pcdata->disc_research != -1)
        {
                stc("You are already researching a discipline.\n\r", ch);
                stc("Type 'research cancel' to stop.\n\r", ch);
                return;
        }

        for (i = 1; i < MAX_DISCIPLINES; i++)
        {
                if (discipline[i][0] != '\0'
                    && !str_prefix(argument, discipline[i]))
                {
                        needed = ((ch->power[i] - 5) * 10);
                        if (ch->power[i] < 0)
                        {
                                stc("You don't know any disciplines of that name.\n\r", ch);
                                return;
                        }

                        if (IS_CLASS(ch, CLASS_VAMPIRE)
                            && ch->power[i] >= maxlevel)
                        {
                                stc("You need to age more before progressing in this discipline.\n\r", ch);
                                return;
                        }
                        if (IS_CLASS(ch, CLASS_FAE)
                            && ch->power[i] >= maxlevel)
                        {
                                stc("You are not ready to go beyond that level yet.\n\r", ch);
                                return;
                        }
                        if (ch->power[i] >= 10)
                        {
                                stc("You have reached full mastery of this discipline.\n\r", ch);
                                return;
                        }

                        xprintf(buf, "You begin your research into %s.\n\r",
                                discipline[i]);
                        stc(buf, ch);
                        ch->pcdata->disc_points = 0;
                        ch->pcdata->disc_research = i;
                        return;
                }
        }
        stc("You don't know any disciplines of that name.\n\r", ch);
}

const char *wwgift[MAX_GIFTS] = {
        "",
        "Homid", "Metis",
        "Lupus", "Ragabash",
        "Theurge", "Philodox",
        "Galliard", "Ahroun",
        "Black Furies", "Bone Gnawers",
        "Children of Gaia", "Fianna",
        "Get of Fenris", "Glass Walkers",
        "Red Talons", "Shadow Lords",
        "Silent Striders", "Silver Fangs",
        "Stargazers", "Uktena",
        "Wendigo"
};

const char *discipline[MAX_DISCIPLINES] = {
        "", "nature",
        "celerity", "fortitude",
        "obtenebration", "presence",
        "quietus", "thaumaturgy",
        "auspex", "dominate",
        "obfuscate", "potence",
        "protean", "serpentis",
        "vicissitude", "daimoinon",
        "animalism", "arcane",

/* Werewolf */

        "bear", "lynx",
        "boar", "owl",
        "spider", "wolf",
        "hawk", "mantis",
        "raptor", "luna",
        "pain", "congregation",

/* Daemon */
        "hellfire", "attack",
        "temptation", "morphosis",
        "corruption", "geluge",
        "discord", "nether",
        "immunae", "chimerstry",
        "thanatosis", "obeah",
        "necromancy", "melpominee",

/* Drow */

        "groundhog",             "rodent",
        "arachnid",            "jellyfish",
        "chameleon",            "bat",
        "crustation",                       "raven",
        "pirana"

};

void do_disciplines(CHAR_DATA * ch, char *argument)
{
        char      buf[MAX_INPUT_LENGTH];
        char      buf2[MAX_INPUT_LENGTH];
        int       loop;
        int       percent = 0, i;
        int       indent = 0;

        if (IS_NPC(ch))
                return;

        if (!IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_VAMPIRE)
            && !IS_CLASS(ch, CLASS_DEMON) && !IS_CLASS(ch, CLASS_FAE))
        {
                send_to_char("Huh.\n\r", ch);
                return;
        }

        if (IS_CLASS(ch, CLASS_WEREWOLF))
                xprintf(buf, "#0---===#W[ #RGuardians #W]#0===---#n");
        else
                xprintf(buf, "#0---===#W[ #RDisciplines #W]#0===---#n");


        cent_to_char(buf, ch);
        send_to_char("\n\r", ch);

        for (loop = 1; loop < MAX_DISCIPLINES; loop++)
        {
                if (discipline[loop][0] != '\0'
                    && strlen(discipline[loop]) != 0 && ch->power[loop] >= 0)
                {
                        xprintf_2(buf2, "%s", discipline[loop]);
                        buf2[0] = UPPER(buf2[0]);

                        xprintf(buf, "     #0%-14s#B: #w%-2d#n",
                                buf2, ch->power[loop]);
                        send_to_char(buf, ch);
                        indent++;
                        if (indent == 3)
                        {
                                stc("\n\r", ch);
                                indent = 0;
                        }
                }
        }
        if (indent != 0)
                stc("\n\r", ch);

        /*
         * This tells them if they are currently researching 
         */
        if (ch->pcdata->disc_research < 0)
        {
                send_to_char("\n\r", ch);
                divide6_to_char(ch);
                return;
        }

        if (ch->power[ch->pcdata->disc_research] < 0)
        {
                send_to_char("\n\r", ch);
                divide6_to_char(ch);
                return;
        }

        if (ch->pcdata->disc_points == 999)
        {
                stc("\n\r", ch);
                xprintf(buf, "You have finished researching %s.",
                        discipline[ch->pcdata->disc_research]);
                cent_to_char(buf, ch);
                stc("\n\r", ch);
                divide6_to_char(ch);
                return;
        }

        if (ch->pcdata->disc_research < MAX_DISCIPLINES)
        {
                send_to_char("\n\r", ch);
                xprintf(buf, "You are currently researching %s.",
                        discipline[ch->pcdata->disc_research]);
                cent_to_char(buf, ch);
        }

        percent = ch->pcdata->disc_points * 40 / disc_points_needed(ch);

        xprintf(buf, "-[#R");

        for (i = 0; i < percent; i++)
                strcat(buf, "*");

        for (i = percent; i < 40; i++)
                strcat(buf, " ");

        strcat(buf, "#n]-");

        cent_to_char(buf, ch);
}

void do_relearn(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_STRING_LENGTH];

        argument = one_argument(argument, arg1);

        if (IS_NPC(ch))
                return;

        if (!str_cmp(arg1, "slash") || !str_cmp(arg1, "slice"))
        {
                if (IS_IMMUNE(ch, IMM_SLASH))
                {
                        REMOVE_BIT(ch->immune, IMM_SLASH);
                        send_to_char
                                ("You forget Slash & Slice resistances.\n\r",
                                 ch);
                        return;
                }
        }
        if (!strcmp(arg1, "stab") || !str_cmp(arg1, "pierce"))
        {
                if (IS_IMMUNE(ch, IMM_STAB))
                {
                        REMOVE_BIT(ch->immune, IMM_STAB);
                        send_to_char
                                ("You forget Stab & Pierce resistances.\n\r",
                                 ch);
                        return;
                }
        }
        if (!strcmp(arg1, "smash") || !str_cmp(arg1, "pound")
            || !str_cmp(arg1, "blast") || !str_cmp(arg1, "crush"))
        {
                if (IS_IMMUNE(ch, IMM_SMASH))
                {
                        REMOVE_BIT(ch->immune, IMM_SMASH);
                        send_to_char
                                ("You forget Pound, Blast & Crush resistances.\n\r",
                                 ch);
                        return;
                }
        }
        if (!strcmp(arg1, "beast") || !str_cmp(arg1, "claw")
            || !str_cmp(arg1, "bite"))
        {
                if (IS_IMMUNE(ch, IMM_ANIMAL))
                {
                        REMOVE_BIT(ch->immune, IMM_ANIMAL);
                        send_to_char
                                ("You forget Claw & Bite resistances.\n\r",
                                 ch);
                        return;
                }
        }
        if (!strcmp(arg1, "grab") || !str_cmp(arg1, "grep")
            || !str_cmp(arg1, "whip") || !str_cmp(arg1, "suck"))
        {
                if (IS_IMMUNE(ch, IMM_MISC))
                {
                        REMOVE_BIT(ch->immune, IMM_MISC);
                        send_to_char
                                ("You forget Grep, Whip & Suck resistances.\n\r",
                                 ch);
                        return;
                }
        }
        if (!strcmp(arg1, "charm"))
        {
                if (IS_IMMUNE(ch, IMM_CHARM))
                {
                        REMOVE_BIT(ch->immune, IMM_CHARM);
                        send_to_char("You forget Charm immunity.\n\r", ch);
                        return;
                }
        }
        if (!strcmp(arg1, "heat"))
        {
                if (IS_IMMUNE(ch, IMM_HEAT))
                {
                        REMOVE_BIT(ch->immune, IMM_HEAT);
                        send_to_char("You forget Heat immunity.\n\r", ch);
                        return;
                }
        }
        if (!strcmp(arg1, "cold"))
        {
                if (IS_IMMUNE(ch, IMM_COLD))
                {
                        REMOVE_BIT(ch->immune, IMM_COLD);
                        send_to_char("You forget Cold immunity.\n\r", ch);
                        return;
                }
        }
        if (!strcmp(arg1, "lightning"))
        {
                if (IS_IMMUNE(ch, IMM_LIGHTNING))
                {
                        REMOVE_BIT(ch->immune, IMM_LIGHTNING);
                        send_to_char("You forget Lightning immunity.\n\r",
                                     ch);
                        return;
                }
        }
        if (!strcmp(arg1, "acid"))
        {
                if (IS_IMMUNE(ch, IMM_ACID))
                {
                        REMOVE_BIT(ch->immune, IMM_ACID);
                        send_to_char("You forget Acid immunity.\n\r", ch);
                        return;
                }
        }
        if (!strcmp(arg1, "cold"))
        {
                if (IS_IMMUNE(ch, IMM_COLD))
                {
                        REMOVE_BIT(ch->immune, IMM_COLD);
                        send_to_char("You forget Cold immunity.\n\r", ch);
                        return;
                }
        }
        if (!strcmp(arg1, "drain"))
        {
                if (IS_IMMUNE(ch, IMM_DRAIN))
                {
                        REMOVE_BIT(ch->immune, IMM_DRAIN);
                        send_to_char("You forget Drain immunity.\n\r", ch);
                        return;
                }
        }
        if (!strcmp(arg1, "voodoo"))
        {
                if (IS_IMMUNE(ch, IMM_VOODOO))
                {
                        REMOVE_BIT(ch->immune, IMM_VOODOO);
                        send_to_char("You forget Voodoo immunity.\n\r", ch);
                        return;
                }
        }
        if (!strcmp(arg1, "hurl"))
        {
                if (IS_IMMUNE(ch, IMM_HURL))
                {
                        REMOVE_BIT(ch->immune, IMM_HURL);
                        send_to_char("You forget Hurl immunity.\n\r", ch);
                        return;
                }
        }
        if (!strcmp(arg1, "backstab"))
        {
                if (IS_IMMUNE(ch, IMM_BACKSTAB))
                {
                        REMOVE_BIT(ch->immune, IMM_BACKSTAB);
                        send_to_char("You forget Backstab immunity.\n\r", ch);
                        return;
                }
        }
        if (!strcmp(arg1, "kick"))
        {
                if (IS_IMMUNE(ch, IMM_KICK))
                {
                        REMOVE_BIT(ch->immune, IMM_KICK);
                        send_to_char("You forget Kick immunity.\n\r", ch);
                        return;
                }
        }
        if (!strcmp(arg1, "disarm"))
        {
                if (IS_IMMUNE(ch, IMM_DISARM))
                {
                        REMOVE_BIT(ch->immune, IMM_DISARM);
                        send_to_char("You forget Disarm immunity.\n\r", ch);
                        return;
                }
        }
        if (!strcmp(arg1, "steal"))
        {
                if (IS_IMMUNE(ch, IMM_STEAL))
                {
                        REMOVE_BIT(ch->immune, IMM_STEAL);
                        send_to_char("You forget Steal immunity.\n\r", ch);
                        return;
                }
        }
        else
        {
                send_to_char
                        ("Command: relearn an already learned immunity/resistance.\n\r",
                         ch);
                return;
        }
}

void do_train(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_STRING_LENGTH];
        char      arg2[MAX_STRING_LENGTH];
        char      buf[MAX_STRING_LENGTH];
        sh_int   *pAbility = NULL;
        char     *pOutput = NULL;
        int       cost;
        int       magic;
        int       immcost;
        int       primal;
        int       silver;
        int       gnosis;
        int       ancilla;
        int       methuselah;
        int       lamagra;
        int       trueblood;
        int       elder;
        int       beast;
        int       belt1;
        int       belt2;
        int       belt3;
        int       belt4;
        int       belt5;
        int       belt6;
        int       belt7;
        int       belt8;
        int       belt9;
        int       belt10;
        int       max_stat = 100;
        bool      last = TRUE;
        bool      is_ok = FALSE;
        int       loop;
        int       pow;
        int       cmax = 200;
        int       permstat;
	int rank_xp = 50000000 * ch->pRank;
	int rank_bones = 8000 * ch->pRank;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);

        if (IS_NPC(ch))
                return;

        switch (ch->class)
        {
        default:
                cmax = 200;
                break;
        case CLASS_LICH:
                cmax = 320;
                break;
        case CLASS_MAGE:
                cmax = 240;
                break;
        }

        if (IS_CLASS(ch, CLASS_SAMURAI)
	|| IS_RACE(ch, RACE_ARCHDEMON)
	|| IS_RACE(ch, RACE_ARCHANGEL))
                permstat = 110;
        else
                permstat = 100;


   if( !str_cmp( arg1, "rank" ) ) {
      if( ch->pRank >= MAX_RANKING ) {
         send_to_char( "Already Maximum Rank.\n\r", ch );
         return;
      }
      if( ch->exp < rank_xp ) {
         sprintf( buf, "Not enough Experience points, you need %d more.\n\r", ( rank_xp - ch->exp ) );
         send_to_char( buf, ch );
         return;
      }
      if( ch->bones < rank_bones ) {
         sprintf( buf, "Not enough bones, you need %d more.\n\r", ( rank_bones - ch->bones ) );
         send_to_char( buf, ch );
         return;
      }
      ch->bones -= rank_bones;
      ch->exp -= rank_xp;
      ch->pRank++;
      sprintf( buf, "%s is now known as %s %s", ch->pcdata->switchname, getCurrRanking( ch, ch->pRank ), 
ch->pcdata->switchname );
      do_info( ch, buf );
      return;
   }

        if (IS_CLASS(ch, CLASS_HOBBIT) && !IS_NPC(ch))
        {
                if (!str_cmp(arg1, "combatskills")
                    && ch->pcdata->powers[HOBBIT_COMBAT] < 5)
                {
                        if (ch->exp < 10000000)
                        {
                                send_to_char
                                        ("You don't have the 10 mill exp.\n\r",
                                         ch);
                                return;
                        }
                        ch->exp -= 10000000;
                        ch->pcdata->powers[HOBBIT_COMBAT]++;
                        send_to_char("Ok.\n\r", ch);
                        return;
                }
        }

        if (IS_CLASS(ch, CLASS_TANARRI) && !IS_NPC(ch))
        {
                if (!str_cmp(arg1, "fodder") && ch->pcdata->rank == 0)
                {
                        if (ch->exp < 10000000)
                        {
                                send_to_char("You don't have the exp.\n\r",
                                             ch);
                                return;
                        }
                        ch->exp -= 10000000;
                        ch->pcdata->rank++;
                        xprintf(buf, "%s is now fodder in the Blood Wars!",
                                ch->pcdata->switchname);
                        do_info(ch, buf);
                        return;
                }
                if (!str_cmp(arg1, "fighter") && ch->pcdata->rank == 1)
                {
                        if (ch->pcdata->powers[TANARRI_POWER_COUNTER] < 3)
                        {
                                send_to_char
                                        ("You still haven't gotten all powers, FODDER!\n\r",
                                         ch);
                                return;
                        }
                        if (ch->exp < 20000000)
                        {
                                send_to_char("You don't have the exp.\n\r",
                                             ch);
                                return;
                        }
                        ch->exp -= 20000000;
                        ch->pcdata->rank++;
                        xprintf(buf, "%s is now a fighter in the Blood Wars!",
                                ch->pcdata->switchname);
                        do_info(ch, buf);
                        return;
                }
                if (!str_cmp(arg1, "elite") && ch->pcdata->rank == 2)
                {
                        if (ch->pcdata->powers[TANARRI_POWER_COUNTER] < 6)
                        {
                                send_to_char
                                        ("You still haven't gotten all powers, FIGHTER!\n\r",
                                         ch);
                                return;
                        }
                        if (ch->exp < 40000000)
                        {
                                send_to_char("You don't have the exp.\n\r",
                                             ch);
                                return;
                        }
                        ch->exp -= 40000000;
                        ch->pcdata->rank++;
                        xprintf(buf,
                                "%s is now an elite warrior in the Blood Wars!",
                                ch->pcdata->switchname);
                        do_info(ch, buf);
                        return;
                }
                if (!str_cmp(arg1, "captain") && ch->pcdata->rank == 3)
                {
                        if (ch->pcdata->powers[TANARRI_POWER_COUNTER] < 9)
                        {
                                send_to_char
                                        ("You still haven't gotten all powers, SOLDIER!\n\r",
                                         ch);
                                return;
                        }
                        if (ch->exp < 80000000)
                        {
                                send_to_char("You don't have the exp.\n\r",
                                             ch);
                                return;
                        }
                        ch->exp -= 80000000;
                        ch->pcdata->rank++;
                        xprintf(buf, "%s is now a captain in the Blood Wars!",
                                ch->pcdata->switchname);
                        do_info(ch, buf);
                        return;
                }
                if (!str_cmp(arg1, "warlord") && ch->pcdata->rank == 4)
                {
                        if (ch->pcdata->powers[TANARRI_POWER_COUNTER] < 12)
                        {
                                send_to_char
                                        ("You still haven't gotten all powers, CAPTAIN!\n\r",
                                         ch);
                                return;
                        }
                        if (ch->exp < 160000000)
                        {
                                send_to_char("You don't have the exp.\n\r",
                                             ch);
                                return;
                        }
                        ch->exp -= 160000000;
                        ch->pcdata->rank++;
                        xprintf(buf, "%s is now a warlord in the Blood Wars!",
                                ch->pcdata->switchname);
                        do_info(ch, buf);
                        return;
                }
                if (!str_cmp(arg1, "balor") && ch->pcdata->rank == 5)
                {
                        if (ch->pcdata->powers[TANARRI_POWER_COUNTER] < 15)
                        {
                                send_to_char
                                        ("You still haven't gotten all powers, MR. WARLORD!\n\r",
                                         ch);
                                return;
                        }
                        if (ch->exp < 320000000)
                        {
                                send_to_char("You don't have the exp.\n\r",
                                             ch);
                                return;
                        }
                        ch->exp -= 320000000;
                        ch->pcdata->rank++;
                        xprintf(buf, "%s is now a Tanar'ri Balor!",
                                ch->pcdata->switchname);
                        do_info(ch, buf);
                        return;
                }
        }
		if (!str_cmp(arg1,"earth"))
		{
			if (!IS_CLASS(ch, CLASS_ELEMENTAL) || ch->pcdata->powers[ELEMENTAL_EARTH] > 4)
			{
			do_train(ch,"");
			return;
			}
			if (ch->exp < (ch->pcdata->powers[ELEMENTAL_EARTH]+1)*10000000)
			{
			send_to_char("You don't have enought exp.\n\r",ch);
			return;
			}
			ch->pcdata->powers[ELEMENTAL_EARTH]++;
			ch->exp -= ch->pcdata->powers[ELEMENTAL_EARTH]*10000000;
			send_to_char("Ok.\n\r",ch);
			return;
		}
		if (!str_cmp(arg1,"air"))
		{
			if (!IS_CLASS(ch, CLASS_ELEMENTAL) || ch->pcdata->powers[ELEMENTAL_AIR] > 4)
			{
			do_train(ch,"");
			return;
			}
			if (ch->exp < (ch->pcdata->powers[ELEMENTAL_AIR]+1)*10000000)
			{
			send_to_char("You don't have enought exp.\n\r",ch);
			return;
			}
			ch->pcdata->powers[ELEMENTAL_AIR]++;
			ch->exp -= ch->pcdata->powers[ELEMENTAL_AIR]*10000000;
			send_to_char("Ok.\n\r",ch);
			return;
		}
		if (!str_cmp(arg1,"water"))
		{
			if (!IS_CLASS(ch, CLASS_ELEMENTAL) || ch->pcdata->powers[ELEMENTAL_WATER] > 4)
			{
			do_train(ch,"");
			return;
			}
			if (ch->exp < (ch->pcdata->powers[ELEMENTAL_WATER]+1)*10000000)
			{
			send_to_char("You don't have enought exp.\n\r",ch);
			return;
			}
			ch->pcdata->powers[ELEMENTAL_WATER]++;
			ch->exp -= ch->pcdata->powers[ELEMENTAL_WATER]*10000000;
			send_to_char("Ok.\n\r",ch);
			return;
		}
		if (!str_cmp(arg1,"fire"))
		{
			if (!IS_CLASS(ch, CLASS_ELEMENTAL) || ch->pcdata->powers[ELEMENTAL_FIRE] > 4)
			{
			do_train(ch,"");
			return;
			}
			if (ch->exp < (ch->pcdata->powers[ELEMENTAL_FIRE]+1)*10000000)
			{
			send_to_char("You don't have enought exp.\n\r",ch);
			return;
			}
			ch->pcdata->powers[ELEMENTAL_FIRE]++;
			ch->exp -= ch->pcdata->powers[ELEMENTAL_FIRE]*10000000;
			send_to_char("Ok.\n\r",ch);
			return;
		}
        if (!str_cmp(arg1, "justice"))
        {
                if (!IS_CLASS(ch, CLASS_ANGEL)
                    || ch->pcdata->powers[ANGEL_JUSTICE] > 4)
                {
                        do_train(ch, "");
                        return;
                }
                if (ch->exp <
                    (ch->pcdata->powers[ANGEL_JUSTICE] + 1) * 10000000)
                {
                        send_to_char("You don't have enought exp.\n\r", ch);
                        return;
                }
                ch->pcdata->powers[ANGEL_JUSTICE]++;
                ch->exp -= ch->pcdata->powers[ANGEL_JUSTICE] * 10000000;
                send_to_char("Ok.\n\r", ch);
                return;
        }
        if (!str_cmp(arg1, "love"))
        {
                if (!IS_CLASS(ch, CLASS_ANGEL)
                    || ch->pcdata->powers[ANGEL_LOVE] > 4)
                {
                        do_train(ch, "");
                        return;
                }
                if (ch->exp < (ch->pcdata->powers[ANGEL_LOVE] + 1) * 10000000)
                {
                        send_to_char("You don't have enought exp.\n\r", ch);
                        return;
                }
                ch->pcdata->powers[ANGEL_LOVE]++;
                ch->exp -= ch->pcdata->powers[ANGEL_LOVE] * 10000000;
                send_to_char("Ok.\n\r", ch);
                return;
        }
        if (!str_cmp(arg1, "harmony"))
        {
                if (!IS_CLASS(ch, CLASS_ANGEL)
                    || ch->pcdata->powers[ANGEL_HARMONY] > 4)
                {
                        do_train(ch, "");
                        return;
                }
                if (ch->exp <
                    (ch->pcdata->powers[ANGEL_HARMONY] + 1) * 10000000)
                {
                        send_to_char("You don't have enought exp.\n\r", ch);
                        return;
                }
                ch->pcdata->powers[ANGEL_HARMONY]++;
                ch->exp -= ch->pcdata->powers[ANGEL_HARMONY] * 10000000;
                send_to_char("Ok.\n\r", ch);
                return;
        }
        if (!str_cmp(arg1, "peace"))
        {
                if (!IS_CLASS(ch, CLASS_ANGEL)
                    || ch->pcdata->powers[ANGEL_PEACE] > 4)
                {
                        do_train(ch, "");
                        return;
                }
                if (ch->exp <
                    (ch->pcdata->powers[ANGEL_PEACE] + 1) * 10000000)
                {
                        send_to_char("You don't have enought exp.\n\r", ch);
                        return;
                }
                ch->pcdata->powers[ANGEL_PEACE]++;
                ch->exp -= ch->pcdata->powers[ANGEL_PEACE] * 10000000;
                send_to_char("Ok.\n\r", ch);
                return;
        }
        if (!str_cmp(arg1, "primal"))
        {
                int       amount = 0;
                int       urin = 0;
                int       urin_counter = 0;

                if (!is_number(arg2) && strcmp(arg2, "all"))
                {
                        send_to_char("Please enter a numeric value.\n\r", ch);
                        return;
                }
                if (is_number(arg2))
                {
                        amount = atoi(arg2);
                        if (amount < 1 || amount > 500)
                        {
                                send_to_char
                                        ("Please enter a value between 1 and 500.\n\r",
                                         ch);
                                return;
                        }
                }
                else
                {
                        amount = 100000;
                }

                for (urin = 0; urin < amount; urin++)
                {
                        if (ch->exp >= (ch->practice + 1) * 500)
                        {
                                ch->practice++;
                                ch->exp -= ch->practice * 500;
                                urin_counter++;
                        }
                }
                if (urin_counter == 0)
                {
                        send_to_char
                                ("You need more exp to gain any primal.\n\r",
                                 ch);
                }
                if (urin_counter == 1)
                {
                        send_to_char("You gain 1 primal.\n\r", ch);
                }
                if (urin_counter > 1)
                {
                        xprintf(buf, "You gain %d primal.\n\r", urin_counter);
                        send_to_char(buf, ch);
                }
                return;
        }


        if (!str_cmp(arg1, "purple"))
        {
                int       amount = 0;
                int       urin = 0;
                int       urin_counter = 0;

                if (ch->spl[0] >= cmax)
                {
                        send_to_char
                                ("You've reached the class spell level max.\n\r",
                                 ch);
                        return;
                }
                if (!is_number(arg2) && strcmp(arg2, "all"))
                {
                        send_to_char
                                ("Please enter a numeric value or use 'all'.\n\r",
                                 ch);
                        return;
                }
                if (is_number(arg2))
                {
                        amount = atoi(arg2);
                        if (amount < 1 || amount > (cmax - ch->spl[0]))
                        {
                                xprintf(buf,
                                        "Please enter a value between 1 and %d.\n\r",
                                        cmax - ch->spl[0]);
                                send_to_char(buf, ch);
                                return;
                        }
                }
                else
                        amount = 10000;

                for (urin = 0; urin < amount; urin++)
                {
                        if (ch->bones >= UMAX((ch->spl[0] * 2), 1)
                            && ch->spl[0] < cmax)
                        {
                                ch->spl[0]++;
                                ch->bones -= ch->spl[0] * 2;
                                urin_counter++;
                        }
                        else
                        {
                                send_to_char
                                        ("You cannot afford this the upgrade\n\r",
                                         ch);
                                return;
                        }
                }
                if (urin_counter == 0)
                        send_to_char
                                ("You need more bones to gain any more spell level.\n\r",
                                 ch);
                else if (urin_counter == 1)
                        send_to_char("You gain 1 point of purple.\n\r", ch);
                else
                {
                        xprintf(buf,
                                "You gain %d points of purple spell level.\n\r",
                                urin_counter);
                        send_to_char(buf, ch);
                }
                return;
        }

        if (!str_cmp(arg1, "blue"))
        {
                int       amount = 0;
                int       urin = 0;
                int       urin_counter = 0;

                if (ch->spl[2] >= cmax)
                {
                        send_to_char
                                ("You've reached the class spell level max.\n\r",
                                 ch);
                        return;
                }
                if (!is_number(arg2) && strcmp(arg2, "all"))
                {
                        send_to_char
                                ("Please enter a numeric value or use 'all'.\n\r",
                                 ch);
                        return;
                }
                if (is_number(arg2))
                {
                        amount = atoi(arg2);
                        if (amount < 1 || amount > (cmax - ch->spl[2]))
                        {
                                xprintf(buf,
                                        "Please enter a value between 1 and %d.\n\r",
                                        cmax - ch->spl[2]);
                                send_to_char(buf, ch);
                                return;
                        }
                }
                else
                        amount = 10000;

                for (urin = 0; urin < amount; urin++)
                {
                        if (ch->bones >= UMAX((ch->spl[2] * 2), 1)
                            && ch->spl[2] < cmax)
                        {
                                ch->spl[2]++;
                                ch->bones -= ch->spl[2] * 2;
                                urin_counter++;
                        }
                        else
                        {
                                send_to_char
                                        ("You cannot afford this the upgrade\n\r",
                                         ch);
                                return;
                        }
                }
                if (urin_counter == 0)
                        send_to_char
                                ("You need more bones to gain any more spell level.\n\r",
                                 ch);
                else if (urin_counter == 1)
                        send_to_char("You gain 1 point of blue.\n\r", ch);
                else
                {
                        xprintf(buf,
                                "You gain %d points of blue spell level.\n\r",
                                urin_counter);
                        send_to_char(buf, ch);
                }
                return;
        }

        if (!str_cmp(arg1, "red"))
        {
                int       amount = 0;
                int       urin = 0;
                int       urin_counter = 0;

                if (ch->spl[1] >= cmax)
                {
                        send_to_char
                                ("You've reached the class spell level max.\n\r",
                                 ch);
                        return;
                }
                if (!is_number(arg2) && strcmp(arg2, "all"))
                {
                        send_to_char
                                ("Please enter a numeric value or use 'all'.\n\r",
                                 ch);
                        return;
                }
                if (is_number(arg2))
                {
                        amount = atoi(arg2);
                        if (amount < 1 || amount > (cmax - ch->spl[1]))
                        {
                                xprintf(buf,
                                        "Please enter a value between 1 and %d.\n\r",
                                        cmax - ch->spl[2]);
                                send_to_char(buf, ch);
                                return;
                        }
                }
                else
                        amount = 10000;

                for (urin = 0; urin < amount; urin++)
                {
                        if (ch->bones >= UMAX((ch->spl[1] * 2), 1)
                            && ch->spl[1] < cmax)
                        {
                                ch->spl[1]++;
                                ch->bones -= ch->spl[1] * 2;
                                urin_counter++;
                        }
                        else
                        {
                                send_to_char
                                        ("You cannot afford this the upgrade\n\r",
                                         ch);
                                return;
                        }
                }
                if (urin_counter == 0)
                        send_to_char
                                ("You need more bones to gain any more spell level.\n\r",
                                 ch);
                else if (urin_counter == 1)
                        send_to_char("You gain 1 point of red.\n\r", ch);
                else
                {
                        xprintf(buf,
                                "You gain %d points of red spell level.\n\r",
                                urin_counter);
                        send_to_char(buf, ch);
                }
                return;
        }

        if (!str_cmp(arg1, "green"))
        {
                int       amount = 0;
                int       urin = 0;
                int       urin_counter = 0;

                if (ch->spl[3] >= cmax)
                {
                        send_to_char
                                ("You've reached the class spell level max.\n\r",
                                 ch);
                        return;
                }
                if (!is_number(arg2) && strcmp(arg2, "all"))
                {
                        send_to_char
                                ("Please enter a numeric value or use 'all'.\n\r",
                                 ch);
                        return;
                }
                if (is_number(arg2))
                {
                        amount = atoi(arg2);
                        if (amount < 1 || amount > (cmax - ch->spl[3]))
                        {
                                xprintf(buf,
                                        "Please enter a value between 1 and %d.\n\r",
                                        cmax - ch->spl[3]);
                                send_to_char(buf, ch);
                                return;
                        }
                }
                else
                        amount = 10000;

                for (urin = 0; urin < amount; urin++)
                {
                        if (ch->bones >= UMAX((ch->spl[3] * 2), 1)
                            && ch->spl[3] < cmax)
                        {
                                ch->spl[3]++;
                                ch->bones -= ch->spl[3] * 2;
                                urin_counter++;
                        }
                        else
                        {
                                send_to_char
                                        ("You cannot afford this the upgrade\n\r",
                                         ch);
                                return;
                        }
                }
                if (urin_counter == 0)
                        send_to_char
                                ("You need more bones to gain any more spell level.\n\r",
                                 ch);
                else if (urin_counter == 1)
                        send_to_char("You gain 1 point of green.\n\r", ch);
                else
                {
                        xprintf(buf,
                                "You gain %d points of green spell level.\n\r",
                                urin_counter);
                        send_to_char(buf, ch);
                }
                return;
        }

        if (!str_cmp(arg1, "yellow"))
        {
                int       amount = 0;
                int       urin = 0;
                int       urin_counter = 0;

                if (ch->spl[4] >= cmax)
                {
                        send_to_char
                                ("You've reached the class spell level max.\n\r",
                                 ch);
                        return;
                }
                if (!is_number(arg2) && strcmp(arg2, "all"))
                {
                        send_to_char
                                ("Please enter a numeric value or use 'all'.\n\r",
                                 ch);
                        return;
                }
                if (is_number(arg2))
                {
                        amount = atoi(arg2);
                        if (amount < 1 || amount > (cmax - ch->spl[4]))
                        {
                                xprintf(buf,
                                        "Please enter a value between 1 and %d.\n\r",
                                        cmax - ch->spl[4]);
                                send_to_char(buf, ch);
                                return;
                        }
                }
                else
                        amount = 10000;

                for (urin = 0; urin < amount; urin++)
                {
                        if (ch->bones >= UMAX((ch->spl[0] * 2), 1)
                            && ch->spl[4] < cmax)
                        {
                                ch->spl[4]++;
                                ch->bones -= ch->spl[4] * 2;
                                urin_counter++;
                        }
                        else
                        {
                                send_to_char
                                        ("You cannot afford this the upgrade\n\r",
                                         ch);
                                return;
                        }
                }
                if (urin_counter == 0)
                        send_to_char
                                ("You need more bones to gain any more spell level.\n\r",
                                 ch);
                else if (urin_counter == 1)
                        send_to_char("You gain 1 point of yellow.\n\r", ch);
                else
                {
                        xprintf(buf,
                                "You gain %d points of yellow spell level.\n\r",
                                urin_counter);
                        send_to_char(buf, ch);
                }
                return;
        }

        if (!str_cmp(arg1, "hp"))
        {
                int       amount = 0;
                int       urin = 0;
                int       urin_counter = 0;
                int       statcap = UMIN(300000, 100000 + 10000 * ch->race);

                if (ch->max_hit >= statcap)
                {
                        send_to_char("You've reached the statcap.\n\r", ch);
                        return;
                }
                if (!is_number(arg2) && strcmp(arg2, "all"))
                {
                        send_to_char
                                ("Please enter a numeric value or use 'all'.\n\r",
                                 ch);
                        return;
                }
                if (is_number(arg2))
                {
                        amount = atoi(arg2);
                        if (amount < 1 || amount > 4000)
                        {
                                send_to_char
                                        ("Please enter a value between 1 and 4000.\n\r",
                                         ch);
                                return;
                        }
                }
                else
                        amount = 100000;
                for (urin = 0; urin < amount; urin++)
                {
                        if (ch->exp >= UMAX((ch->max_hit + 1), 1)
                            && ch->max_hit < statcap)
                        {
                                ch->max_hit++;
                                ch->exp -= UMAX(ch->max_hit, 1);
                                urin_counter++;
                        }
                }
                if (urin_counter == 0)
                        send_to_char("You need more exp to gain any hps.\n\r",
                                     ch);
                else if (urin_counter == 1)
                        send_to_char("You gain 1 hp.\n\r", ch);
                else
                {
                        xprintf(buf, "You gain %d hps.\n\r", urin_counter);
                        send_to_char(buf, ch);
                }
                return;
        }
        if (!str_cmp(arg1, "move"))
        {
                int       amount = 0;
                int       urin = 0;
                int       urin_counter = 0;
                int       statcap = UMIN(300000, 50000 + 10000 * ch->race);

                if (ch->max_move >= statcap)
                {
                        send_to_char("You've reached the statcap.\n\r", ch);
                        return;
                }
                if (!is_number(arg2) && strcmp(arg2, "all"))
                {
                        send_to_char
                                ("Please enter a numeric value or use 'all'.\n\r",
                                 ch);
                        return;
                }
                if (is_number(arg2))
                {
                        amount = atoi(arg2);
                        if (amount < 1 || amount > 4000)
                        {
                                send_to_char
                                        ("Please enter a value between 1 and 4000.\n\r",
                                         ch);
                                return;
                        }
                }
                else
                        amount = 100000;
                for (urin = 0; urin < amount; urin++)
                {
                        if (ch->exp >= UMAX((ch->max_move + 1), 1)
                            && ch->max_move < statcap)
                        {
                                ch->max_move++;
                                ch->exp -= UMAX(1, ch->max_move);
                                urin_counter++;
                        }
                }
                if (urin_counter == 0)
                        send_to_char
                                ("You need more exp to gain more move.\n\r",
                                 ch);
                else if (urin_counter == 1)
                        send_to_char("You gain 1 move.\n\r", ch);
                else
                {
                        xprintf(buf, "You gain %d move.\n\r", urin_counter);
                        send_to_char(buf, ch);
                }
                return;
        }

        if (!str_cmp(arg1, "str"))
        {

                pOutput = "strength";
                cost = figure_stat_cost(ch->pcdata->perm_str);

                if (ch->pcdata->perm_str >= permstat)
                {
                        send_to_char("Your all maxxed out there\n\r", ch);
                        return;
                }

                if (cost > ch->practice)
                {
                        ch_printf(ch,
                                  "It costs %d primal to raise your %s by one.\n\r",
                                  cost, pOutput);
                        return;
                }


                ch->pcdata->perm_str++;
                ch->practice -= cost;
                ch_printf(ch, "You have raised your %s.\n\r", pOutput);
                return;

        }
        if (!str_cmp(arg1, "int"))
        {
                pOutput = "intelligence";
                cost = figure_stat_cost(ch->pcdata->perm_int);

                if (ch->pcdata->perm_int >= permstat)
                {
                        send_to_char("Your all maxxed out there\n\r", ch);
                        return;
                }

                if (cost > ch->practice)
                {
                        ch_printf(ch,
                                  "It costs %d primal to raise your %s by one.\n\r",
                                  cost, pOutput);
                        return;
                }


                ch->pcdata->perm_int++;
                ch->practice -= cost;
                ch_printf(ch, "You have raised your %s.\n\r", pOutput);
                return;

        }
        if (!str_cmp(arg1, "wis"))
        {
                pOutput = "wisdom";
                cost = figure_stat_cost(ch->pcdata->perm_wis);


                if (ch->pcdata->perm_wis >= permstat)
                {
                        send_to_char("Your all maxxed out there\n\r", ch);
                        return;
                }

                if (cost > ch->practice)
                {
                        ch_printf(ch,
                                  "It costs %d primal to raise your %s by one.\n\r",
                                  cost, pOutput);
                        return;
                }

                ch->pcdata->perm_wis++;
                ch->practice -= cost;
                ch_printf(ch, "You have raised your %s.\n\r", pOutput);
                return;

        }
        if (!str_cmp(arg1, "dex"))
        {

                pOutput = "dexterity";
                cost = figure_stat_cost(ch->pcdata->perm_dex);

                if (ch->pcdata->perm_dex >= permstat)
                {
                        send_to_char("Your all maxxed out there\n\r", ch);
                        return;
                }

                if (cost > ch->practice)
                {
                        ch_printf(ch,
                                  "It costs %d primal to raise your %s by one.\n\r",
                                  cost, pOutput);
                        return;
                }

                ch->pcdata->perm_dex++;
                ch->practice -= cost;
                ch_printf(ch, "You have raised your %s.\n\r", pOutput);
                return;

        }
        if (!str_cmp(arg1, "con"))
        {

                pOutput = "constitution";
                cost = figure_stat_cost(ch->pcdata->perm_con);

                if (ch->pcdata->perm_con >= permstat)
                {
                        send_to_char("Your all maxxed out there\n\r", ch);
                        return;
                }

                if (cost > ch->practice)
                {
                        ch_printf(ch,
                                  "It costs %d primal to raise your %s by one.\n\r",
                                  cost, pOutput);
                        return;
                }

                ch->pcdata->perm_con++;
                ch->practice -= cost;
                ch_printf(ch, "You have raised your %s.\n\r", pOutput);
                return;

        }


        if (!str_cmp(arg1, "mana"))
        {
                int       amount = 0;
                int       urin = 0;
                int       urin_counter = 0;
                int       statcap = UMIN(300000, 50000 + 10000 * ch->race);

                if (ch->max_mana >= statcap)
                {
                        send_to_char("You've reached the statcap.\n\r", ch);
                        return;
                }
                if (!is_number(arg2) && strcmp(arg2, "all"))
                {
                        send_to_char
                                ("Please enter a numeric value or use 'all'.\n\r",
                                 ch);
                        return;
                }
                if (is_number(arg2))
                {
                        amount = atoi(arg2);
                        if (amount < 1 || amount > 4000)
                        {
                                send_to_char
                                        ("Please enter a value between 1 and 4000.\n\r",
                                         ch);
                                return;
                        }
                }
                else
                        amount = 100000;
                for (urin = 0; urin < amount; urin++)
                {
                        if (ch->exp >= UMAX((ch->max_mana + 1), 1)
                            && ch->max_mana < statcap)
                        {
                                ch->max_mana++;
                                ch->exp -= UMAX(ch->max_mana, 1);
                                urin_counter++;
                        }
                }
                if (urin_counter == 0)
                        send_to_char
                                ("You need more exp to gain any mana.\n\r",
                                 ch);
                else if (urin_counter == 1)
                        send_to_char("You gain 1 mana.\n\r", ch);
                else
                {
                        xprintf(buf, "You gain %d mana.\n\r", urin_counter);
                        send_to_char(buf, ch);
                }
                return;
        }

        if (IS_NPC(ch))
                return;

        if (arg1[0] == '\0')
        {
                xprintf(buf,
                        "#0You have #Y%2.0f #0experience points and #Y%d #0primal.#n\n\r",
                        ch->exp, ch->practice);
                send_to_char(buf, ch);
                xprintf(arg1, "foo");
        }
        if (!str_cmp(arg1, "str"))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "int"))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "wis"))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "dex"))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "con"))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "hp"))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "mana"))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "move"))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "slash") || !str_cmp(arg1, "smash") ||
                 !str_cmp(arg1, "beast") || !str_cmp(arg1, "grab") ||
                 !str_cmp(arg1, "heat") || !str_cmp(arg1, "cold")
                 || !str_cmp(arg1, "lightning") || !str_cmp(arg1, "acid")
                 || !str_cmp(arg1, "drain") || !str_cmp(arg1, "hurl")
                 || !str_cmp(arg1, "backstab") || !str_cmp(arg1, "kick")
                 || !str_cmp(arg1, "disarm") || !str_cmp(arg1, "steal")
                 || !str_cmp(arg1, "stab"))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "sunlight") && IS_CLASS(ch, CLASS_VAMPIRE))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "ancilla") && IS_CLASS(ch, CLASS_VAMPIRE) &&
                 (ch->pcdata->rank == AGE_NEONATE))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "ancilla") && IS_CLASS(ch, CLASS_VAMPIRE) &&
                 (ch->pcdata->rank == AGE_CHILDE))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "silver") && IS_CLASS(ch, CLASS_WEREWOLF))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "forging")
                 && !IS_SET(ch->pcdata->jflags, JFLAG_KNOWS_FORGING))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "gnosis") && (IS_CLASS(ch, CLASS_WEREWOLF)))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "control") && IS_CLASS(ch, CLASS_VAMPIRE)
                 && ch->beast > 0)
                is_ok = TRUE;
        else if (!str_cmp(arg1, "belt1") && IS_CLASS(ch, CLASS_NINJA)
                 && (ch->pcdata->rank == 0))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "belt2") && IS_CLASS(ch, CLASS_NINJA) &&
                 (ch->pcdata->rank == BELT_ONE))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "belt3") && IS_CLASS(ch, CLASS_NINJA) &&
                 (ch->pcdata->rank == BELT_TWO))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "belt4") && IS_CLASS(ch, CLASS_NINJA) &&
                 (ch->pcdata->rank == BELT_THREE))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "belt5") && IS_CLASS(ch, CLASS_NINJA) &&
                 (ch->pcdata->rank == BELT_FOUR))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "belt6") && IS_CLASS(ch, CLASS_NINJA) &&
                 (ch->pcdata->rank == BELT_FIVE))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "belt7") && IS_CLASS(ch, CLASS_NINJA) &&
                 (ch->pcdata->rank == BELT_SIX))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "belt8") && IS_CLASS(ch, CLASS_NINJA) &&
                 (ch->pcdata->rank == BELT_SEVEN))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "belt9") && IS_CLASS(ch, CLASS_NINJA) &&
                 (ch->pcdata->rank == BELT_EIGHT))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "belt10") && IS_CLASS(ch, CLASS_NINJA) &&
                 (ch->pcdata->rank == BELT_NINE))
                is_ok = TRUE;

        else if (!str_cmp(arg1, "elder") && IS_CLASS(ch, CLASS_VAMPIRE) &&
                 (ch->pcdata->rank == AGE_ANCILLA))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "methuselah") && IS_CLASS(ch, CLASS_VAMPIRE)
                 && (ch->pcdata->rank == AGE_ELDER))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "lamagra") && IS_CLASS(ch, CLASS_VAMPIRE) &&
                 (ch->pcdata->rank == AGE_METHUSELAH))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "trueblood") && IS_CLASS(ch, CLASS_VAMPIRE) &&
                 (ch->pcdata->rank == AGE_LA_MAGRA))
                is_ok = TRUE;
        else if (!str_cmp(arg1, "magic") && IS_CLASS(ch, CLASS_DROW))
                is_ok = TRUE;

        for (loop = 1; loop < MAX_DISCIPLINES; loop++)
        {
                if (!str_prefix(arg1, discipline[loop])
                    && ch->power[loop] > -1 && str_cmp(arg1, "con"))
                {

                        if (!IS_CREATOR(ch))
                        {

                                if (ch->pcdata->disc_research == -1)
                                {
                                        stc("You haven't started researching a discipline yet.\n\r", ch);
                                        return;
                                }

                                if (ch->pcdata->disc_points != 999)
                                {
                                        stc("You haven't finished researching the discipline yet.\n\r", ch);
                                        return;
                                }

                                if (loop != ch->pcdata->disc_research)
                                {
                                        stc("You have not researched that discipline.\n\r", ch);
                                        return;
                                }
                        }

                        ch->power[loop] += 1;
                        ch->pcdata->disc_research = -1;
                        ch->pcdata->disc_points = 0;

                        if (loop == DISC_DAEM_GELU)
                        {
                                if (ch->power[DISC_DAEM_GELU] == 5)
                                        stc("Your skin takes on an icy hardness.\n\r", ch);
                        }

                        /*
                         * DAEMONIC ATTACK LEVELS 1 to 10 
                         */
                        if (loop == DISC_DAEM_ATTA)
                        {
                                pow = ch->power[DISC_DAEM_ATTA];

                                if (pow == 2)
                                {
                                        SET_BIT(ch->pcdata->stats[UNI_AFF],
                                                VAM_CLAWS);
                                        send_to_char
                                                ("You grow a pair of wicked claws.\n\r",
                                                 ch);
                                        act("$n grows a pair of wicked claws.\n\r", ch, NULL, NULL, TO_ROOM);
                                }
                                if (pow == 3)
                                {
                                        SET_BIT(ch->pcdata->stats[UNI_AFF],
                                                VAM_TAIL);
                                        send_to_char
                                                ("Your spine extends into a long tail.\n\r",
                                                 ch);
                                        act("$n's spine extends into a long tail.\n\r", ch, NULL, NULL, TO_ROOM);
                                }
                                if (pow == 4)
                                {
                                        horn(ch);
                                }
                                if (pow == 5)
                                {
                                        SET_BIT(ch->pcdata->stats[UNI_GEN],
                                                VAM_WINGS);
                                        send_to_char
                                                ("A pair of leathery wings grow out of your back.\n\r",
                                                 ch);
                                        act("A pair of leathery wings grow out of $n's back.\n\r", ch, NULL, NULL, TO_ROOM);
                                }
                        }

                        send_to_char("Your mastery of ", ch);
                        send_to_char(discipline[loop], ch);
                        send_to_char(" increases.\n\r", ch);
                        return;
                }
        }


        cost = 200;
        immcost = count_imms(ch);
        belt1 = 5000000;
        belt2 = 10000000;
        belt3 = 15000000;
        belt4 = 20000000;
        belt5 = 25000000;
        belt6 = 30000000;
        belt7 = 35000000;
        belt8 = 40000000;
        belt9 = 45000000;
        belt10 = 50000000;

        primal = (1 + ch->practice) * 500;
        magic = (1 + ch->pcdata->stats[DROW_MAGIC]) * 100;
        silver = (1 + ch->siltol) * 2500;
        gnosis = (1 + ch->gnosis[GMAXIMUM]) * 250000;
        if (ch->beast > 25)
                beast = 5;
        else if (ch->beast > 15)
                beast = 10;
        else if (ch->beast > 10)
                beast = 15;
        else if (ch->beast > 2)
                beast = 20;
        else
                beast = 25;
        ancilla = 1500000;
        elder = 7500000;
        methuselah = 15000000;
        lamagra = 30000000;
        trueblood = 60000000;
        if (!str_cmp(arg1, "str"))
        {
                pAbility = &ch->pcdata->perm_str;
                pOutput = "strength";
        }

        else if (!str_cmp(arg1, "int"))
        {
                pAbility = &ch->pcdata->perm_int;
                pOutput = "intelligence";
        }

        else if (!str_cmp(arg1, "wis"))
        {
                pAbility = &ch->pcdata->perm_wis;
                pOutput = "wisdom";
        }

        else if (!str_cmp(arg1, "dex"))
        {
                pAbility = &ch->pcdata->perm_dex;
                pOutput = "dexterity";
        }

        else if (!str_cmp(arg1, "con"))
        {
                pAbility = &ch->pcdata->perm_con;
                pOutput = "constitution";
        }
        else if (!str_cmp(arg1, "avatar") && (ch->level == 2))
        {
                if (ch->max_hit < 2000)
                {
                        stc("You need 2000 hp to train avatar.\n\r", ch);
                        return;
                }
                do_clearstats2(ch, "");
                if (ch->max_hit < 2000)
                {
                        stc("You need 2000 hp to train avatar.\n\r", ch);
                        return;
                }
                ch->level = 3;
                if (!ragnarok)
                        ch->pcdata->safe_counter = 10;
                else
                        ch->pcdata->safe_counter = 3;
                strip_aggression(ch);
                act("You become an avatar!", ch, NULL, NULL, TO_CHAR);
                if (IS_SET(ch->pcdata->jflags, JFLAG_SETAVATAR))
                        avatar_message(ch);
                else
                {
                        xprintf(buf, "%s has become an avatar!",
                                ch->pcdata->switchname);
                        avatar_info(buf);
                }
                if (!IS_NPC(ch) && IS_VAMPAFF(ch, VAM_MORTAL))
                        do_mortalvamp(ch, "");
                return;
        }

        else if (!str_cmp(arg1, "magic") && IS_CLASS(ch, CLASS_DROW))
        {
                sh_int    quiet_pointer = ch->pcdata->stats[DROW_MAGIC];

                cost = magic;
                pAbility = &quiet_pointer;
                pOutput = "drow magic resistance";
        }

        else if (!str_cmp(arg1, "silver") && IS_CLASS(ch, CLASS_WEREWOLF))
        {
                sh_int    quiet_pointer = ch->siltol;   /* dirrent type of int */

                cost = silver;
                pAbility = &quiet_pointer;
                pOutput = "tolerance to silver";
        }
        else if (!str_cmp(arg1, "gnosis") && IS_CLASS(ch, CLASS_WEREWOLF))
        {
                sh_int    quiet_pointer = ch->gnosis[GMAXIMUM];

                cost = gnosis;
                pAbility = &quiet_pointer;
                pOutput = "gnosis points";
        }
        else if (!str_cmp(arg1, "control") && IS_CLASS(ch, CLASS_VAMPIRE))
        {
                sh_int    quiet_pointer = ch->beast;

                cost = beast;
                pAbility = &quiet_pointer;
                pOutput = "control over your beast";
        }

        else if (!str_cmp(arg1, "ancilla") && IS_CLASS(ch, CLASS_VAMPIRE) &&
                 ((ch->pcdata->rank == AGE_NEONATE)
                  || (ch->pcdata->rank == AGE_CHILDE)))
        {

                if (ch->exp < ancilla)
                {
                        send_to_char
                                ("You need 1500000 exp to become ancilla!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 1500000;
                ch->pcdata->rank = AGE_ANCILLA;
                xprintf(buf, "%s is now an Ancilla!", ch->name);
                do_info(ch, buf);
                send_to_char("You are now an Ancilla.\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "elder") && IS_CLASS(ch, CLASS_VAMPIRE) &&
                 (ch->pcdata->rank == AGE_ANCILLA))
        {
                if (ch->exp < elder)
                {
                        send_to_char
                                ("You need 7500000 exp to become an elder.\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 7500000;
                ch->pcdata->rank = AGE_ELDER;
                xprintf(buf, "%s is now an Elder!", ch->name);
                do_info(ch, buf);
                send_to_char("You are now an Elder!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "methuselah") && IS_CLASS(ch, CLASS_VAMPIRE)
                 && (ch->pcdata->rank == AGE_ELDER))
        {
                if (ch->exp < methuselah)
                {
                        send_to_char
                                ("You need 15000000 exp to become Methuselah!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 15000000;
                ch->pcdata->rank = AGE_METHUSELAH;
                xprintf(buf, "%s is now a Methuselah!", ch->name);
                do_info(ch, buf);
                send_to_char("You are now Methuselah!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "lamagra") && IS_CLASS(ch, CLASS_VAMPIRE)
                 && (ch->pcdata->rank == AGE_METHUSELAH))
        {
                if (ch->exp < lamagra)
                {
                        send_to_char
                                ("You need 30000000 exp to become La Magra!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 30000000;
                ch->pcdata->rank = AGE_LA_MAGRA;
                xprintf(buf, "%s is now a La Magra!", ch->name);
                do_info(ch, buf);
                send_to_char("You are now La Magra!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "trueblood") && IS_CLASS(ch, CLASS_VAMPIRE)
                 && (ch->pcdata->rank == AGE_LA_MAGRA))
        {
                if (ch->exp < trueblood)
                {
                        send_to_char
                                ("You need 60000000 exp to become TrueBlood!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 60000000;
                ch->pcdata->rank = AGE_TRUEBLOOD;
                xprintf(buf, "%s is now a TrueBlood!", ch->name);
                do_info(ch, buf);
                send_to_char("You are now TrueBlood!\n\r", ch);
                return;
        }

        else if (!str_cmp(arg1, "sunlight") && !IS_IMMUNE(ch, IMM_SUNLIGHT)
                 && IS_CLASS(ch, CLASS_VAMPIRE))
        {
                if (ch->exp < 10000000)
                {
                        send_to_char("You dont have enough experience.\n\r",
                                     ch);
                        return;
                }

                ch->exp = ch->exp - 10000000;
                SET_BIT(ch->immune, IMM_SUNLIGHT);
                send_to_char("You are now immune to sunlight.\n\r", ch);
                return;
        }

//Training Forging
        else if (!str_cmp(arg1, "forge")
                 && !IS_SET(ch->pcdata->jflags, JFLAG_KNOWS_FORGING))
        {
                if (ch->exp < 10000000)
                {
                        send_to_char("You dont have enough experience.\n\r",
                                     ch);
                        return;
                }

                ch->exp = ch->exp - 10000000;
                SET_BIT(ch->pcdata->jflags, JFLAG_KNOWS_FORGING);
                send_to_char("You can now forge gems and hilts on items.\n\r",
                             ch);
                return;
        }

        else if (!str_cmp(arg1, "belt1") && IS_CLASS(ch, CLASS_NINJA)
                 && (ch->pcdata->rank < BELT_ONE))
        {
                if (ch->exp < belt1)
                {
                        send_to_char
                                ("You need 5000000 exp to start your traning!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 5000000;
                ch->pcdata->rank = BELT_ONE;
                xprintf(buf, "%s has started martial arts studies!",
                        ch->name);
                do_info(ch, buf);
                send_to_char("You are now a first dan!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "belt2") && IS_CLASS(ch, CLASS_NINJA)
                 && (ch->pcdata->rank == BELT_ONE))
        {
                if (ch->exp < belt2)
                {
                        send_to_char
                                ("You need 10000000 exp to advance your traning!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 10000000;
                ch->pcdata->rank = BELT_TWO;
                xprintf(buf, "%s advances in their martial arts studies!",
                        ch->name);
                do_info(ch, buf);
                send_to_char("You are now a second dan!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "belt3") && IS_CLASS(ch, CLASS_NINJA)
                 && (ch->pcdata->rank == BELT_TWO))
        {
                if (ch->exp < belt3)
                {
                        send_to_char
                                ("You need 15000000 exp to advance your traning!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 15000000;
                ch->pcdata->rank = BELT_THREE;
                xprintf(buf, "%s advances in their martial arts studies!",
                        ch->name);
                do_info(ch, buf);
                send_to_char("You are now a third dan!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "belt4") && IS_CLASS(ch, CLASS_NINJA)
                 && (ch->pcdata->rank == BELT_THREE))
        {
                if (ch->exp < belt4)
                {
                        send_to_char
                                ("You need 20000000 exp to advance your traning!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 20000000;
                ch->pcdata->rank = BELT_FOUR;
                xprintf(buf, "%s advances in their martial arts studies!",
                        ch->name);
                do_info(ch, buf);
                send_to_char("You are now a fourth dan!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "belt5") && IS_CLASS(ch, CLASS_NINJA)
                 && (ch->pcdata->rank == BELT_FOUR))
        {
                if (ch->exp < belt5)
                {
                        send_to_char
                                ("You need 25000000 exp to advance your traning!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 25000000;
                ch->pcdata->rank = BELT_FIVE;
                xprintf(buf, "%s advances in their martial arts studies!",
                        ch->name);
                do_info(ch, buf);
                send_to_char("You are now a fifth dan!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "belt6") && IS_CLASS(ch, CLASS_NINJA)
                 && (ch->pcdata->rank == BELT_FIVE))
        {
                if (ch->exp < belt6)
                {
                        send_to_char
                                ("You need 30000000 exp to advance your traning!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 30000000;
                ch->pcdata->rank = BELT_SIX;
                xprintf(buf, "%s advances in their martial arts studies!",
                        ch->name);
                do_info(ch, buf);
                send_to_char("You are now a sixth dan!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "belt7") && IS_CLASS(ch, CLASS_NINJA)
                 && (ch->pcdata->rank == BELT_SIX))
        {
                if (ch->exp < belt7)
                {
                        send_to_char
                                ("You need 35000000 exp to advance your traning!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 35000000;
                ch->pcdata->rank = BELT_SEVEN;
                xprintf(buf, "%s advances in their martial arts studies!",
                        ch->name);
                do_info(ch, buf);
                send_to_char("You are now a seventh dan!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "belt8") && IS_CLASS(ch, CLASS_NINJA)
                 && (ch->pcdata->rank == BELT_SEVEN))
        {
                if (ch->exp < belt8)
                {
                        send_to_char
                                ("You need 40000000 exp to advance your traning!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 40000000;
                ch->pcdata->rank = BELT_EIGHT;
                xprintf(buf, "%s advances in their martial arts studies!",
                        ch->name);
                do_info(ch, buf);
                send_to_char("You are now an eigth dan!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "belt9") && IS_CLASS(ch, CLASS_NINJA)
                 && (ch->pcdata->rank == BELT_EIGHT))
        {
                if (ch->exp < belt9)
                {
                        send_to_char
                                ("You need 45000000 exp to advance your traning!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 45000000;
                ch->pcdata->rank = BELT_NINE;
                xprintf(buf, "%s advances in their martial arts studies!",
                        ch->name);
                do_info(ch, buf);
                send_to_char("You are now a ninth dan!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "belt10") && IS_CLASS(ch, CLASS_NINJA)
                 && (ch->pcdata->rank == BELT_NINE))
        {
                if (ch->exp < belt10)
                {
                        send_to_char
                                ("You need 50000000 exp to complete your traning!\n\r",
                                 ch);
                        return;
                }
                ch->exp -= 50000000;
                ch->pcdata->rank = BELT_TEN;
                xprintf(buf, "%s has completed their martial arts studies!",
                        ch->name);
                do_info(ch, buf);
                send_to_char("You are now a tenth dan!\n\r", ch);
                return;
        }
        else if (!str_cmp(arg1, "slash") && !IS_IMMUNE(ch, IMM_SLASH))

        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_SLASH);
                send_to_char
                        ("You are now more resistant to slashing and slicing weapons.\n\r",
                         ch);
                return;
        }

        else if (!str_cmp(arg1, "stab") && !IS_IMMUNE(ch, IMM_STAB))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_STAB);
                send_to_char
                        ("You are now more resistant to stabbing and piercing weapons.\n\r",
                         ch);
                return;
        }

        else if (!str_cmp(arg1, "smash") && !IS_IMMUNE(ch, IMM_SMASH))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_SMASH);
                send_to_char
                        ("You are now more resistant to blasting, pounding and crushing weapons.\n\r",
                         ch);
                return;
        }

        else if (!str_cmp(arg1, "beast") && !IS_IMMUNE(ch, IMM_ANIMAL))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_ANIMAL);
                send_to_char
                        ("You are now more resistant to claw and bite attacks.\n\r",
                         ch);
                return;
        }

        else if (!str_cmp(arg1, "grab") && !IS_IMMUNE(ch, IMM_MISC))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_MISC);
                send_to_char
                        ("You are now more resistant to grepping, sucking and whipping weapons.\n\r",
                         ch);
                return;
        }

        else if (!str_cmp(arg1, "charm") && !IS_IMMUNE(ch, IMM_CHARM))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_CHARM);
                send_to_char("You are now immune to charm spells.\n\r", ch);
                return;
        }

        else if (!str_cmp(arg1, "heat") && !IS_IMMUNE(ch, IMM_HEAT))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_HEAT);
                send_to_char
                        ("You are now immune to heat and fire spells.\n\r",
                         ch);
                return;
        }

        else if (!str_cmp(arg1, "cold") && !IS_IMMUNE(ch, IMM_COLD))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_COLD);
                send_to_char("You are now immune to cold spells.\n\r", ch);
                return;
        }

        else if (!str_cmp(arg1, "lightning") && !IS_IMMUNE(ch, IMM_LIGHTNING))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_LIGHTNING);
                send_to_char
                        ("You are now immune to lightning and electrical spells.\n\r",
                         ch);
                return;
        }

        else if (!str_cmp(arg1, "acid") && !IS_IMMUNE(ch, IMM_ACID))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_ACID);
                send_to_char("You are now immune to acid spells.\n\r", ch);
                return;
        }

        else if (!str_cmp(arg1, "drain") && !IS_IMMUNE(ch, IMM_DRAIN))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_DRAIN);
                send_to_char
                        ("You are now immune to the energy drain spell.\n\r",
                         ch);
                return;
        }


        else if (!str_cmp(arg1, "voodoo") && !IS_IMMUNE(ch, IMM_VOODOO))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_VOODOO);
                send_to_char("You are now immune to voodoo magic.\n\r", ch);
                return;
        }

        else if (!str_cmp(arg1, "hurl") && !IS_IMMUNE(ch, IMM_HURL))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_HURL);
                send_to_char("You are now immune to being hurled.\n\r", ch);
                return;
        }

        else if (!str_cmp(arg1, "backstab") && !IS_IMMUNE(ch, IMM_BACKSTAB))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_BACKSTAB);
                send_to_char("You are now immune to being backstabbed.\n\r",
                             ch);
                return;
        }

        else if (!str_cmp(arg1, "kick") && !IS_IMMUNE(ch, IMM_KICK))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_KICK);
                send_to_char("You are now immune to being kicked.\n\r", ch);
                return;
        }

        else if (!str_cmp(arg1, "disarm") && !IS_IMMUNE(ch, IMM_DISARM))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_DISARM);
                send_to_char("You are now immune to being disarmed.\n\r", ch);
                return;
        }

        else if (!str_cmp(arg1, "steal") && !IS_IMMUNE(ch, IMM_STEAL))
        {
                if (ch->exp < immcost)
                {
                        send_to_char("You don't have enough exp.\n\r", ch);
                        return;
                }
                ch->exp = ch->exp - immcost;
                SET_BIT(ch->immune, IMM_STEAL);
                send_to_char("You are now immune to being stolen from.\n\r",
                             ch);
                return;
        }
        else if (!str_cmp(arg1, "tier"))
        {   //<--Top Bracket
                if (ch->tier >= 10)
                {
                        send_to_char("You are on the highest tier.\n\r", ch);
                        return;
                }
                if (ch->tier == 0)
                {
                        int       TKsNeeded = 10 - (ch->tks);
                        int       StatusNeeded = 5 - (ch->race);
                        int       HitNeeded = 25000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        if (TKsNeeded > 0 || StatusNeeded > 0
                            || HitNeeded > 0)
                        {
                                stc("#ROne or more of your tiering requirement have not been met#n\n\r", ch);
                                return;
                        }
                        else
                        {
                                ch->tier += 1;
                                ch->tks = 0;
                                stc("#CCongratulations #Yon gaining a new tier!#n\n\r", ch);
                                xprintf(buf,
                                        "#R%s #Chas just become a more #r-#R=P#YOW#7ER#YFU#RL=#r- #cG#Cod!!#n\n\r",
                                        ch->name);
                                do_info(ch, buf);
                                return;
                        }
                }
//Next One below

                if (ch->tier == 1)
                {
                        int       TKsNeeded = 20 - (ch->tks);
                        int       StatusNeeded = 10 - (ch->race);
                        int       HitNeeded = 50000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        if (TKsNeeded > 0 || StatusNeeded > 0
                            || HitNeeded > 0)
                        {
                                stc("#ROne or more of your tiering requirement have not been met#n\n\r", ch);
                                return;
                        }
                        else
                        {
                                ch->tier += 1;
                                ch->tks = 0;
                                stc("#CCongratulations #yon gaining a new tier!#n\n\r", ch);
                                xprintf(buf,
                                        "#R%s #Chas just become a more #r-#R=P#yOW#7ER#yFU#RL=#r- #cG#Cod!!#n\n\r",
                                        ch->name);
                                do_info(ch, buf);
                                return;
                        }
                }

                if (ch->tier == 2)
                {
                        int       TKsNeeded = 30 - (ch->tks);
                        int       StatusNeeded = 15 - (ch->race);
                        int       HitNeeded = 75000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        if (TKsNeeded > 0 || StatusNeeded > 0
                            || HitNeeded > 0)
                        {
                                stc("#ROne or more of your tiering requirement have not been met#n\n\r", ch);
                                return;
                        }
                        else
                        {
                                ch->tier += 1;
                                ch->tks = 0;
                                stc("#CCongratulations #yon gaining a new tier!#n\n\r", ch);
                                xprintf(buf,
                                        "#R%s #Chas just become a more #r-#R=P#yOW#7ER#yFU#RL=#r- #cG#Cod!!#n\n\r",
                                        ch->name);
                                do_info(ch, buf);
                                return;
                        }
                }

                if (ch->tier == 3)
                {
                        int       TKsNeeded = 40 - (ch->tks);
                        int       StatusNeeded = 20 - (ch->race);
                        int       HitNeeded = 100000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        if (TKsNeeded > 0 || StatusNeeded > 0
                            || HitNeeded > 0)
                        {
                                stc("#ROne or more of your tiering requirement have not been met#n\n\r", ch);
                                return;
                        }
                        else
                        {
                                ch->tier += 1;
                                ch->tks = 0;
                                stc("#CCongratulations #yon gaining a new tier!#n\n\r", ch);
                                xprintf(buf,
                                        "#R%s #Chas just become a more #r-#R=P#yOW#7ER#yFU#RL=#r- #cG#Cod!!#n\n\r",
                                        ch->name);
                                do_info(ch, buf);
                                return;
                        }
                }


                if (ch->tier == 4)
                {
                        int       TKsNeeded = 50 - (ch->tks);
                        int       StatusNeeded = 25 - (ch->race);
                        int       HitNeeded = 125000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        if (TKsNeeded > 0 || StatusNeeded > 0
                            || HitNeeded > 0)
                        {
                                stc("#ROne or more of your tiering requirement have not been met#n\n\r", ch);
                                return;
                        }
                        else
                        {
                                ch->tier += 1;
                                ch->tks = 0;
                                stc("#CCongratulations #yon gaining a new tier!#n\n\r", ch);
                                xprintf(buf,
                                        "#R%s #Chas just become a more #r-#R=P#yOW#7ER#yFU#RL=#r- #cG#Cod!!#n\n\r",
                                        ch->name);
                                do_info(ch, buf);
                                return;
                        }
                }


                if (ch->tier == 5)
                {
                        int       TKsNeeded = 60 - (ch->tks);
                        int       StatusNeeded = 30 - (ch->race);
                        int       HitNeeded = 150000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        if (TKsNeeded > 0 || StatusNeeded > 0
                            || HitNeeded > 0)
                        {
                                stc("#ROne or more of your tiering requirement have not been met#n\n\r", ch);
                                return;
                        }
                        else
                        {
                                ch->tier += 1;
                                ch->tks = 0;
                                stc("#CCongratulations #yon gaining a new tier!#n\n\r", ch);
                                xprintf(buf,
                                        "#R%s #Chas just become a more #r-#R=P#yOW#7ER#yFU#RL=#r- #cG#Cod!!#n\n\r",
                                        ch->name);
                                do_info(ch, buf);
                                return;
                        }
                }


                if (ch->tier == 6)
                {
                        int       TKsNeeded = 70 - (ch->tks);
                        int       StatusNeeded = 35 - (ch->race);
                        int       HitNeeded = 200000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        if (TKsNeeded > 0 || StatusNeeded > 0
                            || HitNeeded > 0)
                        {
                                stc("#ROne or more of your tiering requirement have not been met#n\n\r", ch);
                                return;
                        }
                        else
                        {
                                ch->tier += 1;
                                ch->tks = 0;
                                stc("#CCongratulations #yon gaining a new tier!#n\n\r", ch);
                                xprintf(buf,
                                        "#R%s #Chas just become a more #r-#R=P#yOW#7ER#yFU#RL=#r- #cG#Cod!!#n\n\r",
                                        ch->name);
                                do_info(ch, buf);
                                return;
                        }
                }


                if (ch->tier == 7)
                {
                        int       TKsNeeded = 80 - (ch->tks);
                        int       StatusNeeded = 40 - (ch->race);
                        int       HitNeeded = 225000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        if (TKsNeeded > 0 || StatusNeeded > 0
                            || HitNeeded > 0)
                        {
                                stc("#ROne or more of your tiering requirement have not been met#n\n\r", ch);
                                return;
                        }
                        else
                        {
                                ch->tier += 1;
                                ch->tks = 0;
                                stc("#CCongratulations #yon gaining a new tier!#n\n\r", ch);
                                xprintf(buf,
                                        "#R%s #Chas just become a more #r-#R=P#yOW#7ER#yFU#RL=#r- #cG#Cod!!#n\n\r",
                                        ch->name);
                                do_info(ch, buf);
                                return;
                        }
                }


                if (ch->tier == 8)
                {
                        int       TKsNeeded = 90 - (ch->tks);
                        int       StatusNeeded = 45 - (ch->race);
                        int       HitNeeded = 275000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        if (TKsNeeded > 0 || StatusNeeded > 0
                            || HitNeeded > 0)
                        {
                                stc("#ROne or more of your tiering requirement have not been met#n\n\r", ch);
                                return;
                        }
                        else
                        {
                                ch->tier += 1;
                                ch->tks = 0;
                                stc("#CCongratulations #yon gaining a new tier!#n\n\r", ch);
                                xprintf(buf,
                                        "#R%s #Chas just become a more #r-#R=P#yOW#7ER#yFU#RL=#r- #cG#Cod!!#n\n\r",
                                        ch->name);
                                do_info(ch, buf);
                                return;
                        }
                }


                if (ch->tier == 9)
                {
                        int       TKsNeeded = 100 - (ch->tks);
                        int       StatusNeeded = 50 - (ch->race);
                        int       HitNeeded = 300000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        if (TKsNeeded > 0 || StatusNeeded > 0
                            || HitNeeded > 0)
                        {
                                stc("#ROne or more of your tiering requirement have not been met#n\n\r", ch);
                                return;
                        }
                        else
                        {
                                ch->tier += 1;
                                ch->tks = 0;
                                stc("#CCongratulations #yon gaining a new tier!#n\n\r", ch);
                                xprintf(buf,
                                        "#R%s #Chas just become a more #r-#R=P#yOW#7ER#yFU#RL=#r- #cG#Cod!!#n\n\r",
                                        ch->name);
                                do_info(ch, buf);
                                return;
                        }
                }


        }   //<
        else
        {
                xprintf(buf, "#0You can train the following#w:#n\n\r");
                send_to_char(buf, ch);

                send_to_char("#0Stats#w:#W", ch);
                if (ch->pcdata->perm_str < max_stat)
                {
                        xprintf(buf, " #WStr#B(#C%dp#B)",
                                figure_stat_cost(ch->pcdata->perm_str));
                        stc(buf, ch);
                }
                if (ch->pcdata->perm_int < max_stat)
                {
                        xprintf(buf, " #WInt#B(#C%dp#B)",
                                figure_stat_cost(ch->pcdata->perm_int));
                        stc(buf, ch);
                }
                if (ch->pcdata->perm_wis < max_stat)
                {
                        xprintf(buf, " #WWis#B(#C%dp#B)",
                                figure_stat_cost(ch->pcdata->perm_wis));
                        stc(buf, ch);
                }
                if (ch->pcdata->perm_dex < max_stat)
                {
                        xprintf(buf, " #WDex#B(#C%dp#B)",
                                figure_stat_cost(ch->pcdata->perm_dex));
                        stc(buf, ch);
                }
                if (ch->pcdata->perm_con < max_stat)
                {
                        xprintf(buf, " #WCon#B(#C%dp#B)",
                                figure_stat_cost(ch->pcdata->perm_con));
                        stc(buf, ch);
                }
                if ((ch->pcdata->perm_str >= max_stat)
                    && (ch->pcdata->perm_wis >= max_stat)
                    && (ch->pcdata->perm_int >= max_stat)
                    && (ch->pcdata->perm_dex >= max_stat)
                    && (ch->pcdata->perm_con >= max_stat))
                        send_to_char(" #cNone left to train.#n\n\r", ch);
                else
                        send_to_char("#n.\n\r", ch);

                send_to_char("#0Spells#w:#W", ch);
                if (ch->spl[0] < cmax)
                {
                        stc(" #PPurple", ch);
                }
                if (ch->spl[2] < cmax)
                {
                        stc(" #BBlue", ch);
                }
                if (ch->spl[1] < cmax)
                {
                        stc(" #RRed", ch);
                }
                if (ch->spl[3] < cmax)
                {
                        stc(" #GGreen", ch);
                }
                if (ch->spl[4] < cmax)
                {
                        stc(" #YYellow", ch);
                }
                if ((ch->spl[0] >= cmax)
                    && (ch->spl[1] >= cmax)
                    && (ch->spl[2] >= cmax)
                    && (ch->spl[3] >= cmax) && (ch->spl[4] >= cmax))
                        send_to_char(" #wNone left to train.#n\n\r", ch);
                else
                        send_to_char("#n.\n\r", ch);

                if (ch->tier == 0)
                {
                        int       TKsNeeded = 10 - (ch->tks);
                        int       StatusNeeded = 5 - (ch->race);
                        int       HitNeeded = 25000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        xprintf(buf,
                                "#RTier I: #DYou need #Y%d #Dmore Tier Kills, #Y%d #Dmore Status, and #Y%d #Dmore HP.#n\n\r",
                                TKsNeeded, StatusNeeded, HitNeeded);
                        stc(buf, ch);

                }

                if (ch->tier == 1)
                {
                        int       TKsNeeded = 20 - (ch->tks);
                        int       StatusNeeded = 10 - (ch->race);
                        int       HitNeeded = 50000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        xprintf(buf,
                                "#RTier II: #DYou need #Y%d #Dmore Tier Kills, #Y%d #Dmore Status, and #Y%d #Dmore HP.#n\n\r",
                                TKsNeeded, StatusNeeded, HitNeeded);
                        stc(buf, ch);

                }

                if (ch->tier == 2)
                {
                        int       TKsNeeded = 30 - (ch->tks);
                        int       StatusNeeded = 15 - (ch->race);
                        int       HitNeeded = 75000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        xprintf(buf,
                                "#RTier III: #DYou need #Y%d #Dmore Tier Kills, #Y%d #Dmore Status, and #Y%d #Dmore HP.#n\n\r",
                                TKsNeeded, StatusNeeded, HitNeeded);
                        stc(buf, ch);

                }

                if (ch->tier == 3)
                {
                        int       TKsNeeded = 40 - (ch->tks);
                        int       StatusNeeded = 20 - (ch->race);
                        int       HitNeeded = 100000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        xprintf(buf,
                                "#RTier IV: #DYou need #Y%d #Dmore Tier Kills, #Y%d #Dmore Status, and #Y%d #Dmore HP.#n\n\r",
                                TKsNeeded, StatusNeeded, HitNeeded);
                        stc(buf, ch);

                }

                if (ch->tier == 4)
                {
                        int       TKsNeeded = 50 - (ch->tks);
                        int       StatusNeeded = 25 - (ch->race);
                        int       HitNeeded = 125000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        xprintf(buf,
                                "#RTier V: #DYou need #Y%d #Dmore Tier Kills, #Y%d #Dmore Status, and #Y%d #Dmore HP.#n\n\r",
                                TKsNeeded, StatusNeeded, HitNeeded);
                        stc(buf, ch);

                }

                if (ch->tier == 5)
                {
                        int       TKsNeeded = 60 - (ch->tks);
                        int       StatusNeeded = 30 - (ch->race);
                        int       HitNeeded = 150000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        xprintf(buf,
                                "#RTier VI: #DYou need #Y%d #Dmore Tier Kills, #Y%d #Dmore Status, and #Y%d #Dmore HP.#n\n\r",
                                TKsNeeded, StatusNeeded, HitNeeded);
                        stc(buf, ch);

                }

                if (ch->tier == 6)
                {
                        int       TKsNeeded = 70 - (ch->tks);
                        int       StatusNeeded = 35 - (ch->race);
                        int       HitNeeded = 200000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        xprintf(buf,
                                "#RTier VII: #DYou need #Y%d #Dmore Tier Kills, #Y%d #Dmore Status, and #Y%d #Dmore HP.#n\n\r",
                                TKsNeeded, StatusNeeded, HitNeeded);
                        stc(buf, ch);

                }

                if (ch->tier == 7)
                {
                        int       TKsNeeded = 80 - (ch->tks);
                        int       StatusNeeded = 40 - (ch->race);
                        int       HitNeeded = 225000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        xprintf(buf,
                                "#RTier VIII: #DYou need #Y%d #Dmore Tier Kills, #Y%d #Dmore Status, and #Y%d #Dmore HP.#n\n\r",
                                TKsNeeded, StatusNeeded, HitNeeded);
                        stc(buf, ch);

                }

                if (ch->tier == 8)
                {
                        int       TKsNeeded = 90 - (ch->tks);
                        int       StatusNeeded = 45 - (ch->race);
                        int       HitNeeded = 275000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        xprintf(buf,
                                "#RTier IX: #DYou need #Y%d #Dmore Tier Kills, #Y%d #Dmore Status, and #Y%d #Dmore HP.#n\n\r",
                                TKsNeeded, StatusNeeded, HitNeeded);
                        stc(buf, ch);

                }

                if (ch->tier == 9)
                {
                        int       TKsNeeded = 100 - (ch->tks);
                        int       StatusNeeded = 50 - (ch->race);
                        int       HitNeeded = 300000 - (ch->max_hit);

                        if (TKsNeeded < 0)
                                TKsNeeded = 0;
                        if (StatusNeeded < 0)
                                StatusNeeded = 0;
                        if (HitNeeded < 0)
                                HitNeeded = 0;

                        xprintf(buf,
                                "#RTier X: #DYou need #Y%d #Dmore Tier Kills, #Y%d #Dmore Status, and #Y%d #Dmore HP.#n\n\r",
                                TKsNeeded, StatusNeeded, HitNeeded);
                        stc(buf, ch);

                }
                if (ch->level == 2 && ch->max_hit > 1999)
                {
                        xprintf(buf, "#GBecome an avatar -  free.#n\n\r");
                        send_to_char(buf, ch);
                }

                if (ch->max_hit < (300000))
                {
                        xprintf(buf,
                                "#WHp               #R- #Y%d #0exp per point.#n\n\r",
                                (ch->max_hit + 1));
                        send_to_char(buf, ch);
                }

                if (ch->max_mana < (300000))
                {
                        xprintf(buf,
                                "#WMana             #R- #Y%d #0exp per point.#n\n\r",
                                (ch->max_mana + 1));
                        send_to_char(buf, ch);
                }
                if (ch->max_move < (300000))
                {
                        xprintf(buf,
                                "#WMove             #R- #Y%d #0exp per point.#n\n\r",
                                (ch->max_move + 1));
                        send_to_char(buf, ch);
                }
                if (ch->practice < 4000)
                {
                        xprintf(buf,
                                "#WPrimal           #R- #Y%d #0exp per point of primal energy.#n\n\r",
                                (1 + ch->practice) * 500);
                        send_to_char(buf, ch);
                }
		/*Player Rank code*/
		
		   if( ch->pRank < MAX_RANKING )
		{
		      sprintf( buf, "#7R#0ank#n             - %d exp and %d bones.\n\r", rank_xp, rank_bones );
		      send_to_char( buf, ch );
		}

                if (!IS_SET(ch->pcdata->jflags, JFLAG_KNOWS_FORGING))
                {
                        stc("#gForge Skill #R- #Y10,000,000 #gexperience.#n\n\r", ch);
                }

                if (IS_CLASS(ch, CLASS_ANGEL))
                {
                        if (ch->pcdata->powers[ANGEL_JUSTICE] < 5)
                        {
                                xprintf(buf,
                                        "Angel Justice : %d mill exp\n\r",
                                        (ch->pcdata->powers[ANGEL_JUSTICE] +
                                         1) * 10);
                                send_to_char(buf, ch);
                        }
                        if (ch->pcdata->powers[ANGEL_HARMONY] < 5)
                        {
                                xprintf(buf,
                                        "Angel Harmony : %d mill exp\n\r",
                                        (ch->pcdata->powers[ANGEL_HARMONY] +
                                         1) * 10);
                                send_to_char(buf, ch);
                        }
                        if (ch->pcdata->powers[ANGEL_PEACE] < 5)
                        {
                                xprintf(buf,
                                        "Angel Peace   : %d mill exp\n\r",
                                        (ch->pcdata->powers[ANGEL_PEACE] +
                                         1) * 10);
                                send_to_char(buf, ch);
                        }
                        if (ch->pcdata->powers[ANGEL_LOVE] < 5)
                        {
                                xprintf(buf,
                                        "Angel Love    : %d mill exp\n\r",
                                        (ch->pcdata->powers[ANGEL_LOVE] +
                                         1) * 10);
                                send_to_char(buf, ch);
                        }
                }
    if (IS_CLASS(ch, CLASS_ELEMENTAL))
       {
         if (ch->pcdata->powers[ELEMENTAL_FIRE] < 5)
         {
           sprintf(buf,"Elemental Fire : %d mill exp\n\r", (ch->pcdata->powers[ELEMENTAL_FIRE]+1)*10);
           send_to_char(buf,ch);
         }
         if (ch->pcdata->powers[ELEMENTAL_EARTH] < 5)
         {
           sprintf(buf,"Elemental Earth : %d mill exp\n\r", (ch->pcdata->powers[ELEMENTAL_EARTH]+1)*10);
           send_to_char(buf,ch);
         }
         if (ch->pcdata->powers[ELEMENTAL_AIR] < 5)
         {
           sprintf(buf,"Elemental Air   : %d mill exp\n\r", (ch->pcdata->powers[ELEMENTAL_AIR]+1)*10);
           send_to_char(buf,ch);
         }
         if (ch->pcdata->powers[ELEMENTAL_WATER] < 5)
         {
           sprintf(buf,"Elemental Water    : %d mill exp\n\r", (ch->pcdata->powers[ELEMENTAL_WATER]+1)*10);
           send_to_char(buf,ch);
         }
       }

                //if (ch->generation == 3) send_to_char ("#WGeneration         #R: #w400 mill exp\n\r#n",ch);
                //else if (ch->generation == 4) send_to_char ("#WGeneration         #R: #w200 mill exp\n\r#n",ch);
                //else if (ch->generation == 5) send_to_char ("#WGeneration         #R: #w50 mill exp\n\r#n",ch);
                //else if (ch->generation > 5)  send_to_char ("#WGeneration         #R: #w10 mill exp\n\r#n",ch);
                if (IS_CLASS(ch, CLASS_TANARRI))
                {
                        if (ch->pcdata->rank < TANARRI_FODDER)
                                send_to_char
                                        ("#RStatus#W-> #wFodder    #R: #w10 mill exp#n\n\r",
                                         ch);
                        else if (ch->pcdata->rank == TANARRI_FODDER)
                                send_to_char
                                        ("#RStatus#W-> #w Fighter   #R: #w20 mill exp#n\n\r",
                                         ch);
                        else if (ch->pcdata->rank == TANARRI_FIGHTER)
                                send_to_char
                                        ("#RStatus#W-> #w Elite     #R: #w40 mill exp#n\n\r",
                                         ch);
                        else if (ch->pcdata->rank == TANARRI_ELITE)
                                send_to_char
                                        ("#RStatus#W-> #w Captain   #R: #w80 mill exp#n\n\r",
                                         ch);
                        else if (ch->pcdata->rank == TANARRI_CAPTAIN)
                                send_to_char
                                        ("#RStatus#W-> #w Warlord   #R: #w160 mill exp#n\n\r",
                                         ch);
                        else if (ch->pcdata->rank == TANARRI_WARLORD)
                                send_to_char
                                        ("#RStatus#W-> #w Balor     #R: #w320 mill exp#n\n\r",
                                         ch);
                }
                if (IS_CLASS(ch, CLASS_VAMPIRE))
                {
                        if (ch->pcdata->rank == AGE_CHILDE)
                                send_to_char("#0Age#R: Ancilla#n\n\r", ch);
                        else if (ch->pcdata->rank == AGE_NEONATE)
                                send_to_char("#0Age#R: Ancilla#n\n\r", ch);
                        else if (ch->pcdata->rank == AGE_ANCILLA)
                                send_to_char("#0Age#R: Elder#n\n\r", ch);
                        else if (ch->pcdata->rank == AGE_ELDER)
                                send_to_char("#0Age#R: Methuselah#n\n\r", ch);
                        else if (ch->pcdata->rank == AGE_METHUSELAH)
                                send_to_char("#0Age#R: LaMagra#n\n\r", ch);
                        else if (ch->pcdata->rank == AGE_LA_MAGRA)
                                send_to_char("#0Age#R: Trueblood#n\n\r", ch);
                }
                if (IS_CLASS(ch, CLASS_NINJA))
                {
                        if (ch->pcdata->rank < BELT_ONE)
                                send_to_char("#0Studies#R: #wbelt1#n\n\r",
                                             ch);
                        else if (ch->pcdata->rank == BELT_ONE)
                                send_to_char("#0Studies#R: #wbelt2#n\n\r",
                                             ch);
                        else if (ch->pcdata->rank == BELT_TWO)
                                send_to_char("#0Studies#R: #wbelt3#n\n\r",
                                             ch);
                        else if (ch->pcdata->rank == BELT_THREE)
                                send_to_char("#0Studies#R: #wbelt4#n\n\r",
                                             ch);
                        else if (ch->pcdata->rank == BELT_FOUR)
                                send_to_char("#0Studies#R: #wbelt5#n\n\r",
                                             ch);
                        else if (ch->pcdata->rank == BELT_FIVE)
                                send_to_char("#0Studies#R: #wbelt6#n\n\r",
                                             ch);
                        else if (ch->pcdata->rank == BELT_SIX)
                                send_to_char("#0Studies#R: #wbelt7#n\n\r",
                                             ch);
                        else if (ch->pcdata->rank == BELT_SEVEN)
                                send_to_char("#0Studies#R: #wbelt8#n\n\r",
                                             ch);
                        else if (ch->pcdata->rank == BELT_EIGHT)
                                send_to_char("#0Studies#R: #wbelt9#n\n\r",
                                             ch);
                        else if (ch->pcdata->rank == BELT_NINE)
                                send_to_char("#0Studies#R: #wbelt10#n\n\r",
                                             ch);
                }
                if (IS_CLASS(ch, CLASS_VAMPIRE)
                    && !IS_IMMUNE(ch, IMM_SUNLIGHT))
                {
                        stc("#YImmunity to Sunlight #R- #Y10,000,000 #0experience.#n\n\r", ch);
                }
                if (ch->siltol < 100 && IS_CLASS(ch, CLASS_WEREWOLF))
                {
                        xprintf(buf,
                                "#WSilver tolerance #R- #Y%d #0exp per point of tolerance.#n\n\r",
                                silver);
                        send_to_char(buf, ch);
                }
                if (ch->gnosis[GMAXIMUM] < 20
                    && (IS_CLASS(ch, CLASS_WEREWOLF)))
                {
                        xprintf(buf,
                                "#WGnosis Points #R- #Y%d #0exp per point of Gnosis.#n\n\r",
                                gnosis);
                        stc(buf, ch);
                }
                if (ch->beast > 0 && IS_CLASS(ch, CLASS_VAMPIRE))
                {
                        xprintf(buf,
                                "#WBeast Control #R- #Y%d #0primal per point of Beast.#n\n\r",
                                beast);
                        stc(buf, ch);
                }
                if (ch->pcdata->stats[DROW_MAGIC] < 100
                    && IS_CLASS(ch, CLASS_DROW))
                {
                        xprintf(buf,
                                "#PMagic resistance #R- #Y%d #pdrow points per point of magic.#n\n\r",
                                magic);
                        send_to_char(buf, ch);
                }

                xprintf(buf,
                        "#WNatural resistances and immunities #R- #Y%d #0exp each.#n\n\r",
                        immcost);
                send_to_char(buf, ch);

                /*
                 * Weapon resistance affects 
                 */
                send_to_char("#WWeapon resistances#R:", ch);
                if (!IS_IMMUNE(ch, IMM_SLASH))
                        send_to_char(" #wSlash", ch);
                if (!IS_IMMUNE(ch, IMM_STAB))
                        send_to_char(" #wStab", ch);
                if (!IS_IMMUNE(ch, IMM_SMASH))
                        send_to_char(" #wSmash", ch);
                if (!IS_IMMUNE(ch, IMM_ANIMAL))
                        send_to_char(" #wBeast", ch);
                if (!IS_IMMUNE(ch, IMM_MISC))
                        send_to_char(" #wGrab", ch);
                if (IS_IMMUNE(ch, IMM_SLASH) && IS_IMMUNE(ch, IMM_STAB) &&
                    IS_IMMUNE(ch, IMM_SMASH) && IS_IMMUNE(ch, IMM_ANIMAL) &&
                    IS_IMMUNE(ch, IMM_MISC))
                        send_to_char(" #wNone left to learn.#n\n\r", ch);
                else
                        send_to_char(".#n\n\r", ch);

                /*
                 * Spell immunity affects 
                 */
                send_to_char("#WMagical immunities#R:", ch);
                if (!IS_IMMUNE(ch, IMM_CHARM))
                        send_to_char(" #wCharm", ch);
                if (!IS_IMMUNE(ch, IMM_HEAT))
                        send_to_char(" #wHeat", ch);
                if (!IS_IMMUNE(ch, IMM_COLD))
                        send_to_char(" #wCold", ch);
                if (!IS_IMMUNE(ch, IMM_LIGHTNING))
                        send_to_char(" #wLightning", ch);
                if (!IS_IMMUNE(ch, IMM_ACID))
                        send_to_char(" #wAcid", ch);
                if (!IS_IMMUNE(ch, IMM_DRAIN))
                        send_to_char(" #wDrain", ch);
                if (!IS_IMMUNE(ch, IMM_VOODOO))
                        send_to_char(" #wVoodoo", ch);
                if (IS_IMMUNE(ch, IMM_HEAT) &&
                    IS_IMMUNE(ch, IMM_COLD) && IS_IMMUNE(ch, IMM_LIGHTNING) &&
                    IS_IMMUNE(ch, IMM_ACID) &&
                    IS_IMMUNE(ch, IMM_DRAIN) && IS_IMMUNE(ch, IMM_VOODOO))
                        send_to_char(" #wNone left to learn.#n\n\r", ch);
                else
                        send_to_char("#w.#n\n\r", ch);

                /*
                 * Skill immunity affects 
                 */
                send_to_char("#WSkill immunities#R:", ch);
                if (!IS_IMMUNE(ch, IMM_HURL))
                        send_to_char(" #wHurl", ch);
                if (!IS_IMMUNE(ch, IMM_BACKSTAB))
                        send_to_char(" #wBackstab", ch);
                if (!IS_IMMUNE(ch, IMM_KICK))
                        send_to_char(" #wKick", ch);
                if (!IS_IMMUNE(ch, IMM_DISARM))
                        send_to_char(" #wDisarm", ch);
                if (!IS_IMMUNE(ch, IMM_STEAL))
                        send_to_char(" #wSteal", ch);
                if (IS_IMMUNE(ch, IMM_HURL) && IS_IMMUNE(ch, IMM_BACKSTAB) &&
                    IS_IMMUNE(ch, IMM_KICK) && IS_IMMUNE(ch, IMM_DISARM) &&
                    IS_IMMUNE(ch, IMM_STEAL))
                        send_to_char(" #wNone left to learn.#n\n\r", ch);
                else
                        send_to_char(".#n\n\r", ch);

                return;
        }

        if ((*pAbility >= max_stat) && (!str_cmp(arg1, "str")))
        {
                if (last)
                        act("Your $T is already at maximum.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }
        if ((*pAbility >= max_stat) && (!str_cmp(arg1, "int")))
        {
                if (last)
                        act("Your $T is already at maximum.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }
        if ((*pAbility >= max_stat) && (!str_cmp(arg1, "wis")))
        {
                if (last)
                        act("Your $T is already at maximum.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }
        if ((*pAbility >= max_stat) && (!str_cmp(arg1, "dex")))
        {
                if (last)
                        act("Your $T is already at maximum.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }
        if ((*pAbility >= max_stat) && (!str_cmp(arg1, "con")))
        {
                if (last)
                        act("Your $T is already at maximum.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }
        if ((*pAbility >= 15000) && (!str_cmp(arg1, "hp")))
        {
                if (last)
                        act("Your $T is already at maximum.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }
        if ((*pAbility >= 15000) && (!str_cmp(arg1, "mana")))
        {
                if (last)
                        act("Your $T is already at maximum.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }
        if ((*pAbility >= 15000) && (!str_cmp(arg1, "move")))
        {
                if (last)
                        act("Your $T is already at maximum.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }
        if ((*pAbility >= 999) && (!str_cmp(arg1, "primal")))
        {
                if (last)
                        act("Your $T is already at maximum.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }
        if ((*pAbility >= 100) && (!str_cmp(arg1, "magic")))
        {
                if (last)
                        act("Your $T is already at a maximum.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }

        if ((*pAbility >= 20) && (!str_cmp(arg1, "gnosis")))
        {
                if (last)
                        act("Your $T is already at maximum.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }

        if ((*pAbility <= 0) && (!str_cmp(arg1, "control")))
        {
                if (last)
                        act("You have already mastered your beast.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }

        if ((*pAbility >= 100) && (!str_cmp(arg1, "silver")))
        {
                if (last)
                        act("Your $T is already at maximum.", ch, NULL,
                            pOutput, TO_CHAR);
                return;
        }

        if (!strcmp(arg1, "control"))
        {
                if (cost > ch->practice)
                {
                        send_to_char("You don't have enough primal.\n\r", ch);
                        return;
                }
                else
                {
                        ch->practice -= cost;
                        ch->exp += cost;
                }
        }
        if (cost < 1)
                cost = 1;
        if (cost > ch->pcdata->stats[DROW_POWER] && !str_cmp(arg1, "magic"))
        {
                if (last)
                        send_to_char
                                ("You don't have enough drow power points.\n\r",
                                 ch);
                return;
        }
        else if (cost > ch->exp)
        {
                if (last)
                        send_to_char("You don't have enough exp.\n\r", ch);
                return;
        }
        if (!str_cmp(arg1, "magic"))
                ch->pcdata->stats[DROW_POWER] -= cost;
        else
                ch->exp -= cost;
        if (!str_cmp(arg1, "silver"))
                ch->siltol += 1;
        else if (!str_cmp(arg1, "gnosis"))
        {
                ch->gnosis[GCURRENT]++;
                ch->gnosis[GMAXIMUM]++;
        }

        else if (!str_cmp(arg1, "control"))
        {
                ch->beast--;
                if (ch->beast < 0)
                        ch->beast = 0;
        }
        else if (!str_cmp(arg1, "magic"))
                ch->pcdata->stats[DROW_MAGIC] += 1;
        else if (str_cmp(arg1, "avatar"))
        {
                *pAbility += 1;
        }
        if (last)
                act("Your $T increases!", ch, NULL, pOutput, TO_CHAR);
        return;
}

void do_mount(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;

        argument = one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                send_to_char("Mount what?\n\r", ch);
                return;
        }

        if (IS_AFFECTED(ch, AFF_POLYMORPH) && !IS_VAMPAFF(ch, VAM_DISGUISED))
        {
                send_to_char("You cannot ride in this form.\n\r", ch);
                return;
        }

        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }

        if (ch == victim)
        {
                send_to_char("You cannot ride on your own back!\n\r", ch);
                return;
        }

        if (ch->mounted > 0)
        {
                send_to_char("You are already riding.\n\r", ch);
                return;
        }

        if (!IS_NPC(victim) && !IS_IMMORTAL(ch))
        {
                send_to_char("You cannot mount them.\n\r", ch);
                return;
        }
        if (victim->mounted > 0)
        {
                send_to_char("You cannot mount them.\n\r", ch);
                return;
        }
        if (IS_NPC(victim) && !IS_SET(victim->act, ACT_MOUNT)
            && !IS_IMMORTAL(ch))
        {
                send_to_char("You cannot mount them.\n\r", ch);
                return;
        }

        if (victim->position < POS_STANDING)
        {
                if (victim->position < POS_SLEEPING)
                        act("$N is too badly hurt for that.", ch, NULL,
                            victim, TO_CHAR);
                else if (victim->position == POS_SLEEPING)
                        act("First you better wake $m up.", ch, NULL, victim,
                            TO_CHAR);
                else if (victim->position == POS_RESTING)
                        act("First $e better stand up.", ch, NULL, victim,
                            TO_CHAR);
                else if (victim->position == POS_MEDITATING)
                        act("First $e better stand up.", ch, NULL, victim,
                            TO_CHAR);
                else if (victim->position == POS_SITTING)
                        act("First $e better stand up.", ch, NULL, victim,
                            TO_CHAR);
                else if (victim->position == POS_SLEEPING)
                        act("First you better wake $m up.", ch, NULL, victim,
                            TO_CHAR);
                else if (victim->position == POS_FIGHTING)
                        act("Not while $e's fighting.", ch, NULL, victim,
                            TO_CHAR);
                return;
        }

        if (!IS_NPC(ch) && ch->stance[0] != -1)
                do_stance(ch, "");

        ch->mounted = IS_RIDING;
        victim->mounted = IS_MOUNT;
        ch->mount = victim;
        victim->mount = ch;

        act("You clamber onto $N's back.", ch, NULL, victim, TO_CHAR);
        act("$n clambers onto $N's back.", ch, NULL, victim, TO_ROOM);
        return;
}

void do_dismount(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;

        if (IS_SET(ch->mounted, IS_RIDING))
        {
                if ((victim = ch->mount) == NULL)
                {
                        ch->mounted = 0;
                        send_to_char("You stop riding the air.\n\r", ch);
                        return;
                }
        }
        if (ch->mounted == 0)
        {
                send_to_char("But you are not riding!\n\r", ch);
                return;
        }

        if ((victim = ch->mount) == NULL)
        {
                send_to_char("But you are not riding!\n\r", ch);
                return;
        }

        act("You clamber off $N's back.", ch, NULL, victim, TO_CHAR);
        act("$n clambers off $N's back.", ch, NULL, victim, TO_ROOM);

        ch->mounted = IS_ON_FOOT;
        victim->mounted = IS_ON_FOOT;

        ch->mount = NULL;
        victim->mount = NULL;

        return;
}

void do_tie(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        char      arg[MAX_INPUT_LENGTH];
        char      buf[MAX_INPUT_LENGTH];
        ROOM_INDEX_DATA *location;
        bool      found = FALSE;

        if (IS_NPC(ch))
                return;

        argument = one_argument(argument, arg);
        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }
        if (IS_NPC(victim))
        {
                send_to_char("You cannot tie a mob up!\n\r", ch);
                return;
        }
        if (victim == ch)
        {
                send_to_char("You cannot tie yourself up!\n\r", ch);
                return;
        }
        if (IS_EXTRA(victim, TIED_UP))
        {
                send_to_char("But they are already tied up!\n\r", ch);
                return;
        }
        if (victim->position > POS_STUNNED || victim->hit > 0)
        {
                send_to_char("You can only tie up a defenceless person.\n\r",
                             ch);
                return;
        }
        if (victim->in_room != NULL)
        {
                if (IS_SET(victim->in_room->room_flags, ROOM_ARENA))
                {
                        xprintf(buf, "#C%s #yhas been vanquished from the #Rarena#y by #C%s#n", victim->name, ch->name);
                        do_info(ch, buf);
                        victim->pcdata->alosses++;
                        if ((location = get_room_index(ROOM_VNUM_ALTAR)) == NULL)return;
                        char_from_room(victim);
                        char_to_room(victim, location);
                        victim->fight_timer = 0;
                        do_restore(victim, "self");
			do_restore(ch, "self");
                        do_call(victim, "all");
                        /*
                         * Check for winner  
                         */
                        found = FALSE;
                        for (victim = char_list; victim != NULL;
                             victim = victim->next)
                        {
                                if (IS_NPC(victim))
                                        continue;
                                if (victim->in_room != NULL &&
                                    victim->in_room->area == ch->in_room->area
                                    && victim->pcdata->chobj == NULL
                                    && victim != ch)
                                {
                                        found = TRUE;
                                }
                        }
                        if (!found)
                        {
                                xprintf(buf, "#C%s #yemerges victorious from the #Rarena#n", ch->name);
                                ch->pcdata->awins++;
                                do_info(ch, buf);
                                if ((location = get_room_index(ROOM_VNUM_ALTAR)) == NULL) return;
                                char_from_room(ch);
                                char_to_room(ch, location);
                                ch->fight_timer = 0;
                                do_restore(ch, "self");
				current_arena = FALSE;
                                win_prize(ch);
                                if (tourney) update_bets(ch, NULL, TRUE);
                        }
                        return;
                }
        }
        act("You quickly tie up $N.", ch, NULL, victim, TO_CHAR);
        act("$n quickly ties up $N.", ch, NULL, victim, TO_ROOM);
        send_to_char("You have been tied up!\n\r", victim);
        SET_BIT(victim->extra, TIED_UP);
        if (IS_SET(ch->pcdata->jflags, JFLAG_SETTIE))
                tie_message(ch, victim);
        else
        {
                xprintf(buf, "#P%s #Yhas been tied up by #R%s#n",
                        victim->name, ch->name);
                do_info(ch, buf);
        }
        return;
}

void do_untie(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        char      arg[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg);
        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }
        if (!IS_EXTRA(victim, TIED_UP))
        {
                send_to_char("But they are not tied up!\n\r", ch);
                return;
        }
        if (victim == ch)
        {
                send_to_char("You cannot untie yourself!\n\r", ch);
                return;
        }
        act("You quickly untie $N.", ch, NULL, victim, TO_CHAR);
        act("$n quickly unties $N.", ch, NULL, victim, TO_NOTVICT);
        act("$n quickly unties you.", ch, NULL, victim, TO_VICT);
        REMOVE_BIT(victim->extra, TIED_UP);
        return;
}

void do_gag(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        char      arg[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg);
        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }
        if (victim == ch && !IS_EXTRA(victim, GAGGED)
            && IS_EXTRA(victim, TIED_UP))
        {
                send_to_char("You cannot gag yourself!\n\r", ch);
                return;
        }
        if (!IS_EXTRA(victim, TIED_UP) && !IS_EXTRA(victim, GAGGED))
        {
                send_to_char("You can only gag someone who is tied up!\n\r",
                             ch);
                return;
        }
        if (!IS_EXTRA(victim, GAGGED))
        {
                act("You place a gag over $N's mouth.", ch, NULL, victim,
                    TO_CHAR);
                act("$n places a gag over $N's mouth.", ch, NULL, victim,
                    TO_NOTVICT);
                act("$n places a gag over your mouth.", ch, NULL, victim,
                    TO_VICT);
                SET_BIT(victim->extra, GAGGED);
                return;
        }
        if (ch == victim)
        {
                act("You remove the gag from your mouth.", ch, NULL, victim,
                    TO_CHAR);
                act("$n removes the gag from $s mouth.", ch, NULL, victim,
                    TO_ROOM);
                REMOVE_BIT(victim->extra, GAGGED);
                return;
        }
        act("You remove the gag from $N's mouth.", ch, NULL, victim, TO_CHAR);
        act("$n removes the gag from $N's mouth.", ch, NULL, victim,
            TO_NOTVICT);
        act("$n removes the gag from your mouth.", ch, NULL, victim, TO_VICT);
        REMOVE_BIT(victim->extra, GAGGED);
        return;
}

void do_blindfold(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        char      arg[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg);
        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }
        if (victim == ch && !IS_EXTRA(victim, BLINDFOLDED)
            && IS_EXTRA(victim, TIED_UP))
        {
                send_to_char("You cannot blindfold yourself!\n\r", ch);
                return;
        }
        if (!IS_EXTRA(victim, TIED_UP) && !IS_EXTRA(victim, BLINDFOLDED))
        {
                send_to_char
                        ("You can only blindfold someone who is tied up!\n\r",
                         ch);
                return;
        }
        if (!IS_EXTRA(victim, BLINDFOLDED))
        {
                act("You place a blindfold over $N's eyes.", ch, NULL, victim,
                    TO_CHAR);
                act("$n places a blindfold over $N's eyes.", ch, NULL, victim,
                    TO_NOTVICT);
                act("$n places a blindfold over your eyes.", ch, NULL, victim,
                    TO_VICT);
                SET_BIT(victim->extra, BLINDFOLDED);
                return;
        }
        if (ch == victim)
        {
                act("You remove the blindfold from your eyes.", ch, NULL,
                    victim, TO_CHAR);
                act("$n removes the blindfold from $s eyes.", ch, NULL,
                    victim, TO_ROOM);
                REMOVE_BIT(victim->extra, BLINDFOLDED);
                return;
        }
        act("You remove the blindfold from $N's eyes.", ch, NULL, victim,
            TO_CHAR);
        act("$n removes the blindfold from $N's eyes.", ch, NULL, victim,
            TO_NOTVICT);
        act("$n removes the blindfold from your eyes.", ch, NULL, victim,
            TO_VICT);
        REMOVE_BIT(victim->extra, BLINDFOLDED);
        return;
}


int count_imms(CHAR_DATA * ch)
{
        int       count = 0;

        if (IS_IMMUNE(ch, IMM_SLASH))
                count += 1;
        if (IS_IMMUNE(ch, IMM_STAB))
                count += 1;
        if (IS_IMMUNE(ch, IMM_SMASH))
                count += 1;
        if (IS_IMMUNE(ch, IMM_ANIMAL))
                count += 1;
        if (IS_IMMUNE(ch, IMM_MISC))
                count += 1;
        if (IS_IMMUNE(ch, IMM_CHARM))
                count += 1;
        if (IS_IMMUNE(ch, IMM_HEAT))
                count += 1;
        if (IS_IMMUNE(ch, IMM_COLD))
                count += 1;
        if (IS_IMMUNE(ch, IMM_LIGHTNING))
                count += 1;
        if (IS_IMMUNE(ch, IMM_ACID))
                count += 1;
        if (IS_IMMUNE(ch, IMM_SLEEP))
                count += 1;
        if (IS_IMMUNE(ch, IMM_DRAIN))
                count += 1;
        if (IS_IMMUNE(ch, IMM_VOODOO))
                count += 1;
        if (IS_IMMUNE(ch, IMM_HURL))
                count += 1;
        if (IS_IMMUNE(ch, IMM_BACKSTAB))
                count += 1;
        if (IS_IMMUNE(ch, IMM_KICK))
                count += 1;
        if (IS_IMMUNE(ch, IMM_DISARM))
                count += 1;
        if (IS_IMMUNE(ch, IMM_STEAL))
                count += 1;
        return ((count * 50000) + 10000);
}
void do_track(CHAR_DATA * ch, char *argument)
{
        bool      found = FALSE;

        if (!IS_NPC(ch) && number_percent() > ch->pcdata->learned[gsn_track])
        {
                send_to_char
                        ("You cannot sense any trails from this room.\n\r",
                         ch);
                return;
        }
        if (check_track(ch, 0))
                found = TRUE;
        if (check_track(ch, 1))
                found = TRUE;
        if (check_track(ch, 2))
                found = TRUE;
        if (check_track(ch, 3))
                found = TRUE;
        if (check_track(ch, 4))
                found = TRUE;
        if (found == FALSE)
        {
                send_to_char
                        ("You cannot sense any trails from this room.\n\r",
                         ch);
                return;
        }
        act("$n carefully examines the ground for tracks.", ch, NULL, NULL,
            TO_ROOM);
        return;
}

void do_hunt(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                if (strlen(ch->hunting) > 1)
                {
                        free_string(ch->hunting);
                        ch->hunting = str_dup("");
                        send_to_char("You stop hunting your prey.\n\r", ch);
                }
                else
                        send_to_char("Who do you wish to hunt?\n\r", ch);
                return;
        }
        if (!str_cmp(arg, ch->name))
        {
                send_to_char("How can you hunt yourself?\n\r", ch);
                return;
        }
        free_string(ch->hunting);
        ch->hunting = str_dup(arg);
        send_to_char("Ok.\n\r", ch);
        return;
}

void check_hunt(CHAR_DATA * ch)
{
        CHAR_DATA *victim;
        bool      found = FALSE;
        int       direction = 0;
        ROOM_INDEX_DATA *in_room;

        in_room = ch->in_room;
        if (!IS_NPC(ch) && number_percent() > ch->pcdata->learned[gsn_track])
        {
                send_to_char
                        ("You cannot sense any trails from this room.\n\r",
                         ch);
                free_string(ch->hunting);
                ch->hunting = str_dup("");
                return;
        }
        if (check_track(ch, 0))
        {
                found = TRUE;
                direction = ch->in_room->track_dir[0];
        }
        else if (check_track(ch, 1))
        {
                found = TRUE;
                direction = ch->in_room->track_dir[1];
        }
        else if (check_track(ch, 2))
        {
                found = TRUE;
                direction = ch->in_room->track_dir[2];
        }
        else if (check_track(ch, 3))
        {
                found = TRUE;
                direction = ch->in_room->track_dir[3];
        }
        else if (check_track(ch, 4))
        {
                found = TRUE;
                direction = ch->in_room->track_dir[4];
        }
        else if ((victim = get_char_room(ch, NULL, ch->hunting)) == NULL)
        {
                send_to_char
                        ("You cannot sense any trails from this room.\n\r",
                         ch);
                free_string(ch->hunting);
                ch->hunting = str_dup("");
                return;
        }
        if (strlen(ch->hunting) < 2)
                return;
        if ((victim = get_char_room(ch, NULL, ch->hunting)) != NULL)
                return;
        act("$n carefully examines the ground for tracks.", ch, NULL, NULL,
            TO_ROOM);
        move_char(ch, direction);
        if (in_room == ch->in_room || victim != NULL)
        {
                free_string(ch->hunting);
                ch->hunting = str_dup("");
        }
        return;
}

void add_tracks(CHAR_DATA * ch, int direction)
{
        int       loop;

        if (IS_NPC(ch))
                return;
        if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_LYNX] > 0)
                return;
        if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_SORA] >= 4)
                return;
        if (IS_CLASS(ch, CLASS_SHADOW)
            && IS_SET(ch->pcdata->powers[SHADOW_POWERS], NSHADOWS_SILENTWALK))
                return;
        if (IS_ITEMAFF(ch, ITEMA_STALKER))
                return;
        for (loop = 0; loop <= 4; loop++)
        {
                if (ch->in_room->track[loop] != NULL
                    && !str_cmp(ch->in_room->track[loop], ch->name))
                {
                        free_string(ch->in_room->track[loop]);
                        ch->in_room->track[loop] = str_dup("");
                }
        }
        if (ch->in_room->track[0] != NULL
            && strlen(ch->in_room->track[0]) < 2)
        {
                free_string(ch->in_room->track[0]);
                ch->in_room->track[0] = str_dup(ch->pcdata->switchname);
                ch->in_room->track_dir[0] = direction;
        }
        else if (ch->in_room->track[1] != NULL
                 && strlen(ch->in_room->track[1]) < 2)
        {
                free_string(ch->in_room->track[1]);
                ch->in_room->track[1] = str_dup(ch->pcdata->switchname);
                ch->in_room->track_dir[1] = direction;
        }
        else if (ch->in_room->track[2] != NULL
                 && strlen(ch->in_room->track[2]) < 2)
        {
                free_string(ch->in_room->track[2]);
                ch->in_room->track[2] = str_dup(ch->pcdata->switchname);
                ch->in_room->track_dir[2] = direction;
        }
        else if (ch->in_room->track[3] != NULL
                 && strlen(ch->in_room->track[3]) < 2)
        {
                free_string(ch->in_room->track[3]);
                ch->in_room->track[3] = str_dup(ch->pcdata->switchname);
                ch->in_room->track_dir[3] = direction;
        }
        else if (ch->in_room->track[4] != NULL
                 && strlen(ch->in_room->track[4]) < 2)
        {
                free_string(ch->in_room->track[4]);
                ch->in_room->track[4] = str_dup(ch->pcdata->switchname);
                ch->in_room->track_dir[4] = direction;
        }
        else
        {
                free_string(ch->in_room->track[0]);
                ch->in_room->track[0] = str_dup(ch->in_room->track[1]);
                ch->in_room->track_dir[0] = ch->in_room->track_dir[1];
                free_string(ch->in_room->track[1]);
                ch->in_room->track[1] = str_dup(ch->in_room->track[2]);
                ch->in_room->track_dir[1] = ch->in_room->track_dir[2];
                free_string(ch->in_room->track[2]);
                ch->in_room->track[2] = str_dup(ch->in_room->track[3]);
                ch->in_room->track_dir[2] = ch->in_room->track_dir[3];
                free_string(ch->in_room->track[3]);
                ch->in_room->track[3] = str_dup(ch->in_room->track[4]);
                ch->in_room->track_dir[3] = ch->in_room->track_dir[4];
                free_string(ch->in_room->track[4]);
                ch->in_room->track[4] = str_dup(ch->name);
                ch->in_room->track_dir[4] = direction;
        }
        return;
}

bool check_track(CHAR_DATA * ch, int direction)
{
        CHAR_DATA *victim;
        char      buf[MAX_INPUT_LENGTH];
        char      vict[MAX_INPUT_LENGTH];
        int       door;

        if (ch->hunting != NULL)
                strcpy(vict, ch->hunting);
        else
                return FALSE;
        if (!str_cmp(ch->hunting, vict))
        {
                if ((victim = get_char_room(ch, NULL, vict)) != NULL)
                {
                        act("You have found $N!", ch, NULL, victim, TO_CHAR);
                        free_string(ch->hunting);
                        ch->hunting = str_dup("");
                        return TRUE;
                }
        }
        if (strlen(ch->in_room->track[direction]) < 2)
                return FALSE;
        if (!str_cmp(ch->in_room->track[direction], ch->name))
                return FALSE;
        if (strlen(ch->hunting) > 1
            && str_cmp(ch->in_room->track[direction], ch->hunting))
                return FALSE;
        door = ch->in_room->track_dir[direction];
        xprintf(buf, "You sense the trail of %s leading $T from here.",
                ch->in_room->track[direction]);
        act(buf, ch, NULL, dir_name[door], TO_CHAR);
        return TRUE;
}

void do_roll(CHAR_DATA * ch, char *argument)
{
        OBJ_DATA *obj;
        char      arg[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg);

        if (IS_NPC(ch))
                return;

        if (!IS_CLASS(ch, CLASS_DEMON))
        {
                stc("Huh?\n\r", ch);
                return;
        }

        if ((obj = ch->pcdata->chobj) == NULL)
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        if (obj->chobj == NULL || obj->chobj != ch)
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        if (obj->in_room == NULL)
        {
                send_to_char("You are unable to move.\n\r", ch);
                return;
        }

        if (!str_cmp(arg, "n") || !str_cmp(arg, "north"))
                do_north(ch, "");
        else if (!str_cmp(arg, "s") || !str_cmp(arg, "south"))
                do_south(ch, "");
        else if (!str_cmp(arg, "e") || !str_cmp(arg, "east"))
                do_east(ch, "");
        else if (!str_cmp(arg, "w") || !str_cmp(arg, "west"))
                do_west(ch, "");
        else if (!str_cmp(arg, "u") || !str_cmp(arg, "up"))
                do_up(ch, "");
        else if (!str_cmp(arg, "d") || !str_cmp(arg, "down"))
                do_down(ch, "");
        else
        {
                send_to_char
                        ("Do you wish to roll north, south, east, west, up or down?\n\r",
                         ch);
                return;
        }
        obj_from_room(obj);
        obj_to_room(obj, ch->in_room);
        return;
}

void do_leap(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        OBJ_DATA *container;
        OBJ_DATA *obj;
        char      arg[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg);

        if (IS_NPC(ch))
                return;

        if (!IS_CLASS(ch, CLASS_DEMON))
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        if ((obj = ch->pcdata->chobj) == NULL)
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        if (obj->chobj == NULL || obj->chobj != ch)
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        if (obj->in_room != NULL && arg[0] == '\0')
        {
                send_to_char("Where do you want to leap?\n\r", ch);
                return;
        }

        if (obj->in_room != NULL)
        {
                if ((victim = get_char_room(ch, NULL, arg)) != NULL)
                {
                        act("$p leaps into your hands.", victim, obj, NULL,
                            TO_CHAR);
                        act("$p leaps into $n's hands.", victim, obj, NULL,
                            TO_ROOM);
                        obj_from_room(obj);
                        obj_to_char(obj, victim);
                        return;
                }
                else if ((container = get_obj_room(ch, arg)) != NULL)
                {
                        if (container->item_type != ITEM_CONTAINER &&
                            container->item_type != ITEM_CORPSE_NPC &&
                            container->item_type != ITEM_CORPSE_PC)
                        {
                                send_to_char
                                        ("You cannot leap into that sort of object.\n\r",
                                         ch);
                                return;
                        }
                        act("$p leap into $P.", ch, obj, container, TO_CHAR);
                        act("$p leaps into $P.", ch, obj, container, TO_ROOM);
                        obj_from_room(obj);
                        obj_to_obj(obj, container);
                        return;
                }
                else
                        send_to_char("Nothing here by that name.\n\r", ch);
                return;
        }
        if (obj->carried_by != NULL)
        {
                act("$p leaps from your hands.", obj->carried_by, obj, NULL,
                    TO_CHAR);
                act("$p leaps from $n's hands.", obj->carried_by, obj, NULL,
                    TO_ROOM);
                obj_from_char(obj);
                obj_to_room(obj, ch->in_room);
                return;
        }
        else if ((container = obj->in_obj) != NULL
                 && container->in_room != NULL)
        {
                obj_from_obj(obj);
                obj_to_room(obj, container->in_room);
                char_from_room(ch);
                char_to_room(ch, container->in_room);
                act("$p leap from $P.", ch, obj, container, TO_CHAR);
                act("$p leaps from $P.", ch, obj, container, TO_ROOM);
                return;
        }
        if (obj->in_room != NULL)
                send_to_char("You seem unable to leap anywhere.\n\r", ch);
        else
                send_to_char("You seem to be stuck!\n\r", ch);
        return;
}


void drow_hate(CHAR_DATA * ch)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        if (IS_NPC(ch))
                return;

        if (IS_CLASS(ch, CLASS_DROW) && !IS_SET(ch->newbits, NEW_DROWHATE))
                return;
        if (!IS_CLASS(ch, CLASS_DROW) && !IS_ITEMAFF(ch, ITEMA_MADNESS))
                return;

        if (number_percent() < 2)
        {

                for (vch = char_list; vch != NULL; vch = vch_next)
                {
                        vch_next = vch->next;
                        if (ch == vch)
                                continue;
                        if (vch->in_room == NULL)
                                continue;
                        if (vch->in_room == ch->in_room)
                        {
                                send_to_char
                                        ("You scream out in hatred and attack!\n\r",
                                         ch);
                                act("$n screams out in hatred and attacks!",
                                    ch, NULL, vch, TO_ROOM);
                                SET_BIT(ch->pcdata->tempflag,
                                        TEMP_AGGRESSIVE);
                                do_kill(ch, vch->name);
                                REMOVE_BIT(ch->pcdata->tempflag,
                                           TEMP_AGGRESSIVE);
                                return;
                        }
                }
        }

        return;
}

int figure_stat_cost(const int mStat)
{
        int       stat = mStat - 13;

        if (stat < 10)
                return 10;
        else if (stat < 20)
                return 30;
        else if (stat < 30)
                return 50;
        else if (stat < 40)
                return 70;
        else if (stat < 50)
                return 90;
        else if (stat < 60)
                return 120;
        else if (stat < 70)
                return 150;
        else if (stat < 90)
                return 170;
        else
                return 200;
        return 200;
}

