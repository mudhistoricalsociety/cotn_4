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
 * Elemental, another class by Xrakisis       *
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


     void do_elemshift (CHAR_DATA *ch, char *argument)
	{
  	char arg[MAX_INPUT_LENGTH];
  	char buf[MAX_STRING_LENGTH];
  	one_argument( argument, arg);

  	if (IS_NPC(ch)) return;
  	if (!IS_CLASS(ch, CLASS_ELEMENTAL))
    	{
        send_to_char("Elemental Only?\n\r",ch);
        return;
    	}
  	if (ch->pcdata->powers[ELEMENTAL_FIRE] < 5 )
  	{
    	send_to_char("#7You need level #R5 #7in each element to use #RElemshift#7!#n\n\r",ch);
    	return;
  	}
  	if (ch->pcdata->powers[ELEMENTAL_AIR] < 5 )
  	{
    	send_to_char("#7You need level #R5 #7in each element to use #RElemshift#7!#n\n\r",ch);
    	return;
  	}
  	if (ch->pcdata->powers[ELEMENTAL_WATER] < 5 )
  	{
    	send_to_char("#7You need level #R5 #7in each element to use #RElemshift#7!#n\n\r",ch);
    	return;
  	}
  	if (ch->pcdata->powers[ELEMENTAL_EARTH] < 5 )
  	{
    	send_to_char("#7You need level #R5 #7in each element to use #RElemshift#7!#n\n\r",ch);
    	return;
  	}
  	if (arg[0] == '\0')
  	{
    	send_to_char("#7Which #RElemental #7form? #7[#RFire#7-#CAir#7-#LWater#7-#oEarth#7-#YMortal#7]#n\n\r", ch);
    	return;
  	}
  	if (!str_cmp(arg,"fire"))
  	{
    	if (IS_SET(ch->affected_by, AFF_POLYMORPH))
    	{
      	send_to_char("#7You're already in a #RELEMENTAL #7form#7!#n\n\r",ch);
      	return;
    	}
    	if (ch->mana < 10000)
  	{
    	send_to_char("#7You don't have enough #Rmana#7.#n\n\r",ch);
    	return;
  	}
    	act( "You become ingulfed in #RF#Yl#Ra#Ym#Re#Ys#n.", ch, NULL, NULL, TO_CHAR);
    	act( "$n's Becomes a #7Human #YT#Ro#Yr#Rc#Yh#n.", ch, NULL, NULL, TO_ROOM );
    	ch->pcdata->powers[ELEM_FORMS]=FIRE_FORM;
    	SET_BIT(ch->affected_by, AFF_POLYMORPH);
    	ch->hitroll += 400;
    	ch->damroll += 400;
    	ch->armor   -= 200;
    	ch->mana    -= 10000;
    	sprintf(buf, "%s The #RF#Yi#Rr#Ye#n Elemental", ch->name);
    	free_string(ch->morph);
    	ch->morph = str_dup(buf);
    	return;
  	}
  	else if (!str_cmp(arg,"water"))
  	{
    	if (IS_SET(ch->affected_by, AFF_POLYMORPH))
    	{
      	send_to_char("#7You're already in a #RELEMENTAL #7form#7!#n\n\r",ch);
      	return;
    	}
    	if (ch->mana < 1000)
  	{
    	send_to_char("#7You don't have enough #Rmana#7.#n\n\r",ch);
    	return;
  	}
    	act( "Your skin is covered with #7Ice #lCrystals#n.", ch, NULL, NULL, TO_CHAR);
    	act( "$n's skin becomes covered with #7Ice #lcrystals#n.", ch, NULL, NULL, TO_ROOM );
    	ch->pcdata->powers[ELEM_FORMS]=WATER_FORM;
    	SET_BIT(ch->affected_by, AFF_POLYMORPH);
    	ch->hitroll += 300;
    	ch->damroll += 300;
    	ch->mana    -= 10000;
    	sprintf(buf, "%s The #lWater#n Elemental", ch->name);
    	free_string(ch->morph);
    	ch->morph = str_dup(buf);
    	return;
  	}
  	else if (!str_cmp(arg,"air"))
  	{
    	if (IS_SET(ch->affected_by, AFF_POLYMORPH))
    	{
      	send_to_char("#7You're already in a #RELEMENTAL #7form#7!#n\n\r",ch);
      	return;
    	}
    	if (ch->mana < 10000)
  	{
    	send_to_char("#7You don't have enough #Rmana#7.#n\n\r",ch);
    	return;
  	}
    	act( "You becomes a thin #LM#7i#Ls#7t#n.", ch, NULL, NULL, TO_CHAR);
    	act( "$n's Becomes a thin #LM#7i#Ls#7t#n.", ch, NULL, NULL, TO_ROOM );
    	ch->pcdata->powers[ELEM_FORMS]=AIR_FORM;
    	SET_BIT(ch->affected_by, AFF_POLYMORPH);
    	ch->hitroll += 200;
    	ch->damroll += 200;
    	ch->armor   -= 200;
    	ch->mana    -= 10000;
    	sprintf(buf, "%s The #7Air#n Elemental", ch->name);
    	free_string(ch->morph);
    	ch->morph = str_dup(buf);
    	return;
  	}
  	else if (!str_cmp(arg,"earth"))
  	{
    	if (IS_SET(ch->affected_by, AFF_POLYMORPH))
    	{
      	send_to_char("#7You're already in a #RELEMENTAL #7form#7!#n\n\r",ch);
      	return;
    	}
    	if (ch->mana < 10000)
  	{
    	send_to_char("#7You don't have enough #Rmana#7.#n\n\r",ch);
    	return;
  	}
    	act( "Your Skin turns to Hard #GE#garth#n.", ch, NULL, NULL, TO_CHAR);
    	act( "$n's skin turns to Hard #GE#garth#n.", ch, NULL, NULL, TO_ROOM );
    	ch->pcdata->powers[ELEM_FORMS]=EARTH_FORM;
    	SET_BIT(ch->affected_by, AFF_POLYMORPH);
    	ch->hitroll += 500;
   	ch->damroll += 500;
    	ch->armor   -= 500;
    	ch->mana    -= 10000;
    	sprintf(buf, "%s The #GE#garth#n Elemental", ch->name);
    	free_string(ch->morph);
    	ch->morph = str_dup(buf);
    	return;
  	}
  	else if (!str_cmp(arg,"mortal"))
  	{
    	if (!IS_SET(ch->affected_by, AFF_POLYMORPH))
    	{
      	send_to_char("#7Your already in #RMortal #7form.#n\n\r",ch);
      	return;
   	}
    	if (IS_SET(ch->pcdata->powers[ELEM_FORMS], FIRE_FORM))
    	{
      	ch->hitroll -= 400;
      	ch->damroll -= 400;
      	ch->armor   += 200;
    	}
    	else if (IS_SET(ch->pcdata->powers[ELEM_FORMS], AIR_FORM))
    	{
      	ch->hitroll -= 200;
      	ch->damroll -= 200;
      	ch->armor   += 200;
    	}
    	else if (IS_SET(ch->pcdata->powers[ELEM_FORMS], WATER_FORM))
    	{
      	ch->hitroll -= 300;
      	ch->damroll -= 300;
  	}
    	else if (IS_SET(ch->pcdata->powers[ELEM_FORMS], EARTH_FORM))
    	{
      	ch->hitroll -= 500;
      	ch->damroll -= 500;
      	ch->armor   += 500;
  	}
        ch->pcdata->powers[ELEM_FORMS]=0;
     	REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
        send_to_char("#7You return to your #RMortal #7form.#n\n\r",ch);
        act("#7$n returns to $s #RMortal #7form.#n\n\r",ch,NULL,NULL,TO_ROOM);
        free_string(ch->morph);
        ch->morph=str_dup("");
        return;
  	}
  	else
  	{
    	send_to_char("#7That is not a valid form.#n\n\r",ch);
    	return;
  	}
	}


