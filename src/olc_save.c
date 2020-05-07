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
 *  File: olc_save.c                                                       *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 *                                                                         *
 *  This code was freely distributed with the The Isles 1.1 source code,   *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.                *
 *                                                                         *
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
/* OLC_SAVE.C
 * This takes care of saving all the .are information.
 * Notes:
 * -If a good syntax checker is used for setting vnum ranges of areas
 *  then it would become possible to just cycle through vnums instead
 *  of using the iHash stuff and checking that the room or reset or
 *  mob etc is part of that area.
 */

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "olc.h"

/*****************************************************************************
 Name:		fix_string
 Purpose:	Returns a string without \r and ~.
 ****************************************************************************/
char     *fix_string(const char *str)
{
        static char strfix[MAX_STRING_LENGTH];
        int       i;
        int       o;

        if (str == NULL)
                return '\0';

        for (o = i = 0; str[i + o] != '\0'; i++)
        {
                if (str[i + o] == '\r' || str[i + o] == '~')
                        o++;
                if ((strfix[i] = str[i + o]) == '\0')
                        break;
        }
        strfix[i] = '\0';
        return strfix;
}

/*
 * doubles %%'s - a must for saving helps.
 */
char     *fix_string2(const char *str)
{
        static char strfix[2 * MAX_STRING_LENGTH];
        int       i;
        int       o;

        if (str == NULL)
                return '\0';

        for (o = i = 0; str[i] != '\0'; i++)
        {
                if ((strfix[i + o] = str[i]) == '%')
                {
                        o++;
                        strfix[i + o] = '%';
                }
        }
        strfix[i + o] = '\0';
        return strfix;
}

char     *fwrite_flag(long flags, char buf[])
{
        char      offset;
        char     *cp;

        buf[0] = '\0';

        if (flags == 0)
        {
                strcpy(buf, "0");
                return buf;
        }

        /*
         * 32 -- number of bits in a long 
         */

        for (offset = 0, cp = buf; offset < 32; offset++)
                if (flags & ((long) 1 << offset))
                {
                        if (offset <= 'Z' - 'A')
                                *(cp++) = 'A' + offset;
                        else
                                *(cp++) = 'a' + offset - ('Z' - 'A' + 1);
                }

        *cp = '\0';

        return buf;
}

/*****************************************************************************
 Name:		save_area_list
 Purpose:	Saves the listing of files to be loaded at startup.
 Called by:	do_asave(olc_save.c).
 ****************************************************************************/
void save_area_list(void)
{
        FILE     *fp;
        AREA_DATA *pArea;

        if ((fp = fopen("../txt/area.lst", "w")) == NULL)
        {
                bug("Save_area_list: fopen", 0);
                perror("area.lst");
        }
        else
        {
                /*
                 * Add any help files that need to be loaded at
                 * startup to this section.
                 */
                fprintf(fp, "help.are\n");

                for (pArea = area_first; pArea; pArea = pArea->next)
                {
                        fprintf(fp, "%s\n", pArea->filename);
                }

                fprintf(fp, "$\n");
                fclose(fp);
        }

        return;
}



void save_mobprogs(FILE * fp, AREA_DATA * pArea)
{
        PROG_CODE *pMprog;
        int       i;

        fprintf(fp, "#MOBPROGS\n");

        for (i = pArea->lvnum; i <= pArea->uvnum; i++)
        {
                if ((pMprog = get_prog_index(i, PRG_MPROG)) != NULL)
                {
                        fprintf(fp, "#%d\n", i);
                        fprintf(fp, "%s~\n", fix_string(pMprog->code));
                }
        }

        fprintf(fp, "#0\n\n");
        return;
}

void save_objprogs(FILE * fp, AREA_DATA * pArea)
{
        PROG_CODE *pOprog;
        int       i;

        fprintf(fp, "#OBJPROGS\n");

        for (i = pArea->lvnum; i <= pArea->uvnum; i++)
        {
                if ((pOprog = get_prog_index(i, PRG_OPROG)) != NULL)
                {
                        fprintf(fp, "#%d\n", i);
                        fprintf(fp, "%s~\n", fix_string(pOprog->code));
                }
        }

        fprintf(fp, "#0\n\n");
        return;
}

void save_roomprogs(FILE * fp, AREA_DATA * pArea)
{
        PROG_CODE *pRprog;
        int       i;

        fprintf(fp, "#ROOMPROGS\n");

        for (i = pArea->lvnum; i <= pArea->uvnum; i++)
        {
                if ((pRprog = get_prog_index(i, PRG_RPROG)) != NULL)
                {
                        fprintf(fp, "#%d\n", i);
                        fprintf(fp, "%s~\n", fix_string(pRprog->code));
                }
        }

        fprintf(fp, "#0\n\n");
        return;
}

