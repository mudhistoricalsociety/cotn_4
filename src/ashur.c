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



void do_tremble(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char buf[MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;

  if (!IS_RACE(ch, RACE_MINOTAUR) && !IS_RACE(ch, RACE_OGRE)
     && !IS_RACE(ch, RACE_HALFOGRE) && !IS_RACE(ch, RACE_GIANT))
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }

  if (ch->pcRaceLevel < 25)
  {
    send_to_char("Only those who are level 25 and higher may use tremble.\n\r",ch);
    return;
  }

  if ((victim = ch->fighting) == NULL)
  {
    send_to_char("You are not fighting anyone.\n\r", ch);
    return;
  }

  if (IS_NPC(victim))
  {
    send_to_char("You cannot do this to an NPC.\n\r",ch);
    return;
  }

  if (number_percent() >= 35)
  {
    sprintf(buf, "%s falls out of their stance as you tremble the earth.\n\r",victim->name);
    send_to_char(buf,ch);
    sprintf(buf, "You fall from your stance as %s trembles the earth.\n\r",ch->name);
    send_to_char(buf,victim); 
    do_stance(victim, "");
    WAIT_STATE(ch,12);
    return;
  }
  else 
  {
    sprintf(buf, "You slip and fall out of your stance as you tremble the earth.\n\r");
    send_to_char(buf,ch);
    sprintf(buf, "%s slips and falls from their stance as %s trembles the earth.\n\r",ch->name, ch->name);
    send_to_char(buf,victim); 
    do_stance(ch, "");
    WAIT_STATE(ch,6);
    return;  
  }
  return;
}


void do_pirate(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int i;

    if (IS_NPC(ch)) return;

    if (!IS_RACE(ch, RACE_KENDER))
    {
      send_to_char("Huh?\n\r", ch );
      return;
    }

/*
    if (ch->pcRaceLevel < 25)
    {
      send_to_char("Only those who are level 25 and higher may use pirate.\n\r",ch);
      return;
    }
*/

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
     if ((victim = ch->fighting) == NULL)
     {
      send_to_char("But you are not fighting anyone.\n\r", ch);
      return;
     }
   }
   else if ((victim = get_char_room(ch, NULL, arg)) == NULL)
   {
     send_to_char("They are not here.\n\r", ch);
     return;
   }

   if (number_range(1,6) == 2)
   {

    for (i = 0; i < MAX_WEAR; i++)
    {
      if ((obj = get_eq_char(victim, i)) != NULL)
      {
           unequip_char(victim, obj);
           obj_from_char(obj);
           obj_to_char( obj, ch );
           send_to_char( "Item stolen.\n\r", ch );
           do_autosave(ch,"");
           do_autosave(victim,"");
           return;
      }
    }
    WAIT_STATE(ch,10); 
    return;
   }
   else 
   { 
     stc("You failed to steal anything!\n\r", ch);
     return;
   }   
}

