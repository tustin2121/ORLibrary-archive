!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORSpecializedExit [Z, GLULX]
! Extends Exit/Take to support a few common exit commands.
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
! Modified behavior for exit.  Something that has 
! always bothered me.  With the below modifications, 
! "Exit balloon" or "get off table" or "get down from 
! the cabinet" or many other variations will work.
!--------------------------------------------------------------------------------------
! AutoDep:	
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORSpecializedExit; #include "ORSpecializedExit"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORSpecializedExit; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.07.23	Initial Creation
! 2003.02.02	Modified grammar order, which trumped functionality of standard 
!				library's version of Exit.
! 2004.01.11	Modified to leverage new ext_messages functionality of the stdlib 6/11
!--------------------------------------------------------------------------------------
message "          Processing library extension ORSpecializedExit...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORSpecializedExit forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORSpecializedExit has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 


!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORSpecializedExit_DONEREPLACE; constant ORSpecializedExit_DONEREPLACE; #ifdef ORSpecializedExit_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORSpecializedExit_DONEMESSAGE; constant ORSpecializedExit_DONEMESSAGE;  #ifdef ORSpecializedExit_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORSpecializedExit requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORSpecializedExit_DONECODE;constant ORSpecializedExit_DONECODE; #ifdef ORSpecializedExit_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #ifnot; #ifndef ORSpecializedExit_DONEGRAMMAR; constant ORSpecializedExit_DONEGRAMMAR; #ifdef ORSpecializedExit_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
		Extend 'get' last
			* 'down'/'off'/'out' -> Exit
			* 'off' noun -> ExitNoun
			* 'off'/'out' 'of' noun -> ExitNoun
			* 'down' 'from' noun -> ExitNoun
		;
		Extend 'exit' last
			*	noun -> ExitNoun
		;
		object _se_init LibraryExtensions
		with ext_messages[;ExitNoun: 
			print (ig)CICant(actor)," exit ",(the)noun," since ",(IAm)actor," not standing ";
			if(noun has container) print "in"; else print "on";
			print_ret  " it.";
		];
		[ExitNounSub;
			if(noun~=parent(actor)) return L__M(action, 1);
			<<Exit>>;
		];

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================