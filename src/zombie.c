/*This class was created by Kasai for use on Dark Skies originally. */


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

void do_evolve (CHAR_DATA * ch, char *argument)
{
  int cost;
  char buf[MSL];

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }

  cost = ch->pcdata->powers[ZOM_LEVEL] * 1000;
  if (ch->pcdata->powers[ZOM_LEVEL] >= ZOM_WIGHT)
    {
      stc ("You have already perfected your body.\n\r", ch);
      return;
    }
  if (cost > ch->pcdata->powers[ZOM_FLESH])
    {
      sprintf (buf,
	       "You do not have enough flesh points. You need %d more.\n\r",
	       cost - ch->pcdata->powers[ZOM_FLESH]);
      stc (buf, ch);
      return;
    }
  ch->pcdata->powers[ZOM_FLESH] -= cost;
  ch->pcdata->powers[ZOM_LEVEL] += 1;
  switch (ch->pcdata->powers[ZOM_LEVEL])
    {
    default:
      do_evolve (ch, "");
      return;
      break;

    case 2:
      stc
	("Rotting flesh covers your bare bones in sinewy strands. You are now a wretch.\n\r",
	 ch);
      stc ("#0Your bones can now become armor.\n\r", ch);
      stc ("#0Your eyes can now see death itself.\n\r", ch);
      stc ("#0You may destroy the soul of your victim.#n\n\r", ch);
      break;

    case 3:
      stc
	("The flesh covering your body grows thicker and tougher, covering all of your exposed bones. You are now a Zombie.\n\r",
	 ch);
      SET_BIT (ch->pcdata->powers[ZOM_ACTIVE], ZOM_DECAY);
      SET_BIT (ch->pcdata->powers[ZOM_ACTIVE], ZOM_ARISE);
      stc ("#0Your body exudes an aura of decay.\n\r", ch);
      stc ("#0Your body can move at the brink of death.\n\r", ch);
      stc ("#0You can ignite the fumes in corpses.#n\n\r", ch);
      break;

    case 4:
      stc
	("Your flesh darkens and your eyes burn red. You are now a Minion.\n\r",
	 ch);
      stc ("#0Claws grow where your fingers once were.\n\r", ch);
      SET_BIT (ch->pcdata->powers[ZOM_ACTIVE], ZOM_CLAWS);
      stc ("#0You gain control over your regenative powers.\n\r", ch);
      stc
	("#0Your body can spawn the bodies of those you have devoured.#n\n\r",
	 ch);
      break;

    case 5:
      stc
	("Your senses increase, and you hear the souls of the damned screaming.\n\r",
	 ch);
      SET_BIT (ch->pcdata->powers[ZOM_ACTIVE], ZOM_PLAGUE);
      SET_BIT (ch->pcdata->powers[ZOM_ACTIVE], ZOM_TOUGH);
      stc ("#0Your skin has grown tough and resiliant.\n\r", ch);
      stc ("#0Deadly plagues trail in your wake.\n\r", ch);
      stc ("#0You can command the lesser undead.#n\n\r", ch);
      break;

    case 6:
      stc
	("Your strength grows and your flesh begins to swirl about your body. You are now a Forsaken.\n\r",
	 ch);
      stc ("#0Your flesh can assimilate the bodies of the dead.\n\r", ch);
      stc ("#0Your flesh can absorb the dead and regenerate with them.\n\r",
	   ch);
      stc
	("#0You have the power to use the bodies you have assimilated.#n\n\r",
	 ch);
      break;

    case 7:
      stc
	("Your body turns as black as night, and you gain mastery over the plains of death.\n\r",
	 ch);
      stc ("#0You can walk through the plain of shadows.\n\r", ch);
      stc ("#0You can hide yourself from the eye.\n\r", ch);
      stc ("#0You can send off noxious fumes.\n\r", ch);
      stc ("#0You can walk through the ethereal plain.#n\n\r", ch);
      break;
    }

}

