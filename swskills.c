#include <math.h> 
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mud.h"

void    add_reinforcements  args( ( CHAR_DATA *ch ) );
ch_ret  one_hit             args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
int     xp_compute                ( CHAR_DATA *ch , CHAR_DATA *victim );
ROOM_INDEX_DATA *generate_exit( ROOM_INDEX_DATA *in_room, EXIT_DATA **pexit );
int ris_save( CHAR_DATA *ch, int chance, int ris );
CHAR_DATA *get_char_room_mp( CHAR_DATA *ch, char *argument );
void  clear_roomtype( ROOM_INDEX_DATA * room );
bool    aff_paralysis( CHAR_DATA *ch, CHAR_DATA *victim );
bool	check_parse_name	args( ( char *name ) );


extern int      top_affect;
extern int top_r_vnum;

const	char *	sector_name	[SECT_MAX+16]	=
{
		"system", "terminal", "entertainment", "multimedia", "finance", "unknown", "unknown", "unknown",
		"unknown", "unknown", "database", "unknown", "unknown", "unknown",
		"productivity", "unknown", "shopping", "unknown", "database", "graveyard", "unknown",
		"firewall", "unknown", "unknown", "unknown", "subserver", "unknown", "breeding", "bridge",
		"hall", "camp", "stairway", "elevator", "tunnel", "ramp", "corridor", "ruins", "settlement",
		"platform", "chasm", "tower", "shelter", "prison", "factory", "farm", "unknown","stack","host",
		"server","hyperlink","link","wiki","404","webbrowser","cookie", "chatroom", "index","forum",
		"brokenport","torrent","nexus","proxy"
};

char *acctname( CHAR_DATA * ch )
{
   static char buf[MAX_STRING_LENGTH];
   const char *name;
   char *s;
   int len;

   *buf = '\0';
   s = buf;
   len = 0;
   name = ch->name;
#define add_to_s(chr) (*s++ = chr, ++len)
   for( ; *name && len < 15; ++name )
   {
      if( isalpha( *name ) )
      {
         switch ( tolower( *name ) )
         {
            case 'a':
               add_to_s( '9' );
               add_to_s( '1' );
               break;
            case 'b':
               add_to_s( '0' );
               add_to_s( '2' );
               break;
            case 'c':
               add_to_s( '5' );
               add_to_s( '9' );
               break;
            case 'd':
               add_to_s( '1' );
               add_to_s( '4' );
               break;
            case 'e':
               add_to_s( '5' );
               break;
            case 'f':
               add_to_s( '6' );
               break;
            case 'g':
               add_to_s( '7' );
               break;
            case 'h':
               add_to_s( '8' );
               break;
            case 'i':
               add_to_s( '9' );
               break;
            case 'j':
               add_to_s( '1' );
               add_to_s( '0' );
               break;
            case 'k':
               add_to_s( '1' );
               add_to_s( '1' );
               break;
            case 'l':
               add_to_s( '2' );
               add_to_s( '0' );
               break;
            case 'm':
               add_to_s( '0' );
               add_to_s( '1' );
               break;
            case 'n':
               add_to_s( '1' );
               add_to_s( '4' );
               break;
            case 'o':
               add_to_s( '0' );
               add_to_s( '9' );
               break;
            case 'p':
               add_to_s( '1' );
               add_to_s( '6' );
               break;
            case 'q':
               add_to_s( '1' );
               add_to_s( '7' );
               break;
            case 'r':
               add_to_s( '5' );
               add_to_s( '4' );
               break;
            case 's':
               add_to_s( '1' );
               add_to_s( '9' );
               break;
            case 't':
               add_to_s( '2' );
               add_to_s( '0' );
               break;
            case 'u':
               add_to_s( '2' );
               add_to_s( '1' );
               break;
            case 'v':
               add_to_s( '2' );
               add_to_s( '2' );
               break;
            case 'w':
               add_to_s( '2' );
               add_to_s( '4' );
               break;
            case 'x':
               add_to_s( '2' );
               add_to_s( '3' );
               break;
            case 'y':
               add_to_s( '5' );
               add_to_s( '2' );
               break;
            case 'z':
               add_to_s( '2' );
               add_to_s( '6' );
               break;
         }
      }
   }
   if( len < 15 )
   {
      size_t namelen;
      char *filler;
      char fillerbuf[MAX_STRING_LENGTH];
      const char *const fillers[] = { "gewhinnqnppali", "hmmithinkishou",
         "ldinsertsomehi", "ddenmessagesin",
         "thisforfuturec", "coderstolaughat",
         "ireallyshouldb", "esleepingnowbu",
         "timaddictedtot", "hisshit"
      };

      *fillerbuf = '\0';
      name = ch->name;
      namelen = strlen( name );
      strcpy( fillerbuf, name );
      if( namelen == 3 )
         strcpy( fillerbuf + namelen, fillers[0] );
      else if( namelen > 11 || namelen < 3 )
         strcpy( fillerbuf + namelen, fillers[9] );
      else
         strcpy( fillerbuf + namelen, fillers[namelen - 3] );

      *s = '\0';
      filler = fillerbuf + strlen( buf );

      for( ; *filler && len < 15; ++filler )
      {
         if( isalpha( *filler ) )
         {
            switch ( tolower( *filler ) )
            {
               case 'a':
                  add_to_s( '6' );
                  add_to_s( '6' );
                  break;
               case 'b':
                  add_to_s( '9' );
                  add_to_s( '0' );
                  break;
               case 'c':
                  add_to_s( '2' );
                  add_to_s( '7' );
                  break;
               case 'd':
                  add_to_s( '2' );
                  add_to_s( '1' );
                  break;
               case 'e':
                  add_to_s( '2' );
                  add_to_s( '2' );
                  break;
               case 'f':
                  add_to_s( '6' );
                  break;
               case 'g':
                  add_to_s( '7' );
                  break;
               case 'h':
                  add_to_s( '5' );
                  add_to_s( '0' );
                  break;
               case 'i':
                  add_to_s( '9' );
                  break;
               case 'j':
                  add_to_s( '1' );
                  add_to_s( '0' );
                  break;
               case 'k':
                  add_to_s( '1' );
                  add_to_s( '1' );
                  break;
               case 'l':
                  add_to_s( '1' );
                  add_to_s( '2' );
                  break;
               case 'm':
                  add_to_s( '1' );
                  add_to_s( '3' );
                  break;
               case 'n':
                  add_to_s( '0' );
                  add_to_s( '1' );
                  break;
               case 'o':
                  add_to_s( '1' );
                  add_to_s( '5' );
                  break;
               case 'p':
                  add_to_s( '1' );
                  add_to_s( '6' );
                  break;
               case 'q':
                  add_to_s( '2' );
                  break;
               case 'r':
                  add_to_s( '5' );
                  add_to_s( '1' );
                  break;
               case 's':
                  add_to_s( '1' );
                  add_to_s( '8' );
                  break;
               case 't':
                  add_to_s( '7' );
                  add_to_s( '2' );
                  break;
               case 'u':
                  add_to_s( '4' );
                  add_to_s( '4' );
                  break;
               case 'v':
                  add_to_s( '9' );
                  break;
               case 'w':
                  add_to_s( '8' );
                  add_to_s( '2' );
                  break;
               case 'x':
                  add_to_s( '1' );
                  add_to_s( '1' );
                  break;
               case 'y':
                  add_to_s( '1' );
                  add_to_s( '4' );
                  break;
               case 'z':
                  add_to_s( '5' );
                  break;
            }

            if( len >= 15 )
               break;

            switch ( tolower( *filler ) )
            {
               case 'a':
                  add_to_s( '2' );
                  break;
               case 'b':
                  add_to_s( '1' );
                  add_to_s( '7' );
                  add_to_s( '3' );
                  break;
               case 'c':
                  add_to_s( '5' );
                  add_to_s( '5' );
                  add_to_s( '8' );
                  break;
               case 'd':
                  add_to_s( '8' );
                  add_to_s( '1' );
                  break;
               case 'e':
                  add_to_s( '3' );
                  add_to_s( '0' );
                  add_to_s( '9' );
                  break;
               case 'f':
                  add_to_s( '6' );
                  add_to_s( '4' );
                  add_to_s( '1' );
                  break;
               case 'g':
                  add_to_s( '6' );
                  add_to_s( '7' );
                  add_to_s( '8' );
                  break;
               case 'h':
                  add_to_s( '5' );
                  break;
               case 'i':
                  add_to_s( '2' );
                  add_to_s( '0' );
                  add_to_s( '2' );
                  break;
               case 'j':
                  add_to_s( '1' );
                  add_to_s( '7' );
                  add_to_s( '0' );
                  break;
               case 'k':
                  add_to_s( '1' );
                  add_to_s( '1' );
                  add_to_s( '1' );
                  break;
               case 'l':
                  add_to_s( '1' );
                  add_to_s( '5' );
                  add_to_s( '2' );
                  break;
               case 'm':
                  add_to_s( '1' );
                  add_to_s( '3' );
                  break;
               case 'n':
                  add_to_s( '0' );
                  add_to_s( '1' );
                  break;
               case 'o':
                  add_to_s( '6' );
                  add_to_s( '1' );
                  add_to_s( '5' );
                  break;
               case 'p':
                  add_to_s( '1' );
                  add_to_s( '6' );
                  break;
               case 'q':
                  add_to_s( '2' );
                  break;
               case 'r':
                  add_to_s( '3' );
                  add_to_s( '3' );
                  break;
               case 's':
                  add_to_s( '0' );
                  add_to_s( '6' );
                  break;
               case 't':
                  add_to_s( '7' );
                  add_to_s( '2' );
                  break;
               case 'u':
                  add_to_s( '9' );
                  add_to_s( '4' );
                  add_to_s( '7' );
                  break;
               case 'v':
                  add_to_s( '5' );
                  add_to_s( '9' );
                  break;
               case 'w':
                  add_to_s( '7' );
                  add_to_s( '1' );
                  break;
               case 'x':
                  add_to_s( '6' );
                  add_to_s( '1' );
                  add_to_s( '1' );
                  break;
               case 'y':
                  add_to_s( '2' );
                  add_to_s( '4' );
                  break;
               case 'z':
                  add_to_s( '6' );
                  add_to_s( '1' );
                  add_to_s( '7' );
                  break;
            }

            if( len >= 15 )
               break;

            switch ( tolower( *filler ) )
            {
               case 'a':
                  add_to_s( '1' );
                  break;
               case 'b':
                  add_to_s( '2' );
                  break;
               case 'c':
                  add_to_s( '3' );
                  break;
               case 'd':
                  add_to_s( '4' );
                  break;
               case 'e':
                  add_to_s( '5' );
                  break;
               case 'f':
                  add_to_s( '6' );
                  break;
               case 'g':
                  add_to_s( '7' );
                  break;
               case 'h':
                  add_to_s( '8' );
                  break;
               case 'i':
                  add_to_s( '9' );
                  break;
               case 'j':
                  add_to_s( '1' );
                  add_to_s( '0' );
                  break;
               case 'k':
                  add_to_s( '1' );
                  add_to_s( '1' );
                  break;
               case 'l':
                  add_to_s( '1' );
                  add_to_s( '2' );
                  break;
               case 'm':
                  add_to_s( '1' );
                  add_to_s( '3' );
                  break;
               case 'n':
                  add_to_s( '1' );
                  add_to_s( '4' );
                  break;
               case 'o':
                  add_to_s( '1' );
                  add_to_s( '5' );
                  break;
               case 'p':
                  add_to_s( '1' );
                  add_to_s( '6' );
                  break;
               case 'q':
                  add_to_s( '1' );
                  add_to_s( '7' );
                  break;
               case 'r':
                  add_to_s( '1' );
                  add_to_s( '8' );
                  break;
               case 's':
                  add_to_s( '1' );
                  add_to_s( '9' );
                  break;
               case 't':
                  add_to_s( '2' );
                  add_to_s( '0' );
                  break;
               case 'u':
                  add_to_s( '2' );
                  add_to_s( '1' );
                  break;
               case 'v':
                  add_to_s( '2' );
                  add_to_s( '2' );
                  break;
               case 'w':
                  add_to_s( '2' );
                  add_to_s( '3' );
                  break;
               case 'x':
                  add_to_s( '2' );
                  add_to_s( '4' );
                  break;
               case 'y':
                  add_to_s( '2' );
                  add_to_s( '5' );
                  break;
               case 'z':
                  add_to_s( '2' );
                  add_to_s( '6' );
                  break;
            }
         }
      }
#undef add_to_s
   }
   buf[15] = '\0';
   return buf;
}



void do_makeblade( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int level, chance;
	int charge = 0;
	bool checktool, checkdura, checkbatt, checkoven;
	OBJ_DATA *obj;
	OBJ_INDEX_DATA *pObjIndex;
	int vnum, bonus;
	AFFECT_DATA *paf;
	AFFECT_DATA *paf2;

	if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_makeblade] )
	{
		send_to_char("> you do not know the codeblade skillsoft\n\r", ch );
		return;
	}

	strcpy( arg , argument );

	switch( ch->substate )
	{
	default:

		if ( arg[0] == '\0' )
		{
			send_to_char( "&R> syntax: codeblade <name>\n\r&w", ch);
			return;
		}

		checktool = FALSE;
		checkdura = FALSE;
		checkbatt = FALSE;
		checkoven = FALSE;

		if ( !IS_SET( ch->in_room->room_flags, ROOM_RESTAURANT ) )
		{
			send_to_char( "> &Ryou need to be in a coding node&w\n\r", ch );
			return;
		}
		else {
				if ( ch->in_room->level != 0 )
				bonus = ch->in_room->level * 10;
		}

		for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
		{
			if (obj->item_type == ITEM_TOOLKIT)
				checktool = TRUE;
			if (obj->item_type == ITEM_METAL)
				checkdura = TRUE;
			if (obj->item_type == ITEM_BATTERY)
				checkbatt = TRUE;

			if (obj->item_type == ITEM_OVEN)
				checkoven = TRUE;
		}

		if ( !checktool )
		{
			send_to_char( "&R> you need a devkit\n\r", ch);
			return;
		}

		if ( !checkdura )
		{
			send_to_char( "&R> you need a blade class\n\r", ch);
			return;
		}

		if ( !checkbatt )
		{
			send_to_char( "&R> you need an utility patch\n\r", ch);
			return;
		}

		if ( !checkoven )
		{
			send_to_char( "&R> you need a compiler\n\r", ch);
			return;
		}

		chance = IS_NPC(ch) ? ch->top_level
				: (int) (ch->pcdata->learned[gsn_makeblade]);

		if ( bonus > 0 )
			chance += bonus;

		if ( number_percent( ) < chance )
		{
			send_to_char( "&G> you begin coding a blade module\n\r", ch);

			if ( bonus > 0 )
			ch_printf( ch , "node bonus: %d\n\r\n\r", bonus);

			act( AT_PLAIN, "> $n loads $s devkit and a compiler and begins to work on something", ch,
					NULL, argument , TO_ROOM );
			add_timer ( ch , TIMER_DO_FUN , 2 , do_makeblade , 1 );
			ch->dest_buf = str_dup(arg);
			return;
		}
		send_to_char("&R> you fail to code the blade module\n\r",ch);
		learn_from_failure( ch, gsn_makeblade );
		return;

	case 1:
		if ( !ch->dest_buf )
			return;
		strcpy(arg, (const char* ) ch->dest_buf);
		DISPOSE( ch->dest_buf);
		break;

	case SUB_TIMER_DO_ABORT:
		DISPOSE( ch->dest_buf );
		ch->substate = SUB_NONE;
		send_to_char("&R> you are interrupted and fail to finish your work\n\r", ch);
		return;
	}

	ch->substate = SUB_NONE;

	level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makeblade]);
	vnum = OBJ_VNUM_PROTO_BLADE;

	if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
	{
		send_to_char( "&R> the item you are trying to create is missing from the database\n\r> please inform the administration of this error\n\r", ch );
		return;
	}

	checktool = FALSE;
	checkdura = FALSE;
	checkbatt = FALSE;
	checkoven = FALSE;

	for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
	{
		if (obj->item_type == ITEM_TOOLKIT)
			checktool = TRUE;
		if (obj->item_type == ITEM_OVEN)
			checkoven = TRUE;
		if (obj->item_type == ITEM_METAL && checkdura == FALSE)
		{
			checkdura = TRUE;
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
		}
		if (obj->item_type == ITEM_BATTERY && checkbatt == FALSE )
		{
			charge = UMAX( 5, obj->value[0] );
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkbatt = TRUE;
		}
	}

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makeblade]) ;

	if ( number_percent( ) > chance*2  || ( !checktool ) || ( !checkdura ) || ( !checkbatt ) || ( !checkoven ) )
	{
		send_to_char( "> &Ryou fail to code the blade module&w\n\r", ch);
		learn_from_failure( ch, gsn_makeblade );
		return;
	}

	obj = create_object( pObjIndex, level );

	obj->item_type = ITEM_WEAPON;
	SET_BIT( obj->wear_flags, ITEM_WIELD );
	SET_BIT( obj->wear_flags, ITEM_TAKE );
	obj->level = level;
	obj->weight = 3;
	STRFREE( obj->name );
	strcpy( buf, arg );
	strcat( buf, " blade-module blade" );
	obj->name = STRALLOC( buf );
	strcpy( buf, arg );
	strcat( buf, " [blade]" );
	STRFREE( obj->short_descr );
	obj->short_descr = STRALLOC( buf );
	STRFREE( obj->description );
	obj->description = STRALLOC( buf );
	CREATE( paf, AFFECT_DATA, 1 );
	paf->type               = -1;
	paf->duration           = -1;
	paf->location           = get_atype( "backstab" );
	paf->modifier           = level/3;
	paf->bitvector          = 0;
	paf->next               = NULL;
	LINK( paf, obj->first_affect, obj->last_affect, next, prev );
	++top_affect;
	CREATE( paf2, AFFECT_DATA, 1 );
	paf2->type               = -1;
	paf2->duration           = -1;
	paf2->location           = get_atype( "hitroll" );
	paf2->modifier           = -2;
	paf2->bitvector          = 0;
	paf2->next               = NULL;
	LINK( paf2, obj->first_affect, obj->last_affect, next, prev );
	++top_affect;
	obj->value[0] = INIT_WEAPON_CONDITION;
	obj->value[1] = (int) (level/20+10);      /* min dmg  */
	obj->value[2] = (int) (level/10+20);      /* max dmg */
	obj->value[3] = WEAPON_VIBRO_BLADE;
	obj->value[4] = charge;
	obj->value[5] = charge;
	obj->cost = obj->value[2]*10;

	obj = obj_to_char( obj, ch );

	send_to_char( "&G> you finish coding a blade module&w\n\r", ch);
	act( AT_PLAIN, "> $n finishes coding a blade module", ch,
			NULL, argument , TO_ROOM );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more intelligent than before\n\r", ch );
		ch->perm_int++;
		ch->perm_int = UMIN( ch->perm_int , 25 );
	}

	learn_from_success( ch, gsn_makeblade );
	learn_from_success( ch, gsn_makeblade );
}

