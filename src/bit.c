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
 *  Dystopia Mud improvements copyright (C) 2000, 2001 by Brian Graversen  *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
/***************************************************************************
 *                                 _/                            _/        *
 *      _/_/_/  _/_/      _/_/_/  _/    _/_/    _/    _/    _/_/_/         *
 *     _/    _/    _/  _/        _/  _/    _/  _/    _/  _/    _/          *
 *    _/    _/    _/  _/        _/  _/    _/  _/    _/  _/    _/           *
 *   _/    _/    _/    _/_/_/  _/    _/_/      _/_/_/    _/_/_/            *
 ***************************************************************************
 * Mindcloud Copyright 2001-2003 by Jeff Boschee (Zarius),                 *
 * Additional credits are in the help file CODECREDITS                     *
 * All Rights Reserved.                                                    *
 ***************************************************************************/
/***************************************************************************
 *  File: bit.c                                                            *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 *                                                                         *
 *  This code was written by Jason Dinkel and inspired by Russ Taylor,     *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.                *
 *                                                                         *
 ***************************************************************************/

/*
 The code below uses a table lookup system that is based on suggestions
 from Russ Taylor.  There are many routines in handler.c that would benefit
 with the use of tables.  You may consider simplifying your code base by
 implementing a system like below with such functions. -Jason Dinkel
*/



#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



struct flag_stat_type
{
        const struct flag_type *structure;
        bool      stat;
};



/*****************************************************************************
 Name:		flag_stat_table
 Purpose:	This table catagorizes the tables following the lookup
 		functions below into stats and flags.  Flags can be toggled
 		but stats can only be assigned.  Update this table when a
 		new set of flags is installed.
 ****************************************************************************/
const struct flag_stat_type flag_stat_table[] = {
/*  {	structure		stat	}, */
        {area_flags, FALSE},
        {area_bits, FALSE},
        {hobbit_dishes, FALSE},
        {monk_combos, FALSE},
        {sex_flags, TRUE},
        {exit_flags, FALSE},
        {door_resets, TRUE},
        {room_flags, FALSE},
        {sector_flags, TRUE},
        {asector_flags, TRUE},
        {weather_flags, TRUE},
        {aweather_flags, TRUE},
        {type_flags, TRUE},
        {extra_flags, FALSE},
        {wear_flags, FALSE},
        {act_flags, FALSE},
        {affect_flags, FALSE},
        {apply_flags, TRUE},
        {wear_loc_flags, TRUE},
        {wear_loc_strings, TRUE},
        {weapon_flags, TRUE},
        {container_flags, FALSE},
        {quest_flags, TRUE},
        {liquid_flags, TRUE},
        {mprog_flags, TRUE},
        {rprog_flags, TRUE},
        {oprog_flags, TRUE},

        /*
         * J.O.P.E. tables 
         */
        {newbits_table, FALSE},
        {act_table, FALSE},
        {extra_table, FALSE},
        {jflags_table, FALSE},

        /*
         * END 
         */
        {0, 0}
};



/*****************************************************************************
 Name:		is_stat( table )
 Purpose:	Returns TRUE if the table is a stat table and FALSE if flag.
 Called by:	flag_value and flag_string.
 Note:		This function is local and used only in bit.c.
 ****************************************************************************/
bool is_stat(const struct flag_type *flag_table)
{
        int       flag;

        for (flag = 0; flag_stat_table[flag].structure; flag++)
        {
                if (flag_stat_table[flag].structure == flag_table
                    && flag_stat_table[flag].stat)
                        return TRUE;
        }
        return FALSE;
}



/*
 * This function is Russ Taylor's creation.  Thanks Russ!
 * All code copyright (C) Russ Taylor, permission to use and/or distribute
 * has NOT been granted.  Use only in this OLC package has been granted.
 */
/*****************************************************************************
 Name:		flag_lookup( flag, table )
 Purpose:	Returns the value of a single, settable flag from the table.
 Called by:	flag_value and flag_string.
 Note:		This function is local and used only in bit.c.
 ****************************************************************************/
int flag_lookup(const char *name, const struct flag_type *flag_table)
{
        int       flag;

        for (flag = 0; *flag_table[flag].name; flag++)  /* OLC 1.1b */
        {
                if (!str_cmp(name, flag_table[flag].name)
                    && flag_table[flag].settable)
                        return flag_table[flag].bit;
        }

        return NO_FLAG;
}



/*****************************************************************************
 Name:		flag_value( table, flag )
 Purpose:	Returns the value of the flags entered.  Multi-flags accepted.
 Called by:	olc.c and olc_act.c.
 ****************************************************************************/
