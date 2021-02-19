!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.10 ORBracketParserMsgs [Z, GLULX]
! Add brackets, or other wrapping characters to parser generated messaages.
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
! ORBracketParserMsgs depends upon the OREnglish LDF, which was coded to leverage this
! entry if present. Simply including this module will wrap all parser generated 
! messages in [brackets].  If, for some reason, the developer would like to turn off 
! bracketing in the middle of a game, the global variable BracketedParserMsgs can be 
! set to false to accomplish this.  Additionally, the global variables BracketOpen, and
! BracketClose have been provided to set what text the messages will be wrapped in.
! For example, if the developer would prefer to wrap the parser messages like HTML tags
! and tab them over, then the following lines can be defined before the inclusion of 
! grammar:
!
!	Constant BracketOpen "@@9<"; !--TAB is @@9
!	Constant BracketClose ">";
!
! Additionally, these can be defined as routines.  The following will emphasize parser 
! messages:
!
!	[BracketOpen; style underline; print"[";];
!	[BracketClose; print"]"; style roman; ];
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORBracketParserMsgs";					!Constant USE_ORBracketParserMsgs; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORBracketParserMsgs; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.12	Initial Creation (not really, but this is the earlist date I could find 
!				without searching my code archive)
! 2002.02.26	Fit to new template.
! 2002.03.11	Added code to prompt warning message if OREnglish is not used.
! 2002.04.03	modified faulty WORDSIZE definition.
! 2004.01.03	removed WORDSIZE and made minor change to the full score sub to better reflect 
!				state of new 6.11 library
! 2004.02.07	In addition to the new extension template, added support for 
!				BracketOpen and BracketClose to be routines.
! 2004.02.08	Fixed a missed a BracketClose reference.
! 2004.02.10	Removed reference to _DoEndBracket variable which is no longer needed thanks
!				to new ending library messages in the 6/11 library.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORBracketParserMsgs...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORBracketParserMsgs forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORBracketParserMsgs has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORBracketParserMsgs_DONEREPLACE; constant ORBracketParserMsgs_DONEREPLACE; #ifdef ORBracketParserMsgs_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace LMode1Sub;
	replace LMode2Sub;
	replace LMode3Sub;
	replace scoresub;
	Replace NotifyTheScore;	

	Global BracketedParserMsgs=true;
	[bkt_o; if(BracketedParserMsgs==true) PrintOrRunVar(BracketOpen,true);];
	[bkt_c; if(BracketedParserMsgs==true) PrintOrRunVar(BracketClose,true);];
	[bkt txt;
		bkt_o();
		print (string)txt;
		bkt_c();
	];

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORBracketParserMsgs_DONEMESSAGE; constant ORBracketParserMsgs_DONEMESSAGE;  #ifdef ORBracketParserMsgs_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
	#ifndef OREnglish; message fatalerror "ERROR!!!! ORBracketParserMsgs requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORBracketParserMsgs_DONECODE;constant ORBracketParserMsgs_DONECODE; #ifdef ORBracketParserMsgs_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

	#ifndef PrintRank;
		[ PrintRank; print "."; ];
	#endif;

	[ LMode1Sub; lookmode=1; bkt_o();print (string) Story; L__M(##LMode1); bkt_c(); print "^";];  ! Brief
	[ LMode2Sub; lookmode=2; bkt_o();print (string) Story; L__M(##LMode2); bkt_c(); print "^";];  ! Verbose
	[ LMode3Sub; lookmode=3; bkt_o();print (string) Story; L__M(##LMode3); bkt_c(); print "^";];  ! Superbrief

[ ScoreSub;
    #Ifdef NO_SCORE;
    if (deadflag == 0) L__M(##Score, 2);
    #Ifnot;
    bkt_o();
	L__M(##Score, 1);
    PrintRank();
	bkt_c();
	new_line;
    #Endif; ! NO_SCORE
];
	[ NotifyTheScore;
		new_line;
		bkt_o();
		L__M(##Miscellany, 50, score-last_score);  print ".";
		bkt_c();
		new_line;
	];
!======================================================================================
   #endif; #ifnot; #ifndef ORBracketParserMsgs_DONEGRAMMAR; constant ORBracketParserMsgs_DONEGRAMMAR; #ifdef ORBracketParserMsgs_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	default BracketOpen="[";
	default BracketClose="]";

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================