void do_makeblaster( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int level, chance;
	bool checktool, checkdura, checkbatt, checkoven, checkcond, checkcirc, checkammo;
	OBJ_DATA *obj;
	OBJ_INDEX_DATA *pObjIndex;
	int vnum, power, scope, ammo, bonus;
	AFFECT_DATA *paf;
	AFFECT_DATA *paf2;

	if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_makeblaster] )
	{
		send_to_char("> you do not know the codeblaster skillsoft\n\r", ch );
		return;
	}

	strcpy( arg , argument );

	switch( ch->substate )
	{
	default:
		if ( arg[0] == '\0' )
		{
			send_to_char( "> &Rsyntax: codeblaster <name>&w\n\r", ch);
			return;
		}

		checktool = FALSE;
		checkdura = FALSE;
		checkbatt = FALSE;
		checkoven = FALSE;
		checkcond = FALSE;
		checkcirc = FALSE;

		if ( !IS_SET( ch->in_room->room_flags, ROOM_RESTAURANT ) )
		{
			send_to_char( "> &Ryou need to be in a coding node&w\n\r", ch );
			return;
		}
		else {
				if ( ch->in_room->level != 0 )
				bonus = ch->in_room->level * 10;
		}

		for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
		{
			if (obj->item_type == ITEM_TOOLKIT)
				checktool = TRUE;
			if (obj->item_type == ITEM_PLASTIC)
				checkdura = TRUE;
			if (obj->item_type == ITEM_BATTERY)
				checkbatt = TRUE;
			if (obj->item_type == ITEM_OVEN)
				checkoven = TRUE;
			if (obj->item_type == ITEM_CIRCUIT)
				checkcirc = TRUE;
			if (obj->item_type == ITEM_SUPERCONDUCTOR)
				checkcond = TRUE;
		}

		if ( !checktool )
		{
			send_to_char( "&R> you need a devkit\n\r", ch);
			return;
		}

		if ( !checkdura )
		{
			send_to_char( "&R> you need a blaster class\n\r", ch);
			return;
		}

		if ( !checkbatt )
		{
			send_to_char( "&R> you need an utility patch\n\r", ch);
			return;
		}

		if ( !checkoven )
		{
			send_to_char( "&R> you need a compiler\n\r", ch);
			return;
		}

		if ( !checkcirc )
		{
			send_to_char( "&R> you need a function variant one\n\r", ch);
			return;
		}

		if ( !checkcond )
		{
			send_to_char( "&R> you need a function variant two\n\r", ch);
			return;
		}

		chance = IS_NPC(ch) ? ch->top_level
				: (int) (ch->pcdata->learned[gsn_makeblaster]);
		if ( number_percent( ) < chance )
		{
			send_to_char( "&G> you begin the long process of making a blaster\n\r", ch);
			if ( bonus > 0 )
			ch_printf( ch , "node bonus: %d\n\r\n\r", bonus);

			act( AT_PLAIN, "> $n takes $s tools and a small oven and begins to work on something", ch,
					NULL, argument , TO_ROOM );
			add_timer ( ch , TIMER_DO_FUN , 2 , do_makeblaster , 1 );
			ch->dest_buf   = str_dup(arg);
			return;
		}
		send_to_char("&R> you cannot figure out how to fit the parts together\n\r",ch);
		learn_from_failure( ch, gsn_makeblaster );
		return;

	case 1:
		if ( !ch->dest_buf )
			return;
		strcpy(arg, (const char*) ch->dest_buf);
		DISPOSE( ch->dest_buf);
		break;

	case SUB_TIMER_DO_ABORT:
		DISPOSE( ch->dest_buf );
		ch->substate = SUB_NONE;
		send_to_char("&R> you are interupted and fail to finish your work\n\r", ch);
		return;
	}

	ch->substate = SUB_NONE;

	level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makeblaster]);
	vnum = OBJ_VNUM_PROTO_BLASTER;

	if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
	{
		send_to_char( "&R> the item you are trying to create is missing from the database\n\rplease inform the administration of this error\n\r", ch );
		return;
	}

	checkammo= FALSE;
	checktool = FALSE;
	checkdura = FALSE;
	checkbatt = FALSE;
	checkoven = FALSE;
	checkcond = FALSE;
	checkcirc = FALSE;
	power     = 0;
	scope     = 0;
	ammo = 0;

	for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
	{
		if (obj->item_type == ITEM_TOOLKIT)
			checktool = TRUE;
		if (obj->item_type == ITEM_OVEN)
			checkoven = TRUE;
		if (obj->item_type == ITEM_PLASTIC && checkdura == FALSE)
		{
			checkdura = TRUE;
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
		}
		if (obj->item_type == ITEM_AMMO && checkammo == FALSE)
		{
			ammo = obj->value[0];
			checkammo = TRUE;
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
		}
		if (obj->item_type == ITEM_BATTERY && checkbatt == FALSE)
		{
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkbatt = TRUE;
		}
		if (obj->item_type == ITEM_LENS && scope == 0)
		{
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			scope++;
		}
		if (obj->item_type == ITEM_SUPERCONDUCTOR && power<2)
		{
			power++;
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkcond = TRUE;
		}
		if (obj->item_type == ITEM_CIRCUIT && checkcirc == FALSE)
		{
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkcirc = TRUE;
		}
	}

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makeblaster]) ;


	if ( number_percent( ) > chance*2  || ( !checktool ) || ( !checkdura ) || ( !checkbatt ) || ( !checkoven )  || ( !checkcond ) || ( !checkcirc) )
	{
		send_to_char( "> &Ryou fail to code the blaster module&w\n\r", ch);
		learn_from_failure( ch, gsn_makeblaster );
		return;
	}

	obj = create_object( pObjIndex, level );

	obj->item_type = ITEM_WEAPON;
	SET_BIT( obj->wear_flags, ITEM_WIELD );
	SET_BIT( obj->wear_flags, ITEM_TAKE );
	obj->level = level;
	obj->weight = 2+level/10;
	STRFREE( obj->name );
	strcpy( buf , arg );
	strcat( buf , " blaster");
	obj->name = STRALLOC( buf );
	strcpy( buf, arg );
	strcat( buf , " [blaster]");
	STRFREE( obj->short_descr );
	obj->short_descr = STRALLOC( buf );
	STRFREE( obj->description );
	obj->description = STRALLOC( buf );
	CREATE( paf, AFFECT_DATA, 1 );
	paf->type               = -1;
	paf->duration           = -1;
	paf->location           = get_atype( "hitroll" );
	paf->modifier           = URANGE( 0, 1+scope, level/30 );
	paf->bitvector          = 0;
	paf->next               = NULL;
	LINK( paf, obj->first_affect, obj->last_affect, next, prev );
	++top_affect;
	CREATE( paf2, AFFECT_DATA, 1 );
	paf2->type               = -1;
	paf2->duration           = -1;
	paf2->location           = get_atype( "damroll" );
	paf2->modifier           = URANGE( 0, power, level/30);
	paf2->bitvector          = 0;
	paf2->next               = NULL;
	LINK( paf2, obj->first_affect, obj->last_affect, next, prev );
	++top_affect;
	obj->value[0] = INIT_WEAPON_CONDITION;       /* condition  */
	obj->value[1] = (int) (level/10+15);      /* min dmg  */
	obj->value[2] = (int) (level/5+25);      /* max dmg  */
	obj->value[3] = WEAPON_BLASTER;
	obj->value[4] = ammo;
	obj->value[5] = 2000;
	obj->cost = obj->value[2]*50;

	obj = obj_to_char( obj, ch );

	send_to_char( "&G> you finish coding and look at your newly created blaster module&w\n\r", ch);
	act( AT_PLAIN, "> $n finishes coding $s new blaster module", ch,
			NULL, argument , TO_ROOM );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more intelligent than before\n\r", ch );
		ch->perm_int++;
		ch->perm_int = UMIN( ch->perm_int , 25 );
	}

	learn_from_success( ch, gsn_makeblaster );
	learn_from_success( ch, gsn_makeblaster );
	learn_from_success( ch, gsn_makeblaster );
}

void do_makelightsaber( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int chance;
	bool checktool, checkdura, checkbatt,
	checkoven, checkcond, checkcirc, checklens, checkgems, checkmirr;
	OBJ_DATA *obj;
	OBJ_INDEX_DATA *pObjIndex;
	int vnum, level, gems, charge, gemtype;
	AFFECT_DATA *paf;
	AFFECT_DATA *paf2;

	strcpy( arg, argument );

	switch( ch->substate )
	{
	default:
		if ( arg[0] == '\0' )
		{
			send_to_char( "&R> syntax: Makelightsaber <name>\n\r&w", ch);
			return;
		}

		checktool = FALSE;
		checkdura = FALSE;
		checkbatt = FALSE;
		checkoven = FALSE;
		checkcond = FALSE;
		checkcirc = FALSE;
		checklens = FALSE;
		checkgems = FALSE;
		checkmirr = FALSE;

		if ( !IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
		{
			send_to_char( "&R> you need to be in a quiet peaceful place to craft a lightsaber\n\r", ch);
			return;
		}

		for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
		{
			if (obj->item_type == ITEM_TOOLKIT)
				checktool = TRUE;
			if (obj->item_type == ITEM_LENS)
				checklens = TRUE;
			if (obj->item_type == ITEM_CRYSTAL)
				checkgems = TRUE;
			if (obj->item_type == ITEM_MIRROR)
				checkmirr = TRUE;
			if (obj->item_type == ITEM_PLASTIC || obj->item_type == ITEM_METAL )
				checkdura = TRUE;
			if (obj->item_type == ITEM_BATTERY)
				checkbatt = TRUE;
			if (obj->item_type == ITEM_OVEN)
				checkoven = TRUE;
			if (obj->item_type == ITEM_CIRCUIT)
				checkcirc = TRUE;
			if (obj->item_type == ITEM_SUPERCONDUCTOR)
				checkcond = TRUE;
		}

		if ( !checktool )
		{
			send_to_char( "&R> you need toolkit to make a lightsaber\n\r", ch);
			return;
		}

		if ( !checkdura )
		{
			send_to_char( "&R> you need something to make it out of\n\r", ch);
			return;
		}

		if ( !checkbatt )
		{
			send_to_char( "&R> you need an utility subroutine for your lightsaber\n\r", ch);
			return;
		}

		if ( !checkoven )
		{
			send_to_char( "&R> you need a small furnace to heat and shape the components\n\r", ch);
			return;
		}

		if ( !checkcirc )
		{
			send_to_char( "&R> you need a small circuit board\n\r", ch);
			return;
		}

		if ( !checkcond )
		{
			send_to_char( "&R> you still need a small superconductor for your lightsaber\n\r", ch);
			return;
		}

		if ( !checklens )
		{
			send_to_char( "&R> you still need a lens to focus the beam\n\r", ch);
			return;
		}

		if ( !checkgems )
		{
			send_to_char( "&RLightsabers require 1 to 3 gems to work properly\n\r", ch);
			return;
		}

		if ( !checkmirr )
		{
			send_to_char( "&R> you need a high intesity reflective cup to create a lightsaber\n\r", ch);
			return;
		}

		chance = IS_NPC(ch) ? ch->top_level
				: (int) (ch->pcdata->learned[gsn_lightsaber_crafting]);
		if ( number_percent( ) < chance )
		{
			send_to_char( "&G> you begin the long process of crafting a lightsaber\n\r", ch);
			act( AT_PLAIN, "> $n takes $s tools and a small oven and begins to work on something", ch,
					NULL, argument , TO_ROOM );
			add_timer ( ch , TIMER_DO_FUN , 2 , do_makelightsaber , 1 );
			ch->dest_buf = str_dup(arg);
			return;
		}
		send_to_char("&R> you cannot figure out how to fit the parts together\n\r",ch);
		learn_from_failure( ch, gsn_lightsaber_crafting );
		return;

	case 1:
		if ( !ch->dest_buf )
			return;
		strcpy(arg, ch->dest_buf);
		DISPOSE( ch->dest_buf);
		break;

	case SUB_TIMER_DO_ABORT:
		DISPOSE( ch->dest_buf );
		ch->substate = SUB_NONE;
		send_to_char("&R> you are interupted and fail to finish your work\n\r", ch);
		return;
	}

	ch->substate = SUB_NONE;

	level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_lightsaber_crafting]);
	vnum = OBJ_VNUM_PROTO_LIGHTSABER;

	if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
	{
		send_to_char( "&R> the item you are trying to create is missing from the database\n\rPlease inform the administration of this error\n\r", ch );
		return;
	}

	checktool = FALSE;
	checkdura = FALSE;
	checkbatt = FALSE;
	checkoven = FALSE;
	checkcond = FALSE;
	checkcirc = FALSE;
	checklens = FALSE;
	checkgems = FALSE;
	checkmirr = FALSE;
	gems = 0;
	charge = 0;
	gemtype =0;

	for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
	{
		if (obj->item_type == ITEM_TOOLKIT)
			checktool = TRUE;
		if (obj->item_type == ITEM_OVEN)
			checkoven = TRUE;
		if ( (obj->item_type == ITEM_PLASTIC || obj->item_type == ITEM_METAL) && checkdura == FALSE)
		{
			checkdura = TRUE;
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
		}
		if (obj->item_type == ITEM_METAL && checkdura == FALSE)
		{
			checkdura = TRUE;
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
		}
		if (obj->item_type == ITEM_BATTERY && checkbatt == FALSE)
		{
			charge = UMIN(obj->value[1], 10);
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkbatt = TRUE;
		}
		if (obj->item_type == ITEM_SUPERCONDUCTOR && checkcond == FALSE)
		{
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkcond = TRUE;
		}
		if (obj->item_type == ITEM_CIRCUIT && checkcirc == FALSE)
		{
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkcirc = TRUE;
		}
		if (obj->item_type == ITEM_LENS && checklens == FALSE)
		{
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checklens = TRUE;
		}
		if (obj->item_type == ITEM_MIRROR && checkmirr == FALSE)
		{
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkmirr = TRUE;
		}
		if (obj->item_type == ITEM_CRYSTAL && gems < 3)
		{
			gems++;
			if ( gemtype < obj->value[0] )
				gemtype = obj->value[0];
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkgems = TRUE;
		}
	}


	if ( ( !checktool ) || ( !checkdura ) || ( !checkbatt ) || ( !checkoven )
			|| ( !checkmirr ) || ( !checklens ) || ( !checkgems ) || ( !checkcond ) || ( !checkcirc) )

	{
		send_to_char( "&R> you hold up your new lightsaber and press the switch hoping for the best\n\r", ch);
		send_to_char( "&RInstead of a blade of light, smoke starts pouring from the handle\n\r", ch);
		send_to_char( "&R> you drop the hot handle and watch as it melts on away on the floor\n\r", ch);
		learn_from_failure( ch, gsn_lightsaber_crafting );
		return;
	}

	obj = create_object( pObjIndex, level );

	obj->item_type = ITEM_WEAPON;
	SET_BIT( obj->wear_flags, ITEM_WIELD );
	SET_BIT( obj->wear_flags, ITEM_TAKE );
	obj->level = level;
	obj->weight = 5;
	STRFREE( obj->name );
	obj->name = STRALLOC( "lightsaber saber" );
	strcpy( buf, arg );
	STRFREE( obj->short_descr );
	obj->short_descr = STRALLOC( buf );
	STRFREE( obj->description );
	strcat( buf, " was carelessly misplaced here." );
	obj->description = STRALLOC( buf );
	STRFREE( obj->action_desc );
	strcpy( buf, arg );
	strcat( buf, " ignites with a hum and a soft glow." );
	obj->action_desc = STRALLOC( buf );
	CREATE( paf, AFFECT_DATA, 1 );
	paf->type               = -1;
	paf->duration           = -1;
	paf->location           = get_atype( "hitroll" );
	paf->modifier           = URANGE( 0, gems, level/30 );
	paf->bitvector          = 0;
	paf->next               = NULL;
	LINK( paf, obj->first_affect, obj->last_affect, next, prev );
	++top_affect;
	CREATE( paf2, AFFECT_DATA, 1 );
	paf2->type               = -1;
	paf2->duration           = -1;
	paf2->location           = get_atype( "parry" );
	paf2->modifier           = ( level/3 );
	paf2->bitvector          = 0;
	paf2->next               = NULL;
	LINK( paf2, obj->first_affect, obj->last_affect, next, prev );
	++top_affect;
	obj->value[0] = INIT_WEAPON_CONDITION;       /* condition  */
	obj->value[1] = (int) (level/10+gemtype*2);      /* min dmg  */
	obj->value[2] = (int) (level/5+gemtype*6);      /* max dmg */
	obj->value[3] = WEAPON_LIGHTSABER;
	obj->value[4] = charge;
	obj->value[5] = charge;
	obj->cost = obj->value[2]*75;

	obj = obj_to_char( obj, ch );

	send_to_char( "&G> you finish coding and look at your newly created lightsaber.&w\n\r", ch);
	act( AT_PLAIN, "> $n finishes coding $s new lightsaber", ch,
			NULL, argument , TO_ROOM );

	learn_from_success( ch, gsn_lightsaber_crafting );
	learn_from_success( ch, gsn_lightsaber_crafting );
	learn_from_success( ch, gsn_lightsaber_crafting );
}



void do_makejewelry( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int level, chance;
	bool checktool, checkoven, checkmetal;
	OBJ_DATA *obj;
	OBJ_DATA *metal;
	int value, cost, bonus;
	
	
		if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_makejewelry] )
	{
		send_to_char("> you do not know the codeutil skillsoft\n\r", ch );
		return;
	}

	argument = one_argument( argument, arg );
	strcpy ( arg2, argument);


	switch( ch->substate )
	{
	default:

		if ( str_cmp( arg, "finger" )
				&& str_cmp( arg, "wrist" )
				&& str_cmp( arg, "neck" )
				&& str_cmp( arg, "ears" ) )
		{
			send_to_char( "&R> syntax: codeutil <bodypart> <name>\n\r&w", ch);
			send_to_char( "&R> specify body part\n\r&w", ch);
			send_to_char( "&R> try: finger, wrist, neck, ears\n\r&w", ch);
			return;
		}

		if ( arg2[0] == '\0' )
		{
			send_to_char( "&R> syntax: codeutil <bodypart> <name>\n\r&w", ch);
			return;
		}

		checktool = FALSE;
		checkoven = FALSE;
		checkmetal = FALSE;

		if ( !IS_SET( ch->in_room->room_flags, ROOM_RESTAURANT ) )
		{
			send_to_char( "> &Ryou need to be in a coding node&w\n\r", ch );
			return;
		}
		else {
				if ( ch->in_room->level != 0 )
				bonus = ch->in_room->level * 10;
		}

		for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
		{
			if (obj->item_type == ITEM_TOOLKIT)
				checktool = TRUE;
			if (obj->item_type == ITEM_OVEN)
				checkoven = TRUE;
			if (obj->item_type == ITEM_RARE_METAL)
				checkmetal = TRUE;
		}

		if ( !checktool )
		{
			send_to_char( "&R> you need a devkit\n\r", ch);
			return;
		}

		if ( !checkoven )
		{
			send_to_char( "&R> you need a compiler\n\r", ch);
			return;
		}

		if ( !checkmetal )
		{
			send_to_char( "&R> you need an application class\n\r", ch);
			return;
		}

		chance = IS_NPC(ch) ? ch->top_level
				: (int) (ch->pcdata->learned[gsn_makejewelry]);
		if ( number_percent( ) < chance )
		{
			send_to_char( "&G> you begin the long process of coding some utility\n\r", ch);
			if ( bonus > 0 )
			ch_printf( ch , "node bonus: %d\n\r\n\r", bonus);

			act( AT_PLAIN, "> $n takes $s devkit, some code bits and begins to work", ch,
					NULL, argument , TO_ROOM );
			add_timer ( ch , TIMER_DO_FUN , 2 , do_makejewelry , 1 );
			ch->dest_buf = str_dup(arg);
			ch->dest_buf_2 = str_dup(arg2);
			return;
		}
		send_to_char("&R> you cannot figure out what to do\n\r",ch);
		learn_from_failure( ch, gsn_makejewelry );
		return;

	case 1:
		if ( !ch->dest_buf )
			return;
		if ( !ch->dest_buf_2 )
			return;
		strcpy(arg, (const char*) ch->dest_buf);
		DISPOSE( ch->dest_buf);
		strcpy(arg2, (const char*) ch->dest_buf_2);
		DISPOSE( ch->dest_buf_2);
		break;

	case SUB_TIMER_DO_ABORT:
		DISPOSE( ch->dest_buf );
		DISPOSE( ch->dest_buf_2 );
		ch->substate = SUB_NONE;
		send_to_char("&R> you are interrupted and fail to finish your work\n\r", ch);
		return;
	}

	ch->substate = SUB_NONE;

	level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makejewelry]);

	checkmetal = FALSE;
	checkoven = FALSE;
	checktool = FALSE;
	value=0;
	cost=0;

	for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
	{
		if (obj->item_type == ITEM_TOOLKIT)
			checktool = TRUE;
		if (obj->item_type == ITEM_OVEN)
			checkoven = TRUE;
		if (obj->item_type == ITEM_RARE_METAL && checkmetal == FALSE)
		{
			checkmetal = TRUE;
			separate_obj( obj );
			obj_from_char( obj );
			metal = obj;
		}
		if (obj->item_type == ITEM_CRYSTAL)
		{
			cost += obj->cost;
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
		}
	}

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makejewelry]) ;

	if ( number_percent( ) > chance*2  || ( !checkoven ) || ( !checktool ) || ( !checkmetal ) )
	{
		send_to_char( "> &Ryou fail to code the utility module&w\n\r", ch);
		learn_from_failure( ch, gsn_makejewelry );
		return;
	}

	obj = metal;

	obj->item_type = ITEM_ARMOR;
	SET_BIT( obj->wear_flags, ITEM_TAKE );
	value = get_wflag( arg );
	if ( value < 0 || value > 31 )
		SET_BIT( obj->wear_flags, ITEM_WEAR_NECK );
	else
		SET_BIT( obj->wear_flags, 1 << value );
	obj->level = level;
	STRFREE( obj->name );
	strcpy( buf, arg2 );
	obj->name = STRALLOC( buf );
	strcpy( buf, arg2 );
	strcat( buf, " [util]" );
	STRFREE( obj->short_descr );
	obj->short_descr = STRALLOC( buf );
	STRFREE( obj->description );
	obj->description = STRALLOC( buf );
	obj->value[0] = obj->value[1];
	obj->cost *= 10;
	obj->cost += cost;

	obj = obj_to_char( obj, ch );

	send_to_char( "&G> you finish coding and look at your newly created utility&w\n\r", ch);
	act( AT_PLAIN, "> $n finishes coding some new utility", ch,
			NULL, argument , TO_ROOM );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more intelligent than before\n\r", ch );
		ch->perm_int++;
		ch->perm_int = UMIN( ch->perm_int , 25 );
	}

	learn_from_success( ch, gsn_makejewelry );
	learn_from_success( ch, gsn_makejewelry );

}

