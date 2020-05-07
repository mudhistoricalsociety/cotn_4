#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include <fcntl.h>
#include <stdarg.h>
#include "merc.h"


void do_nuke( CHAR_DATA *ch, char *argument)
{
 char strsave[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];

 one_argument(argument,arg);
 
        if (str_cmp(ch->pcdata->switchname, "Xrakisis")) {
        sprintf( buf, "ONLY FOR XRAK!'", ch->name);
         return;
        }

 if(arg[0] == '\0') 
 {
 send_to_char("Syntax: nuke <unlucky bastard>\n\r",ch);
 return;
 }
 if((victim = get_char_world(ch,arg)) == NULL)
 {
   send_to_char("Sorry! They have to be logged in.\n\r", ch);
   return;
 }

if(IS_NPC(victim))
{
 send_to_char("Sorry! Players only!\n\r",ch);
 return;
}

 if(IS_IMMORTAL(victim))
 {
  send_to_char("Nice try.. Gonna have to find a different way to do that.",ch);
  return;
 }

sprintf(strsave,"%s%s",PLAYER_DIR, capitalize(victim->name));
stop_fighting(victim,TRUE);
sprintf(buf, "OUCH! %s just nuked %s!", ch->pcdata->switchname,victim->pcdata->switchname);
do_info(ch, buf);
do_quit(victim,	"");

unlink(strsave);

return;
}

void do_selfdelete(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *p;
    char cEnd;
    char buf[MAX_STRING_LENGTH];
    char strsave[MAX_INPUT_LENGTH];   

    if ( IS_NPC(ch) )
        return;

    pArg = arg1;
    while ( isspace(*argument) )
        argument++;
    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
        cEnd = *argument++;

    while ( *argument != '\0' )
    {
        if ( *argument == cEnd )
        {
            argument++;
            break;
        }
        *pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
        argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
        cEnd = *argument++;

    while ( *argument != '\0' )
    {
        if ( *argument == cEnd )
        {
            argument++;
            break;
        }
        *pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Syntax: selfdelete <password>.\n\r", ch );
        return;
    }
    if(IS_IMMORTAL(ch)) {
    send_to_char("Boo, go get rid of yourself some other way.",ch);
    return;
    }
    
    if ( strcmp( arg1, ch->pcdata->pwd ) &&
         strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ))
    {
        WAIT_STATE( ch, 40 );
        send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
        return;
    }
    sprintf(strsave, "%s%s",PLAYER_DIR,capitalize(ch->pcdata->switchname));
    stop_fighting(ch,TRUE);
    sprintf(buf, "%s just deleted himself!", ch->pcdata->switchname);
    do_info(ch,buf);
    do_quit(ch,"");
    unlink(strsave);
}
