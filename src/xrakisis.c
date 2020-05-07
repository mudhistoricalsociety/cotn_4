#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include <fcntl.h>
#include <stdarg.h>
#include "merc.h"
#include "interp.h"



void do_convert ( CHAR_DATA *ch, char *argument) // Recoded by NebuSoft
{
  char arg1[MAX_INPUT_LENGTH];

  argument = one_argument( argument, arg1);

  if (IS_NPC(ch)) return;

  if (arg1[0] == '\0' || (str_cmp(arg1,"exp") && str_cmp(arg1,"bones")))
  {
    send_to_char("convert <bones|exp>\n\r",ch);
    return;
  }
  if (!str_cmp(arg1,"bones"))
  { 
    if ( ch->bones < 2500 )
    {
      send_to_char("You need at least 2500 bones to convert.\n\r",ch);
      return;
    }
    if ( ch->exp >= 1000000000 )
	{
	  send_to_char("You need to spend some of your XP first.\n\r",ch);
      return;
	}
    ch->exp += 1000000000;
    ch->bones -= 2500;
    send_to_char("You convert 2500 bones into 1 billion XP.\n\r",ch);
  }
  else if (!str_cmp(arg1,"exp"))
  { 
    if ( ch->exp < 1000000000 )
    {
      send_to_char("You need at least 1,000,000,000 exp to convert.\n\r",ch);
      return;
    }
    
    ch->exp -= 1000000000;
    ch->bones += 2500;
    send_to_char("You convert 1 billion XP into 2500 bones\n\r",ch);
  }
  return;
}




void do_prof(CHAR_DATA * ch, char *argument)
{
  char arg1[MAX_STRING_LENGTH];
  char buf[MSL];
  argument = one_argument( argument, arg1 );

  if (IS_NPC(ch))return;


if ( arg1[0] == '\0' )
{
  send_to_char("#0 Slash, Slice, Pound, Crush, Grep, Bite, Unarmed, Pierce, Suck, Blast, Claw, Whip, Stab..\n\r",ch);
  return;
}

if (ch->pcmaxwpnprof >= 3)
{
  send_to_char(" You may only have 3 weapon proficiencies \n\r",ch);
  return;
}

if (!str_cmp(arg1,"slash"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_SLASH))
	{
	send_to_char("#0 You already have Slash proficiency\n\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_SLASH);
	send_to_char("#0 You now have Slash proficiency\n\r",ch);
	return;
	}
}
else if (!str_cmp(arg1,"slice"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_SLICE))
	{
	send_to_char("#0 You already have Slice proficiency\n\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_SLICE);
	send_to_char("#0 You now have Slice proficiency\n\r",ch);
	return;
	}
}
else if (!str_cmp(arg1,"pound"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_POUND))
	{
	send_to_char("#0 You already have Pound proficiency\n\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_POUND);
	send_to_char("#0 You now have Pound proficiency\n\r",ch);
	return;
	}
}
else if (!str_cmp(arg1,"crush"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_CRUSH))
	{
	send_to_char("#0 You already have Crush proficiency\n\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_CRUSH);
	send_to_char("#0 You now have Crush proficiency\n\r",ch);
	return;
	}
}
else if (!str_cmp(arg1,"grep"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_GREP))
	{
	send_to_char("#0 You already have Grep proficiency\n\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_GREP);
	send_to_char("#0 You now have Grep proficiency\n\r",ch);
	return;
	}
}
else if (!str_cmp(arg1,"bite"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_BITE))
	{
	send_to_char("#0 You already have Bite proficiency.\n\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_BITE);
	send_to_char("#0 You now have Bite proficiency.\n\r",ch);
	return;
	}
}
else if (!str_cmp(arg1,"unarmed"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_UNARM))
	{
	send_to_char("#0 You already have Hand-To-Hand proficiency.\n\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_UNARM);
	send_to_char("#0 You now have Unarmed proficiency.\n\r",ch);
	return;
	}
}
else if (!str_cmp(arg1,"pierce"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_PIERCE))
	{
	send_to_char("#0 You already have Dagger Proficiency.\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_PIERCE);
	send_to_char("#0 You now have Pierce proficiency\n\r",ch);
	return;
	}
}
else if (!str_cmp(arg1,"suck"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_SUCK))
	{
	send_to_char("#0 You already have Suck proficiency\n\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_SUCK);
	send_to_char("#0 You now have Suck proficiency\n\r",ch);
	return;
	}
}
else if (!str_cmp(arg1,"blast"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_BLAST))
	{
	send_to_char("#0 You already have Blast proficiency\n\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_BLAST);
	send_to_char("#0 You now have Blast proficiency\n\r",ch);
	return;
	}
}
else if (!str_cmp(arg1,"claw"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_CLAW))
	{
	send_to_char("#0 You already have Claw proficiency\n\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_CLAW);
	send_to_char("#0 You now have Claw proficiency\n\r",ch);
	return;
	}
}
else if (!str_cmp(arg1,"whip"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_WHIP))
	{
	send_to_char("#0 You already have Whip proficiency\n\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_WHIP);
	send_to_char("#0 You now have Whip proficiency\n\r",ch);
	return;
	}
}
else if (!str_cmp(arg1,"stab"))
{
	if (IS_SET(ch->pcwpnprof, WPNPROF_STAB))
	{
	send_to_char("#0 You already have Stab proficiency\n\r",ch);
	return;
	}
	else
	{
	ch->pcmaxwpnprof++;
	SET_BIT(ch->pcwpnprof, WPNPROF_STAB);
	send_to_char("#0 You now have Stab proficiency\n\r",ch);
	return;
	}
}
return;
}

