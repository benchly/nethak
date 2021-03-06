#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "mud.h"

void do_sn_jackhammer(CHAR_DATA *ch, char *argument) {

	OBJ_DATA *obj;
	EXIT_DATA *xit, *texit;
	int edir;
	ROOM_INDEX_DATA *location;
	char buf[MAX_STRING_LENGTH];
	char bufa[MAX_STRING_LENGTH];
	PLANET_DATA *planet;
	bool ch_snippet;

	planet = ch->in_room->area->planet;

	if (IS_NPC(ch) || !ch->pcdata || !ch->in_room)
		return;

	if (IS_SET( ch->in_room->room_flags , ROOM_NOPEDIT )) {
		send_to_char("> you cannot use this app here\n\r", ch);
		return;
	}

	if (argument[0] == '\0') {
		send_to_char("> syntax: jackhammer <direction>\n\r", ch);
		return;
	}

	location = ch->in_room;
	edir = get_dir(argument);
	xit = get_exit(ch->in_room, edir);

	if (!xit) {
		send_to_char("> &Rno connection in that direction&w\n\r", ch);
		return;
	}

	if (!IS_SET( xit->exit_info , EX_ISDOOR )) {
		send_to_char("> &Rno codegate in that direction&w\n\r", ch);
		return;
	} else {

		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if ( obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"jackhammer") && ch_snippet == FALSE ) {
				ch_snippet = TRUE;
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
			}
		}

		if (!ch_snippet) {
			send_to_char("> &Rjackhammer application needed&w\n\r", ch);
			return;
		}

		sprintf(buf, "> %s's jackhammer breaks down the gate to the: %s",
				ch->name, dir_name[edir]);
		echo_to_room(AT_RED, ch->in_room, buf);


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

		REMOVE_BIT( xit->exit_info , EX_ISDOOR );
		REMOVE_BIT( xit->exit_info , EX_LOCKED );
		REMOVE_BIT( xit->exit_info , EX_CLOSED );
		xit->key = -1;
		texit = get_exit_to(xit->to_room, rev_dir[edir], ch->in_room->vnum);
		if (texit) {
			REMOVE_BIT( texit->exit_info , EX_ISDOOR );
			REMOVE_BIT( texit->exit_info , EX_LOCKED );
			REMOVE_BIT( texit->exit_info , EX_CLOSED );
			texit->key = -1;
		}

	}

	SET_BIT( ch->in_room->area->flags , AFLAG_MODIFIED );

}

void do_sn_krash(CHAR_DATA *ch, char *argument) {

	CLAN_DATA *clan;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];
	char bufa[MAX_STRING_LENGTH];
	PLANET_DATA *planet;
	bool ch_snippet;

	planet = ch->in_room->area->planet;

	if (IS_NPC(ch) || !ch->pcdata || !ch->in_room)
		return;

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }

	   clan = ch->pcdata->clan;

	   if ( ( planet = ch->in_room->area->planet ) == NULL )
	   {
	       send_to_char ( "> &Ryou cannot do that here&w\n\r" , ch );
	       return;
	   }

	   if ( IS_SET( planet->flags, PLANET_NOCAP ) )
	   {
	       send_to_char( "> &Ryou cannot do that here&w\n\r", ch );
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
			send_to_char( "> cannot krash in combat\n\r" , ch );
			return;
		}

		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are sleeping\n\r" , ch );
			return;
		}

	   if ( clan == planet->governed_by )
	   {
	       send_to_char ( "> &Ryour organization already controls this system&w\n\r" , ch );
	       return;
	   }


		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"krash") && ch_snippet == FALSE) {
				ch_snippet = TRUE;
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
			}
		}

		if (!ch_snippet) {
			send_to_char("> &Rkrash application needed&w\n\r", ch);
			return;
		}

		WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

		sprintf(buf, "> %s's krash slows down the cpu of the system.",
				ch->name);
		echo_to_room(AT_YELLOW, ch->in_room, buf);

		sprintf(bufa, "> %s used KRASH in %s ",
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

		   planet->pop_support -= 1;

			if ( planet->pop_support > 100 )
				planet->pop_support = 100;
			if ( planet->pop_support < -100 )
				planet->pop_support = -100;

			return;

}

