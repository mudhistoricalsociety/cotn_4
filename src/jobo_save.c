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
#include <time.h>
#include <unistd.h>

#include "merc.h"

void sort_top_board args((void));
void init_topboard  args((void));

void load_topboard()
{
        int       i;
        FILE     *fp;

        if ((fp = fopen("../txt/topboard.txt", "r")) == NULL)
        {
		init_topboard();
		if( (fp = fopen("../txt/topboard.txt", "r")) == NULL )
                {
			log_string(LOG_CRIT, "Error: topboard.txt not found!");
                	exit(1);
		}
        }
        top_board[0].name = " ";
        top_board[0].pkscore = 0;
        for (i = 1; i <= MAX_TOP_PLAYERS; i++)
        {
                top_board[i].name = fread_string(fp);
                top_board[i].pkscore = fread_number(fp);
        }
        fclose(fp);
}

// init_topboard added by Akurei
void init_topboard( void )
{
	int	i;
	FILE	*fp;

	if( (fp = fopen("../txt/topboard.txt", "w")) == NULL ) return;
	for( i = 1; i <= MAX_TOP_PLAYERS; i++ )
	{
		fprintf(fp, "None~\n");
		fprintf(fp, "0\n");
	}
	fclose(fp);
}

void update_top_board(CHAR_DATA * ch)
{
        int       i, j;
        bool      changed = FALSE;

        if (IS_NPC(ch))
                return;
        if (ch->level > 6)
                return;

        for (i = 1; i <= MAX_TOP_PLAYERS && !changed; i++)
        {
                if (!str_cmp(ch->name, top_board[i].name))
                {
                        if (ch->race != top_board[i].pkscore)
                                top_board[i].pkscore = ch->race;
                        changed = TRUE;
                        sort_top_board();
                        i = MAX_TOP_PLAYERS + 1;
                }
        }
        for (i = 1; i <= MAX_TOP_PLAYERS && !changed; i++)
        {
                if (ch->race > top_board[i].pkscore)
                {
                        for (j = MAX_TOP_PLAYERS; j > i; j--)
                        {
                                top_board[j].pkscore =
                                        top_board[j - 1].pkscore;
                                free_string(top_board[j].name);
                                top_board[j].name =
                                        str_dup(top_board[j - 1].name);
                        }
                        top_board[i].pkscore = ch->race;
                        free_string(top_board[i].name);
                        top_board[i].name = str_dup(ch->name);
                        changed = TRUE;
                        i = MAX_TOP_PLAYERS + 1;
                }
        }
        if (changed)
                save_topboard();
        return;
}

void sort_top_board()
{
        int       i, tempscore;
        bool      sorting = FALSE;
        char      buf[MAX_STRING_LENGTH];

        for (i = 1; i < MAX_TOP_PLAYERS; i++)
        {
                if (top_board[i].pkscore < top_board[i + 1].pkscore)
                {
                        tempscore = top_board[i].pkscore;
                        sprintf(buf, "%s", top_board[i].name); // changed from xprintf by Akurei, seems to have helped
                        free_string(top_board[i].name);
                        top_board[i].name = str_dup(top_board[i + 1].name);
                        top_board[i].pkscore = top_board[i + 1].pkscore;
                        free_string(top_board[i + 1].name);
                        top_board[i + 1].name = str_dup(buf);;
                        top_board[i + 1].pkscore = tempscore;
                        sorting = TRUE;
                }
        }
        if (sorting)
                sort_top_board();
        else
                return;
}

void save_topboard()
{
        FILE     *fp;
        int       i;

        if ((fp = fopen("../txt/topboard.txt", "w")) == NULL)
        {
                log_string(LOG_CRIT, "Error writing to topboard.txt");
                return;
        }
        for (i = 1; i <= MAX_TOP_PLAYERS; i++)
        {
                fprintf(fp, "%s~\n", top_board[i].name);
                fprintf(fp, "%d\n", top_board[i].pkscore);
        }
        fclose(fp);
}