void do_solidify(CHAR_DATA *ch,char *argument)
{
  if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch,CLASS_ELEMENTAL))
  {
  send_to_char("Huh?\n\r",ch);
  return;
  }
  if (!IS_SET(ch->newbits, NEW_SKIN))
  {
  send_to_char("Your Earthen Skin Hardens.\n\r",ch);
  ch->armor -= 100 ;
  SET_BIT(ch->newbits, NEW_SKIN);
  return;
  }
  else if (IS_SET(ch->newbits, NEW_SKIN))
  { 
  send_to_char("Your Earthen Skin Softens.\n\r",ch);
  ch->armor += 100;
  REMOVE_BIT(ch->newbits,NEW_SKIN);
  return;
  }
return;
}



void do_enflame( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ELEMENTAL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ELEMENTAL_FIRE] < 5)
  {
    send_to_char("You Need 5 in fire to use enflame.\n\r",ch);
    return;
  }   
  if (IS_SET(ch->pcdata->powers[ELEMENTAL_POWERS], ELEMENTAL_ENFLAME))
  {
    REMOVE_BIT(ch->pcdata->powers[ELEMENTAL_POWERS], ELEMENTAL_ENFLAME);
    act("You Will No Longer Enflame.", ch, NULL, NULL, TO_CHAR);
    act("$n's Will No Longer Enflame.", ch, NULL, NULL, TO_ROOM);
    return;
  }
  SET_BIT(ch->pcdata->powers[ELEMENTAL_POWERS], ELEMENTAL_ENFLAME);
  act("#CYou Will Now Use Your FIRE to Seek Retribution.#n", ch, NULL, NULL, TO_CHAR);
  act("$n Will now use $s Fire to seek RETRIBUTION.", ch, NULL, NULL, TO_ROOM);
  return;
}