int flag_value(const struct flag_type *flag_table, char *argument)
{
        char      word[MAX_INPUT_LENGTH];
        int       bit;
        int       marked = 0;
        bool      found = FALSE;

        if (is_stat(flag_table))
        {
                one_argument(argument, word);

                if ((bit = flag_lookup(word, flag_table)) != NO_FLAG)
                        return bit;
                else
                        return NO_FLAG;
        }

        /*
         * Accept multiple flags.
         */
        for (;;)
        {
                argument = one_argument(argument, word);

                if (word[0] == '\0')
                        break;

                if ((bit = flag_lookup(word, flag_table)) != NO_FLAG)
                {
                        SET_BIT(marked, bit);
                        found = TRUE;
                }
        }

        if (found)
                return marked;
        else
                return NO_FLAG;
}



/*****************************************************************************
 Name:		flag_string( table, flags/stat )
 Purpose:	Returns string with name(s) of the flags or stat entered.
 Called by:	act_olc.c, olc.c, and olc_save.c.
 ****************************************************************************/
char     *flag_string(const struct flag_type *flag_table, int bits)
{
        static char buf[MSL];
        int       flag;

        buf[0] = '\0';

        for (flag = 0; *flag_table[flag].name; flag++)  /* OLC 1.1b */
        {
                if (!is_stat(flag_table)
                    && IS_SET(bits, flag_table[flag].bit))
                {
                        xcatf(buf, " ");
                        xcatf(buf, "%s", flag_table[flag].name);
                }
                else if (flag_table[flag].bit == bits)
                {
                        xcatf(buf, " ");
                        xcatf(buf, "%s", flag_table[flag].name);
                        break;
                }
        }
        return (buf[0] != '\0') ? buf + 1 : "none";
}


const struct flag_type weather_flags[] = {
        {"The sky before you is beautiful and #Wc#Wl#Wo#Wu#Wd#Wl#We#Ws#Ws",
         SKY_CLOUDLESS, FALSE},
        {"Clouds prohibit the sun from making an appearance", SKY_CLOUDY,
         FALSE},
        {"A slight rain is trickling down", SKY_RAINING, FALSE},
        {"The skies are lit up by nature's #Rf#Yi#Rr#Ye#Rw#Yo#Rr#Yk#Rs#n",
         SKY_LIGHTNING, FALSE},
        {"A light #Wsnow#C can be seen falling from above", SKY_SNOWING,
         FALSE},
        {"Your #Bv#Bi#Bs#Bi#Bo#Bn#C is blocked by the #Wb#Cl#Wi#Cz#Wz#C#Wa#Cr#Wd#C all around you", SKY_BLIZZARD, FALSE},
        {"The view ahead is blocked by a dense #0fog#n", SKY_FOGGY, FALSE},
        {"The sky unleashes a storm of #Ch#Ca#Ci#Cl#Cs#Ct#Co#Cn#Ce#Cs",
         SKY_HAILSTORM, FALSE},
        {"A large boom can be heard from all around", SKY_THUNDERSTORM,
         FALSE},
        {"Nature unleashes sheets of #Cice#n, making it hard to walk",
         SKY_ICESTORM, FALSE},
        {"The #Ys#Yu#Yn#C makes an appearance, beautiful and full", SKY_SUNNY,
         FALSE},
        {"Funnel clouds can be seen from all around", SKY_TORNADO, FALSE},
        {"The ground shakes as nature moves about", SKY_EARTHQUAKE, FALSE},
        {"The wind picks up, causing objects to fly around you",
         SKY_HURRICANE, FALSE},
        {"The #Ysweat#C on your face suggests it's rather hot out", SKY_HEAT,
         FALSE},
        {"The world around you begins to #Cf#Cr#Ce#Cz#Ce#n, #Cperhaps you should find shelter", SKY_FREEZING, FALSE},
        {"The mountain seems to collapse as the #Ws#Cn#Wo#Cw#C falls around you", SKY_AVALANCHE, FALSE},
        {"The fierceness of the wind almost picks you up and blows you away",
         SKY_WINDY, FALSE},
        {"", 0, 0}
};

const struct flag_type aweather_flags[] = {
        {"cloudless", SKY_CLOUDLESS, TRUE},
        {"cloudy", SKY_CLOUDY, TRUE},
        {"raining", SKY_RAINING, TRUE},
        {"lightning", SKY_LIGHTNING, TRUE},
        {"snowing", SKY_SNOWING, TRUE},
        {"blizzard", SKY_BLIZZARD, TRUE},
        {"foggy", SKY_FOGGY, TRUE},
        {"hailstorm", SKY_HAILSTORM, TRUE},
        {"icestorm", SKY_ICESTORM, TRUE},
        {"thunderstorm", SKY_THUNDERSTORM, TRUE},
        {"sunny", SKY_SUNNY, TRUE},
        {"tornado", SKY_TORNADO, TRUE},
        {"earthquake", SKY_EARTHQUAKE, TRUE},
        {"hurricane", SKY_HURRICANE, TRUE},
        {"heat", SKY_HEAT, TRUE},
        {"windy", SKY_WINDY, TRUE},
        {"freezing", SKY_FREEZING, TRUE},
        {"avalanche", SKY_AVALANCHE, TRUE},
        {"", 0, 0}
};

