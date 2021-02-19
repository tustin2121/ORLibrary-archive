!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORTransition [Z, GLULX]
! Transition() is best used as a special effect for dividing story parts.  It conveys 
! to the player, a transition between logical breaks in the game (for instance, 
! chapters). It can be called after printing the final lines of a section, and before 
! the first lines of the next. 
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
! The general use of the routine is as follows:
!	1) Text of Section A is printed.
!	2) Transition() prints a small text seperation then prompts for the player to 
!		press space.
!	3) The screen is cleared and the text of Section B is then printed.
! Note:
! Transition can take a single parameter for text to be used in the tranition.
! If no text is supplied the three dashes will be used.  
!--------------------------------------------------------------------------------------
! AutoDep:	ORCenter
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORTransition; #include "ORTransition"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORTransition; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.05.12	Initial Creation
! 2002.05.13	Adjusted closing line feeds.
! 2002.05.18	Addressed bug in GLULX mode.
! 2002.07.27	Added code to optionally skip the erase screen section of code.
! 2004.02.01	Removed reference to REMOVED ORStyleRvrsMonospace module.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORTransition...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORTransition forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORTransition has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORCenter_DONEREPLACE; default USE_ORCenter 0; message "          [ORTransition forcing inclusion of ORCenter]"; #include "ORCenter"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORTransition_DONEREPLACE; constant ORTransition_DONEREPLACE; #ifdef ORTransition_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORTransition_DONEMESSAGE; constant ORTransition_DONEMESSAGE;  #ifdef ORTransition_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORTransition requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORTransition_DONECODE;constant ORTransition_DONECODE; #ifdef ORTransition_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	system_file;
	[Transition text skiperase ch;
		if(text==0) text="---";
		new_line;
		Center(text);
		new_line;
		Center("Press SPACE to continue",0,1);
		while(ch~=' ') {
	#ifdef TARGET_GLULX; 
			ch=false;
			while(ch==false){
				glk_request_char_event(gg_mainwin);
				glk_select(gg_event); ! select
				if(gg_event-->0==evtype_CharInput) ch=gg_event-->2;
				HandleGlkEvent(gg_event, 1);
			}
	#ifnot;
			@read_char 1 -> ch;
	#endif;
		}
		if(skiperase==false){
		#ifndef TARGET_GLULX; 
			@erase_window -1; 
			@set_cursor 1 1; 
		#ifnot; 
			glk_window_clear(gg_mainwin); 
			glk_window_move_cursor(gg_mainwin,0,0); 
		#endif;
		}
		new_line;
	];
!======================================================================================
#endif; #ifnot; #ifndef ORTransition_DONEGRAMMAR; constant ORTransition_DONEGRAMMAR; #ifdef ORTransition_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================