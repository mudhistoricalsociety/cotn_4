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

/*
 * Local functions.
 */
#define CD CHAR_DATA
void get_obj args((CHAR_DATA * ch, OBJ_DATA * obj, OBJ_DATA * container));
CD       *find_keeper args((CHAR_DATA * ch));
int get_cost args((CHAR_DATA * keeper, OBJ_DATA * obj, bool fBuy));
void sheath args((CHAR_DATA * ch, bool right));
void draw args((CHAR_DATA * ch, bool right));
void give_token args((CHAR_DATA * questmaster, CHAR_DATA * ch, int value));

#undef	CD

void do_call(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        OBJ_DATA *in_obj;
        CHAR_DATA *gch;
        CHAR_DATA *victim = NULL;
        ROOM_INDEX_DATA *chroom;
        ROOM_INDEX_DATA *objroom;

        one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                send_to_char("What object do you wish to call?\n\r", ch);
                return;
        }
        if (IS_NPC(ch))
                return;
        if (!IS_HEAD(ch, LOST_HEAD))
        {
                act("Your eyes flicker with yellow energy.", ch, NULL, NULL,
                    TO_CHAR);
                act("$n's eyes flicker with yellow energy.", ch, NULL, NULL,
                    TO_ROOM);
        }
        if (!str_cmp(arg, "all"))
        {
                call_all(ch);
                return;
        }
        if ((obj = get_obj_world(ch, arg)) == NULL)
        {
                send_to_char
                        ("Nothing like that in hell, earth, or heaven.\n\r",
                         ch);
                return;
        }
        if (obj->ownerid == 0 || obj->ownerid != ch->pcdata->playerid)
        {
                send_to_char("Nothing happens.\n\r", ch);
                return;
        }
/*
        if (IS_OBJ_STAT(obj, ITEM_MQUEST))
        {
                send_to_char("Nothing happens.\n\r", ch);
                return;
        }
*/

        for (in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj);

        if (in_obj->carried_by != NULL)
        {
                if (in_obj->carried_by == ch)
                        return;
                if ((gch = in_obj->carried_by) != NULL)
                {
                        if (gch->desc && gch->desc->connected != CON_PLAYING)
                                return;
                }
        }
        if (obj->carried_by != NULL && obj->carried_by != ch)
        {
                victim = obj->carried_by;
                if (!IS_NPC(victim) && victim->desc != NULL
                    && victim->desc->connected != CON_PLAYING)
                        return;
                act("$p suddenly vanishes from your hands!", victim, obj,
                    NULL, TO_CHAR);
                act("$p suddenly vanishes from $n's hands!", victim, obj,
                    NULL, TO_ROOM);
                obj_from_char(obj);
        }
        else if (obj->in_room != NULL)
        {
                chroom = ch->in_room;
                objroom = obj->in_room;
                char_from_room(ch);
                char_to_room(ch, objroom);
                act("$p vanishes from the ground!", ch, obj, NULL, TO_ROOM);
                if (chroom == objroom)
                        act("$p vanishes from the ground!", ch, obj, NULL,
                            TO_CHAR);
                char_from_room(ch);
                char_to_room(ch, chroom);
                obj_from_room(obj);
        }
        else if (obj->in_obj != NULL)
                obj_from_obj(obj);
        else
        {
                if (!IS_HEAD(ch, LOST_HEAD))
                        send_to_char("Nothing happens.\n\r", ch);
                return;
        }
        obj_to_char(obj, ch);
        if (IS_SET(obj->extra_flags, ITEM_SHADOWPLANE))
                REMOVE_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
        act("$p materializes in your hands.", ch, obj, NULL, TO_CHAR);
        act("$p materializes in $n's hands.", ch, obj, NULL, TO_ROOM);
        do_autosave(ch, "");
        if (victim != NULL)
                do_autosave(victim, "");
        return;
}

/*
void call_all(CHAR_DATA * ch)
{
        CHAR_DATA *gch;
        OBJ_DATA *obj;
        OBJ_DATA *in_obj;
        CHAR_DATA *victim = NULL;
        DESCRIPTOR_DATA *d;
        ROOM_INDEX_DATA *chroom;
        ROOM_INDEX_DATA *objroom;
        bool      found = FALSE;

        for (obj = object_list; obj != NULL; obj = obj->next)
        {
                if (obj->ownerid == 0 || obj->ownerid != ch->pcdata->playerid)
                        continue;

                if (IS_OBJ_STAT(obj, ITEM_MQUEST))
                        continue;


                found = TRUE;

                for (in_obj = obj; in_obj->in_obj != NULL;
                     in_obj = in_obj->in_obj);

                if (in_obj->carried_by != NULL)
                {
                        if (in_obj->carried_by == ch)
                                continue;
                        if ((gch = in_obj->carried_by) != NULL)
                        {
                                if (gch->desc
                                    && gch->desc->connected != CON_PLAYING)
                                        continue;
                        }
                }
                if (obj->carried_by != NULL && obj->carried_by != ch)
                {
                        if (obj->carried_by == ch
                            || obj->carried_by->desc == NULL
                            || obj->carried_by->desc->connected !=
                            CON_PLAYING)
                        {
                                if (!IS_NPC(obj->carried_by))
                                        return;
                        }
                        act("$p suddenly vanishes from your hands!",
                            obj->carried_by, obj, NULL, TO_CHAR);
                        act("$p suddenly vanishes from $n's hands!",
                            obj->carried_by, obj, NULL, TO_ROOM);
                        SET_BIT(obj->carried_by->extra, EXTRA_CALL_ALL);
                        obj_from_char(obj);
                }
                else if (obj->in_room != NULL)
                {
                        chroom = ch->in_room;
                        objroom = obj->in_room;
                        char_from_room(ch);
                        char_to_room(ch, objroom);
                        act("$p vanishes from the ground!", ch, obj, NULL,
                            TO_ROOM);
                        if (chroom == objroom)
                                act("$p vanishes from the ground!", ch, obj,
                                    NULL, TO_CHAR);
                        char_from_room(ch);
                        char_to_room(ch, chroom);
                        obj_from_room(obj);
                }
                else if (obj->in_obj != NULL)
                        obj_from_obj(obj);
                else
                        continue;
                obj_to_char(obj, ch);
                if (IS_SET(obj->extra_flags, ITEM_SHADOWPLANE))
                        REMOVE_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
                if (!IS_HEAD(ch, LOST_HEAD))
                {
                        act("$p materializes in your hands.", ch, obj, NULL,
                            TO_CHAR);
                        act("$p materializes in $n's hands.", ch, obj, NULL,
                            TO_ROOM);
                }
        }

        if (!found && !IS_HEAD(ch, LOST_HEAD))
                send_to_char("Nothing happens.\n\r", ch);

        for (d = descriptor_list; d != NULL; d = d->next)
        {
                if (d->connected != CON_PLAYING)
                        continue;
                if ((victim = d->character) == NULL)
                        continue;
                if (IS_NPC(victim))
                        continue;
                if (ch != victim && !IS_EXTRA(victim, EXTRA_CALL_ALL))
                        continue;
                REMOVE_BIT(victim->extra, EXTRA_CALL_ALL);
                do_autosave(victim, "");
        }
        return;
}
*/
/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split(CHAR_DATA * ch, char *argument)
{
        char      buf[MAX_STRING_LENGTH];
        char      arg1[MAX_INPUT_LENGTH];
        CHAR_DATA *gch;
        int       members;
        int       amount_bones = 0;
        int       share_bones = 0;
        int       extra_bones = 0;

        argument = one_argument(argument, arg1);


        if (arg1[0] == '\0')
        {
                send_to_char("Split how much?\n\r", ch);
                return;
        }

        amount_bones = atoi(arg1);

        if (amount_bones < 0)
        {
                send_to_char("Your group wouldn't like that.\n\r", ch);
                return;
        }

        if (amount_bones == 0)
        {
                send_to_char
                        ("You hand out zero bones, but no one notices.\n\r",
                         ch);
                return;
        }

        if (ch->bones < amount_bones)
        {
                send_to_char("You don't have that much to split.\n\r", ch);
                return;
        }

        members = 0;
        for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
        {
                if (is_same_group(gch, ch) && !IS_AFFECTED(gch, AFF_CHARM))
                        members++;
        }

        if (members < 2)
        {
                send_to_char("Just keep it all.\n\r", ch);
                return;
        }

        share_bones = amount_bones / members;
        extra_bones = amount_bones % members;


        if (share_bones == 0)
        {
                send_to_char("Don't even bother, cheapskate.\n\r", ch);
                return;
        }

        ch->bones -= amount_bones;

        if (share_bones > 0)
        {
                xprintf(buf,
                        "You split %d bones. Your share is %d bones.\n\r",
                        amount_bones - extra_bones, share_bones);
                send_to_char(buf, ch);
        }

        xprintf(buf, "$n splits %d bones, Giving you %d bones.\n\r",
                amount_bones - extra_bones, share_bones);


        for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
        {
                if (gch != ch && is_same_group(gch, ch)
                    && !IS_AFFECTED(gch, AFF_CHARM))
                {
                        act(buf, ch, NULL, gch, TO_VICT);
                        gch->bones += share_bones;
                }
        }

        return;
}

void get_obj(CHAR_DATA * ch, OBJ_DATA * obj, OBJ_DATA * container)
{
        OBJ_DATA *obj2;
        OBJ_DATA *obj_next;
        ROOM_INDEX_DATA *objroom;
        bool      move_ch = FALSE;
        char      buf[MSL];
        int       members;
        CHAR_DATA *gch;

        /*
         * Objects should only have a shadowplane flag when on the floor 
         */
        if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && obj->in_room != NULL
            && (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)))
        {
                send_to_char("Your hand passes right through it!\n\r", ch);
                return;
        }
        if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && obj->in_room != NULL
            && (IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)))
        {
                send_to_char("Your hand passes right through it!\n\r", ch);
                return;
        }
        if (!CAN_WEAR(obj, ITEM_TAKE))
        {
                send_to_char("You can't take that.\n\r", ch);
                return;
        }

        if (ch->carry_number + 1 > can_carry_n(ch))
        {
                act("$d: you can't carry that many items.", ch, NULL,
                    obj->name, TO_CHAR);
                return;
        }

        if (ch->carry_weight + get_obj_weight(obj) > can_carry_w(ch))
        {
                act("$d: you can't carry that much weight.", ch, NULL,
                    obj->name, TO_CHAR);
                return;
        }

        if (container != NULL)
        {

                if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                    !IS_SET(container->extra_flags, ITEM_SHADOWPLANE) &&
                    (container->carried_by == NULL
                     || container->carried_by != ch))
                {
                        send_to_char("Your hand passes right through it!\n\r",
                                     ch);
                        return;
                }
                if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                    IS_SET(container->extra_flags, ITEM_SHADOWPLANE) &&
                    (container->carried_by == NULL
                     || container->carried_by != ch))
                {
                        send_to_char("Your hand passes right through it!\n\r",
                                     ch);
                        return;
                }
                act("You get $p from $P.", ch, obj, container, TO_CHAR);
                act("$n gets $p from $P.", ch, obj, container, TO_ROOM);
                for (obj2 = container->contains; obj2 != NULL;
                     obj2 = obj_next)
                {
                        obj_next = obj2->next_content;
                        if (obj2->chobj != NULL)
                        {
                                act("A hand reaches inside $P and takes $p out.", obj2->chobj, obj, container, TO_CHAR);
                                move_ch = TRUE;
                        }
                }
                obj_from_obj(obj);
        }
        else
        {
                act("You pick up $p.", ch, obj, container, TO_CHAR);
                act("$n picks $p up.", ch, obj, container, TO_ROOM);
                if (obj != NULL)
                        obj_from_room(obj);
        }

        if (obj->item_type == ITEM_MONEY)
        {
                ch->bones += obj->value[0];
                if (IS_SET(ch->act2, PLR_AUTOSPLIT))
                {   /* AUTOSPLIT code */
                        members = 0;
                        for (gch = ch->in_room->people; gch != NULL;
                             gch = gch->next_in_room)
                        {
                                if (!IS_AFFECTED(gch, AFF_CHARM)
                                    && is_same_group(gch, ch))
                                        members++;
                        }

                        if (members > 1 && (obj->value[0] > 1))
                        {
                                xprintf(buf, "%d", obj->value[0]);
                                do_split(ch, buf);
                        }
                }

                extract_obj(obj);
                //ch->bones += obj->value[0];
                //extract_obj( obj );
        }
        else
        {
                obj_to_char(obj, ch);
                if (HAS_TRIGGER_OBJ(obj, TRIG_GET))
                        p_give_trigger(NULL, obj, NULL, ch, obj, TRIG_GET);
                if (HAS_TRIGGER_ROOM(ch->in_room, TRIG_GET))
                        p_give_trigger(NULL, NULL, ch->in_room, ch, obj,
                                       TRIG_GET);
                if (move_ch && obj->chobj != NULL)
                {
                        if (obj->carried_by != NULL
                            && obj->carried_by != obj->chobj)
                                objroom =
                                        get_room_index(obj->carried_by->
                                                       in_room->vnum);
                        else
                                objroom = NULL;
                        if (objroom != NULL
                            && get_room_index(obj->chobj->in_room->vnum) !=
                            objroom)
                        {
                                char_from_room(obj->chobj);
                                char_to_room(obj->chobj, objroom);
                                do_look(obj->chobj, "auto");
                        }
                }
                if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                    (IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)))
                        REMOVE_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
        }
        if (IS_SET(obj->quest, QUEST_ARTIFACT) && !IS_NPC(ch))
        {
                xprintf(buf, "%s has discovered %s", ch->pcdata->switchname,
                        obj->short_descr);
                do_info(ch, buf);
        }

        return;
}



void do_newbiepack(CHAR_DATA * ch, char *argument)
{

        int       templevel = 0;

        if (ch->level == 3)
        {
                send_to_char
                        ("You must be a mortal or avatar to create a newbie pack!\n\r",
                         ch);
                return;
        }
        else if (ch->level == 1 || ch->level > 6)
        {
                templevel = ch->level;
                ch->level = 12;
                ch->trust = 12;
                do_oload(ch, "30333");
                do_oload(ch, "30334");
                do_oload(ch, "30335");
                do_oload(ch, "30336");
                do_oload(ch, "30337");
                do_oload(ch, "30338");
                do_oload(ch, "30339");
                do_oload(ch, "30339");
                do_oload(ch, "30340");
                do_oload(ch, "30340");
                do_oload(ch, "30342");
                do_oload(ch, "30342");
                do_oload(ch, "30343");
                do_oload(ch, "30343");
                do_oload(ch, "2622");
                do_oload(ch, "2204");
                ch->level = templevel;
                //ch->level = 1;
                ch->trust = 0;
        }
        return;
}

void do_get(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        char      arg3[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        OBJ_DATA *obj_next;
        OBJ_DATA *container;
        bool      found;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        argument = one_argument(argument, arg3);

        if (IS_AFFECTED(ch, AFF_ETHEREAL))
        {
                send_to_char("You cannot pick things up while ethereal.\n\r",
                             ch);
                return;
        }

        /*
         * Get type. 
         */
        if (arg1[0] == '\0')
        {
                send_to_char("Get what?\n\r", ch);
                return;
        }

        if (arg2[0] == '\0')
        {
                if (str_cmp(arg1, "all") && str_prefix("all.", arg1))
                {
                        /*
                         * 'get obj' 
                         */
                        obj = get_obj_list(ch, arg1, ch->in_room->contents);
                        if (obj == NULL)
                        {
                                act("I see no $T here.", ch, NULL, arg1,
                                    TO_CHAR);
                                return;
                        }
                        if (IS_NPC(ch) && IS_SET(obj->quest, QUEST_ARTIFACT))
                        {
                                send_to_char("You can't pick that up.\n\r",
                                             ch);
                                return;
                        }

                        get_obj(ch, obj, NULL);
                }
                else
                {
                        /*
                         * 'get all' or 'get all.obj' 
                         */
                        found = FALSE;
                        for (obj = ch->in_room->contents; obj != NULL;
                             obj = obj_next)
                        {
                                obj_next = obj->next_content;
                                if ((arg1[3] == '\0'
                                     || is_name(&arg1[4], obj->name))
                                    && can_see_obj(ch, obj))
                                {
                                        found = TRUE;
                                        get_obj(ch, obj, NULL);
                                }
                        }

                        if (!found)
                        {
                                if (arg1[3] == '\0')
                                        send_to_char
                                                ("I see nothing here.\n\r",
                                                 ch);
                                else
                                        act("I see no $T here.", ch, NULL,
                                            &arg1[4], TO_CHAR);
                        }
                }
        }
        else
        {
                /*
                 * 'get ... container' 
                 */
                if (!str_cmp(arg2, "all") || !str_prefix("all.", arg2))
                {
                        send_to_char("You can't do that.\n\r", ch);
                        return;
                }

                if ((container = get_obj_here(ch, NULL, arg2)) == NULL)
                {
                        act("I see no $T here.", ch, NULL, arg2, TO_CHAR);
                        return;
                }

                switch (container->item_type)
                {
                default:
                        send_to_char("That's not a container.\n\r", ch);
                        return;

                case ITEM_CONTAINER:
                case ITEM_CORPSE_NPC:
                case ITEM_CORPSE_PC:
                        break;
                }

                if (IS_SET(container->value[1], CONT_CLOSED))
                {
                        act("The $d is closed.", ch, NULL, container->name,
                            TO_CHAR);
                        return;
                }

                if (str_cmp(arg1, "all") && str_prefix("all.", arg1))
                {
                        /*
                         * 'get obj container' 
                         */
                        obj = get_obj_list(ch, arg1, container->contains);
                        if (obj == NULL && str_cmp(arg3, "true"))
                        {
                                act("I see nothing like that in the $T.", ch,
                                    NULL, arg2, TO_CHAR);
                                return;
                        }
                        get_obj(ch, obj, container);
                }
                else
                {
                        /*
                         * 'get all container' or 'get all.obj container' 
                         */
                        found = FALSE;
                        for (obj = container->contains; obj != NULL;
                             obj = obj_next)
                        {
                                obj_next = obj->next_content;
                                if ((arg1[3] == '\0'
                                     || is_name(&arg1[4], obj->name))
                                    && can_see_obj(ch, obj))
                                {
                                        found = TRUE;
                                        get_obj(ch, obj, container);
                                }
                        }

                        if (!found)
                        {
                                if (arg1[3] == '\0')
                                        act("I see nothing in the $T.", ch,
                                            NULL, arg2, TO_CHAR);
                                else if (str_cmp(arg3, "true"))
                                        act("I see nothing like that in the $T.", ch, NULL, arg2, TO_CHAR);
                        }
                }
        }
        do_autosave(ch, "");
        return;
}

void do_put(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        OBJ_DATA *container;
        OBJ_DATA *obj;
        OBJ_DATA *obj2;
        OBJ_DATA *obj_next;
        OBJ_DATA *obj_next2;
        ROOM_INDEX_DATA *objroom = get_room_index(ROOM_VNUM_IN_OBJECT);

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);

        if (arg1[0] == '\0' || arg2[0] == '\0')
        {
                send_to_char("Put what in what?\n\r", ch);
                return;
        }

        if (!str_cmp(arg2, "all") || !str_prefix("all.", arg2))
        {
                send_to_char("You can't do that.\n\r", ch);
                return;
        }

        if ((container = get_obj_here(ch, NULL, arg2)) == NULL)
        {
                act("I see no $T here.", ch, NULL, arg2, TO_CHAR);
                return;
        }

        if (container->item_type != ITEM_CONTAINER)
        {
                send_to_char("That's not a container.\n\r", ch);
                return;
        }

        if (IS_SET(container->value[1], CONT_CLOSED))
        {
                act("The $d is closed.", ch, NULL, container->name, TO_CHAR);
                return;
        }

        if (str_cmp(arg1, "all") && str_prefix("all.", arg1))
        {
                /*
                 * 'put obj container' 
                 */
                if ((obj = get_obj_carry(ch, arg1, ch)) == NULL)
                {
                        send_to_char("You do not have that item.\n\r", ch);
                        return;
                }

                if (obj == container)
                {
                        send_to_char("You can't fold it into itself.\n\r",
                                     ch);
                        return;
                }

                if (IS_SET(obj->quest, QUEST_ARTIFACT))
                {
                        send_to_char
                                ("You cannot put artifacts in a container.\n\r",
                                 ch);
                        return;
                }

                if (IS_OBJ_STAT(obj, ITEM_NOCLAIM))
                {
                        send_to_char
                                ("This item cannot be put in containers.\n\r",
                                 ch);
                        return;
                }

                if (!can_drop_obj(ch, obj))
                {
                        send_to_char("You can't let go of it.\n\r", ch);
                        return;
                }

                if (get_obj_weight(obj) + get_obj_weight(container) >
                    container->value[0])
                {
                        send_to_char("It won't fit.\n\r", ch);
                        return;
                }

                for (obj2 = container->contains; obj2 != NULL;
                     obj2 = obj_next2)
                {
                        obj_next2 = obj2->next_content;
                        if (obj2->chobj != NULL && obj != obj2)
                                act("A hand reaches inside $P and drops $p.",
                                    obj2->chobj, obj, container, TO_CHAR);
                }
                obj_from_char(obj);
                obj_to_obj(obj, container);
                act("$n puts $p in $P.", ch, obj, container, TO_ROOM);
                act("You put $p in $P.", ch, obj, container, TO_CHAR);
        }
        else
        {
                /*
                 * 'put all container' or 'put all.obj container' 
                 */
                for (obj = ch->carrying; obj != NULL; obj = obj_next)
                {
                        obj_next = obj->next_content;

                        if ((arg1[3] == '\0' || is_name(&arg1[4], obj->name))
                            && can_see_obj(ch, obj)
                            && obj->wear_loc == WEAR_NONE
                            && obj != container
                            && !IS_SET(obj->quest, QUEST_ARTIFACT)
                            && !IS_OBJ_STAT(obj, ITEM_NOCLAIM)
                            && can_drop_obj(ch, obj)
                            && get_obj_weight(obj) + get_obj_weight(container)
                            <= container->value[0])
                        {
                                for (obj2 = container->contains; obj2 != NULL;
                                     obj2 = obj_next2)
                                {
                                        obj_next2 = obj2->next_content;
                                        if (obj2->chobj != NULL
                                            && obj2->chobj->in_room != NULL)
                                        {
                                                if (objroom !=
                                                    get_room_index(obj2->
                                                                   chobj->
                                                                   in_room->
                                                                   vnum))
                                                {
                                                        char_from_room(obj2->
                                                                       chobj);
                                                        char_to_room(obj2->
                                                                     chobj,
                                                                     objroom);
                                                        do_look(obj2->chobj,
                                                                "auto");
                                                }
                                                if (obj != obj2)
                                                        act("A hand reaches inside $P and drops $p.", obj2->chobj, obj, container, TO_CHAR);
                                        }
                                }
                                obj_from_char(obj);
                                obj_to_obj(obj, container);
                                act("$n puts $p in $P.", ch, obj, container,
                                    TO_ROOM);
                                act("You put $p in $P.", ch, obj, container,
                                    TO_CHAR);
                        }
                }
        }
        do_autosave(ch, "");
        return;
}

