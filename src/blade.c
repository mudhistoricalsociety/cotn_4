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
 * Blade Master, class by Xrakisis           *
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



void do_pstrike( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  int dam;

  if (IS_NPC(ch)) return;
 
  if (!IS_CLASS(ch,CLASS_BLADE_MASTER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  WAIT_STATE( ch, 12 );

  if (!IS_NPC(victim)) dam = number_range(4500,8000);
  else dam = number_range(12000,14000);

  xprintf(buf,"Your Precision strike $N incredibly hard! [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_CHAR);
  xprintf(buf,"$n's Precision strike you incredibly hard! [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_VICT);
  xprintf(buf,"$n's Precision strike $N incredibly hard! [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_NOTVICT); 
  hurt_person(ch,victim, dam);
  return;
}
 

void do_rapidstrike( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA *obj;

  one_argument( argument, arg );
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_BLADE_MASTER))
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
  if (ch->pcdata->powers[BLADE_MELEE] < 15)
  {
    send_to_char("You need level 15 in Blade Melee before you can use Rapidstrike.\n\r",ch);
    return;
  }
  if ( victim == ch )
  {
    send_to_char( "You may not bring down your Retribution upon yourself.\n\r", ch );
    return;
  }
  if (is_safe( ch, victim )) return;

  WAIT_STATE( ch, 12 );

  {
    one_hit(ch, victim, gsn_lightningslash, 1);
    one_hit(ch, victim, gsn_lightningslash, 1);
    one_hit(ch, victim, gsn_lightningslash, 1);
    one_hit(ch, victim, gsn_lightningslash, 1);
  }

  return;
}



void do_retort( CHAR_DATA *ch, char *argument )
{
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_BLADE_MASTER))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
  if (ch->pcdata->powers[BLADE_SKILL] < 5)
  {
    send_to_char("You need to get your Blade Skill to 5 for this..\n\r",ch);
    return;
  }   
  if (IS_SET(ch->pcdata->powers[BM_BITS], BLADE_RETORT))
  {
    REMOVE_BIT(ch->pcdata->powers[BM_BITS], BLADE_RETORT);
    act("You Will no longer Take advantage of Weakness.", ch, NULL, NULL, TO_CHAR);
    act("$n's Will no longer Take advantage of their Opponents Weakness.", ch, NULL, NULL, TO_ROOM);
    return;
  }
  SET_BIT(ch->pcdata->powers[BM_BITS], BLADE_RETORT);
  act("#CYou Will now Take advantage of Weakness.#n", ch, NULL, NULL, TO_CHAR);
  act("$n Will now take Take advantage of their opponents Weakness.", ch, NULL, NULL, TO_ROOM);
  return;
}

void do_bladegain (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char buf3[MAX_STRING_LENGTH];

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_BLADE_MASTER))
  {
    send_to_char("Huh.\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    sprintf(buf, "#CBlade Technique [#r%d#C]#n",
      ch->pcdata->powers[BLADE_SKILL]);
    send_to_char(buf,ch);
    sprintf(buf2, "#CCombat Prowess [#r%d#C]#n",
      ch->pcdata->powers[BLADE_PROWESS]);
    send_to_char(buf2,ch);
    sprintf(buf3, "#CMelee Mastery [#r%d#C]#n\n\r",
      ch->pcdata->powers[BLADE_MELEE]);
    send_to_char(buf3,ch);
    return;
  }
}