void do_magma( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  int dam;


  if (IS_NPC(ch)) return;
 
  if (!IS_CLASS(ch, CLASS_ELEMENTAL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ELEMENTAL_FIRE] < 3 )
  {
    send_to_char("You need level 3 in Fire to use MAGMA.\n\r",ch);
    return;
  }
  if ( ( victim = get_char_room( ch, NULL,  arg ) ) == NULL )
  {
    if (ch->fighting == NULL) 
    {
      send_to_char( "They aren't here.\n\r", ch );
      return;
    }
    else victim = ch->fighting;
     
  }

  WAIT_STATE( ch, 12 );

  if (!IS_NPC(victim)) dam = number_range(4000,8000);
  else dam = number_range(15000,25000);

  sprintf(buf,"Your Magma strikes $N with DEVESTATING FORCE! [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_CHAR);
  sprintf(buf,"$n's Magma strikes you with DEVESTATING FORCE! [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_VICT);
  sprintf(buf,"$n's Magma strikes $N with DEVESTATING FORCE! [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_NOTVICT); 
  hurt_person(ch,victim, dam);
  return;
}
 
void do_naturesfury (CHAR_DATA *ch, char *argument)
{
//  char arg1[MAX_STRING_LENGTH];
//  char arg2[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
//  AFFECT_DATA af;
//  int i, dam, count, sn;
  int dam;
  int red_magic = ch->spl[RED_MAGIC];
  int blue_magic = ch->spl[BLUE_MAGIC];
  int green_magic = ch->spl[GREEN_MAGIC];
  int purple_magic = ch->spl[PURPLE_MAGIC];
  int yellow_magic = ch->spl[YELLOW_MAGIC];
//  int fire = ch->pcdata->powers[ELEMENTAL_FIRE];
//  int water = ch->pcdata->powers[ELEMENTAL_WATER];
//  int air = ch->pcdata->powers[ELEMENTAL_AIR];
//  int earth = ch->pcdata->powers[ELEMENTAL_EARTH];

  int magic_power = ch->spl[RED_MAGIC] + ch->spl[BLUE_MAGIC] + ch->spl[GREEN_MAGIC] + ch->spl[PURPLE_MAGIC] + ch->spl[YELLOW_MAGIC];
//    int elem_power = ch->pcdata->powers[ELEMENTAL_EARTH] + ch->pcdata->powers[ELEMENTAL_FIRRE]
///  argument=one_argument(argument,arg1);
//  strcpy(arg2,argument);

  one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ELEMENTAL))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
//  if (arg1[0] == '\0')
//  {  
//    send_to_char("Syntax : chant <bless/curse/damage/heal> <target>.\n\r", ch);
//    return;
//  }  
  if (IS_ITEMAFF(ch, ITEMA_AFFENTROPY)) 
  {
    magic_power +=100;
    red_magic +=20;
    yellow_magic +=20;
    purple_magic +=20;
    green_magic +=20;
    blue_magic +=20;
  }
/*      if (ch->pcdata->powers[POWER_TICK] > 0)
      {
        send_to_char("You cannot use Naturefury yet..\n\r",ch);
        return;
      }
*/
//  if (!str_cmp(arg1, "damage"))
//  {
//    if (arg2[0] == '\0' && ch->fighting != NULL) victim = ch->fighting;
    
  if (ch->pcdata->powers[ELEMENTAL_FIRE] < 3)
  {
    send_to_char("You need to get Fire up to 3 to use this.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ELEMENTAL_EARTH] < 3)
  {
    send_to_char("You need to get Earth up to 3 to use this.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ELEMENTAL_AIR] < 3)
  {
    send_to_char("You need to get Air up to 3 to use this.\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[ELEMENTAL_WATER] < 3)
  {
    send_to_char("You need to get Water up to 3 to use this.\n\r",ch);
    return;
  }

  if ((victim = get_char_room(ch, NULL, arg)) == NULL)
  {
    send_to_char("They are not here.\n\r", ch);
    return;
  }
    if (victim == ch)
    {
      send_to_char("You really don't want to hurt yourself.\n\r", ch );
      return;
    }
    if (is_safe(ch, victim)) return;
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
      return;
    if (IS_AFFECTED(ch, AFF_PEACE)) REMOVE_BIT(ch->affected_by, AFF_PEACE);
    if (victim->position > POS_STUNNED)
    {
      if (victim->fighting == NULL) set_fighting(victim, ch, TRUE);
      if (ch->fighting == NULL) set_fighting(ch, victim, TRUE);
    }
    WAIT_STATE(ch,8);
    dam = number_range(red_magic*10, red_magic*15); // was 10 and 15
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    dam = dam * 8;

    if (!IS_NPC(victim))
    {
      if (IS_CLASS(ch, CLASS_ELEMENTAL) && dam > 10) dam = number_range(2000,4500);
      if (dam > 1000) dam = number_range(4500,6500); // 45 and 65
//      ch->pcdata->powers[POWER_TICK] = 2; 
    }
    sprintf(buf1,"$n calls upon the forces of Nature to use #rFIRE#n to destroy $N [#C%d#n]" , dam);
    sprintf(buf2,"You call for the forces of Nature to use #rFIRE#n to destroy $N [#C%d#n]" , dam);
    sprintf(buf3,"$n calls for the forces of Nature to use#rFIRE#n to destroy you [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(blue_magic*4, blue_magic*5); // 4 and 5
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    if (!IS_NPC(victim))
    {
      if (IS_CLASS(ch, CLASS_ELEMENTAL) && dam > 600) dam = number_range(2000,4500);
      if (dam > 1000) dam = number_range(3500,5500);
    }
    sprintf(buf1,"$n calls up the power of #GEARTH#n to attack $N with boulders [#C%d#n]" , dam);
    sprintf(buf2,"You call upon the power of #GEARTH to attack $N with boulders [#C%d#n]" , dam);
    sprintf(buf3,"$n calls upon the power of #GEARTH to attack you with boulders [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(green_magic*10, green_magic*15);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    dam = dam * 8;
    if (!IS_NPC(victim))
    {
      if (IS_CLASS(ch, CLASS_ELEMENTAL) && dam > 600) dam = number_range(2000,5500);
      if (dam > 1000) dam = number_range(2500,4500);
    }
    sprintf(buf1,"$n calls for forth the power of #7AIR#n to strike $N [#C%d#n]" , dam);
    sprintf(buf2,"You call forth the power of #7AIR#n to strike $N [#C%d#n]" , dam);
    sprintf(buf3,"$n calls forth the power of #7AIR#n to strike You with DEVESTATING FORCE! [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(purple_magic*10, purple_magic*15);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    dam = dam * 8;
    if (!IS_NPC(victim))
    {
      if (IS_CLASS(ch, CLASS_ELEMENTAL) && dam > 600) dam = number_range(2000,4500);
      if (dam > 1000) dam = number_range(4500,6500);
    }
    sprintf(buf1,"$n calls for forth the power of #lWATER#n to strike $N [#C%d#n]" , dam);
    sprintf(buf2,"You call forth the power of #lWATER#n to strike $N [#C%d#n]" , dam);
    sprintf(buf3,"$n calls forth the power of #lWATER#n ATONOMIZING You! [#C%d#n]" , dam);
    act(buf1,ch,NULL,victim, TO_NOTVICT);
    act(buf2,ch,NULL,victim, TO_CHAR);
    act(buf3,ch,NULL,victim, TO_VICT);
    hurt_person(ch, victim, dam);
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room) return;
    dam = number_range(yellow_magic*10, yellow_magic*15);
    dam += char_damroll(ch);
    dam = cap_dam(ch,victim,dam);
    dam = dam * 8;
//    ch->pcdata->powers[POWER_TICK] = 2; 
   return;
  }

void do_disperce ( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  argument = one_argument(argument, arg);
  
  if (!IS_CLASS(ch, CLASS_ELEMENTAL))
  {
    send_to_char("Only Elementals May Disperce.\n\r",ch);
    return;
  }
  if (ch->move < 600)
  {
    stc("You do not have enough vitality to do that!\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    send_to_char( "Travel to Who?\n\r", ch );
    return;
  }
  if ( ( victim = get_char_world( ch, arg ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }   
  if( victim == ch )
  {
    stc ( "Not to yourself.\n\r",ch);
    return;
  }
  if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "You can't find it's room.\n\r",ch); 
    return;
  }
  if( IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  if  (!IS_NPC(victim) && !IS_IMMUNE(victim,IMM_SUMMON) )
  {
    send_to_char("They don't want you near them!\n\r",ch);
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
  char_from_room(ch);
  char_to_room(ch,victim->in_room);
  do_look(ch, "auto");
  if (IS_NPC(victim))
  {
    sprintf(buf,"You Disperce into particles to travel to %s!\n\r",victim->short_descr);
    send_to_char(buf, ch);
  }
  if (!IS_NPC(victim))
  {
    sprintf(buf,"Disperces into particles to travel to %s!\n\r",victim->name);
    send_to_char(buf, ch);
  }
  act("You see $n slowly solidify infront of you.\n\r",ch,NULL,NULL,TO_ROOM);
  ch->move -= 600;
  return;
}

void do_enfold( CHAR_DATA *ch, char *argument )
  {
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_ELEMENTAL))
  {
    send_to_char("Only Elementals may use ENFOLD.\n\r",ch);
    return;
  }  
    if (ch->pcdata->powers[ELEMENTAL_AIR] < 3)
    {
      send_to_char("You Need At Least 3 In Air to do this.\n\r",ch);
      return;
    }
    if (IS_SET(ch->pcdata->powers[ELEMENTAL_POWERS], ELEMENTAL_ENFOLD))
    {
      REMOVE_BIT(ch->pcdata->powers[ELEMENTAL_POWERS], ELEMENTAL_ENFOLD);
      send_to_char("Your Air Particles return to your body allowing people to escape.\n\r",ch);
      return;
    }
    else
    {
      send_to_char("You release air particles to keep your enemys from fleeing.\n\r",ch);
      SET_BIT(ch->pcdata->powers[ELEMENTAL_POWERS], ELEMENTAL_ENFOLD);
      return;
    }
  }

void do_hydro(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

  if (!IS_CLASS(ch, CLASS_ELEMENTAL))
    {
      send_to_char("Huh.\n\r",ch);
      return;
    }
/*
  if(is_safe(ch,victim) == TRUE)
  {
  stc("#0Not in a safe room dumbass#R.#n\n\r",ch);
  return;
  }
*/  
  if (ch->pcdata->powers[ELEMENTAL_WATER] < 3 )
  {
    send_to_char("You need level 3 in WATER to use Hydro.\n\r",ch);
    return;
  }
    if (argument[0] == '\0')
    {
	stc("Who do you wish to unstance?\n\r", ch);
	return;
    }

    if ( ( victim = get_char_room(ch, NULL, argument) ) == NULL)
    {
	stc("They aren't here.\n\r", ch);
	return;
    }

    WAIT_STATE(ch, 12);
  if ( number_range(1,3) <= 2 )
{
    WAIT_STATE(ch, 4);
    do_say(ch,"#rEat Shit and Die!#n");
    do_stance(victim, "");
    set_fighting(ch, victim, TRUE);

}
}


void do_evaporate( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_ELEMENTAL) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	
	if (ch->pcdata->powers[ELEMENTAL_WATER] < 5) 
	{
	  send_to_char("You need level 5 in water to evaporate.\n\r", ch);
	  return;
	}
	
	if (ch->move < 1000)
	{
	  send_to_char("You don't have 1000 move to activate your power.\n\r", ch);
	  return;
	} 

        if (ch->fight_timer >0) 
        {
          send_to_char("Not until your fight timer expires.\n\r", ch );
          return;
        }

       if (!IS_SET(ch->pcdata->powers[ELEM_FORMS], WATER_FORM))
       {
          stc("You can only evaporate in Water Form!!\n\r", ch );
          return;
       }

       if ( IS_SET(ch->act, AFF_HIDE) )
       {
   	 REMOVE_BIT(ch->act, AFF_HIDE);
	 act( "$n reforms from a cloud of mist.", ch, NULL, NULL, TO_ROOM );
	 send_to_char( "You reform from a cloud of mist.\n\r", ch );
       }
       else
       {
	 act( "$n evaporates into mist.", ch, NULL, NULL, TO_ROOM );
	 send_to_char( "You evaporate into mist.\n\r", ch );
	 ch->move -= 1000;
         SET_BIT(ch->act, AFF_HIDE);
       }
       return;
}