void do_satiate (CHAR_DATA * ch, char *argument)
{
  OBJ_DATA *corpse;
  OBJ_DATA *c_next;
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_INPUT_LENGTH];
  one_argument (argument, arg);
  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }



  for (corpse = ch->in_room->contents; corpse != NULL; corpse = c_next)
    {
      c_next = corpse->next_content;
      if ((corpse->item_type != ITEM_CORPSE_NPC
	   && corpse->item_type != ITEM_CORPSE_PC)
	  || corpse->pIndexData->vnum == 25)
	continue;
      ch->pcdata->powers[ZOM_FLESH] += 2;
	if(global_cp)      ch->pcdata->powers[ZOM_FLESH] += 4;
      act ("$n absorbs a corpse.", ch, NULL, NULL, TO_ROOM);
      if(!global_cp) sprintf (buf, "You gain #r2#n flesh point.\n\r");
	else sprintf (buf, "You gain #r4#n flesh point.\n\r");	
      send_to_char (buf, ch);
      for (obj = corpse->contains; obj; obj = obj_next)
	{
	  obj_next = obj->next_content;
	  obj_from_obj (obj);
	  obj_to_room (obj, ch->in_room);
	}

      if (corpse != NULL)
	extract_obj (corpse);
    }
  return;
}


void do_gravespawn (CHAR_DATA * ch, char *argument)
{

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }

  if (ch->position == POS_STANDING)
    {
      send_to_char
	("You dig yourself into the healing moist soils of the earth.\n\r",
	 ch);
      act ("$n digs themselves into the healing moist soils.", ch, NULL, NULL,
	   TO_ROOM);
      ch->position = POS_MEDITATING;
    }
  else
    {
      stc ("You must be standing.\n\r", ch);
      return;
    }

}

void do_zombiesight (CHAR_DATA * ch, char *argument)
{

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }

  if (!IS_SET (ch->affected_by, AFF_SHADOWSIGHT))
    {
      stc ("You can see through the shadows and the dark.\n\r", ch);
      SET_BIT (ch->affected_by, AFF_SHADOWSIGHT);
    }
  else
    {
      stc ("You can no longer see through the shadows and the dark.\n\r", ch);
      REMOVE_BIT (ch->affected_by, AFF_SHADOWSIGHT);
    }

}

void do_fleshportal (CHAR_DATA * ch, char *argument)
{

  char arg[MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location;
  CHAR_DATA *victim;

  one_argument (argument, arg);
  if (IS_NPC (ch))
    return;
  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      send_to_char ("Huh?\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_LEVEL] < 2)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }

  if ((victim = get_char_world (ch, arg)) == NULL)
    {
      send_to_char ("Who do you wish to fleshportal to?\n\r", ch);
      return;
    }
  if (IS_SET (victim->in_room->room_flags, ROOM_ASTRAL))
    {
      stc ("You can't find it's room.\n\r", ch);
      return;
    }
  if( IS_IMMUNE(victim, IMM_TRAVEL ) && !ragnarok && victim->fight_timer == 0) {
     send_to_char("He doesnt want you near him!\n\r",ch);
     return;
  }
  if (IS_SET (ch->in_room->room_flags, ROOM_ASTRAL))
    {
      stc ("Your room is not connected to the astral plane.\n\r", ch);
      return;
    }
  location = victim->in_room;
  if (ch->move < 500)
    {
      send_to_char ("You don't have the strength.\n\r", ch);
      return;
    }


  act ("You enter a portal made from flesh.", ch, NULL, NULL, TO_CHAR);
  act ("$n grows a portal of flesh from their body and enters it.", ch, NULL,
       NULL, TO_ROOM);

  char_from_room (ch);
  char_to_room (ch, location);
  do_look (ch, "auto");
  if (victim->fight_timer > 0)
    WAIT_STATE (ch, 8);
  act ("You exit your fleshportal by $N.", ch, NULL, victim, TO_CHAR);
  act ("$n appears from a portal of flesh.", ch, NULL, NULL, TO_ROOM);
  return;

}

void do_shatter (CHAR_DATA * ch, char *argument)
{

  char buf[MSL];
  int damage;
  int damage2;
  int oldl;
  CHAR_DATA *victim;

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }

  if (ch->fighting == NULL || IS_NPC ((victim = ch->fighting)))
    {
      stc ("Only against players in combat!\n\r", ch);
      return;
    }
  if (number_range (1,4) == 1)
    {
      stc ("You failed!\n\r", ch);
      WAIT_STATE (ch, 12);
      return;
    }
  damage = number_range (1000, 5000);
  ch->hit -= damage;
  damage2 = damage *2;
  victim->hit -= damage2;
  sprintf (buf,
	   "You burst into a pile of bones as your enemy attacks to avoid them! [#R%d#n]\n\r",
	   damage);
  stc (buf, ch);
  sprintf (buf,
	   "%s bursts into a pile of bones from your attack! [#7%d#n]\n\r",
	   ch->pcdata->switchname, damage2);
  stc (buf, victim);
  hurt_person (ch, ch, damage);
  hurt_person (ch, victim, damage);
  oldl = ch->level;
  ch->level = 12;
  do_peace (ch, "");
  ch->level = oldl;
  WAIT_STATE (ch, 10);
  WAIT_STATE (victim, 20);

}

void do_deathvision (CHAR_DATA * ch, char *argument)
{

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_LEVEL] < 2)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }

  if (!IS_SET (ch->newbits2, NEW2_DEATHSENSE))
    {
      stc ("You can see the auras of life and death.\n\r", ch);
      SET_BIT (ch->newbits2, NEW2_DEATHSENSE);
      SET_BIT (ch->act, PLR_HOLYLIGHT);
    }
  else
    {
      stc ("You can no longer see the auras of life and death.\n\r", ch);
      REMOVE_BIT (ch->newbits2, NEW2_DEATHSENSE);
      REMOVE_BIT (ch->act, PLR_HOLYLIGHT);

    }

}

