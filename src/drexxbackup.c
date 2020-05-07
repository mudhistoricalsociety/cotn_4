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

/*     void do_kshow(CHAR_DATA *ch, char *argument)
	{
    	char buf[MAX_STRING_LENGTH];
    	CHAR_DATA *gch;

    	if (IS_NPC(ch)) return;
    	if (ch->pcdata->kingdom == 0)
    	{
        send_to_char("You are not in a kingdom.\n\r", ch );
        return;
    	}
    	sprintf(buf, "#7The Kingdom of %s:\n\r",
    	kingdom_table[ch->pcdata->kingdom].whoname);
    	send_to_char( buf, ch );
    	send_to_char("#2[    Name    ] [ Hits ] [ HPmax ] [ Mana ] [ Move ] [ Status ]#n\n\r", ch );
    	for ( gch = char_list; gch != NULL; gch = gch->next )
    	{
        if ( IS_NPC(gch) ) continue;
        if ( gch->pcdata->kingdom != ch->pcdata->kingdom)
        continue;
        sprintf( buf,       "#2[#n%-12s#2] [#n%6d#2] [#n%7d#2] [#n%6d#2] [#n%6d#2] [#n   %-2d   #2]#n\n\r",
        gch->name ,
        gch->hit,gch->max_hit,gch->mana,gch->move,
        gch->pStatus);
        send_to_char( buf, ch );
     	}
     	return;
  	}

     void do_favorclear( CHAR_DATA *ch, char *argument )
	{
	if (IS_SET(ch->act, PLR_RIGHTHAND))
	{
 	REMOVE_BIT(ch->act, PLR_RIGHTHAND);
	}
	if (IS_SET(ch->act, PLR_LEFTHAND))
	{
 	REMOVE_BIT(ch->act, PLR_LEFTHAND);
	}
	if (IS_SET(ch->act, PLR_AMBI))
	{
 	REMOVE_BIT(ch->act, PLR_AMBI);
	}
	send_to_char("You no longer favor any or both hands.\n\r", ch);
	return;
	}

     void do_damn( CHAR_DATA *ch, char *argument )
	{
  	CHAR_DATA *victim;
  	char arg [MAX_INPUT_LENGTH];
  	ROOM_INDEX_DATA *in_room;
  
  	argument = one_argument(argument, arg);

  	if (IS_NPC(ch)) return;
	if ( !IS_CLASS(ch, CLASS_DEMON))
	{
	send_to_char("Huh?\n\r", ch);
	return;
	}
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
 	{
   	send_to_char( "They aren't here.\n\r", ch );  
    	return;
  	}  
        if ( ch == victim && ch->fighting != NULL)
        {
        send_to_char( "You cannot damn yourself while fighting!\n\r", ch );
        return;
        }
        if ( ch == victim && ch->fighting == NULL)
        {
        send_to_char( "You return to hell!\n\r", ch );
	char_from_room( ch );
	char_to_room( ch, get_room_index(93425));
 	do_look( ch, "auto" );	   
	return;
        }
        in_room = ch->in_room;
	if ( ch->in_room == get_room_index(93425) )
        {
	stc("#RAlready there!!#n\n\r",ch);
	return;
	}
	if (ch->fighting == NULL)
	{
	stc("#rYou're not engaged in combat with them!#n\n\r",ch);
	return;
	}
	stc("#7You attempt to focus your hatred...#n\n\r",ch);
        if ((number_range(1,3) == 1) && (ch->fighting != NULL))
	{
	    if ( ch->fighting )
	{
	stop_fighting( ch, TRUE );
	}
	do_say(ch,"#rM#RuAHahahHAHA #rW#Relcome #rt#Ro #rm#Ry #rD#Romain#r!#R!#n");
	stc("#RYou #rDAMN #Rthem to the pits of hell!#n\n\r",ch);
	char_from_room( victim );
	char_to_room( victim, get_room_index(93425));
        do_look( victim, "auto" );
	char_from_room( ch );
	char_to_room( ch, get_room_index(93425));
	stc("#RWelcome to Hell!#n\n\r",victim);
 	do_look( ch, "auto" );	
	set_fighting(ch, victim);
        }
        WAIT_STATE(ch, 6);
        return;
        } 

     void do_gale(CHAR_DATA *ch, char *argument)
	{
  	CHAR_DATA *victim;

  	if (IS_NPC(ch)) return;
  	if (!IS_CLASS(ch, CLASS_ELEMENTAL)   && !IS_CLASS(ch, CLASS_GAIA_WOLF)
      	&&  !IS_CLASS(ch, CLASS_NECROMANCER) && !IS_CLASS(ch, CLASS_DRUID)
	&&  !IS_CLASS(ch, CLASS_DEVA))
    	{
        send_to_char("Elemental Only?\n\r",ch);
        return;
	}
        if (ch->pcdata->powers[ELEMENTAL_AIR] < 4 )
  	{
    	send_to_char("Your power in the element of air is not strong enough.\n\r",ch);
    	return;
  	}
  	if (ch->move < 2000)
  	{
    	send_to_char("This costs 2000 move.\n\r",ch);
    	return;
  	}
  	if ( ( victim = ch->fighting ) == NULL )
  	{
    	send_to_char( "You aren't fighting anyone.\n\r", ch );
    	return;
  	}
  	WAIT_STATE(ch, 18);
	ch->move -= 2000;
  	if (number_range(1,4)==1)
  	{
    	victim->position = POS_STUNNED;
    	ch->position = POS_STANDING;
    	act("#7You send a #CG#cale #7of #CW#cind #7toward #y$N #7sending #y$M flying into the wall.#n",ch,NULL,victim,TO_CHAR);
    	act("#y$n #7sends a #CG#cale #7of #CW#cind #7toward sending you into the wall.#n ",ch,NULL,victim, TO_VICT);
    	act("#y$n #7sends a #CG#cale #7of #CW#cind #7toward #y$N #7sending #y$M #7flying into the wall.#n",ch,NULL,victim, TO_ROOM);
        return;
  	}
        else
        {
        act("#7Your #CG#cale #7of #CW#cind #LB#llows #7past #y$N#7.#n",ch,NULL,victim,TO_CHAR);
	act("#y$n's #CG#cale #7of #CW#cind #LB#llows #7past you.#n",ch,NULL,victim, TO_VICT);
	act("#y$n's #CG#cale #7of #CW#cind #LB#llows #7past #y$N#7.#n",ch,NULL,victim, TO_ROOM);
  	return;
	}
	}

     void do_smokescreen( CHAR_DATA *ch, char *argument )
	{
  	if (!IS_CLASS(ch, CLASS_ASSASSIN)      && !IS_CLASS(ch, CLASS_UNDEAD_ASSASSIN)
     	&&  !IS_CLASS(ch, CLASS_HELLS_SOLDIER) && !IS_CLASS(ch, CLASS_BLACK_ASSASSIN))
  	{
    	send_to_char("Huh?\n\r",ch);
    	return;
  	}
  	if (ch->pcdata->powers[ASSASSIN_KNOWLEDGE] < 3)
  	{
    	send_to_char("You need level 3 in knowledge before you can create a smokescreeen.\n\r",ch);
    	return;
  	}
       	if(ch->fight_timer > 0)
       	{
        stc("Not with a fighttimer.\n\r",ch);
        return;
        }
        if(!IS_SET(ch->act, AFF_HIDE))
	{
        stc("#7You surround yourself in a #nSc#7re#0e#nn #7of #nS#7m#0ok#ne#7.#n\n\r",ch);
        act("#7$n has become #ye#Yngulfed #7in a #nSc#7re#0e#nn #7of #nS#7m#0ok#ne#7.#n.",ch,NULL,NULL,TO_ROOM);
        SET_BIT(ch->act, AFF_HIDE);
        }
        else
	{
        stc("#7Your #nSc#7re#0e#nn #7of #nS#7m#0ok#ne#7 #7vanishes.#n\n\r",ch);
        act("#7$n has left $M #nSc#7re#0e#nn #7of #nS#7m#0ok#ne#7.#n.",ch,NULL,NULL,TO_ROOM);
        REMOVE_BIT(ch->act, AFF_HIDE);
        }
	}
*/
     void add_affect ( OBJ_DATA * obj, int apply, long value )
	{
  	AFFECT_DATA * paf;

  	if ( affect_free == NULL )
    	paf = alloc_perm( sizeof( * paf ) );
  	else
  	{
    	paf         = affect_free;
    	affect_free = affect_free->next;
  	}
  	paf->type      = 0;
  	paf->duration  = -1;
  	paf->location  = apply;
  	paf->modifier  = value;
  	paf->bitvector = 0;
  	paf->next      = obj->affected;
  	obj->affected  = paf;
	}