void do_sn_lockout(CHAR_DATA *ch, char *argument) {

	CLAN_DATA *clan;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];
	char bufa[MAX_STRING_LENGTH];
	PLANET_DATA *planet;
	bool ch_snippet;

	planet = ch->in_room->area->planet;

	if (IS_NPC(ch) || !ch->pcdata || !ch->in_room)
		return;

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }

	   clan = ch->pcdata->clan;

	   if ( ( planet = ch->in_room->area->planet ) == NULL )
	   {
	       send_to_char ( "> &Ryou cannot do that here&w\n\r" , ch );
	       return;
	   }

	   if ( IS_SET( planet->flags, PLANET_NOCAP ) )
	   {
	       send_to_char( "> &Ryou cannot do that here&w\n\r", ch );
	       return;
	   }

		if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
		{
			set_char_color( AT_MAGIC, ch );
			send_to_char( "> this is not a good place to do that\n\r", ch );
			return;
		}

		if ( !IS_SET( ch->in_room->room_flags, ROOM_BARRACKS ) )
		{
			set_char_color( AT_MAGIC, ch );
			send_to_char( "> this is not a good place to do that\n\r", ch );
			return;
		}

		if ( ch->position == POS_FIGHTING )
		{
			send_to_char( "> cannot lockout in combat\n\r" , ch );
			return;
		}

		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are sleeping\n\r" , ch );
			return;
		}

	   if ( clan == planet->governed_by )
	   {
	       send_to_char ( "> &Ryou can not lockout your own firewalls&w\n\r" , ch );
	       return;
	   }

	   if (ch->in_room->lockdown > 0){

	       send_to_char ( "> &Rthis firewall is already locked down&w\n\r" , ch );
	       return;

	   }


		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"lockout") && ch_snippet == FALSE) {
				ch_snippet = TRUE;
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
			}
		}

		if (!ch_snippet) {
			send_to_char("> &Rlockout application needed&w\n\r", ch);
			return;
		}

		WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

		sprintf(buf, "> %s's applications locks down this firewall.",
				ch->name);
		echo_to_room(AT_YELLOW, ch->in_room, buf);

		sprintf(bufa, "> %s used LOCKOUT in %s ",
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

		ch->in_room->lockdown = 1;

		return;

}

void do_sn_reinit(CHAR_DATA *ch, char *argument) {

	CLAN_DATA *clan;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];
	PLANET_DATA *planet;
	bool ch_snippet;

	planet = ch->in_room->area->planet;

	if (IS_NPC(ch) || !ch->pcdata || !ch->in_room)
		return;

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }

	   clan = ch->pcdata->clan;

	   if ( ( planet = ch->in_room->area->planet ) == NULL )
	   {
	       send_to_char ( "> &Ryou cannot do that here&w\n\r" , ch );
	       return;
	   }

	   if ( IS_SET( planet->flags, PLANET_NOCAP ) )
	   {
	       send_to_char( "> &Ryou cannot do that here&w\n\r", ch );
	       return;
	   }

		if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
		{
			set_char_color( AT_MAGIC, ch );
			send_to_char( "> this is not a good place to do that\n\r", ch );
			return;
		}

		if ( !IS_SET( ch->in_room->room_flags, ROOM_BARRACKS ) )
		{
			set_char_color( AT_MAGIC, ch );
			send_to_char( "> this is not a good place to do that\n\r", ch );
			return;
		}

		if ( ch->position == POS_FIGHTING )
		{
			send_to_char( "> cannot reinit in combat\n\r" , ch );
			return;
		}

		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are sleeping\n\r" , ch );
			return;
		}

	   if ( clan != planet->governed_by )
	   {
	       send_to_char ( "> &Ryou can only reinit your own firewalls&w\n\r" , ch );
	       return;
	   }

	   if (ch->in_room->lockdown == 0){

	       send_to_char ( "> &Rthis firewall is already functional&w\n\r" , ch );
	       return;

	   }


		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"reinit") && ch_snippet == FALSE) {
				ch_snippet = TRUE;
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
			}
		}

		if (!ch_snippet) {
			send_to_char("> &Rreinit application needed&w\n\r", ch);
			return;
		}

		WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

		sprintf(buf, "> %s's applications reinitializes this firewall.",
				ch->name);
		echo_to_room(AT_YELLOW, ch->in_room, buf);

		ch->in_room->lockdown = 0;

		return;

}