void do_level(CHAR_DATA * ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char skill[20];
  char buf[MAX_STRING_LENGTH];
    char stance [20];
    float chgen = 0;
    float chgenexp = 0;
    int percent = 0;
    int nextgen = 0;
    int expneeded = 0;
    one_argument( argument, arg );
    ROOM_INDEX_DATA *room;


  one_argument(argument, arg);

  if (IS_NPC(ch))
    return;

	
stc("#0X#r==============================#0[#rW#Reapon #rL#Revels#0]#r==============================#0X#n\n\r",ch);
  sprintf(skill, "#r|#0   Saber   #7: %-4d", ch->wpn[1]);   // slice
  send_to_char(skill, ch);
  sprintf(skill, "#0  Shiv   #7: %-4d", ch->wpn[2]);  // stab
  send_to_char(skill, ch);
  sprintf(skill, "#0 One-Handed Longsword  #7: %-4d", ch->wpn[3]);  // slash
  send_to_char(skill, ch);
  sprintf(skill, "#0Whip #7: %-4d   #r|#n\n\r", ch->wpn[4]); // whip
  send_to_char(skill, ch);
  sprintf(skill, "#r|#0   Claw    #7: %-4d", ch->wpn[5]);  // claw
  send_to_char(skill, ch);
  sprintf(skill, "#0  Blast  #7: %-4d", ch->wpn[6]);  // blast
  send_to_char(skill, ch);
  sprintf(skill, "#0 WarHammer  #7: %-4d", ch->wpn[7]); // pound
  send_to_char(skill, ch);
  sprintf(skill, "#0           Mace #7: %-4d   #r|#n\n\r", ch->wpn[8]); // crush
  send_to_char(skill, ch);
  sprintf(skill, "#r|#0   Grep    #7: %-4d", ch->wpn[9]);  // grep
  send_to_char(skill, ch);
  sprintf(skill, "#0  Fang   #7: %-4d", ch->wpn[10]);  // bite
  send_to_char(skill, ch);
  sprintf(skill, "#0 Dagger #7: %-4d", ch->wpn[11]); // pierce
  send_to_char(skill, ch);
  sprintf(skill, "#0               Vampiric #7:%-4d#r|#n\n\r", ch->wpn[12]); // suck
  send_to_char(skill, ch);
  sprintf(skill, "#r|#0   Hand-to-Hand #7: %-4d                                                     #r|#n\n\r", ch->wpn[0]);
  send_to_char(skill, ch);
	stc("#0X#r==============================#0[#rS#Rtance #rL#Revels#0]#r==============================#0X\n\r",ch);
	sprintf(skill,"#r|#0    Viper #7%-4d    #0Crane #7%-4d     #0Crab #7%-4d    #0Mongoose #7%-4d  #0PitBull #7%-4d#r  |\n\r",ch->stance[1],ch->stance[2],ch->stance[3],ch->stance[4],ch->stance[5]);
	stc(skill,ch);
	sprintf(skill,"#r|#0    Hawk   #7%-4d   #0Badger #7%-4d    #0Cougar #7%-4d  #0Grizzly  #7%-4d  #0Wolverine #7%-4d#r|\n\r",ch->stance[6],ch->stance[7],ch->stance[8],ch->stance[10],ch->stance[9]);
	stc(skill,ch);
	stc("#0X#r==============================#0[#rS#Ruper #rS#Rtances#0]#r==============================#0X#n\n\r",ch);
	sprintf(skill,"#r|#0      SS1 #7%-4d      #0SS2 #7%-4d     #0SS3 #7%-4d       #0Pizza #7%-4d  #0Zarius #7%-4d#r   |#n\n\r",ch->stance[13],ch->stance[14],ch->stance[15],ch->stance[16],ch->stance[17]);
	stc(skill,ch);
	stc("#r|                                                                           |\n\r",ch);
	stc("#r|       #7To gain super stances, you must max all your current stances.       #r|\n\r",ch);
	stc("#r|      #7After this, use 'setstance' to beging editing your superstance.      #r|\n\r",ch);
	
stc("#0X#r==============================#0[#rS#Rpell #rL#Revels#0]#r===============================#0X#n\n\r",ch);
  sprintf(skill, "#r|   #pPurple#n #C: %-4d", ch->spl[0]);  
  send_to_char(skill, ch);
  sprintf(skill, "   #lBlue#n #C: %-4d", ch->spl[2]);
  send_to_char(skill, ch);
  sprintf(skill, "   #rRed#n #C: %-4d", ch->spl[1]);  
  send_to_char(skill, ch);
  sprintf(skill, "   #gGreen#n #C: %-4d", ch->spl[3]);
  send_to_char(skill, ch);
  sprintf(skill, "   #yYellow#n #C: %-4d #r|#n\n\r", ch->spl[4]);
  send_to_char(skill, ch);
	
stc("#0X#r==============================#0[#r+#R-#r+#R-#r+#R-#r+#R-#r+#R-#r+#R-#r+#0]#r==============================#0X#n\n\r",ch);

	stc("#0X#r==============================#0[#r+#R-#r+#R-#r+#R-#r+#R-#r+#R-#r+#R-#r+#0]#r==============================#0X#n\n\r",ch);

	stc("#0x#r----#R=====#r----#R=====#r----#R=====#r----#0x#n\n\r",ch);
   xprintf(buf, "#0You are currently generation #W%d#n\n\r",
                        ch->generation);
                send_to_char(buf, ch);
                chgen = ch->generation;
                chgenexp = ch->genexp;
                if (ch->generation == 6)
                        expneeded = 13000000.00;
                else if (ch->generation == 5)
                        expneeded = 150000000.00;
                else if (ch->generation == 4)
                        expneeded = 400000000.00;
                else if (ch->generation == 3)
                        expneeded = 900000000.00;
                else if (ch->generation == 2)
                        expneeded = 1800000000.00;
                percent = (chgenexp / expneeded) * 100.00;

	nextgen = (ch->generation - 1);
	if (ch->generation > 1 && ch->level > 2)
	{
	sprintf(skill,"#0You are #R%d#W%% #0on your way to generation #R%d.#n\n\r",percent,nextgen);
	stc(skill,ch);
	}
	
	stc("#0x#r----#R=====#r----#R=====#r----#R=====#r----#0x#n\n\r",ch);
	if ( ch->stance[12] == 1 )
      sprintf(stance, " Viper" );
     else if ( ch->stance[12] == 2 )
      sprintf(stance, " Crane" );
     else if ( ch->stance[12] == 3 )
      sprintf(stance, " Crab" );  
     else if ( ch->stance[12] == 4 )
      sprintf(stance, " Mongoose" );  
     else if ( ch->stance[12] == 5 )
      sprintf(stance, " Pitbull" );  
     else if ( ch->stance[12] == 6 )
      sprintf(stance, " Hawk" );  
     else if ( ch->stance[12] == 7 )
      sprintf(stance, " Badger" );  
     else if ( ch->stance[12] == 8 )
      sprintf(stance, " Cougar" );  
     else if ( ch->stance[12] == 9 )
      sprintf(stance, " Wolverine" );  
     else if ( ch->stance[12] == 10 )
      sprintf(stance, " Grizzly" ); 
     else if ( ch->stance[12] == 11 )
      sprintf(stance, " Wolf" );
     else if ( ch->stance[12] == 13 )
      sprintf( stance, " SS1");
     else if ( ch->stance[12] == 14 )
      sprintf( stance, " SS2");
     else if ( ch->stance[12] == 15 )
      sprintf( stance, " SS3");
     else if ( ch->stance[12] == 16 )
      sprintf( stance, " Pizza");
     else if ( ch->stance[12] == 17 )
      sprintf( stance, " Zarius");
     else if ( ch->stance[12] == -1 )
      sprintf(stance, " None" );
     else
      sprintf(stance, " None" );
	sprintf(skill, "#0Autostance#n:  #R%s #n\n\r",stance);
     send_to_char(skill, ch);
    if (IS_SET(ch->act, PLR_RIGHTHAND))
      send_to_char("#wYou favor your right arm in combat.#n\n\r",ch);
    else if(IS_SET(ch->act, PLR_LEFTHAND))
      send_to_char("#wYou favor your left arm in combat.#n\n\r",ch);
    else
      send_to_char("#wYou fight well with both your arms.#n\n\r",ch);

    stc("#0x#r----#R=====#r----#R=====#r----#R=====#r----#0x#n\n\r",ch);
    
                if (IS_CLASS(ch, CLASS_DEMON))
                        send_to_char("You are a demon!\n\r", ch);
                if (IS_CLASS(ch, CLASS_MAGE))
                        send_to_char("You are a battlemage!\n\r", ch);
                if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
                        send_to_char("You are a shapeshifter!\n\r", ch);
                if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                        send_to_char("You are an undead knight!\n\r", ch);
                if (IS_CLASS(ch, CLASS_WEREWOLF))
                        send_to_char("You are a werewolf!\n\r", ch);
                if (IS_CLASS(ch, CLASS_NINJA))
                        send_to_char("You are a ninja!\n\r", ch);
                if (IS_CLASS(ch, CLASS_VAMPIRE))
                        send_to_char("You are a vampire!\n\r", ch);
                if (IS_CLASS(ch, CLASS_SAMURAI))
                        send_to_char("You are a Samurai!\n\r", ch);
                if (IS_CLASS(ch, CLASS_DROW))
                        send_to_char("You are a drow!\n\r", ch);
                if (IS_CLASS(ch, CLASS_ANGEL))
                        send_to_char("You are an angel!\n\r", ch);
                if (IS_CLASS(ch, CLASS_LICH))
                        send_to_char("You are a lich!\n\r", ch);
                if (IS_CLASS(ch, CLASS_SKYBLADE))
                        send_to_char("You are a SkyBlade!!\n\r", ch);
                if (IS_CLASS(ch, CLASS_GHOUL))
                        send_to_char
                                ("You are a foul smelling #GGhoul#n!!\n\r",
                                 ch);
                if (IS_CLASS(ch, CLASS_DRACONIAN))
                        send_to_char("You are a Draconian!!\n\r", ch);
                if (IS_CLASS(ch, CLASS_TANARRI))
                        send_to_char
                                ("You are a warrior of The Bloodwars.\n\r",
                                 ch);
                if (IS_CLASS(ch, CLASS_MONK))
                        stc("You are a monk!\n\r", ch);
                if (IS_CLASS(ch, CLASS_CYBORG))
                        stc("You are a Mix Between Man and Machine!\n\r", ch);
                if (IS_CLASS(ch, CLASS_SHADOW))
                        stc("You are a Shadow!\n\r", ch);
                if (IS_CLASS(ch, CLASS_PRIEST))
                        stc("You are a holy Priest\n\r", ch);
                if (IS_CLASS(ch, CLASS_JEDI))
                        stc("You are a Jedi Warrior!\n\r", ch);
    if (IS_CLASS(ch, CLASS_BLADE_MASTER))
     send_to_char("You are a Blade Master.\n\r", ch);
     if ( IS_CLASS(ch, CLASS_DRAGON) )
     send_to_char(" You are a dragon!                      \n\r", ch);    
    if (IS_CLASS(ch, CLASS_PALADIN))
     send_to_char("You are a Paladin.\n\r", ch);
     if ( IS_CLASS(ch, CLASS_ZOMBIE) )
     send_to_char(" You are a Zombie!                      \n\r", ch);    
    if (IS_CLASS(ch, CLASS_SHINOBI))
     send_to_char("You are a Shinobi.\n\r", ch);
     if ( IS_CLASS(ch, CLASS_ELEMENTAL) )
     send_to_char(" You are a Elemental!                      \n\r", ch);    

                if (IS_CLASS(ch, CLASS_TANARRI))
                {
                        if (ch->pcdata->rank == TANARRI_FODDER)
                                send_to_char
                                        ("You are nothing but fodder for the great war.\n\r",
                                         ch);
                        if (ch->pcdata->rank == TANARRI_FIGHTER)
                                send_to_char
                                        ("You are a fighter in the great war.\n\r",
                                         ch);
                        if (ch->pcdata->rank == TANARRI_ELITE)
                                send_to_char
                                        ("You are an elite warrior in the great war.\n\r",
                                         ch);
                        if (ch->pcdata->rank == TANARRI_CAPTAIN)
                                send_to_char
                                        ("You are a captain in the great war.\n\r",
                                         ch);
                        if (ch->pcdata->rank == TANARRI_WARLORD)
                                send_to_char
                                        ("You are a warlord in the great war.\n\r",
                                         ch);
                        if (ch->pcdata->rank == TANARRI_BALOR)
                                send_to_char("You are a Tanar'ri Balor.\n\r",
                                             ch);
                }

                if (IS_CLASS(ch, CLASS_VAMPIRE))
                {

                        if (ch->pcdata->rank == AGE_TRUEBLOOD)
                                send_to_char("You are a TrueBlood!\n\r", ch);
                        else if (ch->pcdata->rank == AGE_LA_MAGRA)
                                send_to_char("You are a La Magra!\n\r", ch);
                        else if (ch->pcdata->rank == AGE_METHUSELAH)
                                send_to_char("You are a Methuselah!\n\r", ch);
                        else if (ch->pcdata->rank == AGE_ELDER)
                                send_to_char("You are an Elder!\n\r", ch);
                        else if (ch->pcdata->rank == AGE_ANCILLA)
                                send_to_char("You are an Ancilla!\n\r", ch);
                        else if (ch->pcdata->rank == AGE_NEONATE)
                                send_to_char("You are a Neonate!\n\r", ch);
                        else
                                send_to_char("You are a Childe!\n\r", ch);
                }
    room = get_room_index( ch->home );
    
	sprintf(skill, "#0Recall Room#R : #Y%s#n\n\r", room->name);
    send_to_char( skill, ch );
	stc("#0x#r----#R=====#r----#R=====#r----#R=====#r----#0x#n\n\r",ch);	

  return;
}




