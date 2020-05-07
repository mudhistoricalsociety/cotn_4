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
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "merc.h"
#include "changes.h"
#include "recycle.h"
#include "music.h"

#if !defined(macintosh)
extern int _filbuf args((FILE *));
#endif

/*
 * Globals.
 */
bool      nodamcap = FALSE;
HELP_DATA *help_first;
HELP_DATA *help_last;
DUMMY_ARG *dummy_free;
DUMMY_ARG *dummy_list;
CHAR_DATA *char_free;
EXTRA_DESCR_DATA *extra_descr_free;
NOTE_DATA *note_free;
OBJ_DATA *obj_free;
PC_DATA  *pcdata_free;
BAN_DATA *ban_free;
BAN_DATA *newbieban_free;
MOB_TRIGGER *trigger_free;

char      bug_buf[2 * MAX_INPUT_LENGTH];
CHAR_DATA *char_list;
char     *help_greetinga;
char     *help_greetingb;
char      log_buf[2 * MAX_INPUT_LENGTH];
KILL_DATA kill_table[MAX_LEVEL];
OBJ_DATA *object_list;
TIME_INFO_DATA time_info;
WEATHER_DATA weather_info;
sh_int    gsn_stuntubes;
AUCTION_DATA auction_info;
PROG_CODE *mprog_list;
PROG_CODE *oprog_list;
PROG_CODE *rprog_list;

// Cotn 
sh_int    gsn_retribution;
sh_int    gsn_holyaura;
sh_int	  gsn_stench;
sh_int    gsn_zomplague;
sh_int	  gsn_shin;
sh_int	  gsn_shinfire;
sh_int	  gsn_shinholy;
sh_int	  gsn_shinshadow;
sh_int	  gsn_shinwater;
sh_int	  gsn_shinearth;
sh_int	  gsn_shinwind;
sh_int	  gsn_requiem;

sh_int    gsn_thrownpie;
sh_int    gsn_bash;
sh_int    gsn_tattoo;
sh_int    gsn_smack;
sh_int    gsn_thwack;
sh_int    gsn_mocha;
sh_int    gsn_plasma;
sh_int    gsn_telekinetic;
sh_int    gsn_potato;
sh_int    gsn_laser;
sh_int    gsn_backstab;
sh_int    gsn_shred;
sh_int    gsn_quills;
sh_int    gsn_stinger;
sh_int    gsn_bladespin;
sh_int    gsn_faith;
sh_int    gsn_thorns;
sh_int    gsn_fiery;
sh_int    gsn_hooves;
sh_int    gsn_fireball;
sh_int    gsn_tentacle;
sh_int    gsn_lightning;
sh_int    gsn_supreme;
sh_int    gsn_deathaura;
sh_int    gsn_wrathofgod;
sh_int    gsn_claws;
sh_int    gsn_heavenlyaura;
sh_int    gsn_mageshield;
sh_int    gsn_breath;
sh_int    gsn_darktendrils;
sh_int    gsn_cheapshot;
sh_int    gsn_spit;
sh_int    gsn_venomtong;
sh_int    gsn_spiketail;
sh_int    gsn_lunarflare;
sh_int    gsn_magma;
sh_int    gsn_hellfire;
sh_int    gsn_shards;
sh_int    gsn_hide;
sh_int    gsn_peek;
sh_int    gsn_pick_lock;
sh_int    gsn_sneak;
sh_int    gsn_steal;
sh_int    gsn_spiderform;
sh_int    gsn_garotte;
sh_int    gsn_disarm;
sh_int    gsn_tendrils;
sh_int    gsn_berserk;
sh_int    gsn_punch;
sh_int    gsn_headbutt;
sh_int    gsn_spiket;
sh_int    gsn_venomt;
sh_int    gsn_shiroken;
sh_int    gsn_inferno;
sh_int    gsn_blinky;
sh_int    gsn_fangs;
sh_int    gsn_buffet;
sh_int    gsn_sweep;
sh_int    gsn_knee;
sh_int    gsn_lightningslash;
sh_int    gsn_rfangs;
sh_int    gsn_thrustkick;
sh_int    gsn_spinkick;
sh_int    gsn_backfist;
sh_int    gsn_elbow;
sh_int    gsn_shinkick;
sh_int    gsn_palmstrike;
sh_int    gsn_lightningkick;
sh_int    gsn_tornadokick;
sh_int    gsn_jumpkick;
sh_int    gsn_monksweep;
sh_int    gsn_circle;
sh_int    gsn_booming;
sh_int    gsn_chillhand;
sh_int    gsn_kick;
sh_int    gsn_hurl;
sh_int    gsn_rescue;
sh_int    gsn_track;
sh_int    gsn_polymorph;
sh_int    gsn_web;
sh_int    gsn_drowfire;
sh_int    gsn_infirmity;
sh_int    gsn_spew;
sh_int    gsn_blindness;
sh_int    gsn_charm_person;
sh_int    gsn_curse;
sh_int    gsn_invis;
sh_int    gsn_mass_invis;
sh_int    gsn_poison;
sh_int    gsn_sleep;
sh_int    gsn_multiplearms;

sh_int    gsn_darkness;
sh_int    gsn_paradox;
sh_int    gsn_nova;

/* shadow attacks (CLASS_REMOVE) */
sh_int    gsn_knifespin;
sh_int    gsn_wakasashislice;
sh_int    gsn_caltrops;
sh_int    gsn_soulreaper;
sh_int    gsn_moonstrike;
sh_int    gsn_shadowthrust;
sh_int    gsn_gutcutter;
sh_int    gsn_dirtthrow;
sh_int    gsn_soulseeker;

/* cyberdemon attacks (CLASS_REMOVE) */
sh_int    gsn_steam;
sh_int    gsn_machinegun;
sh_int    gsn_rocket;
sh_int    gsn_pincers;

/*
 * Locals.
 */
MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
OBJ_INDEX_DATA *obj_index_hash[MAX_KEY_HASH];
ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];
char     *string_hash[MAX_KEY_HASH];

ROOM_INDEX_DATA *	update_room_list = NULL;


AREA_DATA *area_first;
AREA_DATA *area_last;
ROOM_INDEX_DATA *room_list;
HELP_DATA *first_help;
HELP_DATA *last_help;
char     *string_space;
char     *top_string;
char      str_empty[1] = { '\0' };

int       top_affect;
int       top_area;
int       top_rt;
int       top_ed;
int       top_exit;
int       top_help;
int       top_mob_index;
int       top_obj_index;
int       top_reset;
int       top_room;
int       top_vnum_room;    /* OLC */
int       top_vnum_mob; /* OLC */
int       top_vnum_obj; /* OLC */
int       top_mprog_index;  /* OLC */
int       top_oprog_index;
int       top_rprog_index;

bool      CHAOS = FALSE;
bool      VISOR = FALSE;
bool      DARKNESS = FALSE;
bool      SPEED = FALSE;
bool      BRACELET = FALSE;
bool      TORC = FALSE;
bool      ARMOUR = FALSE;
bool      CLAWS = FALSE;
bool      ITEMAFFMANTIS = FALSE;
bool      ITEMAFFENTROPY = FALSE;

/*
 * Memory management.
 * Increase MAX_STRING if you have too.
 * Tune the others only if you understand what you're doing.
 */
#define			MAX_STRING	2099200
#define			MAX_PERM_BLOCK	131072
#define			MAX_MEM_LIST	11

void     *rgFreeList[MAX_MEM_LIST];
const int rgSizeList[MAX_MEM_LIST] = {
        16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384, 32768 - 64
};

int       nAllocString;
int       sAllocString;
int       nAllocPerm;
int       sAllocPerm = 0;

/*
 * Semi-locals.
 */
bool      fBootDb;
FILE     *fpArea;
char      strArea[MAX_INPUT_LENGTH];

/*
 * Local booting procedures.
 */
void init_mm args((void));
void load_helps args((FILE * fp));
void load_mobiles args((FILE * fp));
void load_objects args((FILE * fp));
void load_resets args((FILE * fp));
void load_specials args((FILE * fp));
void load_area args((FILE * fp));
void load_rooms args((FILE * fp));
void load_projects args((void));
void fix_exits args((void));
//void load_artis args((void));
void randomize_object args((int vnum));
void load_wizlist args((void));

//olc  stuff
void load_mobprogs args((FILE * fp));
void load_objprogs args((FILE * fp));
void load_roomprogs args((FILE * fp));
void fix_mobprogs args((void));
void fix_objprogs args((void));
void fix_roomprogs args((void));

/*
 * Big mama top level function.
 */
void boot_db(bool fCopyOver)
{
        char      buf[MAX_STRING_LENGTH];

        /*
         * Init some data space stuff.
         */
        {
                log_string(LOG_GAME, "Init data space.");

                if ((string_space = calloc(1, MAX_STRING)) == NULL)
                {
                        bug("Boot_db: can't alloc %d string space.",
                            MAX_STRING);
                        exit(1);
                }
                top_string = string_space;
                fBootDb = TRUE;
        }

        /*
         * Init random number generator.
         */
        {
                init_mm();
        }
        auction_info.item = NULL;
        auction_info.owner = NULL;
        auction_info.high_bidder = NULL;
        auction_info.current_bid = 0;
        auction_info.status = 0;
        auction_info.bones_held = 0;
        /*
         * Set time and weather.
         */
        {
                long      lhour, lday, lmonth;


                log_string(LOG_GAME, "Setting time and weather.");

                lhour = (current_time - 650336715)
                        / (PULSE_TICK / PULSE_PER_SECOND);
                time_info.hour = lhour % 24;
                lday = lhour / 24;
                time_info.day = lday % 35;
                lmonth = lday / 35;
                time_info.month = lmonth % 17;
                time_info.year = lmonth / 17;

                if (time_info.hour < 5)
                        weather_info.sunlight = SUN_DARK;
                else if (time_info.hour < 6)
                        weather_info.sunlight = SUN_RISE;
                else if (time_info.hour < 19)
                        weather_info.sunlight = SUN_LIGHT;
                else if (time_info.hour < 20)
                        weather_info.sunlight = SUN_SET;
                else
                        weather_info.sunlight = SUN_DARK;
                weather_info.change = 0;
                //weather_info.mmhg = 960;
                //if ( time_info.month >= 7 && time_info.month <=12 )
                //    weather_info.mmhg += number_range( 1, 50 );
                //else
                //    weather_info.mmhg += number_range( 1, 80 );

                //if (weather_info.mmhg <= 980)
                //weather_info.sky = SKY_LIGHTNING;
                //else if (weather_info.mmhg <= 1000)
                //weather_info.sky = SKY_RAINING;
                //else if (weather_info.mmhg <= 1020)
                //weather_info.sky = SKY_CLOUDY;
                //else
                //weather_info.sky = SKY_CLOUDLESS;

        }

        /*
         * Assign gsn's for skills which have them.
         */
        {
                int       sn;

                log_string(LOG_GAME, "Assigning GSN's.");
                for (sn = 0; sn < MAX_SKILL; sn++)
                {
                        if (skill_table[sn].pgsn != NULL)
                                *skill_table[sn].pgsn = sn;
                }
        }

        /*
         * Read in all the area files.
         */
        {
                FILE     *fpList;

                log_string(LOG_GAME, "Reading Area List.");
                if ((fpList = fopen(AREA_LIST, "r")) == NULL)
                {
                        perror(AREA_LIST);
                        exit(1);
                }

                for (;;)
                {
                        strcpy(strArea, fread_word(fpList));
                        if (strArea[0] == '$')
                                break;

                        if (strArea[0] == '-')
                        {
                                fpArea = stdin;
                        }
                        else
                        {
                                xprintf(buf, "loading %s", strArea);
                                log_string(LOG_GAME, buf);
                                if ((fpArea = fopen(strArea, "r")) == NULL)
                                {
                                        perror(strArea);
                                        exit(1);
                                }
                        }

                        for (;;)
                        {
                                char     *word;

                                if (fread_letter(fpArea) != '#')
                                {
                                        bug("Boot_db: # not found.", 0);
                                        exit(1);
                                }

                                word = fread_word(fpArea);

                                if (word[0] == '$')
                                        break;
                                else if (!str_cmp(word, "HELPS"))
                                        load_helps(fpArea);
                                else if (!str_cmp(word, "MOBILES"))
                                        load_mobiles(fpArea);
                                else if (!str_cmp(word, "OBJECTS"))
                                        load_objects(fpArea);
                                else if (!str_cmp(word, "RESETS"))
                                        load_resets(fpArea);
                                else if (!str_cmp(word, "ROOMS"))
                                        load_rooms(fpArea);
                                else if (!str_cmp(word, "SPECIALS"))
                                        load_specials(fpArea);
                                else if (!str_cmp(word, "MOBPROGS"))
                                        load_mobprogs(fpArea);
                                else if (!str_cmp(word, "OBJPROGS"))
                                        load_objprogs(fpArea);
                                else if (!str_cmp(word, "ROOMPROGS"))
                                        load_roomprogs(fpArea);
                                else if (!str_cmp(word, "SHOPS"))
                                        break;
                                else if (!str_cmp(word, "AREADATA"))
                                        load_area(fpArea);
                                else if (!str_cmp(word, "ROOMDATA"))
                                        load_rooms(fpArea);

                                else
                                {
                                        bug("Boot_db: bad section name.", 0);
                                        exit(1);
                                }
                        }

                        if (fpArea != stdin)
                                fclose(fpArea);
                        fpArea = NULL;
                }
                fclose(fpList);
        }

        /*
         * Load stuff
         */
        {
                log_string(LOG_GAME, "Fixing exits.");
                fix_exits();
                fBootDb = FALSE;
                area_update();
                log_string(LOG_GAME, "Loading Bans");
                load_bans();
                load_newbiebans();
                log_string(LOG_GAME, "Loading Topboard");
                load_topboard();
                log_string(LOG_GAME, "Loading Religions");
                load_religions();
                log_string(LOG_GAME, "Loading Leaderboard");
                load_leaderboard();
                log_string(LOG_GAME, "Loading Kingdoms");
                load_kingdoms();
                log_string(LOG_GAME, "Loading Religions");
//                load_religions();
                log_string(LOG_GAME, "Loading Wars");
                load_war();
                log_string(LOG_GAME, "Loading Boards");
                load_boards();
                log_string(LOG_GAME, "Loading Notes");
                save_notes();
                log_string(LOG_GAME, "Loading Disabled");
                load_disabled();
                log_string(LOG_GAME, "Loading Mudinfo");
                load_mudinfo();
                log_string(LOG_GAME, "Loading CCenter");
                load_ccenter();
                log_string(LOG_GAME, "Loading Projects");
                load_projects();
                log_string(LOG_GAME, "Loading Coreinfo");
                load_coreinfo();
                log_string(LOG_GAME, "Loading Changes");
                load_changes();
                log_string(LOG_GAME, "Loading Wizlist.");
                load_wizlist();
                log_string(LOG_GAME, "Loading Classes.");
                load_classes();
                init_tell_storage();
                log_string(LOG_GAME, "Loading Socials");
                load_social_table();
                log_string(LOG_GAME, "Fixing Mob/Obj/room progs");
                fix_mobprogs();
                fix_objprogs();
                fix_roomprogs();
                init_channel_storage();
        }

        if (fCopyOver)
                copyover_recover();

        return;
}

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )                \
                if ( !str_cmp( word, literal ) )    \
                {                                   \
                    field  = value;                 \
                    fMatch = TRUE;                  \
                    break;                          \
                }

