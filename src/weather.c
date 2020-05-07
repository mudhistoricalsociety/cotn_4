/*--------------------------------------------------------------------------
              .88b  d88. db    db d8888b.   .d888b. db   dD
              88'YbdP`88 88    88 88  `8D   VP  `8D 88 ,8P'
              88  88  88 88    88 88   88      odD' 88,8P
              88  88  88 88    88 88   88    .88'   88`8b
              88  88  88 88b  d88 88  .8D   j88.    88 `88.
              YP  YP  YP ~Y8888P' Y8888D'   888888D YP   YD
This material is copyrighted (c) Thomas J Whiting (wolf@wolfstream.net). 
Usage of this material  means that you have read and agree to all of the 
licenses in the ../licenses directory. None of these licenses may ever be 
removed. In addition, these headers may never be removed.
----------------------------------------------------------------------------
A LOT of time has gone into this code by a LOT of people. Not just on
this individual code, but on all of the codebases this even takes a piece
of. I hope that you find this code in some way useful and you decide to
contribute a small bit to it. There's still a lot of work yet to do.
---------------------------------------------------------------------------*/
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <sys/signal.h>
#include <sys/resource.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h> /* OLC -- for close read write etc */
#include <stdarg.h> /* printf_to_char */
#include <signal.h>
#include "merc.h"
#include "recycle.h"
#include "interp.h"
#include "olc.h"

extern char *target_name;

/*  
 * Update the time.
 */

