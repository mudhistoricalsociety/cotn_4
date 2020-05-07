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

void weather_affects args((AREA_DATA * pArea));
void freeze args((AREA_DATA * pArea));
void heat args((AREA_DATA * pArea));
void ice  args((AREA_DATA * pArea));
void lightning args((AREA_DATA * pArea));
void earthquake args((AREA_DATA * pArea));
void blizzard args((AREA_DATA * pArea));
void fog  args((AREA_DATA * pArea));
void avalanche args((AREA_DATA * pArea));
void time_update args((void));
void valid_weather args((CHAR_DATA * ch));
bool IS_OUTDOORS args((CHAR_DATA * ch));
void show_weather args((CHAR_DATA * ch));
void city_update args((AREA_DATA * pArea));
void air_update args((AREA_DATA * pArea));
void desert_update args((AREA_DATA * pArea));
void mountain_update args((AREA_DATA * pArea));
void underwater_update args((AREA_DATA * pArea));
void forest_update args((AREA_DATA * pArea));
void hills_update args((AREA_DATA * pArea));
void swamp_update args((AREA_DATA * pArea));

/*
 * Time and weather stuff.
 */
#define SUN_DARK		    0
#define SUN_RISE		    1
#define SUN_LIGHT		    2
#define SUN_SET			    3

#define SKY_CLOUDLESS		    1
#define SKY_CLOUDY		    2
#define SKY_RAINING		    4
#define SKY_LIGHTNING		    8
#define SKY_SNOWING		    16
#define SKY_BLIZZARD		    32
#define SKY_FOGGY		    64
#define SKY_HAILSTORM		    128
#define SKY_THUNDERSTORM	    256
#define SKY_ICESTORM		    512
#define SKY_SUNNY		    1024
#define SKY_TORNADO		    2056
#define SKY_EARTHQUAKE		    4096
#define SKY_HURRICANE		    8192
#define SKY_HEAT		    16384
#define SKY_WINDY		    32768
#define SKY_FREEZING		    65536
#define SKY_AVALANCHE		    131072

/*
AREA_XXX bits, for different affects per zone. This is an area flag.
*/

#define AREA_INDOORS		    1
#define AREA_DESERT		    (B)
#define AREA_SWAMP		    (C)
#define AREA_MOUNTAIN		    (D)
#define AREA_UNDERWATER		    (E)
#define AREA_CITY		    (F)
#define AREA_CAVERN		    (G)
#define AREA_FOREST		    (H)
#define AREA_HILLS		    (I)
#define AREA_AIR		    (J)
