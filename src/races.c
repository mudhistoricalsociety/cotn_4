

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


void do_ascension(CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_STRING_LENGTH];
  char buf[MSL];
  argument = one_argument( argument, arg1 );

  if (IS_NPC(ch))return;

  if (!IS_NPC(ch) && ch->pcRaceLevel < 200)
  {   
    send_to_char("You must be level 200 to Ascend\n\r",ch);
    return;
  }
  if (ch->level < 3)
  {
    send_to_char("You must be avatar to Ascend.\n\r",ch);
    return;
  }
if ( arg1[0] == '\0' )
{
  send_to_char("Ascension  : Type Ascend (race) to choose your Demi-God Race.\n\r\n\r",ch);
  send_to_char("Options are ArchDemon or ArchAngel.\n\r\n\r",ch);
  return;
}

do_clearstats2(ch,"");

if (!str_cmp(arg1,"archdemon"))
{
	ch->pcRace = RACE_ARCHDEMON;
	send_to_char( "You have Become An Arch-Demon Demi-God.\n\r", ch);
	return;
}
else if (!str_cmp(arg1,"archangel"))
{
        ch->pcRace = RACE_ARCHANGEL;
        send_to_char( "You have Become an ArchAngel.\n\r", ch);
        return;
}

}


void do_racepowers(CHAR_DATA * ch, char *argument)
{
        char      buf[MAX_STRING_LENGTH];

	if (IS_NPC(ch))
	return;

	send_to_char("[--------------------[Race Powers]--------------------]\n\r", ch);

	if (IS_RACE(ch, RACE_GULLYDWARF) || IS_RACE(ch, RACE_MTDWARF)
	|| IS_RACE(ch, RACE_DUERGARDWARF) || IS_RACE(ch, RACE_HILLDWARF)) 
	{
	send_to_char("#R(#0ThunderHammer#R)#0 Charge an ethereal Hammer and send a shockwave.#n\n\r", ch);
	}
        if (IS_RACE(ch, RACE_GOBLIN) || IS_RACE(ch, RACE_HOBGOBLIN))
        {
        send_to_char("#R(#0Goblingrenade#R)#0 Drop a Goblin Grenade.#n\n\r", ch);
        }
        if (IS_RACE(ch, RACE_KENDER))
        {
        send_to_char("#R(#0Pirate#R)#0 Steal equipment from your opponent!!#n\n\r", ch);
        }
        if (IS_RACE(ch, RACE_GNOME))
        {
        send_to_char("#R(#0AirshipTravel#R)#0 Supreme method of travel.#n\n\r", ch);
        }
        if (IS_RACE(ch, RACE_MINOTAUR) || IS_RACE(ch, RACE_OGRE)
        || IS_RACE(ch, RACE_HUMAN) || IS_RACE(ch, RACE_TROLL)
        || IS_RACE(ch, RACE_HALFTROLL) || IS_RACE(ch, RACE_HALFOGRE)
        || IS_RACE(ch, RACE_ORC) || IS_RACE(ch, RACE_HALFORC)
        || IS_RACE(ch, RACE_GIANT) || IS_RACE(ch, RACE_CENTAUR)
	|| IS_RACE(ch, RACE_ARCHANGEL) || IS_RACE(ch, RACE_ARCHDEMON)
	|| IS_RACE(ch, RACE_ELF) || IS_RACE(ch, RACE_DROW)
	|| IS_RACE(ch, RACE_HALFELF) || IS_RACE(ch, RACE_WILDELF))
        {
        send_to_char("#R(#0SuperBerserk#R)#0 Knowledge of a Combat Technique passed down from Barbarians.#n\n\r", ch);
        }
        if (IS_RACE(ch, RACE_ELF) || IS_RACE(ch, RACE_HALFELF)
        || IS_RACE(ch, RACE_GNOME) || IS_RACE(ch, RACE_DUERGARDWARF)
        || IS_RACE(ch, RACE_GULLYDWARF) || IS_RACE(ch, RACE_HILLDWARF)
        || IS_RACE(ch, RACE_MTDWARF) || IS_RACE(ch, RACE_DROW)
        || IS_RACE(ch, RACE_WILDELF) || IS_RACE(ch, RACE_ARCHANGEL)
        || IS_RACE(ch, RACE_ARCHDEMON))
        {
        send_to_char("#R(#0Infravision#R)#0 The Ability to see at night.#n\n\r", ch);
        }
	if (IS_RACE(ch, RACE_ELF) || IS_RACE(ch, RACE_WILDELF)
	|| IS_RACE(ch, RACE_HALFELF) || IS_RACE(ch, RACE_DROW))
	{
        send_to_char("#R(#0ElfSing#R)#0 Sing Songs.#n\n\r", ch);
        send_to_char("#R(#0Lessons#R)#0 Learn Songs.#n\n\r", ch);
        send_to_char("#R(#0Songs#R)#0 Listing of Songs.#n\n\r", ch);
	}
        if (IS_CLASS (ch, RACE_TROLL) || IS_RACE(ch, RACE_HALFTROLL)
        || IS_RACE(ch, RACE_ARCHANGEL) || IS_RACE(ch, RACE_ARCHDEMON)
        || IS_RACE(ch, RACE_DRAGONKIND))
        {
        send_to_char("#R(#0Toughskin#R)#0 Have your Skin take a Leathery Texture.#n\n\r", ch);
        }



        send_to_char("[--------------------[  level 25  ]-------------------]\n\r", ch);
        if (ch->pcRaceLevel >= 25)
        {
        send_to_char("#R(#0WeaponEnhance#R)#0 Raise the average damage on a weapon.#n\n\r", ch);
        send_to_char("#R(#0ArmorEnhance#R)#0 AC bonus to armor.#n\n\r", ch);
        }
        if (IS_RACE(ch, RACE_MINOTAUR) || IS_RACE(ch, RACE_OGRE)
        || IS_RACE(ch, RACE_HALFOGRE) || IS_RACE(ch, RACE_GIANT))
        {
        send_to_char("#R(#0Tremble#R)#0 The ability to destance your opponent.#n\n\r", ch);
        }
	send_to_char("[--------------------[  level 50  ]-------------------]\n\r", ch);
        if (ch->pcRaceLevel >= 50)
        {
        send_to_char("#R(#0Mistform#R)#0 The Ability to turn Ethereal.#n\n\r", ch);
        send_to_char("#R(#0Flameblade#R)#0 The Power to surround a blade with Flames.#n\n\r", ch);
        }
        send_to_char("[--------------------[Race Powers]--------------------]\n\r", ch);


	return;
}