void load_religions()
{
        int       i, j;
        FILE     *fp;

        if ((fp = fopen("../txt/religion.txt", "r")) == NULL)
        {
                log_string(LOG_CRIT, "Error: religion.txt not found!");
                exit(1);
        }
        religion_table[0].name = "#7of #RD#7aneguard#n";
        religion_table[0].truename = "Daneguard";
        religion_table[0].decapmessage =
                "#R%s #0was destroyed by the #RD#7a#RN#7e#RG#7u#RA#7r#RD #0member #R%s#n";
        religion_table[0].channel = "#Y.:#P%s#Y:.#n '#C$t#n'.";
        for (i = 0; i < 5; i++)
                religion_table[0].rankname[i] = "#RP#7rince#n";
        for (i = 1; i <= MAX_RELIGIONS; i++)
        {
                religion_table[i].name = fread_string(fp);
                religion_table[i].truename = fread_string(fp);
                for (j = 0; j < 5; j++)
                        religion_table[i].rankname[j] = fread_string(fp);
                religion_table[i].decapmessage = fread_string(fp);
                religion_table[i].channel = fread_string(fp);
        }
        fclose(fp);
}

void save_religions()
{
        int       i, j;
        FILE     *fp;

        if ((fp = fopen("../txt/religion.txt", "w")) == NULL)
        {
                log_string(LOG_CRIT, "Error writing to religion.txt");
                return;
        }
        for (i = 1; i <= MAX_RELIGIONS; i++)
        {
                fprintf(fp, "%s~\n", religion_table[i].name);
                fprintf(fp, "%s~\n", religion_table[i].truename);
                for (j = 0; j < 5; j++)
                        fprintf(fp, "%s~\n", religion_table[i].rankname[j]);
                fprintf(fp, "%s~\n", religion_table[i].decapmessage);
                fprintf(fp, "%s~\n", religion_table[i].channel);
        }
        fclose(fp);
}

void load_leaderboard()
{
        FILE     *fp;

        if ((fp = fopen("../txt/leader.txt", "r")) == NULL)
        {
                log_string(LOG_CRIT, "Error: leader.txt not found!");
                exit(1);
        }
        leader_board.pk_name = fread_string(fp);
        leader_board.pk_number = fread_number(fp);
        leader_board.time_name = fread_string(fp);
        leader_board.time_number = fread_number(fp);
        leader_board.quest_name = fread_string(fp);
        leader_board.quest_number = fread_number(fp);
        leader_board.mobkills_name = fread_string(fp);
        leader_board.mobkills_number = fread_number(fp);
        leader_board.pkscore_name = fread_string(fp);
        leader_board.pkscore_number = fread_number(fp);
        leader_board.arena_name = fread_string(fp);
        leader_board.arena_number = fread_number(fp);
        fclose(fp);
}

void save_leaderboard()
{
        FILE     *fp;

        if ((fp = fopen("../txt/leader.txt", "w")) == NULL)
        {
                log_string(LOG_CRIT, "Error writing to leader.txt");
                return;
        }
        fprintf(fp, "%s~\n", leader_board.pk_name);
        fprintf(fp, "%d\n", leader_board.pk_number);
        fprintf(fp, "%s~\n", leader_board.time_name);
        fprintf(fp, "%d\n", leader_board.time_number);
        fprintf(fp, "%s~\n", leader_board.quest_name);
        fprintf(fp, "%d\n", leader_board.quest_number);
        fprintf(fp, "%s~\n", leader_board.mobkills_name);
        fprintf(fp, "%d\n", leader_board.mobkills_number);
        fprintf(fp, "%s~\n", leader_board.pkscore_name);
        fprintf(fp, "%d\n", leader_board.pkscore_number);
        fprintf(fp, "%s~\n", leader_board.arena_name);
        fprintf(fp, "%d\n", leader_board.arena_number);
        fclose(fp);
}

void check_leaderboard(CHAR_DATA * ch)
{
        bool      changed = FALSE;

        if (IS_NPC(ch))
                return;
        if (ch->level > 6)
                return;
        if (get_ratio(ch) > leader_board.pkscore_number
            || !str_cmp(leader_board.pkscore_name, ch->name))
        {
                leader_board.pkscore_number = get_ratio(ch);
                free_string(leader_board.pkscore_name);
                leader_board.pkscore_name = str_dup(ch->name);
                changed = TRUE;
        }
        if (ch->mkill > leader_board.mobkills_number)
        {
                leader_board.mobkills_number = ch->mkill;
                free_string(leader_board.mobkills_name);
                leader_board.mobkills_name = str_dup(ch->name);
                changed = TRUE;
        }
        if (ch->pkill > leader_board.pk_number)
        {
                leader_board.pk_number = ch->pkill;
                free_string(leader_board.pk_name);
                leader_board.pk_name = str_dup(ch->name);
                changed = TRUE;
        }
        if (ch->pcdata->questtotal > leader_board.quest_number)
        {
                leader_board.quest_number = ch->pcdata->questtotal;
                free_string(leader_board.quest_name);
                leader_board.quest_name = str_dup(ch->name);
                changed = TRUE;
        }
        if ((get_age(ch) - 17) * 2 > leader_board.time_number)
        {
                leader_board.time_number = (get_age(ch) - 17) * 2;
                free_string(leader_board.time_name);
                leader_board.time_name = str_dup(ch->name);
                changed = TRUE;
        }
        if (ch->pcdata->awins > leader_board.arena_number)
        {
                leader_board.arena_number = ch->pcdata->awins;
                free_string(leader_board.arena_name);
                leader_board.arena_name = str_dup(ch->name);
                changed = TRUE;
        }
        if (changed)
                save_leaderboard();
        return;
}

