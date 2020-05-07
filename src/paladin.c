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

/*********************************************
 * Paladin, class for cotn by Akurei         *
 *********************************************/

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


void do_retribution( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj;

  one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
  {
    if (ch->fighting == NULL) 
    {
      send_to_char( "They aren't here.\n\r", ch );
      return;
    }
    else victim = ch->fighting;
  }
  if (ch->pcdata->powers[PALADIN_POWER] < 2)
  {
    send_to_char("You need level 2 in power before you can use retribution.\n\r",ch);
    return;
  }
  if ( victim == ch )
  {
    send_to_char( "You may not bring down your Retribution upon yourself.\n\r", ch );
    return;
  }
  if (is_safe( ch, victim )) return;

  WAIT_STATE( ch, 12 );

    one_hit(ch, victim, gsn_retribution, 1);
    one_hit(ch, victim, gsn_retribution, 1);
    one_hit(ch, victim, gsn_retribution, 1);
    one_hit(ch, victim, gsn_retribution, 1);

  return;
}

void do_enlighten(CHAR_DATA *ch, char *argument)
{
  int cost;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    sprintf(buf, "Learn what? [Combat(%d) Speed(%d) Power(%d) Toughness(%d)]\n\r",
      ch->pcdata->powers[PALADIN_COMBAT], ch->pcdata->powers[PALADIN_SPEED],
      ch->pcdata->powers[PALADIN_POWER], ch->pcdata->powers[PALADIN_TOUGHNESS]);
    stc(buf,ch);
    return;
  }
  if (!str_cmp(arg, "combat"))
  {
    cost = (ch->pcdata->powers[PALADIN_COMBAT]+1)*150;
    if (ch->pcdata->powers[PALADIN_COMBAT] > 4)
    {
      send_to_char("You already have all 5 levels of combat.\n\r",ch);
      return;
    }
    if (ch->practice < cost)
    {
      sprintf(buf,"You require %d primal.\n\r",cost);
      stc(buf,ch);
      return;
    }
    ch->pcdata->powers[PALADIN_COMBAT]++;
    ch->practice -= cost;
    send_to_char("You further your understanding of combat.\n\r",ch);
  }
  else if (!str_cmp(arg, "speed"))
  {
    cost = (ch->pcdata->powers[PALADIN_SPEED]+1)*150;
    if (ch->pcdata->powers[PALADIN_SPEED] > 4)
    {
      send_to_char("You already have all 5 levels of speed.\n\r",ch);
      return;
    }
    if (ch->practice < cost)
    {
      sprintf(buf,"You require %d primal.\n\r",cost);
      stc(buf,ch);
      return;
    }
    ch->pcdata->powers[PALADIN_SPEED]++;
    ch->practice -= cost;
    send_to_char("Your speed has improved.\n\r",ch);
  }
  else if (!str_cmp(arg, "toughness"))
  {
    cost = (ch->pcdata->powers[PALADIN_TOUGHNESS]+1)*150;
    if (ch->pcdata->powers[PALADIN_TOUGHNESS] > 4)
    {
      send_to_char("You already have all 5 levels of Toughness.\n\r",ch);
      return;
    }
    if (ch->practice < cost)
    {
      sprintf(buf,"You require %d primal.\n\r",cost);
      stc(buf,ch);
      return;
    }
    ch->pcdata->powers[PALADIN_TOUGHNESS]++;
    ch->practice -= cost;
    send_to_char("Your body becomes more solid.\n\r",ch);
  }
  else if (!str_cmp(arg, "power"))
  {
    cost = (ch->pcdata->powers[PALADIN_POWER]+1)*150;
    if (ch->pcdata->powers[PALADIN_POWER] > 9)
    {
      send_to_char("You already have all 10 levels of power.\n\r",ch);
      return;
    }
    if (ch->practice < cost)
    {
      sprintf(buf,"You require %d primal.\n\r",cost);
      stc(buf,ch);
      return;
    }
    ch->pcdata->powers[PALADIN_POWER]++;
    ch->practice -= cost;
    send_to_char("Your muscles tighten.\n\r",ch);
  }
  else do_enlighten(ch,"");
  return;
}