void do_gainlevel(CHAR_DATA * ch, char *argument)
{
        char      buf[MAX_STRING_LENGTH];
	int lgainexp  = 250000000;
	int lgainbones = 400 * ch->pcRaceLevel;

	if( ch->pcRaceLevel >= 200 )
	{
        send_to_char( "Already Maximum Race Level.\n\r", ch );
        return;
	}
	if( ch->exp < lgainexp )
	{
        sprintf( buf, "Not enough Experience points, you need %d more.\n\r", ( lgainexp - ch->exp ) );
        send_to_char( buf, ch );
        return;
        }
        if( ch->bones < lgainbones ) 
	{
        sprintf( buf, "Not enough bones, you need %d more.\n\r", ( lgainbones - ch->bones ) );
        send_to_char( buf, ch );
        return;
        }
	if (lgainbones > 35000) lgainbones = 30000;

        ch->bones -= lgainbones;
        ch->exp -= lgainexp;
        ch->pcRaceLevel++;
        ch->pcpractice += number_range(2,8);
	sprintf( buf, "#0%s is now #RLevel #R[#0%d#R]#n", ch->pcdata->switchname, ch->pcRaceLevel );
        do_info( ch, buf );
        return;
}



void do_raceself(CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_STRING_LENGTH];
  char buf[MSL];
  argument = one_argument( argument, arg1 );

  if (IS_NPC(ch))return;

  if (!IS_NPC(ch) && ch->pcRace != 0)
  {   
    send_to_char("You already have a race.\n\r",ch);
    return;
  }
  if (ch->level < 3)
  {
    send_to_char("You must be avatar to selfrace.\n\r",ch);
    return;
  }
if ( arg1[0] == '\0' )
{
  send_to_char("Races: Type selfrace (race) to choose your Race.\n\r\n\r",ch);
  send_to_char("#0<<--------------------------------------------------------------------------------->> \n\r",ch);
  send_to_char("#0 | Human, Elf, Drow, HalfElf, Gnome, Gullydwarf, Hilldwarf, MountainDwarf          |.\n\r",ch);
  send_to_char("#0 | Dragonkind, Hobbit, Kender, Giant, Ogre, Halfogre, Minotaur, Goblin, Hobgoblin  |.\n\r",ch);
  send_to_char("#0 | Duergardwarf, Troll, Halftroll, Orc, Halforc, Centaur, Wildelf.		     |.\n\r",ch);
  send_to_char("#0 |_________________________________________________________________________________|\n\r",ch);
  return;
}

do_clearstats2(ch,"");

