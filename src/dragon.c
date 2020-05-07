/********************************************************
 *                                                      *
 * Coded by Corey Olney(Kip) and Justin C. Good(bohica) *
 * If you are going to use this code and did not get it *
 * from me, please tell me on ICQ #21355940 or AOL IM   *
 * kippykp.                                             *
 * (copyrighted 1998-1999)                              *
 * also send me improvements you make.                  *
 *                                                      *
 ********************************************************/
/********************************************************
 *                                                       *
 * Re-done by GryphonHawk for DarkSkies                  *
 *                                                       *
 * STAY OUT OF MY CODE!!!!!!!!!!                         * 
 *                                                       *
 * March 2003                                            *
 *                                                      *
 ********************************************************/

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


void do_drackform( CHAR_DATA *ch, char *argument)
{
	 char buf [MAX_STRING_LENGTH];
//	 char buf2 [MAX_STRING_LENGTH];
	 char AGE_STRING [MAX_STRING_LENGTH];

	 if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
	 {
		send_to_char( "Huh?\n\r",ch);
		return;
	 }
	 if (IS_POLYAFF(ch, POLY_DRAGON))
	 {
		  REMOVE_BIT(ch->polyaff, POLY_DRAGON);
		  REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
		  act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
		  act( "$n's shrinks back into human form.", ch, NULL, NULL, TO_ROOM );
		  ch->max_hit = (ch->max_hit - (ch->pcdata->powers[dragonage] * 550));
		  ch->hit = (ch->hit - (ch->pcdata->powers[dragonage] * 550));
		  if (ch->hit < 1) ch->hit = 1;
                  ch->pcdata->powers[10] = ch->pcdata->powers[DRAGON_COLOR];
		  ch->damroll -= 1000;
		  ch->hitroll -= 1000;
		  ch->armor   += 2000;
                  free_string( ch->morph );
		  ch->morph = str_dup( "" );
		  return;
	 }
	 else if (IS_AFFECTED(ch,AFF_POLYMORPH))
	 {
		send_to_char("You cant demon form when changed.\n\r",ch);
    	return;
	 }
	      if (ch->pcdata->powers[dragonage] == 1  ) sprintf(AGE_STRING,"Hatchling");
	 else if (ch->pcdata->powers[dragonage] == 2  ) sprintf(AGE_STRING,"Very Young");
	 else if (ch->pcdata->powers[dragonage] == 3  ) sprintf(AGE_STRING,"Young");
	 else if (ch->pcdata->powers[dragonage] == 4  ) sprintf(AGE_STRING,"Juvenile");
	 else if (ch->pcdata->powers[dragonage] == 5  ) sprintf(AGE_STRING,"Young Adult");
	 else if (ch->pcdata->powers[dragonage] == 6  ) sprintf(AGE_STRING,"Adult");
	 else if (ch->pcdata->powers[dragonage] == 7  ) sprintf(AGE_STRING,"Mature Adult");
	 else if (ch->pcdata->powers[dragonage] == 8  ) sprintf(AGE_STRING,"Old");
	 else if (ch->pcdata->powers[dragonage] == 9  ) sprintf(AGE_STRING,"Very Old");
	 else if (ch->pcdata->powers[dragonage] == 10 ) sprintf(AGE_STRING,"Elder");
	 else if (ch->pcdata->powers[dragonage] == 11 ) sprintf(AGE_STRING,"Venerable");
	 else if (ch->pcdata->powers[dragonage] == 12 ) sprintf(AGE_STRING,"Ancient");
         else if (ch->pcdata->powers[dragonage] == 13 ) sprintf(AGE_STRING,"Immortal");
	 else return;
//	sprintf(buf2,"%d",ch->pcdata->powers[DRAGON_COLOR]);
//	stc(buf2,ch);

	switch(ch->pcdata->powers[DRAGON_COLOR])
	 {
                default:
			if (ch->pcdata->powers[DRAGON_COLOR] == 0) 
			{
                           send_to_char("Your scales change hues.\n\r",ch);
                        }
                        ch->pcdata->powers[DRAGON_COLOR] = number_range(1,16);
			do_drackform(ch,"");
			return;
// that was left like that so if color was NOT set it would be reported
// and it could be traced where and when the problem was occureing.
// setting a default color is not so great an idea, if we get a major recurrence of the 
// problem we end up flooded with silvers.

                        break;
                case 1:
			sprintf(buf, "%s the %s #RRed#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 2:
			sprintf(buf, "%s the %s #GGreen#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 3:
			sprintf(buf, "%s the %s #LBlue#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 4:
			sprintf(buf, "%s the %s #oBronze#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 5:
			sprintf(buf, "%s the %s #oO#ypa#ol#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 6:
			sprintf(buf, "%s the %s #7White#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 7:
			sprintf(buf, "%s the %s #0Black#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 8:
			sprintf(buf, "%s the %s #cG#nre#cy#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 9:
			sprintf(buf, "%s the %s #0Obs#7id#0ian#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 10:
			sprintf(buf, "%s the %s #pP#Pur#ppl#Pe#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 11:
			sprintf(buf, "%s the %s #rC#0h#ra#0o#rs#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 12:
			sprintf(buf, "%s the %s #LS#la#Lp#lp#Lh#li#Lr#le#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 13:
			sprintf(buf, "%s the %s #GE#gm#Ge#gr#Ga#gl#Gd#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 14:
			sprintf(buf, "%s the %s #RR#ru#Rb#ry#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 15:
			sprintf(buf, "%s the %s #yT#oo#yp#oa#yz#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
		case 16:
			sprintf(buf, "%s the %s #7D#Ci#7a#Cm#7o#Cn#7d#n Dragon", ch->name,AGE_STRING);
			free_string( ch->morph );
			ch->morph = str_dup( buf );
			break;
/*                default: 
                        sprintf(buf, "%s the %s invalid color Dragon", ch->name,AGE_STRING);
                        free_string( ch->morph );
                        ch->morph = str_dup( buf );
                        break;*/
	 }

	 act( "You transform into a huge dragon.", ch, NULL, NULL, TO_CHAR );
	 act( "$n's body grows and distorts into a huge dragon.", ch, NULL, NULL, TO_ROOM );
//	 ch->pcdata->mod_str = 15;
//	 ch->pcdata->mod_dex = 15;
	 SET_BIT(ch->polyaff, POLY_DRAGON);
	 SET_BIT(ch->affected_by, AFF_POLYMORPH);

	 ch->max_hit += ch->pcdata->powers[dragonage] * 550;
	 ch->hit     += ch->pcdata->powers[dragonage] * 550;
	 ch->damroll += 1000;
	 ch->hitroll += 1000;
	 ch->armor   -= 2000;
	 return;
}

void do_hatch( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg [MAX_INPUT_LENGTH];

	 argument = one_argument( argument, arg );

	if (IS_NPC(ch)) return;

	if( !IS_IMMORTAL(ch) )
	{
    if (!IS_CLASS(ch, CLASS_DRAGON))
		{
			send_to_char( "Huh?\n\r", ch);
			return;
		}
		if( ch->pcdata->powers[dragonage] < D_ADULT)
		{
			send_to_char( "You must be an adult to create hatchlings.\n\r", ch );
			return;
		}
		if( ch->pcdata->powers[hatch] >= 3)
		{
			send_to_char( "You have already created 3 hatchlings.\n\r", ch );
			return;
		}
	}

    if ( arg[0] == '\0' )
    {
    send_to_char( "Hatch whom?\n\r", ch );
    return;
	 }

	 if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
    send_to_char( "They aren't here.\n\r", ch );
	 return;
    }

    if ( IS_NPC(victim) )
    {
    send_to_char( "Not on NPC's.\n\r", ch );
    return;
    }

    if ( ch == victim )
	 {
    send_to_char( "You cannot hatch yourself.\n\r", ch );
	 return;
	 }
	 if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
    send_to_char( "You can only hatch a dragon from avatars.\n\r", ch );
    return;
	 }

	if( IS_CLASS( victim, CLASS_DEMON ) )
	{
		send_to_char( "You can't hatch a dragon from a Demon.\n\r", ch );
		return;
	}
	if( IS_CLASS( victim, CLASS_MAGE ) )
	{
		send_to_char( "You can't hatch a dragon from a Mage.\n\r", ch );
		return;
	}
	if( IS_CLASS( victim, CLASS_WEREWOLF ) )
	{
		send_to_char( "You can't hatch a dragon from a Werewolf.\n\r", ch );
		return;
	}
	if( IS_CLASS( victim, CLASS_VAMPIRE ) )
	{
		send_to_char( "You can't hatch a dragon from a Vampire.\n\r", ch );
		return;
	}
	if( IS_CLASS( victim, CLASS_DROW ) )
	{
		send_to_char( "You can't hatch a dragon from a Drow.\n\r", ch );
		return;
	}
        if (ch->exp < 50000000 || ch->pcdata->stats[DEMON_CURRENT] < 5000000)
        {
          send_to_char("You need 50000000 exp and 5000000 dps to hatch a dragon.\n\r",ch);
          return;
        }
        if (victim->exp < 5000000)
        {
          send_to_char("You need 5000000 exp to be hatched.\n\r",victim);
          send_to_char("They need 5000000 exp to be hatched.\n\r",ch);
          return;
        }
       
	 act("You consume $N's essence and hatch a new egg!", ch, NULL, victim, TO_CHAR);
	 act("$n consumes $N's essence and hatch a new egg!", ch, NULL, victim, TO_NOTVICT);
	 act("$n consumes your essence and makes you a dragon!", ch, NULL, victim, TO_VICT);

	 victim->class=0;
	 victim->class=CLASS_DRAGON;
	 victim->pcdata->powers[dragonage] = 1;
	 victim->pcdata->powers[DRAGON_COLOR] = ch->pcdata->powers[DRAGON_COLOR];
	 if ( ch->pcdata->powers[DRAGON_COLOR] == 0 ) 
           victim->pcdata->powers[DRAGON_COLOR] = number_range (1, 15);
	 victim->special=0;
	 victim->level = LEVEL_AVATAR;
	 victim->class = CLASS_DRAGON;
	 ch->pcdata->powers[hatch] += 1;
         ch->pcdata->stats[DEMON_CURRENT] -= 5000000;
         ch->exp -= 50000000;
         victim->exp -= 5000000;
	 save_char_obj(ch);
	 save_char_obj(victim);
	 return;
}

void do_dgain( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	int count = 1;

	 argument = one_argument( argument, arg );

    if (!IS_CLASS(ch, CLASS_DRAGON))
	  {
       stc("You're not a dragon!\n\r",ch);
       return;
	  }

	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_WINGS )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FANGS )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LICK )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FROST_BREATH )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FIRE_BREATH )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LIGHTNING_BREATH )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_ACID_BREATH )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_GAS_BREATH )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TAIL )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TERROR )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CONFUSE )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_MASK )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_MAGIC )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_POLYMORPH )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LUCK )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FLY )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_SCRY )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_PEER )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TRUESIGHT )) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TOUGHNESS)) count += 1;
	if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CLAWS)) count += 1;

	if( IS_NPC(ch) ) return;

	if( !IS_IMMORTAL(ch))
	{
    if (!IS_CLASS(ch, CLASS_DRAGON))
	{
		send_to_char( "Huh?!?\n\r", ch );
		return;
	}
	}

	if( arg[0] == '\0' )
	{
		int x = 0;
		send_to_char( "Valid choices are:  ALL(shows skills learned)\n\r", ch );
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_WINGS ) && ch->pcdata->powers[dragonage] >= D_ADULT)
		{
			send_to_char( "Wings,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_SCRY ) )
		{
			send_to_char( " Scry,", ch );
			x++;
			if( x > 5){send_to_char("\n\r", ch);x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CLAWS ) )
		{
			send_to_char( " Claws,", ch );
			x++;
			if( x > 5){send_to_char("\n\r", ch);x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TRUESIGHT ) )
		{
			send_to_char(" Truesight,", ch );
			x++;
			if(x>5){send_to_char("\n\r", ch);x=0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_PEER ) )
		{
			send_to_char( " Peer,", ch );
			x++;
			if(x > 5){send_to_char("\n\r", ch);x=0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FANGS ) )
		{
			send_to_char( " Fangs,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LICK ) )
		{
			send_to_char( " Lick,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FROST_BREATH ) && ch->pcdata->powers[dragonage] >= D_MATURE)
		{
			send_to_char( " FrostBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FIRE_BREATH ) && ch->pcdata->powers[dragonage] >= D_JUVENILE)
		{
			send_to_char( " FireBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LIGHTNING_BREATH ) && ch->pcdata->powers[dragonage] >= D_YADULT )
		{
			send_to_char( " LightningBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_ACID_BREATH ) && ch->pcdata->powers[dragonage] >= D_VENERABLE )
		{
			send_to_char( " AcidBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_GAS_BREATH ) )
		{
			send_to_char( " GasBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TAIL ) && ch->pcdata->powers[dragonage] >= D_YOUNG )
		{
			send_to_char( " Tail,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TERROR ) )
		{
			send_to_char( " Terror,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CONFUSE ) )
		{
			send_to_char( " Confuse,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_MASK ) && ch->pcdata->powers[dragonage] >= D_OLD )
		{
			send_to_char( " Mask,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_MAGIC ) )
		{
			send_to_char( " Magic,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_POLYMORPH ) && ch->pcdata->powers[dragonage] >= D_IMMORTAL )
		{
			send_to_char( " Polymorph,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LUCK ) && ch->pcdata->powers[dragonage] >= D_ADULT )
		{
			send_to_char( " Luck,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FLY	) && ch->pcdata->powers[dragonage] >= D_ADULT )
		{
			send_to_char( " Fly,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TOUGHNESS) && ch->pcdata->powers[dragonage] >= D_ANCIENT)
		{
			send_to_char( " Scales,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		send_to_char( "\n\r", ch );
		send_to_char( "Some are only avaiable after a certain age. Check helps for ages.\n\r", ch );
		return;
	}
	else if( !str_cmp( arg, "all"	) )
	{
		int x = 0;
		send_to_char( "You have the folowing dragon powers:\n\r", ch );
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_WINGS ))
		{
			send_to_char( "Wings,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CLAWS	) )
		{
			send_to_char( " Claws,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}

		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FLY	) )
		{
			send_to_char( " Fly,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TRUESIGHT ) )
		{
			send_to_char(" Truesight,", ch );
			x++;
			if(x>5){send_to_char("\n\r", ch);x=0;}
		}
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_PEER ) )
		{
			send_to_char( " Peer,", ch );
			x++;
			if(x > 5){send_to_char("\n\r", ch);x=0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FANGS ))
		{
			send_to_char( " Fangs,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_SCRY ) )
		{
			send_to_char( " Scry,", ch );
			x++;
			if( x > 5){send_to_char("\n\r", ch);x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LICK ))
		{
			send_to_char( " Lick,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FROST_BREATH ))
		{
			send_to_char( " FrostBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FIRE_BREATH )) 
		{
			send_to_char( " FireBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LIGHTNING_BREATH )) 
		{
			send_to_char( " LightningBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_ACID_BREATH )) 
		{
			send_to_char( " AcidBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_GAS_BREATH ))
		{
			send_to_char( " GasBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TAIL ))
		{
			send_to_char( " Tail,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TERROR )) 
		{
			send_to_char( " Terror,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CONFUSE )) 
		{
			send_to_char( " Confuse,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_MASK )) 
		{
			send_to_char( " Mask,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_MAGIC ))
		{
			send_to_char( " Magic,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_POLYMORPH ))
		{
			send_to_char( " Polymorph,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if(IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LUCK ))
		{
			send_to_char( " Luck,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TOUGHNESS))
		{
			send_to_char( " Scales,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		send_to_char( "\n\r", ch );
		return;
	}
	else if( !str_cmp( arg, "wings" ) )
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_WINGS ) )
		{
			send_to_char( "You already have your wings.\n\r", ch );
			return;
		}
		if( ch->pcdata->powers[dragonage] < D_ADULT )
		{
			send_to_char( "You must be an adult dragon to learn this power.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
								sprintf( buf, "You need %d exp.\n\r", (count * 1000) );
			return;
		}
		ch->exp -= (count * 1000);
		send_to_char( "You now have your wings.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_WINGS );
		return;
	}
	else if( !str_cmp( arg, "truesight" ) )
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TRUESIGHT ))
		{
			send_to_char( "You already have sight beyond sight.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
			sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now have sight beyond sight.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_TRUESIGHT );
		return;
	}
	else if( !str_cmp( arg, "fangs" ) )
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FANGS ))
		{
			send_to_char( "You already have your fangs.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now have your fangs.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_FANGS );
		return;
	}
	else if( !str_cmp( arg, "claws" ) )
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CLAWS ))
		{
			send_to_char( "You already have your claws.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now have your claws.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_CLAWS );
		return;
	}

	else if( !str_cmp( arg, "lick" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LICK ))
		{
			send_to_char( "You already can can heal your wounds by licking 'em.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now can heal your wounds by licking them.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_LICK );
		return;
	}
	else if( !str_cmp( arg, "frostbreath" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FROST_BREATH ))
		{
			send_to_char( "You already can breath frost.\n\r", ch );
			return;
		}
		if( ch->pcdata->powers[dragonage] < D_MATURE )
		{
			send_to_char( "You must be a mautre adult dragon to learn this skill.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now can breath frost.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_FROST_BREATH );
		return;
	}
	else if( !str_cmp( arg, "firebreath" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FIRE_BREATH ))
		{
			send_to_char( "You already can breath fire.\n\r", ch );
			return;
		}
		if( ch->pcdata->powers[dragonage] < D_JUVENILE )
		{
			send_to_char( "You must be a juvenile dragon to learn this power.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now can breath fire.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_FIRE_BREATH );
		return;
	}
	else if( !str_cmp( arg, "lightningbreath" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LIGHTNING_BREATH ))
		{
			send_to_char( "You already can breath lightning.\n\r", ch );
			return;
		}
		if( ch->pcdata->powers[dragonage] < D_YADULT )
		{
			send_to_char( "You mus be a young dragon adult to learn this power.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now can breath lightning.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_LIGHTNING_BREATH );
		return;
	}
	else if( !str_cmp( arg, "acidbreath" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_ACID_BREATH ))
		{
			send_to_char( "You already can breath acid.\n\r", ch );
			return;
		}
		if( ch->pcdata->powers[dragonage] < D_VENERABLE )
		{
			send_to_char( "You mus be a venerable dragon to learn this power.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now can breath acid.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_ACID_BREATH );
		return;
	}
	else if( !str_cmp( arg, "gasbreath" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_GAS_BREATH ))
		{
			send_to_char( "You already can breath gas.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now can breath gas.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_GAS_BREATH );
		return;
	}
	else if( !str_cmp( arg, "tail" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TAIL ))
		{
			send_to_char( "You already can use your tail to attack.\n\r", ch );
			return;
		}
		if( ch->pcdata->powers[dragonage] < D_VYOUNG )
		{
			send_to_char( "You must be a very young dragon to learn this power.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now can use your tail to attack.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_TAIL );
		return;
	}
	else if( !str_cmp( arg, "terror" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TERROR ))
		{
			send_to_char( "You already inspire terror in mobs.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now inspire terror in mobs.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_TERROR );
		return;
	}
	else if( !str_cmp( arg, "confuse" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CONFUSE ))
		{
			send_to_char( "You already can confuse your targets.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now can confuse your targets.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_CONFUSE );
		return;
	}
	else if( !str_cmp( arg, "mask" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_MASK ))
		{
			send_to_char( "You already can mask people.\n\r", ch );
			return;
		}
		if( ch->pcdata->powers[dragonage] < D_OLD )
		{
			send_to_char( "You must be an old dragon to learn this power.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now can mask other people.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_MASK );
		return;
	}
	else if( !str_cmp( arg, "scry" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_SCRY))
		{
			send_to_char( "You already can see people who are far away.\n\r", ch);
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You can now see people who are far away.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_SCRY );
		return;
	}
	else if( !str_cmp( arg, "polymorph" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_POLYMORPH ))
		{
			send_to_char( "You already can polymorph.\n\r", ch );
			return;
		}
		if( ch->pcdata->powers[dragonage] < D_IMMORTAL )
		{
			send_to_char( "You must be an immortal dragon to learn this power.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now can polymorph.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_POLYMORPH );
		return;
	}
	else if( !str_cmp( arg, "luck" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LUCK ))
		{
			send_to_char( "You already have extra luck.\n\r", ch );
			return;
		}
		if( ch->pcdata->powers[dragonage] < D_ADULT )
		{
			send_to_char( "You have to be at least an adult dragon to get this power.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now have extra luck.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_LUCK );
		return;
	}
	else if( !str_cmp( arg, "fly"	))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FLY ))
		{
			send_to_char( "You already can fly to targets.\n\r", ch );
			return;
		}
		if( ch->pcdata->powers[dragonage] < D_ADULT )
		{
			send_to_char( "You have to be at least an adult dragon to get this power.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now can fly to people.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_FLY );
		return;
	}
	else if( !str_cmp( arg, "scales"))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TOUGHNESS ))
		{
			send_to_char( "Your scales are already very tough.\n\r", ch );
			return;
		}
		if( ch->pcdata->powers[dragonage] < D_ANCIENT )
		{
			send_to_char( "You have to be at least a great wyrm to get this power.\n\r", ch);
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "Your scales now are tougher than ever.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_TOUGHNESS );
		return;
	}
	else if( !str_cmp( arg, "peer"	))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_PEER	))
		{
			send_to_char( "You already can see people's stats.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
                       sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );                  
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		SET_BIT(ch->pcdata->powers[DPOWER_FLAGS], DRA_PEER);
		send_to_char( "You now can see other people's stats.\n\r", ch );
		return;
	}
	else if( !str_cmp( arg, "magic" ))
	{
		if( IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_MAGIC ))
		{
			send_to_char( "You already have magic control.\n\r", ch );
			return;
		}
		if( ch->pcdata->stats[DEMON_CURRENT] < (count * 1000) )
		{
							  sprintf( buf, "You need %d dragon points\n\r", (count * 1000) );
			send_to_char( buf, ch );
			return;
		}
		ch->pcdata->stats[DEMON_CURRENT] -= (count * 1000);
		send_to_char( "You now have great magical control.\n\r", ch );
		SET_BIT( ch->pcdata->powers[DPOWER_FLAGS], DRA_MAGIC);
		return;
	}
	else
	{
		int x = 0;
		send_to_char( "Valid choices are:\n\r", ch );
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_WINGS ) && ch->pcdata->powers[dragonage] >= D_ADULT)
		{
			send_to_char( "Wings,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FLY	) && ch->pcdata->powers[dragonage] >= D_ADULT )
		{
			send_to_char( " Fly,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TRUESIGHT ) )
		{
			send_to_char(" Truesight,", ch );
			x++;
			if(x>5){send_to_char("\n\r", ch);x=0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_SCRY ) )
		{
			send_to_char( " Scry,", ch );
			x++;
			if( x > 5){send_to_char("\n\r", ch);x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FANGS ) )
		{
			send_to_char( " Fangs,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CLAWS ) )
		{
			send_to_char( " Claws,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LICK ) )
		{
			send_to_char( " Lick,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FROST_BREATH ) && ch->pcdata->powers[dragonage] >= D_MATURE)
		{
			send_to_char( " FrostBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FIRE_BREATH ) && ch->pcdata->powers[dragonage] >= D_JUVENILE)
		{
			send_to_char( " FireBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LIGHTNING_BREATH ) && ch->pcdata->powers[dragonage] >= D_YADULT)
		{
			send_to_char( " LightningBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_ACID_BREATH ) && ch->pcdata->powers[dragonage] >= D_VENERABLE )
		{
			send_to_char( " AcidBreath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_GAS_BREATH ) )
		{
			send_to_char( " Gas Breath,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_PEER ) )
		{
			send_to_char( " Peer,", ch );
			x++;
			if(x > 5){send_to_char("\n\r", ch);x=0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TAIL ) && ch->pcdata->powers[dragonage] >= D_YOUNG )
		{
			send_to_char( " Tail,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TERROR ) )
		{
			send_to_char( " Terror,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CONFUSE ) )
		{
			send_to_char( " Confuse,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_MASK ) && ch->pcdata->powers[dragonage] >= D_OLD )
		{
			send_to_char( "Mask,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_MAGIC ) )
		{
			send_to_char( " Magic,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_POLYMORPH ) && ch->pcdata->powers[dragonage] >= D_IMMORTAL )
		{
			send_to_char( " Polymorph,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LUCK ) && ch->pcdata->powers[dragonage] >= D_ADULT )
		{
			send_to_char( " Luck,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_TOUGHNESS) && ch->pcdata->powers[dragonage] >= D_ANCIENT)
		{
			send_to_char( " Scales,", ch );
			x++;
			if( x > 5 ){send_to_char( "\n\r", ch );x = 0;}
		}
		send_to_char( "\n\r", ch );
		send_to_char( "Some are only avaiable after a certain age. Check helps for ages.\n\r", ch );
		return;
	}

	return;
}


void do_deat(CHAR_DATA * ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;
  one_argument(argument, arg);


    if (!IS_CLASS(ch, CLASS_DRAGON))
 send_to_char("huh?\n\r", ch);


  if (arg[0] == '\0')
  {
    send_to_char("Eat what?\n\r", ch);
    return;
  }

  if( str_cmp(arg, "gem") )
  {
     send_to_char("only gems\n\r", ch);
     return;
  }

  if ((obj = get_obj_carry(ch, arg, ch)) == NULL)
  {
    send_to_char("You do not have that item.\n\r", ch);
    return;
  }

  if (!IS_NPC(ch))
  

  act("$n eats $p.", ch, obj, NULL, TO_ROOM);
  act("You eat $p.", ch, obj, NULL, TO_CHAR);
  ch->pcdata->stats[DEMON_CURRENT] += obj->value[0];
  if (obj != NULL)
  extract_obj(obj);
  return;
  
}
void do_lick( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;

	one_argument(argument, arg );

	if( !IS_IMMORTAL(ch) )
	{
    if (!IS_CLASS(ch, CLASS_DRAGON))
		{
			send_to_char( "Huh?!?\n\r", ch );
			return;
		}
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LICK) )
		{
			send_to_char( "You haven't learned this skill yet.\n\r", ch );
			return;
		}
	        if(ch->hit <= 0) return;
	}
        if( arg[0] != '\0' )
        {

	  if ( ( victim = get_char_room( ch, NULL,  arg ) ) == NULL )
          {
              send_to_char( "They aren't here.\n\r", ch );
              return;
	  }
	  if ( (victim->position == POS_FIGHTING) && (victim != ch))
          {
              send_to_char( "You can only heal yourself during combat.\n\r", ch );
              return;
          }
        }

	if( arg[0] == '\0' ) victim = ch;

  if (victim->loc_hp[6] > 0)
  {
    stc("Your wounds close up and stop bleeding.\n\r",victim);
    victim->loc_hp[6] = 0;
  }

  if ((victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] + victim->loc_hp[3] + victim->loc_hp[4] +
victim->loc_hp[5]) != 0)
  {
    stc("Your bones mend themselves together and new limbs grow out of your body.\n\r",victim);
    victim->loc_hp[0] = 0;
    victim->loc_hp[1] = 0;
    victim->loc_hp[2] = 0;
    victim->loc_hp[3] = 0;
    victim->loc_hp[4] = 0;
    victim->loc_hp[5] = 0;
  }


	victim->hit += 250 * ch->pcdata->powers[dragonage];
	victim->move += 250 * ch->pcdata->powers[dragonage];
	victim->mana += 250 * ch->pcdata->powers[dragonage];
	if( victim->hit > victim->max_hit )
		victim->hit = victim->max_hit;
	if( victim->move > victim->max_move )
		victim->move = victim->max_move;
	if( victim->mana > victim->max_mana )
		victim->mana = victim->max_mana;

	update_pos(victim);

	act( "You lick $N's wounds and heal $M.",ch,NULL,victim,TO_CHAR);
	act( "$n licks your wounds and heals $M.",ch,NULL,victim,TO_VICT);
	act( "$n licks $N's wounds and heals $M.",ch,NULL,victim,TO_NOTVICT);
		  gain_condition( ch, COND_FULL, -(number_range(10,30)));
	WAIT_STATE(ch,6);
	return;
}

void do_dconfuse( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg[MAX_INPUT_LENGTH];
	 AFFECT_DATA af;

	one_argument( argument, arg );

	if( !IS_IMMORTAL(ch))
	{
    if (!IS_CLASS(ch, CLASS_DRAGON))
	{
		send_to_char( "Huh?!?\n\r", ch );
		return;
	}
	if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CONFUSE) )
	{
		send_to_char( "You haven't learned this skill yet.\n\r", ch );
		return;
	}
	}

	if( arg[0] == '\0' )
	{
		send_to_char( "Confuse whom?\n\r", ch );
		return;
	}

	if( (victim = get_char_room(ch, NULL,  arg)) == NULL )
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}

	 af.type      = gsn_curse;
	 af.duration  = 5;
	 af.location  = APPLY_HITROLL;
	 af.modifier  = -2000;
	 af.bitvector = 0;
	 affect_to_char( victim, &af );

	 af.location  = APPLY_DAMROLL;
	 af.modifier  = -2000;
	 affect_to_char( victim, &af );
	 gain_condition( ch, COND_FULL, -(number_range(50,100)));
	 WAIT_STATE(ch,8);

	return;
}

void do_fly( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
		  CHAR_DATA *mount;

	one_argument( argument, arg );

	if(!IS_IMMORTAL(ch))
	{
    if (!IS_CLASS(ch, CLASS_DRAGON))
	 {
	  send_to_char("Huh?\n\r", ch);
	  return;
	 }

	 if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FLY) )
	 {
	  send_to_char( "You haven't learned this skill yet.\n\r", ch );
	  return;
	 }
	}

	if ( arg[0] == '\0' )
	 {
	  send_to_char( "Fly to whom?\n\r", ch );
	  return;
	 }

	if ( ch->position == POS_FIGHTING )
	 {
	  send_to_char( "No way! You are fighting.\n\r", ch );
	  return;
	 }

	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	 {
	  send_to_char( "Nobody by that name.\n\r", ch );
 	  return;
	 }

	if( victim == ch )
	 {
	  stc( "Not to yourself.\n\r",ch);
	  return;
	 }

	if ( victim->fight_timer == 0 && !IS_IMMUNE(victim, IMM_SUMMON) && !IS_NPC(victim) && !IS_IMMORTAL(ch))
	{
	  send_to_char("You can't find them.\n\r", ch);
	  return;
	}

	if(IS_SET(victim->in_room->room_flags, ROOM_ASTRAL) || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
	  {
	   send_to_char("You can't find them.\n\r",ch);
	   return;
	  }
	act( "You disappear up into the sky.", ch, NULL, NULL, TO_CHAR );
	act( "$n disappears up into the sky.", ch, NULL, NULL, TO_ROOM );
	char_from_room( ch );
	char_to_room( ch, get_room_index(victim->in_room->vnum) );
	act( "You swoop down and land infront of $N", ch, NULL, victim, TO_CHAR );
	act( "$n swoop down and land infront of $N.", ch, NULL, victim, TO_ROOM );
	do_look( ch, "auto" );
	WAIT_STATE(ch,4);
	if ( (mount = victim->mount) == NULL ) return;
	char_from_room( mount );
	char_to_room( mount, get_room_index(victim->in_room->vnum) );
	return;
}

void do_peer(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
	 char      arg [MAX_INPUT_LENGTH];
    char      buf [MAX_INPUT_LENGTH];

	 one_argument( argument, arg );

	if (IS_NPC(ch)) return;

	if(!IS_IMMORTAL(ch))
	{
    if (!IS_CLASS(ch, CLASS_DRAGON))
	 {
	  send_to_char( "Huh?!?\n\r", ch );
	  return;
	 }
	 if(!IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_PEER))
	 {
	  send_to_char( "You don't have the ability to know people's stats.\n\r", ch);
	  return;
	 }
	}

	if( arg[0] == '\0' )
	{
		send_to_char( "Peer at who?\n\r", ch );
		return;
	}
	 if ( ( victim = get_char_room( ch, NULL,  arg ) ) == NULL )
    {
    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
		  send_to_char( "Peer at what?\n\r", ch );
        return;
	 }
	 act("$n examines $p intently.",ch,obj,NULL,TO_ROOM);
//	 spell_identify( skill_lookup( "identify" ), ch->level, ch, obj );
	 return;
	}
	 act("$n examines $N intently.",ch,NULL,victim,TO_NOTVICT);
	 act("$n examines you intently.",ch,NULL,victim,TO_VICT);
    if (IS_NPC(victim)) sprintf(buf, "%s is an NPC.\n\r",victim->short_descr);
    else 
     {
       if      (victim->level == 12) sprintf(buf, "%s is an Implementor.\n\r", victim->name);
       else if (victim->level == 11) sprintf(buf, "%s is a High Judge.\n\r", victim->name);
       else if (victim->level == 10) sprintf(buf, "%s is a Judge.\n\r", victim->name);
       else if (victim->level == 9 ) sprintf(buf, "%s is an Enforcer.\n\r", victim->name);
       else if (victim->level == 8 ) sprintf(buf, "%s is a Quest Maker.\n\r", victim->name);
       else if (victim->level == 7 ) sprintf(buf, "%s is a Builder.\n\r", victim->name);
       else if (victim->level >= 3 ) sprintf(buf, "%s is an Avatar.\n\r", victim->name);
       else sprintf(buf, "%s is a Mortal.\n\r", victim->name);
     }
	 send_to_char(buf,ch);

	 if (!IS_NPC(victim))
	{
		sprintf(buf,"Hitroll:%d, Damroll:%d, AC:%d.\n\r",char_hitroll(victim),char_damroll(victim),char_ac(victim));
	}
	 else sprintf(buf,"AC:%d.\n\r",char_ac(victim));
	 send_to_char(buf,ch);
	 if (!IS_NPC(victim))
	 {
	 sprintf(buf,"Status:%d, ",victim->pcdata->souls);
	 send_to_char(buf,ch);
	 if (IS_CLASS(victim, CLASS_VAMPIRE))
	 {
		  sprintf(buf,"Blood:%d, ",victim->pcdata->condition[COND_THIRST]);
		  send_to_char(buf,ch);
	 }
	 }
	 sprintf(buf,"Alignment:%d.\n\r",victim->alignment);
    send_to_char(buf,ch);
	 if (!IS_NPC(victim) && IS_EXTRA(victim, EXTRA_PREGNANT) && victim->sex == SEX_FEMALE)
	 act("$N is pregnant.",ch,NULL,victim,TO_CHAR);
	return;
}

void do_consume( CHAR_DATA *ch, char *argument )
{
	 OBJ_DATA *corpse;
	 OBJ_DATA *c_next;
//	 OBJ_DATA *obj;
//	 OBJ_DATA *obj_next;
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	int points;

	one_argument( argument, arg );

    if (!IS_CLASS(ch, CLASS_DRAGON))
return;

	 for ( corpse = ch->in_room->contents; corpse != NULL; corpse = c_next )
	 {
	c_next = corpse->next_content;
	if ( corpse->item_type != ITEM_CORPSE_NPC && corpse->item_type != ITEM_CORPSE_PC)
		 continue;
if (!global_cp)
	points = number_range( 280, 350 * (ch->pcdata->powers[dragonage]) );
else
	points = number_range( 400, 650 * (ch->pcdata->powers[dragonage]) );
	ch->pcdata->stats[DEMON_CURRENT] += points;
	ch->pcdata->stats[DEMON_TOTAL] += points;
	act( "$n savagely devours a corpse.", ch, NULL, NULL, TO_ROOM );
	sprintf( buf, "You gain %d dragon points.\n\r", points );
	send_to_char( buf, ch );
/*	for ( obj = corpse->contains; obj; obj = obj_next )
	{
		 obj_next = obj->next_content;
		 obj_from_obj( obj );
		 obj_to_room( obj, ch->in_room );
	}*/

	if (corpse != NULL) extract_obj( corpse );
	 }
	return;
}

void do_dgem ( CHAR_DATA *ch, char *argument )
{
	 char arg1[MAX_INPUT_LENGTH];
	 char buf[MAX_STRING_LENGTH];
	 OBJ_INDEX_DATA *pObjIndex;
	 OBJ_DATA *obj;
	 int value;

	 argument = one_argument( argument, arg1 );

    if (!IS_CLASS(ch, CLASS_DRAGON))
	 {
	stc("Huh?\n\r", ch);
	return;
	 }

	 if ( IS_NPC(ch) || (ch->pcdata->stats[DEMON_CURRENT] < 1 ) )
	 {
	send_to_char("You are unable to make a gem.\n\r",ch);
	return;
	 }
	 else if ( arg1[0] == '\0' || !is_number( arg1 ) )
	 {
	send_to_char("Please specify a value for gem.\n\r",ch);
	return;
	 }
	 else
	 {
		  value = atoi( arg1 );
	if ( value < 1 || value > 100000 )
		  {
		 send_to_char( "Gem value should have a value between 1 and 100000.\n\r", ch );
		 return;
		  }
	else if ( value > ch->pcdata->stats[DEMON_CURRENT] )
		  {
		 sprintf(buf,"You only have %d dragon points left to put into gems.\n\r",ch->pcdata->stats[DEMON_CURRENT]);
	    send_to_char( buf, ch );
	    return;
        }
	 }

	 if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error...missing object, please inform KaVir.\n\r", ch );
	return;
    }

	 ch->pcdata->stats[DEMON_CURRENT] -= value;
	 ch->pcdata->stats[DEMON_TOTAL]	 -= value;
	 if (ch->pcdata->stats[DEMON_CURRENT]  < 0) ch->pcdata->stats[DEMON_CURRENT] = 0;
    obj = create_object( pObjIndex, value );
    obj->value[0] = value;
    obj->level = 1;
    obj->cost = value*1000;
	 obj->item_type = ITEM_DRAGONGEM;
    obj_to_char(obj,ch);
	 if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    free_string( obj->name );
	 obj->name = str_dup( "dragon gem" );
    sprintf(buf,"a %d point gem",value);
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf(buf,"A %d point gem lies on the floor.",value);
    free_string( obj->description );
    obj->description = str_dup( buf );

		act( "A tear rolls down your cheek and crystalizes into $p.", ch, obj, NULL, TO_CHAR );
   	act( "A tear rolls down $n's cheek and crystalizes into $p.", ch, obj, NULL, TO_ROOM );

    return;
}


void do_age( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    if (!IS_CLASS(ch, CLASS_DRAGON))
    return;

    if(ch->pcdata->stats[DEMON_CURRENT] < (ch->pcdata->powers[dragonage] * 30000))
      {
	sprintf( buf, "You need %d dragon points to age \n\r", (ch->pcdata->powers[dragonage] * 30000));
	  send_to_char( buf, ch );
	return;
      }
    if ( ch->practice < (ch->pcdata->powers[dragonage]*100))
      {
        sprintf(buf,"It costs %d points of primal to age.\n\r",(ch->pcdata->powers[dragonage]*100));
          send_to_char(buf,ch);
        return;
      }

    if(ch->max_hit < (ch->pcdata->powers[dragonage] * 5000))
      {
	sprintf( buf, "You need %d hitpoints to age \n\r", (ch->pcdata->powers[dragonage] * 5000));
	  send_to_char( buf, ch );
	return;
      }
	 if(ch->pcdata->powers[dragonage] == 12 && !IS_IMMORTAL(ch))
      {
	sprintf( buf, "You cannot age any further\n\r");
 	  send_to_char( buf, ch );
 	return;
		}
    if (IS_IMMORTAL(ch))
      {
	  ch->pcdata->powers[dragonage] = 13;
	  return;
      }
    ch->pcdata->stats[DEMON_CURRENT] -= (ch->pcdata->powers[dragonage] * 30000);
    ch->practice -= (100 * ch->pcdata->powers[dragonage]);
    ch->pcdata->powers[dragonage] +=1;
    send_to_char( "You have aged.\n\r", ch );
    return;
}


void do_fbreath(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  int i=0;
  int dam = 0;
  int chance =0;
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char arg [MAX_INPUT_LENGTH];

  one_argument( argument, arg );

  if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))

 {
	 send_to_char("Huh?\n\r",ch);
	 return;
  }
    if (!IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FIRE_BREATH ))
  {
	 send_to_char("You haven't learned how to breath fire yet.\n\r",ch);
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
 if (ch->mana < 100 )
  {
	 send_to_char("You dont have enough mana.\n\r",ch);
	 return;
  }
  if ((victim = ch->fighting) == NULL)
  {
	 send_to_char( "You aren't fighting anyone.\n\r", ch );
	 return;
  }
  ch->mana -= 100;

  chance = number_range (1, (100 + ch->pcdata->powers[dragonage]+ch->pRank));

  if (chance <= 7) i = 1;
  if (chance > 7 && chance <= 22)   i = 2;
  if (chance > 22 && chance <= 32)   i = 3;
//  if (chance > 32 && chance <= 52)   i = 4;
  if (chance > 52 && chance <= 90)   i = 5;
  if (chance > 90 && chance <= 94)   i = 6;
//  if (chance > 94 && chance <= 110)  i = 7;
  if (chance > 110 && chance <= 125) i = 8;
//  if (chance > 125 && chance <= 128) i = 9;
  if (chance > 128 ) i = 10;

  for(i=i;i>0;i--)
   {
     dam = number_range(6000 , 880 * (ch->pRank+ch->pcdata->powers[dragonage]));
     if (dam < 6000) dam = 6000;
     dam = cap_dam(ch,victim,dam);
     sprintf(buf, "%s's blast of #rf#Rl#ra#Rm#re #Rimmolates#n you [#r%d#n]\n\r", ch->name, dam);
     send_to_char( buf, victim );
     sprintf(buf2, "Your blast of #rf#Rl#ra#Rm#re #Rimmolates#n %s [#r%d#n]\n\r",victim->name, dam);
     send_to_char( buf2, ch );
     act( "$n's blast of #rf#Rl#ra#Rm#re #Rimmolates#n $N",ch,NULL,victim,TO_NOTVICT);
     fire_effect(victim,ch->explevel,dam,TARGET_CHAR);
     hurt_person(ch,victim,dam);
     if (victim->hit < 1) break;
   }
  if (!IS_AFFECTED(victim, AFF_FLAMING))
    SET_BIT(victim->affected_by, AFF_FLAMING);
  WAIT_STATE(ch,6);
  return;
}

void do_lbreath(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  int i=0;
  int dam = 0;
  int chance =0;
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  char arg [MAX_INPUT_LENGTH];

  if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
{	
 send_to_char("Huh?\n\r",ch);
	 return;
  }
    if (!IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_LIGHTNING_BREATH ))
  {
	 send_to_char("You haven't learned how to breath lightning yet.\n\r",ch);
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


   if  (ch->mana < 100 )
  {
	 send_to_char("You dont have enough mana.\n\r",ch);
	 return;
  }
  if ((victim = ch->fighting) == NULL)
  {
	 send_to_char( "You aren't fighting anyone.\n\r", ch );
	 return;
  }
  ch->mana -= 100;
  chance = number_range (1, (100 + ch->pcdata->powers[dragonage]+ch->pRank));

  if (chance <= 7) i = 1;
  else if (chance > 7 && chance <= 22)   i = 2;
  else if (chance > 22 && chance <= 32)   i = 3;
//  else if (chance > 32 && chance <= 52)   i = 4;
  else if (chance > 52 && chance <= 90)   i = 5;
  else if (chance > 90 && chance <= 94)   i = 6;
  else if (chance > 94 && chance <= 110)  i = 7;
//  else if (chance > 110 && chance <= 125) i = 8;
  else if (chance > 125 && chance <= 128) i = 9;
  else if (128 < chance ) i = 10;

  for(i=i;i>0;i--)
    {
	dam = number_range(6000 , 680 * (ch->pRank+ch->pcdata->powers[dragonage]));
	if (dam < 6000) dam =6000;
        dam = cap_dam(ch,victim,dam);
	sprintf(buf, "%s's blast of #yl#oi#yg#oh#yt#on#yi#on#yg #yelectrifies#n you [#y%d#n]\n\r", ch->name, dam);
	send_to_char( buf, victim );
	sprintf(buf2,"Your blast of #yl#oi#yg#oh#yt#on#yi#on#yg #yelectrifes#n %s [#y%d#n]\n\r",victim->name, dam);
	send_to_char( buf2, ch );
	act( "$n's blast of #yl#oi#yg#oh#yt#on#yi#on#yg #yelectrifes#n $N.",ch,NULL,victim,TO_NOTVICT);
	hurt_person(ch,victim,dam);
	if (victim->hit < 1) break;
	shock_effect(victim,ch->explevel,dam,TARGET_CHAR);
    }
    chance = number_range (1,10);
    if(ch->pcdata->powers[DRAGON_COLOR] == 3) chance += 1;
    if(ch->pcdata->powers[DRAGON_COLOR] ==12) chance += 1;
    if (chance >= 9)
      {
        act( "Your blast of #yl#oi#yg#oh#yt#on#yi#on#yg#n stuns $N.",ch,NULL,victim,TO_CHAR);
        act( "$n's blast of #yl#oi#yg#oh#yt#on#yi#on#yg stuns you.",ch,NULL,victim,TO_VICT);
        act( "$n's blast of #yl#oi#yg#oh#yt#on#yi#on#yg stuns $N.",ch,NULL,victim,TO_NOTVICT);
        victim->position = POS_STUNNED;
      }
  WAIT_STATE(ch,6);
  return;
}

void do_gbreath(CHAR_DATA *ch, char *argument)
{
	 CHAR_DATA *vch;
	 CHAR_DATA *vch_next;
         CHAR_DATA *victim;
	 char buf[MAX_STRING_LENGTH];
	 char buf2[MAX_STRING_LENGTH];
	 int dam;
	int rrdam = 0;
        int rrcount = 0;
         int i = 0;
	 int chance = 0;


    if (!IS_CLASS(ch, CLASS_DRAGON))

         {
            send_to_char ("Huh?\n\r",ch);
            return;
         }

	 if (ch->mana < 100)
	 {
		send_to_char ("You dont have enough mana.\n\r",ch);
		return;
	 }

        chance = (number_range (1, (100+ch->pRank+ch->pcdata->powers[dragonage])));
        if (chance <= 7) i = 1;
  	else if (chance > 7 && chance <= 22)   i = 2;
  	else if (chance > 22 && chance <= 32)   i = 3;
  	else if (chance > 32 && chance <= 52)   i = 4;
  	else if (chance > 52 && chance <= 90)   i = 5;
  	else if (chance > 90 && chance <= 94)   i = 6;
  	else if (chance > 94 && chance <= 110)  i = 7;
  	else if (chance > 110 && chance <= 125) i = 8;
  	else if (chance > 125 && chance <= 128) i = 9;
  	else if (128 < chance ) i = 10;

   for(i=i;i>0;i--)
   {
	  send_to_char("#GYou belch forth a cloud of poisonous gas#n.\n\r",ch);
     for ( vch = ch->in_room->people; vch!=NULL;vch = vch_next)     
      {
      	if(ch->mana <= 100)
      		break;
        vch_next = vch->next_in_room;
        if (can_see(ch,vch))
	if ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) )
	{
          dam = number_range((ch->damcap[DAM_CAP] - 500), (ch->damcap[DAM_CAP]+(ch->pRank*125)));
	  sprintf(buf, "%s's #sblast of gas#n chokes you [#G%d#n]\n\r", ch->name, dam);
	  send_to_char( buf, vch );
	if(!IS_SET(ch->act,PLR_BRIEF5)){
	  sprintf(buf2, "Your #sblast of gas#n chokes %s [#G%d#n]\n\r",vch->name, dam);
	  send_to_char( buf2, ch );
	}
	else{
		rrdam += dam;
		rrcount += 1;
	}
	  //act( "$n's #sblast of gas#n chokes $N.",ch,NULL,vch,TO_NOTVICT);
          spell_poison(gsn_poison,(ch->level*number_range(100,120)),ch,victim);
	  set_fighting(ch,vch, TRUE);
	  hurt_person(ch,vch,dam);
	  ch->mana -= 100;
	}
      }
    }
	if(IS_SET(ch->act,PLR_BRIEF5)){
		sprintf(buf,"#GYou've dealt #C%d #Gattacks causing #C%d #Gdamage. #0(#y%d #Gaverage#0)#n\n\r",rrcount,rrdam,dam);
		stc(buf,ch);
	}

  WAIT_STATE (ch, 4);
  return;
}



void do_abreath(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
		  int dam = 0;
	int damp = 0;
	int vorpchance = 0;
	bool is_vorpal = FALSE;
	char buf[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	char arg [MAX_INPUT_LENGTH];


  if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
{
     send_to_char("Huh?\n\r",ch);
     return;
   }
    if (!IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_ACID_BREATH ))
   {
     send_to_char("You haven't learned how to breath acid yet.\n\r",ch);
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

  if   (ch->mana < 100 )
  {
	 send_to_char("You dont have enough mana.\n\r",ch);
	 return;
  }
  if ((victim = ch->fighting) == NULL)
  {
	 send_to_char( "You aren't fighting anyone.\n\r", ch );
	 return;
  }
//  ch->mana -= 100;
  dam = number_range(8000 , (5000+(ch->pRank*100)));
   WAIT_STATE (ch, 12);
  sprintf(buf, "%s's blast of acid covers you [#C%d#n]\n\r", ch->name, dam);
  send_to_char( buf, victim );
  sprintf(buf2, "Your blast of acid covers %s [#C%d#n]\n\r",victim->name, dam);
  send_to_char( buf2, ch );
  act( "$n's blast of acid covers $N.",ch,NULL,victim,TO_NOTVICT);
  hurt_person(ch,victim,dam);
  acid_effect(victim,ch->explevel,dam,TARGET_CHAR);
  vorpchance = number_range (1, 100);
  vorpchance += (2 * ch->pcdata->powers[dragonage]);
  if ( ch->pcdata->powers[DRAGON_COLOR] == 7 ) vorpchance += 35;   // Black Dragon Acid Bonus
  if ( ch->pcdata->powers[DRAGON_COLOR] == 15 ) vorpchance += 25;  // Topaz Dragon Acid Bonus
  if ( vorpchance >= 40) is_vorpal = TRUE;
  if ( is_vorpal )
  {
//    ch->mana-=9000;
    damp=number_range(1,8);
    if ( damp == 1 )
     {
 	return;
     }
    else if ( damp == 2 )
     {
 	return;
     }
    else if ( damp == 3 )
     {
 	return;
     }
    else if ( damp == 4 )
     {
	return;
     }
    else if ( damp == 5 ) 
     {
	return;
     }
    else if ( damp == 6 )
     {
	return;
     }
    else if ( damp == 7 )
     {
	if (!IS_ARM_L(victim,LOST_ARM))
	 {
	  act("Your acid burns off $N's arm.", ch, NULL, victim, TO_CHAR);
	  act("$n's acid burns off $N's arm.", ch, NULL, victim, TO_NOTVICT);
	  act("$n's acid burns off your arm.", ch, NULL, victim, TO_VICT);
	  make_part(victim,"arm");
	  SET_BIT(victim->loc_hp[2],LOST_ARM);
	  if (!IS_BLEEDING(victim,BLEEDING_ARM_L)) SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
	  if (IS_BLEEDING(victim,BLEEDING_HAND_L)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
	}
       else if (!IS_ARM_R(victim,LOST_ARM))
	{
	  act("Your acid burns off $N's arm.", ch, NULL, victim, TO_CHAR);
	  act("$n's acid burns $N's arm.", ch, NULL, victim, TO_NOTVICT);
	  act("$n's acid burns off your arm.", ch, NULL, victim, TO_VICT);
	  make_part(victim,"arm");
	  SET_BIT(victim->loc_hp[3],LOST_ARM);
	  if (!IS_BLEEDING(victim,BLEEDING_ARM_R)) SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
	  if (IS_BLEEDING(victim,BLEEDING_HAND_R)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
	}
       else
	{
	  return;
	}
     }
    else if ( damp == 8 )
     {
	if (!IS_LEG_L(victim,LOST_LEG))
	{
	  act("Your acid burns off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
	  act("$n's acid burns off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
	  act("$n's acid burns off your leg at the hip.", ch, NULL, victim, TO_VICT);
	  make_part(victim,"leg");
	  SET_BIT(victim->loc_hp[4],LOST_LEG);
	  if (!IS_BLEEDING(victim,BLEEDING_LEG_L)) SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
	  if (IS_BLEEDING(victim,BLEEDING_FOOT_L)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
	}
	else if (!IS_LEG_R(victim,LOST_LEG))
	{
	  act("Your acid burns off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
	  act("$n's acid burns off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
	  act("$n's acid burns off your leg at the hip.", ch, NULL, victim, TO_VICT);
	  make_part(victim,"leg");
	  SET_BIT(victim->loc_hp[5],LOST_LEG);
	  if (!IS_BLEEDING(victim,BLEEDING_LEG_R)) SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
	  if (IS_BLEEDING(victim,BLEEDING_FOOT_R)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
	}
	else
	{
		return;
	}
     }
 }

 if (!IS_AFFECTED(victim, AFF_FLAMING))
 SET_BIT(victim->affected_by, AFF_FLAMING);
 WAIT_STATE(ch,6);
 return;
}

void do_cbreath( CHAR_DATA *ch, char *argument )
{
	 CHAR_DATA *victim;
	 int i=0;
	 int dam = 0;
	 int chance =0;
	 char buf[MAX_STRING_LENGTH];
	 char buf2[MAX_STRING_LENGTH];
	char arg [MAX_INPUT_LENGTH];

	 if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_DRAGON))
	  {
		  send_to_char("Huh?\n\r",ch);
		  return;
	  }
    if (!IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FROST_BREATH ))
	  {
		  send_to_char("You haven't learned how to breath frost yet.\n\r",ch);
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

	 if ((victim = ch->fighting) == NULL)
	  {
		  send_to_char( "You aren't fighting anyone.\n\r", ch );
		  return;
	  }

	 if (ch->mana < 100)
	  {
		  send_to_char("You don't have enough mana.\n\r", ch);
		  return;
	  }
  chance = number_range (1, (100 + ch->pcdata->powers[dragonage]+ch->pRank));

  if (chance <= 7) i = 1;
  else if (chance > 7 && chance <= 22)   i = 2;
  else if (chance > 22 && chance <= 32)   i = 3;
  else if (chance > 32 && chance <= 52)   i = 4;
  else if (chance > 52 && chance <= 90)   i = 5;
  else if (chance > 90 && chance <= 94)   i = 6;
  else if (chance > 94 && chance <= 110)  i = 7;
  else if (chance > 110 && chance <= 125) i = 8;
  else if (chance > 125 && chance <= 128) i = 9;
  else if (128 < chance ) i = 10;

  for(i=i;i>0;i--)
	  {
		dam = number_range(1000 ,2500);
		if (dam > 5000) dam = 5000;
		sprintf(buf, "%s's cone of #Cf#7r#Co#7s#Ct#n freezes you [#C%d#n]\n\r", ch->name, dam);
		send_to_char( buf, victim );
		sprintf(buf2, "Your cone of #Cf#7r#Co#7s#Ct#n freezes %s [#C%d#n]\n\r",victim->name, dam);
		send_to_char( buf2, ch );
		act( "$n's cone of #Cf#7r#Co#7s#Ct#n freezes $N.",ch,NULL,victim,TO_NOTVICT);
		hurt_person(ch,victim,dam);
		if (victim->hit < 1) break;
		cold_effect(victim,ch->explevel,dam,TARGET_CHAR);
	  }

  ch->mana = ch->mana - 100;
  WAIT_STATE(ch,12);
  WAIT_STATE(victim, 6);
  return;
}

void do_nest( CHAR_DATA *ch, char *argument )
{
	 char buf[MAX_STRING_LENGTH];
	 char arg[MAX_INPUT_LENGTH];
	 CHAR_DATA *gch;

	 one_argument( argument, arg );

	 if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
	 {
	send_to_char("Huh?\n\r",ch);
	return;
	 }
	 sprintf( buf, "                 #C[#LThe Lords of the Skies#C]\n\r");
	 send_to_char( buf, ch );
	 send_to_char("#0[      #rName      #0] [   #rHits  #0] [   #rMana  #0] [   #rMove  #0] [  #rAge  #0]\n\r", ch );
	 for ( gch = char_list; gch != NULL; gch = gch->next )
	 {
	if ( IS_NPC(gch) ) continue;
    if (!IS_CLASS(gch, CLASS_DRAGON)) continue;
//	if ( gch->level >= 7) continue; 
// if i had WANTED that in there i would have PUT it in there, GryphonHawk
	sprintf( buf,"#0[#R%-16s#0] [#r%-9d#0] [#r%-9d#0] [#r%-9d#0] [   #r%-2d  #0]\n\r",
	capitalize( gch->name ),
	gch->hit,gch->mana,gch->move,
	gch->pcdata->powers[dragonage]);
	send_to_char( buf, ch );
	 }
	 return;
}


void do_dswoop( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  ROOM_INDEX_DATA *location;
  CHAR_DATA *victim;
          
  one_argument (argument, arg);
  if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DRAGON))
  {
    send_to_char("Huh?\n\r", ch );
    return;
  }
		if( !IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FLY) )
		{
			send_to_char( "You haven't learned this skill yet.\n\r", ch );
			return;
		}
  if ((victim = get_char_world(ch, arg)) == NULL)
  {
    send_to_char("Swoop down on whom?\n\r", ch );
    return;
  }
  if (IS_SET(victim->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "You can't find it's room.\n\r",ch);
    return;
  }
  if( IS_IMMUNE(victim, IMM_TRAVEL ) && victim->fight_timer == 0) {
     send_to_char("He doesnt want you near him!\n\r",ch);
     return;
  }
  if (IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))
  {
    stc( "Your room is not connected to the astral plane.\n\r",ch);
    return;
  }
  location = victim->in_room;
  if (ch->move < 500)
  {
    send_to_char("You don't have the move to fly that far.\n\r", ch );
    return;
  }

	act( "You disappear up into the sky.", ch, NULL, NULL, TO_CHAR );
	act( "$n disappears up into the sky.", ch, NULL, NULL, TO_ROOM );
  ch->move -= 500;
  char_from_room(ch);
  char_to_room(ch, location);
  do_look(ch, "auto");
  if(victim->fight_timer > 0) WAIT_STATE(ch,4);
	  act("You swoop down from the sky onto $N.",ch,NULL,victim,TO_CHAR);
	  act("$n swoops down from the sky onto $N.",ch,NULL,victim,TO_NOTVICT);
	  act("$n swoops down from the sky onto you.",ch,NULL,victim,TO_VICT);
	  if (IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CLAWS) ) one_hit( ch, victim, gsn_claws,8);
	  if (IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_CLAWS) ) one_hit( ch, victim, gsn_claws,8);
	  if (IS_SET(ch->pcdata->powers[DPOWER_FLAGS], DRA_FANGS) ) one_hit( ch, victim, gsn_fangs,8);
	  one_hit(ch, victim, gsn_fireball,8);
          one_hit(ch, victim, gsn_fireball,8);
          one_hit(ch, victim, gsn_fireball,8);
          one_hit(ch, victim, gsn_sweep,8);
	  one_hit(ch, victim, gsn_headbutt,8);
  return;
}





void do_dsight(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch)) return;

    if (IS_CLASS(ch, CLASS_DRAGON) && !IS_SET(ch->pcdata->powers[DPOWER_FLAGS],DRA_TRUESIGHT))
	 {
	  send_to_char("Huh?\n\r", ch );
	  return;
	 }

	 if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
	 {
		  REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
		  send_to_char( "Your senses return to normal.\n\r", ch );
	 }
	 else
	 {
		  SET_BIT(ch->act, PLR_HOLYLIGHT);
		  send_to_char( "Your senses increase to incredible proportions.\n\r", ch);
	 }

	return;
}


void do_breathforge(CHAR_DATA *ch, char *argument)
{
	 OBJ_DATA *obj;

    if (!IS_CLASS(ch, CLASS_DRAGON))

          {
             send_to_char("huh?\n\r",ch);
             return;
          }
    if (ch->pcdata->powers[dragonage] < 12)
	  {
		 send_to_char("You have earned the power to forge items yet.\n\r",ch);
		 return;
	  }
 	if ( ch->practice < 1500)
	  {
		 send_to_char("It costs 1500 primal to forge an item.\n\r",ch);
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
	if (IS_SET(obj->quest,QUEST_RELIC) || IS_SET(obj->quest,QUEST_ARTIFACT))
	  {
		send_to_char("Not on a Relic.\n\r",ch);
		return;
	  }
        if (IS_SET(obj->quest2,QUEST2_BREATH))
          {
                send_to_char("This item has already been forged by a dragon!!\n\r",ch);
                return;
          }
	if (obj->item_type == ITEM_WEAPON)
	  {
		 obj->value[1] += 50;
		 obj->value[2] += 50;
		 obj->level     =  50;
		 oset_affect(ch,obj,50,APPLY_HITROLL,TRUE);
		 oset_affect(ch,obj,50,APPLY_DAMROLL,TRUE);
		 SET_BIT(obj->quest2,QUEST2_BREATH);
		 obj->condition = 100;
		 obj->toughness = 100;
		 obj->resistance = 1;
		 act("You temper $p in a blast of flame.", ch, obj, NULL, TO_CHAR);
		 act("$n tempers $p in a blast of flame.", ch, obj, NULL, TO_ROOM);
		 if(!IS_SET(obj->weapflags, WEAPON_FLAMING)) SET_BIT(obj->weapflags, WEAPON_FLAMING);
		 ch->practice -= 1500;
		 WAIT_STATE(ch, 8); 
		 return;
	  }
	if (obj->item_type == ITEM_ARMOR)
	  {
		obj->value[0] += 50;
		oset_affect(ch,obj,30,APPLY_HITROLL,TRUE);
		oset_affect(ch,obj,30,APPLY_DAMROLL,TRUE);
		oset_affect(ch,obj,-50,APPLY_AC,TRUE);
		SET_BIT(obj->quest2,QUEST2_BREATH);
		obj->condition = 100;
		obj->toughness = 100;
		obj->resistance = 1;
		act("You temper $p in a blast of flame.", ch, obj, NULL, TO_CHAR);
		act("$n tempers $p in a blast of flame.", ch, obj, NULL, TO_ROOM);
		ch->practice -= 1500;
		WAIT_STATE(ch, 8);
		return;
	  }

}