void do_makearmor( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	char buf1[MAX_STRING_LENGTH];
	int level, chance;
	bool checksew, checkfab;
	OBJ_DATA *obj;
	OBJ_DATA *material = NULL;
	int value, bonus;
	
		if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_makearmor] )
	{
		send_to_char("> you do not know the codedef skillsoft\n\r", ch );
		return;
	}

	argument = one_argument( argument, arg );
	strcpy ( arg2, argument);


	switch( ch->substate )
	{
	default:

		if ( str_cmp( arg, "body" )
				&& str_cmp( arg, "head" )
				&& str_cmp( arg, "legs" )
				&& str_cmp( arg, "feet" )
				&& str_cmp( arg, "hands" )
				&& str_cmp( arg, "arms" )
				&& str_cmp( arg, "about" )
				&& str_cmp( arg, "waist" ) )
		{
			send_to_char( "&R> you cannot make a defensive module for that body part\n\r&w", ch);
			send_to_char( "&R> try: body, head, legs, feet, hands, arms, about, or waist\n\r&w", ch);
			return;
		}

		if ( arg2[0] == '\0' )
		{
			send_to_char( "&R> syntax: codedef <wearloc> <name>\n\r&w", ch);
			return;
		}

		checksew = FALSE;
		checkfab = FALSE;

		if ( !IS_SET( ch->in_room->room_flags, ROOM_RESTAURANT ) )
		{
			send_to_char( "> &Ryou need to be in a coding node&w\n\r", ch );
			return;
		}
		else {
				if ( ch->in_room->level != 0 )
				bonus = ch->in_room->level * 10;
		}

		for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
		{
			if (obj->item_type == ITEM_FABRIC)
				checkfab = TRUE;
			if (obj->item_type == ITEM_THREAD)
				checksew = TRUE;
		}

		if ( !checkfab )
		{
			send_to_char( "&R> you need some kind of defensive class\n\r", ch);
			return;
		}

		if ( !checksew )
		{
			send_to_char( "&R> you need a parser\n\r", ch);
			return;
		}

		chance = IS_NPC(ch) ? ch->top_level
				: (int) (ch->pcdata->learned[gsn_makearmor]);
		if ( number_percent( ) < chance )
		{
			send_to_char( "&G> you begin the long process of coding a defensive module\n\r", ch);
			if ( bonus > 0 )
			ch_printf( ch , "node bonus: %d\n\r\n\r", bonus);
			act( AT_PLAIN, "> $n takes $s parser, some code fragments and begins to work", ch,
					NULL, argument , TO_ROOM );
			add_timer ( ch , TIMER_DO_FUN , 2 , do_makearmor , 1 ); // 10 to 2
			ch->dest_buf = str_dup(arg);
			ch->dest_buf_2 = str_dup(arg2);
			return;
		}
		send_to_char("&R> you cannot figure out what to do\n\r",ch);
		learn_from_failure( ch, gsn_makearmor );
		return;

	case 1:
		if ( !ch->dest_buf )
			return;
		if ( !ch->dest_buf_2 )
			return;
		strcpy(arg, (const char*) ch->dest_buf);
		DISPOSE( ch->dest_buf);
		strcpy(arg2, (const char*) ch->dest_buf_2);
		DISPOSE( ch->dest_buf_2);
		break;

	case SUB_TIMER_DO_ABORT:
		DISPOSE( ch->dest_buf );
		DISPOSE( ch->dest_buf_2 );
		ch->substate = SUB_NONE;
		send_to_char("&R> you are interrupted and fail to finish your work\n\r", ch);
		return;
	}

	ch->substate = SUB_NONE;

	level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makearmor]);

	checksew = FALSE;
	checkfab = FALSE;

	for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
	{
		if (obj->item_type == ITEM_THREAD)
			checksew = TRUE;
		if (obj->item_type == ITEM_FABRIC && checkfab == FALSE)
		{
			checkfab = TRUE;
			separate_obj( obj );
			obj_from_char( obj );
			material = obj;
		}
	}

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makearmor]) ;


	if ( number_percent( ) > chance*2  || ( !checkfab ) || ( !checksew ) )
	{
		send_to_char( "> &Ryou fail to code the def module&w\n\r", ch);
		learn_from_failure( ch, gsn_makearmor );
		return;
	}

	obj = material;

	obj->item_type = ITEM_ARMOR;
	SET_BIT( obj->wear_flags, ITEM_TAKE );
	value = get_wflag( arg );
	if ( value < 0 || value > 31 )
		SET_BIT( obj->wear_flags, ITEM_WEAR_BODY );
	else
		SET_BIT( obj->wear_flags, 1 << value );
	obj->level = level;
	STRFREE( obj->name );
	strcpy( buf, arg2 );
	obj->name = STRALLOC( buf );
	strcpy( buf, arg2 );
	strcat( buf, " [def]" );
	STRFREE( obj->short_descr );
	obj->short_descr = STRALLOC( buf );
	sprintf( buf1," [coder: %s]", ch->name);
	strcat( buf, buf1 );
	STRFREE( obj->description );
	obj->description = STRALLOC( buf );
	obj->value[0] = obj->value[1];
	obj->cost *= 10;

	obj = obj_to_char( obj, ch );

	send_to_char( "&G> you finish coding and look at your newly created defensive module&w\n\r", ch);
	act( AT_PLAIN, "> $n finishes coding a new defensive module", ch,
			NULL, argument , TO_ROOM );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more intelligent than before\n\r", ch );
		ch->perm_int++;
		ch->perm_int = UMIN( ch->perm_int , 25 );
	}

	learn_from_success( ch, gsn_makearmor );
	learn_from_success( ch, gsn_makearmor );
}


void do_makeshield( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int chance;
	bool checktool, checkbatt, checkcond, checkcirc, checkgems;
	OBJ_DATA *obj;
	OBJ_INDEX_DATA *pObjIndex;
	int bonus, vnum, level, charge, gemtype = 0;
	
		if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_makeshield] )
	{
		send_to_char("> you do not know the codeshield skillsoft\n\r", ch );
		return;
	}

	strcpy( arg, argument );

	switch( ch->substate )
	{
	default:
		if ( arg[0] == '\0' )
		{
			send_to_char( "&R> syntax: codeshield <name>\n\r&w", ch);
			return;
		}

		checktool = FALSE;
		checkbatt = FALSE;
		checkcond = FALSE;
		checkgems = FALSE;

		if ( !IS_SET( ch->in_room->room_flags, ROOM_RESTAURANT ) )
		{
			send_to_char( "> &Ryou need to be in a coding node&w\n\r", ch );
			return;
		}
		else {
				if ( ch->in_room->level != 0 )
				bonus = ch->in_room->level * 10;
		}

		for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
		{
			if (obj->item_type == ITEM_TOOLKIT)
				checktool = TRUE;
			if (obj->item_type == ITEM_CRYSTAL)
				checkgems = TRUE;
			if (obj->item_type == ITEM_BATTERY)
				checkbatt = TRUE;
			if (obj->item_type == ITEM_CIRCUIT)
				checkcirc = TRUE;
			if (obj->item_type == ITEM_SUPERCONDUCTOR)
				checkcond = TRUE;
		}

		if ( !checktool )
		{
			send_to_char( "&R> you need a devkit\n\r", ch);
			return;
		}

		if ( !checkbatt )
		{
			send_to_char( "&R> you need an utility patch\n\r", ch);
			return;
		}

		if ( !checkcirc )
		{
			send_to_char( "&R> you need a function variant one\n\r", ch);
			return;
		}

		if ( !checkcond )
		{
			send_to_char( "&R> you need a function variant two\n\r", ch);
			return;
		}

		if ( !checkgems )
		{
			send_to_char( "&R> you need an utility class\n\r", ch);
			return;
		}

		chance = IS_NPC(ch) ? ch->top_level
				: (int) (ch->pcdata->learned[gsn_makeshield]);
		if ( number_percent( ) < chance )
		{
			send_to_char( "&G> you begin the long process of coding a shield module\n\r", ch);
			if ( bonus > 0 )
			ch_printf( ch , "node bonus: %d\n\r\n\r", bonus);
			act( AT_PLAIN, "> $n takes $s devkit and begins to work on something", ch,
					NULL, argument , TO_ROOM );
			add_timer ( ch , TIMER_DO_FUN , 2 , do_makeshield , 1 );
			ch->dest_buf = str_dup(arg);
			return;
		}
		send_to_char("&R> you cannot figure out how to fit the parts together\n\r",ch);
		learn_from_failure( ch, gsn_makeshield );
		return;

	case 1:
		if ( !ch->dest_buf )
			return;
		strcpy(arg, (const char*) ch->dest_buf);
		DISPOSE( ch->dest_buf);
		break;

	case SUB_TIMER_DO_ABORT:
		DISPOSE( ch->dest_buf );
		ch->substate = SUB_NONE;
		send_to_char("&R> you are interrupted and fail to finish your work\n\r", ch);
		return;
	}

	ch->substate = SUB_NONE;

	level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makeshield]);
	vnum = OBJ_VNUM_PROTO_SHIELD;

	if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
	{
		send_to_char( "&R> the item you are trying to create is missing from the database\n\rPlease inform the administration of this error\n\r", ch );
		return;
	}

	checktool = FALSE;
	checkbatt = FALSE;
	checkcond = FALSE;
	checkcirc = FALSE;
	checkgems = FALSE;
	charge = 0;

	for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
	{
		if (obj->item_type == ITEM_TOOLKIT)
			checktool = TRUE;

		if (obj->item_type == ITEM_BATTERY && checkbatt == FALSE)
		{
			charge = UMIN(obj->value[1], 10);
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkbatt = TRUE;
		}
		if (obj->item_type == ITEM_SUPERCONDUCTOR && checkcond == FALSE)
		{
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkcond = TRUE;
		}
		if (obj->item_type == ITEM_CIRCUIT && checkcirc == FALSE)
		{
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkcirc = TRUE;
		}
		if (obj->item_type == ITEM_CRYSTAL && checkgems == FALSE)
		{
			gemtype = obj->value[0];
			separate_obj( obj );
			obj_from_char( obj );
			extract_obj( obj );
			checkgems = TRUE;
		}
	}

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makeshield]) ;


	if ( number_percent( ) > chance*2  || ( !checktool ) || ( !checkbatt )
			|| ( !checkgems ) || ( !checkcond ) || ( !checkcirc) )

	{
		send_to_char( "> &Ryou fail to code the shield module&w\n\r", ch);
		learn_from_failure( ch, gsn_makeshield );
		return;
	}

	obj = create_object( pObjIndex, level );

	obj->item_type = ITEM_ARMOR;
	SET_BIT( obj->wear_flags, ITEM_WIELD );
	SET_BIT( obj->wear_flags, ITEM_WEAR_SHIELD );
	obj->level = level;
	obj->weight = 2;
	STRFREE( obj->name );
	strcpy( buf, arg );
	strcat( buf, " shield module" );
	obj->name = STRALLOC( buf );
	strcpy( buf, arg );
	strcat( buf, " [shield]" );
	STRFREE( obj->short_descr );
	obj->short_descr = STRALLOC( buf );
	STRFREE( obj->description );
	obj->description = STRALLOC( buf );
	obj->value[0] = (int) (level/10+gemtype*2);      /* condition */
	obj->value[1] = (int) (level/10+gemtype*2);      /* armor */
	obj->value[4] = charge;
	obj->value[5] = charge;
	obj->cost = obj->value[2]*100;

	obj = obj_to_char( obj, ch );

	send_to_char( "&G> you finish coding and look at your newly created shield module&w\n\r", ch);
	act( AT_PLAIN, "> $n finishes coding $s new shield module", ch,
			NULL, argument , TO_ROOM );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more intelligent than before\n\r", ch );
		ch->perm_int++;
		ch->perm_int = UMIN( ch->perm_int , 25 );
	}

	learn_from_success( ch, gsn_makeshield );
	learn_from_success( ch, gsn_makeshield );

}

void do_makecontainer( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int level, chance;
	bool checksew, checkfab;
	OBJ_DATA *obj;
	OBJ_DATA *material;
	int value, bonus;
	
		if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_makecontainer] )
	{
		send_to_char("> you do not know the codecontainer skillsoft\n\r", ch );
		return;
	}

	argument = one_argument( argument, arg );
	strcpy( arg2 , argument );


	switch( ch->substate )
	{
	default:

		if ( str_cmp( arg, "body" )
				&& str_cmp( arg, "legs" )
				&& str_cmp( arg, "about" )
				&& str_cmp( arg, "waist" )
				&& str_cmp( arg, "hold" ) )
		{
			send_to_char( "&R> you cannot code a container for that body part\n\r&w", ch);
			send_to_char( "&R> try body, legs, about, waist or hold\n\r&w", ch);
			return;
		}

		if ( arg2[0] == '\0' )
		{
			send_to_char( "&R> syntax: codecontainer <wearloc> <name>\n\r&w", ch);
			return;
		}

		checksew = FALSE;
		checkfab = FALSE;

		if ( !IS_SET( ch->in_room->room_flags, ROOM_RESTAURANT ) )
		{
			send_to_char( "> &Ryou need to be in a coding node&w\n\r", ch );
			return;
		}
		else {
				if ( ch->in_room->level != 0 )
				bonus = ch->in_room->level * 10;
		}

		for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
		{
			if (obj->item_type == ITEM_FABRIC)
				checkfab = TRUE;
			if (obj->item_type == ITEM_THREAD)
				checksew = TRUE;
		}

		if ( !checkfab )
		{
			send_to_char( "&R> you need some sort of def class\n\r", ch);
			return;
		}

		if ( !checksew )
		{
			send_to_char( "&R> you need a parser\n\r", ch);
			return;
		}

		chance = IS_NPC(ch) ? ch->top_level
				: (int) (ch->pcdata->learned[gsn_makecontainer]);
		if ( number_percent( ) < chance )
		{
			send_to_char( "&G> you begin the long process of coding a container module\n\r", ch);
			if ( bonus > 0 )
			ch_printf( ch , "node bonus: %d\n\r\n\r", bonus);
			act( AT_PLAIN, "> $n takes $s parser and some code and begins to work", ch,
					NULL, argument , TO_ROOM );
			add_timer ( ch , TIMER_DO_FUN , 2 , do_makecontainer , 1 );
			ch->dest_buf = str_dup(arg);
			ch->dest_buf_2 = str_dup(arg2);
			return;
		}
		send_to_char("&R> you cannot figure out what to do\n\r",ch);
		learn_from_failure( ch, gsn_makecontainer );
		return;

	case 1:
		if ( !ch->dest_buf )
			return;
		if ( !ch->dest_buf_2 )
			return;
		strcpy(arg, (const char*) ch->dest_buf);
		DISPOSE( ch->dest_buf);
		strcpy(arg2, (const char*) ch->dest_buf_2);
		DISPOSE( ch->dest_buf_2);
		break;

	case SUB_TIMER_DO_ABORT:
		DISPOSE( ch->dest_buf );
		DISPOSE( ch->dest_buf_2 );
		ch->substate = SUB_NONE;
		send_to_char("&R> you are interrupted and fail to finish your work\n\r", ch);
		return;
	}

	ch->substate = SUB_NONE;

	level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makecontainer]);

	checksew = FALSE;
	checkfab = FALSE;

	for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
	{
		if (obj->item_type == ITEM_THREAD)
			checksew = TRUE;
		if (obj->item_type == ITEM_FABRIC && checkfab == FALSE)
		{
			checkfab = TRUE;
			separate_obj( obj );
			obj_from_char( obj );
			material = obj;
		}
	}

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makecontainer]) ;

	if ( number_percent( ) > chance*2  || ( !checkfab ) || ( !checksew ) )
	{
		send_to_char( "> &Ryou fail to code the container module&w\n\r", ch);
		learn_from_failure( ch, gsn_makecontainer );
		return;
	}

	obj = material;

	obj->item_type = ITEM_CONTAINER;
	SET_BIT( obj->wear_flags, ITEM_TAKE );
	value = get_wflag( arg );
	if ( value < 0 || value > 31 )
		SET_BIT( obj->wear_flags, ITEM_HOLD );
	else
		SET_BIT( obj->wear_flags, 1 << value );
	obj->level = level;
	STRFREE( obj->name );
	strcpy( buf, arg2 );
	obj->name = STRALLOC( buf );
	strcpy( buf, arg2 );
	strcat( buf, " [cont]" );
	STRFREE( obj->short_descr );
	obj->short_descr = STRALLOC( buf );
	STRFREE( obj->description );
	obj->description = STRALLOC( buf );
	obj->value[0] = level;
	obj->value[1] = 0;
	obj->value[2] = 0;
	obj->value[3] = 10;
	obj->cost *= 2;

	obj = obj_to_char( obj, ch );

	send_to_char( "&G> you finish coding and look at your newly created container module&w\n\r", ch);
	act( AT_PLAIN, "> $n finishes coding a new container module", ch,
			NULL, argument , TO_ROOM );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more intelligent than before\n\r", ch );
		ch->perm_int++;
		ch->perm_int = UMIN( ch->perm_int , 25 );
	}

	learn_from_success( ch, gsn_makecontainer );
	learn_from_success( ch, gsn_makecontainer );
	learn_from_success( ch, gsn_makecontainer );
}