void do_relinquish (CHAR_DATA * ch, char *argument)
{

  char arg[MSL];
  int sn;
  CHAR_DATA *victim;
  int level;
  one_argument (argument, arg);

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_LEVEL] < 2)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }
  if (arg[0] == '\0')
    {
      stc ("Desanc whom?\n\r", ch);
      return;
    }
  if (ch->fighting != NULL)
    {
      stc ("You can only use this power to start combat.\n\r", ch);
      return;
    }
  if ((victim = get_char_room (ch, NULL, arg)) == NULL)
    {
      stc ("They are not here.\n\r", ch);
      return;
    }
  if(victim == ch) 
    {
     stc("not on yourself dummy",ch);
     return;
    }

  if (is_safe (ch, victim))
    return;

  if (number_range (1,3) == 1)
    {
      stc ("You failed to Desanct!\n\r", ch);
      WAIT_STATE (ch, 6);
      return;
    }

  act
    ("$n cackles with glee as he destroys the hold of god from your body.\n\r",
     ch, NULL, victim, TO_VICT);
  act
    ("$n cackles with glee as he destroys the hold of god from $N's body.\n\r",
     ch, NULL, victim, TO_ROOM);
  act
    ("You cackles with glee as you destroy the hold of god from $N's body.\n\r",
     ch, NULL, victim, TO_CHAR);
  sn = skill_lookup ("desanct");
  level = 25;
  (*skill_table[sn].spell_fun) (sn, level, ch, victim);
  set_fighting (ch, victim, TRUE);
  set_fighting (victim, ch, TRUE);
  if (!IS_NPC (victim))
    ch->combat = victim;

  WAIT_STATE (ch, 18);

}

void do_explosion (CHAR_DATA * ch, char *argument)
{
  OBJ_DATA *corpse;
  int dam;
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  char arg[MSL];
  char buf[MSL];

  one_argument (argument, arg);

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_LEVEL] < 3)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }

  if (arg[0] == '\0')
    {
      stc ("Destroy which corpses?\n\r", ch);
      return;
    }

  if ((corpse = get_obj_room (ch, arg)) == NULL)
    {
      stc ("That corpse isnt here.\n\r", ch);
      return;
    }
  if (corpse->item_type != ITEM_CORPSE_NPC)
    {
      stc ("Only on mobile corpses.\n\r", ch);
      return;
    }
  sprintf (buf, "%s ignites the gases within %s!", ch->pcdata->switchname,
	   corpse->short_descr);
  act (buf, ch, NULL, NULL, TO_ROOM);
  act (buf, ch, NULL, NULL, TO_CHAR);

  for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
      vch_next = vch->next_in_room;
      if (can_see (ch, vch))
	if (IS_NPC (vch))
	  {
	    dam = 2500 * ch->pcdata->powers[ZOM_LEVEL];
	    dam = number_range (dam * .5, dam * .9);
	    sprintf (buf, "The firestorm rolls over %s!#r [#R%d#r]#n\n\r",
		     vch->name, dam);
	    send_to_char (buf, ch);
	    set_fighting (vch, ch, TRUE);
	    hurt_person (ch, vch, dam);
	  }
    }
  WAIT_STATE (ch, 12);
  if (corpse != NULL)
    extract_obj (corpse);

}