#define SKEY( string, field )                       \
                if ( !str_cmp( word, string ) )     \
                {                                   \
	free_string( field );           \
                    field = fread_string( fp );     \
                    fMatch = TRUE;                  \
                    break;                          \
                }


/* OLC
 * Snarf an 'area' header line.   Check this format.  MUCH better.  Add fields
 * too.             
dv *
 * #AREAFILE
 * Name   { All } Locke    Newbie School~
 * Repop  A teacher pops in the room and says, 'Repop coming!'~
 * End
 */
void load_area(FILE * fp)
{
        AREA_DATA *pArea;
        char     *word;
        bool      fMatch;

        pArea = alloc_perm(sizeof(*pArea));
        pArea->affects = NULL;
        pArea->age = 6;
        pArea->nplayer = 0;
        pArea->filename = str_dup(strArea);
        pArea->vnum = top_area;
        pArea->name = str_dup("New Area");
        pArea->builders = str_dup("");
        pArea->music = str_dup("");
        pArea->areabits = 0;
        pArea->security = 3;
        pArea->lvnum = 0;
        pArea->uvnum = 0;
        pArea->cvnum = 0;
        pArea->a_weather = 1024;    //Defaults to Sunny
        pArea->sector = 32; //Defaults to City
        pArea->sky = 0;
        pArea->area_flags = 0;

        for (;;)
        {
                word = feof(fp) ? "End" : fread_word(fp);
                fMatch = FALSE;

                switch (UPPER(word[0]))
                {
                case 'N':
                        SKEY("Name", pArea->name);
                        break;
                case 'A':
                        KEY("Areabits", pArea->areabits, fread_number(fp));
                        break;
                case 'S':
                        KEY("Security", pArea->security, fread_number(fp));
                        KEY("SecType", pArea->sector, fread_number(fp));
                        break;
                case 'C':
                        KEY("Cvnum", pArea->cvnum, fread_number(fp));
                        break;
                case 'V':
                        if (!str_cmp(word, "VNUMs"))
                        {
                                pArea->lvnum = fread_number(fp);
                                pArea->uvnum = fread_number(fp);
                        }
                        break;
                case 'W':
                        KEY("WeatAl", pArea->a_weather, fread_number(fp));
                        break;
                case 'E':
                        if (!str_cmp(word, "End"))
                        {
                                fMatch = TRUE;
                                if (area_first == NULL)
                                        area_first = pArea;
                                if (area_last != NULL)
                                        area_last->next = pArea;
                                area_last = pArea;
                                pArea->next = NULL;
                                top_area++;
                                return;
                        }
                        break;
                case 'B':
                        SKEY("Builders", pArea->builders);
                        break;
                case 'M':
                        SKEY("Music", pArea->music);
                        break;
                case 'R':
                        if (!str_cmp(word, "Recall"))
                        {
                                fread_number(fp);
                                break;
                        }
                        break;
                }
        }
        log_string(LOG_GAME, pArea->name);
}

/*
 * Sets vnum range for area using OLC protection features.
 */
void assign_area_vnum(int vnum)
{
        if (area_last->lvnum == 0 || area_last->uvnum == 0)
                area_last->lvnum = area_last->uvnum = vnum;
        if (vnum != URANGE(area_last->lvnum, vnum, area_last->uvnum))
        {
                if (vnum < area_last->lvnum)
                {
                        area_last->lvnum = vnum;
                }
                else
                {
                        area_last->uvnum = vnum;
                }
        }
        return;
}

/*
 * Returns an uppercase string.
 */
char     *strupper(const char *str)
{
        static char strup[MAX_STRING_LENGTH];
        int       i;

        for (i = 0; str[i] != '\0'; i++)
                strup[i] = UPPER(str[i]);
        strup[i] = '\0';
        return strup;
}

/*
 * Adds a help page to the list if it is not a duplicate of an existing page.
 * Page is insert-sorted by keyword.			-Thoric
 * (The reason for sorting is to keep do_hlist looking nice)
 */
void add_help(HELP_DATA * pHelp)
{
        HELP_DATA *tHelp;

        /*
         * char buf[MAX_STRING_LENGTH];
         */
        int       match;

        for (tHelp = first_help; tHelp; tHelp = tHelp->next)
        {
                if ((match =
                     strcmp(pHelp->keyword[0] ==
                            '\'' ? pHelp->keyword + 1 : pHelp->keyword,
                            tHelp->keyword[0] ==
                            '\'' ? tHelp->keyword + 1 : tHelp->keyword)) < 0
                    || (match == 0 && pHelp->level > tHelp->level))
                {
                        if (!tHelp->prev)
                                first_help = pHelp;
                        else
                                tHelp->prev->next = pHelp;
                        pHelp->prev = tHelp->prev;
                        pHelp->next = tHelp;
                        tHelp->prev = pHelp;
                        break;
                }
        }
        if (!tHelp)
                LINK(pHelp, first_help, last_help, next, prev);

        top_help++;
}


/*
 * Load a help section.
 */
void load_helps(FILE * fp)
{
        HELP_DATA *pHelp;

        for (;;)
        {
               pHelp = calloc( 1, sizeof(  HELP_DATA ) );
                pHelp->level = fread_number(fp);
                pHelp->keyword = fread_string(fp);
                if (pHelp->keyword[0] == '$')
                        break;
                pHelp->text = fread_string(fp);
                if (pHelp->keyword[0] == '\0')
                {
                        STRFREE(pHelp->text);
                        STRFREE(pHelp->keyword);
                        DISPOSE(pHelp);
                        continue;
                }

                if (!str_cmp(pHelp->keyword, "greeting1"))
                        help_greetinga = pHelp->text;
                if (!str_cmp(pHelp->keyword, "greeting2"))
                        help_greetingb = pHelp->text;
                add_help(pHelp);
        }
        return;
}


/*
 * Snarf a mob section.
 */
void load_mobiles(FILE * fp)
{
        MOB_INDEX_DATA *pMobIndex;


        if (!area_last) /* OLC */
        {
                bug("Load_mobiles: no #AREA seen yet.", 0);
                exit(1);
        }

        for (;;)
        {
                int       vnum;
                char      letter;
                int       iHash;

                letter = fread_letter(fp);
                if (letter != '#')
                {
                        bug("Load_mobiles: # not found.", 0);
                        exit(1);
                }

                vnum = fread_number(fp);
                if (vnum == 0)
                        break;

                fBootDb = FALSE;
                if (get_mob_index(vnum) != NULL)
                {
                        bug("Load_mobiles: vnum %d duplicated.", vnum);
                        exit(1);
                }
                fBootDb = TRUE;

                pMobIndex = alloc_perm(sizeof(*pMobIndex));
                pMobIndex->vnum = vnum;
                pMobIndex->area = area_last;    /* OLC */
                pMobIndex->player_name = fread_string(fp);
                pMobIndex->short_descr = fread_string(fp);
                pMobIndex->long_descr = fread_string(fp);
                pMobIndex->description = fread_string(fp);

                pMobIndex->long_descr[0] = UPPER(pMobIndex->long_descr[0]);
                pMobIndex->description[0] = UPPER(pMobIndex->description[0]);
                pMobIndex->act = fread_number(fp) | ACT_IS_NPC;
                pMobIndex->affected_by = fread_number(fp);
                pMobIndex->itemaffect = 0;
                pMobIndex->alignment = fread_number(fp);
                pMobIndex->level = number_fuzzy(fread_number(fp));
                pMobIndex->toughness = fread_number(fp);
                pMobIndex->extra_attack = fread_number(fp);
                pMobIndex->dam_modifier = fread_number(fp);
                pMobIndex->sex = fread_number(fp);
                pMobIndex->natural_attack = fread_number(fp);
                pMobIndex->bones = fread_number(fp);
                pMobIndex->default_pos = fread_number(fp);
                //letter            = fread_letter( fp );

                if (pMobIndex <= 0)
                        pMobIndex->natural_attack = number_range(1, 12);

                if (pMobIndex->default_pos < POS_SLEEPING)
                        pMobIndex->default_pos = POS_STANDING;

                //keep mobs from holding too many bones    
                pMobIndex->bones = 0;

                for (;;)
                {
                        letter = fread_letter(fp);

                        if (letter == 'M')
                        {
                                PROG_LIST *pMprog;
                                char     *word;
                                int       trigger = 0;

                                pMprog = alloc_perm(sizeof(*pMprog));
                                word = fread_word(fp);
                                if (!
                                    (trigger =
                                     flag_lookup(word, mprog_flags)))
                                        if ((trigger =
                                             flag_lookup(word,
                                                         mprog_flags)) ==
                                            NO_FLAG)
                                        {
                                                bug("MOBprogs: invalid trigger.", 0);
                                                exit(1);
                                        }
                                SET_BIT(pMobIndex->mprog_flags, trigger);
                                pMprog->trig_type = trigger;
                                pMprog->vnum = fread_number(fp);
                                pMprog->trig_phrase = fread_string(fp);
                                pMprog->next = pMobIndex->mprogs;
                                pMobIndex->mprogs = pMprog;
                        }
                        else if (letter == 'P')
                        {
                                MOB_TRIGGER *mProg;

                                mProg = alloc_perm(sizeof(*mProg));
                                mProg->type = fread_number(fp);
                                mProg->vnum = fread_number(fp);
                                mProg->keywords = fread_string(fp);
                                mProg->roomOutput = fread_string(fp);
                                mProg->chOutput = fread_string(fp);

                                mProg->next = pMobIndex->triggers;
                                pMobIndex->triggers = mProg;

                                letter = fread_letter(fp);
                        }
                        else
                        {
                                ungetc(letter, fp);
                                break;
                        }
                }

                iHash = vnum % MAX_KEY_HASH;
                pMobIndex->next = mob_index_hash[iHash];
                mob_index_hash[iHash] = pMobIndex;
                top_mob_index++;
                top_vnum_mob = top_vnum_mob < vnum ? vnum : top_vnum_mob;   /* OLC */
                assign_area_vnum(vnum); /* OLC */

                kill_table[URANGE(0, pMobIndex->level, MAX_LEVEL - 1)].
                        number++;
        }

        return;
}

/*
 * Load mobprogs section
 */
void load_mobprogs(FILE * fp)
{
        PROG_CODE *pMprog;

        if (area_last == NULL)
        {
                bug("Load_mobprogs: no #AREA seen yet.", 0);
                exit(1);
        }

        for (;;)
        {
                int       vnum;
                char      letter;

                letter = fread_letter(fp);
                if (letter != '#')
                {
                        bug("Load_mobprogs: # not found.", 0);
                        exit(1);
                }

                vnum = fread_number(fp);
                if (vnum == 0)
                        break;

                fBootDb = FALSE;
                if (get_prog_index(vnum, PRG_MPROG) != NULL)
                {
                        bug("Load_mobprogs: vnum %d duplicated.", vnum);
                        exit(1);
                }
                fBootDb = TRUE;

                pMprog = alloc_perm(sizeof(*pMprog));
                pMprog->vnum = vnum;
                pMprog->code = fread_string(fp);
                if (mprog_list == NULL)
                        mprog_list = pMprog;
                else
                {
                        pMprog->next = mprog_list;
                        mprog_list = pMprog;
                }
                top_mprog_index++;
        }
        return;
}

