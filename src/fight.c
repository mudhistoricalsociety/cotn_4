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
#include <limits.h>
#include "merc.h"
#include "interp.h"
#include "magic.h"

#define MAX_SLAY_TYPES 3
#define MONK_AUTODROP 12

/*
 * Local functions.
 */
void autodrop args((CHAR_DATA * ch));
bool check_dodge args((CHAR_DATA * ch, CHAR_DATA * victim, int dt));
bool check_parry args((CHAR_DATA * ch, CHAR_DATA * victim, int dt));
void dam_message args((CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt));
void death_cry args((CHAR_DATA * ch));
void group_gain args((CHAR_DATA * ch, CHAR_DATA * victim));
int xp_compute args((CHAR_DATA * gch, CHAR_DATA * victim));
bool can_counter args((CHAR_DATA * ch));
bool can_bypass args((CHAR_DATA * ch, CHAR_DATA * victim));
int number_attacks args((CHAR_DATA * ch, CHAR_DATA * victim));
int dambonus args((CHAR_DATA * ch, CHAR_DATA * victim, int dam, int stance));
void decap_message args((CHAR_DATA * ch, CHAR_DATA * victim));
void fae_update args((CHAR_DATA * ch));
void angel_eye args((CHAR_DATA * ch, CHAR_DATA * victim, int dam));
void elemental_enflame       args((CHAR_DATA *ch, CHAR_DATA *victim, int dam));
void fae_shield args((CHAR_DATA * ch, CHAR_DATA * victim, int dam));
void improve_weapon_res args((CHAR_DATA * ch, int dtype));

extern CHAR_DATA *quest_target;
extern CHAR_DATA *quest_mob;

/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update(void)
{
        CHAR_DATA *ch;
        CHAR_DATA *ch_next;
        CHAR_DATA *emb;
        CHAR_DATA *victim;
        CHAR_DATA *rch;
        CHAR_DATA *rch_next;
        CHAR_DATA *mount;
        OBJ_DATA *obj, *obj_next;
        bool      room_trig = FALSE;
        int       chance;

        chance = number_percent();

        for (ch = char_list; ch != NULL; ch = ch->next)
        {
                if (IS_NPC(ch) && IS_SET(ch->act, ACT_NODAMAGE))
                {
                        ch->hit = ch->max_hit;
                }
                if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_FAE)
                    && ch->position != POS_STUNNED)
                        fae_update(ch);

                victim = ch->blinkykill;

                if (victim != NULL)
                {

                        if (IS_SET(ch->flag3, AFF3_BLINK_1ST_RD))
                        {
                                REMOVE_BIT(ch->flag3, AFF3_BLINK_1ST_RD);
                                SET_BIT(ch->flag3, AFF3_BLINK_2ND_RD);
                        }
                        else if (IS_SET(ch->flag3, AFF3_BLINK_2ND_RD))
                        {
                                REMOVE_BIT(ch->flag3, AFF3_BLINK_2ND_RD);
                                REMOVE_BIT(ch->flag3, AFF3_BLINK_1ST_RD);
                                /*
                                 * Has the victim left? 
                                 */
                                if (victim->in_room != ch->in_room)
                                {
                                        SET_BIT(ch->affected_by2,
                                                EXTRA_BLINKY);
                                        victim = NULL;
                                        act("$n pops back into existence.",
                                            ch, NULL, NULL, TO_ROOM);
                                        stc("You pop back into existence.\n\r", ch);
                                        stc("Your victim is no longer in the room.\n\r", ch);
                                        break;
                                }
                                SET_BIT(ch->affected_by2, EXTRA_BLINKY);
                                act("$n pops back into existence next to $N.",
                                    ch, NULL, victim, TO_NOTVICT);
                                act("You pop back into existence next to $N.",
                                    ch, NULL, victim, TO_CHAR);
                                act("$n pops back into existence next to you.", ch, NULL, victim, TO_VICT);
                                if (!is_safe(ch, victim))
                                {
                                        if (ch->hit > 0)
                                        {
                                                set_fighting(ch, victim,
                                                             TRUE);
                                                do_say(ch, "Muhahahahahaha");
                                                multi_hit(ch, victim,
                                                          gsn_blinky);
                                                multi_hit(ch, victim,
                                                          gsn_blinky);
                                                if (IS_SET
                                                    (ch->newbits, THIRD_HAND)
                                                    && get_eq_char(ch,
                                                                   WEAR_THIRD)
                                                    != NULL)
                                                        multi_hit(ch, victim,
                                                                  gsn_blinky);
                                                if (IS_SET
                                                    (ch->newbits, FOURTH_HAND)
                                                    && get_eq_char(ch,
                                                                   WEAR_FOURTH)
                                                    != NULL)
                                                        multi_hit(ch, victim,
                                                                  gsn_blinky);
                                                update_pos(victim);
                                        }
                                }
                        }
                }   /*end of blink */
                ch_next = ch->next;
                if (!IS_NPC(ch))
                {
                        if (ch->fight_timer > 0)
                                ch->fight_timer--;
            		if (ch->fight_timer == 0)
			{
                        	if(ch->combat != NULL && !IS_NPC(ch->combat) && ch->combat->fight_timer == 0)
				{
                                	stc("Your victim is no longer fighting you.\n\r",ch->combat);
                        	}
                        	ch->combat = NULL;
                	}


                        if (ch->pcdata->agg_counter > 0)
                        {
                                ch->pcdata->agg_counter--;
                                if (ch->pcdata->agg_counter == 0)
                                {
                                        ch->pcdata->aggress_from = 0;
                                        ch->pcdata->aggress_towards = 0;
                                }
                        }
                }
                if (ch->rage > 0 && IS_CLASS(ch, CLASS_DEMON))
                        ch->rage--;
                if (ch->embracing != NULL)
                {
                        emb = ch->embracing;
                        if (emb == NULL)
                                ch->embracing = NULL;
                        if (ch->in_room != emb->in_room)
                        {
                                if (emb->embraced != NULL)
                                {
                                        emb->embraced = NULL;
                                        ch->embracing = NULL;
                                }
                                else
                                        ch->embracing = NULL;
                        }
                }
                if (ch->embraced != NULL)
                {
                        emb = ch->embraced;
                        if (emb == NULL)
                                ch->embraced = NULL;
                        if (ch->in_room != emb->in_room)
                        {
                                if (emb->embracing != NULL)
                                {
                                        emb->embracing = NULL;
                                        ch->embraced = NULL;
                                }
                                else
                                        ch->embraced = NULL;
                        }
                }
                if (IS_SET(ch->monkstuff, MONK_DEATH))
                {
                        if (ch->hit > (ch->max_hit / 2))
                        {
                                ch->hit -= number_range(50, 200);
                                stc("Your writhe in agony as magical energies tear you asunder.\n\r", ch);
                                act("$n writhes in agony as magical forces tear apart $s body.", ch, NULL, NULL, TO_ROOM);
                        }
                        else
                        {
                                if (number_range(1, 2) == 1)
                                {
                                        stc("You feel the magical forces leave your body.\n\r", ch);
                                        act("The magical forces leave $n's body.", ch, NULL, NULL, TO_ROOM);
                                        REMOVE_BIT(ch->monkstuff, MONK_DEATH);
                                }
                                else
                                {
                                        ch->hit -= number_range(50, 200);
                                        stc("Your writhe in agony as magical energies tear you asunder.\n\r", ch);
                                        act("$n writhes in agony as magical forces tear apart $s body.", ch, NULL, NULL, TO_ROOM);
                                }
                        }
                }
                if (IS_SET(ch->monkstuff, MONK_HEAL))
                {
                        if (ch->hit < (ch->max_hit / 2) && ch->hit > 0)
                        {
                                if (ch->hit < ch->max_hit)
                                        ch->hit += number_range(200, 400);
                                if (ch->move < ch->max_move)
                                        ch->move += number_range(175, 400);
                                stc("Your body emits glowing sparks.\n\r",
                                    ch);
                                act("$n's body emits glowing sparks and fizzes.", ch, NULL, NULL, TO_ROOM);
                        }
                        else if (ch->hit > 0)
                        {
                                if (number_range(1, 2) == 1)
                                {
                                        stc("The sparks fizzle and die.\n\r",
                                            ch);
                                        act("The sparks around $n's body fizzle and die.", ch, NULL, NULL, TO_ROOM);
                                        REMOVE_BIT(ch->monkstuff, MONK_HEAL);
                                }
                                else
                                {
                                        if (ch->hit < ch->max_hit)
                                                ch->hit +=
                                                        number_range(200,
                                                                     400);
                                        if (ch->move < ch->max_move)
                                                ch->move +=
                                                        number_range(175,
                                                                     400);
                                        stc("Your body emits glowing sparks.\n\r", ch);
                                        act("$n's body emits glowing sparks and fizzes.", ch, NULL, NULL, TO_ROOM);
                                }
                        }
                }
                if ((victim = ch->fighting) == NULL || ch->in_room == NULL)
                        continue;
                if (!IS_NPC(ch) && !IS_NPC(victim) && !is_safe(ch, victim)
                    && !is_safe(victim, ch))
                {
                        if (ch->fight_timer < 9)
                                ch->fight_timer = 10;
                        else if (ch->fight_timer < 25)
                                ch->fight_timer += 3;
                        if (ch->pcdata->aggress_towards != 0
                            || ch->pcdata->aggress_from != 0)
                        {
                                if (ch->pcdata->agg_counter < 9)
                                        ch->pcdata->agg_counter = 10;
                                else if (ch->pcdata->agg_counter < 25)
                                        ch->pcdata->agg_counter += 3;
                        }
                }
                if (IS_AWAKE(ch) && IS_AWAKE(victim)
                    && ch->in_room == victim->in_room)
                        multi_hit(ch, victim, TYPE_UNDEFINED);
                else
                        stop_fighting(ch, FALSE);
                if ((victim = ch->fighting) == NULL)
                        continue;
                /*
                 * Fun for the whole family!
                 */
                for (rch = ch->in_room->people; rch != NULL; rch = rch_next)
                {
                        rch_next = rch->next_in_room;
                        if (IS_AWAKE(rch) && rch->fighting == NULL)
                        {
                                /*
                                 * Mount's auto-assist their riders and vice versa.
                                 */
                                if ((mount = rch->mount) != NULL)
                                {
                                        if (mount == ch)
                                                multi_hit(rch, victim,
                                                          TYPE_UNDEFINED);
                                        continue;
                                }

                                /*
                                 * PC's auto-assist others in their group.
                                 */
                                if (!IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM))
                                {
                                        if ((!IS_NPC(rch)
                                             || IS_AFFECTED(rch, AFF_CHARM))
                                            && is_same_group(ch, rch))
                                                multi_hit(rch, victim,
                                                          TYPE_UNDEFINED);
                                        continue;
                                }
                                /*
                                 * NPC's assist NPC's of same type or 12.5% chance regardless.
                                 */
                                if (IS_NPC(rch)
                                    && !IS_AFFECTED(rch, AFF_CHARM))
                                {
                                        if (rch->pIndexData == ch->pIndexData
                                            || number_bits(3) == 0)
                                        {
                                                CHAR_DATA *vch;
                                                CHAR_DATA *target;
                                                int       number;

                                                target = NULL;
                                                number = 0;
                                                for (vch =
                                                     ch->in_room->people; vch;
                                                     vch = vch->next)
                                                {
                                                        if (can_see(rch, vch)
                                                            &&
                                                            is_same_group(vch,
                                                                          victim)
                                                            && number_range(0,
                                                                            number)
                                                            == 0)
                                                        {
                                                                target = vch;
                                                                number++;
                                                        }
                                                }
                                                if (target != NULL)
                                                        multi_hit(rch, target,
                                                                  TYPE_UNDEFINED);
                                        }
                                }
                        }
                }
                //mob prog
                if (IS_NPC(ch))
                {
                        if (HAS_TRIGGER_MOB(ch, TRIG_FIGHT))
                                p_percent_trigger(ch, NULL, NULL, victim,
                                                  NULL, NULL, TRIG_FIGHT);
                        if (HAS_TRIGGER_MOB(ch, TRIG_HPCNT))
                                p_hprct_trigger(ch, victim);
                }
                for (obj = ch->carrying; obj; obj = obj_next)
                {
                        obj_next = obj->next_content;

                        if (obj->wear_loc != WEAR_NONE
                            && HAS_TRIGGER_OBJ(obj, TRIG_FIGHT))
                                p_percent_trigger(NULL, obj, NULL, victim,
                                                  NULL, NULL, TRIG_FIGHT);
                }

                if (HAS_TRIGGER_ROOM(ch->in_room, TRIG_FIGHT)
                    && room_trig == FALSE)
                {
                        room_trig = TRUE;
                        p_percent_trigger(NULL, NULL, ch->in_room, victim,
                                          NULL, NULL, TRIG_FIGHT);
                }
                if (IS_AFF2(ch, AFF2_FROZEN))
                {
                        send_to_char("The ice around melts a little bit.\n\r",
                                     ch);
                        act("The ice around $n melts a little.", ch, NULL,
                            NULL, TO_ROOM);
                        if (number_range(1, 100) > 49)
                        {
                                send_to_char
                                        ("The ice around you is so thin you flex your muscles and shatter it.\n\r",
                                         ch);
                                act("The ice around $n shatters revealing one pissed off $n.", ch, NULL, NULL, TO_ROOM);
                                REMOVE_BIT(ch->affected_by2, AFF2_FROZEN);
                        }
                }
        }
        return;
}

void fae_update(CHAR_DATA * ch)
{
        if (ch->hit < 1)
                return;
        if (ch->pcdata->powers[FAE_PLASMA_TICK] >
            ch->pcdata->powers[FAE_PLASMA])
        {
                ch->hit =
                        UMAX(1,
                             ch->hit -
                             ch->pcdata->powers[FAE_PLASMA_TICK] *
                             number_range(200, 400));
                send_to_char
                        ("#oYou scream in pain as you lose control over the forces of plasma.#n\n\r",
                         ch);
                act("$n screams in pain, strange plasma flows from $s body.",
                    ch, NULL, NULL, TO_ROOM);
                ch->pcdata->powers[FAE_PLASMA_TICK] = 0;
                ch->pcdata->powers[FAE_PLASMA_GROWTH] = 0;
        }
        if (ch->pcdata->powers[FAE_WILL_TICK] > ch->pcdata->powers[FAE_WILL])
        {
                ch->hit =
                        UMAX(1,
                             ch->hit -
                             ch->pcdata->powers[FAE_WILL_TICK] *
                             number_range(200, 400));
                send_to_char
                        ("#oYou scream in pain as you lose concentration and your willpower crumbles.#n\n\r",
                         ch);
                act("$n screams in pain, and clutches $s head.", ch, NULL,
                    NULL, TO_ROOM);
                ch->pcdata->powers[FAE_WILL_TICK] = 0;
                ch->pcdata->powers[FAE_WILL_GROWTH] = 0;
        }
        if (ch->pcdata->powers[FAE_ENERGY_TICK] >
            ch->pcdata->powers[FAE_ENERGY])
        {
                ch->hit =
                        UMAX(1,
                             ch->hit -
                             ch->pcdata->powers[FAE_ENERGY_TICK] *
                             number_range(200, 400));
                send_to_char
                        ("#oYou scream in pain as you lose control over the forces of energy.#n\n\r",
                         ch);
                act("$n screams in pain, strange energies flows from $s body.", ch, NULL, NULL, TO_ROOM);
                ch->pcdata->powers[FAE_ENERGY_TICK] = 0;
                ch->pcdata->powers[FAE_ENERGY_GROWTH] = 0;
        }
        if (ch->pcdata->powers[FAE_MATTER_TICK] >
            ch->pcdata->powers[FAE_MATTER])
        {
                ch->hit =
                        UMAX(1,
                             ch->hit -
                             ch->pcdata->powers[FAE_MATTER_TICK] *
                             number_range(200, 400));
                send_to_char
                        ("#oYou scream in pain as you lose control over the forces of matter.#n\n\r",
                         ch);
                act("$n screams in pain, strange matter flows from $s body.",
                    ch, NULL, NULL, TO_ROOM);
                ch->pcdata->powers[FAE_MATTER_TICK] = 0;
                ch->pcdata->powers[FAE_MATTER_GROWTH] = 0;
        }
        if (ch->pcdata->powers[FAE_PLASMA_GROWTH] > 0)
        {
                ch->pcdata->powers[FAE_PLASMA_TICK] +=
                        ch->pcdata->powers[FAE_PLASMA_GROWTH];
                send_to_char
                        ("#YYou channel more #Gplasma#Y from your surroundings into your body.#n\n\r",
                         ch);
        }
        if (ch->pcdata->powers[FAE_MATTER_GROWTH] > 0)
        {
                ch->pcdata->powers[FAE_MATTER_TICK] +=
                        ch->pcdata->powers[FAE_MATTER_GROWTH];
                send_to_char
                        ("#YYou channel more #Gmatter#Y from your homeplane into existance.#n\n\r",
                         ch);
        }
        if (ch->pcdata->powers[FAE_ENERGY_GROWTH] > 0)
        {
                ch->pcdata->powers[FAE_ENERGY_TICK] +=
                        ch->pcdata->powers[FAE_ENERGY_GROWTH];
                send_to_char
                        ("#YYou channel more #Genergy#Y from the arcane into your mind.#n\n\r",
                         ch);
        }
        if (ch->pcdata->powers[FAE_WILL_GROWTH] > 0)
        {
                ch->pcdata->powers[FAE_WILL_TICK] +=
                        ch->pcdata->powers[FAE_WILL_GROWTH];
                send_to_char
                        ("#YYou concentrate on your #Gwillpower#Y, focusing your thoughts into combat.#n\n\r",
                         ch);
        }
        return;
}

/*
 * Do one group of attacks.
 */
void multi_hit(CHAR_DATA * ch, CHAR_DATA * victim, int dt)
{
        OBJ_DATA *wield;
        OBJ_DATA *wield1;
        OBJ_DATA *wield2;
        OBJ_DATA *wield3;
        OBJ_DATA *wield4;
        OBJ_DATA *familiar;
        char      buf[MAX_STRING_LENGTH];
        int       sn, level, chance, unarmed, maxcount, countup = -1, x =
                0, wieldie, i;
        AFFECT_DATA af;
        int       wieldorig = 0;
        int       wieldtype = 0;
	char buf1[MSL];
	char buf2[MSL];
        if (ch->position < POS_SLEEPING)
                return;
        if (IS_CLASS(ch, CLASS_MONK))
        {
                if (!IS_NPC(ch) && wieldorig == 0)
                {
                        if (countup == 0)
                                unarmed = number_range(0, 3);
                        else if (countup == 1)
                                unarmed = number_range(4, 7);
                        else
                                unarmed = number_range(0, 7);
                        if (!IS_NPC(ch) && ch->cmbt[unarmed] != 0
                            && wieldorig == 0)
                                fightaction(ch, victim, ch->cmbt[unarmed], dt,
                                            wieldtype);
                }
        }
        if (dt == gsn_headbutt)
        {
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_hooves)
        {
                one_hit(ch, victim, dt, 1);
                one_hit(ch, victim, dt, 1);
                if (number_range(1, 3) != 2)
                {
                        one_hit(ch, victim, dt, 1);
                        one_hit(ch, victim, dt, 1);
                }
                return;
        }
        if (dt == gsn_shiroken)
        {
                for (i = number_range(3, 5); i > 0; i--)
                        one_hit(ch, victim, dt, 1);
                if (ch->pcdata->powers[NPOWER_NINGENNO] >= 4)
                        spell_poison(gsn_poison,
                                     (ch->level * number_range(50, 60)), ch,
                                     victim);
                return;
        }

  if ( dt == gsn_zomplague ){
	for(i = number_range(2,3); i > 0; i--)
	one_hit(ch,victim,dt,1);
	return;	
  }
  if ( dt == gsn_stench ){
	for(i = number_range(1,1); i > 0; i--)
	one_hit(ch,victim,dt,1);
	if(!IS_NPC(victim) && !IS_SET(victim->newbits2,NEW2_STENCH)) SET_BIT(victim->newbits2,NEW2_STENCH);
	return;	
  }

  if(IS_CLASS(ch,CLASS_SHINOBI)){
		switch (ch->pcdata->powers[S_ELEMENT]){
			case S_FIRE:
				if(IS_SET(ch->in_room->shinobi, ROOM_SFIRE)){
				one_hit(ch,victim,gsn_shinfire,0);
				one_hit(ch,victim,gsn_shinfire,0);
				one_hit(ch,victim,gsn_shinfire,0);}
				break;
			case S_WIND:
				if(IS_SET(ch->in_room->shinobi, ROOM_SWIND)){
				one_hit(ch,victim,gsn_shinwind,0);
				one_hit(ch,victim,gsn_shinwind,0);
				one_hit(ch,victim,gsn_shinwind,0);}
				break;
			case S_WATER:
				if(IS_SET(ch->in_room->shinobi, ROOM_SWATER)){
				one_hit(ch,victim,gsn_shinwater,0);
				one_hit(ch,victim,gsn_shinwater,0);
				one_hit(ch,victim,gsn_shinwater,0);}
				break;
			case S_EARTH:
				if(IS_SET(ch->in_room->shinobi, ROOM_SEARTH)){
				one_hit(ch,victim,gsn_shinearth,0);
				one_hit(ch,victim,gsn_shinearth,0);
				one_hit(ch,victim,gsn_shinearth,0);}
				break;
			case S_HOLY:
				if(IS_SET(ch->in_room->shinobi, ROOM_SHOLY)){
				one_hit(ch,victim,gsn_shinholy,0);
				one_hit(ch,victim,gsn_shinholy,0);
				one_hit(ch,victim,gsn_shinholy,0);}
				break;
			case S_SHADOW:
				if(ch->pcdata->powers[S_HANDSHADOW] == 6){
				one_hit(ch,victim,gsn_shinshadow,0);
				one_hit(ch,victim,gsn_shinshadow,0);
				one_hit(ch,victim,gsn_shinshadow,0);}
				break;
		}
		one_hit(ch,victim,gsn_shin,0);
	}

        if (dt == gsn_laser)
        {
                one_hit(ch, victim, dt, 1);
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_stinger)
        {
                one_hit(ch, victim, dt, 1);
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_claws)
        {
                one_hit(ch, victim, dt, 1);
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_tentacle)
        {

                act("You shoot forth a shower of tentacles to destroy $N.",
                    ch, NULL, victim, TO_CHAR);
                act("$n shoots forth a shower of tentacles aiming straight at you.", ch, NULL, victim, TO_VICT);
                act("$n shoots forth a shower of tentacles striking $N.", ch,
                    NULL, victim, TO_NOTVICT);

                x = number_range(2, 5);
                while (--x >= 0)
                        one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_fangs)
        {
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_quills)
        {
                if ((IS_CLASS(ch, CLASS_WEREWOLF))
                    && (ch->power[DISC_WERE_PAIN] > 9))
                {
                        one_hit(ch, victim, dt, 1);
                        one_hit(ch, victim, dt, 1);
                        one_hit(ch, victim, dt, 1);
                        one_hit(ch, victim, dt, 1);
                }
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_darktendrils && IS_CLASS(ch, CLASS_DROW))
        {
                one_hit(ch, victim, dt, 1);
                one_hit(ch, victim, dt, 1);
                one_hit(ch, victim, dt, 1);
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_venomtong)
        {
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_spiketail)
        {
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_lunarflare)
        {
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_magma)
        {
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_shards)
        {
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_cheapshot)
        {
                send_to_char("You stun them with a shoulder charge!\n\r", ch);
                send_to_char("You are stunned by a shoulder charge!\n\r",
                             victim);
                one_hit(ch, victim, dt, 1);
                one_hit(ch, victim, dt, 1);
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_buffet)
        {
                one_hit(ch, victim, dt, 1);
                return;
        }
	if (IS_CLASS(ch, CLASS_DRAGON) && IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TAIL))
	{
	if ( number_range (1,18) < ch->pcdata->powers[dragonage])
	{
	 one_hit ( ch, victim, gsn_spiket, 0);
	}
	one_hit ( ch, victim, gsn_spiket, 0);
	}
	if (IS_CLASS(ch, CLASS_DRAGON) && IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_WINGS) &&
	number_range(1,3)==1) one_hit (ch,victim,gsn_buffet,0);
	if (IS_CLASS(ch, CLASS_DRAGON) && IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CLAWS))
	{
	if (number_range(1, 3)==1) one_hit (ch, victim, gsn_claws,0);
	one_hit (ch, victim, gsn_claws,0);
	}
	if (IS_CLASS(ch, CLASS_DRAGON) && IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FANGS))
	{
	if (number_range(1,4)==1) one_hit(ch,victim,gsn_rfangs,1);
	if (number_range(1,10)==1)multi_hit(ch,victim,gsn_rfangs);
	one_hit (ch, victim, gsn_rfangs,0);
	}


	if(!IS_NPC(ch) && IS_CLASS(ch, CLASS_ZOMBIE) && IS_SET(ch->pcdata->powers[ZOM_ACTIVE], ZOM_PLAGUE)){
		multi_hit(ch,victim,gsn_zomplague);
	}
	if(!IS_NPC(ch) && IS_CLASS(ch, CLASS_ZOMBIE) && IS_SET(ch->pcdata->powers[ZOM_ACTIVE], ZOM_DECAY)){
		multi_hit(ch,victim,gsn_stench);
	}
        if (dt == gsn_sweep)
        {
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_rfangs)
        {
                one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_heavenlyaura)
        {
                if (IS_CLASS(ch, CLASS_MONK))
                        x = number_range(1, 4);
                else
                {
                        if (IS_CLASS(victim, CLASS_SAMURAI))
                                x = number_range(3, 5); // angels don't like em samurais.
                        else
                                x = number_range(2, 5);
                }
                for (i = 0; i < x; i++)
                        one_hit(ch, victim, dt, 1);
                return;
        }
        if (dt == gsn_mageshield)
        {
                if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
                {
                        one_hit(ch, victim, dt, 1);
                        one_hit(ch, victim, dt, 1);
                        one_hit(ch, victim, dt, 1);
                        one_hit(ch, victim, dt, 1);
                        one_hit(ch, victim, dt, 1);
                }
                return;
        }
        if (dt == gsn_bash) // giants only, mind you.
        {
                for (i = 1; i < ch->pcdata->rank; i++)
                {
                        x = number_range(1, 3);
                        if (x == 1)
                                one_hit(ch, victim, gsn_thwack, 1);
                        else if (x == 2)
			     {
                                one_hit(ch, victim, gsn_smack, 1);
			     }
                        else
                                one_hit(ch, victim, gsn_bash, 1);
                }
                ch->pcdata->powers[GIANT_ATTACK] = x;   // setting the attack bit.
                return;
        }
        if (dt == gsn_bladespin)
        {
                if (number_range(1, 2) == 1)
                        one_hit(ch, victim, dt, 1);
                if (number_range(1, 2) == 1)
                        one_hit(ch, victim, gsn_lightningkick, 1);
                if (number_range(1, 2) == 1)
                        one_hit(ch, victim, gsn_knee, 1);
                if (number_range(1, 2) == 1)
                        one_hit(ch, victim, gsn_lightningslash, 1);
                if ((ch->wpn[0] >= 1000) && (ch->wpn[3] >= 1000)
                    && (ch->wpn[6] >= 1000) && (ch->wpn[9] >= 1000)
                    && (ch->wpn[1] >= 1000) && (ch->wpn[4] >= 1000)
                    && (ch->wpn[7] >= 1000)
                    && (ch->wpn[10] >= 1000) && (ch->wpn[2] >= 1000)
                    && (ch->wpn[5] >= 1000) && (ch->wpn[8] >= 1000)
                    && (ch->wpn[11] >= 1000) && (ch->wpn[12] >= 1000))
                {
                        if (number_range(1, 2) == 1)
                                one_hit(ch, victim, dt, 1);
                        if (number_range(1, 2) == 1)
                                one_hit(ch, victim, gsn_lightningkick, 1);
                        if (number_range(1, 2) == 1)
                                one_hit(ch, victim, gsn_knee, 1);
                        if (number_range(1, 2) == 1)
                                one_hit(ch, victim, gsn_lightningslash, 1);
                }
                return;
        }
        wield1 = get_eq_char(ch, WEAR_WIELD);
        wield2 = get_eq_char(ch, WEAR_HOLD);
        wield3 = get_eq_char(ch, WEAR_THIRD);
        wield4 = get_eq_char(ch, WEAR_FOURTH);
        if (wield1 != NULL && wield1->item_type == ITEM_WEAPON)
                wieldorig = 1;
        if (wield2 != NULL && wield2->item_type == ITEM_WEAPON)
                wieldorig += 2;
        if (wield3 != NULL && wield3->item_type == ITEM_WEAPON)
                wieldorig += 4;
        if (wield4 != NULL && wield4->item_type == ITEM_WEAPON)
                wieldorig += 8;
        wieldtype = wieldorig;
        wieldie = number_range(1, 4);
        if (wieldorig == 15)
        {
                if (wieldie == 2)
                        wieldtype = 8;
                else
                        wieldtype = 4;
        }
        else if (wieldorig == 14)
        {
                if (wieldie == 2)
                        wieldtype = 8;
                else if (wieldie == 1)
                        wieldtype = 4;
                else if (wieldie == 3)
                        wieldtype = 2;
                else
                        wieldtype = 1;
        }
        else if (wieldorig == 13)
        {
                if (wieldie == 1)
                        wieldtype = 8;
                else if (wieldie == 2)
                        wieldtype = 4;
                else
                        wieldtype = 1;
        }
        else if (wieldorig == 12)
        {
                if (wieldie == 1)
                        wieldtype = 8;
                else
                        wieldtype = 4;
        }
        else if (wieldorig == 11)
        {
                if (wieldie == 1)
                        wieldtype = 8;
                if (wieldie == 2)
                        wieldtype = 2;
                else
                        wieldtype = 1;
        }
        else if (wieldorig == 10)
        {
                if (wieldie == 1)
                        wieldtype = 8;
                else
                        wieldtype = 2;
        }
        else if (wieldorig == 9)
        {
                if (wieldie == 1)
                        wieldtype = 8;
                else
                        wieldtype = 1;
        }
        else if (wieldorig == 8)
                wieldtype = 8;
        else if (wieldorig == 7)
        {
                if (wieldie == 1)
                        wieldtype = 4;
                else if (wieldie == 2)
                        wieldtype = 2;
                else
                        wieldtype = 1;
        }
        else if (wieldorig == 6)
        {
                if (wieldie == 1 || wieldie == 2)
                        wieldtype = 2;
                else
                        wieldtype = 4;
        }
        else if (wieldorig == 5)
        {
                if (wieldie == 1)
                        wieldtype = 4;
                else
                        wieldtype = 1;
        }
        else if (wieldorig == 4)
                wieldtype = 4;
        else if (wieldorig == 3)
        {
                if (wieldie == 2 || wieldie == 4)
                        wieldtype = 2;
                else
                        wieldtype = 1;
        }
        else if (wieldorig == 2)
                wieldtype = 2;
        else if (wieldorig == 1)
                wieldtype = 1;

        if (wieldtype == 8)
                wield = wield4;
        else if (wieldtype == 4)
                wield = wield3;
        else if (wieldtype == 2)
                wield = wield2;
        else
                wield = wield1;

        if (!IS_NPC(ch) && ch->stance[0] > 0 && number_percent() == 5)
        {
                int       stance = ch->stance[0];

                if (ch->stance[stance] >= 200)
                {
                        special_move(ch, victim);
                        return;
                }
        }

        unarmed = 0;
        if (IS_SET(ch->flag2, VAMP_OBJMASK))
        {
                act("You return to your normal form.", ch, NULL, NULL,
                    TO_CHAR);
                xprintf(buf, "%s reforms as %s.\n\r", ch->morph, ch->name);
                act(buf, ch, NULL, NULL, TO_ROOM);
                free_string(ch->morph);
                free_string(ch->objdesc);
                ch->long_descr = str_dup("");
                REMOVE_BIT(ch->flag2, VAMP_OBJMASK);
                REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        }
        one_hit(ch, victim, dt, wieldtype);

        if (victim == NULL || victim->position != POS_FIGHTING)
                return;

        /*
         * Only want one spell per round from spell weapons...otherwise it's 
         * * too powerful, and would unbalance player killing (as this is a PK mud).
         */

        if (dt == TYPE_UNDEFINED)
        {
                dt = TYPE_HIT;
                if (wield != NULL && wield->item_type == ITEM_WEAPON)
                {
                        dt += wield->value[3];
                        if (wield->value[0] >= 1)
                        {
                                /*
                                 * Look, just don't ask...   KaVir 
                                 */
                                if (wield->value[0] >= 1000)
                                        sn = wield->value[0] -
                                                ((wield->value[0] / 1000) *
                                                 1000);
                                else
                                        sn = wield->value[0];
                                if (sn != skill_lookup("gas breath")
                                    && sn != skill_lookup("desanct")
                                    && sn != skill_lookup("faith armor")
                                    && sn != 0)
                                {
                                        if (victim->position == POS_FIGHTING)
                                                (*skill_table[sn].
                                                 spell_fun) (sn, wield->level,
                                                             ch, victim);
                                }
                        }
                }
        }
        if (victim == NULL || victim->position != POS_FIGHTING)
                return;
        if (ch->fighting != victim || dt == gsn_backstab || dt == gsn_shiroken
            || dt == gsn_garotte || dt == gsn_headbutt || dt == gsn_circle 
	    || dt == gsn_retribution)
                return;
        maxcount = number_attacks(ch, victim);
        if (!IS_NPC(ch))
        {
                if (wield != NULL && wield->item_type == ITEM_WEAPON)
                {
                        int       tempnum = wield->value[3];
                        chance = (ch->wpn[tempnum]) * 0.5;
                }
                else chance = (ch->wpn[0]) * 0.5;
                if (number_percent() <= chance) maxcount += 1;
        }
        if (wieldorig == 3) maxcount += 1;
        for (countup = 0; countup <= maxcount; countup++)
        {
                wieldtype = wieldorig;
                if (wieldorig == 15)
                {
                        if (wieldie == 2)
                                wieldtype = 8;
                        else
                                wieldtype = 4;
                }
                else if (wieldorig == 14)
                {
                        if (wieldie == 2)
                                wieldtype = 8;
                        else if (wieldie == 1)
                                wieldtype = 4;
                        else if (wieldie == 3)
                                wieldtype = 2;
                        else
                                wieldtype = 1;
                }
                else if (wieldorig == 13)
                {
                        if (wieldie == 1)
                                wieldtype = 8;
                        else if (wieldie == 2)
                                wieldtype = 4;
                        else
                                wieldtype = 1;
                }
                else if (wieldorig == 12)
                {
                        if (wieldie == 1)
                                wieldtype = 8;
                        else
                                wieldtype = 4;
                }
                else if (wieldorig == 11)
                {
                        if (wieldie == 1)
                                wieldtype = 8;
                        else if (wieldie == 2)
                                wieldtype = 2;
                        else
                                wieldtype = 1;
                }
                else if (wieldorig == 10)
                {
                        if (wieldie == 1)
                                wieldtype = 8;
                        else
                                wieldtype = 2;
                }
                else if (wieldorig == 9)
                {
                        if (wieldie == 1)
                                wieldtype = 8;
                        else
                                wieldtype = 1;
                }
                else if (wieldorig == 7)
                {
                        if (wieldie == 1)
                                wieldtype = 4;
                        else if (wieldie == 2)
                                wieldtype = 2;
                        else
                                wieldtype = 1;
                }
                else if (wieldorig == 6)
                {
                        if (wieldie == 1 || wieldie == 2)
                                wieldtype = 2;
                        else
                                wieldtype = 4;
                }
                else if (wieldorig == 5)
                {
                        if (wieldie == 1)
                                wieldtype = 4;
                        else
                                wieldtype = 1;
                }
                else if (wieldorig == 3)
                {
                        if (wieldie == 2 || wieldie == 4)
                                wieldtype = 2;
                        else
                                wieldtype = 1;
                }
                one_hit(ch, victim, -1, wieldtype);
                if (victim == NULL || victim->position != POS_FIGHTING)
                        return;
                if (ch->fighting != victim)
                        return;
        }

        if (!IS_NPC(ch) && !IS_NPC(victim) && ch->race > 24) // pkpower
        {
        	i = number_range(1, 10);
                if (i < 5)
                	one_hit(ch, victim, gsn_supreme, 0);
                else if (i < 7)
                        one_hit(ch, victim, gsn_supreme, 1);
        }
        if (!IS_NPC(ch))
        {
	    if (IS_VAMPAFF(ch, VAM_FANGS))
	    {
                if (!IS_NPC(victim))
                {
                        one_hit(ch, victim, (TYPE_HIT + 10), 0);
                        one_hit(ch, victim, (TYPE_HIT + 10), 0);
                }
                else
                {
                        one_hit(ch, victim, (TYPE_HIT + 10), 1);
                        one_hit(ch, victim, (TYPE_HIT + 10), 1);
                }
	    }
            if (ch->level > 2)
	    {
		if (IS_VAMPAFF(ch, VAM_HORNS) && number_range(1, 3) == 1)
                    multi_hit(ch, victim, gsn_headbutt);
		if (IS_VAMPAFF(ch, VAM_WINGS) && number_range(1, 2) == 2)
		    multi_hit(ch, victim, gsn_buffet);
                if (IS_VAMPAFF(ch, VAM_WINGS) && number_range(1, 2) == 1)
		    multi_hit(ch, victim, gsn_buffet);
	    }
            if (IS_VAMPAFF(ch, VAM_TAIL) && number_range(1, 2) == 1)
                multi_hit(ch, victim, gsn_sweep);
            if (IS_VAMPAFF(ch, VAM_HEAD) && number_range(1, 2) == 1)
                multi_hit(ch, victim, gsn_fangs);
        }

        if (IS_CLASS(ch, CLASS_SHADOW))
        {
                if (IS_SET
                    (ch->pcdata->powers[SHADOW_BITS], NPOWER_KNIFESHIELD))
                {
                        one_hit(ch, victim, gsn_knifespin, 1);
                        one_hit(ch, victim, gsn_knifespin, 1);
                }
                if (IS_SET(ch->pcdata->powers[SHADOW_BITS], NPOWER_BLOODRAGE))
                        one_hit(ch, victim, gsn_gutcutter, 1);
        }
        else if (IS_CLASS(ch, CLASS_DRACONIAN) && number_range(1, 3) == 1)
        {
                if (IS_NPC(victim))
                {
                        one_hit(ch, victim, gsn_spiket, 0);
                        one_hit(ch, victim, gsn_spiket, 1);
                }
                else
                        one_hit(ch, victim, gsn_spiket, 0);
        }
        else if (IS_CLASS(ch, CLASS_DEMON))
        {
	    if (IS_SET(ch->warp, WARP_SPIKETAIL) && number_range(1, 3) == 1)
	    {
                if (IS_NPC(victim))
                {
                        one_hit(ch, victim, gsn_spiket, 0);
                        one_hit(ch, victim, gsn_spiket, 1);
                }
                else
                        one_hit(ch, victim, gsn_spiket, 0);
	    }
            if (IS_SET(ch->warp, WARP_SHARDS) && number_range(1, 3) == 1)
                one_hit(ch, victim, gsn_shards, 0);
            if (IS_SET(ch->warp, WARP_MAGMA) && number_range(1, 3) == 1)
            {
                if (IS_NPC(victim))
                {
                        one_hit(ch, victim, gsn_magma, 0);
                        one_hit(ch, victim, gsn_magma, 1);
                }
                else
                        one_hit(ch, victim, gsn_magma, 0);
            }
            if (IS_SET(ch->warp, WARP_VENOMTONG) && number_range(1, 3) == 1)
                one_hit(ch, victim, gsn_venomt, 0);
            if (ch->power[DISC_DAEM_HELL] > 3 && number_range(1, 3) == 1)
                one_hit(ch, victim, gsn_hellfire, 0);
            if (IS_DEMAFF(ch, DEM_HORNS) && number_range(1, 3) == 1)
                one_hit(ch, victim, gsn_headbutt, 0);
            if (IS_DEMAFF(ch, DEM_TAIL) && number_range(1, 2) == 2)
                one_hit(ch, victim, gsn_sweep, 0);
	}
        //Ghoul Disease
        else if (IS_CLASS(ch, CLASS_GHOUL) && ch->pcdata->powers[GPOWER_ZOMBIE] >= 2)
        {
                char      buf1[MAX_STRING_LENGTH];
                char      buf2[MAX_STRING_LENGTH];
                int       rand_num = number_range(1, 4);
                int       dam = ch->damcap[DAM_CAP] / 2;

                dam += ch->pcdata->powers[GPOWER_ZOMBIE] * 500;

                if (rand_num > 2)
                {
                        xprintf(buf1,
                                "#G$N #ccringes in pain as your diseased claws rend their flesh!  #g[#G%d#g]#n",
                                dam);
                        xprintf(buf2,
                                "#cYou cringe in pain as #G$n's #cdiseased claws rend your flesh!  #g[#G%d#g]#n",
                                dam);
                        act(buf1, ch, NULL, victim, TO_CHAR);
                        act(buf2, ch, NULL, victim, TO_VICT);

                        hurt_person(ch, victim, dam);
                }
        }
        //Thief Critical Strike
        else if (IS_CLASS(ch, CLASS_THIEF)
            && IS_SET(ch->pcdata->powers[1], THIEF_CRITICALSTRIKE)
            && wield1 != NULL && wield1->value[3] == 11)
        {
                char      buf1[MAX_STRING_LENGTH];
                char      buf2[MAX_STRING_LENGTH];
                int       rand_num = number_range(1, 6);
                int       dam = number_range(ch->damcap[DAM_CAP] * 1.5,
                                             ch->damcap[DAM_CAP] * 2);

                if (rand_num >= 5)
                {
                        xprintf(buf1,
                                "#0You #Rdeftly lunge #0with your #7dagger #0and score a #rC#Rritical #rH#Rit#n #0on #r$N!  #r[#R%d#r]#n",
                                dam);
                        xprintf(buf2,
                                "#r$n #Rdeftly lunges #0at you, #Rslicing #0you in a vital area and scoring a #rC#Rritical #rH#Rit! #r[#R%d#r]#n",
                                dam);
                        act(buf1, ch, NULL, victim, TO_CHAR);
                        act(buf2, ch, NULL, victim, TO_VICT);
                        hurt_person(ch, victim, dam);
                }
        }
        else if (IS_CLASS(ch, CLASS_SKYBLADE)
            && ch->pcdata->powers[SKYBLADE_SKILLS] >= 4)
        {
                char      buf1[MAX_STRING_LENGTH];
                char      buf2[MAX_STRING_LENGTH];
                int       rand_num = number_range(1, 4);
                int       dam = number_range(ch->damcap[DAM_CAP] / 4,
                                             ch->damcap[DAM_CAP]);

                if (rand_num == 3)
                {
                        do_say(ch, "Haha, you swine, take this!");
                        xprintf(buf1,
                                "#B$n sidesteps $N#B's jab and strikes back!#0[#R%d#0]#n",
                                dam);
                        xprintf(buf2,
                                "#BYou sidestep $N#B's jab and strike back!#0[#R%d#0]#n",
                                dam);
                        act(buf1, ch, NULL, victim, TO_VICT);
                        act(buf2, ch, NULL, victim, TO_CHAR);
                        victim->hit -= dam;
                }
        }
        else if (IS_CLASS(ch, CLASS_DRONE))
        {
	    if (number_range(1, 3) != 2)
	    {
                if (ch->pcdata->powers[DRONE_EVASION] > 2
                    && ch->pcdata->powers[DRONE_FOCUS] > 2
                    && ch->pcdata->powers[DRONE_FORTITUDE] > 2)
                        multi_hit(ch, victim, gsn_tentacle);
	    }
        }
        else if (IS_CLASS(ch, CLASS_HOBBIT))
        {
                if (ch->pcdata->powers[HOBBIT_DISHES] > 5)
                        one_hit(ch, victim, gsn_thrownpie, 1);
                if (ch->pcdata->powers[HOBBIT_DISHES] > 10)
                        one_hit(ch, victim, gsn_thrownpie, 1);
                if (ch->pcdata->powers[HOBBIT_DISHES] > 15)
                        one_hit(ch, victim, gsn_thrownpie, 1);
        }
        else if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
        {
                if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
                {
                        one_hit(ch, victim, gsn_fangs, 1);
                        if (ch->pcdata->powers[HYDRA_LEVEL] > 0)
                                one_hit(ch, victim, gsn_fangs, 1);
                        if (ch->pcdata->powers[HYDRA_LEVEL] > 1)
                                one_hit(ch, victim, gsn_fangs, 1);
                        if (ch->pcdata->powers[HYDRA_LEVEL] > 3)
                                one_hit(ch, victim, gsn_fangs, 1);
                        if (ch->pcdata->powers[HYDRA_LEVEL] > 4)
                                one_hit(ch, victim, gsn_fangs, 1);
                }
                else if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
                {
                        if (ch->pcdata->powers[TIGER_LEVEL] > 0)
                                multi_hit(ch, victim, gsn_claws);
                        if (ch->pcdata->powers[TIGER_LEVEL] > 1)
                                multi_hit(ch, victim, gsn_fangs);
                }
                else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
                {
                        if (ch->pcdata->powers[BULL_LEVEL] > 0)
                                multi_hit(ch, victim, gsn_headbutt);
                        if (ch->pcdata->powers[BULL_LEVEL] > 1)
                                multi_hit(ch, victim, gsn_hooves);
                }
                else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
                {
                        if (IS_NPC(victim))
                        {
                                one_hit(ch, victim, gsn_fireball, 1);
                                one_hit(ch, victim, gsn_fireball, 1);
                        }
                        else
                        {
                                one_hit(ch, victim, gsn_fireball, 0);
                                one_hit(ch, victim, gsn_fireball, 0);
                        }
                        if (ch->pcdata->powers[FAERIE_LEVEL] > 1)
                                one_hit(ch, victim, gsn_buffet, 1);
                }
        }
        else if (IS_CLASS(ch, CLASS_TANARRI))
        {
                if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FANGS))
                {
                        one_hit(ch, victim, gsn_fangs, 1);
                        if (IS_SET
                            (ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD))
                                one_hit(ch, victim, gsn_fangs, 1);
                }
                if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FIERY))
                {
                        one_hit(ch, victim, gsn_fiery, 1);
                        if (IS_CLASS(victim, CLASS_ANGEL))
                                one_hit(ch, victim, gsn_fiery, 1);
                }
        }
if (IS_CLASS(ch, CLASS_ELEMENTAL) && IS_SET(ch->pcdata->powers[ELEM_FORMS], FIRE_FORM))
{
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int dam;

    dam = (number_range(1500,2500));
    if (!IS_SET(ch->act, PLR_BRIEF5))
    {
      printf(buf1, "#0$N! is completly engulfed with bright #7white #RF#Yl#Ra#Ym#Re#Ys#n #0[#R%d#0]#n", dam);
      printf(buf2, "#0Fire from $n's strikes you!  #0[#R%d#0]#n",dam);
      act(buf1, ch, NULL, victim, TO_CHAR);
      act(buf2, ch, NULL, victim, TO_VICT);
    }
      hurt_person(ch, victim, dam);
 }

if (IS_CLASS(ch, CLASS_ELEMENTAL) && IS_SET(ch->pcdata->powers[ELEM_FORMS], WATER_FORM))
{
   if (number_percent() > 94 && !IS_NPC(victim))
     {
      printf(buf1, "#0The Power of your #lwater#0 form stuns $N!#n");
      printf(buf2, "#0The Power of $n's #lWater#0 form stuns you!#n");
      act(buf1, ch, NULL, victim, TO_CHAR);
      act(buf2, ch, NULL, victim, TO_VICT);
      victim->position = POS_STUNNED;
     }
 }

if (IS_CLASS(ch, CLASS_ELEMENTAL) && IS_SET(ch->pcdata->powers[ELEM_FORMS], WATER_FORM))
{
   if (number_percent() > 90 && !IS_NPC(victim))
     {
      printf(buf1, "#0The Power of your #lwater#0 form stuns $N!#n");
      printf(buf2, "#0The Power of $n's #lWater#0 form stuns you!#n");
      act(buf1, ch, NULL, victim, TO_CHAR);
      act(buf2, ch, NULL, victim, TO_VICT);
      victim->position = POS_STUNNED;
     }
 }

        else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
        {
	    if (ch->pcdata->powers[WEAPONSKILL] > 8)
	    {
                one_hit(ch, victim, gsn_lightningslash, 1);
                one_hit(ch, victim, gsn_lightningslash, 1);
                one_hit(ch, victim, gsn_lightningslash, 1);
                one_hit(ch, victim, gsn_lightningslash, 1);
	    }
        }
        else if (IS_CLASS(ch, CLASS_BLADE_MASTER))
        {
            if (ch->pcdata->powers[WEAPONSKILL] > 8)
            {
                one_hit(ch, victim, gsn_lightningslash, 1);
                one_hit(ch, victim, gsn_lightningslash, 1);
            }
        }
	else if (IS_CLASS(ch, CLASS_BLADE_MASTER) && IS_SET(ch->newbits2, NEW2_AUTOATTACK))
	{
	one_hit(ch,victim,gsn_lightningslash,1);
	one_hit(ch,victim,gsn_lightningslash,1);
	}
        else if (IS_CLASS(ch, CLASS_LICH))
        {
                if (ch->pcdata->powers[CON_LORE] > 4)
                        one_hit(ch, victim, gsn_fireball, 1);
                if (ch->pcdata->powers[NECROMANTIC] > 4)
                        one_hit(ch, victim, gsn_chillhand, 1);
                if (ch->pcdata->powers[DEATH_LORE] > 4)
                        one_hit(ch, victim, gsn_deathaura, 1);
        }
        else if (IS_CLASS(ch, CLASS_NINJA))
	{
		if (ch->pcdata->powers[NPOWER_NINGENNO] >= 5
		    && number_range(1, 2) != 1)
            		multi_hit(ch, victim, gsn_shiroken);
	}
	else if (IS_CLASS(ch, CLASS_WEREWOLF))
	{
            if (IS_SET(ch->newbits, NEW_QUILLS))
                multi_hit(ch, victim, gsn_quills);
            if (IS_SET(ch->newbits, NEW_SLAM)
                && number_range(1, 5 - ch->power[DISC_WERE_BEAR] / 3) == 1)
                multi_hit(ch, victim, gsn_cheapshot);
	}
	else if (IS_CLASS(ch, CLASS_SAMURAI))
	{
            if (IS_SET(ch->newbits, NEW_BLADESPIN))
                multi_hit(ch, victim, gsn_bladespin);
	}
        else if (IS_CLASS(ch, CLASS_GIANT))
	{
                multi_hit(ch, victim, gsn_bash);
	}
        else if (IS_CLASS(ch, CLASS_DROW))
	{
	    if (IS_SET(ch->newbits, NEW_DARKTENDRILS))
                multi_hit(ch, victim, gsn_darktendrils);
	}
        else if (IS_CLASS(ch, CLASS_MAGE))
	{
	    if (IS_SET(ch->newbits, NEW_MULTIARMS))
                multi_hit(ch, victim, gsn_mageshield);
	}
        else if (IS_CLASS(ch, CLASS_MONK))
	{
	    if (ch->monkab[BODY] > 3)
                multi_hit(ch, victim, gsn_heavenlyaura);
	}
        else if (IS_CLASS(ch, CLASS_ANGEL))
        {
                if (IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_AURA))
                        multi_hit(ch, victim, gsn_heavenlyaura);
                if (IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_HALO))
                {
                    switch (number_range(1, 5))
                    {
                        default: break;
                        case 1:
                            if ((sn = skill_lookup("curse")) > 0)
                                    (*skill_table[sn].spell_fun) (sn, 50, ch, victim);
                            break;
                        case 2:
                            if ((sn = skill_lookup("imp heal")) > 0)
                                        (*skill_table[sn].spell_fun) (sn, 50, ch, ch);
                            break;
                        case 3:
                            if ((sn = skill_lookup("imp fireball")) > 0)
                                        (*skill_table[sn].spell_fun) (sn, 50, ch, victim);
                            break;
                        case 4:
                            if ((sn = skill_lookup("godbless")) > 0)
                                        (*skill_table[sn].spell_fun) (sn, 50, ch, ch);
                            break;
                    }
                }
        }
        else if (IS_CLASS(ch, CLASS_DROID))
        {
                one_hit(ch, victim, gsn_pincers, 1);
                one_hit(ch, victim, gsn_pincers, 1);
                if (IS_SET(ch->newbits, NEW_CUBEFORM))
                {
                        one_hit(ch, victim, gsn_stinger, 1);
                        one_hit(ch, victim, gsn_stinger, 1);
                }
        }
        else if (IS_CLASS(ch, CLASS_CYBORG))
	{
	    if (IS_SET(ch->newbits, NEW_CUBEFORM))
	    {
                one_hit(ch, victim, gsn_stuntubes, 1);
                multi_hit(ch, victim, gsn_stinger);
	    }
        }
        if (IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_WINGS)
            && number_range(1, 3) == 1)
                one_hit(ch, victim, gsn_buffet, 0);
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF)
            && ch->power[DISC_WERE_RAPT] > 0)
                multi_hit(ch, victim, gsn_rfangs);

        /*
         * Familiars Attack Round
         */
        if ((familiar = get_eq_char(ch, WEAR_FAMILIAR)) != NULL
            && familiar->item_type == ITEM_FAMILIAR && familiar->value[2] > 0)
        {
                char      buf1[MSL];
                char      buf2[MSL];
                int       dam;
                int       fart;
                int       attacks;

                if (familiar->value[0] != 9)
                {
                        if (familiar->value[1] < 33)
                                attacks = 1;
                        else if (familiar->value[1] >= 33
                                 && familiar->value[1] < 99)
                                attacks = 2;
                        else if (familiar->value[1] >= 99)
                                attacks = 3;
                        else
                                (attacks = 1);
                }
                else
                {
                        if (familiar->value[1] < 33)
                                attacks = 2;
                        else if (familiar->value[1] >= 33
                                 && familiar->value[1] < 99)
                                attacks = 3;
                        else if (familiar->value[1] >= 99)
                                attacks = 4;
                        else
                                (attacks = 2);
                }
                for (fart = 0; fart < attacks; fart++)
                {
                        /*
                         * A hit
                         */
                        if (victim->position != POS_DEAD && number_percent() < (familiar->value[4]))
                        {
                                dam = familiar->value[2];
                                dam += number_range(0, 250);
                                dam += 5 * familiar->value[1];
                                sprintf(buf1, familiar->hit_msg1, dam);
                                sprintf(buf2, familiar->hit_msg2, dam);

                                act(buf1, ch, familiar, victim, TO_CHAR);
                                act(buf2, ch, familiar, victim, TO_VICT);

            if ( victim->position != POS_DEAD )  hurt_person(ch, victim, dam);
                        }
                        /*
                         * A miss
                         */
                        else
                        {
                                if (victim->position != POS_DEAD && (IS_NPC(ch)
                                    || !IS_SET(ch->act, PLR_BRIEF3)))
                                        act("$p's #7attack misses $N!#n", ch,
                                            familiar, victim, TO_CHAR);
                                if (IS_NPC(victim) 
                                    || !IS_SET(victim->act, PLR_BRIEF3))
                                        act("$p's #7attack misses you!#n", ch,
                                            familiar, victim, TO_VICT);
                        }
                }

        }


        if (wield1 != NULL)
        {
	    if (wield->item_type == ITEM_WEAPON)
	    {
                char      buf1[MAX_STRING_LENGTH];
                char      buf2[MAX_STRING_LENGTH];
                int       dam;
		int	  fire = wield->fire_level;
		int 	  water = wield->water_level;
		int	  electric = wield->lightning_level;
		int	  earth = wield->earth_level;
                
		if (fire > 0)
		{
			dam = (number_range(500,1000) * fire);
			if (!IS_SET(ch->act, PLR_BRIEF5))
			{
                	    xprintf(buf1, "#0Fire from your enflamed weapon strikes out at $N!  #0[#R%d#0]#n", dam);
                	    xprintf(buf2, "#0Fire from $n's enflamed weapon strikes you!  #0[#R%d#0]#n",dam);
                     	    act(buf1, ch, NULL, victim, TO_CHAR);
                	    act(buf2, ch, NULL, victim, TO_VICT);
			}
                	hurt_person(ch, victim, dam);
		}
                if (water > 0)
                {
                    dam = (number_range(500,1000) * water);
                    if (!IS_SET(ch->act, PLR_BRIEF5))
                    {
                    	xprintf(buf1, "#0Ice from your frozen weapon strikes out at $N!  #0[#R%d#0]#n", dam);
                    	xprintf(buf2, "#0Ice from $n's frozen weapon strikes you!  #0[#R%d#0]#n", dam);
                    	act(buf1, ch, NULL, victim, TO_CHAR);
                    	act(buf2, ch, NULL, victim, TO_VICT);
		    }
                    hurt_person(ch, victim, dam);
                }
                if (electric > 0)
                {
                    dam = (number_range(500,1000) * electric);
                    if (!IS_SET(ch->act, PLR_BRIEF5))
                    {
                    	xprintf(buf1,
                        "#0Lightning from your electric weapon strikes out at $N!  #0[#R%d#0]#n",
                        dam);
                    	xprintf(buf2,
                        "#0Lightning from $n's electric weapon strikes you!  #0[#R%d#0]#n",
                        dam);
                    	act(buf1, ch, NULL, victim, TO_CHAR);
                    	act(buf2, ch, NULL, victim, TO_VICT);
		    }
                    hurt_person(ch, victim, dam);
                }
                if (earth > 0)
                {
                    dam = (number_range(500,1000) * earth);
                    if (!IS_SET(ch->act, PLR_BRIEF5))
                    {

	                xprintf(buf1,
	                "#0Stones from your earthly weapon strikes out at $N!  #0[#R%d#0]#n",
			dam);
                    	xprintf(buf2,
                        "#0Stones from $n's earthly weapon strikes you!  #0[#R%d#0]#n",
                        dam);
			act(buf1, ch, NULL, victim, TO_CHAR);
                    	act(buf2, ch, NULL, victim, TO_VICT);
 		    }
                    hurt_person(ch, victim, dam);
                }
	    }
            if (IS_SET(wield1->extra_flags, ITEM_ELECENCHANT)
                || wield1->pIndexData->vnum == 42721)
	    {
                char      buf1[MAX_STRING_LENGTH];
                char      buf2[MAX_STRING_LENGTH];
                int       dam = number_range(1000, 2000);

                xprintf(buf1,
                        "#0Your weapon #oS#yH#7OC#yK#oS #0$N!  #0[#R%d#0]#n",
                        dam);
                xprintf(buf2,
                        "#0$n's weapon #oS#yH#7OC#yK#oS #0you!  #0[#R%d#0]#n",
                        dam);
                act(buf1, ch, NULL, victim, TO_CHAR);
                act(buf2, ch, NULL, victim, TO_VICT);

                hurt_person(ch, victim, dam);
	    }
            if (IS_SET(wield1->extra_flags, ITEM_FIREENCHANT)
                || wield1->pIndexData->vnum == 42722
                || wield1->pIndexData->vnum == 42723)
            {
                char      buf1[MAX_STRING_LENGTH];
                char      buf2[MAX_STRING_LENGTH];
                int       dam = number_range(1500, 2500);

                xprintf(buf1,
                        "#0Your weapon #rIN#RCI#yN#7E#yR#RAT#rES #0$N!  #0[#R%d#0]#n",
                        dam);
                xprintf(buf2,
                        "#0$n's weapon #rIN#RCI#yN#7E#yR#RAT#rES #0you!  #0[#R%d#0]#n",
                        dam);
                act(buf1, ch, NULL, victim, TO_CHAR);
                act(buf2, ch, NULL, victim, TO_VICT);

                hurt_person(ch, victim, dam);

                if (!IS_AFFECTED(victim, AFF_FLAMING))
                {
                        SET_BIT(victim->affected_by, AFF_FLAMING);
                }

            }
	    if (IS_SET(wield1->extra_flags, ITEM_ICEENCHANT))
            {
                char      buf1[MAX_STRING_LENGTH];
                char      buf2[MAX_STRING_LENGTH];
                int       dam = number_range(1200, 2200);

                xprintf(buf1,
                        "#0Your weapon #cF#CRE#7E#CZE#cS #0$N!  #0[#R%d#0]#n",
                        dam);
                xprintf(buf2,
                        "#0$n's weapon #cF#CRE#7E#CZE#cS #0you!  #0[#R%d#0]#n",
                        dam);
                act(buf1, ch, NULL, victim, TO_CHAR);
                act(buf2, ch, NULL, victim, TO_VICT);

                hurt_person(ch, victim, dam);
            }
	    if (IS_SET(wield1->quest, QUEST_WWPOISON))
            {
                char      buf1[MAX_STRING_LENGTH];
                char      buf2[MAX_STRING_LENGTH];
                int       dam = number_range(500, 1500);

                xprintf(buf1,
                        "#0Your weapon's #gp#Goiso#gn #0seeps into #R$N!  #0[#R%d#0]#n",
                        dam);
                xprintf(buf2,
                        "#R$n's #0weapon #co#C0#gP#GoI#ys#GOn#GS#C0#co #0you!  #0[#R%d#0]#n",
                        dam);
                act(buf1, ch, NULL, victim, TO_CHAR);
                act(buf2, ch, NULL, victim, TO_VICT);

                hurt_person(ch, victim, dam);
            }
            if (IS_SET(wield1->quest2, QUEST2_FLAMEBLADE))
            {
                char      buf1[MAX_STRING_LENGTH];
                char      buf2[MAX_STRING_LENGTH];
                int       dam = number_range(500, 1500);

                xprintf(buf1,
                        "#0Your weapon's #RFIRE#n #0scorches #R$N!  #0[#R%d#0]#n",
                        dam);
                xprintf(buf2,
                        "#R$n's #0weapon #RSCORCHES#n #0You!  #0[#R%d#0]#n",
                        dam);
                act(buf1, ch, NULL, victim, TO_CHAR);
                act(buf2, ch, NULL, victim, TO_VICT);

                hurt_person(ch, victim, dam);
            }

	}

        /*
         * Golems specials 
         */
        if (IS_NPC(ch))
        {
                if (ch->pIndexData->vnum == MOB_VNUM_FIRE)
                {
                        if ((sn = skill_lookup("curse")) > 0)
                                (*skill_table[sn].spell_fun) (sn, 50, ch,
                                                              victim);
                        if ((sn = skill_lookup("imp faerie fire")) > 0)
                                (*skill_table[sn].spell_fun) (sn, 50, ch,
                                                              victim);
                }
                else if (ch->pIndexData->vnum == MOB_VNUM_STONE
                    && (char_damroll(victim) > 0 || char_hitroll(victim) > 0))
                {
                        af.type = skill_lookup("reserved");
                        af.duration = 20;
                        af.location = APPLY_DAMROLL;
                        af.modifier = -50;
                        af.bitvector = 0;
                        affect_to_char(victim, &af);

                        af.type = skill_lookup("reserved");
                        af.duration = 20;
                        af.location = APPLY_HITROLL;
                        af.modifier = -50;
                        af.bitvector = 0;
                        affect_to_char(victim, &af);
                        send_to_char
                                ("You feel weak in the presence of the stone golem.\n\r",
                                 victim);
                }
                else if (ch->pIndexData->vnum == MOB_VNUM_CLAY)
                {
                        if ((sn = skill_lookup("clay")) > 0)
                                (*skill_table[sn].spell_fun) (sn, 50, ch,
                                                              victim);
                }
                else if (ch->pIndexData->vnum == MOB_VNUM_IRON)
                {
                        if ((sn = skill_lookup("group heal")) > 0)
                                (*skill_table[sn].spell_fun) (sn, 50, ch,
                                                              victim);
                }
        }

        if (victim == NULL || victim->position != POS_FIGHTING)
                return;

        if (!IS_NPC(ch))
	{
	    if (IS_CLASS(ch, CLASS_VAMPIRE) && IS_VAMPAFF(ch, VAM_SERPENTIS))
                spell_poison(gsn_poison, (ch->level * number_range(5, 20)),
                             ch, victim);
            else if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_SPID] > 0)
                spell_poison(gsn_poison, (ch->level * number_range(5, 15)),
                             ch, victim);
 	    else if (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON))
                spell_poison(gsn_poison, (ch->level * number_range(10, 20)),
                             ch, victim);
	    else if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_NINGENNO] >= 5)
                spell_poison(gsn_poison, (ch->level * number_range(5, 10)),
                             ch, victim);
	}

        if (victim->itemaffect < 1) return;

        if (IS_CLASS(victim, CLASS_DRACONIAN) && victim->pcdata->stats[DRACONIAN_SPIKES] > 0)
        {
                char      buf1[MAX_STRING_LENGTH];
                char      buf2[MAX_STRING_LENGTH];
                int       dam =
                        number_range(victim->pcdata->stats[DRACONIAN_SPIKES] *
                                     10,
                                     victim->pcdata->stats[DRACONIAN_SPIKES] *
                                     20);
                if (number_percent() > 70)
                {
                        xprintf(buf1,
                                "#Y$N's #Gspikes come in contact with you, piercing your flesh!  #r[#R%d#r]#n",
                                dam);
                        xprintf(buf2,
                                "#GYour spikes come in contact with #Y$n, #Gpiercing their flesh! #r[#R%d#r]#n",
                                dam);
                        act(buf1, ch, NULL, victim, TO_CHAR);
                        act(buf2, ch, NULL, victim, TO_VICT);

                        hurt_person(victim, ch, dam);
                }
        }

        if (IS_SET(victim->newbits, NEW_MONKFLAME) && !IS_AFFECTED(ch, AFF_FLAMING))
        {
                if (number_percent() > 93)
                {
                        SET_BIT(ch->affected_by, AFF_FLAMING);
                        if (!IS_CLASS(ch, CLASS_WEREWOLF))
                        {
                                act("Your flaming hands catch $n on fire!",
                                    ch, NULL, victim, TO_VICT);
                                act("$N's flaming hands catch $n on fire!",
                                    ch, NULL, victim, TO_NOTVICT);
                                act("$N's flaming hands catch you on fire!.",
                                    ch, NULL, victim, TO_CHAR);
                        }
                        else
                        {
                                act("Your flaming claws catch $n on fire!",
                                    ch, NULL, victim, TO_VICT);
                                act("$N's flaming claws catch $n on fire!",
                                    ch, NULL, victim, TO_NOTVICT);
                                act("$N's flaming claws catch you on fire!.",
                                    ch, NULL, victim, TO_CHAR);
                        }
                }
                else
                        return;
        }

	  if (!IS_NPC(victim) && IS_SET(victim->pcdata->powers[PALADIN_EFFECTS], PAL_AURA)
	  && IS_CLASS(victim, CLASS_PALADIN))
	  {
	    while (number_range(1,2) != 1)
	    one_hit(victim,ch,gsn_holyaura,0);
            while (number_range(1,2) != 1)
	    one_hit(victim,ch,gsn_holyaura,0);
            while (number_range(1,2) != 1)
	    one_hit(victim,ch,gsn_holyaura,0);
	  }


        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
        {
                if (IS_SET(victim->pcdata->powers[AURAS], DEATH_AURA))
                {
                        while (number_range(1, 2) != 1)
                                one_hit(victim, ch, gsn_deathaura, 0);
                        while (number_range(1, 2) != 1)
                                one_hit(victim, ch, gsn_deathaura, 0);
                        while (number_range(1, 2) != 1)
                                one_hit(victim, ch, gsn_deathaura, 0);
                }
                if (IS_SET(victim->pcdata->powers[AURAS], FEAR_AURA)
                    && (char_damroll(ch) > 0 || char_hitroll(ch) > 0))
                {
                        af.type = skill_lookup("reserved");
                        af.duration = 20;
                        af.location = APPLY_DAMROLL;
                        af.modifier = -20;
                        af.bitvector = 0;
                        affect_to_char(ch, &af);

                        af.type = skill_lookup("reserved");
                        af.duration = 20;
                        af.location = APPLY_HITROLL;
                        af.modifier = -20;
                        af.bitvector = 0;
                        affect_to_char(ch, &af);
                        send_to_char("You piss your pants out of fear.\n\r",
                                     ch);
                }
        }

        if (IS_ITEMAFF(victim, ITEMA_TATTOO) && ch->position == POS_FIGHTING)
        {
                int       tdam;

                tdam = number_range(500, 1500);
                if (IS_NPC(ch))
                        tdam *= 3;
                if (tdam > victim->damcap[DAM_CAP])
                        tdam = victim->damcap[DAM_CAP];
                if (tdam > ch->hit - 1)
                        tdam = ch->hit - 1;
                if (tdam < 0)
                        tdam = 0;
                damage(victim, ch, tdam, gsn_tattoo);
        }

	if (ch->position == POS_FIGHTING)
	{
	    if (IS_NPC(victim) || victim->spl[1] < 4)
                level = victim->level;
	    else
                level = (victim->spl[1] * 0.25);

            if (IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD))
                if ((sn = skill_lookup("lightning bolt")) > 0)
                    (*skill_table[sn].spell_fun) (sn, level, victim, ch);
            if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD))
                if ((sn = skill_lookup("fireball")) > 0)
                    (*skill_table[sn].spell_fun) (sn, level, victim, ch);
            if (IS_ITEMAFF(victim, ITEMA_ICESHIELD))
                if ((sn = skill_lookup("chill touch")) > 0)
                    (*skill_table[sn].spell_fun) (sn, level, victim, ch);
            if (IS_ITEMAFF(victim, ITEMA_ACIDSHIELD))
                if ((sn = skill_lookup("acid blast")) > 0)
                    (*skill_table[sn].spell_fun) (sn, level, victim, ch);
            if (IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD))
                if ((sn = skill_lookup("chaos blast")) > 0)
                    (*skill_table[sn].spell_fun) (sn, level, victim, ch);
	}

        return;
}

int number_attacks(CHAR_DATA * ch, CHAR_DATA * victim)
{
        int       count = 1;
        OBJ_DATA *wield;

        if (IS_NPC(ch))
        {
                if (ch->level >= 50)
                        count += 1;
                if (ch->level >= 100)
                        count += 1;
                if (ch->level >= 500)
                        count += 1;
                if (ch->level >= 1000)
                        count += 1;
                if (ch->level >= 1500)
                        count += 1;
                if (ch->level >= 2000)
                        count += 2;
                if (ch->pIndexData->extra_attack > 20)
                        count += 20;
                else
                        count += ch->pIndexData->extra_attack;
                return count;
        }

	// NPC ch is done, no need for any more checks for this

        if (IS_NPC(victim))
        {
                if (IS_STANCE(ch, STANCE_VIPER)
                    && number_percent() < ch->stance[STANCE_VIPER] * 0.5)
                        count += 1;
                else if (IS_STANCE(ch, STANCE_HAWK)
                         && number_percent() <
                         ch->stance[STANCE_HAWK] * 0.5)
                        count += 1;
                else if (IS_STANCE(ch, STANCE_COUGAR)
                         && number_percent() < ch->stance[STANCE_COUGAR] * 0.5)
                        count += 1;
                else if (IS_STANCE(ch, STANCE_WOLF)
                         && number_percent() < ch->stance[STANCE_WOLF] * 0.5)
                        count += 2;
                else if (ch->stance[0] > 12
                         && IS_SET(ch->stance[(ch->stance[0] + 6)],
                                   STANCEPOWER_SPEED)
                         && number_percent() <
                         ch->stance[(ch->stance[0])] * 0.5)
                        count += 2;
		if ((wield = get_eq_char(ch, WEAR_WIELD)) != NULL
           	    && wield->item_type == ITEM_WEAPON)
			if (wield->attack_level > 0) count += (wield->attack_level);

                if (ch->tier > 0) count += ch->tier;
                if (ch->pcdata->relrank > 0) count += ch->pcdata->relrank;

                if (IS_CLASS(ch, CLASS_VAMPIRE))
		{
                    if (ch->power[DISC_VAMP_CELE] > 7)
                    {
                        count += 1;
                        if (ch->power[DISC_VAMP_CELE] > 9) count += 1;
                    }
		}
                else if (IS_CLASS(ch, CLASS_JEDI))
		{
		    if (ch->pcdata->powers[JPOWER_MIND]      >= 4) count += 2;
                    if (ch->pcdata->powers[JPOWER_SPIRIT]    >= 5) count += 3;
		    if (ch->pcdata->powers[JPOWER_BODY]      >= 6) count += 3;
                    if (ch->pcdata->powers[JPOWER_TECHNIQUE] >= 9) count += 4;
		}
                else if (IS_CLASS(ch, CLASS_SHADOW))
                    count += ch->pcdata->powers[SHADOW_MARTIAL] / 5;
                else if (IS_CLASS(ch, CLASS_GIANT))
                    count += ch->pcdata->rank / 3;
                else if (IS_CLASS(ch, CLASS_DRONE))
                    count += ch->pcdata->powers[DRONE_FOCUS] * 2;
                else if (IS_CLASS(ch, CLASS_FAE))
                {
                    if (ch->pcdata->powers[FAE_PLASMA] > 7) count += 2;
                    if (ch->pcdata->powers[FAE_MATTER] > 7) count += 2;
                    if (ch->pcdata->powers[FAE_ENERGY] > 7) count += 2;
                    if (ch->pcdata->powers[FAE_WILL]   > 7) count += 2;
                }
                else if (IS_CLASS(ch, CLASS_TANARRI))
		    count += ch->pcdata->rank;
                else if (IS_CLASS(ch, CLASS_CYBORG))
                    count += ch->pcdata->powers[CYBORG_LIMBS];
                else if (IS_CLASS(ch, CLASS_THIEF))
		{
                    if (IS_SET(ch->pcdata->powers[1], THIEF_SWIFTNESS)) count += 5;
		}
                else if (IS_CLASS(ch, CLASS_DROID))
                {
                    count += ch->pcdata->powers[DROID_SPEED] / 2;   // max 3 at speed 6
                    if (IS_SET(ch->pcdata->powers[DROID_IMPLANTS], IMPLANT_EXTRA_ARMS))
                        count += 2;
                }
                else if (IS_CLASS(ch, CLASS_HOBBIT))
                    count += ch->pcdata->powers[HOBBIT_DISHES] / 4;
                else if (IS_CLASS(ch, CLASS_ANGEL))
                    count += ch->pcdata->powers[ANGEL_JUSTICE];
		else if (IS_CLASS(ch, CLASS_PALADIN))
		    count += ch->pcdata->powers[PALADIN_COMBAT];
		else if (IS_CLASS(ch, CLASS_ELEMENTAL))
		{
		count += ch->pcdata->powers[ELEMENTAL_FIRE];
	        if (IS_SET(ch->pcdata->powers[ELEM_FORMS], AIR_FORM)) count += 2;
		}
		else if (IS_CLASS(ch, CLASS_PRIEST)) count += 4;
		else if (IS_CLASS(ch, CLASS_DRAGON)) count += 5;
		else if (IS_CLASS(ch, CLASS_ZOMBIE)) count += 5;
	        else if (IS_CLASS(ch,CLASS_SHINOBI)){
		count += ch->chakra / 2000;
		count += 2;
		switch(ch->pcdata->powers[S_ELEMENT]){
			case S_FIRE:
				count += ch->pcdata->powers[S_HANDFIRE];
				break;
			case S_WIND:
				count += ch->pcdata->powers[S_HANDWIND];
				break;
			case S_WATER:
				count += ch->pcdata->powers[S_HANDWATER];
				break;
			case S_HOLY:
				count += ch->pcdata->powers[S_HANDHOLY];
				break;
			case S_EARTH:
				count += ch->pcdata->powers[S_HANDEARTH];
				break;
			case S_SHADOW:
				count += ch->pcdata->powers[S_HANDSHADOW];
				break;
		}
	}

                else if (IS_CLASS(ch, CLASS_WEREWOLF))
                {
                    if (ch->power[DISC_WERE_LYNX] > 2) count += 4;
                    if (ch->power[DISC_WERE_BOAR] > 6) count += 7;
                }
                else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                    count += ch->pcdata->powers[WEAPONSKILL] / 1.6;
                else if (IS_CLASS(ch, CLASS_BLADE_MASTER))
                    count += ch->pcdata->powers[WEAPONSKILL] / 1.6;

                else if (IS_CLASS(ch, CLASS_LICH))
                    count += 4;
		else if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
                {
                    count += 3;
                    if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
			count += ch->pcdata->powers[BULL_LEVEL] / 2;
                    else if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
                        count += ch->pcdata->powers[TIGER_LEVEL];
                    else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
                        count += ch->pcdata->powers[FAERIE_LEVEL];
                    else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
                        count += ch->pcdata->powers[HYDRA_LEVEL] / 2;
                }
                else if (IS_CLASS(ch, CLASS_DEMON))
                {
                    if (IS_DEMPOWER(ch, DEM_SPEED)) count += 3;
                    if (IS_SET(ch->warp, WARP_QUICKNESS)) count += 4;
                }
		else if (IS_CLASS(ch, CLASS_SKYBLADE))
                    count += 6;
                else if (IS_CLASS(ch, CLASS_GHOUL))
                    count += 7;
                else if (IS_CLASS(ch, CLASS_DRACONIAN))
                    count += 8;
                else if (IS_CLASS(ch, CLASS_DROW))
                {
                    if (IS_SET(ch->special, SPC_DROW_WAR)) count += 3;
                    if (IS_SET(ch->pcdata->powers[1], DPOWER_SPEED)) count += 5;
                }
                else if (IS_CLASS(ch, CLASS_MAGE))
		{
                    if (IS_ITEMAFF(ch, ITEMA_BEAST)) count += 4;
		}
                else if (IS_CLASS(ch, CLASS_NINJA))
		{
                    if (ch->pcdata->powers[NPOWER_CHIKYU] >= 3) count += 3;
		    if (ch->pcdata->powers[HARA_KIRI] > 0) count += 2;
		    switch( ch->pcdata->rank )
                    {
			default: break;
			case BELT_ONE:   count += 1; break;
                        case BELT_TWO:   count += 1; break;
                        case BELT_THREE: count += 2; break;
                        case BELT_FOUR:  count += 2; break;
                        case BELT_FIVE:  count += 2; break;
                        case BELT_SIX:   count += 3; break;
                        case BELT_SEVEN: count += 3; break;
                        case BELT_EIGHT: count += 4; break;
                        case BELT_NINE:  count += 4; break;
                        case BELT_TEN:   count += 5; break;
		    }
		}
                else if (IS_CLASS(ch, CLASS_SAMURAI))
                    count += 6;

                if (IS_ITEMAFF(ch, ITEMA_SPEED)) count += 2;
        }
        else
        {
                if (ch->tier > 0) count += ch->tier;
                if (ch->pcdata->relrank > 0) count += ch->pcdata->relrank;
                if (IS_STANCE(ch, STANCE_VIPER)
                    && number_percent() < ch->stance[STANCE_VIPER] * 0.5)
                        count += 1;
                else if (IS_STANCE(ch, STANCE_HAWK)
                         && number_percent() <
                         ch->stance[STANCE_HAWK] * 0.5)
                        count += 1;
                else if (IS_STANCE(ch, STANCE_COUGAR)
                         && number_percent() < ch->stance[STANCE_COUGAR] * 0.5)
                        count += 1;
                else if (ch->stance[0] > 12
                         && IS_SET(ch->stance[(ch->stance[0] + 6)],
                                   STANCEPOWER_SPEED)
                         && number_percent() <
                         ch->stance[(ch->stance[0])] * 0.5)
                        count += 2;

                if (rel_oppose(ch, victim) && ch->in_room &&
                    has_area_affect(ch->in_room->area, AREA_AFF_GZONE, ch->pcdata->religion))
                        count++;

                if (IS_CLASS(ch, CLASS_CYBORG))
                    count += ch->pcdata->powers[CYBORG_LIMBS];
                else if (IS_CLASS(ch, CLASS_GIANT))
                    count += ch->pcdata->rank / 3;
                else if (IS_CLASS(ch, CLASS_SHADOW))
                    count += ch->pcdata->powers[SHADOW_MARTIAL] / 5;
                else if (IS_CLASS(ch, CLASS_FAE))
                {
                    if (ch->pcdata->powers[FAE_PLASMA] > 7)
                        count++;
                    if (ch->pcdata->powers[FAE_MATTER] > 7)
                        count++;
                    if (ch->pcdata->powers[FAE_ENERGY] > 7)
                        count++;
                    if (ch->pcdata->powers[FAE_WILL] > 7)
                        count++;
                }
                else if (IS_CLASS(ch, CLASS_HOBBIT))
                    count += ch->pcdata->powers[HOBBIT_DISHES] / 4;
                else if (IS_CLASS(ch, CLASS_DROID))
                {
                    count += ch->pcdata->powers[DROID_SPEED] / 2;   // max 3 at speed 6
                    if (IS_SET(ch->pcdata->powers[DROID_IMPLANTS], IMPLANT_EXTRA_ARMS))
                        count += 2;
                }
                else if (IS_CLASS(ch, CLASS_VAMPIRE))
                {
                                if (ch->power[DISC_VAMP_CELE] > 0)
                                        count += 1;
                                if (ch->power[DISC_VAMP_CELE] > 5)
                                        count += 1;
                                if (ch->power[DISC_VAMP_CELE] > 7)
                                        count += 1;
                                if (ch->power[DISC_VAMP_CELE] > 9)
                                        count += 3;
                }
                else if (IS_CLASS(ch, CLASS_SHADOW)) /* CLASS_REMOVE */
                                count += ch->pcdata->powers[SHADOW_MARTIAL] / 5;
                else if (IS_CLASS(ch, CLASS_JEDI))
		{
                    if (ch->pcdata->powers[JPOWER_SPIRIT]    >= 5) count += 3;
                    if (ch->pcdata->powers[JPOWER_MIND]      >= 4) count += 2;
                    if (ch->pcdata->powers[JPOWER_BODY]      >= 6) count += 3;
                    if (ch->pcdata->powers[JPOWER_TECHNIQUE] >= 9) count += 4;
		}
                else if (IS_CLASS(ch, CLASS_MONK))
		{
                    if (ch->monkab[COMBAT] > 0) count += ch->monkab[COMBAT];
		}
                else if (IS_CLASS(ch, CLASS_MAGE))
		{
                    if (IS_ITEMAFF(ch, ITEMA_BEAST)) count += 8;
		}
                else if (IS_CLASS(ch, CLASS_ANGEL))
                                count += 2;
		else if (IS_CLASS(ch, CLASS_PALADIN))
				count += ch->pcdata->powers[PALADIN_COMBAT];
		else if (IS_CLASS(ch, CLASS_ELEMENTAL))
                {
                count += ch->pcdata->powers[ELEMENTAL_FIRE];
                if (IS_SET(ch->pcdata->powers[ELEM_FORMS], AIR_FORM)) count += 2;
                }
                else if (IS_CLASS(ch, CLASS_PRIEST)) count += 4;
		else if (IS_CLASS(ch, CLASS_DRAGON)) count += 5;
		else if (IS_CLASS(ch, CLASS_ZOMBIE)) count += 5;
	        else if (IS_CLASS(ch,CLASS_SHINOBI)){
		count += ch->chakra / 2000;
		count += 2;
		switch(ch->pcdata->powers[S_ELEMENT]){
			case S_FIRE:
				count += ch->pcdata->powers[S_HANDFIRE];
				break;
			case S_WIND:
				count += ch->pcdata->powers[S_HANDWIND];
				break;
			case S_WATER:
				count += ch->pcdata->powers[S_HANDWATER];
				break;
			case S_HOLY:
				count += ch->pcdata->powers[S_HANDHOLY];
				break;
			case S_EARTH:
				count += ch->pcdata->powers[S_HANDEARTH];
				break;
			case S_SHADOW:
				count += ch->pcdata->powers[S_HANDSHADOW];
				break;
		}
	}

                else if (IS_CLASS(ch, CLASS_DRONE))
                                count += ch->pcdata->powers[DRONE_FOCUS] * 2;
                else if (IS_CLASS(ch, CLASS_WEREWOLF))
                {
                    if (ch->power[DISC_WERE_LYNX] > 2) count += 4;
                    if (ch->power[DISC_WERE_BOAR] > 6) count += 7;
                }
                else if (IS_CLASS(ch, CLASS_LICH))
                    count += 4;
                else if (IS_CLASS(ch, CLASS_TANARRI))
                {
                    count += ch->pcdata->rank;
                    if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED))
                        count += 4;
                }
                else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                    count += ch->pcdata->powers[WEAPONSKILL] / 1.6;
                else if (IS_CLASS(ch, CLASS_BLADE_MASTER))
                    count += ch->pcdata->powers[WEAPONSKILL] / 1.6;

		else if (IS_CLASS(ch, CLASS_NINJA))
		{
		    if (ch->pcdata->powers[HARA_KIRI] > 0) count += 2;
		    switch( ch->pcdata->rank )
		    {
			default: break;
                        case BELT_ONE:   count += 1; break;
                        case BELT_TWO:   count += 1; break;
                        case BELT_THREE: count += 2; break;
                        case BELT_FOUR:  count += 2; break;
                        case BELT_FIVE:   count += 2; break;
                        case BELT_SIX:   count += 3; break;
                        case BELT_SEVEN: count += 3; break;
                        case BELT_EIGHT: count += 4; break;
                        case BELT_NINE:  count += 4; break;
                        case BELT_TEN:   count += 5; break;
		    }
		}
                else if (IS_CLASS(ch, CLASS_SAMURAI))
                                count += 6;
                else if (IS_CLASS(ch, CLASS_DEMON))
		{
                    if (ch->power[DISC_DAEM_ATTA] > 0)
                                count += ch->power[DISC_DAEM_ATTA] / 1.5;
                }
		else if (IS_CLASS(ch, CLASS_DROW))
		{
                    if (IS_SET(ch->special, SPC_DROW_WAR)) count += 3;
                    if (IS_SET(ch->pcdata->powers[1], DPOWER_SPEED)) count += 5;
                }

                if (IS_ITEMAFF(ch, ITEMA_SPEED)) count += 1;

                /*
                 * bonus for high stats 
                 */
                count += (get_curr_str(ch) + get_curr_dex(ch)) / 50;

                if (IS_CLASS(victim, CLASS_JEDI)
                    && (victim->pcdata->powers[JPOWER_SPIRIT] > 3))
                        count -= 1;
                if (victim->power[DISC_VAMP_CELE] >= 3)
                        count -= victim->power[DISC_VAMP_CELE] / 3;
                if (victim->power[DISC_DAEM_ATTA] > 0)
                        count -= victim->power[DISC_DAEM_ATTA] / 2;
                if (victim->power[DISC_WERE_MANT] >= 3)
                        count -= victim->power[DISC_WERE_MANT] / 3;
                else if (IS_ITEMAFF(victim, ITEMA_AFFMANTIS))
                        count -= 1;

	        if (number_range(1, 4) == 2) count -= 1;
	}

        if (count < 1) count = 1;

        return count;
}


/*
 * Hit one guy once.
 */
void one_hit(CHAR_DATA * ch, CHAR_DATA * victim, int dt, int handtype)
{
        OBJ_DATA *wield;
        int       victim_ac;
        int       thac0;
        int       thac0_00;
        int       thac0_32;
        int       dam, wdam;
        int       diceroll;
        int       level;
        int       attack_modify;
        int       right_hand;

        /*
         * Can't beat a dead char!
         * Guard against weird room-leavings.
         */
        if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
                return;
        if (!IS_NPC(ch))
        {
                if (IS_AFFECTED(ch, AFF_PEACE) && ch->fighting != NULL)
                        REMOVE_BIT(ch->affected_by, AFF_PEACE);
        }
        if (!IS_NPC(victim))
        {
                if (IS_AFFECTED(victim, AFF_PEACE)
                    && victim->fighting != NULL)
                        REMOVE_BIT(victim->affected_by, AFF_PEACE);
        }
        /*
         * Figure out the type of damage message.
         */
        if (handtype == 8)
        {
                wield = get_eq_char(ch, WEAR_FOURTH);
                right_hand = 8;
        }
        else if (handtype == 4)
        {
                wield = get_eq_char(ch, WEAR_THIRD);
                right_hand = 4;
        }
        else if (handtype == 2)
        {
                wield = get_eq_char(ch, WEAR_HOLD);
                right_hand = 2;
        }
        else if (handtype == 0)
        {
                wield = NULL;
                right_hand = 0;
        }
        else
        {
                if (IS_SET(ch->act, PLR_RIGHTHAND))
                        wield = get_eq_char(ch, WEAR_WIELD);
                else if (IS_SET(ch->act, PLR_LEFTHAND))
                        wield = get_eq_char(ch, WEAR_HOLD);
                else
                {
                        if (number_range(1, 10) > 4)
                                wield = get_eq_char(ch, WEAR_HOLD);
                        else
                                wield = get_eq_char(ch, WEAR_WIELD);
                }
                right_hand = 1; // improve the right hand anyway...
        }
        if (dt == TYPE_UNDEFINED)
        {
                dt = TYPE_HIT;
                if (wield != NULL && wield->item_type == ITEM_WEAPON)
                        dt += wield->value[3];
                else if (IS_NPC(ch))
                        dt += ch->pIndexData->natural_attack;
        }
        if (ch->wpn[dt - 1000] > 5)
                level = (ch->wpn[dt - 1000] / 5);
        else
                level = 1;
        if (level > 40)
                level = 40;

        /*
         * Calculate to-hit-armor-class-0 versus armor.
         */
        if (IS_NPC(ch))
        {
                thac0_00 = 20;
                thac0_32 = 0;
        }
        else
        {
                thac0_00 = SKILL_THAC0_00;
                thac0_32 = SKILL_THAC0_32;
        }
        thac0 = interpolate(level, thac0_00, thac0_32) - char_hitroll(ch);
        victim_ac = UMAX(-100, char_ac(victim) / 10);
        if (!can_see(ch, victim))
                victim_ac -= 4;
        if (ch->stance[0] > 12
            && IS_SET(ch->stance[(ch->stance[0] + 6)], STANCEPOWER_DAMAGE_3))
                victim_ac += 300;
        if (ch->stance[0] > 12
            && IS_SET(ch->stance[(ch->stance[0] + 6)], STANCEPOWER_DAMAGE_2))
                victim_ac += 200;
        if (ch->stance[0] > 12
            && IS_SET(ch->stance[(ch->stance[0] + 6)], STANCEPOWER_DAMAGE_1))
                victim_ac += 100;


        /*
         * The moment of excitement!
         */
        while ((diceroll = number_bits(5)) >= 20)
                ;

        if (diceroll == 0 || (diceroll != 19 && diceroll < thac0 - victim_ac))
        {   /* Miss. */
                damage(ch, victim, 0, dt);
                tail_chain();
                if (!is_safe(ch, victim))
                {

                        improve_wpn(ch, dt, right_hand);
                        improve_stance(ch);
                }
                return;
        }

        /*
         * Hit.
         * Calc damage.
         */
        if (IS_NPC(ch))
        {
                dam = number_range(ch->level / 2, ch->level * 3 / 2);
                if (wield != NULL)
                        dam += dam * 0.5;
        }
        else
        {
                if (dt == gsn_mageshield)
                {
                        dam = (ch->spl[RED_MAGIC] + ch->spl[YELLOW_MAGIC] +
                               ch->spl[GREEN_MAGIC] + ch->spl[PURPLE_MAGIC] +
                               ch->spl[BLUE_MAGIC]) / 5;
                        dam = number_range(2 * dam, 3 * dam);
                }
                else if (wield != NULL && wield->item_type == ITEM_WEAPON)
                        dam = dice(wield->value[1], wield->value[2]);
                else if (IS_VAMPAFF(ch, VAM_CLAWS)
                         && IS_GAR1(ch, WOLF_RAZORCLAWS))
                        dam = dice(25, 35);
                else if (IS_VAMPAFF(ch, VAM_CLAWS))
                        dam = dice(10, 20);
                else if (IS_SET(ch->newbits, NEW_MONKADAM))
                        dam = dice(10, 25);
                else
                        dam = dice(4, 10);
                if (IS_CLASS(ch, CLASS_NINJA))
		{
                    if (ch->pcdata->rank == BELT_SEVEN)      dam += dice(1, 5);
                    else if (ch->pcdata->rank == BELT_EIGHT) dam += dice(6, 10);
                    else if (ch->pcdata->rank == BELT_NINE)  dam += dice(11, 15);
                    else if (ch->pcdata->rank == BELT_TEN)   dam += dice(16, 20);
		}
        }
        /*
         * Bonuses.
         */
        dam += char_damroll(ch);
        if (!IS_AWAKE(victim))
                dam *= 2;
        if (!IS_NPC(ch) && dt >= TYPE_HIT)
                dam = dam +
                        (dam * (UMIN(350, (ch->wpn[dt - 1000] + 1)) / 60));
        dam = up_dam(ch, victim, dam);

        /*
         * Other Resistances 
         */

        if (IS_ITEMAFF(victim, ITEMA_RESISTANCE))
        {
                dam *= 3;
                dam /= 4;
        }

        /*
         * damage reduction 
         */
        if (!IS_NPC(ch) && !IS_NPC(victim))
                dam /= 2;   /* slow down pk */
        if (IS_NPC(ch) && dam > 2000)
                dam = 2000 + (dam - 2000) / 2;  /* mob damage >2000 halved */
        if (dam <= 0)
                dam = 5;

	if (!IS_NPC(ch))
	{
        /*
         * I doubt this has much effect - Jobo
         */
          if (IS_CLASS(ch, CLASS_VAMPIRE))
	  {
	    if (IS_NPC(victim))
                dam *= (1 + ch->power[DISC_VAMP_POTE] / 15);
	  }
          else if (IS_CLASS(ch, CLASS_TANARRI))
          {
            if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_MIGHT))
              dam *= 1.5;
          }
          /*
           * This one for angels have no effect, take a look and rebalance. Jobo
           */
          else if (IS_CLASS(ch, CLASS_ANGEL))
                dam *= (1 + ch->pcdata->powers[ANGEL_JUSTICE] / 10);
	  else if (IS_CLASS(ch, CLASS_PALADIN))
		dam *= (1 + ch->pcdata->powers[PALADIN_COMBAT] / 10);
	  else if (IS_CLASS(ch, CLASS_ELEMENTAL))
		dam *= (1 + ch->pcdata->powers[ELEMENTAL_FIRE] / 10);
	  else if (IS_CLASS(ch, CLASS_DRAGON))
		dam *= 1.4;
	  else if (IS_CLASS(ch, CLASS_ZOMBIE))
		dam *= 1.5;
	  else if (IS_CLASS(ch, CLASS_SHINOBI))
		dam *= 1.5;
          else if (IS_CLASS(ch, CLASS_DEMON))
	  {
            if (IS_DEMPOWER(ch, DEM_MIGHT))
                dam *= 1.1;
            if (IS_SET(ch->warp, WARP_STRONGARMS))
                dam *= 1.2;
	  }
          else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
	  {
            if (ch->pcdata->powers[WEAPONSKILL] > 4)
              dam *= 1.4;
            if (ch->pcdata->powers[WEAPONSKILL] > 8)
              dam *= 1.6;
            if (IS_CLASS(victim, CLASS_SHAPESHIFTER))
              dam *= 1.2;
	  }
          else if (IS_CLASS(ch, CLASS_BLADE_MASTER))
          {
            if (ch->pcdata->powers[WEAPONSKILL] > 4)
              dam *= 1.4;
            if (ch->pcdata->powers[WEAPONSKILL] > 8)
              dam *= 1.6;
            if (IS_CLASS(victim, CLASS_SAMURAI))
              dam *= 1.2;
          }

          else if (IS_CLASS(ch, CLASS_MAGE))
          {
            if (dt == gsn_mageshield && ch->pcdata->powers[PINVOKE] > 6)
              dam *= 1.4;
            if (dt == gsn_mageshield && ch->pcdata->powers[PINVOKE] > 9)
              dam *= 1.4;
            if (dt == gsn_mageshield)
            {
              if (ch->pcdata->powers[PINVOKE] > 6)
                dam *= 1.4;
              if (ch->pcdata->powers[PINVOKE] > 9)
                dam *= 1.4;
            }
          }
          else if (IS_CLASS(ch, CLASS_NINJA))
          {
                if (ch->pcdata->rank == BELT_ONE)
                        dam *= 1.1;
                else if (ch->pcdata->rank == BELT_TWO)
                        dam *= 1.2;
                else if (ch->pcdata->rank == BELT_THREE)
                        dam *= 1.3;
                else if (ch->pcdata->rank == BELT_FOUR)
                        dam *= 1.4;
                else if (ch->pcdata->rank == BELT_FIVE)
                        dam *= 1.5;
                else if (ch->pcdata->rank == BELT_SIX)
                        dam *= 1.6;
                else if (ch->pcdata->rank == BELT_SEVEN)
                        dam *= 1.7;
                else if (ch->pcdata->rank == BELT_EIGHT)
                        dam *= 1.8;
                else if (ch->pcdata->rank == BELT_NINE)
                        dam *= 1.9;
                else if (ch->pcdata->rank == BELT_TEN)
                        dam *= 2.0;
          }
          else if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
          {
                dam *= 1.4;
                if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM
                    && ch->pcdata->powers[TIGER_LEVEL] > 1)
                        dam *= 1.5;
                else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM
                         && ch->pcdata->powers[FAERIE_LEVEL] > 1)
                        dam *= 1.2;
                else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM
                         && ch->pcdata->powers[HYDRA_LEVEL] > 1)
                        dam *= 1.6;
                else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM
                         && ch->pcdata->powers[BULL_LEVEL] > 1)
                        dam *= 1.7;
          }
          else if (IS_CLASS(ch, CLASS_CYBORG))
          {
                if (ch->pcdata->powers[CYBORG_LIMBS] > 0)
                        dam *= 1.3;
                if (ch->pcdata->powers[CYBORG_LIMBS] > 2)
                        dam *= 1.5;
          }
          else if (IS_CLASS(ch, CLASS_WEREWOLF))
	  {
            if (ch->power[DISC_WERE_BEAR] > 5)
              dam *= 1.2;
	  }
          else if (IS_CLASS(ch, CLASS_MONK))
	  {
	    if (dt != gsn_heavenlyaura)
            {
                if (ch->chi[CURRENT] > 0 && ch->chi[CURRENT] < 3)
                        dam *= 1.5;
                if (ch->chi[CURRENT] > 2 && ch->chi[CURRENT] < 7)
                        dam *= ((ch->chi[CURRENT]) / 2);
	    }
          }
	}

        if (IS_NPC(victim))
        {
                if (ch->stance[0] > 12
                    && IS_SET(ch->stance[(ch->stance[0] + 6)],
                              STANCEPOWER_DAMAGE_3)
                    && ch->stance[(ch->stance[0])] > 100)
                        dam += dam * 1.5 * (ch->stance[(ch->stance[0])] /
                                            200);
                else if (ch->stance[0] > 12
                         && IS_SET(ch->stance[(ch->stance[0] + 6)],
                                   STANCEPOWER_DAMAGE_2)
                         && ch->stance[(ch->stance[0])] > 100)
                        dam += dam * 1.0 * (ch->stance[(ch->stance[0])] /
                                            200);
                else if (ch->stance[0] > 12
                         && IS_SET(ch->stance[(ch->stance[0] + 6)],
                                   STANCEPOWER_DAMAGE_1)
                         && ch->stance[(ch->stance[0])] > 100)
                        dam += dam * 0.5 * (ch->stance[(ch->stance[0])] /
                                            200);
        }
        else
        {
                if (ch->stance[0] > 12
                    && IS_SET(ch->stance[(ch->stance[0] + 6)],
                              STANCEPOWER_DAMAGE_3)
                    && ch->stance[(ch->stance[0])] > 100)
                        dam += dam * 0.5 * (ch->stance[(ch->stance[0])] /
                                            200);
                else if (ch->stance[0] > 12
                         && IS_SET(ch->stance[(ch->stance[0] + 6)],
                                   STANCEPOWER_DAMAGE_2)
                         && ch->stance[(ch->stance[0])] > 100)
                        dam += dam * 0.3 * (ch->stance[(ch->stance[0])] /
                                            200);
                else if (ch->stance[0] > 12
                         && IS_SET(ch->stance[(ch->stance[0] + 6)],
                                   STANCEPOWER_DAMAGE_1)
                         && ch->stance[(ch->stance[0])] > 100)
                        dam += dam * 0.1 * (ch->stance[(ch->stance[0])] /
                                            200);
        }

        if (victim->stance[0] > 12
            && IS_SET(victim->stance[(victim->stance[0] + 6)],
                      STANCEPOWER_RESIST_3)
            && victim->stance[(victim->stance[0])] > 100)
                dam *= 0.7;
        else if (victim->stance[0] > 12
                 && IS_SET(victim->stance[(victim->stance[0] + 6)],
                           STANCEPOWER_RESIST_2)
                 && victim->stance[(victim->stance[0])] > 100)
                dam *= 0.8;
        else if (victim->stance[0] > 12
                 && IS_SET(victim->stance[(victim->stance[0] + 6)],
                           STANCEPOWER_RESIST_1)
                 && victim->stance[(victim->stance[0])] > 100)
                dam *= 0.9;

        /*
         * The test ends here 
         */

        if (!IS_NPC(victim))
	{
          if (IS_CLASS(victim, CLASS_SKYBLADE))
	  {
            if (victim->pcdata->powers[SKYBLADE_MAGIC] > 6) dam -= dam * .3;
	  }
          else if (IS_CLASS(victim, CLASS_SHAPESHIFTER) || IS_CLASS(victim, CLASS_HOBBIT))
          {
            if (IS_CLASS(victim, CLASS_HOBBIT)
              || (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM
               && victim->pcdata->powers[FAERIE_LEVEL] > 0))
            {
              int       growl = number_range(1, 50);
              if (growl == 10 || growl == 20 || growl == 30 || growl == 40 || growl == 50)
              {
                stc("#3You dodge your opponents attack#n.\n\r", victim);
                stc("#3Your opponent dodges your attack#n.\n\r", ch);
                dam = 0;
              }
            }
	  }
          else if (IS_CLASS(victim, CLASS_DROW))
	  {
            if (IS_SET(victim->newbits, NEW_DARKTENDRILS))
            {
              int       ddodge = number_range(1, 50);

              if (ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40 || ddodge == 50)
              {
                stc("#0You dark aura absorbs the damage#n.\n\r", victim);
                stc("#0Your opponent's dark aura absorbs your damage#n.\n\r", ch);
                dam = 0;
              }
	    }
          }
	  else if (IS_CLASS(victim, CLASS_MAGE))
	  {
            if (IS_ITEMAFF(victim, ITEMA_ILLUSIONS))
            {
                int       ddodge = number_range(1, 50);

                if (ddodge == 10 || ddodge == 20 || ddodge == 30
                    || ddodge == 40 || ddodge == 50)
                {
                        stc("#CYour illusion takes the blow, saving you from harm#n.\n\r", victim);
                        stc("#CYour weapon passes through your opponent without doing any harm#n.\n\r", ch);
                        dam = 0;
                }
            }
          }
          else if (IS_CLASS(victim, CLASS_SHADOW))
	  {
            if (IS_SET(victim->pcdata->powers[SHADOW_BITS], NPOWER_BLUR))
            {
              int       ddodge = number_range(1, 50);

              if (ddodge == 5 || ddodge == 20 || ddodge == 35 || ddodge == 50)
              {
                stc("#0You feel the strike pass through your shadow body#n.\n\r", victim);
                stc("#0You hit nothing but shadows#n.\n\r", ch);
                dam = 0;
              }
            }
	  }
	}


        if (!IS_NPC(ch) && wield != NULL)
        {
                if (number_range(1, 6) == 2)
                {
                    if (ch->fighting == victim)
		    {
                        if (IS_WEAP(wield, WEAPON_VAMPIRIC))
                        {
                                wdam = number_range(1, wield->level / 5 + 1);
                                act("$p draws life from $n.", victim, wield,
                                    NULL, TO_ROOM);
                                act("You feel $p drawing your life away.",
                                    victim, wield, NULL, TO_CHAR);
                                hurt_person(ch, victim, wdam);
                                if (victim->hit <= 0)
                                        return;
                        }
			if (IS_WEAP(wield, WEAPON_FLAMING)
                                || IS_WEAP(wield, WEAPON_ELE_FLAME))
                        {
                                wdam = number_range(1, wield->level / 4 + 1);
                                if (!IS_SET(ch->act, PLR_BRIEF2))
                                        act("$N is burned by your $p.", ch,
                                            wield, victim, TO_CHAR);
                                if (!IS_SET(victim->act, PLR_BRIEF2))
                                        act("You are burned by $n's $p.", ch,
                                            wield, victim, TO_VICT);
                                fire_effect((void *) victim, wield->level / 2,
                                            wdam, TARGET_CHAR);
                                hurt_person(ch, victim, wdam);
                                if (victim->hit <= 0)
                                        return;
                        }
                        if (IS_WEAP(wield, WEAPON_FROST)
                                || IS_WEAP(wield, WEAPON_ELE_WATER))
                        {
                                int       sn = skill_lookup("chill touch");

                                if (!is_affected(victim, sn))
                                {
                                        wdam = number_range(1,
                                                            wield->level / 6 +
                                                            2);
                                        if (!IS_SET(ch->act, PLR_BRIEF2))
                                                act("Your $p freezes $N.", ch,
                                                    wield, victim, TO_CHAR);
                                        if (!IS_SET(victim->act, PLR_BRIEF2))
                                                act("You are frozen by $n's $p.", ch, wield, victim, TO_VICT);
                                        cold_effect(victim, wield->level / 2,
                                                    wdam, TARGET_CHAR);
                                        hurt_person(ch, victim, wdam);
                                }
                                if (victim->hit <= 0)
                                        return;
                        }
                        if (IS_WEAP(wield, WEAPON_ELE_EARTH))
                        {
                                wdam = number_range(1, wield->level / 5 + 2);
                                act("$p covers $n in acid.", victim, wield,
                                    NULL, TO_ROOM);
                                act("The corrosive touch of $p surrounds you with acid.", victim, wield, NULL, TO_CHAR);
                                acid_effect(victim, wield->level / 2, wdam,
                                            TARGET_CHAR);
                                hurt_person(ch, victim, wdam);
                                if (victim->hit <= 0)
                                        return;
                        }
                        if (IS_WEAP(wield, WEAPON_SHOCKING)
                                || IS_WEAP(wield, WEAPON_ELE_AIR))
                        {
                                wdam = number_range(1, wield->level / 5 + 2);
                                act("$n is struck by lightning from $p.",
                                    victim, wield, NULL, TO_ROOM);
                                act("You are shocked by $p.", victim, wield,
                                    NULL, TO_CHAR);
                                shock_effect(victim, wield->level / 2, wdam,
                                             TARGET_CHAR);
                                hurt_person(ch, victim, wdam);
                                if (victim->hit <= 0)
                                        return;
                        }
		    }
                }
        }

        /*
         * Damage type modifiers.
         */

        if (dt == gsn_garotte)
                dam *= (number_range(3, 5));
        else if (dt == gsn_soulseeker)
                dam *= (number_range(4, 9));
        else if (dt == gsn_backstab)
        {
                if (IS_CLASS(ch, CLASS_NINJA) && !IS_NPC(ch)
                    && ch->pcdata->powers[NPOWER_NINGENNO] >= 2)
                        dam *= number_range(50, 60);
                else if (IS_CLASS(ch, CLASS_DROW))
                {
                        dam += number_range(100, 1000);
                        dam *= number_range(7, 10);
                }
                else
                        dam *= number_range(2, 4);
        }
        else if (dt == gsn_magma)
	{
	    if (IS_CLASS(ch, CLASS_TANARRI))
                dam *= 1.7;
	}
        else if (dt == gsn_stuntubes)
	{
	    if (IS_CLASS(ch, CLASS_CYBORG))
		dam *= 1.8;
	}
        else if (dt == gsn_booming)
	{
	    if (!IS_NPC(ch))
                dam *= ch->pcdata->rank / 2;
	}
        else if (dt == gsn_circle)
	{
	    if (ch->pcdata->rank == BELT_EIGHT)
                dam *= 2;
            else if (ch->pcdata->rank == BELT_NINE)
                dam *= 2.5;
            else if (ch->pcdata->rank == BELT_TEN)
                dam *= 3.0;
	}
        else if (dt == gsn_heavenlyaura)
	{
	    if (IS_CLASS(ch, CLASS_MONK))
                dam /= 10;
	}
        else if (dt == gsn_deathaura)
                dam /= 1.2;
        else if (dt == gsn_potato)
                dam /= 1.2;
        else if (dt == gsn_shred)
                dam *= 3;


        dam = cap_dam(ch, victim, dam);

        if (IS_NPC(ch) && ch->pIndexData->dam_modifier > 3) dam *= 3;
        else if (IS_NPC(ch) && ch->pIndexData->dam_modifier > 0) dam *= ch->pIndexData->dam_modifier;

/*        if (IS_NPC(victim) && !IS_NPC(ch))
        {
                if (victim->pIndexData->toughness > 100)
                        victim->pIndexData->toughness = 99;
                dam = dam * (100 - victim->pIndexData->toughness) / 100;
        }
*/
        attack_modify = dice(1, 100);
        randomize_damage(ch, dam, attack_modify);
        if (dt == gsn_deathaura && dam >= victim->hit && IS_NPC(victim))
                dam = victim->hit - 1;  /* trust me, Jobo */
        damage(ch, victim, dam, dt);

        improve_weapon_res(victim, dt);

        tail_chain();
        if (!is_safe(ch, victim))
        {
                improve_wpn(ch, dt, right_hand);
                improve_stance(ch);
        }
	return;
}

int randomize_damage(CHAR_DATA * ch, int dam, int am)
{
        dam = dam * (am + 50) / 100;
        return dam;
}

int up_dam(CHAR_DATA * ch, CHAR_DATA * victim, int dam)
{
        if (!IS_NPC(ch))
        {
                /*
                 * class vs class stuff 
                 */
                if (IS_CLASS(ch, CLASS_VAMPIRE)
                    && IS_CLASS(victim, CLASS_HOBBIT))
                {
                        dam *= 12;
                        dam /= 10;
                }
                if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)
                    && IS_CLASS(victim, CLASS_SHAPESHIFTER))
                        dam *= 1.2;
                if (IS_CLASS(ch, CLASS_MAGE) && IS_CLASS(victim, CLASS_NINJA))
                        dam *= 1.25;

                /*
                 * class modifiers 
                 */
                if (IS_CLASS(ch, CLASS_TANARRI))
                {
                        if (IS_SET
                            (ch->pcdata->powers[TANARRI_POWER],
                             TANARRI_MIGHT))
                        {
                                dam *= 15;
                                dam /= 10;
                        }
                }
                else if (IS_CLASS(ch, CLASS_DRONE))
                {
                        if (ch->pcdata->powers[DRONE_FOCUS] > 0)
                                dam *= 1.2;
                        if (ch->pcdata->powers[DRONE_FOCUS] > 1)
                                dam *= 1.2;
                        if (ch->pcdata->powers[DRONE_FOCUS] > 2)
                                dam *= 1.2;
                }
                else if (IS_CLASS(ch, CLASS_DEMON))
                {
                        if (IS_DEMPOWER(ch, DEM_MIGHT))
                                dam *= 1.1;
                        if (IS_SET(ch->warp, WARP_STRONGARMS))
                                dam *= 1.2;
                }
                else if (IS_CLASS(ch, CLASS_HOBBIT))
                {
                        if (ch->pcdata->powers[HOBBIT_COMBAT] > 4)
                                dam *= 1.4;
                }
                else if (IS_CLASS(ch, CLASS_GIANT))
                        dam *=  1.3;  // 3xmult? who needs that much-xrak
		//	(1 + (1 * ch->pcdata->rank) / 5);    // 3 x multiplier max.
                else if (IS_CLASS(ch, CLASS_SHADOW))
                {
                        if (IS_SET
                            (ch->pcdata->powers[SHADOW_POWERS],
                             NSHADOWS_DPACT))
                                dam *= 1.6;
                        if (IS_SET
                            (ch->pcdata->powers[SHADOW_BITS],
                             NPOWER_SHADOWFORM))
                                dam /= 1.8;
                }
                else if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
                {
                        dam *= 1.4;
                        if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM
                            && ch->pcdata->powers[TIGER_LEVEL] > 1)
                                dam *= 1.5;
                        else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM
                                 && ch->pcdata->powers[FAERIE_LEVEL] > 1)
                                dam *= 1.2;
                        else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM
                                 && ch->pcdata->powers[HYDRA_LEVEL] > 1)
                                dam *= 1.6;
                        else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM
                                 && ch->pcdata->powers[BULL_LEVEL] > 1)
                                dam *= 1.7;
                }
                else if (IS_CLASS(ch, CLASS_DROID))
                {
                        if (IS_SET
                            (ch->pcdata->powers[DROID_IMPLANTS],
                             IMPLANT_STEAMENGINE))
                        {
                                dam *= 1.5;
                                if (IS_SET
                                    (ch->pcdata->powers[DROID_IMPLANTS],
                                     IMPLANT_TUNEDENGINE))
                                        dam *= 1.3;
                        }
                }
                else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                {
                        if (ch->pcdata->powers[WEAPONSKILL] > 4)
                                dam *= 1.2;
                        if (ch->pcdata->powers[WEAPONSKILL] > 8)
                                dam *= 1.3;
                }
                else if (IS_CLASS(ch, CLASS_BLADE_MASTER))
                {
                        if (ch->pcdata->powers[WEAPONSKILL] > 4)
                                dam *= 1.2;
                        if (ch->pcdata->powers[WEAPONSKILL] > 8)
                                dam *= 1.3;
                }

                else if (IS_CLASS(ch, CLASS_WEREWOLF)
                    && ch->power[DISC_WERE_BEAR] > 5)
                        dam *= 1.2;
        }
        return dam;
}

int cap_dam(CHAR_DATA * ch, CHAR_DATA * victim, int dam)
{
        if (is_safe(ch, victim))
                return 0;

        if (!IS_NPC(victim))
        {
                if (victim->tier == 1)
                        dam *= 0.95;
                else if (victim->tier == 2)
                        dam *= 0.90;
                else if (victim->tier == 3)
                        dam *= 0.85;
                else if (victim->tier == 4)
                        dam *= 0.80;
                else if (victim->tier == 5)
                        dam *= 0.75;
                /*
                 * 5% less damage per level 
                 */
        }

        if (!IS_NPC(ch) && !IS_NPC(victim))
                dam /= 2;
        if (IS_NPC(ch) && dam > 2000)
                dam = 2000 + (dam - 2000) / 2;

        if (IS_ITEMAFF(victim, ITEMA_RESISTANCE))
        {
                dam *= 75;
                dam /= 100;
        }
        if (IS_AFFECTED(victim, AFF_PROTECT) && IS_EVIL(ch))
        {
                dam *= 75;
                dam /= 100;
        }
        if (IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch))
        {
                dam *= 75;
                dam /= 100;
        }
        if (IS_AFF2(victim, AFF2_DARMOR) && rel_oppose(ch, victim))
        {
                dam *= 85;
                dam /= 100;
        }

        /*
         * Any special religion modifiers
         */
        if (rel_oppose(ch, victim))
        {
                if (IS_AFF2(victim, AFF2_DARMOR))
                {
                        dam *= 85;
                        dam /= 100;
                }
                if (victim->in_room)
                {
                        if (has_area_affect
                            (victim->in_room->area, AREA_AFF_GZONE,
                             victim->pcdata->religion))
                        {
                                dam *= 90;
                                dam /= 100;
                        }
                }
        }

	if (!IS_NPC(victim))
	{
          if (IS_CLASS(victim, CLASS_DROW))
          {
            if (IS_SET(victim->pcdata->powers[1], DPOWER_TOUGHSKIN)) dam /= 3;
          }
          else if (IS_CLASS(victim, CLASS_SHADOW))
          {
            if (IS_SET(victim->pcdata->powers[SHADOW_POWERS], NSHADOWS_TPACT))
            {
              dam *= 75;
              dam /= 100;
            }
            if (IS_CLASS(ch, CLASS_GIANT)) dam *= .85;    // evil giants
            if (IS_SET(victim->pcdata->powers[SHADOW_BITS], NPOWER_SHADOWFORM)) dam /= 1.75;
          }
	 else if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ELEMENTAL) && IS_SET(victim->pcdata->powers[ELEM_FORMS], EARTH_FORM))
	  {
	  dam /=1.4;
	  }
	 else if (IS_SET(victim->newbits2, NEW2_TOUGHSKIN))
	 {
	 dam /=1.3;
	 }


	  else if (!IS_NPC(victim) && victim->pcRaceLevel > 1)
	  {
          if (victim->pcRaceLevel >= 1 && victim->pcRaceLevel <= 19) dam /= 1.1;
	  else if (victim->pcRaceLevel >= 20) dam /= 1.2;
	  else if (victim->pcRaceLevel >= 50) dam /= 1.3;
          else if (victim->pcRaceLevel >= 100) dam /= 1.4;
          else if (victim->pcRaceLevel >= 150) dam /= 1.5;
          else if (victim->pcRaceLevel >= 190) dam /= 1.6;
	  }



          else if (IS_CLASS(victim, CLASS_THIEF))
          {
            if (IS_SET(victim->pcdata->powers[1], THIEF_TOUGHSKIN)) dam /= 1.2;
          }
          else if (IS_CLASS(victim, CLASS_LICH))
          {
            dam /= 5;
          }
          else if (IS_CLASS(victim, CLASS_DRONE))
          {
            dam *= 100 - victim->pcdata->powers[DRONE_FORTITUDE] * 25;
            dam /= 100;
          }
          else if (IS_CLASS(victim, CLASS_HOBBIT))
          {
            dam *= 110 - victim->pcdata->powers[HOBBIT_DISHES] * 4.75;
            dam /= 100;
          }
          else if (IS_CLASS(victim, CLASS_FAE))
          {
            dam /= 3;
          }
          else if (IS_CLASS(victim, CLASS_GIANT))
          {
            if (IS_SET(victim->pcdata->powers[GIANT_GIFTS], GGIFT_LEATHERSKIN)) dam *= 0.90;
            if (victim->pcdata->powers[GIANT_STANDFIRM] == 1)
            {
              if (victim->pcdata->stats[GIANT_POINTS] < dam / 10) do_standfirm(victim, "");
              else
              {
                victim->pcdata->stats[GIANT_POINTS] -= dam / 10;
                dam *= 0.85;
              }
            }
          }
          else if (IS_CLASS(victim, CLASS_ANGEL))
          {
            dam *= (100 - (victim->pcdata->powers[ANGEL_HARMONY] * 12));
            dam /= 100;
          }
          else if (IS_CLASS(victim, CLASS_PALADIN))
          {
            dam *= (100 - (victim->pcdata->powers[PALADIN_COMBAT] * 12));
            dam /= 100;
          }
          else if (IS_CLASS(victim, CLASS_ELEMENTAL))
          {
            dam *= (100 - (victim->pcdata->powers[ELEMENTAL_EARTH] * 12));
            dam /= 100;
          }
	  else if (IS_CLASS(victim, CLASS_DRAGON)) dam *= 0.70;
	  else if (IS_CLASS(victim, CLASS_ZOMBIE)) dam *= 0.80;
	  else if (IS_CLASS(victim, CLASS_SHINOBI)) dam *= 0.75;
          else if (IS_CLASS(victim, CLASS_JEDI))
          {
            if (victim->pcdata->powers[JEDI_STATE] > 5) dam *= .75;
          }
          else if (IS_CLASS(victim, CLASS_DRACONIAN))
	  {
            if (IS_SET(victim->newbits2, NEW2_SCALES)) dam *= 0.8;
	  }
          else if (IS_CLASS(victim, CLASS_TANARRI))
          {
            if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_EXOSKELETON)) dam *= 0.20;
            if (IS_CLASS(ch, CLASS_ANGEL)) dam *= 0.75;    /* trained to kill angels */
            if (IS_CLASS(ch, CLASS_CYBORG)) dam *= 0.8;
            if (IS_CLASS(ch, CLASS_DROID)) dam *= 0.8;
          }
          else if (IS_CLASS(victim, CLASS_DROID))
          {
            if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) dam *= 0.85;
            if (IS_CLASS(ch, CLASS_SAMURAI)) dam *= 0.9;
            if (IS_SET(victim->pcdata->powers[DROID_IMPLANTS], IMPLANT_BODYARMOR)) dam /= 2;
          }
          else if (IS_CLASS(victim, CLASS_CYBORG))
          {
            if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) dam *= 0.85;
            if (IS_CLASS(ch, CLASS_SAMURAI)) dam *= 0.9;
            if (victim->pcdata->powers[CYBORG_BODY] > 0) dam /= 1.75;
          }
          else if (IS_CLASS(victim, CLASS_VAMPIRE))
          {
            if (victim->power[DISC_VAMP_PROT] >= 10) dam /= 2;
            if (victim->power[DISC_VAMP_FORT] > 0)
            {
              dam *= (100 - (victim->power[DISC_VAMP_FORT] * 6));
              dam /= 100;
            }
          }
          else if (IS_CLASS(victim, CLASS_SAMURAI))
          {
            if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) dam /= 1.6; /* damn shapies */
            dam /= 1.75;
          }
          else if (IS_CLASS(victim, CLASS_MAGE))
          {
                if (IS_ITEMAFF(victim, ITEMA_STEELSHIELD))
                        dam /= 1.8;
                if (IS_ITEMAFF(victim, ITEMA_BEAST))
                        dam /= 1.8;
                if (IS_CLASS(ch, CLASS_GIANT))
                        dam /= 1.5; // giants fear magic
          }
          else if (IS_CLASS(victim, CLASS_SHAPESHIFTER))
          {
                dam /= 2.5;
                if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
                {
                        dam *= (100 -
                                victim->pcdata->powers[FAERIE_LEVEL] * 9);
                        dam /= 100;
                }
                else if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
                {
                        dam *= (100 -
                                victim->pcdata->powers[TIGER_LEVEL] * 8);
                        dam /= 100;
                        if (victim->pcdata->powers[PHASE_COUNTER] > 0)
                        {
                                dam *= (100 -
                                        victim->pcdata->
                                        powers[PHASE_COUNTER] * 2.5);
                                dam /= 100;
                        }
                }
                else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
                {
                        dam *= (100 -
                                victim->pcdata->powers[HYDRA_LEVEL] * 10);
                        dam /= 100;
                }
                else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM)
                {
                        dam *= (100 -
                                victim->pcdata->powers[BULL_LEVEL] * 12);
                        dam /= 100;
                }
          }
          else if (IS_CLASS(victim, CLASS_MONK))
          {
                if (victim->monkab[BODY] >= 2)
                        dam /= 1.5;
                if (victim->chi[CURRENT] > 0)
                {
                        dam *= (100 - (victim->chi[CURRENT] * 7));
                        dam /= 100;
                }
          }
          else if (IS_CLASS(victim, CLASS_DEMON))
          {
                if (victim->power[DISC_DAEM_IMMU] > 0)
                {
                        dam *= (100 - (victim->power[DISC_DAEM_IMMU] * 4));
                        dam /= 100;
                }
                if (IS_DEMPOWER(victim, DEM_TOUGH))
                        dam *= 0.8;
                if (IS_SET(victim->warp, WARP_STAMINA))
                        dam *= 0.8;
                if (IS_SET(victim->warp, WARP_SBODY))
                        dam *= 0.8;
                if (IS_SET(victim->warp, WARP_CBODY))
                        dam *= 0.8;
          }
           else if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
          {
                if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
                        dam *= 0.75;
                if (IS_CLASS(ch, CLASS_SAMURAI))
                        dam *= 0.7;
                dam *= (100 - (victim->pcdata->powers[UNDEAD_SPIRIT] * 6.3));
                dam /= 100;
          }
	  else if (IS_CLASS(victim, CLASS_BLADE_MASTER))
	{
		dam *= (100 - (victim->pcdata->powers[BLADE_MELEE] * 3.2));
		dam /= 100;
	}
          else if (IS_CLASS(victim, CLASS_NINJA))
          {
                if (victim->pcdata->powers[NPOWER_CHIKYU] >= 1)
                        dam /= 2.2;
          }
          else if (IS_CLASS(victim, CLASS_WEREWOLF))
          {
                if (victim->power[DISC_WERE_BOAR] > 2)
                        dam /= 2.3;
                if (IS_GAR1(victim, WOLF_COCOON))
                        dam /= 2.5;
          }
	}

        if (dam >= 2000)
                dam = dam - (dam / number_range(3, 10) +
                             number_range(10, 30));
        if (!IS_NPC(victim))
        {
                dam = dam + (dam / number_range(2, 5) + number_range(10, 50));
                dam *= (number_range(2, 4) * number_range(2, 3) /
                        number_range(4, 6));
        }

        if (IS_CLASS(ch, CLASS_JEDI))
        {
                if ((ch->generation == 1) && IS_SET(ch->special, SPC_PRINCE))
                        dam *= 3.5;
                else if (ch->generation == 1)
                        dam *= 2.7;
                else
                        dam *= 2.3;
                if (ch->pcdata->powers[JPOWER_SPIRIT] > 4)
                        dam *= 1.5;

                if (ch->pcdata->powers[JEDI_STATE] > 0)
                        dam *= 1 + ((ch->pcdata->powers[JEDI_STATE]) * .15);
        }


        if (dam <= 0)
                dam = 1;
        if (dam > 1000000)
                dam = 1000000;
        if (dam > ch->damcap[DAM_CAP])
                dam = number_range((ch->damcap[DAM_CAP] - 200),
                                   (ch->damcap[DAM_CAP] + 100));
        if (IS_AFFECTED(victim, AFF_SANCTUARY))
                dam *= 0.5;
	if ( IS_SET(victim->in_room->shinobi, ROOM_SEARTH) && IS_CLASS(victim,CLASS_SHINOBI) && victim->pcdata->powers[S_ELEMENT] != S_WIND) dam *= 0.9;
	if ( !IS_NPC(victim) && IS_CLASS(victim,CLASS_SHINOBI) && victim->pcdata->powers[S_ELEMENT] == S_EARTH) dam *= .8;

        if (IS_AFFECTED(victim, AFF_HOLYAURA) && dam > 1)
                dam *= 0.5;
        return dam;
}

bool can_counter(CHAR_DATA * ch)
{
        if (!IS_NPC(ch) && IS_STANCE(ch, STANCE_WOLVERINE))
                return TRUE;
        return FALSE;
}

bool can_bypass(CHAR_DATA * ch, CHAR_DATA * victim)
{
        if (IS_NPC(ch) || IS_NPC(victim))
                return FALSE;
        else if (IS_STANCE(ch, STANCE_VIPER))
                return TRUE;
        else if (IS_STANCE(ch, STANCE_HAWK))
                return TRUE;
        else if (IS_STANCE(ch, STANCE_COUGAR))
                return TRUE;
        else if (IS_STANCE(ch, STANCE_WOLF))
                return TRUE;
        else if (ch->stance[0] > 12
                 && IS_SET(ch->stance[(ch->stance[0] + 6)],
                           STANCEPOWER_BYPASS))
                return TRUE;
        return FALSE;
}

void update_damcap(CHAR_DATA * ch, CHAR_DATA * victim)
{
        int       max_dam = 1000;
        int       i = 0;

        if (!IS_NPC(ch))
        {
          if (ch->pcdata->relrank > 0)
                        max_dam += ch->pcdata->relrank * 100;
          if (ch->generation == 1)
                        max_dam += 350;
          else if (ch->generation == 2)
                        max_dam += 200;
          else if (ch->generation == 3)
                        max_dam += 150;
          else if (ch->generation == 4)
                        max_dam += 100;
          else if (ch->generation == 5)
                        max_dam += 50;
          if (ch->tier > 0)
                        max_dam += ch->tier * 300;

	  max_dam += (ch->pcRaceLevel * 5);

   switch( ch->pRank )
{
      case  0: max_dam +=   50; break;             
      case  1: max_dam +=  100; break;
      case  2: max_dam +=  150; break;
      case  3: max_dam +=  200; break;
      case  4: max_dam +=  250; break;
      case  5: max_dam +=  300; break;
      case  6: max_dam +=  350; break;
      case  7: max_dam +=  400; break;
      case  8: max_dam +=  450; break;
      case  9: max_dam +=  500; break;
      case 10: max_dam +=  550; break;
      case 11: max_dam +=  600; break;
      case 12: max_dam +=  650; break;
      case 13: max_dam +=  700; break;
      case 14: max_dam +=  720; break;
      case 15: max_dam +=  740; break;
      case 16: max_dam +=  760; break;
      case 17: max_dam +=  800; break;
      case 18: max_dam +=  840; break;
      case 19: max_dam +=  860; break;
      case 20: max_dam +=  880; break;
      case 21: max_dam +=  900; break;
      default: break;
   }


          if (IS_CLASS(ch, CLASS_HOBBIT))
          {
                        max_dam += ch->pcdata->powers[HOBBIT_COMBAT] * 170;
                        max_dam += ch->pcdata->powers[HOBBIT_DISHES] * 100;
          }
          else if (IS_CLASS(ch, CLASS_GIANT))
                {
                        max_dam += 2500;
                        max_dam += ch->pcdata->rank * 200;
                }
          else if (IS_CLASS(ch, CLASS_DRONE))
                {
                        max_dam +=
                                ch->pcdata->powers[DRONE_HORRID_REALITY] *
                                110;
                        max_dam +=
                                ch->pcdata->powers[DRONE_DISTORTED_REALITY] *
                                110;
                        max_dam +=
                                ch->pcdata->powers[DRONE_ALTERNATE_REALITY] *
                                110;
                        max_dam += ch->pcdata->powers[DRONE_EVASION] * 100;
                        max_dam += ch->pcdata->powers[DRONE_FORTITUDE] * 100;
                        max_dam += ch->pcdata->powers[DRONE_FOCUS] * 100;
                        max_dam += ch->pcdata->powers[DRONE_WILLPOWER] * 110;
                        max_dam +=
                                ch->pcdata->powers[DRONE_MINDCONTROL] * 110;
                        max_dam += ch->pcdata->powers[DRONE_PSIONICS] * 110;
                }
          else if (IS_CLASS(ch, CLASS_MAGE))
                {
                        if (IS_ITEMAFF(ch, ITEMA_BEAST))
                                max_dam += 750;
                        max_dam += 500;
                }
          else if (IS_CLASS(ch, CLASS_PRIEST))
                {
                        max_dam += 1600;
                        if (IS_PRIPOWER(ch, PRI_PFORM))
                                max_dam += 1500;
                }
          else if (IS_CLASS(ch, CLASS_JEDI))
                {
                        if ((ch->generation == 1)
                            && IS_SET(ch->special, SPC_PRINCE))
                                max_dam = 3600;
                        else if (ch->generation == 1)
                                max_dam = 3300;
                        else
                                max_dam = 2800;
                        max_dam +=
                                ((ch->pcdata->powers[JPOWER_BODY] +
                                  ch->pcdata->powers[JPOWER_MIND] +
                                  ch->pcdata->powers[JPOWER_SPIRIT] +
                                  ch->pcdata->powers[JPOWER_TECHNIQUE]) * 50);
                }
          else if (IS_CLASS(ch, CLASS_TANARRI))
                {
                        if (IS_SET
                            (ch->pcdata->powers[TANARRI_POWER],
                             TANARRI_MIGHT))
                                max_dam += 500;
                        max_dam += ch->pcdata->rank * 375;  /* max rank is 6 btw */
                }
          else if (IS_CLASS(ch, CLASS_FAE))
                {
                        max_dam += 500;
                        if (ch->pcdata->powers[FAE_PLASMA] > 7)
                                max_dam += 270;
                        if (ch->pcdata->powers[FAE_MATTER] > 7)
                                max_dam += 270;
                        if (ch->pcdata->powers[FAE_ENERGY] > 7)
                                max_dam += 270;
                        if (ch->pcdata->powers[FAE_WILL] > 7)
                                max_dam += 270;
                        if (ch->pcdata->powers[FAE_PLASMA] > 3)
                                max_dam += 270;
                        if (ch->pcdata->powers[FAE_MATTER] > 3)
                                max_dam += 270;
                        if (ch->pcdata->powers[FAE_ENERGY] > 3)
                                max_dam += 270;
                        if (ch->pcdata->powers[FAE_WILL] > 3)
                                max_dam += 270;
                }
          else if (IS_CLASS(ch, CLASS_LICH))
                {
                        max_dam += 450;
                        if (ch->pcdata->powers[CON_LORE] > 4)
                                max_dam += 320;
                        if (ch->pcdata->powers[DEATH_LORE] > 4)
                                max_dam += 320;
                        if (ch->pcdata->powers[LIFE_LORE] > 4)
                                max_dam += 320;
                        if (ch->pcdata->powers[NECROMANTIC] > 4)
                                max_dam += 320;
                        if (ch->pcdata->powers[CHAOS_MAGIC] > 4)
                                max_dam += 320;
                }
          else if (IS_CLASS(ch, CLASS_SHADOW))
                {
                        max_dam += ch->pcdata->powers[SHADOW_MARTIAL] * 80;
                }

                //Ghouls
          else if (IS_CLASS(ch, CLASS_GHOUL))
                {
                        if (ch->pcdata->powers[GPOWER_UNDEADMIGHT] > 0)
                                max_dam += 400;
                        if (ch->pcdata->powers[GPOWER_UNDEADMIGHT] > 2)
                                max_dam += 400;
                        if (ch->pcdata->powers[GPOWER_UNDEADMIGHT] > 3)
                                max_dam += 800;
                        if (ch->pcdata->powers[GPOWER_UNDEADMIGHT] > 4)
                                max_dam += 1800;
                }
          else if (IS_CLASS(ch, CLASS_DRACONIAN))
                {
                        if (ch->pcdata->powers[ORB_GREEN] >= 3)
                                max_dam += 900;
                        if (IS_POLYAFF(ch, POLY_LIZARDFORM))
                                max_dam += 1700;
                        if (ch->pcdata->stats[DRACONIAN_SPIKES] > 0)
                                max_dam +=
                                        ch->pcdata->stats[DRACONIAN_SPIKES] *
                                        2;
                }
          else if (IS_CLASS(ch, CLASS_THIEF)
                    && IS_SET(ch->pcdata->powers[1], THIEF_MIGHT))
                {
                        max_dam += 2750;
                }
          else if (IS_CLASS(ch, CLASS_SKYBLADE))
                {
                        if (ch->pcdata->powers[SKYBLADE_POWER] >= 1)
                                max_dam += 50;
                        if (ch->pcdata->powers[SKYBLADE_POWER] >= 2)
                                max_dam += 75;
                        if (ch->pcdata->powers[SKYBLADE_POWER] >= 3)
                                max_dam += 100;
                        if (ch->pcdata->powers[SKYBLADE_POWER] >= 4)
                                max_dam += 100;
                        if (ch->pcdata->powers[SKYBLADE_POWER] >= 5)
                                max_dam += 150;
                        if (ch->pcdata->powers[SKYBLADE_POWER] >= 6)
                                max_dam += 250;
                        if (ch->pcdata->powers[SKYBLADE_POWER] >= 7)
                                max_dam += 400;
                        if (ch->pcdata->powers[SKYBLADE_POWER] >= 8)
                                max_dam += 500;
                        if (ch->pcdata->powers[SKYBLADE_POWER] >= 9)
                                max_dam += 600;
                        if (ch->pcdata->powers[SKYBLADE_POWER] == 10)
                                max_dam += 900;
                }
          else if (IS_CLASS(ch, CLASS_ANGEL))
                {
                        max_dam += ch->pcdata->powers[ANGEL_JUSTICE] * 155;
                        max_dam += ch->pcdata->powers[ANGEL_PEACE] * 155;
                        max_dam += ch->pcdata->powers[ANGEL_HARMONY] * 165;
                        max_dam += ch->pcdata->powers[ANGEL_LOVE] * 165;
                }
	  else if (IS_CLASS(ch, CLASS_PALADIN)) 
		{
                        max_dam += ch->pcdata->powers[PALADIN_POWER] * 80;
                        max_dam += ch->pcdata->powers[PALADIN_COMBAT] * 155;
                        max_dam += ch->pcdata->powers[PALADIN_TOUGHNESS] * 165;
                        max_dam += ch->pcdata->powers[PALADIN_SPEED] * 165;
		}
          else if (IS_CLASS(ch, CLASS_ELEMENTAL))
                {
                        max_dam += ch->pcdata->powers[ELEMENTAL_EARTH] * 155;
                        max_dam += ch->pcdata->powers[ELEMENTAL_FIRE] * 155;
                        max_dam += ch->pcdata->powers[ELEMENTAL_WATER] * 165;
                        max_dam += ch->pcdata->powers[ELEMENTAL_AIR] * 165;
                }
	  else if (IS_CLASS(ch, CLASS_DRAGON))
		{
		max_dam += (ch->pcdata->powers[dragonage] * 250);
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_WINGS )) max_dam += 145;
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FANGS )) max_dam += 155;
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TAIL ))  max_dam += 125;
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LUCK ))  max_dam += 125;
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CLAWS))  max_dam += 125;
		}
	  else if (IS_CLASS(ch, CLASS_ZOMBIE)) max_dam += 2000;
	  else if (IS_CLASS(ch,CLASS_SHINOBI)) max_dam += ch->chakra / 1.55;
	
          else if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
                {
                        max_dam += 800;
                        if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
                                max_dam +=
                                        ch->pcdata->powers[TIGER_LEVEL] * 325;
                        else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
                                max_dam +=
                                        ch->pcdata->powers[HYDRA_LEVEL] * 350;
                        else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
                                max_dam +=
                                        ch->pcdata->powers[BULL_LEVEL] * 300;
                        else if (ch->pcdata->powers[SHAPE_FORM] ==
                                 FAERIE_FORM)
                                max_dam +=
                                        ch->pcdata->powers[FAERIE_LEVEL] *
                                        275;
                }
          else if (IS_CLASS(ch, CLASS_DROW))
                {
                        max_dam += 1200;
                        if (IS_SET(ch->newbits, NEW_DROWHATE))
                                max_dam += 950;
                        if (IS_SET(ch->newbits, NEW_DFORM))
                                max_dam += 950;
                }
          else if (IS_CLASS(ch, CLASS_DEMON))
                {
                        max_dam += ch->power[DISC_DAEM_ATTA] * 60;
                        max_dam += 1300;
                        if (ch->pcdata->souls > 0)
                                max_dam += UMIN(350, 70 * ch->pcdata->souls);
		        if (ch->in_room != NULL && ch->in_room->vnum >= 93420
                            && ch->in_room->vnum <= 93426)
                                max_dam += 2000;
                        max_dam += (ch->pcdata->stats[DEMON_POWER] * 2);
                }
          else if (IS_CLASS(ch, CLASS_CYBORG))
                {
                        if (ch->pcdata->powers[CYBORG_LIMBS] > 0)
                                max_dam += 450;
                        if (ch->pcdata->powers[CYBORG_LIMBS] > 1)
                                max_dam += 450;
                        if (ch->pcdata->powers[CYBORG_LIMBS] > 2)
                                max_dam += 450;
                        if (ch->pcdata->powers[CYBORG_LIMBS] > 3)
                                max_dam += 450;
                        if (ch->pcdata->powers[CYBORG_LIMBS] > 4)
                                max_dam += 500;
                }
          else if (IS_CLASS(ch, CLASS_MONK))
                {
                        max_dam += (ch->monkab[COMBAT] * 250);
                        if (ch->chi[CURRENT] > 0)
                                max_dam += ch->chi[CURRENT] * 250;
                }
          else if (IS_CLASS(ch, CLASS_VAMPIRE))
                {
                        max_dam += (ch->rage * 5);
                        if (ch->power[DISC_VAMP_POTE] > 0)
                                max_dam += (ch->power[DISC_VAMP_POTE] * 150);
                        if (ch->pcdata->rank == AGE_TRUEBLOOD)
                                max_dam += 1150;
                        else if (ch->pcdata->rank == AGE_LA_MAGRA)
                                max_dam += 950;
                        else if (ch->pcdata->rank == AGE_METHUSELAH)
                                max_dam += 650;
                        else if (ch->pcdata->rank == AGE_ELDER)
                                max_dam += 250;
                        else if (ch->pcdata->rank == AGE_ANCILLA)
                                max_dam += 170;
                        if (ch->played > 0)
                        {
                                if ((ch->played / 3600) < 100)
                                {
                                        max_dam += ((ch->played / 3600) * 10);
                                }
                                else
                                {
                                        max_dam += 1000;
                                }
                        }
                }
          else if (IS_CLASS(ch, CLASS_NINJA))
                {
                        max_dam += (ch->rage * 5);
                        if (ch->pcdata->powers[NPOWER_CHIKYU] >= 6
                            && ch->pcdata->powers[HARA_KIRI] > 0)
                                max_dam += 900;
                        if (ch->pcdata->powers[NPOWER_CHIKYU] >= 2)
                                max_dam += 900;
                }
          else if (IS_CLASS(ch, CLASS_WEREWOLF))
                {
                        if (ch->rage > 99)
                        {
                                max_dam += ch->rage;
                                max_dam += 1500;
                        }
                        if (ch->power[DISC_WERE_PAIN] > 9)
                                max_dam += 850;
                }
          else if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                        max_dam += ch->pcdata->powers[WEAPONSKILL] * 220;
          else if (IS_CLASS(ch, CLASS_BLADE_MASTER))
                        max_dam += ch->pcdata->powers[WEAPONSKILL] * 210;
	  else if (IS_CLASS(ch, CLASS_BLADE_MASTER) && IS_SET(ch->newbits2, NEW2_DAMCAP))
			max_dam += 700;

          else if (IS_CLASS(ch, CLASS_SAMURAI)
                    && (get_eq_char(ch, WEAR_WIELD) != NULL))
                {
                        for (i = 0; i < 13; i++)
                                if (ch->wpn[i] >= 1000)
                                        max_dam += 325;
                        max_dam += 375;
                }
        }

        if (IS_ITEMAFF(ch, ITEMA_ARTIFACT))
                max_dam += 500;

        if (IS_NPC(victim) || victim->stance[0] != STANCE_WOLVERINE)
        {
                if (ch->stance[0] == STANCE_PITBULL)
                        max_dam += 200;
                else if (ch->stance[0] == STANCE_BADGER)
                        max_dam += 250;
                else if (ch->stance[0] == STANCE_WOLF)
                        max_dam += 250;
                else if (ch->stance[0] == STANCE_COUGAR)
                        max_dam += 200;
                else if (ch->stance[0] > 12
                         && IS_SET(ch->stance[(ch->stance[0] + 6)],
                                   STANCEPOWER_DAMCAP_3))
                        max_dam += 550;
                else if (ch->stance[0] > 12
                         && IS_SET(ch->stance[(ch->stance[0] + 6)],
                                   STANCEPOWER_DAMCAP_2))
                        max_dam += 400;
                else if (ch->stance[0] > 12
                         && IS_SET(ch->stance[(ch->stance[0] + 6)],
                                   STANCEPOWER_DAMCAP_1))
                        max_dam += 250;
        }

        if (!IS_NPC(victim) && ch->stance[0] != STANCE_WOLVERINE)
        {
                if (victim->stance[0] > 12
                    && IS_SET(victim->stance[(victim->stance[0] + 6)],
                              STANCEPOWER_REV_DAMCAP_3))
                        max_dam -= 550;
                else if (victim->stance[0] > 12
                         && IS_SET(victim->stance[(victim->stance[0] + 6)],
                                   STANCEPOWER_REV_DAMCAP_2))
                        max_dam -= 400;
                else if (victim->stance[0] > 12
                         && IS_SET(victim->stance[(victim->stance[0] + 6)],
                                   STANCEPOWER_REV_DAMCAP_1))
                        max_dam -= 250;
        }


        /*
         * Max Dam Reductions 
         */
        /*
         * Werewolves 
         */

        if (!IS_NPC(victim))
        {
                int       silver_tol = (victim->siltol * 2.5);

                if (IS_CLASS(victim, CLASS_WEREWOLF)
                    && IS_ITEMAFF(ch, ITEMA_RIGHT_SILVER))
                        max_dam += (250 - silver_tol);
                if (IS_CLASS(victim, CLASS_WEREWOLF)
                    && IS_ITEMAFF(ch, ITEMA_LEFT_SILVER))
                        max_dam += (250 - silver_tol);
                /*
                 * Vampies 
                 */
                if (!IS_NPC(ch) && (IS_CLASS(victim, CLASS_VAMPIRE)))
                        max_dam -= (victim->power[DISC_VAMP_FORT] * 75);
                if (IS_NPC(ch) || ch->stance[0] != STANCE_WOLVERINE)
                {
                        if (victim->stance[0] == STANCE_CRAB)
                                max_dam -= 250;
                        else if (victim->stance[0] == STANCE_BADGER)
                                max_dam -= 250;
                        else if (victim->stance[0] == STANCE_GRIZZLY)
                                max_dam -= 250;
                }
                if (IS_CLASS(victim, CLASS_DEMON)
                    && (victim->in_room->vnum >= 93420
                        && victim->in_room->vnum <= 93426))
                        max_dam *= 0.75;
        }
        max_dam += get_curr_str(ch) * 5;
        max_dam += (ch->race / 5) * 250;    /* This might be too much */

        if (ch->level >= LEVEL_BUILDER)
                max_dam = 30000;
        if (ch->level >= LEVEL_BUILDER && !IS_NPC(ch))
                max_dam = 100000;   // Det er pisse irreterende Thomas.. jeg kan intet teste mod mobs, de doer automatisk. DIE!
        ch->damcap[DAM_CAP] = max_dam;
        ch->damcap[DAM_CHANGE] = 0;
        return;
}


/*
 * Inflict damage from a hit.
 */
void damage(CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt)
{
        CHAR_DATA *emb;

        if (victim->position == POS_DEAD)
                return;
        if (ch->embracing != NULL)
        {
                if ((emb = ch->embracing) != NULL)
                        stop_embrace(ch, emb);
                else
                        stop_embrace(ch, NULL);
        }
        /*
         * Stop up any residual loopholes.
         */
        if (ch->damcap[DAM_CHANGE] == 1)
                update_damcap(ch, victim);
        if (dam > ch->damcap[0])
                dam = ch->damcap[0];

        /*
         * Certain attacks are forbidden.
         * Most other attacks are returned.
         */
        if (victim != ch)
        {
                if (is_safe(ch, victim))
                        return;
                if (victim->position > POS_STUNNED)
                {
                        if (victim->fighting == NULL)
                        {
                                set_fighting(victim, ch, FALSE);
                                if (IS_NPC(victim)
                                    && HAS_TRIGGER_MOB(victim, TRIG_KILL))
                                        p_percent_trigger(victim, NULL, NULL,
                                                          ch, NULL, NULL,
                                                          TRIG_KILL);
                        }
                        victim->position = POS_FIGHTING;
                }
                if (victim->position > POS_STUNNED)
                {
                        if (ch->fighting == NULL)
                                set_fighting(ch, victim, TRUE);
                        /*
                         * If victim is charmed, ch might attack victim's master.
                         */
                        if (IS_NPC(ch)
                            && IS_NPC(victim)
                            && IS_AFFECTED(victim, AFF_CHARM)
                            && victim->master != NULL
                            && victim->master->in_room == ch->in_room
                            && number_bits(3) == 0)
                        {
                                stop_fighting(ch, FALSE);
                                multi_hit(ch, victim->master, TYPE_UNDEFINED);
                                return;
                        }
                }
                if (victim->master == ch)
                {
                        if (IS_NPC(victim))
                        {
                                if (!
                                    (victim->pIndexData->vnum >= MOB_VNUM_FIRE
                                     && victim->pIndexData->vnum <=
                                     MOB_VNUM_CLAY))
                                        stop_follower(victim, FALSE);
                        }
                        else
                                stop_follower(victim, FALSE);
                }
                /*
                 * Damage modifiers.
                 */
                if (IS_AFFECTED(ch, AFF_HIDE))
                {
                        if (!can_see(victim, ch))
                        {
                                dam *= 1.5;
                                send_to_char
                                        ("You use your concealment to get a surprise attack!\n\r",
                                         ch);
                        }
                        REMOVE_BIT(ch->affected_by, AFF_HIDE);
                        act("$n leaps from $s concealment.", ch, NULL, NULL,
                            TO_ROOM);
                }
                if (IS_AFFECTED(victim, AFF_PROTECT) && IS_EVIL(ch)
                    && dam > 1)
                        dam -= dam * 0.25;
                if (IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch)
                    && dam > 1)
                        dam -= dam * 0.25;

/*                if (!IS_NPC(victim))
                {
                        if (victim->pcdata->damreduct == 100)
                                dam -= 0;
                        else if (victim->pcdata->damreductdec > 0)
                                dam = (dam *
                                       (victim->pcdata->damreduct + 1)) / 100;
                        else
                                dam = (dam * victim->pcdata->damreduct) / 100;
                }
*/

                if (dam < 0)
                        dam = 0;
                if (IS_NPC(victim) && IS_SET(victim->act, ACT_NODAMAGE))
                {
                        dam = 0;
                }
                /*
                 * Check for disarm, trip, parry, and dodge.
                 */
                if (dt >= TYPE_HIT)
                {
                        if (IS_NPC(ch) && number_percent() < ch->level * 0.5)
                                disarm(ch, victim);
                        if (IS_NPC(ch) && number_percent() < ch->level * 0.5)
                                trip(ch, victim);
                        if (check_dodge(ch, victim, dt))
                                return;
                        if (!IS_NPC(victim)
                            && IS_STANCE(victim, STANCE_MONGOOSE)
                            && victim->stance[STANCE_MONGOOSE] > 100
                            && !can_counter(ch) && !can_bypass(ch, victim)
                            && check_dodge(ch, victim, dt))
                                return;
                        else if (!IS_NPC(victim)
                                 && IS_STANCE(victim, STANCE_GRIZZLY)
                                 && victim->stance[STANCE_GRIZZLY] > 100
                                 && !can_counter(ch)
                                 && !can_bypass(ch, victim)
                                 && check_dodge(ch, victim, dt))
                                return;

/* ------------ This is the part for superstances, Jobo ------------------- */
                        /*
                         * else if (!IS_NPC(victim) && victim->stance[0] > 12
                         * && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_DODGE)
                         * && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch)
                         * && !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
                         * return; 
                         */

                        else if (!IS_NPC(victim) && victim->stance[0] > 12
                                 && IS_SET(victim->
                                           stance[(victim->stance[0] + 6)],
                                           STANCEPOWER_DODGE)
                                 && victim->stance[(victim->stance[0])] > 100
                                 && !can_bypass(ch, victim)
                                 && check_dodge(ch, victim, dt))
                                return;
/* ------------ This is the end for superstances, Jobo ------------------- */

                        if (check_parry(ch, victim, dt))
                                return;
                        if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_CRANE)
                            && victim->stance[STANCE_CRANE] > 100
                            && !can_counter(ch) && !can_bypass(ch, victim)
                            && check_parry(ch, victim, dt))
                                return;
                        else if (!IS_NPC(victim)
                                 && IS_STANCE(victim, STANCE_HAWK)
                                 && victim->stance[STANCE_HAWK] > 100
                                 && !can_counter(ch)
                                 && !can_bypass(ch, victim)
                                 && check_parry(ch, victim, dt))
                                return;

/* ------------ This is the part for superstances, Jobo ------------------- */
                        /*
                         * else if (!IS_NPC(victim) && victim->stance[0] > 12
                         * && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_PARRY)
                         * && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch)
                         * && !can_bypass(ch,victim) && check_parry( ch, victim, dt ))
                         * return; 
                         */

                        else if (!IS_NPC(victim) && victim->stance[0] > 12
                                 && IS_SET(victim->
                                           stance[(victim->stance[0] + 6)],
                                           STANCEPOWER_PARRY)
                                 && victim->stance[(victim->stance[0])] > 100
                                 && !can_bypass(ch, victim)
                                 && check_parry(ch, victim, dt))
                                return;
/* ------------ This is the end for superstances, Jobo ------------------- */

                }
                dam_message(ch, victim, dam, dt);

                /*
                 * Brief 5 System - Jobo
                 */
                if (!IS_NPC(ch) && IS_SET(ch->act, PLR_BRIEF5))
                {
                        ch->amount_damage_dealt += dam;
                        ch->amount_attacks_dealt++;
                }
                if (!IS_NPC(victim) && IS_SET(victim->act, PLR_BRIEF5))
                {
                        victim->amount_damage_recieved += dam;
                        victim->amount_attacks_recieved++;
                }
        }
        if (IS_CLASS(victim, CLASS_MONK) && victim->monkab[SPIRIT] >= 2)
        {
                if (IS_SET(victim->newbits, NEW_NATURAL))
                {
                        stc("You absorb the natural imbalances of your opponent.\n\r", victim);
                        act("$n absorbs the natural imbalances of you attack.", victim, NULL, ch, TO_VICT);
                        if (dam > 2000)
                                dam /= 2;
                        victim->hit += dam;
                        REMOVE_BIT(victim->newbits, NEW_NATURAL);
                        return;
                }
        }
        if (IS_SET(victim->newbits, NEW_IRONMIND))
        {
                REMOVE_BIT(victim->newbits, NEW_IRONMIND);
                send_to_char
                        ("Your focus your full concentration on the attack,\n\r",
                         victim);
                send_to_char
                        ("and absorb the full impact into your system, channeling it into a healing force.\n\r",
                         victim);
                victim->hit += dam;
                dam = 0;
        }
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DRONE))
        {
                if (IS_SET(victim->pcdata->jflags, JFLAG_DRONE_DISTORT)
                    && number_range(1, 3) != 2)
                {
                        act("$N seems healed by your attack, as if reality was suddenly turned upside down.", ch, NULL, victim, TO_CHAR);
                        act("$n's attack was reversed by your shift in reality, healing you instead.", ch, NULL, victim, TO_VICT);
                        act("$n's attack seems to have no effect on $N, actually healing $M instead.", ch, NULL, victim, TO_NOTVICT);
                        victim->hit += dam;
                        dam = 0;
                }
        }
        hurt_person(ch, victim, dam);
        dropinvis(ch);
        dropinvis(victim);
        return;
}

void fae_shield(CHAR_DATA * ch, CHAR_DATA * victim, int dam)
{
        char      buf[MAX_STRING_LENGTH];
        char      buf2[MAX_STRING_LENGTH];
        int       new_dam;

        if (victim->pcdata->powers[FAE_SHIELD] > 0)
        {
                if (--victim->pcdata->powers[FAE_SHIELD] == 0)
                        send_to_char
                                ("#pYour shield of matter flickers and dies.#n\n\r",
                                 victim);
                new_dam = number_range(dam / 4, dam / 5);
                if (new_dam > 300)
                        new_dam = number_range(275, 325);
                if (new_dam >= victim->hit)
                        new_dam = victim->hit - 1;
                if (new_dam < 10)
                        new_dam = 10;
                hurt_person(victim, ch, new_dam);
                xprintf(buf,
                        "#pA flickering shield of matter strikes you hard#n [#C%d#n]",
                        new_dam);
                xprintf(buf2,
                        "#pYour mattershield returns some of the damage#n [#C%d#n]",
                        new_dam);

                /*
                 * Brief 5 System - Jobo
                 */
                if (!IS_NPC(victim) && IS_SET(victim->act, PLR_BRIEF5))
                {
                        victim->amount_damage_dealt += new_dam;
                        victim->amount_attacks_dealt++;
                }
                if (!IS_NPC(ch) && IS_SET(ch->act, PLR_BRIEF5))
                {
                        ch->amount_damage_recieved += dam;
                        ch->amount_attacks_recieved++;
                }
                if (!IS_SET(ch->act, PLR_BRIEF5))
                        act(buf, victim, NULL, ch, TO_VICT);
                if (!IS_SET(victim->act, PLR_BRIEF5))
                        act(buf2, victim, NULL, ch, TO_CHAR);
        }
        return;
}

void angel_eye(CHAR_DATA * ch, CHAR_DATA * victim, int dam)
{
        char      buf[MAX_STRING_LENGTH];
        char      buf2[MAX_STRING_LENGTH];
        int       new_dam;

        if (IS_SET(victim->pcdata->powers[ANGEL_POWERS], ANGEL_EYE)
            && dam > 100)
        {
                new_dam = number_range(dam / 4, dam / 5);
                if (new_dam > 300)
                        new_dam = number_range(275, 325);
                if (new_dam >= victim->hit)
                        new_dam = victim->hit - 1;
                if (new_dam < 10)
                        new_dam = 10;
                hurt_person(victim, ch, new_dam);
                xprintf(buf, "God's justice strikes you HARD [#C%d#n]",
                        new_dam);
                xprintf(buf2,
                        "The sinner dares to lay hands on you, God's wrath on the sinner [#C%d#n]",
                        new_dam);

                /*
                 * Brief 5 System - Jobo
                 */
                if (!IS_NPC(victim) && IS_SET(victim->act, PLR_BRIEF5))
                {
                        victim->amount_damage_dealt += new_dam;
                        victim->amount_attacks_dealt++;
                }
                if (!IS_NPC(ch) && IS_SET(ch->act, PLR_BRIEF5))
                {
                        ch->amount_damage_recieved += dam;
                        ch->amount_attacks_recieved++;
                }
                if (!IS_SET(ch->act, PLR_BRIEF5))
                        act(buf, victim, NULL, ch, TO_VICT);
                if (!IS_SET(victim->act, PLR_BRIEF5))
                        act(buf2, victim, NULL, ch, TO_CHAR);
        }
        return;
}

void elemental_enflame( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  int new_dam;

  if (IS_SET(victim->pcdata->powers[ELEMENTAL_POWERS], ELEMENTAL_ENFLAME) && dam > 100)
  {
    new_dam = number_range(dam/4,dam/5);
    if (new_dam > 300) new_dam = number_range(275,325);
    if (new_dam >= victim->hit) new_dam = victim->hit -1;
    if (new_dam < 10) new_dam = number_range(275,325);
    hurt_person(victim, ch, new_dam);
    sprintf(buf, "The Elemental Flames DEVESTATE you [#C%d#n]",new_dam);
    sprintf(buf2, "You seek RETRIBUTION! [#C%d#n]", new_dam);
    act(buf, victim, NULL, ch, TO_VICT);
    act(buf2, victim, NULL, ch, TO_CHAR);
  }
  return;
}


void hurt_person(CHAR_DATA * ch, CHAR_DATA * victim, int dam)
{
        OBJ_DATA *familiar;
        char      buf[MAX_STRING_LENGTH];
        char      buf2[MAX_STRING_LENGTH];
        int       newdam;
	int       dam2 = 0;

        if (IS_NPC(victim) && IS_SET(victim->act, ACT_NODAMAGE))
                dam = 0;

        victim->hit -= dam;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SKYBLADE)
            && victim->pcdata->powers[SKYBLADE_SKILLS] >= 5)
        {
                if (dam >= 2000)
                        newdam = number_range(dam, dam * 2);
                else
                        newdam = number_range(dam / 2, dam);
                if (number_range(1, 50) > 48)
                {
                        hurt_person(ch, victim, newdam);
                        do_say(victim, "#sEnguarde you swine!#n");
                        xprintf(buf,
                                "#Y%s retorts and hits you hard! #0[#R%d#0]#n",
                                victim->name, newdam);
                        xprintf(buf2,
                                "#YYou retort the attack harshly! #0[#R%d#0]#n",
                                newdam);
                        act(buf, victim, NULL, ch, TO_VICT);
                        act(buf2, victim, NULL, ch, TO_CHAR);
                }
        }

        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL)
            && !IS_CLASS(ch, CLASS_ANGEL))
                angel_eye(ch, victim, dam); // check for that eye for an eye.
	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ELEMENTAL) 
	    && !IS_CLASS(ch, CLASS_ELEMENTAL))
		elemental_enflame(ch,victim,dam); 
	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHINOBI) && 
	IS_SET(victim->pcdata->powers[S_ACTIVE], S_TAIEGRA))
	{
	dam2 = dam * .1;
	sprintf(buf,"%s's shield of flame burns %s! [#R%d#n]\n\r",GET_PROPER_NAME(victim),GET_PROPER_NAME(ch),dam2);
	stc(buf,ch);
	stc(buf,victim);
	ch->hit -= dam2;
	}

        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_FAE)
            && !IS_CLASS(ch, CLASS_FAE))
                fae_shield(ch, victim, dam);
        if (!IS_NPC(victim) && victim->level >= LEVEL_IMMORTAL
            && victim->hit < 1)
                victim->hit = 1;
        if (!IS_NPC(victim) && IS_SET(victim->newbits, NEW_CLOAK)
            && victim->hit < 1)
        {
                victim->hit += UMIN((victim->max_hit * .1), 4000);
                if (IS_CLASS(victim, CLASS_MONK))
                        send_to_char
                                ("your cloak of life saves your from certain death.\n\r",
                                 victim);
                else
                        send_to_char
                                ("your cloak of death prevents the destruction of your body.\n\r",
                                 victim);
                send_to_char("#0They escape your final blow.#n\n\r", ch);
                REMOVE_BIT(victim->newbits, NEW_CLOAK);
        }
        update_pos(victim);

        switch (victim->position)
        {
        case POS_MORTAL:
                act("$n is mortally wounded, and spraying blood everywhere.",
                    victim, NULL, NULL, TO_ROOM);
                send_to_char
                        ("You are mortally wounded, and spraying blood everywhere.\n\r",
                         victim);
		if(!IS_NPC(victim) && IS_CLASS(victim,CLASS_ZOMBIE)) WAIT_STATE(victim, 12);

                if ((familiar = get_eq_char(victim, WEAR_FAMILIAR)) != NULL
                    && familiar->item_type == ITEM_FAMILIAR
                    && familiar->value[0] == 2 && (number_percent() > 80))
                {
                        sprintf(buf, "%s squeals, 'Shit!  Get up bitch!'\n\r",
                                familiar->short_descr);
                        stc(buf, victim);
                        act("$p squeals, 'Shit!  Get up bitch!'", NULL,
                            familiar, NULL, TO_ROOM);
                        sprintf(buf, "%s #Cslaps #7you!#n\n\r",
                                familiar->short_descr);
                        stc(buf, victim);
                        //act("$p heals $N!", ch, familiar, victim, TO_ROOM);
                        //victim->hit +=
                        //      number_range((familiar->value[1] * 500),
                        //                     (familiar->value[1] * 1000));
                        if (victim->hit > victim->max_hit)
                                victim->hit = victim->max_hit;
                        update_pos(victim);
                }


                break;
        case POS_INCAP:
                act("$n is incapacitated, and bleeding badly.", victim, NULL,
                    NULL, TO_ROOM);
                send_to_char("You are incapacitated, and bleeding badly.\n\r",
                             victim);

                if ((familiar = get_eq_char(victim, WEAR_FAMILIAR)) != NULL
                    && familiar->item_type == ITEM_FAMILIAR
                    && familiar->value[0] == 2 && (number_percent() > 80))
                {
                        sprintf(buf, "%s squeals, 'Shit!  Get up bitch!'\n\r",
                                familiar->short_descr);
                        stc(buf, victim);
                        act("$p squeals, 'Shit!  Get up bitch!'", NULL,
                            familiar, NULL, TO_ROOM);
                        sprintf(buf, "%s #Cslaps #7you!#n\n\r",
                                familiar->short_descr);
                        stc(buf, victim);
                        act("$p slaps $N!", ch, familiar, victim, TO_ROOM);
                        //victim->hit +=
                        //        number_range((familiar->value[1] * 500),
                        //                     (familiar->value[1] * 1000));
                        if (victim->hit > victim->max_hit)
                                victim->hit = victim->max_hit;
                        update_pos(victim);
                }

                break;
        case POS_STUNNED:
                act("$n is stunned, but will soon recover.", victim, NULL,
                    NULL, TO_ROOM);
                send_to_char("You are stunned, but will soon recover.\n\r",
                             victim);
                if ((familiar = get_eq_char(victim, WEAR_FAMILIAR)) != NULL
                    && familiar->item_type == ITEM_FAMILIAR
                    && familiar->value[0] == 2 && (number_percent() > 80))
                {
                        sprintf(buf, "%s squeals, 'Shit!  Get up bitch!'\n\r",
                                familiar->short_descr);
                        stc(buf, victim);
                        act("$p squeals, 'Shit!  Get up bitch!'", NULL,
                            familiar, NULL, TO_ROOM);
                        sprintf(buf, "%s #Cslaps #7you!#n\n\r",
                                familiar->short_descr);
                        stc(buf, victim);
                        act("$p slaps $N!", ch, familiar, victim, TO_ROOM);
                        //victim->hit +=
                        //        number_range((familiar->value[1] * 500),
                        //                     (familiar->value[1] * 1000));
                        if (victim->hit > victim->max_hit)
                                victim->hit = victim->max_hit;
                        update_pos(victim);
                }

                break;
        case POS_DEAD:
                act("#0$n is #RDEAD!!#n", victim, 0, 0, TO_ROOM);
                send_to_char("#RYou have been KILLED!!#n\n\r\n\r", victim);
                break;
        default:
                if (dam > victim->max_hit / 4)
                        send_to_char("That really did HURT!\n\r", victim);
                if (victim->hit < victim->max_hit / 4 && dam > 0)
                {
                        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE)
                            && number_percent() < victim->beast)
                                vamp_rage(victim);
                        else
                                send_to_char("You sure are #rBLEEDING#n!\n\r",
                                             victim);

                }
                break;
        }

        /*
         * Sleep spells and extremely wounded folks.
         */
        if (!IS_AWAKE(victim))
                stop_fighting(victim, FALSE);

        /*
         * Payoff for killing things.
         */
        if (victim->hit <= 0 && IS_NPC(victim))
        {
                group_gain(ch, victim);
                victim->position = POS_DEAD;

                /*
                 * quest updating
                 */
                if (!IS_NPC(ch))
                {
                        QUEST_DATA *quest;
                        bool      found = FALSE;

                        for (quest = ch->pcdata->quests; quest && !found;
                             quest = quest->next)
                        {
                                if (quest->type == QT_MOB
                                    || quest->type == QT_MOB_AND_OBJ)
                                {
                                        if (quest->vnums[0] ==
                                            victim->pIndexData->vnum)
                                        {
                                                quest->vnums[0] = -1;
                                                found = TRUE;
                                                send_to_char
                                                        ("#GYou have fulfilled a quest.#n\n\r",
                                                         ch);
                                        }
                                }
                                else if (quest->type == QT_MASS_KILL)
                                {
                                        if (victim->pIndexData->vnum >=
                                            quest->vnums[0]
                                            && victim->pIndexData->vnum <=
                                            quest->vnums[1])
                                        {
                                                quest->vnums[3]++;
                                                if (quest->vnums[3] ==
                                                    quest->vnums[2])
                                                        send_to_char
                                                                ("#GYou have fulfilled a quest.#n\n\r",
                                                                 ch);
                                                found = TRUE;
                                        }
                                }
                        }
                }
        }

        if (victim->position == POS_DEAD)
        {
                /*
                 * Death trigger
                 */
                if (IS_NPC(victim) && HAS_TRIGGER_MOB(victim, TRIG_DEATH))
                {
                        victim->position = POS_STANDING;
                        p_percent_trigger(victim, NULL, NULL, ch, NULL, NULL,
                                          TRIG_DEATH);
                }
                if (IS_NPC(victim) && !IS_NPC(ch))
                {
                        ch->mkill += 1;
                        if (IS_CLASS(ch, CLASS_GHOUL))
                        {
                                int       aurag = victim->level;

                                aurag = number_fuzzy(number_fuzzy(aurag));
                                if (global_cp)
                                        aurag *= 2;
                                xprintf(buf,
                                        "#GYou savagely devour the corpse! #Y[#W%d#Y]#n\n\r",
                                        aurag);
                                if (!IS_SET(ch->act, PLR_BRIEF5))
                                        stc(buf, ch);
                                else
                                        ch->total_cp_gained += aurag;
                                ch->pcdata->stats[GHOUL_PTS] += aurag;
                        }
                        if (IS_CLASS(ch, CLASS_PRIEST))
                        {
                                int       aurag = victim->level;

                                aurag = number_fuzzy(number_fuzzy(aurag));
                                if (global_cp)
                                        aurag *= 2;
                                xprintf(buf,
                                        "#WYou pray for the salvation of the slain! #BGain #w%d #YFaith#n\n\r",
                                        aurag);
                                if (!IS_SET(ch->act, PLR_BRIEF5))
                                        stc(buf, ch);
                                else
                                        ch->total_cp_gained += aurag;
                                ch->pcdata->stats[FAITH_PTS] += aurag;
                        }

                        if (IS_CLASS(ch, CLASS_DEMON)
                            || IS_CLASS(ch, CLASS_GIANT)
                            || IS_CLASS(ch, CLASS_DROW)
                            || IS_CLASS(ch, CLASS_CYBORG)
                            || IS_CLASS(ch, CLASS_TANARRI)
                            || IS_CLASS(ch, CLASS_DROID)
			    || IS_CLASS(ch, CLASS_ZOMBIE))
                        {
                                if (IS_NPC(victim)
                                    && !IS_SET(victim->act, ACT_NOEXP))
                                {
					if (IS_CLASS(ch, CLASS_ZOMBIE))
					{
					if (global_cp)
					{
					ch->pcdata->powers[ZOM_FLESH] += 8;
				 	xprintf (buf, "#0You gain #R8#n#0 flesh points.#n\n\r");
					}
					else
					{
					ch->pcdata->powers[ZOM_FLESH] += 4;
                                        xprintf (buf, "#0You gain #R4#n#0 flesh points.#n\n\r");
					}
					}
                                        if (IS_CLASS(ch, CLASS_DEMON))
                                        {
                                                if (global_cp)
                                                {
                                                        ch->pcdata->
                                                                stats
                                                                [DEMON_CURRENT]
                                                                +=
                                                                victim->
                                                                level * 4;
                                                        ch->pcdata->
                                                                stats
                                                                [DEMON_TOTAL]
                                                                +=
                                                                victim->
                                                                level * 4;
                                                        ch->total_cp_gained +=
                                                                victim->
                                                                level * 4;
                                                }
                                                else
                                                {
                                                        ch->pcdata->
                                                                stats
                                                                [DEMON_CURRENT]
                                                                +=
                                                                victim->
                                                                level * 2;
                                                        ch->pcdata->
                                                                stats
                                                                [DEMON_TOTAL]
                                                                +=
                                                                victim->
                                                                level * 2;
                                                        ch->total_cp_gained +=
                                                                victim->
                                                                level * 2;
                                                }
                                        }
                                        else if (global_cp)
                                        {
                                                ch->pcdata->
                                                        stats[DROW_POWER] +=
                                                        victim->level * 4;
                                                ch->total_cp_gained +=
                                                        victim->level * 2;
                                        }
                                        else
                                        {
                                                ch->pcdata->
                                                        stats[DROW_POWER] +=
                                                        victim->level * 2;
                                                ch->total_cp_gained +=
                                                        victim->level * 2;
                                        }
                                        if (global_cp && !IS_CLASS(ch, CLASS_ZOMBIE))
                                        {
                                                xprintf(buf,
                                                        "#R** #WDouble CP #R*** #BYou gain #0(#W%d#0)#B class points.#n\n\r",
                                                        victim->level * 4);
                                                ch->total_cp_gained +=
                                                        victim->level * 4;
                                        }
                                        else
                                        {
					if (!IS_CLASS(ch, CLASS_ZOMBIE))
					{
                                                xprintf(buf,
                                                        "#BYou gain #0(#W%d#0)#B class points.#n\n\r",
                                                        victim->level * 2);
                                                ch->total_cp_gained +=
                                                        victim->level * 2;
					}
                                        }
                                        if (!IS_SET(ch->act, PLR_BRIEF4)
                                            && (!IS_SET(ch->act, PLR_BRIEF5)))
                                                send_to_char(buf, ch);
                                }
                        }
                        if (IS_CLASS(ch, CLASS_SHADOW))
                        {
                                if (global_cp)
                                {
                                        ch->pcdata->powers[SHADOW_POWER] +=
                                                victim->level * 4;
                                        ch->total_cp_gained +=
                                                victim->level * 4;
                                }
                                else
                                {
                                        ch->pcdata->powers[SHADOW_POWER] +=
                                                victim->level * 2;
                                        ch->total_cp_gained +=
                                                victim->level * 2;
                                }

                                if (global_cp)
                                {
                                        xprintf(buf,
                                                "#R** #WDouble CP #R*** #BYou gain #0(#W%d#0)#B class points.#n\n\r",
                                                victim->level * 4);
                                        ch->total_cp_gained +=
                                                victim->level * 4;
                                }
                                else
                                {
                                        xprintf(buf,
                                                "#BYou gain #0(#W%d#0)#B class points.#n\n\r",
                                                victim->level * 2);
                                        ch->total_cp_gained +=
                                                victim->level * 2;
                                }
                                if (!IS_SET(ch->act, PLR_BRIEF4)
                                    && (!IS_SET(ch->act, PLR_BRIEF5)))
                                        send_to_char(buf, ch);
                        }
                        if (ch->level == 1 && ch->mkill > 4)
                        {
                                ch->level = 2;
                                do_save(ch, "");
                                send_to_char
                                        ("Your char is now saved, enjoy Mindcloud!\n\r",
                                         ch);
                        }
                }
                if (!IS_NPC(victim) && IS_NPC(ch))
                {
                        victim->mdeath = victim->mdeath + 1;
                }
                raw_kill(victim, ch);


        switch (number_range (1,10) )
        {
        case 1: rare_weapon1( ch, victim ); break;
        case 2: rare_weapon2( ch, victim ); break;
        case 3: rare_weapon3( ch, victim ); break;
        case 4: rare_weapon4( ch, victim ); break;
        case 5: rare_weapon5( ch, victim ); break;
        case 6: rare_weapon6( ch, victim ); break;
        case 7: rare_weapon7( ch, victim ); break;
        case 8: rare_weapon8( ch, victim ); break;
        case 9: rare_weapon9( ch, victim ); break;
        case 10: rare_weapon10( ch, victim ); break;
        }


        switch (number_range (1,20) )
        {
        case 1: common_eq1( ch, victim ); break;
        case 2: common_eq2( ch, victim ); break;
        case 3: common_eq3( ch, victim ); break;
        case 4: common_eq4( ch, victim ); break;
        case 5: common_eq5( ch, victim ); break;
        case 6: common_eq6( ch, victim ); break;
        case 7: common_eq7( ch, victim ); break;
        case 8: common_eq8( ch, victim ); break;
        case 9: common_eq9( ch, victim ); break;
        case 10: common_eq10( ch, victim ); break;

        }

    	switch (number_range (1,20) )
    	{
    	case 1: rare_eq1( ch, victim ); break;
    	case 2: rare_eq2( ch, victim ); break;
    	case 3: rare_eq3( ch, victim ); break;
    	case 4: rare_eq4( ch, victim ); break;
        case 5: rare_eq5( ch, victim ); break;
        case 6: rare_eq6( ch, victim ); break;
        case 7: rare_eq7( ch, victim ); break;
        case 8: rare_eq8( ch, victim ); break;
        case 9: rare_eq9( ch, victim ); break;
        case 10: rare_eq10( ch, victim ); break;
        case 11: rare_eq11( ch, victim ); break;
        case 12: rare_eq12( ch, victim ); break;
        case 13: rare_eq13( ch, victim ); break;
        case 14: rare_eq14( ch, victim ); break;
        case 15: rare_eq15( ch, victim ); break;
        case 16: rare_eq16( ch, victim ); break;
        case 17: rare_eq17( ch, victim ); break;
        case 18: rare_eq18( ch, victim ); break;
        case 19: rare_eq19( ch, victim ); break;
        case 20: rare_eq20( ch, victim ); break;
	}



                if (!IS_NPC(ch) && !IS_NPC(victim)
                    && victim->pcdata->bounty > 0)
                {
                        xprintf(buf,
                                "You recive a %d bones bounty, for killing %s.\n\r",
                                victim->pcdata->bounty, victim->name);
                        send_to_char(buf, ch);
                        ch->bones += victim->pcdata->bounty;
                        victim->pcdata->bounty = 0;
                }
                if (IS_SET(ch->act2, PLR_AUTOSLAB))
                        do_get(ch, "all.slab corpse true");
                if (IS_SET(ch->act2, PLR_AUTOGEM))
                        do_get(ch, "all.gem corpse true");
                if (IS_SET(ch->act2, PLR_AUTOHILT))
                        do_get(ch, "all.hilt corpse true");
                if (IS_SET(ch->act2, PLR_AUTOBONES))
                        do_get(ch, "all.bone corpse true");
                if (IS_SET(ch->act2, PLR_AUTOSIGIL))
                        do_get(ch, "all.sigil corpse true");
                if (IS_SET(ch->act2, PLR_AUTOLOOT))
                        do_get(ch, "all corpse true");
                else if (!IS_SET(ch->act, PLR_BRIEF5))
                        do_look(ch, "in corpse");
    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
        if ( IS_SET(ch->act2, PLR_AUTOSAC) && !IS_CLASS(ch, CLASS_DRAGON) && !IS_CLASS(ch, CLASS_ZOMBIE))
                  do_sacrifice( ch, "corpse" );

	if ( IS_CLASS(ch, CLASS_DRAGON) && IS_SET(ch->act2, PLR_AUTOSAC) )
        do_consume (ch, "corpse");
	if( IS_CLASS(ch, CLASS_ZOMBIE) && IS_SET(ch->act2, PLR_AUTOSAC))
	do_satiate (ch, "corpse");
    }
                return;
        }
        if (victim == ch)
                return;
        tail_chain();
        return;
}

bool is_safe(CHAR_DATA * ch, CHAR_DATA * victim)
{
        if (!ch->in_room)
                return TRUE;    // when someone calls a one_hit or similar on something/someone who COULD be dead/gone.
        if (!victim->in_room)
                return TRUE;
        if (victim->shop_fun || victim->quest_fun)
        {
                send_to_char("That mobile is protected by the gods.\n\r", ch);
                return TRUE;
        }
        if (!IS_NPC(victim) && !IS_NPC(ch))
        {
                if (!CAN_PK(ch) || !CAN_PK(victim))
                {
                        send_to_char
                                ("Both players must be avatars to fight.\n\r",
                                 ch);
                        return TRUE;
                }
                if ((get_age(ch) - 17) < 2 && !IS_NPC(ch) && !IS_NPC(victim)
                    && !in_arena(ch) && !in_fortress(ch))
                {
                        send_to_char
                                ("You cannot attack other chars till you have 4 hours.\n\r",
                                 ch);
                        return TRUE;
                }
                if ((get_age(victim) - 17) < 2 && !IS_NPC(victim)
                    && !IS_NPC(ch) && !in_arena(victim)
                    && !in_fortress(victim))
                {
                        send_to_char
                                ("You cannot attack a person with less than 4 hours.\n\r",
                                 ch);
                        return TRUE;
                }
                if (ch->pcdata->safe_counter > 0)
                {
                        send_to_char
                                ("You still have a few safe-ticks left.\n\r",
                                 ch);
                        return TRUE;
                }
                if (IS_SET(ch->pcdata->jflags, JFLAG_DRONEWORLD)
                    && !IS_SET(victim->pcdata->jflags, JFLAG_DRONEWORLD))
                {
                        send_to_char("They are not in your reality.\n\r", ch);
                        return TRUE;
                }
                if (!IS_SET(ch->pcdata->jflags, JFLAG_DRONEWORLD)
                    && IS_SET(victim->pcdata->jflags, JFLAG_DRONEWORLD))
                {
                        send_to_char("They are not in your reality.\n\r", ch);
                        return TRUE;
                }
                if (victim->pcdata->aggress_from != 0
                    && ch->pcdata->playerid != victim->pcdata->aggress_from)
                {
                        send_to_char
                                ("You cannot attack them - gang protection.\n\r",
                                 ch);
                        return TRUE;
                }
                if (victim->pcdata->safe_counter > 0)
                {
                        send_to_char
                                ("They are safe for 10 ticks after training avatar.\n\r",
                                 ch);
                        return TRUE;
                }
                if (victim->desc)
                {
                        if (victim->desc->connected >= CON_NOTE_TO
                            && victim->desc->connected <= CON_NOTE_FINISH)
                        {
                                send_to_char
                                        ("That player is writing a note, be nice.\n\r",
                                         ch);
                                return TRUE;
                        }
                }

        }
        if(ch != victim)
	{

            if(!IS_NPC(ch) && !IS_NPC(victim) && !IS_SET(ch->in_room->room_flags, ROOM_ARENA) 
		&& ch->combat != victim && victim->combat != ch && !ragnarok)
	    {
                if(victim->combat != NULL || ch->combat != NULL)
		{
                        if((ch->combat != victim) || (victim->combat != ch))
			{
                            if(victim->combat != NULL && ( (victim->pcdata->kingdom == 0 || ch->pcdata->kingdom == 0)
                        	|| victim->combat->pcdata->kingdom == 0))
			    {
                                stc("Thats ganging, be nice.\n\r",ch);
                                return TRUE;
                            }

                            if(ch->combat != NULL
                        	&& ((victim->pcdata->kingdom == 0 || ch->pcdata->kingdom == 0)
                        	|| ch->combat->pcdata->kingdom == 0))
			    {
                                stc("Thats ganging, be nice.\n\r",ch);
                                return TRUE;
                            }
                        }
                }
            }
        }

        if (IS_AFFECTED(ch, AFF_ETHEREAL))
        {
                send_to_char("You cannot while ethereal.\n\r", ch);
                return TRUE;
        }
        if (IS_AFFECTED(victim, AFF_ETHEREAL))
        {
                send_to_char("You cannot fight an ethereal person.\n\r", ch);
                return TRUE;
        }
        if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
        {
                act("You are too insubstantial!", ch, NULL, victim, TO_CHAR);
                return TRUE;
        }
        if (IS_AFFECTED(victim, AFF_SHADOWPLANE))
        {
                act("$E is too insubstantial!", ch, NULL, victim, TO_CHAR);
                return TRUE;
        }
        if (IS_HEAD(ch, LOST_HEAD) || IS_EXTRA(ch, EXTRA_OSWITCH))
        {
                send_to_char("Objects cannot fight!\n\r", ch);
                return TRUE;
        }
        if (IS_HEAD(victim, LOST_HEAD) || IS_EXTRA(victim, EXTRA_OSWITCH))
        {
                send_to_char("You cannot attack an object.\n\r", ch);
                return TRUE;
        }
        if (IS_SET(victim->extra, EXTRA_AFK))
        {
                send_to_char("They are AFK!\n\r", ch);
                return TRUE;
        }
        if (!IS_NPC(victim) && victim->pcdata->logout_counter == 0
            && victim->desc == NULL && victim->timer > 1
            && victim->fight_timer == 0 && !in_fortress(ch))
        {
                send_to_char("Nooo, they are linkdead.\n\r", ch);
                return TRUE;
        }

        if (victim->fight_timer > 0)
                return FALSE;
        if (!IS_NPC(ch))
                if (IS_AFFECTED(ch, AFF_PEACE) && ch->fighting != NULL)
                        REMOVE_BIT(ch->affected_by, AFF_PEACE);
        if (!IS_NPC(victim))
                if (IS_AFFECTED(victim, AFF_PEACE)
                    && victim->fighting != NULL)
                        REMOVE_BIT(victim->affected_by, AFF_PEACE);
        if (IS_SET(ch->in_room->room_flags, ROOM_SAFE) && !ragnarok)
        {
                send_to_char("You cannot fight in a safe room.\n\r", ch);
                return TRUE;
        }
        if (IS_NPC(ch) || IS_NPC(victim))
                return FALSE;

        if (!IS_NPC(victim))
        {
                if (victim->pcdata->logout_counter > 0)
                        return FALSE;
        }

        if (victim->fighting == ch)
                return FALSE;
        if (IS_AFFECTED(ch, AFF_PEACE))
        {
                send_to_char("You are unable to attack them.\n\r", ch);
                return TRUE;
        }
        if (IS_AFFECTED(victim, AFF_PEACE))
        {
                send_to_char("You can't seem to attack them.\n\r", ch);
                return TRUE;
        }
        return FALSE;
}

/*
 * Check for parry.
 */
bool check_parry(CHAR_DATA * ch, CHAR_DATA * victim, int dt)
{
        OBJ_DATA *obj = NULL;
        int       chance = 0;
        bool      claws = FALSE;
        int       random_message;

        if (!IS_AWAKE(victim))
                return FALSE;
        if (IS_NPC(victim))
                obj = NULL;
        else if (IS_CLASS(victim, CLASS_WEREWOLF)
                 && victim->power[DISC_WERE_BEAR] > 2
                 && IS_VAMPAFF(victim, VAM_CLAWS)
                 && get_eq_char(victim, WEAR_WIELD) == NULL
                 && get_eq_char(victim, WEAR_HOLD) == NULL)
        {
                obj = NULL;
                claws = TRUE;
        }
        else if (IS_CLASS(victim, CLASS_MONK)
                 && IS_SET(victim->newbits, NEW_MONKADAM)
                 && get_eq_char(victim, WEAR_WIELD) == NULL
                 && get_eq_char(victim, WEAR_HOLD) == NULL)
        {
                obj = NULL;
                claws = TRUE;
        }
        else if (IS_CLASS(victim, CLASS_SHAPESHIFTER)
                 && get_eq_char(victim, WEAR_WIELD) == NULL
                 && get_eq_char(victim, WEAR_HOLD) == NULL)
        {
                obj = NULL;
                claws = TRUE;
        }
	else if( IS_CLASS(victim, CLASS_ZOMBIE) && get_eq_char(victim, WEAR_WIELD) == NULL
	&& get_eq_char(victim, WEAR_HOLD) == NULL && IS_SET(victim->pcdata->powers[ZOM_ACTIVE],ZOM_CLAWS))
	{
		obj = NULL;
		claws = TRUE;
	}

        else
        {
                if ((obj = get_eq_char(victim, WEAR_WIELD)) == NULL
                    || obj->item_type != ITEM_WEAPON)
                {
                        if ((obj = get_eq_char(victim, WEAR_HOLD)) == NULL
                            || obj->item_type != ITEM_WEAPON)
                                return FALSE;
                }
        }
        if ((dt < 1000 || dt > 1012) && !IS_CLASS(ch, CLASS_MONK))
                return FALSE;
        if (dt >= 1000 && dt <= 1012)
        {
                if (!IS_NPC(ch))
                        chance -= (ch->wpn[dt - 1000] * 0.1);
                else
                        chance -= (ch->level * 0.2);
                if (!IS_NPC(victim))
                        chance += (victim->wpn[dt - 1000] * 0.5);
                else
                        chance += victim->level;
        }
        else
        {
                if (!IS_NPC(ch))
                        chance -= (ch->wpn[0] * 0.1);
                else
                        chance -= (ch->level * 0.2);
                if (!IS_NPC(victim))
                        chance += (victim->wpn[0] * 0.5);
                else
                        chance += victim->level;
        }
        if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_CRANE)
            && victim->stance[STANCE_CRANE] > 0 && !can_counter(ch)
            && !can_bypass(ch, victim))
                chance += (victim->stance[STANCE_CRANE] * 0.25);
        else if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_HAWK)
                 && victim->stance[STANCE_HAWK] > 0 && !can_counter(ch)
                 && !can_bypass(ch, victim))
                chance += (victim->stance[STANCE_HAWK] * 0.25);

/* ------------ This is the part for superstances, Jobo ------------------- */
        if (!IS_NPC(victim) && victim->stance[0] > 12
            && IS_SET(victim->stance[(victim->stance[0] + 6)],
                      STANCEPOWER_PARRY)
            && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch)
            && !can_bypass(ch, victim))
                chance += victim->stance[(victim->stance[0])] * 0.25;
/* ------------ This is the end for superstances, Jobo ------------------- */

        chance -= (char_hitroll(ch) * 0.1);
        chance += (get_curr_dex(victim) / 10);
        chance -= (get_curr_dex(ch) / 10);
        if (claws)
        {
                if (victim->power[DISC_WERE_LYNX] > 3)
                        chance += (char_hitroll(victim) * 0.1);
                else
                        chance += (char_hitroll(victim) * 0.075);
        }
        if (!IS_NPC(ch))
        {
                if (IS_CLASS(ch, CLASS_FAE))
                {
                        if (IS_NPC(victim))
                                chance -= 50;
                        else
                                chance -= 15;
                        chance -= ch->pcdata->powers[FAE_PLASMA];
                        chance -= ch->pcdata->powers[FAE_MATTER];
                        chance -= ch->pcdata->powers[FAE_ENERGY];
                        chance -= ch->pcdata->powers[FAE_WILL];
                }
                if (IS_CLASS(ch, CLASS_DRONE))
                        chance -= ch->pcdata->powers[DRONE_FOCUS] * 15;
                if (IS_CLASS(ch, CLASS_DROID))
                {
                        chance -= 10;
                        if (IS_SET
                            (ch->pcdata->powers[DROID_IMPLANTS],
                             IMPLANT_ARMWIRE))
                                chance -= 40;
                }
                if (IS_CLASS(ch, CLASS_LICH))
                        chance -= 30;
                if (IS_CLASS(ch, CLASS_DRACONIAN))
                        chance -= 40;
                if (IS_CLASS(ch, CLASS_HOBBIT))
                        chance -= ch->pcdata->powers[HOBBIT_DISHES];
                if (IS_CLASS(ch, CLASS_HOBBIT))
                        chance -= ch->pcdata->powers[HOBBIT_COMBAT] * 3;
                if (IS_CLASS(ch, CLASS_GIANT))
                        chance -= ch->pcdata->rank * 5;
                if (IS_CLASS(ch, CLASS_GIANT)
                    && ch->pcdata->powers[GIANT_FRENZY] == 1)
                        chance -= 30;
                if (IS_CLASS(ch, CLASS_CYBORG))
                        chance -= ch->pcdata->powers[CYBORG_LIMBS] * 10;
	 if (IS_CLASS(ch,CLASS_SHINOBI)){
		switch(ch->pcdata->powers[S_ELEMENT]){
			case S_FIRE: chance -= 35; break;
			case S_WATER: chance -= 40; break;
			case S_EARTH: chance -= 45; break;
			case S_WIND: chance -= 40; break;
			case S_HOLY: chance -= 20; break;
			case S_SHADOW: chance -= 25; break;
		}
	 }

                if (ch->tier > 0)
                        chance -= ch->tier * 10;
                //if (IS_CLASS(ch, CLASS_LICH)) chance -= 40;
                if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
                {
                        if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
                                chance -= ch->pcdata->powers[TIGER_LEVEL] * 11;
                        else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
                                chance -= ch->pcdata->powers[BULL_LEVEL] * 7.5;
                        else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
                                chance -= ch->pcdata->powers[HYDRA_LEVEL] * 7;
                        else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
                                chance -= ch->pcdata->powers[FAERIE_LEVEL] * 6.5;
                        else
                                chance -= 10;
                }
                if (IS_CLASS(ch, CLASS_TANARRI))
                {
                        if (IS_SET
                            (ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD))
                                chance -= 15;
                        if (IS_SET
                            (ch->pcdata->powers[TANARRI_POWER],
                             TANARRI_SPEED))
                                chance -= 17;
                        if (ch->pcdata->powers[TANARRI_FURY_ON] == 1)
                                chance -= 17;
                }
                if (IS_CLASS(ch, CLASS_SHADOW)) /* CLASS_REMOVE */
                        chance -= ch->pcdata->powers[SHADOW_MARTIAL] * 1.2;
                if (IS_CLASS(ch, CLASS_ANGEL))
                        chance -= ch->pcdata->powers[ANGEL_JUSTICE] * 9;
		if (IS_CLASS(ch, CLASS_PALADIN))
			chance -= ch->pcdata->powers[PALADIN_COMBAT] * 9;
		if (IS_CLASS(ch, CLASS_ELEMENTAL))
			chance -= ch->pcdata->powers[ELEMENTAL_AIR] * 13;
		if (IS_CLASS(ch, CLASS_DRAGON))
			chance -= ch->pcdata->powers[dragonage] * 6;
		if (IS_CLASS(ch, CLASS_ZOMBIE))
			chance -= 45;
                if (IS_CLASS(ch, CLASS_VAMPIRE)
                    && ch->power[DISC_VAMP_CELE] > 0)
                        chance -= (ch->power[DISC_VAMP_CELE] * 3);
                if (IS_CLASS(ch, CLASS_NINJA)
                    && ch->pcdata->powers[NPOWER_CHIKYU] >= 6
                    && ch->pcdata->powers[HARA_KIRI] > 0)
                        chance -= 25;
                if (ch->pcdata->powers[NPOWER_CHIKYU] >= 5
                    && IS_CLASS(ch, CLASS_NINJA))
                        chance -= 20;
                if (IS_CLASS(ch, CLASS_DEMON) && IS_DEMPOWER(ch, DEM_SPEED))
                        chance -= 25;
                if (IS_CLASS(ch, CLASS_MONK) && ch->chi[CURRENT] > 0)
                        chance -= (ch->chi[CURRENT] * 7);
                if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                        chance -= ch->pcdata->powers[WEAPONSKILL] * 3.7;
                if (IS_CLASS(ch, CLASS_BLADE_MASTER))
                        chance -= ch->pcdata->powers[WEAPONSKILL] * 3.7;
		if (IS_CLASS(ch, CLASS_BLADE_MASTER) && IS_SET(ch->newbits2, NEW2_DEFENSE)) chance -= 20;

                if (IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST))
                        chance -= 30;
                if (IS_CLASS(ch, CLASS_DROW)
                    && IS_SET(ch->pcdata->powers[1], DPOWER_ARMS))
                        chance -= 45;
                if (IS_SET(ch->newbits, NEW_MONKFLAME))
                        chance -= 6;
                if (IS_CLASS(ch, CLASS_WEREWOLF))
                        chance -= (ch->power[DISC_WERE_MANT] * 3);
                if (IS_CLASS(ch, CLASS_SAMURAI))
                        chance -= 25;
                if (IS_CLASS(ch, CLASS_SKYBLADE))
                        chance -= 25;
                if (IS_CLASS(ch, CLASS_GHOUL))
                        chance -= 25;
                if (IS_CLASS(ch, CLASS_PRIEST))
                        chance -= 10;
                if (IS_CLASS(ch, CLASS_JEDI)
                    && ch->pcdata->powers[JPOWER_BODY] > 3)
                        chance -= 35;
                if (IS_ITEMAFF(ch, ITEMA_AFFMANTIS))
                        chance -= 12;
        }
        if (!IS_NPC(victim))
        {
                if (IS_CLASS(victim, CLASS_GIANT))
                        chance += victim->pcdata->rank * 5;
                if (IS_CLASS(victim, CLASS_FAE))
                {
                        chance += victim->pcdata->powers[FAE_PLASMA];
                        chance += victim->pcdata->powers[FAE_MATTER];
                        chance += victim->pcdata->powers[FAE_ENERGY];
                        chance += victim->pcdata->powers[FAE_WILL];
                }
                if (IS_CLASS(victim, CLASS_DRONE))
                        chance += victim->pcdata->powers[DRONE_EVASION] * 15;
                if (IS_CLASS(victim, CLASS_DROID))
                {
                        chance += 10;
                        if (IS_SET
                            (victim->pcdata->powers[DROID_IMPLANTS],
                             IMPLANT_ARMWIRE))
                                chance += 40;
                }
                if (IS_CLASS(victim, CLASS_LICH))
                        chance += 30;
                if (IS_CLASS(victim, CLASS_DRACONIAN))
                        chance += 30;
                if (IS_CLASS(victim, CLASS_HOBBIT))
                        chance += victim->pcdata->powers[HOBBIT_COMBAT] * 5;
                if (IS_CLASS(victim, CLASS_HOBBIT))
                        chance += victim->pcdata->powers[HOBBIT_DISHES] * 1.5;
                if (IS_CLASS(victim, CLASS_CYBORG))
                        chance += victim->pcdata->powers[CYBORG_LIMBS] * 10;
                if (victim->tier > 0)
                        chance += victim->tier * 10;
                if (IS_CLASS(victim, CLASS_SHAPESHIFTER))
                {
                        if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
                                chance += victim->pcdata->powers[TIGER_LEVEL] *6.5;
                        else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM)
                                chance += victim->pcdata->powers[BULL_LEVEL] * 7.5;
                        else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
                                chance += victim->pcdata->powers[HYDRA_LEVEL] * 6.5;
                        else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
                                chance += victim->pcdata->powers[FAERIE_LEVEL] * 11;
                        else
                                chance += 10;
                }

                if (IS_CLASS(victim, CLASS_JEDI)
                    && victim->pcdata->powers[JPOWER_BODY] >= 4)
                        chance += 35;
                if (IS_CLASS(victim, CLASS_SHADOW))
                        chance += victim->pcdata->powers[SHADOW_MARTIAL] * 1.2;
                if (IS_CLASS(victim, CLASS_TANARRI))
                {
                        if (IS_SET
                            (victim->pcdata->powers[TANARRI_POWER],
                             TANARRI_HEAD))
                                chance += 17;
                        if (IS_SET
                            (victim->pcdata->powers[TANARRI_POWER],
                             TANARRI_SPEED))
                                chance += 19;
                        if (victim->pcdata->powers[TANARRI_FURY_ON] == 1)
                                chance += 12;
                }
	if(IS_CLASS(victim,CLASS_SHINOBI)){
		switch(victim->pcdata->powers[S_ELEMENT]){
			case S_FIRE: chance += 40; break;
			case S_WATER: chance += 40; break;
			case S_EARTH: chance += 50; break;
			case S_WIND: chance += 50; break;
			case S_HOLY: chance += 35; break;
			case S_SHADOW: chance += 35; break;
		}
		}
                if (IS_CLASS(victim, CLASS_ANGEL))
                        chance += victim->pcdata->powers[ANGEL_PEACE] * 9;
		if (IS_CLASS(victim, CLASS_PALADIN))
			chance += victim->pcdata->powers[PALADIN_SPEED] * 9;
		if (IS_CLASS(victim, CLASS_ELEMENTAL))
			chance += victim->pcdata->powers[ELEMENTAL_WATER] * 10;
                if (IS_CLASS(victim, CLASS_DRAGON))
                        chance += victim->pcdata->powers[dragonage] * 6;
		if (IS_CLASS(victim, CLASS_ZOMBIE))
			chance += 40;
                if (victim->pcdata->powers[NPOWER_CHIKYU] >= 5
                    && IS_CLASS(victim, CLASS_NINJA))
                        chance += 35;
                if (IS_CLASS(victim, CLASS_NINJA)
                    && victim->pcdata->powers[NPOWER_CHIKYU] >= 6
                    && victim->pcdata->powers[HARA_KIRI] > 0)
                        chance += 25;
                if (victim->power[DISC_VAMP_CELE] > 0
                    && IS_CLASS(victim, CLASS_VAMPIRE))
                        chance += (victim->power[DISC_VAMP_CELE] * 4);
                if (IS_CLASS(victim, CLASS_DROW)
                    && IS_SET(victim->pcdata->powers[1], DPOWER_ARMS))
                        chance += 45;
                if (IS_CLASS(victim, CLASS_MONK) && ch->chi[CURRENT] > 0)
                        chance += (ch->chi[CURRENT] * 8);
                if (IS_SET(victim->newbits, NEW_MONKFLAME))
                        chance += 6;
                if (IS_CLASS(victim, CLASS_SAMURAI))
                        chance += 40;
                if (IS_CLASS(victim, CLASS_SKYBLADE))
                        chance += 35;
                if (IS_CLASS(victim, CLASS_GHOUL))
                        chance += 35;
                if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
                        chance += victim->pcdata->powers[WEAPONSKILL] * 4.2;
                if (IS_CLASS(victim, CLASS_BLADE_MASTER))
                        chance += victim->pcdata->powers[WEAPONSKILL] * 4.2;
                if (IS_CLASS(victim, CLASS_BLADE_MASTER) && IS_SET(victim->newbits2, NEW2_DEFENSE)) chance += 40;

                if (IS_CLASS(victim, CLASS_MAGE)
                    && IS_ITEMAFF(victim, ITEMA_DEFLECTOR))
                        chance += 35;
                if (IS_CLASS(victim, CLASS_DEMON)
                    && IS_DEMPOWER(victim, DEM_SPEED))
                        chance += 30;
        }
        if (chance > 80)
                chance = 80;
        else if (chance < 20)
                chance = 20;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_GIANT))
                chance -= ch->pcdata->rank * 5;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_GIANT))
                chance += victim->pcdata->rank * 5;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_LICH))
                chance -= 10;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_LICH))
                chance += 10;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHADOW))
                chance -= ch->pcdata->powers[SHADOW_MARTIAL] * 0.8;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHADOW))
                chance += victim->pcdata->powers[SHADOW_MARTIAL] * 0.8;
        if (IS_CLASS(victim, CLASS_FAE) && !IS_NPC(victim))
        {
                chance += victim->pcdata->powers[FAE_PLASMA] / 2;
                chance += victim->pcdata->powers[FAE_MATTER] / 2;
                chance += victim->pcdata->powers[FAE_ENERGY] / 2;
                chance += victim->pcdata->powers[FAE_WILL] / 2;
        }
        if (IS_CLASS(ch, CLASS_FAE) && !IS_NPC(ch))
        {
                chance -= ch->pcdata->powers[FAE_PLASMA] / 2;
                chance -= ch->pcdata->powers[FAE_MATTER] / 2;
                chance -= ch->pcdata->powers[FAE_ENERGY] / 2;
                chance -= ch->pcdata->powers[FAE_WILL] / 2;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DRONE))
                chance -= ch->pcdata->powers[DRONE_FOCUS] * 5;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DRONE))
                chance += victim->pcdata->powers[DRONE_EVASION] * 5;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_HOBBIT))
                chance -= ch->pcdata->powers[HOBBIT_DISHES] / 3.5;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_HOBBIT))
                chance += victim->pcdata->powers[HOBBIT_DISHES];
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_HOBBIT))
                chance -= ch->pcdata->powers[HOBBIT_COMBAT] * 2.5;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_HOBBIT))
                chance += victim->pcdata->powers[HOBBIT_COMBAT] * 3.5;
        if (!IS_NPC(ch) && ch->power[DISC_VAMP_CELE] > 0
            && IS_CLASS(ch, CLASS_VAMPIRE))
        {
                if (ch->pcdata->rank == AGE_TRUEBLOOD)
                        chance -= 25;
                else if (ch->pcdata->rank == AGE_LA_MAGRA)
                        chance -= 20;
                else if (ch->pcdata->rank == AGE_METHUSELAH)
                        chance -= 15;
                else if (ch->pcdata->rank == AGE_ELDER)
                        chance -= 10;
                else if (ch->pcdata->rank == AGE_ANCILLA)
                        chance -= 5;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
        {
                if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
                        chance -= ch->pcdata->powers[TIGER_LEVEL] * 5;
                else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
                        chance -= ch->pcdata->powers[BULL_LEVEL] * 4;
                else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
                        chance -= ch->pcdata->powers[HYDRA_LEVEL] * 4;
                else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
                        chance -= ch->pcdata->powers[FAERIE_LEVEL] * 3;
                else
                        chance -= 2;
        }
        if (IS_CLASS(ch, CLASS_TANARRI) && !IS_NPC(ch))
        {
                if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD))
                        chance -= 5;
                if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED))
                        chance -= 6;
                if (ch->pcdata->powers[TANARRI_FURY_ON] == 1)
                        chance -= 5;
        }
        if (IS_CLASS(victim, CLASS_TANARRI) && !IS_NPC(victim))
        {
                if (IS_SET
                    (victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD))
                        chance += 5;
                if (IS_SET
                    (victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED))
                        chance += 6;
                if (victim->pcdata->powers[TANARRI_FURY_ON] == 1)
                        chance += 5;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL))
                chance -= ch->pcdata->powers[ANGEL_JUSTICE] * 3;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL))
                chance += victim->pcdata->powers[ANGEL_PEACE] * 3;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_PALADIN))
                chance -= ch->pcdata->powers[PALADIN_SPEED] * 3;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_PALADIN))
                chance += victim->pcdata->powers[PALADIN_SPEED] * 3;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ELEMENTAL))
                chance -= ch->pcdata->powers[ELEMENTAL_EARTH] * 4;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ELEMENTAL))
                chance += victim->pcdata->powers[ELEMENTAL_EARTH] * 4;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DRAGON))
                chance -= ch->pcdata->powers[dragonage] * 2;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DRAGON))
                chance += victim->pcdata->powers[dragonage] * 2;
	if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ZOMBIE))
		chance -= 20;
	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ZOMBIE))
		chance += 20;
	if (IS_CLASS(ch, CLASS_SHINOBI) && !IS_NPC(ch))
		chance -= 20;
  	if (IS_CLASS(victim, CLASS_SHINOBI) && !IS_NPC(victim))
		chance += 20;


        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHAPESHIFTER))
        {
                if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
                        chance += victim->pcdata->powers[TIGER_LEVEL] * 3;
                else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM)
                        chance += victim->pcdata->powers[BULL_LEVEL] * 4;
                else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
                        chance += victim->pcdata->powers[HYDRA_LEVEL] * 5;
                else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
                        chance += victim->pcdata->powers[FAERIE_LEVEL] * 7;
                else
                        chance += 2;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE)
            && IS_ITEMAFF(ch, ITEMA_BEAST))
                chance -= 10;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                chance -= ch->pcdata->powers[WEAPONSKILL];
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
                chance += victim->pcdata->powers[WEAPONSKILL] * 1.5;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_BLADE_MASTER))
                chance -= ch->pcdata->powers[WEAPONSKILL];
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_BLADE_MASTER))
                chance += victim->pcdata->powers[WEAPONSKILL] * 1.5;

        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA))
        {
                if (ch->pcdata->rank == BELT_TEN)
                        chance -= 25;
                else if (ch->pcdata->rank == BELT_NINE)
                        chance -= 18;
                else if (ch->pcdata->rank == BELT_EIGHT)
                        chance -= 16;
                else if (ch->pcdata->rank == BELT_SEVEN)
                        chance -= 13;
                else if (ch->pcdata->rank == BELT_SIX)
                        chance -= 10;
                else if (ch->pcdata->rank == BELT_FIVE)
                        chance -= 7;
                else if (ch->pcdata->rank == BELT_FOUR)
                        chance -= 4;
                else if (ch->pcdata->rank == BELT_THREE)
                        chance -= 2;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON))
                chance -= (ch->power[DISC_DAEM_ATTA]);
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DEMON))
                chance += (victim->power[DISC_DAEM_ATTA]);
        if (!IS_NPC(victim) && victim->power[DISC_VAMP_CELE] > 0
            && IS_CLASS(victim, CLASS_VAMPIRE))
        {
                if (victim->pcdata->rank == AGE_TRUEBLOOD)
                        chance += 25;
                else if (victim->pcdata->rank == AGE_LA_MAGRA)
                        chance += 17;
                else if (victim->pcdata->rank == AGE_METHUSELAH)
                        chance += 15;
                else if (victim->pcdata->rank == AGE_ELDER)
                        chance += 10;
                else if (victim->pcdata->rank == AGE_ANCILLA)
                        chance += 5;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROID))
        {
                chance -= 5;
                if (IS_SET
                    (ch->pcdata->powers[DROID_IMPLANTS], IMPLANT_ARMWIRE))
                        chance -= 20;
        }
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROID))
        {
                chance += 5;
                if (IS_SET
                    (victim->pcdata->powers[DROID_IMPLANTS], IMPLANT_ARMWIRE))
                        chance += 15;
        }
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
        {
                if (victim->pcdata->rank == BELT_TEN)
                        chance += 30;
                else if (victim->pcdata->rank == BELT_NINE)
                        chance += 22;
                else if (victim->pcdata->rank == BELT_EIGHT)
                        chance += 16;
                else if (victim->pcdata->rank == BELT_SEVEN)
                        chance += 13;
                else if (victim->pcdata->rank == BELT_SIX)
                        chance += 12;
                else if (victim->pcdata->rank == BELT_FIVE)
                        chance += 10;
                else if (victim->pcdata->rank == BELT_FOUR)
                        chance += 9;
                else if (victim->pcdata->rank == BELT_THREE)
                        chance += 6;
                else if (victim->pcdata->rank == BELT_TWO)
                        chance += 4;
                else if (victim->pcdata->rank == BELT_ONE)
                        chance += 2;
        }

        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DRACONIAN))
                chance -= 15;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DRACONIAN))
                chance += 19;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SAMURAI))
                chance += 19;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_GHOUL))
                chance += 15;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SKYBLADE))
                chance += 15;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SAMURAI))
                chance -= 15;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_GHOUL))
                chance -= 15;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SKYBLADE))
                chance -= 15;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_PRIEST))
                chance -= 15;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_CYBORG))
                chance -= ch->pcdata->powers[CYBORG_LIMBS] * 4;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_CYBORG))
                chance += victim->pcdata->powers[CYBORG_LIMBS] * 4;
        if (victim->monkblock / 4 > 0)
                chance += victim->monkblock / 4;
        if (number_percent() < 100 && number_percent() >= chance)
                return FALSE;
        if (!IS_NPC(victim))
        {
                if (IS_SET(victim->pcdata->jflags, JFLAG_BULLY)
                    && number_range(1, 3) != 2)
                        return FALSE;
        }
        if (!IS_NPC(ch) && IS_NPC(victim)
            && IS_SET(ch->newbits, NEW_FIGHTDANCE)
            && IS_CLASS(ch, CLASS_DROW)
            && number_percent() >= 50
            &&
            (((obj = get_eq_char(ch, WEAR_WIELD)) != NULL
              && (obj->value[3] == 4 || obj->value[3] == 11))
             || ((obj = get_eq_char(ch, WEAR_HOLD)) != NULL
                 && (obj->value[3] == 4 || obj->value[3] == 11))))
        {
                act("$n's dark weapon dances around your defences, making a strange sound.", ch, NULL, victim, TO_VICT);
                act("Your sacrificial weapon hums in the air as it dances $N's defences.", ch, obj, victim, TO_CHAR);
                return FALSE;
        }
        if (claws)
        {
                if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF3))
                {
                        if (IS_CLASS(victim, CLASS_WEREWOLF))
                                act("You parry $n's blow with your claws.",
                                    ch, NULL, victim, TO_VICT);
                        else
                                act("You parry $n's blow with your Adamantium hands.", ch, NULL, victim, TO_VICT);
                }
                if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK)
                    && victim->monkblock < 150 && number_range(1, 2) == 1)
                        victim->monkblock++;
                if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF3))
                {
                        if (IS_CLASS(victim, CLASS_WEREWOLF))
                                act("$N parries your blow with $S claws.", ch,
                                    NULL, victim, TO_CHAR);
                        else
                                act("$N parries your blow with $S Adamantium claws.", ch, NULL, victim, TO_CHAR);
                }
                return TRUE;
        }
        if (!IS_NPC(ch) && !IS_NPC(victim) && obj != NULL
            && obj->item_type == ITEM_WEAPON && obj->value[3] >= 0
            && obj->value[3] <= 12)
        {
                random_message = number_range(1, 10);

                if (!IS_NPC(victim) && !IS_SET(victim->act, PLR_BRIEF3))
                {

                        if (random_message == 1 || random_message == 2)
                                act("You parry $n's blow with $p.", ch, obj,
                                    victim, TO_VICT);
                        if (random_message == 3 || random_message == 4)
                                act("You spin around just in time to parry $n's attack.", ch, NULL, victim, TO_VICT);
                        if (random_message == 5 || random_message == 6)
                                act("You throw your weapon behind your back parrying $n's attack.", ch, NULL, victim, TO_VICT);
                        if (random_message == 7 || random_message == 8)
                                act("You duck under $n's wild swipe then bring your weapon up and parry their second attempt.", ch, NULL, victim, TO_VICT);
                        if (random_message == 9 || random_message == 10)
                                act("You push your weapon up above your head wielding off $n's blow.", ch, NULL, victim, TO_VICT);
                }
                if (!IS_NPC(ch) && !IS_SET(ch->act, PLR_BRIEF3))
                {
                        if (random_message == 1 || random_message == 2)
                                act("$N parries your blow with $p.", ch, obj,
                                    victim, TO_CHAR);
                        if (random_message == 3 || random_message == 4)
                                act("$N spins around just in time to parry your attack.", ch, NULL, victim, TO_CHAR);
                        if (random_message == 5 || random_message == 6)
                                act("$N swings their weapon behind their back parrying off your back attack.", ch, NULL, victim, TO_CHAR);
                        if (random_message == 7 || random_message == 8)
                                act("$N ducks under your first swipe then brings their weapon up to dodge your second swipe.", ch, NULL, victim, TO_CHAR);
                        if (random_message == 9 || random_message == 10)
                                act("$N shoves their weapon above their head warding off your overhead blow.", ch, NULL, victim, TO_CHAR);
                }
                return TRUE;
        }
        if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF3))
                act("You parry $n's attack.", ch, NULL, victim, TO_VICT);
        if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF3))
                act("$N parries your attack.", ch, NULL, victim, TO_CHAR);
        return TRUE;
}

/*
 * Check for dodge.
 */
bool check_dodge(CHAR_DATA * ch, CHAR_DATA * victim, int dt)
{
        OBJ_DATA *familiar;
        int       chance = 0;
        int       random_message;
        int       skychance = 0;

        if (!IS_AWAKE(victim))
                return FALSE;
        if (!IS_NPC(ch))
                chance -= (ch->wpn[dt - 1000] * 0.1);
        else
                chance -= (ch->level * 0.2);
        if (!IS_NPC(victim))
                chance += (victim->wpn[0] * 0.5);
        else
                chance += victim->level;
        if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MONGOOSE)
            && victim->stance[STANCE_MONGOOSE] > 0 && !can_counter(ch)
            && !can_bypass(ch, victim))
                chance += victim->stance[STANCE_MONGOOSE] * 0.25;
        if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_GRIZZLY)
            && victim->stance[STANCE_GRIZZLY] > 0 && !can_counter(ch)
            && !can_bypass(ch, victim))
                chance += victim->stance[STANCE_GRIZZLY] * 0.25;

/* ------------ This is the part for superstances, Jobo ------------------- */
        /*
         * if (!IS_NPC(victim) && victim->stance[0] > 12 
         * && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_DODGE)
         * && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch) && !can_bypass(ch,victim))
         * chance += victim->stance[(victim->stance[0])] * 0.25; 
         */

        if (!IS_NPC(victim) && victim->stance[0] > 12
            && IS_SET(victim->stance[(victim->stance[0] + 6)],
                      STANCEPOWER_DODGE)
            && victim->stance[(victim->stance[0])] > 100
            && !can_bypass(ch, victim))
                chance += victim->stance[(victim->stance[0])] * 0.25;
/* ------------ This is the end for superstances, Jobo ------------------- */

        if (!IS_NPC(ch))
        {
                if (IS_CLASS(ch, CLASS_GIANT))
                        chance -= ch->pcdata->rank * 5;
                if (IS_CLASS(ch, CLASS_GIANT)
                    && ch->pcdata->powers[GIANT_FRENZY] == 1)
                        chance -= 30;
                if (IS_CLASS(ch, CLASS_SHADOW))
                {
                        if (IS_SET
                            (ch->pcdata->powers[SHADOW_BITS],
                             NPOWER_BLOODRAGE))
                                chance -= 10;
                        chance -= ch->pcdata->powers[SHADOW_MARTIAL];
                }
                if (IS_CLASS(ch, CLASS_FAE))
                {
                        if (IS_NPC(victim))
                                chance -= 50;
                        else
                                chance -= 15;
                        chance -= ch->pcdata->powers[FAE_PLASMA];
                        chance -= ch->pcdata->powers[FAE_MATTER];
                        chance -= ch->pcdata->powers[FAE_ENERGY];
                        chance -= ch->pcdata->powers[FAE_WILL];
                }
                if (IS_CLASS(ch, CLASS_DRONE))
                        chance -= ch->pcdata->powers[DRONE_FOCUS] * 15;
                if (ch->tier > 0)
                        chance -= ch->tier * 10;
                if (IS_CLASS(ch, CLASS_CYBORG))
                        chance -= ch->pcdata->powers[CYBORG_LIMBS] * 10;
                if (IS_CLASS(ch, CLASS_DROID))
                {
                        chance -= 10;
                        if (IS_SET
                            (ch->pcdata->powers[DROID_IMPLANTS],
                             IMPLANT_360VISION))
                                chance -= 40;
                }
                if (IS_CLASS(ch, CLASS_LICH))
                        chance -= 25;
                if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
                {
                        if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
                                chance -= ch->pcdata->powers[TIGER_LEVEL] * 9;
                        else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
                                chance -= ch->pcdata->powers[BULL_LEVEL] * 7.5;
                        else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
                                chance -= ch->pcdata->powers[HYDRA_LEVEL] * 7;
                        else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
                                chance -= ch->pcdata->powers[FAERIE_LEVEL] * 6;
                        else
                                chance -= 10;
                }
                if (IS_CLASS(ch, CLASS_HOBBIT))
                        chance -= ch->pcdata->powers[HOBBIT_COMBAT] * 3;
                if (IS_CLASS(ch, CLASS_HOBBIT))
                        chance -= ch->pcdata->powers[HOBBIT_DISHES];
                if (IS_CLASS(ch, CLASS_JEDI)
                    && ch->pcdata->powers[JPOWER_BODY] >= 4)
                        chance -= 35;
                if (IS_CLASS(ch, CLASS_TANARRI))
                {
                        if (IS_SET
                            (ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD))
                                chance -= 15;
                        if (IS_SET
                            (ch->pcdata->powers[TANARRI_POWER],
                             TANARRI_SPEED))
                                chance -= 17;
                        if (ch->pcdata->powers[TANARRI_FURY_ON] == 1)
                                chance -= 17;
                }
	if(IS_CLASS(ch,CLASS_SHINOBI)){
		switch(ch->pcdata->powers[S_ELEMENT]){
			case S_FIRE: chance -= 35; break;
			case S_WATER: chance -= 30; break;
			case S_EARTH: chance -= 40; break;
			case S_WIND: chance -= 45; break;
			case S_HOLY: chance -= 20; break;
			case S_SHADOW: chance -= 25; break;
		}
	}

                if (IS_CLASS(ch, CLASS_ANGEL))
                        chance -= ch->pcdata->powers[ANGEL_JUSTICE] * 9;
		if (IS_CLASS(ch, CLASS_PALADIN))
			chance -= ch->pcdata->powers[PALADIN_SPEED] * 9;
		if (IS_CLASS(ch, CLASS_ELEMENTAL))
			chance -= ch->pcdata->powers[ELEMENTAL_FIRE] * 9;
		if (IS_CLASS(ch, CLASS_DRAGON))
                	chance -= ch->pcdata->powers[dragonage] * 4;
		if (IS_CLASS(ch, CLASS_ZOMBIE))
			chance -= 45;
                if (IS_CLASS(ch, CLASS_NINJA)
                    && ch->pcdata->powers[NPOWER_CHIKYU] >= 6
                    && ch->pcdata->powers[HARA_KIRI] > 0)
                        chance -= 25;
                if (IS_CLASS(ch, CLASS_MONK))
                        chance -= ch->chi[CURRENT] * 3;
                if (ch->pcdata->powers[NPOWER_CHIKYU] >= 5
                    && IS_CLASS(ch, CLASS_NINJA))
                        chance -= 20;
                if (ch->power[DISC_VAMP_CELE] > 0
                    && IS_CLASS(ch, CLASS_VAMPIRE))
                        chance -= (ch->power[DISC_VAMP_CELE] * 4);
                if (IS_CLASS(ch, CLASS_DEMON) && IS_DEMPOWER(ch, DEM_SPEED))
                        chance -= 25;
                if (IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST))
                        chance -= 25;
                if (IS_CLASS(ch, CLASS_WEREWOLF)
                    && ch->power[DISC_WERE_MANT] > 6)
                        chance -= (ch->power[DISC_WERE_MANT] * 3);
                if (!IS_CLASS(ch, CLASS_WEREWOLF)
                    && (IS_ITEMAFF(ch, ITEMA_AFFMANTIS)))
                        chance -= 18;
                if (IS_CLASS(ch, CLASS_DROW)
                    && IS_SET(ch->pcdata->powers[1], DPOWER_SPEED))
                        chance -= 50;
                if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                        chance -= ch->pcdata->powers[WEAPONSKILL] * 4.2;
                if (IS_CLASS(ch, CLASS_BLADE_MASTER))
                        chance -= ch->pcdata->powers[WEAPONSKILL] * 4.2;

                if (IS_CLASS(ch, CLASS_SAMURAI))
                        chance -= 35;
                if (IS_CLASS(ch, CLASS_DRACONIAN))
                        chance -= 35;
                if (IS_CLASS(ch, CLASS_SKYBLADE))
                        chance -= 35;
                if (IS_CLASS(ch, CLASS_GHOUL))
                        chance -= 35;
        }
        if (!IS_NPC(victim))
        {
                if (IS_CLASS(victim, CLASS_GIANT))
                        chance += victim->pcdata->rank * 5;
                if (IS_CLASS(victim, CLASS_SHADOW))
                        chance += victim->pcdata->powers[SHADOW_MARTIAL] * 1.2;
                if (IS_CLASS(victim, CLASS_FAE))
                {
                        chance += 20;
                        chance += victim->pcdata->powers[FAE_PLASMA];
                        chance += victim->pcdata->powers[FAE_MATTER];
                        chance += victim->pcdata->powers[FAE_ENERGY];
                        chance += victim->pcdata->powers[FAE_WILL];
                }
                if (IS_CLASS(victim, CLASS_DRONE))
                        chance += victim->pcdata->powers[DRONE_EVASION] * 15;
                if (IS_CLASS(victim, CLASS_DROID))
                {
                        chance += 10;
                        if (IS_SET
                            (victim->pcdata->powers[DROID_IMPLANTS],
                             IMPLANT_360VISION))
                                chance += 40;
                }
                if (IS_CLASS(victim, CLASS_JEDI)
                    && victim->pcdata->powers[JPOWER_BODY] >= 4)
                        chance += 35;
                if (IS_CLASS(victim, CLASS_HOBBIT))
                        chance += victim->pcdata->powers[HOBBIT_COMBAT] * 4;
                if (IS_CLASS(victim, CLASS_HOBBIT))
                        chance += victim->pcdata->powers[HOBBIT_DISHES] * 2;
                if (IS_CLASS(victim, CLASS_CYBORG))
                        chance += victim->pcdata->powers[CYBORG_LIMBS] * 10;
                if (victim->tier > 0)
                        chance += victim->tier * 10;
                if (IS_CLASS(victim, CLASS_LICH))
                        chance += 25;
                if (victim->pcdata->powers[NPOWER_CHIKYU] >= 5
                    && IS_CLASS(victim, CLASS_NINJA))
                        chance += 30;
                if (IS_CLASS(victim, CLASS_NINJA)
                    && victim->pcdata->powers[NPOWER_CHIKYU] >= 6
                    && victim->pcdata->powers[HARA_KIRI] > 0)
                        chance += 20;
                if (IS_CLASS(victim, CLASS_SHAPESHIFTER))
                {
                        if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
                                chance +=
                                        victim->pcdata->powers[TIGER_LEVEL] *
                                        6;
                        else if (victim->pcdata->powers[SHAPE_FORM] ==
                                 BULL_FORM)
                                chance +=
                                        victim->pcdata->powers[BULL_LEVEL] *
                                        7;
                        else if (victim->pcdata->powers[SHAPE_FORM] ==
                                 HYDRA_FORM)
                                chance +=
                                        victim->pcdata->powers[HYDRA_LEVEL] *
                                        6;
                        else if (victim->pcdata->powers[SHAPE_FORM] ==
                                 FAERIE_FORM)
                                chance +=
                                        victim->pcdata->powers[FAERIE_LEVEL] *
                                        11;
                        else
                                chance += 10;
                }
                if (IS_CLASS(victim, CLASS_MONK))
                        chance += victim->chi[CURRENT] * 3;
                if (IS_CLASS(victim, CLASS_TANARRI))
                {
                        if (IS_SET
                            (victim->pcdata->powers[TANARRI_POWER],
                             TANARRI_HEAD))
                                chance += 15;
                        if (IS_SET
                            (victim->pcdata->powers[TANARRI_POWER],
                             TANARRI_SPEED))
                                chance += 17;
                        if (victim->pcdata->powers[TANARRI_FURY_ON] == 1)
                                chance += 12;
                }
	if(IS_CLASS(victim,CLASS_SHINOBI)){
		switch(victim->pcdata->powers[S_ELEMENT]){
			case S_FIRE: chance += 40; break;
			case S_WATER: chance += 40; break;
			case S_EARTH: chance += 45; break;
			case S_WIND: chance += 45; break;
			case S_HOLY: chance += 35; break;
			case S_SHADOW: chance += 35; break;
		}
		}
                if (IS_CLASS(victim, CLASS_ANGEL))
                        chance += victim->pcdata->powers[ANGEL_PEACE] * 9;
		if (IS_CLASS(victim, CLASS_PALADIN))
			chance += victim->pcdata->powers[PALADIN_SPEED] * 9;
		if (IS_CLASS(victim, CLASS_ELEMENTAL))
			chance += victim->pcdata->powers[ELEMENTAL_FIRE] *9;
	        if (IS_CLASS(victim, CLASS_DRAGON))
	                chance += victim->pcdata->powers[dragonage] * 3;
		if (IS_CLASS(victim, CLASS_ZOMBIE))
			chance += 45;
                if (victim->power[DISC_VAMP_CELE] > 0
                    && IS_CLASS(victim, CLASS_VAMPIRE))
                        chance += (victim->power[DISC_VAMP_CELE] * 3);
                if (IS_CLASS(victim, CLASS_DEMON)
                    && IS_DEMPOWER(victim, DEM_SPEED))
                        chance += 25;
                if (IS_CLASS(victim, CLASS_WEREWOLF)
                    && victim->power[DISC_WERE_MANT] > 6)
                        chance += (victim->power[DISC_WERE_MANT] * 3.5);
                if (IS_CLASS(victim, CLASS_DROW)
                    && IS_SET(victim->pcdata->powers[1], DPOWER_SPEED))
                        chance += 50;
                if (IS_CLASS(victim, CLASS_SAMURAI))
                        chance += 35;
                if (IS_CLASS(victim, CLASS_DRACONIAN))
                        chance += 35;
                if (IS_CLASS(victim, CLASS_GHOUL))
                        chance += 35;
                if (IS_CLASS(victim, CLASS_SKYBLADE))
                        chance += 35;
                if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
                        chance += victim->pcdata->powers[WEAPONSKILL] * 4.5;
                if (IS_CLASS(victim, CLASS_BLADE_MASTER))
                        chance += victim->pcdata->powers[WEAPONSKILL] * 4.5;
                if (!IS_CLASS(victim, CLASS_WEREWOLF)
                    && IS_ITEMAFF(victim, ITEMA_AFFMANTIS))
                        chance += 25;
                if (IS_CLASS(victim, CLASS_MAGE)
                    && IS_ITEMAFF(victim, ITEMA_DEFLECTOR))
                        chance += 50;
        }
        if (chance > 80)
                chance = 80;
        else if (chance < 20)
                chance = 20;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROID))
        {
                chance -= 5;
                if (IS_SET
                    (ch->pcdata->powers[DROID_IMPLANTS], IMPLANT_360VISION))
                        chance -= 25;
        }
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROID))
        {
                chance += 5;
                if (IS_SET
                    (victim->pcdata->powers[DROID_IMPLANTS],
                     IMPLANT_360VISION))
                        chance += 25;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_CYBORG))
                chance -= ch->pcdata->powers[CYBORG_LIMBS] * 4;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_CYBORG))
                chance += victim->pcdata->powers[CYBORG_LIMBS] * 4;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_GIANT))
                chance -= ch->pcdata->rank * 5;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_GIANT))
                chance += victim->pcdata->rank * 5;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHADOW))
                chance -= ch->pcdata->powers[SHADOW_MARTIAL] * 0.8;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHADOW))
                chance += victim->pcdata->powers[SHADOW_MARTIAL] * 0.8;
        if (IS_CLASS(victim, CLASS_FAE) && !IS_NPC(victim))
        {
                chance += victim->pcdata->powers[FAE_PLASMA] / 2;
                chance += victim->pcdata->powers[FAE_MATTER] / 2;
                chance += victim->pcdata->powers[FAE_ENERGY] / 2;
                chance += victim->pcdata->powers[FAE_WILL] / 2;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DRONE))
                chance -= ch->pcdata->powers[DRONE_FOCUS] * 5;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DRONE))
                chance += victim->pcdata->powers[DRONE_EVASION] * 5;
        if (IS_CLASS(ch, CLASS_FAE) && !IS_NPC(ch))
        {
                chance -= ch->pcdata->powers[FAE_PLASMA] / 2;
                chance -= ch->pcdata->powers[FAE_MATTER] / 2;
                chance -= ch->pcdata->powers[FAE_ENERGY] / 2;
                chance -= ch->pcdata->powers[FAE_WILL] / 2;
        }
        if (IS_CLASS(ch, CLASS_LICH) && !IS_NPC(ch))
                chance -= 15;
        if (IS_CLASS(victim, CLASS_LICH) && !IS_NPC(victim))
                chance += 10;
        if (IS_CLASS(ch, CLASS_HOBBIT) && !IS_NPC(ch))
                chance -= ch->pcdata->powers[HOBBIT_DISHES] / 4;
        if (IS_CLASS(victim, CLASS_HOBBIT) && !IS_NPC(victim))
                chance += victim->pcdata->powers[HOBBIT_DISHES];
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_HOBBIT))
                chance -= ch->pcdata->powers[HOBBIT_COMBAT] * 2;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_HOBBIT))
                chance += victim->pcdata->powers[HOBBIT_COMBAT] * 3;
        if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && !IS_NPC(ch))
        {
                if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
                        chance -= ch->pcdata->powers[TIGER_LEVEL] * 5;
                else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
                        chance -= ch->pcdata->powers[BULL_LEVEL] * 4;
                else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
                        chance -= ch->pcdata->powers[HYDRA_LEVEL] * 4;
                else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
                        chance -= ch->pcdata->powers[FAERIE_LEVEL] * 3;
                else
                        chance -= 2;
        }
        if (IS_CLASS(victim, CLASS_SHAPESHIFTER) && !IS_NPC(victim))
        {
                if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
                        chance += victim->pcdata->powers[TIGER_LEVEL] * 3;
                else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM)
                        chance += victim->pcdata->powers[BULL_LEVEL] * 4;
                else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
                        chance += victim->pcdata->powers[HYDRA_LEVEL] * 4;
                else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
                        chance += victim->pcdata->powers[FAERIE_LEVEL] * 7;
                else
                        chance += 2;
        }
        if (IS_CLASS(ch, CLASS_TANARRI) && !IS_NPC(ch))
        {
                if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD))
                        chance -= 5;
                if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED))
                        chance -= 6;
                if (ch->pcdata->powers[TANARRI_FURY_ON] == 1)
                        chance -= 5;
        }
        if (IS_CLASS(victim, CLASS_TANARRI) && !IS_NPC(victim))
        {
                if (IS_SET
                    (victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD))
                        chance += 5;
                if (IS_SET
                    (victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED))
                        chance += 6;
                if (victim->pcdata->powers[TANARRI_FURY_ON] == 1)
                        chance += 5;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL))
                chance -= ch->pcdata->powers[ANGEL_JUSTICE] * 3;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL))
                chance += victim->pcdata->powers[ANGEL_PEACE] * 3;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_PALADIN))
                chance -= ch->pcdata->powers[PALADIN_SPEED] * 3;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_PALADIN))
                chance += victim->pcdata->powers[PALADIN_SPEED] * 3;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ELEMENTAL))
                chance -= ch->pcdata->powers[ELEMENTAL_EARTH] * 3;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ELEMENTAL))
                chance += victim->pcdata->powers[ELEMENTAL_EARTH] * 3;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DRAGON))
                chance -= ch->pcdata->powers[dragonage] * 1;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DRAGON))
                chance += victim->pcdata->powers[dragonage] * 1;
	if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ZOMBIE))
		chance -= 20;
	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ZOMBIE))
		chance += 20;
	if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_SHINOBI)) chance -= 15;
	if (!IS_NPC(victim) && IS_CLASS(victim,CLASS_SHINOBI)) chance += 15;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW)
            && IS_SET(victim->pcdata->powers[1], DPOWER_SPEED))
                chance += 20;
        if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMPOWER(victim, DEM_SPEED))
                chance += 25;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                chance -= ch->pcdata->powers[WEAPONSKILL];
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
                chance += victim->pcdata->powers[WEAPONSKILL] * 2;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_BLADE_MASTER))
                chance -= ch->pcdata->powers[WEAPONSKILL];
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_BLADE_MASTER))
                chance += victim->pcdata->powers[WEAPONSKILL] * 2;

        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SAMURAI))
                chance -= 12;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SKYBLADE))
                chance -= 15;
        if (!IS_NPC(ch) && IS_CLASS(victim, CLASS_DRACONIAN))
                chance -= 20;
        if (!IS_NPC(ch) && IS_CLASS(victim, CLASS_PRIEST))
                chance -= 18;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SAMURAI))
                chance += 15;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_GHOUL))
                chance -= 15;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE)
            && IS_ITEMAFF(ch, ITEMA_BEAST))
                chance -= 15;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MAGE)
            && IS_ITEMAFF(victim, ITEMA_BEAST))
                chance += 15;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF)
            && ch->power[DISC_WERE_MANT] > 6)
                chance -= (ch->power[DISC_WERE_MANT]);
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF)
            && victim->power[DISC_WERE_MANT] > 6)
                chance += (victim->power[DISC_WERE_MANT]);
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
        {
                if (ch->pcdata->rank == AGE_TRUEBLOOD)
                        chance -= 20;
                else if (ch->pcdata->rank == AGE_LA_MAGRA)
                        chance -= 17;
                else if (ch->pcdata->rank == AGE_METHUSELAH)
                        chance -= 10;
                else if (ch->pcdata->rank == AGE_ELDER)
                        chance -= 7;
                else if (ch->pcdata->rank == AGE_ANCILLA)
                        chance -= 3;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA))
        {
                if (ch->pcdata->rank == BELT_TEN)
                        chance -= 25;
                else if (ch->pcdata->rank == BELT_NINE)
                        chance -= 18;
                else if (ch->pcdata->rank == BELT_EIGHT)
                        chance -= 17;
                else if (ch->pcdata->rank == BELT_SEVEN)
                        chance -= 14;
                else if (ch->pcdata->rank == BELT_SIX)
                        chance -= 12;
                else if (ch->pcdata->rank == BELT_FIVE)
                        chance -= 10;
                else if (ch->pcdata->rank == BELT_FOUR)
                        chance -= 9;
                else if (ch->pcdata->rank == BELT_THREE)
                        chance -= 7;
                else if (ch->pcdata->rank == BELT_TWO)
                        chance -= 4;
                else if (ch->pcdata->rank == BELT_ONE)
                        chance -= 2;
        }
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))
        {
                if (victim->pcdata->rank == AGE_TRUEBLOOD)
                        chance += 20;
                else if (victim->pcdata->rank == AGE_LA_MAGRA)
                        chance += 17;
                else if (victim->pcdata->rank == AGE_METHUSELAH)
                        chance += 10;
                else if (victim->pcdata->rank == AGE_ELDER)
                        chance += 7;
                else if (victim->pcdata->rank == AGE_ANCILLA)
                        chance += 3;
        }
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
        {
                if (victim->pcdata->rank == BELT_TEN)
                        chance += 25;
                else if (victim->pcdata->rank == BELT_NINE)
                        chance += 18;
                else if (victim->pcdata->rank == BELT_EIGHT)
                        chance += 17;
                else if (victim->pcdata->rank == BELT_SEVEN)
                        chance += 14;
                else if (victim->pcdata->rank == BELT_SIX)
                        chance += 11;
                else if (victim->pcdata->rank == BELT_FIVE)
                        chance += 9;
                else if (victim->pcdata->rank == BELT_FOUR)
                        chance += 7;
                else if (victim->pcdata->rank == BELT_THREE)
                        chance += 5;
                else if (victim->pcdata->rank == BELT_TWO)
                        chance += 4;
                else if (victim->pcdata->rank == BELT_ONE)
                        chance += 2;
        }
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK)
            && (victim->monkblock / 2 > 0))
                chance += (victim->monkblock / 2);
        if (chance > 90)
                chance = 90;
        if (!IS_NPC(victim))
        {
                if (IS_SET(victim->pcdata->jflags, JFLAG_BULLY)
                    && number_range(1, 3) != 2)
                        return FALSE;
        }
        if (number_percent() >= chance)
                return FALSE;
        if (IS_CLASS(victim, CLASS_SKYBLADE)
            && victim->pcdata->powers[SKYBLADE_SKILLS] >= 6)
        {
                skychance = 25;
                if (IS_SET(ch->newbits, NEW_CLOUDBLESS))
                        skychance += 25;
                if (number_percent() >= skychance)
                        if (!IS_SET(victim->act, PLR_BRIEF3))
                        {
                                act("$N jumps high into the clouds, over your attack.", ch, NULL, victim, TO_CHAR);
                                act("You jump high into the clouds, over $n's attack.", ch, NULL, victim, TO_VICT);
                        }
                return TRUE;
                skychance = 101;
        }
        if (!IS_NPC(ch) && !IS_NPC(victim) && skychance != 101)
        {
                random_message = number_range(1, 10);

                if (!IS_NPC(victim) && !IS_SET(victim->act, PLR_BRIEF))
                {

                        if (random_message == 1 || random_message == 2)
                                act("You duck under $n's wild swing.", ch,
                                    NULL, victim, TO_VICT);
                        if (random_message == 3 || random_message == 4)
                                act("You jump over $n's low swing.", ch, NULL,
                                    victim, TO_VICT);
                        if (random_message == 5 || random_message == 6)
                                act("You spin around, barely dodging $n's attack.", ch, NULL, victim, TO_VICT);
                        if (random_message == 7 || random_message == 8)
                                act("$n lunges forward as you sidestep the attack.", ch, NULL, victim, TO_VICT);
                        if (random_message == 9 || random_message == 10)
                                act("You trip and fall down just barely dodging $n's crushing blow.", ch, NULL, victim, TO_VICT);
                }
                if (!IS_NPC(ch) && !IS_SET(ch->act, PLR_BRIEF)
                    && skychance != 101)
                {
                        if (random_message == 1 || random_message == 2)
                                act("$N ducks under your wild swing.", ch,
                                    NULL, victim, TO_CHAR);
                        if (random_message == 3 || random_message == 4)
                                act("$N jumps over your low swing.", ch, NULL,
                                    victim, TO_CHAR);
                        if (random_message == 5 || random_message == 6)
                                act("$N spins around barely dodging your attack.", ch, NULL, victim, TO_CHAR);
                        if (random_message == 7 || random_message == 8)
                                act("$N sidesteps your lunge, you narrowly missed.", ch, NULL, victim, TO_CHAR);
                        if (random_message == 9 || random_message == 10)
                                act("$N falls on his face barely dodging your crushing blow.", ch, NULL, victim, TO_CHAR);
                }
                return TRUE;
        }
        if (IS_CLASS(victim, CLASS_HOBBIT)
            && (familiar = get_eq_char(victim, WEAR_FAMILIAR)) != NULL
            && familiar->item_type == ITEM_FAMILIAR
            && familiar->value[0] == 7)
        {
                if (number_percent() > 80)
                {
                        if (!IS_SET(ch->act, PLR_BRIEF3))
                                act("$p #nmoves in front of the blow taking it for $N!", ch, familiar, victim, TO_CHAR);
                        if (!IS_SET(victim->act, PLR_BRIEF3))
                                act("$p #nmoves in front of the blow taking it for you!", ch, familiar, victim, TO_VICT);
                        return TRUE;
                }
        }


        if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF3))
                act("You dodge $n's attack.", ch, NULL, victim, TO_VICT);
        if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF3))
                act("$N dodges your attack.", ch, NULL, victim, TO_CHAR);
        return TRUE;
}

/*
 * Set position of a victim.
 */
void update_pos(CHAR_DATA * victim)
{
        CHAR_DATA *mount;

        if (victim->hit > 0)
        {
                if (victim->position <= POS_STUNNED)
                {
                        bool      gm_stance = FALSE;

                        victim->position = POS_STANDING;
                        if (!IS_NPC(victim) && victim->stance[0] > 0)
                        {
                                int       stance = victim->stance[0];

                                if (victim->stance[stance] >= 200)
                                        gm_stance = TRUE;
                        }
                        if (IS_NPC(victim)
                            || victim->max_hit * 0.25 > victim->hit)
                        {
                                act("$n clambers back to $s feet.", victim,
                                    NULL, NULL, TO_ROOM);
                                act("You clamber back to your feet.", victim,
                                    NULL, NULL, TO_CHAR);
                        }
                        else
                        {
                                act("$n flips back up to $s feet.", victim,
                                    NULL, NULL, TO_ROOM);
                                act("You flip back up to your feet.", victim,
                                    NULL, NULL, TO_CHAR);
                        }
                }
                return;
        }
        else if ((mount = victim->mount) != NULL)
        {
                if (victim->mounted == IS_MOUNT)
                {
                        act("$n rolls off $N.", mount, NULL, victim, TO_ROOM);
                        act("You roll off $N.", mount, NULL, victim, TO_CHAR);
                }
                else if (victim->mounted == IS_RIDING)
                {
                        act("$n falls off $N.", victim, NULL, mount, TO_ROOM);
                        act("You fall off $N.", victim, NULL, mount, TO_CHAR);
                }
                mount->mount = NULL;
                victim->mount = NULL;
                mount->mounted = IS_ON_FOOT;
                victim->mounted = IS_ON_FOOT;
        }
        if (!IS_NPC(victim) && victim->hit <= -11 && IS_HERO(victim))
        {
                victim->hit = -10;
                if (victim->position == POS_FIGHTING)
                        stop_fighting(victim, TRUE);
                return;
        }
        if (!IS_NPC(victim) && victim->hit < -9 && !IS_HERO(victim))
        {
                victim->hit = 1;
                do_killperson(victim, victim->name);
                victim->mdeath = victim->mdeath + 1;
        }
        if (IS_NPC(victim) && victim->hit < -6)
        {
                victim->position = POS_DEAD;
                return;
        }
        if (victim->hit <= -6)
                victim->position = POS_MORTAL;
        else if (victim->hit <= -3)
                victim->position = POS_INCAP;
        else
                victim->position = POS_STUNNED;
        return;
}

/*
 * Start fights.
 */
void set_fighting(CHAR_DATA * ch, CHAR_DATA * victim, bool aggressive)
{
        if(!IS_NPC(ch) && !IS_NPC(victim) && ch != victim)
	{
                if(ch->combat == NULL) ch->combat = victim;
                if(victim->combat == NULL) victim->combat = ch;
                ch->fight_timer += 1;
                victim->fight_timer += 1;
        }

        if (ch->fighting != NULL)
                return;
        ch->fighting = victim;
        ch->position = POS_FIGHTING;
        ch->damcap[DAM_CHANGE] = 1;
        autodrop(ch);
        autodrop(victim);
        dropinvis(ch);
        dropinvis(victim);
        if (aggressive)
                update_aggressive_flags(ch, victim);
        return;
}

/*
 * Stop fights.
 */
void stop_embrace(CHAR_DATA * ch, CHAR_DATA * victim)
{
        if (victim == NULL)
        {
                ch->embracing = NULL;
                return;
        }
        victim->embraced = NULL;
        ch->embracing = NULL;
        return;
}
void stop_fighting( CHAR_DATA *ch, bool fBoth )  
{  
  CHAR_DATA *fch, *fch_next;  
  if ( ch->in_room != NULL && fBoth )  
  {  
    for ( fch = ch->in_room->people; fch != NULL; fch = fch_next )  
    {  
      fch_next = fch->next_in_room;  
      if ( fch->fighting == ch && fch != ch )  
      {  
        fch->fighting = NULL;  
        fch->position = POS_STANDING;  
        update_pos( fch );  
      }  
    }  
  }  
  ch->fighting = NULL;  
  ch->position = POS_STANDING;  
  update_pos( ch );  
} 
/*
void stop_fighting(CHAR_DATA * ch, bool fBoth)
{
        CHAR_DATA *fch;
        char      buf[MSL];

        for (fch = ch->in_room->people; fch != NULL; fch = fch->next)
        {
                if (fch == ch || (fBoth && fch->fighting == ch))
                {
                        fch->fighting = NULL;
                        fch->position = POS_STANDING;
                        update_pos(fch);
                        if (IS_SET(fch->act2, PLR_AUTOSTORE))
                                if (fch->tells)
                                {
                                        xprintf(buf,
                                                "You have #R%d#n tells waiting.\n\r",
                                                fch->tells);
                                        send_to_char(buf, fch);
                                        send_to_char
                                                ("Type 'replay' to see tells.\n\r",
                                                 fch);
                                }
                }
        }
        return;
}
*/
/*
 * Make a corpse out of a character.
 */
void make_corpse(CHAR_DATA * ch)
{
        char      buf[MAX_STRING_LENGTH];
        OBJ_DATA *corpse;
        OBJ_DATA *obj;
        OBJ_DATA *obj_next;
        char     *name;
        int       rndbones = 0;
        extern OBJ_DATA *aquest_object;


        if (!ch)
                return;
        if (ch->in_room == NULL)
        {
                return;
        }
        rndbones = number_range(15, 30);
        if (IS_NPC(ch))
        {
                name = ch->short_descr;
                corpse = create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC), 0);
                corpse->timer = number_range(4, 8);
                corpse->value[2] = ch->pIndexData->vnum;
                if (global_qp)
                {
                        rndbones *= 2;
                }
                rndbones += number_range(0, ch->level / 300);
                if (ch->bones > 0)
                {
                        obj_to_obj(create_money(ch->bones), corpse);
                        ch->bones = 0;
                }
                else
                {
                        obj_to_obj(create_money(rndbones), corpse);
                }
        }
        else
        {
                name = ch->name;
                corpse = create_object(get_obj_index(OBJ_VNUM_CORPSE_PC), 0);
                corpse->timer = number_range(25, 40);
             /*   if (ch->bones > 0)
                {
                        obj = create_money(ch->bones);
                        if (IS_AFFECTED(ch, AFF_SHADOWPLANE)
                            && (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)))
                                SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
                        obj_to_obj(obj, corpse);
                        ch->bones = 0;
                }*/
        }
        if (IS_SET(ch->extra, EXTRA_ZOMBIE))
                SET_BIT(corpse->quest, QUEST_ZOMBIE);
        xprintf(buf, corpse->short_descr, name);
        free_string(corpse->short_descr);
        corpse->short_descr = str_dup(buf);
        xprintf(buf, corpse->description, name);
        free_string(corpse->description);
        corpse->description = str_dup(buf);
        for (obj = ch->carrying; obj != NULL; obj = obj_next)
        {
                obj_next = obj->next_content;
                obj_from_char(obj);
                if (obj == aquest_object)
                {
                        act("$p falls to the floor.", ch, obj, NULL, TO_ROOM);
                        obj_to_room(obj, ch->in_room);
                }
                else if (IS_SET(obj->extra_flags, ITEM_VANISH))
                        extract_obj(obj);
                else
                {
                        if (IS_AFFECTED(ch, AFF_SHADOWPLANE)
                            && (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)))
                                SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
                        obj_to_obj(obj, corpse);
                }
        }
        if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
                SET_BIT(corpse->extra_flags, ITEM_SHADOWPLANE);
        obj_to_room(corpse, ch->in_room);
        return;
}

void make_part(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        int       vnum;

        argument = one_argument(argument, arg);
        vnum = 0;

        if (arg[0] == '\0')
                return;
        if (ch->in_room == NULL)
                return;

        if (!str_cmp(arg, "head"))
                vnum = OBJ_VNUM_SEVERED_HEAD;
        else if (!str_cmp(arg, "arm"))
                vnum = OBJ_VNUM_SLICED_ARM;
        else if (!str_cmp(arg, "leg"))
                vnum = OBJ_VNUM_SLICED_LEG;
        else if (!str_cmp(arg, "heart"))
                vnum = OBJ_VNUM_TORN_HEART;
        else if (!str_cmp(arg, "turd"))
                vnum = OBJ_VNUM_TORN_HEART;
        else if (!str_cmp(arg, "entrails"))
                vnum = OBJ_VNUM_SPILLED_ENTRAILS;
        else if (!str_cmp(arg, "brain"))
                vnum = OBJ_VNUM_QUIVERING_BRAIN;
        else if (!str_cmp(arg, "eyeball"))
                vnum = OBJ_VNUM_SQUIDGY_EYEBALL;
        else if (!str_cmp(arg, "blood"))
                vnum = OBJ_VNUM_SPILT_BLOOD;
        else if (!str_cmp(arg, "face"))
                vnum = OBJ_VNUM_RIPPED_FACE;
        else if (!str_cmp(arg, "windpipe"))
                vnum = OBJ_VNUM_TORN_WINDPIPE;
        else if (!str_cmp(arg, "cracked_head"))
                vnum = OBJ_VNUM_CRACKED_HEAD;
        else if (!str_cmp(arg, "ear"))
                vnum = OBJ_VNUM_SLICED_EAR;
        else if (!str_cmp(arg, "nose"))
                vnum = OBJ_VNUM_SLICED_NOSE;
        else if (!str_cmp(arg, "tooth"))
                vnum = OBJ_VNUM_KNOCKED_TOOTH;
        else if (!str_cmp(arg, "tongue"))
                vnum = OBJ_VNUM_TORN_TONGUE;
        else if (!str_cmp(arg, "hand"))
                vnum = OBJ_VNUM_SEVERED_HAND;
        else if (!str_cmp(arg, "foot"))
                vnum = OBJ_VNUM_SEVERED_FOOT;
        else if (!str_cmp(arg, "thumb"))
                vnum = OBJ_VNUM_SEVERED_THUMB;
        else if (!str_cmp(arg, "index"))
                vnum = OBJ_VNUM_SEVERED_INDEX;
        else if (!str_cmp(arg, "middle"))
                vnum = OBJ_VNUM_SEVERED_MIDDLE;
        else if (!str_cmp(arg, "ring"))
                vnum = OBJ_VNUM_SEVERED_RING;
        else if (!str_cmp(arg, "little"))
                vnum = OBJ_VNUM_SEVERED_LITTLE;
        else if (!str_cmp(arg, "toe"))
                vnum = OBJ_VNUM_SEVERED_TOE;

        if (vnum != 0)
        {
                char      buf[MAX_STRING_LENGTH];
                OBJ_DATA *obj;
                char     *name;

                name = IS_NPC(ch) ? ch->short_descr : ch->name;
                obj = create_object(get_obj_index(vnum), 0);
                obj->timer = number_range(4, 7);
                if (!str_cmp(arg, "head") && IS_NPC(ch))
                        obj->value[1] = ch->pIndexData->vnum;
                else if (!str_cmp(arg, "head") && !IS_NPC(ch))
                {
                        ch->pcdata->chobj = obj;
                        obj->chobj = ch;
                        obj->timer = number_range(1, 2);
                        obj->item_type = ITEM_HEAD;
                }
                else if (!str_cmp(arg, "arm"))
                        SET_BIT(obj->extra_flags2, ITEM_ARM);
                else if (!str_cmp(arg, "brain") && !IS_NPC(ch)
                         && IS_AFFECTED(ch, AFF_POLYMORPH)
                         && IS_HEAD(ch, LOST_HEAD))
                {
                        if (ch->pcdata->chobj != NULL)
                                ch->pcdata->chobj->chobj = NULL;
                        ch->pcdata->chobj = obj;
                        obj->chobj = ch;
                        obj->timer = number_range(1, 2);
                        obj->item_type = ITEM_HEAD;
                }
                if (vnum == OBJ_VNUM_SPILT_BLOOD)
                        obj->timer = 2;
                if (!IS_NPC(ch))
                {
                        xprintf(buf, obj->name, name);
                        free_string(obj->name);
                        obj->name = str_dup(buf);
                }
                else
                {
                        xprintf(buf, obj->name, "mob");
                        free_string(obj->name);
                        obj->name = str_dup(buf);
                }
                xprintf(buf, obj->short_descr, name);
                free_string(obj->short_descr);
                obj->short_descr = str_dup(buf);
                xprintf(buf, obj->description, name);
                free_string(obj->description);
                obj->description = str_dup(buf);
                if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
                        SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
                obj_to_room(obj, ch->in_room);
        }
        return;
}

void raw_kill(CHAR_DATA * victim, CHAR_DATA * killer)
{
        char      buf[MAX_STRING_LENGTH];
        CHAR_DATA *mount;

        stop_fighting(victim, TRUE);
        make_corpse(victim);

        if (victim == quest_target)
                quest_target = NULL;
        if (victim == quest_mob)
        {
                xprintf(buf,
                        "Oh well, I guess the quest is over, since %s is killing me!!!!!",
                        killer->name);
                do_qtalk(victim, buf);
                quest_mob = NULL;
        }

        if (IS_NPC(victim))
                killer->total_kills++;


        if ((mount = victim->mount) != NULL)
        {
                if (victim->mounted == IS_MOUNT)
                {
                        act("$n rolls off the corpse of $N.", mount, NULL,
                            victim, TO_ROOM);
                        act("You roll off the corpse of $N.", mount, NULL,
                            victim, TO_CHAR);
                }
                else if (victim->mounted == IS_RIDING)
                {
                        act("$n falls off $N.", victim, NULL, mount, TO_ROOM);
                        act("You fall off $N.", victim, NULL, mount, TO_CHAR);
                }
                mount->mount = NULL;
                victim->mount = NULL;
                mount->mounted = IS_ON_FOOT;
                victim->mounted = IS_ON_FOOT;
        }
        if (IS_NPC(victim))
        {
                victim->pIndexData->killed++;
                kill_table[URANGE(0, victim->level, MAX_LEVEL - 1)].killed++;
                extract_char(victim, TRUE);
                return;
        }
        extract_char(victim, FALSE);
        while (victim->affected)
                affect_remove(victim, victim->affected);
        if (IS_AFFECTED(victim, AFF_POLYMORPH)
            && IS_AFFECTED(victim, AFF_ETHEREAL))
        {
                victim->affected_by = AFF_POLYMORPH + AFF_ETHEREAL;
        }
        else if (IS_AFFECTED(victim, AFF_POLYMORPH))
                victim->affected_by = AFF_POLYMORPH;
        else if (IS_AFFECTED(victim, AFF_ETHEREAL))
                victim->affected_by = AFF_ETHEREAL;
        else
                victim->affected_by = 0;
        REMOVE_BIT(victim->immune, IMM_STAKE);
        REMOVE_BIT(victim->extra, TIED_UP);
        REMOVE_BIT(victim->extra, GAGGED);
        REMOVE_BIT(victim->extra, BLINDFOLDED);
        victim->pcdata->stats[DEMON_POWER] = 0;
        victim->itemaffect = 0;
        victim->loc_hp[0] = 0;
        victim->loc_hp[1] = 0;
        victim->loc_hp[2] = 0;
        victim->loc_hp[3] = 0;
        victim->loc_hp[4] = 0;
        victim->loc_hp[5] = 0;
        victim->loc_hp[6] = 0;
        victim->armor = 100;
        victim->position = POS_RESTING;
        victim->hit = UMAX(1, victim->hit);
        victim->mana = UMAX(1, victim->mana);
        victim->move = UMAX(1, victim->move);
        victim->hitroll = 0;
        victim->damroll = 0;
        victim->saving_throw = 0;
        victim->carry_weight = 0;
        victim->carry_number = 0;
        do_call(victim, "all");
        save_char_obj(victim);
        return;
}

void behead(CHAR_DATA * victim)
{
        char      buf[MAX_STRING_LENGTH];
        ROOM_INDEX_DATA *location;

        if (IS_NPC(victim))
                return;
        powerdown(victim);  /* remove all class shit - Jobo */
        location = victim->in_room;
        stop_fighting(victim, TRUE);
        make_corpse(victim);
        extract_char(victim, FALSE);
        char_from_room(victim);
        char_to_room(victim, location);
        if (victim == NULL)
        {
                bug("Behead: Victim no longer exists.", 0);
                return;
        }
        make_part(victim, "head");
        while (victim->affected)
                affect_remove(victim, victim->affected);
        victim->affected_by = 0;
        REMOVE_BIT(victim->pcdata->jflags, JFLAG_DRONEWORLD);
        REMOVE_BIT(victim->immune, IMM_STAKE);
        REMOVE_BIT(victim->extra, TIED_UP);
        REMOVE_BIT(victim->extra, GAGGED);
        REMOVE_BIT(victim->extra, BLINDFOLDED);
        REMOVE_BIT(victim->extra, EXTRA_PREGNANT);
        REMOVE_BIT(victim->extra, EXTRA_LABOUR);
        victim->pcdata->stats[DEMON_POWER] = 0;
        victim->itemaffect = 0;
        victim->loc_hp[0] = 0;
        victim->loc_hp[1] = 0;
        victim->loc_hp[2] = 0;
        victim->loc_hp[3] = 0;
        victim->loc_hp[4] = 0;
        victim->loc_hp[5] = 0;
        victim->loc_hp[6] = 0;
        victim->affected_by = 0;
        victim->armor = 100;
        victim->position = POS_STANDING;
        victim->hit = 1;
        victim->mana = UMAX(1, victim->mana);
        victim->move = UMAX(1, victim->move);
        victim->hitroll = 0;
        victim->damroll = 0;
        victim->saving_throw = 0;
        victim->carry_weight = 0;
        victim->carry_number = 0;
        victim->pcdata->sit_safe = 0;
        victim->fight_timer = 0;
        SET_BIT(victim->loc_hp[0], LOST_HEAD);
        SET_BIT(victim->affected_by, AFF_POLYMORPH);
        xprintf(buf, "the severed head of %s", victim->name);
        free_string(victim->morph);
        victim->morph = str_dup(buf);
        do_call(victim, "all");
        save_char_obj(victim);
        return;
}

void group_gain(CHAR_DATA * ch, CHAR_DATA * victim)
{
        char      buf[MAX_STRING_LENGTH];
        char      buf2[MAX_STRING_LENGTH];
        CHAR_DATA *gch;
        CHAR_DATA *lch;
        CHAR_DATA *mount;
        OBJ_DATA *familiar;
        OBJ_INDEX_DATA *familiar2;
        bool      warlord = FALSE;
        int       xp;
        int       temp = 0;
        int       members;
        int       expneeded = 0;
        int       familiar_exp;
        int       ultima = 78019;
        int       bladeexp;
	int	  bladeexpneeded = 0;
        /*
         * Monsters don't get kill xp's or alignment changes.
         * P-killing doesn't help either.
         * Dying of mortal wounds or poison doesn't give xp to anyone!
         */
        if ((IS_NPC(ch) && (mount = ch->mount) == NULL) || victim == ch)
                return;
        members = 0;
        for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
        {
                if (IS_NPC(gch))
                        continue;
                if (is_same_group(gch, ch))
                        members++;
        }
        if (members == 0)
        {
                bug("Group_gain: members.", members);
                members = 1;
        }
        if (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_WARLORD)
                warlord = TRUE;
        lch = (ch->leader != NULL) ? ch->leader : ch;
        for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
        {
                int       xp_modifier = 100;

                if (IS_NPC(gch))
                        continue;
                if (!is_same_group(gch, ch))
                        continue;
                xp = xp_compute(gch, victim) * 2.5;

                xp *= ccenter[CCENTER_EXP_LEVEL];
                xp /= 100;

                xprintf(buf2, "#0-=-=-= #WExp modifiers  #G:");
                if ((IS_EVIL(gch) && IS_GOOD(victim))
                    || (IS_GOOD(gch) && IS_EVIL(victim)))
                {
                        xp_modifier += 25;
                        strcat(buf2, " #Calignment#n");
                }
                if ((IS_EVIL(gch) && IS_EVIL(victim))
                    || (IS_GOOD(gch) && IS_GOOD(victim)))
                {
                        xp_modifier -= 25;
                        strcat(buf2, " #palignment#n");
                }
                if (gch != NULL && gch->desc != NULL)
                {
                        if (gch->desc->out_compress)
                        {
                                xp_modifier += 25;
                                strcat(buf2, " #Cmccp#n");
                        }
                        else if ((get_age(gch) - 17) >= 2)
                        {
                                xp_modifier += 25;
                                strcat(buf2, " #pmccp#n");
                        }
                }
                if ((get_age(gch) - 17) < 2)    /* 4 hours worth of newbie exp. */
                {
                        xp_modifier += 200;
                        strcat(buf2, " #CNewbie#n");
                }
                if (global_exp)
                {
                        xp_modifier += 200;
                        strcat(buf2, " #sDouble exp#n");
                }
                if (gch->tick_timer[TIMER_EXPERIENCE] > 0)
                {
                        xp_modifier += 250;
                        strcat(buf2, " #sFairy#n");
                }
                if (gch->race < 1);
                else if (gch->race < 5)
                {
                        xp_modifier += 25;
                        xp_modifier += UMIN(100, temp / 50);
                        strcat(buf2, " #0status#n");
                }
                else if (gch->race < 10)
                {
                        xp_modifier += 50;
                        xp_modifier += UMIN(100, temp / 50);
                        strcat(buf2, " #cstatus#n");
                }
                else if (gch->race < 15)
                {
                        xp_modifier += 75;
                        xp_modifier += UMIN(100, temp / 50);
                        strcat(buf2, " #Cstatus#n");
                }
                else if (gch->race < 20)
                {
                        xp_modifier += 100;
                        xp_modifier += UMIN(100, temp / 50);
                        strcat(buf2, " #Bstatus#n");
                }
                else if (gch->race < 25)
                {
                        xp += 150;
                        xp_modifier += UMIN(100, temp / 50);
                        strcat(buf2, " #gstatus#n");
                }
                else
                {
                        xp += 200;
                        xp_modifier += UMIN(100, temp / 50);
                        strcat(buf2, " #Gstatus#n");
                }
                if (!IS_NPC(gch) && gch->pcdata->religion != 0)
                {
                        xp_modifier -= 5;
                        strcat(buf2, " #ptithe#n");
                }
                if (!IS_NPC(gch) && gch->pcdata->kingdom != 0)
                {
                        xp_modifier -= 5;
                        strcat(buf2, " #pKingdom Tax#n");
                }
                if (!IS_NPC(gch) && gch->pcdata->time_tick > 49)
                {
                        xp_modifier += gch->pcdata->time_tick / 5;
                        strcat(buf2, " #Caddict#n");
                }
                if (!IS_NPC(gch) && IS_SET(gch->pcdata->tempflag, TEMP_EDGE))
                {
                        xp_modifier += 15;
                        strcat(buf2, " #Cedge#n");
                }
                if (members > 1)
                {
                        strcat(buf2, " #pgrouped#n");
                        xp_modifier *= 100;
                        xp_modifier /= 75 * members;
                }
                strcat(buf2, " #0=-=-=-#n\n\r");
                xp = xp * xp_modifier / 100;
                if (!IS_SET(gch->act, PLR_BRIEF5))
                        send_to_char(buf2, gch);
                xprintf(buf2, "#RTotal modifier #G:#n %d percent bonus\n\r",
                        xp_modifier - 100);
                if (!IS_SET(gch->act, PLR_BRIEF5))
                        send_to_char(buf2, gch);
                if ((get_age(gch) - 17) > 2 && ch->level < 3)
                        xp /= 10;   /* anti mortal exp bot thingie */
                if (gch->exp > 5000000)
                        xp = 5000000;   /* avoid overflow */
                if (xp > ccenter[CCENTER_MAX_EXP])
                        xp = number_range(ccenter[CCENTER_MAX_EXP] * 0.9,
                                          ccenter[CCENTER_MAX_EXP] * 1.1);
                if (warlord)
                        xp = 5000000;

                if ((get_age(gch) - 17) >= 2
                    && !IS_SET(gch->pcdata->jflags, JFLAG_POLICY))
                {
                        xp = 0;
                        send_to_char
                                ("#BYou must accept the policy before you can gain experience [#wHELP POLICY#B]#n\n\r",
                                 gch);
                }
                if (xp < 0)
                        xp = 0;
                xprintf(buf, "#R-->#0You receive #W%d #0experience points.#n\n\r", xp);
                if (ch->generation == 6)
                        expneeded = 13000000;
                else if (ch->generation == 5)
                        expneeded = 150000000;
                else if (ch->generation == 4)
                        expneeded = 400000000;
                else if (ch->generation == 3)
                        expneeded = 900000000;
                else if (ch->generation == 2)
                        expneeded = 1800000000;


    if (IS_CLASS(ch, CLASS_BLADE_MASTER)) // Class by Xrakisis
     {
     if (ch->pcdata->powers[BLADE_SKILL] == 0)
         bladeexpneeded = 50000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 0)
         bladeexpneeded = 50000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 0)
         bladeexpneeded = 50000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 1)
         bladeexpneeded = 100000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 1)
         bladeexpneeded = 100000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 1)
         bladeexpneeded = 100000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 2)
         bladeexpneeded = 150000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 2)
         bladeexpneeded = 150000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 2)
         bladeexpneeded = 150000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 3)
         bladeexpneeded = 150000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 3)
         bladeexpneeded = 150000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 3)
         bladeexpneeded = 150000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 4)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 4)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 4)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 5)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 5)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 5)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 6)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 6)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 6)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 7)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 7)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 7)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 8)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 8)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 8)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 9)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 9)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 9)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 10)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 10)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 10)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 11)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 11)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 11)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 12)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 12)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 12)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 13)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 13)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 13)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 14)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 14)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 14)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 15)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 15)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 15)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 16)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 16)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 16)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 17)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 17)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 17)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 18)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 18)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 18)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_SKILL] == 19)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_PROWESS] == 19)
         bladeexpneeded = 200000000;
     else if (ch->pcdata->powers[BLADE_MELEE] == 19)
         bladeexpneeded = 200000000;
}

                if (!IS_SET(gch->act, PLR_BRIEF5))
                        send_to_char(buf, gch);

                ch->total_exp_gained += xp;
                if ((mount = gch->mount) != NULL)
                        send_to_char(buf, mount);
                gain_exp(gch, xp);

                /*
                 * Familiar Stuff 
                 */

                if ((familiar = get_eq_char(gch, WEAR_FAMILIAR)) != NULL
                    && familiar->item_type == ITEM_FAMILIAR
                    && familiar->value[1] < 99)
                {
                        familiar_exp = (xp * .005);
                        xp -= (familiar_exp);
                        familiar_exp *= (familiar->value[6] * .01);
                        sprintf(buf,
                                "#0- #wYour familiar receives #G%d #wexperience points.#n\n\r",
                                familiar_exp);
                        if (!IS_SET(ch->act, PLR_BRIEF4))
                                stc(buf, gch);
                        familiar->value[5] += familiar_exp;

                        /*
                         * Level Up!
                         */
                        if (familiar->value[5] >=
                            ((familiar->value[1] + 1) * 1000000))
                        {
                                act("$p #yhas gained a level!#n", gch,
                                    familiar, NULL, TO_CHAR);
                                familiar->value[1]++;
                                familiar->value[5] = 0;

                                /*
                                 * Increase Chance to Hit
                                 */
                                if (number_percent() > 50)
                                {
                                        familiar->value[4]++;
                                        act("$p's #yhitroll increases!#n",
                                            gch, familiar, NULL, TO_CHAR);
                                }

                                /*
                                 * Base Damage Increase
                                 */
                                if ((familiar->value[1] % 5) == 0)
                                {
                                        familiar->value[2] +=
                                                number_range(2, 5);
                                        act("$p's #Rbase damage increases!#n",
                                            gch, familiar, NULL, TO_CHAR);
                                }

                                /*
                                 * Learning Ability Increase
                                 */
                                if ((familiar->value[1] % 10) == 0)
                                {
                                        familiar->value[6] +=
                                                number_range(1, 10);
                                        act("$p's #Clearning ability increases!#n", gch, familiar, NULL, TO_CHAR);
                                }
                                if ((familiar->value[0] == 8
                                     && familiar->value[1] >= 50))
                                {
                                        act("$p #Lmetamorphs into the ultimate weapon!!!!#n", gch, familiar, NULL, TO_CHAR);
                                        extract_obj(familiar);
                                        familiar2 = get_obj_index(ultima);
                                        if ((familiar =
                                             create_object(familiar2,
                                                           50)) != NULL)
                                        {
                                                obj_to_char(familiar, gch);
                                        }
                                }
                        }
                }

    if (IS_CLASS(ch, CLASS_BLADE_MASTER) && (ch->pcdata->powers[BLADE_MELEE] < 20
    || ch->pcdata->powers[BLADE_PROWESS] || ch->pcdata->powers[BLADE_SKILL] < 20))
      {
	ch->bladeexpneeded += number_range((xp / 10), (xp / 2));
	if (ch->bladeexpneeded >= bladeexp)

     if (ch->pcdata->powers[BLADE_SKILL] == 0)
     {  
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7One#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 0)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7One#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 0)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your BladeMaster Melee to level #7One#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 1)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Two#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 1)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Two#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 1)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Two#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 2)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Three#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 2)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Three#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 2)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Three#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 3)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Four#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 3)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Four#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 3)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Four#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 4)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Five#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 4)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Five#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 4)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Five#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 5)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Six#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 5)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Six#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 5)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Six#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 6)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Seven#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 6)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Seven#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 6)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Seven#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 7)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Eight#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 7)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Eight#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 7)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Eight#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 8)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Nine#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 8)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Nine#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 8)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Nine#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 9)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Ten#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 9)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Ten#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 9)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Ten#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 10)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Eleven#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 10)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Eleven#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 10)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Eleven#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 11)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Twelve#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 11)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Twelve#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 11)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Twelve#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 12)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Thirteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 12)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Thirteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 12)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Thirteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 13)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Fourteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 13)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Fourteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 13)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Fourteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 14)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Fifteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 14)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Fifteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 14)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Fifteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 15)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Sixteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 15)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Sixteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 15)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Sixteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 16)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Seventeen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 16)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Seventeen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 16)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Seventeen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 17)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Eighteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 17)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Eighteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 17)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Eighteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 18)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Nineteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 18)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Nineteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 18)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Nineteen#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_SKILL] == 19)
     {
         ch->pcdata->powers[BLADE_SKILL] += 1;
         send_to_char("#0Your Skill with a blade is now level #7Twenty#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_PROWESS] == 19)
     {
         ch->pcdata->powers[BLADE_PROWESS] += 1;
         send_to_char("#0Your combat prowess is now level #7Twenty#n.\n\r",ch);
	   ch->bladeexp = 0;
     }
     else if (ch->pcdata->powers[BLADE_MELEE] == 19)
     {
         ch->pcdata->powers[BLADE_MELEE] += 1;
         send_to_char("#0You bring your Blade Master Melee to level #7Twenty#n.\n\r",ch);
	   ch->bladeexp = 0;
     }

      
}

                if ((IS_CLASS(ch, CLASS_BLADE_MASTER) || !IS_CLASS(ch, CLASS_BLADE_MASTER)) 
		&& ch->generation > 1 && ch->level > 2)
                {
                        ch->genexp += number_range((xp / 10), (xp / 2));
                        if (ch->genexp >= expneeded)
                        {
                                ch->genexp = 0;
                                ch->generation -= 1;
                                xprintf(buf,"#B%s #0has advanced a #GGeneration!#n",ch->name);
                               do_info(ch, buf);
                                do_save(ch, "");
                        }
                }
 
      }
        return;
}

int xp_compute(CHAR_DATA * gch, CHAR_DATA * victim)
{
        double    xp;

        if (victim->level < 100)
                xp = 300 - URANGE(-5, 3 - (victim->level * 5 / 3), 6) * 50;
        else
                xp = 300 - URANGE(-10, 3 - (victim->level * 5 / 3), 6) * 50;

        /*
         * Adjust for popularity of target:
         *   -1/8 for each target over  'par' (down to -100%)
         *   +1/8 for each target under 'par' (  up to + 25%)
         */
        xp -= xp * number_range(-2, 2) / 8;
        xp = number_range(xp * 3 / 4, xp * 5 / 4);
        xp = UMAX(0, xp);
        xp = (xp * (victim->level) * 0.60);
        xp = xp / 2;    /* Put in cause players compaling to much exp :P */
        if (nodamcap)
                xp = xp / 2;
        if (!IS_NPC(gch))
        {
                gch->pcdata->score[SCORE_TOTAL_LEVEL] += victim->level;
                if (victim->level > gch->pcdata->score[SCORE_HIGH_LEVEL])
                        gch->pcdata->score[SCORE_HIGH_LEVEL] += victim->level;
                gch->pcdata->score[SCORE_TOTAL_XP] += xp;
                if (xp > gch->pcdata->score[SCORE_HIGH_XP])
                        gch->pcdata->score[SCORE_HIGH_XP] += xp;
        }
        if (!IS_NPC(gch) && xp > 499 && gch->pcdata->disc_points != 999
            && gch->pcdata->disc_research != -1)
        {
                if (!IS_SET(gch->act, PLR_BRIEF4)
                    && !IS_SET(gch->act, PLR_BRIEF5))
                        stc("#RYou gained a discipline point.#n\n\r", gch);
                else if (IS_SET(gch->act, PLR_BRIEF5))
                        gch->total_disc_gained++;
                gain_disc_points(gch, 1);
                if ((!IS_SET(gch->act, PLR_BRIEF4)
                     && !IS_SET(gch->act, PLR_BRIEF5)) && (victim->level > 200
                                                           && gch->pcdata->
                                                           disc_points !=
                                                           999))
                {
                        if (IS_SET(gch->act, PLR_BRIEF5))
                                gch->total_disc_gained++;
                        else
                                stc("#RYou gained a discipline point.#n\n\r",
                                    gch);
                        gain_disc_points(gch, 1);
                }
                else if (IS_SET(gch->act, PLR_BRIEF5))
                        gch->total_disc_gained++;
                if ((!IS_SET(gch->act, PLR_BRIEF4)
                     && !IS_SET(gch->act, PLR_BRIEF5)) && (victim->level > 400
                                                           && gch->pcdata->
                                                           disc_points !=
                                                           999))
                {
                        if (IS_SET(gch->act, PLR_BRIEF5))
                                gch->total_disc_gained++;
                        else
                                stc("#RYou gained a discipline point.#n\n\r",
                                    gch);
                        gain_disc_points(gch, 1);
                }
                else if (IS_SET(gch->act, PLR_BRIEF5))
                        gch->total_disc_gained++;
                xp -= 500;
        }
        if (xp < 0)
                xp = 0;
        if (xp > 0 && xp < ccenter[CCENTER_MIN_EXP])
                xp = number_range(ccenter[CCENTER_MIN_EXP] * 0.9,
                                  ccenter[CCENTER_MIN_EXP] * 1.1);
        if (IS_NPC(victim) && (IS_SET(victim->act, ACT_NOEXP)))
                return 0;
        return (int) xp;
}

void dam_message(CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt)
{
        static char *const attack_table[] = {
                "hit", "slice", "stab", "slash", "whip", "claw",
                "blast", "pound", "crush", "grep", "bite", "pierce", "suck"
        };
        static char *const attack_table2[] = {
                "hits", "slices", "stabs", "slashes", "whips", "claws",
                "blasts", "pounds", "crushes", "greps", "bites", "pierces",
                "sucks"
        };
        char      buf1[512], buf2[512], buf3[512];
        const char *vs;
        const char *vp;
        const char *attack;
        const char *attack2;
        int       damp;
        int       bodyloc;
        bool      critical = FALSE;
        char      punct;

        if (!IS_NPC(ch) && IS_SET(ch->newbits, NEW_REND) && dam > 0
            && number_range(1, 7) == 5)
                critical = TRUE;
        if (dam == 0)
        {
                vs = " miss";
                vp = " misses";
        }
        else if (dam <= 50)
        {
                vs = " #Clightly#n";
                vp = " #Clightly#n";
        }
        else if (dam <= 250)
        {
                vs = " #Yhard#n";
                vp = " #Yhard#n";
        }
        else if (dam <= 500)
        {
                vs = " #Bvery hard#n";
                vp = " #Bvery hard#n";
        }
        else if (dam <= 750)
        {
                vs = " #Gextremely hard!#n";
                vp = " #Gextremely hard!#n";
        }
        else if (dam <= 1250)
        {
                vs = " #wincredibly hard!#n";
                vp = "#w incredibly hard!#n";
        }
        else if (dam <= 2000)
        {
                vs = ", #rshredding flesh!#n";
                vp = ", #rshredding flesh!#n";
        }
        else if (dam <= 3000)
        {
                vs = ", #ysplintering bone!#n";
                vp = ", #ysplintering bone!#n";
        }
        else if (dam <= 4500)
        {
                vs = ", #Rspraying blood like a fine red mist!#n";
                vp = ", #Rspraying blood like a fine red mist!#n";
        }
        else if (dam <= 6000)
        {
                vs = " #Yso hard it #R#fCASTRATES!!#n";
                vp = " #Yso hard it #R#fCASTRATES!!#n";
        }
        else if (dam <= 8000)
        {
                vs = " #G<#Y*#B{#R*#B}#Y*#G> #sextracting organs #G<#Y*#B{#R*#B}#Y*#G>#n";
                vp = " #G<#Y*#B{#R*#B}#Y*#G> #sextracting organs #G<#Y*#B{#R*#B}#Y*#G>#n";
        }
        else
        {
                vs = " #R()#G()#R() #CHumiliatingly Hard #R()#G()#R()#n";
                vp = " #R()#G()#R() #CHumiliatingly Hard #R()#G()#R()#n";
        }

        /*
         * If victim's hp are less/equal to 'damp', attacker gets a death blow 
         */
        if (IS_NPC(victim))
                damp = 0;
        else
                damp = -10;
        if ((victim->hit - dam > damp) || (dt >= 0 && dt < MAX_SKILL) ||
            (IS_NPC(victim) && IS_SET(victim->act, ACT_NOPARTS)))
        {
                punct = (dam <= 250) ? ' ' : ' ';
                if (dt == TYPE_HIT && !IS_NPC(ch)
                    && !IS_VAMPAFF(ch, VAM_CLAWS))
                {
                        if (dam == 0)
                        {
                                xprintf(buf1, "$n%s $N%c #0[#w%d#0]#n", vp,
                                        punct, dam);
                                xprintf(buf2, "You%s $N%c #0[#w%d#0]#n", vs,
                                        punct, dam);
                                xprintf(buf3, "$n%s you%c", vp, punct);
                        }
                        else
                        {
                                xprintf(buf1, "$n hits $N%s%c #0[#w%d#0]#n",
                                        vp, punct, dam);
                                xprintf(buf2, "You hit $N%s%c #0[#w%d#0]#n",
                                        vs, punct, dam);
                                xprintf(buf3, "$n hits you%s%c #0[#w%d#0]#n",
                                        vp, punct, dam);
                                critical = TRUE;
                        }
                }
                else if (dt == TYPE_HIT && !IS_NPC(ch)
                         && IS_SET(ch->newbits, NEW_MONKFLAME))
                {
                        if (dam >= 1)
                        {
                                if (!IS_CLASS(ch, CLASS_WEREWOLF))
                                {
                                        xprintf(buf1,
                                                "$n's #rflaming#n hands hit $N%s%c #0[#w%d#0]#n",
                                                vp, punct, dam);
                                        xprintf(buf2,
                                                "Your #rflaming#n hands hit $N%s%c #0[#w%d#0]#n",
                                                vs, punct, dam);
                                        xprintf(buf3,
                                                "$n's #rflaming#n hands hit you%s%c #0[#w%d#0]#n",
                                                vp, punct, dam);
                                }
                                else
                                {
                                        xprintf(buf1,
                                                "$n's #rflaming#n claws hit $N%s%c #0[#w%d#0]#n",
                                                vp, punct, dam);
                                        xprintf(buf2,
                                                "Your #rflaming#n claws hit $N%s%c #0[#w%d#0]#n",
                                                vs, punct, dam);
                                        xprintf(buf3,
                                                "$n's #rflaming#n claws hit you%s%c #0[#w%d#0]#n",
                                                vp, punct, dam);
                                }
                        }
                        else
                        {
                                xprintf(buf1, "$n's hit%s $N%c #0[#w%d#0]#n",
                                        vp, punct, dam);
                                xprintf(buf2, "Your hit%s $N%c #0[#w%d#0]#n",
                                        vs, punct, dam);
                                xprintf(buf3, "$n hit%s you%c #0[#w%d#0]#n",
                                        vp, punct, dam);
                                critical = TRUE;
                        }
                }
                else
                {
                        if (dt == TYPE_HIT && !IS_NPC(ch)
                            && (IS_VAMPAFF(ch, VAM_CLAWS)))
                        {
                                attack = attack_table[dt - TYPE_HIT + 5];
                                attack2 = attack_table2[dt - TYPE_HIT + 5];
                        }
                        else if (dt >= 0 && dt < MAX_SKILL)
                        {
                                attack = skill_table[dt].noun_damage;
                                attack2 = skill_table[dt].noun_damage;
                        }
                        else if (dt >= TYPE_HIT
                                 && dt <
                                 TYPE_HIT +
                                 sizeof(attack_table) /
                                 sizeof(attack_table[0]))
                        {
                                attack = attack_table[dt - TYPE_HIT];
                                attack2 = attack_table2[dt - TYPE_HIT];
                        }
                        else
                        {
                                dt = TYPE_HIT;
                                attack = attack_table[0];
                                attack2 = attack_table2[0];
                        }
                        if (dam == 0)
                        {
                                xprintf(buf1, "$n's %s%s $N%c #0[#w%d#0]#n",
                                        attack, vp, punct, dam);
                                xprintf(buf2, "Your %s%s $N%c #0[#w%d#0]#n",
                                        attack, vp, punct, dam);
                                xprintf(buf3, "$n's %s%s you%c #0[#w%d#0]#n",
                                        attack, vp, punct, dam);
                        }
                        else if (IS_SET(ch->newbits, NEW_MONKFLAME)
                                 && (dam == 0))
                        {
                                xprintf(buf1,
                                        "$n's flaming %s%s $N%c #0[#w%d#0]#n",
                                        attack2, vp, punct, dam);
                                xprintf(buf2,
                                        "Your flaming %s%s $N%c #0[#w%d#0]#n",
                                        attack, vp, punct, dam);
                                xprintf(buf3,
                                        "$n's flaming %s%s you%c #0[#w%d#0]#n",
                                        attack2, vp, punct, dam);
                        }
                        else
                        {
                                if (dt >= 0 && dt < MAX_SKILL)
                                {
                                        xprintf(buf1,
                                                "$n's %s strikes $N%s%c #0[#w%d#0]#n",
                                                attack2, vp, punct, dam);
                                        xprintf(buf2,
                                                "Your %s strikes $N%s%c #0[#w%d#0]#n",
                                                attack, vp, punct, dam);
                                        xprintf(buf3,
                                                "$n's %s strikes you%s%c #0[#w%d#0]#n",
                                                attack2, vp, punct, dam);
                                }
                                else if (IS_SET(ch->newbits, NEW_MONKFLAME))
                                {
                                        xprintf(buf1,
                                                "$n's flaming %s hits $N%s%c #0[#w%d#0]#n",
                                                attack2, vp, punct, dam);
                                        xprintf(buf2,
                                                "Your flaming %s hits $N%s%c #0[#w%d#0]#n",
                                                attack, vp, punct, dam);
                                        xprintf(buf3,
                                                "$n's flaming %s hits you%s%c #0[#w%d#0]#n",
                                                attack2, vp, punct, dam);
                                }
                                else
                                {
                                        xprintf(buf1,
                                                "$n %s $N%s%c #0[#w%d#0]#n",
                                                attack2, vp, punct, dam);
                                        xprintf(buf2,
                                                "You %s $N%s%c #0[#w%d#0]#n",
                                                attack, vp, punct, dam);
                                        xprintf(buf3,
                                                "$n %s you%s%c #0[#w%d#0]#n",
                                                attack2, vp, punct, dam);
                                        critical = TRUE;
                                }
                        }
                }
                act_brief(buf1, ch, NULL, victim, TO_NOTVICT, PLR_BRIEF7);

                //act( buf1, ch, NULL, victim, TO_NOTVICT );
                if (!IS_SET(ch->act, PLR_BRIEF5))
                {
                        if (!
                            (IS_SET(ch->act, PLR_BRIEF2)
                             && (dam == 0
                                 || dt == skill_lookup("lightning bolt")
                                 || dt == skill_lookup("acid blast")
                                 || dt == skill_lookup("chill touch")
                                 || dt == skill_lookup("fireball"))))
                        {
                                if (!
                                    (dt == skill_lookup("chaos blast")
                                     && !IS_NPC(victim)))
                                        act(buf2, ch, NULL, victim, TO_CHAR);
                        }
                }

                /*
                 * Message to victim
                 */
                if (!IS_SET(victim->act, PLR_BRIEF5))
                {
                        if (!
                            (IS_SET(victim->act, PLR_BRIEF2)
                             && (dam == 0
                                 || dt == skill_lookup("lightning bolt")
                                 || dt == skill_lookup("acid blast")
                                 || dt == skill_lookup("chill touch")
                                 || dt == skill_lookup("fireball"))))
                                act(buf3, ch, NULL, victim, TO_VICT);
                }
                if (critical)
                        critical_hit(ch, victim, dt, dam);
                return;
        }
        if (dt == TYPE_HIT && !IS_NPC(ch) && !IS_VAMPAFF(ch, VAM_CLAWS)
            && !IS_VAMPAFF(ch, VAM_FANGS))
        {
                damp = number_range(1, 5);
                if (damp == 1)
                {
                        act("You ram your fingers into $N's eye sockets and rip $S face off.", ch, NULL, victim, TO_CHAR);
                        act("$n rams $s fingers into $N's eye sockets and rips $S face off.", ch, NULL, victim, TO_NOTVICT);
                        act("$n rams $s fingers into your eye sockets and rips your face off.", ch, NULL, victim, TO_VICT);
                        make_part(victim, "face");
                }
                else if (damp == 2)
                {
                        act("You grab $N by the throat and tear $S windpipe out.", ch, NULL, victim, TO_CHAR);
                        act("$n grabs $N by the throat and tears $S windpipe out.", ch, NULL, victim, TO_NOTVICT);
                        act("$n grabs you by the throat and tears your windpipe out.", ch, NULL, victim, TO_VICT);
                        make_part(victim, "windpipe");
                }
                else if (damp == 3)
                {
                        act("You punch your fist through $N's stomach and rip out $S entrails.", ch, NULL, victim, TO_CHAR);
                        act("$n punches $s fist through $N's stomach and rips out $S entrails.", ch, NULL, victim, TO_NOTVICT);
                        act("$n punches $s fist through your stomach and rips out your entrails.", ch, NULL, victim, TO_VICT);
                        make_part(victim, "entrails");
                }
                else if (damp == 4)
                {
                        if (!IS_BODY(victim, BROKEN_SPINE))
                                SET_BIT(victim->loc_hp[1], BROKEN_SPINE);
                        act("You hoist $N above your head and slam $M down upon your knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_CHAR);
                        act("$n hoists $N above $s head and slams $M down upon $s knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_NOTVICT);
                        act("$n hoists you above $s head and slams you down upon $s knee.\n\rThere is a loud cracking sound as your spine snaps.", ch, NULL, victim, TO_VICT);
                }
                else if (damp == 5)
                {
                        act("You lock your arm around $N's head, and give it a vicious twist.", ch, NULL, victim, TO_CHAR);
                        act("$n locks $s arm around $N's head, and gives it a vicious twist.", ch, NULL, victim, TO_NOTVICT);
                        act("$n locks $s arm around your head, and gives it a vicious twist.", ch, NULL, victim, TO_VICT);
                        if (!IS_BODY(victim, BROKEN_NECK))
                        {
                                act("There is a loud snapping noise as your neck breaks.", victim, NULL, NULL, TO_CHAR);
                                act("There is a loud snapping noise as $n's neck breaks.", victim, NULL, NULL, TO_ROOM);
                                SET_BIT(victim->loc_hp[1], BROKEN_NECK);
                        }
                }
                return;
        }
        if (dt >= 0 && dt < MAX_SKILL)
                attack = skill_table[dt].noun_damage;
        else if (dt >= TYPE_HIT
                 && dt <
                 TYPE_HIT + sizeof(attack_table) / sizeof(attack_table[0]))
                attack = attack_table[dt - TYPE_HIT];
        else
        {
                dt = TYPE_HIT;
                attack = attack_table[0];
        }
        if (attack == "slash" || attack == "slice")
        {
                damp = number_range(1, 8);
                if (damp == 1)
                {
                        act("You swing your blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_CHAR);
                        act("$n swings $s blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_NOTVICT);
                        act("$n swings $s blade in a low arc, rupturing your abdominal cavity.\n\rYour entrails spray out over a wide area.", ch, NULL, victim, TO_VICT);
                        make_part(victim, "entrails");
                }
                else if (damp == 2)
                {
                        act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR);
                        act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT);
                        act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT);
                }
                else if (damp == 3)
                {
                        if (!IS_BODY(victim, CUT_THROAT))
                                SET_BIT(victim->loc_hp[1], CUT_THROAT);
                        if (!IS_BLEEDING(victim, BLEEDING_THROAT))
                                SET_BIT(victim->loc_hp[6], BLEEDING_THROAT);
                        act("Your blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_CHAR);
                        act("$n's blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_NOTVICT);
                        act("$n's blow slices open your carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_VICT);
                        make_part(victim, "blood");
                }
                else if (damp == 4)
                {
                        if (!IS_BODY(victim, CUT_THROAT))
                                SET_BIT(victim->loc_hp[1], CUT_THROAT);
                        if (!IS_BLEEDING(victim, BLEEDING_THROAT))
                                SET_BIT(victim->loc_hp[6], BLEEDING_THROAT);
                        act("You swing your blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR);
                        act("$n swings $s blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT);
                        act("$n swings $s blade across your throat, showering the area with blood.", ch, NULL, victim, TO_VICT);
                        make_part(victim, "blood");
                }
                else if (damp == 5)
                {
                        if (!IS_HEAD(victim, BROKEN_SKULL))
                        {
                                act("You swing your blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_CHAR);
                                act("$n swings $s blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_NOTVICT);
                                act("$n swings $s blade down upon your head, splitting it open.\n\rYour brains pour out of your forehead.", ch, NULL, victim, TO_VICT);
                                make_part(victim, "brain");
                                SET_BIT(victim->loc_hp[0], BROKEN_SKULL);
                        }
                        else
                        {
                                act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
                                act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
                                act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
                        }
                }
                else if (damp == 6)
                {
                        act("You swing your blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_CHAR);
                        act("$n swings $s blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_NOTVICT);
                        act("$n swings $s blade between your legs, nearly splitting you in half.", ch, NULL, victim, TO_VICT);
                }
                else if (damp == 7)
                {
                        if (!IS_ARM_L(victim, LOST_ARM))
                        {
                                act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR);
                                act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT);
                                act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT);
                                make_part(victim, "arm");
                                SET_BIT(victim->loc_hp[2], LOST_ARM);
                                if (!IS_BLEEDING(victim, BLEEDING_ARM_L))
                                        SET_BIT(victim->loc_hp[6],
                                                BLEEDING_ARM_L);
                                if (IS_BLEEDING(victim, BLEEDING_HAND_L))
                                        REMOVE_BIT(victim->loc_hp[6],
                                                   BLEEDING_HAND_L);
                        }
                        else if (!IS_ARM_R(victim, LOST_ARM))
                        {
                                act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR);
                                act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT);
                                act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT);
                                make_part(victim, "arm");
                                SET_BIT(victim->loc_hp[3], LOST_ARM);
                                if (!IS_BLEEDING(victim, BLEEDING_ARM_R))
                                        SET_BIT(victim->loc_hp[6],
                                                BLEEDING_ARM_R);
                                if (IS_BLEEDING(victim, BLEEDING_HAND_R))
                                        REMOVE_BIT(victim->loc_hp[6],
                                                   BLEEDING_HAND_R);
                        }
                        else
                        {
                                act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
                                act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
                                act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
                        }
                }
                else if (damp == 8)
                {
                        if (!IS_LEG_L(victim, LOST_LEG))
                        {
                                act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
                                act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
                                act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT);
                                make_part(victim, "leg");
                                SET_BIT(victim->loc_hp[4], LOST_LEG);
                                if (!IS_BLEEDING(victim, BLEEDING_LEG_L))
                                        SET_BIT(victim->loc_hp[6],
                                                BLEEDING_LEG_L);
                                if (IS_BLEEDING(victim, BLEEDING_FOOT_L))
                                        REMOVE_BIT(victim->loc_hp[6],
                                                   BLEEDING_FOOT_L);
                        }
                        else if (!IS_LEG_R(victim, LOST_LEG))
                        {
                                act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
                                act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
                                act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT);
                                make_part(victim, "leg");
                                SET_BIT(victim->loc_hp[5], LOST_LEG);
                                if (!IS_BLEEDING(victim, BLEEDING_LEG_R))
                                        SET_BIT(victim->loc_hp[6],
                                                BLEEDING_LEG_R);
                                if (IS_BLEEDING(victim, BLEEDING_FOOT_R))
                                        REMOVE_BIT(victim->loc_hp[6],
                                                   BLEEDING_FOOT_R);
                        }
                        else
                        {
                                act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
                                act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
                                act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
                        }
                }
        }
        else if (attack == "stab" || attack == "pierce")
        {
                damp = number_range(1, 5);
                if (damp == 1)
                {
                        act("You defty invert your weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_CHAR);
                        act("$n defty inverts $s weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_NOTVICT);
                        act("$n defty inverts $s weapon and plunge it point first into your chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_VICT);
                        make_part(victim, "blood");
                }
                else if (damp == 2)
                {
                        act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR);
                        act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT);
                        act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT);
                }
                else if (damp == 3)
                {
                        act("You thrust your weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_CHAR);
                        act("$n thrusts $s weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_NOTVICT);
                        act("$n thrusts $s weapon up under your jaw and through your head.", ch, NULL, victim, TO_VICT);
                }
                else if (damp == 4)
                {
                        act("You ram your weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_CHAR);
                        act("$n rams $s weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_NOTVICT);
                        act("$n rams $s weapon through your body, pinning you to the ground.", ch, NULL, victim, TO_VICT);
                }
                else if (damp == 5)
                {
                        act("You stab your weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_CHAR);
                        act("$n stabs $s weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_NOTVICT);
                        act("$n stabs $s weapon into your eye and out the back of your head.", ch, NULL, victim, TO_VICT);
                        if (!IS_HEAD(victim, LOST_EYE_L)
                            && number_percent() < 50)
                                SET_BIT(victim->loc_hp[0], LOST_EYE_L);
                        else if (!IS_HEAD(victim, LOST_EYE_R))
                                SET_BIT(victim->loc_hp[0], LOST_EYE_R);
                        else if (!IS_HEAD(victim, LOST_EYE_L))
                                SET_BIT(victim->loc_hp[0], LOST_EYE_L);
                }
        }
        else if (attack == "blast" || attack == "pound" || attack == "crush")
        {
                damp = number_range(1, 3);
                bodyloc = 0;
                if (damp == 1)
                {
                        act("Your blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_CHAR);
                        act("$n's blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_NOTVICT);
                        act("$n's blow smashes through your chest, caving in half your ribcage.", ch, NULL, victim, TO_VICT);
                        if (IS_BODY(victim, BROKEN_RIBS_1))
                        {
                                bodyloc += 1;
                                REMOVE_BIT(victim->loc_hp[1], BROKEN_RIBS_1);
                        }
                        if (IS_BODY(victim, BROKEN_RIBS_2))
                        {
                                bodyloc += 2;
                                REMOVE_BIT(victim->loc_hp[1], BROKEN_RIBS_2);
                        }
                        if (IS_BODY(victim, BROKEN_RIBS_4))
                        {
                                bodyloc += 4;
                                REMOVE_BIT(victim->loc_hp[1], BROKEN_RIBS_4);
                        }
                        if (IS_BODY(victim, BROKEN_RIBS_8))
                        {
                                bodyloc += 8;
                                REMOVE_BIT(victim->loc_hp[1], BROKEN_RIBS_8);
                        }
                        if (IS_BODY(victim, BROKEN_RIBS_16))
                        {
                                bodyloc += 16;
                                REMOVE_BIT(victim->loc_hp[1], BROKEN_RIBS_16);
                        }
                        bodyloc += number_range(1, 3);
                        if (bodyloc > 24)
                                bodyloc = 24;
                        if (bodyloc >= 16)
                        {
                                bodyloc -= 16;
                                SET_BIT(victim->loc_hp[1], BROKEN_RIBS_16);
                        }
                        if (bodyloc >= 8)
                        {
                                bodyloc -= 8;
                                SET_BIT(victim->loc_hp[1], BROKEN_RIBS_8);
                        }
                        if (bodyloc >= 4)
                        {
                                bodyloc -= 4;
                                SET_BIT(victim->loc_hp[1], BROKEN_RIBS_4);
                        }
                        if (bodyloc >= 2)
                        {
                                bodyloc -= 2;
                                SET_BIT(victim->loc_hp[1], BROKEN_RIBS_2);
                        }
                        if (bodyloc >= 1)
                        {
                                bodyloc -= 1;
                                SET_BIT(victim->loc_hp[1], BROKEN_RIBS_1);
                        }
                }
                else if (damp == 2)
                {
                        act("Your blow smashes $N's spine, shattering it in several places.", ch, NULL, victim, TO_CHAR);
                        act("$n's blow smashes $N's spine, shattering it in several places.", ch, NULL, victim, TO_NOTVICT);
                        act("$n's blow smashes your spine, shattering it in several places.", ch, NULL, victim, TO_VICT);
                        if (!IS_BODY(victim, BROKEN_SPINE))
                                SET_BIT(victim->loc_hp[1], BROKEN_SPINE);
                }
                else if (damp == 3)
                {
                        if (!IS_HEAD(victim, BROKEN_SKULL))
                        {
                                act("You swing your weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, leaking out brains.", ch, NULL, victim, TO_CHAR);
                                act("$n swings $s weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, covering you with brains.", ch, NULL, victim, TO_NOTVICT);
                                act("$n swings $s weapon down upon your head.\n\rYour head cracks open like an overripe melon, spilling your brains everywhere.", ch, NULL, victim, TO_VICT);
                                make_part(victim, "brain");
                                SET_BIT(victim->loc_hp[0], BROKEN_SKULL);
                        }
                        else
                        {
                                act("You hammer your weapon into $N's side, crushing bone.", ch, NULL, victim, TO_CHAR);
                                act("$n hammers $s weapon into $N's side, crushing bone.", ch, NULL, victim, TO_NOTVICT);
                                act("$n hammers $s weapon into your side, crushing bone.", ch, NULL, victim, TO_VICT);
                        }
                }
        }
        else if (!IS_NPC(ch)
                 && (attack == "bite" || IS_VAMPAFF(ch, VAM_FANGS)))
        {
                act("You sink your teeth into $N's throat and tear out $S jugular vein.\n\rYou wipe the blood from your chin with one hand.", ch, NULL, victim, TO_CHAR);
                act("$n sink $s teeth into $N's throat and tears out $S jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_NOTVICT);
                act("$n sink $s teeth into your throat and tears out your jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_VICT);
                make_part(victim, "blood");
                if (!IS_BODY(victim, CUT_THROAT))
                        SET_BIT(victim->loc_hp[1], CUT_THROAT);
                if (!IS_BLEEDING(victim, BLEEDING_THROAT))
                        SET_BIT(victim->loc_hp[6], BLEEDING_THROAT);
        }
        else if (!IS_NPC(ch)
                 && (attack == "claw" || IS_VAMPAFF(ch, VAM_CLAWS)))
        {
                damp = number_range(1, 2);
                if (damp == 1)
                {
                        act("You tear out $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR);
                        act("$n tears out $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT);
                        act("$n tears out your throat, showering the area with blood.", ch, NULL, victim, TO_VICT);
                        make_part(victim, "blood");
                        if (!IS_BODY(victim, CUT_THROAT))
                                SET_BIT(victim->loc_hp[1], CUT_THROAT);
                        if (!IS_BLEEDING(victim, BLEEDING_THROAT))
                                SET_BIT(victim->loc_hp[6], BLEEDING_THROAT);
                }
                if (damp == 2)
                {
                        if (!IS_HEAD(victim, LOST_EYE_L)
                            && number_percent() < 50)
                        {
                                act("You rip an eyeball from $N's face.", ch,
                                    NULL, victim, TO_CHAR);
                                act("$n rips an eyeball from $N's face.", ch,
                                    NULL, victim, TO_NOTVICT);
                                act("$n rips an eyeball from your face.", ch,
                                    NULL, victim, TO_VICT);
                                make_part(victim, "eyeball");
                                SET_BIT(victim->loc_hp[0], LOST_EYE_L);
                        }
                        else if (!IS_HEAD(victim, LOST_EYE_R))
                        {
                                act("You rip an eyeball from $N's face.", ch,
                                    NULL, victim, TO_CHAR);
                                act("$n rips an eyeball from $N's face.", ch,
                                    NULL, victim, TO_NOTVICT);
                                act("$n rips an eyeball from your face.", ch,
                                    NULL, victim, TO_VICT);
                                make_part(victim, "eyeball");
                                SET_BIT(victim->loc_hp[0], LOST_EYE_R);
                        }
                        else if (!IS_HEAD(victim, LOST_EYE_L))
                        {
                                act("You rip an eyeball from $N's face.", ch,
                                    NULL, victim, TO_CHAR);
                                act("$n rips an eyeball from $N's face.", ch,
                                    NULL, victim, TO_NOTVICT);
                                act("$n rips an eyeball from your face.", ch,
                                    NULL, victim, TO_VICT);
                                make_part(victim, "eyeball");
                                SET_BIT(victim->loc_hp[0], LOST_EYE_L);
                        }
                        else
                        {
                                act("You claw open $N's chest.", ch, NULL,
                                    victim, TO_CHAR);
                                act("$n claws open $N's chest.", ch, NULL,
                                    victim, TO_NOTVICT);
                                act("$n claws open $N's chest.", ch, NULL,
                                    victim, TO_VICT);
                        }
                }
        }
        else if (attack == "whip")
        {
                act("You entangle $N around the neck, and squeeze out $S life.", ch, NULL, victim, TO_CHAR);
                act("$n entangles $N around the neck, and squeezes out $S life.", ch, NULL, victim, TO_NOTVICT);
                act("$n entangles you around the neck, and squeezes the life out of you.", ch, NULL, victim, TO_VICT);
                if (!IS_BODY(victim, BROKEN_NECK))
                        SET_BIT(victim->loc_hp[1], BROKEN_NECK);
        }
        else if (attack == "suck" || attack == "grep")
        {
                act("You place your weapon on $N's head and suck out $S brains.", ch, NULL, victim, TO_CHAR);
                act("$n places $s weapon on $N's head and suck out $S brains.", ch, NULL, victim, TO_NOTVICT);
                act("$n places $s weapon on your head and suck out your brains.", ch, NULL, victim, TO_VICT);
        }
        else
        {
                /*
                 * bug( "Dam_message: bad dt %d.", dt );
                 */
        }
        return;
}

void do_surrender(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *mob;

        if ((mob = ch->fighting) == NULL)
        {
                send_to_char("But you're not fighting!\n\r", ch);
                return;
        }
        act("You surrender to $N!", ch, NULL, mob, TO_CHAR);
        act("$n surrenders to you!", ch, NULL, mob, TO_VICT);
        act("$n tries to surrender to $N!", ch, NULL, mob, TO_NOTVICT);
        stop_fighting(ch, TRUE);

        if (!IS_NPC(ch) && IS_NPC(mob)
            && (!HAS_TRIGGER_MOB(mob, TRIG_SURR)
                || !p_percent_trigger(mob, NULL, NULL, ch, NULL, NULL,
                                      TRIG_SURR)))
        {
                act("$N seems to ignore your cowardly act!", ch, NULL, mob,
                    TO_CHAR);
                multi_hit(mob, ch, TYPE_UNDEFINED);
        }
}

/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm(CHAR_DATA * ch, CHAR_DATA * victim)
{
        OBJ_DATA *obj;
        char      buf[MAX_STRING_LENGTH];

        if (IS_NPC(ch) && victim->level > 2 && number_percent() > 10)
                return;
        if (!IS_IMMUNE(ch, IMM_DISARM))
                return;
        if (((obj = get_eq_char(victim, WEAR_WIELD)) == NULL)
            || obj->item_type != ITEM_WEAPON)
        {
                if (((obj = get_eq_char(victim, WEAR_HOLD)) == NULL)
                    || obj->item_type != ITEM_WEAPON)
                        return;
        }
        xprintf(buf, "$n disarms you!");
        ADD_COLOUR(ch, buf, WHITE);
        act(buf, ch, NULL, victim, TO_VICT);
        xprintf(buf, "You disarm $N!");
        ADD_COLOUR(ch, buf, WHITE);
        act(buf, ch, NULL, victim, TO_CHAR);
        xprintf(buf, "$n disarms $N!");
        ADD_COLOUR(ch, buf, WHITE);
        act(buf, ch, NULL, victim, TO_NOTVICT);
        obj_from_char(obj);
        if (IS_SET(obj->extra_flags, ITEM_LOYAL) && (!IS_NPC(victim)))
        {
                act("$p leaps back into your hand!", victim, obj, NULL,
                    TO_CHAR);
                act("$p leaps back into $n's hand!", victim, obj, NULL,
                    TO_ROOM);
                obj_to_char(obj, victim);
                do_wear(victim, obj->name);
        }
        else if (IS_NPC(victim))
                obj_to_char(obj, victim);
        else
                obj_to_room(obj, victim->in_room);
        return;
}

/*
 * Trip a creature.
 * Caller must check for successful attack.
 */
void trip(CHAR_DATA * ch, CHAR_DATA * victim)
{
        char      buf[MAX_STRING_LENGTH];
        int       dam = 0;

        if (IS_AFFECTED(victim, AFF_FLYING) && !IS_CLASS(ch, CLASS_DRACONIAN))
                return;
        if (IS_NPC(ch) && victim->level > 2 && number_percent() > 5)
                return;
        if (!IS_NPC(victim))
        {
                if (IS_CLASS(victim, CLASS_VAMPIRE)
                    && IS_VAMPAFF(victim, VAM_FLYING)
                    && !IS_CLASS(ch, CLASS_DRACONIAN))
                        return;
                if (IS_CLASS(victim, CLASS_DEMON)
                    && IS_DEMAFF(victim, DEM_UNFOLDED)
                    && !IS_CLASS(ch, CLASS_DRACONIAN))
                        return;
                if (IS_CLASS(victim, CLASS_ANGEL)
                    && IS_SET(victim->pcdata->powers[ANGEL_POWERS],
                              ANGEL_WINGS) && !IS_CLASS(ch, CLASS_DRACONIAN))
                        return;
                if (IS_CLASS(victim, CLASS_DROW)
                    && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION)
                    && !IS_CLASS(ch, CLASS_DRACONIAN))
                        return;
                if ((IS_SET(ch->special, SPC_JEDI_LEVITATE))
                    && IS_CLASS(ch, CLASS_JEDI)
                    && !IS_CLASS(ch, CLASS_DRACONIAN))
                        return;
        }
        if (victim->wait == 0)
        {
                if (IS_CLASS(ch, CLASS_DRACONIAN))
                {
                        dam = number_range(900, 2750);

                        xprintf(buf,
                                "$n's tail lash trips you, and you go down!");
                        ADD_COLOUR(ch, buf, WHITE);
                        act(buf, ch, NULL, victim, TO_VICT);
                        xprintf(buf,
                                "Your tail lash trips $N and $E goes down!");
                        ADD_COLOUR(ch, buf, WHITE);
                        act(buf, ch, NULL, victim, TO_CHAR);
                        xprintf(buf,
                                "$n's tail lash trips $N and $E goes down!");
                        ADD_COLOUR(ch, buf, WHITE);
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        hurt_person(ch, victim, dam);
                        WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
                        victim->position = POS_RESTING;
                }

                else
                {
                        xprintf(buf, "$n trips you and you go down!");
                        ADD_COLOUR(ch, buf, WHITE);
                        act(buf, ch, NULL, victim, TO_VICT);
                        xprintf(buf, "You trip $N and $E goes down!");
                        ADD_COLOUR(ch, buf, WHITE);
                        act(buf, ch, NULL, victim, TO_CHAR);
                        xprintf(buf, "$n trips $N and $E goes down!");
                        ADD_COLOUR(ch, buf, WHITE);
                        act(buf, ch, NULL, victim, TO_NOTVICT);
                        WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
                        victim->position = POS_RESTING;
                }
        }
        return;
}

void do_kill(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;

        one_argument(argument, arg);
        if (IS_NPC(ch) && ch->desc == NULL)
                return;
        if (arg[0] == '\0')
        {
                send_to_char("Kill whom?\n\r", ch);
                return;
        }
        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }
        if (victim == ch)
        {
                send_to_char("You cannot kill yourself!\n\r", ch);
                return;
        }
        if (is_safe(ch, victim))
                return;
        if (IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim)
        {
                act("$N is your beloved master.", ch, NULL, victim, TO_CHAR);
                return;
        }
        if (ch->position == POS_FIGHTING)
        {
                send_to_char("You do the best you can!\n\r", ch);
                return;
        }
//        WAIT_STATE(ch, 1 * PULSE_VIOLENCE);
	WAIT_STATE(ch, 3);
        if (!IS_NPC(ch) && !IS_NPC(victim))
        {
                ch->fight_timer += 3;
                victim->fight_timer += 3;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF)
            && number_range(1, 3) == 1 && ch->power[DISC_WERE_BOAR] > 1
            && victim->position == POS_STANDING)
        {
                act("You charge into $N, knocking $M from $S feet.", ch, NULL,
                    victim, TO_CHAR);
                act("$n charge into $N, knocking $M from $S feet.", ch, NULL,
                    victim, TO_NOTVICT);
                act("$n charge into you, knocking you from your feet.", ch,
                    NULL, victim, TO_VICT);
                victim->position = POS_STUNNED;
                multi_hit(ch, victim, TYPE_UNDEFINED);
                multi_hit(ch, victim, TYPE_UNDEFINED);
                return;
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL))
        {
                act("You swoop down from the sky and charge into $N.", ch,
                    NULL, victim, TO_CHAR);
                act("$n swoops down from the sky and charges into $N.", ch,
                    NULL, victim, TO_NOTVICT);
                act("$n swoops down from the sky and charges into you.", ch,
                    NULL, victim, TO_VICT);
                if (ch->pcdata->powers[ANGEL_JUSTICE] >= 5)
                        multi_hit(ch, victim, TYPE_UNDEFINED);
                else if (number_range(1, 3) == 1)
                        multi_hit(ch, victim, TYPE_UNDEFINED);
        }
        multi_hit(ch, victim, TYPE_UNDEFINED);
        return;
}

void do_backstab(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        OBJ_DATA *obj;

        one_argument(argument, arg);
        if (arg[0] == '\0')
        {
                send_to_char("Backstab whom?\n\r", ch);
                return;
        }
        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }
        if (victim == ch)
        {
                send_to_char("How can you sneak up on yourself?\n\r", ch);
                return;
        }
        if (is_safe(ch, victim))
                return;
        if (((obj = get_eq_char(ch, WEAR_WIELD)) == NULL
             || obj->value[3] != 11)
            && ((obj = get_eq_char(ch, WEAR_HOLD)) == NULL
                || obj->value[3] != 11))
        {
                send_to_char("You need to wield a piercing weapon.\n\r", ch);
                return;
        }
        if (victim->fighting != NULL)
        {
                send_to_char("You can't backstab a fighting person.\n\r", ch);
                return;
        }
        if (victim->hit < victim->max_hit)
        {
                act("$N is hurt and suspicious ... you can't sneak up.",
                    ch, NULL, victim, TO_CHAR);
                return;
        }
        WAIT_STATE(ch, skill_table[gsn_backstab].beats);
        if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_BACKSTAB)
            && ch->power[DISC_VAMP_QUIE] < 10 && !IS_CLASS(ch, CLASS_NINJA)
            && !IS_CLASS(ch, CLASS_DROW))
                damage(ch, victim, 0, gsn_backstab);
        else if (!IS_AWAKE(victim) || IS_NPC(ch)
                 || number_percent() < ch->pcdata->learned[gsn_backstab])
                multi_hit(ch, victim, gsn_backstab);
        if ((ch->pcdata->rank == BELT_SEVEN || ch->pcdata->rank == BELT_EIGHT
             || ch->pcdata->rank == BELT_NINE || ch->pcdata->rank == BELT_TEN)
            && number_range(1, 3) == 1)
                multi_hit(ch, victim, gsn_backstab);
        if ((ch->pcdata->rank == BELT_NINE || ch->pcdata->rank == BELT_TEN)
            && number_range(1, 6) == 1)
                multi_hit(ch, victim, gsn_backstab);
        else
                damage(ch, victim, 0, gsn_backstab);
        return;
}

void do_flee(CHAR_DATA * ch, char *argument)
{
        ROOM_INDEX_DATA *was_in;
        ROOM_INDEX_DATA *now_in;
	ROOM_INDEX_DATA *to_room;
        CHAR_DATA *victim;
        char      buf[MAX_STRING_LENGTH];
        int       attempt;
        int       fleechance;

        WAIT_STATE(ch, 8);  /* to prevent spam fleeing */

        if (IS_EXTRA(ch, TIED_UP))
        {
                send_to_char("Not while tied up.\n\r", ch);
                return;
        }
        if ((victim = ch->fighting) == NULL)
        {
                if (ch->position == POS_FIGHTING)
                        ch->position = POS_STANDING;
                send_to_char("You aren't fighting anyone.\n\r", ch);
                return;
        }
        if (ch->move <= 0)
        {
                send_to_char("You can't find the energy to leave.\n\r", ch);
                return;
        }
        if (IS_AFFECTED(ch, AFF_WEBBED))
        {
                send_to_char
                        ("You are unable to move with all this sticky webbing on.\n\r",
                         ch);
                return;
        }
        if (IS_AFF2(ch, AFF2_THORNS))
        {
                send_to_char
                        ("You are rooted to the ground by all these brambles.\n\r",
                         ch);
                return;
        }
        if (!IS_NPC(victim) && victim->pcdata->powers[SKYBLADE_MAGIC] >= 9
            && IS_CLASS(victim, CLASS_SKYBLADE))
        {
                fleechance = 15;
                if (victim->pcdata->powers[SKYBLADE_MAGIC] >= 10)
                        fleechance = 30;
                if (number_percent() <= fleechance)
                {
                        act("You are frozen in fear by $N's wyrm aura.", ch,
                            NULL, victim, TO_CHAR);
                        act("$n is frozen in fear by $N's wyrm aura.", ch,
                            NULL, victim, TO_NOTVICT);
                        act("$n is frozen in fear by your wyrm aura.", ch,
                            NULL, victim, TO_VICT);
                        return;
                }
        }
        if (IS_SET(ch->flag2, AFF_TOTALBLIND))
        {
                stc("You are unable to see anything, let alone flee!\n\r",
                    victim);
                return;
        }
        if (ch->in_room)
        {
                if (has_area_affect(ch->in_room->area, AREA_AFF_THORNS, 0)
                    && number_percent() > 30)
                {
                        act("$n cannot escape with all the thorns and brambles holding $m down.", ch, NULL, NULL, TO_ROOM);
                        send_to_char
                                ("This area is shrouded in thorns and brambles, making it hard to run.\n\r",
                                 ch);
                        return;
                }
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_TANARRI))
        {
                if (ch->pcdata->powers[TANARRI_FURY_ON] == 1)
                {
                        stc("Only cowards retreat from combat.\n\r", ch);
                        return;
                }
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_GIANT))
        {
                if (ch->pcdata->powers[GIANT_FRENZY] == 1)
                {
                        stc("NO! You must stay and kill them stinky huuumans.\n\r", ch);
                        return;
                }
        }
        if (IS_CLASS(victim, CLASS_VAMPIRE)
            && IS_SET(victim->newbits, NEW_COIL) && number_percent() > 30)
        {
                xprintf(buf, "Not with %s coiled around you!\n\r",
                        victim->name);
                send_to_char(buf, ch);
                act("$n cant escape with $N coiled around them.", ch, NULL,
                    victim, TO_ROOM);
                return;
        }
        if (!IS_NPC(victim))
        {
                if (IS_CLASS(victim, CLASS_DRONE)
                    && victim->pcdata->powers[DRONE_NIGHTMARE] == 1
                    && number_percent() > 30)
                {
                        xprintf(buf,
                                "You cannot awaken from the nightmare that %s has created.\n\r",
                                victim->name);
                        send_to_char(buf, ch);
                        act("$n tries to wake from the nightmare created by $N, but fails.", ch, NULL, victim, TO_ROOM);
                        return;
                }
                if (IS_CLASS(victim, CLASS_GIANT)
                    && IS_SET(victim->pcdata->powers[GIANT_GIFTS],
                              GGIFT_LONGLEGS) && number_percent() > 30)
                {
                        xprintf(buf,
                                "%s steps down in front of you, blocking your path!\n\r",
                                victim->name);
                        send_to_char(buf, ch);
                        act("$n cant escape with $N blocking the path.", ch,
                            NULL, victim, TO_ROOM);
                        return;
                }
                if (IS_CLASS(victim, CLASS_TANARRI)
                    && IS_SET(victim->pcdata->powers[TANARRI_POWER],
                              TANARRI_TENDRILS) && number_percent() > 30)
                {
                        xprintf(buf,
                                "%s spews a shower of tendrils at you, preventing your escape!",
                                victim->name);
                        send_to_char(buf, ch);
                        act("$n tries to run, but $N spews forth a shower of tendrils, preventing $n from running.", ch, NULL, victim, TO_ROOM);
                        return;
                }
        }
        if (IS_CLASS(victim, CLASS_MAGE)
            && IS_ITEMAFF(victim, ITEMA_ILLUSIONS) && number_percent() > 30)
        {
                xprintf(buf, "Theres to many of %s to escape!\n\r",
                        victim->name);
                send_to_char(buf, ch);
                act("$n cant escape with $N blocking his way.", ch, NULL,
                    victim, TO_ROOM);
                return;
        }
        if (IS_CLASS(victim, CLASS_MONK)
            && IS_SET(victim->newbits, NEW_JAWLOCK) && number_percent() > 30)
        {
                xprintf(buf, "Not with god holding you!\n\r");
                send_to_char(buf, ch);
                act("$n cant escape god's mighty hold!", ch, NULL, victim,
                    TO_ROOM);
                return;
        }
        if (IS_CLASS(victim, CLASS_WEREWOLF)
            && IS_SET(victim->newbits, NEW_JAWLOCK) && number_percent() > 30)
        {
                xprintf(buf, "Not with %s's jaws clamped on your neck!\n\r",
                        victim->name);
                send_to_char(buf, ch);
                act("$n cant escape $N's clamped jaws!", ch, NULL, victim,
                    TO_ROOM);
                return;
        }
        if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)
            && IS_SET(victim->pcdata->powers[AURAS], BOG_AURA)
            && number_percent() > 30)
        {
                xprintf(buf, "Your stuck in the swamp surrounding %s!\n\r",
                        victim->name);
                send_to_char(buf, ch);
                act("$n is stuck in the swamp!", ch, NULL, victim, TO_ROOM);
                return;
        }
	if (IS_CLASS(victim, CLASS_SHINOBI) && (victim->pcdata->powers[S_ELEMENT] == S_FIRE) && 
	IS_SET(to_room->shinobi, ROOM_SFIRE) && number_percent( ) > 30)  
	{
		sprintf(buf,"You cannot seem to pull yourself from the burning lava!!\n\r");
		stc(buf,ch);
		act("$n cant flee from the burning lava!!",ch,NULL,victim,TO_ROOM);
		return;
	}
        was_in = ch->in_room;
        for (attempt = 0; attempt < 6; attempt++)
        {
                EXIT_DATA *pexit;
                int       door;

                door = number_door();
                if ((pexit = was_in->exit[door]) == 0
                    || pexit->to_room == NULL
                    || IS_SET(pexit->exit_info, EX_CLOSED) || (IS_NPC(ch)
                                                               &&
                                                               IS_SET(pexit->
                                                                      to_room->
                                                                      room_flags,
                                                                      ROOM_NO_MOB)))
                        continue;
                move_char(ch, door);
                if ((now_in = ch->in_room) == was_in)
                        continue;
                ch->in_room = was_in;
                act("$n has fled!", ch, NULL, NULL, TO_ROOM);
                ch->in_room = now_in;
                if (!IS_NPC(ch))
                        send_to_char("You flee from combat!  Coward!\n\r",
                                     ch);
                stop_fighting(ch, TRUE);
                return;
        }
        send_to_char("You were unable to escape!\n\r", ch);
        return;
}

void do_rescue(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        CHAR_DATA *fch;

        one_argument(argument, arg);
        if (arg[0] == '\0')
        {
                send_to_char("Rescue whom?\n\r", ch);
                return;
        }
        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }
        if (victim == ch)
        {
                send_to_char("What about fleeing instead?\n\r", ch);
                return;
        }
        if (!IS_NPC(ch) && IS_NPC(victim))
        {
                send_to_char("Doesn't need your help!\n\r", ch);
                return;
        }
        if (ch->fighting == victim)
        {
                send_to_char("Too late.\n\r", ch);
                return;
        }
        if ((fch = victim->fighting) == NULL)
        {
                send_to_char("That person is not fighting right now.\n\r",
                             ch);
                return;
        }
        if (is_safe(ch, fch) || is_safe(ch, victim))
                return;
        WAIT_STATE(ch, skill_table[gsn_rescue].beats);
        if (!IS_NPC(ch) && number_percent() > ch->pcdata->learned[gsn_rescue])
        {
                send_to_char("You fail the rescue.\n\r", ch);
                return;
        }
        act("You rescue $N!", ch, NULL, victim, TO_CHAR);
        act("$n rescues you!", ch, NULL, victim, TO_VICT);
        act("$n rescues $N!", ch, NULL, victim, TO_NOTVICT);
        stop_fighting(fch, FALSE);
        stop_fighting(victim, FALSE);
        set_fighting(ch, fch, TRUE);
        set_fighting(fch, ch, FALSE);
        do_humanity(ch, "");
        return;
}

void do_kick(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        OBJ_DATA *boots;
        int       dam;
        int       stance;
        int       hitpoints;

        if (!IS_NPC(ch) && ch->level < skill_table[gsn_kick].skill_level)
        {
                send_to_char("First you should learn to kick.\n\r", ch);
                return;
        }
        if ((victim = ch->fighting) == NULL)
        {
                send_to_char("You aren't fighting anyone.\n\r", ch);
                return;
        }
        WAIT_STATE(ch, skill_table[gsn_kick].beats);
        if ((IS_NPC(ch) && ch->level < 1800))
        {
                dam = 500;
                damage(ch, victim, dam, gsn_kick);
                return;
        }
        if ((IS_NPC(ch)) || number_percent() < ch->pcdata->learned[gsn_kick])
                dam = number_range(1, 4);
        else
        {
                dam = 0;
                damage(ch, victim, dam, gsn_kick);
                return;
        }
        dam += char_damroll(ch);
        if (dam == 0)
                dam = 1;
        if (!IS_AWAKE(victim))
                dam *= 2;
        if (IS_NPC(ch))
                dam *= 100;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF))
        {
                if (IS_SET(victim->special, SPC_WOLFMAN))
                        dam *= 0.8;
                if (victim->power[DISC_WERE_BOAR] > 2)
                        dam *= 0.5;
                if ((boots = get_eq_char(ch, WEAR_FEET)) != NULL
                    && IS_SET(boots->spectype, SITEM_SILVER))
                        dam *= 2;
        }
        if (!IS_NPC(ch))
                dam = dam + (dam * ((ch->wpn[0] + 1) / 100));
        if (!IS_NPC(ch))
        {
                stance = ch->stance[0];
                if (IS_STANCE(ch, STANCE_NORMAL))
                        dam *= 1.25;
                else
                        dam = dambonus(ch, victim, dam, stance);
        }
        if (!IS_NPC(ch) && !IS_NPC(victim) && dam > 750)
                dam = 750;
        if (dam <= 0)
                dam = 2;
        dam = cap_dam(ch, victim, dam);
        hitpoints = victim->hit;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_PRIEST))
        {
                dam *= 2.0;
        }
        if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_KICK) && !IS_NPC(ch)
            && !(IS_CLASS(ch, CLASS_DEMON) && IS_DEMAFF(ch, DEM_HOOVES))
            && !(IS_CLASS(ch, CLASS_SHAPESHIFTER)
                 && ch->pcdata->powers[SHAPE_FORM] == BULL_FORM))
        {
                if (IS_CLASS(ch, CLASS_DEMON) && IS_DEMAFF(ch, DEM_HOOVES))
                        dam = dam / 2;
                if (IS_NPC(ch))
                        dam = dam * 10;
                victim->hit += dam;
                damage(ch, victim, dam, gsn_kick);
                victim->hit = hitpoints;
        }
        else if IS_IMMUNE
                (victim, IMM_KICK)
        {
                dam = dam / 2;
                damage(ch, victim, dam, gsn_kick);
        }
        else
                damage(ch, victim, dam, gsn_kick);
        return;
}


int dambonus(CHAR_DATA * ch, CHAR_DATA * victim, int dam, int stance)
{
        if (dam < 1)
                return 0;
        if (stance < 1)
                return dam;
        if (!IS_NPC(ch) && !can_counter(victim))
        {
                if (IS_STANCE(ch, STANCE_WOLVERINE))
                {
                        int       mindam = dam * 0.25;

                        dam *= (ch->stance[STANCE_WOLVERINE] + 1) / 200;
                        if (dam < mindam)
                                dam = mindam;
                }
                else if (IS_STANCE(ch, STANCE_PITBULL)
                         && ch->stance[STANCE_PITBULL] > 100)
                        dam += dam * (ch->stance[STANCE_PITBULL] / 100);
                else if (IS_STANCE(ch, STANCE_BADGER)
                         && ch->stance[STANCE_BADGER] > 100)
                        dam += dam * (ch->stance[STANCE_BADGER] / 100);
                else if (IS_STANCE(ch, STANCE_WOLF)
                         && ch->stance[STANCE_WOLF] > 100)
                        dam += dam * (ch->stance[STANCE_WOLF] / 100);
                else if (IS_STANCE(ch, STANCE_COUGAR)
                         && ch->stance[STANCE_COUGAR] > 100)
                        dam += dam * (ch->stance[STANCE_COUGAR] / 100);
/* ------------ This is the part for superstances, Jobo ------------------- */
                else if (ch->stance[0] > 12
                         && IS_SET(ch->stance[(ch->stance[0] + 6)],
                                   STANCEPOWER_DAMAGE_3)
                         && ch->stance[(ch->stance[0])] > 100)
                        dam += dam * 3 * (ch->stance[(ch->stance[0])] / 100);
                else if (ch->stance[0] > 12
                         && IS_SET(ch->stance[(ch->stance[0] + 6)],
                                   STANCEPOWER_DAMAGE_2)
                         && ch->stance[(ch->stance[0])] > 100)
                        dam += dam * 2 * (ch->stance[(ch->stance[0])] / 100);
                else if (ch->stance[0] > 12
                         && IS_SET(ch->stance[(ch->stance[0] + 6)],
                                   STANCEPOWER_DAMAGE_1)
                         && ch->stance[(ch->stance[0])] > 100)
                        dam += dam * (ch->stance[(ch->stance[0])] / 100);
/* ------------ This is the end for superstances, Jobo ------------------- */

                else if (ch->stance[0] > 0 && ch->stance[stance] < 100)
                        dam *= 0.5;
        }
        if (!IS_NPC(victim) && !can_counter(ch))
        {
                if (IS_STANCE(victim, STANCE_CRAB)
                    && victim->stance[STANCE_CRAB] > 100)
                        dam /= victim->stance[STANCE_CRAB] / 100;
                else if (IS_STANCE(victim, STANCE_BADGER)
                         && victim->stance[STANCE_BADGER] > 100)
                        dam /= victim->stance[STANCE_BADGER] / 100;
                else if (IS_STANCE(victim, STANCE_BADGER)
                         && victim->stance[STANCE_BADGER] > 100)
                        dam /= victim->stance[STANCE_BADGER] / 100;
                else if (IS_STANCE(victim, STANCE_GRIZZLY)
                         && victim->stance[STANCE_GRIZZLY] > 100)
                        dam /= victim->stance[STANCE_GRIZZLY] / 100;
/* ------------ This is the part for superstances, Jobo ------------------- */
                else if (victim->stance[0] > 12 &&
                         IS_SET(victim->stance[(victim->stance[0] + 6)],
                                STANCEPOWER_RESIST_3)
                         && victim->stance[(victim->stance[0])] > 100)
                        dam /= 3 * (victim->stance[(victim->stance[0])] /
                                    100);
                else if (victim->stance[0] > 12
                         && IS_SET(victim->stance[(victim->stance[0] + 6)],
                                   STANCEPOWER_RESIST_2)
                         && victim->stance[(victim->stance[0])] > 100)
                        dam /= 2 * (victim->stance[(victim->stance[0])] /
                                    100);
                else if (victim->stance[0] > 12
                         && IS_SET(victim->stance[(victim->stance[0] + 6)],
                                   STANCEPOWER_RESIST_1)
                         && victim->stance[(victim->stance[0])] > 100)
                        dam /= (victim->stance[(victim->stance[0])] / 100);
/* ------------ This is the end for superstances, Jobo ------------------- */
        }
        return dam;
}

void do_punch(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        int       dam;
        int       store;
        bool      broke = FALSE;

        one_argument(argument, arg);
        if (IS_NPC(ch))
                return;
        if (ch->level < skill_table[gsn_punch].skill_level)
        {
                send_to_char("First you should learn to punch.\n\r", ch);
                return;
        }
        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("They are not here.\n\r", ch);
                return;
        }
        if (ch == victim)
        {
                send_to_char("You cannot punch yourself!\n\r", ch);
                return;
        }
        if (is_safe(ch, victim))
                return;
        if (victim->hit < victim->max_hit)
        {
                send_to_char("They are hurt and suspicious.\n\r", ch);
                return;
        }
        if (victim->position < POS_FIGHTING)
        {
                send_to_char
                        ("You can only punch someone who is standing.\n\r",
                         ch);
                return;
        }
        act("You draw your fist back and aim a punch at $N.", ch, NULL,
            victim, TO_CHAR);
        act("$n draws $s fist back and aims a punch at you.", ch, NULL,
            victim, TO_VICT);
        act("$n draws $s fist back and aims a punch at $N.", ch, NULL, victim,
            TO_NOTVICT);
        WAIT_STATE(ch, skill_table[gsn_punch].beats);
        if (IS_NPC(ch) || number_percent() < ch->pcdata->learned[gsn_punch])
                dam = number_range(1, 4);
        else
        {
                dam = 0;
                damage(ch, victim, dam, gsn_punch);
                return;
        }
        dam += char_damroll(ch);
        if (dam == 0)
                dam = 1;
        if (!IS_AWAKE(victim))
                dam *= 2;
        if (!IS_NPC(ch))
                dam = dam + (dam * (ch->wpn[0] / 100));
        if (dam <= 0)
                dam = 1;
        if (dam > 1000)
                dam = 1000;
        if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF)
            && victim->power[DISC_WERE_BOAR] > 3)
        {
                store = victim->hit;
                victim->hit += dam;
                damage(ch, victim, dam, gsn_punch);
                victim->hit = store;
                if (number_percent() <= 25 && !IS_ARM_L(ch, LOST_ARM)
                    && !IS_ARM_L(ch, LOST_HAND))
                {
                        if (!IS_ARM_L(ch, LOST_FINGER_I)
                            && !IS_ARM_L(ch, BROKEN_FINGER_I))
                        {
                                SET_BIT(ch->loc_hp[LOC_ARM_L],
                                        BROKEN_FINGER_I);
                                broke = TRUE;
                        }
                        if (!IS_ARM_L(ch, LOST_FINGER_M)
                            && !IS_ARM_L(ch, BROKEN_FINGER_M))
                        {
                                SET_BIT(ch->loc_hp[LOC_ARM_L],
                                        BROKEN_FINGER_M);
                                broke = TRUE;
                        }
                        if (!IS_ARM_L(ch, LOST_FINGER_R)
                            && !IS_ARM_L(ch, BROKEN_FINGER_R))
                        {
                                SET_BIT(ch->loc_hp[LOC_ARM_L],
                                        BROKEN_FINGER_R);
                                broke = TRUE;
                        }
                        if (!IS_ARM_L(ch, LOST_FINGER_L)
                            && !IS_ARM_L(ch, BROKEN_FINGER_L))
                        {
                                SET_BIT(ch->loc_hp[LOC_ARM_L],
                                        BROKEN_FINGER_L);
                                broke = TRUE;
                        }
                        if (broke)
                        {
                                act("The fingers on your left hand shatter under the impact of the blow!", ch, NULL, NULL, TO_CHAR);
                                act("The fingers on $n's left hand shatter under the impact of the blow! ", ch, NULL, NULL, TO_ROOM);
                        }
                }
                else if (number_percent() <= 25 && !IS_ARM_R(ch, LOST_ARM)
                         && !IS_ARM_R(ch, LOST_HAND))
                {
                        if (!IS_ARM_R(ch, LOST_FINGER_I)
                            && !IS_ARM_R(ch, BROKEN_FINGER_I))
                        {
                                SET_BIT(ch->loc_hp[LOC_ARM_R],
                                        BROKEN_FINGER_I);
                                broke = TRUE;
                        }
                        if (!IS_ARM_R(ch, LOST_FINGER_M)
                            && !IS_ARM_R(ch, BROKEN_FINGER_M))
                        {
                                SET_BIT(ch->loc_hp[LOC_ARM_R],
                                        BROKEN_FINGER_M);
                                broke = TRUE;
                        }
                        if (!IS_ARM_R(ch, LOST_FINGER_R)
                            && !IS_ARM_R(ch, BROKEN_FINGER_R))
                        {
                                SET_BIT(ch->loc_hp[LOC_ARM_R],
                                        BROKEN_FINGER_R);
                                broke = TRUE;
                        }
                        if (!IS_ARM_R(ch, LOST_FINGER_L)
                            && !IS_ARM_R(ch, BROKEN_FINGER_L))
                        {
                                SET_BIT(ch->loc_hp[LOC_ARM_R],
                                        BROKEN_FINGER_L);
                                broke = TRUE;
                        }
                        if (broke)
                        {
                                act("The fingers on your right hand shatter under the impact of the blow!", ch, NULL, NULL, TO_CHAR);
                                act("The fingers on $n's right hand shatter under the impact of the blow! ", ch, NULL, NULL, TO_ROOM);
                        }
                }
                stop_fighting(victim, TRUE);
                return;
        }
        damage(ch, victim, dam, gsn_punch);
        if (victim == NULL || victim->position == POS_DEAD || dam < 1)
                return;
        if (victim->position == POS_FIGHTING)
                stop_fighting(victim, TRUE);
        if (number_percent() <= 25 && !IS_HEAD(victim, BROKEN_NOSE)
            && !IS_HEAD(victim, LOST_NOSE))
        {
                act("Your nose shatters under the impact of the blow!",
                    victim, NULL, NULL, TO_CHAR);
                act("$n's nose shatters under the impact of the blow!",
                    victim, NULL, NULL, TO_ROOM);
                SET_BIT(victim->loc_hp[LOC_HEAD], BROKEN_NOSE);
        }
        else if (number_percent() <= 25 && !IS_HEAD(victim, BROKEN_JAW))
        {
                act("Your jaw shatters under the impact of the blow!", victim,
                    NULL, NULL, TO_CHAR);
                act("$n's jaw shatters under the impact of the blow!", victim,
                    NULL, NULL, TO_ROOM);
                SET_BIT(victim->loc_hp[LOC_HEAD], BROKEN_JAW);
        }
        act("You fall to the ground stunned!", victim, NULL, NULL, TO_CHAR);
        act("$n falls to the ground stunned!", victim, NULL, NULL, TO_ROOM);
        victim->position = POS_STUNNED;
        if (dam > 1000)
                dam = 1000;
        return;
}
void do_berserk(CHAR_DATA *ch, char *argument)
{
  char arg [MAX_INPUT_LENGTH];
  char buf [MSL];
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  CHAR_DATA *mount;
  int number_hit = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if ( ch->level < skill_table[gsn_berserk].skill_level)
  {
    send_to_char( "You are not wild enough to go berserk.\n\r", ch );
    return;
  }

  WAIT_STATE( ch, 3);
  if ( number_percent( ) > ch->pcdata->learned[gsn_berserk] )
  {
    act("You rant and rave, but nothing much happens.",ch,NULL,NULL,TO_CHAR);
    act("$n gets a wild look in $s eyes, but nothing much happens.",ch,NULL,NULL,TO_ROOM);
    return;
  }
  act("You go #7B#wE#7R#wS#7E#wR#7K#n!",ch,NULL,NULL,TO_CHAR);
  act("$n goes #7B#wE#7R#wS#7E#wR#7K#n!",ch,NULL,NULL,TO_ROOM);

        if ( ch->pcdata->learned[gsn_berserk] < 100 )
        {
            sprintf( buf, "Your skill with Berserk increases.\n\r" );
            send_to_char( buf, ch );
        ch->pcdata->learned[gsn_berserk]++;
        }


  for ( vch = char_list; vch != NULL; vch = vch_next )
  {
    vch_next  = vch->next;
    if (number_hit > 4) continue;
    if (vch->in_room == NULL) continue;
    if (!IS_NPC(vch)) continue;
    if ( ch == vch ) continue;
    if ( vch->in_room == ch->in_room )
    {
      if ( ( mount = ch->mount ) != NULL )
        if ( mount == vch ) continue;
      if (can_see(ch,vch))
      {
        multi_hit( ch, vch, TYPE_UNDEFINED );
        number_hit++;
      }
    }  
  }    
  return;
}

void do_berserk2( CHAR_DATA *ch, char *argument )
{
  char arg [MAX_INPUT_LENGTH];
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  CHAR_DATA *mount;
  int number_hit = 0;

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if ( ch->level < skill_table[gsn_berserk].skill_level)
  {
    send_to_char( "You are not wild enough to go berserk.\n\r", ch );
    return;
  }
  WAIT_STATE( ch, skill_table[gsn_berserk].beats );

  if ( number_percent( ) > ch->pcdata->learned[gsn_berserk] )
  {
    act("You rant and rave, but nothing much happens.",ch,NULL,NULL,TO_CHAR);
    act("$n gets a wild look in $s eyes, but nothing much happens.",ch,NULL,NULL,TO_ROOM);
    return;
  }
  act("You go #7B#wE#7R#wS#7E#wR#7K#n!",ch,NULL,NULL,TO_CHAR);
  act("$n goes #7B#wE#7R#wS#7E#wR#7K#n!",ch,NULL,NULL,TO_ROOM);
  for ( vch = char_list; vch != NULL; vch = vch_next )
  {
    vch_next  = vch->next;
    if (number_hit > 4) continue;
    if (vch->in_room == NULL) continue;
    if (!IS_NPC(vch) && vch->pcdata->chobj != NULL) continue;
    if ( ch == vch ) continue;
    if ( vch->in_room == ch->in_room ) 
    {
      if ( ( mount = ch->mount ) != NULL ) 
        if ( mount == vch ) continue;
      if (can_see(ch,vch))
      {
        multi_hit( ch, vch, TYPE_UNDEFINED );
        number_hit++;
      }
    }
  }
  do_beastlike(ch,"");
  return;
}

/*
void do_berserk(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        CHAR_DATA *mount;
        int       number_hit = 0;

        argument = one_argument(argument, arg);
        if (IS_NPC(ch))
                return;
        if (ch->level < skill_table[gsn_berserk].skill_level)
        {
                send_to_char("You are not wild enough to go berserk.\n\r",
                             ch);
                return;
        }
	if ((get_age(ch) - 17) < 2)
	{
		 WAIT_STATE(ch, 2);
	}
	else
	{
		WAIT_STATE(ch, 2);
	}
        if (number_percent() > ch->pcdata->learned[gsn_berserk])
        {
                act("You rant and rave, but nothing much happens.", ch, NULL,
                    NULL, TO_CHAR);
                act("$n gets a wild look in $s eyes, but nothing much happens.", ch, NULL, NULL, TO_ROOM);
                return;
        }
        act("You go BERSERK!", ch, NULL, NULL, TO_CHAR);
        act("$n goes BERSERK!", ch, NULL, NULL, TO_ROOM);
        for (vch = char_list; vch != NULL; vch = vch_next)
        {
                vch_next = vch->next;
                if (number_hit > 4)
                        continue;
                if (vch->in_room == NULL)
                        continue;
                if (!IS_NPC(vch))
                        continue;
                if (ch == vch)
                        continue;
                if (vch->in_room == ch->in_room)
                {
                        if ((mount = ch->mount) != NULL)
                                if (mount == vch)
                                        continue;
                        if (can_see(ch, vch))
                        {
                                multi_hit(ch, vch, TYPE_UNDEFINED);
                                number_hit++;
                        }
                }
        }
        return;
}

void do_berserk2(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        CHAR_DATA *mount;
        int       number_hit = 0;

        argument = one_argument(argument, arg);
        if (IS_NPC(ch))
                return;
        if (ch->level < skill_table[gsn_berserk].skill_level)
        {
                send_to_char("You are not wild enough to go berserk.\n\r",
                             ch);
                return;
        }
        WAIT_STATE(ch, skill_table[gsn_berserk].beats);
        if (number_percent() > ch->pcdata->learned[gsn_berserk])
        {
                act("You rant and rave, but nothing much happens.", ch, NULL,
                    NULL, TO_CHAR);
                act("$n gets a wild look in $s eyes, but nothing much happens.", ch, NULL, NULL, TO_ROOM);
                return;
        }
        act("You go BERSERK!", ch, NULL, NULL, TO_CHAR);
        act("$n goes BERSERK!", ch, NULL, NULL, TO_ROOM);
        for (vch = char_list; vch != NULL; vch = vch_next)
        {
                vch_next = vch->next;
                if (number_hit > 4)
                        continue;
                if (vch->in_room == NULL)
                        continue;
                if (!IS_NPC(vch) && vch->pcdata->chobj != NULL)
                        continue;
                if (ch == vch)
                        continue;
                if (vch->in_room == ch->in_room)
                {
                        if ((mount = ch->mount) != NULL)
                                if (mount == vch)
                                        continue;
                        if (can_see(ch, vch))
                        {
                                multi_hit(ch, vch, TYPE_UNDEFINED);
                                number_hit++;
                        }
                }
        }
        do_beastlike(ch, "");
        return;
}
*/
/* Hurl skill by KaVir */
void do_hurl(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        CHAR_DATA *mount;
        ROOM_INDEX_DATA *to_room;
        EXIT_DATA *pexit;
        EXIT_DATA *pexit_rev;
        char      buf[MAX_INPUT_LENGTH];
        char      direction[MAX_INPUT_LENGTH];
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        int       door;
        int       rev_dir;
        int       dam;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        if (!IS_NPC(ch) && ch->pcdata->learned[gsn_hurl] < 1)
        {
                send_to_char("Maybe you should learn the skill first?\n\r",
                             ch);
                return;
        }
        if (arg1[0] == '\0')
        {
                send_to_char("Who do you wish to hurl?\n\r", ch);
                return;
        }
        if ((victim = get_char_room(ch, NULL, arg1)) == NULL)
        {
                send_to_char("They are not here.\n\r", ch);
                return;
        }
        if (victim == ch)
        {
                send_to_char("How can you hurl yourself?\n\r", ch);
                return;
        }
        if (!IS_NPC(victim) && is_safe(ch, victim))
                return;
        if ((mount = victim->mount) != NULL && victim->mounted == IS_MOUNT)
        {
                send_to_char("But they have someone on their back!\n\r", ch);
                return;
        }
        else if ((mount = victim->mount) != NULL
                 && victim->mounted == IS_RIDING)
        {
                send_to_char("But they are riding!\n\r", ch);
                return;
        }
        if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_HURL))
        {
                send_to_char
                        ("You are unable to get their feet of the ground.\n\r",
                         ch);
                return;
        }
        if (IS_NPC(victim) && victim->level > 900)
        {
                send_to_char
                        ("You are unable to get their feet of the ground.\n\r",
                         ch);
                return;
        }
        if ((victim->hit < victim->max_hit)
            || (victim->position == POS_FIGHTING && victim->fighting != ch))
        {
                act("$N is hurt and suspicious, and you are unable to approach $M.", ch, NULL, victim, TO_CHAR);
                return;
        }
        WAIT_STATE(ch, skill_table[gsn_hurl].beats);
        if (!IS_NPC(ch) && number_percent() > ch->pcdata->learned[gsn_hurl])
        {
                send_to_char
                        ("You are unable to get their feet of the ground.\n\r",
                         ch);
                multi_hit(victim, ch, TYPE_UNDEFINED);
                return;
        }
        rev_dir = 0;
        if (arg2[0] == '\0')
                door = number_range(0, 3);
        else
        {
                if (!str_cmp(arg2, "n") || !str_cmp(arg2, "north"))
                        door = 0;
                else if (!str_cmp(arg2, "e") || !str_cmp(arg2, "east"))
                        door = 1;
                else if (!str_cmp(arg2, "s") || !str_cmp(arg2, "south"))
                        door = 2;
                else if (!str_cmp(arg2, "w") || !str_cmp(arg2, "west"))
                        door = 3;
                else
                {
                        send_to_char
                                ("You can only hurl people north, south, east or west.\n\r",
                                 ch);
                        return;
                }
        }
        if (door == 0)
        {
                xprintf(direction, "north");
                rev_dir = 2;
        }
        if (door == 1)
        {
                xprintf(direction, "east");
                rev_dir = 3;
        }
        if (door == 2)
        {
                xprintf(direction, "south");
                rev_dir = 0;
        }
        if (door == 3)
        {
                xprintf(direction, "west");
                rev_dir = 1;
        }
        if ((pexit = ch->in_room->exit[door]) == NULL
            || (to_room = pexit->to_room) == NULL)
        {
                xprintf(buf, "$n hurls $N into the %s wall.", direction);
                act(buf, ch, NULL, victim, TO_NOTVICT);
                xprintf(buf, "You hurl $N into the %s wall.", direction);
                act(buf, ch, NULL, victim, TO_CHAR);
                xprintf(buf, "$n hurls you into the %s wall.", direction);
                act(buf, ch, NULL, victim, TO_VICT);
                dam = number_range(ch->level, (ch->level * 4));
                victim->hit = victim->hit - dam;
                update_pos(victim);
                if (IS_NPC(victim) && !IS_NPC(ch))
                        ch->mkill = ch->mkill + 1;
                if (!IS_NPC(victim) && IS_NPC(ch))
                        victim->mdeath = victim->mdeath + 1;
                if (victim->position == POS_DEAD)
                {
                        raw_kill(victim, ch);
                        return;
                }
                return;
        }
        pexit = victim->in_room->exit[door];
        if (IS_SET(pexit->exit_info, EX_PRISMATIC_WALL))
        {
                xprintf(buf, "$n hurls $N into the %s wall.", direction);
                act(buf, ch, NULL, victim, TO_NOTVICT);
                xprintf(buf, "You hurl $N into the %s wall.", direction);
                act(buf, ch, NULL, victim, TO_CHAR);
                xprintf(buf, "$n hurls you into the %s wall.", direction);
                act(buf, ch, NULL, victim, TO_VICT);
                dam = number_range(ch->level, (ch->level * 4));
                victim->hit = victim->hit - dam;
                update_pos(victim);
                if (IS_NPC(victim) && !IS_NPC(ch))
                        ch->mkill = ch->mkill + 1;
                if (!IS_NPC(victim) && IS_NPC(ch))
                        victim->mdeath = victim->mdeath + 1;
                if (victim->position == POS_DEAD)
                {
                        raw_kill(victim, ch);
                        return;
                }
                return;
        }
        if (IS_SET(pexit->exit_info, EX_CLOSED)
            && !IS_AFFECTED(victim, AFF_PASS_DOOR)
            && !IS_AFFECTED(victim, AFF_ETHEREAL))
        {
                if (IS_SET(pexit->exit_info, EX_LOCKED))
                        REMOVE_BIT(pexit->exit_info, EX_LOCKED);
                if (IS_SET(pexit->exit_info, EX_CLOSED))
                        REMOVE_BIT(pexit->exit_info, EX_CLOSED);
                xprintf(buf, "$n hoists $N in the air and hurls $M %s.",
                        direction);
                act(buf, ch, NULL, victim, TO_NOTVICT);
                xprintf(buf, "You hoist $N in the air and hurl $M %s.",
                        direction);
                act(buf, ch, NULL, victim, TO_CHAR);
                xprintf(buf, "$n hurls you %s, smashing you through the %s.",
                        direction, pexit->keyword);
                act(buf, ch, NULL, victim, TO_VICT);
                xprintf(buf,
                        "There is a loud crash as $n smashes through the $d.");
                act(buf, victim, NULL, pexit->keyword, TO_ROOM);
                if ((to_room = pexit->to_room) != NULL
                    && (pexit_rev = to_room->exit[rev_dir]) != NULL
                    && pexit_rev->to_room == ch->in_room
                    && pexit_rev->keyword != NULL)
                {
                        if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
                                REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);
                        if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
                                REMOVE_BIT(pexit_rev->exit_info, EX_CLOSED);
                        if (door == 0)
                                xprintf(direction, "south");
                        if (door == 1)
                                xprintf(direction, "west");
                        if (door == 2)
                                xprintf(direction, "north");
                        if (door == 3)
                                xprintf(direction, "east");
                        char_from_room(victim);
                        char_to_room(victim, to_room);
                        xprintf(buf,
                                "$n comes smashing in through the %s $d.",
                                direction);
                        act(buf, victim, NULL, pexit->keyword, TO_ROOM);
                        dam = number_range(ch->level, (ch->level * 6));
                        victim->hit = victim->hit - dam;
                        update_pos(victim);
                        if (IS_NPC(victim) && !IS_NPC(ch))
                                ch->mkill = ch->mkill + 1;
                        if (!IS_NPC(victim) && IS_NPC(ch))
                                victim->mdeath = victim->mdeath + 1;
                        if (victim->position == POS_DEAD)
                        {
                                raw_kill(victim, ch);
                                return;
                        }
                }
        }
        else
        {
                xprintf(buf, "$n hurls $N %s.", direction);
                act(buf, ch, NULL, victim, TO_NOTVICT);
                xprintf(buf, "You hurl $N %s.", direction);
                act(buf, ch, NULL, victim, TO_CHAR);
                xprintf(buf, "$n hurls you %s.", direction);
                act(buf, ch, NULL, victim, TO_VICT);
                if (door == 0)
                        xprintf(direction, "south");
                if (door == 1)
                        xprintf(direction, "west");
                if (door == 2)
                        xprintf(direction, "north");
                if (door == 3)
                        xprintf(direction, "east");
                char_from_room(victim);
                char_to_room(victim, to_room);
                xprintf(buf, "$n comes flying in from the %s.", direction);
                act(buf, victim, NULL, NULL, TO_ROOM);
                dam = number_range(ch->level, (ch->level * 2));
                victim->hit = victim->hit - dam;
                update_pos(victim);
                if (IS_NPC(victim) && !IS_NPC(ch))
                        ch->mkill = ch->mkill + 1;
                if (!IS_NPC(victim) && IS_NPC(ch))
                        victim->mdeath = victim->mdeath + 1;
                if (victim->position == POS_DEAD)
                {
                        raw_kill(victim, ch);
                        return;
                }
        }
        return;
}

void do_disarm(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        OBJ_DATA *obj;
        int       percent;

        if (!IS_NPC(ch) && ch->level < skill_table[gsn_disarm].skill_level)
        {
                send_to_char("You don't know how to disarm opponents.\n\r",
                             ch);
                return;
        }

        if ((get_eq_char(ch, WEAR_WIELD) == NULL)
            && (get_eq_char(ch, WEAR_HOLD) == NULL))
        {
                send_to_char("You must wield a weapon to disarm.\n\r", ch);
                return;
        }

        if ((victim = ch->fighting) == NULL)
        {
                send_to_char("You aren't fighting anyone.\n\r", ch);
                return;
        }

        if (((obj = get_eq_char(victim, WEAR_WIELD)) == NULL)
            && ((obj = get_eq_char(victim, WEAR_HOLD)) == NULL))
        {
                send_to_char("Your opponent is not wielding a weapon.\n\r",
                             ch);
                return;
        }

        WAIT_STATE(ch, skill_table[gsn_disarm].beats);
        percent = number_percent() + victim->level - ch->level;
        if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_DISARM))
                send_to_char("You failed.\n\r", ch);
        else if (IS_NPC(ch)
                 || percent < ch->pcdata->learned[gsn_disarm] * 2 / 3)
                disarm(ch, victim);
        else
                send_to_char("You failed.\n\r", ch);
        return;
}



void do_sla(CHAR_DATA * ch, char *argument)
{
        send_to_char("If you want to SLAY, spell it out.\n\r", ch);
        return;
}

/* Had problems with people not dying when POS_DEAD...KaVir */
void do_killperson(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        char      arg[MAX_INPUT_LENGTH];

        one_argument(argument, arg);
        if (arg[0] == '\0')
                return;
        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
                return;
        send_to_char("#R#fYou have been KILLED!!#n\n\r\n\r", victim);
        if (IS_NPC(victim) && !IS_NPC(ch))
                ch->mkill = ch->mkill + 1;
        if (!IS_NPC(victim) && IS_NPC(ch))
                victim->mdeath = victim->mdeath + 1;
        raw_kill(victim, ch);
        return;
}

/* For tier kills - Hutoshi */
void do_tiercap( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  bool deathmatch = FALSE;
  bool paradox_punish = FALSE;

  if (IS_NPC(ch)) return;
  one_argument( argument, arg );

  if (ch->in_room != NULL)
  {
    if (IS_SET (ch->in_room->room_flags, ROOM_ARENA))
    {
      send_to_char("Your in the arena.\n\r",ch);
      return;
    }
  }
  if ( arg[0] == '\0' )
  {
    send_to_char( "Decapitate whom?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }
  if ( ch == victim )
  {
    send_to_char( "That might be a bit tricky...\n\r", ch );
    return;
  }
  if (IS_NPC(victim))
  {
    send_to_char( "You can only tiercap other players.\n\r", ch );
    return;
  }
  if ( victim->position > 1 )
  {
    send_to_char( "You can only do this to mortally wounded players.\n\r", ch );
    return;
  }
  if((get_age(ch)-17)<2)
  {
    send_to_char("You'll have to wait till you have 4 hours.\n\r",ch);
    return;
  }
  if((get_age(victim)-17)<2)
  {
    send_to_char("They are a newbie, don't hurt them.\n\r",ch);
    return;
  }
  if (is_safe(ch, victim))
    return;
  if (ragnarok)
  {
    ragnarokdecap(ch,victim);
    return;
  }
/*
  if (!str_cmp(ch->pcdata->last_decap[0], victim->name)
      && str_cmp(ch->pcdata->retaliation, victim->name))
  {
        send_to_char("Don't spamcap, it's not nice, find someone else to pick on.\n\r", ch);
        return;
  }
*/
  if ((victim->race == 0 || ch->race == 0) && victim->tier == 0) paradox_punish = TRUE;
  else if ( (victim->tier) < ch->tier) paradox_punish = TRUE;
  if (ch->level > 6 && paradox_punish == TRUE) paradox_punish = FALSE;
  if (paradox_punish)
  {
    if (!IS_ITEMAFF(victim, ITEMA_ARTIFACT)) do_paradox(ch,"self");
      if (IS_CLASS(victim, CLASS_VAMPIRE))           do_mortalvamp(victim,"");
      else if (IS_SET(victim->special, SPC_WOLFMAN)) do_unwerewolf(victim,"");
    if (IS_CLASS(victim, CLASS_MAGE) && IS_AFFECTED(victim, AFF_POLYMORPH)) do_unpolymorph(victim,"");
    ch->fight_timer += 10;
    if (IS_CLASS(victim,CLASS_DRACONIAN)) draconian_death(ch,victim);
      behead( victim );
    do_beastlike(ch,"");
      victim->level = 2;
      return;
  }

  /*
   * Update the last decaps to prevent spamcapping.
   */
  free_string( ch->pcdata->last_decap[1] );
  ch->pcdata->last_decap[1] = str_dup(ch->pcdata->last_decap[0]);
  free_string( ch->pcdata->last_decap[0] );
  ch->pcdata->last_decap[0] = str_dup(victim->name);
  /*
   * Retaliation update
   */
  free_string(victim->pcdata->retaliation);
  victim->pcdata->retaliation = str_dup(ch->name);
  if (!str_cmp(ch->pcdata->retaliation, victim->name))
  {
    free_string(ch->pcdata->retaliation);
    ch->pcdata->retaliation = str_dup("Noone");
  }
  ch->tks = ch->tks + 1;
  if (victim->tks > 0)  victim->tks = victim->tks - 1;
  act( "A misty white vapour pours from $N's corpse into your body.",  ch, NULL, victim, TO_CHAR    );
  act( "A misty white vapour pours from $N's corpse into $n's body.",  ch, NULL, victim, TO_NOTVICT );
  act( "You double over in agony as raw energy pulses through your veins.",  ch, NULL, NULL, TO_CHAR    );
  act( "$n doubles over in agony as sparks of energy crackle around $m.",  ch, NULL, NULL, TO_NOTVICT );
  if (IS_CLASS(victim, CLASS_VAMPIRE))      do_mortalvamp(victim,"");
  else if (IS_SET(victim->special, SPC_WOLFMAN)) do_unwerewolf(victim,"");
  if (!deathmatch) ch->fight_timer += 10;
  if (IS_CLASS(ch, CLASS_DEMON))
  {
    send_to_char("You rip away a part of their soul.\n\r", ch);
    send_to_char("You feel a part of your soul has been torn away.\n\r", victim);
    ch->pcdata->souls += 1;
  }
  if (IS_CLASS(victim,CLASS_DRACONIAN))
  draconian_death(ch,victim);
  behead( victim );
  dropinvis(ch);
  /*  Removes mage powers */
  if (IS_ITEMAFF(victim, ITEMA_STEELSHIELD)) REMOVE_BIT(ch->itemaffect, ITEMA_STEELSHIELD);
  if (IS_ITEMAFF(victim, ITEMA_BEAST)) REMOVE_BIT(victim->itemaffect, ITEMA_BEAST);
  if (IS_ITEMAFF(victim, ITEMA_MAGESHIELD)) REMOVE_BIT(victim->itemaffect, ITEMA_MAGESHIELD);
  if (IS_ITEMAFF(victim, ITEMA_ILLUSIONS)) REMOVE_BIT(victim->itemaffect, ITEMA_ILLUSIONS);
  if (IS_ITEMAFF(victim, ITEMA_DEFLECTOR)) REMOVE_BIT(victim->itemaffect, ITEMA_DEFLECTOR);
  
  do_beastlike(ch,"");
  victim->rage = 0;
  victim->level = 2;
  sprintf( buf, "#C%s #Ghas been #RTIERCAPPED #Gby #C%s #Gat #C%d.#n",
  victim->pcdata->switchname, ch->pcdata->switchname, victim->in_room->vnum );
  death_info(buf);
  xprintf(buf, "%s tiercapped by %s at %d.",
                victim->pcdata->switchname, ch->pcdata->switchname,
                victim->in_room->vnum);
  wiznet(buf, ch, NULL, WIZ_DEATHS, 0, 0);
  log_string(LOG_GAME, buf);

  if (!IS_NPC(victim) && IS_SET(ch->act2, PLR_AUTOHEAD))
  do_sacrifice(victim, "head");
  return;
}

/* For decapitating players - KaVir */
void do_decapitate(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        ROOM_INDEX_DATA *location;
        char      arg[MAX_INPUT_LENGTH];
        char      buf[MAX_STRING_LENGTH];
        int       faith;
        bool      deathmatch = FALSE;
        bool      paradox_punish = FALSE;

        if (IS_NPC(ch))
                return;
        one_argument(argument, arg);

        if (ch->in_room != NULL)
        {
                if (IS_SET(ch->in_room->room_flags, ROOM_ARENA))
                {
                        send_to_char("Your in the arena.\n\r", ch);
                        return;
                }
        }
        if (arg[0] == '\0')
        {
                send_to_char("Decapitate whom?\n\r", ch);
                return;
        }
        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r", ch);
                return;
        }
        if (ch == victim)
        {
                send_to_char("That might be a bit tricky...\n\r", ch);
                return;
        }
        if (IS_NPC(victim))
        {
                send_to_char("You can only decapitate other players.\n\r",
                             ch);
                return;
        }
        if (victim->position > 1)
        {
                send_to_char
                        ("You can only do this to mortally wounded players.\n\r",
                         ch);
                return;
        }
        if ((get_age(ch) - 17) < 2)
        {
                send_to_char("You'll have to wait till you have 4 hours.\n\r",
                             ch);
                return;
        }
        if ((get_age(victim) - 17) < 2)
        {
                send_to_char("They are a newbie, don't hurt them.\n\r", ch);
                return;
        }

        if (is_safe(ch, victim))
                return;
        if (in_fortress(ch) && !arena2death)
        {
                fortresskill(ch, victim);
                return;
        }
        else if (in_fortress(ch) && arena2death)
        {
                deathmatch = TRUE;
                arena2 = TRUE;
                if ((location = get_room_index(ROOM_VNUM_VICTORY)) == NULL)
                        return;
                char_from_room(victim);
                char_to_room(victim, location);
                char_from_room(ch);
                char_to_room(ch, location);
                ch->fight_timer = 0;
                update_bets(ch, victim, TRUE);
        }
        else if (ragnarok)
        {
                ragnarokdecap(ch, victim);
                return;
        }

        /*
         * spamcap 
         */
/*
        if (!str_cmp(ch->pcdata->last_decap[0], victim->name)
            && str_cmp(ch->pcdata->retaliation, victim->name))
        {
                send_to_char
                        ("Don't spamcap, it's not nice, find someone else to pick on.\n\r",
                         ch);
                return;
        }
*/

        if (victim->race == 0 && ch->race > 0 && str_cmp( victim->name, ch->pcdata->retaliation ) )
	{
                paradox_punish = TRUE;
		do_jail(ch,"self");
	}
        else if ((victim->tier) < ch->tier && (ch->race - victim->race) > 5)
                paradox_punish = TRUE;
        else if ((ch->tier) < 1 && (victim->tier) < 1
                 && (ch->race - victim->race) > 5)
                paradox_punish = TRUE;
        else if (victim->tier == 0 && victim->race == 0
                 && (ch->tier > 0 && ch->race > 3))
                paradox_punish = TRUE;
        if (ch->level > 6 && paradox_punish == TRUE)
                paradox_punish = FALSE;
        if (in_fortress(ch) || in_arena(ch))
                paradox_punish = FALSE;
        if (paradox_punish)
        {
                ch->paradox[0] += ch->race;
                ch->paradox[1] += ch->race;
                ch->paradox[2] += PARADOX_TICK;
                xprintf(buf, "%s has been decapitated by %s for no status.",
                        victim->pcdata->switchname, ch->pcdata->switchname);
                do_info(ch, buf);
                xprintf(buf, "%s decapitated by %s at %d for no status.",
                        victim->pcdata->switchname, ch->pcdata->switchname,
                        victim->in_room->vnum);
                wiznet(buf, ch, NULL, WIZ_DEATHS, 0, 0);
                log_string(LOG_GAME, buf);
                if (!IS_ITEMAFF(victim, ITEMA_ARTIFACT))
                        do_paradox(ch, "self");
                if (IS_CLASS(victim, CLASS_VAMPIRE))
                        do_mortalvamp(victim, "");
                else if (IS_SET(victim->special, SPC_WOLFMAN))
                        do_unwerewolf(victim, "");
                if (IS_CLASS(victim, CLASS_MAGE)
                    && IS_AFFECTED(victim, AFF_POLYMORPH))
                        do_unpolymorph(victim, "");
                ch->fight_timer += 10;
                if (IS_CLASS(victim, CLASS_DRACONIAN))
                        draconian_death(ch, victim);
                behead(victim);
                do_beastlike(ch, "");
                if (ch->pcdata->kingdom != 0)
                        kingdom_table[ch->pcdata->kingdom].kills += 1;
                if (victim->pcdata->kingdom != 0)
                        kingdom_table[victim->pcdata->kingdom].deaths += 1;
                save_kingdoms();
                ch->pkill = ch->pkill + 1;
                victim->pdeath = victim->pdeath + 1;
                victim->level = 2;
                ch->pcdata->bounty += number_range(30, 80);
                players_decap++;
                return;
        }

        if (victim->pcdata->bounty > 0)
        {
                xprintf(buf,
                        "You recive a %d bones bounty, for killing %s.\n\r",
                        victim->pcdata->bounty, victim->name);
                send_to_char(buf, ch);
                ch->bones += victim->pcdata->bounty;
                victim->pcdata->bounty = 0;
        }

        /*
         * update quest status 
         */
        {
                QUEST_DATA *quest;
                bool      found = FALSE;

                for (quest = ch->pcdata->quests; quest && !found;
                     quest = quest->next)
                {
                        if (quest->type == QT_PK)
                        {
                                if (quest->vnums[0] ==
                                    victim->pcdata->playerid)
                                {
                                        quest->vnums[0] = -1;
                                        found = TRUE;
                                        send_to_char
                                                ("#GYou have fulfilled a quest.#n\n\r",
                                                 ch);
                                }
                        }
                }
        }

        update_edge(ch);

        ch->exp += victim->exp / 2;
        victim->exp -= victim->exp / 2;

        send_losthead(victim);
        send_decap(ch);

        if (ch->pcdata->religion != 0)
        {
                faith = victim->max_hit / 2500;
                //if (is_upgrade(victim)) faith *= 1.5;
                if (rel_oppose(ch, victim))
                        faith *= 1.5;
                if (ch->pcdata->relrank > victim->pcdata->relrank)
                        faith /= 2;
                if (victim->pcdata->relrank > ch->pcdata->relrank)
                        faith *= 3;
                ch->pcdata->faith += faith;
                ch->pcdata->current_faith += faith;
                victim->pcdata->faith -= faith / 2;
                victim->pcdata->current_faith -= faith / 2;
                if (victim->pcdata->faith < 0)
                        victim->pcdata->faith = 0;
                if (victim->pcdata->current_faith < 0)
                        victim->pcdata->current_faith = 0;
                xprintf(buf, "#pYou gain #C%d#p faithpoints.#n\n\r", faith);
                send_to_char(buf, ch);
                check_faith(ch);
                check_faith(victim);
        }

        /*
         * hitpoints reward for killing
         */
        ch->hit += victim->max_hit * 0.1;
        if (ch->hit > ch->max_hit)
                ch->hit = ch->max_hit;

        /*
         * Update the last decaps to prevent spamcapping.
         */
        free_string(ch->pcdata->last_decap[1]);
        ch->pcdata->last_decap[1] = str_dup(ch->pcdata->last_decap[0]);
        free_string(ch->pcdata->last_decap[0]);
        ch->pcdata->last_decap[0] = str_dup(victim->name);
        /*
         * Retaliation update
         */
        free_string(victim->pcdata->retaliation);
        victim->pcdata->retaliation = str_dup(ch->name);
        if (!str_cmp(ch->pcdata->retaliation, victim->name))
        {
                free_string(ch->pcdata->retaliation);
                ch->pcdata->retaliation = str_dup("Noone");
        }
	victim->combat = NULL;
  	ch->combat = NULL;
        if (ch->race - ((ch->race / 100) * 100) == 0)
                ch->race = ch->race + 1;
        else if (ch->race - ((ch->race / 100) * 100) < 300)
                ch->race = ch->race + 1;
        if (ch->race - ((ch->race / 100) * 100) == 0)
                victim->race = victim->race;
        else if (victim->race - ((victim->race / 100) * 100) > 0)
                victim->race = victim->race - 1;
        act("A misty white vapour pours from $N's corpse into your body.", ch,
            NULL, victim, TO_CHAR);
        act("A misty white vapour pours from $N's corpse into $n's body.", ch,
            NULL, victim, TO_NOTVICT);
        act("You double over in agony as raw energy pulses through your veins.", ch, NULL, NULL, TO_CHAR);
        act("$n doubles over in agony as sparks of energy crackle around $m.",
            ch, NULL, NULL, TO_NOTVICT);
        if (IS_CLASS(victim, CLASS_VAMPIRE))
                do_mortalvamp(victim, "");
        else if (IS_SET(victim->special, SPC_WOLFMAN))
                do_unwerewolf(victim, "");
        if (!deathmatch)
                ch->fight_timer += 10;
        if (IS_CLASS(ch, CLASS_DEMON))
        {
                send_to_char("You rip away a part of their soul.\n\r", ch);
                send_to_char
                        ("You feel a part of your soul has been torn away.\n\r",
                         victim);
                ch->pcdata->souls += 1;
        }
        if (IS_CLASS(victim, CLASS_DRACONIAN))
                draconian_death(ch, victim);
        behead(victim);
        dropinvis(ch);
        /*
         * Removes mage powers 
         */
        if (IS_ITEMAFF(victim, ITEMA_STEELSHIELD))
                REMOVE_BIT(ch->itemaffect, ITEMA_STEELSHIELD);
        if (IS_ITEMAFF(victim, ITEMA_BEAST))
                REMOVE_BIT(victim->itemaffect, ITEMA_BEAST);
        if (IS_ITEMAFF(victim, ITEMA_MAGESHIELD))
                REMOVE_BIT(victim->itemaffect, ITEMA_MAGESHIELD);
        if (IS_ITEMAFF(victim, ITEMA_ILLUSIONS))
                REMOVE_BIT(victim->itemaffect, ITEMA_ILLUSIONS);
        if (IS_ITEMAFF(victim, ITEMA_DEFLECTOR))
                REMOVE_BIT(victim->itemaffect, ITEMA_DEFLECTOR);

        if (IS_SET(victim->sohbits, SOH_TALONS))
        {
                stc("#rYour talons retract back into your fingers#n\n\r",
                    victim);
                REMOVE_BIT(victim->sohbits, SOH_TALONS);
        }

        if (!TIME_UP(victim, TIMER_SLOW) || IS_SET(victim->sohbits, SOH_SLOW))
        {
                SET_TIMER(victim, TIMER_SLOW, 0);
                REMOVE_BIT(victim->sohbits, SOH_SLOW);
                stc("#pYour movements return to normal speed.#n\n\r", victim);
        }

        if (!TIME_UP(victim, TIMER_HASTE)
            || IS_SET(victim->sohbits, SOH_HASTE))
        {
                SET_TIMER(victim, TIMER_HASTE, 0);
                REMOVE_BIT(victim->sohbits, SOH_HASTE);
                stc("#BYour movements return to normal speed.#n\n\r", victim);
        }

        if (!TIME_UP(victim, TIMER_ACCURACY)
            || IS_SET(victim->sohbits, SOH_ACCURACY))
        {
                SET_TIMER(victim, TIMER_ACCURACY, 0);
                REMOVE_BIT(victim->sohbits, SOH_ACCURACY);
                stc("#gThe #Ggreen #Yglow #gabout you fades.#n\n\r", victim);
        }

        do_beastlike(ch, "");
        ch->pkill = ch->pkill + 1;
        victim->pdeath = victim->pdeath + 1;
        if (!IS_CLASS(victim, CLASS_NINJA))
                victim->rage = 0;
        victim->level = 2;
        decap_message(ch, victim);
        xprintf(buf, "%s decapitated by %s at %d.",
                victim->pcdata->switchname, ch->pcdata->switchname,
                victim->in_room->vnum);
        wiznet(buf, ch, NULL, WIZ_DEATHS, 0, 0);
        log_string(LOG_GAME, buf);
        players_decap++;
        ch->pcdata->bounty += number_range(30, 80);

        /*
         * update kingdoms
         */
        if (ch->pcdata->kingdom != 0)
                kingdom_table[ch->pcdata->kingdom].kills++;
        if (victim->pcdata->kingdom != 0)
                kingdom_table[victim->pcdata->kingdom].deaths++;
	save_kingdoms();
        if (!IS_NPC(victim) && IS_SET(ch->act2, PLR_AUTOHEAD))
                do_sacrifice(victim, "head");
        return;
}

void decap_message(CHAR_DATA * ch, CHAR_DATA * victim)
{
        OBJ_DATA *obj;
        char      buf[MAX_STRING_LENGTH];
        bool      unarmed = FALSE;

        if (IS_NPC(ch) || IS_NPC(victim))
                return;
        if ((obj = get_eq_char(ch, WEAR_WIELD)) == NULL)
        {
                if ((obj = get_eq_char(ch, WEAR_HOLD)) == NULL)
                {
                        unarmed = TRUE;
                }
        }
        if ((obj != NULL && obj->item_type != ITEM_WEAPON))
                unarmed = TRUE;

        /*
         * The players own decap message 
         */
        if (IS_SET(ch->pcdata->jflags, JFLAG_SETDECAP))
        {
                special_decap_message(ch, victim);
                return;
        }

        if (ch->pcdata->religion != 0)
                xprintf(buf,
                        religion_table[ch->pcdata->religion].decapmessage,
                        victim->name, ch->name);
        else if (unarmed)
        {
                if (victim->sex == SEX_MALE)
                        xprintf(buf, "#P%s #Ygot his head torn off by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
                else if (victim->sex == SEX_FEMALE)
                        xprintf(buf, "#P%s #Ygot her head torn off by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
                else
                        xprintf(buf, "#P%s #Ygot its head torn off by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
        }
        else if (obj->value[3] == 1)
        {
                if (victim->sex == SEX_MALE)
                        xprintf(buf,
                                "#P%s #Ygot his head sliced off by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
                else if (victim->sex == SEX_FEMALE)
                        xprintf(buf,
                                "#P%s #Ygot her head sliced off by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
                else
                        xprintf(buf,
                                "#P%s #Ygot its head sliced off by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
        }
        else if (obj->value[3] == 2)
        {
                if (victim->sex == SEX_MALE)
                        xprintf(buf,
                                "#P%s #Ygot his heart stabbed through by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
                else if (victim->sex == SEX_FEMALE)
                        xprintf(buf,
                                "#P%s #Ygot her heart stabbed through by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
                else
                        xprintf(buf,
                                "#P%s #Ygot its heart stabbed through by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
        }
        else if (obj->value[3] == 3)
        {
                if (victim->sex == SEX_MALE)
                        xprintf(buf,
                                "#P%s #Ygot his head slashed off by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
                else if (victim->sex == SEX_FEMALE)
                        xprintf(buf,
                                "#P%s #Ygot her head slashed off by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
                else
                        xprintf(buf,
                                "#P%s #Ygot its head slashed off by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
        }
        else if (obj->value[3] == 4)
                xprintf(buf, "#P%s #Ygot strangled by #R%s",
                        victim->pcdata->switchname, ch->pcdata->switchname);
        else if (obj->value[3] == 5)
                xprintf(buf,
                        "#R%s #Yruns a clawed hand through #P%s #Yand pulls out the heart#n",
                        ch->pcdata->switchname, victim->pcdata->switchname);
        else if (obj->value[3] == 6)
                xprintf(buf,
                        "#R%s #Yshoots #P%s #Yseveral times and spits on the corpse#n",
                        ch->pcdata->switchname, victim->pcdata->switchname);
        else if (obj->value[3] == 7)
                xprintf(buf,
                        "#R%s #Ypounds #P%s #Yon the head and the skull caves in#n",
                        ch->pcdata->switchname, victim->pcdata->switchname);
        else if (obj->value[3] == 8)
                xprintf(buf, "#R%s #Ycrushes #P%s #Yto a bloody pulp#n",
                        ch->pcdata->switchname, victim->pcdata->switchname);
        else if (obj->value[3] == 9)
                xprintf(buf,
                        "#P%s #Yhas been grepped by #R%s#Y, that's just mean!#n",
                        victim->pcdata->switchname, ch->pcdata->switchname);
        else if (obj->value[3] == 10)
                xprintf(buf, "#P%s #Ywas bitten to death by #R%s#n",
                        victim->pcdata->switchname, ch->pcdata->switchname);
        else if (obj->value[3] == 11)
                xprintf(buf,
                        "#R%s #Yhas punctured the lungs of #P%s#Y, what a meanie!#n",
                        ch->pcdata->switchname, victim->pcdata->switchname);
        else if (obj->value[3] == 12)
                xprintf(buf,
                        "#R%s #Ygrabs #P%s #Yby the head and sucks the brain out#n",
                        ch->pcdata->switchname, victim->pcdata->switchname);
        else
        {
                if (victim->sex == SEX_MALE)
                        xprintf(buf,
                                "#P%s #Ygot his head sliced off by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
                else if (victim->sex == SEX_FEMALE)
                        xprintf(buf,
                                "#P%s #Ygot her head sliced off by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
                else
                        xprintf(buf,
                                "#P%s #Ygot its head sliced off by #R%s#n",
                                victim->pcdata->switchname,
                                ch->pcdata->switchname);
        }
        if (IS_CLASS(ch, CLASS_JEDI))
                xprintf(buf,
                        "#B%s #0drops to his knees and practically melts as %s's #Wlightsabre#0 Is thrust into the lazer charred corpse.#n",
                        victim->pcdata->switchname, ch->pcdata->switchname);
        death_info(buf);
        return;
}

void do_crack(CHAR_DATA * ch, char *argument)
{
        OBJ_DATA *obj;
        OBJ_DATA *right;
        OBJ_DATA *left;

        right = get_eq_char(ch, WEAR_WIELD);
        left = get_eq_char(ch, WEAR_HOLD);
        if (right != NULL && right->pIndexData->vnum == 12)
                obj = right;
        else if (left != NULL && left->pIndexData->vnum == 12)
                obj = left;
        else
        {
                send_to_char("You are not holding any heads.\n\r", ch);
                return;
        }
        act("You hurl $p at the floor.", ch, obj, NULL, TO_CHAR);
        act("$n hurls $p at the floor.", ch, obj, NULL, TO_ROOM);
        act("$p cracks open, leaking brains out across the floor.", ch, obj,
            NULL, TO_CHAR);
        if (obj->chobj != NULL)
        {
                act("$p cracks open, leaking brains out across the floor.",
                    ch, obj, obj->chobj, TO_NOTVICT);
                act("$p crack open, leaking brains out across the floor.", ch,
                    obj, obj->chobj, TO_VICT);
        }
        else
        {
                act("$p cracks open, leaking brains out across the floor.",
                    ch, obj, NULL, TO_ROOM);
        }
        crack_head(ch, obj, obj->name);
        obj_from_char(obj);
        extract_obj(obj);
}

void crack_head(CHAR_DATA * ch, OBJ_DATA * obj, char *argument)
{
        CHAR_DATA *victim;
        MOB_INDEX_DATA *pMobIndex;

        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        char      buf[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        if (str_cmp(arg2, "mob") && obj->chobj != NULL && !IS_NPC(obj->chobj)
            && IS_AFFECTED(obj->chobj, AFF_POLYMORPH))
        {
                victim = obj->chobj;
                make_part(victim, "cracked_head");
                make_part(victim, "brain");
                xprintf(buf, "the quivering brain of %s", victim->name);
                free_string(victim->morph);
                victim->morph = str_dup(buf);
                return;
        }
        else if (!str_cmp(arg2, "mob"))
        {
                if ((pMobIndex = get_mob_index(obj->value[1])) == NULL)
                        return;
                victim = create_mobile(pMobIndex);
                char_to_room(victim, ch->in_room);
                make_part(victim, "cracked_head");
                make_part(victim, "brain");
                extract_char(victim, TRUE);
                return;
        }
        else
        {
                if ((pMobIndex = get_mob_index(30002)) == NULL)
                        return;
                victim = create_mobile(pMobIndex);
                xprintf(buf, capitalize(arg2));
                free_string(victim->short_descr);
                victim->short_descr = str_dup(buf);
                char_to_room(victim, ch->in_room);
                make_part(victim, "cracked_head");
                make_part(victim, "brain");
                extract_char(victim, TRUE);
                return;
        }
        return;
}

/* Voodoo skill by KaVir */

void do_voodoo(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *victim;
        OBJ_DATA *obj;
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        char      buf[MAX_INPUT_LENGTH];
        char      part1[MAX_INPUT_LENGTH];
        char      part2[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);
        if (arg1[0] == '\0')
        {
                send_to_char("Who do you wish to use voodoo magic on?\n\r",
                             ch);
                return;
        }
        if ((obj = get_eq_char(ch, WEAR_HOLD)) == NULL)
        {
                send_to_char("You are not holding a voodoo doll.\n\r", ch);
                return;
        }
        if ((victim = get_char_world(ch, arg1)) == NULL)
        {
                send_to_char("They are not here.\n\r", ch);
                return;
        }
        if (IS_NPC(victim))
        {
                send_to_char("Not on NPC's.\n\r", ch);
                return;
        }
        xprintf(part2, obj->name);
        xprintf(part1, "%s voodoo doll", victim->name);
        if (str_cmp(part1, part2))
        {
                xprintf(buf, "But you are holding %s, not %s!\n\r",
                        obj->short_descr, victim->name);
                send_to_char(buf, ch);
                return;
        }
        if (arg2[0] == '\0')
                send_to_char
                        ("You can 'stab', 'burn' or 'throw' the doll.\n\r",
                         ch);
        else if (!str_cmp(arg2, "stab"))
        {
                WAIT_STATE(ch, 12);
                act("You stab a pin through $p.", ch, obj, NULL, TO_CHAR);
                act("$n stabs a pin through $p.", ch, obj, NULL, TO_ROOM);
                if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO))
                        return;
                act("You feel an agonising pain in your chest!", victim, NULL,
                    NULL, TO_CHAR);
                act("$n clutches $s chest in agony!", victim, NULL, NULL,
                    TO_ROOM);
        }
        else if (!str_cmp(arg2, "burn"))
        {
                WAIT_STATE(ch, 12);
                act("You set fire to $p.", ch, obj, NULL, TO_CHAR);
                act("$n sets fire to $p.", ch, obj, NULL, TO_ROOM);
                act("$p burns to ashes.", ch, obj, NULL, TO_CHAR);
                act("$p burns to ashes.", ch, obj, NULL, TO_ROOM);
                obj_from_char(obj);
                extract_obj(obj);
                if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO))
                        return;
                if (IS_AFFECTED(victim, AFF_FLAMING))
                        return;
                SET_BIT(victim->affected_by, AFF_FLAMING);
                act("You suddenly burst into flames!", victim, NULL, NULL,
                    TO_CHAR);
                act("$n suddenly bursts into flames!", victim, NULL, NULL,
                    TO_ROOM);
        }
        else if (!str_cmp(arg2, "throw"))
        {
                WAIT_STATE(ch, 12);
                act("You throw $p to the ground.", ch, obj, NULL, TO_CHAR);
                act("$n throws $p to the ground.", ch, obj, NULL, TO_ROOM);
                obj_from_char(obj);
                obj_to_room(obj, ch->in_room);
                if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO))
                        return;
                if (victim->position < POS_STANDING)
                        return;
                if (victim->position == POS_FIGHTING)
                        stop_fighting(victim, TRUE);
                act("A strange force picks you up and hurls you to the ground!", victim, NULL, NULL, TO_CHAR);
                act("$n is hurled to the ground by a strange force.", victim,
                    NULL, NULL, TO_ROOM);
                victim->position = POS_RESTING;
                victim->hit =
                        victim->hit - number_range(ch->level,
                                                   (5 * ch->level));
                update_pos(victim);
                if (victim->position == POS_DEAD && !IS_NPC(victim))
                {
                        do_killperson(ch, victim->name);
                        return;
                }
        }
        else
                send_to_char
                        ("You can 'stab', 'burn' or 'throw' the doll.\n\r",
                         ch);
        return;
}

void do_garotte(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        OBJ_DATA *obj;
        int       chance;

        one_argument(argument, arg);
        chance = number_percent();

        if (IS_NPC(ch))
                return;
        if (!IS_CLASS(ch, CLASS_DROW)
            || !IS_SET(ch->pcdata->powers[1], DPOWER_GAROTTE))
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }
        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("Garotte whom?\n\r", ch);
                return;
        }
        if (victim == ch)
        {
                send_to_char("That would be a bad idea.\n\r", ch);
                return;
        }
        if (is_safe(ch, victim))
                return;
        if (((obj = get_eq_char(ch, WEAR_WIELD)) == NULL
             || obj->value[3] != 4)
            && ((obj = get_eq_char(ch, WEAR_HOLD)) == NULL
                || obj->value[3] != 4))
        {
                send_to_char("You need to wield a whip.\n\r", ch);
                return;
        }
        WAIT_STATE(ch, 12);
        if (chance <= 2)
        {
                send_to_char("You cannot get close enough.\n\r", ch);
                return;
        }
        if (chance == 10 || chance == 15 || chance == 20 || chance == 25)
        {
                act("$n wraps a whip around $N's neck! $N is choking to death!", ch, NULL, victim, TO_NOTVICT);
                act("You wrap your whip around $N's neck and refuse to let go!", ch, NULL, victim, TO_CHAR);
                act("$n wraps his whip around your neck, and you can't break free!", ch, NULL, victim, TO_VICT);
                if (!IS_NPC(victim))
                {
                        one_hit(ch, victim, gsn_garotte, 1);
                        one_hit(ch, victim, gsn_garotte, 1);
                        one_hit(ch, victim, gsn_garotte, 1);
                        one_hit(ch, victim, gsn_garotte, 1);
                }
                else
                {
                        victim->hit = 1;
                        damage(ch, victim, 1000, gsn_garotte);
                }
                return;
        }
        else
        {
                one_hit(ch, victim, gsn_garotte, 1);
                one_hit(ch, victim, gsn_garotte, 1);
                one_hit(ch, victim, gsn_garotte, 1);
                one_hit(ch, victim, gsn_garotte, 1);
                return;
        }
        return;
}

void do_dark_garotte(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        OBJ_DATA *obj;
        int       chance;
        int       sn;

        one_argument(argument, arg);
        chance = number_percent();
        if (IS_NPC(ch))
                return;
        if (ch->in_room != NULL)
        {
                if (IS_SET(ch->in_room->room_flags, ROOM_ARENA))
                {
                        send_to_char("Your in the arena.\n\r", ch);
                        return;
                }
        }
        if (!IS_CLASS(ch, CLASS_DROW)
            || !IS_SET(ch->pcdata->powers[1], DPOWER_GAROTTE)
            || !IS_SET(ch->pcdata->powers[1], DPOWER_DGAROTTE))
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }
        if (!IS_SET(ch->newbits, NEW_DARKNESS))
        {
                send_to_char("You have to be surrounded in darkness.\n\r",
                             ch);
                return;
        }
        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                send_to_char("Dark Garotte whom?\n\r", ch);
                return;
        }
        if (victim == ch)
        {
                send_to_char("That would be a bad idea.\n\r", ch);
                return;
        }
        if (IS_NPC(victim) && victim->level > 2000)
        {
                send_to_char("That would be a bad idea, too big mob.\n\r",
                             ch);
                return;
        }
        if (is_safe(ch, victim))
                return;
        if (((obj = get_eq_char(ch, WEAR_WIELD)) == NULL
             || obj->value[3] != 4)
            && ((obj = get_eq_char(ch, WEAR_HOLD)) == NULL
                || obj->value[3] != 4))
        {
                send_to_char("You need to wield a whip.\n\r", ch);
                return;
        }
        if (victim->fighting != NULL)
        {
                send_to_char("You can't garotte a fighting person.\n\r", ch);
                return;
        }
        if (victim->hit < victim->max_hit)
        {
                act("$N is too hurt for you to sneak up.", ch, NULL, victim,
                    TO_CHAR);
                return;
        }
        WAIT_STATE(ch, skill_table[gsn_garotte].beats);
        REMOVE_BIT(ch->newbits, NEW_DARKNESS);
        REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
        if (IS_SET(ch->pcdata->powers[1], DPOWER_WEB))
        {
                if ((sn = skill_lookup("web")) > 0)
                        (*skill_table[sn].spell_fun) (sn, 50, ch, victim);
        }
        if (chance > 95)
        {
                act("$n wraps a whip around $N's neck! $N is choking to death!", ch, NULL, victim, TO_NOTVICT);
                act("You wrap your whip around $N's neck and refuse to let go!", ch, NULL, victim, TO_CHAR);
                act("$n wraps his whip around your neck, and you can't break free!", ch, NULL, victim, TO_VICT);
                one_hit(ch, victim, gsn_garotte, 1);
                one_hit(ch, victim, gsn_garotte, 1);
                one_hit(ch, victim, gsn_garotte, 1);
                one_hit(ch, victim, gsn_garotte, 1);
                one_hit(ch, victim, gsn_garotte, 1);
                one_hit(ch, victim, gsn_garotte, 1);
        }
        else
        {
                one_hit(ch, victim, gsn_garotte, 1);
                one_hit(ch, victim, gsn_garotte, 1);
                one_hit(ch, victim, gsn_garotte, 1);
        }
        return;
}

bool has_timer(CHAR_DATA * ch)
{
        if (ch->fight_timer > 0 && !IS_NPC(ch))
        {
                send_to_char("Not until your fight timer runs out!\n\r", ch);
                return TRUE;
        }
        return FALSE;
}

void do_circle(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        OBJ_DATA *obj;

        one_argument(argument, arg);
        if (IS_NPC(ch))
                return;

        if (IS_CLASS(ch, CLASS_NINJA)
            && ch->pcdata->powers[NPOWER_NINGENNO] < 6)
        {
                stc("You have not learned this attack.", ch);
                return;
        }

        if ((victim = get_char_room(ch, NULL, arg)) == NULL)
        {
                if (ch->fighting == NULL)
                {
                        send_to_char("They aren't here.\n\r", ch);
                        return;
                }
                else
                        victim = ch->fighting;
        }
        if (victim == ch)
        {
                send_to_char("How can you circle yourself?\n\r", ch);
                return;
        }
        if (is_safe(ch, victim))
                return;
        if (((obj = get_eq_char(ch, WEAR_WIELD)) == NULL
             || obj->value[3] != 11)
            && ((obj = get_eq_char(ch, WEAR_HOLD)) == NULL
                || obj->value[3] != 11))
        {
                send_to_char("You need to wield a piercing weapon.\n\r", ch);
                return;
        }
        one_hit(ch, victim, gsn_circle, 1);
        if (number_range(1, 4) == 2)
        {
                send_to_char
                        ("You spin around once more, scoring a second hit.\n\r",
                         ch);
                one_hit(ch, victim, gsn_circle, 1);
        }
        WAIT_STATE(ch, 8);
        return;
}

void do_autostance(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];

        argument = one_argument(argument, arg);
        if (IS_NPC(ch))
                return;
        if (!str_cmp(arg, "none"))
        {
                send_to_char("You no longer autostance.\n\r", ch);
                ch->stance[MONK_AUTODROP] = STANCE_NONE;
        }
        else if (!str_cmp(arg, "crane"))
        {
                send_to_char("You now autostance into the crane stance.\n\r",
                             ch);
                ch->stance[MONK_AUTODROP] = STANCE_CRANE;
        }
        else if (!str_cmp(arg, "crab"))
        {
                send_to_char("You now autostance into the crab stance.\n\r",
                             ch);
                ch->stance[MONK_AUTODROP] = STANCE_CRAB;
        }
  else if (!str_cmp(arg, "pitbull"))
  {
    send_to_char("You now autostance into the Pitbull stance.\n\r", ch );
    ch->stance[MONK_AUTODROP] = STANCE_PITBULL;
  }
  else if (!str_cmp(arg, "viper"))
  {
    send_to_char("You now autostance into the viper stance.\n\r", ch );
    ch->stance[MONK_AUTODROP] = STANCE_VIPER;
  }
  else if (!str_cmp(arg, "mongoose"))
  {
    send_to_char("You now autostance into the mongoose stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_MONGOOSE;
  }
  else if (!str_cmp(arg, "hawk") && ch->stance[STANCE_CRANE] >= 200 && ch->stance[STANCE_VIPER] >= 200)
  {
    send_to_char("You now autostance into the Hawk stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_HAWK;
  }
  else if (!str_cmp(arg, "wolverine") && ch->stance[STANCE_CRANE] >= 200 && ch->stance[STANCE_MONGOOSE] >= 200)
  {
    send_to_char("You now autostance into the Wolverine stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_WOLVERINE;
  }
  else if (!str_cmp(arg, "grizzly") && ch->stance[STANCE_CRAB] >= 200 && ch->stance[STANCE_MONGOOSE] >= 200)
  {
    send_to_char("You now autostance into the Grizzly stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_GRIZZLY;
  }
  else if (!str_cmp(arg, "cougar") && ch->stance[STANCE_PITBULL] >= 200 && ch->stance[STANCE_VIPER] >= 200)
  {
    send_to_char("You now autostance into the Cougar stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_COUGAR;
  }
  else if (!str_cmp(arg, "badger") && ch->stance[STANCE_CRAB] >= 200 && ch->stance[STANCE_PITBULL] >= 200)
  {
    send_to_char("You now autostance into the Badger stance.\n\r", ch);
    ch->stance[MONK_AUTODROP] = STANCE_BADGER;
  }

        else if (!str_cmp(arg, "wolf") && (IS_CLASS(ch, CLASS_WEREWOLF))
                 && ch->stance[STANCE_COUGAR] >= 200
                 && ch->stance[STANCE_GRIZZLY] >= 200
                 && ch->stance[STANCE_WOLVERINE] >= 200
                 && ch->stance[STANCE_HAWK] >= 200
                 && ch->stance[STANCE_BADGER] >= 200)
        {
                send_to_char("You now autostance into the wolf stance.\n\r",
                             ch);
                ch->stance[MONK_AUTODROP] = STANCE_WOLF;
        }
        else if (!str_cmp(arg, "ss1") && ch->stance[19] != -1)
        {
                send_to_char("You now autostance into superstance one.\n\r",
                             ch);
                ch->stance[MONK_AUTODROP] = STANCE_SS1;
        }
        else if (!str_cmp(arg, "ss2") && ch->stance[20] != -1)
        {
                send_to_char("You now autostance into superstance two.\n\r",
                             ch);
                ch->stance[MONK_AUTODROP] = STANCE_SS2;
        }
        else if (!str_cmp(arg, "ss3") && ch->stance[21] != -1)
        {
                send_to_char("You now autostance into superstance three.\n\r",
                             ch);
                ch->stance[MONK_AUTODROP] = STANCE_SS3;
        }
        else if (!str_cmp(arg, "pizza") && ch->stance[22] != -1)
        {
                send_to_char
                        ("You now will assume the Pizzaman delivery stance.\n\r",
                         ch);
                ch->stance[MONK_AUTODROP] = STANCE_SS4;
        }
        else if (!str_cmp(arg, "zarius") && ch->stance[23] != -1)
        {
                send_to_char
                        ("#wYou now will fly into a #Rwicked #0Zarius #wstance!#n\n\r",
                         ch);
                ch->stance[MONK_AUTODROP] = STANCE_SS5;
        }
        else
                send_to_char("You can't set your autostance to that!\n\r",
                             ch);
}

void autodrop(CHAR_DATA * ch)
{
        char      buf[MAX_INPUT_LENGTH];
        char      buf2[MAX_INPUT_LENGTH];
        char      stancename[10];

        if (IS_NPC(ch))
                return;
        if (ch->stance[MONK_AUTODROP] == STANCE_NONE)
                return;
        if (ch->stance[MONK_AUTODROP] == STANCE_VIPER)
                xprintf(stancename, "viper");
        else if (ch->stance[MONK_AUTODROP] == STANCE_CRANE)
                xprintf(stancename, "crane");
        else if (ch->stance[MONK_AUTODROP] == STANCE_CRAB)
                xprintf(stancename, "crab");
        else if (ch->stance[MONK_AUTODROP] == STANCE_MONGOOSE)
                xprintf(stancename, "mongoose");
	else if (ch->stance[MONK_AUTODROP]==STANCE_PITBULL) sprintf(stancename,"Pitbull");
	else if (ch->stance[MONK_AUTODROP]==STANCE_HAWK) sprintf(stancename,"Hawk");
	else if (ch->stance[MONK_AUTODROP]==STANCE_BADGER) sprintf(stancename,"Badger");
	else if (ch->stance[MONK_AUTODROP]==STANCE_COUGAR) sprintf(stancename,"Cougar");
	else if (ch->stance[MONK_AUTODROP]==STANCE_WOLVERINE) sprintf(stancename,"Wolverine");
	else if (ch->stance[MONK_AUTODROP]==STANCE_GRIZZLY) sprintf(stancename,"Grizzly");
        else if (ch->stance[MONK_AUTODROP] == STANCE_SS1)
                xprintf(stancename, "ss1");
        else if (ch->stance[MONK_AUTODROP] == STANCE_SS2)
                xprintf(stancename, "ss2");
        else if (ch->stance[MONK_AUTODROP] == STANCE_SS3)
                xprintf(stancename, "ss3");
        else if (ch->stance[MONK_AUTODROP] == STANCE_SS4)
                xprintf(stancename, "pizza");
        else if (ch->stance[MONK_AUTODROP] == STANCE_SS5)
                xprintf(stancename, "zarius");
        else if (ch->stance[MONK_AUTODROP])
                xprintf(stancename, "wolf");
        else
                return;
        if (ch->stance[0] < 1)
        {
                ch->stance[0] = ch->stance[MONK_AUTODROP];;
                xprintf(buf, "#wYou drop into the #Y%s#w stance.",
                        stancename);
                act(buf, ch, NULL, NULL, TO_CHAR);
                xprintf(buf2, "#w$n drops into the #Y%s#w stance.",
                        stancename);
                act(buf2, ch, NULL, NULL, TO_ROOM);
        }
}

void dropinvis(CHAR_DATA * ch)
{
        if (ch->level < 7 && IS_SET(ch->act, AFF_HIDE))
                REMOVE_BIT(ch->act, AFF_HIDE);
        if (ch->level < 7 && IS_SET(ch->newbits2,NEW2_HIDE))
                REMOVE_BIT(ch->newbits2,NEW2_HIDE);
        if (ch->level < 7 && IS_SET(ch->act, PLR_WIZINVIS))
                REMOVE_BIT(ch->act, PLR_WIZINVIS);
        if ((IS_CLASS(ch, CLASS_DROW) || IS_CLASS(ch, CLASS_DROID)
             || IS_CLASS(ch, CLASS_CYBORG))
            && IS_SET(ch->newbits, NEW_DARKNESS))
        {
                REMOVE_BIT(ch->newbits, NEW_DARKNESS);
                REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
        }
        return;
}
