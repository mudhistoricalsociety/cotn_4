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
        sprice = number_range( 2000, 5000 );
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
        sprice = number_range( 2000, 5000 );
     	if ( ch->bones < 5000 )
	{
	send_to_char("You need atleast 5000 bones to identify a piece of Rare armor.\n\r", ch);
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 150, 250   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 2500 ) == 1 )
        {
        if ( rnum == 0 || ( pObjIndex = get_obj_index( rnum ) ) == NULL )
        {
        stc( "Missing object, please notify a member of the staff.\n\r", ch );
        return;
        }
        obj     = create_object( pObjIndex, 52 );
        hitroll = number_range( 80, 120   );
        damroll = number_range( 80, 120   );
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
        if ( number_range( 1 , 200 ) == 1 )
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
        if ( number_range( 1 , 200 ) == 1 )
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
        if ( number_range( 1 , 200 ) == 1 )
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
        if ( number_range( 1 , 200 ) == 1 )
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
        if ( number_range( 1 , 200 ) == 1 )
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
        if ( number_range( 1 , 200 ) == 1 )
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
        if ( number_range( 1 , 200 ) == 1 )
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
        if ( number_range( 1 , 200 ) == 1 )
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
        if ( number_range( 1 , 200 ) == 1 )
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
        if ( number_range( 1 , 200 ) == 1 )
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
        if ( number_range( 1 , 6000 ) == 1 )
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
        if ( number_range( 1 , 6000 ) == 1 )
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
        if ( number_range( 1 , 6000 ) == 1 )
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
        if ( number_range( 1 , 6000 ) == 1 )
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
        if ( number_range( 1 , 6000 ) == 1 )
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
        if ( number_range( 1 , 6000 ) == 1 )
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
        if ( number_range( 1 , 6000 ) == 1 )
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
        if ( number_range( 1 , 6000 ) == 1 )
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
        if ( number_range( 1 , 6000 ) == 1 )
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
        if ( number_range( 1 , 6000 ) == 1 )
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