void do_sn_nightvision(CHAR_DATA *ch, char *argument) {

	OBJ_DATA *obj;
	AFFECT_DATA af;
	char buf[MAX_STRING_LENGTH];
	bool ch_snippet;

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }


		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are hibernating\n\r" , ch );
			return;
		}



		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"nightvision") && ch_snippet == FALSE) {
				ch_snippet = TRUE;
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
			}
		}

		if (!ch_snippet) {
			send_to_char("> &Rnightvision application needed&w\n\r", ch);
			return;
		}

		WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

		sprintf(buf, "> %s uses a nightvision application",
				ch->name);
		echo_to_room(AT_RED, ch->in_room, buf);

		if ( !IS_AFFECTED( ch, AFF_DETECT_HIDDEN ) )
		{
	  	   af.type      = gsn_detection;
	  	   af.location  = 0;
	  	   af.modifier  = 0;
	  	   af.duration  = 5;
	  	   af.bitvector = AFF_DETECT_HIDDEN;
	  	   affect_to_char( ch, &af );
		}
		else{
			send_to_char( "> nightvision already running\n\r" , ch );
			return;
		}

		return;

}

void do_sn_spun(CHAR_DATA *ch, char *argument) {

	OBJ_DATA *obj;
	int energyplus;
	char buf[MAX_STRING_LENGTH];
	bool ch_snippet;

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }


		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are hibernating\n\r" , ch );
			return;
		}

		if ( ch->move >= ch->max_move )
		{
			send_to_char( "> you are already at maximum\n\r" , ch );
			return;
		}

		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"spun") && ch_snippet == FALSE) {
				ch_snippet = TRUE;
				energyplus = obj->value[0];
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
			}
		}

		if (!ch_snippet) {
			send_to_char("> &Rspun application needed&w\n\r", ch);
			return;
		}

		WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

		sprintf(buf, "> %s uses spun to regain some energy",
				ch->name);
		echo_to_room(AT_RED, ch->in_room, buf);


		if ( ch->move < ch->max_move )
		{
			if ( ch->move + energyplus > ch->max_move )
				ch->move = ch->max_move;
			else
				ch->move = ch->move + energyplus;
		}

		return;

}

void do_sn_reconstruct(CHAR_DATA *ch, char *argument) {

	OBJ_DATA *obj;
	int energyplus;
	char buf[MAX_STRING_LENGTH];
	bool ch_snippet;

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }


		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are hibernating\n\r" , ch );
			return;
		}

		if ( ch->hit >= ch->max_hit )
		{
			send_to_char( "> you are already at maximum\n\r" , ch );
			return;
		}

		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"reconstruct") && ch_snippet == FALSE) {
				ch_snippet = TRUE;
				energyplus = obj->value[0] / 4;
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
			}
		}

		if (!ch_snippet) {
			send_to_char("> &Rreconstruct application needed&w\n\r", ch);
			return;
		}

		WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

		sprintf(buf, "> %s uses reconstruct to regain some health",
				ch->name);
		echo_to_room(AT_RED, ch->in_room, buf);


		if ( ch->hit < ch->max_hit )
		{
			if ( ch->hit + energyplus > ch->max_hit )
				ch->hit = ch->max_hit;
			else
				ch->hit = ch->hit + energyplus;
		}

		return;

}

void do_sn_dropline(CHAR_DATA *ch, char *argument) {

	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];
	bool ch_snippet;

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }


		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are hibernating\n\r" , ch );
			return;
		}

		if ( ch->fighting )
		{
			send_to_char( "> dropline cannot be used in combat\n\r", ch );
			return;
		}

		if ( IS_SET( ch->in_room->room_flags, ROOM_ARENA ) )
		{
			send_to_char( "> &Rfinish the current match first&w\n\r", ch );
			return;
		}

		if ( ch->in_room->vnum <= 20 )
		{
			send_to_char( "> &Ryou cannot use this command in the tutorial&w\n\r", ch );
			return;
		}

		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"dropline") && ch_snippet == FALSE) {
				ch_snippet = TRUE;
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
			}
		}

		if (!ch_snippet) {
			send_to_char("> &Rdropline application needed&w\n\r", ch);
			return;
		}

		WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

		sprintf(buf, "> %s drops their line",
				ch->name);
		echo_to_room(AT_RED, ch->in_room, buf);

		if( !ch->plr_home )
		{
			char_from_room( ch );
			char_to_room( ch, get_room_index( ROOM_VNUM_STRAY ) );
			do_look( ch, "auto" );
			return;
		}

		char_from_room( ch );
		char_to_room( ch, ch->plr_home );
		do_look( ch, "auto" );

		return;


}