void do_reinforcements( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *och;
	char arg[MAX_INPUT_LENGTH];
	int chance, credits, count;

	if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_reinforcements] )
	{
		send_to_char("> you do not know the reinforcements skillsoft\n\r", ch );
		return;
	}

	if ( IS_NPC( ch ) || !ch->pcdata )
		return;

	strcpy( arg, argument );

	switch( ch->substate )
	{
	default:
		if ( ch->backup_wait )
		{
			send_to_char( "&R> your reinforcements are already on the way\n\r", ch );
			return;
		}

		if ( !ch->pcdata->clan )
		{
			send_to_char( "&R> you need to be a member of an organization before you can call for reinforcements\n\r", ch );
			return;
		}

		if ( ch->gold < 5000 )
		{
			ch_printf( ch, "&R> you dont have enough credits to send for reinforcements\n\r" );
			return;
		}

		count = 0;
		for ( och = ch->in_room->first_person; och; och = och->next_in_room )
		{
			if ( IS_AFFECTED(och, AFF_CHARM)
					&&   och->master == ch )
				count++;
		}
		if( count >= 12 )
		{
			send_to_char("> you have enough reinforcements\n\r", ch );
			return;
		}


		chance = (int) (ch->pcdata->learned[gsn_reinforcements]);
		if ( number_percent( ) < chance )
		{
			send_to_char( "&G> you begin making the call for reinforcements\n\r", ch);
			act( AT_PLAIN, "> $n begins issuing orders into $s comlink", ch,
					NULL, argument , TO_ROOM );
			add_timer ( ch , TIMER_DO_FUN , 1 , do_reinforcements , 1 );
			ch->dest_buf = str_dup(arg);
			return;
		}
		send_to_char("&R> you call for reinforcements but nobody answers\n\r",ch);
		learn_from_failure( ch, gsn_reinforcements );
		return;

	case 1:
		if ( !ch->dest_buf )
			return;
		strcpy(arg, (const char*) ch->dest_buf);
		DISPOSE( ch->dest_buf);
		break;

	case SUB_TIMER_DO_ABORT:
		DISPOSE( ch->dest_buf );
		ch->substate = SUB_NONE;
		send_to_char("&R> you are interupted before you can finish your call\n\r", ch);
		return;
	}

	ch->substate = SUB_NONE;

	send_to_char( "&G> your reinforcements are on the way\n\r", ch);
	credits = 5000;
	ch_printf( ch, "> it cost you %d credits\n\r", credits);
	ch->gold -= UMIN( credits , ch->gold );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more charming than before\n\r", ch );
		ch->perm_cha++;
		ch->perm_cha = UMIN( ch->perm_cha , 25 );
	}

	learn_from_success( ch, gsn_reinforcements );
	learn_from_success( ch, gsn_reinforcements );

	ch->backup_mob = MOB_VNUM_SOLDIER;

	ch->backup_wait = 1;

}

void do_postguard( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	int chance, credits;
	
		if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_postguard] )
	{
		send_to_char("> you do not know the postguard skillsoft\n\r", ch );
		return;
	}

	if ( IS_NPC( ch ) || !ch->pcdata )
		return;

	strcpy( arg, argument );

	switch( ch->substate )
	{
	default:
		if ( ch->backup_wait )
		{
			send_to_char( "&R> you already have backup coming\n\r", ch );
			return;
		}

		if ( !ch->pcdata->clan )
		{
			send_to_char( "&R> you need to be a member of an organization before you can call for a guard\n\r", ch );
			return;
		}

		if ( !ch->in_room || !ch->in_room->area ||
				( ch->in_room->area->planet && ch->in_room->area->planet->governed_by != ch->pcdata->clan ) )
		{
			send_to_char( "&R> you cannot post guards in enemy systems - try calling for reinforcements instead\n\r", ch );
			return;
		}

		if ( ch->gold < 5000 )
		{
			ch_printf( ch, "&R> you do not have enough credits\n\r", ch );
			return;
		}

		chance = (int) (ch->pcdata->learned[gsn_postguard]);
		if ( number_percent( ) < chance )
		{
			send_to_char( "&G> you begin making the call for reinforcements\n\r", ch);
			act( AT_PLAIN, "> $n begins issuing orders into $s comlink", ch,
					NULL, argument , TO_ROOM );
			add_timer ( ch , TIMER_DO_FUN , 1 , do_postguard , 1 );
			ch->dest_buf = str_dup(arg);
			return;
		}
		send_to_char("&R> you call for a guard but nobody answers\n\r",ch);
		learn_from_failure( ch, gsn_postguard );
		return;

	case 1:
		if ( !ch->dest_buf )
			return;
		strcpy(arg, ch->dest_buf);
		DISPOSE( ch->dest_buf);
		break;

	case SUB_TIMER_DO_ABORT:
		DISPOSE( ch->dest_buf );
		ch->substate = SUB_NONE;
		send_to_char("&R> you are interrupted before you can finish your call\n\r", ch);
		return;
	}

	ch->substate = SUB_NONE;

	send_to_char( "&G> your guard is on the way\n\r", ch);

	credits = 5000;
	ch_printf( ch, "> it cost you %d credits\n\r", credits);
	ch->gold -= UMIN( credits , ch->gold );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more convincing than before\n\r", ch );
		ch->perm_cha++;
		ch->perm_cha = UMIN( ch->perm_cha , 25 );
	}

	learn_from_success( ch, gsn_postguard );
	learn_from_success( ch, gsn_postguard );

	ch->backup_mob = MOB_VNUM_GUARD;

	ch->backup_wait = 1;

}

void add_reinforcements( CHAR_DATA *ch )
{
	MOB_INDEX_DATA  * pMobIndex;
	OBJ_DATA        * blaster;
	OBJ_INDEX_DATA  * pObjIndex;
	char buf[MAX_STRING_LENGTH];

	if ( !ch->in_room )
		return;

	if ( ( pMobIndex = get_mob_index( ch->backup_mob ) ) == NULL )
		return;

	if ( ch->backup_mob == MOB_VNUM_SOLDIER       )
	{
		CHAR_DATA * mob[3];
		int         mob_cnt;

		send_to_char( "> your reinforcements have arrived\n\r", ch );
		for ( mob_cnt = 0 ; mob_cnt < 3 ; mob_cnt++ )
		{
			mob[mob_cnt] = create_mobile( pMobIndex );
			if ( !mob[mob_cnt] )
				return;
			char_to_room( mob[mob_cnt], ch->in_room );
			act( AT_IMMORT, "> $N has arrived", ch, NULL, mob[mob_cnt], TO_ROOM );
			mob[mob_cnt]->top_level = 50;
			mob[mob_cnt]->hit = 100;
			mob[mob_cnt]->max_hit = 100;
			mob[mob_cnt]->armor = 50;
			mob[mob_cnt]->damroll = 0;
			mob[mob_cnt]->hitroll = 10;
			if ( ( pObjIndex = get_obj_index( OBJ_VNUM_BLASTER ) ) != NULL )
			{
				blaster = create_object( pObjIndex, mob[mob_cnt]->top_level );
				obj_to_char( blaster, mob[mob_cnt] );
				equip_char( mob[mob_cnt], blaster, WEAR_WIELD );
			}

			if ( mob[mob_cnt]->master )
				stop_follower( mob[mob_cnt] );
			add_follower( mob[mob_cnt], ch );
			SET_BIT( mob[mob_cnt]->affected_by, AFF_CHARM );
			do_setblaster( mob[mob_cnt] , "full" );
			if ( ch->pcdata && ch->pcdata->clan )
				mob[mob_cnt]->mob_clan = ch->pcdata->clan;
		}
	}
	else if ( ch->backup_mob == MOB_VNUM_WORKMATE       )
	{
		CHAR_DATA *mob;

		send_to_char( "> your workmate has loaded\n\r", ch );

			mob = create_mobile( pMobIndex );
			if ( !mob )
				return;

			// name it

			strcpy( buf , ch->pcdata->wm_name );

			STRFREE( mob->name );
			mob->name	= STRALLOC( buf );

			strcat( buf ," [wm]" );

			STRFREE( mob->long_descr );
			mob->long_descr	= STRALLOC( buf );

			STRFREE( mob->short_descr );
			mob->short_descr	= STRALLOC( buf );

			char_to_room( mob, ch->in_room );
			act( AT_IMMORT, "> $N has loaded", ch, NULL, mob, TO_ROOM );
			mob->top_level = 10 * ch->pcdata->wm_top_level;
			mob->hit = 10 * ch->pcdata->wm_top_level;
			mob->max_hit = 10 * ch->pcdata->wm_top_level;
			mob->damroll = ch->pcdata->wm_top_level;
			mob->hitroll = ch->pcdata->wm_top_level;

			mob->perm_str = ch->pcdata->wm_str;
			mob->mod_str = ch->pcdata->wm_str;
			mob->perm_dex = ch->pcdata->wm_dex;
			mob->mod_dex = ch->pcdata->wm_dex;
			mob->perm_con = ch->pcdata->wm_con;
			mob->mod_con = ch->pcdata->wm_con;
			mob->perm_int = ch->pcdata->wm_int;
			mob->mod_int = ch->pcdata->wm_int;
			mob->perm_wis = ch->pcdata->wm_wis;
			mob->mod_wis = ch->pcdata->wm_wis;
			mob->perm_cha = ch->pcdata->wm_cha;
			mob->mod_cha = ch->pcdata->wm_cha;

			if ( mob->master )
				stop_follower( mob );
			add_follower( mob, ch );
			SET_BIT( mob->affected_by, AFF_CHARM );
			if ( ch->pcdata && ch->pcdata->clan )
				mob->mob_clan = ch->pcdata->clan;
	}
	else
	{
		CHAR_DATA *mob;

		mob = create_mobile( pMobIndex );
		char_to_room( mob, ch->in_room );
		if ( ch->pcdata && ch->pcdata->clan )
		{
			char tmpbuf[MAX_STRING_LENGTH];

			sprintf( tmpbuf , "> a guard ICE [%s] stands at attention\n\r" , ch->pcdata->clan->name );
			STRFREE( mob->long_descr );
			mob->long_descr = STRALLOC( tmpbuf );
		}
		act( AT_IMMORT, "$N has arrived", ch, NULL, mob, TO_ROOM );
		send_to_char( "> your guard has arrived\n\r", ch );
		mob->top_level = 75;
		mob->hit = 200;
		mob->max_hit = 200;
		mob->armor = 0;
		mob->damroll = 5;
		mob->hitroll = 20;
		if ( ( pObjIndex = get_obj_index( OBJ_VNUM_BLASTER ) ) != NULL )
		{
			blaster = create_object( pObjIndex, mob->top_level );
			obj_to_char( blaster, mob );
			equip_char( mob, blaster, WEAR_WIELD );
		}
		do_setblaster( mob , "full" );
		if ( ch->pcdata && ch->pcdata->clan )
			mob->mob_clan = ch->pcdata->clan;
	}
}

//void do_torture( CHAR_DATA *ch, char *argument )
//{
//	char arg[MAX_INPUT_LENGTH];
//	CHAR_DATA *victim;
//	int chance, dam;
//	bool fail;
//
//	if ( !IS_NPC(ch)
//			&&  ch->pcdata->learned[gsn_torture] <= 0  )
//	{
//		send_to_char(
//				"> your mind races as you realize you have no idea how to do that\n\r", ch );
//		return;
//	}
//
//	if ( IS_NPC(ch) && IS_AFFECTED( ch, AFF_CHARM ) )
//	{
//		send_to_char( "> you cannot do that right now\n\r", ch );
//		return;
//	}
//
//	one_argument( argument, arg );
//
//	if ( ch->mount )
//	{
//		send_to_char( "> you cannot get close enough while mounted\n\r", ch );
//		return;
//	}
//
//	if ( arg[0] == '\0' )
//	{
//		send_to_char( "Torture whom?\n\r", ch );
//		return;
//	}
//
//	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
//	{
//		send_to_char( "> they aren't here\n\r", ch );
//		return;
//	}
//
//	if ( victim == ch )
//	{
//		send_to_char( "> are you masochistic or what\n\r", ch );
//		return;
//	}
//
//	if ( !IS_AWAKE(victim) )
//	{
//		send_to_char( "> you need to wake them first\n\r", ch );
//		return;
//	}
//
//	if ( is_safe( ch, victim ) )
//		return;
//
//	if ( victim->fighting )
//	{
//		send_to_char( "> you cannot torture someone whos in combat\n\r", ch );
//		return;
//	}
//
//	ch->alignment = ch->alignment -= 100;
//	ch->alignment = URANGE( -1000, ch->alignment, 1000 );
//
//	WAIT_STATE( ch, skill_table[gsn_torture]->beats );
//
//	fail = FALSE;
//	chance = ris_save( victim, IS_NPC(ch) ? ch->top_level : ch->pcdata->learned[gsn_torture], RIS_PARALYSIS );
//	if ( chance == 1000 )
//		fail = TRUE;
//	else
//		fail = saves_para_petri( chance, victim );
//
//	chance = 5;
//	if ( !fail
//			&& (  IS_NPC(ch)
//					|| (number_percent( ) + chance) < ch->pcdata->learned[gsn_torture] ) )
//	{
//		learn_from_success( ch, gsn_torture );
//		WAIT_STATE( ch,     2 * PULSE_VIOLENCE );
//		WAIT_STATE( victim, PULSE_VIOLENCE );
//		act( AT_SKILL, "$N slowly tortures you. The pain is excruciating", victim, NULL, ch, TO_CHAR );
//		act( AT_SKILL, "> you torture $N, leaving $M screaming in pain", ch, NULL, victim, TO_CHAR );
//		act( AT_SKILL, "> $n tortures $N, leaving $M screaming in agony!", ch, NULL, victim, TO_NOTVICT );
//
//		dam = dice( (IS_NPC(ch) ? ch->top_level : ch->pcdata->learned[gsn_torture])/10 , 4 );
//		dam = URANGE( 0, victim->max_hit-10, dam );
//		victim->hit -= dam;
//		victim->max_hit -= dam;
//
//		ch_printf( victim, "> you lose %d permanent hit points" ,dam);
//		ch_printf( ch, "> they lose %d permanent hit points" , dam);
//
//	}
//	else
//	{
//		act( AT_SKILL, "$N tries to cut off your finger!", victim, NULL, ch, TO_CHAR );
//		act( AT_SKILL, "> you mess up big time", ch, NULL, victim, TO_CHAR );
//		act( AT_SKILL, "> $n tries to painfully torture $N", ch, NULL, victim, TO_NOTVICT );
//		WAIT_STATE( ch,     2 * PULSE_VIOLENCE );
//		global_retcode = multi_hit( victim, ch, TYPE_UNDEFINED );
//	}
//	return;
//
//}

void do_disguise( CHAR_DATA *ch, char *argument )
{
	int chance;

	if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_disguise] )
	{
		send_to_char("> you do not know the disguise skillsoft\n\r", ch );
		return;
	}

	if ( IS_NPC(ch) )
		return;

	if ( IS_SET( ch->pcdata->flags, PCFLAG_NOTITLE ))
	{
		send_to_char( "> you try but the Force resists you\n\r", ch );
		return;
	}

	if ( argument[0] == '\0' )
	{
		send_to_char( "Change your title to what?\n\r", ch );
		return;
	}

	if ( !check_parse_name( argument ) )
	{
		send_to_char( "> &Rinvalid disguise name&w\n\r", ch );
		return;
	}

	//if ( !str_cmp( argument, "Wintermute" ) || !str_cmp( argument, "Wintermute " ) )
	
	if (!IS_IMMORTAL(ch)){
	if ( nifty_is_name( "Wintermute", argument) )
	{
		ch_printf( ch, "> They would not like that\n\r", argument );
		return;
	}
}

	chance = (int) (ch->pcdata->learned[gsn_disguise]);

	if ( number_percent( ) > chance )
	{
		send_to_char( "> you try to disguise yourself but fail\n\r", ch );
		return;
	}

	if ( strlen(argument) > 50 )
		argument[50] = '\0';

	smash_tilde( argument );
	set_title( ch, argument );
	send_to_char( "Ok\n\r", ch );
	learn_from_success( ch , gsn_disguise );

}

void do_first_aid( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA   *medpac;
	CHAR_DATA  *victim;
	int         heal;
	char        buf[MAX_STRING_LENGTH];
	
		if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_first_aid] )
	{
		send_to_char("> you do not know the firstaid skillsoft\n\r", ch );
		return;
	}

	if ( ch->position == POS_FIGHTING )
	{
		send_to_char( "> you cannot do that while fighting\n\r",ch );
		return;
	}

	medpac = get_eq_char( ch, WEAR_HOLD );
	if ( !medpac || medpac->item_type != ITEM_MEDPAC )
	{
		send_to_char( "> you need to be holding a medmod\n\r",ch );
		return;
	}

	   if ( IS_SET(ch->in_room->room_flags, ROOM_ARENA) )
	   {
		send_to_char("> &Rmedmods are not allowed in the arena&w\n\r", ch);
		return;
	   }

	if ( medpac->value[0] <= 0 )
	{
		send_to_char( "> your medmod seems to be empty\n\r",ch );
		return;
	}

	if ( argument[0] == '\0' )
		victim = ch;
	else
		victim = get_char_room( ch, argument );

	if ( !victim )
	{
		ch_printf( ch, "> entity not found\n\r" , argument );
		return;
	}

	heal = number_range( 1, 150 );

	if ( heal > ch->pcdata->learned[gsn_first_aid]*2 )
	{
		ch_printf( ch, "> you fail in your attempt at first aid\n\r");
		learn_from_failure( ch , gsn_first_aid );
		return;
	}

	if ( victim == ch )
	{
		ch_printf( ch, "> you tend to your wounds\n\r");
		sprintf( buf , "> $n uses %s to help heal $s wounds" , medpac->short_descr );
		act( AT_ACTION, buf, ch, NULL, victim, TO_ROOM );
	}
	else
	{
		sprintf( buf , "> you tend to $N's wounds" );
		act( AT_ACTION, buf, ch, NULL, victim, TO_CHAR );
		sprintf( buf , "> $n uses %s to help heal $N's wounds" , medpac->short_descr );
		act( AT_ACTION, buf, ch, NULL, victim, TO_NOTVICT );
		sprintf( buf , "> $n uses %s to help heal your wounds" , medpac->short_descr );
		act( AT_ACTION, buf, ch, NULL, victim, TO_VICT );
	}

	--medpac->value[0];
	victim->hit += URANGE ( 0, heal , victim->max_hit - victim->hit );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel wiser than before\n\r", ch );
		ch->perm_wis++;
		ch->perm_wis = UMIN( ch->perm_wis , 25 );
	}

	learn_from_success( ch , gsn_first_aid );
}