void load_objprogs(FILE * fp)
{
        PROG_CODE *pOprog;

        if (area_last == NULL)
        {
                bug("Load_objprogs: no #AREA seen yet.", 0);
                exit(1);
        }

        for (;;)
        {
                int       vnum;
                char      letter;

                letter = fread_letter(fp);
                if (letter != '#')
                {
                        bug("Load_objprogs: # not found.", 0);
                        exit(1);
                }

                vnum = fread_number(fp);
                if (vnum == 0)
                        break;

                fBootDb = FALSE;
                if (get_prog_index(vnum, PRG_OPROG) != NULL)
                {
                        bug("Load_objprogs: vnum %d duplicated.", vnum);
                        exit(1);
                }
                fBootDb = TRUE;

                pOprog = alloc_perm(sizeof(*pOprog));
                pOprog->vnum = vnum;
                pOprog->code = fread_string(fp);
                if (oprog_list == NULL)
                        oprog_list = pOprog;
                else
                {
                        pOprog->next = oprog_list;
                        oprog_list = pOprog;
                }
                top_oprog_index++;
        }
        return;
}

void load_roomprogs(FILE * fp)
{
        PROG_CODE *pRprog;

        if (area_last == NULL)
        {
                bug("Load_roomprogs: no #AREA seen yet.", 0);
                exit(1);
        }

        for (;;)
        {
                int       vnum;
                char      letter;

                letter = fread_letter(fp);
                if (letter != '#')
                {
                        bug("Load_roomprogs: # not found.", 0);
                        exit(1);
                }

                vnum = fread_number(fp);
                if (vnum == 0)
                        break;

                fBootDb = FALSE;
                if (get_prog_index(vnum, PRG_RPROG) != NULL)
                {
                        bug("Load_roomprogs: vnum %d duplicated.", vnum);
                        exit(1);
                }
                fBootDb = TRUE;

                pRprog = alloc_perm(sizeof(*pRprog));
                pRprog->vnum = vnum;
                pRprog->code = fread_string(fp);
                if (rprog_list == NULL)
                        rprog_list = pRprog;
                else
                {
                        pRprog->next = rprog_list;
                        rprog_list = pRprog;
                }
                top_rprog_index++;
        }
        return;
}

/*
 *  Translate mobprog vnums pointers to real code
 */
void fix_mobprogs(void)
{
        MOB_INDEX_DATA *pMobIndex;
        PROG_LIST *list;
        PROG_CODE *prog;
        int       iHash;

        for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
        {
                for (pMobIndex = mob_index_hash[iHash];
                     pMobIndex != NULL; pMobIndex = pMobIndex->next)
                {
                        for (list = pMobIndex->mprogs; list != NULL;
                             list = list->next)
                        {
                                if ((prog =
                                     get_prog_index(list->vnum,
                                                    PRG_MPROG)) != NULL)
                                        list->code = prog->code;
                                else
                                {
                                        bug("Fix_mobprogs: code vnum %d not found.", list->vnum);
                                        exit(1);
                                }
                        }
                }
        }
}

void fix_objprogs(void)
{
        OBJ_INDEX_DATA *pObjIndex;
        PROG_LIST *list;
        PROG_CODE *prog;
        int       iHash;

        for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
        {
                for (pObjIndex = obj_index_hash[iHash];
                     pObjIndex != NULL; pObjIndex = pObjIndex->next)
                {
                        for (list = pObjIndex->oprogs; list != NULL;
                             list = list->next)
                        {
                                if ((prog =
                                     get_prog_index(list->vnum,
                                                    PRG_OPROG)) != NULL)
                                        list->code = prog->code;
                                else
                                {
                                        bug("Fix_objprogs: code vnum %d not found.", list->vnum);
                                        exit(1);
                                }
                        }
                }
        }
}

void fix_roomprogs(void)
{
        ROOM_INDEX_DATA *pRoomIndex;
        PROG_LIST *list;
        PROG_CODE *prog;
        int       iHash;

        for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
        {
                for (pRoomIndex = room_index_hash[iHash];
                     pRoomIndex != NULL; pRoomIndex = pRoomIndex->next)
                {
                        for (list = pRoomIndex->rprogs; list != NULL;
                             list = list->next)
                        {
                                if ((prog =
                                     get_prog_index(list->vnum,
                                                    PRG_RPROG)) != NULL)
                                        list->code = prog->code;
                                else
                                {
                                        bug("Fix_roomprogs: code vnum %d not found.", list->vnum);
                                        exit(1);
                                }
                        }
                }
        }
}

/*
void load_artis(void)
{
        randomize_object(33960);
        randomize_object(33961);
        randomize_object(33962);
        randomize_object(33963);
        randomize_object(33964);
        randomize_object(33965);
        randomize_object(33966);
        randomize_object(33967);
        randomize_object(33968);
        randomize_object(33969);
        randomize_object(33970);
        randomize_object(33971);
        randomize_object(33972);
        randomize_object(33973);
        randomize_object(33974);
        randomize_object(33975);
        randomize_object(33976);
        randomize_object(34000);
}
*/

/*
 * Snarf an obj section.
 */
void load_objects(FILE * fp)
{
        OBJ_INDEX_DATA *pObjIndex;

        if (!area_last)
        {
                bug("Load_objects: no #AREA seen yet.", 0);
                exit(1);
        }

        for (;;)
        {
                int       vnum;
                char      letter;
                int       iHash;

                letter = fread_letter(fp);
                if (letter != '#')
                {
                        bug("Load_objects: # not found.", 0);
                        exit(1);
                }

                vnum = fread_number(fp);
                if (vnum == 0)
                        break;

                fBootDb = FALSE;
                if (get_obj_index(vnum) != NULL)
                {
                        bug("Load_objects: vnum %d duplicated.", vnum);
                        exit(1);
                }

                fBootDb = TRUE;

                /*
                 * init object 
                 */
                pObjIndex = alloc_perm(sizeof(*pObjIndex));
                pObjIndex->vnum = vnum;
                pObjIndex->area = area_last;
                pObjIndex->affected = NULL;
                pObjIndex->extra_descr = NULL;

                /*
                 * loading object from file 
                 */
                pObjIndex->name = fread_string(fp);
                pObjIndex->short_descr = fread_string(fp);
                pObjIndex->description = fread_string(fp);

                pObjIndex->short_descr[0] = LOWER(pObjIndex->short_descr[0]);
                pObjIndex->description[0] = UPPER(pObjIndex->description[0]);

                pObjIndex->item_type = fread_number(fp);
                pObjIndex->extra_flags = fread_number(fp);
                pObjIndex->quest = fread_number(fp);
                pObjIndex->wear_flags = fread_number(fp);
                pObjIndex->condition = fread_number(fp);
                pObjIndex->toughness = fread_number(fp);
                pObjIndex->resistance = fread_number(fp);

                pObjIndex->hit_msg1 = fread_string(fp);
                pObjIndex->hit_msg2 = fread_string(fp);

                switch (pObjIndex->item_type)
                {
                case ITEM_WAND:
                case ITEM_STAFF:
                        pObjIndex->value[0] = fread_number(fp);
                        pObjIndex->value[1] = fread_number(fp);
                        pObjIndex->value[2] = fread_number(fp);
                        pObjIndex->value[3] = skill_lookup(fread_word(fp));
                        break;
                case ITEM_FAMILIAR:
                        pObjIndex->value[0] = fread_number(fp);
                        pObjIndex->value[1] = fread_number(fp);
                        pObjIndex->value[2] = fread_number(fp);
                        pObjIndex->value[3] = fread_number(fp);
                        pObjIndex->value[4] = fread_number(fp);
                        pObjIndex->value[5] = fread_number(fp);
                        pObjIndex->value[6] = fread_number(fp);
                        pObjIndex->value[7] = fread_number(fp);
                        break;
                case ITEM_SIGIL:
                        pObjIndex->value[0] = fread_number(fp);
                        pObjIndex->value[1] = fread_number(fp);
                        pObjIndex->value[2] = fread_number(fp);
                        pObjIndex->value[3] = fread_number(fp);
                        pObjIndex->value[4] = fread_number(fp);
                        pObjIndex->value[5] = fread_number(fp);
                        pObjIndex->value[6] = fread_number(fp);
                        pObjIndex->value[7] = fread_number(fp);
                        pObjIndex->value[8] = fread_number(fp);

                        break;
                case ITEM_POTION:
                case ITEM_PILL:
                case ITEM_SCROLL:
                        pObjIndex->value[0] = fread_number(fp);
                        pObjIndex->value[1] = skill_lookup(fread_word(fp));
                        pObjIndex->value[2] = skill_lookup(fread_word(fp));
                        pObjIndex->value[3] = skill_lookup(fread_word(fp));
                        break;
                default:
                        pObjIndex->value[0] = fread_number(fp);
                        pObjIndex->value[1] = fread_number(fp);
                        pObjIndex->value[2] = fread_number(fp);
                        pObjIndex->value[3] = fread_number(fp);
                        break;
                }
                pObjIndex->weight = fread_number(fp);
                pObjIndex->cost = fread_number(fp); /* Unused */
                pObjIndex->affected = NULL;
                pObjIndex->extra_descr = NULL;
                pObjIndex->chpoweron = NULL;
                pObjIndex->chpoweroff = NULL;
                pObjIndex->chpoweruse = NULL;
                pObjIndex->victpoweron = NULL;
                pObjIndex->victpoweroff = NULL;
                pObjIndex->victpoweruse = NULL;
                pObjIndex->spectype = 0;
                pObjIndex->specpower = 0;
                /*
                 * Cost per day           fread_number( fp ); 
                 */

                for (;;)
                {

                        letter = fread_letter(fp);

                        if (letter == 'A')
                        {
                                AFFECT_DATA *paf;

                                paf = alloc_perm(sizeof(*paf));
                                paf->type = -1;
                                paf->duration = -1;
                                paf->location = fread_number(fp);
                                paf->modifier = fread_number(fp);
                                paf->bitvector = 0;
                                paf->next = pObjIndex->affected;
                                pObjIndex->affected = paf;
                                top_affect++;
                        }

                        else if (letter == 'E')
                        {
                                EXTRA_DESCR_DATA *ed;

                                ed = alloc_perm(sizeof(*ed));
                                ed->keyword = fread_string(fp);
                                ed->description = fread_string(fp);
                                ed->buffer1 = fread_string(fp);
                                ed->buffer2 = fread_string(fp);
                                ed->type = fread_number(fp);
                                ed->vnum = fread_number(fp);
                                ed->action = fread_number(fp);
                                ed->next = pObjIndex->extra_descr;
                                pObjIndex->extra_descr = ed;
                                top_ed++;
                        }
                        else if (letter == 'O')
                        {
                                PROG_LIST *pOprog;
                                char     *word;
                                int       trigger = 0;

                                pOprog = alloc_perm(sizeof(*pOprog));
                                word = fread_word(fp);
                                if (!
                                    (trigger =
                                     flag_lookup(word, oprog_flags)))
                                {
                                        bug("OBJprogs: invalid trigger.", 0);
                                        exit(1);
                                }
                                SET_BIT(pObjIndex->oprog_flags, trigger);
                                pOprog->trig_type = trigger;
                                pOprog->vnum = fread_number(fp);
                                pOprog->trig_phrase = fread_string(fp);
                                pOprog->next = pObjIndex->oprogs;
                                pObjIndex->oprogs = pOprog;
                        }
                        else if (letter == 'Q')
                        {
                                pObjIndex->chpoweron = fread_string(fp);
                                pObjIndex->chpoweroff = fread_string(fp);
                                pObjIndex->chpoweruse = fread_string(fp);
                                pObjIndex->victpoweron = fread_string(fp);
                                pObjIndex->victpoweroff = fread_string(fp);
                                pObjIndex->victpoweruse = fread_string(fp);
                                pObjIndex->spectype = fread_number(fp);
                                pObjIndex->specpower = fread_number(fp);
                        }

                        else
                        {
                                ungetc(letter, fp);
                                break;
                        }
                }

                /*
                 * Translate spell "slot numbers" to internal "skill numbers."
                 */
                switch (pObjIndex->item_type)
                {
                case ITEM_PILL:
                case ITEM_POTION:
                case ITEM_SCROLL:
                case ITEM_STAFF:
                case ITEM_WAND:


                        if (vnum == 29503)
                                CHAOS = TRUE;
                        if (vnum == 29515)
                                VISOR = TRUE;
                        if (vnum == 29512)
                                DARKNESS = TRUE;
                        if (vnum == 29505)
                                SPEED = TRUE;
                        if (vnum == 29518)
                                BRACELET = TRUE;
                        if (vnum == 29504)
                                TORC = TRUE;
                        if (vnum == 29514)
                                ARMOUR = TRUE;
                        if (vnum == 29516)
                                CLAWS = TRUE;
                        if (vnum == 29555)
                                ITEMAFFMANTIS = TRUE;
                        if (vnum == 2654)
                                ITEMAFFENTROPY = TRUE;
                        if (vnum == 29598)
                                ITEMAFFENTROPY = TRUE;
                }

                iHash = vnum % MAX_KEY_HASH;
                pObjIndex->next = obj_index_hash[iHash];
                obj_index_hash[iHash] = pObjIndex;
                top_obj_index++;
                top_vnum_obj = top_vnum_obj < vnum ? vnum : top_vnum_obj;   /* OLC */
                assign_area_vnum(vnum); /* OLC */
        }
        return;
}