void do_pholyaura( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[PALADIN_POWER] < 7)
  {
    send_to_char("You are not a strong enough Paladin for this.\n\r",ch);
    return;
  }
  if (IS_SET(ch->pcdata->powers[PALADIN_EFFECTS], PAL_AURA))
  {
    REMOVE_BIT(ch->pcdata->powers[PALADIN_EFFECTS], PAL_AURA);
    act("Your aura fades.", ch, NULL, NULL, TO_CHAR);
    act("$n looks less powerful.", ch, NULL, NULL, TO_ROOM);
    return;
  }
  SET_BIT(ch->pcdata->powers[PALADIN_EFFECTS], PAL_AURA);
  act("Your aura flickers into existance.", ch, NULL, NULL, TO_CHAR);
  act("$n' Paladin aura radiates around $m.", ch, NULL, NULL, TO_ROOM);
  return;
}

void do_pmight(CHAR_DATA *ch, char *argument)
{

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( ch->pcdata->powers[PALADIN_POWER] < 4 )
  {
    stc("You are not yet powerful enough.\n\r",ch);
    return;
  }
  if (IS_SET(ch->newbits, NEW_CUBEFORM))
  {
    REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
    ch->hitroll -= 500;
    ch->damroll -= 500;
    stc("Your Paladin Might leaves you.\n\r",ch);
    return;
  }
  SET_BIT(ch->newbits, NEW_CUBEFORM);
  ch->hitroll += 500;
  ch->damroll += 500;
  stc("Your body is filled with Paladin Might.\n\r",ch);
}

