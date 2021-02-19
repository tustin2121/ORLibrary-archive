!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.28 ORKeyboardPrimitive	[Z, GLULX]
! A modified keyboard primitive that allows other modules to specify routines to replace
! the normal keyboard input routines.  
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- - 
! License: 
! Matches that of Inform's standard library (see included header, below).
!--------------------------------------------------------------------------------------
! This code is a slightly modified version of the same KeyboardPrimitive
! routine that comes from Graham Nelson's standard library (actually Andrew Plotkin's
! Biplatform library which is a modified version of Graham Nelson's standard library).  
! It falls under the same copyright as the code upon which it was based 
! which can be found in the standard library's parserm file.  The header to this file 
! is included now:
!--------------------------------------------------------------------------------------
	!  PARSERM:  Core of parser.
	!
	!  Supplied for use with Inform 6                         Serial number 991113
	!                                                                 Release 6/10
	!  (c) Graham Nelson 1993, 1994, 1995, 1996, 1997, 1998, 1999
	!      but freely usable (see manuals)
!--------------------------------------------------------------------------------------
! To add this module to your library, add the line:
!
!		#include "ORKeyboardPrimitive";					!Constant USE_ORKeyboardPrimitive; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORKeyboardPrimitive; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.10	Initial Creation
! 2002.02.26	Fit to new framework
! 2002.02.28	Modified for GLULX compiling.
! 2002.04.03	Added ability to usurp the normal keyboard input routines. This ability
!				is available for both GLULX and Z-Code.
! 2004.01.30	Matched to new standard library 6/11.  Replaced "instead" terminology 
!				to "preempt" to better communicate purpose of objects for future support.
!				Also removed ORKBTimer references and ORKBFilter references because 
!
!					(1)They were unused by the library; their original intent 
!						never fully realized. 
!					(2)They were not supported in Glulx and compatibility between 
!						the two modes is desired. 
!					(3)They were not documented, as so it is very unlikely that 
!						anyone has taken advantage of these.
!					(4)Removing them makes keeping the ORLibrary and Standard 
!						Library in sync less painful.
!					(5)They created flashing input issues on WinFrotz (although not 
!						WindowsFrotz or Nitfol).
!
!				Finally, I also added missing autodependancy code for orroutinelist.
! 2004.02.11	Removed a reference to an unused autodependancy.
! 2004.02.28	Modification to return the result of preempting code.
!--------------------------------------------------------------------------------------
		message "          Processing library extension ORKeyboardPrimitive...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORKeyboardPrimitive forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORKeyboardPrimitive has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORRoutinesList_DONEREPLACE; default USE_ORRoutinesList 0; message "          [ORKeyboardPrimitive forcing inclusion of ORRoutinesList]"; #include "ORRoutinesList"; #endif; 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORKeyboardPrimitive_DONEREPLACE; constant ORKeyboardPrimitive_DONEREPLACE; #ifdef ORKeyboardPrimitive_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace KeyboardPrimitive;
!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORKeyboardPrimitive_DONEMESSAGE; constant ORKeyboardPrimitive_DONEMESSAGE;  #ifdef ORKeyboardPrimitive_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORKeyboardPrimitive requires the OREnglish file.";#endif; 


!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORKeyboardPrimitive_DONECODE;constant ORKeyboardPrimitive_DONECODE; #ifdef ORKeyboardPrimitive_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
		ORRoutinesList ORKBPreempt with routines 0 0 0 0 0 0;
		[__preemptfunc a_buffer a_table;
			return ORKBPreempt.run_until_not(false,a_buffer, a_table);
		];

#ifdef TARGET_ZCODE;
		[KeyboardPrimitive a_buffer a_table retval;
			a_buffer->1=0;
			a_table->1=0;
			retval=__preemptfunc(a_buffer, a_table);
			if(retval~=false) return retval;
			read a_buffer a_table;
			#Iftrue (#version_number == 6);
				@output_stream -1;
				@loadb a_buffer 1 -> sp;
				@add a_buffer 2 -> sp;
				@print_table sp sp;
				new_line;
				@output_stream 1;
			#Endif;		
		];
#ifnot; !--glulx
	[ KeyboardPrimitive  a_buffer a_table done ix retval;
			a_buffer->1=0;
			a_table->1=0;
			retval=__preemptfunc(a_buffer, a_table);
			if(retval~=false) return retval;
			if (gg_commandstr ~= 0 && gg_command_reading ~= false) {
			! get_line_stream
			done = glk($0091, gg_commandstr, a_buffer+WORDSIZE,
			(INPUT_BUFFER_LEN-WORDSIZE)-1);
			if (done == 0) {
			glk($0044, gg_commandstr, 0); ! stream_close
			gg_commandstr = 0;
			gg_command_reading = false;
            ! L__M(##CommandsRead, 5); would come after prompt
            ! fall through to normal user input.
			}
			else {
			! Trim the trailing newline
			if ((a_buffer+WORDSIZE)->(done-1) == 10) done = done-1;
			a_buffer-->0 = done;
			glk($0086, 8); ! set input style
			glk($0084, a_buffer+WORDSIZE, done); ! put_buffer
			glk($0086, 0); ! set normal style
			print "^";
			jump KPContinue;
			}
		}
		done = false;
		glk($00D0, gg_mainwin, a_buffer+WORDSIZE, INPUT_BUFFER_LEN-WORDSIZE, 0); ! request_line_event
		while (~~done) {
			glk($00C0, gg_event); ! select
			switch (gg_event-->0) {
			5: ! evtype_Arrange
				DrawStatusLine();
			3: ! evtype_LineInput
				if (gg_event-->1 == gg_mainwin) {
				a_buffer-->0 = gg_event-->2;
				done = true;
				}
			}
			ix = HandleGlkEvent(gg_event, 0, a_buffer);
			if (ix == 2) done = true;
			else if (ix == -1) done = false;
		}
		if (gg_commandstr ~= 0 && gg_command_reading == false) {
			! put_buffer_stream
			glk($0085, gg_commandstr, a_buffer+WORDSIZE, a_buffer-->0); 
			glk($0081, gg_commandstr, 10); ! put_char_stream (newline)
		}
		.KPContinue;
		Tokenise__(a_buffer,a_table);
		! It's time to close any quote window we've got going.
		if (gg_quotewin) {
			glk($0024, gg_quotewin, 0); ! close_window
			gg_quotewin = 0;
		}
		];

	#endif;
!======================================================================================
   #endif; #ifnot; #ifndef ORKeyboardPrimitive_DONEGRAMMAR; constant ORKeyboardPrimitive_DONEGRAMMAR; #ifdef ORKeyboardPrimitive_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================