int       percent;
void time_update(void)
{
        char      buf[MSL];
        DESCRIPTOR_DATA *d;
        bool      char_up;
        CHAR_DATA *ch = NULL;

        buf[0] = '\0';

        switch (++time_info.hour)
        {
        case 5:
                weather_info.sunlight = SUN_LIGHT;
                strcat(buf, "The #Yday#n has begun.\n\r");
                break;

        case 6:
                weather_info.sunlight = SUN_RISE;
                strcat(buf, "The #Ysun#n rises in the east.\n\r");
                break;

        case 19:
                weather_info.sunlight = SUN_SET;
                strcat(buf, "The #Ysun#n slowly disappears in the west.\n\r");
                break;

        case 20:
                weather_info.sunlight = SUN_DARK;
                strcat(buf, "The #0night#n has begun.\n\r");
                break;

        case 24:
                time_info.hour = 0;
                time_info.day++;
                for (d = descriptor_list; d != NULL; d = d->next)
                {
                        char_up = FALSE;
                        if ((d->connected == CON_PLAYING
                             || d->connected == CON_EDITING)
                            && (ch = d->character) != NULL && !IS_NPC(ch))
                        {
                                if (ch->fighting == NULL
                                    && !IS_SET(ch->newbits, NEW_NATURAL)
                                    && ch->monkab[SPIRIT] >= 2)
                                        SET_BIT(ch->newbits, NEW_NATURAL);

                                send_to_char("You hear a clock in the distance strike midnight.\n\r", ch);
                                if (IS_EXTRA(ch, EXTRA_ROT))
                                {
                                        send_to_char
                                                ("Your flesh feels better.\n\r",
                                                 ch);
                                        REMOVE_BIT(ch->extra, EXTRA_ROT);

                                }
                                if (IS_SET
                                    (ch->in_room->room_flags, ROOM_SILENCE))
                                {
                                        send_to_char
                                                ("The silence leaves the room.\n\r",
                                                 ch);
                                        act("The silence leaves the room.",
                                            ch, NULL, NULL, TO_ROOM);
                                        REMOVE_BIT(ch->in_room->room_flags,
                                                   ROOM_SILENCE);

                                }

                                if (IS_SET(ch->in_room->room_flags, ROOM_FLAMING))
                                {
                                        send_to_char("The flames in the room die down.\n\r", ch);
                                        act("The flames in the room die down.", ch, NULL, NULL, TO_ROOM);
                                        REMOVE_BIT(ch->in_room->room_flags, ROOM_FLAMING);
                                }
                                if (IS_CLASS(ch, CLASS_VAMPIRE))
                                {
                                        if (ch->hit < ch->max_hit)
                                        {
                                                ch->hit = ch->max_hit;
                                                char_up = TRUE;
                                        }
                                        if (ch->mana < ch->max_mana)
                                        {
                                                ch->mana = ch->max_mana;
                                                char_up = TRUE;
                                        }
                                        if (ch->move < ch->max_move)
                                        {
                                                ch->move = ch->max_move;
                                                char_up = TRUE;
                                        }
                                        if (char_up)
                                                send_to_char
                                                        ("You feel the strength of the kindred flow through your veins!\n\r",
                                                         ch);

                                        if (IS_SET(ch->newbits, NEW_TIDE))
                                        {
                                                REMOVE_BIT(ch->newbits,
                                                           NEW_TIDE);
                                                send_to_char
                                                        ("The tide of vitae leaves you.\n\r",
                                                         ch);
                                        }

                                        if (IS_SET(ch->extra, EXTRA_BAAL))
                                        {
                                                send_to_char
                                                        ("The spirit of Baal escapes you.\n\r",
                                                         ch);
                                                ch->power[DISC_VAMP_POTE] -=
                                                        2;
                                                ch->power[DISC_VAMP_CELE] -=
                                                        2;
                                                ch->power[DISC_VAMP_FORT] -=
                                                        2;
                                                REMOVE_BIT(ch->extra,
                                                           EXTRA_BAAL);
                                        }

                                        if (IS_EXTRA(ch, EXTRA_FLASH))
                                        {
                                                send_to_char
                                                        ("Your speed slows.\n\r",
                                                         ch);
                                                ch->power[DISC_VAMP_CELE] -=
                                                        2;
                                                REMOVE_BIT(ch->extra,
                                                           EXTRA_FLASH);
                                        }

                                }
                        }


                }
                break;

        }

        if (time_info.day >= 35)
        {
                time_info.day = 0;
                time_info.month++;
        }

        if (time_info.month >= 12)
        {
                time_info.month = 0;
                time_info.year++;
        }

        if (buf[0] != '\0')
        {
                for (d = descriptor_list; d != NULL; d = d->next)
                {
                        if (d->connected == CON_PLAYING
                            && IS_OUTDOORS(d->character)
                            && IS_AWAKE(d->character))
                                send_to_char(buf, d->character);
                }
        }
}
void desert_update(AREA_DATA * pArea)
{
        switch (pArea->sky)
        {
        default:
                pArea->sky = SKY_SUNNY;
                break;
        case SKY_SUNNY:
                percent = number_range(0, 100);
                if (percent <= 10)
                        pArea->sky = SKY_CLOUDLESS;
                else if (percent <= 50)
                        pArea->sky = SKY_SUNNY;
                else if (percent <= 75)
                        pArea->sky = SKY_TORNADO;
                else if (percent <= 100)
                        pArea->sky = SKY_HEAT;
                break;
        case SKY_CLOUDLESS:
                percent = number_range(0, 100);
                if (percent <= 15)
                        pArea->sky = SKY_SUNNY;
                else if (percent <= 30)
                        pArea->sky = SKY_CLOUDLESS;
                else if (percent <= 60)
                        pArea->sky = SKY_TORNADO;
                else if (percent <= 100)
                        pArea->sky = SKY_HEAT;
                break;
        case SKY_TORNADO:
                percent = number_range(0, 100);
                if (percent <= 5)
                        pArea->sky = SKY_SUNNY;
                else if (percent <= 15)
                        pArea->sky = SKY_CLOUDLESS;
                else if (percent <= 40)
                        pArea->sky = SKY_TORNADO;
                else if (percent <= 100)
                        pArea->sky = SKY_HEAT;
                break;
        case SKY_HEAT:
                percent = number_range(0, 100);
                if (percent <= 15)
                        pArea->sky = SKY_SUNNY;
                else if (percent <= 30)
                        pArea->sky = SKY_CLOUDLESS;
                else if (percent <= 60)
                        pArea->sky = SKY_TORNADO;
                else if (percent <= 100)
                        pArea->sky = SKY_HEAT;
                break;
        }
}
void swamp_update(AREA_DATA * pArea)
{
        switch (pArea->sky)
        {
        default:
                pArea->sky = SKY_CLOUDY;
                break;
        case SKY_CLOUDY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_THUNDERSTORM;
                }

                else if (percent <= 80)
                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                break;
        case SKY_RAINING:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_THUNDERSTORM;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                else
                {
                        if (percent <= 100)
                                pArea->sky = SKY_FOGGY;
                }
                break;
        case SKY_THUNDERSTORM:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_THUNDERSTORM;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                break;
        case SKY_LIGHTNING:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_THUNDERSTORM;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                break;
        case SKY_FOGGY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                if (percent <= 85)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_HAILSTORM;
                }

                break;
        case SKY_HAILSTORM:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_RAINING;
                }
                if (percent <= 85)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_LIGHTNING;
                }

                break;
        }

        return;
}

