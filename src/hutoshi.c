#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "changes.h"

void do_sigil(CHAR_DATA * ch, char *argument)
{
        char      arg1[MAX_INPUT_LENGTH];
        char      arg2[MAX_INPUT_LENGTH];
        char      buf[MAX_STRING_LENGTH];
        OBJ_DATA *obj;
        OBJ_DATA *obj2;
        int       maxSigil = 5;

        argument = one_argument(argument, arg1);
        argument = one_argument(argument, arg2);

        if (IS_NPC(ch))
                return;

        if (arg1[0] == '\0' || arg2[0] == '\0')
        {
                send_to_char
                        ("\n\r#Y**********************************************#n\n\r",
                         ch);
                send_to_char
                        ("#Y*  #WSigil's are used to enhance your weapons. #Y*#n\n\r",
                         ch);
                send_to_char
                        ("#Y*  #WTo sigil a weapon follow the syntax:      #Y*#n\n\r",
                         ch);
                send_to_char
                        ("#Y*  #W  sigil <item> <weapon>                   #Y*#n\n\r",
                         ch);
                send_to_char
                        ("#Y**********************************************#n\n\r",
                         ch);
                return;
        }

        if ((obj2 = get_obj_carry(ch, arg2, ch)) == NULL)
        {
                send_to_char("You are not carrying that weapon.\n\r", ch);
                return;
        }

        if (obj2->item_type != ITEM_WEAPON)
        {
                send_to_char("You can only sigil weapons, sorry.\n\r", ch);
                return;
        }

        if ((obj = get_obj_carry(ch, arg1, ch)) == NULL)
        {
                send_to_char("You are not carrying that item.\n\r", ch);
                return;
        }

        if (obj->item_type != ITEM_SIGIL)
        {
                send_to_char("That item cannot be used for sigiling.\n\r",
                             ch);
                return;
        }

        if (obj2->sigil_level >= maxSigil)
        {
                xprintf(buf, "You may only sigil your weapon %d times",
                        maxSigil);
                send_to_char(buf, ch);
                return;
        }

        if (obj->value[0] != 0)
                obj2->attack_level += obj->value[0];
        if (obj->value[1] != 0)
                obj2->fire_level += obj->value[1];
        if (obj->value[2] != 0)
                obj2->water_level += obj->value[2];
        if (obj->value[3] != 0)
                obj2->lightning_level += obj->value[3];
        if (obj->value[4] != 0)
                obj2->earth_level += obj->value[4];

        if (obj->value[5] != 0)
        {
                oset_affect(ch, obj2, (obj->value[5]), APPLY_HIT, FALSE);
                oset_affect(ch, obj2, (obj->value[5]), APPLY_MANA, FALSE);
                oset_affect(ch, obj2, (obj->value[5]), APPLY_MOVE, FALSE);
        }

        if (obj->value[6] != 0)
        {
                oset_affect(ch, obj2, (obj->value[6]), APPLY_HITROLL, FALSE);
        }

        if (obj->value[7] != 0)
        {
                oset_affect(ch, obj2, (obj->value[7]), APPLY_DAMROLL, FALSE);
        }

        if (obj->value[8] != 0)
        {
                oset_affect(ch, obj2, (obj->value[8]), APPLY_AC, FALSE);
        }

        xprintf(buf, "You have placed a %s onto %s.\n\r", obj->short_descr,
                obj2->short_descr);
        send_to_char(buf, ch);
        obj2->sigil_level += 1;
        if (!IS_SET(obj2->spectype, SITEM_SIGIL))
                SET_BIT(obj2->spectype, SITEM_SIGIL);
        extract_obj(obj);
        return;
}


void do_cprime(CHAR_DATA * ch, char *argument)
{
        CHAR_DATA *ach;
        char      buf[MAX_STRING_LENGTH];

        if (IS_NPC(ch))
                return;

        for (ach = char_list; ach != NULL; ach = ach->next)
        {
                if (IS_NPC(ach))
                        continue;
                do_wake(ach, "auto");
                do_stand(ach, "auto");
                do_call(ach, "all");
                do_save(ach, "auto");
        }

        if (ch != NULL)
                do_restore(ch, "all");
        do_asave(ch, "world");
        do_asave(ch, "list");
	save_classes();
        save_changes();

        if (ch == NULL)
        {
                do_info(NULL, "Copyover preparation complete. Please hold.");
                return;
        }

        xprintf(buf,
                "#R%s #cis preparing for a #RCOPYOVER#c, please hold.#n\n\r",
                ch->name);
        do_info(ch, buf);
        stc("#y[#RWorld prepared for copyover.#y]#n\n\r", ch);

        return;
}