/*
     void do_elemshift (CHAR_DATA *ch, char *argument)
	{
  	char arg[MAX_INPUT_LENGTH];
  	char buf[MAX_STRING_LENGTH];
  	one_argument( argument, arg);

  	if (IS_NPC(ch)) return;
  	if (!IS_CLASS(ch, CLASS_ELEMENTAL)   && !IS_CLASS(ch, CLASS_GAIA_WOLF)
      	&&  !IS_CLASS(ch, CLASS_NECROMANCER) && !IS_CLASS(ch, CLASS_DRUID)
      	&&  !IS_CLASS(ch, CLASS_DEVA))
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
*/
     void rare_eq1 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
	SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#GDa#gr#0k #nE#7m#ne#0r#ga#Gld#7 %s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#GDa#gr#0k #nE#7m#ne#0r#ga#Gld#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq2 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#pCra#Pck#7e#nd #0A#nm#7e#Pth#pyst#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#pCra#Pck#7e#nd #0A#nm#7e#Pth#pyst#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq3 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#sPe#sr#sf#se#sct #sP#sr#si#ssm#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#sPe#sr#sf#se#sct #sP#sr#si#ssm#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq4 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#rBl#Ro#7o#nd S#7t#Ro#rne#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#rBl#Ro#7o#nd S#7t#Ro#rne#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq5 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#0G#no#0t#nh#0i#nc#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#0G#no#0t#nh#0i#nc#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq6 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#ySt#0u#ydd#0e#yd L#0e#yat#0h#yer#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#ySt#0u#ydd#0e#yd L#0e#yat#0h#yer#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq7 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#7Q#nu#0i#7l#nt#0e#7d#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#7Q#nu#0i#7l#nt#0e#7d#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq8 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#RRu#rn#ne #7pl#na#rt#Red#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#RRu#rn#ne #7pl#na#rt#Red#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq9 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#7O#nb#0s#ni#7d#ni#0a#nn #7P#nl#0a#nt#7e#nd #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#7O#nb#0s#ni#7d#ni#0a#nn #7P#nl#0a#nt#7e#nd #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq10 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#yW#0o#yo#nd#ye#0n #yP#nl#ya#0t#ye#nd #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#yW#0o#yo#nd#ye#0n #yP#nl#ya#0t#ye#nd #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq11 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#7S#nh#7a#nt#7t#ne#7r#ne#7d #CC#cr#7y#Cs#ct#7a#Cl#ci#7n#Ce#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#7S#nh#7a#nt#7t#ne#7r#ne#7d #CC#cr#7y#Cs#ct#7a#Cl#ci#7n#Ce#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq12 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#RCh#ra#no#0t#ri#7c #rI#0m#nb#ru#Red#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#RCh#ra#no#0t#ri#7c #rI#0m#nb#ru#Red#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq13 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#GLi#gm#0e#ns#yt#7o#yn#ne #0O#gp#Gal#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#GLi#gm#0e#ns#yt#7o#yn#ne #0O#gp#Gal#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq14 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#gVe#Gn#7o#nm #0St#na#7i#Gn#ged#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#gVe#Gn#7o#nm #0St#na#7i#Gn#ged#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq15 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#7B#no#0n#ne#7w#ne#0a#nv#7e#nd #0S#ni#7l#nk #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#7B#no#0n#ne#7w#ne#0a#nv#7e#nd #0S#ni#7l#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq16 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#RP#ru#0r#ne #RH#re#0l#nl#Rf#ri#0r#ne #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#RP#ru#0r#ne #RH#re#0l#nl#Rf#ri#0r#ne #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq17 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#0H#7o#0r#nn #0C#no#0v#7e#0r#ne#0d#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#0H#7o#0r#nn #0C#no#0v#7e#0r#ne#0d#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq18 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#YA#yn#nc#7i#ne#yn#Yt #7S#nh#0a#nd#7ow#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#YA#yn#nc#7i#ne#yn#Yt #7S#nh#0a#nd#7ow#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq19 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#LS#lk#ny #7Gu#na#lr#Ld#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#LS#lk#ny #7Gu#na#lr#Ld#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void rare_eq20 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 600 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 200   );
        damroll = number_range( 150, 200   );
        hp      = number_range( 300, 500   );
        mana    = number_range( 300, 500   );
        move    = number_range( 300, 500   );
        armor   = number_range( -300, -200 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RARE );
        SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#CS#ca#ncr#ce#Cd #7M#ni#0d#nn#7i#ng#0h#nt#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#CS#ca#ncr#ce#Cd #7M#ni#0d#nn#7i#ng#0h#nt#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Rare armor!#n\n\r", ch );
        }
        }

     void common_eq1 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 100 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 50, 100 );
        damroll = number_range( 50, 100 );
        hp      = number_range( 100, 200 );
        mana    = number_range( 100, 200 );
        move    = number_range( 100, 200 );
        armor   = number_range( -50, 150 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_COMMON );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga , "#LD#la#nm#0a#ng#le#Ld #0S#lt#Le#le#0l#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#LD#la#nm#0a#ng#le#Ld #0S#lt#Le#le#0l#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Basic %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Basic armor!#n\n\r", ch );
        }
        }

     void common_eq2 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 100 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 50, 100 );
        damroll = number_range( 50, 100 );
        hp      = number_range( 100, 200 );
        mana    = number_range( 100, 200 );
        move    = number_range( 100, 200 );
        armor   = number_range( -50, 150 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_COMMON );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#GG#gu#ni#7l#nd#ge#Gd #YC#yo#7pp#ye#Yr#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#GG#gu#ni#7l#nd#ge#Gd #YC#yo#7pp#ye#Yr#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Basic %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Basic armor!#n\n\r", ch );
        }
        }

     void common_eq3 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 100 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 50, 100 );
        damroll = number_range( 50, 100 );
        hp      = number_range( 100, 200 );
        mana    = number_range( 100, 200 );
        move    = number_range( 100, 200 );
        armor   = number_range( -50, 150 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_COMMON );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#CM#ci#7rr#co#Cr #7p#nl#7a#nt#7e#nd #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#CM#ci#7rr#co#Cr #7p#nl#7a#nt#7e#nd #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Basic %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Basic armor!#n\n\r", ch );
        }
        }

     void common_eq4 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 100 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 50, 100 );
        damroll = number_range( 50, 100 );
        hp      = number_range( 100, 200 );
        mana    = number_range( 100, 200 );
        move    = number_range( 100, 200 );
        armor   = number_range( -50, 150 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_COMMON );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#RS#rh#7a#rt#Rt#re#7r#re#Rd #rR#7u#rn#Re#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#RS#rh#7a#rt#Rt#re#7r#re#Rd #rR#7u#rn#Re#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Basic %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Basic armor!#n\n\r", ch );
        }
        }

     void common_eq5 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 100 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 50, 100 );
        damroll = number_range( 50, 100 );
        hp      = number_range( 100, 200 );
        mana    = number_range( 100, 200 );
        move    = number_range( 100, 200 );
        armor   = number_range( -50, 150 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_COMMON );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#gCr#Ga#7c#nk#0ed #nS#7c#Ga#gle#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#gCr#Ga#7c#nk#0ed #nS#7c#Ga#gle#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Basic %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Basic armor!#n\n\r", ch );
        }
        }

     void common_eq6 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 100 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 50, 100 );
        damroll = number_range( 50, 100 );
        hp      = number_range( 100, 200 );
        mana    = number_range( 100, 200 );
        move    = number_range( 100, 200 );
        armor   = number_range( -50, 150 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_COMMON );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#GE#gl#0v#ne#7n S#nt#0e#ge#Gl#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#GE#gl#0v#ne#7n S#nt#0e#ge#Gl#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Basic %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Basic armor!#n\n\r", ch );
        }
        }

     void common_eq7 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 100 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 50, 100 );
        damroll = number_range( 50, 100 );
        hp      = number_range( 100, 200 );
        mana    = number_range( 100, 200 );
        move    = number_range( 100, 200 );
        armor   = number_range( -50, 150 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_COMMON );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#YL#yi#0g#nh#7t P#nl#0a#yt#Ye#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#YL#yi#0g#nh#7t P#nl#0a#yt#Ye#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Basic %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Basic armor!#n\n\r", ch );
        }
        }

     void common_eq8 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 100 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 50, 100 );
        damroll = number_range( 50, 100 );
        hp      = number_range( 100, 200 );
        mana    = number_range( 100, 200 );
        move    = number_range( 100, 200 );
        armor   = number_range( -50, 150 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_COMMON );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#CRu#cs#0t#ne#7d Mi#nt#0h#cr#Cil#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#CRu#cs#0t#ne#7d Mi#nt#0h#cr#Cil#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Basic %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Basic armor!#n\n\r", ch );
        }
        }

     void common_eq9 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 100 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 50, 100 );
        damroll = number_range( 50, 100 );
        hp      = number_range( 100, 200 );
        mana    = number_range( 100, 200 );
        move    = number_range( 100, 200 );
        armor   = number_range( -50, 150 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_COMMON );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#LSp#ll#0i#nn#7tered #nS#0t#le#Lel#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#LSp#ll#0i#nn#7tered #nS#0t#le#Lel#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Basic %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Basic armor!#n\n\r", ch );
        }
        }

     void common_eq10 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 100 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 50, 100 );
        damroll = number_range( 50, 100 );
        hp      = number_range( 100, 200 );
        mana    = number_range( 100, 200 );
        move    = number_range( 100, 200 );
        armor   = number_range( -50, 150 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[0] = 30;

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_COMMON );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 12 );
        switch ( wear )
        {
        case 1: wear = ITEM_WEAR_FINGER;
        sprintf( name, "Ring" );
        break;
        case 2: wear = ITEM_WEAR_NECK;
        sprintf( name, "Necklace" );
        break;
        case 3: wear = ITEM_WEAR_BODY;
        sprintf( name, "Armor" );
        break;
        case 4: wear = ITEM_WEAR_HEAD;
        sprintf( name, "Helmet" );
        break;
        case 5: wear = ITEM_WEAR_LEGS;
        sprintf( name, "Leggings" );
        break;
        case 6: wear = ITEM_WEAR_FEET;
        sprintf( name, "Boots" );
        break;
        case 7: wear = ITEM_WEAR_HANDS;
        sprintf( name, "Gauntlets" );
        break;
        case 8: wear = ITEM_WEAR_ARMS;
        sprintf( name, "Sleeves" );
        break;
        case 9: wear = ITEM_WEAR_ABOUT;
        sprintf( name, "Cloak" );
        break;
        case 10: wear = ITEM_WEAR_WAIST;
        sprintf( name, "Belt" );
        break;
        case 11: wear = ITEM_WEAR_WRIST;
        sprintf( name, "Bracelet" );
        break;
        case 12: wear = ITEM_WEAR_FACE;
        sprintf( name, "Mask" );
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_ARMOR;
        sprintf( ooga ,"#0S#nt#7u#nd#0d#ne#7d #nO#0n#ny#7x#n #7%s#n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#0S#nt#7u#nd#0d#ne#7d #nO#0n#ny#7x#n #7%s#n lies on the ground.", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Basic %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a piece of Basic armor!#n\n\r", ch );
        }
        }

     void rare_weapon1 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 1000 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 400, 600 );
        damroll = number_range( 400, 600 );
        hp      = number_range( 1000, 3000 );
        mana    = number_range( 1000, 3000 );
        move    = number_range( 1000, 3000 );
        armor   = number_range( -500, -1 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[1] = number_range (400,500);
        obj->value[2] = number_range (500,600);

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RAREWEAP );
	SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 13 );
        switch ( wear )
        {
        case 1: wear = ITEM_WIELD;
        sprintf( name, "Whip");
        obj->value[3] = 4;
        obj->value[0] = 18000;
        break;
        case 2: wear = ITEM_WIELD;
        sprintf( name, "Dagger");
        obj->value[3] = 2;
        obj->value[0] = 17000;
        break;
        case 3: wear = ITEM_WIELD;
        sprintf( name, "Rapier");
        obj->value[3] = 11;
        obj->value[0] = 18000;
        break;
        case 4: wear = ITEM_WIELD;
        sprintf( name, "Claw");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 5: wear = ITEM_WIELD;
        sprintf( name, "Mace");
        obj->value[3] = 7;
        obj->value[0] = 18000;
        break;
        case 6: wear = ITEM_WIELD;
        sprintf( name, "Flail");
        obj->value[3] = 8;
        obj->value[0] = 18000;
        break;
        case 7: wear = ITEM_WIELD;
        sprintf( name, "Scythe");
        obj->value[3] = 1;
        obj->value[0] = 18000;
        break;
        case 8: wear = ITEM_WIELD;
        sprintf( name, "Kama");
        obj->value[3] = 3;
        obj->value[0] = 17000;
        break;
        case 9: wear = ITEM_WIELD;
        sprintf( name, "Katana");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        case 10: wear = ITEM_WIELD;
        sprintf( name, "Talon");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 11: wear = ITEM_WIELD;
        sprintf( name, "Revolver");
        obj->value[3] = 6;
        obj->value[0] = 17000;
        break;
        case 12: wear = ITEM_WIELD;
        sprintf( name, "Longsword");
        obj->value[3] = 3;
        obj->value[0] = 18000;
        break;
        case 13: wear = ITEM_WIELD;
        sprintf( name, "Shortsword");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_WEAPON;
        sprintf( ooga ,"#RBl#ro#7o#rd#Rly #7O#nn#0y#7x #7%s #n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#RBl#ro#7o#rd#Rly #7O#nn#0y#7x #7%s lies on the ground.#n", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare weapon %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a Rare Weapon!#n\n\r", ch );
        }
        }

     void rare_weapon2 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 1000 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 400, 600 );
        damroll = number_range( 400, 600 );
        hp      = number_range( 1000, 3000 );
        mana    = number_range( 1000, 3000 );
        move    = number_range( 1000, 3000 );
        armor   = number_range( -500, -1 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[1] = number_range (400,500);
        obj->value[2] = number_range (500,600);

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RAREWEAP );
	SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 13 );
        switch ( wear )
        {
        case 1: wear = ITEM_WIELD;
        sprintf( name, "Whip");
        obj->value[3] = 4;
        obj->value[0] = 18000;
        break;
        case 2: wear = ITEM_WIELD;
        sprintf( name, "Dagger");
        obj->value[3] = 2;
        obj->value[0] = 17000;
        break;
        case 3: wear = ITEM_WIELD;
        sprintf( name, "Rapier");
        obj->value[3] = 11;
        obj->value[0] = 18000;
        break;
        case 4: wear = ITEM_WIELD;
        sprintf( name, "Claw");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 5: wear = ITEM_WIELD;
        sprintf( name, "Mace");
        obj->value[3] = 7;
        obj->value[0] = 18000;
        break;
        case 6: wear = ITEM_WIELD;
        sprintf( name, "Flail");
        obj->value[3] = 8;
        obj->value[0] = 18000;
        break;
        case 7: wear = ITEM_WIELD;
        sprintf( name, "Scythe");
        obj->value[3] = 1;
        obj->value[0] = 18000;
        break;
        case 8: wear = ITEM_WIELD;
        sprintf( name, "Kama");
        obj->value[3] = 3;
        obj->value[0] = 17000;
        break;
        case 9: wear = ITEM_WIELD;
        sprintf( name, "Katana");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        case 10: wear = ITEM_WIELD;
        sprintf( name, "Talon");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 11: wear = ITEM_WIELD;
        sprintf( name, "Revolver");
        obj->value[3] = 6;
        obj->value[0] = 17000;
        break;
        case 12: wear = ITEM_WIELD;
        sprintf( name, "Longsword");
        obj->value[3] = 3;
        obj->value[0] = 18000;
        break;
        case 13: wear = ITEM_WIELD;
        sprintf( name, "Shortsword");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_WEAPON;
        sprintf( ooga ,"#7D#ni#7m#no#7n#nd#7e#nd #LE#ld#7g#le#Ld #7%s #n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#7D#ni#7m#no#7n#nd#7e#nd #LE#ld#7g#le#Ld #7%s lies on the ground.#n", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare weapon %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a Rare Weapon!#n\n\r", ch );
        }
        }

     void rare_weapon3 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 1000 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 400, 600 );
        damroll = number_range( 400, 600 );
        hp      = number_range( 1000, 3000 );
        mana    = number_range( 1000, 3000 );
        move    = number_range( 1000, 3000 );
        armor   = number_range( -500, -1 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[1] = number_range (400,500);
        obj->value[2] = number_range (500,600);

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RAREWEAP );
	SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 13 );
        switch ( wear )
        {
        case 1: wear = ITEM_WIELD;
        sprintf( name, "Whip");
        obj->value[3] = 4;
        obj->value[0] = 18000;
        break;
        case 2: wear = ITEM_WIELD;
        sprintf( name, "Dagger");
        obj->value[3] = 2;
        obj->value[0] = 17000;
        break;
        case 3: wear = ITEM_WIELD;
        sprintf( name, "Rapier");
        obj->value[3] = 11;
        obj->value[0] = 18000;
        break;
        case 4: wear = ITEM_WIELD;
        sprintf( name, "Claw");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 5: wear = ITEM_WIELD;
        sprintf( name, "Mace");
        obj->value[3] = 7;
        obj->value[0] = 18000;
        break;
        case 6: wear = ITEM_WIELD;
        sprintf( name, "Flail");
        obj->value[3] = 8;
        obj->value[0] = 18000;
        break;
        case 7: wear = ITEM_WIELD;
        sprintf( name, "Scythe");
        obj->value[3] = 1;
        obj->value[0] = 18000;
        break;
        case 8: wear = ITEM_WIELD;
        sprintf( name, "Kama");
        obj->value[3] = 3;
        obj->value[0] = 17000;
        break;
        case 9: wear = ITEM_WIELD;
        sprintf( name, "Katana");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        case 10: wear = ITEM_WIELD;
        sprintf( name, "Talon");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 11: wear = ITEM_WIELD;
        sprintf( name, "Revolver");
        obj->value[3] = 6;
        obj->value[0] = 17000;
        break;
        case 12: wear = ITEM_WIELD;
        sprintf( name, "Longsword");
        obj->value[3] = 3;
        obj->value[0] = 18000;
        break;
        case 13: wear = ITEM_WIELD;
        sprintf( name, "Shortsword");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_WEAPON;
        sprintf( ooga ,"#YS#yu#np#7r#ne#ym#Ye #7%s #n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#YS#yu#np#7r#ne#ym#Ye #7%s lies on the ground.#n", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare weapon %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a Rare Weapon!#n\n\r", ch );
        }
        }

     void rare_weapon4 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 1000 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 400, 600 );
        damroll = number_range( 400, 600 );
        hp      = number_range( 1000, 3000 );
        mana    = number_range( 1000, 3000 );
        move    = number_range( 1000, 3000 );
        armor   = number_range( -500, -1 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[1] = number_range (400,500);
        obj->value[2] = number_range (500,600);

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RAREWEAP );
	SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 13 );
        switch ( wear )
        {
        case 1: wear = ITEM_WIELD;
        sprintf( name, "Whip");
        obj->value[3] = 4;
        obj->value[0] = 18000;
        break;
        case 2: wear = ITEM_WIELD;
        sprintf( name, "Dagger");
        obj->value[3] = 2;
        obj->value[0] = 17000;
        break;
        case 3: wear = ITEM_WIELD;
        sprintf( name, "Rapier");
        obj->value[3] = 11;
        obj->value[0] = 18000;
        break;
        case 4: wear = ITEM_WIELD;
        sprintf( name, "Claw");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 5: wear = ITEM_WIELD;
        sprintf( name, "Mace");
        obj->value[3] = 7;
        obj->value[0] = 18000;
        break;
        case 6: wear = ITEM_WIELD;
        sprintf( name, "Flail");
        obj->value[3] = 8;
        obj->value[0] = 18000;
        break;
        case 7: wear = ITEM_WIELD;
        sprintf( name, "Scythe");
        obj->value[3] = 1;
        obj->value[0] = 18000;
        break;
        case 8: wear = ITEM_WIELD;
        sprintf( name, "Kama");
        obj->value[3] = 3;
        obj->value[0] = 17000;
        break;
        case 9: wear = ITEM_WIELD;
        sprintf( name, "Katana");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        case 10: wear = ITEM_WIELD;
        sprintf( name, "Talon");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 11: wear = ITEM_WIELD;
        sprintf( name, "Revolver");
        obj->value[3] = 6;
        obj->value[0] = 17000;
        break;
        case 12: wear = ITEM_WIELD;
        sprintf( name, "Longsword");
        obj->value[3] = 3;
        obj->value[0] = 18000;
        break;
        case 13: wear = ITEM_WIELD;
        sprintf( name, "Shortsword");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_WEAPON;
        sprintf( ooga ,"#0E#nb#0o#nn#0y #CC#cr#ne#7sc#ne#cn#Ct #7%s #n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#0E#nb#0o#nn#0y #CC#cr#ne#7sc#ne#cn#Ct #7%s lies on the ground.#n", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare weapon %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a Rare Weapon!#n\n\r", ch );
        }
        }

     void rare_weapon5 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 1000 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 400, 600 );
        damroll = number_range( 400, 600 );
        hp      = number_range( 1000, 3000 );
        mana    = number_range( 1000, 3000 );
        move    = number_range( 1000, 3000 );
        armor   = number_range( -500, -1 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[1] = number_range (400,500);
        obj->value[2] = number_range (500,600);

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RAREWEAP );
	SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 13 );
        switch ( wear )
        {
        case 1: wear = ITEM_WIELD;
        sprintf( name, "Whip");
        obj->value[3] = 4;
        obj->value[0] = 18000;
        break;
        case 2: wear = ITEM_WIELD;
        sprintf( name, "Dagger");
        obj->value[3] = 2;
        obj->value[0] = 17000;
        break;
        case 3: wear = ITEM_WIELD;
        sprintf( name, "Rapier");
        obj->value[3] = 11;
        obj->value[0] = 18000;
        break;
        case 4: wear = ITEM_WIELD;
        sprintf( name, "Claw");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 5: wear = ITEM_WIELD;
        sprintf( name, "Mace");
        obj->value[3] = 7;
        obj->value[0] = 18000;
        break;
        case 6: wear = ITEM_WIELD;
        sprintf( name, "Flail");
        obj->value[3] = 8;
        obj->value[0] = 18000;
        break;
        case 7: wear = ITEM_WIELD;
        sprintf( name, "Scythe");
        obj->value[3] = 1;
        obj->value[0] = 18000;
        break;
        case 8: wear = ITEM_WIELD;
        sprintf( name, "Kama");
        obj->value[3] = 3;
        obj->value[0] = 17000;
        break;
        case 9: wear = ITEM_WIELD;
        sprintf( name, "Katana");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        case 10: wear = ITEM_WIELD;
        sprintf( name, "Talon");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 11: wear = ITEM_WIELD;
        sprintf( name, "Revolver");
        obj->value[3] = 6;
        obj->value[0] = 17000;
        break;
        case 12: wear = ITEM_WIELD;
        sprintf( name, "Longsword");
        obj->value[3] = 3;
        obj->value[0] = 18000;
        break;
        case 13: wear = ITEM_WIELD;
        sprintf( name, "Shortsword");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_WEAPON;
        sprintf( ooga ,"#GO#gr#ng#7a#nn#gi#Gc #7%s #n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#GO#gr#ng#7a#nn#gi#Gc #7%s lies on the ground.#n", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare weapon %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a Rare Weapon!#n\n\r", ch );
        }
        }

     void rare_weapon6 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 1000 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 400, 600 );
        damroll = number_range( 400, 600 );
        hp      = number_range( 1000, 3000 );
        mana    = number_range( 1000, 3000 );
        move    = number_range( 1000, 3000 );
        armor   = number_range( -500, -1 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[1] = number_range (400,500);
        obj->value[2] = number_range (500,600);

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RAREWEAP );
	SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 13 );
        switch ( wear )
        {
        case 1: wear = ITEM_WIELD;
        sprintf( name, "Whip");
        obj->value[3] = 4;
        obj->value[0] = 18000;
        break;
        case 2: wear = ITEM_WIELD;
        sprintf( name, "Dagger");
        obj->value[3] = 2;
        obj->value[0] = 17000;
        break;
        case 3: wear = ITEM_WIELD;
        sprintf( name, "Rapier");
        obj->value[3] = 11;
        obj->value[0] = 18000;
        break;
        case 4: wear = ITEM_WIELD;
        sprintf( name, "Claw");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 5: wear = ITEM_WIELD;
        sprintf( name, "Mace");
        obj->value[3] = 7;
        obj->value[0] = 18000;
        break;
        case 6: wear = ITEM_WIELD;
        sprintf( name, "Flail");
        obj->value[3] = 8;
        obj->value[0] = 18000;
        break;
        case 7: wear = ITEM_WIELD;
        sprintf( name, "Scythe");
        obj->value[3] = 1;
        obj->value[0] = 18000;
        break;
        case 8: wear = ITEM_WIELD;
        sprintf( name, "Kama");
        obj->value[3] = 3;
        obj->value[0] = 17000;
        break;
        case 9: wear = ITEM_WIELD;
        sprintf( name, "Katana");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        case 10: wear = ITEM_WIELD;
        sprintf( name, "Talon");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 11: wear = ITEM_WIELD;
        sprintf( name, "Revolver");
        obj->value[3] = 6;
        obj->value[0] = 17000;
        break;
        case 12: wear = ITEM_WIELD;
        sprintf( name, "Longsword");
        obj->value[3] = 3;
        obj->value[0] = 18000;
        break;
        case 13: wear = ITEM_WIELD;
        sprintf( name, "Shortsword");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_WEAPON;
        sprintf( ooga ,"#PD#pe#0a#nt#7h#ns#0o#pn#Pg #7%s #n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#PD#pe#0a#nt#7h#ns#0o#pn#Pg #7%s lies on the ground.#n", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare weapon %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a Rare Weapon!#n\n\r", ch );
        }
        }

     void rare_weapon7 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 1000 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 400, 600 );
        damroll = number_range( 400, 600 );
        hp      = number_range( 1000, 3000 );
        mana    = number_range( 1000, 3000 );
        move    = number_range( 1000, 3000 );
        armor   = number_range( -500, -1 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[1] = number_range (400,500);
        obj->value[2] = number_range (500,600);

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RAREWEAP );
	SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 13 );
        switch ( wear )
        {
        case 1: wear = ITEM_WIELD;
        sprintf( name, "Whip");
        obj->value[3] = 4;
        obj->value[0] = 18000;
        break;
        case 2: wear = ITEM_WIELD;
        sprintf( name, "Dagger");
        obj->value[3] = 2;
        obj->value[0] = 17000;
        break;
        case 3: wear = ITEM_WIELD;
        sprintf( name, "Rapier");
        obj->value[3] = 11;
        obj->value[0] = 18000;
        break;
        case 4: wear = ITEM_WIELD;
        sprintf( name, "Claw");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 5: wear = ITEM_WIELD;
        sprintf( name, "Mace");
        obj->value[3] = 7;
        obj->value[0] = 18000;
        break;
        case 6: wear = ITEM_WIELD;
        sprintf( name, "Flail");
        obj->value[3] = 8;
        obj->value[0] = 18000;
        break;
        case 7: wear = ITEM_WIELD;
        sprintf( name, "Scythe");
        obj->value[3] = 1;
        obj->value[0] = 18000;
        break;
        case 8: wear = ITEM_WIELD;
        sprintf( name, "Kama");
        obj->value[3] = 3;
        obj->value[0] = 17000;
        break;
        case 9: wear = ITEM_WIELD;
        sprintf( name, "Katana");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        case 10: wear = ITEM_WIELD;
        sprintf( name, "Talon");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 11: wear = ITEM_WIELD;
        sprintf( name, "Revolver");
        obj->value[3] = 6;
        obj->value[0] = 17000;
        break;
        case 12: wear = ITEM_WIELD;
        sprintf( name, "Longsword");
        obj->value[3] = 3;
        obj->value[0] = 18000;
        break;
        case 13: wear = ITEM_WIELD;
        sprintf( name, "Shortsword");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_WEAPON;
        sprintf( ooga ,"#gC#Gr#7y#np#7t#Gi#gc #7%s #n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#gC#Gr#7y#np#7t#Gi#gc #7%s lies on the ground.#n", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare weapon %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a Rare Weapon!#n\n\r", ch );
        }
        }

     void rare_weapon8 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 1000 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 400, 600 );
        damroll = number_range( 400, 600 );
        hp      = number_range( 1000, 3000 );
        mana    = number_range( 1000, 3000 );
        move    = number_range( 1000, 3000 );
        armor   = number_range( -500, -1 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[1] = number_range (400,500);
        obj->value[2] = number_range (500,600);

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RAREWEAP );
	SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 13 );
        switch ( wear )
        {
        case 1: wear = ITEM_WIELD;
        sprintf( name, "Whip");
        obj->value[3] = 4;
        obj->value[0] = 18000;
        break;
        case 2: wear = ITEM_WIELD;
        sprintf( name, "Dagger");
        obj->value[3] = 2;
        obj->value[0] = 17000;
        break;
        case 3: wear = ITEM_WIELD;
        sprintf( name, "Rapier");
        obj->value[3] = 11;
        obj->value[0] = 18000;
        break;
        case 4: wear = ITEM_WIELD;
        sprintf( name, "Claw");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 5: wear = ITEM_WIELD;
        sprintf( name, "Mace");
        obj->value[3] = 7;
        obj->value[0] = 18000;
        break;
        case 6: wear = ITEM_WIELD;
        sprintf( name, "Flail");
        obj->value[3] = 8;
        obj->value[0] = 18000;
        break;
        case 7: wear = ITEM_WIELD;
        sprintf( name, "Scythe");
        obj->value[3] = 1;
        obj->value[0] = 18000;
        break;
        case 8: wear = ITEM_WIELD;
        sprintf( name, "Kama");
        obj->value[3] = 3;
        obj->value[0] = 17000;
        break;
        case 9: wear = ITEM_WIELD;
        sprintf( name, "Katana");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        case 10: wear = ITEM_WIELD;
        sprintf( name, "Talon");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 11: wear = ITEM_WIELD;
        sprintf( name, "Revolver");
        obj->value[3] = 6;
        obj->value[0] = 17000;
        break;
        case 12: wear = ITEM_WIELD;
        sprintf( name, "Longsword");
        obj->value[3] = 3;
        obj->value[0] = 18000;
        break;
        case 13: wear = ITEM_WIELD;
        sprintf( name, "Shortsword");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_WEAPON;
        sprintf( ooga ,"#7Mi#nd#0ni#ng#7ht #RH#re#0l#Rl#rf#0i#Rr#re #7%s #n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#7Mi#nd#0ni#ng#7ht #RH#re#0l#Rl#rf#0i#Rr#re #7%s lies on the ground.#n", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare weapon %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a Rare Weapon!#n\n\r", ch );
        }
        }

     void rare_weapon9 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 1000 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 400, 600 );
        damroll = number_range( 400, 600 );
        hp      = number_range( 1000, 3000 );
        mana    = number_range( 1000, 3000 );
        move    = number_range( 1000, 3000 );
        armor   = number_range( -500, -1 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[1] = number_range (400,500);
        obj->value[2] = number_range (500,600);

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RAREWEAP );
	SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 13 );
        switch ( wear )
        {
        case 1: wear = ITEM_WIELD;
        sprintf( name, "Whip");
        obj->value[3] = 4;
        obj->value[0] = 18000;
        break;
        case 2: wear = ITEM_WIELD;
        sprintf( name, "Dagger");
        obj->value[3] = 2;
        obj->value[0] = 17000;
        break;
        case 3: wear = ITEM_WIELD;
        sprintf( name, "Rapier");
        obj->value[3] = 11;
        obj->value[0] = 18000;
        break;
        case 4: wear = ITEM_WIELD;
        sprintf( name, "Claw");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 5: wear = ITEM_WIELD;
        sprintf( name, "Mace");
        obj->value[3] = 7;
        obj->value[0] = 18000;
        break;
        case 6: wear = ITEM_WIELD;
        sprintf( name, "Flail");
        obj->value[3] = 8;
        obj->value[0] = 18000;
        break;
        case 7: wear = ITEM_WIELD;
        sprintf( name, "Scythe");
        obj->value[3] = 1;
        obj->value[0] = 18000;
        break;
        case 8: wear = ITEM_WIELD;
        sprintf( name, "Kama");
        obj->value[3] = 3;
        obj->value[0] = 17000;
        break;
        case 9: wear = ITEM_WIELD;
        sprintf( name, "Katana");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        case 10: wear = ITEM_WIELD;
        sprintf( name, "Talon");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 11: wear = ITEM_WIELD;
        sprintf( name, "Revolver");
        obj->value[3] = 6;
        obj->value[0] = 17000;
        break;
        case 12: wear = ITEM_WIELD;
        sprintf( name, "Longsword");
        obj->value[3] = 3;
        obj->value[0] = 18000;
        break;
        case 13: wear = ITEM_WIELD;
        sprintf( name, "Shortsword");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_WEAPON;
        sprintf( ooga ,"#CCr#cy#0s#nt#7a#nl#0l#ci#Cne #0D#re#Rm#ro#0n #7%s #n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#CCr#cy#0s#nt#7a#nl#0l#ci#Cne #0D#re#Rm#ro#0n #7%s lies on the ground.#n", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare weapon %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a Rare Weapon!#n\n\r", ch );
        }
        }

     void rare_weapon10 ( CHAR_DATA * ch, CHAR_DATA * victim )
        {
        OBJ_INDEX_DATA * pObjIndex;
        OBJ_DATA * obj;
        char name [MSL];
        char ooga [MSL];
        int hitroll = 0;
        int damroll = 0;
        int hp      = 0;
        int mana    = 0;
        int move    = 0;
        int wear    = 0;
        int armor   = 0;
        int rnum    = 30037;

        if ( IS_NPC( ch ) )
        return;
        if ( number_range( 1 , 1000 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 400, 600 );
        damroll = number_range( 400, 600 );
        hp      = number_range( 1000, 3000 );
        mana    = number_range( 1000, 3000 );
        move    = number_range( 1000, 3000 );
        armor   = number_range( -500, -1 );
        add_affect( obj, APPLY_HITROLL, hitroll );
        add_affect( obj, APPLY_DAMROLL, damroll );
        add_affect( obj, APPLY_HIT, hp );
        add_affect( obj, APPLY_MANA, mana );
        add_affect( obj, APPLY_MOVE, move );
        add_affect( obj, APPLY_AC, armor );
        obj->value[1] = number_range (400,500);
        obj->value[2] = number_range (500,600);

        SET_BIT( obj->quest, QUEST_RELIC );
        SET_BIT( obj->spectype, SITEM_RAREWEAP );
	SET_BIT( obj->spectype, SITEM_UNID );
        SET_BIT( obj->wear_flags, ITEM_TAKE );
        obj->questowner = str_dup( ch->name );
        wear = number_range( 1, 13 );
        switch ( wear )
        {
        case 1: wear = ITEM_WIELD;
        sprintf( name, "Whip");
        obj->value[3] = 4;
        obj->value[0] = 18000;
        break;
        case 2: wear = ITEM_WIELD;
        sprintf( name, "Dagger");
        obj->value[3] = 2;
        obj->value[0] = 17000;
        break;
        case 3: wear = ITEM_WIELD;
        sprintf( name, "Rapier");
        obj->value[3] = 11;
        obj->value[0] = 18000;
        break;
        case 4: wear = ITEM_WIELD;
        sprintf( name, "Claw");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 5: wear = ITEM_WIELD;
        sprintf( name, "Mace");
        obj->value[3] = 7;
        obj->value[0] = 18000;
        break;
        case 6: wear = ITEM_WIELD;
        sprintf( name, "Flail");
        obj->value[3] = 8;
        obj->value[0] = 18000;
        break;
        case 7: wear = ITEM_WIELD;
        sprintf( name, "Scythe");
        obj->value[3] = 1;
        obj->value[0] = 18000;
        break;
        case 8: wear = ITEM_WIELD;
        sprintf( name, "Kama");
        obj->value[3] = 3;
        obj->value[0] = 17000;
        break;
        case 9: wear = ITEM_WIELD;
        sprintf( name, "Katana");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        case 10: wear = ITEM_WIELD;
        sprintf( name, "Talon");
        obj->value[3] = 5;
        obj->value[0] = 17000;
        break;
        case 11: wear = ITEM_WIELD;
        sprintf( name, "Revolver");
        obj->value[3] = 6;
        obj->value[0] = 17000;
        break;
        case 12: wear = ITEM_WIELD;
        sprintf( name, "Longsword");
        obj->value[3] = 3;
        obj->value[0] = 18000;
        break;
        case 13: wear = ITEM_WIELD;
        sprintf( name, "Shortsword");
        obj->value[3] = 1;
        obj->value[0] = 17000;
        break;
        }
        SET_BIT( obj->wear_flags, wear );
        obj->item_type = ITEM_WEAPON;
        sprintf( ooga ,"#LAt#ll#na#7n#nt#le#Lan #7%s #n", name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( ooga );
        sprintf( ooga ,"#LAt#ll#na#7n#nt#le#Lan #7%s lies on the ground.#n", name );
        free_string( obj->description );
        obj->description = str_dup( ooga );
        sprintf( ooga, "Rare weapon %s ", name );
        free_string( obj->name );
        obj->name = str_dup( ooga );
        obj_to_char( obj, ch );
        act( "$p #7appears in your hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_CHAR );
        act( "$p #7appears in $n's hands in a great #Ye#yxplosion#7.#n", ch, obj, NULL, TO_ROOM );
        stc( "#7You have obtained a Rare Weapon!#n\n\r", ch );
        }
        }


     void do_objsell(CHAR_DATA *ch, char *argument)
	{
        char arg [MAX_INPUT_LENGTH];
        ROOM_INDEX_DATA *in_room;
        char buf[MAX_STRING_LENGTH];
	OBJ_DATA * obj;
	int sprice;

        argument = one_argument(argument, arg);       

     	if ( IS_NPC(ch) )
	return;
    	if ( arg[0] == '\0' )
	{
	send_to_char("Sell what?\n\r", ch);
	return;
	}
      	in_room = ch->in_room;
        if ( ch->in_room > get_room_index(3006) )
        {
        stc("You can only sell equipment to Griswold at the Blacksmith's Shop.#n\n\r",ch);
        return;
        }
        in_room = ch->in_room;
        if ( ch->in_room < get_room_index(3006) )
        {
        stc("You can only sell equipment to Griswold at the Blacksmith's Shop.#n\n\r",ch);
        return;
        }
    	if ( (obj = get_obj_carry( ch, arg, ch)) == NULL )
    	{
        send_to_char("You aren't carrying that item.\n\r",ch);
        return;
    	}
        if ( IS_SET( obj->spectype, SITEM_UNID ) )
        {
        send_to_char("That item has not been identified yet.\n\r", ch);
        return;
        }
	if ( IS_SET( obj->spectype, SITEM_COMMON ) ) 
	{	
   	sprice = number_range( 500, 2000 );
     	obj_from_char( obj );
	ch->bones += sprice;
	send_to_char("Griswold says '#YWhat can i do for ya?#n'.\n\r", ch);
	do_say(ch, "I found this while fighting through the dungeons");
	sprintf(buf, "You show Griswold %s#n.\n\r", obj->short_descr);
	send_to_char(buf, ch);
        sprintf(buf, "Griswold says '#YOHhHHhHh %s #Ylooks like a dandy#n'.\n\r", obj->short_descr);
	send_to_char(buf, ch);
	do_say(ch, "How much can I get for it?");
	sprintf(buf, "Griswold says '#YI'll take it for the price of %d bones#n'.\n\r", sprice);
	send_to_char(buf, ch);
	do_say(ch, "Hrmm..... Okay.");
        sprintf(buf, "You hand %s #nto Griswold the Blacksmith.\n\r", obj->short_descr);
        send_to_char(buf, ch);
	sprintf(buf, "Griswold hands you %d bones.\n\r", sprice);
	send_to_char(buf, ch);
        do_say(ch, "Thank you Griswold!" );     
        sprintf(buf, "Griswold says '#YNo problem %s, come back any time#n'.\n\r", ch->pcdata->switchname);
	send_to_char(buf, ch);
	return;
	}
        if ( IS_SET( obj->spectype, SITEM_RARE ) )
        {
        sprice = number_range( 3000, 6000 );
        obj_from_char( obj );
        ch->bones += sprice;
        send_to_char("Griswold says '#YWhat can i do for ya?#n'.\n\r", ch);
        do_say(ch, "I found this while fighting through the dungeons");
        sprintf(buf, "You show Griswold %s#n.\n\r", obj->short_descr);
        send_to_char(buf, ch);
        sprintf(buf, "Griswold says '#YOHhHHhHh %s #Ylooks like a dandy#n'.\n\r", obj->short_descr);
        send_to_char(buf, ch);
        do_say(ch, "How much can I get for it?");
        sprintf(buf, "Griswold says '#YI'll take it for the price of %d bones#n'.\n\r", sprice);
        send_to_char(buf, ch);
        do_say(ch, "Hrmm..... Okay.");
        sprintf(buf, "You hand %s #nto Griswold the Blacksmith.\n\r", obj->short_descr);
        send_to_char(buf, ch);
        sprintf(buf, "Griswold hands you %d bones.\n\r", sprice);
        send_to_char(buf, ch);
        do_say(ch, "Thank you Griswold!" );
        sprintf(buf, "Griswold says '#YNo problem %s, come back any time#n'.\n\r", ch->pcdata->switchname);
        send_to_char(buf, ch);
        return;
        }
        if ( IS_SET( obj->spectype, SITEM_RAREWEAP ) )
        {
        sprice = number_range( 3000, 6000 );
        obj_from_char( obj );
        ch->bones += sprice;
        send_to_char("Griswold says '#YWhat can i do for ya?#n'.\n\r", ch);
        do_say(ch, "I found this while fighting through the dungeons");
        sprintf(buf, "You show Griswold %s#n.\n\r", obj->short_descr);
        send_to_char(buf, ch);
        sprintf(buf, "Griswold says '#YOHhHHhHh %s #Ylooks like a dandy#n'.\n\r", obj->short_descr);
        send_to_char(buf, ch);
        do_say(ch, "How much can I get for it?");
        sprintf(buf, "Griswold says '#YI'll take it for the price of %d bones#n'.\n\r", sprice);
        send_to_char(buf, ch);
        do_say(ch, "Hrmm..... Okay.");
        sprintf(buf, "You hand %s #nto Griswold the Blacksmith.\n\r", obj->short_descr);
        send_to_char(buf, ch);
        sprintf(buf, "Griswold hands you %d bones.\n\r", sprice);
        send_to_char(buf, ch);
        do_say(ch, "Thank you Griswold!" );
        sprintf(buf, "Griswold says '#YNo problem %s, come back any time#n'.\n\r", ch->pcdata->switchname);
        send_to_char(buf, ch);
        return;
        }

	else
	{
	send_to_char("Griswold says '#YI don't want that piece of junk.#n'.\n\r", ch);
	return;
	}
	}

     void do_identify(CHAR_DATA *ch, char *argument)
        {
        char arg [MAX_INPUT_LENGTH];
        ROOM_INDEX_DATA *in_room;
        char buf[MAX_STRING_LENGTH];
        OBJ_DATA * obj;
        int sprice;

        argument = one_argument(argument, arg);

        if ( IS_NPC(ch) )
        return;
        if ( arg[0] == '\0' )
        {
        send_to_char("Identify what?\n\r", ch);
        return;
        }
        in_room = ch->in_room;
        if ( ch->in_room > get_room_index(3007) )
        {
        stc("Only Caine can identify equipment.#n\n\r",ch);
        return;
        }
        in_room = ch->in_room;
        if ( ch->in_room < get_room_index(3007) )
        {
        stc("Only Caine can identify equipment.#n\n\r",ch);
        return;
	}
        if ( (obj = get_obj_carry( ch, arg, ch)) == NULL )
        {
        send_to_char("You aren't carrying that item.\n\r",ch);
        return;
        }
        if ( !IS_SET( obj->spectype, SITEM_UNID ) )
        {
        send_to_char("That item has already been identified.\n\r", ch);
        return;
        }
        sprice = number_range( 1000, 3000 );
     	if ( ch->bones < 3000 )
	{
	send_to_char("You need atleast 3000 bones identify a piece of Rare armor.\n\r", ch);
	return;
	}
    	ch->bones -= sprice;
	do_say(ch, "Caine i got something for you to identify for me.");
	send_to_char("Caine says '#YHMmMmm its gonna cost you lad#n'.\n\r", ch);
	sprintf(buf, "You hand %s #nto Caine.\n\r", obj->short_descr);
	send_to_char(buf, ch);
	send_to_char("Caine chants the words '#sA#h#sU#sM#sa #sH#sU#sbb#so#sO #sw#sa#st#sO#sO#sO#so#so#sO#sO#n'.\n\r", ch);
	sprintf(buf, "%s #nbegins to glow a bright #YY#yellow #ncolor\n\r", obj->short_descr); 
	send_to_char(buf, ch);
	send_to_char("Caine says '#YOHhhHHh that be a nice piece of armor#n'.\n\r", ch);
	sprintf(buf, "Caine says '#YThats gonna cost you %d bones#n'.\n\r", sprice);
	send_to_char(buf, ch);
	do_say(ch, "Ok caine lemme get that for you");
	sprintf(buf, "You hand %d bones to Caine#n\n\r", sprice);
	send_to_char(buf, ch);
	send_to_char("Caine says '#YAhhh Thank you lad, Here you go#n'.\n\r", ch);
	sprintf(buf, "Caine hands %s #nto you\n\r", obj->short_descr);
	send_to_char(buf, ch);
        REMOVE_BIT(obj->spectype, SITEM_UNID);
     	return;
        }

     void do_weapsell(CHAR_DATA *ch, char *argument)
        {
        char arg [MAX_INPUT_LENGTH];
        ROOM_INDEX_DATA *in_room;
        char buf[MAX_STRING_LENGTH];
        OBJ_DATA * obj;
        int sprice;

        argument = one_argument(argument, arg);

        if ( IS_NPC(ch) )
        return;
        if ( arg[0] == '\0' )
        {
        send_to_char("Sell what?\n\r", ch);
        return;
        }
        in_room = ch->in_room;
        if ( ch->in_room > get_room_index(20) )
        {
        stc("You can only sell weapons to Wirt by the cave entrance.#n\n\r",ch);
        return;
        }
        in_room = ch->in_room;
        if ( ch->in_room < get_room_index(20) )
        {
        stc("You can only sell weapons to Wirt by the cave entrance.#n\n\r",ch);
        return;
        }
        if ( (obj = get_obj_carry( ch, arg, ch)) == NULL )
        {
        send_to_char("You aren't carrying that item.\n\r",ch);
        return;
        }
        if ( IS_SET( obj->spectype, SITEM_UNID ) )
        {
        send_to_char("That item has not been identified yet.\n\r", ch);
        return;
        }
        if ( !IS_SET( obj->spectype, SITEM_RAREWEAP ) )
        {
        send_to_char("You can only sell Rare Weapons.\n\r", ch);
        return;
        }
        sprice = number_range( 5000, 10000 );
        obj_from_char( obj );
        ch->bones += sprice;
        send_to_char("Wirt says '#YHow can I help you?#n'.\n\r", ch);
        do_say(ch, "I'd like to sell some weapons are you interested?");
	send_to_char("Wirt says '#YLook at this leg.. OF COURSE I NEED WEAPONS!#n'.\n\r", ch);
	do_say(ch, "Okay okay.. this is what i want to sell");
	sprintf(buf, "You hand %s #nto Wirt.\n\r", obj->short_descr);
	send_to_char(buf, ch);
	sprintf(buf, "Wirt examines %s#n\n\r", obj->short_descr);
	send_to_char(buf, ch);
	send_to_char("Wirt says '#YHmmm.. nice average.. decent hit/dam.. ARMOR CLASS!!#n'.\n\r", ch);
	sprintf(buf, "Wirt says '#YI'll give you %d for it.#n'.\n\r", sprice);
	send_to_char(buf, ch);
	do_say(ch, "FUCK YEH WIRT! You got a deal!");
	sprintf(buf, "Wirt hands you %d bones.#n\n\r", sprice);
	send_to_char(buf, ch);
	do_say(ch, "Thank you Wirt.");
	sprintf(buf, "Wirt says '#YYeah yeah.. now get the fuck outta here before i cut you with my new %s#n!'.\n\r", obj->short_descr);
	send_to_char(buf, ch);
	send_to_char("Wirt grins evily.#n\n\r", ch);
	do_west( ch, "" );
	return;
	}

/*     void do_mercy( CHAR_DATA *ch, char *argument )
	{
	CHAR_DATA *victim;
	char arg[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];

	one_argument( argument, arg );

	if ( IS_NPC( ch ) ) return;
 	if ( arg[0] == '\0' )
 	{
 	send_to_char( "Show mercy to whom?\n\r", ch );
 	return;
 	}
  	if (ch->in_room != NULL)
  	{
    	if (IS_SET (ch->in_room->room_flags, ROOM_ARENA))
    	{
      	send_to_char("Your in the arena.\n\r",ch);
      	return;
    	}
  	}
  	if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
  	{
    	send_to_char( "They aren't here.\n\r", ch );
    	return;
  	}
  	if (ch->hitfirst == FALSE)
   	{
    	send_to_char("You attacked them first fool.\n\r", ch);
    	return;
   	}
  	if ( ch == victim )
  	{
    	send_to_char( "That might be a bit tricky...\n\r", ch );
    	return;
  	}
  	if (IS_NPC(victim))
  	{
    	send_to_char( "You can only decapitate other players.\n\r", ch );
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
 	if (IS_NPC(victim))
 	{
 	send_to_char( "Show mercy to a mob? NEVER!\n\r",ch);
 	return;
 	}
 	if (is_safe(ch,victim)) return;
  	sprintf(buf, "#P%s #0has been left for dead by #C%s#n.",victim->pcdata->switchname,ch->pcdata->switchname);
  	do_info(ch, buf);
  	sprintf(log_buf, "%s shown mercy by %s at %d.",victim->pcdata->switchname,ch->pcdata->switchname);
  	log_string(LOG_COMMAND, log_buf);
  	victim->fight_timer += 10;
  	if (ch->fight_timer > 10)
  	ch->fight_timer = 10;
  	act( "You bring your weapon down upon $N's neck!",  ch, NULL, victim, TO_CHAR);
  	sprintf(buf, "Your life is spared by the mercy of %s!\n\r", ch->pcdata->switchname);
  	stc(buf, victim);
  	act( "$n swings $s weapon down towards $N's neck!",  ch, NULL, victim, TO_NOTVICT);
  	act( "$n's head is spared by $N's mercy!", victim, NULL, ch, TO_ROOM);
  	act( "The gods shine upon the mecrcy you have shown $N.",  ch, NULL, victim, TO_VICT);
  	act( "The gods shine upon $n as $m shows $N mercy.",  ch, NULL, victim, TO_NOTVICT);
  	act( "You fall to the ground in agony as the gods smile on you.",  ch, NULL, NULL, TO_CHAR);
  	act( "$n falls to the ground as the gods smile at $m.",  ch, NULL, NULL, TO_NOTVICT);
  	if ( ch->hit < ch->max_hit ) ch->hit = ch->max_hit;
  	if ( ch->mana < ch->max_mana ) ch->mana = ch->max_mana;
  	if ( ch->move < ch->max_move ) ch->move = ch->max_move;
  	char_from_room(victim);
  	char_to_room(victim,get_room_index(ROOM_VNUM_ALTAR));
  	ch->fighting = NULL;
  	victim->fighting = NULL;
  	ch->hitfirst = FALSE;
  	victim->hitfirst = FALSE;
  	update_pos(victim);
  	WAIT_STATE(ch, 24);
  	WAIT_STATE(victim,96);
  	return;
	}
*/