if (!str_cmp(arg1,"human"))
{
	if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
	|| IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
	|| IS_CLASS(ch, CLASS_DROW))
	{
	send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
	}
	else 
	{
	ch->pcRace = RACE_HUMAN;
	send_to_char( "You have Become Human.\n\r", ch);
	}
	return;
}
else if (!str_cmp(arg1,"elf"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
	else
	{
	ch->pcRace = RACE_ELF;
	send_to_char( "You have Become an Elf.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"gnome"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
	}
	else
	{
	ch->pcRace = RACE_GNOME;
	send_to_char( "You have Become a Gnome.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"gullydwarf"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
        {
	ch->pcRace = RACE_GULLYDWARF;
	send_to_char( "You have Become a Gully Dwarf.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"hilldwarf"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_HILLDWARF;
	send_to_char( "You have Become a Hill Dwarf.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"mountaindwarf"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_MTDWARF;
	send_to_char( "You have Become a MT. Dwarf.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"dragonkind"))
{
        if (!IS_CLASS(ch, CLASS_DRAGON) && !IS_CLASS(ch, CLASS_DRACONIAN))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_DRAGONKIND;
	send_to_char( "Your veins run rich with Immortal Dragon Blood.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"hobbit"))
{
        if (!IS_CLASS(ch, CLASS_HOBBIT))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_HOBBIT;
	send_to_char( "You have become a Shireling.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"kender"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_KENDER;
	send_to_char( "Your Feet feel the calling of Wanderlust.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"drow"))
{
        if (!IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_DROW;
	send_to_char( "The Suns becomes Brilliant and painful to your eyes.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"giant"))
{
	if (!IS_CLASS(ch, CLASS_GIANT))
	{
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
	}
	else
	{
	ch->pcRace = RACE_GIANT;
	send_to_char( "You choose to stay a Giant.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"halfelf"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_HALFELF;
	send_to_char( "You are a Half Elf.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"ogre"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_OGRE;
	send_to_char( "You are an Ogre.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"halfogre"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_HALFOGRE;
	send_to_char( "You are a Half Ogre.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"minotaur"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_MINOTAUR;
	send_to_char( "You grow in stature, Your skin turns red and Bull Horns sprout from your head.\n\r", ch);
	}
        return;
}
else if (!str_cmp(arg1,"goblin"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_GOBLIN;
	send_to_char( "You are a stinky, thieving little goblin.\n\r", ch);
	}
	return;
}
else if (!str_cmp(arg1,"hobgoblin"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_HOBGOBLIN;
	send_to_char( "You are a Hobgoblin Commander of a Goblin Squad.\n\r", ch);
	}
	return;
}
else if (!str_cmp(arg1,"duergar"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_DUERGARDWARF;
	send_to_char( "You are now a Duergar Dwarf.\n\r", ch);
	}
	return;
}
else if (!str_cmp(arg1,"troll"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_TROLL;
	send_to_char( "You are now a Half-Troll.\n\r", ch);
	}
	return;
}
else if (!str_cmp(arg1,"orc"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_ORC;
	send_to_char( "You are now an Orc.\n\r", ch);
	}
	return;
}
else if (!str_cmp(arg1,"halforc"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_HALFORC;
	send_to_char( "You are now a Half-Orc.\n\r", ch);
	}
	return;
}
else if (!str_cmp(arg1,"centaur"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_CENTAUR;
	send_to_char( "you are now a Centaur.\n\r", ch);
	}
	return;
}
else if (!str_cmp(arg1,"wildelf"))
{
        if (IS_CLASS(ch, CLASS_HOBBIT) || IS_CLASS(ch, CLASS_GIANT)
        || IS_CLASS(ch, CLASS_DRACONIAN) || IS_CLASS(ch, CLASS_DRAGON)
        || IS_CLASS(ch, CLASS_DROW))
        {
        send_to_char( "Pick the Race Apropriate for your Class.\n\r", ch);
        }
        else
	{
	ch->pcRace = RACE_WILDELF;
	send_to_char( "You are a WildElf.\n\r", ch);
	}
	return;
}
  else do_raceself(ch,"");   
  return;
}

void do_armorenhance(CHAR_DATA *ch, char *argument)
{
	 OBJ_DATA *obj;

        if (ch->pcRaceLevel < 25)
          {
             send_to_char("huh?\n\r",ch);
             return;
          }
	if ( ch->practice < 1500)
	  {
		 send_to_char("It costs 1500 primal to enhance a weapon.\n\r",ch);
		 return;
	  }
	if (argument[0] == '\0')
	  {
		 send_to_char("Which item do you wish to forge?\n\r", ch);
		 return;
	  }
	if ( (obj = get_obj_carry(ch, argument, ch)) == NULL)
	  {
		 send_to_char("You are not carrying that item.\n\r", ch);
		 return;
	  }
/*	if (IS_SET(obj->quest,QUEST_RELIC) || IS_SET(obj->quest,QUEST_ARTIFACT))
	  {
		send_to_char("Not on a Relic.\n\r",ch);
		return;
	  }
*/
        if (IS_SET(obj->quest2,QUEST2_RITUAL))
          {
                send_to_char("This item has already been through the Ritual!\n\r",ch);
                return;       
	  }
	if (obj->item_type == ITEM_ARMOR)
          {
                obj->value[0] += 50;
                oset_affect(ch,obj,30,APPLY_HITROLL,TRUE);
                oset_affect(ch,obj,30,APPLY_DAMROLL,TRUE);
                oset_affect(ch,obj,-50,APPLY_AC,TRUE);
                SET_BIT(obj->quest2,QUEST2_RITUAL);
                obj->condition = 100;
                obj->toughness = 100;
                obj->resistance = 1;
		    act("You bestow power into $p through the Dark Ritual.", ch, obj, NULL, TO_CHAR);
		    act("$n bestows power into $p through the Dark Ritual.", ch, obj, NULL, TO_ROOM);
                ch->practice -= 1500;
                WAIT_STATE(ch, 8);
                return;
          }
          

}
 
void do_flameblade(CHAR_DATA *ch, char *argument)
{
	 OBJ_DATA *obj;

        if (ch->pcRaceLevel < 50)
          {
             send_to_char("huh?\n\r",ch);
             return;
          }
	if ( ch->practice < 1500)
	  {
		 send_to_char("It costs 1500 primal to flameblade a weapon.\n\r",ch);
		 return;
	  }
	if (argument[0] == '\0')
	  {
		 send_to_char("Which item do you wish to forge?\n\r", ch);
		 return;
	  }
	if ( (obj = get_obj_carry(ch, argument, ch)) == NULL)
	  {
		 send_to_char("You are not carrying that item.\n\r", ch);
		 return;
	  }
/*	if (IS_SET(obj->quest,QUEST_RELIC) || IS_SET(obj->quest,QUEST_ARTIFACT))
	  {
		send_to_char("Not on a Relic.\n\r",ch);
		return;
	  }
*/
        if (IS_SET(obj->quest2,QUEST2_FLAMEBLADE))
          {
                send_to_char("This item has already been through the Ritual!\n\r",ch);
                return;
          }
	if (obj->item_type == ITEM_WEAPON)
	  {
		 obj->level     =  50;
		 SET_BIT(obj->quest2,QUEST2_FLAMEBLADE);
		 obj->condition = 100;
		 obj->toughness = 100;
		 obj->resistance = 1;
		 act(" $p is surrounded with Black Flames.", ch, obj, NULL, TO_CHAR);
		 act("$n stares into  $p and it becomes surrounded with black flames.", ch, obj, NULL, TO_ROOM);
		 if(!IS_SET(obj->weapflags, WEAPON_FLAMING)) SET_BIT(obj->weapflags, WEAPON_FLAMING);
		 ch->practice -= 1500;
		 WAIT_STATE(ch, 8); 
		 return;
	  }
}

void do_elveninfra(CHAR_DATA *ch, char *argument)
{
	 OBJ_DATA *obj;

	if (!IS_RACE(ch, RACE_ELF) && !IS_RACE(ch, RACE_HALFELF)
	&& !IS_RACE(ch, RACE_GNOME) && !IS_RACE(ch, RACE_DUERGARDWARF)
	&& !IS_RACE(ch, RACE_GULLYDWARF) && !IS_RACE(ch, RACE_HILLDWARF)
	&& !IS_RACE(ch, RACE_MTDWARF) && !IS_RACE(ch, RACE_DROW)
	&& !IS_RACE(ch, RACE_WILDELF) && !IS_RACE(ch, RACE_ARCHANGEL)
	&& !IS_RACE(ch, RACE_ARCHDEMON))
          {
             send_to_char("huh?\n\r",ch);
             return;
          }
               if (IS_SET(ch->act, PLR_HOLYLIGHT))
                {
                        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
                        send_to_char("Your Infravision Fades.\n\r",
                                     ch);
                }
                else
                {
                        SET_BIT(ch->act, PLR_HOLYLIGHT);
                        send_to_char("Your eyes glow red with Infravision.\n\r",
                                     ch);
                }
	 return;
}


void do_weaponenhance(CHAR_DATA *ch, char *argument)
{
	 OBJ_DATA *obj;

        if (ch->pcRaceLevel < 25)
          {
             send_to_char("huh?\n\r",ch);
             return;
          }
	if ( ch->practice < 1500)
	  {
		 send_to_char("It costs 1500 primal to enhance a weapon.\n\r",ch);
		 return;
	  }
	if (argument[0] == '\0')
	  {
		 send_to_char("Which item do you wish to forge?\n\r", ch);
		 return;
	  }
	if ( (obj = get_obj_carry(ch, argument, ch)) == NULL)
	  {
		 send_to_char("You are not carrying that item.\n\r", ch);
		 return;
	  }
/*	if (IS_SET(obj->quest,QUEST_RELIC) || IS_SET(obj->quest,QUEST_ARTIFACT))
	  {
		send_to_char("Not on a Relic.\n\r",ch);
		return;
	  }
*/
        if (IS_SET(obj->quest2,QUEST2_RITUAL))
          {
                send_to_char("This item has already been through the Ritual!\n\r",ch);
                return;
          }
	if (obj->item_type == ITEM_WEAPON)
	  {
		 obj->value[1] += 100;
		 obj->value[2] += 100;
		 obj->level     =  50;
		 oset_affect(ch,obj,50,APPLY_HITROLL,TRUE);
		 oset_affect(ch,obj,50,APPLY_DAMROLL,TRUE);
		 SET_BIT(obj->quest2,QUEST2_RITUAL);
		 obj->condition = 100;
		 obj->toughness = 100;
		 obj->resistance = 1;
		 act("You bestow power into $p through the Dark Ritual.", ch, obj, NULL, TO_CHAR);
		 act("$n bestows power into $p through the Dark Ritual.", ch, obj, NULL, TO_ROOM);
		 if(!IS_SET(obj->weapflags, WEAPON_FLAMING)) SET_BIT(obj->weapflags, WEAPON_FLAMING);
		 ch->practice -= 1500;
		 WAIT_STATE(ch, 8); 
		 return;
	  }
}

void do_mistform(CHAR_DATA * ch, char *argument)
{
        if (IS_NPC(ch))
                return;
        if (ch->pcRaceLevel < 50)
        {
                send_to_char("You need your race level to 50 to use this.\n\r", ch);
                return;
        }
        if (IS_AFFECTED(ch, AFF_ETHEREAL))
        {
                REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
                act("You shift back int your solid material form.", ch, NULL, NULL,
                    TO_CHAR);
                act("$n regains $s form and re-enters their solid form.", ch, NULL,
                    NULL, TO_ROOM);
                return;
        }
        if (has_timer(ch))
                return;
        SET_BIT(ch->affected_by, AFF_ETHEREAL);
        act("You shift into the ethereal realm.", ch, NULL, NULL, TO_CHAR);
        act("$n turns ethereal.", ch, NULL, NULL, TO_ROOM);
        return;
}


void do_goblingrenade(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        int       dam;
        int       level;


        if (IS_NPC(ch))
                return;

	if (!IS_RACE(ch, RACE_GOBLIN) && !IS_RACE(ch, RACE_HOBGOBLIN))
	{
                send_to_char("Huh?\n\r", ch);
                return;
        }

        if (ch->mana < 1150)
        {
                send_to_char("You need more mana.\n\r", ch);
                return;
        }

        level = ch->spl[PURPLE_MAGIC];
        ch->mana -= 1150;

        send_to_char
                ("You pull the pin on a Goblin Grenade.\n\r",
                 ch);
        act("$n drops a Goblin Grenade!", ch, NULL, NULL, TO_ROOM);

        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
                vch_next = vch->next_in_room;

                if (vch == ch)
                        continue;
                if (vch->trust > 6)
                        continue;
                dam = dice(level, 40);
                if (saves_spell(level, vch))
                        dam /= 2;
                damage(ch, vch, dam, skill_lookup("earthquake"));
        }
        WAIT_STATE(ch, 9);
        return;
}


void do_airshiptravel(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;

        argument = one_argument(argument, arg);

        if (IS_NPC(ch))
                return;

        if (!IS_RACE(ch, RACE_GNOME))
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        if (arg[0] == '\0')
        {
                send_to_char("Airship Travel to who?\n\r", ch);
                return;
        }

        if ((victim = get_char_world(ch, arg)) == NULL)
        {
                send_to_char("They are not here.\n\r", ch);
                return;
        }

        if (victim == ch)
        {
                send_to_char("You cannot travel to yourself.\n\r", ch);
                return;
        }

        if (victim->in_room == NULL)
        {
                send_to_char("Nothing happens.\n\r", ch);
                return;
        }

        send_to_char("You set off a signal flare and an airship picks you up.\n\r", ch);
        act("$n sets off a signal flare and is picked up by an airship.", ch, NULL, NULL, TO_ROOM);
        char_from_room(ch);
        char_to_room(ch, victim->in_room);
        do_look(ch, "");
        send_to_char("You are dropped off by a Gnome Airship.\n\r", ch);
        act("$n is dropped off by a Gnome Airship.", ch, NULL, NULL, TO_ROOM);
        return;
}


void do_thunderhammer(CHAR_DATA * ch, char *argument)
{

	char buf[MSL];
	char buf2[MSL];
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	int dam;
	//int dam2;
	dam = ch->pRank * 865;
	dam *= 3;
	dam /= 2;
	
	if(IS_NPC(ch)) return;
	
	if (!IS_RACE(ch, RACE_MTDWARF) && !IS_RACE(ch, RACE_HILLDWARF)
	&& !IS_RACE(ch, RACE_DUERGARDWARF) && !IS_RACE(ch, RACE_GULLYDWARF))
	{
		stc("Huh?\n\r",ch);
		return;
	}
	for ( vch = ch->in_room->people; vch!=NULL;vch = vch_next)
	      {
	        vch_next = vch->next_in_room;
	        if (can_see(ch,vch))
	        if ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) )
	        {
		if(is_safe(ch,vch)) continue;
	          sprintf(buf, "#0Your body is struck by lightning! [#C%d#n]\n\r", dam);
	          send_to_char( buf, vch );
	          sprintf(buf2, "#r%s's#0 body is struck by lightning! [#C%d#n]\n\r",vch->name, dam);
	          send_to_char( buf2, ch );
	          set_fighting(ch,vch, TRUE);
	          hurt_person(ch,vch,dam);
	        }
	      }
	
	WAIT_STATE(ch,12);

}

void do_superberserk(CHAR_DATA *ch, char *argument)
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

  WAIT_STATE( ch, 6);
  if ( number_percent( ) > ch->pcdata->learned[gsn_berserk] )
  {
    act("You rant and rave, but nothing much happens.",ch,NULL,NULL,TO_CHAR);
    act("$n gets a wild look in $s eyes, but nothing much happens.",ch,NULL,NULL,TO_ROOM);
    return;
  }
	if (!IS_RACE(ch, RACE_MINOTAUR) && !IS_RACE(ch, RACE_OGRE)
	&& !IS_RACE(ch, RACE_HUMAN) && !IS_RACE(ch, RACE_TROLL)
	&& !IS_RACE(ch, RACE_HALFTROLL) && !IS_RACE(ch, RACE_HALFOGRE)
	&& !IS_RACE(ch, RACE_ORC) && !IS_RACE(ch, RACE_HALFORC)
	&& !IS_RACE(ch, RACE_GIANT) && !IS_RACE(ch, RACE_CENTAUR)
	&& !IS_RACE(ch, RACE_ARCHANGEL) && !IS_RACE(ch, RACE_ARCHDEMON)
	&& !IS_RACE(ch, RACE_ELF) && !IS_RACE(ch, RACE_WILDELF)
	&& !IS_RACE(ch, RACE_HALFELF) && !IS_RACE(ch, RACE_DROW))
        {
                stc("Huh?\n\r",ch);
                return;
        }


  act("#0You Roar with Rage and #7B#wE#7R#wS#7E#wR#7K#0 into a Barbarian style Frenzy.#n",ch,NULL,NULL,TO_CHAR);
  act("#0$n Roars with Rage and #7B#wE#7R#wS#7E#wR#7K#wS#0 into a Barbarian style Frenzy!#n",ch,NULL,NULL,TO_ROOM);
        for (vch = char_list; vch != NULL; vch = vch_next)
        {
                vch_next = vch->next;
                if (vch->in_room == NULL)
                        continue;
                if (!IS_NPC(vch)) continue;
                if (ch == vch)
                {
                        act("You throw back your head and howl with rage!",
                            ch, NULL, NULL, TO_CHAR);
                        continue;
                }
                if (!IS_NPC(vch) && vch->pcdata->chobj != NULL)
                        continue;
                if (!IS_NPC(vch))
                {
//                        if (vch->in_room == ch->in_room)
                        if (vch->in_room->area == ch->in_room->area)
                                send_to_char
                                        ("You hear a Warcry close by!\n\r",
                                         vch);
                        else if (!CAN_PK(vch))
                                continue;
                }
                if (vch->in_room == ch->in_room && can_see(ch, vch))
                {
                        multi_hit(ch, vch, TYPE_UNDEFINED);
                        if (vch == NULL || vch->position <= POS_STUNNED)
                                continue;
                        multi_hit(ch, vch, TYPE_UNDEFINED);
                }
        }
return;
}



void do_lessons(CHAR_DATA *ch, char *argument)
{
 char buf[MAX_INPUT_LENGTH];
 int cost;
 int cost2;
 cost = ch->pcdata->powers[BARD_LESSONS] * 100000;  
 cost2 = cost - ch->exp;


if (!IS_RACE(ch, RACE_ELF) && !IS_RACE(ch, RACE_WILDELF)
&& !IS_RACE(ch, RACE_HALFELF) && !IS_RACE(ch, RACE_DROW))
{
 send_to_char("Huh?\n\r", ch);
 return;
}

if (argument[0] == '\0')
{
 send_to_char("Options are: show, improve.\n\r", ch);
 return;
}
if (!str_cmp(argument, "show"))
{
 sprintf(buf, "Your skill is currently at %d.\n\r", ch->pcdata->powers[BARD_LESSONS]);
 send_to_char(buf, ch);
}
else if (!str_cmp(argument, "improve"))
{
 if (ch->exp < cost)
 {
  sprintf(buf, "You need %d more experience.\n\r", cost2);
  send_to_char(buf, ch);
  return;
 }
 if (ch->pcdata->powers[BARD_LESSONS] >= 10)
 {
  send_to_char("You're already as skilled as you can become.\n\r", ch);
  return;
 }
 ch->exp -= cost;
 send_to_char("You learn a little more about the arts.\n\r", ch);
 ch->pcdata->powers[BARD_LESSONS]++;
 return;
}
else 
{
 send_to_char("Try improve or show.\n\r", ch);
 return;
}
return;
}

void do_elfsing(CHAR_DATA *ch, char *argument) 
{
 char buf[MAX_INPUT_LENGTH];
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
  int chance;
  int dam[4];
 CHAR_DATA *victim;
 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);



if (!IS_RACE(ch, RACE_ELF) && !IS_RACE(ch, RACE_WILDELF)
&& !IS_RACE(ch, RACE_HALFELF) && !IS_RACE(ch, RACE_DROW))
{
 send_to_char("Huh?\n\r", ch);
 return;
}

 if (arg1[0] == '\0')
 {
  send_to_char("Syntax: sing (song) (target)\n\r", ch);
  send_to_char("Possible Songs: \n\r", ch);
  send_to_char("Minuet, Minne, Madrigal, March, Dissonance \n\r", ch);
  send_to_char("Requiem, Paeon, Ballad, Lullaby, Hymnus, Spiritual \n\r", 
ch);
  return;
 }
  if (str_cmp(arg1, "minuet") && str_cmp(arg1, "minne") && str_cmp(arg1, "madrigal") &&
str_cmp(arg1, "march") && str_cmp(arg1, "requiem") && str_cmp(arg1, "paeon") &&
str_cmp(arg1, "ballad") && str_cmp(arg1, "lullaby") && str_cmp(arg1, "hymnnus") &&
str_cmp(arg1, "spiritual") && str_cmp(arg1, "dissonance"))
  {
   send_to_char("Possible Songs: Minuet, Minne, Madrigal, March, Requiem, Paeon,\n\r", ch);
   send_to_char("Ballad, Lullaby, Hymnus, Dissonance, Spiritual\n\r", ch);
   return;
  }
  
  if (!str_cmp(arg2, "self") || !str_cmp(arg2, ch->name))
   victim = ch;
  else if ((victim = get_char_room(ch, NULL, arg2)) == NULL)
  {
   send_to_char("They aren't here.\n\r", ch);
   return;
  }
  
  if (ch->pcdata->powers[SONG_MASTERY] < 1)
   ch->pcdata->powers[SONG_MASTERY] = 1;

  chance = number_range(1, ch->pcdata->powers[BARD_LESSONS]);
  if (chance == 1)
  {
   if (ch->pcdata->powers[SONG_MASTERY] < 10)
   {
    send_to_char("Your skill at singing improves.\n\r", ch);
    ch->pcdata->powers[SONG_MASTERY]++;
   }
  }

  if (!str_cmp(arg1, "minuet"))
  {
   if (IS_NPC(victim))
   { 
    send_to_char("Not on NPCs.\n\r", ch);
    return;
   }
   if (victim->pcdata->songs[SONG_MINUET] > 0)
   {
    send_to_char("They're already affected!\n\r", ch);
    return;
   }
   sprintf(buf, "#nYou sing a wonderful #rminuet#n to %s.\n\r", victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "#n%s sings a wonderful #rminuet#n to you.\n\r", ch->name);
   send_to_char(buf, victim);
   victim->pcdata->songs[SONG_MINUET] += (50 * ch->pcdata->powers[SONG_MASTERY]);
   victim->damroll += (50 * ch->pcdata->powers[SONG_MASTERY]);
   return;
  }
  else if (!str_cmp(arg1, "minne"))
  {
   if (IS_NPC(victim))
   {
    send_to_char("Not on NPCs.\n\r", ch);
    return;
   }
   if (ch->pcdata->powers[BARD_LESSONS] < 1)
   {
    send_to_char("You need to take more lessons.\n\r", ch);
    return;
   }
   if(victim->pcdata->songs[SONG_MINNE] > 0)
   {
    send_to_char("They're already affected!\n\r", ch);
    return;
   }
   sprintf(buf, "#nYou sing a wonderful #Lminne#n to %s.\n\r", victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "#n%s sings a wonderful #Lminne#n to you.\n\r", ch->name);
   send_to_char(buf,victim);
   victim->pcdata->songs[SONG_MINNE] += (50 * ch->pcdata->powers[SONG_MASTERY]);
   victim->armor -= (50 * ch->pcdata->powers[SONG_MASTERY]);
   return;
  }
  else if (!str_cmp(arg1, "madrigal"))
  {
   if (IS_NPC(victim))
   {
    send_to_char("Not on NPCs.\n\r", ch);
   return;
   }
   if (ch->pcdata->powers[BARD_LESSONS] < 2)
   { 
    send_to_char("You need to take more lessons.\n\r", ch);
    return;
   }
   if (victim->pcdata->songs[SONG_MADRIGAL] > 0)
   {
    send_to_char("They're already affected!\n\r", ch);
    return;
   }
   sprintf(buf, "#nYou sing a wonderful #Gmadrigal#n to %s.\n\r", victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "#n%s sings a wonderful #Gmadrigal#n to you.\n\r", ch->name);
   send_to_char(buf, victim);
   victim->pcdata->songs[SONG_MADRIGAL] += (50 * ch->pcdata->powers[SONG_MASTERY]);
   victim->hitroll += (50 * ch->pcdata->powers[SONG_MASTERY]);
   return;
  }
  else if (!str_cmp(arg1, "spiritual"))
  {
   if(ch->pcdata->powers[BARD_LESSONS] < 10)
   {
    send_to_char("You need to take more lessons.\n\r", ch);
    return;
   } 
   dam[0] = ch->pcdata->songs[0] + ch->pcdata->songs[1] + ch->pcdata->songs[2] + ch->pcdata->songs[3];
   dam[1] = ch->pcdata->songs[4] + ch->pcdata->songs[5] + ch->pcdata->songs[6] + ch->pcdata->songs[7];
   dam[2] = dam[0] + dam[1];
   dam[3] = dam[2] * 12.5;
   damage(ch, victim, dam[3], gsn_requiem);
   sprintf(buf, "#yYou channel all your #7inspiration#y into one powerful song, hitting #C%s #yincredibly hard. [#L%d#y]#n\n\r", 
   victim->name, dam[3]);
   send_to_char(buf, ch);
   sprintf(buf, "#C%s #ychannels every bit of their #7inspiration #yinto one powerful song, hitting you hard. [#L%d#y]#n\n\r", ch->name, dam[3]);
   send_to_char(buf, victim);
   WAIT_STATE(ch, 36);
   return;
  }
  else if (!str_cmp(arg1, "lullaby"))
  {
   if (IS_NPC(victim))
   {
    send_to_char("Only on PCs.\n\r", ch);
    return;
   }
   if (ch->pcdata->powers[BARD_LESSONS] < 7)
   {
    send_to_char("You need to take more lessons.\n\r", ch);
    return;
   }
   if (victim->pcdata->songs[SONG_LULLABY] > 0)
   {
    send_to_char("They're already asleep.\n\r", ch);
    return;
   }
   sprintf(buf, "#nYou sing a #0lullaby#n to %s, putting them to sleep.\n\r", victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "#n%s sings a #0lullaby#n to you, putting you to sleep.\n\r", ch->name);
   send_to_char(buf, victim);
   victim->position = POS_STUNNED;
  }   
  else if (!str_cmp(arg1, "requiem"))
  {
   if (IS_NPC(victim))
   {
    send_to_char("Only on PCs.\n\r", ch);
    return;
   }
   if (ch->pcdata->powers[BARD_LESSONS] < 4)
   {
    send_to_char("You need to take more lessons.\n\r", ch);
    return;
   }
   if (victim->pcdata->songs[SONG_REQUIEM] > 0)
   {
    send_to_char("They're already suffering.\n\r", ch);
    return;
   }
   sprintf(buf, "#nYou instill within %s a horrific #rrequiem#n.\n\r", victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "#n%s instills within you a horrific #rrequiem#n.\n\r", ch->name);
   send_to_char(buf, victim);
   victim->pcdata->songs[SONG_REQUIEM] += ch->pcdata->powers[SONG_MASTERY];
   return;
  }
  else if (!str_cmp(arg1, "dissonance"))
  {
   if (ch->pcdata->powers[BARD_LESSONS] < 9)
   {
    send_to_char("You need to take more lessons.\n\r", ch);
    return;
   }
   dam[0] = number_range(1, 100);
   dam[1] = number_range(5, 500);
   dam[2] = number_range(9, 900);
   dam[3] = number_range(13, 1300);
   dam[4] = number_range(17, 1700);
   damage(ch, victim, dam[0], gsn_requiem);
   damage(ch, victim, dam[1], gsn_requiem);
   damage(ch, victim, dam[2], gsn_requiem);
   damage(ch, victim, dam[3], gsn_requiem);
   damage(ch, victim, dam[4], gsn_requiem);
   sprintf(buf, "#nThe #7intrdouction#n to your dissonant rush does #y[#L%d#y]#n damage to #C%s.#n\n\r", dam[0], victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "#nThe #7warmup#n to your dissonant attack does #y[#L%d#y]#n damage to #C%s.#n\n\r", dam[1], victim->name);
   send_to_char(buf, ch); 
   sprintf(buf, "#nThe #7body#n of your dissonant onslaught does #y[#L%d#y]#n damage to #C%s.#n\n\r", dam[2], victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "#nThe #7outro#n to your dissonant song does #y[#L%d#y]#n damage to #C%s.#n\n\r", dam[3], victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "#nThe #7aftermath#n of your attack does #y[#L%d#y]#n damage to #C%s.#n\n\r", dam[4], victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "You take #y[#L%d#y]#n damage from %s's dissonant song #7introduction.#m\n\r", dam[0], ch->name);
   send_to_char(buf, victim);
   sprintf(buf, "The #7warmup#n to %s's dissonant attack does #y[#L%d#y]#n damage to you.\n\r", ch->name, dam[1]);
   send_to_char(buf, victim);
   sprintf(buf, "The #7body#n of %s's dissonant onslaught does #[#L%d#y]#n damage to you.\n\r", ch->name, dam[2]);
   send_to_char(buf, victim);
   sprintf(buf, "%s's dissonant song #7outro#n deals #y[#L%d#y]#n damage to you.\n\r", ch->name, dam[3]);
   send_to_char(buf, victim);
   sprintf(buf, "You take #y[#L%d#y]#n damage from %s's dissonant #7aftermath.#n\n\r", dam[4], ch->name);
   send_to_char(buf, victim);
   WAIT_STATE(ch, 36);
   return;
  }
  else if (!str_cmp(arg1, "paeon"))
  {
   if (IS_NPC(victim))
   {
    send_to_char("Only on PCs.\n\r", ch);
    return;
   }
   if (ch->pcdata->powers[BARD_LESSONS] < 5)
   {
    send_to_char("You need to take more lessons.\n\r", ch);
    return;
   }
   if (victim->pcdata->songs[SONG_PAEON] > 0)
   {
    send_to_char("They're regenerating quickly already.\n\r", ch);
    return;
   }
   sprintf(buf, "#nYou successfully cause %s to regenerate quickly with your #Cpaeon#n.\n\r", victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "#n%s successfully causes you to regenerate quickly with their #Cpaeon#n.\n\r", ch->name);
   send_to_char(buf, victim);
   victim->pcdata->songs[SONG_PAEON] += ch->pcdata->powers[SONG_MASTERY];
   return;
  }
  else if (!str_cmp(arg1, "ballad"))
  {
   if (IS_NPC(victim))
   {
    send_to_char("Only on players.\n\r", ch);
    return;
   }
   if (ch->pcdata->powers[BARD_LESSONS] < 6)
   {
    send_to_char("You need to take more lessons.\n\r", ch);
    return;
   }
   if (victim->pcdata->songs[SONG_BALLAD] > 0)
   {
    send_to_char("They're regenerating mana fiercely already.\n\r", ch);
    return;
   }
   sprintf(buf, "#nYou cause %s to regenerate mana fiercely with your #Pballad#n.\n\r", victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "#n%s causes you to regenerate mana fiercely with their #Pballad#n.\n\r", ch->name);
   send_to_char(buf, victim);
   victim->pcdata->songs[SONG_BALLAD] += ch->pcdata->powers[SONG_MASTERY];
   return;
  }
  else if (!str_cmp(arg1, "hymnus"))
  {
   if (IS_NPC(victim))
   {
    send_to_char("Only PCs.\n\r", ch);
    return;
   }
   if (ch->pcdata->powers[BARD_LESSONS] < 8)
   { 
    send_to_char("You need to take more lessons.\n\r", ch);
    return;
   }
   if (victim->pcdata->songs[SONG_HYMNUS] > 0)
   {
    send_to_char("They are already suffering from it.\n\r", ch);
    return;
   }
   sprintf(buf, "#nYou degrade %s and their ability to regenerate mana with your #7hymnus#n.\n\r", victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "#n%s degrades your ability to regenerate mana with their #7hymnus#n.\n\r", victim->name);
   send_to_char(buf, victim);
   victim->pcdata->songs[SONG_HYMNUS] += ch->pcdata->powers[SONG_MASTERY];
   return;
  }
  else if (!str_cmp(arg1, "march"))
  {
   if (IS_NPC(victim))
   {
    send_to_char("Only on players.\n\r", ch);
    return;
   }
   if (ch->pcdata->powers[BARD_LESSONS] < 3)
   {
    send_to_char("You need to take more lessons.\n\r", ch);
    return;
   }
   if (victim->pcdata->songs[SONG_MARCH] > 0)
   {
    send_to_char("They're already marching!\n\r", ch);
    return;
   }
   sprintf(buf, "#nYou sing a wonderful #ymarch#n to %s\n\r", victim->name);
   send_to_char(buf, ch);
   sprintf(buf, "#n%s sings a wonderful #ymarch#n to you.\n\r", ch->name);
   send_to_char(buf, victim);
   victim->pcdata->songs[SONG_MARCH] += (ch->pcdata->powers[SONG_MASTERY]*50);
   return;
  }
 return; 
}

void do_songs(CHAR_DATA *ch, char *argument)
{
 char uta[MAX_INPUT_LENGTH];
 char buf[MAX_INPUT_LENGTH];
 int song = 0;


if (!IS_RACE(ch, RACE_ELF) && !IS_RACE(ch, RACE_WILDELF)
&& !IS_RACE(ch, RACE_HALFELF) && !IS_RACE(ch, RACE_DROW))
{
 send_to_char("Huh?\n\r", ch);
 return;
}

 for (song = 0; song < 9; song++)
 {
  switch(song)
  {
   case 0:
    sprintf(uta, "Minuet:   ");
    break;
   case 2:
    sprintf(uta, "Madrigal: ");
    break;
   case 1:
    sprintf(uta, "Minne:    ");
    break;
   case 3:
    sprintf(uta, "March:    ");
    break;
   case 4:
    sprintf(uta, "Requiem:  ");
    break;
   case 5:
    sprintf(uta, "Paeon:    ");
    break;
   case 6:
    sprintf(uta, "Ballad:   ");
    break;
   case 7:
    sprintf(uta, "Lullaby:  ");
    break;
   case 8:
    sprintf(uta, "Hymnus:   ");
    break;
  }
  sprintf(buf, "%s %d\n\r", uta, ch->pcdata->songs[song]);
  send_to_char(buf, ch);
 }
 sprintf(buf, "Mastery:   %d\n", ch->pcdata->powers[SONG_MASTERY]);
 send_to_char(buf, ch);
 return;
}

void do_toughskin (CHAR_DATA * ch, char *argument)
{

  if (IS_NPC (ch))
    return;

	if (!IS_CLASS (ch, RACE_TROLL) && !IS_RACE(ch, RACE_HALFTROLL)
	&& !IS_RACE(ch, RACE_ARCHANGEL) && !IS_RACE(ch, RACE_ARCHDEMON)
	&& !IS_RACE(ch, RACE_DRAGONKIND))
    {
      stc ("Huh?\n\r", ch);
      return;
    }

  if (!IS_SET (ch->newbits2, NEW2_TOUGHSKIN))
    {
      stc ("Your skin grows stronger.\n\r", ch);
      SET_BIT (ch->newbits2, NEW2_TOUGHSKIN);
    }
  else
    {
      stc ("Your skin looses its leathery texture.\n\r", ch);
      REMOVE_BIT (ch->newbits2, NEW2_TOUGHSKIN);

    }
return;
}