const struct flag_type asector_flags[] = {
        {"indoors", AREA_INDOORS, TRUE},
        {"desert", AREA_DESERT, TRUE},
        {"swamp", AREA_SWAMP, TRUE},
        {"mountain", AREA_MOUNTAIN, TRUE},
        {"underwater", AREA_UNDERWATER, TRUE},
        {"city", AREA_CITY, TRUE},
        {"cavern", AREA_CAVERN, TRUE},
        {"forest", AREA_FOREST, TRUE},
        {"hills", AREA_HILLS, TRUE},
        {"air", AREA_AIR, TRUE},
        {"", 0, 0}
};

const struct flag_type area_flags[] = {
        {"none", AREA_NONE, FALSE},
        {"changed", AREA_CHANGED, FALSE},
        {"added", AREA_ADDED, FALSE},
        {"loading", AREA_LOADING, FALSE},
        {"verbose", AREA_VERBOSE, FALSE},
        {"", 0, 0}
};

/*
 * Area bits.
 */
const struct flag_type area_bits[] = {
        {"none", AREA_BIT_NONE, TRUE},
        {"no_travel_to_mob", AREA_BIT_NO_TRAVEL, TRUE},
        {"increased_regen", AREA_BIT_IREGEN, TRUE},
        {"decreased_regen", AREA_BIT_DREGEN, TRUE},
        {"no_magic", AREA_BIT_NOMAGIC, TRUE},
        {"no_home", AREA_BIT_NOHOME, TRUE},
        {"no_edit", AREA_BIT_NOEDIT, TRUE},
        {"no_edit2", AREA_BIT_NOEDIT2, TRUE},
        {"", 0, 0}
};



const struct flag_type sex_flags[] = {
        {"male", SEX_MALE, TRUE},
        {"female", SEX_FEMALE, TRUE},
        {"neutral", SEX_NEUTRAL, TRUE},
        {"", 0, 0}
};



const struct flag_type exit_flags[] = {
        {"door", EX_ISDOOR, TRUE},
        {"closed", EX_CLOSED, TRUE},
        {"locked", EX_LOCKED, TRUE},
        {"pickproof", EX_PICKPROOF, TRUE},
        {"no_pass", EX_NOPASS, TRUE},
        {"ice_wall", EX_ICE_WALL, TRUE},
        {"fire_wall", EX_FIRE_WALL, TRUE},
        {"sword_wall", EX_SWORD_WALL, TRUE},
        {"prismatic_wall", EX_PRISMATIC_WALL, TRUE},
        {"iron_wall", EX_IRON_WALL, TRUE},
        {"mushroom_wall", EX_MUSHROOM_WALL, TRUE},
        {"caltrop_wall", EX_CALTROP_WALL, TRUE},
        {"ash_wall", EX_ASH_WALL, TRUE},
        {"warding", EX_WARDING, TRUE},
        {"", 0, 0}
};



const struct flag_type door_resets[] = {
        {"open and unlocked", 0, TRUE},
        {"closed and unlocked", 1, TRUE},
        {"closed and locked", 2, TRUE},
        {"", 0, 0}
};



const struct flag_type room_flags[] = {
        {"dark", ROOM_DARK, TRUE},
        {"no_mob", ROOM_NO_MOB, TRUE},
        {"indoors", ROOM_INDOORS, TRUE},
        {"private", ROOM_PRIVATE, TRUE},
        {"safe", ROOM_SAFE, TRUE},
        {"solitary", ROOM_SOLITARY, TRUE},
        {"no_recall", ROOM_NO_RECALL, TRUE},
        {"no_chant", ROOM_NO_CHANT, TRUE},
        {"no_otrans", ROOM_NO_OTRANS, TRUE},
        {"no_teleport", ROOM_NO_TELEPORT, TRUE},
        {"total_darkness", ROOM_TOTAL_DARKNESS, TRUE},
        {"blade_barrier", ROOM_BLADE_BARRIER, TRUE},
        {"arena", ROOM_ARENA, TRUE},
        {"flaming", ROOM_FLAMING, TRUE},
        {"silence", ROOM_SILENCE, TRUE},
        {"astral", ROOM_ASTRAL, TRUE},
        {"no_command", ROOM_ORDER, TRUE},
        {"orgy", ROOM_SEX, TRUE},
        {"bank", ROOM_BANK, TRUE},
        {"always_light", ROOM_LIGHT, TRUE},
        {"", 0, 0}
};

