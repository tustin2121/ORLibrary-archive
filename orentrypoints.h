!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 OREntryPoints [Z, GLULX]
! Definition of common entry points so that multiple definitions can work together.
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
! A more detailed description of this file, what it does, and how to use it goes here.
!
!--------------------------------------------------------------------------------------
! AutoDep:	ORRoutineList
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_OREntryPoints; #include "OREntryPoints"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_OREntryPoints; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.08.31	Initial Creation
! 2002.09.08	Fixed issue with beforeparsing calling parsenoun object.
! 2002.10.13	1) Created a reference to the chooseobject property to avoid errors when
!					it is not used by the developer. 
!				2) Fixed reference to ParseNoun from within UnknownVerb code. 
!				3) Fixed issue with ParseNoun return when no ParseNoun routines have been
!					registered.
! 2003.01.25	Modified the structure of the call to ChooseObjects to better suite its
!				definition in the DM4.
! 2003.01.31	Added AfterPrompt.
! 2003.03.01	Fixed problem with nested if's in chooseobjects functionality.
! 2004.01.24	Added Inscope.
!--------------------------------------------------------------------------------------
message "          Processing library extension OREntryPoints...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [OREntryPoints forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "OREntryPoints has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORRoutinesList_DONEREPLACE; default USE_ORRoutinesList 0; message "          [OREntryPoints forcing inclusion of ORRoutinesList]"; #include "ORRoutinesList"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef OREntryPoints_DONEREPLACE; constant OREntryPoints_DONEREPLACE; #ifdef OREntryPoints_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef OREntryPoints_DONEMESSAGE; constant OREntryPoints_DONEMESSAGE;  #ifdef OREntryPoints_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! OREntryPoints requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef OREntryPoints_DONECODE;constant OREntryPoints_DONECODE; #ifdef OREntryPoints_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	!-- - - - - - - - - - - - - - - - - - - - - - - - - - -
	#ifndef BeforeParsing; message "               --BeforeParsing...Implemented";
		ORRoutinesList ORBeforeParsing
			with routines 0 0 0 0 0  
			,	save_wn 0 !--save the wn variable
			,	between_calls[; wn=self.save_wn;]
		;
		[BeforeParsing obj rv;
			ORBeforeParsing.save_wn=wn;
			rv=ORBeforeParsing.run_until(true,obj);
			wn=ORBeforeParsing.save_wn;
			return rv;
		];
	#ifnot; message "               --BeforeParsing...Already defined. NOT IMPLEMENTING."; #endif;
	!-- - - - - - - - - - - - - - - - - - - - - - - - - - -
	#ifndef ChooseObjects; message "               --ChooseObjects...Implemented";
		ORRoutinesList ORChooseObjects with routines 0 0 0 0 0,	chooseobject;
		[ChooseObjects obj code retval;
			retval=0; if(code==2) retval=1; !--default to 0 if testing against "All" or 1 if asking for assistance
			if(obj provides chooseobject) retval=obj.chooseobject(code); !--check object for routine first...
			if(code<2){
				if(retval==0) retval=ORChooseObjects.run_until_not(0,obj,code); !--check "all" routines until something actually returns a real value...
			}else{
				if(retval==1) retval=ORChooseObjects.run_until_not(1,obj,code); !--check "assistance" routines until something actually returns a real value...
			}
			return retval;
	];
	#ifnot; message "               --ChooseObjects...Already defined. NOT IMPLEMENTING."; #endif;
	!-- - - - - - - - - - - - - - - - - - - - - - - - - - -
	#ifndef ParseNoun; message "               --ParseNoun...Implemented";
		ORRoutinesList ORParseNoun
			with routines 0 0 0 0 0  
			,	save_wn 0 !--save the wn variable
			,	between_calls[; wn=self.save_wn;]
		;
		[ParseNoun obj;
			if(ORParseNoun.count_routines()==0) return -1;
			ORParseNoun.save_wn=wn;
			return ORParseNoun.run_until_not(-1,obj);
		];
	#ifnot; message "               --ParseNoun...Already defined. NOT IMPLEMENTING."; #endif;
	!-- - - - - - - - - - - - - - - - - - - - - - - - - - -
	#ifndef UnknownVerb; message "               --UnknownVerb...Implemented";
		ORRoutinesList ORUnknownVerb with routines 0 0 0 0 0;
		[UnknownVerb obj; return ORUnknownVerb.run_until_not(false,obj); ];
	#ifnot; message "               --UnknownVerb...Already defined. NOT IMPLEMENTING."; #endif;
	!-- - - - - - - - - - - - - - - - - - - - - - - - - - -
	#ifndef AfterPrompt; message "               --AfterPrompt...Implemented";
		ORRoutinesList ORAfterPrompt with routines 0 0 0 0 0;
		[AfterPrompt; return ORAfterPrompt.run_until(true); ];
	#ifnot; message "               --AfterPrompt...Already defined. NOT IMPLEMENTING."; #endif;
	!-- - - - - - - - - - - - - - - - - - - - - - - - - - -
	#ifndef InScope; message "               --InScope...Implemented";
		ORRoutinesList ORInScope with routines 0 0 0 0 0;
		[InScope obj; return ORInScope.run_until(true,obj); ];
	#ifnot; message "               --InScope...Already defined. NOT IMPLEMENTING."; #endif;
	!-- - - - - - - - - - - - - - - - - - - - - - - - - - -
!======================================================================================
   #endif; #ifnot; #ifndef OREntryPoints_DONEGRAMMAR; constant OREntryPoints_DONEGRAMMAR; #ifdef OREntryPoints_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================
!-- AfterLife       0;
!-- AfterPrompt     0;
!-- Amusing         0;
!-------- BeforeParsing
!-------- ChooseObjects 
!-- DarkToDark      0;
!-- DeathMessage    0;
!-- GamePostRoutine 0;
!-- GamePreRoutine  0;
!-------- InScope
!-- LookRoutine     0;
!-- NewRoom         0;
!-------- ParseNoun
!-- ParseNumber     2;
!-- ParserError     1;
!-- PrintRank
!-- PrintTaskName   1;
!-- PrintVerb       1;
!-- TimePasses      0;
!-------- UnknownVerb

!-- #ifdef TARGET_GLULX;
	!-- HandleGlkEvent  2;
	!-- IdentifyGlkObject 4;
	!-- InitGlkWindow   1;
!-- #endif; ! TARGET_GLULX