/*****************************************************************************
 Name:		save_mobiles
 Purpose:	Save #MOBILES secion of an area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_mobiles(FILE * fp, AREA_DATA * pArea)
{
        int       vnum;
        MOB_INDEX_DATA *pMobIndex;
        PROG_LIST *pMprog;
        MOB_TRIGGER *mProg;

        fprintf(fp, "#MOBILES\n");
        for (vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++)
        {
                if ((pMobIndex = get_mob_index(vnum)))
                {
                        if (pMobIndex->area == pArea)
                        {
                                fprintf(fp, "#%d\n", pMobIndex->vnum);
                                fprintf(fp, "%s~\n", pMobIndex->player_name);
                                fprintf(fp, "%s~\n", pMobIndex->short_descr);
                                fprintf(fp, "%s~\n",
                                        fix_string(pMobIndex->long_descr));
                                fprintf(fp, "%s~\n",
                                        fix_string(pMobIndex->description));
                                fprintf(fp, "%d ", pMobIndex->act);
                                fprintf(fp, "%d ", pMobIndex->affected_by);
                                fprintf(fp, "%d\n", pMobIndex->alignment);
                                fprintf(fp, "%d ", pMobIndex->level);
                                fprintf(fp, "%d %d %d\n",
                                        pMobIndex->toughness,
                                        pMobIndex->extra_attack,
                                        pMobIndex->dam_modifier);
                                fprintf(fp, "%d %d\n", pMobIndex->sex,
                                        pMobIndex->natural_attack);
                                fprintf(fp, "%d\n", pMobIndex->bones);
                                fprintf(fp, "%d\n", 0);
                                for (pMprog = pMobIndex->mprogs; pMprog;
                                     pMprog = pMprog->next)
                                {
                                        fprintf(fp, "M %s %d %s~\n",
                                                prog_type_to_name(pMprog->
                                                                  trig_type),
                                                pMprog->vnum,
                                                pMprog->trig_phrase);
                                }
                                for (mProg = pMobIndex->triggers; mProg;
                                     mProg = mProg->next)
                                {
                                        fprintf(fp, "P\n");
                                        fprintf(fp, "%d %d\n", mProg->type,
                                                mProg->vnum);
                                        fprintf(fp, "%s~\n", mProg->keywords);
                                        fprintf(fp, "%s~\n",
                                                mProg->roomOutput);
                                        fprintf(fp, "%s~\n", mProg->chOutput);
                                }
                                //fprintf( fp, "S\n");
                        }
                }
        }
        fprintf(fp, "#0\n\n\n\n");
        return;
}

/*****************************************************************************
 Name:		save_objects
 Purpose:	Save #OBJECTS section of an area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_objects(FILE * fp, AREA_DATA * pArea)
{
        int       vnum;
        OBJ_INDEX_DATA *pObjIndex;
        AFFECT_DATA *pAf;
        EXTRA_DESCR_DATA *pEd;
        PROG_LIST *pOprog;

        fprintf(fp, "#OBJECTS\n");
        for (vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++)
        {
                if ((pObjIndex = get_obj_index(vnum)))
                {
                        if (pObjIndex->area == pArea)
                        {
                                fprintf(fp, "#%d\n", pObjIndex->vnum);
                                fprintf(fp, "%s~\n", pObjIndex->name);
                                fprintf(fp, "%s~\n", pObjIndex->short_descr);
                                fprintf(fp, "%s~\n",
                                        fix_string(pObjIndex->description));
                                fprintf(fp, "%d ", pObjIndex->item_type);
                                fprintf(fp, "%d ", pObjIndex->extra_flags);
                                fprintf(fp, "%d ", pObjIndex->quest);
                                fprintf(fp, "%d\n", pObjIndex->wear_flags);
                                fprintf(fp, "%d %d %d\n",
                                        pObjIndex->condition,
                                        pObjIndex->toughness,
                                        pObjIndex->resistance);

                                if (pObjIndex->hit_msg1 == NULL)
                                        pObjIndex->hit_msg1 = str_dup("none");
                                if (pObjIndex->hit_msg2 == NULL)
                                        pObjIndex->hit_msg2 = str_dup("none");
                                fprintf(fp, "%s~\n", pObjIndex->hit_msg1);
                                fprintf(fp, "%s~\n", pObjIndex->hit_msg2);

                                switch (pObjIndex->item_type)
                                {
                                default:
                                        fprintf(fp, "%d %d %d %d\n",
                                                pObjIndex->value[0],
                                                pObjIndex->value[1],
                                                pObjIndex->value[2],
                                                pObjIndex->value[3]);
                                        break;
                                case ITEM_PILL:
                                case ITEM_POTION:
                                case ITEM_SCROLL:
                                        fprintf(fp, "%d '%s' '%s' '%s'\n",
                                                pObjIndex->value[0],
                                                pObjIndex->value[1] != -1 ?
                                                skill_table[pObjIndex->
                                                            value[1]].
                                                name : " ",
                                                pObjIndex->value[2] !=
                                                -1 ? skill_table[pObjIndex->
                                                                 value[2]].
                                                name : " ",
                                                pObjIndex->value[3] !=
                                                -1 ? skill_table[pObjIndex->
                                                                 value[3]].
                                                name : " ");
                                        break;
                                case ITEM_SLOT_MACHINE:
                                        fprintf(fp, "%d %d %d %d\n",
                                                pObjIndex->value[0],
                                                pObjIndex->value[1],
                                                pObjIndex->value[2],
                                                pObjIndex->value[3]);
                                        break;

                                case ITEM_WINDOW:
                                        fprintf(fp, "%d 0 0 0\n",
                                                pObjIndex->value[0]);
                                        break;

                                case ITEM_FURNITURE:
                                        fprintf(fp, "%d %d %d %d\n",
                                                pObjIndex->value[0],
                                                pObjIndex->value[1],
                                                //fwrite_flag( pObjIndex->value[2], buf),
                                                pObjIndex->value[2],
                                                pObjIndex->value[3]);
                                        break;

                                case ITEM_FAMILIAR:
                                        fprintf(fp,
                                                "%d %d %d %d %d %d %d %d\n",
                                                pObjIndex->value[0],
                                                pObjIndex->value[1],
                                                pObjIndex->value[2],
                                                pObjIndex->value[3],
                                                pObjIndex->value[4],
                                                pObjIndex->value[5],
                                                pObjIndex->value[6],
                                                pObjIndex->value[7]);
                                        break;

                                case ITEM_SIGIL:
                                        fprintf(fp,
                                                "%d %d %d %d %d %d %d %d %d\n",
                                                pObjIndex->value[0],
                                                pObjIndex->value[1],
                                                pObjIndex->value[2],
                                                pObjIndex->value[3],
                                                pObjIndex->value[4],
                                                pObjIndex->value[5],
                                                pObjIndex->value[6],
                                                pObjIndex->value[7],
                                                pObjIndex->value[8]);
                                        break;

                                case ITEM_STAFF:
                                case ITEM_WAND:
                                        fprintf(fp, "%d %d %d '%s'\n",
                                                pObjIndex->value[0],
                                                pObjIndex->value[1],
                                                pObjIndex->value[2],
                                                pObjIndex->value[3] != -1 ?
                                                skill_table[pObjIndex->
                                                            value[3]].
                                                name : " ");
                                        break;
                                }
                                fprintf(fp, "%d ", pObjIndex->weight);
                                fprintf(fp, "%d\n", pObjIndex->cost);

                                for (pAf = pObjIndex->affected; pAf;
                                     pAf = pAf->next)
                                {
                                        fprintf(fp, "A\n%d %d\n",
                                                pAf->location, pAf->modifier);
                                }

                                for (pEd = pObjIndex->extra_descr; pEd;
                                     pEd = pEd->next)
                                {
                                        fprintf(fp,
                                                "E\n%s~\n%s~\n%s~\n%s~\n%d %d %d\n",
                                                pEd->keyword,
                                                fix_string(pEd->description),
                                                pEd->buffer1, pEd->buffer2,
                                                pEd->type, pEd->vnum,
                                                pEd->action);
                                }
                                fprintf(fp,
                                        "Q\n%s~\n%s~\n%s~\n%s~\n%s~\n%s~\n%d %d\n",
                                        pObjIndex->chpoweron,
                                        pObjIndex->chpoweroff,
                                        pObjIndex->chpoweruse,
                                        pObjIndex->victpoweron,
                                        pObjIndex->victpoweroff,
                                        pObjIndex->victpoweruse,
                                        pObjIndex->spectype,
                                        pObjIndex->specpower);
                                for (pOprog = pObjIndex->oprogs; pOprog;
                                     pOprog = pOprog->next)
                                {
                                        fprintf(fp, "O %s %d %s~\n",
                                                prog_type_to_name(pOprog->
                                                                  trig_type),
                                                pOprog->vnum,
                                                pOprog->trig_phrase);
                                }
                        }
                }

        }
        fprintf(fp, "#0\n\n\n\n");
        return;
}

/* OLC 1.1b */
/*****************************************************************************
 Name:		save_rooms
 Purpose:	Save #ROOMDATA section of an area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_rooms(FILE * fp, AREA_DATA * pArea)
{
        ROOM_INDEX_DATA *pRoomIndex;
        EXTRA_DESCR_DATA *pEd;
        ROOMTEXT_DATA *prt;
        EXIT_DATA *pExit;
        int       vnum;
        int       door;
        PROG_LIST *pRprog;

        fprintf(fp, "#ROOMDATA\n");
        for (vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++)
        {
                if ((pRoomIndex = get_room_index(vnum)))
                {
                        if (pRoomIndex->area == pArea)
                        {
                                fprintf(fp, "#%d\n", pRoomIndex->vnum);
                                fprintf(fp, "%s~\n", pRoomIndex->name);
                                fprintf(fp, "%s~\n",
                                        fix_string(pRoomIndex->description));
                                fprintf(fp, "%d ", pRoomIndex->room_flags);

                                fprintf(fp, "%d %d\n",
                                        pRoomIndex->sector_type,
                                        pRoomIndex->shop_type);
                                fprintf(fp, "%d ", pRoomIndex->items_sold[1]);
                                fprintf(fp, "%d ", pRoomIndex->items_sold[2]);
                                fprintf(fp, "%d ", pRoomIndex->items_sold[3]);
                                fprintf(fp, "%d ", pRoomIndex->items_sold[4]);
                                fprintf(fp, "%d ", pRoomIndex->items_sold[5]);
                                fprintf(fp, "%d ", pRoomIndex->items_sold[6]);
                                fprintf(fp, "%d ", pRoomIndex->items_sold[7]);
                                fprintf(fp, "%d ", pRoomIndex->items_sold[8]);
                                fprintf(fp, "%d ", pRoomIndex->items_sold[9]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[10]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[11]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[12]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[13]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[14]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[15]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[16]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[17]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[18]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[19]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[20]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[21]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[22]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[23]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[24]);
                                fprintf(fp, "%d ",
                                        pRoomIndex->items_sold[25]);

                                fprintf(fp, "\n");

                                for (pEd = pRoomIndex->extra_descr; pEd;
                                     pEd = pEd->next)
                                {
                                        fprintf(fp,
                                                "E\n%s~\n%s~\n%s~\n%s~\n%d %d %d\n",
                                                pEd->keyword,
                                                fix_string(pEd->description),
                                                pEd->buffer1, pEd->buffer2,
                                                pEd->type, pEd->vnum,
                                                pEd->action);
                                }
                                for (prt = pRoomIndex->roomtext; prt;
                                     prt = prt->next)
                                {
                                        fprintf(fp,
                                                "T\n%s~\n%s~\n%s~\n%s~\n%d %d %d\n",
                                                prt->input, prt->output,
                                                prt->choutput, prt->name,
                                                prt->type, prt->power,
                                                prt->mob);

                                }
                                for (door = 0; door < MAX_DIR; door++)
                                {
                                        if ((pExit = pRoomIndex->exit[door]))
                                        {
                                                fprintf(fp, "D%d\n", door);
                                                fprintf(fp, "%s~\n",
                                                        fix_string(pExit->
                                                                   description));
                                                fprintf(fp, "%s~\n",
                                                        pExit->keyword);
                                                fprintf(fp, "%d %d %d\n",
                                                        pExit->rs_flags,
                                                        pExit->key,
                                                        pExit->
                                                        to_room ? pExit->
                                                        to_room->vnum : 0);
                                        }
                                }

                                for (pRprog = pRoomIndex->rprogs; pRprog;
                                     pRprog = pRprog->next)
                                {
                                        fprintf(fp, "R %s %d %s~\n",
                                                prog_type_to_name(pRprog->
                                                                  trig_type),
                                                pRprog->vnum,
                                                pRprog->trig_phrase);
                                }

                                fprintf(fp, "S\n");
                        }
                }
        }
        fprintf(fp, "#0\n\n\n\n");
        return;
}

/* OLC 1.1b */
/*****************************************************************************
 Name:		save_specials
 Purpose:	Save #SPECIALS section of area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_specials(FILE * fp, AREA_DATA * pArea)
{
        int       vnum;
        MOB_INDEX_DATA *pMobIndex;

        fprintf(fp, "#SPECIALS\n");

        for (vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++)
        {
                if ((pMobIndex = get_mob_index(vnum)))
                {
                        if (pMobIndex->area == pArea && pMobIndex->spec_fun)
                        {
                                fprintf(fp, "M %d %s\n", pMobIndex->vnum,
                                        spec_string(pMobIndex->spec_fun));
                        }
                        if (pMobIndex->area == pArea && pMobIndex->quest_fun)
                        {
                                fprintf(fp, "Q %d %s\n", pMobIndex->vnum,
                                        quest_string(pMobIndex->quest_fun));
                        }
                        if (pMobIndex->area == pArea && pMobIndex->shop_fun)
                        {
                                fprintf(fp, "Z %d %s\n", pMobIndex->vnum,
                                        shop_string(pMobIndex->shop_fun));
                        }

                }
        }

        fprintf(fp, "S\n\n\n\n");
        return;
}

/* OLC 1.1b */
/*****************************************************************************
 Name:		vsave_specials
 Purpose:	Save #SPECIALS section of area file.
 		New formating thanks to Rac.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void vsave_specials(FILE * fp, AREA_DATA * pArea)
{
        int       vnum;
        MOB_INDEX_DATA *pMobIndex;

        fprintf(fp, "#SPECIALS\n");

        for (vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++)
        {
                if ((pMobIndex = get_mob_index(vnum)))
                {
                        if (pMobIndex->area == pArea && pMobIndex->spec_fun)
                        {
                                fprintf(fp, "M %d %s \t; %s\n",
                                        pMobIndex->vnum,
                                        spec_string(pMobIndex->spec_fun),
                                        pMobIndex->short_descr);
                        }
                }
        }

        fprintf(fp, "S\n\n\n\n");
        return;
}

/* OLC 1.1b */
/*****************************************************************************
 Name:		save_resets
 Purpose:	Saves the #RESETS section of an area file.
                New format thanks to Rac.
 Called by:	save_area(olc_save.c)
 ****************************************************************************/