const struct flag_type position_flags[] = {
        {"dead", POS_DEAD, FALSE},
        {"mortal", POS_MORTAL, FALSE},
        {"incap", POS_INCAP, FALSE},
        {"stunned", POS_STUNNED, FALSE},
        {"sleeping", POS_SLEEPING, TRUE},
        {"meditating", POS_MEDITATING, FALSE},
        {"sitting", POS_SITTING, TRUE},
        {"resting", POS_RESTING, TRUE},
        {"fighting", POS_FIGHTING, FALSE},
        {"standing", POS_STANDING, TRUE},
        {"", 0, 0}
};

#define POS_DEAD		      0
#define POS_MORTAL		      1
#define POS_INCAP		      2
#define POS_STUNNED		      3
#define POS_SLEEPING		      4
#define POS_MEDITATING		      5
#define POS_SITTING		      6
#define POS_RESTING		      7
#define POS_FIGHTING                  8
#define POS_STANDING                  9

/* for position */
const struct position_type position_table[] = {
        {"dead", "dead"},
        {"mortally wounded", "mort"},
        {"incapacitated", "incap"},
        {"stunned", "stun"},
        {"sleeping", "sleep"},
        {"meditating", "medit"},
        {"sitting", "sit"},
        {"resting", "rest"},
        {"fighting", "fight"},
        {"standing", "stand"},
        {"", ""}
};

const struct flag_type sector_flags[] = {
        {"inside", SECT_INSIDE, TRUE},
        {"city", SECT_CITY, TRUE},
        {"field", SECT_FIELD, TRUE},
        {"forest", SECT_FOREST, TRUE},
        {"hills", SECT_HILLS, TRUE},
        {"mountain", SECT_MOUNTAIN, TRUE},
        {"swim", SECT_WATER_SWIM, TRUE},
        {"noswim", SECT_WATER_NOSWIM, TRUE},
        {"air", SECT_AIR, TRUE},
        {"desert", SECT_DESERT, TRUE},
        {"", 0, 0}
};

const struct flag_type mprog_flags[] = {
        {"act", TRIG_ACT, TRUE},
        {"bribe", TRIG_BRIBE, TRUE},
        {"death", TRIG_DEATH, TRUE},
        {"entry", TRIG_ENTRY, TRUE},
        {"fight", TRIG_FIGHT, TRUE},
        {"give", TRIG_GIVE, TRUE},
        {"greet", TRIG_GREET, TRUE},
        {"grall", TRIG_GRALL, TRUE},
        {"kill", TRIG_KILL, TRUE},
        {"hpcnt", TRIG_HPCNT, TRUE},
        {"random", TRIG_RANDOM, TRUE},
        {"speech", TRIG_SPEECH, TRUE},
        {"exit", TRIG_EXIT, TRUE},
        {"exall", TRIG_EXALL, TRUE},
        {"delay", TRIG_DELAY, TRUE},
        {"surr", TRIG_SURR, TRUE},
        {"regex", TRIG_REGEX, TRUE},
        {"", 0, TRUE}
};

const struct flag_type oprog_flags[] = {
        {"act", TRIG_ACT, TRUE},
        {"fight", TRIG_FIGHT, TRUE},
        {"give", TRIG_GIVE, TRUE},
        {"greet", TRIG_GRALL, TRUE},
        {"random", TRIG_RANDOM, TRUE},
        {"speech", TRIG_SPEECH, TRUE},
        {"exall", TRIG_EXALL, TRUE},
        {"delay", TRIG_DELAY, TRUE},
        {"drop", TRIG_DROP, TRUE},
        {"get", TRIG_GET, TRUE},
        {"sit", TRIG_SIT, TRUE},
        {"", 0, TRUE},
};

const struct flag_type furniture_flags[] = {
        {"stand_at", STAND_AT, TRUE},
        {"stand_on", STAND_ON, TRUE},
        {"stand_in", STAND_IN, TRUE},
        {"sit_at", SIT_AT, TRUE},
        {"sit_on", SIT_ON, TRUE},
        {"sit_in", SIT_IN, TRUE},
        {"rest_at", REST_AT, TRUE},
        {"rest_on", REST_ON, TRUE},
        {"rest_in", REST_IN, TRUE},
        {"sleep_at", SLEEP_AT, TRUE},
        {"sleep_on", SLEEP_ON, TRUE},
        {"sleep_in", SLEEP_IN, TRUE},
        {"put_at", PUT_AT, TRUE},
        {"put_on", PUT_ON, TRUE},
        {"put_in", PUT_IN, TRUE},
        {"put_inside", PUT_INSIDE, TRUE},
        {"", 0, TRUE}
};