/*
 * Adds a reset to a room.  OLC
 * Similar to add_reset in olc.c
 */
void new_reset(ROOM_INDEX_DATA * pR, RESET_DATA * pReset)
{
        RESET_DATA *pr;

        if (!pR)
                return;

        pr = pR->reset_last;

        if (!pr)
        {
                pR->reset_first = pReset;
                pR->reset_last = pReset;
        }
        else
        {
                pR->reset_last->next = pReset;
                pR->reset_last = pReset;
                pR->reset_last->next = NULL;
        }

        top_reset++;
        return;
}

/*   
 * Snarf a reset section.       Changed for OLC.
 */
void load_resets(FILE * fp)
{
        RESET_DATA *pReset;
        int       iLastRoom = 0;
        int       iLastObj = 0;

        if (!area_last)
        {
                bug("Load_resets: no #AREA seen yet.", 0);
                exit(1);
        }

        for (;;)
        {
                EXIT_DATA *pexit;
                ROOM_INDEX_DATA *pRoomIndex;
                char      letter;

                if ((letter = fread_letter(fp)) == 'S')
                        break;

                if (letter == '*')
                {
                        fread_to_eol(fp);
                        continue;
                }

                pReset = alloc_perm(sizeof(*pReset));
                pReset->command = letter;
                /*
                 * if_flag 
                 */ fread_number(fp);
                pReset->arg1 = fread_number(fp);
                pReset->arg2 = fread_number(fp);
                pReset->arg3 = (letter == 'G' || letter == 'R')
                        ? 0 : fread_number(fp);
                fread_to_eol(fp);

                /*
                 * Validate parameters.
                 * We're calling the index functions for the side effect.
                 */
                switch (letter)
                {
                default:
                        bug("Load_resets: bad command '%c'.", letter);
                        exit(1);
                        break;

                case 'M':
                        get_mob_index(pReset->arg1);
                        if ((pRoomIndex = get_room_index(pReset->arg3)))
                        {
                                new_reset(pRoomIndex, pReset);
                                iLastRoom = pReset->arg3;
                        }
                        break;

                case 'O':
                        get_obj_index(pReset->arg1);
                        if ((pRoomIndex = get_room_index(pReset->arg3)))
                        {
                                new_reset(pRoomIndex, pReset);
                                iLastObj = pReset->arg3;
                        }
                        break;

                case 'P':
                        get_obj_index(pReset->arg1);
                        if ((pRoomIndex = get_room_index(iLastObj)))
                        {
                                new_reset(pRoomIndex, pReset);
                        }
                        break;

                case 'G':
                case 'E':
                        get_obj_index(pReset->arg1);
                        if ((pRoomIndex = get_room_index(iLastRoom)))
                        {
                                new_reset(pRoomIndex, pReset);
                                iLastObj = iLastRoom;
                        }
                        break;

                case 'D':
                        pRoomIndex = get_room_index(pReset->arg1);

                        if (pReset->arg2 < 0
                            || pReset->arg2 > 5
                            || !pRoomIndex
                            || !(pexit = pRoomIndex->exit[pReset->arg2])
                            || !IS_SET(pexit->rs_flags, EX_ISDOOR))
                        {
                                bug("Load_resets: 'D': exit %d not door.",
                                    pReset->arg2);
                                exit(1);
                        }

                        switch (pReset->arg3)   /* OLC 1.1b */
                        {
                        default:
                                bug("Load_resets: 'D': bad 'locks': %d.",
                                    pReset->arg3);
                        case 0:
                                break;
                        case 1:
                                SET_BIT(pexit->rs_flags, EX_CLOSED);
                                break;
                        case 2:
                                SET_BIT(pexit->rs_flags,
                                        EX_CLOSED | EX_LOCKED);
                                break;
                        }
                        break;

                case 'R':
                        if (pReset->arg2 < 0 || pReset->arg2 > 6)   /* Last Door. */
                        {
                                bug("Load_resets: 'R': bad exit %d.",
                                    pReset->arg2);
                                exit(1);
                        }

                        if ((pRoomIndex = get_room_index(pReset->arg1)))
                                new_reset(pRoomIndex, pReset);

                        break;
                }
        }

        return;
}

/* OLC 1.1b */
void load_rooms(FILE * fp)
{
        ROOM_INDEX_DATA *pRoomIndex;

        if (!area_last)
        {
                bug("Load_rooms: no #AREA seen yet.", 0);
                exit(1);
        }

        for (;;)
        {
                char      letter;
                int       vnum;
                int       door;
                int       iHash, i = 0;

                letter = fread_letter(fp);
                if (letter != '#')
                {
                        bug("Load_rooms: # not found.", 0);
                        exit(1);
                }

                vnum = fread_number(fp);
                if (vnum == 0)
                        break;

                fBootDb = FALSE;
                if (get_room_index(vnum))
                {
                        bug("Load_rooms: vnum %d duplicated.", vnum);
                        exit(1);
                }
                fBootDb = TRUE;

                pRoomIndex = alloc_perm(sizeof(*pRoomIndex));
                pRoomIndex->people = NULL;
                pRoomIndex->contents = NULL;
                pRoomIndex->extra_descr = NULL;
                pRoomIndex->area = area_last;
                pRoomIndex->vnum = vnum;
                pRoomIndex->name = fread_string(fp);
                pRoomIndex->description = fread_string(fp);
                pRoomIndex->room_flags = fread_number(fp);
                pRoomIndex->sector_type = fread_number(fp);


                pRoomIndex->shop_type = fread_number(fp);
                for (i = 1; i <= 25; i++)
                {
                        pRoomIndex->items_sold[i] = fread_number(fp);
                }


                pRoomIndex->light = 0;
                pRoomIndex->blood = 0;
                pRoomIndex->roomtext = NULL;
                for (door = 0; door <= 4; door++)
                {
                        pRoomIndex->track[door] = str_dup("");
                        pRoomIndex->track_dir[door] = 0;
                }
                for (door = 0; door <= 5; door++)
                        pRoomIndex->exit[door] = NULL;

                for (;;)
                {
                        letter = fread_letter(fp);

                        if (letter == 'S' || letter == 's')
                        {
                                if (letter == 's')
                                        bug("Load_rooms: vnum %d has lowercase 's'", vnum);
                                break;
                        }

                        if (letter == 'D')
                        {
                                EXIT_DATA *pexit;
                                int       locks;

                                door = fread_number(fp);
                                if (door < 0 || door > 5)
                                {
                                        bug("Fread_rooms: vnum %d has bad door number.", vnum);
                                        exit(1);
                                }

                                pexit = alloc_perm(sizeof(*pexit));
                                pexit->description = fread_string(fp);
                                pexit->keyword = fread_string(fp);
                                locks = fread_number(fp);
                                pexit->exit_info = locks;
                                pexit->rs_flags = locks;
                                pexit->key = fread_number(fp);
                                pexit->vnum = fread_number(fp);

                                pRoomIndex->exit[door] = pexit;
                                top_exit++;
                        }
                        else if (letter == 'E')
                        {
                                EXTRA_DESCR_DATA *ed;

                                ed = alloc_perm(sizeof(*ed));
                                ed->keyword = fread_string(fp);
                                ed->description = fread_string(fp);
                                ed->buffer1 = fread_string(fp);
                                ed->buffer2 = fread_string(fp);
                                ed->type = fread_number(fp);
                                ed->vnum = fread_number(fp);
                                ed->action = fread_number(fp);
                                ed->next = pRoomIndex->extra_descr;
                                pRoomIndex->extra_descr = ed;
                                top_ed++;
                        }
                        else if (letter == 'T')
                        {
                                ROOMTEXT_DATA *rt;

                                rt = alloc_perm(sizeof(*rt));
                                rt->input = fread_string(fp);
                                rt->output = fread_string(fp);
                                rt->choutput = fread_string(fp);
                                rt->name = fread_string(fp);
                                rt->type = fread_number(fp);
                                rt->power = fread_number(fp);
                                rt->mob = fread_number(fp);
                                rt->next = pRoomIndex->roomtext;
                                pRoomIndex->roomtext = rt;
                                top_rt++;
                        }
                        else if (letter == 'R')
                        {
                                PROG_LIST *pRprog;
                                char     *word;
                                int       trigger = 0;

                                pRprog = alloc_perm(sizeof(*pRprog));
                                word = fread_word(fp);
                                if (!
                                    (trigger =
                                     flag_lookup(word, rprog_flags)))
                                {
                                        bug("ROOMprogs: invalid trigger.", 0);
                                        exit(1);
                                }
                                SET_BIT(pRoomIndex->rprog_flags, trigger);
                                pRprog->trig_type = trigger;
                                pRprog->vnum = fread_number(fp);
                                pRprog->trig_phrase = fread_string(fp);
                                pRprog->next = pRoomIndex->rprogs;
                                pRoomIndex->rprogs = pRprog;
                        }
                        else
                        {
                                bug("Load_rooms: vnum %d has flag not 'DES'.",
                                    vnum);
                                exit(1);
                        }
                }

                iHash = vnum % MAX_KEY_HASH;
                pRoomIndex->next = room_index_hash[iHash];
                room_index_hash[iHash] = pRoomIndex;
                top_room++;
                top_vnum_room = top_vnum_room < vnum ? vnum : top_vnum_room;
                assign_area_vnum(vnum);
        }

        return;
}

/*
 * Snarf spec proc declarations.
 */
void load_specials(FILE * fp)
{
        for (;;)
        {
                MOB_INDEX_DATA *pMobIndex;
                char      letter;

                switch (letter = fread_letter(fp))
                {
                default:
                        bug("Load_specials: letter '%c' not *MS.", letter);
                        exit(1);

                case 'S':
                        return;

                case '*':
                        break;

                case 'M':
                        pMobIndex = get_mob_index(fread_number(fp));
                        pMobIndex->spec_fun = spec_lookup(fread_word(fp));
                        if (pMobIndex->spec_fun == 0)
                        {
                                bug("Load_specials: 'M': vnum %d.",
                                    pMobIndex->vnum);
                                exit(1);
                        }
                        break;

                case 'Q':
                        pMobIndex = get_mob_index(fread_number(fp));
                        pMobIndex->quest_fun = quest_lookup(fread_word(fp));
                        if (pMobIndex->quest_fun == 0)
                        {
                                bug("Load_specials: 'Q': vnum %d.",
                                    pMobIndex->vnum);
                                exit(1);
                        }
                        break;

                case 'Z':
                        pMobIndex = get_mob_index(fread_number(fp));
                        pMobIndex->shop_fun = shop_lookup(fread_word(fp));
                        if (pMobIndex->shop_fun == 0)
                        {
                                bug("Load_specials: 'Z': vnum %d.",
                                    pMobIndex->vnum);
                                exit(1);
                        }
                        break;

                }

                fread_to_eol(fp);
        }
}

/*
 * Translate all room exits from virtual to real.
 * Has to be done after all rooms are read in.
 * Check for bad reverse exits.
 */
void fix_exits(void)
{
        extern const sh_int rev_dir[];
        char      buf[MAX_STRING_LENGTH];
        ROOM_INDEX_DATA *pRoomIndex;
        ROOM_INDEX_DATA *to_room;
        EXIT_DATA *pexit;
        EXIT_DATA *pexit_rev;
        int       iHash;
        int       door;

        for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
        {
                for (pRoomIndex = room_index_hash[iHash]; pRoomIndex != NULL;
                     pRoomIndex = pRoomIndex->next)
                {
                        bool      fexit;

                        fexit = FALSE;
                        for (door = 0; door <= 5; door++)
                        {
                                if ((pexit = pRoomIndex->exit[door]) != NULL)
                                {
                                        fexit = TRUE;
                                        if (pexit->vnum <= 0)
                                                pexit->to_room = NULL;
                                        else
                                                pexit->to_room =
                                                        get_room_index(pexit->
                                                                       vnum);
                                }
                        }

                        if (!fexit)
                                SET_BIT(pRoomIndex->room_flags, ROOM_NO_MOB);
                }
        }

        for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
        {
                for (pRoomIndex = room_index_hash[iHash];
                     pRoomIndex != NULL; pRoomIndex = pRoomIndex->next)
                {
                        for (door = 0; door <= 5; door++)
                        {
                                if ((pexit = pRoomIndex->exit[door]) != NULL
                                    && (to_room = pexit->to_room) != NULL
                                    && (pexit_rev =
                                        to_room->exit[rev_dir[door]]) != NULL
                                    && pexit_rev->to_room != pRoomIndex)
                                {
                                        xprintf(buf,
                                                "Fix_exits: %d:%d -> %d:%d -> %d.",
                                                pRoomIndex->vnum, door,
                                                to_room->vnum, rev_dir[door],
                                                (pexit_rev->to_room ==
                                                 NULL) ? 0 : pexit_rev->
                                                to_room->vnum);
/*		    bug( buf, 0 ); */
                                }
                        }
                }
        }

        return;
}

/*
 * Repopulate areas periodically.
 */