void do_snipe( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA        * wield;
	char              arg[MAX_INPUT_LENGTH];
	char              arg2[MAX_INPUT_LENGTH];
	sh_int            dir, dist;
	sh_int            max_dist = 3;
	EXIT_DATA       * pexit;
	ROOM_INDEX_DATA * was_in_room;
	ROOM_INDEX_DATA * to_room;
	CHAR_DATA       * victim = NULL;
	char              buf[MAX_STRING_LENGTH];
	bool              pfound = FALSE;

	if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
	{
		set_char_color( AT_MAGIC, ch );
		send_to_char( "> you'll have to do that elsewhere\n\r", ch );
		return;
	}

	if ( get_eq_char( ch, WEAR_DUAL_WIELD ) != NULL )
	{
		send_to_char( "> you cannot do that while wielding two weapons",ch );
		return;
	}

	wield = get_eq_char( ch, WEAR_WIELD );
	if ( !wield || wield->item_type != ITEM_WEAPON || wield->value[3] != WEAPON_BLASTER )
	{
		send_to_char( "> you dont seem to be holding a blaster",ch );
		return;
	}

	argument = one_argument( argument, arg );
	argument = one_argument( argument, arg2 );

	if ( ( dir = get_door( arg ) ) == -1 || arg2[0] == '\0' )
	{
		send_to_char( "> syntax: snipe <dir> <target>\n\r", ch );
		return;
	}

	if ( ( pexit = get_exit( ch->in_room, dir ) ) == NULL )
	{
		send_to_char( "> are you expecting to fire through a wall\n\r", ch );
		return;
	}

	if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
	{
		send_to_char( "> are you expecting to fire through a door\n\r", ch );
		return;
	}

	was_in_room = ch->in_room;

	for ( dist = 0; dist <= max_dist; dist++ )
	{
		if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
			break;

		if ( !pexit->to_room )
			break;

//	    to_room = NULL;
//	    if ( pexit->distance > 1 )
//	       to_room = generate_exit( ch->in_room , &pexit );
//
//	    if ( to_room == NULL )
		to_room = pexit->to_room;

		char_from_room( ch );
		char_to_room( ch, to_room );


		if ( IS_NPC(ch) && ( victim = get_char_room_mp( ch, arg2 ) ) != NULL )
		{
			pfound = TRUE;
			break;
		}
		else if ( !IS_NPC(ch) && ( victim = get_char_room( ch, arg2 ) ) != NULL )
		{
			pfound = TRUE;
			break;
		}


		if ( ( pexit = get_exit( ch->in_room, dir ) ) == NULL )
			break;

	}

	char_from_room( ch );
	char_to_room( ch, was_in_room );

	if ( !pfound )
	{
		ch_printf( ch, "> you do not see that person to the %s\n\r", dir_name[dir] );
		char_from_room( ch );
		char_to_room( ch, was_in_room );
		return;
	}

	if ( victim == ch )
	{
		send_to_char( "> invalid target\n\r", ch );
		return;
	}

    if ( IS_NPC(victim) && IS_SET( victim->act, ACT_IMMORTAL )  )
    {
	    send_to_char( "> this program cannot be flatlined\n\r", ch );
	    return;
    }

	if ( IS_SET( victim->in_room->room_flags, ROOM_SAFE ) )
	{
		set_char_color( AT_MAGIC, ch );
		send_to_char( "> you cannot shoot them there\n\r", ch );
		return;
	}

	if ( IS_SET( victim->in_room->room_flags, ROOM_PLR_HOME ) )
	{
		set_char_color( AT_MAGIC, ch );
		send_to_char( "> you cannot shoot them there\n\r", ch );
		return;
	}

	if ( !IS_NPC(victim) )
	{
    if ( get_age(victim) <= 20 && !IS_SET(ch->in_room->room_flags,ROOM_ARENA) )
    {
	send_to_char( "> that character is too new\n\r", ch );
	return;
    }
	}

	if ( is_safe( ch, victim ) )
		return;

	if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	{
		act( AT_PLAIN, "> > $N is your beloved master", ch, NULL, victim, TO_CHAR );
		return;
	}

	if ( ch->position == POS_FIGHTING )
	{
		send_to_char( "> you do the best you can\n\r", ch );
		return;
	}

	if ( !IS_NPC( victim ) && IS_SET( ch->act, PLR_NICE ) )
	{
		send_to_char( "> you feel too nice to do that\n\r", ch );
		return;
	}

	switch ( dir )
	{
	case 0:
	case 1:
		dir += 2;
		break;
	case 2:
	case 3:
		dir -= 2;
		break;
	case 4:
	case 7:
		dir += 1;
		break;
	case 5:
	case 8:
		dir -= 1;
		break;
	case 6:
		dir += 3;
		break;
	case 9:
		dir -=3;
		break;
	}

	char_from_room( ch );
	char_to_room( ch, victim->in_room );

	sprintf( buf , "> a blaster shot fires at you from the %s" , dir_name[dir] );
	act( AT_ACTION, buf , victim, NULL, ch, TO_CHAR );
	act( AT_ACTION, "> you fire at $N", ch, NULL, victim, TO_CHAR );
	sprintf( buf, "> a blaster shot fires at $N from the %s" , dir_name[dir] );
	act( AT_ACTION, buf, ch, NULL, victim, TO_NOTVICT );

	one_hit( ch, victim, TYPE_UNDEFINED );

	if ( char_died(ch) )
	{
		char_from_room( ch );
		char_to_room( ch, was_in_room );
		return;
	}

	stop_fighting( ch , TRUE );


	char_from_room( ch );
	char_to_room( ch, was_in_room );

	WAIT_STATE( ch, 1 * PULSE_VIOLENCE );

	if ( IS_NPC( victim ) && !char_died(victim) )
	{
		if ( IS_SET( victim->act , ACT_SENTINEL ) )
		{
			victim->was_sentinel = victim->in_room;
			REMOVE_BIT( victim->act, ACT_SENTINEL );
		}

		start_hating( victim , ch );
		start_hunting( victim, ch );

	}

}

/* syntax throw <obj> [direction] [target] */

//void do_throw( CHAR_DATA *ch, char *argument )
//{
//	OBJ_DATA        * obj;
//	OBJ_DATA        * tmpobj;
//	char              arg[MAX_INPUT_LENGTH];
//	char              arg2[MAX_INPUT_LENGTH];
//	char              arg3[MAX_INPUT_LENGTH];
//	sh_int            dir;
//	EXIT_DATA       * pexit;
//	ROOM_INDEX_DATA * was_in_room;
//	ROOM_INDEX_DATA * to_room;
//	CHAR_DATA       * victim;
//	char              buf[MAX_STRING_LENGTH];
//
//		if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_throw] )
//	{
//		send_to_char("> you do not know the throw skillsoft\n\r", ch );
//		return;
//	}
//
//	argument = one_argument( argument, arg );
//	argument = one_argument( argument, arg2 );
//	argument = one_argument( argument, arg3 );
//
//	was_in_room = ch->in_room;
//
//	if ( arg[0] == '\0' )
//	{
//		send_to_char( "> syntax: throw <object> [direction] [target]\n\r", ch );
//		return;
//	}
//
//
//	obj = get_eq_char( ch, WEAR_MISSILE_WIELD );
//	if ( !obj || !nifty_is_name( arg, obj->name ) )
//		obj = get_eq_char( ch, WEAR_HOLD );
//	if ( !obj || !nifty_is_name( arg, obj->name ) )
//		obj = get_eq_char( ch, WEAR_WIELD );
//	if ( !obj || !nifty_is_name( arg, obj->name ) )
//		obj = get_eq_char( ch, WEAR_DUAL_WIELD );
//	if ( !obj || !nifty_is_name( arg, obj->name ) )
//		if ( !obj || !nifty_is_name_prefix( arg, obj->name ) )
//			obj = get_eq_char( ch, WEAR_HOLD );
//	if ( !obj || !nifty_is_name_prefix( arg, obj->name ) )
//		obj = get_eq_char( ch, WEAR_WIELD );
//	if ( !obj || !nifty_is_name_prefix( arg, obj->name ) )
//		obj = get_eq_char( ch, WEAR_DUAL_WIELD );
//	if ( !obj || !nifty_is_name_prefix( arg, obj->name ) )
//	{
//		ch_printf( ch, "> you do not seem to be holding or wielding %s\n\r", arg );
//		return;
//	}
//
//	if ( IS_OBJ_STAT(obj, ITEM_NOREMOVE) )
//	{
//		act( AT_PLAIN, "> you cannot throw $p", ch, obj, NULL, TO_CHAR );
//		return;
//	}
//
//	if ( ch->position == POS_FIGHTING )
//	{
//		victim = who_fighting( ch );
//		if ( char_died ( victim ) )
//			return;
//		act( AT_ACTION, "> you throw $p at $N", ch, obj, victim, TO_CHAR );
//		act( AT_ACTION, "> $n throws $p at $N", ch, obj, victim, TO_NOTVICT );
//		act( AT_ACTION, "> $n throw $p at you", ch, obj, victim, TO_VICT );
//	}
//	else if ( arg2[0] == '\0' )
//	{
//		sprintf( buf, "> $n throws %s at the floor" , obj->short_descr );
//		act( AT_ACTION, buf, ch, NULL, NULL, TO_ROOM );
//		ch_printf( ch, "> you throw %s at the floor\n\r", obj->short_descr );
//
//		victim = NULL;
//	}
//	else  if ( ( dir = get_door( arg2 ) ) != -1 )
//	{
//		if ( ( pexit = get_exit( ch->in_room, dir ) ) == NULL )
//		{
//			send_to_char( "> are you expecting to throw it through a wall\n\r", ch );
//			return;
//		}
//
//
//		if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
//		{
//			send_to_char( "> are you expecting to throw it through a door\n\r", ch );
//			return;
//		}
//
//
//		switch ( dir )
//		{
//		case 0:
//		case 1:
//			dir += 2;
//			break;
//		case 2:
//		case 3:
//			dir -= 2;
//			break;
//		case 4:
//		case 7:
//			dir += 1;
//			break;
//		case 5:
//		case 8:
//			dir -= 1;
//			break;
//		case 6:
//			dir += 3;
//			break;
//		case 9:
//			dir -=3;
//			break;
//		}
//
////	      to_room = NULL;
////	      if ( pexit->distance > 1 )
////	       to_room = generate_exit( ch->in_room , &pexit );
////
////	      if ( to_room == NULL )
//		to_room = pexit->to_room;
//
//
//		char_from_room( ch );
//		char_to_room( ch, to_room );
//
//		victim = get_char_room( ch, arg3 );
//
//		if ( victim )
//		{
//			if ( is_safe( ch, victim ) )
//				return;
//
//			if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
//			{
//				act( AT_PLAIN, "> > $N is your beloved master", ch, NULL, victim, TO_CHAR );
//				return;
//			}
//
//			if ( !IS_NPC( victim ) && IS_SET( ch->act, PLR_NICE ) )
//			{
//				send_to_char( "> you feel too nice to do that\n\r", ch );
//				return;
//			}
//
//			if ( IS_SET( victim->in_room->room_flags, ROOM_SAFE ) )
//			{
//				set_char_color( AT_MAGIC, ch );
//				send_to_char( "> you cannot shoot them there\n\r", ch );
//				return;
//			}
//
//			if ( IS_SET( victim->in_room->room_flags, ROOM_PLR_HOME ) )
//			{
//				set_char_color( AT_MAGIC, ch );
//				send_to_char( "> you cannot shoot them there\n\r", ch );
//				return;
//			}
//
//			if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
//			{
//				set_char_color( AT_MAGIC, ch );
//				send_to_char( "> you'll have to do that elsewhere\n\r", ch );
//				return;
//			}
//
////	        to_room = NULL;
////	        if ( pexit->distance > 1 )
////	           to_room = generate_exit( ch->in_room , &pexit );
////
////	        if ( to_room == NULL )
//			to_room = pexit->to_room;
//
//
//            char_from_room( ch );
//            char_to_room( ch, to_room );
//
//   sprintf( buf, "> someone throws %s at you from the %s", obj->short_descr, dir_name[dir] );
//   act( AT_ACTION, buf, victim, NULL, ch, TO_CHAR );
//   act( AT_ACTION, "> you throw %p at $N", ch, obj, victim, TO_CHAR );
//   char_from_room( ch );
//   char_to_room( ch, was_in_room );
//   sprintf( buf, "> $n throws %s to the %s", obj->short_descr, dir_name[get_dir(arg2)] );
//   act( AT_ACTION, buf, ch, NULL, NULL, TO_ROOM );
//   char_from_room( ch );
//   char_to_room( ch, to_room );
//   sprintf( buf, "> %s is thrown at $N from the %s", obj->short_descr, dir_name[dir] );
//   act( AT_ACTION, buf, ch, NULL, victim, TO_NOTVICT );
//}
//else
//{
//   ch_printf( ch, "> you throw %s %s\r\n", obj->short_descr, dir_name[get_dir( arg2 )] );
//   char_from_room( ch );
//   char_to_room( ch, was_in_room );
//   sprintf( buf, "> $n throws %s to the %s", obj->short_descr, dir_name[get_dir(arg2)] );
//   act( AT_ACTION, buf, ch, NULL, NULL, TO_ROOM );
//   char_from_room( ch );
//   char_to_room( ch, to_room );
//   sprintf( buf, "> %s is thrown from the %s", obj->short_descr, dir_name[dir] );
//   act( AT_ACTION, buf, ch, NULL, NULL, TO_ROOM );
//   char_from_room( ch );
//   char_to_room( ch, was_in_room );
//}
//	}
//	else if ( ( victim = get_char_room( ch, arg2 ) ) != NULL )
//	{
//		if ( is_safe( ch, victim ) )
//			return;
//
//		if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
//		{
//			act( AT_PLAIN, "> $N is your beloved master", ch, NULL, victim, TO_CHAR );
//			return;
//		}
//
//		if ( !IS_NPC( victim ) && IS_SET( ch->act, PLR_NICE ) )
//		{
//			send_to_char( "> you feel too nice to do that\n\r", ch );
//			return;
//		}
//
//	}
//	else
//	{
//		ch_printf( ch, "> they dont seem to be here\n\r");
//		return;
//	}
//
//
//	if ( obj == get_eq_char( ch, WEAR_WIELD )
//			&& ( tmpobj = get_eq_char( ch, WEAR_DUAL_WIELD)) != NULL )
//		tmpobj->wear_loc = WEAR_WIELD;
//
//	unequip_char( ch, obj );
//	separate_obj( obj );
//	obj_from_char( obj );
//	obj = obj_to_room( obj, ch->in_room );
//
//	damage_obj ( obj );
//
//	/* NOT NEEDED UNLESS REFERING TO OBJECT AGAIN
//
//   if( obj_extracted(obj) )
//      return;
//	 */
//	if ( ch->in_room !=  was_in_room )
//	{
//		char_from_room( ch );
//		char_to_room( ch, was_in_room );
//	}
//
//	if ( !victim || char_died( victim ) )
//		learn_from_failure( ch, gsn_throw );
//	else
//	{
//
//		WAIT_STATE( ch, skill_table[gsn_throw]->beats );
//		if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_throw] )
//		{
//			learn_from_success( ch, gsn_throw );
//			global_retcode = damage( ch, victim, number_range( obj->weight*2 , (obj->weight*2 + ch->perm_str) ), TYPE_HIT );
//		}
//		else
//		{
//			learn_from_failure( ch, gsn_throw );
//			global_retcode = damage( ch, victim, 0, TYPE_HIT );
//		}
//
//		if ( IS_NPC( victim ) && !char_died ( victim) )
//		{
//			if ( IS_SET( victim->act , ACT_SENTINEL ) )
//			{
//				victim->was_sentinel = victim->in_room;
//				REMOVE_BIT( victim->act, ACT_SENTINEL );
//			}
//
//			start_hating( victim , ch );
//			start_hunting( victim, ch );
//
//		}
//
//	}
//
//	return;
//
//}

void do_throw( CHAR_DATA *ch, char *argument )
{
   OBJ_DATA        * obj;
   OBJ_DATA        * tmpobj;
   char              arg[MAX_INPUT_LENGTH];
   char              arg2[MAX_INPUT_LENGTH];
   char              arg3[MAX_INPUT_LENGTH];
   sh_int            dir;
   EXIT_DATA       * pexit;
   ROOM_INDEX_DATA * was_in_room;
   ROOM_INDEX_DATA * to_room;
   CHAR_DATA       * victim;
   char              buf[MAX_STRING_LENGTH];


   argument = one_argument( argument, arg );
   argument = one_argument( argument, arg2 );
   argument = one_argument( argument, arg3 );

   was_in_room = ch->in_room;

   if ( arg[0] == '\0' )
   {
     send_to_char( "> syntax: throw <object> [direction] [target]\n\r", ch );
     return;
   }


   obj = get_eq_char( ch, WEAR_MISSILE_WIELD );
   if ( !obj || !nifty_is_name( arg, obj->name ) )
      obj = get_eq_char( ch, WEAR_HOLD );
      if ( !obj || !nifty_is_name( arg, obj->name ) )
          obj = get_eq_char( ch, WEAR_WIELD );
          if ( !obj || !nifty_is_name( arg, obj->name ) )
              obj = get_eq_char( ch, WEAR_DUAL_WIELD );
              if ( !obj || !nifty_is_name( arg, obj->name ) )
   if ( !obj || !nifty_is_name_prefix( arg, obj->name ) )
      obj = get_eq_char( ch, WEAR_HOLD );
      if ( !obj || !nifty_is_name_prefix( arg, obj->name ) )
          obj = get_eq_char( ch, WEAR_WIELD );
          if ( !obj || !nifty_is_name_prefix( arg, obj->name ) )
              obj = get_eq_char( ch, WEAR_DUAL_WIELD );
              if ( !obj || !nifty_is_name_prefix( arg, obj->name ) )
   {
         ch_printf( ch, "> you do not seem to be holding or wielding %s\n\r", arg );
         return;
   }

    if ( IS_OBJ_STAT(obj, ITEM_NOREMOVE) )
    {
	act( AT_PLAIN, ">You can't throw $p", ch, obj, NULL, TO_CHAR );
	return;
    }

    if ( obj->pIndexData->vnum == 3 || obj->pIndexData->vnum == 4 || obj->pIndexData->vnum == 5 )
    {
	act( AT_PLAIN, ">You can't throw $p", ch, obj, NULL, TO_CHAR );
	return;
    }

   if ( ch->position == POS_FIGHTING )
   {
       victim = who_fighting( ch );
       if ( char_died ( victim ) )
           return;
       act( AT_ACTION, "> you throw $p at $N", ch, obj, victim, TO_CHAR );
       act( AT_ACTION, "> $n throws $p at $N", ch, obj, victim, TO_NOTVICT );
       act( AT_ACTION, "> $n throw $p at you", ch, obj, victim, TO_VICT );
   }
   else if ( arg2[0] == '\0' )
   {
       sprintf( buf, "> $n throws %s at the floor" , obj->short_descr );
       act( AT_ACTION, buf, ch, NULL, NULL, TO_ROOM );
       ch_printf( ch, "> you throw %s at the floor\n\r", obj->short_descr );

       victim = NULL;
   }
   else  if ( ( dir = get_door( arg2 ) ) != -1 )
   {
      if ( ( pexit = get_exit( ch->in_room, dir ) ) == NULL )
      {
         send_to_char( "> are you expecting to throw it through a wall?\n\r", ch );
         return;
      }


      if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
      {
          send_to_char( "> are you expecting to throw it through a door?\n\r", ch );
          return;
      }


      switch ( dir )
      {
        case 0:
        case 1:
           dir += 2;
           break;
        case 2:
        case 3:
           dir -= 2;
           break;
        case 4:
        case 7:
           dir += 1;
           break;
        case 5:
        case 8:
           dir -= 1;
           break;
        case 6:
           dir += 3;
           break;
        case 9:
           dir -=3;
           break;
      }

       to_room = pexit->to_room;


      char_from_room( ch );
      char_to_room( ch, to_room );

      victim = get_char_room( ch, arg3 );

      if ( victim )
      {
        if ( is_safe( ch, victim ) )
        {
            char_from_room( ch );
            char_to_room( ch, was_in_room );
            return;
        }

        if ( IS_NPC(victim) && IS_SET( victim->act, ACT_IMMORTAL )  )
        {
    	    send_to_char( "> this program cannot be flatlined\n\r", ch );
            char_from_room( ch );
            char_to_room( ch, was_in_room );
    	    return;
        }


        if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
        {
        act( AT_PLAIN, "> $N is your beloved master", ch, NULL, victim, TO_CHAR );
        char_from_room( ch );
        char_to_room( ch, was_in_room );
        return;
        }

        if ( !IS_NPC( victim ) && IS_SET( ch->act, PLR_NICE ) )
        {
        send_to_char( "> you feel too nice to do that!\n\r", ch );
        char_from_room( ch );
        char_to_room( ch, was_in_room );
        return;
        }

        if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
        {
        	set_char_color( AT_MAGIC, ch );
        	send_to_char( "> you will have to do that elsewhere\n\r", ch );
            char_from_room( ch );
            char_to_room( ch, was_in_room );
        	return;
        }

        if ( IS_SET( victim->in_room->room_flags, ROOM_SAFE ) )
        {
        	set_char_color( AT_MAGIC, ch );
        	send_to_char( "> you will have to do that elsewhere\n\r", ch );
            char_from_room( ch );
            char_to_room( ch, was_in_room );
        	return;
        }

           to_room = pexit->to_room;

        sprintf( buf , "> someone throws %s at you from the %s" , obj->short_descr , dir_name[dir] );
        act( AT_ACTION, buf , victim, NULL, ch, TO_CHAR );
        act( AT_ACTION, "> you throw %p at $N", ch, obj, victim, TO_CHAR );
        sprintf( buf, "> %s is thrown at $N from the %s" , obj->short_descr , dir_name[dir] );
        act( AT_ACTION, buf, ch, NULL, victim, TO_NOTVICT );


      }
      else
      {
         ch_printf( ch, "> you throw %s %s\n\r", obj->short_descr , dir_name[get_dir( arg2 )] );
         sprintf( buf, "> %s is thrown from the %s" , obj->short_descr , dir_name[dir] );
         act( AT_ACTION, buf, ch, NULL, NULL, TO_ROOM );

      }
   }
   else if ( ( victim = get_char_room( ch, arg2 ) ) != NULL )
   {
        if ( is_safe( ch, victim ) )
        {
        	return;
        }
        if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
        {
        act( AT_PLAIN, "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
	return;
        }

        if ( !IS_NPC( victim ) && IS_SET( ch->act, PLR_NICE ) )
        {
        send_to_char( "You feel too nice to do that!\n\r", ch );
        return;
        }

        if ( IS_NPC(victim) && IS_SET( victim->act, ACT_IMMORTAL )  )
        {
    	    send_to_char( "> this program cannot be flatlined\n\r", ch );
    	    return;
        }

   }
   else
   {
       ch_printf( ch, "They don't seem to be here!\n\r");
       return;
   }


   if ( obj == get_eq_char( ch, WEAR_WIELD )
   && ( tmpobj = get_eq_char( ch, WEAR_DUAL_WIELD)) != NULL )
       tmpobj->wear_loc = WEAR_WIELD;

   unequip_char( ch, obj );
   separate_obj( obj );
   obj_from_char( obj );
   obj = obj_to_room( obj, ch->in_room );

   damage_obj ( obj );

/* NOT NEEDED UNLESS REFERING TO OBJECT AGAIN

   if( obj_extracted(obj) )
      return;
*/
   if ( ch->in_room !=  was_in_room )
   {
     char_from_room( ch );
     char_to_room( ch, was_in_room );
   }

   if ( !victim || char_died( victim ) )
       learn_from_failure( ch, gsn_throw );
   else
   {

       WAIT_STATE( ch, skill_table[gsn_throw]->beats );
       if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_throw] )
       {
	 learn_from_success( ch, gsn_throw );
	 global_retcode = damage( ch, victim, number_range( obj->weight , (obj->weight + (ch->perm_str / 2)) ), TYPE_HIT );
       }
       else
       {
	 learn_from_failure( ch, gsn_throw );
	 global_retcode = damage( ch, victim, 0, TYPE_HIT );
       }

       if ( IS_NPC( victim ) && !char_died ( victim) )
       {
          if ( IS_SET( victim->act , ACT_SENTINEL ) )
          {
             victim->was_sentinel = victim->in_room;
             REMOVE_BIT( victim->act, ACT_SENTINEL );
          }

          start_hating( victim , ch );
          start_hunting( victim, ch );

       }

   }

   return;

}