void do_drop(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        OBJ_DATA *obj_next;
        bool      found;
        char      buf[MSL];

        if (IS_NPC(ch))
                return;

        argument = one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                send_to_char("Drop what?\n\r", ch);
                return;
        }

        if (is_number(arg))
        {
                /*
                 * 'drop NNNN bones' 
                 */
                int       amount;

                amount = atoi(arg);
                argument = one_argument(argument, arg);
                if (amount <= 0
                    || (str_cmp(arg, "bones") && str_cmp(arg, "bone")))
                {
                        send_to_char("Sorry, you can't do that.\n\r", ch);
                        return;
                }

                /*
                 * Otherwise causes complications if there's a pile on each plane 
                 */
                if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
                {
                        send_to_char
                                ("You cannot drop bones in the shadowplane.\n\r",
                                 ch);
                        return;
                }

                if (ch->bones < amount)
                {
                        send_to_char("You haven't got that many bones.\n\r",
                                     ch);
                        return;
                }

                ch->bones -= amount;

                for (obj = ch->in_room->contents; obj != NULL; obj = obj_next)
                {
                        obj_next = obj->next_content;

                        switch (obj->pIndexData->vnum)
                        {
                        case OBJ_VNUM_MONEY_ONE:
                                amount += 1;
                                extract_obj(obj);
                                break;

                        case OBJ_VNUM_MONEY_SOME:
                                amount += obj->value[0];
                                extract_obj(obj);
                                break;
                        }
                }
                obj_to_room(create_money(amount), ch->in_room);
                act("$n drops some bones.", ch, NULL, NULL, TO_ROOM);
                send_to_char("OK.\n\r", ch);
                do_autosave(ch, "");
                return;
        }

        if (str_cmp(arg, "all") && str_prefix("all.", arg))
        {
                /*
                 * 'drop obj' 
                 */
                if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
                {
                        send_to_char("You do not have that item.\n\r", ch);
                        return;
                }

                if (!can_drop_obj(ch, obj))
                {
                        send_to_char("You can't let go of it.\n\r", ch);
                        return;
                }

                obj_from_char(obj);
                if (!IS_SET(obj->quest, QUEST_ARTIFACT))
                {
                        obj_to_room(obj, ch->in_room);
                }


                /*
                 * Objects should only have a shadowplane flag when on the floor 
                 */
                if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                    (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)))
                        SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
                if (IS_SET(obj->quest, QUEST_ARTIFACT) && !IS_NPC(ch))
                {
                        xprintf(buf, "%s has returned to the game",
                                obj->short_descr);
                        do_info(ch, buf);
                        //obj_from_room(obj);
                        xprintf(buf, "%d", obj->pIndexData->vnum);
                        extract_obj(obj);
                        do_randomload(ch, buf);
                }

                act("$n drops $p.", ch, obj, NULL, TO_ROOM);
                act("You drop $p.", ch, obj, NULL, TO_CHAR);
                if (HAS_TRIGGER_OBJ(obj, TRIG_DROP))
                        p_give_trigger(NULL, obj, NULL, ch, obj, TRIG_DROP);
                if (HAS_TRIGGER_ROOM(ch->in_room, TRIG_DROP))
                        p_give_trigger(NULL, NULL, ch->in_room, ch, obj,
                                       TRIG_DROP);
        }
        else
        {
                /*
                 * 'drop all' or 'drop all.obj' 
                 */
                found = FALSE;
                for (obj = ch->carrying; obj != NULL; obj = obj_next)
                {
                        obj_next = obj->next_content;

                        if ((arg[3] == '\0' || is_name(&arg[4], obj->name))
                            && can_see_obj(ch, obj)
                            && obj->wear_loc == WEAR_NONE
                            && can_drop_obj(ch, obj))
                        {
                                found = TRUE;
                                obj_from_char(obj);
                                if (!IS_SET(obj->quest, QUEST_ARTIFACT))
                                {
                                        obj_to_room(obj, ch->in_room);
                                }
                                /*
                                 * Objects should only have a shadowplane flag when on the floor 
                                 */
                                if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                                    (!IS_SET
                                     (obj->extra_flags, ITEM_SHADOWPLANE)))
                                        SET_BIT(obj->extra_flags,
                                                ITEM_SHADOWPLANE);
                                if (IS_SET(obj->quest, QUEST_ARTIFACT)
                                    && !IS_NPC(ch))
                                {
                                        xprintf(buf,
                                                "%s has returned to the game",
                                                obj->short_descr);
                                        do_info(ch, buf);
                                        obj_from_room(obj);
                                        xprintf(buf, "%d",
                                                obj->pIndexData->vnum);
                                        extract_obj(obj);
                                        do_randomload(ch, buf);
                                }


                                act("$n drops $p.", ch, obj, NULL, TO_ROOM);
                                act("You drop $p.", ch, obj, NULL, TO_CHAR);
                                if (HAS_TRIGGER_OBJ(obj, TRIG_DROP))
                                        p_give_trigger(NULL, obj, NULL, ch,
                                                       obj, TRIG_DROP);
                                if (HAS_TRIGGER_ROOM(ch->in_room, TRIG_DROP))
                                        p_give_trigger(NULL, NULL,
                                                       ch->in_room, ch, obj,
                                                       TRIG_DROP);
                        }
                }

                if (!found)
                {
                        if (arg[3] == '\0')
                                act("You are not carrying anything.", ch,
                                    NULL, arg, TO_CHAR);
                        else
                                act("You are not carrying any $T.", ch, NULL,
                                    &arg[4], TO_CHAR);
                }
        }


        do_autosave(ch, "");
        return;
}

void do_give(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        char      buf[MSL];
        CHAR_DATA *victim;
        OBJ_DATA *obj;
        extern bool quest;
        extern bool auto_quest;
        extern CHAR_DATA *quest_mob;
        extern OBJ_DATA *aquest_object;

        if (IS_NPC(ch))
                return;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);

        if (arg1[0] == '\0' || arg2[0] == '\0')
        {
                send_to_char("Give what to whom?\n\r", ch);
                return;
        }

        if (is_number(arg1))
        {
                /*
                 * 'give NNNN bones victim' 
                 */
                int       amount;

                amount = atoi(arg1);
                if (amount <= 0
                    || (str_cmp(arg2, "bones") && str_cmp(arg2, "bone")))
                {
                        send_to_char("Sorry, you can't do that.\n\r", ch);
                        return;
                }

                argument = one_argument(argument, arg2);
                if (arg2[0] == '\0')
                {
                        send_to_char("Give what to whom?\n\r", ch);
                        return;
                }

                if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
                {
                        send_to_char("They aren't here.\n\r", ch);
                        return;
                }

                if (IS_AFFECTED(victim, AFF_ETHEREAL))
                {
                        send_to_char
                                ("You cannot give things to ethereal people.\n\r",
                                 ch);
                        return;
                }

                if (ch->bones < amount)
                {
                        send_to_char("You haven't got that many bones.\n\r",
                                     ch);
                        return;
                }

                ch->bones -= amount;
                victim->bones += amount;
                act("$n gives you some bones.", ch, NULL, victim, TO_VICT);
                act("$n gives $N some bones.", ch, NULL, victim, TO_NOTVICT);
                act("You give $N some bones.", ch, NULL, victim, TO_CHAR);
                send_to_char("OK.\n\r", ch);
                if (IS_NPC(victim) && HAS_TRIGGER_MOB(victim, TRIG_BRIBE))
                        p_bribe_trigger(victim, ch, amount);
                return;
        }



        if ((obj = get_obj_carry(ch, arg1, ch)) == NULL)
        {
                send_to_char("You do not have that item.\n\r", ch);
                return;
        }

        if (obj->wear_loc != WEAR_NONE)
        {
                send_to_char("You must remove it first.\n\r", ch);
                return;
        }
        if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }
        if (IS_SET(obj->quest, QUEST_ARTIFACT) && !IS_IMMORTAL(ch))
        {
                stc("The artifact does not want to leave you.\n\r", ch);
                return;
        }

        if (!can_drop_obj(ch, obj))
        {
                send_to_char("You can't let go of it.\n\r", ch);
                return;
        }

        if (IS_AFFECTED(victim, AFF_ETHEREAL))
        {
                send_to_char("You cannot give things to ethereal people.\n\r",
                             ch);
                return;
        }

        if (victim->carry_number + 1 > can_carry_n(victim))
        {
                act("$N has $S hands full.", ch, NULL, victim, TO_CHAR);
                return;
        }

        if (victim->shop_fun)
        {
                send_to_char("They don't want it.\n\r", ch);
                return;
        }

        if (victim->carry_weight + get_obj_weight(obj) > can_carry_w(victim))
        {
                act("$N can't carry that much weight.", ch, NULL, victim,
                    TO_CHAR);
                return;
        }

        if (!can_see_obj(victim, obj))
        {
                act("$N can't see it.", ch, NULL, victim, TO_CHAR);
                return;
        }

        obj_from_char(obj);
        obj_to_char(obj, victim);
        MOBtrigger = FALSE;
        act("$n gives $p to $N.", ch, obj, victim, TO_NOTVICT);
        act("$n gives you $p.", ch, obj, victim, TO_VICT);
        act("You give $p to $N.", ch, obj, victim, TO_CHAR);
        MOBtrigger = TRUE;

        if (HAS_TRIGGER_OBJ(obj, TRIG_GIVE))
                p_give_trigger(NULL, obj, NULL, ch, obj, TRIG_GIVE);
        if (HAS_TRIGGER_ROOM(ch->in_room, TRIG_GIVE))
                p_give_trigger(NULL, NULL, ch->in_room, ch, obj, TRIG_GIVE);

        /*
         * Give trigger
         */
        if (IS_NPC(victim) && HAS_TRIGGER_MOB(victim, TRIG_GIVE))
                p_give_trigger(victim, NULL, NULL, ch, obj, TRIG_GIVE);

        if ((quest || auto_quest)
            && IS_NPC(victim) && victim == quest_mob && obj == aquest_object)
        {
                /*
                 * Then ch has recovered the quest object!!!!! 
                 */
                xprintf(buf, "Oh! %s you found %s for me!  Thank You!",
                        ch->name, obj->short_descr);
                do_say(victim, buf);
                interpret(victim, "hop");
                interpret(victim, "clap");

                if (!IS_NPC(ch))
                {
                        xprintf(buf,
                                "%s I shall reward you well for recovering this for me!",
                                ch->name);
                        do_say(victim, buf);
                        give_token(victim, ch, 5000);
                }
                quest_complete(ch);
        }

        WAIT_STATE(ch, 6);
        return;
}

void do_fill(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        OBJ_DATA *fountain;
        bool      found;
        int       liquid;

        one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                send_to_char("Fill what?\n\r", ch);
                return;
        }

        if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
        {
                send_to_char("You do not have that item.\n\r", ch);
                return;
        }

        found = FALSE;
        for (fountain = ch->in_room->contents; fountain != NULL;
             fountain = fountain->next_content)
        {
                if (fountain->item_type == ITEM_FOUNTAIN)
                {
                        found = TRUE;
                        break;
                }
        }

        if (!found)
        {
                send_to_char("There is no fountain here!\n\r", ch);
                return;
        }

        if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
            fountain->in_room != NULL &&
            !IS_SET(fountain->extra_flags, ITEM_SHADOWPLANE))
        {
                send_to_char("You are too insubstantual.\n\r", ch);
                return;
        }
        else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                 fountain->in_room != NULL &&
                 IS_SET(fountain->extra_flags, ITEM_SHADOWPLANE))
        {
                send_to_char("It is too insubstantual.\n\r", ch);
                return;
        }
        else if (IS_AFFECTED(ch, AFF_ETHEREAL))
        {
                send_to_char("You cannot fill containers while ethereal.\n\r",
                             ch);
                return;
        }

        if (obj->item_type != ITEM_DRINK_CON)
        {
                send_to_char("You can't fill that.\n\r", ch);
                return;
        }

        if (obj->value[1] >= obj->value[0])
        {
                send_to_char("Your container is already full.\n\r", ch);
                return;
        }

        if ((obj->value[2] != fountain->value[2]) && obj->value[1] > 0)
        {
                send_to_char("You cannot mix two different liquids.\n\r", ch);
                return;
        }

        act("$n dips $p into $P.", ch, obj, fountain, TO_ROOM);
        act("You dip $p into $P.", ch, obj, fountain, TO_CHAR);
        obj->value[2] = fountain->value[2];
        obj->value[1] = obj->value[0];
        liquid = obj->value[2];
        act("$n fills $p with $T.", ch, obj, liq_table[liquid].liq_name,
            TO_ROOM);
        act("You fill $p with $T.", ch, obj, liq_table[liquid].liq_name,
            TO_CHAR);
        return;
}

