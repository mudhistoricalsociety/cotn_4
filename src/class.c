/***************************************************************************
 *  This code is used to keep classes in seperate files in a class folder. *
 *  Adding/Removing/Changing values is much the same as adding in an entry *
 *  for pfiles. This code allows for online class balancing of melee stats *
 *  to ease balancing. Classes are in seperate files to prevent corruption.*
 *  © 2003 Demona                                                          *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "merc.h"
#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )    if ( !str_cmp( word, literal ) ){field  = value;fMatch = TRUE;break;}


//void fread_class();
bool create_new_class   args( (int in, char *argument ) );
int MAX_PC_CLASS;
struct  class_type *    class_table     [MAX_CLASS];

void write_class_file( int cl )
{
    FILE *fpout;
    char buf[MAX_STRING_LENGTH];
    char filename[MAX_INPUT_LENGTH];
    struct class_type *class = class_table[cl];

    xprintf( filename, "%s%s.class", CLASSDIR, class->class_nm );
    if ( (fpout=fopen(filename, "w")) == NULL )
    {
	sprintf( buf, "Cannot open: %s for writing", filename );
	bug( buf, 0 );
	return;
    }
    fprintf( fpout, "Name       %s~\n",		class->class_nm	);
    fprintf( fpout, "Class      %d\n",		cl			    );
    fprintf( fpout, "Classbit   %ld\n",     class->class_bit);
    fprintf( fpout, "Min_Damcap %d\n",		class->min_dam	);
    fprintf( fpout, "Base_AC    %d\n",		class->base_ac  );
    fprintf( fpout, "Damroll    %d\n",      class->damroll  );
    fprintf( fpout, "Hitroll    %d\n",      class->hitroll  );
    fprintf( fpout, "Speed      %d\n",		class->speed	);
    fprintf( fpout, "Dodge      %d\n",		class->dodge	);
    fprintf( fpout, "Parry      %d\n",		class->parry	);
    fprintf( fpout, "Weapons    %d\n",      class->weapons  );
    fprintf( fpout, "Spells     %d\n",      class->spells   );
    fprintf( fpout, "Hitroll    %d\n",      class->hitroll  );
    fprintf( fpout, "Damroll    %d\n",      class->damroll  );
    fprintf( fpout, "Playable   %d\n",      class->playable );
    fprintf( fpout, "End\n" );
    fclose( fpout );
}



void save_classes()
{
    int x;

    for ( x = 0; x < MAX_CLASS; x++ )
      write_class_file( x );
}


bool load_class_file( char *fname )
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    bool fMatch;
    struct class_type *class;
    int cl = -1;
    FILE *fp;

    sprintf( buf, "%s%s", CLASS_DIR, fname );
    if ( ( fp = fopen( buf, "r" ) ) == NULL )
    {
	perror( buf );
	return FALSE;
    }

    CREATE( class, struct class_type, 1 );

    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;
    case 'B':
            KEY( "Base_AC", class->base_ac, fread_number(fp));
            break;
	case 'C':
     	    KEY( "Class",	cl,		fread_number( fp )	);
            KEY( "Classbit",    class->class_bit, fread_number(fp));
	    break;
   case 'D':
            KEY("Damroll", class->damroll, fread_number(fp));
            KEY("Dodge",	class->dodge,		fread_number(fp));
            break;
	case 'E':
	        if ( !str_cmp( word, "End" ) )
     	    {
		     fclose( fp );
            if ( cl < 0 || cl >= MAX_CLASS )
     		{
		     sprintf( buf, "Load_class_file: Class (%s) bad/not found (%d)",
		 	 class->class_nm ? class->class_nm : "name not found", cl );
		     bug( buf, 0 );
		     if ( class->class_nm )
			 STRFREE( class->class_nm );
		     DISPOSE( class );
		     return FALSE;
		    }
		class_table[cl] = class;
		return TRUE;
	    }
	    break;
        case 'H':
            KEY("Hitroll", class->hitroll, fread_number(fp));
            break;
	case 'M':
            KEY("Min_Dam",	class->min_dam,		fread_number(fp));
	    break;
        case 'N':
            KEY("Name",		class->class_nm,	fread_string(fp));
            break;
        case 'P':
            KEY("Parry",	class->parry,	fread_number(fp));
            KEY("Playable",	class->playable,fread_number(fp));
            break;

	case 'S':
            KEY("Speed",	class->speed,		fread_number(fp));
            KEY("Spells",   class->spells,      fread_number(fp));
	    break;
        case 'W':
            KEY("Weapons", class->weapons, fread_number(fp));
            break;
	}
	if ( !fMatch )
	{
            sprintf( buf, "load_class_file: no match: %s", word );
	    bug( buf, 0 );
	}
   }
    return FALSE;
}

void load_classes( )
{
    FILE *fpList;
    char *filename;
    char classlist[256];
    char buf[MAX_STRING_LENGTH];
    int i;
     
//    MAX_PC_CLASS = 0;

    for ( i=0;i<MAX_CLASS;i++ )
	class_table[i] = NULL;
    	
    sprintf( classlist, "%s%s", CLASS_DIR, CLASS_LIST );
    if ( ( fpList = fopen( classlist, "r" ) ) == NULL )
    {
	perror( classlist );
	exit( 1 );
    }

    for ( ; ; )
    {
	filename = feof( fpList ) ? "$" : fread_word( fpList );
	if ( filename[0] == '$' )
	  break;

	if ( !load_class_file( filename ) )
	{
	  sprintf( buf, "Cannot load class file: %s", filename );
	  bug( buf, 0 );
	}
	else
	  MAX_PC_CLASS++;
    }
    fclose( fpList );
    for ( i=0;i<MAX_CLASS;i++ )
    {
    	if ( class_table[i] == NULL ) 
	{
	  CREATE( class_table[i], struct class_type, 1 );
    	  create_new_class( i, "" );
	}
    }
    return;
}

bool create_new_class( int in, char *argument )
{

  if ( in >= MAX_CLASS || class_table[in] == NULL )
  	return FALSE;
  if ( class_table[in]->class_nm )
  	STRFREE( class_table[in]->class_nm );
  if ( argument[0] != '\0' )
  	argument[0] = UPPER(argument[0]);
  class_table[in]->class_nm   = STRALLOC( argument );
  class_table[in]->class_bit  = 0;
  class_table[in]->min_dam    = 0;
  class_table[in]->base_ac    = 0;
  class_table[in]->speed      = 0;
  class_table[in]->dodge      = 0;
  class_table[in]->parry      = 0;
  class_table[in]->weapons    = 0;
  class_table[in]->spells     = 0;
  class_table[in]->damroll    = 0;
  class_table[in]->hitroll    = 0;
  class_table[in]->playable   = 0;
  return TRUE;
}

void do_setclass( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    FILE *fpList;
    char classlist[256];
    struct class_type *class;
    int cl, i, x;


    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' )
    {
	 send_to_char( "Syntax: setclass <class> <field> <value>\n\r",	ch );
	 send_to_char( "Syntax: setclass <class> create\n\r",		ch );
	 send_to_char( "\n\rField being one of:\n\r",			ch );
	 send_to_char( "dodge parry ac attacks weapons spells damcap bit name playable\n\r", ch );
         send_to_char( "damroll hitroll\n\r",ch);
     return;
    }

    if ( is_number(arg1) && (cl=atoi(arg1)) >= 0 && cl < MAX_CLASS )
	class = class_table[cl];
    else
    {
	class = NULL;
	for ( cl = 0; cl < MAX_CLASS && class_table[cl]; cl++ )
	{
	    if ( !class_table[cl]->class_nm ) continue;
	    if ( !str_cmp(class_table[cl]->class_nm, arg1) )
	    {
		class = class_table[cl];
		break;
	    }
	}
    }
    if ( !str_cmp( arg1, "load"))
    {
     load_classes();
     return;
    }
    if ( !str_cmp( arg1, "list"))
    {
      for(x = 0; x <= MAX_PC_CLASS; x++)
      {
        xprintf(buf1,"%d:  %s\n\r",x,class_table[x+1]->class_nm);
      }
        send_to_char(buf1, ch);
        xprintf(buf1,"%d\n\r",MAX_PC_CLASS);
        send_to_char(buf1, ch);
      return;
    }

    if ( !str_cmp( arg2, "create" ) && class )
    {
    	send_to_char( "That class already exists!\n\r", ch );
	return;
    }
    
    if ( !class && str_cmp(arg2, "create") )
    {
	send_to_char( "No such class.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "save" ) )
    {
	write_class_file( cl );
	send_to_char( "Saved.\n\r", ch );
	return;
    }

   
    if ( !str_cmp( arg2, "create") )
    {
      if ( MAX_PC_CLASS >= MAX_CLASS )
      {
       send_to_char("You need to up MAX_CLASS in mud and make clean.\n\r",ch);
       return;
      }
      
      if ( ( create_new_class( MAX_PC_CLASS, arg1 ) ) == FALSE )
      {
       send_to_char("Couldn't create a new class.\n\r", ch );
       return;
      }
       write_class_file( MAX_PC_CLASS );
       MAX_PC_CLASS++;
       sprintf( classlist, "%s%s", CLASS_DIR, CLASS_LIST );
      
      if ( ( fpList = fopen( classlist, "w") ) == NULL )
      {
       bug("Can't open Class list for writing.", 0 );
	   return;
      }

      for ( i = 0; i<MAX_PC_CLASS; i++ )
    	fprintf( fpList, "%s.class\n", class_table[i]->class_nm );
     
      fprintf(fpList, "$\n");
      fclose( fpList );
      send_to_char("Done.\n\r", ch );
      return;
    }

    if ( !argument )
    {
     send_to_char("You must specify an argument.\n\r", ch );
	 return;
    }

    if ( !str_cmp( arg2, "list"))
    {
       send_to_char("#C-#c-#L=#l=#RC#rlass #RD#ratabase #RV#ralues#l=#L=#c-#C-#n\n\r",ch);
       xprintf(buf1,"Class Name: %s\n\r",class->class_nm);
       send_to_char(buf1,ch);
       xprintf(buf1,"Bit Vector: %ld\n\r",class->class_bit);
       send_to_char(buf1,ch);
       xprintf(buf1,"Playable  : %d\n\r",class->playable);
       send_to_char(buf1,ch);
       send_to_char("#C-#c-#L-#l=#yC#oombat #yD#oata#l=#L-#c-#C-#n\n\r",ch);
       xprintf(buf1,"DamCap : %d\n\r",class->min_dam);
       send_to_char(buf1,ch);
       xprintf(buf1,"Dodge  : %d\n\r",class->dodge);
       send_to_char(buf1,ch);
       xprintf(buf1,"Parry  : %d\n\r",class->parry);
       send_to_char(buf1,ch);
       xprintf(buf1,"Armor  : %d\n\r",class->base_ac);
       send_to_char(buf1,ch);
       xprintf(buf1,"Attacks: %d\n\r",class->speed);
       send_to_char(buf1,ch);
       xprintf(buf1,"Hitroll: %d\n\r",class->hitroll);
       send_to_char(buf1,ch);
       xprintf(buf1,"Damroll: %d\n\r",class->damroll);
       send_to_char(buf1,ch);
       send_to_char("#C-#c-#L-#l=#GW#geapon#n/#GS#gpell #GD#gata#l=#L-#c-#C-#n\n\r",ch);
       xprintf(buf1,"Weapon Max: %d\n\r",class->weapons);
       send_to_char(buf1,ch);
       xprintf(buf1,"Spell Max : %d\n\r",class->spells);
       send_to_char(buf1,ch);
       return;
    }
    if ( !str_cmp( arg2, "hitroll" ) )
    {
     xprintf(buf2,"Hitroll was %d.\n\r",class->hitroll);
       send_to_char(buf2,ch);
     class->hitroll = atoi( argument );
     xprintf(buf2,"%s's hitroll set to %d.\n\r",class->class_nm, class->hitroll);
       send_to_char(buf2,ch);
    }

    if ( !str_cmp( arg2, "damroll" ) )
    {
     xprintf(buf2,"Damroll was %d.\n\r",class->damroll);
       send_to_char(buf2,ch);
     class->damroll = atoi( argument );
     xprintf(buf2,"%s's damroll set to %d.\n\r",class->class_nm, class->damroll);
       send_to_char(buf2,ch);
    }

    if ( !str_cmp( arg2, "weapons" ) )
    {
     xprintf(buf2,"Weapon max was %d.\n\r",class->weapons);
       send_to_char(buf2,ch);
     class->weapons = atoi( argument );
     xprintf(buf2,"%s's weapon max set to %d.\n\r",class->class_nm, class->weapons);
       send_to_char(buf2,ch);
    }
    
    if ( !str_cmp( arg2, "spells" ) )
    {
     xprintf(buf2,"Spell max was %d.\n\r",class->spells);
       send_to_char(buf2,ch);
     class->spells = atoi( argument );
     xprintf(buf2,"%s's spell max set to %d.\n\r",class->class_nm, class->spells);
       send_to_char(buf2,ch);
    }

    if ( !str_cmp( arg2, "name" ) )
    {
 	 STRFREE(class->class_nm );
 	 class->class_nm = STRALLOC( argument );
	 send_to_char( "Done.\n\r", ch );
    }
    
    if ( !str_cmp( arg2, "attacks" ) )
    {
     xprintf(buf2,"Attacks was %d.\n\r",class->speed);
       send_to_char(buf2,ch);
     class->speed = atoi( argument );
     xprintf(buf2,"%s's attacks set to %d.\n\r",class->class_nm, class->speed);
       send_to_char(buf2,ch);
    }
    
    if ( !str_cmp( arg2, "damcap" ) )
    {
     xprintf(buf2,"Min_damcap was %d.\n\r",class->min_dam);
       send_to_char(buf2,ch);
     class->min_dam = atoi( argument );
     xprintf(buf2,"%s's min_damcap set to %d.\n\r",class->class_nm, class->min_dam);
       send_to_char(buf2,ch);
    }

    else if ( !str_cmp( arg2, "dodge" ) )
    {
     xprintf(buf2,"Dodge was %d.\n\r",class->dodge);
       send_to_char(buf2,ch);
     class->dodge = atoi( argument );
     xprintf(buf2,"%s's dodge set to %d.\n\r",class->class_nm, class->dodge);
       send_to_char(buf2,ch);
    }

    else if ( !str_cmp( arg2, "parry" ) )
    {
     xprintf(buf2,"Parry was %d.\n\r",class->parry);
       send_to_char(buf2,ch);
     class->parry = atoi( argument );
     xprintf(buf2,"%s's parry set to %d.\n\r",class->class_nm, class->parry);
       send_to_char(buf2,ch);
    }

    else if ( !str_cmp( arg2, "armor" ) )
    {
     xprintf(buf2,"Base AC was %d.\n\r",class->base_ac);
       send_to_char(buf2,ch);
     class->base_ac = atoi( argument );
     xprintf(buf2,"%s's base AC set to %d.\n\r",class->class_nm, class->base_ac);
       send_to_char(buf2,ch);
    }

    else if ( !str_cmp( arg2, "playable" ) )
    {
     xprintf(buf2,"Playble was %d.\n\r",class->playable);
       send_to_char(buf2,ch);
     class->playable = atoi( argument );
     xprintf(buf2,"%s's playable set to %d.\n\r",class->class_nm, class->playable);
       send_to_char(buf2,ch);
    }
    
    else if (!str_cmp(arg2, "bit"))
    {
     class->class_bit = atoi( argument );
     xprintf(buf2,"%s's bitvector is now: %d\n\r",class->class_nm,class->class_bit);
       send_to_char(buf2,ch);
    }

//    do_setclass( ch, "" );
   save_classes();
   return;
}