void do_pickshiplock( CHAR_DATA *ch, char *argument )
{
	do_pick( ch, argument );
}

void do_hijack( CHAR_DATA *ch, char *argument )
{
	int chance;
	SHIP_DATA *ship;
	SHIP_DATA *onship;
	char buf[MAX_STRING_LENGTH];
	bool uhoh = FALSE;
	CHAR_DATA *guard;
	ROOM_INDEX_DATA *room;
	PLANET_DATA *planet;

	if ( (ship = ship_from_cockpit(ch->in_room)) == NULL )
	{
		send_to_char("&R> you must be in the cockpit of a ship to do that\n\r",ch);
		return;
	}

	if ( ship->class > SPACE_STATION )
	{
		send_to_char("&RThis is not a spacecraft\n\r",ch);
		return;
	}

	if( get_age(ch) < 19 )
	{
		send_to_char("&R> you must be at least 19 or older to takeover decks\n\r", ch  );
		return;
	}

	if ( check_pilot( ch , ship ) )
	{
		send_to_char("&RWhat would be the point of that\n\r",ch);
		return;
	}

	if ( ship->type == MOB_SHIP && get_trust(ch) < 102 )
	{
		send_to_char("&RThis ship is not pilotable by mortals at this point in time\n\r",ch);
		return;
	}

	if  ( ship->class == SPACE_STATION )
	{
		send_to_char( "> you cannot do that here\n\r" , ch );
		return;
	}

	if ( ship->lastdoc != ship->location )
	{
		send_to_char("&rYou dont seem to be docked right now\n\r",ch);
		return;
	}

	if ( ship->shipstate != SHIP_DOCKED && ship->shipstate != SHIP_DISABLED )
	{
		send_to_char("The ship is not docked right now\n\r",ch);
		return;
	}

	if ( ship->shipstate == SHIP_DISABLED )
	{
		send_to_char("The ships drive is disabled \n\r",ch);
		return;
	}

	onship = ship_from_room(ship->in_room);
	if( !onship )
		planet = get_room_index(ship->location)->area->planet;
	if ( planet && !onship)
		if( IS_SET(planet->flags, PLANET_SHUT ) )
		{
			send_to_char("&RNow is not a good time to do that\n\r", ch );
			return;
		}
	if( onship )
		if( onship->first_hanger->bayopen == FALSE )
		{
			send_to_char("&R> the hangars are closed\n\r", ch );
			return;
		}

	for ( room = ship->first_room ; room ; room = room->next_in_ship )
	{
		for ( guard = room->first_person; guard ; guard = guard->next_in_room )
			if ( IS_NPC(guard) && guard->pIndexData && guard->pIndexData->vnum == MOB_VNUM_SHIP_GUARD
					&& guard->position > POS_SLEEPING && !guard->fighting )
			{
				start_hating( guard, ch );
				start_hunting( guard , ch );
				uhoh = TRUE;
			}
	}

	if ( uhoh )
	{
		send_to_char("Uh oh.\n\r",ch);
		return;
	}

	chance = IS_NPC(ch) ? 0
			: (int)  (ch->pcdata->learned[gsn_hijack]) ;
	if ( number_percent( ) > chance )
	{
		send_to_char("> you fail to figure out the correct launch code\n\r",ch);
		return;
	}

	chance = IS_NPC(ch) ? 0
			: (int)  (ch->pcdata->learned[gsn_spacecraft]) ;
	if ( number_percent( ) < chance )
	{

		if (ship->hatchopen)
		{
			ship->hatchopen = FALSE;
			sprintf( buf , "The hatch on %s closes" , ship->name);
			echo_to_room( AT_YELLOW , get_room_index(ship->location) , buf );
			echo_to_room( AT_YELLOW , ship->entrance , "The hatch slides shut." );
		}
		set_char_color( AT_GREEN, ch );
		send_to_char( "Launch sequence initiated\n\r", ch);
		act( AT_PLAIN, "> $n starts up the ship and begins the launch sequence", ch,
				NULL, argument , TO_ROOM );
		echo_to_ship( AT_YELLOW , ship , "The ship hums as it lifts off the ground.");
		sprintf( buf, "%s begins to launch", ship->name );
		echo_to_room( AT_YELLOW , get_room_index(ship->location) , buf );
		ship->shipstate = SHIP_LAUNCH;
		ship->currspeed = ship->realspeed;
		learn_from_success( ch, gsn_spacecraft );
		learn_from_success( ch, gsn_hijack );
		sprintf( buf, "%s has been hijacked!", ship->name );
		echo_to_all( AT_RED , buf, 0 );

		return;
	}
	set_char_color( AT_RED, ch );
	send_to_char("> you fail to work the controls properly\n\r",ch);
	return;

}


void do_propaganda ( CHAR_DATA *ch , char *argument )
{
	char buf  [MAX_STRING_LENGTH];
	char bufa  [MAX_STRING_LENGTH];
	char arg1 [MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	PLANET_DATA *planet;
	CLAN_DATA   *clan;
	OBJ_DATA *obj;
	int percent, chance;
	bool ch_snippet, ch_complete;

		if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_propaganda] )
	{
		send_to_char("> you do not know the propaganda skillsoft\n\r", ch );
		return;
	}

	//if ( IS_NPC(ch) || !ch->pcdata || !ch->pcdata->clan || !ch->in_room->area || !ch->in_room->area->planet )
	if ( IS_NPC(ch) || !ch->pcdata || !ch->in_room->area || !ch->in_room->area->planet )
	{
		send_to_char( "> what would be the point of that\n\r", ch );
		return;
	}

    if ( get_age(ch) <= 20 )
    {
	send_to_char( "> your uptime is not high enough\n\r", ch );
	return;
    }

	argument = one_argument( argument, arg1 );
	planet = ch->in_room->area->planet;

	if ( ch->mount )
	{
		send_to_char( "> you cannot do that while mounted\n\r", ch );
		return;
	}

	   if( IS_SET( planet->flags, PLANET_NOCAP ) )
    {
		   send_to_char( "> &Rthis system cannot be affected&w", ch );
       return;
    }

	if ( arg1[0] == '\0' )
	{
		send_to_char( "> spread propaganda to who\n\r", ch );
		return;
	}

	if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
	{
		send_to_char( "> they are not here\n\r", ch );
		return;
	}

	if ( victim == ch )
	{
		send_to_char( "> that is pointless\n\r", ch );
		return;
	}

	if ( !IS_SET( victim->act , ACT_CITIZEN ) )
	{
		send_to_char( "> that will not work on them\n\r" , ch );
		return;
	}

	if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
	{
		set_char_color( AT_MAGIC, ch );
		send_to_char( "> this is not a good place to do that\n\r", ch );
		return;
	}

	if ( IS_SET( ch->in_room->room_flags, ROOM_HOTEL ) )
	{
		set_char_color( AT_MAGIC, ch );
		send_to_char( "> this is not a good place to do that\n\r", ch );
		return;
	}

	if ( IS_SET( ch->in_room->room_flags, ROOM_EMPLOYMENT ) )
	{
		set_char_color( AT_MAGIC, ch );
		send_to_char( "> this is not a good place to do that\n\r", ch );
		return;
	}

	if ( ch->position == POS_FIGHTING )
	{
		send_to_char( "> interesting combat technique\n\r" , ch );
		return;
	}

	if ( victim->position == POS_FIGHTING )
	{
		send_to_char( "> they are a little busy right now\n\r" , ch );
		return;
	}


	if ( ch->position <= POS_SLEEPING )
	{
		send_to_char( "> you are sleeping\n\r" , ch );
		return;
	}

	if ( victim->position <= POS_SLEEPING )
	{
		send_to_char( "> you might want to wake them first\n\r" , ch );
		return;
	}

	clan = ch->pcdata->clan;

	if (!clan)
	{

		ch_snippet = FALSE;
		ch_complete = FALSE;

		sprintf( buf , "contract for %s" , ch->in_room->area->planet->name );

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_TRASH && !str_cmp(obj->name,
					buf)) {
				obj->value[0] -= 1;
				ch_snippet = TRUE;

				if (obj->value[0] < 1)
				{
				ch_complete = TRUE;
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
				}


			}
		}

			if (!ch_snippet) {
				ch_printf( ch, "> you need: %s\n\r", buf );
				return;
			}

			if (ch_complete) {
				send_to_char( "> &Gyou finish the contract&w\n\r", ch);
				ch->gold += 1000;
			}

				WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

				chance = IS_NPC(ch) ? ch->top_level
						: (int) (ch->pcdata->learned[gsn_propaganda]);
				if ( number_percent( ) > chance )
				{
					send_to_char( "> &Ryou fail to affect the program&w\n\r", ch);

					sprintf(bufa, "> %s used JACKHAMMER in %s ",
							ch->name, ch->in_room->area->planet->name);
					echo_to_clan(AT_RED, bufa, ECHOTAR_ALL, ch->in_room->area->planet->governed_by);

					if (ch->pcdata->threataction < 1) {
					send_to_char( "> &Wthreat status changed to: &btraced&w\n\r",        ch );
					ch->pcdata->threataction = 1;
					}

					ch->pcdata->threatlevel += 1;
					if ( ch->pcdata->threatlevel > 10 )
						ch->pcdata->threatlevel = 10;

					ch->pcdata->bounty += ch->pcdata->threatlevel * 100;

					return;
				}

					planet->pop_support -= .2;
					send_to_char( "> &Gsupport for the current owner decreases&w\n\r", ch );

					act( AT_ACTION, "> $n injects some code\n\r", ch, NULL, victim, TO_VICT    );
					act( AT_ACTION, "> $n injects $N with their code\n\r",  ch, NULL, victim, TO_NOTVICT );

				if ( number_percent() == 23 )
				{
					send_to_char( "> you feel a bit more charming than you used to\n\r", ch );
					ch->perm_cha++;
					ch->perm_cha = UMIN( ch->perm_cha , 25 );
				}

				learn_from_success( ch, gsn_propaganda );

				if ( planet->pop_support > 100 )
					planet->pop_support = 100;
				if ( planet->pop_support < -100 )
					planet->pop_support = -100;
				return;

			}
	else
	{
	planet = ch->in_room->area->planet;

	sprintf( buf, ", and delete some of the %s code" , planet->governed_by ? planet->governed_by->name : "AI" );
	ch_printf( ch, "> you inject some code from %s%s\n\r", ch->pcdata->clan->name,
			planet->governed_by == clan ? "" : buf );
	act( AT_ACTION, "> $n injects some organization code\n\r", ch, NULL, victim, TO_VICT    );
	act( AT_ACTION, "> $n injects $N with their organization code\n\r",  ch, NULL, victim, TO_NOTVICT );

	WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

	percent = number_percent( );

	if ( percent - get_curr_cha(ch) + victim->top_level > ch->pcdata->learned[gsn_propaganda]  )
	{

		if ( planet->governed_by != clan )
		{
			sprintf( buf, "%s is a traitor!" , ch->name);
			do_yell( victim, buf );
			global_retcode = multi_hit( victim, ch, TYPE_UNDEFINED );
		}

		send_to_char( "> &Ryou fail to use propaganda&w\n\r", ch );

		if ( planet->governed_by != clan )
		{
			if (ch->pcdata->threataction < 1) {
			send_to_char( "> &Wthreat status changed to: &btraced&w\n\r",        ch );
			ch->pcdata->threataction = 1;
			}

			ch->pcdata->threatlevel += 1;
			if ( ch->pcdata->threatlevel > 10 )
				ch->pcdata->threatlevel = 10;

			ch->pcdata->bounty += ch->pcdata->threatlevel * 100;

		sprintf(bufa, "> %s used PROPAGANDA in %s ",
				ch->name, ch->in_room->area->planet->name);
		echo_to_clan(AT_RED, bufa, ECHOTAR_ALL, ch->in_room->area->planet->governed_by);

		}
		return;
	}

	if ( planet->governed_by == clan )
	{
		planet->pop_support += 1;
		send_to_char( "> support for your organization increases\n\r", ch );
	}
	else
	{
		planet->pop_support -= .1;
		send_to_char( "> support for the current owner decreases\n\r", ch );
	}

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel a bit more charming than you used to\n\r", ch );
		ch->perm_cha++;
		ch->perm_cha = UMIN( ch->perm_cha , 25 );
	}

	learn_from_success( ch, gsn_propaganda );

	if ( planet->pop_support > 100 )
		planet->pop_support = 100;
	if ( planet->pop_support < -100 )
		planet->pop_support = -100;
	}
}

void  clear_roomtype( ROOM_INDEX_DATA * location )
{

	int nodelevel, baselevel;

	if ( location->area && location->area->planet )
	{

		baselevel = location->level;

		if ( baselevel == 0 )
			nodelevel = 1;
		else if (baselevel == 1 )
			nodelevel = 2;
		else if (baselevel == 2 )
			nodelevel = 4;
		else if (baselevel == 3 )
			nodelevel = 8;
		else if (baselevel == 4 )
			nodelevel = 16;
		else if (baselevel == 5 )
			nodelevel = 32;
		else
			nodelevel = 1;

		if ( location->sector_type == SECT_DESERT )
			location->area->planet->citysize--;// = location->area->planet->citysize - nodelevel;
		else if ( location->sector_type == SECT_FARMLAND )
			location->area->planet->farmland--; // = location->area->planet->farmland - nodelevel;
		else if ( location->sector_type == SECT_GLACIAL )
			location->area->planet->wilderness--; // = location->area->planet->wilderness - nodelevel;
		else if ( location->sector_type == SECT_FIELD )
		{
			location->area->planet->entertain_plus = location->area->planet->entertain_plus - nodelevel;
			location->area->planet->multimedia_minus = location->area->planet->multimedia_minus - nodelevel;
		}
		else if ( location->sector_type == SECT_FOREST )
		{
			location->area->planet->multimedia_plus = location->area->planet->multimedia_plus - nodelevel;
			location->area->planet->entertain_minus = location->area->planet->entertain_minus - nodelevel;
		}
		else if ( location->sector_type == SECT_HILLS )
		{
			location->area->planet->finance_plus = location->area->planet->finance_plus - nodelevel;
			location->area->planet->product_minus = location->area->planet->product_minus - nodelevel;
		}
		else if ( location->sector_type == SECT_SCRUB )
		{
			location->area->planet->product_plus = location->area->planet->product_plus - nodelevel;
			location->area->planet->finance_minus = location->area->planet->finance_minus - nodelevel;
		}

		if (location->seccode != 0)
			location->seccode = 0;

		if ( IS_SET( location->room_flags , ROOM_BARRACKS ) )
			location->area->planet->barracks--;
		if ( IS_SET( location->room_flags , ROOM_CONTROL ) )
			location->area->planet->controls--;

	}

	REMOVE_BIT( location->room_flags , ROOM_NO_MOB );
	REMOVE_BIT( location->room_flags , ROOM_HOTEL );
	REMOVE_BIT( location->room_flags , ROOM_SAFE );
	REMOVE_BIT( location->room_flags , ROOM_CAN_LAND );
	REMOVE_BIT( location->room_flags , ROOM_SHIPYARD );
	REMOVE_BIT( location->room_flags , ROOM_EMPTY_HOME );
	REMOVE_BIT( location->room_flags , ROOM_DARK );
	REMOVE_BIT( location->room_flags , ROOM_INFO );
	REMOVE_BIT( location->room_flags , ROOM_MAIL );
	REMOVE_BIT( location->room_flags , ROOM_TRADE );
	REMOVE_BIT( location->room_flags , ROOM_SUPPLY );
	REMOVE_BIT( location->room_flags , ROOM_PAWN );
	REMOVE_BIT( location->room_flags , ROOM_RESTAURANT );
	REMOVE_BIT( location->room_flags , ROOM_BAR );
	REMOVE_BIT( location->room_flags , ROOM_CONTROL );
	REMOVE_BIT( location->room_flags , ROOM_BARRACKS );
	REMOVE_BIT( location->room_flags , ROOM_GARAGE );
	REMOVE_BIT( location->room_flags , ROOM_BANK );
	REMOVE_BIT( location->room_flags , ROOM_EMPLOYMENT );
	REMOVE_BIT( location->room_flags , ROOM_PUBLICIO );
	REMOVE_BIT( location->room_flags2 , ROOM_INTRUSION );

}