void do_drink(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        int       amount;
        int       liquid;

        one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                for (obj = ch->in_room->contents; obj;
                     obj = obj->next_content)
                {
                        if (obj->item_type == ITEM_FOUNTAIN)
                                break;
                }

                if (obj == NULL)
                {
                        send_to_char("Drink what?\n\r", ch);
                        return;
                }
        }
        else
        {
                if ((obj = get_obj_here(ch, NULL, arg)) == NULL)
                {
                        send_to_char("You can't find it.\n\r", ch);
                        return;
                }
        }

        if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
        {
                send_to_char("You fail to reach your mouth.  *Hic*\n\r", ch);
                return;
        }

        switch (obj->item_type)
        {
        default:
                send_to_char("You can't drink from that.\n\r", ch);
                break;

        case ITEM_POTION:
                do_quaff(ch, obj->name);
                return;
        case ITEM_FOUNTAIN:
                if ((liquid = obj->value[2]) >= LIQ_MAX)
                {
                        bug("Do_drink: bad liquid number %d.", liquid);
                        liquid = obj->value[2] = 0;
                }

                if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                    obj->in_room != NULL &&
                    !IS_SET(obj->extra_flags, ITEM_SHADOWPLANE))
                {
                        send_to_char("You are too insubstantual.\n\r", ch);
                        break;
                }
                else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                         obj->in_room != NULL &&
                         IS_SET(obj->extra_flags, ITEM_SHADOWPLANE))
                {
                        send_to_char("It is too insubstantual.\n\r", ch);
                        break;
                }
                else if (IS_AFFECTED(ch, AFF_ETHEREAL))
                {
                        send_to_char
                                ("You can only drink from things you are carrying while ethereal.\n\r",
                                 ch);
                        return;
                }

                if (liquid != 13 && liquid != 16 && liquid != 17
                    && liquid != 18 && IS_CLASS(ch, CLASS_VAMPIRE))
                {
                        send_to_char("You can only drink blood.\n\r", ch);
                        break;
                }


                if (liquid == 13 && IS_CLASS(ch, CLASS_VAMPIRE))
                {
                        ch->pcdata->condition[COND_THIRST] +=
                                number_range(200, 300);
                }

                act("$n drinks $T from $p.", ch, obj,
                    liq_table[liquid].liq_name, TO_ROOM);
                act("You drink $T from $p.", ch, obj,
                    liq_table[liquid].liq_name, TO_CHAR);

                amount = number_range(25, 50);
                amount = UMIN(amount, obj->value[1]);

                gain_condition(ch, COND_DRUNK,
                               amount *
                               liq_table[liquid].liq_affect[COND_DRUNK]);
                gain_condition(ch, COND_FULL,
                               amount *
                               liq_table[liquid].liq_affect[COND_FULL]);
                gain_condition(ch, COND_THIRST,
                               amount *
                               liq_table[liquid].liq_affect[COND_THIRST]);

                if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
                    ch->pcdata->condition[COND_DRUNK] > 10)
                        send_to_char("You feel drunk.\n\r", ch);
                if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
                    ch->pcdata->condition[COND_FULL] > 50)
                        send_to_char("You are full.\n\r", ch);
                if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
                    ch->pcdata->condition[COND_THIRST] > 50)
                        send_to_char("You do not feel thirsty.\n\r", ch);
                if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) &&
                    ch->pcdata->condition[COND_THIRST] >=
                    20000 / ch->generation && ch->generation < 4)
                        send_to_char("Your blood thirst is sated.\n\r", ch);
                else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) &&
                         ch->pcdata->condition[COND_THIRST] >=
                         5000 / ch->generation)
                        send_to_char("Your blood thirst is sated.\n\r", ch);

                if (obj->value[3] != 0
                    && (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE)))
                {
                        /*
                         * The shit was poisoned ! 
                         */
                        AFFECT_DATA af;

                        act("$n chokes and gags.", ch, NULL, NULL, TO_ROOM);
                        send_to_char("You choke and gag.\n\r", ch);
                        af.type = gsn_poison;
                        af.duration = 3 * amount;
                        af.location = APPLY_NONE;
                        af.modifier = 0;
                        af.bitvector = AFF_POISON;
                        affect_join(ch, &af);
                }
                break;

        case ITEM_DRINK_CON:
                if (obj->value[1] <= 0)
                {
                        send_to_char("It is already empty.\n\r", ch);
                        return;
                }

                if ((liquid = obj->value[2]) >= LIQ_MAX)
                {
                        bug("Do_drink: bad liquid number %d.", liquid);
                        liquid = obj->value[2] = 0;
                }

                if (liquid != 13 && IS_CLASS(ch, CLASS_VAMPIRE))
                {
                        send_to_char("You can only drink blood.\n\r", ch);
                        break;
                }


                if (liquid == 13 && IS_CLASS(ch, CLASS_VAMPIRE))
                {
                        ch->pcdata->condition[COND_THIRST] +=
                                number_range(200, 300);
                }

                act("$n drinks $T from $p.", ch, obj,
                    liq_table[liquid].liq_name, TO_ROOM);
                act("You drink $T from $p.", ch, obj,
                    liq_table[liquid].liq_name, TO_CHAR);

                amount = number_range(25, 50);
                amount = UMIN(amount, obj->value[1]);

                gain_condition(ch, COND_DRUNK,
                               amount *
                               liq_table[liquid].liq_affect[COND_DRUNK]);
                gain_condition(ch, COND_FULL,
                               amount *
                               liq_table[liquid].liq_affect[COND_FULL]);
                gain_condition(ch, COND_THIRST,
                               amount *
                               liq_table[liquid].liq_affect[COND_THIRST]);

                if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
                    ch->pcdata->condition[COND_DRUNK] > 10)
                        send_to_char("You feel drunk.\n\r", ch);
                if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
                    ch->pcdata->condition[COND_FULL] > 50)
                        send_to_char("You are full.\n\r", ch);
                if (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
                    ch->pcdata->condition[COND_THIRST] > 50)
                        send_to_char("You do not feel thirsty.\n\r", ch);
                if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) &&
                    ch->pcdata->condition[COND_THIRST] >=
                    20000 / ch->generation)
                        send_to_char("Your blood thirst is sated.\n\r", ch);

                if (obj->value[3] != 0
                    && (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE)))
                {
                        /*
                         * The shit was poisoned ! 
                         */
                        AFFECT_DATA af;

                        act("$n chokes and gags.", ch, NULL, NULL, TO_ROOM);
                        send_to_char("You choke and gag.\n\r", ch);
                        af.type = gsn_poison;
                        af.duration = 3 * amount;
                        af.location = APPLY_NONE;
                        af.modifier = 0;
                        af.bitvector = AFF_POISON;
                        affect_join(ch, &af);
                }

                obj->value[1] -= amount;
                if (obj->value[1] <= 0)
                {
                        obj->value[1] = 0;
                }
                break;
        }

        return;
}

void do_empty(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        int       liquid;

        one_argument(argument, arg);

        if (arg[0] == '\0')
        {
                send_to_char("Empty what?\n\r", ch);
                return;
        }

        if ((obj = get_obj_here(ch, NULL, arg)) == NULL)
        {
                send_to_char("You can't find it.\n\r", ch);
                return;
        }

        switch (obj->item_type)
        {
        default:
                send_to_char("You cannot empty that.\n\r", ch);
                break;

        case ITEM_DRINK_CON:
                if (obj->value[1] <= 0)
                {
                        send_to_char("It is already empty.\n\r", ch);
                        return;
                }

                if ((liquid = obj->value[2]) >= LIQ_MAX)
                {
                        bug("Do_drink: bad liquid number %d.", liquid);
                        liquid = obj->value[2] = 0;
                }

                act("$n empties $T from $p.", ch, obj,
                    liq_table[liquid].liq_name, TO_ROOM);
                act("You empty $T from $p.", ch, obj,
                    liq_table[liquid].liq_name, TO_CHAR);

                obj->value[1] = 0;
                break;
        }

        return;
}

void do_eat(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        int       level;

        one_argument(argument, arg);
        if (arg[0] == '\0')
        {
                send_to_char("Eat what?\n\r", ch);
                return;
        }

        if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
        {
                send_to_char("You do not have that item.\n\r", ch);
                return;
        }

        if (!IS_IMMORTAL(ch))
        {
                if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE)
                    && obj->item_type == ITEM_FOOD)
                {
                        send_to_char("You are unable to stomach it.\n\r", ch);
                        return;
                }

                if (obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL
                    && obj->item_type != ITEM_EGG
                    && obj->item_type != ITEM_QUEST
                    && obj->item_type != ITEM_DRAGONGEM
                    && obj->item_type != ITEM_DTOKEN)
                {
                        if (IS_NPC(ch) || !IS_SET(ch->special, SPC_WOLFMAN) ||
                            obj->item_type != ITEM_TRASH)
                        {
                                send_to_char("That's not edible.\n\r", ch);
                                return;
                        }
                }

                if (!IS_NPC(ch) && ch->pcdata->condition[COND_FULL] > 50 &&
                    obj->item_type != ITEM_TRASH
                    && obj->item_type != ITEM_QUEST
                    && obj->item_type != ITEM_PILL
                    && obj->item_type != ITEM_DRAGONGEM
                    && obj->item_type != ITEM_FAITHTOKEN)
                {
                        send_to_char("You are too full to eat more.\n\r", ch);
                        return;
                }
                if (!IS_NPC(ch) && obj->item_type != ITEM_QUEST
                    && obj->item_type != ITEM_PILL
                    && obj->item_type != ITEM_FAITHTOKEN
                    && obj->item_type != ITEM_FOOD)
                {
                        send_to_char("You can't eat that.\n\r", ch);
                        return;
                }
        }

        act("$n eats $p.", ch, obj, NULL, TO_ROOM);
        act("You eat $p.", ch, obj, NULL, TO_CHAR);

        switch (obj->item_type)
        {
        default:
                break;

        case ITEM_FOOD:
                if (!IS_NPC(ch))
                {
                        int       condition;

                        condition = ch->pcdata->condition[COND_FULL];
                        gain_condition(ch, COND_FULL, obj->value[0]);
                        if (condition == 0
                            && ch->pcdata->condition[COND_FULL] > 10)
                                send_to_char("You are no longer hungry.\n\r",
                                             ch);
                        else if (ch->pcdata->condition[COND_FULL] > 50)
                                send_to_char("You are full.\n\r", ch);
                }

                if (obj->value[3] != 0)
                {
                        /*
                         * The shit was poisoned! 
                         */
                        AFFECT_DATA af;

                        act("$n chokes and gags.", ch, NULL, NULL, TO_ROOM);
                        send_to_char("You choke and gag.\n\r", ch);

                        af.type = gsn_poison;
                        af.duration = 2 * obj->value[0];
                        af.location = APPLY_NONE;
                        af.modifier = 0;
                        af.bitvector = AFF_POISON;
                        affect_join(ch, &af);
                }
                break;

        case ITEM_PILL:
                level = obj->value[0];
                if (level < 1)
                        level = 1;
                if (level > MAX_SPELL)
                        level = MAX_SPELL;

                obj_cast_spell(obj->value[1], level, ch, ch, NULL);
                obj_cast_spell(obj->value[2], level, ch, ch, NULL);
                obj_cast_spell(obj->value[3], level, ch, ch, NULL);
                if (ch->position == POS_FIGHTING)
                {
                        if (!IS_IMMORTAL(ch))
                                WAIT_STATE(ch, 6);
                }
                break;

        case ITEM_QUEST:
                if (!IS_NPC(ch))
                        ch->bones += obj->value[0];
                break;

        case ITEM_FAITHTOKEN:
                if (!IS_NPC(ch))
                {
                        ch->pcdata->current_faith += obj->value[0];
                        if (ch->pcdata->current_faith > ch->pcdata->faith)
                                ch->pcdata->current_faith = ch->pcdata->faith;
                }
                break;

        case ITEM_DTOKEN:
                if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON))
                        ch->pcdata->stats[DEMON_CURRENT] += obj->value[0];
                break;

        case ITEM_EGG:
                if (!IS_NPC(ch))
                {
                        int       condition;

                        condition = ch->pcdata->condition[COND_FULL];
                        gain_condition(ch, COND_FULL, obj->value[1]);
                        if (condition == 0
                            && ch->pcdata->condition[COND_FULL] > 10)
                                send_to_char("You are no longer hungry.\n\r",
                                             ch);
                        else if (ch->pcdata->condition[COND_FULL] > 50)
                                send_to_char("You are full.\n\r", ch);
                }

                /*
                 * Note to myself...remember to set v2 for mobiles that hatch within
                 * * the player (like aliens ;).  KaVir.
                 */

                if (obj->value[3] != 0)
                {
                        /*
                         * The shit was poisoned! 
                         */
                        AFFECT_DATA af;

                        act("$n chokes and gags.", ch, NULL, NULL, TO_ROOM);
                        send_to_char("You choke and gag.\n\r", ch);

                        af.type = gsn_poison;
                        af.duration = 2 * obj->value[0];
                        af.location = APPLY_NONE;
                        af.modifier = 0;
                        af.bitvector = AFF_POISON;
                        affect_join(ch, &af);
                }
                break;
        }

        if (obj != NULL)
                extract_obj(obj);
        return;
}

/*
 * Remove an object.
 */
bool remove_obj(CHAR_DATA * ch, int iWear, bool fReplace)
{
        OBJ_DATA *obj;

        if ((obj = get_eq_char(ch, iWear)) == NULL)
                return TRUE;

        if (!fReplace)
                return FALSE;

        if (IS_SET(obj->extra_flags, ITEM_NOREMOVE) && !IS_IMMORTAL(ch))
        {
                act("You can't remove $p.", ch, obj, NULL, TO_CHAR);
                return FALSE;
        }

        unequip_char(ch, obj);
        act("$n stops using $p.", ch, obj, NULL, TO_ROOM);
        act("You stop using $p.", ch, obj, NULL, TO_CHAR);
        return TRUE;
}