void area_update(void)
{
        AREA_DATA *pArea;
        int       hash;
        ROOM_INDEX_DATA *room;
        char      buf[MAX_STRING_LENGTH];

        for (pArea = area_first; pArea != NULL; pArea = pArea->next)
        {
                AREA_AFFECT *paf;
                AREA_AFFECT *paf_next;

                /*
                 * Update Area Affects - Jobo
                 */
                for (paf = pArea->affects; paf; paf = paf_next)
                {
                        paf_next = paf->next;

                        if (paf->duration > 0)
                                paf->duration--;
                        else
                        {
                                area_affect_remove(pArea, paf);
                        }
                }

                if (++pArea->age < 3)
                        continue;

                switch (pArea->sector)
                {
                default:
                        break;
                case AREA_DESERT:
                        desert_update(pArea);
                        break;
                case AREA_SWAMP:
                        swamp_update(pArea);
                        break;
                case AREA_MOUNTAIN:
                        mountain_update(pArea);
                        break;
                case AREA_CITY:
                        city_update(pArea);
                        break;
                case AREA_UNDERWATER:
                        underwater_update(pArea);
                        break;
                case AREA_FOREST:
                        forest_update(pArea);
                        break;
                case AREA_HILLS:
                        hills_update(pArea);
                        break;
                case AREA_AIR:
                        air_update(pArea);
                        break;
                }
                weather_affects(pArea); //COMMENT THIS OUT if you don't want adversely affecting weather (trust me)

                /*
                 * Check age and reset.
                 * Note: Mud School resets every 3 minutes (not 6).
                 */
                if ((!pArea->empty
                     && (pArea->nplayer == 0 || pArea->age >= 6))
                    || pArea->age >= 15)
                {
                        ROOM_INDEX_DATA *pRoomIndex;

                        reset_area(pArea);
                        xprintf(buf, "%s has just been reset.", pArea->name);
                        wiznet(buf, NULL, NULL, WIZ_RESETS, 0, 0);

                        pArea->age = number_range(0, 3);
                        pRoomIndex = get_room_index(ROOM_VNUM_SCHOOL);
                        if (pRoomIndex != NULL && pArea == pRoomIndex->area)
                                pArea->age = 6 - 3;
                        else if (pArea->nplayer == 0)
                                pArea->empty = TRUE;
                }
        }
        /*
         * ROOMprog Triggers!
         */
        for (hash = 0; hash < MAX_KEY_HASH; hash++)
                for (room = room_index_hash[hash]; room; room = room->next)
                {
                        if (room->area->empty)
                                continue;

                        if (HAS_TRIGGER_ROOM(room, TRIG_DELAY)
                            && room->rprog_delay > 0)
                        {
                                if (--room->rprog_delay <= 0)
                                        p_percent_trigger(NULL, NULL, room,
                                                          NULL, NULL, NULL,
                                                          TRIG_DELAY);
                        }
                        else if (HAS_TRIGGER_ROOM(room, TRIG_RANDOM))
                                p_percent_trigger(NULL, NULL, room, NULL,
                                                  NULL, NULL, TRIG_RANDOM);
                }
        return;
}


/*
* OLC
 * Reset one room.  Called by reset_area and olc.
 */
void reset_room(ROOM_INDEX_DATA * pRoom)
{
        RESET_DATA *pReset;
        CHAR_DATA *pMob;
        OBJ_DATA *pObj;
        CHAR_DATA *LastMob = NULL;
        OBJ_DATA *LastObj = NULL;
        int       iExit;
        int       level = 0;
        bool      last;
        extern const sh_int rev_dir[];

        if (!pRoom)
                return;

        pMob = NULL;
        last = FALSE;

        for (iExit = 0; iExit < MAX_DIR; iExit++)
        {
                EXIT_DATA *pExit;

                if ((pExit = pRoom->exit[iExit]))
                {
                        pExit->exit_info = pExit->rs_flags;
                        if ((pExit->to_room != NULL)
                            &&
                            ((pExit = pExit->to_room->exit[rev_dir[iExit]])))
                        {
                                /*
                                 * nail the other side 
                                 */
                                pExit->exit_info = pExit->rs_flags;
                        }
                }
        }

        for (pReset = pRoom->reset_first; pReset != NULL;
             pReset = pReset->next)
        {
                MOB_INDEX_DATA *pMobIndex;
                OBJ_INDEX_DATA *pObjIndex;
                OBJ_INDEX_DATA *pObjToIndex;
                ROOM_INDEX_DATA *pRoomIndex;

                switch (pReset->command)
                {
                default:
                        bug("Reset_room: bad command %c.", pReset->command);
                        break;

                case 'M':
                        if (!(pMobIndex = get_mob_index(pReset->arg1)))
                        {
                                bug("Reset_room: 'M': bad vnum %d.",
                                    pReset->arg1);
                                continue;
                        }

                        if (pMobIndex->count >= pReset->arg2)
                        {
                                last = FALSE;
                                break;
                        }

                        pMob = create_mobile(pMobIndex);

                        /*
                         * Some more hard coding.
                         */
                        if (room_is_dark(pRoom))
                                SET_BIT(pMob->affected_by, AFF_INFRARED);

                        char_to_room(pMob, pRoom);

                        LastMob = pMob;
                        level = URANGE(0, pMob->level - 2, LEVEL_HERO);
                        last = TRUE;
                        break;

                case 'O':
                        if (!(pObjIndex = get_obj_index(pReset->arg1)))
                        {
                                bug("Reset_room: 'O': bad vnum %d.",
                                    pReset->arg1);
                                continue;
                        }

                        if (!(pRoomIndex = get_room_index(pReset->arg3)))
                        {
                                bug("Reset_room: 'O': bad vnum %d.",
                                    pReset->arg3);
                                continue;
                        }

                        if (pRoom->area->nplayer > 0
                            || count_obj_list(pObjIndex, pRoom->contents) > 0)
                                break;

                        pObj = create_object(pObjIndex, number_fuzzy(level));
                        pObj->cost = 0;
                        obj_to_room(pObj, pRoom);
                        break;

                case 'P':
                        if (!(pObjIndex = get_obj_index(pReset->arg1)))
                        {
                                bug("Reset_room: 'P': bad vnum %d.",
                                    pReset->arg1);
                                continue;
                        }
                        if (!(pObjToIndex = get_obj_index(pReset->arg3)))
                        {
                                bug("Reset_room: 'P': bad vnum %d.",
                                    pReset->arg3);
                                continue;
                        }

                        if (pRoom->area->nplayer > 0
                            || !(LastObj = get_obj_type(pObjToIndex))
                            || count_obj_list(pObjIndex,
                                              LastObj->contains) > 0)
                                break;

                        pObj = create_object(pObjIndex, number_fuzzy(level));
                        obj_to_obj(pObj, LastObj);

                        /*
                         * Ensure that the container gets reset.    OLC 1.1b
                         */
                        if (LastObj->item_type == ITEM_CONTAINER)
                        {
                                LastObj->value[1] =
                                        LastObj->pIndexData->value[1];
                        }
                        else
                        {
                                /*
                                 * THIS SPACE INTENTIONALLY LEFT BLANK 
                                 */
                        }
                        break;

                case 'G':
                case 'E':
                        if (!(pObjIndex = get_obj_index(pReset->arg1)))
                        {
                                bug("Reset_room: 'E' or 'G': bad vnum %d.",
                                    pReset->arg1);
                                continue;
                        }

                        if (!last)
                                break;

                        if (!LastMob)
                        {
                                bug("Reset_room: 'E' or 'G': null mob for vnum %d.", pReset->arg1);
                                last = FALSE;
                                break;
                        }

                        pObj = create_object(pObjIndex, number_fuzzy(level));
                        obj_to_char(pObj, LastMob);
                        if (pReset->command == 'E')
                                equip_char(LastMob, pObj, pReset->arg3);
                        last = TRUE;
                        break;

                case 'D':
                        break;

                case 'R':
/* OLC 1.1b
            if ( !( pRoomIndex = get_room_index( pReset->arg1 ) ) )  
            {
                bug( "Reset_room: 'R': bad vnum %d.", pReset->arg1 );
                continue;
            }
                 
            {       
                EXIT_DATA *pExit;
                int d0;
                int d1;
             
                for ( d0 = 0; d0 < pReset->arg2 - 1; d0++ )
                {
                    d1                   = number_range( d0, pReset->arg2-1 );
                    pExit                = pRoomIndex->exit[d0];
                    pRoomIndex->exit[d0] = pRoomIndex->exit[d1];
                    pRoomIndex->exit[d1] = pExit;
                }
            }
*/
                        break;
                }
        }

        return;
}

/* OLC
 * Reset one area.
 */
void reset_area(AREA_DATA * pArea)
{
        ROOM_INDEX_DATA *pRoom;
        int       vnum;

        for (vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++)
        {
                if ((pRoom = get_room_index(vnum)))
                        reset_room(pRoom);
        }

        return;
}

/*
 * Create an instance of a mobile.
 */
CHAR_DATA *create_mobile(MOB_INDEX_DATA * pMobIndex)
{
        CHAR_DATA *mob;
        OBJ_DATA *obj;

//    OBJ_DATA *obj2;

        int       tempvalue;

        if (pMobIndex == NULL)
        {
                bug("Create_mobile: NULL pMobIndex.", 0);
                exit(1);
        }

        if (char_free == NULL)
        {
                mob = alloc_perm(sizeof(*mob));
        }
        else
        {
                mob = char_free;
                char_free = char_free->next;
        }

        clear_char(mob);
        mob->pIndexData = pMobIndex;

        mob->hunting = str_dup("");
        mob->lord = str_dup("");
        mob->morph = str_dup("");
        mob->createtime = str_dup("");
        mob->lasttime = str_dup("");
        mob->lasthost = str_dup("");
        mob->powertype = str_dup("");
        mob->poweraction = str_dup("");
        mob->pload = str_dup("");
        mob->prompt = str_dup("");
        mob->cprompt = str_dup("");

        mob->name = str_dup(pMobIndex->player_name);    /* OLC */
        mob->short_descr = str_dup(pMobIndex->short_descr); /* OLC */
        mob->long_descr = str_dup(pMobIndex->long_descr);   /* OLC */
        mob->description = str_dup(pMobIndex->description); /* OLC */

        mob->spec_fun = pMobIndex->spec_fun;
        mob->quest_fun = pMobIndex->quest_fun;
        mob->shop_fun = pMobIndex->shop_fun;
        mob->mprog_target = NULL;
        mob->home = 3001;
        mob->form = 32767;
        mob->level = number_fuzzy(pMobIndex->level);
        mob->act = pMobIndex->act;
        mob->affected_by = pMobIndex->affected_by;
        mob->alignment = pMobIndex->alignment;
        mob->sex = pMobIndex->sex;
        mob->bones = pMobIndex->bones;
        mob->default_pos = pMobIndex->default_pos;
        mob->decay_pulse = -1;

        mob->armor = interpolate(mob->level, 100, -100);

        tempvalue =
                mob->level * 12 + number_range(mob->level * mob->level / 4,
                                               mob->level * mob->level);

        if (tempvalue > 300000 || tempvalue < 0)
                mob->max_hit = 300000;
        else
                mob->max_hit = tempvalue;

        mob->hit = mob->max_hit;

        mob->hitroll = mob->level * 1.5;
        mob->damroll = mob->level * 1.5;
        mob->practice = mob->level * (number_range(10, 20) / 5);


        if (number_range(1, 60) == 40)
        {
                obj = create_object(get_obj_index(number_range(11059, 11066)),
                                    0);

                if (obj->item_type == ITEM_SIGIL)
                {
                        if (number_range(1, 80) == 50)
                                obj_to_char(obj, mob);
                        else
                                extract_obj(obj);
                }
//      else
//        obj_to_char(obj,mob);
        }

        /*
         * Random Forge Objects 
         */
/*
        if (!mob->shop_fun)
        {
                if (number_percent() < 3)
                {
                        obj = create_object(get_obj_index
                                            (number_range(30048, 30071)), 0);

                        if (obj->pIndexData->vnum == 30052)
                        {
                                if (number_range(1, 5) == 2)
                                        obj_to_char(obj, mob);
                                else
                                        extract_obj(obj);

                        }
                        else if (obj->pIndexData->vnum == OBJ_VNUM_MITHRIL)
                        {
                                if (number_range(1, 15) == 5)
                                        obj_to_char(obj, mob);
                                else
                                        extract_obj(obj);

                        }
                        else
                                obj_to_char(obj, mob);

                }
        }
*/

        /*
         * Insert in list.
         */
        mob->next = char_list;
        char_list = mob;
        pMobIndex->count++;
        return mob;
}

PROG_CODE *get_prog_index(int vnum, int type)
{
        PROG_CODE *prg;

        switch (type)
        {
        case PRG_MPROG:
                prg = mprog_list;
                break;
        case PRG_OPROG:
                prg = oprog_list;
                break;
        case PRG_RPROG:
                prg = rprog_list;
                break;
        default:
                return NULL;
        }

        for (; prg; prg = prg->next)
        {
                if (prg->vnum == vnum)
                        return (prg);
        }
        return NULL;
}

/*
 * Create an instance of an object.
 */
