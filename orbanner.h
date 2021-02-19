!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORBanner	[Z, GLULX]
! Extends the standard library's banner routine, adding a userdefined value to append 
! to the bottom of the output and ORLib versioning too.
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
! This is just a simple extension to the standard library's banner routine.  It 
! identifies the version of the ORLibrary that was compiled in and exposes the
! global variable ORBannerText which can be defined in the program to add custom text
! to the banner.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORBanner";					!Constant USE_ORBanner; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORBanner; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.07.03	Initial Creation
! 2002.02.12	repackaged with new library entry template
! 2002.02.16	repackaged with new library entry template (again).
! 2002.04.03	modified faulty WORDSIZE definition.
! 2002.04.19	Added support for ORBannerText variable to be a routine.
! 2004.01.22	Removed references to ORTextFormatting, since glulx now supports the
!				style statements.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORBanner...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORBanner forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORBanner has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 


!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORBanner_DONEREPLACE; constant ORBanner_DONEREPLACE; #ifdef ORBanner_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace Banner;
	default ORLibVersion "undefined";
	default ORLibDateStamp "undefined";
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORBanner_DONEMESSAGE; constant ORBanner_DONEMESSAGE;  #ifdef ORBanner_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
! #ifndef OREnglish; message fatalerror "ERROR!!!! ORBanner requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORBanner_DONECODE;constant ORBanner_DONECODE; #ifdef ORBanner_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	default ORBannerText 0;
	[ Banner i;
		if(Story ~= 0){
			style bold;
			print (string) Story;
			style roman;
		}
		if (Headline ~= 0)
			print (string) Headline;
		#ifndef TARGET_GLULX;
			print "Release ", (0-->1) & $03ff, " / Serial number ";
			for (i=18:i<24:i++) print (char) 0->i;
		#ifnot; ! TARGET_ZCODE;
			print "Release ";
			@aloads 52 0 i;
			print i;
			print " / Serial number ";
			for (i=0:i<6:i++) print (char) 54->i;
		#endif; 
			print " / Inform v"; inversion;
			print " Library ", (string) LibRelease, " ";
		#ifdef STRICT_MODE;
			print "S";
		#endif;
		#ifdef INFIX;
			print "X";
		#ifnot;
			#ifdef DEBUG;
				print "D";
			#endif;
		#endif;
		print "^ORLibrary release ",(string)ORLibVersion," (",(string)ORLibDateStamp,")^";
		if(ORBannerText~=0) {
			switch(metaclass(ORBannerText)){
				string: print (string) ORBannerText;
				routine: ORBannerText();
			}
			
		}
		new_line;
	];
!======================================================================================
   #endif; #ifnot; #ifndef ORBanner_DONEGRAMMAR; constant ORBanner_DONEGRAMMAR; #ifdef ORBanner_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================


!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================