void save_resets(FILE * fp, AREA_DATA * pArea)
{
        RESET_DATA *pReset;
        MOB_INDEX_DATA *pLastMob = NULL;
        OBJ_INDEX_DATA *pLastObj;
        ROOM_INDEX_DATA *pRoomIndex;
        char      buf[MAX_STRING_LENGTH];
        int       vnum;

        fprintf(fp, "#RESETS\n");

        for (vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++)
        {
                if ((pRoomIndex = get_room_index(vnum)))
                {
                        if (pRoomIndex->area == pArea)
                        {
                                for (pReset = pRoomIndex->reset_first; pReset;
                                     pReset = pReset->next)
                                {
                                        switch (pReset->command)
                                        {
                                        default:
                                                bug("Save_resets: bad command %c.", pReset->command);
                                                break;

                                        case 'M':
                                                pLastMob =
                                                        get_mob_index(pReset->
                                                                      arg1);
                                                fprintf(fp, "M 0 %d %d %d\n",
                                                        pReset->arg1,
                                                        pReset->arg2,
                                                        pReset->arg3);
                                                break;

                                        case 'O':
                                                pLastObj =
                                                        get_obj_index(pReset->
                                                                      arg1);
                                                fprintf(fp, "O 0 %d 0 %d\n",
                                                        pReset->arg1,
                                                        pReset->arg3);
                                                break;

                                        case 'P':
                                                pLastObj =
                                                        get_obj_index(pReset->
                                                                      arg1);
                                                fprintf(fp, "P 0 %d 0 %d\n",
                                                        pReset->arg1,
                                                        pReset->arg3);
                                                break;

                                        case 'G':
                                                fprintf(fp, "G 0 %d 0\n",
                                                        pReset->arg1);
                                                if (!pLastMob)
                                                {
                                                        xprintf(buf,
                                                                "Save_resets: !NO_MOB! in [%s]",
                                                                pArea->
                                                                filename);
                                                        bug(buf, 0);
                                                }
                                                break;

                                        case 'E':
                                                fprintf(fp, "E 0 %d 0 %d\n",
                                                        pReset->arg1,
                                                        pReset->arg3);
                                                if (!pLastMob)
                                                {
                                                        xprintf(buf,
                                                                "Save_resets: !NO_MOB! in [%s]",
                                                                pArea->
                                                                filename);
                                                        bug(buf, 0);
                                                }
                                                break;

                                        case 'D':
                                                break;

                                        case 'R':
                                                fprintf(fp, "R 0 %d %d\n",
                                                        pReset->arg1,
                                                        pReset->arg2);
                                                break;
                                        }   /* End switch */
                                }   /* End for pReset */
                        }   /* End if correct area */
                }   /* End if pRoomIndex */
        }   /* End for vnum */
        fprintf(fp, "S\n\n\n\n");
        return;
}