const struct flag_type rprog_flags[] = {
        {"act", TRIG_ACT, TRUE},
        {"fight", TRIG_FIGHT, TRUE},
        {"drop", TRIG_DROP, TRUE},
        {"greet", TRIG_GRALL, TRUE},
        {"random", TRIG_RANDOM, TRUE},
        {"speech", TRIG_SPEECH, TRUE},
        {"exall", TRIG_EXALL, TRUE},
        {"delay", TRIG_DELAY, TRUE},
        {"", 0, TRUE},
};

const struct flag_type type_flags[] = {
        {"light", ITEM_LIGHT, TRUE},
        {"scroll", ITEM_SCROLL, TRUE},
        {"wand", ITEM_WAND, TRUE},
        {"staff", ITEM_STAFF, TRUE},
        {"window", ITEM_WINDOW, TRUE},
        {"weapon", ITEM_WEAPON, TRUE},
        {"treasure", ITEM_TREASURE, TRUE},
        {"grenade", ITEM_GRENADE, TRUE},
        {"furniture", ITEM_FURNITURE, TRUE},
        {"armor", ITEM_ARMOR, TRUE},
        {"potion", ITEM_POTION, TRUE},
        {"trash", ITEM_TRASH, TRUE},
        {"container", ITEM_CONTAINER, TRUE},
        {"drink-container", ITEM_DRINK_CON, TRUE},
        {"key", ITEM_KEY, TRUE},
        {"food", ITEM_FOOD, TRUE},
        {"money", ITEM_MONEY, TRUE},
        {"boat", ITEM_BOAT, TRUE},
        {"npc corpse", ITEM_CORPSE_NPC, TRUE},
        {"pc corpse", ITEM_CORPSE_PC, FALSE},
        {"fountain", ITEM_FOUNTAIN, TRUE},
        {"pill", ITEM_PILL, TRUE},
        {"portal", ITEM_PORTAL, TRUE},
        {"egg", ITEM_EGG, TRUE},
        {"voodoo", ITEM_VOODOO, TRUE},
        {"stake", ITEM_STAKE, TRUE},
        {"missile", ITEM_MISSILE, TRUE},
        {"ammo", ITEM_AMMO, TRUE},
        {"quest", ITEM_QUEST, TRUE},
        {"piece", ITEM_PIECE, TRUE},
        {"mithril", ITEM_MITHRIL, TRUE},
        {"symbol", ITEM_SYMBOL, TRUE},
        {"book", ITEM_BOOK, TRUE},
        {"page", ITEM_PAGE, TRUE},
        {"tool", ITEM_TOOL, TRUE},
        {"wall", ITEM_WALL, TRUE},
        {"copper", ITEM_COPPER, TRUE},
        {"iron", ITEM_IRON, TRUE},
        {"steel", ITEM_STEEL, TRUE},
        {"adamantite", ITEM_ADAMANTITE, TRUE},
        {"gemstone", ITEM_GEMSTONE, TRUE},
        {"hilt", ITEM_HILT, TRUE},
        {"cookingpot", ITEM_COOKINGPOT, TRUE},
        {"slotmachine", ITEM_SLOT_MACHINE, TRUE},
        {"faithtoken", ITEM_FAITHTOKEN, TRUE},
        {"holy symbol", ITEM_HOLYSYMBOL, TRUE},
        {"familiar", ITEM_FAMILIAR, TRUE},
        {"sigil", ITEM_SIGIL, TRUE},
        {"", 0, 0}
};


const struct flag_type extra_flags[] = {
        {"glow", ITEM_GLOW, TRUE},
        {"hum", ITEM_HUM, TRUE},
        {"thrown", ITEM_THROWN, TRUE},
        {"keep", ITEM_KEEP, TRUE},
        {"vanish", ITEM_VANISH, TRUE},
        {"invis", ITEM_INVIS, TRUE},
        {"magic", ITEM_MAGIC, TRUE},
        {"nodrop", ITEM_NODROP, TRUE},
        {"bless", ITEM_BLESS, TRUE},
        {"anti-good", ITEM_ANTI_GOOD, TRUE},
        {"anti-evil", ITEM_ANTI_EVIL, TRUE},
        {"anti-neutral", ITEM_ANTI_NEUTRAL, TRUE},
        {"noremove", ITEM_NOREMOVE, TRUE},
        {"inventory", ITEM_INVENTORY, TRUE},
        {"shadowplane", ITEM_SHADOWPLANE, TRUE},
        {"loyal", ITEM_LOYAL, TRUE},
        {"noclaim", ITEM_NOCLAIM, TRUE},
        {"nolocate", ITEM_NOLOCATE, TRUE},
        {"", 0, 0}
};

