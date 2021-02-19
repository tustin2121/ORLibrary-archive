!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORExits [Z, GLULX]
! Implements the "Exits" verb functionality.
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
! Implements the "Exits" verb as well as a standalone routine for which does the 
! same thing DescribeExits().
!--------------------------------------------------------------------------------------
! AutoDep:	None.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORExits; #include "ORExits"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORExits; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2003.02.01	Initial Creation
! 2003.02.16	Added new property "exits_text" to override the calculated response.
! 2003.02.19	Added better ifdef for self docs
! 2003.03.14	Modifications allowing containers (parent objects) to define the 
!				exits_text property.
! 2003.08.03	Added a line feed following the exits text.
! 2004.01.11	Modified to use the stdlib 6/11b2 new ext_initialise functionality
! 2004.01.19	Modified code to take advantage of the new SINGULAR_OBJ and PLURAL_OBJ constants
!				defined for the vrb() routine.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORExits...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORExits forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORExits has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORExits_DONEREPLACE; constant ORExits_DONEREPLACE; #ifdef ORExits_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORExits_DONEMESSAGE; constant ORExits_DONEMESSAGE;  #ifdef ORExits_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORExits requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORExits_DONECODE;constant ORExits_DONECODE; #ifdef ORExits_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	[DescribeExits qualifier ignoreexits_text count d save plurality;
		if(parent(actor) provides exits_text && parent(actor).exits_text~=0 && ignoreexits_text==false){
			printorrun(parent(actor),exits_text,true); !--true, suppress new_line
			return;
		}
		count=0;
		objectloop(d in compass) if(metaclass(location.(d.door_dir))==object) count++;
		if(count==0) {
			print "There",(string)ARE__TX," no obvious exits from ",(ig)ppf("t","",""),"here.";
			return;
		}
		if(count>1) plurality=PLURAL_OBJ; else plurality=SINGULAR_OBJ;
		print "The only exit";
		if(plurality~=SINGULAR_OBJ) print"s";
		print " ";
		if(qualifier~=0) print (string)qualifier;
		print(ig)vrb(plurality,"lay","lie");
		save=count;
		objectloop(d in compass) if(metaclass(location.(d.door_dir))==object) {
			if(count<save && save>2) print ",";
			if(count==1 && plurality~=SINGULAR_OBJ) print " and";
			print" ",(LanguageDirection)(d.door_dir);
			count--;					
		}
		print ".^";
	];
!======================================================================================
   #endif; #ifnot; #ifndef ORExits_DONEGRAMMAR; constant ORExits_DONEGRAMMAR; #ifdef ORExits_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	Verb 'exits' * -> Exits;
	[ExitsSub;
		if(RunRoutines(actors_location,before)~=0) rtrue;
		DescribeExits();
		print "^";
	];
#ifdef ORLibDoc;
	!--Commands, document thyselves.
	ORMenu ORHelpExits "Exits" LibraryExtensions with text "The EXITS command will usually list all obvious exits for the current location.";
#endif;
	object with exits_text;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================