void forest_update(AREA_DATA * pArea)
{
        switch (pArea->sky)
        {
        default:
                pArea->sky = SKY_CLOUDY;
                break;
        case SKY_CLOUDY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_THUNDERSTORM;
                }

                else if (percent <= 80)
                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                break;
        case SKY_RAINING:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_THUNDERSTORM;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                else
                {
                        if (percent <= 100)
                                pArea->sky = SKY_FOGGY;
                }
                break;
        case SKY_THUNDERSTORM:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_THUNDERSTORM;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                break;
        case SKY_LIGHTNING:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_THUNDERSTORM;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                break;
        case SKY_FOGGY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                if (percent <= 85)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_HAILSTORM;
                }

                break;
        case SKY_HAILSTORM:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_RAINING;
                }
                if (percent <= 85)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_LIGHTNING;
                }

                break;
        case SKY_CLOUDLESS:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_RAINING;
                }
                break;
        case SKY_SUNNY:
                percent = number_range(0, 100);
                if (percent <= 45)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 70)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_CLOUDY;

                }
                break;


        }

        return;
}

void city_update(AREA_DATA * pArea)
{
/*cities = not a lot of bad weather, but it can do a little damage*/
        switch (pArea->sky)
        {
        default:
                pArea->sky = SKY_SUNNY;
                break;
        case SKY_SUNNY:
                percent = number_range(0, 100);
                if (percent <= 45)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 80)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_RAINING;
                }

                break;
        case SKY_FOGGY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_SUNNY;
                }

                break;
        case SKY_CLOUDLESS:
                percent = number_range(0, 100);
                if (percent <= 45)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_RAINING;
                }
                break;
        case SKY_CLOUDY:
                percent = number_range(0, 100);
                if (percent <= 40)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 55)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 70)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 96)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_SNOWING;
                }

                break;

        case SKY_RAINING:
                percent = number_range(0, 100);
                if (percent <= 45)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_SNOWING;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 95)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_WINDY;
                }
                break;
        case SKY_LIGHTNING:
                percent = number_range(0, 100);
                if (percent <= 45)
                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 80)
                {
                        pArea->sky = SKY_THUNDERSTORM;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                break;

        case SKY_THUNDERSTORM:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_THUNDERSTORM;
                }
                else if (percent <= 60)

                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_WINDY;
                }

                break;
        case SKY_WINDY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_RAINING;
                }

                break;

        }
        return;
}

void hills_update(AREA_DATA * pArea)
{
        switch (pArea->sky)
        {
        default:
                pArea->sky = SKY_CLOUDY;
                break;
        case SKY_SUNNY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 80)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_RAINING;
                }

                break;
        case SKY_FOGGY:
                percent = number_range(0, 100);
                if (percent <= 45)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 80)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 95)
                {
                        pArea->sky = SKY_SNOWING;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_SUNNY;
                }

                break;
        case SKY_CLOUDLESS:
                percent = number_range(0, 100);
                if (percent <= 45)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_FOGGY;
                }
                break;
        case SKY_CLOUDY:
                percent = number_range(0, 100);
                if (percent <= 40)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 55)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 70)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_SNOWING;
                }

                break;

        case SKY_RAINING:
                percent = number_range(0, 100);
                if (percent <= 45)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_SNOWING;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 95)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_WINDY;
                }
                break;
        case SKY_LIGHTNING:
                percent = number_range(0, 100);
                if (percent <= 45)
                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 80)
                {
                        pArea->sky = SKY_THUNDERSTORM;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                break;

        case SKY_THUNDERSTORM:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_THUNDERSTORM;
                }
                else if (percent <= 60)

                {
                        pArea->sky = SKY_LIGHTNING;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_WINDY;
                }

                break;
        case SKY_WINDY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_RAINING;
                }

                break;

        }
        return;
}

void mountain_update(AREA_DATA * pArea)
{
        /*
         * Mountains... Windy, Snowy, Sunny, brrrrrrrr it's cold
         */
        switch (pArea->sky)
        {
        default:
                pArea->sky = SKY_SNOWING;
                break;
        case SKY_SNOWING:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_SNOWING;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 55)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 70)
                {
                        pArea->sky = SKY_BLIZZARD;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_FREEZING;
                }


                break;
        case SKY_FREEZING:
                percent = number_range(0, 100);
                if (percent <= 45)
                {
                        pArea->sky = SKY_FREEZING;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_BLIZZARD;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_SNOWING;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_ICESTORM;
                }

        case SKY_RAINING:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_RAINING;
                }
                if (percent <= 55)
                {
                        pArea->sky = SKY_SNOWING;
                }
                if (percent <= 70)
                {
                        pArea->sky = SKY_ICESTORM;
                }
                if (percent <= 80)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                if (percent <= 90)
                {
                        pArea->sky = SKY_FOGGY;
                }
                if (percent <= 100)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }

                break;
        case SKY_BLIZZARD:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_BLIZZARD;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_SNOWING;
                }
                else if (percent <= 70)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_ICESTORM;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_FREEZING;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                break;

        case SKY_FOGGY:
                percent = number_range(0, 100);
                if (percent <= 15)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 40)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 80)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_SNOWING;
                }
                break;
        case SKY_HAILSTORM:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_ICESTORM;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_FREEZING;
                }
                break;



        case SKY_ICESTORM:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_ICESTORM;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_HAILSTORM;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_WINDY;
                }
                break;

        case SKY_CLOUDY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_SNOWING;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_RAINING;
                }
                break;

        case SKY_SUNNY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 70)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_SNOWING;
                }
                break;
        case SKY_WINDY:
                percent = number_range(0, 100);

                if (percent <= 35)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 55)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 70)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 85)
                {
                        pArea->sky = SKY_SNOWING;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_FREEZING;
                }


                break;
        }
        return;

}