void do_landscape ( CHAR_DATA *ch , char *argument )
{
	CLAN_DATA * clan;
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	ROOM_INDEX_DATA * location;
	EXIT_DATA   *xit;
	int chance;
	//char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	char bufa[MAX_STRING_LENGTH];
	PLANET_DATA *planet;
	
			if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_landscape] )
	{
		send_to_char("> you do not know the modify skillsoft\n\r", ch );
		return;
	}

	if ( IS_NPC(ch) || !ch->pcdata )
		return;

	if ( !ch->desc )
		return;

	if( ship_from_room( ch->in_room ) )
	{
		send_to_char( "> you cannot do that on a ship\n\r", ch );
		return;
	}

	switch( ch->substate )
	{
	default:
		break;
	case SUB_ROOM_DESC:
		location = ch->dest_buf;
		if ( !location )
		{
			bug( "landscape: sub_room_desc: NULL ch->dest_buf", 0 );
			location = ch->in_room;
		}

		STRFREE( location->description );
		location->description = copy_buffer( ch );
		stop_editing( ch );
		ch->substate = ch->tempnum;

		SET_BIT( location->area->flags , AFLAG_MODIFIED );
		for ( obj = ch->in_room->first_content; obj; obj = obj_next )
		{
			obj_next = obj->next_content;
			extract_obj( obj );
		}
		echo_to_room( AT_WHITE, location , "> construction code finished" );
		return;
	}

	location = ch->in_room;
	clan = ch->pcdata->clan;
	planet = ch->in_room->area->planet;

	if ( !IS_SET( planet->flags, PLANET_NOCAP ) )
	{

		if ( str_cmp(ch->in_room->area->planet->name, "users") ) {

		if ( !clan )
		{
			send_to_char( "> you need to be part of an organization before you can do that\n\r", ch );
			return;
		}

		if ( (ch->pcdata && ch->pcdata->bestowments
				&&    is_name("build", ch->pcdata->bestowments))
				|| nifty_is_name( ch->name, clan->leaders  ) )
			;
		else
		{
			send_to_char( "> your organization has not given you permission to modify their systems\n\r", ch );
			return;
		}

		if ( !location->area || !location->area->planet ||
				clan != location->area->planet->governed_by  )
		{
			send_to_char( "> you may only modify nodes in systems that your organization controls\n\r", ch );
			return;
		}
		}

	}

	if( strcmp(location->owner, ch->name) && strcmp(ch->in_room->area->planet->name, "newtutorial") )
	{
		send_to_char( "> &Rthis is not your node&w\n\r", ch );
		return;
	}

	if ( IS_SET( location->room_flags , ROOM_NOPEDIT ) )
	{
		send_to_char( "> &Ryou can not modify this node&w\n\r", ch );
		return;
	}

	//argument = one_argument( argument, arg );

	if ( argument[0] == '\0' )
	{
		send_to_char( "> syntax: MODIFY  <node type>  <new node name>\n\r", ch );
		send_to_char( " <node type> may be one of the following:\n\r\n\r", ch );

		send_to_char( "database     - generates resources\n\r", ch );
		send_to_char( "subserver    - supports terminals\n\r", ch );
		send_to_char( "terminal     - generates funds\n\r", ch );
		send_to_char( "ionode       - protected io node\n\r", ch );
		send_to_char( "publicio     - public io node\n\r", ch );
		send_to_char( "home         - may be used as a private node\n\r", ch );
		send_to_char( "agent        - users can get jobs here\n\r", ch );
		send_to_char( "trade        - users can sell resources\n\r", ch );
		send_to_char( "supply       - supply node\n\r", ch );
		send_to_char( "pawn         - will trade useful code\n\r", ch );
		send_to_char( "coding       - users can code classes here\n\r", ch );
		send_to_char( "firewall     - generates ICE [cost: 20,000]\n\r", ch );
		send_to_char( "bank         - banking node\n\r", ch );
		send_to_char( "employment   - job office\n\r", ch );
		send_to_char( "entertain    - entertainment repo\n\r", ch );
		send_to_char( "product      - productivity repo\n\r", ch );
		send_to_char( "finance      - finance repo\n\r", ch );
		send_to_char( "multimedia   - multimedia repo\n\r", ch );
		send_to_char( "intrusion    - intrusion node\n\r", ch );
		//send_to_char( "monitoring   - monitoring node\n\r", ch );
		return;
	}


	chance = (int) (ch->pcdata->learned[gsn_landscape]);
	if ( number_percent( ) > chance )
	{
		send_to_char( "&R> you fail to modify. try again!&w\n\r", ch );
		return;
	}

	if ( IS_SET( location->room_flags2 , ROOM_INTRUSION ) ){

		if ( ch->gold < 500 ) {
			send_to_char( "> &Rit costs 500c to modify an intrusion node&w\n\r", ch );
			return;
		}
		else {
			ch->gold -= 500;
			send_to_char( "> &G500c spent to modify intrusion node&w\n\r", ch );
		}

	}

	clear_roomtype( location );

	if ( IS_SET( location->room_flags , ROOM_PLR_HOME ))
	{
		location->area->planet->citysize++;
		SET_BIT( location->room_flags , ROOM_NO_MOB );
		SET_BIT( location->room_flags , ROOM_HOTEL );
		SET_BIT( location->room_flags , ROOM_SAFE );
	}
	else if ( !str_cmp( argument, "entertain" ) )
	{
		location->sector_type = SECT_FIELD;
		location->area->planet->entertain_plus++;
		location->area->planet->multimedia_minus++;
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Centertainment" );
		strcpy( bufa , "an entertainment repository node\n\r" );
		//ch->pcdata->qtaxnodes = ch->pcdata->qtaxnodes + 1;
	}
	else if ( !str_cmp( argument, "intrusion" ) )
	{
		location->sector_type = SECT_INSIDE;
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cintrusion" );
		strcpy( bufa , "an intrusion node\n\r" );
		SET_BIT( location->room_flags2 , ROOM_INTRUSION );

	}
	else if ( !str_cmp( argument, "multimedia" ) )
	{
		location->sector_type = SECT_FOREST;
		location->area->planet->multimedia_plus++;
		location->area->planet->entertain_minus++;
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cmultimedia" );
		strcpy( bufa , "a multimedia repository node\n\r" );
		//ch->pcdata->qtaxnodes = ch->pcdata->qtaxnodes + 1;
	}
	else if ( !str_cmp( argument, "finance" ) )
	{
		location->sector_type = SECT_HILLS;
		location->area->planet->finance_plus++;
		location->area->planet->product_minus++;
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cfinance" );
		strcpy( bufa , "a finance repository node\n\r" );
		//ch->pcdata->qtaxnodes = ch->pcdata->qtaxnodes + 1;
	}
	else if ( !str_cmp( argument, "product" ) )
	{
		location->sector_type = SECT_SCRUB;
		location->area->planet->product_plus++;
		location->area->planet->finance_minus++;
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cproductivity" );
		strcpy( bufa , "a productivity repository node\n\r" );
		//ch->pcdata->qtaxnodes = ch->pcdata->qtaxnodes + 1;
	}
	else if ( !str_cmp( argument, "terminal" ) )
	{
		location->area->planet->citysize++;
		location->sector_type = SECT_DESERT;
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cterminal" );
		strcpy( bufa , "a terminal node.\n\r" );
		//ch->pcdata->qtaxnodes = ch->pcdata->qtaxnodes + 1;
	}
	else if ( !str_cmp( argument, "database" ) )
	{
		location->area->planet->wilderness++;
		location->sector_type = SECT_GLACIAL; //location->area->planet->sector;
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cdatabase" );
		strcpy( bufa , "a database node.\n\r" );
		//ch->pcdata->qtaxnodes = ch->pcdata->qtaxnodes + 1;
	}
	else if ( !str_cmp( argument, "subserver" ) )
	{
		location->area->planet->farmland++;
		location->sector_type = SECT_FARMLAND;
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Csubserver" );
		strcpy( bufa , "a subserver node.\n\r" );
		//ch->pcdata->qtaxnodes = ch->pcdata->qtaxnodes + 1;
	}
	else if ( !str_cmp( argument, "home" ) )
	{

		int amountexits = 0;
		for( xit = location->first_exit; xit; xit = xit->next )
			amountexits = amountexits + 1;

		if( amountexits > 1 )
		{
			send_to_char("> &Rnode already has more than one exit&w\n\r", ch );
			return;
		}

		for( xit = location->first_exit; xit; xit = xit->next )
			if( IS_SET( xit->to_room->room_flags, ROOM_BARRACKS ) )
			{
				send_to_char("> &Ryou cannot put home nodes around firewalls&w\n\r", ch );
				return;
			}

		for( xit = location->first_exit; xit; xit = xit->next )
			if( IS_SET( xit->to_room->room_flags, ROOM_EMPTY_HOME ) )
			{
				send_to_char("> &Ryou cannot put home nodes around home nodes&w\n\r", ch );
				return;
			}

		for( xit = location->first_exit; xit; xit = xit->next )
			if( IS_SET( xit->to_room->room_flags, ROOM_PLR_HOME ) )
			{
				send_to_char("> &Ryou cannot put home nodes around home nodes&w\n\r", ch );
				return;
			}

		for( xit = location->first_exit; xit; xit = xit->next )
			if( IS_SET( xit->to_room->room_flags, ROOM_SAFE ) )
			{
				send_to_char("> &Ryou cannot put home nodes around safe nodes&w\n\r", ch );
				return;
			}

		location->sector_type = SECT_INSIDE;
		SET_BIT( location->room_flags , ROOM_EMPTY_HOME );
		SET_BIT( location->room_flags , ROOM_NO_MOB );
		//SET_BIT( location->room_flags , ROOM_NOPEDIT );
		strcpy( buf , "&Chomenode" );
		strcpy( bufa , "use BUYHOME to buy this node for 10k.\n\r" );
	}
	else if ( !str_cmp( argument, "bank" ) )
	{
		location->sector_type = SECT_INSIDE;
		SET_BIT( location->room_flags , ROOM_BANK );
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cbank" );
		strcpy( bufa , "a bank node.\n\r" );
	}
	else if ( !str_cmp( argument, "agent" ) )
	{

		location->sector_type = SECT_INSIDE;
		SET_BIT( location->room_flags , ROOM_HOTEL );
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cagent" );
		strcpy( bufa , "an agent node.\n\r" );
	}
	else if ( !str_cmp( argument, "ionode" ) )
	{
		if ( IS_SET( planet->flags, PLANET_NOCAP ) )
		{
			if ( str_cmp(ch->in_room->area->planet->name, "users") ) {
			send_to_char("> &Ryou cannot build protected io nodes here&w\n\r", ch );
			return;
			}
		}

		for( xit = location->first_exit; xit; xit = xit->next )
			if( IS_SET( xit->to_room->room_flags, ROOM_EMPTY_HOME ) )
			{
				send_to_char("> &Ryou cannot put io nodes around home nodes&w\n\r", ch );
				return;
			}

		location->sector_type = SECT_CITY;
		SET_BIT( location->room_flags , ROOM_CAN_LAND );
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cio" );
		strcpy( bufa , "an io node.\n\r" );
	}
	else if ( !str_cmp( argument, "publicio" ) )
	{

		for( xit = location->first_exit; xit; xit = xit->next )
			if( IS_SET( xit->to_room->room_flags, ROOM_EMPTY_HOME ) )
			{
				send_to_char("> &Ryou cannot put io nodes around home nodes&w\n\r", ch );
				return;
			}

		location->sector_type = SECT_CITY;
		SET_BIT( location->room_flags , ROOM_PUBLICIO );
		SET_BIT( location->room_flags , ROOM_NO_MOB );
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cio" );
		strcpy( bufa , "a public io node.\n\r" );
	}
	else if ( !str_cmp( argument, "trade" ) )
	{
		location->sector_type = SECT_SAVANNA;
		SET_BIT( location->room_flags , ROOM_TRADE );
		SET_BIT( location->room_flags , ROOM_NO_MOB );
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Ctrade" );
		strcpy( bufa , "a trade node.\n\r" );
	}
	else if ( !str_cmp( argument, "supply" ) )
	{
		location->sector_type = SECT_SAVANNA;
		SET_BIT( location->room_flags , ROOM_SUPPLY );
		SET_BIT( location->room_flags , ROOM_NO_MOB );
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Csupply" );
		strcpy( bufa , "a supply node.\n\r" );
	}
	else if ( !str_cmp( argument, "pawn" ) )
	{
		location->sector_type = SECT_SAVANNA;
		SET_BIT( location->room_flags , ROOM_PAWN );
		SET_BIT( location->room_flags , ROOM_NO_MOB );
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cpawn" );
		strcpy( bufa , "a pawn node.\n\r" );
	}

	else if ( !str_cmp( argument, "coding" ) )
	{
		location->sector_type = SECT_INSIDE;
		SET_BIT( location->room_flags , ROOM_RESTAURANT );
		SET_BIT( location->room_flags , ROOM_NO_MOB );
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Ccoding" );
		strcpy( bufa , "a coding node.\n\r" );
	}

	else if ( !str_cmp( argument, "firewall" ) )
	{

		if ( IS_SET( planet->flags, PLANET_NOCAP ) )
		{
			send_to_char("> &Ryou cannot build firewalls here&w\n\r", ch );
			return;
		}

		if( ch->gold < 20000 )
		{
			send_to_char("> insufficient funds [20k needed] - \n\r", ch );
			return;
		}

		for( xit = location->first_exit; xit; xit = xit->next )
			if( IS_SET( xit->to_room->room_flags, ROOM_SAFE ) )
			{
				send_to_char("&R> safe node nearby.&w\n\r", ch );
				return;
			}

		for( xit = location->first_exit; xit; xit = xit->next )
			if( IS_SET( xit->to_room->room_flags, ROOM_PLR_HOME ) )
			{
				send_to_char("&R> home node nearby.&w\n\r", ch );
				return;
			}

		for( xit = location->first_exit; xit; xit = xit->next )
			if( IS_SET( xit->to_room->room_flags, ROOM_EMPTY_HOME ) )
			{
				send_to_char("&R> unused home node nearby.&w\n\r", ch );
				return;
			}

		location->sector_type = SECT_SWAMP;
		SET_BIT( location->room_flags , ROOM_BARRACKS );
		// if( location->area->planet->barracks > 4 )
		ch->gold -= 20000;
		location->area->planet->barracks++;
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cfirewall" );
		strcpy( bufa , "a firewall node.\n\r" );
	}

	else if ( !str_cmp( argument, "employment" ) )
	{
		location->sector_type = SECT_INSIDE;
		SET_BIT( location->room_flags , ROOM_EMPLOYMENT );
		strcpy( buf , ch->name );
		strcat( buf , "&Y.&Cemployment" );
		strcpy( bufa , "an employment node.\n\r" );
	}
	else
	{
		do_landscape( ch, "" );
		return;
	}

	echo_to_room( AT_WHITE, location, "> construction code begins to execute" );

	STRFREE ( location->name );
	//location->name = STRALLOC( argument );
	location->name = STRALLOC( buf );

	STRFREE( location->description );
	//	  ch->substate = ch->tempnum;

	SET_BIT( location->area->flags , AFLAG_MODIFIED );
	for ( obj = ch->in_room->first_content; obj; obj = obj_next )
	{
		obj_next = obj->next_content;
		extract_obj( obj );
	}
	echo_to_room( AT_WHITE, location , "> construction code finished" );

	location->description = STRALLOC( bufa );

	if ( number_percent() == 23 )
		learn_from_success( ch , gsn_landscape );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more intelligent than before\n\r", ch );
		ch->perm_int++;
		ch->perm_int = UMIN( ch->perm_int , 25 );
	}

	return;

}

void do_construction ( CHAR_DATA *ch , char *argument )
{
	CLAN_DATA * clan;
	int chance, ll;
	EXIT_DATA * xit;
	EXIT_DATA * xit2;
	int edir;
	ROOM_INDEX_DATA *nRoom;
	char buf[MAX_STRING_LENGTH];
	//PLANET_DATA * dPlanet = NULL;
	PLANET_DATA *planet;
	
			if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_construction] )
	{
		send_to_char("> you do not know the construct skillsoft\n\r", ch );
		return;
	}

	if ( IS_NPC(ch) || !ch->pcdata || !ch->in_room )
		return;

	clan = ch->pcdata->clan;
	planet = ch->in_room->area->planet;

	if ( str_cmp( argument, "north" )
			&& str_cmp( argument, "south" )
			&& str_cmp( argument, "west" )
			&& str_cmp( argument, "east" )
			&& str_cmp( argument, "up" )
			&& str_cmp( argument, "down")
			&& str_cmp( argument, "n")
			&& str_cmp( argument, "e")
			&& str_cmp( argument, "s")
			&& str_cmp( argument, "w")
			&& str_cmp( argument, "u")
			&& str_cmp( argument, "d"))
	{
		send_to_char( "> &Ryou cannot construct in that direction, try:\n\r&w", ch);
		send_to_char( "> north, east, south, west, up or down\n\r", ch);
		return;
	}

	if ( !IS_IMMORTAL(ch) )
	{

		if( IS_SET(planet->flags, PLANET_SHUT ) )
		{
			send_to_char("> &Ryou can not construct here\n\r", ch );
			return;
		}

		if ( !IS_SET( planet->flags, PLANET_NOCAP ) )
		{


			if ( !clan )
			{
				send_to_char( "> you need to be part of an organization before you can do that\n\r", ch );
				return;
			}

			if ( (ch->pcdata && ch->pcdata->bestowments
					&&    is_name("build", ch->pcdata->bestowments))
					|| nifty_is_name( ch->name, clan->leaders  ) )
				;
			else
			{
				send_to_char( "> your organization has not given you permission to construct in their systems\n\r", ch );
				return;
			}

			if ( !ch->in_room->area || !ch->in_room->area->planet ||
					clan != ch->in_room->area->planet->governed_by      )
			{
				send_to_char( "> you may only construct in systems that your organization controls\n\r", ch );
				return;
			}

			if( !IS_IMMORTAL(ch) )
				if ( ch->in_room->area->planet->size >= 250 && str_cmp(ch->in_room->area->planet->name, "straylight") )
				{
					send_to_char( "> this system is too big - go construct somewhere else\n\r", ch );
					return;
				}

		}
		else {

			if ( !str_cmp(ch->in_room->area->planet->name, "straylight") ) {

			if ( ch->pcdata->qtaxnodes > 99 )
			{
				send_to_char( "> you may not build any more nodes as a freelancer\n\r", ch );
				return;
			}

			if ( clan )
			{

				send_to_char( "> you cannot construct in this system\n\r", ch );
				return;
			}

			}
			else if ( !str_cmp(ch->in_room->area->planet->name, "users") ) {

				if ( ch->pcdata->homesyssize > 99 )
				{
					send_to_char( "> you may not build any more nodes in your home system\n\r", ch );
					return;
				}

			}
			else if ( !str_cmp(ch->in_room->area->planet->name, "newtutorial") ) {

				if ( str_cmp( ch->pcdata->clan_name, "tutors") )
				{
					send_to_char( "> you may not build here. join the tutors first!\n\r", ch );
					return;
				}

			}

		}

		if( !IS_IMMORTAL(ch) )
			if ( IS_SET( ch->in_room->room_flags , ROOM_NOPEDIT ) )
			{
		         if( !IS_SET( ch->in_room->room_flags2, ROOM_HOMESYSIO ) )
		         {
						send_to_char( "> you may not edit this node\n\r", ch );
						return;
		         }

			}
	}

	if ( argument[0] == '\0' )
	{
		send_to_char( "> begin construction in what direction\n\r", ch );
		return;
	}

	if ( ch->gold < 500 )
	{
		send_to_char( "> you do not have enough credits - it costs 500 credits\n\r", ch );
		return;
	}

	if ( IS_SET( ch->in_room->room_flags , ROOM_EMPTY_HOME ) )
	{
		send_to_char( "> you may not edit this node\n\r", ch );
		return;
	}

	edir = get_dir(argument);
	xit = get_exit(ch->in_room, edir);
	if ( xit )
	{
		send_to_char( "> there is already a node in that direction\n\r", ch );
		return;
	}

	chance = (int) (ch->pcdata->learned[gsn_construction]);
	if ( number_percent( ) > chance )
	{
		send_to_char( "> you fail to construct. try again!\n\r", ch );
		ch->gold -= 5;
		return;
	}

	ch->gold -= 500;

	nRoom = make_room( ++top_r_vnum );
	nRoom->area = ch->in_room->area;
	LINK( nRoom , ch->in_room->area->first_room , ch->in_room->area->last_room , next_in_area , prev_in_area );
	STRFREE( nRoom->name );
	STRFREE( nRoom->description );
	nRoom->name = STRALLOC( "raw" );
	nRoom->description = STRALLOC ( "use MODIFY command to customize node\n\r" );
	//STRFREE( nRoom->owner );
	nRoom->owner = STRALLOC( ch->name );
	nRoom->sector_type = SECT_DUNNO;
	SET_BIT( nRoom->room_flags , ROOM_NO_MOB );

	xit = make_exit( ch->in_room, nRoom, edir );
	xit->keyword		= STRALLOC( "" );
	xit->description	= STRALLOC( "" );
	xit->key		= -1;
	xit->exit_info	= 0;

	xit2 = make_exit( nRoom , ch->in_room  , rev_dir[edir] );
	xit2->keyword		= STRALLOC( "" );
	xit2->description	= STRALLOC( "" );
	xit2->key		= -1;
	xit2->exit_info	= 0;

	ch->in_room->area->planet->size++;

	for ( ll = 1 ; ll <= 20 ; ll++ )
		learn_from_success( ch , gsn_construction );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more intelligent than before\n\r", ch );
		ch->perm_int++;
		ch->perm_int = UMIN( ch->perm_int , 25 );
	}

	SET_BIT( ch->in_room->area->flags , AFLAG_MODIFIED );