const struct flag_type extra_flags2[] = {
        {"no_sell", ITEM_NO_SELL, TRUE},
        {"", 0, 0}
};

const struct flag_type itema_flags[] = {
        {"shockshield", ITEMA_SHOCKSHIELD, TRUE},
        {"fireshield", ITEMA_FIRESHIELD, TRUE},
        {"iceshield", ITEMA_ICESHIELD, TRUE},
        {"acidshield", ITEMA_ACIDSHIELD, TRUE},
        {"monkchi", ITEMA_MONKCHI, TRUE},
        {"chaosshield", ITEMA_CHAOSSHIELD, TRUE},
        {"artifact", ITEMA_ARTIFACT, TRUE},
        {"regenerate", ITEMA_REGENERATE, TRUE},
        {"vision", ITEMA_VISION, TRUE},
        {"stalker", ITEMA_STALKER, TRUE},
        {"vanish", ITEMA_VANISH, TRUE},
        {"rager", ITEMA_RAGER, TRUE},
        {"talon", ITEMA_TALON, TRUE},
        {"chaoshands", ITEMA_CHAOSHANDS, TRUE},
        {"affmantis", ITEMA_AFFMANTIS, TRUE},
        {"affentropy", ITEMA_AFFENTROPY, TRUE},
        {"affeye", ITEMA_AFFEYE, TRUE},
        {"mageshield", ITEMA_MAGESHIELD, TRUE},
        {"steelshield", ITEMA_STEELSHIELD, TRUE},
        {"deflector", ITEMA_DEFLECTOR, TRUE},
        {"illusions", ITEMA_ILLUSIONS, TRUE},
        {"beast", ITEMA_BEAST, TRUE},
        {"", 0, 0}
};

const struct flag_type wear_flags[] = {
        {"take", ITEM_TAKE, TRUE},
        {"finger", ITEM_WEAR_FINGER, TRUE},
        {"neck", ITEM_WEAR_NECK, TRUE},
        {"body", ITEM_WEAR_BODY, TRUE},
        {"head", ITEM_WEAR_HEAD, TRUE},
        {"legs", ITEM_WEAR_LEGS, TRUE},
        {"feet", ITEM_WEAR_FEET, TRUE},
        {"hands", ITEM_WEAR_HANDS, TRUE},
        {"arms", ITEM_WEAR_ARMS, TRUE},
        {"shield", ITEM_WEAR_SHIELD, TRUE},
        {"about", ITEM_WEAR_ABOUT, TRUE},
        {"waist", ITEM_WEAR_WAIST, TRUE},
        {"wrist", ITEM_WEAR_WRIST, TRUE},
        {"wield", ITEM_WIELD, TRUE},
        {"hold", ITEM_HOLD, TRUE},
        {"face", ITEM_WEAR_FACE, TRUE},
        {"floating", ITEM_WEAR_FLOAT, TRUE},
        {"special", ITEM_WEAR_SPECIAL, TRUE},
        {"medal", ITEM_WEAR_MEDAL, TRUE},
        {"bodyart", ITEM_WEAR_BODYART, TRUE},
        {"accompanied", ITEM_WEAR_FAMILIAR, TRUE},
        {"", 0, 0}
};



const struct flag_type act_flags[] = {
        {"npc", ACT_IS_NPC, FALSE},
        {"sentinel", ACT_SENTINEL, TRUE},
        {"scavenger", ACT_SCAVENGER, TRUE},
        {"aggressive", ACT_AGGRESSIVE, TRUE},
        {"stay_area", ACT_STAY_AREA, TRUE},
        {"wimpy", ACT_WIMPY, TRUE},
        {"train", ACT_TRAIN, TRUE},
        {"practice", ACT_PRACTICE, TRUE},
        {"mount", ACT_MOUNT, TRUE},
        {"no_parts", ACT_NOPARTS, TRUE},
        {"no_exp", ACT_NOEXP, TRUE},
        {"no_autokill", ACT_NOAUTOKILL, TRUE},
        {"notravel", ACT_NOTRAVEL, TRUE},
        {"nosummon", ACT_NOSUMMON, TRUE},
        {"nodamage", ACT_NODAMAGE, TRUE},
        {"dealer", ACT_DEALER, TRUE},
        {"intelligent", ACT_INTELLIGENT, TRUE},
        {"", 0, 0}
};