void underwater_update(AREA_DATA * pArea)
{

        switch (pArea->sky)
        {
        default:
                pArea->sky = SKY_CLOUDLESS;
                break;
        case SKY_CLOUDLESS:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_FREEZING;
                }
                break;
        case SKY_CLOUDY:
                percent = number_range(0, 100);
                if (percent <= 50)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_FREEZING;
                }
                break;

        case SKY_SUNNY:
                percent = number_range(0, 100);
                if (percent <= 50)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                break;
        case SKY_FREEZING:
                percent = number_range(0, 100);
                if (percent <= 40)
                {
                        pArea->sky = SKY_FREEZING;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 80)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_SUNNY;
                }
                break;


        }
        return;
}

void air_update(AREA_DATA * pArea)
{
        switch (pArea->sky)
        {
        default:
                pArea->sky = SKY_CLOUDLESS;
                break;
        case SKY_CLOUDLESS:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 65)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 80)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_FOGGY;
                }
                break;
        case SKY_WINDY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                break;

        case SKY_RAINING:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_SUNNY;
                }
                break;

        case SKY_FOGGY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_FOGGY;
                }
                else if (percent <= 60)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 75)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_SUNNY;
                }
                break;

        case SKY_SUNNY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 70)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 80)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_FOGGY;
                }
                break;

        case SKY_CLOUDY:
                percent = number_range(0, 100);
                if (percent <= 35)
                {
                        pArea->sky = SKY_CLOUDY;
                }
                else if (percent <= 50)
                {
                        pArea->sky = SKY_CLOUDLESS;
                }
                else if (percent <= 65)
                {
                        pArea->sky = SKY_SUNNY;
                }
                else if (percent <= 80)
                {
                        pArea->sky = SKY_RAINING;
                }
                else if (percent <= 90)
                {
                        pArea->sky = SKY_WINDY;
                }
                else if (percent <= 100)
                {
                        pArea->sky = SKY_FOGGY;
                }
                break;


        }
        return;
}

/*this is what adds the affects to various weather paterns. Uncomment the weather_affects line (or
 don't add it) in db.c if you don't want weather affects. They've been rather tamed down for v4 */
void weather_affects(AREA_DATA * pArea)
{
        switch (pArea->sky)
        {
        default:
                break;
        case SKY_FREEZING:
                freeze(pArea);
                break;
        case SKY_HEAT:
                heat(pArea);
                break;
        case SKY_ICESTORM:
                ice(pArea);
                break;
        case SKY_BLIZZARD:
                blizzard(pArea);
                break;
        case SKY_LIGHTNING:
        case SKY_THUNDERSTORM:
                lightning(pArea);
                break;
        case SKY_EARTHQUAKE:
                earthquake(pArea);
                break;
        case SKY_FOGGY:
                fog(pArea);
                break;
        case SKY_AVALANCHE:
                avalanche(pArea);
                break;
        }
        return;

}



