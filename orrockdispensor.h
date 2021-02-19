!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORRockDispensor	[GLULX]
! An object to generate GLULX rocks dynamically, so modules do not confict.
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
! In GLULX, defining a rock is as simple as choosing an arbitrary number within a 
! specified range.  (i.e.: window rocks: 210 or above, stream rocks: 310 or above, 
! etc.) When creating modules that utilize rocks, this practice becomes problematic, 
! since there is no guarantee that two modules won�t select the same rock value for two 
! different uses.  ORRockDispensor is a simple object that modules can utilize to 
! obtain a rock value.  The routines in this object never return the same value twice 
! and honor the ranges for the specific type of rock requested.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORRockDispensor";					!Constant USE_ORRockDispensor; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORRockDispensor; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.20	Initial Creation
! 2002.02.26	Fit to new template.
! 2002.02.28	Fixed bug with inclusion for GLULX code.
!--------------------------------------------------------------------------------------
	#ifndef TARGET_GLULX;
		message "          Processing library extension ORRockDispensor...ignoring (GLULX only)"; 
	#ifnot;
		message "          Processing library extension ORRockDispensor...";
	#endif;
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORRockDispensor forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORRockDispensor has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORRockDispensor_DONEREPLACE; constant ORRockDispensor_DONEREPLACE; #ifdef ORRockDispensor_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORRockDispensor_DONEMESSAGE; constant ORRockDispensor_DONEMESSAGE;  #ifdef ORRockDispensor_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORRockDispensor requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORRockDispensor_DONECODE;constant ORRockDispensor_DONECODE; #ifdef ORRockDispensor_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	#ifdef TARGET_GLULX;
		object ORRockDispensor 
			with	rock_win	299 !--Count rock requests down. Less likly to conflict with user defined rocks
			,		rock_stream	399
			,		rock_file	499
			,		get_window[; return self.rock_win--;]
			,		get_stream[; return self.rock_stream--;]
			,		get_file[; return self.rock_file--;]
		;
	#endif;
!======================================================================================
#endif; #ifnot; #ifndef ORRockDispensor_DONEGRAMMAR; constant ORRockDispensor_DONEGRAMMAR; #ifdef ORRockDispensor_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================