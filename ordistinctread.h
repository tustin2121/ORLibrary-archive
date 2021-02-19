!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORDistinctRead [Z, GLULX]
! Generally, the commands Read and Examine are treated the same and are mapped to the 
! same verb which simply displays the description.  This routine adds a behavior to 
! makes a distinction between reading and just looking at an object. 
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
! ORDistinctRead relies upon a property called "read_value".  If present, 
! then the value of this property will be displayed in response to the READ command.  
! If not, then a message is printed explaining that the object "cannot be read, only 
! examined" and the normal description is printed.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORDistinctRead";					!Constant USE_ORDistinctRead; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORDistinctRead; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.01.03	Initial Creation (not really, but this is the earliest date I could find
!				without searching my code archive.)
! 2002.02.26	Fit to new template.
! 2002.04.03	modified faulty WORDSIZE definition.
! 2002.07.07	Added menu documentation.
! 2002.07.30	made a bogus object to declare the readvalue property rather than
!				making it a common property.
! 2002.10.19	Added autodep for ObjectInitialise 
! 2003.02.19	Added better ifdef for self docs
! 2003.03.02	Made the new grammar for read replace the previous grammar.  The 
!				topic token matches everything including things that should be 
!				rejected by the parser.
! 2004.01.11	Modified to use the stdlib 6/11b2 new ext_initialise functionality
!--------------------------------------------------------------------------------------
message "          Processing library extension ORDistinctRead...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORDistinctRead forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORDistinctRead has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORDistinctRead_DONEREPLACE; constant ORDistinctRead_DONEREPLACE; #ifdef ORDistinctRead_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORDistinctRead_DONEMESSAGE; constant ORDistinctRead_DONEMESSAGE;  #ifdef ORDistinctRead_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORDistinctRead requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORDistinctRead_DONECODE;constant ORDistinctRead_DONECODE; #ifdef ORDistinctRead_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
   #endif; #ifnot; #ifndef ORDistinctRead_DONEGRAMMAR; constant ORDistinctRead_DONEGRAMMAR; #ifdef ORDistinctRead_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	object with read_value;
	Extend "read" replace 
		* noun		->Read
	;
	[ReadSub;
		if(noun provides read_value) printorrun(noun,read_value);
		else {
			print "(",(The)noun," could not be ",(strong)"read",", just ",(strong)"examined","...)^";
			ExamineSub();	
		}
	];
	#ifdef ORLibDoc;
		!--Commands, document thyself.
		ORMenu ORHelpRead "Read" ORLibDoc with text "For the sake of completeness, it is worth noting that the commands ~READ~ and ~EXAMINE~ are treated separately and may be used with differing results.";
	#endif;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================