void do_sn_uninstall(CHAR_DATA *ch, char *argument) {

	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];
	bool ch_snippet;

	if (IS_NPC(ch) || !ch->pcdata)
		return;

	if (argument[0] == '\0') {
		send_to_char("> syntax: uninstall <skillsoft>\n\r", ch);
		return;
	}

		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if ( obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"uninstall") && ch_snippet == FALSE) {
				ch_snippet = TRUE;
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
			}
		}

		if (!ch_snippet) {
			send_to_char("> &Runinstall application needed&w\n\r", ch);
			return;
		}

	    int sn;

	    sn = skill_lookup( argument );

	    if ( sn == -1 )
	    {
	        send_to_char( "> no such skillsoft\n\r", ch );
		return;
	    }

	    if ( ch->pcdata->learned[sn] <= 0 )
	    {
	    send_to_char( "> you dont know that skillsoft\n\r", ch );
		return;
	    }

		sprintf(buf, "> %s uninstalls a skillsoft",
				ch->name);
		echo_to_room(AT_YELLOW, ch->in_room, buf);

	    send_to_char( "> you uninstalled the skillsoft\n\r", ch );

	    if ( ch->pcdata->learned[sn] == 100 )
	    {
		    ch->pcdata->adept_skills--;
	    }

	    ch->pcdata->learned[sn] = 0;
	    ch->pcdata->num_skills--;
	    save_char_obj(ch);

	    return;

}

void do_sn_anchor( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	ROOM_INDEX_DATA *destination;
	char buf[MAX_STRING_LENGTH];
	bool ch_snippet;
	int targetnode;

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }


		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are hibernating\n\r" , ch );
			return;
		}

		if ( ch->fighting )
		{
			send_to_char( "> anchor cannot be used in combat\n\r", ch );
			return;
		}

		if ( IS_SET( ch->in_room->room_flags, ROOM_ARENA ) )
		{
			send_to_char( "> &Rfinish the current match first&w\n\r", ch );
			return;
		}

		if ( ch->in_room->vnum <= 20 )
		{
			send_to_char( "> &Ryou cannot use this in the tutorial&w\n\r", ch );
			return;
		}

		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if ( obj->item_type == ITEM_SNIPPET && nifty_is_name("anchor", obj->name) && ch_snippet == FALSE ) {
				ch_snippet = TRUE;
				targetnode = obj->value[1];

				destination = get_room_index( targetnode );

				if ( IS_SET( destination->room_flags , ROOM_PLR_HOME ) )
				{
					send_to_char( "> &Rtarget location is not valid&w\n\r", ch );
					separate_obj(obj);
					obj_from_char(obj);
					extract_obj( obj );
					return;
				}

				separate_obj(obj);
				obj->value[0] -= 1;

				if (obj->value[0] < 1)
				{
				obj_from_char(obj);
				extract_obj( obj );
				send_to_char("> &Ranchor application has expired&w\n\r", ch);
				}
			}
		}

		if (!ch_snippet) {
			send_to_char("> &Ranchor application needed&w\n\r", ch);
			return;
		}



		WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

		sprintf(buf, "> %s uses an anchor app",
				ch->name);
		echo_to_room(AT_RED, ch->in_room, buf);

			char_from_room( ch );
			char_to_room( ch, get_room_index( targetnode ) );
			do_look( ch, "auto" );
			return;

	}