//	if ( IS_SET( planet->flags, PLANET_NOCAP ) )
//	{
//		ch->pcdata->qtaxnodes = ch->pcdata->qtaxnodes + 1;
//	}

	if ( str_cmp(ch->in_room->area->planet->name, "users") )
		ch->pcdata->qtaxnodes = ch->pcdata->qtaxnodes + 1;
	else
		ch->pcdata->homesyssize = ch->pcdata->homesyssize + 1;

	sprintf( buf , "> a new node appears in this dir: %s" , dir_name[edir] );
	send_to_char( "> cost: 500 credits\n\r", ch );
	echo_to_room( AT_WHITE, ch->in_room, buf );

}

void do_bridge ( CHAR_DATA *ch , char *argument )
{
	CLAN_DATA * clan;
	int chance, ll;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	EXIT_DATA   *xit, *texit;
	int   evnum, edir, ekey;
	ROOM_INDEX_DATA *toroom;
	ROOM_INDEX_DATA *location;
	ROOM_INDEX_DATA *room;
	ROOM_INDEX_DATA *current;
	char buf[MAX_STRING_LENGTH];
	PLANET_DATA *planet;

	planet = ch->in_room->area->planet;

			if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_bridge] )
	{
		send_to_char("> you do not know the bridge skillsoft\n\r", ch );
		return;
	}

	if ( IS_NPC(ch) || !ch->pcdata || !ch->in_room )
		return;

	clan = ch->pcdata->clan;

	if ( !IS_SET( planet->flags, PLANET_NOCAP ) )
	{

		if ( !clan )
		{
			send_to_char( "> you need to be in an organization\n\r", ch );
			return;
		}

		if ( (ch->pcdata && ch->pcdata->bestowments
				&&    is_name("build", ch->pcdata->bestowments))
				|| nifty_is_name( ch->name, clan->leaders  ) )
			;
		else
		{
			send_to_char( "> you need access to bridge\n\r", ch );
			return;
		}

	}


	if ( !ch->in_room->area || !ch->in_room->area->planet ||
			clan != ch->in_room->area->planet->governed_by      )
	{
		send_to_char( "> you cannot construct in this system\n\r", ch );
		return;
	}

	if ( IS_SET( ch->in_room->room_flags , ROOM_NOPEDIT ) )
	{
		send_to_char( "> you cannot edit this room\n\r", ch );
		return;
	}

	if ( ch->gold < 250 )
	{
		send_to_char( "> insufficient funds [250 needed]\n\r", ch );
		return;
	}

	argument = one_argument( argument , arg1 );
	if ( argument[0] == '\0' )
	{
		send_to_char( "> syntax: bridge <direction> <action> <argument>\n\r", ch );
		send_to_char( "> action being one of the following:\n\r", ch );
		send_to_char( "> connect, door, keycode\n\r", ch );
		return;
	}
	argument = one_argument( argument , arg2 );
	argument = one_argument( argument , arg3 );

	chance = (int) (ch->pcdata->learned[gsn_bridge]);
	if ( number_percent( ) > chance )
	{
		send_to_char( "> you fail to bridge\n\r", ch );
		ch->gold -= 10;
		return;
	}

	location = ch->in_room;
	if( strcmp(location->owner, ch->name) && strcmp(ch->in_room->area->planet->name, "newtutorial") )
	{
		send_to_char( "&R> this is not your node&w\n\r", ch );
		return;
	}

	edir = get_dir(arg1);
	xit = get_exit(ch->in_room, edir);

	if ( !str_cmp( arg2 , "connect" ) )
	{
		if ( xit )
		{
			send_to_char( "> connection already exists\n\r", ch );
			return;
		}
		evnum = atoi( arg3 );
		if ( (toroom = get_room_index( evnum )) == NULL )
		{
			ch_printf( ch, "> non-existent room: %d\n\r", evnum );
			return;
		}
		if ( ch->in_room->area != toroom->area )
		{
			ch_printf( ch, "> %d is not in this system\n\r" , evnum );
			return;
		}

		if ( IS_SET(toroom->room_flags, ROOM_NOPEDIT ) )
		{
			ch_printf( ch, "> %d cannot be edited\n\r" , evnum );
			return;
		}

		texit = get_exit( toroom, rev_dir[edir] );
		if ( texit )
		{
			ch_printf( ch, "> %d already has an entrance from that direction\n\r" , evnum );
			return;
		}

		current = ch->in_room;
		room = get_room_index( atoi( arg3 ) );

		if ( IS_SET( current->room_flags, ROOM_SAFE ) )
		{
			send_to_char( "> &Rcan not bridge in safe nodes&w\n\r", ch );
			return;
		}

		if ( IS_SET( room->room_flags, ROOM_SAFE ) )
		{
			send_to_char( "> &Rcan not bridge into safe nodes&w\n\r", ch );
			return;
		}

		if ( current == room )
		{
			send_to_char( "> &Rinvalid connection&w\n\r", ch );
			return;
		}

//		ch_printf( ch, "> current: %ld\n\r" , current->vnum );
//		ch_printf( ch, "> destina: %ld\n\r" , arg3 );
//		ch_printf( ch, "> destiva: %ld\n\r" , room->vnum );

		xit = make_exit( ch->in_room, toroom, edir );
		xit->keyword		= STRALLOC( "" );
		xit->description	= STRALLOC( "" );
		xit->key		= -1;
		xit->exit_info	= 0;
		texit = make_exit( toroom , ch->in_room  , rev_dir[edir] );
		texit->keyword		= STRALLOC( "" );
		texit->description	= STRALLOC( "" );
		texit->key		= -1;
		texit->exit_info	= 0;

		sprintf( buf , "> construction code opens a connection to the %s" , dir_name[edir] );
		echo_to_room( AT_WHITE, ch->in_room, buf );
		sprintf( buf , "> construction code opens a connection to the %s" , dir_name[rev_dir[edir]] );
		echo_to_room( AT_WHITE, toroom , buf );
	}
	else if ( !str_cmp( arg2 , "keycode" ) )
	{

		if ( !xit )
		{
			send_to_char( "> no exit in that direction\n\r", ch );
			return;
		}

		if ( !IS_SET( xit->exit_info , EX_ISDOOR ) )
		{
			send_to_char( "> no door in that direction\n\r", ch );
			return;
		}

		ekey = atoi( arg3 );

		if ( ekey == 0 )
		{

			REMOVE_BIT( xit->exit_info , EX_LOCKED );
			xit->key = -1;

			texit = get_exit_to( xit->to_room, rev_dir[edir], ch->in_room->vnum );

			if ( texit )
			{
				REMOVE_BIT( texit->exit_info , EX_LOCKED );
				texit->key = -1;
			}

			send_to_char( "> keycode removed\n\r", ch );
			return;
		}
		else if ( ekey < 0 ) {
			send_to_char( "> invalid code [use 0 to remove code]\n\r", ch );
			return;
		}
		else {

			ch_printf( ch , "> code is now: %d\n\r" , ekey );
			xit->key = ekey;

			texit = get_exit_to( xit->to_room, rev_dir[edir], ch->in_room->vnum );

			if ( texit )
			{
				texit->key = ekey;
			}
		}

	}
	else if ( !str_cmp( arg2 , "door" ) )
	{
		if ( !xit )
		{
			send_to_char( "> no exit in that direction\n\r", ch );
			return;
		}

		if ( !IS_SET( xit->exit_info , EX_ISDOOR ) )
		{
			sprintf( buf , "> construction code builds a door to the %s" , dir_name[edir] );
			echo_to_room( AT_WHITE, ch->in_room, buf );
			SET_BIT(  xit->exit_info , EX_ISDOOR );
			texit = get_exit_to( xit->to_room, rev_dir[edir], ch->in_room->vnum );
			if ( texit )
			{
				SET_BIT(  texit->exit_info , EX_ISDOOR );
			}
		}

		else

		{

			//send_to_char( "> &Rcannot remove existing connection gate&w\n\r", ch );
			//return;

			sprintf( buf , "> construction code removes door at: %s" , dir_name[edir] );
			echo_to_room( AT_WHITE, ch->in_room, buf );
			REMOVE_BIT(  xit->exit_info , EX_ISDOOR );
			REMOVE_BIT( xit->exit_info , EX_LOCKED );
			REMOVE_BIT( xit->exit_info , EX_CLOSED );
			xit->key = -1;
			texit = get_exit_to( xit->to_room, rev_dir[edir], ch->in_room->vnum );
			if ( texit )
			{
				REMOVE_BIT(  texit->exit_info , EX_ISDOOR );
				REMOVE_BIT( texit->exit_info , EX_LOCKED );
				REMOVE_BIT( texit->exit_info , EX_CLOSED );
				texit->key = -1;
			}

		}

	}
	else
	{
		do_bridge( ch , "" );
		return;
	}

	ch->gold -= 250;

	send_to_char( "> 250c spent\n\r", ch );

	for ( ll = 1 ; ll <= 20 ; ll++ )
		learn_from_success( ch , gsn_bridge );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more intelligent than before\n\r", ch );
		ch->perm_int++;
		ch->perm_int = UMIN( ch->perm_int , 25 );
	}

	SET_BIT( ch->in_room->area->flags , AFLAG_MODIFIED );

}



void do_survey ( CHAR_DATA *ch , char *argument )
{
	ROOM_INDEX_DATA * room;
	int chance;
	
				if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_survey] )
	{
		send_to_char("> you do not know the analyze skillsoft\n\r", ch );
		return;
	}

	if ( IS_NPC(ch) || !ch->pcdata || !ch->in_room )
		return;

	room = ch->in_room;

	chance = (int) (ch->pcdata->learned[gsn_survey]);
	if ( number_percent( ) > chance )
	{
		send_to_char( "> you have a hard time analyzing this node\n\r", ch );
		return;   
	}

	ch_printf( ch, "&Y%s\n\r\n\r", room->name );

	if (room->level == 0)
		send_to_char( "&Wlevel: &Bblue&w\n\r", ch );
	else if (room->level == 1)
		send_to_char( "&Wlevel: &Ggreen&w\n\r", ch );
	else if (room->level == 2)
		send_to_char( "&Wlevel: &Oorange&w\n\r", ch );
	else if (room->level == 3)
		send_to_char( "&Wlevel: &Rred&w\n\r", ch );
	else if (room->level == 4)
		send_to_char( "&Wlevel: &puv&w\n\r", ch );
	else if (room->level == 5)
		send_to_char( "&Wlevel: &zblack&w\n\r", ch );
	else
		send_to_char( "&Wlevel: &wunknown&w\n\r", ch );

	ch_printf( ch, "&Windex:&Y %d\n\r", room->vnum );
	if ( room->area && room->area->planet )
	ch_printf( ch, "&Wsystem:&Y %s\n\r", room->area->planet->name );
	ch_printf( ch, "&Wsector:&Y %s\n\r", sector_name[room->sector_type] );
	ch_printf( ch, "&Wowner:&Y %s\n\r", room->owner );

	if ( !str_cmp(ch->in_room->area->planet->name, "metropolis") )
	{
		ch_printf( ch, "&Wclaimpower:&Y %d/%d\n\r", room->claimpower, ((room->level + 1) * 100) );
	}


	if ( IS_SET( room->room_flags , ROOM_DARK) )
		ch_printf( ch, "&Y   node is always dark\n\r" );

	if ( IS_SET( room->room_flags , ROOM_INDOORS) )
		ch_printf( ch, "&Y   node is indoors\n\r" );
	/*
       if ( IS_SET( room->room_flags , ROOM_SHIPYARD) )
       	   ch_printf( ch, "   &YSpacecraft can be built or purchased here\n\r" );

       if ( IS_SET( room->room_flags , ROOM_CAN_LAND) )
       	   ch_printf( ch, "&Y   Spacecraft can land here\n\r" );
	 */
	if ( IS_SET( room->room_flags , ROOM_EMPLOYMENT) )
		ch_printf( ch, "&Y   you can get delivery missions here\n\r" );

	if ( room->seccode != 0 )
		ch_printf( ch, "&Y   this is a secured io node\n\r" );

	if ( IS_SET( room->room_flags , ROOM_BANK) )
		ch_printf( ch, "&Y   this node may be used as a bank\n\r" );

	if ( IS_SET( room->room_flags , ROOM_SAFE) )
		ch_printf( ch, "&Y   combat cannot take place in this node\n\r" );

	if ( IS_SET( room->room_flags , ROOM_EMPTY_HOME ) )
		ch_printf( ch, "&Y   this node may be purchased with BUYHOME\n\r" );

	if ( IS_SET( room->room_flags , ROOM_PLR_HOME ) )
		ch_printf( ch, "&Y   this node is a user's private node\n\r" );

	if ( IS_SET( room->room_flags , ROOM_TRADE ) )
		ch_printf( ch, "&Y   this node is used for resource trade\n\r" );

	if ( IS_SET( room->room_flags , ROOM_SUPPLY ) )
		ch_printf( ch, "&Y   this is a supply node\n\r" );

	if ( IS_SET( room->room_flags , ROOM_PAWN ) )
		ch_printf( ch, "&Y   you can buy and sell useful modules here\n\r" );

	if ( IS_SET( room->room_flags , ROOM_BARRACKS ) ){

		if (room->lockdown == 0)
		ch_printf( ch, "&Y   this firewall spawns ICE\n\r" );
		else
		ch_printf( ch, "&Y   this firewall is LOCKED DOWN\n\r" );

	}

	if ( IS_SET( room->room_flags , ROOM_NOPEDIT ) )
		ch_printf( ch, "&Wthis node is NOT user editable\n\r" );
	else
		ch_printf( ch, "&Wthis node IS editable by users\n\r" );

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more intelligent than before\n\r", ch );
		ch->perm_int++;
		ch->perm_int = UMIN( ch->perm_int , 25 );
	}

	learn_from_success( ch , gsn_survey );
	return;

}


void do_quicktalk ( CHAR_DATA *ch , char *argument )
{
	CHAR_DATA *rch;
	int chance;
	
				if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_quicktalk] )
	{
		send_to_char("> you do not know the quicktalk skillsoft\n\r", ch );
		return;
	}

	if ( ch->position != POS_FIGHTING )
	{
		send_to_char( "> you cannot talk your way out of a fight that has not started yet\n\r",ch );
		return;
	}

	act( AT_ACTION, "> $n attempts to stop the fight", ch, NULL, NULL, TO_ROOM );

	chance = (int) (ch->pcdata->learned[gsn_quicktalk]);
	chance = UMIN( chance , 95 );
	if ( number_percent( ) > chance )
	{
		send_to_char( "> you fail to calm your attackers\n\r", ch );
		return;
	}

	for ( rch = ch->in_room->first_person ; rch; rch = rch->next_in_room )
	{
		if ( rch->fighting )
			stop_fighting( rch, TRUE );

		stop_hating( rch );
		stop_hunting( rch );
		stop_fearing( rch );
	}

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more charming than before\n\r", ch );
		ch->perm_cha++;
		ch->perm_cha = UMIN( ch->perm_cha , 25 );
	}

	learn_from_success( ch , gsn_quicktalk );

	send_to_char( "> you successfully talk your way out of a sticky situation\n\r", ch );
	return;

}

void do_codemed( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_STRING_LENGTH], buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj, *obj_next, *cont;
    bool checkchem = FALSE, checktool = FALSE;
    int chance, level, wearbit = get_wflag("hold");

			if ( !IS_NPC(ch) && !ch->pcdata->learned[gsn_codemed] )
	{
		send_to_char("> you do not know the codemed skillsoft\n\r", ch );
		return;
	}


    switch( ch->substate )
    {
    case 0:
 if( argument[0] == '\0' )
 {
     send_to_char("> syntax: codemed <medmod name>\n\r",ch);
     return;
 }

 for( obj = ch->first_carrying; obj; obj = obj->next_content )
 {

	 if( obj->item_type == ITEM_MIRROR && !checkchem )
     {
  checkchem = TRUE;
  continue;
     }

     if( obj->item_type == ITEM_TOOLKIT && !checktool )
  checktool = TRUE;
 }

 if( !checkchem )
 {
     send_to_char("> &Ryou need a wilderspace subroutine&w\n\r",ch);
    return;
 }

 if( !checktool )
 {
    send_to_char("> &Ryou need a devkit&w\n\r",ch);
    return;
 }

 chance = IS_NPC(ch) ? ch->top_level : ch->pcdata->learned[gsn_codemed];

 if( number_percent() <= chance )
 {
    send_to_char("> you begin making a med module\n\r",ch);
    act( AT_PLAIN, "> $n begins making a med module.", ch, NULL, NULL, TO_ROOM );
    ch->dest_buf = str_dup(argument);
    add_timer( ch, TIMER_DO_FUN, 2, do_codemed, 1 );
    return;
 }
 send_to_char("> &Ryou fail creating a mod module - try again&w\n\r",ch);
 return;

    case 1:
 if( !ch->dest_buf ) return;
 strcpy( arg, (const char*) ch->dest_buf );
 DISPOSE( ch->dest_buf );
 break;

    case SUB_TIMER_DO_ABORT:
 DISPOSE( ch->dest_buf );
 send_to_char("> &Ryour work is interrupted and you fail&w\n\r",ch);
        return;
    }

    for( obj = ch->first_carrying; obj; obj = obj_next )
    {
 obj_next = obj->next_content;

 if( obj->item_type == ITEM_MIRROR && !checkchem )
 {
	 separate_obj(obj);
     cont = obj;
     checkchem = TRUE;
     continue;
 }

 if( obj->item_type == ITEM_TOOLKIT && !checktool )
     checktool = TRUE;
    }

    level = chance = IS_NPC(ch) ? ch->top_level : ch->pcdata->learned[gsn_codemed];

    if( number_percent() > chance || !checkchem || !checktool )
    {
		send_to_char( "> &Ryou fail to code the med module&w\n\r", ch);
		return;
    }

    cont->item_type = ITEM_MEDPAC;
    cont->value[0] = level/10;
    sprintf( buf, "%s [medmod]", arg );
    STRFREE( cont->name );
    cont->name = STRALLOC( buf );
    STRFREE( cont->short_descr );
    cont->short_descr = STRALLOC( buf );
    //sprintf( buf, " was left here.");
    STRFREE( cont->description );
    cont->description = STRALLOC( buf );
    if( !CAN_WEAR( cont, 1 << wearbit ) )
    SET_BIT( cont->wear_flags, 1 << wearbit );

    send_to_char("&G> you hold up your new med module\n\r",ch);
    act( AT_PLAIN, "> $n finished their new med module",ch,NULL,NULL,TO_ROOM);

	if ( number_percent() == 23 )
	{
		send_to_char( "> you feel more intelligent than before\n\r", ch );
		ch->perm_int++;
		ch->perm_int = UMIN( ch->perm_int , 25 );
	}

    learn_from_success( ch, gsn_codemed );

}

//done for Neuro
