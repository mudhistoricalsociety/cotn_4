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
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "olc.h"


/*
	type 1			shop
	type 2			shop
	type 3			shop
	type 4			shop
	type 5			shop
	type 6			shop
	type 7			shop
	type 8			shop
	type 9			shop
	type 10			shop
	type 11			bank	
*/
bool can_buy args((CHAR_DATA * ch));

void do_buy(CHAR_DATA * ch, char *argument)
{
        ROOM_INDEX_DATA *room;
        OBJ_INDEX_DATA *pObjIndex;
        OBJ_DATA *obj;
        int       vnum = 0;
        char      arg[MAX_INPUT_LENGTH];
        char      buf[MAX_STRING_LENGTH];
        int       itemcost = 0;

        argument = one_argument(argument, arg);
        if (IS_NPC(ch))
                return;
        room = ch->in_room;
        if (room == NULL)
                return;
        if (room->shop_type == 0 || room->shop_type >= 10)
        {
                stc("You aren't at a shop.\n\r", ch);
                return;
        }

        if (arg[0] == '\0')
        {
                stc("Buy what? Type 'list' to see what items are for sale here.", ch);
                return;
        }


        if (atoi(arg) == 0)
        {
                stc("Invalid item number.\n\r", ch);
                return;
        }

        if (can_buy(ch) == FALSE)
        {
                stc("You are not permitted to buy that item!\n\r", ch);
                return;
        }



        if (room->items_sold[atoi(arg)] != 0)
        {
                vnum = room->items_sold[atoi(arg)];
                //pObjIndex = get_obj_index(value);

                if (vnum != 0 && (pObjIndex = get_obj_index(vnum)) != NULL)
                {
                        obj = create_object(pObjIndex, 50);
                        itemcost = obj->cost;

                        if (ch->bones < itemcost)
                        {
                                sprintf(buf,
                                        "This item costs %d bones, while you only have %d.\n\r",
                                        itemcost, ch->bones);
                                stc(buf, ch);
                                extract_obj(obj);
                                return;
                        }

                        obj_to_char(obj, ch);
                        if (obj->questowner != NULL)
                                free_string(obj->questowner);
                        obj->questowner = str_dup(ch->pcdata->switchname);

                        sprintf(buf, "#7You buy %s#7 for #y%d#7 bones.\n\r",
                                obj->short_descr, itemcost);
                        stc(buf, ch);
                        act("#C$n #7purchases $p#7 from the storekeep.#n\n\r",
                            ch, obj, NULL, TO_ROOM);
                        ch->bones -= itemcost;
                        return;
                }
                else
                {
                        stc("Error check vnum shit in shop.c\n\r", ch);
                        return;
                }
        }
        else
        {
                stc("That item number is not for sale. Type 'list' to see what *is* for sale.\n\r", ch);
                return;
        }
        return;
}