void do_regrow (CHAR_DATA * ch, char *argument)
{

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE) && !IS_RACE(ch, RACE_TROLL)
	&& !IS_RACE(ch, RACE_HALFTROLL))
    {
      stc ("Huh?\n\r", ch);
      return;
    }
  if (IS_CLASS(ch, CLASS_ZOMBIE) && ch->pcdata->powers[ZOM_LEVEL] < 4)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }
  if (IS_CLASS(ch, CLASS_ZOMBIE) && ch->pcdata->powers[ZOM_FLESH] < 100)
    {
      stc ("You need 100 flesh points to do this!\n\r", ch);
      return;
    }
  act ("$n regrows all of their lost limbs.", ch, NULL, NULL, TO_ROOM);
  act ("You regrow all of your lost limbs.", ch, NULL, NULL, TO_CHAR);
  ch->loc_hp[0] = 0;
  ch->loc_hp[1] = 0;
  ch->loc_hp[2] = 0;
  ch->loc_hp[3] = 0;
  ch->loc_hp[4] = 0;
  ch->loc_hp[5] = 0;
  ch->loc_hp[6] = 0;
  WAIT_STATE (ch, 6);
  ch->pcdata->powers[ZOM_FLESH] -= 100;

}

void do_spawn (CHAR_DATA * ch, char *argument)
{
  int i = 0;
  OBJ_DATA *corpse;

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_LEVEL] < 4)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_FLESH] < 10)
    {
      stc ("You need 10 flesh points to do this!\n\r", ch);
      return;
    }

  act ("$n spawns forth a group of corpses from $s skin.\n\r", ch, NULL, NULL,
       TO_ROOM);
  act ("You spawn forth a group of corpses from your skin.\n\r", ch, NULL,
       NULL, TO_CHAR);
  for (i = 0; i < 3; i += 1)
    {
      corpse = create_object (get_obj_index (25), 0);
      corpse->timer = number_range (2, 3);
      corpse->value[2] = 1000;
      SET_BIT (corpse->quest, QUEST_ZOMBIE);
      if (IS_AFFECTED (ch, AFF_SHADOWPLANE))
	SET_BIT (corpse->extra_flags, ITEM_SHADOWPLANE);
      obj_to_room (corpse, ch->in_room);
    }
  WAIT_STATE (ch, 4);
  return;

}

void
do_infest (CHAR_DATA * ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MSL];

  one_argument (argument, arg);

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_LEVEL] < 5)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }

  if (arg[0] == '\0')
    {
      if (ch->fighting == NULL)
	{
	  stc ("Infest who?\n\r", ch);
	  return;
	}
      victim = ch->fighting;
      if (IS_SET (victim->newbits2, NEW2_INFEST))
	{
	  stc ("They are already infested.\n\r", ch);
	  return;
	}
      if (IS_NPC (victim))
	{
	  stc ("Not on mobs.\n\r", ch);
	  return;
	}
      SET_BIT (victim->newbits2, NEW2_INFEST);
      act ("$n calls forth a hoard of zombies to restrain $N!", ch, NULL,
	   victim, TO_ROOM);
      act ("$n calls forth a hoard of zombies to restrain you!", ch, NULL,
	   victim, TO_VICT);
      act ("You call forth a hoard of zombies to restrain $N!", ch, NULL,
	   victim, TO_CHAR);
    }
  else
    {
      if ((victim = get_char_room (ch, NULL, arg)) == NULL)
	{
	  stc ("Infest who?\n\r", ch);
	  return;
	}
      if ((victim->fighting != NULL) && (victim->fighting->name != ch->name))
        {
	  if ((victim->pcdata->kingdom == 0) || (ch->pcdata->kingdom == 0))
	  {
	    stc ("They are fighting someone else.", ch);
	    return;
	  }
	}

      if (IS_SET (victim->newbits2, NEW2_INFEST))
	{
	  stc ("They are already infested.\n\r", ch);
	  return;
	}
      if (IS_NPC (victim))
	{
	  stc ("Not on mobs.\n\r", ch);
	  return;
	}
      SET_BIT (victim->newbits2, NEW2_INFEST);
      act ("$n calls forth a hoard of zombies to restrain $N!", ch, NULL,
	   victim, TO_ROOM);
      act ("$n calls forth a hoard of zombies to restrain you!", ch, NULL,
	   victim, TO_VICT);
      act ("You call forth a hoard of zombies to restrain $N!", ch, NULL,
	   victim, TO_CHAR);
    }

}

