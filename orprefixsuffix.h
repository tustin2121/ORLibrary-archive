!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORPrefixSuffix [Z, GLULX]
! Allows the use of prefixes and suffixes that are followed by periods (for example,
! "Dr. Bob" or "Col. Mustard" or "Dan Jr." or "Mr. Armstrong")
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
! When creating an object with a period in the name -- like "Mrs. Robinson" --  the 
! player often has issues.  For instance:
!
!     >examine Mrs. Robinson
!
! will cause the parser to stop parseing the input at the period and treat it as two 
! seperate commands.  "examine mrs" and "Robinson."  Thus, assuming there is only one
! object which the word "Mrs" can refer to, then you will get the description followed
! by the the "That's not a verb I recognize" error message. The parser fails entirely
! if the prefix can refer to multiple objects ("Mrs. Robinson" standing next to 
! "Mrs. Baker").
!
! This module scans the input and removes periods that follow general prefixes, such as 
! "Mr" "Mrs" "Dr" "Col", etc...
!--------------------------------------------------------------------------------------
! AutoDep:	
!	ORRoutinesList
!	OREntryPoints
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORPrefixSuffix; #include "ORPrefixSuffix"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORPrefixSuffix; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.10.13	Initial Creation
! 2004.01.11	Made modifications to fit new stdlib 6.11 functionality.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORPrefixSuffix...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORPrefixSuffix forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORPrefixSuffix has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORRoutinesList_DONEREPLACE; default USE_ORRoutinesList 0; message "          [ORPrefixSuffix forcing inclusion of ORRoutinesList]"; #include "ORRoutinesList"; #endif; 
	#ifndef OREntryPoints_DONEREPLACE; default USE_OREntryPoints 0; message "          [ORPrefixSuffix forcing inclusion of OREntryPoints]"; #include "OREntryPoints"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORPrefixSuffix_DONEREPLACE; constant ORPrefixSuffix_DONEREPLACE; #ifdef ORPrefixSuffix_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORPrefixSuffix_DONEMESSAGE; constant ORPrefixSuffix_DONEMESSAGE;  #ifdef ORPrefixSuffix_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORPrefixSuffix requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORPrefixSuffix_DONECODE;constant ORPrefixSuffix_DONECODE; #ifdef ORPrefixSuffix_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	object _psinit LibraryExtensions with ext_initialise[; ORBeforeParsing.add_routine(self.bp);]
	,	bp[pos lstwrd curwrd; 
		wn=1;
		while(wn<=num_words){ !--search for a period
			curwrd = NextWord();
			pos=WordAddress(wn-1);
			if (curwrd == THEN1__WD && lstwrd=='lt' or 'bro' or 'capt' or 'cmdr' or 'col' or 'dr' or 'mrs' or 'mr' or 'ms' or 'rev' or 'sgt' or 'jr' or 'sr') {
				0->pos=' '; !--turn the period into a space
			}
			lstwrd=curwrd;
		}
		Tokenise__(buffer, parse);		
		rfalse; !-- Normally BeforeParsing return values are ignored, but plugging this into the ORRoutineList object to simulate Additive routines looks at the return value to decide whether or not to continue calling other registered BeforeParsing routines. (Like additive properties, true means we've done something so discontinue) We should probably always return false here, so that other registered handlers can have a go at the input as well
	];
!======================================================================================
#endif; #ifnot; #ifndef ORPrefixSuffix_DONEGRAMMAR; constant ORPrefixSuffix_DONEGRAMMAR; #ifdef ORPrefixSuffix_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================