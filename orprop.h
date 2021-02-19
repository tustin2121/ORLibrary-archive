!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORProp [Z, GLULX]
!	This class eases implementation of a generic object which does not need to be 
!	referenced in the game.  
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
! AutoDep :	none
!--------------------------------------------------------------------------------------
! Sample use: 
!
!			ORProp -> "butterflies" has pluralname with name 'butterflies';
!
! Used with the OREnglish LDF, this method has the advantage (over specifying words
! in the room's name property) of specifing pluralname and generating a default 
! message similar to "those don't need to be referenced in this game" rather than
! "that doesn't need to be ..."
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORProp";					!Constant USE_ORProp; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORProp; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.12.01	Initial Creation (Not really, but this is the earliest date I can find 
!				without searching my code archive)
! 2002.02.17	Tested for GLULX support
! 2002.02.18	Modified to not use the standard message (39). Added dependancy to the 
!				ORLibraryMessages module.
! 2002.02.21	Corrected typo in the new message created above 2002.02.18
! 2002.02.26	Fit to new template.
! 2002.05.19	Minor adjustment to "not significant" message.
! 2002.05.22	Added code to the "before" routine to allow props to be containers. (I 
!				know, they're not really props then, but it really can only help)
! 2002.05.24	Suppressed a warning.
! 2002.05.31	Allow props to be examined. Also reconsidered allowing props to be 
!				containers (2002.5.22).  That allows for problems when accessing 
!				other containers within the prop.
! 2003.01.11	Restored default item in before property of ORProp object. (Don't know
!				why it wasn't there.)
! 2003.02.01	Added a new property "message" to the ORProp object giving the 
!				developer the ability to override the default message on an object by 
!				object basis.
! 2003.02.15	Slowly but surely conquering my whole linefeed problem.
! 2004.01.14	Modified to match 6.3 compiler and library.
! 2004.01.18	Increased misc message number from 54 to 99.  Since new library defines 54 & 55.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORProp...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORProp forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORProp has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORProp_DONEREPLACE; constant ORProp_DONEREPLACE; #ifdef ORProp_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORProp_DONEMESSAGE; constant ORProp_DONEMESSAGE;  #ifdef ORProp_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORProp requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORProp_DONECODE;constant ORProp_DONECODE; #ifdef ORProp_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	system_file;
	Class ORProp	
		has		scenery static
		with	before[;
						Examine: if(self provides description) rfalse;
						default:
							if(self.message~=0) 
								printorrun(self,message);
							else
								L__M(##Miscellany, 99,self); rtrue;]	
		,	message 0
		,	chooseobject[;return 0;] !--Take advantage of OREntryPoints, if included.  This will make the parser prefer other objects with the same name...
	;
	object _op_init LibraryExtensions
		with ext_messages[; 
			Miscellany: if(lm_n==99){
							print (The)lm_o," ",(IsOrAre)lm_o," not significant. (It is not necessary to refer to ",(ig)ItOrThem(lm_o)," in the course of this game.)^";
							rtrue;
						}
		]
	;
!======================================================================================
#endif; #ifnot; #ifndef ORProp_DONEGRAMMAR; constant ORProp_DONEGRAMMAR; #ifdef ORProp_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================