/* OLC 1.1b */
/*****************************************************************************
 Name:		save_resets
 Purpose:	Saves the #RESETS section of an area file.
                New format thanks to Rac.
 Called by:	save_area(olc_save.c)
 ****************************************************************************/
void vsave_resets(FILE * fp, AREA_DATA * pArea)
{
        RESET_DATA *pReset;
        MOB_INDEX_DATA *pLastMob = NULL;
        OBJ_INDEX_DATA *pLastObj;
        ROOM_INDEX_DATA *pRoomIndex;
        char      buf[MAX_STRING_LENGTH];
        int       vnum;

        fprintf(fp, "#RESETS\n");

        for (vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++)
        {
                if ((pRoomIndex = get_room_index(vnum)))
                {
                        if (pRoomIndex->area == pArea)
                        {
                                for (pReset = pRoomIndex->reset_first; pReset;
                                     pReset = pReset->next)
                                {
                                        switch (pReset->command)
                                        {
                                        default:
                                                bug("Save_resets: bad command %c.", pReset->command);
                                                break;

                                        case 'M':
                                                pLastMob =
                                                        get_mob_index(pReset->
                                                                      arg1);
                                                fprintf(fp,
                                                        "M 0 %d %2d %-5d \t; %s to %s\n",
                                                        pReset->arg1,
                                                        pReset->arg2,
                                                        pReset->arg3,
                                                        pLastMob->short_descr,
                                                        pRoomIndex->name);
                                                break;

                                        case 'O':
                                                pLastObj =
                                                        get_obj_index(pReset->
                                                                      arg1);
                                                fprintf(fp,
                                                        "O 0 %d  0 %-5d \t; %s to %s\n",
                                                        pReset->arg1,
                                                        pReset->arg3,
                                                        capitalize(pLastObj->
                                                                   short_descr),
                                                        pRoomIndex->name);
                                                break;

                                        case 'P':
                                                pLastObj =
                                                        get_obj_index(pReset->
                                                                      arg1);
                                                fprintf(fp,
                                                        "P 0 %d  0 %-5d \t; %s inside %s\n",
                                                        pReset->arg1,
                                                        pReset->arg3,
                                                        capitalize
                                                        (get_obj_index
                                                         (pReset->arg1)->
                                                         short_descr),
                                                        pLastObj ? pLastObj->
                                                        short_descr :
                                                        "!NO_OBJ!");

                                                if (!pLastObj)  /* Thanks Rac! */
                                                {
                                                        xprintf(buf,
                                                                "Save_resets: P with !NO_OBJ! in [%s]",
                                                                pArea->
                                                                filename);
                                                        bug(buf, 0);
                                                }

                                                break;

                                        case 'G':
                                                pLastObj =
                                                        get_obj_index(pReset->
                                                                      arg1);
                                                fprintf(fp,
                                                        "G 0 %d  0      \t;   %s\n",
                                                        pReset->arg1,
                                                        capitalize(pLastObj->
                                                                   short_descr));

                                                if (!pLastMob)
                                                {
                                                        xprintf(buf,
                                                                "Save_resets: !NO_MOB! in [%s]",
                                                                pArea->
                                                                filename);
                                                        bug(buf, 0);
                                                }
                                                break;

                                        case 'E':
                                                fprintf(fp,
                                                        "E 0 %d  0 %-5d \t;   %s %s\n",
                                                        pReset->arg1,
                                                        pReset->arg3,
                                                        capitalize
                                                        (get_obj_index
                                                         (pReset->arg1)->
                                                         short_descr),
                                                        flag_string
                                                        (wear_loc_strings,
                                                         pReset->arg3));
                                                if (!pLastMob)
                                                {
                                                        xprintf(buf,
                                                                "Save_resets: !NO_MOB! in [%s]",
                                                                pArea->
                                                                filename);
                                                        bug(buf, 0);
                                                }
                                                break;

                                        case 'D':
                                                break;

                                        case 'R':
                                                fprintf(fp,
                                                        "R 0 %d %2d      \t; Randomize %s\n",
                                                        pReset->arg1,
                                                        pReset->arg2,
                                                        pRoomIndex->name);
                                                break;
                                        }   /* End switch */
                                }   /* End for pReset */
                        }   /* End if correct area */
                }   /* End if pRoomIndex */
        }   /* End for vnum */
        fprintf(fp, "S\n\n\n\n");
        return;
}

