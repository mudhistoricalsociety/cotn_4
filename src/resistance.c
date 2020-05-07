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
#include "recycle.h"

char     *show_res(CHAR_DATA * ch, int resist);

/*
 * Resistance improvement system.
 */

static char *const attack_types[] = {
        "unarmed", "slicing", "stabbing", "slashing", "whipping", "clawing",
        "blasting", "pounding", "crushing", "grepping", "biting", "piercing",
        "sucking"
};

static char *const magic_types[] = {
        "body", "mind", "fire", "ice", "electric", "acid", "voodoo", "drain"
};

char     *show_res(CHAR_DATA * ch, int resist)
{
        int       x, howmany = .1;  //How many stars to show
        static char buffer[MSL];

        howmany = (resist * 0.2) / 2;   //Showing 50 stars
        sprintf(buffer, "#D[#b");

        for (x = 1; x <= 50; x++)
        {
                if (x <= howmany)
                        xcatf(buffer, "#G*#n");
                else
                        xcatf(buffer, "#r-#n");
        }

        xcatf(buffer, "#D]#n\n\r");

        return buffer;
}


void do_resistance(CHAR_DATA * ch, char *argument)
{
        char      arg[MAX_INPUT_LENGTH];
        char      skill[13];

        one_argument(argument, arg);

        if (IS_NPC(ch))
                return;

        send_to_char
                ("---------------------------=[#6Weapon Resistance#n]=--------------------------------\n\r",
                 ch);
        sprintf(skill, "Slice  : %s", show_res(ch, ch->wpn_res[1]));
        send_to_char(skill, ch);
        sprintf(skill, "Stab   : %s", show_res(ch, ch->wpn_res[2]));
        send_to_char(skill, ch);
        sprintf(skill, "Slash  : %s", show_res(ch, ch->wpn_res[3]));
        send_to_char(skill, ch);
        sprintf(skill, "Whip   : %s", show_res(ch, ch->wpn_res[4]));
        send_to_char(skill, ch);
        sprintf(skill, "Claw   : %s", show_res(ch, ch->wpn_res[5]));
        send_to_char(skill, ch);
        sprintf(skill, "Blast  : %s", show_res(ch, ch->wpn_res[6]));
        send_to_char(skill, ch);
        sprintf(skill, "Pound  : %s", show_res(ch, ch->wpn_res[7]));
        send_to_char(skill, ch);
        sprintf(skill, "Crush  : %s", show_res(ch, ch->wpn_res[8]));
        send_to_char(skill, ch);
        sprintf(skill, "Grep   : %s", show_res(ch, ch->wpn_res[9]));
        send_to_char(skill, ch);
        sprintf(skill, "Bite   : %s", show_res(ch, ch->wpn_res[10]));
        send_to_char(skill, ch);
        sprintf(skill, "Pierce : %s", show_res(ch, ch->wpn_res[11]));
        send_to_char(skill, ch);
        sprintf(skill, "Suck   : %s", show_res(ch, ch->wpn_res[12]));
        send_to_char(skill, ch);
        sprintf(skill, "Unarmed: %s\r\n", show_res(ch, ch->wpn_res[0]));
        send_to_char(skill, ch);

        send_to_char
                ("---------------------------=[#6Magic Resistance#n]=--------------------------------\n\r",
                 ch);
        sprintf(skill, "Mind   : %-4d", ch->magic_res[MAGIC_MIND]);
        send_to_char(skill, ch);
        sprintf(skill, " Body   : %-4d", ch->magic_res[MAGIC_BODY]);
        send_to_char(skill, ch);
        sprintf(skill, " Fire   : %-4d", ch->magic_res[MAGIC_FIRE]);
        send_to_char(skill, ch);
        sprintf(skill, " Ice    : %-4d\n", ch->magic_res[MAGIC_ICE]);
        send_to_char(skill, ch);
        sprintf(skill, "Electric: %-4d", ch->magic_res[MAGIC_ELEC]);
        send_to_char(skill, ch);
        sprintf(skill, " Voodoo : %-4d", ch->magic_res[MAGIC_VOODOO]);
        send_to_char(skill, ch);
        sprintf(skill, " Drain  : %-4d", ch->magic_res[MAGIC_DRAIN]);
        send_to_char(skill, ch);
        sprintf(skill, " Acid   : %-4d\n\r\n", ch->magic_res[MAGIC_ACID]);
        send_to_char(skill, ch);

        send_to_char
                ("---------------------------=[#6**************#n]=--------------------------------\n\r",
                 ch);
        return;
}