const struct flag_type affect_flags[] = {
        {"blind", AFF_BLIND, TRUE},
        {"invisible", AFF_INVISIBLE, TRUE},
        {"detect-evil", AFF_DETECT_EVIL, TRUE},
        {"detect-invis", AFF_DETECT_INVIS, TRUE},
        {"detect-magic", AFF_DETECT_MAGIC, TRUE},
        {"detect-hidden", AFF_DETECT_HIDDEN, TRUE},
        {"shadowplane", AFF_SHADOWPLANE, TRUE},
        {"sanctuary", AFF_SANCTUARY, TRUE},
        {"faerie-fire", AFF_FAERIE_FIRE, TRUE},
        {"infrared", AFF_INFRARED, TRUE},
        {"curse", AFF_CURSE, TRUE},
        {"flaming", AFF_FLAMING, TRUE},
        {"poison", AFF_POISON, TRUE},
        {"protect", AFF_PROTECT, TRUE},
        {"ethereal", AFF_ETHEREAL, TRUE},
        {"sneak", AFF_SNEAK, TRUE},
        {"hide", AFF_HIDE, TRUE},
        {"sleep", AFF_SLEEP, TRUE},
        {"charm", AFF_CHARM, TRUE},
        {"flying", AFF_FLYING, TRUE},
        {"pass-door", AFF_PASS_DOOR, TRUE},
        {"polymorph", AFF_POLYMORPH, FALSE},
        {"shadowsight", AFF_SHADOWSIGHT, TRUE},
        {"web", AFF_WEBBED, TRUE},
//    { "tendrils",     AFF_TENDRILS,       TRUE    },
        {"drowfire", AFF_DROWFIRE, TRUE},
        {"zuloform", AFF_ZULOFORM, FALSE},
        {"peace", AFF_PEACE, TRUE},
        {"infirmity", AFF_INFIRMITY, TRUE},
        {"shift", AFF_SHIFT, FALSE},
        {"", 0, 0}
};

const struct flag_type affect2_flags[] = {
        {"faith armor", AFF2_FARMOR, TRUE},
        {"thorns", AFF2_THORNS, FALSE},
        {"", 0, 0}
};

const struct flag_type quest_flags[] = {
        {"relic", QUEST_RELIC, TRUE},
        {"artifact", QUEST_ARTIFACT, TRUE},
        {"spellproof", QUEST_SPELLPROOF, TRUE},
        {"holysymbol", QUEST_HOLYSYMBOL, TRUE},
        {"", 0, 0}
};


/*
 * Used when adding an affect to tell where it goes.
 * See addaffect and delaffect in act_olc.c
 */
const struct flag_type apply_flags[] = {
        {"none", APPLY_NONE, TRUE},
        {"strength", APPLY_STR, TRUE},
        {"dexterity", APPLY_DEX, TRUE},
        {"intelligence", APPLY_INT, TRUE},
        {"wisdom", APPLY_WIS, TRUE},
        {"constitution", APPLY_CON, TRUE},
        {"sex", APPLY_SEX, TRUE},
        {"class", APPLY_CLASS, TRUE},
        {"level", APPLY_LEVEL, TRUE},
        {"age", APPLY_AGE, TRUE},
        {"height", APPLY_HEIGHT, TRUE},
        {"weight", APPLY_WEIGHT, TRUE},
        {"mana", APPLY_MANA, TRUE},
        {"hp", APPLY_HIT, TRUE},
        {"move", APPLY_MOVE, TRUE},
        {"gold", APPLY_GOLD, TRUE},
        {"experience", APPLY_EXP, TRUE},
        {"ac", APPLY_AC, TRUE},
        {"hitroll", APPLY_HITROLL, TRUE},
        {"damroll", APPLY_DAMROLL, TRUE},
        {"saving-para", APPLY_SAVING_PARA, TRUE},
        {"saving-rod", APPLY_SAVING_ROD, TRUE},
        {"saving-petri", APPLY_SAVING_PETRI, TRUE},
        {"saving-breath", APPLY_SAVING_BREATH, TRUE},
        {"saving-spell", APPLY_SAVING_SPELL, TRUE},
        {"polymorph", APPLY_POLY, TRUE},
        {"", 0, 0}
};



/*
 * What is seen.
 */