void do_sell(CHAR_DATA * ch, char *argument)
{
        ROOM_INDEX_DATA *room;
        OBJ_DATA *obj;
        OBJ_DATA *obj_next;
        bool      found;
        char      arg[MAX_INPUT_LENGTH];
        char      buf[MAX_STRING_LENGTH];
        int       value = 0, total = 0, counter = 0;

        argument = one_argument(argument, arg);
        if (IS_NPC(ch))
                return;
        room = ch->in_room;
        if (room == NULL)
                return;
        if (room->shop_type == 0 || room->shop_type >= 10)
        {
                stc("You aren't at a shop.\n\r", ch);
                return;
        }

        if (arg[0] == '\0')
        {
                stc("Sell what?", ch);
                return;
        }
        if (str_cmp(arg, "all") && str_prefix("all.", arg))
        {
                if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
                {
                        stc("You don't seem to have that item!\n\r", ch);
                        return;
                }

                if (obj->questowner != NULL && strlen(obj->questowner) > 1
                    && str_cmp(ch->pcdata->switchname, obj->questowner))
                {
                        stc("You can't sell that! It belongs to someone else!\n\r", ch);
                        return;
                }
                if (IS_SET(obj->extra_flags2, ITEM_NO_SELL))
                {
                        stc("You are not allowed to sell that item!\n\r", ch);
                        return;
                }
                if (obj->cost > 0)
                {

                        if (obj->item_type == ITEM_TREASURE)
                        {
                                value = obj->cost;
                                //value += number_range(obj->cost * 0.1, obj->cost * 0.3);
                        }

                        else
                        {
                                value = obj->cost / 2;

                                if (number_range(1, 2) == 2)
                                        value += number_range(value * 0.1,
                                                              value * 0.3);
                                else
                                        value -= number_range(value * 0.1,
                                                              value * 0.3);

                        }

                        sprintf(buf, "You sell $p for %d bones pieces!",
                                value);
                        act(buf, ch, obj, NULL, TO_CHAR);
                        act("$n sells $p to the shopkeeper!", ch, obj, NULL,
                            TO_ROOM);
                        extract_obj(obj);
                        ch->bones += value;
                        return;
                }
                else
                {
                        stc("That object is worthless!\n\r", ch);
                        return;
                }
                return;
        }
        else
        {
                found = FALSE;
                for (obj = ch->carrying; obj != NULL; obj = obj_next)
                {
                        obj_next = obj->next_content;
                        if (arg[3] != '\0' && !is_name(&arg[4], obj->name))
                                continue;
                        if (!can_see_obj(ch, obj)
                            || obj->wear_loc != WEAR_NONE)
                                continue;
                        if (obj->questowner != NULL
                            && strlen(obj->questowner) > 1
                            && str_cmp(ch->pcdata->switchname,
                                       obj->questowner))
                                continue;
                        if (IS_SET(obj->extra_flags2, ITEM_NO_SELL))
                                continue;
                        if (obj->cost < 1)
                                continue;
                        if (obj->item_type == ITEM_TREASURE)
                                value = obj->cost;
                        else
                        {
                                value = obj->cost / 2;
                                if (number_range(1, 2) == 2)
                                        value += number_range(value * 0.1,
                                                              value * 0.3);
                                else
                                        value -= number_range(value * 0.1,
                                                              value * 0.3);
                        }
                        if (value < 1)
                                value = 1;
                        total += value;
                        extract_obj(obj);
                        found = TRUE;
                        counter += 1;
                }
                if (found)
                {
                        if (counter > 1)
                        {
                                act("$n sells several items to the shopkeeper!", ch, NULL, NULL, TO_ROOM);
                                ch->bones += total;
                                act("You sell several items to the shopkeeper!", ch, NULL, NULL, TO_CHAR);
                                sprintf(buf,
                                        "You make %d bones from your sells!\n\r",
                                        total);
                                stc(buf, ch);
                        }
                        else
                        {
                                act("$n sells an item to the shopkeeper!", ch,
                                    NULL, NULL, TO_ROOM);
                                ch->bones += total;
                                sprintf(buf,
                                        "You sell an item to the shopkeeper for %d bones\n\r!",
                                        total);
                                act(buf, ch, NULL, NULL, TO_CHAR);
                        }
                }
                else
                {
                        stc("Either you don't have them or the shopkeeper doesn't want them.\n\r", ch);
                        return;
                }
        }
        return;
}