void do_sn_audit( CHAR_DATA *ch, char *argument )
{
	char buf [MAX_STRING_LENGTH];
	ROOM_INDEX_DATA	*location;
	AREA_DATA *area;
	PLANET_DATA *planet;
	OBJ_DATA *obj;
	bool ch_snippet;
	int chance, roll, margin, count = 0;

	location = ch->in_room;
	planet = ch->in_room->area->planet;
	area = ch->in_room->area;

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }


		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are hibernating\n\r" , ch );
			return;
		}

		if ( ch->fighting )
		{
			send_to_char( "> audit cannot be used in combat\n\r", ch );
			return;
		}

		if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
		{
			send_to_char( "> &Raudit cannot be used in safe nodes&w\n\r", ch );
			return;
		}

		if ( ch->in_room->vnum <= 20 )
		{
			send_to_char( "> &Ryou cannot use this in the tutorial&w\n\r", ch );
			return;
		}


		if ( argument[0] == '\0' )
		{
			send_to_char("> &Rsyntax: audit [option]&w\n\r", ch);
			send_to_char("> &Wgain information about specified node&w\n\r", ch);
			send_to_char("> &Woptions: fw (firewall), rn (runner)&w\n\r", ch);
			return;
		}

		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"audit") && ch_snippet == FALSE) {
				ch_snippet = TRUE;

				obj->value[0] -= 1;

				if (obj->value[0] < 1)
				{
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
				send_to_char("> &Raudit application has expired&w\n\r", ch);
				}

			}

		}

	if (!ch_snippet) {
		send_to_char("> &Raudit application needed&w\n\r", ch);
		return;
	}

	WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

    sprintf(buf,"> &y%s uses an audit application", ch->name);
    act(AT_WHITE,buf, ch, NULL, NULL, TO_ROOM);

	chance = IS_NPC(ch) ? ch->top_level
			: (int) (ch->pcdata->learned[gsn_spacecraft]);

	if ( location->level != 0 )
	chance -= ( location->level * 10 );

	roll = number_percent();

	if ( roll >= chance )
	{
		send_to_char("> &Ryou failed the audit&w\n\r", ch);

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

	margin = chance - roll;

	if ( !str_cmp( argument, "fw" ) )
	{
		ch_printf( ch, "> &Gaudit for: %s - type: %s&w\n\r", planet->name, argument );

		for ( location = planet->area->first_room ; location ; location = location->next_in_area )
		{
			if ( IS_SET( location->room_flags, ROOM_BARRACKS ) )
			{
				ch_printf( ch , "   %-15d  %s\n\r", location->vnum, location->name);
				count++;
			}
		}

	    if ( !count )
	    {
		set_char_color( AT_BLOOD, ch);
	        send_to_char( "     no firewalls in this system\n\r", ch );
	    }
	    else
	    {
			ch_printf( ch, "\n\r   &Wtotal: %d&w\n\r", count );

	    }

	}
	else
		do_sn_audit( ch, "");


    return;

}

void do_sn_shortcut( CHAR_DATA *ch, char *argument )
{
	char buf [MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	char arg1[MAX_INPUT_LENGTH];
	ROOM_INDEX_DATA	*location;
	AREA_DATA *area;
	PLANET_DATA *planet;
	OBJ_DATA *obj;
	bool ch_snippet;
	int chance, roll, margin, count = 0;
	int destinationid;

	argument = one_argument( argument , arg );
	argument = one_argument( argument , arg1 );

	location = ch->in_room;
	planet = ch->in_room->area->planet;
	area = ch->in_room->area;

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }


		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are hibernating\n\r" , ch );
			return;
		}

		if ( ch->fighting )
		{
			send_to_char( "> shortcut cannot be used in combat\n\r", ch );
			return;
		}

		if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
		{
			send_to_char( "> &Rshortcut cannot be used in safe nodes&w\n\r", ch );
			return;
		}

		if ( ch->in_room->vnum <= 20 )
		{
			send_to_char( "> &Ryou cannot use this in the tutorial&w\n\r", ch );
			return;
		}

		if ( IS_SET( ch->in_room->room_flags, ROOM_ARENA ) )
		{
			send_to_char( "> &Rfinish the current match first&w\n\r", ch );
			return;
		}


		if ( arg[0] == '\0' )
		{
			send_to_char("> &Rsyntax: shortcut [type] [nodeid]&w\n\r", ch);
			send_to_char("> &Wconnect to specified node&w\n\r", ch);
			send_to_char("> &Woptions: fw (firewall), in (intrusion)&w\n\r", ch);
			return;
		}

		if ( arg1[0] == '\0' )
		{
			send_to_char("> &Rerror: no nodeid specified&w\n\r", ch);
			return;
		}

		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"shortcut") && ch_snippet == FALSE) {
				ch_snippet = TRUE;

				obj->value[0] -= 1;

				if (obj->value[0] < 1)
				{
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
				send_to_char("> &Rshortcut application has expired&w\n\r", ch);
				}
			}

		}

	if (!ch_snippet) {
		send_to_char("> &Rshortcut application needed&w\n\r", ch);
		return;
	}

	WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

    sprintf(buf,"> &y%s uses a shortcut application", ch->name);
    act(AT_WHITE,buf, ch, NULL, NULL, TO_ROOM);

	chance = IS_NPC(ch) ? ch->top_level
			: (int) (ch->pcdata->learned[gsn_spacecraft]);

	if ( location->level != 0 )
	chance -= ( location->level * 10 );

	roll = number_percent();

	if ( roll >= chance )
	{
		send_to_char("> &Ryou failed to connect&w\n\r", ch);
		return;
	}

	margin = chance - roll;

	if ( !str_cmp( arg, "fw" ) )
	{
		for ( location = planet->area->first_room ; location ; location = location->next_in_area )
		{
			if ( IS_SET( location->room_flags, ROOM_BARRACKS ) )
			{

				destinationid = atoi(arg1);
				if ( location->vnum == destinationid )
				count++;

			}
		}

	    if ( !count )
	    {
		set_char_color( AT_BLOOD, ch);
	        send_to_char( "> &Rdestination not found&w\n\r", ch );
	    }
	    else
	    {
	    	send_to_char( "> &Gyou connect to the destination node&w\n\r", ch );
			char_from_room( ch );
			char_to_room( ch, get_room_index( destinationid ) );
			act( AT_ACTION, "> $n has entered the node", ch, NULL, NULL, TO_ROOM );
			do_look( ch, "auto" );
	    }

	}

	if ( !str_cmp( arg, "in" ) )
	{
		for ( location = planet->area->first_room ; location ; location = location->next_in_area )
		{
			if ( IS_SET( location->room_flags, ROOM_INTRUSION ) )
			{

				destinationid = atoi(arg1);
				if ( location->vnum == destinationid )
				count++;

			}
		}

	    if ( !count )
	    {
		set_char_color( AT_BLOOD, ch);
	        send_to_char( "> &Rdestination not found&w\n\r", ch );
	    }
	    else
	    {
	    	send_to_char( "> &Gyou connect to the destination node&w\n\r", ch );
			char_from_room( ch );
			char_to_room( ch, get_room_index( destinationid ) );
			act( AT_ACTION, "> $n has entered the node", ch, NULL, NULL, TO_ROOM );
			do_look( ch, "auto" );
	    }

	}

    return;

}