void do_pwrath(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  int dam;
  char buf[MSL];

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( ch->pcdata->powers[PALADIN_COMBAT] < 5 )
  {
    stc("Your understanding of combat is not great enough.\n\r",ch);
    return;
  }
  if ( (victim = get_char_room(ch, NULL, argument)) == NULL )
  if ( (victim = ch->fighting) == NULL )
  {
    stc("Who do you wish to attack?\n\r",ch);
    return;
  }
  if ( is_safe(ch,victim) ) return;
  dam = number_range(ch->pcdata->powers[PALADIN_POWER] * 1100, ch->pcdata->powers[PALADIN_POWER] * 1300);
  sprintf(buf, "%s's Holy Wrath wrecks your body [%d]\n\r",ch->name,dam);
  stc(buf,victim);
  sprintf(buf, "Your Holy Wrath wrecks %s's body [%d]\n\r",victim->name,dam);
  stc(buf,ch);
  sprintf(buf, "$n's Holy Wrath wrecks $N's body [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_NOTVICT);
  WAIT_STATE(ch, 8);
  hurt_person(ch, victim, dam);
  set_fighting(ch,victim, TRUE);
  set_fighting(victim,ch, TRUE);
}

void do_pblade(CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  char arg[MSL];
  int oldlev;

  argument = one_argument(argument,arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( ch->pcdata->powers[PALADIN_COMBAT] < 5 )
  {
    stc("Your understanding of combat is not great enough.\n\r",ch);
    return;
  }
  if ( (obj = get_obj_carry(ch,arg, ch)) == NULL )
  {
    stc("You aren't carrying that.\n\r",ch);
    return;
  }
/*
  if ( obj->pIndexData->vnum != 94068 )
  {
    stc("Only on Paladin weapons.\n\r",ch);
    return;
  }
*/
  if ( IS_SET(obj->quest2, QUEST2_PALADINBLESS) )
  {
    stc("That weapon has already been blessed.\n\r",ch);
    return;
  }
  if ( ch->practice < 100 )
  {
    stc("You require 100 primal.\n\r",ch);
    return;
  }
  SET_BIT(obj->quest2, QUEST2_PALADINBLESS);
  oldlev = ch->level;
  ch->level = 12;
  oset_affect(ch,obj,200,APPLY_HITROLL,TRUE);
  oset_affect(ch,obj,200,APPLY_DAMROLL,TRUE);
  ch->level = oldlev;
  obj->value[1] += 20;
  obj->value[2] += 20;
  obj->points -= 12000;
  ch->practice -= 100;
  act("You infuse $p with Paladin Might.",ch,obj,NULL,TO_CHAR);
  act("$n infuses $p with Paladin Might.",ch,obj,NULL,TO_ROOM);
}

void do_barmor(CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  char arg[MSL];
  int oldlev;

  argument = one_argument(argument,arg);

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( ch->pcdata->powers[PALADIN_COMBAT] < 5 )
  {
    stc("Your understanding of combat is not great enough.\n\r",ch);
    return;
  }
  if ( (obj = get_obj_carry(ch,arg, ch)) == NULL )
  {
    stc("You aren't carrying that.\n\r",ch);
    return;
  }
/*
  if ( (obj->pIndexData->vnum < 94056 || obj->pIndexData->vnum > 94067) && obj->pIndexData->vnum != 94069 )
  {
    stc("Only on Paladin armor.\n\r",ch);
    return;
  }
*/
  if ( IS_SET(obj->quest2, QUEST2_PALADINBLESS) )
  {
    stc("That armor has already been blessed.\n\r",ch);
    return;
  }
  if ( ch->practice < 100 )
  {
    stc("You require 100 primal.\n\r",ch);
    return;
  }
  SET_BIT(obj->quest2, QUEST2_PALADINBLESS);
  oldlev = ch->level;
  ch->level = 12;
  oset_affect(ch,obj,-50,APPLY_AC,TRUE);
  obj->points -= 500;
  ch->level = oldlev;
  ch->practice -= 100;
  act("You bless $p with Paladin Strength.",ch,obj,NULL,TO_CHAR);
  act("$n blesses $p with Paladin Strength.",ch,obj,NULL,TO_ROOM);
}

void do_paladinpractice(CHAR_DATA *ch, char *argument)
{
  int cost;
  char buf[MSL];

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( ch->pcdata->powers[PALADIN_PRACTICE] > 4 )
  {
    stc("Your skills have reached their pinacle.\n\r",ch);
    return;
  }
  cost = (ch->pcdata->powers[PALADIN_PRACTICE] + 1) * 100;
  if ( ch->practice < cost )
  {
    sprintf(buf, "You require %d primal.\n\r",cost);
    stc(buf,ch);
    return;
  }
  if ( ch->pcdata->powers[PALADIN_COMBAT] <= ch->pcdata->powers[PALADIN_PRACTICE] )
  {
    stc("Your understanding of combat is not great enough.\n\r",ch);
    return;
  }
  ch->pcdata->powers[PALADIN_PRACTICE]++;
  ch->practice -= cost;
  stc("Your skills have improved.\n\r",ch);
}

void do_holyvision(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if ( !IS_CLASS(ch, CLASS_PALADIN) )
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if (IS_SET(ch->act, PLR_HOLYLIGHT))
  {
    REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
    send_to_char("The world becomes darker.\n\r", ch);
  }
  else
  {
    SET_BIT(ch->act, PLR_HOLYLIGHT);
    send_to_char( "The world becomes brighter.\n\r", ch );
  }
}

void do_paladinleap(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;

  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( ch->pcdata->powers[PALADIN_POWER] < 3 )
  {
    stc("Your Paladin strength is not great enough.\n\r",ch);
    return;
  }
  if ( ch->move < 1000 )
  {
    stc("You are too tired.\n\r",ch);
    return;
  }
  if ( argument[0] == '\0' )
  {
    stc("Leap to whom?\n\r",ch);
    return;
  }
  if ( (victim = get_char_world(ch,argument)) == NULL )
  {
    stc("They are not here.\n\r",ch);
    return;
  }
  if ( victim == ch )
  {
    stc("You leap high into the air, and land in the same spot.\n\r",ch);
    return;
  }
  if ( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    stc("You can't find its room.\n\r",ch);
    return;
  }
  if ( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc("Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  if (IS_IMMUNE(victim, IMM_TRAVEL) && !IS_NPC(victim))
  {
    send_to_char("I don't think they want you to do that.\n\r",ch);
    return;
  }
  if ( room_is_private(victim->in_room ) )
  {
    send_to_char( "That room is private right now.\n\r", ch );
    return;
  }
  if (victim->in_room->vnum == ch->in_room->vnum)
  {
    send_to_char("But you're already there!\n\r",ch);
    return;
  }
  act("$n leaps high into the air.",ch,NULL,NULL,TO_ROOM);
  char_from_room(ch);
  char_to_room(ch,victim->in_room);
  act("You leap high into the air and land beside $N.",ch,NULL,victim,TO_CHAR);
  act("$n lands beside you with a loud crash.",ch,NULL,victim,TO_VICT);
  act("$n lands beside $N with a loud crash.",ch,NULL,victim,TO_NOTVICT);
  ch->move -= 1000;
  WAIT_STATE(ch, 5);
  do_look(ch,"auto");
}

void do_righteous(CHAR_DATA *ch, char *argument)
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    stc("Huh?\n\r",ch);
    return;
  }
  if ( ch->pcdata->powers[PALADIN_COMBAT] < 4 )
  {
    stc("Your knowledge of combat is not great enough.\n\r",ch);
    return;
  }
  if ( IS_IMMUNE(ch, IMM_SHIELDED) )
  {
    stc("Your Paladin aura stops protecting you.\n\r",ch);
    REMOVE_BIT(ch->immune, IMM_SHIELDED);
  }
  else
  {
    stc("You raise your Paladin might to protect you.\n\r",ch);
    SET_BIT(ch->immune, IMM_SHIELDED);
  }
}


void do_pbash(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  AFFECT_DATA af;
  CHAR_DATA *ich;
  CHAR_DATA *ich_next;
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  
  argument = one_argument( argument, arg );
  argument = one_argument( argument, arg2 );
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_PALADIN))
  {
    send_to_char("Huh.\n\r",ch);
    return;
  }
    if (!(arg2[0] == '\0') || ch->fighting != NULL)
    {
      if (arg2[0] == '\0') victim = ch->fighting;
      else if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
      {
        send_to_char("They are not here.\n\r", ch );
        return;
      }
      if (is_safe(ch, victim)) return;
      WAIT_STATE(ch, 18);

  act("You take a step back, then launch yourself straight at $N...",ch,NULL,victim,TO_CHAR);
  act("$n takes a step back, then launches $mself straight at you...",ch,NULL,victim,TO_VICT);
  act("$n takes a step back, then launches $mself straight at $N...",ch,NULL,victim,TO_NOTVICT);

  if ( number_range(1,3) == 3 )
  {
    act("$N sidesteps, leaving you wide open!",ch,NULL,victim,TO_CHAR);
    act("You sidestep, leaving $n wide open!",ch,NULL,victim,TO_VICT);
    act("$N sidesteps, leaving $n wide open!",ch,NULL,victim,TO_NOTVICT);
    if ( number_range(1,2) == 1 )
    {
      act("$N takes the opportunity to strike back!",ch,NULL,victim,TO_CHAR);
      act("You take the opportunity to strike back!",ch,NULL,victim,TO_VICT);
      act("$N takes the opportunity to strike back!",ch,NULL,victim,TO_NOTVICT);
      multi_hit(victim,ch,TYPE_UNDEFINED);
    }
    else
    {
      act("You recover yourself before $N can strike back.",ch,NULL,victim,TO_CHAR);
      act("$n recovers $mself before you can strike back.",ch,NULL,victim,TO_VICT);
      act("$n recovers $mself before $N can strike back.",ch,NULL,victim,TO_NOTVICT);
    }
    return;
  }
      WAIT_STATE(victim, 24);
      stop_fighting(ch, victim);
      return;
    }
  }