/*****************************************************************************
 Name:		save_helps
 Purpose:	Save #HELPS section of an area file.
 Written by:	Walker <nkrendel@evans.Denver.Colorado.EDU>
 Called by:	save_area(olc_save.c).
 ****************************************************************************/

void save_help(void)
{
        FILE     *fp;
        HELP_DATA *pHelp;

        rename("help.are", "help.bak");

        if ((fp = fopen("help.are", "w")) == NULL)
        {
                bug("save_helps: fopen", 0);
                perror("help.are");
                return;
        }
        fprintf(fp, "#HELPS\n\n");
        for (pHelp = first_help; pHelp != NULL; pHelp = pHelp->next)
        {
                fprintf(fp, "%d %s ~\n%s~\n\n", pHelp->level,
                        all_capitalize(pHelp->keyword),
                        fix_string(pHelp->text));
        }
        fprintf(fp, "0 $~\n\n#$\n");
        fclose(fp);
}

void save_helps(FILE * fp, AREA_DATA * pArea)
{
        HELP_DATA *pHelp;
        bool      found = FALSE;

        for (pHelp = first_help; pHelp; pHelp = pHelp->next)
        {
                if (pHelp->area && pHelp->area == pArea)
                {
                        if (!found)
                        {
                                fprintf(fp, "#HELPS\n\n");
                                found = TRUE;
                        }
                        fprintf(fp, "%d %s~\n%s~\n", pHelp->level,
                                all_capitalize(pHelp->keyword),
                                fix_string(pHelp->text));
                }
        }

        if (found)
                fprintf(fp, "\n0 $~\n\n");

        return;
}