void do_sn_checkout( CHAR_DATA *ch, char *argument )
{
	char buf [MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	ROOM_INDEX_DATA	*location;
	AREA_DATA *area;
	PLANET_DATA *planet;
	OBJ_DATA *obj;
	bool ch_snippet;
	int chance, roll, margin;

	location = ch->in_room;
	planet = ch->in_room->area->planet;
	area = ch->in_room->area;

	argument = one_argument( argument, arg );

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }


		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are hibernating\n\r" , ch );
			return;
		}

		if ( ch->fighting )
		{
			send_to_char( "> checkout cannot be used in combat\n\r", ch );
			return;
		}

		if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
		{
			send_to_char( "> &Rcheckout cannot be used in safe nodes&w\n\r", ch );
			return;
		}


		if ( arg[0] == '\0' )
		{
			send_to_char("> &Rsyntax: checkout [program]&w\n\r", ch);
			send_to_char("> &Wgain resource from program&w\n\r", ch);
			return;
		}

	    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	    {
		send_to_char( "> &Rthat program is not here&w\n\r", ch);
		return;
	    }

	    if ( !IS_NPC(victim) )
	    {
		send_to_char( "> &Rnot on players&w\n\r", ch );
		return;
	    }

	    if ( victim->pIndexData->vnum != 56 && victim->pIndexData->vnum != 57 && victim->pIndexData->vnum != 58
	    		&& victim->pIndexData->vnum != 59)
	    {
		send_to_char( "> &Rthat is not a program&w\n\r", ch );
		return;
	    }

		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"checkout") && ch_snippet == FALSE) {
				ch_snippet = TRUE;

				obj->value[0] -= 1;

				if (obj->value[0] < 1)
				{
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
				send_to_char("> &Rcheckout application has expired&w\n\r", ch);
				}

			}

		}

	if (!ch_snippet) {
		send_to_char("> &Rcheckout application needed&w\n\r", ch);
		return;
	}

	WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

    sprintf(buf,"> &y%s uses a checkout application", ch->name);
    act(AT_WHITE,buf, ch, NULL, NULL, TO_ROOM);

	chance = IS_NPC(ch) ? ch->top_level
			: (int) (ch->pcdata->learned[gsn_spacecraft]);

	if ( location->level != 0 )
	chance -= ( location->level * 10 );

	roll = number_percent();

	if ( roll >= chance )
	{
		send_to_char("> &Ryou failed the checkout&w\n\r", ch);
		return;
	}

	margin = chance - roll;

    sh_int decrease = victim->top_level / 20;
	int chargain;

	if ( decrease == 1 )
		chargain = 1;
	else if ( decrease == 2 )
		chargain = 2;
	else if ( decrease == 3 )
		chargain = 4;
	else if ( decrease == 4 )
		chargain = 8;
	else if ( decrease == 5 )
		chargain = 16;
	else
		chargain = 1;

	switch(victim->pIndexData->vnum) {

	default:
		break;

	case 56:
		ch->pcdata->rentertain += chargain;

  	 victim->in_room->area->planet->entertain_count = victim->in_room->area->planet->entertain_count - decrease;
  	 victim->in_room->area->planet->entertain_count = UMAX( victim->in_room->area->planet->entertain_count , 0 );
		break;

	case 57:
		ch->pcdata->rmultimedia += chargain;
  	 victim->in_room->area->planet->multimedia_count = victim->in_room->area->planet->multimedia_count - decrease;
  	 victim->in_room->area->planet->multimedia_count = UMAX( victim->in_room->area->planet->multimedia_count , 0 );
		break;

	case 58:
		ch->pcdata->rfinance += chargain;
  	 victim->in_room->area->planet->finance_count = victim->in_room->area->planet->finance_count - decrease;
  	 victim->in_room->area->planet->finance_count = UMAX( victim->in_room->area->planet->finance_count , 0 );
		break;

	case 59:
		ch->pcdata->rproduct += chargain;
  	 victim->in_room->area->planet->product_count = victim->in_room->area->planet->product_count - decrease;
  	 victim->in_room->area->planet->product_count = UMAX( victim->in_room->area->planet->product_count , 0 );
		break;
	}

	ch_printf( ch , "> &Gyou gain %d repos from %s&w\n\r", chargain, victim->short_descr);
	act( AT_PLAIN, "> $n checks out a program", ch,
			NULL, argument , TO_ROOM );

	extract_char( victim, TRUE );

    return;

}