OBJ_DATA *create_object(OBJ_INDEX_DATA * pObjIndex, int level)
{
        static OBJ_DATA obj_zero;
        OBJ_DATA *obj;

        if (pObjIndex == NULL)
        {
                bug("Create_object: NULL pObjIndex.", 0);
                exit(1);
        }

        if (obj_free == NULL)
        {
                obj = alloc_perm(sizeof(*obj));
        }
        else
        {
                obj = obj_free;
                obj_free = obj_free->next;
        }

        *obj = obj_zero;
        obj->pIndexData = pObjIndex;
        obj->in_room = NULL;
        obj->level = level;
        obj->wear_loc = -1;

        obj->name = str_dup(pObjIndex->name);   /* OLC */
        obj->short_descr = str_dup(pObjIndex->short_descr); /* OLC */
        obj->description = str_dup(pObjIndex->description); /* OLC */
        obj->hit_msg1 = str_dup(pObjIndex->hit_msg1);
        obj->hit_msg2 = str_dup(pObjIndex->hit_msg2);


        if (pObjIndex->chpoweron != NULL)
        {
                obj->chpoweron = pObjIndex->chpoweron;
                obj->chpoweroff = pObjIndex->chpoweroff;
                obj->chpoweruse = pObjIndex->chpoweruse;
                obj->victpoweron = pObjIndex->victpoweron;
                obj->victpoweroff = pObjIndex->victpoweroff;
                obj->victpoweruse = pObjIndex->victpoweruse;
                obj->spectype = pObjIndex->spectype;
                obj->specpower = pObjIndex->specpower;
        }
        else
        {
                obj->chpoweron = str_dup("(null)");
                obj->chpoweroff = str_dup("(null)");
                obj->chpoweruse = str_dup("(null)");
                obj->victpoweron = str_dup("(null)");
                obj->victpoweroff = str_dup("(null)");
                obj->victpoweruse = str_dup("(null)");
                obj->spectype = 0;
                obj->specpower = 0;
        }
        obj->questmaker = str_dup("");
        obj->questowner = str_dup("");
        obj->chobj = NULL;
        obj->points = 0;
        obj->item_type = pObjIndex->item_type;
        obj->extra_flags = pObjIndex->extra_flags;
        obj->quest = pObjIndex->quest;
        obj->wear_flags = pObjIndex->wear_flags;
        obj->value[0] = pObjIndex->value[0];
        obj->value[1] = pObjIndex->value[1];
        obj->value[2] = pObjIndex->value[2];
        obj->value[3] = pObjIndex->value[3];
        obj->value[4] = pObjIndex->value[4];
        obj->value[5] = pObjIndex->value[5];
        obj->value[6] = pObjIndex->value[6];
        obj->value[7] = pObjIndex->value[7];
        obj->value[8] = pObjIndex->value[8];
        obj->value[9] = pObjIndex->value[9];
        obj->value[10] = pObjIndex->value[10];
        obj->weight = pObjIndex->weight;
        obj->cost = pObjIndex->cost;
        obj->attack_level = 0;
        obj->fire_level = 0;
        obj->water_level = 0;
        obj->lightning_level = 0;
        obj->earth_level = 0;
        obj->sigil_level = 0;

        if (obj->pIndexData->vnum >= 33800 && obj->pIndexData->vnum <= 33899)   /* Prizes */
        {
                SET_BIT(obj->quest, QUEST_PRIZE);
                obj->condition = 100;
                obj->toughness = 100;
                obj->resistance = 1;
                obj->level = 60;
                obj->cost = 1000000;
        }
        else if (obj->pIndexData->vnum >= 33000 && obj->pIndexData->vnum <= 33320)  /* normal class eq */
        {
                obj->condition = 100;
                obj->toughness = 100;
                obj->resistance = 1;
                SET_BIT(obj->quest, QUEST_RELIC);
        }
        else if (obj->pIndexData->vnum >= 33321 && obj->pIndexData->vnum <= 33421)  /* normal class eq */
        {
                obj->condition = 100;
                obj->toughness = 100;
                obj->resistance = 1;
                SET_BIT(obj->quest, QUEST_RELIC);
        }
        else if (obj->pIndexData->vnum >= 33178 && obj->pIndexData->vnum <= 33326)  /* normal class eq */
        {
                obj->condition = 100;
                obj->toughness = 100;
                obj->resistance = 1;
                SET_BIT(obj->quest, QUEST_RELIC);
        }
        else if (obj->pIndexData->vnum >= 6204 && obj->pIndexData->vnum <= 6216)    /* normal class eq */
        {
                obj->condition = 100;
                obj->toughness = 100;
                obj->resistance = 1;
                SET_BIT(obj->quest, QUEST_RELIC);
        }
        else if (obj->pIndexData->vnum >= 29775 && obj->pIndexData->vnum <= 29990)  /* Undead Knight eq */
        {
                obj->condition = 100;
                obj->toughness = 100;
                obj->resistance = 1;
                SET_BIT(obj->quest, QUEST_RELIC);
        }
        else if (obj->pIndexData->vnum >= 800 && obj->pIndexData->vnum <= 825)  /* rune EQ */
        {
                SET_BIT(obj->quest, QUEST_RELIC);
                obj->condition = 100;
                obj->toughness = 100;
                obj->resistance = 1;
        }

        if (obj->condition == 0)
                obj->condition = 100;
        if (obj->toughness == 0)
                obj->toughness = 5;
        if (obj->toughness == 0)
                obj->resistance = 25;

        /*
         * Special bits for class eq
         */
        if (obj->pIndexData->vnum >= 33060 && obj->pIndexData->vnum <= 33079)
                SET_BIT(obj->spectype, SITEM_DROW);
        if (obj->pIndexData->vnum >= 33120 && obj->pIndexData->vnum <= 33139)
                SET_BIT(obj->spectype, SITEM_DEMONIC);


        /*
         * Mess with object properties.
         */
        switch (obj->item_type)
        {
        default:
                bug("Read_object: vnum %d bad type.", pObjIndex->vnum);
                break;

        case ITEM_LIGHT:
        case ITEM_TREASURE:
        case ITEM_GRENADE:
        case ITEM_FURNITURE:
        case ITEM_TRASH:
        case ITEM_CONTAINER:
        case ITEM_DRINK_CON:
        case ITEM_WINDOW:
        case ITEM_KEY:
        case ITEM_FOOD:
        case ITEM_BOAT:
        case ITEM_CORPSE_NPC:
        case ITEM_CORPSE_PC:
        case ITEM_FOUNTAIN:
        case ITEM_PORTAL:
        case ITEM_EGG:
        case ITEM_HOLYSYMBOL:
        case ITEM_VOODOO:
        case ITEM_STAKE:
        case ITEM_MISSILE:
        case ITEM_AMMO:
        case ITEM_QUEST:
        case ITEM_PIECE:
        case ITEM_MITHRIL:
        case ITEM_SYMBOL:
        case ITEM_BOOK:
        case ITEM_PAGE:
        case ITEM_TOOL:
        case ITEM_COPPER:
        case ITEM_IRON:
        case ITEM_STEEL:
        case ITEM_ADAMANTITE:
        case ITEM_COOKINGPOT:
        case ITEM_GEMSTONE:
        case ITEM_HILT:
        case ITEM_SCROLL:
        case ITEM_WAND:
        case ITEM_STAFF:
        case ITEM_FAITHTOKEN:
        case ITEM_SIGIL:
        case ITEM_FAMILIAR:
	case ITEM_RUNE:
                break;

        case ITEM_WEAPON:
                if (!IS_SET(obj->quest, QUEST_ARTIFACT)
                    && !IS_SET(obj->quest, QUEST_RELIC) && !IS_SET(obj->quest,
                                                                   QUEST_PRIZE))
                {
                        obj->value[1] =
                                number_range(obj->value[1], obj->value[2]);
                        obj->value[2] =
                                number_range((obj->value[1] + 1),
                                             (obj->value[1] * 2));
                }
                break;

        case ITEM_SLOT_MACHINE:
                if (obj->value[2] > 5)
                        obj->value[2] = 5;
                else if (obj->value[2] < 3)
                        obj->value[2] = 3;
                break;
        case ITEM_ARMOR:
                if (!IS_SET(obj->quest, QUEST_ARTIFACT)
                    && !IS_SET(obj->quest, QUEST_RELIC) && !IS_SET(obj->quest,
                                                                   QUEST_PRIZE))
                        obj->value[0] = number_range(10, obj->value[0]);
                break;

        case ITEM_POTION:
        case ITEM_PILL:
                obj->value[0] = number_fuzzy(number_fuzzy(obj->value[0]));
                break;

        case ITEM_MONEY:
                obj->value[0] = obj->cost;
                break;
        }

        obj->next = object_list;
        object_list = obj;
        pObjIndex->count++;

        return obj;
}

/*
 * Clear a new character.
 */
void clear_char(CHAR_DATA * ch)
{
        static CHAR_DATA ch_zero;

        *ch = ch_zero;
        ch->name = &str_empty[0];
        ch->short_descr = &str_empty[0];
        ch->long_descr = &str_empty[0];
        ch->description = &str_empty[0];
        ch->lord = &str_empty[0];
        ch->morph = &str_empty[0];
        ch->createtime = &str_empty[0];
        ch->lasthost = &str_empty[0];
        ch->lasttime = &str_empty[0];
        ch->powertype = &str_empty[0];
        ch->poweraction = &str_empty[0];
        ch->pload = &str_empty[0];
        ch->prompt = &str_empty[0];
        ch->cprompt = &str_empty[0];
        ch->hunting = &str_empty[0];

        ch->tracking = FALSE;
        ch->logon = current_time;
        ch->armor = 100;
        ch->position = POS_STANDING;
        ch->practice = 0;
        ch->hit = 1000;
        ch->max_hit = 1000;
        ch->mana = 1500;
        ch->max_mana = 1500;
        ch->move = 1500;
        ch->max_move = 1500;
        ch->master = NULL;
        ch->leader = NULL;
        ch->fighting = NULL;
        ch->mount = NULL;
        ch->wizard = NULL;
        ch->paradox[0] = 0;
        ch->paradox[1] = 0;
        ch->paradox[2] = 0;
        ch->damcap[0] = 1000;
        ch->damcap[1] = 0;
        return;
}

/*
 * Free a character.
 */
void free_char(CHAR_DATA * ch)
{
        OBJ_DATA *obj;
        OBJ_DATA *obj_next;
        AFFECT_DATA *paf;
        AFFECT_DATA *paf_next;
        AFFECT2_DATA *paf2;
        AFFECT2_DATA *paf2_next;
        ALIAS_DATA *ali;
        ALIAS_DATA *ali_next;
        QUEST_DATA *quest;
        QUEST_DATA *quest_next;
        MOB_TRIGGER *mProg;
        MOB_TRIGGER *mProg_next;

        for (obj = ch->carrying; obj != NULL; obj = obj_next)
        {
                obj_next = obj->next_content;
                extract_obj(obj);
        }

        for (paf2 = ch->affected2; paf2 != NULL; paf2 = paf2_next)
        {
                paf2_next = paf2->next;
                affect2_remove(ch, paf2);
        }

        for (paf = ch->affected; paf != NULL; paf = paf_next)
        {
                paf_next = paf->next;
                affect_remove(ch, paf);
        }

        for (mProg = ch->triggers; mProg != NULL; mProg = mProg_next)
        {
                mProg_next = mProg->next;
                trigger_remove(ch, mProg);
        }

        free_string(ch->name);
        free_string(ch->short_descr);
        free_string(ch->long_descr);
        free_string(ch->description);
        free_string(ch->lord);
        free_string(ch->morph);
        free_string(ch->createtime);
        free_string(ch->lasttime);
        free_string(ch->lasthost);
        free_string(ch->powertype);
        free_string(ch->poweraction);
        free_string(ch->pload);
        free_string(ch->prompt);
        free_string(ch->cprompt);
        free_string(ch->hunting);
        free_string(ch->objdesc);
        free_string(ch->prefix);
        free_string(ch->clan);
        free_string(ch->inter_editing);

        if (ch->pcdata != NULL)
        {
                /*
                 * recycling history 
                 */
                strip_history(ch);

                for (ali = ch->pcdata->alias; ali; ali = ali_next)
                {
                        ali_next = ali->next;
                        alias_remove(ch, ali);
                }



                for (quest = ch->pcdata->quests; quest; quest = quest_next)
                {
                        quest_next = quest->next;
                        quest_from_char(ch, quest);
                }


                free_string(ch->pcdata->retaliation);
                free_string(ch->pcdata->switchname);
                free_string(ch->pcdata->last_decap[0]);
                free_string(ch->pcdata->last_decap[1]);
                free_string(ch->pcdata->logoutmessage);
                free_string(ch->pcdata->avatarmessage);
                free_string(ch->pcdata->loginmessage);
                free_string(ch->pcdata->decapmessage);
                free_string(ch->pcdata->tiemessage);
                free_string(ch->pcdata->last_global);
                free_string(ch->pcdata->pwd);
                free_string(ch->pcdata->bamfin);
                free_string(ch->pcdata->bamfout);
                free_string(ch->pcdata->title);
                free_string(ch->pcdata->conception);
                free_string(ch->pcdata->parents);
                free_string(ch->pcdata->cparents);
                free_string(ch->pcdata->marriage);
                free_string(ch->pcdata->soultarget);
                free_string(ch->pcdata->love);
                free_string(ch->pcdata->host);



                ch->pcdata->next = pcdata_free;
                pcdata_free = ch->pcdata;
        }

        ch->next = char_free;
        char_free = ch;
        return;
}

/*
 * Get an extra description from a list.
 */
char     *get_extra_descr(char *name, EXTRA_DESCR_DATA * ed)
{
        for (; ed != NULL; ed = ed->next)
        {
                if (is_name(name, ed->keyword))
                        return ed->description;
        }
        return NULL;
}