void
do_assimilate (CHAR_DATA * ch, char *argument)
{
  OBJ_DATA *corpse;
  char arg[MSL];
  char buf[MSL];

  one_argument (argument, arg);

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_LEVEL] < 6)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }

  if (arg[0] == '\0')
    {
      stc ("Assimilate which corpse?\n\r", ch);
      return;
    }

  if ((corpse = get_obj_room (ch, arg)) == NULL)
    {
      stc ("That corpse isnt here.\n\r", ch);
      return;
    }
  if (corpse->item_type != ITEM_CORPSE_NPC)
    {
      stc ("Only on mobile corpses.\n\r", ch);
      return;
    }
  sprintf (buf, "%s absorbs %s!", ch->pcdata->switchname,
	   corpse->short_descr);
  act (buf, ch, NULL, NULL, TO_ROOM);
  act (buf, ch, NULL, NULL, TO_CHAR);
  WAIT_STATE (ch, 12);
  ch->hit += number_range (7500, 8000);
  if (ch->hit > ch->max_hit)
    ch->hit = ch->max_hit;
  if (corpse != NULL)
    extract_obj (corpse);

}

void
do_store (CHAR_DATA * ch, char *argument)
{
  OBJ_DATA *corpse;
  char arg[MSL];
  char buf[MSL];

  one_argument (argument, arg);

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_LEVEL] < 6)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_ASSIM] > (ch->tier + 1) * 5)
    {
      stc ("Your body can hold no more corpses.\n\r", ch);
      return;
    }
  if (arg[0] == '\0')
    {
      stc ("Store which corpse?\n\r", ch);
      return;
    }

  if ((corpse = get_obj_room (ch, arg)) == NULL)
    {
      stc ("That corpse isnt here.\n\r", ch);
      return;
    }
  if (corpse->item_type != ITEM_CORPSE_NPC)
    {
      stc ("Only on mobile corpses.\n\r", ch);
      return;
    }
  sprintf (buf, "%s stores %s inside of $mself!", ch->pcdata->switchname,
	   corpse->short_descr);
  act (buf, ch, NULL, NULL, TO_ROOM);
  act (buf, ch, NULL, NULL, TO_CHAR);
  WAIT_STATE (ch, 4);
  ch->pcdata->powers[ZOM_ASSIM] += 1;
  if (corpse != NULL)
    extract_obj (corpse);

}

void do_zombieforsake (CHAR_DATA * ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MSL];
//  char buf[MSL];
  int i;

  one_argument (argument, arg);

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_LEVEL] < 6)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }

  if (arg[0] == '\0' && ch->fighting == NULL)
    {
      stc ("Forsake whom?\n\r", ch);
      return;
    }

  if (arg[0] != '\0')
    {
      if ((victim = get_char_room (ch, NULL,  arg)) == NULL)
	{
	  stc ("They are not here.\n\r", ch);
	  return;
	}
    }
  else
    {
      if ((victim = ch->fighting) == NULL)
	{
	  stc ("Forsake whom?\n\r", ch);
	  return;
	}
    }

  if (victim == ch)
    {
      stc ("Are you stupid?\n\r", ch);
      return;
    }
  stc ("Their life is forfeit...\n\r", ch);

  for (i = 0; i < ch->pcdata->powers[ZOM_ASSIM]; i += 1)
    {
      one_hit (ch, victim, -1, 0);
    }
  ch->pcdata->powers[ZOM_ASSIM] = 0;
  return;

}