/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
void wear_obj(CHAR_DATA * ch, OBJ_DATA * obj, bool fReplace)
{
        bool      wolf_ok = FALSE;

        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF))
                wolf_ok = TRUE;

        if (CAN_WEAR(obj, ITEM_WIELD) || CAN_WEAR(obj, ITEM_HOLD))
        {
                if (get_eq_char(ch, WEAR_WIELD) != NULL
                    && get_eq_char(ch, WEAR_HOLD) != NULL )
//                    && !remove_obj(ch, WEAR_WIELD, fReplace)
//                    && !remove_obj(ch, WEAR_HOLD, fReplace))
		{
		  // quick fix by Akurei
		  int blah1 = FALSE, blah2 = FALSE;
		  if( IS_SET(ch->newbits, THIRD_HAND) && get_eq_char(ch, WEAR_THIRD) == NULL ) blah1 = TRUE;
		  if( IS_SET(ch->newbits, FOURTH_HAND) && get_eq_char(ch, WEAR_FOURTH) == NULL ) blah2 = TRUE;
		  if( !blah1 && !blah2 )
                        return;
		}
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WIELD))
                {
                        send_to_char("You are unable to use it.\n\r", ch);
                        return;
                }

                if (get_eq_char(ch, WEAR_WIELD) == NULL
                    && is_ok_to_wear(ch, wolf_ok, "right_hand"))
                {
                        {
                                act("$n clutches $p in $s right hand.", ch,
                                    obj, NULL, TO_ROOM);
                                act("You clutch $p in your right hand.", ch,
                                    obj, NULL, TO_CHAR);
                        }
                        equip_char(ch, obj, WEAR_WIELD);
                        return;
                }
                else if (get_eq_char(ch, WEAR_HOLD) == NULL
                         && is_ok_to_wear(ch, wolf_ok, "left_hand"))
                {
                        {
                                act("$n clutches $p in $s left hand.", ch,
                                    obj, NULL, TO_ROOM);
                                act("You clutch $p in your left hand.", ch,
                                    obj, NULL, TO_CHAR);
                        }
                        if (obj->item_type == ITEM_WEAPON)
                        {
                                if (!IS_NPC(ch)
                                    && (obj->pIndexData->vnum == 30000
                                        || IS_OBJ_STAT(obj, ITEM_LOYAL)))
                                {
                                        if (obj->questowner != NULL
                                            && str_cmp(ch->pcdata->switchname,
                                                       obj->questowner)
                                            && strlen(obj->questowner) > 1)
                                        {
                                                act("$p leaps out of $n's hand.", ch, obj, NULL, TO_ROOM);
                                                act("$p leaps out of your hand.", ch, obj, NULL, TO_CHAR);
                                                obj_from_char(obj);
                                                obj_to_room(obj, ch->in_room);
                                                return;
                                        }
                                }
                                equip_char(ch, obj, WEAR_HOLD);
                                if (!IS_NPC(ch))
                                        do_skill(ch, ch->name);
                                return;
                        }
                        equip_char(ch, obj, WEAR_HOLD);
                        return;
                }
                else if (get_eq_char(ch, WEAR_THIRD) == NULL
                         && is_ok_to_wear(ch, wolf_ok, "third_hand"))
                {
                        {
                                act("$n clutches $p in $s third hand.", ch,
                                    obj, NULL, TO_ROOM);
                                act("You clutch $p in your third hand.", ch,
                                    obj, NULL, TO_CHAR);
                        }
                        equip_char(ch, obj, WEAR_THIRD);
                        return;
                }
                else if (get_eq_char(ch, WEAR_FOURTH) == NULL
                         && is_ok_to_wear(ch, wolf_ok, "fourth_hand"))
                {
			{
                                act("$n clutches $p in $s fourth hand.", ch,
                                    obj, NULL, TO_ROOM);
                                act("You clutch $p in your fourth hand.", ch,
                                    obj, NULL, TO_CHAR);
                        }
                        if (obj->item_type == ITEM_WEAPON)
                        {
                                if (!IS_NPC(ch)
                                    && (obj->pIndexData->vnum == 30000
                                        || IS_OBJ_STAT(obj, ITEM_LOYAL)))
                                {
                                        if (obj->questowner != NULL
                                            && str_cmp(ch->pcdata->switchname,
                                                       obj->questowner)
                                            && strlen(obj->questowner) > 1)
                                        {
                                                act("$p leaps out of $n's hand.", ch, obj, NULL, TO_ROOM);
                                                act("$p leaps out of your hand.", ch, obj, NULL, TO_CHAR);
                                                obj_from_char(obj);
                                                obj_to_room(obj, ch->in_room);
                                                return;
                                        }
                                }
                                equip_char(ch, obj, WEAR_FOURTH);
                                if (!IS_NPC(ch))
                                        do_skill(ch, ch->name);
                                return;
                        }
                        equip_char(ch, obj, WEAR_FOURTH);
                        return;
                }
                if (!is_ok_to_wear(ch, wolf_ok, "left_hand")
                    && !is_ok_to_wear(ch, wolf_ok, "right_hand"))
                        send_to_char
                                ("You cannot use anything in your hands.\n\r",
                                 ch);
                else
                        send_to_char("You have no free hands.\n\r", ch);
                return;
        }
        if (obj->item_type == ITEM_LIGHT)
        {
                if (!remove_obj(ch, WEAR_LIGHT, fReplace))
                        return;
                act("$n lights $p and holds it.", ch, obj, NULL, TO_ROOM);
                act("You light $p and hold it.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_LIGHT);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_FINGER))
        {
                if (get_eq_char(ch, WEAR_FINGER_L) != NULL
                    && get_eq_char(ch, WEAR_FINGER_R) != NULL
                    && !remove_obj(ch, WEAR_FINGER_L, fReplace)
                    && !remove_obj(ch, WEAR_FINGER_R, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_FINGER))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }

                if (get_eq_char(ch, WEAR_FINGER_L) == NULL
                    && is_ok_to_wear(ch, wolf_ok, "left_finger"))
                {
                        act("$n wears $p on $s left finger.", ch, obj, NULL,
                            TO_ROOM);
                        act("You wear $p on your left finger.", ch, obj, NULL,
                            TO_CHAR);
                        equip_char(ch, obj, WEAR_FINGER_L);
                        return;
                }
                else if (get_eq_char(ch, WEAR_FINGER_R) == NULL
                         && is_ok_to_wear(ch, wolf_ok, "right_finger"))
                {
                        act("$n wears $p on $s right finger.", ch, obj, NULL,
                            TO_ROOM);
                        act("You wear $p on your right finger.", ch, obj,
                            NULL, TO_CHAR);
                        equip_char(ch, obj, WEAR_FINGER_R);
                        return;
                }
                if (!is_ok_to_wear(ch, wolf_ok, "left_finger")
                    && !is_ok_to_wear(ch, wolf_ok, "right_finger"))
                        send_to_char("You cannot wear any rings.\n\r", ch);
                else
                        send_to_char("You cannot wear any more rings.\n\r",
                                     ch);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_NECK))
        {
                if (get_eq_char(ch, WEAR_NECK_1) != NULL
                    && get_eq_char(ch, WEAR_NECK_2) != NULL
                    && !remove_obj(ch, WEAR_NECK_1, fReplace)
                    && !remove_obj(ch, WEAR_NECK_2, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_NECK))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }

                if (get_eq_char(ch, WEAR_NECK_1) == NULL)
                {
                        act("$n slips $p around $s neck.", ch, obj, NULL,
                            TO_ROOM);
                        act("You slip $p around your neck.", ch, obj, NULL,
                            TO_CHAR);
                        equip_char(ch, obj, WEAR_NECK_1);
                        return;
                }

                if (get_eq_char(ch, WEAR_NECK_2) == NULL)
                {
                        act("$n slips $p around $s neck.", ch, obj, NULL,
                            TO_ROOM);
                        act("You slip $p around your neck.", ch, obj, NULL,
                            TO_CHAR);
                        equip_char(ch, obj, WEAR_NECK_2);
                        return;
                }
                bug("Wear_obj: no free neck.", 0);
                send_to_char
                        ("You are already wearing two things around your neck.\n\r",
                         ch);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_BODY))
        {
                if (!remove_obj(ch, WEAR_BODY, fReplace))
                        return;

                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_BODY))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }
                act("$n fits $p on $s body.", ch, obj, NULL, TO_ROOM);
                act("You fit $p on your body.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_BODY);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_HEAD))
        {
                if (!remove_obj(ch, WEAR_HEAD, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_HEAD))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }
                if (!is_ok_to_wear(ch, wolf_ok, "head"))
                {
                        send_to_char("You have no head to wear it on.\n\r",
                                     ch);
                        return;
                }
                act("$n places $p on $s head.", ch, obj, NULL, TO_ROOM);
                act("You place $p on your head.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_HEAD);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_FACE))
        {
                if (!remove_obj(ch, WEAR_FACE, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_HEAD))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }
                if (!is_ok_to_wear(ch, wolf_ok, "face"))
                {
                        send_to_char("You have no face to wear it on.\n\r",
                                     ch);
                        return;
                }
                act("$n places $p on $s face.", ch, obj, NULL, TO_ROOM);
                act("You place $p on your face.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_FACE);
                return;
        }

        if (obj->item_type == ITEM_FAMILIAR)
        {
                if (!remove_obj(ch, WEAR_FAMILIAR, fReplace))
                        return;

                if (get_eq_char(ch, WEAR_FAMILIAR) != NULL
                    && !remove_obj(ch, WEAR_FAMILIAR, fReplace))
                        return;

                if (IS_NPC(ch))
                {
                        send_to_char("You are unable to summon it.\n\r", ch);
                        return;
                }

                if ((ch->alignment > 0 && obj->value[7] < 0)
                    || (ch->alignment < 0 && obj->value[7] > 0))
                {
                        stc("You may not summon a familiar with opposite alignment!\n\r", ch);
                        return;
                }

                if (ch->tier < obj->value[3])
                {
                        stc("Your tier level is too low!\n\r", ch);
                        return;
                }

                act("$n summons $p to accompany $m.", ch, obj, NULL, TO_ROOM);
                act("You summon $p to accompany you.", ch, obj, NULL,
                    TO_CHAR);
                equip_char(ch, obj, WEAR_FAMILIAR);
                return;
        }



        if (CAN_WEAR(obj, ITEM_WEAR_LEGS))
        {
                if (!remove_obj(ch, WEAR_LEGS, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_LEGS))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }
                if (!is_ok_to_wear(ch, wolf_ok, "legs"))
                {
                        send_to_char("You have no legs to wear them on.\n\r",
                                     ch);
                        return;
                }
                act("$n slips $s legs into $p.", ch, obj, NULL, TO_ROOM);
                act("You slip your legs into $p.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_LEGS);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_FEET))
        {
                if (!remove_obj(ch, WEAR_FEET, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_FEET))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }
                if (!is_ok_to_wear(ch, wolf_ok, "feet"))
                {
                        send_to_char("You have no feet to wear them on.\n\r",
                                     ch);
                        return;
                }
                act("$n slips $s feet into $p.", ch, obj, NULL, TO_ROOM);
                act("You slip your feet into $p.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_FEET);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_HANDS))
        {
                if (!remove_obj(ch, WEAR_HANDS, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_HANDS))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }
                if (!is_ok_to_wear(ch, wolf_ok, "hands"))
                {
                        send_to_char("You have no hands to wear them on.\n\r",
                                     ch);
                        return;
                }
                act("$n pulls $p onto $s hands.", ch, obj, NULL, TO_ROOM);
                act("You pull $p onto your hands.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_HANDS);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_ARMS))
        {
                if (!remove_obj(ch, WEAR_ARMS, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_ARMS))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }
                if (!is_ok_to_wear(ch, wolf_ok, "arms"))
                {
                        send_to_char("You have no arms to wear them on.\n\r",
                                     ch);
                        return;
                }
                act("$n slides $s arms into $p.", ch, obj, NULL, TO_ROOM);
                act("You slide your arms into $p.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_ARMS);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_ABOUT))
        {
                if (!remove_obj(ch, WEAR_ABOUT, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_ABOUT))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }
                act("$n pulls $p about $s body.", ch, obj, NULL, TO_ROOM);
                act("You pull $p about your body.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_ABOUT);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_WAIST))
        {
                if (!remove_obj(ch, WEAR_WAIST, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_WAIST))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }
                act("$n ties $p around $s waist.", ch, obj, NULL, TO_ROOM);
                act("You tie $p around your waist.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_WAIST);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_SPECIAL))
        {
                if (!remove_obj(ch, WEAR_SPECIAL, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_SPECIAL))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }
                act("$n holds $p in $s hands.", ch, obj, NULL, TO_ROOM);
                act("You hold onto $p.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_SPECIAL);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_FLOAT))
        {
                if (!remove_obj(ch, WEAR_FLOAT, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_FLOAT))
                {
                        send_to_char
                                ("You are unable to wear it, perhaps you suck\n\r",
                                 ch);
                        return;
                }
                act("$n throws $p into the air and it starts flying about.",
                    ch, obj, NULL, TO_ROOM);
                act("You throw $p into the air and it starts circling around.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_FLOAT);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_MEDAL))
        {
                if (!remove_obj(ch, WEAR_MEDAL, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_MEDAL))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }
                act("$n puts $p on $s uniform.", ch, obj, NULL, TO_ROOM);
                act("You put $p on your uniform.", ch, obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_MEDAL);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_BODYART))
        {
                if (!remove_obj(ch, WEAR_BODYART, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_BODYART))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }
                act("$n screams in agony as $p bores into $s body.", ch, obj,
                    NULL, TO_ROOM);
                act("You scream in agony as $p bores into your body.", ch,
                    obj, NULL, TO_CHAR);
                equip_char(ch, obj, WEAR_BODYART);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_WRIST))
        {
                if (get_eq_char(ch, WEAR_WRIST_L) != NULL
                    && get_eq_char(ch, WEAR_WRIST_R) != NULL
                    && !remove_obj(ch, WEAR_WRIST_L, fReplace)
                    && !remove_obj(ch, WEAR_WRIST_R, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WEAR_WRIST))
                {
                        send_to_char("You are unable to wear it.\n\r", ch);
                        return;
                }

                if (get_eq_char(ch, WEAR_WRIST_L) == NULL
                    && is_ok_to_wear(ch, wolf_ok, "right_wrist"))
                {
                        act("$n slides $s left wrist into $p.", ch, obj, NULL,
                            TO_ROOM);
                        act("You slide your left wrist into $p.", ch, obj,
                            NULL, TO_CHAR);
                        equip_char(ch, obj, WEAR_WRIST_L);
                        return;
                }
                else if (get_eq_char(ch, WEAR_WRIST_R) == NULL
                         && is_ok_to_wear(ch, wolf_ok, "left_wrist"))
                {
                        act("$n slides $s left wrist into $p.", ch, obj, NULL,
                            TO_ROOM);
                        act("You slide your right wrist into $p.", ch, obj,
                            NULL, TO_CHAR);
                        equip_char(ch, obj, WEAR_WRIST_R);
                        return;
                }
                if (!is_ok_to_wear(ch, wolf_ok, "left_wrist")
                    && !is_ok_to_wear(ch, wolf_ok, "right_wrist"))
                        send_to_char
                                ("You cannot wear anything on your wrists.\n\r",
                                 ch);
                else
                        send_to_char
                                ("You cannot wear any more on your wrists.\n\r",
                                 ch);
                return;
        }

        if (CAN_WEAR(obj, ITEM_WEAR_SHIELD))
        {
             if (!remove_obj(ch, WEAR_SHIELD, fReplace)) return;
             act("$n straps $p onto $s shield arm.", ch, obj, NULL, TO_ROOM);
             act("You strap $p onto your shield arm.", ch, obj, NULL, TO_CHAR);
             equip_char(ch, obj, WEAR_SHIELD);
             return;
        }

        if (CAN_WEAR(obj, ITEM_WIELD))
        {
                if (!remove_obj(ch, WEAR_WIELD, fReplace))
                        return;
                if (!IS_NPC(ch) && !IS_FORM(ch, ITEM_WIELD))
                {
                        send_to_char("You are unable to wield it.\n\r", ch);
                        return;
                }
                /*
                 * This might need fixing 
                 */
                if (get_obj_weight(obj) >
                    str_app[(get_curr_str(ch) >
                             25) ? 25 : get_curr_str(ch)].wield);
                {
                        send_to_char("It is too heavy for you to wield.\n\r",
                                     ch);
                        return;
                }

                act("$n wields $p.", ch, obj, NULL, TO_ROOM);
                act("You wield $p.", ch, obj, NULL, TO_CHAR);
                if (!IS_NPC(ch)
                    && (obj->pIndexData->vnum == 30000
                        || IS_OBJ_STAT(obj, ITEM_LOYAL)))
                {
                        if (obj->questowner != NULL
                            && str_cmp(ch->pcdata->switchname,
                                       obj->questowner)
                            && strlen(obj->questowner) > 1)
                        {
                                act("$p leaps out of $n's hand.", ch, obj,
                                    NULL, TO_ROOM);
                                act("$p leaps out of your hand.", ch, obj,
                                    NULL, TO_CHAR);
                                obj_from_char(obj);
                                obj_to_room(obj, ch->in_room);
                                return;
                        }
                }
                equip_char(ch, obj, WEAR_WIELD);
                if (!IS_NPC(ch))
                        do_skill(ch, ch->name);
                return;
        }

        if (fReplace)
                send_to_char("You can't wear, wield or hold that.\n\r", ch);
        return;
}

void do_wear(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;

        one_argument(argument, arg);
        if (!IS_NPC(ch) && IS_POLYAFF(ch, POLY_ZULOFORM)
            && IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] < 9)
        {
                send_to_char
                        ("You are too mad to decide what you want to wear!\n\r",
                         ch);
                return;
        }
        if (!IS_NPC(ch) && IS_POLYAFF(ch, POLY_ZULOFORM)
            && IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] > 8)
        {
                send_to_char("#rYou force down yer beast#n.\n\r", ch);
        }



     /*   if (IS_AFFECTED(ch, AFF_POLYMORPH) && !IS_NPC(ch)
            && !IS_CLASS(ch, CLASS_ANGEL)
            && !IS_VAMPAFF(ch, VAM_DISGUISED)
            && !IS_CLASS(ch, CLASS_WEREWOLF) && !IS_CLASS(ch, CLASS_FAE)
            && !IS_CLASS(ch, CLASS_DEMON) && ch->power[DISC_VAMP_OBEA] < 9
	    && !IS_CLASS(ch, CLASS_GIANT)
            && !IS_CLASS(ch, CLASS_CYBORG) && !IS_CLASS(ch, CLASS_DROID))
	
        {
                send_to_char("You cannot wear anything in this form.\n\r",
                             ch);
                return;
        }
*/
        if (arg[0] == '\0')
        {
                send_to_char("Wear, wield, or hold what?\n\r", ch);
                return;
        }

        if (!str_cmp(arg, "all"))
        {
                OBJ_DATA *obj_next;

                for (obj = ch->carrying; obj != NULL; obj = obj_next)
                {
                        obj_next = obj->next_content;
                        if (obj->wear_loc == WEAR_NONE
                            && can_see_obj(ch, obj))
                                wear_obj(ch, obj, FALSE);
                }
                return;
        }
        else
        {
                if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
                {
                        send_to_char("You do not have that item.\n\r", ch);
                        return;
                }

                wear_obj(ch, obj, TRUE);
        }

        return;
}

void do_remove(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        int       sn;

        one_argument(argument, arg);
        if (arg[0] == '\0')
        {
                send_to_char("Remove what?\n\r", ch);
                return;
        }


        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE)
            && IS_POLYAFF(ch, POLY_ZULOFORM) && ch->generation > 1
            && ch->power[DISC_VAMP_OBEA] < 9)
        {
                stc("You are too mad to remove anything!\n\r", ch);
                return;
        }
        if (!IS_NPC(ch) && IS_POLYAFF(ch, POLY_ZULOFORM)
            && IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] > 8)
        {
                send_to_char("#rYou force down yer beast#n.\n\r", ch);
        }

        if (!str_cmp(arg, "all"))
        {
                OBJ_DATA *obj_next;

                for (obj = ch->carrying; obj != NULL; obj = obj_next)
                {
                        obj_next = obj->next_content;
                        if (obj->wear_loc != WEAR_NONE
                            && can_see_obj(ch, obj))
                        {
                                if (obj->item_type == ITEM_ARMOR)
                                        sn = obj->value[3];
                                else
                                        sn = obj->value[0] / 1000;
                                if (sn != 25)
                                        remove_obj(ch, obj->wear_loc, TRUE);
                        }
                }
                return;
        }
        if ((obj = get_obj_wear(ch, arg, TRUE)) == NULL)
        {
                send_to_char("You do not have that item.\n\r", ch);
                return;
        }
        remove_obj(ch, obj->wear_loc, TRUE);
        return;
}

void do_sacrifice(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        OBJ_DATA *next_obj;
        char      buf[MAX_INPUT_LENGTH];

        //extern OBJ_DATA *aquest_object;
        int       i = 0;
        int       ereward;

        one_argument(argument, arg);
        if (IS_NPC(ch))
                return;
        if (arg[0] == '\0')
        {
                send_to_char("Sacrifice what?\n\r", ch);
                return;
        }
        if (IS_AFFECTED(ch, AFF_CHARM))
        {
                stc("Not while charmed.\n\r", ch);
                return;
        }
        if (!str_cmp(arg, "all") || !str_prefix("all.", arg))
        {
                next_obj = ch->in_room->contents;
                for (obj = ch->in_room->contents; next_obj != NULL;
                     obj = next_obj)
                {
                        next_obj = obj->next_content;
                        if (arg[3] != '\0' && !is_name(&arg[4], obj->name))
                                continue;
                        if (++i > 35)
                                break;
                        if (!CAN_WEAR(obj, ITEM_TAKE)
                            || obj->item_type == ITEM_QUEST
                            || IS_SET(obj->quest, QUEST_ARTIFACT)
                            || (obj->questowner != NULL
                                && strlen(obj->questowner) > 1
                                && str_cmp(ch->name, obj->questowner)))
                        {
                                act("You are unable to drain any energy from $p.", ch, obj, 0, TO_CHAR);
                                continue;
                        }
                        else if (IS_SET(obj->quest, QUEST_MQUEST))
                        {
                                act("You are unable to drain any energy from $p.", ch, obj, 0, TO_CHAR);
                                continue;
                        }
                        else if (obj->chobj != NULL && !IS_NPC(obj->chobj)
                                 && obj->chobj->pcdata->obj_vnum != 0)
                        {
                                act("You are unable to drain any energy from $p.", ch, obj, 0, TO_CHAR);
                                continue;
                        }
                        if (obj->points > 0 && !IS_NPC(ch)
                            && obj->item_type != ITEM_PAGE)
                        {
                                xprintf(buf,
                                        "You receive a refund of %d bones from $p.",
                                        obj->points);
                                act(buf, ch, obj, NULL, TO_CHAR);
                                ch->bones += obj->points;
                        }
                        if (obj->pIndexData->vnum > 813
                            && obj->pIndexData->vnum < 826)
                        {
                                ch->pcdata->rune_count--;
                                send_to_char
                                        ("you get 50K bones from the titanium eq.\n\r",
                                         ch);
                                ch->bones += 50000;
                        }
                        ereward = number_range(1, 50000);
                        if (obj->item_type == ITEM_CORPSE_NPC)
                        {
   switch( number_range(1, 21) )
	{
      default: 
		xprintf(buf,
            "#0Paladine appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (ch->alignment > 900)
                	send_to_char("#0Paladine rewards those with a kind heart.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
            break;
      case  1:
		xprintf(buf,
            "#0Astinus appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (get_curr_int(ch) >= 100)
			{
                	send_to_char("Astinus #0rewards those of high Intellegence!!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}
			}
            break;
      case  2:
		xprintf(buf,
            "#0Chemosh appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) || IS_CLASS(ch, CLASS_GHOUL)
			|| IS_CLASS(ch, CLASS_ZOMBIE))
			{
                	send_to_char("#0The God of the UnDead dropps a pile of bones before you!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}
			}
            break;
      case  3:
		xprintf(buf,
            "#0Hiddukel appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_THIEF))
			{
                	send_to_char("#0The God of Thieves rewards you!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}
			}
            break;
      case  4:
		xprintf(buf,
            "#0Nuitari appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (ch->alignment <= -250 && (IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_LICH)))
			{
                	send_to_char("#0Nuitari #0rewards Black Robed Mages!\n\r", ch);
                	send_to_char("#0You Revieve 50,000,000 EXP.\n\r", ch);
			ch->exp += 50000000;
			}
			}			
            break;
      case  5:
		xprintf(buf,
            "#0Sargonnas appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_RACE(ch, RACE_MINOTAUR))
			{
                	send_to_char("#0Sargonnas Rewards his children the Minotaurs\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case  6:
		xprintf(buf,
            "#0Zeboim appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
			{
                	send_to_char("#0Zeboim Rewards knights of EVIL!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case  7:
		xprintf(buf,
            "#0Branchala appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_RACE(ch, RACE_ELF) || IS_RACE(ch, RACE_HALFELF)
			|| IS_RACE(ch, RACE_DROW) || IS_CLASS(ch, CLASS_DROW)
			|| IS_RACE(ch, RACE_KENDER) || IS_RACE(ch, RACE_WILDELF))
			{
                	send_to_char("#0The god reveared by kender and elves rewards you.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case  8:
		xprintf(buf,
            "#0Habbakuk appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_RACE(ch, RACE_MINOTAUR) || IS_CLASS(ch, CLASS_GIANT)
			|| IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DEMON)
			|| IS_CLASS(ch, CLASS_WEREWOLF) || IS_CLASS(ch, CLASS_VAMPIRE)
			|| IS_CLASS(ch, CLASS_GHOUL) || IS_CLASS(ch, CLASS_TANARRI)
			|| IS_RACE(ch, RACE_TROLL) || IS_RACE(ch, RACE_HALFTROLL)
			|| IS_RACE(ch, RACE_OGRE) || IS_RACE(ch, RACE_HALFOGRE)
			|| IS_RACE(ch, RACE_ORC) || IS_RACE(ch, RACE_HALFORC))
			{
                	send_to_char("#0The lord of Beasts smiles upon you.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case  9:
		xprintf(buf,
            "#0Kiri-Jolith appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_RACE(ch, RACE_MINOTAUR) || IS_CLASS(ch, CLASS_PALADIN))
			{
                	send_to_char("#0Kiri-Jolith looks kindly upon you.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case 10:
		xprintf(buf,
            "#0Majere appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_BLADE_MASTER) || IS_CLASS(ch, CLASS_PALADIN)
			|| IS_CLASS(ch, CLASS_MONK) || IS_CLASS(ch, CLASS_SAMURAI) 
			|| IS_CLASS(ch, CLASS_NINJA))
			{
                	send_to_char("#0Majere Favors those of Discipline.\n\r", ch);
                	send_to_char("#0You Revieve 50,000,000 EXP.\n\r", ch);
			ch->exp += 50000000;
			}}
            break;
      case 11:
		xprintf(buf,
            "#0Mishakal appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (ch->sex == SEX_FEMALE)
			{
                	send_to_char("#0The Godess of Motherhood looks kindly upon Women.\n\r", ch);
                	send_to_char("#0You get a Full Restore.\n\r", ch);
			ch->hit = ch->max_hit;
			ch->mana = ch->max_mana;
			ch->move = ch->max_move;
        		ch->loc_hp[0] = 0;
        		ch->loc_hp[1] = 0;
       		ch->loc_hp[2] = 0;
        		ch->loc_hp[3] = 0;
        		ch->loc_hp[4] = 0;
        		ch->loc_hp[5] = 0;
        		ch->loc_hp[6] = 0;
			}
			}
            break;
      case 12:
		xprintf(buf,
            "#0Solinari appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_MAGE) && (ch->alignment > 700))
			{
                	send_to_char("#0Solinari rewards Mages of the White Robes.\n\r", ch);
                	send_to_char("#0You get a Full Restore.\n\r", ch);
			ch->hit = ch->max_hit;
			ch->mana = ch->max_mana;
			ch->move = ch->max_move;
        		ch->loc_hp[0] = 0;
        		ch->loc_hp[1] = 0;
       		ch->loc_hp[2] = 0;
        		ch->loc_hp[3] = 0;
        		ch->loc_hp[4] = 0;
        		ch->loc_hp[5] = 0;
        		ch->loc_hp[6] = 0;
			}
            break;
      case 13:
		xprintf(buf,
            "#0Chislev appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (IS_RACE(ch, RACE_HOBBIT) || IS_CLASS(ch, CLASS_ELEMENTAL)
			|| IS_CLASS(ch, CLASS_FAE) || IS_CLASS(ch, CLASS_MONK))
			{
                	send_to_char("#0Chislev rewards those who respect Nature.\n\r", ch);
                	send_to_char("#0You get a Full Restore.\n\r", ch);
			ch->hit = ch->max_hit;
			ch->mana = ch->max_mana;
			ch->move = ch->max_move;
        		ch->loc_hp[0] = 0;
        		ch->loc_hp[1] = 0;
       		ch->loc_hp[2] = 0;
        		ch->loc_hp[3] = 0;
        		ch->loc_hp[4] = 0;
        		ch->loc_hp[5] = 0;
        		ch->loc_hp[6] = 0;
			}}
            break;
      case 14:
		xprintf(buf,
            "#0Gilean appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (ch->alignment == 0)
			{
                	send_to_char("#0Gilean rewards those who respect Neutrality.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case 15:
		xprintf(buf,
            "#0Lunitari appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_LICH))
		 	{
                	send_to_char("#0Lunitari Favors Mages of the Red Robes.\n\r", ch);
                	send_to_char("#0500 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 500;
			}}
            break;
      case 16:
		xprintf(buf,
            "#0Reorx appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_RACE(ch, RACE_GULLYDWARF) || IS_RACE(ch, RACE_HILLDWARF)
			|| IS_RACE(ch, RACE_DUERGARDWARF) || IS_RACE(ch, RACE_MTDWARF))
		 	{
                	send_to_char("#0Reorx patron God of Dwarves looks kindly upon you.\n\r", ch);
                	send_to_char("#0500 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 500;
			}}
            break;
      case 17:
		xprintf(buf,
            "#0Shinare appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_RACE(ch, RACE_GNOME))
			{
                	send_to_char("#0Shinare is a friend of the Gnome, a people of Trade.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case 18:
		xprintf(buf,
            "#0Sirrion #Ra#7ppreciate#Rs#0 your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_ELEMENTAL))
			{
                	send_to_char("#0Sirrion Favors children of the #RF#Yl#Ra#Ym#Re#n!!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case 19:
		xprintf(buf,
            "#0Zivilyn appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (get_curr_wis(ch) >= 100)
			{
                	send_to_char("Zivilyn #0rewards those of high Wisdom!!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case 20: 
		xprintf(buf,
            "#0Takhisis appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (ch->alignment <= -250)
			{
                	send_to_char("#RT#ra#wk#7h#Wi#rs#Ri#7s#n #0rewards the Ruthless!!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case 21: 
		xprintf(buf,
            "#0Paladine appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (ch->alignment > 900 || IS_CLASS(ch, CLASS_ANGEL) || IS_CLASS(ch, CLASS_PALADIN)
			|| IS_CLASS(ch, CLASS_PRIEST))
			{
                	send_to_char("#LP#7aladin#Le #0rewards those with a #Lkind #Rh#7ear#Rt#n.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;

	}
	break;


                        }
                        else
                        {
                                xprintf(buf,
                                        "$p disintegrates into a fine powder");
                                act(buf, ch, obj, NULL, TO_CHAR);
                        }
                        //if(!IS_SET(ch->act, PLR_BRIEF5) || ( ch->position != POS_FIGHTING ))
                        //act( buf, ch, obj, NULL, TO_CHAR );
                        act("$p disintegrates into a fine powder.", ch, obj,
                            NULL, TO_ROOM);
                        extract_obj(obj);
                }
                if (i == 0)
                        send_to_char("Nothing found.\n\r", ch);
                else
                        act("$n destroys most of the items in the room.", ch,
                            NULL, NULL, TO_ROOM);
                return;
        }
        obj = get_obj_list(ch, arg, ch->in_room->contents);
        if (obj == NULL)
        {
                send_to_char("You can't find it.\n\r", ch);
                return;
        }
        if (!CAN_WEAR(obj, ITEM_TAKE) || obj->item_type == ITEM_QUEST ||
            obj->item_type == ITEM_MONEY || obj->item_type == ITEM_TREASURE
            || IS_SET(obj->quest, QUEST_ARTIFACT)
            || (obj->questowner != NULL && strlen(obj->questowner) > 1
                && str_cmp(ch->name, obj->questowner)))
        {
                act("You are unable to drain any energy from $p.", ch, obj, 0,
                    TO_CHAR);
                return;
        }
        else if (obj->chobj != NULL && !IS_NPC(obj->chobj) &&
                 obj->chobj->pcdata->obj_vnum != 0)
        {
                act("You are unable to drain any energy from $p.", ch, obj, 0,
                    TO_CHAR);
                return;
        }
        ereward = number_range(1, 50000);
        if (obj->item_type == ITEM_CORPSE_NPC)
        {
   switch( number_range(1, 21) )
	{
      default: 
		xprintf(buf,
            "#0Paladine appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (ch->alignment > 900)
                	send_to_char("#0Paladine rewards those with a kind heart.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
            break;
      case  1:
		xprintf(buf,
            "#0Astinus appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (get_curr_int(ch) >= 100)
			{
                	send_to_char("Astinus #0rewards those of high Intellegence!!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}
			}
            break;
      case  2:
		xprintf(buf,
            "#0Chemosh appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) || IS_CLASS(ch, CLASS_GHOUL)
			|| IS_CLASS(ch, CLASS_ZOMBIE))
			{
                	send_to_char("#0The God of the UnDead dropps a pile of bones before you!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}
			}
            break;
      case  3:
		xprintf(buf,
            "#0Hiddukel appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_THIEF))
			{
                	send_to_char("#0The God of Thieves rewards you!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}
			}
            break;
      case  4:
		xprintf(buf,
            "#0Nuitari appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (ch->alignment <= -250 && (IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_LICH)))
			{
                	send_to_char("#0Nuitari #0rewards Black Robed Mages!\n\r", ch);
                	send_to_char("#0You Revieve 50,000,000 EXP.\n\r", ch);
			ch->exp += 50000000;
			}
			}			
            break;
      case  5:
		xprintf(buf,
            "#0Sargonnas appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_RACE(ch, RACE_MINOTAUR))
			{
                	send_to_char("#0Sargonnas Rewards his children the Minotaurs\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case  6:
		xprintf(buf,
            "#0Zeboim appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
			{
                	send_to_char("#0Zeboim Rewards knights of EVIL!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case  7:
		xprintf(buf,
            "#0Branchala appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_RACE(ch, RACE_ELF) || IS_RACE(ch, RACE_HALFELF)
			|| IS_RACE(ch, RACE_DROW) || IS_CLASS(ch, CLASS_DROW)
			|| IS_RACE(ch, RACE_KENDER) || IS_RACE(ch, RACE_WILDELF))
			{
                	send_to_char("#0The god reveared by kender and elves rewards you.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case  8:
		xprintf(buf,
            "#0Habbakuk appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_RACE(ch, RACE_MINOTAUR) || IS_CLASS(ch, CLASS_GIANT)
			|| IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DEMON)
			|| IS_CLASS(ch, CLASS_WEREWOLF) || IS_CLASS(ch, CLASS_VAMPIRE)
			|| IS_CLASS(ch, CLASS_GHOUL) || IS_CLASS(ch, CLASS_TANARRI)
			|| IS_RACE(ch, RACE_TROLL) || IS_RACE(ch, RACE_HALFTROLL)
			|| IS_RACE(ch, RACE_OGRE) || IS_RACE(ch, RACE_HALFOGRE)
			|| IS_RACE(ch, RACE_ORC) || IS_RACE(ch, RACE_HALFORC))
			{
                	send_to_char("#0The lord of Beasts smiles upon you.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case  9:
		xprintf(buf,
            "#0Kiri-Jolith appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_RACE(ch, RACE_MINOTAUR) || IS_CLASS(ch, CLASS_PALADIN))
			{
                	send_to_char("#0Kiri-Jolith looks kindly upon you.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case 10:
		xprintf(buf,
            "#0Majere appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_BLADE_MASTER) || IS_CLASS(ch, CLASS_PALADIN)
			|| IS_CLASS(ch, CLASS_MONK) || IS_CLASS(ch, CLASS_SAMURAI) 
			|| IS_CLASS(ch, CLASS_NINJA))
			{
                	send_to_char("#0Majere Favors those of Discipline.\n\r", ch);
                	send_to_char("#0You Revieve 50,000,000 EXP.\n\r", ch);
			ch->exp += 50000000;
			}}
            break;
      case 11:
		xprintf(buf,
            "#0Mishakal appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (ch->sex == SEX_FEMALE)
			{
                	send_to_char("#0The Godess of Motherhood looks kindly upon Women.\n\r", ch);
                	send_to_char("#0You get a Full Restore.\n\r", ch);
			ch->hit = ch->max_hit;
			ch->mana = ch->max_mana;
			ch->move = ch->max_move;
        		ch->loc_hp[0] = 0;
        		ch->loc_hp[1] = 0;
       		ch->loc_hp[2] = 0;
        		ch->loc_hp[3] = 0;
        		ch->loc_hp[4] = 0;
        		ch->loc_hp[5] = 0;
        		ch->loc_hp[6] = 0;
			}
			}
            break;
      case 12:
		xprintf(buf,
            "#0Solinari appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_MAGE) && (ch->alignment > 700))
			{
                	send_to_char("#0Solinari rewards Mages of the White Robes.\n\r", ch);
                	send_to_char("#0You get a Full Restore.\n\r", ch);
			ch->hit = ch->max_hit;
			ch->mana = ch->max_mana;
			ch->move = ch->max_move;
        		ch->loc_hp[0] = 0;
        		ch->loc_hp[1] = 0;
       		ch->loc_hp[2] = 0;
        		ch->loc_hp[3] = 0;
        		ch->loc_hp[4] = 0;
        		ch->loc_hp[5] = 0;
        		ch->loc_hp[6] = 0;
			}
            break;
      case 13:
		xprintf(buf,
            "#0Chislev appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (IS_RACE(ch, RACE_HOBBIT) || IS_CLASS(ch, CLASS_ELEMENTAL)
			|| IS_CLASS(ch, CLASS_FAE) || IS_CLASS(ch, CLASS_MONK))
			{
                	send_to_char("#0Chislev rewards those who respect Nature.\n\r", ch);
                	send_to_char("#0You get a Full Restore.\n\r", ch);
			ch->hit = ch->max_hit;
			ch->mana = ch->max_mana;
			ch->move = ch->max_move;
        		ch->loc_hp[0] = 0;
        		ch->loc_hp[1] = 0;
       		ch->loc_hp[2] = 0;
        		ch->loc_hp[3] = 0;
        		ch->loc_hp[4] = 0;
        		ch->loc_hp[5] = 0;
        		ch->loc_hp[6] = 0;
			}}
            break;
      case 14:
		xprintf(buf,
            "#0Gilean appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (ch->alignment == 0)
			{
                	send_to_char("#0Gilean rewards those who respect Neutrality.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case 15:
		xprintf(buf,
            "#0Lunitari appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_LICH))
		 	{
                	send_to_char("#0Lunitari Favors Mages of the Red Robes.\n\r", ch);
                	send_to_char("#0500 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 500;
			}}
            break;
      case 16:
		xprintf(buf,
            "#0Reorx appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_RACE(ch, RACE_GULLYDWARF) || IS_RACE(ch, RACE_HILLDWARF)
			|| IS_RACE(ch, RACE_DUERGARDWARF) || IS_RACE(ch, RACE_MTDWARF))
		 	{
                	send_to_char("#0Reorx patron God of Dwarves looks kindly upon you.\n\r", ch);
                	send_to_char("#0500 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 500;
			}}
            break;
      case 17:
		xprintf(buf,
            "#0Shinare appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_RACE(ch, RACE_GNOME))
			{
                	send_to_char("#0Shinare is a friend of the Gnome, a people of Trade.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case 18:
		xprintf(buf,
            "#0Sirrion #Ra#7ppreciate#Rs#0 your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (IS_CLASS(ch, CLASS_ELEMENTAL))
			{
                	send_to_char("#0Sirrion Favors children of the #RF#Yl#Ra#Ym#Re#n!!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case 19:
		xprintf(buf,
            "#0Zivilyn appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (get_curr_wis(ch) >= 100)
			{
                	send_to_char("Zivilyn #0rewards those of high Wisdom!!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case 20: 
		xprintf(buf,
            "#0Takhisis appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (ch->alignment <= -250)
			{
                	send_to_char("#RT#ra#wk#7h#Wi#rs#Ri#7s#n #0rewards the Ruthless!!\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;
      case 21: 
		xprintf(buf,
            "#0Paladine appreciates your sacrifice of #w$p#n");
            if ((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING))
            act(buf, ch, obj, NULL, TO_CHAR);
			if (number_range(1, 5) == 1)
			{
			if (ch->alignment > 900 || IS_CLASS(ch, CLASS_ANGEL) || IS_CLASS(ch, CLASS_PALADIN)
			|| IS_CLASS(ch, CLASS_PRIEST))
			{
                	send_to_char("#LP#7aladin#Le #0rewards those with a #Lkind #Rh#7ear#Rt#n.\n\r", ch);
                	send_to_char("#0250 Bones appear in your Money Purse.\n\r", ch);
			ch->bones += 250;
			}}
            break;

	}
//	break;
                }
        else
        {
                xprintf(buf, "$p disintegrates into a fine powder");
                act(buf, ch, obj, NULL, TO_CHAR);
        }
        //if((!IS_SET(ch->act, PLR_BRIEF5)) || (ch->position != POS_FIGHTING) )
        //act( buf, ch, obj, NULL, TO_CHAR );
        act("$n drains the energy from $p.", ch, obj, NULL, TO_ROOM);
        act("$p disintegrates into a fine powder.", ch, obj, NULL, TO_ROOM);
        if (obj->points > 0 && !IS_NPC(ch) && obj->item_type != ITEM_PAGE)
        {
                xprintf(buf, "You receive a refund of %d bones from $p.",
                        obj->points);
                act(buf, ch, obj, NULL, TO_CHAR);
                ch->bones += obj->points;
        }
        if (obj->pIndexData->vnum > 813 && obj->pIndexData->vnum < 826)
        {
                ch->pcdata->rune_count--;
                send_to_char("you get 50K bones from the rune eq.\n\r", ch);
                ch->bones += 50000;
        }
        extract_obj(obj);
        return;
}



void do_quaff(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        int       level;

        one_argument(argument, arg);
        if (IS_NPC(ch))
                return;
        if (arg[0] == '\0')
        {
                send_to_char("Quaff what?\n\r", ch);
                return;
        }

        if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
        {
                send_to_char("You do not have that potion.\n\r", ch);
                return;
        }

        if (obj->item_type != ITEM_POTION)
        {
                send_to_char("You can quaff only potions.\n\r", ch);
                return;
        }
        if (IS_NPC(ch))
                return;
        act("$n quaffs $p.", ch, obj, NULL, TO_ROOM);
        act("You quaff $p.", ch, obj, NULL, TO_CHAR);
        level = obj->value[0];
        if (level < 1)
                level = 1;
        if (level > MAX_SPELL)
                level = MAX_SPELL;
        obj_cast_spell(obj->value[1], level, ch, ch, NULL);
        obj_cast_spell(obj->value[2], level, ch, ch, NULL);
        obj_cast_spell(obj->value[3], level, ch, ch, NULL);
        extract_obj(obj);
        if (ch->position == POS_FIGHTING)
        {
                if (!IS_IMMORTAL(ch))
                        WAIT_STATE(ch, 6);
        }
        return;
}

void do_recite(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        OBJ_DATA *scroll;
        OBJ_DATA *obj;
        int       level;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        if (IS_NPC(ch))
                return;
        if ((scroll = get_obj_carry(ch, arg1, ch)) == NULL)
        {
                send_to_char("You do not have that scroll.\n\r", ch);
                return;
        }

        if (scroll->item_type != ITEM_SCROLL)
        {
                send_to_char("You can recite only scrolls.\n\r", ch);
                return;
        }

        obj = NULL;
        if (arg2[0] == '\0')
        {
                victim = ch;
        }
        else
        {
                if ((victim = get_char_room(ch, NULL, arg2)) == NULL
                    && (obj = get_obj_here(ch, NULL, arg2)) == NULL)
                {
                        send_to_char("You can't find it.\n\r", ch);
                        return;
                }
        }
        if (IS_NPC(ch))
                return;
        act("$n recites $p.", ch, scroll, NULL, TO_ROOM);
        act("You recite $p.", ch, scroll, NULL, TO_CHAR);
        level = scroll->value[0];
        if (level < 1)
                level = 1;
        if (level > MAX_SPELL)
                level = MAX_SPELL;
        obj_cast_spell(scroll->value[1], level, ch, victim, obj);
        obj_cast_spell(scroll->value[2], level, ch, victim, obj);
        obj_cast_spell(scroll->value[3], level, ch, victim, obj);
        extract_obj(scroll);
        if (ch->position == POS_FIGHTING)
        {
                if (!IS_IMMORTAL(ch))
                        WAIT_STATE(ch, 6);
        }
        return;
}

void do_brandish(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        OBJ_DATA *staff;
        char      arg[MAX_INPUT_LENGTH];
        int       sn, level;

        if (IS_NPC(ch))
                return;
        one_argument(argument, arg);
        if ((staff = get_obj_carry(ch, arg, ch)) == NULL)
        {
                send_to_char("You do not have that staff.\n\r", ch);
                return;
        }
        if (staff->item_type != ITEM_STAFF)
        {
                send_to_char("You can brandish only with a staff.\n\r", ch);
                return;
        }
        if ((sn = staff->value[3]) < 0 || sn >= MAX_SKILL
            || skill_table[sn].spell_fun == 0)
        {
                bug("Do_brandish: Bad Staff [obj vnum %d].",
                    staff->pIndexData->vnum);
                send_to_char("Something is wrong with this staff.\n\r", ch);
                return;
        }
        if (staff->value[2] > 0)
        {
                act("$n brandishes $p.", ch, staff, NULL, TO_ROOM);
                act("You brandish $p.", ch, staff, NULL, TO_CHAR);
                for (vch = ch->in_room->people; vch; vch = vch_next)
                {
                        vch_next = vch->next_in_room;
                        switch (skill_table[sn].target)
                        {
                        default:
                                bug("Do_brandish: bad spelltype for vnum %d.",
                                    staff->pIndexData->vnum);
                                return;
                        case TAR_CHAR_OFFENSIVE:
                        case TAR_CHAR_DEFENSIVE:
                                break;
                        }
                        level = staff->value[0];
                        if (level < 1)
                                level = 1;
                        if (level > MAX_SPELL)
                                level = MAX_SPELL;
                        obj_cast_spell(staff->value[3], level, ch, vch, NULL);
                }
        }
        if (--staff->value[2] <= 0)
        {
                act("$n's $p blazes bright and is gone.", ch, staff, NULL,
                    TO_ROOM);
                act("Your $p blazes bright and is gone.", ch, staff, NULL,
                    TO_CHAR);
                extract_obj(staff);
        }
        WAIT_STATE(ch, 18);
        return;
}

void do_zap(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        char      arg1[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        OBJ_DATA *wand;
        OBJ_DATA *obj;
        int       level;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg);
        if (IS_NPC(ch))
                return;
        if (arg[0] == '\0' || arg1[0] == '\0')
        {
                send_to_char("Zap whom with what?\n\r", ch);
                return;
        }

        if ((wand = get_obj_carry(ch, arg1, ch)) == NULL)
        {
                send_to_char("You do not have that wand.\n\r", ch);
                return;
        }
        if (wand->item_type != ITEM_WAND)
        {
                send_to_char("You can zap only with a wand.\n\r", ch);
                return;
        }

        obj = NULL;
        if ((victim = get_char_room(ch, NULL, arg)) == NULL
            && (obj = get_obj_here(ch, NULL, arg)) == NULL)
        {
                send_to_char("You can't find it.\n\r", ch);
                return;
        }

        WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
        if (wand->value[2] > 0)
        {
                if (victim != NULL)
                {
                        act("$n zaps $N with $p.", ch, wand, victim, TO_ROOM);
                        act("You zap $N with $p.", ch, wand, victim, TO_CHAR);
                }
                else
                {
                        act("$n zaps $P with $p.", ch, wand, obj, TO_ROOM);
                        act("You zap $P with $p.", ch, wand, obj, TO_CHAR);
                }

                level = wand->value[0];
                if (level < 1)
                        level = 1;
                if (level > MAX_SPELL)
                        level = MAX_SPELL;
                obj_cast_spell(wand->value[3], level, ch, victim, obj);
        }

        if (--wand->value[2] <= 0)
        {
                act("$n's $p explodes into fragments.", ch, wand, NULL,
                    TO_ROOM);
                act("Your $p explodes into fragments.", ch, wand, NULL,
                    TO_CHAR);
                extract_obj(wand);
        }

        return;
}

void do_steal(CHAR_DATA * ch, char *argument)
{
        char      buf[MAX_STRING_LENGTH];
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        OBJ_DATA *obj;
        int       percent;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        if (arg1[0] == '\0' || arg2[0] == '\0')
        {
                send_to_char("Steal what from whom?\n\r", ch);
                return;
        }

        if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }

        if (victim == ch)
        {
                send_to_char("That's pointless.\n\r", ch);
                return;
        }

        if (victim->shop_fun)
        {
                send_to_char("You must have a deathwish!.\n\r", ch);
                return;
        }

        if (IS_IMMORTAL(victim))
        {
                send_to_char("Steal from an immortal are you crazy!\n\r", ch);
                return;
        }

        if (!IS_IMMORTAL(ch))
                WAIT_STATE(ch, skill_table[gsn_steal].beats);
        percent = number_percent() + (IS_AWAKE(victim) ? 10 : -50);
        if ((ch->level + number_range(1, 20) < victim->level)
            || (!IS_NPC(ch) && !IS_NPC(victim) && ch->level < 3)
            || (!IS_NPC(ch) && !IS_NPC(victim) && victim->level < 3)
            || (victim->position == POS_FIGHTING)
            || (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_STEAL))
            || (!IS_NPC(victim) && IS_IMMORTAL(victim))
            || (!IS_NPC(ch) && percent > ch->pcdata->learned[gsn_steal]))
        {
                /*
                 * Failure.
                 */
                send_to_char("Oops.\n\r", ch);
                act("$n tried to steal from you.\n\r", ch, NULL, victim,
                    TO_VICT);
                act("$n tried to steal from $N.\n\r", ch, NULL, victim,
                    TO_NOTVICT);
                xprintf(buf, "%s is a bloody thief!", ch->name);
                do_yell(victim, buf);
                if (!IS_NPC(ch))
                {
                        if (IS_NPC(victim))
                        {
                                multi_hit(victim, ch, TYPE_UNDEFINED);
                        }
                        else
                        {
//              log_string( buf );
                                save_char_obj(ch);
                        }
                }

                return;
        }

        if (!str_cmp(arg1, "bone")
            || !str_cmp(arg1, "bones") || !str_cmp(arg1, "money"))
        {
                int       amount;

                amount = victim->bones * number_range(1, 10) / 100;
                if (amount <= 0)
                {
                        send_to_char("You couldn't get any bones.\n\r", ch);
                        return;
                }

                ch->bones += amount;
                victim->bones -= amount;
                if (victim->bones < 0)
                        victim->bones = 0;
                xprintf(buf, "Bingo!  You got %d bones.\n\r", amount);
                send_to_char(buf, ch);
                do_autosave(ch, "");
                do_autosave(victim, "");
                return;
        }

        if ((obj = get_obj_carry(victim, arg1, ch)) == NULL)
        {
                send_to_char("You can't find it.\n\r", ch);
                return;
        }

        if (!can_drop_obj(ch, obj)
            || IS_SET(obj->extra_flags, ITEM_LOYAL)
            || IS_SET(obj->extra_flags, ITEM_INVENTORY))
        {
                send_to_char("You can't pry it away.\n\r", ch);
                return;
        }

        if (ch->carry_number + 1 > can_carry_n(ch))
        {
                send_to_char("You have your hands full.\n\r", ch);
                return;
        }

        if (ch->carry_weight + get_obj_weight(obj) > can_carry_w(ch))
        {
                send_to_char("You can't carry that much weight.\n\r", ch);
                return;
        }

        obj_from_char(obj);
        obj_to_char(obj, ch);
        send_to_char("You got it!\n\r", ch);
        do_autosave(ch, "");
        do_autosave(victim, "");
        return;
}


void do_activate(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        OBJ_DATA *obj2;
        CHAR_DATA *victim;
        CHAR_DATA *victim_next;
        CHAR_DATA *mount;
        CHAR_DATA *mob;
        ROOM_INDEX_DATA *pRoomIndex;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        if (IS_NPC(ch))
                return;
        if (IS_SET(ch->in_room->room_flags, ROOM_NO_OTRANS))
        {
                send_to_char
                        ("A strange force is preventing the item from working.\n\r",
                         ch);
                return;
        }
        if (arg1[0] == '\0')
        {
                send_to_char("Which item do you wish to activate?\n\r", ch);
                return;
        }
        if ((obj = get_obj_wear(ch, arg1, TRUE)) == NULL)
        {
                if ((obj = get_obj_here(ch, NULL, arg1)) == NULL)
                {
                        send_to_char("You can't find that item.\n\r", ch);
                        return;
                }
                /*
                 * You should only be able to use nontake items on floor 
                 */
                if (CAN_WEAR(obj, ITEM_TAKE))
                {
                        send_to_char("But you are not wearing it!\n\r", ch);
                        return;
                }
        }
/*        if (obj == NULL || !IS_SET(obj->spectype, SITEM_ACTIVATE))
        {
                send_to_char("This item cannot be activated.\n\r", ch);
                return;
        }
*/
        if (IS_SET(obj->spectype, SITEM_TARGET) && arg2[0] == '\0')
        {
                send_to_char("Who do you wish to activate it on?\n\r", ch);
                return;
        }
        if (IS_SET(obj->spectype, SITEM_TARGET))
        {
                if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
                {
                        send_to_char("Nobody here by that name.\n\r", ch);
                        return;
                }
        }
        else
                victim = ch;
        if (obj->chpoweruse != NULL && obj->chpoweruse != '\0'
            && str_cmp(obj->chpoweruse, "(null)"))
                kavitem(obj->chpoweruse, ch, obj, NULL, TO_CHAR);
        if (obj->victpoweruse != NULL && obj->victpoweruse != '\0'
            && str_cmp(obj->victpoweruse, "(null)"))
                kavitem(obj->victpoweruse, ch, obj, NULL, TO_ROOM);
        if (IS_SET(obj->spectype, SITEM_SPELL))
        {
                int       castlevel = obj->level;

                if (castlevel < 1)
                        castlevel = 1;
                else if (castlevel > 60)
                        castlevel = 60;
                obj_cast_spell(obj->specpower, castlevel, ch, victim, NULL);
                if (!IS_IMMORTAL(ch))
                        WAIT_STATE(ch, 6);
                if (IS_SET(obj->spectype, SITEM_DELAY1))
                        WAIT_STATE(ch, 6);
                if (IS_SET(obj->spectype, SITEM_DELAY2))
                        WAIT_STATE(ch, 12);
                return;
        }
        else if (IS_SET(obj->spectype, SITEM_TRANSPORTER))
        {
                if (obj->chpoweron != NULL && obj->chpoweron != '\0'
                    && str_cmp(obj->chpoweron, "(null)"))
                        kavitem(obj->chpoweron, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweron != NULL && obj->victpoweron != '\0'
                    && str_cmp(obj->victpoweron, "(null)"))
                        kavitem(obj->victpoweron, ch, obj, NULL, TO_ROOM);
                pRoomIndex = get_room_index(obj->specpower);
                obj->specpower = ch->in_room->vnum;
                if (pRoomIndex == NULL)
                        return;
                char_from_room(ch);
                char_to_room(ch, pRoomIndex);
                do_look(ch, "auto");
                if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
                    && str_cmp(obj->chpoweroff, "(null)"))
                        kavitem(obj->chpoweroff, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
                    && str_cmp(obj->victpoweroff, "(null)"))
                        kavitem(obj->victpoweroff, ch, obj, NULL, TO_ROOM);
                if (!IS_SET(obj->quest, QUEST_ARTIFACT)
                    && (IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT)
                        || ch->in_room->vnum == 30000
                        || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
                    && CAN_WEAR(obj, ITEM_TAKE))
                {
                        send_to_char
                                ("A powerful force hurls you from the room.\n\r",
                                 ch);
                        act("$n is hurled from the room by a powerful force.",
                            ch, NULL, NULL, TO_ROOM);
                        ch->position = POS_STUNNED;
                        char_from_room(ch);
                        char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
                        act("$n appears in the room, and falls to the ground stunned.", ch, NULL, NULL, TO_ROOM);
                }
                if ((mount = ch->mount) == NULL)
                        return;
                char_from_room(mount);
                char_to_room(mount, ch->in_room);
                do_look(mount, "auto");
                return;
        }
        else if (IS_SET(obj->spectype, SITEM_TELEPORTER))
        {
                if (obj->chpoweron != NULL && obj->chpoweron != '\0'
                    && str_cmp(obj->chpoweron, "(null)"))
                        kavitem(obj->chpoweron, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweron != NULL && obj->victpoweron != '\0'
                    && str_cmp(obj->victpoweron, "(null)"))
                        kavitem(obj->victpoweron, ch, obj, NULL, TO_ROOM);
                pRoomIndex = get_room_index(obj->specpower);
                if (pRoomIndex == NULL)
                        return;
                char_from_room(ch);
                char_to_room(ch, pRoomIndex);
                do_look(ch, "auto");
                if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
                    && str_cmp(obj->chpoweroff, "(null)"))
                        kavitem(obj->chpoweroff, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
                    && str_cmp(obj->victpoweroff, "(null)"))
                        kavitem(obj->victpoweroff, ch, obj, NULL, TO_ROOM);
                if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
                    (IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT)
                     || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
                    && CAN_WEAR(obj, ITEM_TAKE))
                {
                        send_to_char
                                ("A powerful force hurls you from the room.\n\r",
                                 ch);
                        act("$n is hurled from the room by a powerful force.",
                            ch, NULL, NULL, TO_ROOM);
                        ch->position = POS_STUNNED;
                        char_from_room(ch);
                        char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
                        act("$n appears in the room, and falls to the ground stunned.", ch, NULL, NULL, TO_ROOM);
                }
                if ((mount = ch->mount) == NULL)
                        return;
                char_from_room(mount);
                char_to_room(mount, ch->in_room);
                do_look(mount, "auto");
                return;
        }
        else if (IS_SET(obj->spectype, SITEM_OBJECT))
        {
                if (get_obj_index(obj->specpower) == NULL)
                        return;
                obj2 = create_object(get_obj_index(obj->specpower),
                                     ch->level);
                if (CAN_WEAR(obj2, ITEM_TAKE))
                        obj_to_char(obj2, ch);
                else
                        obj_to_room(obj2, ch->in_room);
        }
        else if (IS_SET(obj->spectype, SITEM_MOBILE))
        {
                if (get_mob_index(obj->specpower) == NULL)
                        return;
                mob = create_mobile(get_mob_index(obj->specpower));
                char_to_room(mob, ch->in_room);
        }
        else if (IS_SET(obj->spectype, SITEM_ACTION))
        {
                interpret(ch, obj->victpoweron);
                if (obj->victpoweroff != NULL
                    && str_cmp(obj->victpoweroff, "(null)")
                    && obj->victpoweroff != '\0')
                {
                        for (victim = char_list; victim != NULL;
                             victim = victim_next)
                        {
                                victim_next = victim->next;
                                if (victim->in_room == NULL)
                                        continue;
                                if (victim == ch)
                                        continue;
                                if (victim->in_room == ch->in_room)
                                {
                                        interpret(victim, obj->victpoweroff);
                                        continue;
                                }
                        }
                }
        }
        return;
}

void do_press(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        OBJ_DATA *obj2;
        CHAR_DATA *victim;
        CHAR_DATA *victim_next;
        CHAR_DATA *mount;
        CHAR_DATA *mob;
        ROOM_INDEX_DATA *pRoomIndex;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        if (IS_NPC(ch))
                return;
        if (IS_SET(ch->in_room->room_flags, ROOM_NO_OTRANS))
        {
                send_to_char
                        ("A strange force is preventing the item from working.\n\r",
                         ch);
                return;
        }


        if (arg1[0] == '\0')
        {
                send_to_char("Which item do you wish to press?\n\r", ch);
                return;
        }
        if ((obj = get_obj_wear(ch, arg1, TRUE)) == NULL)
        {
                if ((obj = get_obj_here(ch, NULL, arg1)) == NULL)
                {
                        send_to_char("You can't find that item.\n\r", ch);
                        return;
                }
                /*
                 * You should only be able to use nontake items on floor 
                 */
                if (CAN_WEAR(obj, ITEM_TAKE))
                {
                        send_to_char("But you are not wearing it!\n\r", ch);
                        return;
                }
        }
/*        if (obj == NULL || !IS_SET(obj->spectype, SITEM_PRESS))
        {
                send_to_char("There is nothing on this item to press.\n\r",
                             ch);
                return;
        }
*/
        if (IS_SET(obj->spectype, SITEM_TARGET) && arg2[0] == '\0')
        {
                send_to_char("Who do you wish to use it on?\n\r", ch);
                return;
        }
        if (IS_SET(obj->spectype, SITEM_TARGET))
        {
                if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
                {
                        send_to_char("Nobody here by that name.\n\r", ch);
                        return;
                }
        }
        else
                victim = ch;
        if (obj->chpoweruse != NULL && obj->chpoweruse != '\0'
            && str_cmp(obj->chpoweruse, "(null)"))
                kavitem(obj->chpoweruse, ch, obj, NULL, TO_CHAR);
        if (obj->victpoweruse != NULL && obj->victpoweruse != '\0'
            && str_cmp(obj->victpoweruse, "(null)"))
                kavitem(obj->victpoweruse, ch, obj, NULL, TO_ROOM);
        if (IS_SET(obj->spectype, SITEM_SPELL))
        {
                int       castlevel = obj->level;

                if (castlevel < 1)
                        castlevel = 1;
                else if (castlevel > 60)
                        castlevel = 60;
                obj_cast_spell(obj->specpower, castlevel, ch, victim, NULL);
                if (!IS_IMMORTAL(ch))
                        WAIT_STATE(ch, 6);
                if (IS_SET(obj->spectype, SITEM_DELAY1))
                {
                        if (!IS_IMMORTAL(ch))
                                WAIT_STATE(ch, 6);
                }
                if (IS_SET(obj->spectype, SITEM_DELAY2))
                {
                        if (!IS_IMMORTAL(ch))
                                WAIT_STATE(ch, 12);
                }
                return;
        }
        else if (IS_SET(obj->spectype, SITEM_TRANSPORTER))
        {
                if (obj->chpoweron != NULL && obj->chpoweron != '\0'
                    && str_cmp(obj->chpoweron, "(null)"))
                        kavitem(obj->chpoweron, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweron != NULL && obj->victpoweron != '\0'
                    && str_cmp(obj->victpoweron, "(null)"))
                        kavitem(obj->victpoweron, ch, obj, NULL, TO_ROOM);
                pRoomIndex = get_room_index(obj->specpower);
                obj->specpower = ch->in_room->vnum;
                if (pRoomIndex == NULL)
                        return;
                char_from_room(ch);
                char_to_room(ch, pRoomIndex);
                do_look(ch, "auto");
                if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
                    && str_cmp(obj->chpoweroff, "(null)"))
                        kavitem(obj->chpoweroff, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
                    && str_cmp(obj->victpoweroff, "(null)"))
                        kavitem(obj->victpoweroff, ch, obj, NULL, TO_ROOM);
                if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
                    (IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT)
                     || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
                    && CAN_WEAR(obj, ITEM_TAKE))
                {
                        send_to_char
                                ("A powerful force hurls you from the room.\n\r",
                                 ch);
                        act("$n is hurled from the room by a powerful force.",
                            ch, NULL, NULL, TO_ROOM);
                        ch->position = POS_STUNNED;
                        char_from_room(ch);
                        char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
                        act("$n appears in the room, and falls to the ground stunned.", ch, NULL, NULL, TO_ROOM);
                }
                if ((mount = ch->mount) == NULL)
                        return;
                char_from_room(mount);
                char_to_room(mount, ch->in_room);
                do_look(mount, "auto");
                return;
        }
        else if (IS_SET(obj->spectype, SITEM_TELEPORTER))
        {
                if (obj->chpoweron != NULL && obj->chpoweron != '\0'
                    && str_cmp(obj->chpoweron, "(null)"))
                        kavitem(obj->chpoweron, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweron != NULL && obj->victpoweron != '\0'
                    && str_cmp(obj->victpoweron, "(null)"))
                        kavitem(obj->victpoweron, ch, obj, NULL, TO_ROOM);
                pRoomIndex = get_room_index(obj->specpower);
                if (pRoomIndex == NULL)
                        return;
                char_from_room(ch);
                char_to_room(ch, pRoomIndex);
                do_look(ch, "auto");
                if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
                    && str_cmp(obj->chpoweroff, "(null)"))
                        kavitem(obj->chpoweroff, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
                    && str_cmp(obj->victpoweroff, "(null)"))
                        kavitem(obj->victpoweroff, ch, obj, NULL, TO_ROOM);
                if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
                    (IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT)
                     || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
                    && CAN_WEAR(obj, ITEM_TAKE))
                {
                        send_to_char
                                ("A powerful force hurls you from the room.\n\r",
                                 ch);
                        act("$n is hurled from the room by a powerful force.",
                            ch, NULL, NULL, TO_ROOM);
                        ch->position = POS_STUNNED;
                        char_from_room(ch);
                        char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
                        act("$n appears in the room, and falls to the ground stunned.", ch, NULL, NULL, TO_ROOM);
                }
                if ((mount = ch->mount) == NULL)
                        return;
                char_from_room(mount);
                char_to_room(mount, ch->in_room);
                do_look(mount, "auto");
                return;
        }
        else if (IS_SET(obj->spectype, SITEM_OBJECT))
        {
                if (get_obj_index(obj->specpower) == NULL)
                        return;
                obj2 = create_object(get_obj_index(obj->specpower),
                                     ch->level);
                if (CAN_WEAR(obj2, ITEM_TAKE))
                        obj_to_char(obj2, ch);
                else
                        obj_to_room(obj2, ch->in_room);
        }
        else if (IS_SET(obj->spectype, SITEM_MOBILE))
        {
                if (get_mob_index(obj->specpower) == NULL)
                        return;
                mob = create_mobile(get_mob_index(obj->specpower));
                char_to_room(mob, ch->in_room);
        }
        else if (IS_SET(obj->spectype, SITEM_ACTION))
        {
                interpret(ch, obj->victpoweron);
                if (obj->victpoweroff != NULL
                    && str_cmp(obj->victpoweroff, "(null)")
                    && obj->victpoweroff != '\0')
                {
                        for (victim = char_list; victim != NULL;
                             victim = victim_next)
                        {
                                victim_next = victim->next;
                                if (victim->in_room == NULL)
                                        continue;
                                if (victim == ch)
                                        continue;
                                if (victim->in_room == ch->in_room)
                                {
                                        interpret(victim, obj->victpoweroff);
                                        continue;
                                }
                        }
                }
        }
        return;
}

void do_twist(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        OBJ_DATA *obj2;
        CHAR_DATA *victim;
        CHAR_DATA *victim_next;
        CHAR_DATA *mount;
        CHAR_DATA *mob;
        ROOM_INDEX_DATA *pRoomIndex;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        if (IS_NPC(ch))
                return;
        if (IS_SET(ch->in_room->room_flags, ROOM_NO_OTRANS))
        {
                send_to_char
                        ("A strange force is preventing the item from working.\n\r",
                         ch);
                return;
        }


        if (arg1[0] == '\0')
        {
                send_to_char("Which item do you wish to twist?\n\r", ch);
                return;
        }
        if ((obj = get_obj_wear(ch, arg1, TRUE)) == NULL)
        {
                if ((obj = get_obj_here(ch, NULL, arg1)) == NULL)
                {
                        send_to_char("You can't find that item.\n\r", ch);
                        return;
                }
                /*
                 * You should only be able to use nontake items on floor 
                 */
                if (CAN_WEAR(obj, ITEM_TAKE))
                {
                        send_to_char("But you are not wearing it!\n\r", ch);
                        return;
                }
        }
/*        if (obj == NULL || !IS_SET(obj->spectype, SITEM_TWIST))
        {
                send_to_char("This item cannot be twisted.\n\r", ch);
                return;
        }
*/
        if (IS_SET(obj->spectype, SITEM_TARGET) && arg2[0] == '\0')
        {
                send_to_char("Who do you wish to use it on?\n\r", ch);
                return;
        }
        if (IS_SET(obj->spectype, SITEM_TARGET))
        {
                if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
                {
                        send_to_char("Nobody here by that name.\n\r", ch);
                        return;
                }
        }
        else
                victim = ch;
        if (obj->chpoweruse != NULL && obj->chpoweruse != '\0'
            && str_cmp(obj->chpoweruse, "(null)"))
                kavitem(obj->chpoweruse, ch, obj, NULL, TO_CHAR);
        if (obj->victpoweruse != NULL && obj->victpoweruse != '\0'
            && str_cmp(obj->victpoweruse, "(null)"))
                kavitem(obj->victpoweruse, ch, obj, NULL, TO_ROOM);
        if (IS_SET(obj->spectype, SITEM_SPELL))
        {
                int       castlevel = obj->level;

                if (castlevel < 1)
                        castlevel = 1;
                else if (castlevel > 60)
                        castlevel = 60;
                obj_cast_spell(obj->specpower, castlevel, ch, victim, NULL);
                if (!IS_IMMORTAL(ch))
                        WAIT_STATE(ch, 6);
                if (IS_SET(obj->spectype, SITEM_DELAY1))
                {
                        if (!IS_IMMORTAL(ch))
                                WAIT_STATE(ch, 6);
                }
                if (IS_SET(obj->spectype, SITEM_DELAY2))
                {
                        if (!IS_IMMORTAL(ch))
                                WAIT_STATE(ch, 12);
                }
                return;
        }
        else if (IS_SET(obj->spectype, SITEM_TRANSPORTER))
        {
                if (obj->chpoweron != NULL && obj->chpoweron != '\0'
                    && str_cmp(obj->chpoweron, "(null)"))
                        kavitem(obj->chpoweron, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweron != NULL && obj->victpoweron != '\0'
                    && str_cmp(obj->victpoweron, "(null)"))
                        kavitem(obj->victpoweron, ch, obj, NULL, TO_ROOM);
                pRoomIndex = get_room_index(obj->specpower);
                obj->specpower = ch->in_room->vnum;
                if (pRoomIndex == NULL)
                        return;
                char_from_room(ch);
                char_to_room(ch, pRoomIndex);
                do_look(ch, "auto");
                if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
                    && str_cmp(obj->chpoweroff, "(null)"))
                        kavitem(obj->chpoweroff, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
                    && str_cmp(obj->victpoweroff, "(null)"))
                        kavitem(obj->victpoweroff, ch, obj, NULL, TO_ROOM);
                if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
                    (IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT)
                     || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
                    && CAN_WEAR(obj, ITEM_TAKE))
                {
                        send_to_char
                                ("A powerful force hurls you from the room.\n\r",
                                 ch);
                        act("$n is hurled from the room by a powerful force.",
                            ch, NULL, NULL, TO_ROOM);
                        ch->position = POS_STUNNED;
                        char_from_room(ch);
                        char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
                        act("$n appears in the room, and falls to the ground stunned.", ch, NULL, NULL, TO_ROOM);
                }
                if ((mount = ch->mount) == NULL)
                        return;
                char_from_room(mount);
                char_to_room(mount, ch->in_room);
                do_look(mount, "auto");
                return;
        }
        else if (IS_SET(obj->spectype, SITEM_TELEPORTER))
        {
                if (obj->chpoweron != NULL && obj->chpoweron != '\0'
                    && str_cmp(obj->chpoweron, "(null)"))
                        kavitem(obj->chpoweron, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweron != NULL && obj->victpoweron != '\0'
                    && str_cmp(obj->victpoweron, "(null)"))
                        kavitem(obj->victpoweron, ch, obj, NULL, TO_ROOM);
                pRoomIndex = get_room_index(obj->specpower);
                if (pRoomIndex == NULL)
                        return;
                char_from_room(ch);
                char_to_room(ch, pRoomIndex);
                do_look(ch, "auto");
                if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
                    && str_cmp(obj->chpoweroff, "(null)"))
                        kavitem(obj->chpoweroff, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
                    && str_cmp(obj->victpoweroff, "(null)"))
                        kavitem(obj->victpoweroff, ch, obj, NULL, TO_ROOM);
                if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
                    (IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT)
                     || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
                    && CAN_WEAR(obj, ITEM_TAKE))
                {
                        send_to_char
                                ("A powerful force hurls you from the room.\n\r",
                                 ch);
                        act("$n is hurled from the room by a powerful force.",
                            ch, NULL, NULL, TO_ROOM);
                        ch->position = POS_STUNNED;
                        char_from_room(ch);
                        char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
                        act("$n appears in the room, and falls to the ground stunned.", ch, NULL, NULL, TO_ROOM);
                }
                if ((mount = ch->mount) == NULL)
                        return;
                char_from_room(mount);
                char_to_room(mount, ch->in_room);
                do_look(mount, "auto");
                return;
        }
        else if (IS_SET(obj->spectype, SITEM_OBJECT))
        {
                if (get_obj_index(obj->specpower) == NULL)
                        return;
                obj2 = create_object(get_obj_index(obj->specpower),
                                     ch->level);
                if (CAN_WEAR(obj2, ITEM_TAKE))
                        obj_to_char(obj2, ch);
                else
                        obj_to_room(obj2, ch->in_room);
        }
        else if (IS_SET(obj->spectype, SITEM_MOBILE))
        {
                if (get_mob_index(obj->specpower) == NULL)
                        return;
                mob = create_mobile(get_mob_index(obj->specpower));
                char_to_room(mob, ch->in_room);
        }
        else if (IS_SET(obj->spectype, SITEM_ACTION))
        {
                interpret(ch, obj->victpoweron);
                if (obj->victpoweroff != NULL
                    && str_cmp(obj->victpoweroff, "(null)")
                    && obj->victpoweroff != '\0')
                {
                        for (victim = char_list; victim != NULL;
                             victim = victim_next)
                        {
                                victim_next = victim->next;
                                if (victim->in_room == NULL)
                                        continue;
                                if (victim == ch)
                                        continue;
                                if (victim->in_room == ch->in_room)
                                {
                                        interpret(victim, obj->victpoweroff);
                                        continue;
                                }
                        }
                }
        }
        return;
}

void do_pull(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        OBJ_DATA *obj2;
        CHAR_DATA *victim;
        CHAR_DATA *victim_next;
        CHAR_DATA *mount;
        CHAR_DATA *mob;
        ROOM_INDEX_DATA *pRoomIndex;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        if (IS_NPC(ch))
                return;
        if (IS_SET(ch->in_room->room_flags, ROOM_NO_OTRANS))
        {
                send_to_char
                        ("A strange force is preventing the item from working.\n\r",
                         ch);
                return;
        }


        if (arg1[0] == '\0')
        {
                send_to_char("What do you wish to pull?\n\r", ch);
                return;
        }
        if ((obj = get_obj_wear(ch, arg1, TRUE)) == NULL)
        {
                if ((obj = get_obj_here(ch, NULL, arg1)) == NULL)
                {
                        send_to_char("You can't find that item.\n\r", ch);
                        return;
                }
                /*
                 * You should only be able to use nontake items on floor 
                 */
                if (CAN_WEAR(obj, ITEM_TAKE))
                {
                        send_to_char("But you are not wearing it!\n\r", ch);
                        return;
                }
        }
/*        if (obj == NULL || !IS_SET(obj->spectype, SITEM_PULL))
        {
                send_to_char("This item cannot be pulled.\n\r", ch);
                return;
        }*/
        if (IS_SET(obj->spectype, SITEM_TARGET) && arg2[0] == '\0')
        {
                send_to_char("Who do you wish to use it on?\n\r", ch);
                return;
        }
        if (IS_SET(obj->spectype, SITEM_TARGET))
        {
                if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
                {
                        send_to_char("Nobody here by that name.\n\r", ch);
                        return;
                }
        }
        else
                victim = ch;
        if (obj->chpoweruse != NULL && obj->chpoweruse != '\0'
            && str_cmp(obj->chpoweruse, "(null)"))
                kavitem(obj->chpoweruse, ch, obj, NULL, TO_CHAR);
        if (obj->victpoweruse != NULL && obj->victpoweruse != '\0'
            && str_cmp(obj->victpoweruse, "(null)"))
                kavitem(obj->victpoweruse, ch, obj, NULL, TO_ROOM);
        if (IS_SET(obj->spectype, SITEM_SPELL))
        {
                int       castlevel = obj->level;

                if (castlevel < 1)
                        castlevel = 1;
                else if (castlevel > 60)
                        castlevel = 60;
                obj_cast_spell(obj->specpower, castlevel, ch, victim, NULL);
                if (!IS_IMMORTAL(ch))
                        WAIT_STATE(ch, 6);
                if (IS_SET(obj->spectype, SITEM_DELAY1))
                {
                        if (!IS_IMMORTAL(ch))
                                WAIT_STATE(ch, 6);
                }
                if (IS_SET(obj->spectype, SITEM_DELAY2))
                {
                        if (!IS_IMMORTAL(ch))
                                WAIT_STATE(ch, 12);
                }
                return;
        }
        else if (IS_SET(obj->spectype, SITEM_TRANSPORTER))
        {
                if (obj->chpoweron != NULL && obj->chpoweron != '\0'
                    && str_cmp(obj->chpoweron, "(null)"))
                        kavitem(obj->chpoweron, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweron != NULL && obj->victpoweron != '\0'
                    && str_cmp(obj->victpoweron, "(null)"))
                        kavitem(obj->victpoweron, ch, obj, NULL, TO_ROOM);
                pRoomIndex = get_room_index(obj->specpower);
                obj->specpower = ch->in_room->vnum;
                if (pRoomIndex == NULL)
                        return;
                char_from_room(ch);
                char_to_room(ch, pRoomIndex);
                do_look(ch, "auto");
                if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
                    && str_cmp(obj->chpoweroff, "(null)"))
                        kavitem(obj->chpoweroff, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
                    && str_cmp(obj->victpoweroff, "(null)"))
                        kavitem(obj->victpoweroff, ch, obj, NULL, TO_ROOM);
                if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
                    (IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT)
                     || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
                    && CAN_WEAR(obj, ITEM_TAKE))
                {
                        send_to_char
                                ("A powerful force hurls you from the room.\n\r",
                                 ch);
                        act("$n is hurled from the room by a powerful force.",
                            ch, NULL, NULL, TO_ROOM);
                        ch->position = POS_STUNNED;
                        char_from_room(ch);
                        char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
                        act("$n appears in the room, and falls to the ground stunned.", ch, NULL, NULL, TO_ROOM);
                }
                if ((mount = ch->mount) == NULL)
                        return;
                char_from_room(mount);
                char_to_room(mount, ch->in_room);
                do_look(mount, "auto");
                return;
        }
        else if (IS_SET(obj->spectype, SITEM_TELEPORTER))
        {
                if (obj->chpoweron != NULL && obj->chpoweron != '\0'
                    && str_cmp(obj->chpoweron, "(null)"))
                        kavitem(obj->chpoweron, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweron != NULL && obj->victpoweron != '\0'
                    && str_cmp(obj->victpoweron, "(null)"))
                        kavitem(obj->victpoweron, ch, obj, NULL, TO_ROOM);
                pRoomIndex = get_room_index(obj->specpower);
                if (pRoomIndex == NULL)
                        return;
                char_from_room(ch);
                char_to_room(ch, pRoomIndex);
                do_look(ch, "auto");
                if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
                    && str_cmp(obj->chpoweroff, "(null)"))
                        kavitem(obj->chpoweroff, ch, obj, NULL, TO_CHAR);
                if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
                    && str_cmp(obj->victpoweroff, "(null)"))
                        kavitem(obj->victpoweroff, ch, obj, NULL, TO_ROOM);
                if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
                    (IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT)
                     || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
                    && CAN_WEAR(obj, ITEM_TAKE))
                {
                        send_to_char
                                ("A powerful force hurls you from the room.\n\r",
                                 ch);
                        act("$n is hurled from the room by a powerful force.",
                            ch, NULL, NULL, TO_ROOM);
                        ch->position = POS_STUNNED;
                        char_from_room(ch);
                        char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
                        act("$n appears in the room, and falls to the ground stunned.", ch, NULL, NULL, TO_ROOM);
                }
                if ((mount = ch->mount) == NULL)
                        return;
                char_from_room(mount);
                char_to_room(mount, ch->in_room);
                do_look(mount, "auto");
                return;
        }
        else if (IS_SET(obj->spectype, SITEM_OBJECT))
        {
                if (get_obj_index(obj->specpower) == NULL)
                        return;
                obj2 = create_object(get_obj_index(obj->specpower),
                                     ch->level);
                if (CAN_WEAR(obj2, ITEM_TAKE))
                        obj_to_char(obj2, ch);
                else
                        obj_to_room(obj2, ch->in_room);
        }
        else if (IS_SET(obj->spectype, SITEM_MOBILE))
        {
                if (get_mob_index(obj->specpower) == NULL)
                        return;
                mob = create_mobile(get_mob_index(obj->specpower));
                char_to_room(mob, ch->in_room);
        }
        else if (IS_SET(obj->spectype, SITEM_ACTION))
        {
                interpret(ch, obj->victpoweron);
                if (obj->victpoweroff != NULL
                    && str_cmp(obj->victpoweroff, "(null)")
                    && obj->victpoweroff != '\0')
                {
                        for (victim = char_list; victim != NULL;
                             victim = victim_next)
                        {
                                victim_next = victim->next;
                                if (victim->in_room == NULL)
                                        continue;
                                if (victim == ch)
                                        continue;
                                if (victim->in_room == ch->in_room)
                                {
                                        interpret(victim, obj->victpoweroff);
                                        continue;
                                }
                        }
                }
        }
        return;
}

bool is_ok_to_wear(CHAR_DATA * ch, bool wolf_ok, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        int       count;

        argument = one_argument(argument, arg);
        if (!str_cmp(arg, "head"))
        {
                if (IS_HEAD(ch, LOST_HEAD))
                        return FALSE;
        }
        else if (!str_cmp(arg, "face"))
        {
                if (IS_HEAD(ch, LOST_HEAD))
                        return FALSE;
        }
        else if (!str_cmp(arg, "left_hand"))
        {
                if (!IS_NPC(ch) && IS_SET(ch->special, SPC_WOLFMAN)
                    && !wolf_ok && !IS_CLASS(ch, CLASS_VAMPIRE))
                        return FALSE;
                if (IS_ARM_L(ch, LOST_ARM))
                        return FALSE;
                else if (IS_ARM_L(ch, BROKEN_ARM))
                        return FALSE;
                else if (IS_ARM_L(ch, LOST_HAND))
                        return FALSE;
                else if (IS_ARM_L(ch, BROKEN_THUMB))
                        return FALSE;
                else if (IS_ARM_L(ch, LOST_THUMB))
                        return FALSE;
                count = 0;
                if (IS_ARM_L(ch, LOST_FINGER_I)
                    || IS_ARM_L(ch, BROKEN_FINGER_I))
                        count += 1;
                if (IS_ARM_L(ch, LOST_FINGER_M)
                    || IS_ARM_L(ch, BROKEN_FINGER_M))
                        count += 1;
                if (IS_ARM_L(ch, LOST_FINGER_R)
                    || IS_ARM_L(ch, BROKEN_FINGER_R))
                        count += 1;
                if (IS_ARM_L(ch, LOST_FINGER_L)
                    || IS_ARM_L(ch, BROKEN_FINGER_L))
                        count += 1;
                if (count > 2)
                        return FALSE;
        }
        else if (!str_cmp(arg, "right_hand"))
        {
                if (!IS_NPC(ch) && IS_SET(ch->special, SPC_WOLFMAN)
                    && !wolf_ok && !IS_CLASS(ch, CLASS_VAMPIRE))
                        return FALSE;
                if (IS_ARM_R(ch, LOST_ARM))
                        return FALSE;
                else if (IS_ARM_R(ch, BROKEN_ARM))
                        return FALSE;
                else if (IS_ARM_R(ch, LOST_HAND))
                        return FALSE;
                else if (IS_ARM_R(ch, BROKEN_THUMB))
                        return FALSE;
                else if (IS_ARM_R(ch, LOST_THUMB))
                        return FALSE;
                count = 0;
                if (IS_ARM_R(ch, LOST_FINGER_I)
                    || IS_ARM_R(ch, BROKEN_FINGER_I))
                        count += 1;
                if (IS_ARM_R(ch, LOST_FINGER_M)
                    || IS_ARM_R(ch, BROKEN_FINGER_M))
                        count += 1;
                if (IS_ARM_R(ch, LOST_FINGER_R)
                    || IS_ARM_R(ch, BROKEN_FINGER_R))
                        count += 1;
                if (IS_ARM_R(ch, LOST_FINGER_L)
                    || IS_ARM_R(ch, BROKEN_FINGER_L))
                        count += 1;
                if (count > 2)
                        return FALSE;
        }
        else if (!str_cmp(arg, "third_hand"))
        {
                if (!IS_SET(ch->newbits, THIRD_HAND))
                        return FALSE;
                if (!IS_NPC(ch) && IS_SET(ch->special, SPC_WOLFMAN)
                    && !wolf_ok && !IS_CLASS(ch, CLASS_VAMPIRE))
                        return FALSE;
        }
        else if (!str_cmp(arg, "fourth_hand"))
        {
                if (!IS_SET(ch->newbits, FOURTH_HAND))
                        return FALSE;
                if (!IS_NPC(ch) && IS_SET(ch->special, SPC_WOLFMAN)
                    && !wolf_ok && !IS_CLASS(ch, CLASS_VAMPIRE))
                        return FALSE;
        }
        else if (!str_cmp(arg, "left_wrist"))
        {
                if (IS_ARM_L(ch, LOST_ARM))
                        return FALSE;
                else if (IS_ARM_L(ch, LOST_HAND))
                        return FALSE;
        }
        else if (!str_cmp(arg, "right_wrist"))
        {
                if (IS_ARM_R(ch, LOST_ARM))
                        return FALSE;
                else if (IS_ARM_R(ch, LOST_HAND))
                        return FALSE;
        }
        else if (!str_cmp(arg, "left_finger"))
        {
                if (IS_ARM_L(ch, LOST_ARM))
                        return FALSE;
                else if (IS_ARM_L(ch, LOST_HAND))
                        return FALSE;
                else if (IS_ARM_L(ch, LOST_FINGER_R))
                        return FALSE;
        }
        else if (!str_cmp(arg, "right_finger"))
        {
                if (IS_ARM_R(ch, LOST_ARM))
                        return FALSE;
                else if (IS_ARM_R(ch, LOST_HAND))
                        return FALSE;
                else if (IS_ARM_R(ch, LOST_FINGER_R))
                        return FALSE;
        }
        else if (!str_cmp(arg, "arms"))
        {
                if (IS_ARM_L(ch, LOST_ARM) && IS_ARM_R(ch, LOST_ARM))
                        return FALSE;
        }
        else if (!str_cmp(arg, "hands"))
        {
                if (IS_ARM_L(ch, LOST_ARM) && IS_ARM_R(ch, LOST_ARM))
                        return FALSE;
                if (IS_ARM_L(ch, LOST_HAND) || IS_ARM_R(ch, LOST_HAND))
                        return FALSE;
        }
        else if (!str_cmp(arg, "legs"))
        {
                if (IS_LEG_L(ch, LOST_LEG) && IS_LEG_R(ch, LOST_LEG))
                        return FALSE;
        }
        else if (!str_cmp(arg, "feet"))
        {
                if (IS_LEG_L(ch, LOST_LEG) && IS_LEG_R(ch, LOST_LEG))
                        return FALSE;
                if (IS_LEG_L(ch, LOST_FOOT) || IS_LEG_R(ch, LOST_FOOT))
                        return FALSE;
        }
        return TRUE;
}

void do_sheath(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];

        one_argument(argument, arg);
        if (arg[0] == '\0')
                send_to_char("Which hand, left or right?\n\r", ch);
        else if (!str_cmp(arg, "all") || !str_cmp(arg, "both"))
        {
                sheath(ch, TRUE);
                sheath(ch, FALSE);
        }
        else if (!str_cmp(arg, "l") || !str_cmp(arg, "left"))
                sheath(ch, FALSE);
        else if (!str_cmp(arg, "r") || !str_cmp(arg, "right"))
                sheath(ch, TRUE);
        else
                send_to_char("Which hand, left or right?\n\r", ch);
        return;
}

void do_draw(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];

        one_argument(argument, arg);
        if (arg[0] == '\0')
                send_to_char("Which hand, left or right?\n\r", ch);
        else if (!str_cmp(arg, "all") || !str_cmp(arg, "both"))
        {
                if (IS_ARM_L(ch, LOST_ARM) || IS_ARM_R(ch, LOST_ARM))
                {
                        send_to_char("You don't have both of your arms.\n\r",
                                     ch);
                        return;
                }
                draw(ch, TRUE);
                draw(ch, FALSE);
        }
        else if (!str_cmp(arg, "l") || !str_cmp(arg, "left"))
        {
                if (IS_ARM_L(ch, LOST_ARM))
                {
                        send_to_char("You have lost your left arm.\n\r", ch);
                        return;
                }
                draw(ch, FALSE);
        }
        else if (!str_cmp(arg, "r") || !str_cmp(arg, "right"))
        {
                if (IS_ARM_R(ch, LOST_ARM))
                {
                        send_to_char("You have lost your right arm.\n\r", ch);
                        return;
                }
                draw(ch, TRUE);
        }
        else
                send_to_char("Which hand, left or right?\n\r", ch);
        return;
}

void sheath(CHAR_DATA * ch, bool right)
{
        OBJ_DATA *obj;
        OBJ_DATA *obj2;
        int       scabbard;
        int       sn;

        if (right)
        {
                scabbard = WEAR_SCABBARD_R;
                if ((obj = get_eq_char(ch, WEAR_WIELD)) == NULL)
                {
                        send_to_char
                                ("You are not holding anything in your right hand.\n\r",
                                 ch);
                        return;
                }
                else if ((obj2 = get_eq_char(ch, scabbard)) != NULL)
                {
                        act("You already have $p in your right scabbard.", ch,
                            obj2, NULL, TO_CHAR);
                        return;
                }
                act("You slide $p into your right scabbard.", ch, obj, NULL,
                    TO_CHAR);
                act("$n slides $p into $s right scabbard.", ch, obj, NULL,
                    TO_ROOM);
        }
        else
        {
                scabbard = WEAR_SCABBARD_L;
                if ((obj = get_eq_char(ch, WEAR_HOLD)) == NULL)
                {
                        send_to_char
                                ("You are not holding anything in your left hand.\n\r",
                                 ch);
                        return;
                }
                else if ((obj2 = get_eq_char(ch, scabbard)) != NULL)
                {
                        act("You already have $p in your left scabbard.", ch,
                            obj2, NULL, TO_CHAR);
                        return;
                }
                act("You slide $p into your left scabbard.", ch, obj, NULL,
                    TO_CHAR);
                act("$n slides $p into $s left scabbard.", ch, obj, NULL,
                    TO_ROOM);
        }
        if (obj->item_type == ITEM_ARMOR)
                sn = obj->value[3];
        else
                sn = obj->value[0] / 1000;
        if (sn == 25)
        {
                stc("You can't sheath a rage weapon.\n\r", ch);
                return;
        }
        if (obj->item_type != ITEM_WEAPON)
        {
                act("$p is not a weapon.", ch, obj, NULL, TO_CHAR);
                return;
        }
        unequip_char(ch, obj);
        obj->wear_loc = scabbard;
        return;
}

void draw(CHAR_DATA * ch, bool right)
{
        OBJ_DATA *obj;
        OBJ_DATA *obj2;
        int       scabbard;
        int       worn;

        if (right)
        {
                scabbard = WEAR_SCABBARD_R;
                worn = WEAR_WIELD;
                if ((obj = get_eq_char(ch, scabbard)) == NULL)
                {
                        send_to_char("Your right scabbard is empty.\n\r", ch);
                        return;
                }
                else if ((obj2 = get_eq_char(ch, WEAR_WIELD)) != NULL)
                {
                        act("You already have $p in your right hand.", ch,
                            obj2, NULL, TO_CHAR);
                        return;
                }
                act("You draw $p from your right scabbard.", ch, obj, NULL,
                    TO_CHAR);
                act("$n draws $p from $s right scabbard.", ch, obj, NULL,
                    TO_ROOM);
        }
        else
        {
                scabbard = WEAR_SCABBARD_L;
                worn = WEAR_HOLD;
                if ((obj = get_eq_char(ch, scabbard)) == NULL)
                {
                        send_to_char("Your left scabbard is empty.\n\r", ch);
                        return;
                }
                else if ((obj2 = get_eq_char(ch, WEAR_HOLD)) != NULL)
                {
                        act("You already have $p in your left hand.", ch,
                            obj2, NULL, TO_CHAR);
                        return;
                }
                act("You draw $p from your left scabbard.", ch, obj, NULL,
                    TO_CHAR);
                act("$n draws $p from $s left scabbard.", ch, obj, NULL,
                    TO_ROOM);
        }
        obj->wear_loc = -1;
        equip_char(ch, obj, worn);
        return;
}


void do_thirdeye(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        DESCRIPTOR_DATA *d;
        char      buf[MAX_STRING_LENGTH];

        if (IS_NPC(ch))
                return;
        if (!IS_ITEMAFF(ch, ITEMA_AFFEYE))
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }
        send_to_char
                ("#CYou experience the world through your third eye.#n\n\r",
                 ch);
        for (d = descriptor_list; d != NULL; d = d->next)
        {
                if (d->connected != CON_PLAYING)
                        continue;
                if ((victim = d->character) == NULL)
                        continue;
                if (IS_NPC(victim) || victim->in_room == NULL)
                        continue;
                if (victim == ch)
                        continue;
                if (!can_see(ch, victim))
                        continue;
                if (victim->level > 6)
                        continue;
                xprintf(buf, "#n%-14s - %s\n\r", victim->name,
                        victim->in_room->name);
                send_to_char(buf, ch);
        }
}

void do_connect(CHAR_DATA * ch, char *argument)
{
        OBJ_DATA *first_ob;
        OBJ_DATA *second_ob;
        OBJ_DATA *new_ob;
        char      arg1[MAX_STRING_LENGTH], arg2[MAX_STRING_LENGTH];
        char      buf[MAX_STRING_LENGTH];

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        if (arg1[0] == '\0' || arg2[0] == '\0')
        {
                send_to_char("Syntax: Connect <firstobj> <secondobj>.\n\r",
                             ch);
                return;
        }

        if ((first_ob = get_obj_carry(ch, arg1, ch)) == NULL)
        {
                xprintf(buf, "You must be holding both parts!!\n\r");
                send_to_char(buf, ch);
                return;
        }



        if ((second_ob = get_obj_carry(ch, arg2, ch)) == NULL)
        {
                xprintf(buf, "You must be holding both parts!!\n\r");
                send_to_char(buf, ch);
                return;
        }

        if (first_ob->item_type != ITEM_PIECE
            || second_ob->item_type != ITEM_PIECE)
        {
                send_to_char("Both items must be pieces of another item!\n\r",
                             ch);
                return;
        }

        /*
         * check for bad pieces 
         */
        if (first_ob->value[2] == 0)
        {
                xprintf(buf,
                        "Piece Object %d does not have a valid v2 value\n\r",
                        first_ob->pIndexData->vnum);
                bug(buf, 0);
                xprintf(buf, "Your obj %s has a bad value, tell an imm.\n\r",
                        first_ob->name);
                send_to_char(buf, ch);
                return;
        }

        if (second_ob->value[2] == 0)
        {
                xprintf(buf,
                        "Piece Object %d does not have a valid v2 value\n\r",
                        second_ob->pIndexData->vnum);
                bug(buf, 0);
                xprintf(buf, "Your obj %s has a bad value, tell an imm.\n\r",
                        second_ob->name);
                send_to_char(buf, ch);
                return;
        }

        /*
         * check to see if the pieces connect 
         */

        if ((first_ob->value[0] == second_ob->pIndexData->vnum)
            || (first_ob->value[1] == second_ob->pIndexData->vnum)
            || (second_ob->value[0] == first_ob->pIndexData->vnum)
            || (second_ob->value[1] == first_ob->pIndexData->vnum))
                /*
                 * good connection  
                 */
        {
                new_ob = create_object(get_obj_index(first_ob->value[2]),
                                       ch->level);
                extract_obj(first_ob);
                extract_obj(second_ob);
                obj_to_char(new_ob, ch);
                act("$n jiggles some pieces together...\r\n ...suddenly they snap in place, creating $p!", ch, new_ob, NULL, TO_ROOM);
                act("You jiggle the pieces together...\r\n ...suddenly they snap into place, creating $p!", ch, new_ob, NULL, TO_CHAR);
        }
        else
        {
                act("$n jiggles some pieces together, but can't seem to make them connect.", ch, NULL, NULL, TO_ROOM);
                act("You try to fit them together every which way, but they just don't want to fit together.", ch, NULL, NULL, TO_CHAR);
                return;
        }

        return;
}

/*
void do_claim(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;

        one_argument(argument, arg);
        if (IS_NPC(ch))
        {
                send_to_char("Not while switched.\n\r", ch);
                return;
        }

        if (ch->exp < 500)
        {
                send_to_char
                        ("It costs 500 exp to claim ownership of an item.\n\r",
                         ch);
                return;
        }

        if (arg[0] == '\0')
        {
                send_to_char
                        ("What object do you wish to claim ownership of?\n\r",
                         ch);
                return;
        }

        if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
        {
                send_to_char("You are not carrying that item.\n\r", ch);
                return;
        }

        if (IS_OBJ_STAT(obj, ITEM_NOCLAIM))
        {
                send_to_char("You cannot claim this item.\n\r", ch);
                return;
        }

        if (obj->item_type == ITEM_QUEST || obj->item_type == ITEM_AMMO ||
            obj->item_type == ITEM_EGG || obj->item_type == ITEM_VOODOO ||
            obj->item_type == ITEM_MONEY || obj->item_type == ITEM_TREASURE
            || obj->item_type == ITEM_PAGE
            || IS_SET(obj->quest, QUEST_ARTIFACT))
        {
                send_to_char("You cannot claim that item.\n\r", ch);
                return;
        }

        else if (obj->chobj != NULL && !IS_NPC(obj->chobj)
                 && obj->chobj->pcdata->obj_vnum != 0)
        {
                send_to_char("You cannot claim that item.\n\r", ch);
                return;
        }

        if (obj->ownerid != 0)
        {
                if (obj->ownerid == ch->pcdata->playerid)
                        send_to_char("But you already own it.\n\r", ch);
                else
                        send_to_char("Someone else already own it.\n\r", ch);
                return;
        }

        if (obj->questowner != NULL && strlen(obj->questowner) > 1)
        {
                if (!str_cmp(ch->pcdata->switchname, obj->questowner))
                {
                        if (obj->ownerid != 0)
                                send_to_char("But you already own it!\n\r",
                                             ch);
                        else
                        {
                                send_to_char
                                        ("You #Creclaim#n the object.\n\r",
                                         ch);
                                obj->ownerid = ch->pcdata->playerid;
                        }
                }
                else
                        send_to_char
                                ("Someone else has already claimed ownership to it.\n\r",
                                 ch);
                return;
        }

        ch->exp -= 500;
        if (obj->questowner != NULL)
                free_string(obj->questowner);
        obj->questowner = str_dup(ch->pcdata->switchname);
        obj->ownerid = ch->pcdata->playerid;
        act("You are now the owner of $p.", ch, obj, NULL, TO_CHAR);
        act("$n is now the owner of $p.", ch, obj, NULL, TO_ROOM);
        return;
}
*/

void do_claim( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    if ( ch->exp < 500 ) {send_to_char("It costs 500 exp to claim ownership of an item.\n\r",ch); return;}

    if ( arg[0] == '\0' )
    {
	send_to_char( "What object do you wish to claim ownership of?\n\r", ch );
	return;
    }
        if ((obj = get_obj_carry(ch, arg, ch)) == NULL)

//    if ( ( obj = get_obj_carry(ch, arg) ) == NULL )
	{send_to_char("You are not carrying that item.\n\r",ch); return;}

    if (obj->item_type == ITEM_QUEST || obj->item_type == ITEM_AMMO ||
        obj->item_type == ITEM_EGG   || obj->item_type == ITEM_VOODOO ||
        obj->item_type == ITEM_MONEY || obj->item_type == ITEM_TREASURE ||
        obj->item_type == ITEM_PAGE)
    {
	send_to_char( "You cannot claim that item.\n\r", ch );
	return;
    }
/*    
if (!IS_SET(obj->spectype, SITEM_ADAMANTITE) &&
        !IS_SET(obj->spectype, SITEM_COPPER) &&
        !IS_SET(obj->spectype, SITEM_IRON) &&
        !IS_SET(obj->spectype, SITEM_STEEL))
{
send_to_char("You can only claim items forged from a rare metal.\n\r",ch);
return;
}
*/
 else if (obj->chobj != NULL && !IS_NPC(obj->chobj) &&
	obj->chobj->pcdata->obj_vnum != 0)
    {
	send_to_char( "You cannot claim that item.\n\r", ch );
	return;
    }

    if ( obj->questowner != NULL && strlen(obj->questowner) > 1 )
    {
	if (!str_cmp(ch->pcdata->switchname,obj->questowner))
	    send_to_char("But you already own it!\n\r",ch);
	else
	    send_to_char("Someone else has already claimed ownership to it.\n\r",ch);
	return;
    }

    ch->exp -= 500;
    if (obj->questowner != NULL) free_string(obj->questowner);
    obj->questowner = str_dup(ch->pcdata->switchname);
    obj->ownerid = ch->pcdata->playerid;
    act("You are now the owner of $p.",ch,obj,NULL,TO_CHAR);
    act("$n is now the owner of $p.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_gift(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        OBJ_DATA *obj;
        CHAR_DATA *victim;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        if (IS_NPC(ch))
        {
                send_to_char("Not while switched.\n\r", ch);
                return;
        }

        if (ch->exp < 500)
        {
                send_to_char
                        ("It costs 500 exp to make a gift of an item.\n\r",
                         ch);
                return;
        }

        if (arg1[0] == '\0' || arg2[0] == '\0')
        {
                send_to_char("Make a gift of which object to whom?\n\r", ch);
                return;
        }

        if ((obj = get_obj_carry(ch, arg1, ch)) == NULL)
        {
                send_to_char("You are not carrying that item.\n\r", ch);
                return;
        }
        if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
        {
                send_to_char("Nobody here by that name.\n\r", ch);
                return;
        }

        if (IS_NPC(victim))
        {
                send_to_char("Not on NPC's.\n\r", ch);
                return;
        }
        if (obj->ownerid == 0)
        {
                send_to_char("That item has not yet been claimed.\n\r", ch);
                return;
        }
        if (obj->ownerid != ch->pcdata->playerid && ch->level < 7)
        {
                send_to_char("But you don't own it!\n\r", ch);
                return;
        }
        if (IS_SET(obj->quest, QUEST_RELIC) && ch->level < 7)
        {
                send_to_char("You can't gift relics.\n\r", ch);
                return;
        }
        ch->exp -= 500;
        if (obj->questowner != NULL)
                free_string(obj->questowner);
        obj->questowner = str_dup(victim->pcdata->switchname);
        obj->ownerid = victim->pcdata->playerid;
        act("You grant ownership of $p to $N.", ch, obj, victim, TO_CHAR);
        act("$n grants ownership of $p to $N.", ch, obj, victim, TO_NOTVICT);
        act("$n grants ownership of $p to you.", ch, obj, victim, TO_VICT);
        return;
}
