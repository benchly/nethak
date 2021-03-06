/*
 *  Original Header
 *  ---------------
 *
 *  This Code originally created by Matthew Peck, aka Valatar
 *              sauron@digital-genesis.com
 *  Modified by Nebseni of Clandestine MUD
 *              http://clandestine.mudnet.net
 *
 *  Created so that I wouldn't have to make a CGI script that would update
 *  the page everytime you check.  All you have to do to use this is email 
 *  me and let me know that you are.  Include your mud address and webpage 
 *  if you don't mind.  You have to know a bit of HTML to 
 *  make the changes necessary for this to work for your mud, but other 
 *  than that, its all self contained.  This code is compatible with Lope's
 *  Colour codes, if you use some other system, you'll need to change 
 *  get_who_data to fit.  If there are any problems, email me and let me know.
 *
 *  To get this to work, save this as a .c file.  Then add a line in update.c
 *  that declares the function, and another that calls it.  I added it to 
 *  the pulse_music part, so it updates about every 5 seconds.  You can add
 *  it elsewhere if you like.  The declaration looks like this:
 *                  void    who_html_update args( ( void ) );
 *  Then the function call looks like this:
 *                  who_html_update();
 *  Update the makefile to include this code, then compile and you'll be all set.
 */

/*
 * Converted to work with SWR by Odis
 *		admin@ew.xidus.net
 *     huhahua@yahoo.com
 *     http://ew.xidus.net
 *
 * For more clear instructions, please read the Readme file included with this distribution
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include "mud.h"

#if !defined(macintosh)
extern int _filbuf args( (FILE *) );
#endif

int html_colour args (( char type, char *string ));
void html_colourconv args (( char *buffer, const char *txt, CHAR_DATA *ch ));

void who_html_update(void) {

	/* this code assumes 45-character titles and max color switches (485 bytes).
	 if title length is increased the buf sizes must be increased */
	FILE *fp;
	DESCRIPTOR_DATA *d;
	char buf[2 * MAX_INPUT_LENGTH];
	char buf2[2 * MAX_INPUT_LENGTH];
	int sn, count;

#ifdef DEBUG
	Debug ("who_html_update");
#endif

	buf[0] = '\0';
	buf2[0] = '\0';

	fclose(fpReserve);
	if ((fp = fopen("../../public_html/online.html", "w")) == NULL)

	/*
	 * change the directory above to the absolute directory and filename
	 * of the page you are going to make.  IMPORTANT:  The file needs to
	 * exist before you attempt to run this.
	 *         --Valatar
	 */
	/*
	 * Note: The above path specifies to move up out of the src directory,
	 * then out of the swr directory, into the public_html directory, and then the destination..
	 * which is online.html
	 */

	{
		bug("online.html: fopen", 0);
		perror("online.html");
	} else {
		fprintf(fp, "<html>\n");
		fprintf(fp, "<head>\n");
		fprintf(fp, "<title>");
		fprintf(fp, "Players currently jacked-in:");

		fprintf(fp, "</title>\n");
		fprintf(fp, "<META HTTP-EQUIV=REFRESH CONTENT=30>\n");
		fprintf(
				fp,
				"<BODY TEXT=" "#4189c8" " BGCOLOR=" "#FFFFFF" " LINK=" "#00FFFF" "");
		fprintf(fp, "VLINK=" "#FFFFFF" " ALINK=" "#008080" ">\n\n");
		fprintf(fp, "<CENTER>\n\n<TABLE BORDER=0 BGCOLOR=" "#FFFFFF" " >\n");
		fprintf(fp, "<TR ALIGN=CENTER VALIGN=CENTER>\n");
		//fprintf(fp, "<TD>Level</TD>\n");
		fprintf(fp,
				"<TD><U><B><h3>Runner-Title [Skill-Levels]</h3></B></U><P></TD></TR>\n");

		for (d = first_descriptor; d != NULL; d = d->next) {
			CHAR_DATA *wch;
			char class[5];

			if (d->connected != CON_PLAYING)
				continue;
			wch = (d->original != NULL) ? d->original : d->character;
			class[0] = '\0';

			if (!IS_SET(wch->act, PLR_WIZINVIS )) {
				fprintf(fp, "<TR ALIGN=CENTER VALIGN=CENTER>\n");
				//fprintf(fp, "<TD>%d</TD>\n<TD>", wch->top_level);
				fprintf(fp, "<TD>");
				buf2[0] = '\0';
				//sprintf(buf2, "%s", (IS_NPC(wch)? "" : wch->pcdata->title));

				count = 0;

				for (sn = 0; sn < top_sn; sn++)
					if (wch->pcdata->learned[sn] > 0) {
						count = count + wch->pcdata->learned[sn];
					}

				sprintf(buf2, "%s  [%d]", (IS_NPC(wch) ? ""
						: wch->pcdata->title), count);

				html_colourconv(buf, buf2, wch);
				fprintf(fp, "%s", buf);
				fprintf(fp, "</TD></TR>\n");

			} /*end if */
		} /*end for */

		fprintf(fp, "</TABLE></CENTER>\n");
		fprintf(fp, "<BR><BR><BR><BR>\n\n");
		fprintf(fp, "<font face=" "Times New Roman" "><center>\n");
		sprintf(buf, "Last updated at %s GMT\n",
				((char *) ctime(&current_time)));
		fprintf(fp, "%s", buf);
		fprintf(fp, "</center></font>\n");

		//fprintf(fp, "<br><br>\n");
		//fprintf(fp, "<CENTER><P><A HREF=\"index.html\">\n");
		/*
		 * You may need to change the line above, depending on where you want this link to go to
		 */
		//fprintf(fp, "Return to main page</A> </P></CENTER></Font>\n");

		fprintf(fp, "</body>\n");
		fprintf(fp, "</html>\n");
		fclose(fp);
		fpReserve = fopen(NULL_FILE, "r");
	} /*end if */

	return;
}/* end function */