/*
 * Translates mob virtual number to its mob index struct.
 * Hash table lookup.
 */
MOB_INDEX_DATA *get_mob_index(int vnum)
{
        MOB_INDEX_DATA *pMobIndex;

        for (pMobIndex = mob_index_hash[vnum % MAX_KEY_HASH];
             pMobIndex != NULL; pMobIndex = pMobIndex->next)
        {
                if (pMobIndex->vnum == vnum)
                        return pMobIndex;
        }

        if (fBootDb)
        {
                bug("Get_mob_index: bad vnum %d.", vnum);
                exit(1);
        }

        return NULL;
}

/*
 * Translates mob virtual number to its obj index struct.
 * Hash table lookup.
 */
OBJ_INDEX_DATA *get_obj_index(int vnum)
{
        OBJ_INDEX_DATA *pObjIndex;

        for (pObjIndex = obj_index_hash[vnum % MAX_KEY_HASH];
             pObjIndex != NULL; pObjIndex = pObjIndex->next)
        {
                if (pObjIndex->vnum == vnum)
                        return pObjIndex;
        }

        if (fBootDb)
        {
                bug("Get_obj_index: bad vnum %d.", vnum);
                exit(1);
        }

        return NULL;
}

/*
 * Translates mob virtual number to its room index struct.
 * Hash table lookup.
 */
ROOM_INDEX_DATA *get_room_index(int vnum)
{
        ROOM_INDEX_DATA *pRoomIndex;

        for (pRoomIndex = room_index_hash[vnum % MAX_KEY_HASH];
             pRoomIndex != NULL; pRoomIndex = pRoomIndex->next)
        {
                if (pRoomIndex->vnum == vnum)
                        return pRoomIndex;
        }

        if (fBootDb)
        {
                bug("Get_room_index: bad vnum %d.", vnum);
                exit(1);
        }

        return NULL;
}

/*
 * Read a letter from a file.
 */
char fread_letter(FILE * fp)
{
        char      c;

        do
        {
                c = getc(fp);
        }
        while (isspace(c));

        return c;
}

/*
 * Read a number from a file.
 */
int fread_number(FILE * fp)
{
        int       number;
        bool      sign;
        char     *ptr;
        int       c;

        ptr = top_string + sizeof(char *);

        do
        {
                c = getc(fp);
                *ptr = c;
        }
        while (isspace(c));

        number = 0;

        sign = FALSE;
        if (c == '+')
        {
                c = getc(fp);
                *ptr = c;
        }
        else if (c == '-')
        {
                sign = TRUE;
                c = getc(fp);
                *ptr = c;
        }

        if (!isdigit(c))
        {
                bug("Fread_number: bad format.", 0);
                exit(1);
        }

        while (isdigit(c))
        {
                number = number * 10 + c - '0';
                c = getc(fp);
                *ptr = c;
        }

        if (sign)
                number = 0 - number;

        if (c == '|')
                number += fread_number(fp);
        else if (c != ' ')
                ungetc(c, fp);

        return number;
}

long fread_flag(FILE * fp)
{
        int       number;
        char      c;
        bool      negative = FALSE;

        do
        {
                c = getc(fp);
        }
        while (isspace(c));

        if (c == '-')
        {
                negative = TRUE;
                c = getc(fp);
        }

        number = 0;

        if (!isdigit(c))
        {
                while (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
                {
                        number += flag_convert(c);
                        c = getc(fp);
                }
        }

        while (isdigit(c))
        {
                number = number * 10 + c - '0';
                c = getc(fp);
        }

        if (c == '|')
                number += fread_flag(fp);

        else if (c != ' ')
                ungetc(c, fp);

        if (negative)
                return -1 * number;

        return number;
}

long flag_convert(char letter)
{
        long      bitsum = 0;
        char      i;

        if ('A' <= letter && letter <= 'Z')
        {
                bitsum = 1;
                for (i = letter; i > 'A'; i--)
                        bitsum *= 2;
        }
        else if ('a' <= letter && letter <= 'z')
        {
                bitsum = 67108864;  /* 2^26 */
                for (i = letter; i > 'a'; i--)
                        bitsum *= 2;
        }

        return bitsum;
}




/*
 * Read and allocate space for a string from a file.
  * These strings are read-only and shared.
 * Strings are hashed:
 *   each string prepended with hash pointer to prev string,
 *   hash code is simply the string length.
 *   this function takes 40% to 50% of boot-up time.
 */
char     *fread_string(FILE * fp)
{
        char     *plast;
        char      c;

        plast = top_string + sizeof(char *);
        if (plast > &string_space[MAX_STRING - MAX_STRING_LENGTH])
        {
                bug("Fread_string: MAX_STRING %d exceeded.", MAX_STRING);
                exit(1);
        }

        /*
         * Skip blanks.
         * Read first char.
         */
        do
        {
                c = getc(fp);
        }
        while (isspace(c));

        if ((*plast++ = c) == '~')
                return &str_empty[0];
        for (;;)
        {
                /*
                 * Back off the char type lookup,
                 *   it was too dirty for portability.
                 *   -- Furey
                 */

                switch (*plast = getc(fp))
                {
                default:
                        plast++;
                        break;

                case EOF:
                        /*
                         * temp fix 
                         */
                        bug("Fread_string: EOF", 0);
                        return NULL;
                        /*
                         * exit( 1 ); 
                         */
                        break;

                case '\n':
                        plast++;
                        *plast++ = '\r';
                        break;

                case '\r':
                        break;

                case '~':
                        plast++;
                        {
                                union
                                {
                                        char     *pc;
                                        char      rgc[sizeof(char *)];
                                } u1;
                                int       ic;
                                int       iHash;
                                char     *pHash;
                                char     *pHashPrev;
                                char     *pString;

                                plast[-1] = '\0';
                                iHash = UMIN(MAX_KEY_HASH - 1,
                                             plast - 1 - top_string);
                                for (pHash = string_hash[iHash]; pHash;
                                     pHash = pHashPrev)
                                {
                                        for (ic = 0; ic < sizeof(char *);
                                             ic++)
                                                u1.rgc[ic] = pHash[ic];
                                        pHashPrev = u1.pc;
                                        pHash += sizeof(char *);

                                        if (top_string[sizeof(char *)] ==
                                            pHash[0]
                                            && !strcmp(top_string +
                                                       sizeof(char *) + 1,
                                                       pHash + 1))
                                                return pHash;
                                }

                                if (fBootDb)
                                {
                                        pString = top_string;
                                        top_string = plast;
                                        u1.pc = string_hash[iHash];
                                        for (ic = 0; ic < sizeof(char *);
                                             ic++)
                                                pString[ic] = u1.rgc[ic];
                                        string_hash[iHash] = pString;

                                        nAllocString += 1;
                                        sAllocString += top_string - pString;
                                        return pString + sizeof(char *);
                                }
                                else
                                {
                                        return str_dup(top_string +
                                                       sizeof(char *));
                                }
                        }
                }
        }
}

char     *fread_string_eol(FILE * fp)
{
        static bool char_special[256 - EOF];
        char     *plast;
        char      c;

        if (char_special[EOF - EOF] != TRUE)
        {
                char_special[EOF - EOF] = TRUE;
                char_special['\n' - EOF] = TRUE;
                char_special['\r' - EOF] = TRUE;
        }

        plast = top_string + sizeof(char *);
        if (plast > &string_space[MAX_STRING - MAX_STRING_LENGTH])
        {
                bug("Fread_string: MAX_STRING %d exceeded.", MAX_STRING);
                exit(1);
        }

        /*
         * Skip blanks.
         * Read first char.
         */
        do
        {
                c = getc(fp);
        }
        while (isspace(c));

        if ((*plast++ = c) == '\n')
                return &str_empty[0];

        for (;;)
        {
                if (!char_special[(*plast++ = getc(fp)) - EOF])
                        continue;

                switch (plast[-1])
                {
                default:
                        break;

                case EOF:
                        bug("Fread_string_eol  EOF", 0);
                        exit(1);
                        break;

                case '\n':
                case '\r':
                        {
                                union
                                {
                                        char     *pc;
                                        char      rgc[sizeof(char *)];
                                } u1;
                                int       ic;
                                int       iHash;
                                char     *pHash;
                                char     *pHashPrev;
                                char     *pString;

                                plast[-1] = '\0';
                                iHash = UMIN(MAX_KEY_HASH - 1,
                                             plast - 1 - top_string);
                                for (pHash = string_hash[iHash]; pHash;
                                     pHash = pHashPrev)
                                {
                                        for (ic = 0; ic < sizeof(char *);
                                             ic++)
                                                u1.rgc[ic] = pHash[ic];
                                        pHashPrev = u1.pc;
                                        pHash += sizeof(char *);

                                        if (top_string[sizeof(char *)] ==
                                            pHash[0]
                                            && !strcmp(top_string +
                                                       sizeof(char *) + 1,
                                                       pHash + 1))
                                                return pHash;
                                }

                                if (fBootDb)
                                {
                                        pString = top_string;
                                        top_string = plast;
                                        u1.pc = string_hash[iHash];
                                        for (ic = 0; ic < sizeof(char *);
                                             ic++)
                                                pString[ic] = u1.rgc[ic];
                                        string_hash[iHash] = pString;

                                        nAllocString += 1;
                                        sAllocString += top_string - pString;
                                        return pString + sizeof(char *);
                                }
                                else
                                {
                                        return str_dup(top_string +
                                                       sizeof(char *));
                                }
                        }
                }
        }
}



/*
 * Read to end of line (for comments).
 */
void fread_to_eol(FILE * fp)
{
        char      c;

        do
        {
                c = getc(fp);
        }
        while (c != '\n' && c != '\r');

        do
        {
                c = getc(fp);
        }
        while (c == '\n' || c == '\r');

        ungetc(c, fp);
        return;
}

/*
 * Read one word (into static buffer).
 */
char     *fread_word(FILE * fp)
{
        static char word[MAX_INPUT_LENGTH];
        char      buf[100];
        char     *pword;
        char      cEnd;

        do
        {
                cEnd = getc(fp);
        }
        while (isspace(cEnd));

        if (cEnd == '\'' || cEnd == '"')
        {
                pword = word;
        }
        else
        {
                word[0] = cEnd;
                pword = word + 1;
                cEnd = ' ';
        }

        for (; pword < word + MAX_INPUT_LENGTH; pword++)
        {
                *pword = getc(fp);
                if (cEnd == ' ' ? isspace(*pword) : *pword == cEnd)
                {
                        if (cEnd == ' ')
                                ungetc(*pword, fp);
                        *pword = '\0';
                        return word;
                }
        }

        word[20] = '\0';
        xprintf(buf, "Fread_word: word '%s' too long.", word);
        bug(buf, 0);
        exit(1);
        return NULL;
}

/*
 * Allocate some ordinary memory,
 *   with the expectation of freeing it someday.
 */
void     *alloc_mem(int sMem)
{
        void     *pMem;
        int       iList;

        pthread_mutex_lock(&memory_mutex);

        for (iList = 0; iList < MAX_MEM_LIST; iList++)
        {
                if (sMem <= rgSizeList[iList])
                        break;
        }

        if (iList == MAX_MEM_LIST)
        {
                bug("Alloc_mem: size %d too large.", sMem);
                exit(1);
        }

        if (rgFreeList[iList] == NULL)
        {
                pthread_mutex_unlock(&memory_mutex);
                pMem = alloc_perm(rgSizeList[iList]);
                pthread_mutex_lock(&memory_mutex);
        }
        else
        {
                pMem = rgFreeList[iList];
                rgFreeList[iList] = *((void **) rgFreeList[iList]);
        }

        pthread_mutex_unlock(&memory_mutex);
        return pMem;
}

/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
/*void free_mem(void *pMem, int sMem)
{
        int       iList;

        pthread_mutex_lock(&memory_mutex);

        for (iList = 0; iList < MAX_MEM_LIST; iList++)
        {
                if (sMem <= rgSizeList[iList])
                        break;
        }

        if (iList == MAX_MEM_LIST)
        {
                bug("Free_mem: size %d too large.", sMem);
                exit(1);
        }

        *((void **) pMem) = rgFreeList[iList];
        rgFreeList[iList] = pMem;

        pthread_mutex_unlock(&memory_mutex);
        return;
}
*/
void free_mem( void *pMem, int sMem )
{
    int iList;

    pthread_mutex_lock(&memory_mutex);

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
        if ( sMem <= rgSizeList[iList] )
            break;
    }

    if ( iList == MAX_MEM_LIST )
    {
        bug( "Free_mem: size %d too large.", sMem );
        exit( 1 );
    }

    * ((void **) pMem) = rgFreeList[iList];
    rgFreeList[iList]  = pMem;

    pthread_mutex_unlock(&memory_mutex);
    return;
}

/*
 * Allocate some permanent memory.
 * Permanent memory is never freed,
 *   pointers into it may be copied safely.
 */