/*
New weather command. Reads the new weather stats as well as  tells you
what time of day it is (morning, noon, night) 
Added so that players don't HAVE to have autoweather enabled*/
void do_weather(CHAR_DATA * ch, char *argument)
{
        char      buf[MSL];
        char      buf2[MSL];
        char     *suf;
        int       day;

        day = time_info.day + 1;

        if (day > 4 && day < 20)
                suf = "th";
        else if (day % 10 == 1)
                suf = "st";
        else if (day % 10 == 2)
                suf = "nd";
        else if (day % 10 == 3)
                suf = "rd";
        else
                suf = "th";


        if (!IS_OUTDOORS(ch))
        {
                send_to_char("You can't see the weather indoors.\n\r", ch);
                return;
        }
        sprintf(buf2,
                "#B[#W Time  #B] #RIt is #C%d#R o'clock #Y%s#n\n\r",
                (time_info.hour % 12 == 0) ? 12 : time_info.hour % 12,
                time_info.hour >= 12 ? "pm" : "am");
        send_to_char(buf2, ch);
        if (!ch->in_room->area->sky)
        {
                send_to_char
                        ("#g[#YWeather#g] #CUnknown Weather Type!! Please Alert The Immortals\n\r",
                         ch);
                return;
        }

        if (ch->in_room->area->sky == SKY_RAINING)
        {
                if (number_range(0, 100) <= 15)
                {
                        send_to_char
                                ("#g[#YWeather#g] #CThere is a cold rain trickling down.#n\n\r",
                                 ch);
                }
                else if (number_range(0, 100) <= 30)
                {
                        send_to_char
                                ("#g[#YWeather#g] #CThere is a warm rain trickling down.#n\n\r",
                                 ch);
                }
                else if (number_range(0, 100) <= 45)
                {
                        send_to_char
                                ("#g[#YWeather#g] #CBig Huge #gFrogs#C are dropping from the sky like raindrops\n\r",
                                 ch);
                }

                else if (number_range(0, 100) <= 60)
                {
                        send_to_char
                                ("#g[#YWeather#g] #CBig Huge #YDragons#C are falling from the sky.\n\r",
                                 ch);
                }
                else if (number_range(0, 100) <= 75)
                {
                        send_to_char
                                ("#g[#YWeather#g] #CBig Huge #BPuppies#C are falling out of the sky like raindrops.\n\r",
                                 ch);
                }
                else if (number_range(0, 100) <= 85)
                {
                        send_to_char
                                ("#g[#YWeather#g] #YKitten#C Sized animals appear to be falling left and right. Bonzai!!!!!.\n\r",
                                 ch);
                }
                else if (number_range(0, 100) <= 100)
                {
                        send_to_char
                                ("#g[#YWeather#g] #CBig Huge #RRats#C are falling out of the sky like raindrops.\n\r",
                                 ch);
                }



        }

        else if (ch->in_room->area->sky == SKY_CLOUDY)
        {
                sprintf(buf, "#g[#YWeather#g] %s#n.\n\r",
                        weather_info.change >= 0
                        ? "#CA warm breeze can be felt about"
                        : "#CA cold breeze can be felt about");
                send_to_char(buf, ch);
        }
        else
        {
                sprintf(buf, "#g[#YWeather#g]#C %s\n\r",
                        flag_string(weather_flags, ch->in_room->area->sky));
                stc(buf, ch);
        }


        return;

}

/*  Xantha  10/00 */



bool IS_OUTDOORS(CHAR_DATA * ch)
{
        if (IS_SET(ch->in_room->room_flags, ROOM_INDOORS))
        {
                return FALSE;
        }
        if (ch->in_room->sector_type == SECT_INSIDE)
        {
                return FALSE;
        }
        if (ch->in_room->area->sector == AREA_INDOORS)
        {
                return FALSE;
        }
        if (ch->in_room->area->sector == AREA_CAVERN)
        {
                return FALSE;
        }

        else
                return TRUE;
}

void do_autoweather(CHAR_DATA * ch, char *argument)
{
        if (IS_NPC(ch))
                return;

        if (IS_SET(ch->act2, PLR_AUTOWEATHER))
        {
                send_to_char
                        ("You will no longer see weather descriptions in rooms\n\r",
                         ch);
                REMOVE_BIT(ch->act2, PLR_AUTOWEATHER);
        }
        else
        {
                send_to_char
                        ("You will now see weather descriptions in rooms\n\r",
                         ch);
                SET_BIT(ch->act2, PLR_AUTOWEATHER);
        }
}

void show_weather(CHAR_DATA * ch)
{
        char      buf[MSL];

        if (!ch->in_room->area->sky)
        {
                send_to_char
                        ("#g[#YWeather#g] #CUnknown Weather Type!! Please Alert The Immortals\n\r",
                         ch);
                return;
        }

        if (ch->in_room->area->sky == SKY_RAINING)
        {
                if (number_range(0, 100) <= 15)
                {
                        send_to_char
                                ("#g[#YWeather#g] #CThere is a cold rain trickling down.\n\r#n",
                                 ch);
                }
                else if (number_range(0, 100) <= 30)
                {
                        send_to_char
                                ("#g[#YWeather#g] #CThere is a warm rain trickling down.\n\r#n",
                                 ch);
                }
                else if (number_range(0, 100) <= 45)
                {
                        send_to_char
                                ("#g[#YWeather#g] #CBig Huge #gFrogs#C are dropping from the sky like raindrops\n\r",
                                 ch);
                }

                else if (number_range(0, 100) <= 60)
                {
                        send_to_char
                                ("#g[#YWeather#g] #CBig Huge #YDragons#C are falling from the sky.\n\r",
                                 ch);
                }
                else if (number_range(0, 100) <= 75)
                {
                        send_to_char
                                ("#g[#YWeather#g] #CBig Huge #BPuppies#C are falling out of the sky like raindrops.\n\r",
                                 ch);
                }
                else if (number_range(0, 100) <= 85)
                {
                        send_to_char
                                ("#g[#YWeather#g] #YKitten#C Sized animals appear to be falling left and right. Bonzai!!!!!.\n\r",
                                 ch);
                }
                else if (number_range(0, 100) <= 100)
                {
                        send_to_char
                                ("#g[#YWeather#g] #CBig Huge #RRats#C are falling out of the sky like raindrops.\n\r",
                                 ch);
                }



        }

        else if (ch->in_room->area->sky == SKY_CLOUDY)
        {
                sprintf(buf, "#g[#YWeather#g] %s#n.\n\r",
                        weather_info.change >= 0
                        ? "#CA warm breeze can be felt about"
                        : "#CA cold breeze can be felt about");
                send_to_char(buf, ch);
        }
        else
        {
                sprintf(buf, "#g[#YWeather#g]#C %s\n\r",
                        flag_string(weather_flags, ch->in_room->area->sky));
                stc(buf, ch);
        }
}