void load_mudinfo()
{
        FILE     *fp;
        int       i;

        if ((fp = fopen("../txt/mudinfo.txt", "r")) == NULL)
        {
                log_string(LOG_CRIT, "Error: mudinfo.txt not found!");
                exit(1);
        }

        for (i = 0; i < (MUDINFO_MAX - 2); i++)
        {
                mudinfo[i] = fread_number(fp);
        }
        mudinfo[MUDINFO_MAX - 2] = 0;
        mudinfo[MUDINFO_MAX - 1] = 0;
        fclose(fp);
}

void load_ccenter()
{
        FILE     *fp;
        int       i;

        if ((fp = fopen("../txt/ccenter.txt", "r")) == NULL)
        {
                log_string(LOG_CRIT, "Error: ccenter.txt not found!");
                exit(1);
        }
        for (i = 0; i < CCENTER_MAX; i++)
        {
                ccenter[i] = fread_number(fp);
        }
        fclose(fp);
}

void save_ccenter()
{
        FILE     *fp;
        int       i;

        if ((fp = fopen("../txt/ccenter.txt", "w")) == NULL)
        {
                bug("could not write to ccenter.txt!", 0);
                return;
        }
        for (i = 0; i < CCENTER_MAX; i++)
        {
                fprintf(fp, "%d\n", ccenter[i]);
        }
        fclose(fp);
}

void write_mudinfo_database()
{
        FILE     *fp;
        int       ratio, mspusers, avusers, a, b, c;

        if ((fp = fopen("../txt/mud_data.txt", "a")) == NULL)
        {
                log_string(LOG_ERR, "Error writing to mud_data.txt");
                return;
        }

        /*
         * Calculate the ratio of users that use msp 
         */
        mspusers =
                (100 * mudinfo[MUDINFO_MSP_USERS] /
                 (mudinfo[MUDINFO_MCCP_USERS] +
                  mudinfo[MUDINFO_OTHER_USERS]));

        /*
         * Calculate the ratio of users that use mccp 
         */
        ratio = (100 * mudinfo[MUDINFO_MCCP_USERS] /
                 (mudinfo[MUDINFO_MCCP_USERS] +
                  mudinfo[MUDINFO_OTHER_USERS]));

        /*
         * Calculate the amount of average players online 
         */
        avusers =
                (mudinfo[MUDINFO_MCCP_USERS] +
                 mudinfo[MUDINFO_OTHER_USERS]) / mudinfo[MUDINFO_UPDATED];

        /*
         * Calculate the average tranfer rate in kbyte 
         */
        a = mudinfo[MUDINFO_MBYTE] * 1024 + mudinfo[MUDINFO_BYTE] / 1024;
        b = a / (mudinfo[MUDINFO_UPDATED] * 3);
        c = b / 10;
        c = c * 10;
        c = b - c;

        /*
         * Append it all to the file 
         */
        fprintf(fp, "\nMudinfo Database Entry\n");
        fprintf(fp, "Average Online Users       %d\n", avusers);
        fprintf(fp, "Peak Users Online          %d\n",
                mudinfo[MUDINFO_PEAK_USERS]);
        fprintf(fp, "Mccp Ratio                 %d %%\n", ratio);
        fprintf(fp, "MSP Ratio                  %d %%\n", mspusers);
        fprintf(fp, "Amount of MB send          %d MB\n",
                mudinfo[MUDINFO_MBYTE]);
        fprintf(fp, "Datatransfer Average       %d.%d\n", b / 10, c);

        /*
         * Calculating the peak transfer rate 
         */
        b = mudinfo[MUDINFO_DATA_PEAK] / (3 * 1024);
        c = b / 10;
        c = c * 10;
        c = b - c;

        fprintf(fp, "Datatransfer Peak          %d.%d\n", b / 10, c);

        fclose(fp);
}