/* void     *alloc_perm(int sMem)
{
        static char *pMemPerm;
        static int iMemPerm;
        void     *pMem;

        pthread_mutex_lock(&memory_mutex);

        while (sMem % sizeof(long) != 0)
                sMem++;
        if (sMem > MAX_PERM_BLOCK)
        {
                bug("Alloc_perm: %d too large.", sMem);
                exit(1);
        }

        if (pMemPerm == NULL || iMemPerm + sMem > MAX_PERM_BLOCK)
        {
                iMemPerm = 0;
                if ((pMemPerm = calloc(1, MAX_PERM_BLOCK)) == NULL)
                {
                        dump_last_command();
                        perror("Alloc_perm");
                        exit(1);
                }
        }

        pMem = pMemPerm + iMemPerm;
        iMemPerm += sMem;
        nAllocPerm += 1;
        sAllocPerm += sMem;

        pthread_mutex_unlock(&memory_mutex);
        return pMem;
}
*/
void *alloc_perm( int sMem )
{
    static char *pMemPerm;
    static int iMemPerm;
    void *pMem;

    pthread_mutex_lock(&memory_mutex);

    while ( sMem % sizeof(long) != 0 )
        sMem++;
    if ( sMem > MAX_PERM_BLOCK )
    {
        bug( "Alloc_perm: %d too large.", sMem );
        exit( 1 );
    }

    if ( pMemPerm == NULL || iMemPerm + sMem > MAX_PERM_BLOCK )
    {
        iMemPerm = 0;
        if ( ( pMemPerm = calloc( 1, MAX_PERM_BLOCK ) ) == NULL )
        {
            dump_last_command();
            perror( "Alloc_perm" );
            exit( 1 );
        }
    }

    pMem        = pMemPerm + iMemPerm;
    iMemPerm   += sMem;
    nAllocPerm += 1;
    sAllocPerm += sMem;

    pthread_mutex_unlock(&memory_mutex);
    return pMem;
}

/*
 * Duplicate a string into dynamic memory.
 * Fread_strings are read-only and shared.
 */
char     *str_dup(const char *str)
{
    char *str_new;

    if ( str[0] == '\0' )
    {
	return &str_empty[0];
    }
/*    if ( str >= string_space && str < top_string )
    {
	return (char *) str;
    }*/
    str_new = alloc_mem( strlen(str) + 1 );
    strcpy( str_new, str );
    return str_new; 
/*        static char *ret;
        int       len;

        if (!str)
                return NULL;

        len = strlen(str) + 1;

        CREATE(ret, char, len);
        mudstrlcpy(ret, str, len);
        return ret;*/
}

/*
 * Free a string.
 * Null is legal here to simplify callers.
 * Read-only shared strings are not touched.
 */
void free_string(char *pstr)
{

        if (pstr == NULL
            || pstr == &str_empty[0]
            || (pstr >= string_space && pstr < top_string))
        {
                return;
        }

        free_mem(pstr, strlen(pstr) + 1);
        return;
}

void do_areas(CHAR_DATA * ch, char *argument)
{
        char      buf[MAX_STRING_LENGTH];
        AREA_DATA *pArea1;
        AREA_DATA *pArea2;
        int       iArea;
        int       iAreaHalf;

        WAIT_STATE(ch, 10);
        iAreaHalf = (top_area + 1) / 2;
        pArea1 = area_first;
        pArea2 = area_first;
        for (iArea = 0; iArea < iAreaHalf; iArea++)
                pArea2 = pArea2->next;

        for (iArea = 0; iArea < iAreaHalf; iArea++)
        {
                xprintf(buf, "%-39s%-39s\n\r", pArea1->name,
                        (pArea2 != NULL) ? pArea2->name : "");
                send_to_char(buf, ch);
                pArea1 = pArea1->next;
                if (pArea2 != NULL)
                        pArea2 = pArea2->next;
        }

        return;
}

void do_memory(CHAR_DATA * ch, char *argument)
{
        char      buf[MAX_STRING_LENGTH];

        xprintf(buf, "Affects %5d\n\r", top_affect);
        send_to_char(buf, ch);
        xprintf(buf, "Areas   %5d\n\r", top_area);
        send_to_char(buf, ch);
        xprintf(buf, "RmTxt   %5d\n\r", top_rt);
        send_to_char(buf, ch);
        xprintf(buf, "ExDes   %5d\n\r", top_ed);
        send_to_char(buf, ch);
        xprintf(buf, "Exits   %5d\n\r", top_exit);
        send_to_char(buf, ch);
        xprintf(buf, "Helps   %5d\n\r", top_help);
        send_to_char(buf, ch);
        xprintf(buf, "Mobs    %5d\n\r", top_mob_index);
        send_to_char(buf, ch);
        xprintf(buf, "Objs    %5d\n\r", top_obj_index);
        send_to_char(buf, ch);
        xprintf(buf, "Resets  %5d\n\r", top_reset);
        send_to_char(buf, ch);
        xprintf(buf, "Rooms   %5d\n\r", top_room);
        send_to_char(buf, ch);

        xprintf(buf, "Strings %5d strings of %7d bytes (max %d).\n\r",
                nAllocString, sAllocString, MAX_STRING);
        send_to_char(buf, ch);

        xprintf(buf, "Perms   %5d blocks  of %7d bytes.\n\r", nAllocPerm,
                sAllocPerm);
        send_to_char(buf, ch);
        xprintf(buf, "\n\rMccp Allocated  %7d bytes.\n\r", mccp_mem_usage);
        send_to_char(buf, ch);
        xprintf(buf, "Mccp Freed      %7d bytes.\n\r", mccp_mem_freed);
        send_to_char(buf, ch);
        return;
}

/*
 * Stick a little fuzz on a number.
 */
int number_fuzzy(int number)
{
        switch (number_bits(2))
        {
        case 0:
                number -= 1;
                break;
        case 3:
                number += 1;
                break;
        }

        return UMAX(1, number);
}

/*
 * Generate a random number.
 */
int number_range(int from, int to)
{
        int       power;
        int       number;

        if ((to = to - from + 1) <= 1)
                return from;

        for (power = 2; power < to; power <<= 1)
                ;

        while ((number = number_mm() & (power - 1)) >= to)
                ;

        return from + number;
}

/*
 * Generate a percentile roll.
 */
int number_percent(void)
{
        int       percent;

        while ((percent = number_mm() & (128 - 1)) > 99)
                ;

        return 1 + percent;
}

/*
 * Generate a random door.
 */
int number_door(void)
{
        int       door;

        while ((door = number_mm() & (8 - 1)) > 5)
                ;

        return door;
}

int number_bits(int width)
{
        return number_mm() & ((1 << width) - 1);
}

/*
 * I've gotten too many bad reports on OS-supplied random number generators.
 * This is the Mitchell-Moore algorithm from Knuth Volume II.
 * Best to leave the constants alone unless you've read Knuth.
 * -- Furey
 */
static int rgiState[2 + 55];

void init_mm()
{
        int      *piState;
        int       iState;

        piState = &rgiState[2];

        piState[-2] = 55 - 55;
        piState[-1] = 55 - 24;

        piState[0] = ((int) current_time) & ((1 << 30) - 1);
        piState[1] = 1;
        for (iState = 2; iState < 55; iState++)
        {
                piState[iState] =
                        (piState[iState - 1] +
                         piState[iState - 2]) & ((1 << 30) - 1);
        }
        return;
}

int number_mm(void)
{
        int      *piState;
        int       iState1;
        int       iState2;
        int       iRand;

        piState = &rgiState[2];
        iState1 = piState[-2];
        iState2 = piState[-1];
        iRand = (piState[iState1] + piState[iState2]) & ((1 << 30) - 1);
        piState[iState1] = iRand;
        if (++iState1 == 55)
                iState1 = 0;
        if (++iState2 == 55)
                iState2 = 0;
        piState[-2] = iState1;
        piState[-1] = iState2;
        return iRand >> 6;
}

/*
 * Roll some dice.
 */
int dice(int number, int size)
{
        int       idice;
        int       sum;

        switch (size)
        {
        case 0:
                return 0;
        case 1:
                return number;
        }

        for (idice = 0, sum = 0; idice < number; idice++)
                sum += number_range(1, size);

        return sum;
}

/*
 * Simple linear interpolation.
 */
int interpolate(int level, int value_00, int value_32)
{
        return value_00 + level * (value_32 - value_00) / 32;
}

/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 */
void smash_tilde(char *str)
{
        for (; *str != '\0'; str++)
        {
                if (*str == '~')
                        *str = '-';
        }

        return;
}

/*
 * Compare strings, case insensitive.
 * Return TRUE if different
 *   (compatibility with historical functions).
 */
bool str_cmp(const char *astr, const char *bstr)
{
        if (astr == NULL)
        {
                bug("Str_cmp: null astr.", 0);
                return TRUE;
        }

        if (bstr == NULL)
        {
                bug("Str_cmp: null bstr.", 0);
                return TRUE;
        }

        for (; *astr || *bstr; astr++, bstr++)
        {
                if (LOWER(*astr) != LOWER(*bstr))
                        return TRUE;
        }

        return FALSE;
}

/*
 * Compare strings, case insensitive, for prefix matching.
 * Return TRUE if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix(const char *astr, const char *bstr)
{
        if (astr == NULL)
        {
                bug("Strn_cmp: null astr.", 0);
                return TRUE;
        }

        if (bstr == NULL)
        {
                bug("Strn_cmp: null bstr.", 0);
                return TRUE;
        }

        for (; *astr; astr++, bstr++)
        {
                if (LOWER(*astr) != LOWER(*bstr))
                        return TRUE;
        }

        return FALSE;
}

/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns TRUE is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix(const char *astr, const char *bstr)
{
        int       sstr1;
        int       sstr2;
        int       ichar;
        char      c0;

        if ((c0 = LOWER(astr[0])) == '\0')
                return FALSE;

        sstr1 = strlen(astr);
        sstr2 = strlen(bstr);

        for (ichar = 0; ichar <= sstr2 - sstr1; ichar++)
        {
                if (c0 == LOWER(bstr[ichar])
                    && !str_prefix(astr, bstr + ichar))
                        return FALSE;
        }

        return TRUE;
}

/*
 * Compare strings, case insensitive, for suffix matching.
 * Return TRUE if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix(const char *astr, const char *bstr)
{
        int       sstr1;
        int       sstr2;

        sstr1 = strlen(astr);
        sstr2 = strlen(bstr);
        if (sstr1 <= sstr2 && !str_cmp(astr, bstr + sstr2 - sstr1))
                return FALSE;
        else
                return TRUE;
}

/*
 * Returns an initial-capped string.
 */
char     *capitalize(const char *str)
{
        static char strcap[MAX_STRING_LENGTH];
        int       i;

        for (i = 0; str[i] != '\0'; i++)
                strcap[i] = LOWER(str[i]);
        strcap[i] = '\0';
        strcap[0] = UPPER(strcap[0]);
        return strcap;
}

/*
 * Append a string to a file.
 */
void append_file(CHAR_DATA * ch, char *file, char *str)
{
        FILE     *fp;

        if (IS_NPC(ch) || str[0] == '\0')
                return;

        fclose(fpReserve);
        if ((fp = fopen(file, "a")) == NULL)
        {
                perror(file);
                send_to_char("Could not open the file!\n\r", ch);
        }
        else
        {
                fprintf(fp, "[%5d] %s: %s\n",
                        ch->in_room ? ch->in_room->vnum : 0, ch->name, str);
                fclose(fp);
        }

        fpReserve = fopen(NULL_FILE, "r");
        return;
}

/*
 * Reports a bug.
 */
void bug(const char *str, int param)
{
        char      buf[MAX_STRING_LENGTH];
        FILE     *fp;

        if (fpArea != NULL)
        {
                int       iLine;
                int       iChar;

                if (fpArea == stdin)
                {
                        iLine = 0;
                }
                else
                {
                        iChar = ftell(fpArea);
                        fseek(fpArea, 0, 0);
                        for (iLine = 0; ftell(fpArea) < iChar; iLine++)
                        {
                                while (getc(fpArea) != '\n')
                                        ;
                        }
                        fseek(fpArea, iChar, 0);
                }

                xprintf(buf, "[*****] FILE: %s LINE: %d", strArea, iLine);
                log_string(LOG_BUG, buf);
                wiznet(buf, NULL, NULL, WIZ_DEBUG, 0, 0);

                if ((fp = fopen("shutdown.txt", "a")) != NULL)
                {
                        fprintf(fp, "[*****] %s\n", buf);
                        fclose(fp);
                }
        }

        strcpy(buf, "[*****] BUG: ");
        sprintf(buf + strlen(buf), str, param);
        log_string(LOG_BUG, buf);

        fclose(fpReserve);
        if ((fp = fopen(BUG_FILE, "a")) != NULL)
        {
                fprintf(fp, "%s\n", buf);
                fclose(fp);
        }
        fpReserve = fopen(NULL_FILE, "r");

        return;
}

/*
 * Writes a string to the log.
 */
/*
void log_string( const char *str )
{
    char *strtime;

    strtime                    = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    fprintf( stderr, "%s :: %s\n", strtime, str );
    return;
}
*/
/*
 * This function is here to aid in debugging.
 * If the last expression in a function is another function call,
 *   gcc likes to generate a JMP instead of a CALL.
 * This is called "tail chaining."
 * It hoses the debugger call stack for that call.
 * So I make this the last call in certain critical functions,
 *   where I really need the call stack to be right for debugging!
 *
 * If you don't understand this, then LEAVE IT ALONE.
 * Don't remove any calls to tail_chain anywhere.
 *
 * -- Furey
 */
void tail_chain(void)
{
        return;
}
