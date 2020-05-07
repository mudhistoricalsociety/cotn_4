#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "olc.h"

void do_removeexit( CHAR_DATA *ch, char *argument )
{
  stc("You must type out 'removeexits'.\n\r",ch);
  return;
}

void do_removeexits( CHAR_DATA *ch, char *argument )
{
  ROOM_INDEX_DATA *room;
  AREA_DATA *area;
  bool found = FALSE;
  int r, e;

  if( IS_NPC(ch) ) return;
  if( ch->in_room == NULL ) return;
  if( ch->trust < MAX_LEVEL)
  {
    if (!IS_BUILDER(ch, ch->in_room->area))
    {
      send_to_char("Huh?\n\r", ch );
      return;
    }
  }

  if( str_cmp(argument,"confirm") )
  {
    stc("This command deletes all exits in this area to other areas,\n\r",ch);
    stc("as well as all exits leading into it from other areas.\n\r",ch);
    stc("To use this command, you must type 'removeexits confirm'.\n\r",ch);
    return;
  }

  for( r = ch->in_room->area->lvnum; r <= ch->in_room->area->uvnum; r++ )
  {
    if( (room = get_room_index(r)) == NULL ) continue;
    for( e = 0; e < 6; e++ )
    {
      if( room->exit[e] != NULL
       && room->exit[e]->to_room != NULL
       && room->exit[e]->to_room->area != room->area )
      {
        found = TRUE;
        free_exit( room->exit[e] );
        room->exit[e] = NULL;
      }
    }
  }

  for( area = area_first; area != NULL; area = area->next )
  {
    if( area == ch->in_room->area ) continue;
    for( r = area->lvnum; r <= area->uvnum; r++ )
    {
      if( (room = get_room_index(r)) == NULL ) continue;
      for( e = 0; e < 6; e++ )
      {
        if( room->exit[e] != NULL
         && room->exit[e]->to_room != NULL
         && room->exit[e]->to_room->area == ch->in_room->area )
        {
          found = TRUE;
          free_exit( room->exit[e] );
          room->exit[e] = NULL;
        }
      }
    }
  }

  if( found )
    stc("Area unlinked from all other areas.\n\r",ch);
  else
    stc("No exits to or from other areas found.\n\r",ch);

  return;
}

void do_enterexits( CHAR_DATA *ch, char *argument )
{
  AREA_DATA *area;
  ROOM_INDEX_DATA *room;
  char buf[MSL];
  bool found = FALSE;
  int r, i, e;

  if( IS_NPC(ch) ) return;
  if( ch->in_room == NULL ) return;

  if( argument[0] == '\0' )
    stc("Limit defaulted to 10.\n\rEnterexits <number> to specify limit.\n\r",ch);

  i = atoi(argument);
  if( i <= 0 ) i = 10;

  for( area = area_first; area != NULL; area = area->next )
  {
    if( area == ch->in_room->area ) continue;
    for( r = area->lvnum; r <= area->uvnum; r++ )
    {
      if( 0 >= i ) break;
      if( (room = get_room_index(r)) == NULL ) continue;
      for( e = 0; e < 6; e++ )
      {
        if( room->exit[e] != NULL
         && room->exit[e]->to_room != NULL
         && room->exit[e]->to_room->area == ch->in_room->area )
        {
          i--;
          found = TRUE;
          sprintf(buf,"Room %d has exit %s to room %d\n\r",
            room->vnum,
            e == 0 ? "North" : e == 1 ? "East " : e == 2 ? "South" : e == 3 ? "West " : e == 4 ? "Up   " : "Down ",
            room->exit[e]->to_room->vnum);
          send_to_char(buf,ch);
        }
      }
    }
  }

  if( !found )
    stc("No exits found leading into this area.\n\r",ch);

  return;
}

void do_outerexits( CHAR_DATA *ch, char *argument )
{
  ROOM_INDEX_DATA *room;
  char buf[MSL];
  bool found = FALSE;
  int r, i, e;

  if( IS_NPC(ch) ) return;
  if( ch->in_room == NULL ) return;

  if( argument[0] == '\0' )
    stc("Limit defaulted to 10.\n\rOuterexits <number> to specify limit.\n\r",ch);

  i = atoi(argument);
  if( i <= 0 ) i = 10;

  for( r = ch->in_room->area->lvnum; r <= ch->in_room->area->uvnum; r++ )
  {
    if( 0 >= i ) break;
    if( (room = get_room_index(r)) == NULL ) continue;
    for( e = 0; e < 6; e++ )
    {
      if( room->exit[e] != NULL
       && room->exit[e]->to_room != NULL
       && room->exit[e]->to_room->area != ch->in_room->area )
      {
        i--;
        found = TRUE;
        sprintf(buf,"Room %d has exit %s to room %d.\n\r",
          room->vnum,
          e == 0 ? "North" : e == 1 ? "East " : e == 2 ? "South" : e == 3 ? "West " : e == 4 ? "Up   " : "Down ",
          room->exit[e]->to_room->vnum);
        send_to_char(buf,ch);
      }
    }
  }

  if( !found )
    stc("No outside exits found.\n\r",ch);

  return;
}