void
do_shed (CHAR_DATA * ch, char *argument)
{

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }
  if (has_timer (ch))
    return;
  if (ch->pcdata->powers[ZOM_LEVEL] < 7)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }

  if (!IS_SET (ch->affected_by, AFF_ETHEREAL))
    {
      stc ("Your spirit now roams freely.\n\r", ch);
      SET_BIT (ch->affected_by, AFF_ETHEREAL);
    }
  else
    {
      stc ("You grow yourself a body from the earth.\n\r", ch);
      REMOVE_BIT (ch->affected_by, AFF_ETHEREAL);
    }
  WAIT_STATE (ch, 6);
}

void
do_haunt (CHAR_DATA * ch, char *argument)
{

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_LEVEL] < 7)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }

  if (!IS_IMMUNE (ch, IMM_SHIELDED))
    {
      send_to_char
	("Spirits haunt you, protecting you from other's scrying eyes.\n\r",
	 ch);
      SET_BIT (ch->immune, IMM_SHIELDED);
      return;
    }
  send_to_char ("You banish your protecting spirits.\n\r", ch);
  REMOVE_BIT (ch->immune, IMM_SHIELDED);

}

void
do_noxious (CHAR_DATA * ch, char *argument)
{

  CHAR_DATA *ich;
  int stun;

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      stc ("Huh?\n\r", ch);
      return;
    }
  if (ch->pcdata->powers[ZOM_LEVEL] < 7)
    {
      stc ("You are not a strong enough zombie for this.\n\r", ch);
      return;
    }

  act ("$n releases noxious fumes!", ch, NULL, NULL, TO_ROOM);
  act ("You release noxious fumes!", ch, NULL, NULL, TO_CHAR);
  for (ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room)
    {

      if (ich == ch || ich->trust > 6)
	continue;
      if (IS_NPC (ich))
	continue;

      stun = FALSE;
      if (IS_SET (ich->newbits2, NEW2_HAUNT))
	{
	  REMOVE_BIT (ich->newbits2, NEW2_HAUNT);
	  stun = TRUE;
	}
      if (IS_SET (ich->act, PLR_WIZINVIS))
	{
	  REMOVE_BIT (ich->act, PLR_WIZINVIS);
	  stun = TRUE;
	}
      if (IS_SET (ich->act, AFF_HIDE))
	{
	  REMOVE_BIT (ich->act, AFF_HIDE);
	  stun = TRUE;
	}
      if (IS_SET (ich->affected_by, AFF_SHIFT))
	{
	  REMOVE_BIT (ich->affected_by, AFF_SHIFT);
	  stun = TRUE;
	}
      if (IS_SET (ich->extra, EXTRA_EARTHMELD))
	{
	  REMOVE_BIT (ich->extra, EXTRA_EARTHMELD);
	  stun = TRUE;
	}
      if (!IS_AFFECTED (ch, AFF_SHADOWPLANE)
	  && IS_AFFECTED (ich, AFF_SHADOWPLANE))
	{
	  REMOVE_BIT (ich->affected_by, AFF_SHADOWPLANE);
	  stun = TRUE;
	}
      if (IS_AFFECTED (ich, AFF_PEACE))
	{
	  REMOVE_BIT (ich->affected_by, AFF_PEACE);
	  stun = TRUE;
	}
      if (IS_AFFECTED (ich, AFF_ETHEREAL))
	{
	  REMOVE_BIT (ich->affected_by, AFF_ETHEREAL);
	  stun = TRUE;
	}
      if (IS_SET (ich->newbits2, NEW2_SPECTER))
	{
	  REMOVE_BIT (ich->newbits2, NEW2_SPECTER);
	  stun = TRUE;
	}
      if (IS_SET (ich->newbits2, NEW2_HIDE))
	{
	  REMOVE_BIT (ich->newbits2, NEW2_HIDE);
	  stun = TRUE;
	}

      if (IS_CLASS (ich, CLASS_VAMPIRE) && IS_SET (ich->polyaff, POLY_MIST))
	{
	  do_change (ich, "human");
	  stun = TRUE;
	}
      if ((IS_CLASS (ich, CLASS_DROW))
	  && IS_SET (ich->newbits, NEW_DARKNESS))
	{
	  REMOVE_BIT (ich->newbits, NEW_DARKNESS);
	  REMOVE_BIT (ich->in_room->room_flags, ROOM_TOTAL_DARKNESS);
	  stun = TRUE;
	}
      if (stun == TRUE)
	{
	  WAIT_STATE (ich, 16);
	  act ("$n is stunned by the noxious fumes and becomes visibile!",
	       ich, NULL, NULL, TO_ROOM);
	  act ("You are stunned by the noxious fumes and become visibile!",
	       ich, NULL, NULL, TO_CHAR);
	}
    }

}