void do_sn_emp(CHAR_DATA *ch, char *argument) {

	OBJ_DATA *obj;
	CHAR_DATA *rch;
	int energyplus;
	char buf[MAX_STRING_LENGTH];
	bool ch_snippet;

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }


		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are hibernating\n\r" , ch );
			return;
		}

		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"emp") && ch_snippet == FALSE) {
				ch_snippet = TRUE;
				energyplus = obj->value[0];
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
			}
		}

		if (!ch_snippet) {
			send_to_char("> &Remp application needed&w\n\r", ch);
			return;
		}

		WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

		sprintf(buf, "> %s uses an emp application",
				ch->name);
		echo_to_room(AT_RED, ch->in_room, buf);

	    for ( rch = ch->in_room->first_person; rch; rch = rch->next_in_room )
	    {

	    	if ( IS_AFFECTED(rch, AFF_HIDE) && !IS_NPC(rch) ){
	    		affect_strip ( ch, gsn_hide			);
	    		REMOVE_BIT(rch->affected_by, AFF_HIDE);
	    		send_to_char("> &Ryou are no longer hidden&w\n\r", rch);
	    	}

	    	if ( IS_AFFECTED(rch, AFF_SNEAK) && !IS_NPC(rch) ){
	    		affect_strip ( ch, gsn_sneak			);
	    		REMOVE_BIT(rch->affected_by, AFF_SNEAK);
	    		send_to_char("> &Ryou are no longer sneaking&w\n\r", rch);
	    	}


	    }

	    return;

}

void do_sn_bounce( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int exit_dir;
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    bool nogo;
    ROOM_INDEX_DATA *to_room;
    int chance;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );


    if ( arg[0] == '\0' )
    {
	send_to_char( "> bounce whom\n\r", ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "> user not found\n\r", ch);
	return;
    }

    if (victim == ch)
    {
	send_to_char("> you bounce around\n\r", ch);
	return;
    }

    if( get_age( ch ) < 20 )
    {
	send_to_char("> wait until you get older before bouncing people around\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	send_to_char( "> bounce them in which direction\n\r", ch);
	return;
    }

    exit_dir = get_dir( arg2 );
    if ( IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    &&  get_timer(victim, TIMER_SHOVEDRAG) <= 0)
    {
	send_to_char("> that character cannot be bounced right now\n\r", ch);
	return;
    }
    victim->position = POS_SHOVE;
    nogo = FALSE;
    if ((pexit = get_exit(ch->in_room, exit_dir)) == NULL )
      nogo = TRUE;
    else
    if ( IS_SET(pexit->exit_info, EX_CLOSED)
    && (!IS_AFFECTED(victim, AFF_PASS_DOOR)
    ||   IS_SET(pexit->exit_info, EX_NOPASSDOOR)) )
      nogo = TRUE;
    if ( nogo )
    {
	send_to_char( "> there is no exit in that direction\n\r", ch );
        victim->position = POS_STANDING;
	return;
    }
    to_room = pexit->to_room;

    if ( IS_NPC(victim) )
    {
	send_to_char("> you can only shove player characters\n\r", ch);
	return;
    }

chance = 50;

/* Add 3 points to chance for every str point above 15, subtract for
below 15 */

chance += ((get_curr_str(ch) - 15) * 3);

chance += (ch->top_level - victim->top_level);

/* Debugging purposes - show percentage for testing */

/* sprintf(buf, "Shove percentage of %s = %d", ch->name, chance);
act( AT_ACTION, buf, ch, NULL, NULL, TO_ROOM );
*/

if (chance < number_percent( ))
{
  send_to_char("> you failed\n\r", ch);
  victim->position = POS_STANDING;
  return;
}
    act( AT_ACTION, "> you shove $M", ch, NULL, victim, TO_CHAR );
    act( AT_ACTION, "> $n shoves you", ch, NULL, victim, TO_VICT );
    move_char( victim, get_exit(ch->in_room,exit_dir), 0);
    if ( !char_died(victim) )
      victim->position = POS_STANDING;
    WAIT_STATE(ch, 12);
    /* Remove protection from shove/drag if char shoves -- Blodkai */
    if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)
    &&   get_timer(ch, TIMER_SHOVEDRAG) <= 0 )
      add_timer( ch, TIMER_SHOVEDRAG, 10, NULL, 0 );

    return;

}