void metro_html_update(void) {

	/* this code assumes 45-character titles and max color switches (485 bytes).
	 if title length is increased the buf sizes must be increased */
	FILE *fp;
	DESCRIPTOR_DATA *d;
	char buf[2 * MAX_INPUT_LENGTH];
	char buf2[2 * MAX_INPUT_LENGTH];
	int sn, count;

#ifdef DEBUG
	Debug ("metro_html_update");
#endif

	buf[0] = '\0';
	buf2[0] = '\0';

	fclose(fpReserve);
	if ((fp = fopen("../../public_html/metro.html", "w")) == NULL)

	/*
	 * change the directory above to the absolute directory and filename
	 * of the page you are going to make.  IMPORTANT:  The file needs to
	 * exist before you attempt to run this.
	 *         --Valatar
	 */
	/*
	 * Note: The above path specifies to move up out of the src directory,
	 * then out of the swr directory, into the public_html directory, and then the destination..
	 * which is online.html
	 */

	{
		bug("metro.html: fopen", 0);
		perror("metro.html");
	} else {
		fprintf(fp, "<html>\n");
		fprintf(fp, "<head>\n");
		fprintf(fp, "<title>");
		fprintf(fp, "Players currently jacked-in:");

		fprintf(fp, "</title>\n");
		fprintf(fp, "<META HTTP-EQUIV=REFRESH CONTENT=30>\n");
		fprintf(
				fp,
				"<BODY TEXT=" "#4189c8" " BGCOLOR=" "#FFFFFF" " LINK=" "#00FFFF" "");
		fprintf(fp, "VLINK=" "#FFFFFF" " ALINK=" "#008080" ">\n\n");
		fprintf(fp, "<CENTER>\n\n<TABLE BORDER=0 BGCOLOR=" "#FFFFFF" " >\n");
		fprintf(fp, "<TR ALIGN=CENTER VALIGN=CENTER>\n");
		//fprintf(fp, "<TD>Level</TD>\n");
		fprintf(fp,
				"<TD><U><B><h3>Runner-Title [Metro Score]</h3></B></U><P></TD></TR>\n");

		for (d = first_descriptor; d != NULL; d = d->next) {
			CHAR_DATA *wch;
			char class[5];

			if (d->connected != CON_PLAYING)
				continue;
			wch = (d->original != NULL) ? d->original : d->character;
			class[0] = '\0';

			if (!IS_SET(wch->act, PLR_WIZINVIS )) {
				fprintf(fp, "<TR ALIGN=CENTER VALIGN=CENTER>\n");
				fprintf(fp, "<TD>");
				buf2[0] = '\0';

				sprintf(buf2, "%s  [%d]", (IS_NPC(wch) ? ""
						: wch->pcdata->title), wch->pcdata->homesmetro);

				html_colourconv(buf, buf2, wch);
				fprintf(fp, "%s", buf);
				fprintf(fp, "</TD></TR>\n");

			} /*end if */
		} /*end for */

		fprintf(fp, "</TABLE></CENTER>\n");
		fprintf(fp, "<BR><BR><BR><BR>\n\n");
		fprintf(fp, "<font face=" "Times New Roman" "><center>\n");
		sprintf(buf, "Last updated at %s GMT\n",
				((char *) ctime(&current_time)));
		fprintf(fp, "%s", buf);
		fprintf(fp, "</center></font>\n");

		//fprintf(fp, "<br><br>\n");
		//fprintf(fp, "<CENTER><P><A HREF=\"index.html\">\n");
		/*
		 * You may need to change the line above, depending on where you want this link to go to
		 */
		//fprintf(fp, "Return to main page</A> </P></CENTER></Font>\n");

		fprintf(fp, "</body>\n");
		fprintf(fp, "</html>\n");
		fclose(fp);
		fpReserve = fopen(NULL_FILE, "r");
	} /*end if */

	return;
}/* end function */