const struct flag_type wear_loc_strings[] = {
        {"in the inventory", WEAR_NONE, TRUE},
        {"as a light", WEAR_LIGHT, TRUE},
        {"on the left finger", WEAR_FINGER_L, TRUE},
        {"on the right finger", WEAR_FINGER_R, TRUE},
        {"around the neck (1)", WEAR_NECK_1, TRUE},
        {"around the neck (2)", WEAR_NECK_2, TRUE},
        {"on the body", WEAR_BODY, TRUE},
        {"over the head", WEAR_HEAD, TRUE},
        {"on the legs", WEAR_LEGS, TRUE},
        {"on the feet", WEAR_FEET, TRUE},
        {"on the hands", WEAR_HANDS, TRUE},
        {"on the arms", WEAR_ARMS, TRUE},
        {"about the body", WEAR_ABOUT, TRUE},
        {"around the waist", WEAR_WAIST, TRUE},
        {"on the left wrist", WEAR_WRIST_L, TRUE},
        {"on the right wrist", WEAR_WRIST_R, TRUE},
        {"on the face", WEAR_FACE, TRUE},
        {"as a shield", WEAR_SHIELD, TRUE},
        {"first hand", WEAR_WIELD, TRUE},
        {"second hand", WEAR_HOLD, TRUE},
        {"third hand", WEAR_THIRD, TRUE},
        {"fourth hand", WEAR_FOURTH, TRUE},
        {"left scabbard", WEAR_SCABBARD_L, TRUE},
        {"right scabbard", WEAR_SCABBARD_R, TRUE},
        {"special", WEAR_SPECIAL, TRUE},
        {"floating", WEAR_FLOAT, TRUE},
        {"medal", WEAR_MEDAL, TRUE},
        {"bodyart", WEAR_BODYART, TRUE},
        {"accompanied by", WEAR_FAMILIAR, TRUE},
        {"", 0}
};


/*
 * What is typed.
 * Neck2 should not be settable for loaded mobiles.
 */
const struct flag_type wear_loc_flags[] = {
        {"none", WEAR_NONE, TRUE},
        {"light", WEAR_LIGHT, TRUE},
        {"lfinger", WEAR_FINGER_L, TRUE},
        {"rfinger", WEAR_FINGER_R, TRUE},
        {"neck", WEAR_NECK_1, TRUE},
        {"body", WEAR_BODY, TRUE},
        {"head", WEAR_HEAD, TRUE},
        {"legs", WEAR_LEGS, TRUE},
        {"feet", WEAR_FEET, TRUE},
        {"hands", WEAR_HANDS, TRUE},
        {"arms", WEAR_ARMS, TRUE},
        {"shield", WEAR_SHIELD, TRUE},
        {"about", WEAR_ABOUT, TRUE},
        {"waist", WEAR_WAIST, TRUE},
        {"lwrist", WEAR_WRIST_L, TRUE},
        {"rwrist", WEAR_WRIST_R, TRUE},
        {"wielded", WEAR_WIELD, TRUE},
        {"hold", WEAR_HOLD, TRUE},
        {"face", WEAR_FACE, TRUE},
        {"special", WEAR_SPECIAL, TRUE},
        {"floating", WEAR_FLOAT, TRUE},
        {"bodyart", WEAR_BODYART, TRUE},
        {"familiar", WEAR_FAMILIAR, TRUE},
        {"", 0, 0}
};



const struct flag_type weapon_flags[] = {
        {"hit", 0, TRUE},
        {"slice", 1, TRUE},
        {"stab", 2, TRUE},
        {"slash", 3, TRUE},
        {"whip", 4, TRUE},
        {"claw", 5, TRUE},
        {"blast", 6, TRUE},
        {"pound", 7, TRUE},
        {"crush", 8, TRUE},
        {"grep", 9, TRUE},
        {"bite", 10, TRUE},
        {"pierce", 11, TRUE},
        {"suck", 12, TRUE},
        {"", 0, 0}
};


const struct flag_type container_flags[] = {
        {"closeable", 1, TRUE},
        {"pickproof", 2, TRUE},
        {"closed", 4, TRUE},
        {"locked", 8, TRUE},
        {"", 0, 0}
};



const struct flag_type liquid_flags[] = {
        {"water", 0, TRUE},
        {"beer", 1, TRUE},
        {"wine", 2, TRUE},
        {"ale", 3, TRUE},
        {"dark-ale", 4, TRUE},
        {"whisky", 5, TRUE},
        {"lemonade", 6, TRUE},
        {"firebreather", 7, TRUE},
        {"local-specialty", 8, TRUE},
        {"slime-mold-juice", 9, TRUE},
        {"milk", 10, TRUE},
        {"tea", 11, TRUE},
        {"coffee", 12, TRUE},
        {"blood", 13, TRUE},
        {"salt-water", 14, TRUE},
        {"cola", 15, TRUE},
        {"cum", 16, TRUE},
        {"cunt-juice", 17, TRUE},
        {"urine", 18, TRUE},
        {"", 0, 0}
};
