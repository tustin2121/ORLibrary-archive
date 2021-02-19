!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORRedefineObject	[Z, GLULX]
! Redefine an object's routines at runtime.
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
! AutoDep :	None
!--------------------------------------------------------------------------------------
! ORLibDev: Note, that this module is not useful as a library extension alone.  By 
!     itself it provides no new functionality for the IF developer. Instead it provides
!     a basis for other module to be built upon and will primarily be useful to those 
!     who are developing ORLibrary entrys of their own.
!--------------------------------------------------------------------------------------
! The DM4 doth proclaim (section 25):
!
! "...to edit and change the library files themselves is an inconvenience and an 
! inelegant way to carry on, because it would lead to needing a separate copy of 
! all the files for each project you work on."
!
! This is, after all, what library extensions often do.  They alter the standard 
! library's behavior.  To accomplish this, library extensions have traditionally 
! leveraged Inform's REPLACE directive to replace standard library routines with 
! altered ones.  This works reasonably well with stand alone routines, but there is a 
! great deal of code in the standard library that is NOT contained in stand alone 
! routines and so cannot be replaced.  Instead, this code is contained in objects. The 
! InformLibrary object is particularly noteworthy for this, and historically it has 
! not been an easy task to write a library extension which modifies the code contained 
! in the InformLibrary object.  Instead, modifications were often written down as 
! instructions for modifications that had to be made to a standard library file, like 
! "First change line 638 of the parserm file from THIS to read as THAT. Now change 
! line..." 
!
! ORRedefineObject eliminates this need.  Simply define a replacement routine on 
! an instance of the ORRedefineObject class and specify the object to change in the 
! "dest_object" property.  Before Initialise() is called, the ORRedefineObject will 
! replace the same-named properties from the destination object with its own. 
!
! It should be noted that unlike the REPLACE directive, this process tends to 
! bloat code since the object routines are redefined at runtime and the previous 
! definitions continue to exist in memory.  However to avoid this, the standard 
! library files must be altered directly.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORRedefineObject";					!Constant USE_ORRedefineObject; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORRedefineObject; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.06	Initial Creation
! 2002.02.13	Added GLULX Support
! 2002.02.26	Fit to new template
! 2004.01.11	Reworked to fit with stdlib 6.11
!--------------------------------------------------------------------------------------
message "          Processing library extension ORRedefineObject...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORRedefineObject forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORRedefineObject has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORRedefineObject_DONEREPLACE; constant ORRedefineObject_DONEREPLACE; #ifdef ORRedefineObject_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORRedefineObject_DONEMESSAGE; constant ORRedefineObject_DONEMESSAGE;  #ifdef ORRedefineObject_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORRedefineObject requires the OREnglish file.";#endif; 
	class ORRedefineObject
		with dest_object 0 
		,	after_init 0
	;
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORRedefineObject_DONECODE;constant ORRedefineObject_DONECODE; #ifdef ORRedefineObject_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	object _rdfinit LibraryExtensions 
		with ext_initialise[o i count;
#ifndef TARGET_GLULX;
			count = #identifiers_table-->0;
#ifnot; 
			count = INDIV_PROP_START + #identifiers_table-->3;
#endif; 
			objectloop(o ofclass ORRedefineObject){
				for (i=4:i<=count:i++){   !--ignore basic properties (like name and class list)
					if (o provides i && o.dest_object provides i){   
						(o.dest_object).i=o.i;
					}
				}
				if(o.after_init~=0) o.after_init();
			}
		]
	;
!======================================================================================
#endif; #ifnot; #ifndef ORRedefineObject_DONEGRAMMAR; constant ORRedefineObject_DONEGRAMMAR; #ifdef ORRedefineObject_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================