void orgs_html_update(void) {

	FILE *fp;
	char buf[2 * MAX_INPUT_LENGTH];
	char buf2[2 * MAX_INPUT_LENGTH];

	// might need to change the names of the variables below if you want them to make more sense for the thing you want to do
	// also need to change the structure that you want to get data from, here it is a CLAN structure and the PLANET structure to get
	// the score of the clan

	int score;
	CLAN_DATA *clan;
	PLANET_DATA *planet;

	// just adjust the first bit of the file name. eg: orgs

#ifdef DEBUG
	Debug ("orgs_html_update");
#endif

	buf[0] = '\0';
	buf2[0] = '\0';

	fclose(fpReserve);

	// change file name of html file. eg: orgs.html

	if ((fp = fopen("../../public_html/orgs.html", "w")) == NULL)

		/*
		 * change the directory above to the absolute directory and filename
		 * of the page you are going to make.  IMPORTANT:  The file needs to
		 * exist before you attempt to run this.
		 *         --Valatar
		 */
		/*
		 * Note: The above path specifies to move up out of the src directory,
		 * then out of the swr directory, into the public_html directory, and then the destination..
		 * which is online.html
		 */

	{

		// same here, just change orgs

		bug("orgs.html: fopen", 0);
		perror("orgs.html");
	} else {
		fprintf(fp, "<html>\n");
		fprintf(fp, "<head>\n");
		fprintf(fp, "<title>");

		// change name of page
		fprintf(fp, "Organization Info:");

		fprintf(fp, "</title>\n");
		fprintf(fp, "<META HTTP-EQUIV=REFRESH CONTENT=30>\n");
		fprintf(
				fp,
				"<BODY TEXT=" "#4189c8" " BGCOLOR=" "#FFFFFF" " LINK=" "#00FFFF" "");
		fprintf(fp, "VLINK=" "#FFFFFF" " ALINK=" "#008080" ">\n\n");
		fprintf(fp, "<CENTER>\n\n<TABLE BORDER=0 BGCOLOR=" "#FFFFFF" " >\n");
		fprintf(fp, "<TR ALIGN=CENTER VALIGN=CENTER>\n");

		// change text for header row
		fprintf(fp,
				"<TD><U><B><h3>Organization [Score]</h3></B></U><P></TD></TR>\n");

		// now here is the main bit. I look through all organizations in the game

		for (clan = first_clan; clan; clan = clan->next) {
			// setting score to 0
			score = 0;

			// looping through all systems to see if it is owned by the organization that is currently checked by the for loop
			for (planet = first_planet; planet; planet = planet->next)
				if (clan == planet->governed_by) {
					score += get_taxes(planet) / 720;
				}

			fprintf(fp, "<TR ALIGN=CENTER VALIGN=CENTER>\n");
			fprintf(fp, "<TD>");
			buf2[0] = '\0';

			// I am fetching the name of the clan and the score into the string variable buf2
			// normally just change clan->name and score to what you want to have
			sprintf(buf2, "%s  [%d]", clan->name, score);

			fprintf(fp, "%s", buf2);
			fprintf(fp, "</TD></TR>\n");

		}

		fprintf(fp, "</TABLE></CENTER>\n");
		fprintf(fp, "<BR><BR><BR><BR>\n\n");
		fprintf(fp, "<font face=" "Times New Roman" "><center>\n");
		sprintf(buf, "Last updated at %s GMT\n",
				((char *) ctime(&current_time)));
		fprintf(fp, "%s", buf);
		fprintf(fp, "</center></font>\n");

		fprintf(fp, "</body>\n");
		fprintf(fp, "</html>\n");
		fclose(fp);
		fpReserve = fopen(NULL_FILE, "r");
	} /*end if */

	return;
}/* end function */

