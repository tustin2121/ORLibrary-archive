!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORBetterChoice [Z, GLULX]
! Give the parser better decision making skills when considering objects.
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
! Give the parser better decision making skills when considering objects.
! For example, objects already carried are given less priority during a "Take" action
! and objects that are edible are given more priority for the "Eat command"
!--------------------------------------------------------------------------------------
! AutoDep:	
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORBetterChoice; #include "ORBetterChoice"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORBetterChoice; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2003.01.02	Initial Creation
! 2003.01.25	Added code to prefer edible objects when eating.
! 2003.03.01	Made objects that are already being held even lower for "take" and 
!				"remove". Seems zero wasn't enough.
! 2003.07.19	Added code to prefer taking objects that actually have a place in the 
!				game world, rather than ones that are brought into scope by the presence
!				of another objects.
! 2003.12.23	Merged code from ORDefer2ndReference.
! 2004.01.11	Modified to use the stdlib 6/11b2 new ext_initialise functionality
! 
!--------------------------------------------------------------------------------------
message "          Processing library extension ORBetterChoice...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORBetterChoice forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORBetterChoice has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORRoutinesList_DONEREPLACE; default USE_ORRoutinesList 0; message "          [ORBetterChoice forcing inclusion of ORRoutinesList]"; #include "ORRoutinesList"; #endif; 
	#ifndef OREntryPoints_DONEREPLACE; default USE_OREntryPoints 0; message "          [ORBetterChoice forcing inclusion of OREntryPoints]"; #include "OREntryPoints"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORBetterChoice_DONEREPLACE; constant ORBetterChoice_DONEREPLACE; #ifdef ORBetterChoice_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORBetterChoice_DONEMESSAGE; constant ORBetterChoice_DONEMESSAGE;  #ifdef ORBetterChoice_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
! #ifndef OREnglish; message fatalerror "ERROR!!!! ORBetterChoice requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORBetterChoice_DONECODE;constant ORBetterChoice_DONECODE; #ifdef ORBetterChoice_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	object _betterchoice LibraryExtensions with ext_initialise[;ORChooseObjects.add_routine(__betterchoice);];
	[__betterchoice obj code val;
		if(code<2) return 0; !--return default for "all" check
		!--parser is looking for a hint...
		for(val=0:val<pcount:val++) if(pattern-->val==obj) return -1; !--let's defer to another object if this one was already referred to in the input (avoids "PUT WATER IN WATER")
		switch(action_to_be){
			##take, ##remove:  if(obj in actor) return -1; !--if we are trying to take something, then lets not consider things that we already are holding
							if(ObjectScopedBySomething(obj)~=0) return -1; !--lets also try not to grab objects that are brought into scope by something else (add_to_scope). 
			##eat: if(object has edible) return 2; !--lets prefer edible things to non-edible things...
		}
		return 1;!--return default for "assistance" check
	];
!======================================================================================
   #endif; #ifnot; #ifndef ORBetterChoice_DONEGRAMMAR; constant ORBetterChoice_DONEGRAMMAR; #ifdef ORBetterChoice_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================