/*****************************************************************************
 Name:		save_area
 Purpose:	Save an area, note that this format is new.
 Called by:	do_asave(olc_save.c).
 ****************************************************************************/
void save_area(AREA_DATA * pArea)
{
        FILE     *fp;

        fclose(fpReserve);

        if (!(fp = fopen(pArea->filename, "w")))
        {
                bug("Open_area: fopen", 0);
                perror(pArea->filename);
        }

        fprintf(fp, "#AREADATA\n");
        fprintf(fp, "Name        %s~\n", pArea->name);
        fprintf(fp, "Builders    %s~\n", fix_string(pArea->builders));
        fprintf(fp, "Music       %s~\n", pArea->music);
        fprintf(fp, "VNUMs       %d %d\n", pArea->lvnum, pArea->uvnum);
        fprintf(fp, "Cvnum       %d\n", pArea->cvnum);
        fprintf(fp, "Security    %d\n", pArea->security);
        fprintf(fp, "Areabits    %d\n", pArea->areabits);
        fprintf(fp, "SecType  %d\n", pArea->sector);
        fprintf(fp, "WeatAl  %d\n", pArea->a_weather);
        fprintf(fp, "End\n\n\n\n");

        save_helps(fp, pArea);  /* OLC 1.1b */
        save_mobiles(fp, pArea);
        save_objects(fp, pArea);
        save_rooms(fp, pArea);
        save_mobprogs(fp, pArea);
        save_objprogs(fp, pArea);
        save_roomprogs(fp, pArea);
        if (IS_SET(pArea->area_flags, AREA_VERBOSE))    /* OLC 1.1b */
        {
                vsave_specials(fp, pArea);
                vsave_resets(fp, pArea);
        }
        else
        {
                save_specials(fp, pArea);
                save_resets(fp, pArea);
        }

        fprintf(fp, "#$\n");

        fclose(fp);
        fpReserve = fopen(NULL_FILE, "r");
        return;
}