int html_colour(char type, char *string) {
	char code[25];
	char *p = '\0';

#ifdef DEBUG
	Debug ("html_colour");
#endif

	switch (type) {
	default:
	case '\0':
		code[0] = '\0';
		break;
	case ' ':
		sprintf(code, " ");
		break;
	case 'x':
		sprintf(code, "<font color=" "#006400" ">");
		break;
	case 'b':
		sprintf(code, "<font color=" "#00008B" ">");
		break;
	case 'c':
		sprintf(code, "<font color=" "#008B8B" ">");
		break;
	case 'g':
		sprintf(code, "<font color=" "#006400" ">");
		break;
	case 'm':
		sprintf(code, "<font color=" "#8B008B" ">");
		break;
	case 'r':
		sprintf(code, "<font color=" "#8B0000" ">");
		break;
	case 'w':
		sprintf(code, "<font color=" "#808080" ">");
		break;
	case 'y':
		sprintf(code, "<font color=" "#808000" ">");
		break;
	case 'B':
		sprintf(code, "<font color=" "#0000FF" ">");
		break;
	case 'C':
		sprintf(code, "<font color=" "#OOFFFF" ">");
		break;
	case 'G':
		sprintf(code, "<font color=" "#00FF00" ">");
		break;
	case 'M':
		sprintf(code, "<font color=" "#FF00FF" ">");
		break;
	case 'R':
		sprintf(code, "<font color=" "#FF0000" ">");
		break;
	case 'W':
		sprintf(code, "<font color=" "#FFFFFF" ">");
		break;
	case 'Y':
		sprintf(code, "<font color=" "#FFFF00" ">");
		break;
	case 'D':
		sprintf(code, "<font color=" "#636363" ">");
		break;
	case '{':
		sprintf(code, "{");
		break;
	}

	p = code;
	while (*p != '\0') {
		*string = *p++;
		*++string = '\0';
	}

	return (strlen(code));
}

/*
 * Note: Background colors were never tested on here because my mud doesn't use them
 */
void html_colourconv(char *buffer, const char *txt, CHAR_DATA *ch) {
	const char *point;
	int skip = 0;

#ifdef DEBUG
	Debug ("html_colourconv");
#endif

	for (point = txt; *point; point++) {
		if (*point == '&') {
			point++;
			if (*point == '\0')
				point--;
			else
				skip = html_colour(*point, buffer);
			while (skip-- > 0)
				++buffer;
			continue;
		}
		/* Following is put in to prevent adding HTML links to titles,
		 except for IMMS who know what they're doing and can be
		 punished if they screw it up! */
		if ((*point == '<') && (!IS_IMMORTAL(ch))) {
			*buffer = '[';
			*++buffer = '\0';
			continue;
		}
		if ((*point == '>') && (!IS_IMMORTAL(ch))) {
			*buffer = ']';
			*++buffer = '\0';
			continue;
		}
		*buffer = *point;
		*++buffer = '\0';
	}
	*buffer = '\0';
	return;
}