void do_sn_annex(CHAR_DATA *ch, char *argument) {

	CLAN_DATA *clan;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];
	PLANET_DATA *planet;
	bool ch_snippet;
	int chance;


	planet = ch->in_room->area->planet;

	if (IS_NPC(ch) || !ch->pcdata || !ch->in_room)
		return;

	if ( IS_NPC(ch) || !ch->pcdata )
	   {
	       send_to_char ( "huh?\n\r" , ch );
	       return;
	   }

	   clan = ch->pcdata->clan;

	   if ( ( planet = ch->in_room->area->planet ) == NULL )
	   {
	       send_to_char ( "> &Ryou cannot do that here&w\n\r" , ch );
	       return;
	   }

	   if ( IS_SET( planet->flags, PLANET_NOCAP ) )
	   {
	       send_to_char( "> &Ryou cannot do that here&w\n\r", ch );
	       return;
	   }

	   if ( IS_SET( planet->flags, PLANET_HIDDEN ) )
	   {
	       send_to_char( "> &Ryou cannot do that here&w\n\r", ch );
	       return;
	   }

	   if ( IS_SET( planet->flags, PLANET_SHUT ) )
	   {
	       send_to_char( "> &Ryou cannot do that here&w\n\r", ch );
	       return;
	   }

		if ( IS_SET( ch->in_room->room_flags, ROOM_NOPEDIT ) )
		{
			send_to_char( "> &Ryou can not create nodes from this node&w\n\r", ch );
			return;
		}

		if ( ch->position == POS_FIGHTING )
		{
			send_to_char( "> cannot annex in combat\n\r" , ch );
			return;
		}

		if ( ch->position <= POS_SLEEPING )
		{
			send_to_char( "> you are sleeping\n\r" , ch );
			return;
		}

	   if ( clan != planet->governed_by )
	   {
	       send_to_char ( "> &Ryour organization does not control this system&w\n\r" , ch );
	       return;
	   }

	   if ( !str_cmp(ch->in_room->owner, ch->name) )
	   {
	       send_to_char ( "> &Ryou already own this node&w\n\r" , ch );
	       return;
	   }

		ch_snippet = FALSE;

		for (obj = ch->last_carrying; obj; obj = obj->prev_content) {
			if (obj->item_type == ITEM_SNIPPET && !strcmp(obj->name,
					"annex") && ch_snippet == FALSE) {
				ch_snippet = TRUE;
				separate_obj(obj);
				obj_from_char(obj);
				extract_obj( obj );
			}
		}

		if (!ch_snippet) {
			send_to_char("> &Rannex application needed&w\n\r", ch);
			return;
		}

		WAIT_STATE( ch, skill_table[gsn_propaganda]->beats );

		chance = (int) ((ch->pcdata->learned[gsn_spacecraft]) - (ch->in_room->level * 10));

		//ch_printf( ch, "> &GCHANCE: %d&w\n\r", chance );

		if ( number_percent( ) > chance )
		{
			send_to_char( "> &Ryou fail to annex. try again!&w\n\r", ch );
			return;
		}

		sprintf(buf, "> %s annexes this node.",
				ch->name);
		echo_to_room(AT_YELLOW, ch->in_room, buf);

		STRFREE( ch->in_room->owner );
		ch->in_room->owner = STRALLOC( ch->name );

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