void do_list(CHAR_DATA * ch, char *argument)
{
        ROOM_INDEX_DATA *room;
        OBJ_INDEX_DATA *pObjIndex;
        char      buf[MAX_STRING_LENGTH];
        int       i;
        int       x;
        int       s = 0;

        if (IS_NPC(ch))
                return;
        room = ch->in_room;
        if (room == NULL)
                return;
        if (room->shop_type == 0 || room->shop_type == 11
            || room->shop_type == 12)
        {
                stc("You aren't at a shop.\n\r", ch);
                return;
        }


        if (room->shop_type < 13)
        {

                for (x = 1; x < 25; x++)
                {
                        if (room->items_sold[x] != 0)
                                s++;
                }
                if (s > 0)
                {
                        sprintf(buf,
                                "#0[#CN#cumber#0]        [#CC#cost#0]               [#CI#ctems #CF#cor #CS#cale#0]#n\n\r");
                        stc(buf, ch);
                        sprintf(buf,
                                "#0-#C=#P_#C=#0-#C=#P_#C=#0-#C=#P_#C=#0-#C=#P_#C=#0-#C=#P_#C=#0-#C=#P_#C=#0-#C=#P_#C=#0-#C=#P_#C=#0-#C=#P_#C=#0-#C=#P_#C=#0-#C=#P_#C=#0-#C=#P_#C=#0-#C=#P_#C=#0-#C=#P_#C=#0-#n\n\r");
                        stc(buf, ch);
                        stc("#p|                       #p|                           #n\n\r", ch);
                }
                for (i = 1; i < 25; i++)
                {
                        /*
                         * sprintf(buf, "Number    Item                       Cost      #n\n\r");
                         * stc(buf, ch);
                         * sprintf(buf, "===============================================#n\n\r");
                         * stc(buf, ch); 
                         */
                        if (room->items_sold[i] == 0)
                                continue;
                        if (room->items_sold[i] != 0)
                        {
                                pObjIndex =
                                        get_obj_index(room->items_sold[i]);
                                sprintf(buf,
                                        "#p|#n  %-2d   #y%10d bones #p|#C/#0> %s#n\n\r",
                                        i, pObjIndex->cost,
                                        pObjIndex->short_descr);
                                stc(buf, ch);
                        }

                        /*
                         * sprintf(buf, "===============================================\n\r");
                         * stc(buf, ch);
                         */
                }
                if (s > 0)
                {
                        stc("#p|                       #p|                       #n\n\r", ch);
                        sprintf(buf,
                                "#C_#c-#0=#c-#C_#c-#0=#c-#C_#c-#0=#c-#C_#c-#0=#c-#C_#c-#0=#c-#C_#c-#0=#c-#C_#c-#0=#c-#C_#c-#0=#c-#C_#c-#0=#c-#C_#c-#0=#c-#C_#c-#0=#c-#C_#c-#0=#c-#C_#c-#0=#c-#C_#c-#0=#c-#C_#n\n\r");
                        stc(buf, ch);
                }
        }
        return;
}