void do_wset(CHAR_DATA * ch, char *argument)
{
        char      arg1[MIL];

        argument = one_argument(argument, arg1);
        if (arg1[0] == '\0')
        {
                valid_weather(ch);
                return;
        }
        if (!str_cmp(arg1, "cloudless"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_CLOUDLESS))
                {
                        valid_weather(ch);
                        return;
                }

                send_to_char
                        ("You wave your hands and in reverence to you, the clouds dissapear \n\r",
                         ch);
                ch->in_room->area->sky = SKY_CLOUDLESS;
                act("$n makes a strange movement with $M hands and the clouds part.", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "heat"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_HEAT))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, the sun appears full in the sky \n\r",
                         ch);
                ch->in_room->area->sky = SKY_HEAT;
                act("$n makes a strange movement with $M hands and the sun appears full in the sky.", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "freezing"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_FREEZING))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, the temperature drops severely \n\r",
                         ch);
                ch->in_room->area->sky = SKY_FREEZING;
                act("$n makes a strange movement with $M hands and the temperature drops severely.", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "windy"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_WINDY))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, the wind picks up from the south \n\r",
                         ch);
                ch->in_room->area->sky = SKY_WINDY;
                act("$n makes a strange movement with $M hands and the wind starts to pick up.", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "hurricane"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_HURRICANE))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, the calm dissapears \n\r",
                         ch);
                ch->in_room->area->sky = SKY_HURRICANE;
                act("$n makes a strange movement with $M hands and the calm dissapears.", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "earthquake"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_EARTHQUAKE))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, the earth shakes\n\r",
                         ch);
                ch->in_room->area->sky = SKY_SUNNY;
                act("$n makes a strange movement with $M hands and the earth shakes.", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "tornado"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_TORNADO))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, a tornado lifts off the ground \n\r",
                         ch);
                ch->in_room->area->sky = SKY_TORNADO;
                act("$n makes a strange movement with $M hands and a tornado lifts off the ground.", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "sunny"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_SUNNY))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, the sun makes an appearance \n\r",
                         ch);
                ch->in_room->area->sky = SKY_SUNNY;
                act("$n makes a strange movement with $M hands and the sun makes an appearance.", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "foggy"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_FOGGY))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, a mist vapors the horizon \n\r",
                         ch);
                ch->in_room->area->sky = SKY_FOGGY;
                act("$n makes a strange movement with $M hands and a mist vapors the horizon.", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "hail"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_HAILSTORM))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, hailstones fall from the sky \n\r",
                         ch);
                ch->in_room->area->sky = SKY_HAILSTORM;
                act("$n makes a strange movement with $M hands and hailstones fall from the sky.", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "icestorm"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_ICESTORM))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, it starts raining ice \n\r",
                         ch);
                ch->in_room->area->sky = SKY_ICESTORM;
                act("$n makes a strange movement with $M hands and it starts raining ice.", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "blizzard"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_BLIZZARD))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, snowflakes cover the horizon making it impossible to see.\n\r",
                         ch);
                ch->in_room->area->sky = SKY_BLIZZARD;
                act("$n makes a strange movement with $M hands and snowflakes cover the horizon making it impossibile to see", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "snowing"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_SNOWING))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, snowflakes  fall from the sky..\n\r",
                         ch);
                ch->in_room->area->sky = SKY_SNOWING;
                act("$n makes a strange movement with $M hands and snowflakes fall from the sky", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "raining"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_RAINING))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, a warm rain starts to fall.\n\r",
                         ch);
                ch->in_room->area->sky = SKY_RAINING;
                act("$n makes a strange movement with $M hands and a warm rain starts to fall", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "lightning"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_LIGHTNING))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, lightning pierces the sky. \n\r",
                         ch);
                ch->in_room->area->sky = SKY_LIGHTNING;
                act("$n makes a strange movement with $M hands and lightning pierces  the sky", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "thunderstorm"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_THUNDERSTORM))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, The clouds clap in thunder.\n\r",
                         ch);
                ch->in_room->area->sky = SKY_THUNDERSTORM;
                act("$n makes a strange movement with $M hands and the clouds clap in thunder", ch, NULL, NULL, TO_ROOM);
        }
        else if (!str_cmp(arg1, "cloudy"))
        {
                if (!IS_SET(ch->in_room->area->a_weather, SKY_CLOUDY))
                {
                        valid_weather(ch);
                        return;
                }
                send_to_char
                        ("You wave your hands and in reverence to you, clouds cover the horizon, threatening rain.\n\r",
                         ch);
                ch->in_room->area->sky = SKY_CLOUDY;
                act("$n makes a strange movement with $M hands and clouds cover the horizon, threatening rain", ch, NULL, NULL, TO_ROOM);
        }