/* OLC 1.1b */
/*****************************************************************************
 Name:		do_asave
 Purpose:	Entry point for saving area data.
 Called by:	interpreter(interp.c)
 ****************************************************************************/
void do_asave(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        AREA_DATA *pArea;
        int       value;

        if (!ch)    /* Do an autosave */
        {
                save_area_list();
                for (pArea = area_first; pArea; pArea = pArea->next)
                {
                        if (IS_SET(pArea->areabits, AREA_BIT_NOEDIT2))
                                continue;
                        if (IS_SET(pArea->areabits, AREA_BIT_NOEDIT))
                        {
                                SET_BIT(pArea->areabits, AREA_BIT_NOEDIT2);
                                REMOVE_BIT(pArea->areabits, AREA_BIT_NOEDIT);
                        }
                        save_area(pArea);
                        REMOVE_BIT(pArea->area_flags,
                                   AREA_CHANGED | AREA_ADDED);
                }
                return;
        }
        if (IS_NPC(ch))
                return;

        if (ch->pcdata->security < 2)
        {
                send_to_char("Huh?\n\r", ch);
                return;
        }

        argument = one_argument(argument, arg1);

        if (arg1[0] == '\0')
        {
                send_to_char("Syntax:\n\r", ch);
                send_to_char
                        ("  asave <vnum>    - saves a particular area\n\r",
                         ch);
                send_to_char
                        ("  asave list      - saves the area.lst file\n\r",
                         ch);
                send_to_char("  asave helps    - saves the help file\n\r",
                             ch);
                send_to_char
                        ("  asave area      - saves the area being edited\n\r",
                         ch);
                send_to_char
                        ("  asave changed   - saves all changed zones\n\r",
                         ch);
                send_to_char
                        ("  asave world     - saves the world! (db dump)\n\r",
                         ch);
                send_to_char("  asave ^ verbose - saves in verbose mode\n\r",
                             ch);
                send_to_char("\n\r", ch);
                return;
        }

        /*
         * Snarf the value (which need not be numeric). 
         */
        value = atoi(arg1);

        /*
         * Save the area of given vnum. 
         */
        /*
         * ---------------------------- 
         */

        if (!(pArea = get_area_data(value)) && is_number(arg1))
        {
                send_to_char("That area does not exist.\n\r", ch);
                return;
        }

        if (is_number(arg1))
        {
                if (!IS_BUILDER(ch, pArea))
                {
                        send_to_char
                                ("You are not a builder for this area.\n\r",
                                 ch);
                        return;
                }

                save_area_list();
                if (!str_cmp("verbose", argument))
                        SET_BIT(pArea->area_flags, AREA_VERBOSE);
                save_area(pArea);
                REMOVE_BIT(pArea->area_flags, AREA_VERBOSE);
                return;
        }

        /*
         * Save the world, only authorized areas. 
         */
        /*
         * -------------------------------------- 
         */

        if (!str_cmp("world", arg1))
        {
                save_area_list();
                for (pArea = area_first; pArea; pArea = pArea->next)
                {
                        if (IS_SET(pArea->areabits, AREA_BIT_NOEDIT2))
                                continue;
                        if (IS_SET(pArea->areabits, AREA_BIT_NOEDIT))
                        {
                                SET_BIT(pArea->areabits, AREA_BIT_NOEDIT2);
                                REMOVE_BIT(pArea->areabits, AREA_BIT_NOEDIT);
                        }

                        /*
                         * Builder must be assigned this area. 
                         */
                        if (!IS_BUILDER(ch, pArea))
                                continue;

                        if (!str_cmp("verbose", argument))
                                SET_BIT(pArea->area_flags, AREA_VERBOSE);
                        save_area(pArea);
                        REMOVE_BIT(pArea->area_flags,
                                   AREA_CHANGED | AREA_ADDED | AREA_VERBOSE);
                }
                send_to_char("You saved the world.\n\r", ch);
                return;
        }

        if (!str_cmp(arg1, "helps"))
        {

                if (ch->level > 6)
                {
                        save_help();
                        send_to_char(" Helps saved.\n\r", ch);
                }
                return;
        }

        /*
         * Save changed areas, only authorized areas. 
         */
        /*
         * ------------------------------------------ 
         */

        if (!str_cmp("changed", arg1))
        {
                char      buf[MAX_INPUT_LENGTH];

                save_area_list();
                //save_help();
                //send_to_char(" Helps saved.\n\r", ch);
                send_to_char("Saved zones:\n\r", ch);
                xprintf(buf, "None.\n\r");

                for (pArea = area_first; pArea; pArea = pArea->next)
                {
                        if (IS_SET(pArea->areabits, AREA_BIT_NOEDIT2))
                                continue;
                        if (IS_SET(pArea->areabits, AREA_BIT_NOEDIT))
                        {
                                SET_BIT(pArea->areabits, AREA_BIT_NOEDIT2);
                                REMOVE_BIT(pArea->areabits, AREA_BIT_NOEDIT);
                        }

                        /*
                         * Builder must be assigned this area. 
                         */
                        if (!IS_BUILDER(ch, pArea))
                                continue;

                        /*
                         * Save changed areas. 
                         */
                        if (IS_SET(pArea->area_flags, AREA_CHANGED)
                            || IS_SET(pArea->area_flags, AREA_ADDED))
                        {
                                if (!str_cmp("verbose", argument))
                                        SET_BIT(pArea->area_flags,
                                                AREA_VERBOSE);
                                save_area(pArea);
                                REMOVE_BIT(pArea->area_flags,
                                           AREA_CHANGED | AREA_ADDED |
                                           AREA_VERBOSE);
                                xprintf(buf, "%24s - '%s'\n\r", pArea->name,
                                        pArea->filename);
                                send_to_char(buf, ch);
                        }
                }
                if (!str_cmp(buf, "None.\n\r"))
                        send_to_char(buf, ch);
                return;
        }

        /*
         * Save the area.lst file. 
         */
        /*
         * ----------------------- 
         */
        if (!str_cmp(arg1, "list"))
        {
                save_area_list();
                return;
        }

        /*
         * Save area being edited, if authorized. 
         */
        /*
         * -------------------------------------- 
         */
        if (!str_cmp(arg1, "area"))
        {
                /*
                 * Find the area to save. 
                 */
                switch (ch->desc->editor)
                {
                case ED_AREA:
                        pArea = (AREA_DATA *) ch->desc->pEdit;
                        break;
                case ED_ROOM:
                        pArea = ch->in_room->area;
                        break;
                case ED_OBJECT:
                        pArea = ((OBJ_INDEX_DATA *) ch->desc->pEdit)->area;
                        break;
                case ED_MOBILE:
                        pArea = ((MOB_INDEX_DATA *) ch->desc->pEdit)->area;
                        break;
                default:
                        pArea = ch->in_room->area;
                        break;
                }

                if (IS_SET(pArea->areabits, AREA_BIT_NOEDIT2))
                {
                        send_to_char("This area cannot be changed.\n\r", ch);
                        return;
                }
                if (IS_SET(pArea->areabits, AREA_BIT_NOEDIT))
                {
                        SET_BIT(pArea->areabits, AREA_BIT_NOEDIT2);
                        REMOVE_BIT(pArea->areabits, AREA_BIT_NOEDIT);
                }

                if (!IS_BUILDER(ch, pArea))
                {
                        send_to_char
                                ("You are not a builder for this area.\n\r",
                                 ch);
                        return;
                }

                save_area_list();
                if (!str_cmp("verbose", argument))
                        SET_BIT(pArea->area_flags, AREA_VERBOSE);
                save_area(pArea);
                REMOVE_BIT(pArea->area_flags,
                           AREA_CHANGED | AREA_ADDED | AREA_VERBOSE);
                send_to_char("Area saved.\n\r", ch);
                return;
        }

        /*
         * Show correct syntax. 
         */
        /*
         * -------------------- 
         */
        do_asave(ch, "");
        return;
}