int find_spell( CHAR_DATA *ch, const char *name )
{
    /* finds a spell the character can cast if possible */
    int sn, found = -1;

    if (IS_NPC(ch))
	return skill_lookup(name);

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if (skill_table[sn].name == NULL)
	    break;
	if (LOWER(name[0]) == LOWER(skill_table[sn].name[0])
	&&  !str_prefix(name,skill_table[sn].name))
	{
	    if ( found == -1)
		found = sn;
	    if (ch->pcRaceLevel >= skill_table[sn].race_level
	    &&  ch->pcdata->learned[sn] > 0)
		    return sn;
	}
    }
    return found;
}



void do_rompractice( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	int col;

	col    = 0;
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    if ( ch->pcRaceLevel < skill_table[sn].race_level)
	 //     || ch->pcdata->learned[sn] < 1 /* skill is not known */)
		continue;
	    if ( ch->level < skill_table[sn].skill_level )
		continue;

	    sprintf( buf, "%-18s %3d%%  ",
		skill_table[sn].name, ch->pcdata->learned[sn] );
	    send_to_char( buf, ch );
	    if ( ++col % 3 == 0 )
		send_to_char( "\n\r", ch );
	}

	if ( col % 3 != 0 )
	    send_to_char( "\n\r", ch );

	sprintf( buf, "You have %d practice sessions left.\n\r",
	    ch->pcpractice );
	send_to_char( buf, ch );
    }
    else
    {
	CHAR_DATA *mob;
	int adept;

	if ( !IS_AWAKE(ch) )
	{
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    return;
	}

/*	for ( mob = ch->in_room->people; mob != NULL; mob = mob->next_in_room )
	{
	    if ( IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) )
		break;
	}

	if ( mob == NULL )
	{
	    send_to_char( "You can't do that here.\n\r", ch );
	    return;
	}
*/
	if ( ch->pcpractice <= 0 )
	{
	    send_to_char( "You have no practice sessions left.\n\r", ch );
	    return;
	}


	if ( ( sn = find_spell( ch,argument ) ) < 0
	|| ( !IS_NPC(ch)
	&&   (ch->pcRaceLevel < skill_table[sn].race_level)))
// 	||    ch->pcdata->learned[sn] < 1))) /* skill is not known */
//	||    skill_table[sn].rating[ch->class] == 0)))
	{
	    send_to_char( "You can't practice that.\n\r", ch );
	    return;
	}

//	adept = IS_NPC(ch) ? 100 : class_table[ch->class].skill_adept;
	adept = 75;

	if ( ch->pcdata->learned[sn] >= adept )
	{
	    sprintf( buf, "You are already learned at %s.\n\r",
		skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else
	{
	    ch->pcpractice--;
	    ch->pcdata->learned[sn] += 5;
//		int_app[get_curr_int(ch)].learn / 
//	        skill_table[sn].rating[ch->class];
	    if ( ch->pcdata->learned[sn] < adept )
	    {
		act( "You practice $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
		act( "$n practices $T.",
		    ch, NULL, skill_table[sn].name, TO_ROOM );
	    }
	    else
	    {
		ch->pcdata->learned[sn] = adept;
		act( "You are now learned at $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
		act( "$n is now learned at $T.",
		    ch, NULL, skill_table[sn].name, TO_ROOM );
	    }
	}
    }
    return;
}



char *num_punct(int foo)
{
int index,index_new,rest;
char buf[16];
static char buf_new[16];

sprintf(buf,"%d",foo);
rest = strlen(buf)%3;

for (index=index_new=0;index<strlen(buf);index++,index_new++)
  {
   if (index!=0 && (index-rest)%3==0 )
    {
     buf_new[index_new]=',';
     index_new++;
     buf_new[index_new]=buf[index];
    }
  else
  buf_new[index_new] = buf[index];
  }
buf_new[index_new]='\0';
return buf_new;
}



void do_jail( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MSL];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
	int jtime;
	
    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Jail whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ( IS_NPC(victim) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }

    /*if ( get_trust( victim ) > get_trust( ch ) )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }*/


	//How long do you want them jailed?
	if(is_number(arg2)){
	jtime = atoi( arg2 );
	}
	else{
	jtime = 120;
	}
	

    victim->jailtime = jtime;

//    if((IS_SET(ch->newbits2, PLR_UINVIS) || IS_SET(ch->act, PLR_WIZINVIS)) && ch->level > 6)
//    {
//	sprintf(buf,"#C%s #Lgets beaten half to death by an Immortal, then is led to #0prison#l.", victim->name);
// 	do_info(ch,buf);
//    }
   // {
	sprintf(buf,"#C%s #Lgets beaten half to death by %s, then is led to #0prison#l.", victim->name,ch->name);
    	do_info(ch, buf);//    }
    send_to_char( "OK.\n\r", ch );
    stop_fighting(victim,TRUE);
    do_transfer(victim, "self 5");
    SET_BIT(victim->newbits2, IS_JAILED);
    return;
}

void do_freechar( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Unjail whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ( IS_NPC(victim) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }
    

    sprintf(buf,"#y%s #Lhas been released from #0jail#l.#n", victim->name);
    REMOVE_BIT(victim->newbits2, IS_JAILED);
    do_info(ch, buf);
    send_to_char( "OK.\n\r", ch );
    stop_fighting(victim,TRUE);
	victim->jailtime = -1;
    do_transfer(victim, "self 3054");
    return;
}

void do_warden( CHAR_DATA *ch, char *argument )
{
	char buf[MSL];
	CHAR_DATA *gch;

	if( IS_NPC(ch)) return;

	stc("#R[#r--#r------------#r---#RWarden's List#r---------#r----------#R]#n\n\r",ch);
	stc("#R[#r  #yName        #r|     #RTime        #r|      #RType      #R]#n\n\r",ch);
	stc("#R[#r--#r------------#r------#r------------#r-------#r----------#R]#n\n\r",ch);

	for(gch=char_list;gch != NULL;gch=gch->next){
		if(IS_NPC(gch)) continue;
		if(
		   (gch->jailtime <= 0 && gch->in_room->vnum != 10162)
		&& gch->siletime <= 0 && gch->freetime <= 0 && gch->hallutime <= 0
 		) continue;

		if(gch->jailtime > 0){
			sprintf(buf,"#R[ #y%-12s #r| #R%-15d #r| #R%-14s #R]#n\n\r",gch->pcdata->switchname,gch->jailtime,"Jail");
			stc(buf,ch);
		}
		else if(gch->in_room->vnum == 10162){
			sprintf(buf,"#R[ #y%-12s #r| #R%-15s #r| #R%-14s #R]#n\n\r",gch->pcdata->switchname,"No Time Set","Jail");
			stc(buf,ch);
		}

		if(gch->hallutime > 0){
			sprintf(buf,"#R[ #y%-12s #r| #R%-15d #r| #R%-14s #R]#n\n\r",gch->pcdata->switchname,gch->hallutime,"Hallucination");
			stc(buf,ch);
		}

		if(gch->siletime > 0){
			sprintf(buf,"#R[ #y%-12s #r| #R%-15d #r| #R%-14s #R]#n\n\r",gch->pcdata->switchname,gch->siletime,"Silence");
			stc(buf,ch);
		}

		if(gch->freetime > 0){
			sprintf(buf,"#R[ #y%-12s #r| #R%-15d #r| #R%-14s #R]#n\n\r",gch->pcdata->switchname,gch->freetime,"Freeze");
			stc(buf,ch);
		}

	}
	stc("#R[#r-------------------------------------------------#R]#n\n\r",ch);
	return;
}

// dystopia call all
void call_all( CHAR_DATA *ch )
{
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    CHAR_DATA *victim = NULL;
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *objroom;
    bool found = FALSE;

    

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( obj->questowner == NULL || strlen(obj->questowner) < 2 || 
	    str_cmp( ch->pcdata->switchname, obj->questowner ) || obj->item_type == ITEM_PAGE)
	    continue;

	found = TRUE;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if (in_obj->carried_by != NULL)
	{
	    if (in_obj->carried_by == ch) continue;
            if ((gch = in_obj->carried_by) != NULL)
            {
              if (gch->desc && gch->desc->connected != CON_PLAYING) continue;
            }
	}

	if (obj->carried_by != NULL && obj->carried_by != ch)
	{
	    if (obj->carried_by == ch || obj->carried_by->desc == NULL || 
		obj->carried_by->desc->connected != CON_PLAYING) 
	    {
		if (!IS_NPC(obj->carried_by)) return;
	    }
	    act("$p suddenly vanishes from your hands!",obj->carried_by,obj,NULL,TO_CHAR);
	    act("$p suddenly vanishes from $n's hands!",obj->carried_by,obj,NULL,TO_ROOM);
	    SET_BIT(obj->carried_by->extra, EXTRA_CALL_ALL);
	    obj_from_char(obj);
	}
	else if (obj->in_room != NULL)
	{
	    chroom = ch->in_room;
	    objroom = obj->in_room;
	    char_from_room(ch);
	    char_to_room(ch,objroom);
	    act("$p vanishes from the ground!",ch,obj,NULL,TO_ROOM);
	    if (chroom == objroom) act("$p vanishes from the ground!",ch,obj,NULL,TO_CHAR);
	    char_from_room(ch);
	    char_to_room(ch,chroom);
	    obj_from_room(obj);
	}
	else if (obj->in_obj != NULL) obj_from_obj(obj);
	else continue;
	obj_to_char(obj,ch);
	if (IS_SET(obj->extra_flags,ITEM_SHADOWPLANE))
	    REMOVE_BIT(obj->extra_flags,ITEM_SHADOWPLANE);
	if (!IS_HEAD(ch, LOST_HEAD))
	{
	    act("$p materializes in your hands.",ch,obj,NULL,TO_CHAR);
	    act("$p materializes in $n's hands.",ch,obj,NULL,TO_ROOM);
	}
    }

    if ( !found && !IS_HEAD(ch, LOST_HEAD) )
	send_to_char( "Nothing happens.\n\r", ch );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected != CON_PLAYING ) continue;
	if ( (victim = d->character) == NULL ) continue;
	if ( IS_NPC(victim) ) continue;
	if ( ch != victim && !IS_EXTRA(victim,EXTRA_CALL_ALL) ) continue;
	REMOVE_BIT(victim->extra, EXTRA_CALL_ALL);
	do_autosave(victim,"");
    }
    return;
}

