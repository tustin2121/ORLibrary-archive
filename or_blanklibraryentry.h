!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 OR_BlankLibraryEntry
! This file is a template to be used when creating extensions that conform to the 
! ORModule specification.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!--------------------------------------------------------------------------------------
! Template created by Jim Fisher
! Ten points for flawless use of this template: 
!	1)	Determine a unique constant that will be used to identify this extension
!	2)	Copy this file and name it that same as that constant
!	3)	Run a "search and replace" replacing the text "<REPLACEWITHNAME>" with that 
!		constant.  
!	4)	Review and select the sample Licence to ensure this is the license you want
!		to use.  Modify it if it is not.
!	5)	Add a description of the purpose of this extension and samples of how to use.
!   6)	Make sure that the target platform is correct ([Z, Glulx]) or change this 
!		indicator if it is not.
!	7)	Add implementation code to the appropriate sections
!	8)	If the module has prerequisites, add the appropriate code to the dependancies 
!		section. 
!	9)	Register this module with your library as described in the header below.
!	10)	Delete everything above the following "DELETE FROM HERE UP".
!
!--DELETE FROM HERE UP-----------------------------------------------------------------
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.00.00 <REPLACEWITHNAME> [Z, GLULX]
! Short description of this module.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!--------------------------------------------------------------------------------------
! Created by <AUTHOR NAME>
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
! A more detailed description of this file, what it does, and how to use it goes here.
!--------------------------------------------------------------------------------------
! AutoDep:	
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_<REPLACEWITHNAME>; #include "<REPLACEWITHNAME>"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_<REPLACEWITHNAME>; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2004.00.00	Initial Creation
!--------------------------------------------------------------------------------------
message "          Processing library extension <REPLACEWITHNAME>...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [<REPLACEWITHNAME> forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "<REPLACEWITHNAME> has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef <REPLACEWITHNAME>_DONEREPLACE; constant <REPLACEWITHNAME>_DONEREPLACE; #ifdef <REPLACEWITHNAME>_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef <REPLACEWITHNAME>_DONEMESSAGE; constant <REPLACEWITHNAME>_DONEMESSAGE;  #ifdef <REPLACEWITHNAME>_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! <REPLACEWITHNAME> requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef <REPLACEWITHNAME>_DONECODE;constant <REPLACEWITHNAME>_DONECODE; #ifdef <REPLACEWITHNAME>_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #ifnot; #ifndef <REPLACEWITHNAME>_DONEGRAMMAR; constant <REPLACEWITHNAME>_DONEGRAMMAR; #ifdef <REPLACEWITHNAME>_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================