void do_combatstyle (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_BLADE_MASTER))

  {
    send_to_char("Huh.\n\r",ch);
    return;
  }
  if (arg[0] == '\0')
  {
    sprintf(buf, "#CFight Styles\n\r");
    send_to_char(buf,ch);
    sprintf(buf, "#C[Defense] A Cautious fightstyle, more Parry,Dodge Based. \n\r");
    send_to_char(buf,ch);
    sprintf(buf, "#C[Melee] Increased Weapon Hits \n\r");
    send_to_char(buf,ch);
    sprintf(buf, "#C[Power] Advanced Fight Technique (Extra Auto Hits)\n\r");
    send_to_char(buf,ch);
    sprintf(buf, "#C[Fury] Psychotic Rage Filled Hits (Damcap)\n\r");
    send_to_char(buf,ch);
  }
  if (!str_cmp(arg,"defense"))  
  {
    if (ch->pcdata->powers[BLADE_SKILL] < 15)
    {
      send_to_char("You need to get your blade skill to 15.\n\r",ch);
      return;
    }
   	if (IS_SET(ch->newbits2, NEW2_DEFENSE)) {
	send_to_char("You Will No longer Use Defense Fight Style.\n\r", ch );
	REMOVE_BIT(ch->newbits2, NEW2_DEFENSE);
	}
	else {
	send_to_char("You Now Use a More Cautious Approach in Fighting.\n\r", ch );
	SET_BIT(ch->newbits2, NEW2_DEFENSE);
	}

  }
  if (!str_cmp(arg,"melee"))  
  {
    if (ch->pcdata->powers[BLADE_MELEE] < 6)
    {
      send_to_char("You need to get Your Blade Melee to atleast 6.\n\r",ch);
      return;
    }
   	if (IS_SET(ch->newbits2, NEW2_OFFENCE)) {
	send_to_char("You Stop Using Melee Fight Style.\n\r", ch );
	REMOVE_BIT(ch->newbits2, NEW2_OFFENCE);
	}
	else {
	send_to_char("You Focus and Drop into an Offence Based Fight Style.\n\r", ch );
	SET_BIT(ch->newbits2, NEW2_OFFENCE);
	} 
	}
 if (!str_cmp(arg,"fury"))  
  {
    if (ch->pcdata->powers[BLADE_PROWESS] < 8)
    {
      send_to_char("You Need to get Your Prowess to atleast 8.\n\r",ch);
      return;
    }
   	if (IS_SET(ch->newbits2, NEW2_DAMCAP))
      {
	send_to_char("You relax out of the Fury Fight Style .\n\r", ch );
	REMOVE_BIT(ch->newbits2, NEW2_DAMCAP);
	}
	else {
	send_to_char("A Uncontrolable Rage OverWhelms You!.\n\r", ch );
	SET_BIT(ch->newbits2, NEW2_DAMCAP);
	} 
	}
 if (!str_cmp(arg,"power"))  
  {
    if (ch->pcdata->powers[BLADE_SKILL] < 9)
    {
      send_to_char("You Need to get your Blade Skill to 9 for this.\n\r",ch);
      return;
    }
   	if (IS_SET(ch->newbits2, NEW2_AUTOATTACK))
      {
	send_to_char("You Will no Longer Look For Flaws in your Adversarys technique and take Advantage!.\n\r", ch );
	REMOVE_BIT(ch->newbits2, NEW2_AUTOATTACK);
	}
	else {
	send_to_char("You Will Now Look For Flaws in your Adversarys technique and take Advantage!.\n\r", ch );
	SET_BIT(ch->newbits2, NEW2_AUTOATTACK);
	}  
}
return;
}



void do_portal(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location;
  CHAR_DATA *victim;

  one_argument (argument, arg);
  if (IS_NPC(ch)) return;
  if (!IS_CLASS(ch, CLASS_BLADE_MASTER))
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("Travel where?\n\r", ch );
    return;
  }
  if( IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "You can't find it's room.\n\r",ch);
    return;
  }
  if (IS_IMMUNE(victim, IMM_TRAVEL) && !IS_NPC(victim))
  {
    send_to_char("You cant do that.\n\r",ch);
    return;
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  location = victim->in_room;
  if (ch->move < 250)
  {
    send_to_char("You are too tired.\n\r", ch );
    return;
  }
  act("You create a large portal and Enter!.", ch, NULL, NULL, TO_CHAR);
  act("$n creates a large blue portal and Enters!.", ch, NULL, NULL, TO_ROOM);
  ch->move -= 250;
  char_from_room(ch);
  char_to_room(ch, location);
  do_look(ch, "auto");
  act("You step out of your large blue portal.", ch, NULL, NULL, TO_CHAR);
  act("$n steps out of a large blue portal.", ch, NULL, NULL, TO_ROOM);
  return;
}