char *getCurrRanking( CHAR_DATA *ch, int pRank )
{
   static char rank[MAX_STRING_LENGTH];
   rank[0] = '\0';

   switch( pRank ) {
      case  0: strcat( rank, "Serf"         ); break;
      case  1: strcat( rank, "Peasant"      ); break;
      case  2: strcat( rank, "Apprentice"   ); break;
      case  3: strcat( rank, "Wanderer"     ); break;
      case  4: strcat( rank, "Explorer"     ); break;
      case  5: strcat( rank, "Adventurer"   ); break;
      case  6: strcat( rank, "Veteran"      ); break;
      case  7: strcat( rank, "Champion"     ); break;
      case  8: strcat( rank, "Master"       ); break;
      case  9: strcat( rank, "Mystic"       ); break;
      case 10: strcat( rank, "Ancient"      ); break;
      case 11: strcat( rank, "Seer"         ); break;
      case 12: strcat( rank, "Savior"       ); break;
      case 13: strcat( rank, "Myth"         ); break;
      case 14: strcat( rank, "Legend"       ); break;
      case 15: strcat( rank, "Forgotten"    ); break;
      case 16: strcat( rank, "Chosen"       ); break;
      case 17: strcat( rank, "Avatar"       ); break;
      case 18: strcat( rank, "Omnipotent"   ); break;
      case 19: strcat( rank, "Hero"         ); break;
      case 20: strcat( rank, "Warlord"      ); break;
      case 21: strcat( rank, "Demi-God"     ); break;
      default: strcat( rank, "Invalid Rank" ); break;
   }
   return rank;
}



void do_nuke( CHAR_DATA *ch, char *argument)
{
 char strsave[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];

 one_argument(argument,arg);
 
        if (str_cmp(ch->pcdata->switchname, "Xrakisis")) {
        sprintf( buf, "ONLY FOR XRAK!'", ch->name);
         return;
        }

 if(arg[0] == '\0') 
 {
 send_to_char("Syntax: nuke <unlucky bastard>\n\r",ch);
 return;
 }
 if((victim = get_char_world(ch,arg)) == NULL)
 {
   send_to_char("Sorry! They have to be logged in.\n\r", ch);
   return;
 }

if(IS_NPC(victim))
{
 send_to_char("Sorry! Players only!\n\r",ch);
 return;
}

 if(IS_IMMORTAL(victim))
 {
  send_to_char("Nice try.. Gonna have to find a different way to do that.",ch);
  return;
 }

sprintf(strsave,"%s%s",PLAYER_DIR, capitalize(victim->name));
stop_fighting(victim,TRUE);
sprintf(buf, "OUCH! %s just nuked %s!", ch->pcdata->switchname,victim->pcdata->switchname);
do_info(ch, buf);
do_quit(victim,	"");

unlink(strsave);

return;
}