void do_describe(CHAR_DATA * ch, char *argument)
{
        ROOM_INDEX_DATA *room;
        OBJ_INDEX_DATA *pObjIndex;
        OBJ_DATA *obj;
        AFFECT_DATA *paf;
        int       vnum = 0;
        char      arg[MAX_INPUT_LENGTH];
        char      buf[MAX_STRING_LENGTH];
        int       itemcost = 0;
        int       itemtype;


        argument = one_argument(argument, arg);

        if (IS_NPC(ch))
                return;

        room = ch->in_room;

        if (room == NULL)
                return;

        if (room->shop_type == 0 || room->shop_type >= 10)
        {
                stc("You aren't at a shop.\n\r", ch);
                return;
        }

        if (arg[0] == '\0')
        {
                stc("Describe what? Type 'list' to see what items are for sale here.\n\r", ch);
                return;
        }


        if (atoi(arg) == 0)
        {
                stc("Invalid item number.\n\r", ch);
                return;
        }

        if (room->items_sold[atoi(arg)] != 0)
        {
                vnum = room->items_sold[atoi(arg)];

                if (vnum != 0 && (pObjIndex = get_obj_index(vnum)) != NULL)
                {
                        obj = create_object(pObjIndex, 50);
                        itemcost = obj->cost;

                        sprintf(buf,
                                "The shopkeeper gives you a description for %s:\n\r",
                                obj->short_descr);
                        stc(buf, ch);
                        sprintf(buf,
                                "%s\n\rItem Type: %s.\n\rProperties: %s %s\n\r",
                                obj->description, item_type_name(obj),
                                extra_bit_name(obj->extra_flags),
                                extra2_bit_name(obj->extra_flags2));
                        stc(buf, ch);
                        sprintf(buf, "Cost: %d\n\r", obj->cost);
                        stc(buf, ch);

                        switch (obj->item_type)
                        {

                        case ITEM_FAMILIAR:
                                sprintf(buf,
                                        "%s is a familiar.\n\r%s\n\rLevel:             %d\n\r\
             Min Tier Lvl Req:  %d\n\rChance to Hit:     %d%%\n\r\
             Base Damage:       %d\n\rExp:               %d\n\r\
             Learning Ability:  %d%%\n\rAlignment:         %d\n\rKeywords:  '%s'\n\r",
                                        obj->short_descr, obj->description, obj->value[1], obj->value[3], obj->value[4], obj->value[2], obj->value[5], obj->value[6], obj->value[7], obj->name);
                                stc(buf, ch);
                                break;

                        case ITEM_PILL:
                        case ITEM_SCROLL:
                        case ITEM_POTION:
                                xprintf(buf, "Level %d spells of:",
                                        obj->value[0]);
                                send_to_char(buf, ch);

                                if (obj->value[1] >= 0
                                    && obj->value[1] < MAX_SKILL)
                                {
                                        send_to_char(" '", ch);
                                        send_to_char(skill_table
                                                     [obj->value[1]].name,
                                                     ch);
                                        send_to_char("'", ch);
                                }

                                if (obj->value[2] >= 0
                                    && obj->value[2] < MAX_SKILL)
                                {
                                        send_to_char(" '", ch);
                                        send_to_char(skill_table
                                                     [obj->value[2]].name,
                                                     ch);
                                        send_to_char("'", ch);
                                }

                                if (obj->value[3] >= 0
                                    && obj->value[3] < MAX_SKILL)
                                {
                                        send_to_char(" '", ch);
                                        send_to_char(skill_table
                                                     [obj->value[3]].name,
                                                     ch);
                                        send_to_char("'", ch);
                                }

                                send_to_char(".\n\r", ch);
                                break;

                        case ITEM_QUEST:
                                xprintf(buf, "Quest point value is %d.\n\r",
                                        obj->value[0]);
                                send_to_char(buf, ch);
                                break;

                        case ITEM_PIECE:
                                xprintf(buf,
                                        "This is a piece of another object.\n\r");
                                send_to_char(buf, ch);
                                break;

                        case ITEM_FAITHTOKEN:
                                xprintf(buf, "Faith point value is %d.\n\r",
                                        obj->value[0]);
                                send_to_char(buf, ch);
                                break;

                        case ITEM_GRENADE:
                                if (obj->value[2] < 2)
                                        xprintf(buf,
                                                "This is a minor explosive device.\n\r");
                                else if (obj->value[2] < 5)
                                        xprintf(buf,
                                                "This is an explosive device.\n\r");
                                else
                                        xprintf(buf,
                                                "This is a powerful explosive device.\n\r");
                                send_to_char(buf, ch);
                                break;

                        case ITEM_WAND:
                        case ITEM_STAFF:
                                xprintf(buf, "Has %d(%d) charges of level %d",
                                        obj->value[1], obj->value[2],
                                        obj->value[0]);
                                send_to_char(buf, ch);

                                if (obj->value[3] >= 0
                                    && obj->value[3] < MAX_SKILL)
                                {
                                        send_to_char(" '", ch);
                                        send_to_char(skill_table
                                                     [obj->value[3]].name,
                                                     ch);
                                        send_to_char("'", ch);
                                }

                                send_to_char(".\n\r", ch);
                                break;

                        case ITEM_WEAPON:
                                xprintf(buf, "Weapon is of type %s.\n\r",
                                        flag_string(weapon_flags,
                                                    obj->value[3]));
                                send_to_char(buf, ch);
                                xprintf(buf,
                                        "Damage is %d to %d (average %d).\n\r",
                                        obj->value[1], obj->value[2],
                                        (obj->value[1] + obj->value[2]) / 2);
                                send_to_char(buf, ch);

                                if (obj->value[0] >= 1000)
                                        itemtype =
                                                obj->value[0] -
                                                ((obj->value[0] / 1000) *
                                                 1000);
                                else
                                        itemtype = obj->value[0];

                                if (itemtype > 0)
                                {
                                        if (obj->level < 10)
                                                xprintf(buf,
                                                        "%s is a minor spell weapon.\n\r",
                                                        capitalize(obj->
                                                                   short_descr));
                                        else if (obj->level < 20)
                                                xprintf(buf,
                                                        "%s is a lesser spell weapon.\n\r",
                                                        capitalize(obj->
                                                                   short_descr));
                                        else if (obj->level < 30)
                                                xprintf(buf,
                                                        "%s is an average spell weapon.\n\r",
                                                        capitalize(obj->
                                                                   short_descr));
                                        else if (obj->level < 40)
                                                xprintf(buf,
                                                        "%s is a greater spell weapon.\n\r",
                                                        capitalize(obj->
                                                                   short_descr));
                                        else if (obj->level < 50)
                                                xprintf(buf,
                                                        "%s is a major spell weapon.\n\r",
                                                        capitalize(obj->
                                                                   short_descr));
                                        else if (obj->level > 50)
                                                xprintf(buf,
                                                        "%s is an ultimate spell weapon.\n\r",
                                                        capitalize(obj->
                                                                   short_descr));
                                        else
                                                xprintf(buf,
                                                        "%s is a supreme spell weapon.\n\r",
                                                        capitalize(obj->
                                                                   short_descr));
                                        send_to_char(buf, ch);
                                }

                                if (itemtype == 1)
                                        xprintf(buf,
                                                "This weapon is dripping with corrosive acid.\n\r");
                                else if (itemtype == 4)
                                        xprintf(buf,
                                                "This weapon radiates an aura of darkness.\n\r");
                                else if (itemtype == 30)
                                        xprintf(buf,
                                                "This ancient relic is the bane of all evil.\n\r");
                                else if (itemtype == 34)
                                        xprintf(buf,
                                                "This vampiric weapon drinks the souls of its victims.\n\r");
                                else if (itemtype == 37)
                                        xprintf(buf,
                                                "This weapon has been tempered in hellfire.\n\r");
                                else if (itemtype == 48)
                                        xprintf(buf,
                                                "This weapon crackles with sparks of lightning.\n\r");
                                else if (itemtype == 53)
                                        xprintf(buf,
                                                "This weapon is dripping with a dark poison.\n\r");
                                else if (itemtype > 0)
                                        xprintf(buf,
                                                "This weapon has been imbued with the power of %s.\n\r",
                                                skill_table[itemtype].name);
                                if (itemtype > 0)
                                        send_to_char(buf, ch);

                                if (obj->value[0] >= 1000)
                                        itemtype = obj->value[0] / 1000;
                                else
                                        break;

                                if (itemtype == 4)
                                        xprintf(buf,
                                                "This weapon radiates an aura of darkness.\n\r");
                                else if (itemtype == 27 || itemtype == 2)
                                        xprintf(buf,
                                                "This weapon allows the wielder to see invisible things.\n\r");
                                else if (itemtype == 39 || itemtype == 3)
                                        xprintf(buf,
                                                "This weapon grants the power of flight.\n\r");
                                else if (itemtype == 45 || itemtype == 1)
                                        xprintf(buf,
                                                "This weapon allows the wielder to see in the dark.\n\r");
                                else if (itemtype == 46 || itemtype == 5)
                                        xprintf(buf,
                                                "This weapon renders the wielder invisible to the human eye.\n\r");
                                else if (itemtype == 52 || itemtype == 6)
                                        xprintf(buf,
                                                "This weapon allows the wielder to walk through solid doors.\n\r");
                                else if (itemtype == 54 || itemtype == 7)
                                        xprintf(buf,
                                                "This holy weapon protects the wielder from evil.\n\r");
                                else if (itemtype == 139)
                                        xprintf(buf,
                                                "This unholy weapon protects the wielder from good.\n\r");
                                else if (itemtype == 57 || itemtype == 8)
                                        xprintf(buf,
                                                "This ancient weapon protects the wielder in combat.\n\r");
                                else if (itemtype == 9)
                                        xprintf(buf,
                                                "This crafty weapon allows the wielder to walk in complete silence.\n\r");
                                else if (itemtype == 10)
                                        xprintf(buf,
                                                "This powerful weapon surrounds its wielder with a shield of lightning.\n\r");
                                else if (itemtype == 11)
                                        xprintf(buf,
                                                "This powerful weapon surrounds its wielder with a shield of fire.\n\r");
                                else if (itemtype == 12)
                                        xprintf(buf,
                                                "This powerful weapon surrounds its wielder with a shield of ice.\n\r");
                                else if (itemtype == 13)
                                        xprintf(buf,
                                                "This powerful weapon surrounds its wielder with a shield of acid.\n\r");
                                else if (itemtype == 14)
                                        xprintf(buf,
                                                "This weapon protects its wielder from clan DarkBlade guardians.\n\r");
                                else if (itemtype == 15)
                                        xprintf(buf,
                                                "This ancient weapon surrounds its wielder with a shield of chaos.\n\r");
                                else if (itemtype == 16)
                                        xprintf(buf,
                                                "This ancient weapon regenerates the wounds of its wielder.\n\r");
                                else if (itemtype == 26)
                                        xprintf(buf,
                                                "This ancient weapon gives the power of mantis 3.\n\r");
                                else if (itemtype == 17)
                                        xprintf(buf,
                                                "This ancient weapon allows its wielder to move at supernatural speed.\n\r");
                                else if (itemtype == 18)
                                        xprintf(buf,
                                                "This razor sharp weapon can slice through armour without difficulty.\n\r");
                                else if (itemtype == 19)
                                        xprintf(buf,
                                                "This ancient weapon spews powerful magics.\n\r");
                                else if (itemtype == 21)
                                        xprintf(buf,
                                                "This ancient weapon grants superior protection to its wielder.\n\r");
                                else if (itemtype == 22)
                                        xprintf(buf,
                                                "This ancient weapon grants its wielder supernatural vision.\n\r");
                                else if (itemtype == 23)
                                        xprintf(buf,
                                                "This ancient weapon makes its wielder fleet-footed.\n\r");
                                else if (itemtype == 24)
                                        xprintf(buf,
                                                "This ancient weapon conceals its wielder from sight.\n\r");
                                else if (itemtype == 25)
                                        xprintf(buf,
                                                "This ancient weapon invokes the power of the beast.\n\r");
                                else if (itemtype == 28)
                                        xprintf(buf,
                                                "This ancient weapon increases the level of your spells by 20.\n\r");
                                else
                                        xprintf(buf,
                                                "This item is bugged...please report it.\n\r");
                                if (itemtype > 0)
                                        send_to_char(buf, ch);
                                break;

                        case ITEM_ARMOR:
                                xprintf(buf, "Armor class is %d.\n\r",
                                        obj->value[0]);
                                send_to_char(buf, ch);
                                if (obj->value[3] < 1)
                                        break;
                                if (obj->value[3] == 4)
                                        xprintf(buf,
                                                "This object radiates an aura of darkness.\n\r");
                                else if (obj->value[3] == 27
                                         || obj->value[3] == 2)
                                        xprintf(buf,
                                                "This item allows the wearer to see invisible things.\n\r");
                                else if (obj->value[3] == 39
                                         || obj->value[3] == 3)
                                        xprintf(buf,
                                                "This object grants the power of flight.\n\r");
                                else if (obj->value[3] == 45
                                         || obj->value[3] == 1)
                                        xprintf(buf,
                                                "This item allows the wearer to see in the dark.\n\r");
                                else if (obj->value[3] == 46
                                         || obj->value[3] == 5)
                                        xprintf(buf,
                                                "This object renders the wearer invisible to the human eye.\n\r");
                                else if (obj->value[3] == 52
                                         || obj->value[3] == 6)
                                        xprintf(buf,
                                                "This object allows the wearer to walk through solid doors.\n\r");
                                else if (obj->value[3] == 54
                                         || obj->value[3] == 7)
                                        xprintf(buf,
                                                "This holy relic protects the wearer from evil.\n\r");
                                else if (obj->value[3] == 139)
                                        xprintf(buf,
                                                "This unholy relic protects the wearer from good.\n\r");
                                else if (obj->value[3] == 57
                                         || obj->value[3] == 8)
                                        xprintf(buf,
                                                "This ancient relic protects the wearer in combat.\n\r");
                                else if (obj->value[3] == 9)
                                        xprintf(buf,
                                                "This crafty item allows the wearer to walk in complete silence.\n\r");
                                else if (obj->value[3] == 10)
                                        xprintf(buf,
                                                "This powerful item surrounds its wearer with a shield of lightning.\n\r");
                                else if (obj->value[3] == 11)
                                        xprintf(buf,
                                                "This powerful item surrounds its wearer with a shield of fire.\n\r");
                                else if (obj->value[3] == 12)
                                        xprintf(buf,
                                                "This powerful item surrounds its wearer with a shield of ice.\n\r");
                                else if (obj->value[3] == 13)
                                        xprintf(buf,
                                                "This powerful item surrounds its wearer with a shield of acid.\n\r");
                                else if (obj->value[3] == 14)
                                        xprintf(buf,
                                                "This object channels the power of God into its wearer.\n\r");
                                else if (obj->value[3] == 32)
                                        xprintf(buf,
                                                "This object channels the power of Lloth into its wearer.\n\r");
                                else if (obj->value[3] == 15)
                                        xprintf(buf,
                                                "This ancient item surrounds its wearer with a shield of chaos.\n\r");
                                else if (obj->value[3] == 16)
                                        xprintf(buf,
                                                "This ancient item regenerates the wounds of its wearer.\n\r");
                                else if (obj->value[3] == 17)
                                        xprintf(buf,
                                                "This ancient item allows its wearer to move at supernatural speed.\n\r");
                                else if (obj->value[3] == 18)
                                        xprintf(buf,
                                                "This powerful item allows its wearer to shear through armour without difficulty.\n\r");
                                else if (obj->value[3] == 19)
                                        xprintf(buf,
                                                "This powerful item spews horrifying magics.\n\r");
                                else if (obj->value[3] == 21)
                                        xprintf(buf,
                                                "This ancient item grants superior protection to its wearer.\n\r");
                                else if (obj->value[3] == 22)
                                        xprintf(buf,
                                                "This ancient item grants its wearer supernatural vision.\n\r");
                                else if (obj->value[3] == 23)
                                        xprintf(buf,
                                                "This ancient item makes its wearer fleet-footed.\n\r");
                                else if (obj->value[3] == 24)
                                        xprintf(buf,
                                                "This ancient item conceals its wearer from sight.\n\r");
                                else if (obj->value[3] == 25)
                                        xprintf(buf,
                                                "This ancient item invokes the power of the beast.\n\r");
                                else if (obj->value[3] == 28)
                                        xprintf(buf,
                                                "This ancient item invokes the power of ancient magic.\n\r");
                                else if (obj->value[3] == 29)
                                        xprintf(buf,
                                                "This ancient artifact grants its wearer the power of the third eye.\n\r");
                                else if (obj->value[3] == 30)
                                        xprintf(buf,
                                                "This great bird grants the power of talons.\n\r");
                                else if (obj->value[3] == 31)
                                        xprintf(buf,
                                                "This cursed item invokes madness.\n\r");
                                else
                                        xprintf(buf,
                                                "This item is bugged...please report it.\n\r");
                                if (obj->value[3] > 0)
                                        send_to_char(buf, ch);
                                break;
                        }

                        for (paf = obj->pIndexData->affected; paf != NULL;
                             paf = paf->next)
                        {
                                if (paf->location != APPLY_NONE
                                    && paf->modifier != 0)
                                {
                                        xprintf(buf, "Affects %s by %d.\n\r",
                                                affect_loc_name(paf->
                                                                location),
                                                paf->modifier);
                                        send_to_char(buf, ch);
                                }
                        }


                        extract_obj(obj);
                }
                else
                {
                        stc("Bug, inform an imm.\n\r", ch);
                        return;
                }
        }

        else
        {
                stc("That item number is not for sale. Type 'list' to see what *is* for sale.\n\r", ch);
                return;
        }
        return;
}

bool can_buy(CHAR_DATA * ch)
{

        /*
         * Thieves, Nightblades and Assassins
         */
        //if ( (ch->in_room->vnum >= 40021 && ch->in_room->vnum <= 40030) && (!IS_CLASS(ch,CLASS_THIEF) && !IS_CLASS3(ch,CLASS3_ASSASSIN) && !IS_CLASS2(ch,CLASS2_NIGHTBLADE) ) )
        //return FALSE;

        return TRUE;
}