void
do_zomscry (CHAR_DATA * ch, char *argument)
{

  CHAR_DATA *victim;
  ROOM_INDEX_DATA *chroom;
  ROOM_INDEX_DATA *victimroom;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument (argument, arg);

  if (IS_NPC (ch))
    return;

  if (!IS_CLASS (ch, CLASS_ZOMBIE))
    {
      send_to_char ("Huh?\n\r", ch);
      return;
    }
  if (arg[0] == '\0')
    {
      send_to_char ("Scry on whom?\n\r", ch);
      return;
    }

  if ((victim = get_char_world (ch, arg)) == NULL)
    {
      send_to_char ("They aren't here.\n\r", ch);
      return;
    }

        if(ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_ASTRAL)){
                stc("Not in an astral room.\n\r",ch);
                return;
        }

  chroom = ch->in_room;
  victimroom = victim->in_room;

  char_from_room (ch);
  char_to_room (ch, victimroom);
  if (IS_AFFECTED (ch, AFF_SHADOWPLANE)
      && (!IS_AFFECTED (victim, AFF_SHADOWPLANE)))
    {
      REMOVE_BIT (ch->affected_by, AFF_SHADOWPLANE);
      do_look (ch, "auto");
      SET_BIT (ch->affected_by, AFF_SHADOWPLANE);
    }
  else if (!IS_AFFECTED (ch, AFF_SHADOWPLANE)
	   && (IS_AFFECTED (victim, AFF_SHADOWPLANE)))
    {
      SET_BIT (ch->affected_by, AFF_SHADOWPLANE);
      do_look (ch, "auto");
      REMOVE_BIT (ch->affected_by, AFF_SHADOWPLANE);
    }
  else
    do_look (ch, "auto");
  char_from_room (ch);
  char_to_room (ch, chroom);
  return;

}

void do_fleshbond(CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *victim;
        char arg[MIL];
        char arg2[MIL];
        char buf[MSL];
        int tamount = 0;
        argument = one_argument(argument,arg);
        argument = one_argument(argument,arg2);

        if(IS_NPC(ch)) return;
        if(!IS_CLASS(ch,CLASS_ZOMBIE)){
                stc("Huh?\n\r",ch);
                return;
        }
        if( (victim = get_char_room(ch, NULL, arg)) == NULL){
                stc("Who do you wish to transfer power to?\n\r",ch);
                return;
        }
        if( IS_NPC(victim)){
                stc("On an NPC?\n\r",ch);
                return;
        }
        if( !IS_CLASS(victim,CLASS_ZOMBIE)){
                stc("They are not a Zombie.\n\r",ch);
                return;
        }
        if( arg2[0] == '\0'){
                stc("How much power will you transfer? (1-1000000)\n\r",ch);
                return;
        }
        if( (tamount = atoi(arg2)) < 1 || tamount > 1000000){
                stc("You can only transfer 1 to 1,000,000 points of flesh.\n\r",ch);
                return;
        }
        if( (ch->pcdata->powers[ZOM_FLESH] - tamount) < 0){
                stc("You do not have enough power to do that.\n\r",ch);
                return;
        }
        sprintf(buf,"You transfer %d points of energy to %s.\n\r",tamount,victim->pcdata->switchname);
        stc(buf,ch);
        sprintf(buf,"%s transfers %d points of energy to you.\n\r",ch->pcdata->switchname,tamount);
        stc(buf,victim);
        ch->pcdata->powers[ZOM_FLESH] -= tamount;
        victim->pcdata->powers[ZOM_FLESH] += tamount;
        stc(buf,ch);
        stc(buf,victim);

}