void improve_weapon_res(CHAR_DATA * ch, int dtype)
{
        char      bufskill[20];
        char      buf[MAX_INPUT_LENGTH];
        int       dice1;
        int       dice2;
        int       max_skl = 500;

        dice1 = number_percent();
        dice2 = number_percent();

        if (IS_NPC(ch))
                return;

        if (dtype < 1000 || dtype > 1012)
                return;

        dtype -= 1000;

        if (ch->wpn_res[dtype] > max_skl)
                return;

        if ((dice1 > 89) || (dice2 > 89))
                ch->wpn_res[dtype] += 1;
        else
                return;

        if (ch->wpn_res[dtype] == 1)
                sprintf(bufskill, "slightly resistant");
        else if (ch->wpn_res[dtype] == 50)
                sprintf(bufskill, "reasonably resistant");
        else if (ch->wpn_res[dtype] == 100)
                sprintf(bufskill, "fairly resistant");
        else if (ch->wpn_res[dtype] == 150)
                sprintf(bufskill, "highly resistant");
        else if (ch->wpn_res[dtype] == 175)
                sprintf(bufskill, "very resistant");
        else if (ch->wpn_res[dtype] == 200)
                sprintf(bufskill, "extremely resistant");
        else if (ch->wpn_res[dtype] == 251)
                sprintf(bufskill, "slightly immune");
        else if (ch->wpn_res[dtype] == 300)
                sprintf(bufskill, "reasonably immune");
        else if (ch->wpn_res[dtype] == 350)
                sprintf(bufskill, "fairly immune");
        else if (ch->wpn_res[dtype] == 400)
                sprintf(bufskill, "highly immune");
        else if (ch->wpn_res[dtype] == 500)
                sprintf(bufskill, "virtually immune");
        else
                return;

        sprintf(buf, "#cYou are now %s against %s attacks.\n\r#n", bufskill,
                attack_types[dtype]);
        send_to_char(buf, ch);
        return;
}

int check_weapon_res(CHAR_DATA * ch, int dtype)
{
        int       x;

        x = .0018 * ch->wpn_res[dtype];

        if (x == 0)
                x = 1;

        return x;
}

int check_magic_res(CHAR_DATA * ch, int mtype)
{
        int       x;

        x = .0018 * ch->magic_res[mtype];

        if (x == 0) //
                x = 1;

        return x;
}


void improve_magic_res(CHAR_DATA * ch, int mtype)
{
        int       dice1, dice2;
        int       max_skl = 500;
        char      bufskill[20];
        char      buf[MAX_INPUT_LENGTH];

        dice1 = number_percent();
        dice2 = number_percent();

        if (mtype >= MAX_MAGIC || mtype < 0)
                return; /* Invalid Magic type */

        if (IS_NPC(ch))
                return;

        if (max_skl <= ch->magic_res[mtype])
                return;

        if (ch->magic_res[mtype] == 1)
                sprintf(bufskill, "slightly resistant");
        else if (ch->magic_res[mtype] == 50)
                sprintf(bufskill, "reasonably resistant");
        else if (ch->magic_res[mtype] == 100)
                sprintf(bufskill, "fairly resistant");
        else if (ch->magic_res[mtype] == 150)
                sprintf(bufskill, "highly resistant");
        else if (ch->magic_res[mtype] == 175)
                sprintf(bufskill, "very resistant");
        else if (ch->magic_res[mtype] == 200)
                sprintf(bufskill, "extremely resistant");
        else if (ch->magic_res[mtype] == 251)
                sprintf(bufskill, "slightly immune");
        else if (ch->magic_res[mtype] == 300)
                sprintf(bufskill, "reasonably immune");
        else if (ch->magic_res[mtype] == 350)
                sprintf(bufskill, "fairly immune");
        else if (ch->magic_res[mtype] == 400)
                sprintf(bufskill, "highly immune");
        else if (ch->magic_res[mtype] == 500)
                sprintf(bufskill, "virtually immune");
        else
                return;


        if (dice1 > 89 || dice2 > 89)
        {
                ch->magic_res[mtype] += 1;
                sprintf(buf,
                        "#mYou are now #7%s #magainst #7%s #mattacks.#n\n\r#n",
                        bufskill, magic_types[mtype]);
                send_to_char(buf, ch);
                return;
        }
        return;
}
