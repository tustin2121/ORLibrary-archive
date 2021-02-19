!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORExamWithContents	[Z, GLULX]
! Adds the functionality to the EXAMINE verb to describe the contents of a container.
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
! A more detailed description of this file, what it does, and how to use it goes here.
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORExamWithContents";					!Constant USE_ORExamWithContents; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORExamWithContents; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.10.06	Initial Creation
! 2002.02.26	Added to new template
! 2002.12.22	Added the ability to exclude this functionality for specific 
!				objects by giving them the property "SuppressAutoContents" with the 
!				value of "true". 
! 2003.01.02	Eliminated need to specify the SuppressAutoContents; Also added code to 
!				supportthe ORSupporterContainer module if present.
! 2003.02.02	Added code to rework linefeeds, based on a posting by Andrew Plotkin.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORExamWithContents...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORExamWithContents forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORExamWithContents has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORExamWithContents_DONEREPLACE; constant ORExamWithContents_DONEREPLACE; #ifdef ORExamWithContents_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace ExamineSub;
!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORExamWithContents_DONEMESSAGE; constant ORExamWithContents_DONEMESSAGE;  #ifdef ORExamWithContents_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORExamWithContents requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORExamWithContents_DONECODE;constant ORExamWithContents_DONECODE; #ifdef ORExamWithContents_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	#ifndef SuppressAutoContents;
		object with SuppressAutoContents;
	#endif;
		[ ExamineSub i;
			if (location==thedark) return L__M(##Examine,1);
			i=noun.description;
			if (i==0){   
				if (noun has container) <<Search noun>>;
				if (noun has switchable) { L__M(##Examine,3,noun); rfalse; }
				return L__M(##Examine,2,noun);
			}
			PrintOrRun(noun, description);
			if (noun has switchable) L__M(##Examine,3,noun);
		  
			if((noun has supporter) || (noun has container && noun has open or transparent)) { !--if noun can contain stuff that could be visible
				if(~~(noun provides SuppressAutoContents) || (noun provides SuppressAutoContents && ValueOrRun(noun,SuppressAutoContents)==false)) { !--give the ability to suppress the contained description
	#ifdef USE_ORSupporterContainer;
				if(SetContainedChildrenWorkFlag(noun,-1)>0) print "^";
				DescribeContents(noun);
	#ifnot;
				Locale(noun);
	#endif;
				}
			}
			if (AfterRoutines()==1) rtrue;
		];
!======================================================================================
   #endif; #ifnot; #ifndef ORExamWithContents_DONEGRAMMAR; constant ORExamWithContents_DONEGRAMMAR; #ifdef ORExamWithContents_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================