/* okay, we haven't met any conditions so far. Did they forget? */
        else
        {
                valid_weather(ch);
                return;
        }

}
void valid_weather(CHAR_DATA * ch)
{
        char      buf[MSL];

        send_to_char("Syntax:\n\r", ch);
        send_to_char("  set weather <condition> \n\r", ch);
        send_to_char("  Condition can be : ", ch);
        sprintf(buf, "%s\n\r",
                flag_string(aweather_flags, ch->in_room->area->a_weather));
        stc(buf, ch);
        return;
        return;
}

/*

Weather Tables. Some would put these in tables.c, however you can put them where you will.
The thought and idea is to keep all the weather related stuff here.
*/


/* misc weather "baddies" here, like lightning, blizzard, earthquake, etc*/
void blizzard(AREA_DATA * pArea)
{
        DESCRIPTOR_DATA *d;

        for (d = descriptor_list; d != NULL; d = d->next)
        {
                if (d->connected == CON_PLAYING
                    && IS_AWAKE(d->character)
                    && d->character->in_room
                    && d->character->in_room->area == pArea
                    && !IS_IMMORTAL(d->character)
                    && (number_percent() < 10) && pArea->sky == SKY_BLIZZARD)
                {
                        send_to_char
                                ("#n#RYou suddenly find yourself engulfed in a blizzard, surrounded by snow #n\n\r",
                                 d->character);
                        act("$n has been enveloped in snow!", d->character,
                            NULL, NULL, TO_ROOM);
                        if (!IS_IMMUNE(d->character, IMM_COLD))
                        {
                                if (d->character->fighting)
                                        stop_fighting(d->character, TRUE);
                                d->character->hit -= d->character->hit / 30;
                        }
                        else
                        {

                                d->character->hit -= d->character->hit / 10;
                                WAIT_STATE(d->character, 40);
                        }
                }
        }
}
void avalanche(AREA_DATA * pArea)
{
        DESCRIPTOR_DATA *d;

        for (d = descriptor_list; d != NULL; d = d->next)
        {
                if (d->connected == CON_PLAYING
                    && IS_AWAKE(d->character)
                    && d->character->in_room
                    && d->character->in_room->area == pArea
                    && !IS_IMMORTAL(d->character)
                    && (number_percent() < 5) && pArea->sky == SKY_AVALANCHE)
                {
                        send_to_char
                                ("#R Yikes!! The mountain around you caves in, pulling you within it#n\n\r",
                                 d->character);
                        act("$n has become pulled into the oncoming avalanche. Better get help!", d->character, NULL, NULL, TO_ROOM);
                        if (!IS_IMMUNE(d->character, IMM_COLD))
                        {
                                if (d->character->fighting)
                                        stop_fighting(d->character, TRUE);
                                d->character->move -= d->character->move / 30;
                                d->character->hit -= d->character->hit / 30;
                                WAIT_STATE(d->character, 40);
                        }
                }
        }
}
void fog(AREA_DATA * pArea)
{
        DESCRIPTOR_DATA *d;

        for (d = descriptor_list; d != NULL; d = d->next)
        {
                if (d->connected == CON_PLAYING
                    && IS_AWAKE(d->character)
                    && d->character->in_room
                    && d->character->in_room->area == pArea
                    && !IS_IMMORTAL(d->character)
                    && (number_percent() < 8) && pArea->sky == SKY_FOGGY)
                {
                        send_to_char
                                ("#n#RYou trip and fall over your own laces not being able to see where you're headed #n\n\r",
                                 d->character);
                        act("$n slips and trips over $m own shoelaces!",
                            d->character, NULL, NULL, TO_ROOM);

                        if (d->character->fighting)
                                stop_fighting(d->character, TRUE);

                        d->character->move -= d->character->move / 20;


                }
        }
}
void ice(AREA_DATA * pArea)
{
        DESCRIPTOR_DATA *d;

        for (d = descriptor_list; d != NULL; d = d->next)
        {
                if (d->connected == CON_PLAYING
                    && IS_AWAKE(d->character)
                    && d->character->in_room
                    && d->character->in_room->area == pArea
                    && !IS_IMMORTAL(d->character)
                    && (number_percent() < 8) && pArea->sky == SKY_ICESTORM)
                {
                        send_to_char
                                ("#n#RLarge sheets of ice fall from the heavens, pelting you #n\n\r",
                                 d->character);
                        act("$n has been pelted by ice!", d->character, NULL,
                            NULL, TO_ROOM);

                        if (d->character->fighting)
                                stop_fighting(d->character, TRUE);
                        d->character->hit -= d->character->hit / 35;

                }
        }

}
void heat(AREA_DATA * pArea)
{
        DESCRIPTOR_DATA *d;

        for (d = descriptor_list; d != NULL; d = d->next)
        {
                if (d->connected == CON_PLAYING
                    && IS_AWAKE(d->character)
                    && d->character->in_room
                    && d->character->in_room->area == pArea
                    && !IS_IMMORTAL(d->character)
                    && (number_percent() < 10) && pArea->sky == SKY_HEAT)
                {
                        send_to_char
                                ("#n#RYou try to take a step, but pass out from heat exhaustion instead#n\n\r",
                                 d->character);
                        act("$n passes out in front of you from heat exhaustion!", d->character, NULL, NULL, TO_ROOM);
                        if (!IS_IMMUNE(d->character, IMM_HEAT))
                        {
                                if (d->character->fighting)
                                        stop_fighting(d->character, TRUE);
                                d->character->move -= d->character->hit / 30;

                        }
                }
        }
}
void earthquake(AREA_DATA * pArea)
{
        DESCRIPTOR_DATA *d;

        for (d = descriptor_list; d != NULL; d = d->next)
        {
                if (d->connected == CON_PLAYING
                    && IS_AWAKE(d->character)
                    && d->character->in_room
                    && d->character->in_room->area == pArea
                    && !IS_IMMORTAL(d->character)
                    && (number_percent() < 10)
                    && pArea->sky == SKY_EARTHQUAKE)
                {
                        send_to_char
                                ("#n#RYou suddenly find yourself swallowed up by the ground #n\n\r",
                                 d->character);
                        act("$n has been swalloed up by the earth!",
                            d->character, NULL, NULL, TO_ROOM);
                        if (d->character->fighting)
                                stop_fighting(d->character, TRUE);
                        d->character->hit -= d->character->hit / 30;

                }
        }
}

