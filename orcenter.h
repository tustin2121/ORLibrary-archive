!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.12 ORCenter [Z, GLULX]
! Centers text (with wordwrap)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- - 
! License: 
! This work is licensed under the Creative Commons Attribution-ShareAlike License
! (http://creativecommons.org/licenses/by-sa/1.0/).
!
! In summary: you must credit the original author(s); if you alter, transform, or 
! build upon this software, you may distribute the SOURCE FORM of the resulting 
! work only under a license identical to this one. Note that the ShareAlike clause 
! does not affect the way in which you distribute the COMPILED FORM of works built upon
! this software. Copyright remains with the original author(s), from whom you
! must seek permission if you wish to vary any of the terms of this license.
! The author(s) would also welcome bug reports and enhancement suggestions.
!--------------------------------------------------------------------------------------
! AutoDep:	none
!--------------------------------------------------------------------------------------
! center() is a function that works similar to  print except that text is centered 
! on the screen.  Wordwrap also occurs as it does with print, but for centered text, it 
! is generally more attractive to limit the width of a line.  This can be accomplished 
! by specifying an additional value representing the maximum width of a line.  If the 
! width is either not specified or is specified to be larger than the screen, then the 
! screen width is used instead.
!
! It should also be noted that Center takes line breaks into account. For example:
!
!	center("I wandered lonely as a cloud,^That floats on high o'er vales 
!		   and hills,^When all at once I saw a crowd,^A host of golden 
!		   daffodils,^Beside the lake, beneath the trees,^Fluttering and 
!		   dancing in the breeze^^--William Wordsworth");
!
! Will acurately center eight lines of text (including the blank line).
!
! For less poetic centering, the second parameter, width, can be specified:
!
!	center("Fore score and seven years ago our forefathers brought forth on this continent, a new nation, conceived in Liberty, and dedicated to the proposition that all men are created equal.",20);
!
! Will center the specified text on the screen, breaking it up in intervals that 
! are no greater than 20 characters in length.
!
! The final parameter is highlight.  Pass this parameter as a one to get the centered 
! text to appear in reverse font.  Pass a two to this parameter to get the text AND ITS 
! PRECEEDING SPACES to appear in reverse font.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORCenter";					!Constant USE_ORCenter; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORCenter; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.04.01	Initial Creation
! 2002.01.01	Added support for hilighting.
! 2002.02.10	Added support for GLULX.
! 2002.02.12	Mysterious ifdef bug.
! 2002.02.19	Center in status line bug.
! 2002.02.20	Reverse preceding text bug.
! 2002.02.26	Fit to new template.
! 2002.02.28	Fixed bug with WORDSIZE defaulting.
! 2002.04.08	Added support for dynastrings to be passed in so that preformatted 
!				text can be rendered.
! 2002.04.14	Fixed issue caused by .h removal.
! 2003.02.04	changed length of temp array (yes, I'm at that point now.)
! 2003.03.29	Modified Glulx support (leveraging style_hints for centering in the main 
!				window.  These take up user1 & 2.)
! 2003.06.30	The above modifications inhibited compiling to Z-Code.  Fixed this
!				and added auto-depend filling for the Stylehints.
! 2003.12.14	Modified the array ... table definition of ORTempBuffer to take advantage 
!				of the new array ... buffer definition (and also avoid new warning message)
!				Also removed WORDSIZE definition since the 6.3 compiler already defines this.
! 2004.01.30	Removed references to reverse stylehint.
! 2004.02.12	Changed autodependancy code to not even bother trying to include 
!				ORStyleHint if not compiling for Glulx
!--------------------------------------------------------------------------------------

message "          Processing library extension ORCenter...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORCenter forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
#ifdef TARGET_GLULX;
	#ifndef ORLibraryInclude; message fatalerror "ORCenter has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORStyleHint_DONEREPLACE; default USE_ORStyleHint 0; message "          [ORCenter forcing inclusion of ORStyleHint]"; #include "ORStyleHint"; #endif; 
#endif;
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORCenter_DONEREPLACE; constant ORCenter_DONEREPLACE; #ifdef ORCenter_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORCenter_DONEMESSAGE; constant ORCenter_DONEMESSAGE;  #ifdef ORCenter_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
! #ifndef OREnglish; message fatalerror "ERROR!!!! ORCenter requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORCenter_DONECODE;constant ORCenter_DONECODE; #ifdef ORCenter_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	system_file;

	#ifndef ORTempBuffer ;array ORTempBuffer buffer 500; #endif; !ORTempBuffer is used thoughout the library 
	[center text maxwidth hilight width markerstart markerend o i offset sv len svt inFixedWidth;inFixedWidth=true;
		svt=0; sv=0; !--suppress compiler warnings...
		font off; !force a monospaced type		
	#ifdef TARGET_GLULX;
		!--------------------------------------------------------------------------------------------
		! For now, assume that if the current window is NOT the main window then it is statusbar-like.
		! This allows for additional menu windows to work as expected.
		!--------------------------------------------------------------------------------------------
		if(glk_stream_get_current()==glk_window_get_stream(gg_mainwin)) inFixedWidth=false;
		!----------------------------------------------
		!--if(inFixedWidth==true) !--if we are currently writing to the status then get the width of the status window
			glk_window_get_size(gg_statuswin, gg_arguments, gg_arguments+WORDSIZE);
		!--else
		!--	glk_window_get_size(gg_mainwin, gg_arguments, gg_arguments+WORDSIZE); !--otherwise get the width of the main window
        width = gg_arguments-->0;
	#ifnot;
		@loadb $21 0 -> width;				!first get the width of the display
	#endif;
		width=width-2;							!subtract a little from the width
		if(maxwidth==0 || maxwidth >width) maxwidth=width;		!default maxwidth of line to the width of the display if it is not defined or too large to display

		!copy the text to an array that we can manipulate
	#ifndef TARGET_GLULX;
		@output_stream 3 ORTempBuffer;	!capture output to an array
	#ifnot;
		sv=glk_stream_get_current();
		glk_stream_set_current(glk_stream_open_memory(ORTempBuffer+WORDSIZE, 1000, filemode_Write, 661));
	#endif;
		switch(metaclass(text)){
			string: 	print (string)text;		!print text in array
			routine:	text();
			object:		
#ifdef USE_ORDynaString;
				if(text ofclass ORDynaString)
					print (dynastring)text;
				else
					print (name)text;
#ifnot;
				print (name)text;
#endif;
		}
	#ifndef TARGET_GLULX;
		@output_stream -3;		!release output back to screen
		len=ORTempBuffer-->0;
	#ifnot;
		glk_stream_close(glk_stream_get_current(), gg_arguments);
		glk_stream_set_current(sv);
		len=gg_arguments-->1;
	#endif;
		for(markerend=markerstart=o=WORDSIZE:o<=(len+WORDSIZE):o++)
		{
			if(ORTempBuffer->o==' ' || ORTempBuffer->o==10 or 13 ) markerend=o;					!adjust pointer to end-of-line in increments of whole words
			if(ORTempBuffer->o==10 or 13 || o==(len+WORDSIZE)|| o-markerstart>maxwidth)			!dump the captured line if 1)found carriage return, 2)finished parsing input, or 3) reached max size of screen
			{
				if(o==(len+WORDSIZE)) markerend=o;				!--if we have finished parsing the input, then the character pointed to by o is not necessarily a space or return.  Go ahead and include it in the output
				offset=(width/2)-((markerend-markerstart)/2);	!--GLULX note: These /2 are correct.  They do NOT represent WORDSIZE, but calculate the half way point
				
				font off; 
				if(inFixedWidth==true){ !--if writting to the type of screen that mimics Z5 behavior (also includes GLULX status bar)
					if(hilight>1) style reverse;
					spaces offset;
					if(hilight>0) style reverse;
				}else{ !--we are actually trying to write to a GLULX text window which has no certain font size but can center on its own...
#ifdef TARGET_GLULX;
					if(hilight>0) set_hilight_centered();
					else set_centered();
#endif;				}
				for(i=markerstart:i<markerend:i++) print (char) ORTempBuffer->i;	!print every charater from startmark to just prior to endmark (so as not to print ending space or carriage return)
				if(hilight>0) style roman;
				new_line;													!next line
				markerend=markerstart=markerend+1;							!skip the return or the space for the next run (so that it doesn't get appended to the front of the next lfine)
			}
		}
		font on; !restore the font
	];
#ifdef TARGET_GLULX;
	default style_Center		style_User1; 
	default style_CenterRvrs	style_User2; 

	ORStyleHint	with user_num style_Center
		,	windows wintype_TextBuffer
		,	hints stylehint_Proportional stylehint_Justification
		,	values 0 stylehint_just_Centered
	;
	ORStyleHint	with user_num style_CenterRvrs
		,	windows wintype_TextBuffer
		,	hints stylehint_ReverseColor stylehint_Proportional stylehint_Justification
		,	values 1 0 stylehint_just_Centered
	;
	[set_centered; glk_set_style(style_Center);];
	[set_hilight_centered; glk_set_style(style_CenterRvrs);];
#endif;
!======================================================================================
   #endif; #ifnot; #ifndef ORCenter_DONEGRAMMAR; constant ORCenter_DONEGRAMMAR; #ifdef ORCenter_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================