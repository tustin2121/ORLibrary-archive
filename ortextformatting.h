!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORTextFormatting [Z, GLULX]
! Simple file containing various useful print rules.
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
! AutoDep  :	none
!--------------------------------------------------------------------------------------
! Print Rules defined in this module:
! italics		- print in emphasised
! strong		- print in bold font
! highlight		- print in reverse font (header)
! ig			- ignore, print absolutely nothing
! arraystring	- prints out the contents of a character array 
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORTextFormatting";					!Constant USE_ORTextFormatting; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORTextFormatting; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.08.13	Initial Creation (not really, but this was the earliest date found 
!				without searching my code archives)
! 2002.02.10	Added GLULX support (WORDSIZE)
! 2002.02.12	Modified GLULX support (text formating)
! 2002.02.13	Modified GLULX support (arraystring print rule) Also modified code 
!				template
! 2002.02.20	Checked for declaration of ORStyleRvrsMonospace module, rather than the 
!				style_rvrsmonospace value (since both module implement code in the replace 
!				section)
! 2002.02.26	Fit to new template.
! 2004.02.01	Removed unused autodependencies, previously added to avoid nested 
!				include errors
!--------------------------------------------------------------------------------------
message "          Processing library extension ORTextFormatting...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORTextFormatting forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORTextFormatting has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORTextFormatting_DONEREPLACE; constant ORTextFormatting_DONEREPLACE; #ifdef ORTextFormatting_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	!put in the replace section in order to be earlier than anyone else
	system_file;
	[ig o; o=null;]; 
	[italics o;	
		style underline;
		print (string) o;
		style roman;
	];
	[strong o; 
		style bold;
		print (string) o;
		style roman;
	];
	[highlight o; 
		style reverse;
		print (string) o;
		style roman;
	];
	[arraystring o i;	
		for(i=0:i<(o-->0):i++) 
			print (char) o->(i+WORDSIZE); 
	]; 
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORTextFormatting_DONEMESSAGE; constant ORTextFormatting_DONEMESSAGE;  #ifdef ORTextFormatting_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORTextFormatting requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORTextFormatting_DONECODE;constant ORTextFormatting_DONECODE; #ifdef ORTextFormatting_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #ifnot; #ifndef ORTextFormatting_DONEGRAMMAR; constant ORTextFormatting_DONEGRAMMAR; #ifdef ORTextFormatting_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================