void lightning(AREA_DATA * pArea)
{
        DESCRIPTOR_DATA *d;

        for (d = descriptor_list; d != NULL; d = d->next)
        {
                if (d->connected == CON_PLAYING
                    && IS_AWAKE(d->character)
                    && d->character->in_room
                    && d->character->in_room->area == pArea
                    && !IS_IMMORTAL(d->character)
                    && (number_percent() < 15) && pArea->sky == SKY_LIGHTNING)
                {
                        send_to_char
                                ("#n#RYou see a brilliant flash come down from the sky and then black out!#n\n\r",
                                 d->character);
                        act("$n has been struck by lightning!", d->character,
                            NULL, NULL, TO_ROOM);
                        if (d->character->fighting)
                                stop_fighting(d->character, TRUE);
                        d->character->hit -= d->character->hit / 10;
                        WAIT_STATE(d->character, 40);
                }
        }
}
void freeze(AREA_DATA * pArea)
{
        DESCRIPTOR_DATA *d;

        for (d = descriptor_list; d != NULL; d = d->next)
        {
                if (d->connected == CON_PLAYING
                    && IS_AWAKE(d->character)
                    && d->character->in_room
                    && d->character->in_room->area == pArea
                    && !IS_IMMORTAL(d->character)
                    && (number_percent() < 6) && pArea->sky == SKY_FREEZING)
                {
                        send_to_char
                                ("#n#R You find yourself stuck to the surface, unable to move#n\n\r",
                                 d->character);
                        act("$n has been frozen to the ground!", d->character,
                            NULL, NULL, TO_ROOM);
                        if (!IS_IMMUNE(d->character, IMM_COLD))
                        {
                                if (d->character->fighting)
                                        stop_fighting(d->character, TRUE);

                                d->character->move -= d->character->move / 10;
                                WAIT_STATE(d->character, 40);
                        }
                }
        }
}