void save_mudinfo()
{
        FILE     *fp;
        int       i;

        if ((fp = fopen("../txt/mudinfo.txt", "w")) == NULL)
        {
                log_string(LOG_ERR, "Error writing to mudinfo.txt");
                return;
        }
        for (i = 0; i < (MUDINFO_MAX - 2); i++)
        {
                fprintf(fp, "%d\n", mudinfo[i]);
        }
        fclose(fp);
}

void load_coreinfo()
{
        FILE     *fp;

        if ((fp = fopen("../txt/coreinfo.txt", "r")) == NULL)
        {
                log_string(LOG_CRIT, "Error: coreinfo.txt not found!");
                exit(1);
        }

        top_playerid = fread_number(fp);
        pulse_exp = fread_number(fp);
        pulse_dt = fread_number(fp);
        pulse_arena = fread_number(fp);
        pulse_cp = fread_number(fp);

        if (pulse_exp > 0)
                global_exp = TRUE;
        if (pulse_dt > 0)
                global_dt = TRUE;
        if (pulse_cp > 0)
                global_cp = TRUE;

        fclose(fp);
}

void save_coreinfo()
{
        FILE     *fp;
        int       i;

        for (i = 0; i < 2; i++)
        {
                if (i == 0)
                {
                        if ((fp = fopen("../txt/coreinfo.bck", "w")) == NULL)
                        {
                                log_string(LOG_ERR,
                                           "Error writing to coreinfo.bck");
                                return;
                        }
                }
                else
                {
                        if ((fp = fopen("../txt/coreinfo.txt", "w")) == NULL)
                        {
                                log_string(LOG_ERR,
                                           "Error writing to coreinfo.txt");
                                return;
                        }
                }

                fprintf(fp, "%d\n", top_playerid);
                fprintf(fp, "%d\n", pulse_exp);
                fprintf(fp, "%d\n", pulse_dt);
                fprintf(fp, "%d\n", pulse_arena);
                fprintf(fp, "%d\n", pulse_cp);
                fclose(fp);
        }
}

void load_newbiebans()
{
        FILE     *fp;
        BAN_DATA *p;
        char     *name;

        newbieban_list = NULL;
        fp = fopen(NEWBIEBAN_LIST, "r");
        if (!fp)
                return;
        name = fread_word(fp);
        while (str_cmp(name, END_MARKER))
        {
                p = alloc_mem(sizeof(BAN_DATA));
                p->name = str_dup(name);
                p->reason = fread_string(fp);
                p->next = newbieban_list;
                newbieban_list = p;
                name = fread_word(fp);
        }
        fclose(fp);
}

void save_newbiebans()
{
        FILE     *fp;
        BAN_DATA *p;

        if (!newbieban_list)
        {
                unlink(BAN_LIST);
                return;
        }
        fp = fopen(NEWBIEBAN_LIST, "w");
        if (!fp)
        {
                bug("could not open newban.txt", 0);
                return;
        }
        for (p = newbieban_list; p; p = p->next)
        {
                fprintf(fp, "%s\n", p->name);
                fprintf(fp, "%s~\n", p->reason);
        }
        fprintf(fp, "%s\n", END_MARKER);
        fclose(fp);
}



void load_bans()
{
        FILE     *fp;
        BAN_DATA *p;
        char     *name;

        ban_list = NULL;
        fp = fopen(BAN_LIST, "r");
        if (!fp)
                return;

        name = fread_word(fp);
        while (str_cmp(name, END_MARKER))
        {
                p = alloc_mem(sizeof(BAN_DATA));
                p->name = str_dup(name);
                p->reason = fread_string(fp);
                p->next = ban_list;
                ban_list = p;
                name = fread_word(fp);
        }
        fclose(fp);
}

void save_bans()
{
        FILE     *fp;
        BAN_DATA *p;

        if (!ban_list)
        {
                unlink(BAN_LIST);
                return;
        }
        if ((fp = fopen(BAN_LIST, "w")) == NULL)
        {
                bug("could not open ban.txt", 0);
                return;
        }
        for (p = ban_list; p; p = p->next)
        {
                fprintf(fp, "%s\n", p->name);
                fprintf(fp, "%s~\n", p->reason);
        }
        fprintf(fp, "%s\n", END_MARKER);
        fclose(fp);
}
