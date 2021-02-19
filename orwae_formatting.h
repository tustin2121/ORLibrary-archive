!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORWAE_Formatting [Z, GLULX]
! A framework for implementing text that qualifies objects, such as "(providing light)".
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
! During inventory and room descriptions, the standard library interogates objects and 
! prints qualifying text such as "(providing light)" or "(closed, empty and providing 
! light)".  This module implements an extensible framework for defining new qualifiers 
! as well as redefining or removing existing qualifiers.
! 
! This is accomplished through the use of routines that are registered with at least
! one of two ORRoutinesList objects, "ORWAE_PartFormatting" and "ORWAE_FullFormatting".
!
! This module eliminates the use of Messages "ListMiscellany #7-17".
!
!--------------------------------------------------------------------------------------
! AutoDep:	
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORWAE_Formatting; #include "ORWAE_Formatting"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORWAE_Formatting; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2003.03.15	Initial Creation
!--------------------------------------------------------------------------------------
message "          Processing library extension ORWAE_Formatting...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORWAE_Formatting forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORWAE_Formatting has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORRoutinesList_DONEREPLACE; default USE_ORRoutinesList 0; message "          [ORWAE_Formatting forcing inclusion of ORRoutinesList]"; #include "ORRoutinesList"; #endif; 
	#ifndef ORWriteAfterEntry_DONEREPLACE; default USE_ORWriteAfterEntry 0; message "          [ORWAE_Formatting forcing inclusion of ORWriteAfterEntry]"; #include "ORWriteAfterEntry"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORWAE_Formatting_DONEREPLACE; constant ORWAE_Formatting_DONEREPLACE; #ifdef ORWAE_Formatting_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace WAE_PartInv;
	replace WAE_FullInv;
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORWAE_Formatting_DONEMESSAGE; constant ORWAE_Formatting_DONEMESSAGE;  #ifdef ORWAE_Formatting_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORWAE_Formatting requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORWAE_Formatting_DONECODE;constant ORWAE_Formatting_DONECODE; #ifdef ORWAE_Formatting_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
[ WAE_PartInv o comb;
	if (c_style & PARTINV_BIT ~= 0) {
		comb=ORWAE_PartFormatting.run_accumulate(o,true);
		if(comb==0) rfalse;
		ORWAE_PartFormatting.total_count=comb;
		print " (";
		if(comb==1) print "which ",(isorare)o," ";
		ORWAE_PartFormatting.run_accumulate(o);
		print ")";
	}
	rfalse;
];
[WAE_FullInv o comb;
	if (c_style & FULLINV_BIT ~= 0) {
		if (o.invent ~= 0)
		{   inventory_stage=2;
			if (RunRoutines(o,invent)~=0)
			{   if (c_style & NEWLINE_BIT ~= 0) new_line;
				rtrue;
			}
		}
		comb=ORWAE_FullFormatting.run_accumulate(o,true);
		if(comb==0) rfalse;
		ORWAE_FullFormatting.total_count=comb;
		print " (";
		if(comb==1) print "which ",(isorare)o," ";
		ORWAE_FullFormatting.run_accumulate(o);
		print ")";		
	}
	rfalse;
];

system_file;
ORRoutinesList ORWAE_PartFormatting LibraryExtensions
	with routines 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
	,	between_calls[retval;
			if(retval==false) return;
			self.total_count=self.total_count-1;
			if(self.total_count==1) print " and ";
			if(self.total_count>1) print ", ";
		]
	,	ext_initialise[;
			self.add_routine(_WAE_light);
			self.add_routine(_WAE_closedcontainer );
			self.add_routine(_WAE_emptycontainer );
		]
	,	total_count 0
;
ORRoutinesList ORWAE_FullFormatting LibraryExtensions
	with routines 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
	,	between_calls[retval;
			if(retval==false) return;
			self.total_count=self.total_count-1;
			if(self.total_count==1) print " and ";
			if(self.total_count>1) print ", ";
		]
	,	ext_initialise[;
			self.add_routine(_WAE_light);
			self.add_routine(_WAE_worn);
			self.add_routine(_WAE_closedcontainer );
			self.add_routine(_WAE_opencontainer );
			self.add_routine(_WAE_lockedcontainer );
			self.add_routine(_WAE_emptycontainer );
		]

,	total_count 0
;
!--New formatting routines ---------------------------------------------------------------
[_WAE_worn obj suppress retval;
	retval=(obj has worn);
	if(suppress==false && retval==true) L__M(##ListMiscellany, 4);
	return retval;
];
[_WAE_light obj suppress retval;
	retval=(obj has light && location hasnt light && parent(actor) hasnt light);
	if(suppress==false && retval==true) L__M(##ListMiscellany, 3);
	return retval;
];
[_WAE_closedcontainer obj suppress retval;
	retval=(obj has container && obj hasnt open);
	if(suppress==false && retval==true) L__M(##ListMiscellany, 1);
	return retval;
];
[_WAE_opencontainer obj suppress retval;
	retval=(obj has container && obj has openable && obj has open);
	if(suppress==false && retval==true) L__M(##ListMiscellany, 6);
	return retval;
];
[_WAE_lockedcontainer obj suppress retval;
	retval=(obj has openable && obj hasnt open && obj has lockable && obj has locked);
	if(suppress==false && retval==true) L__M(##ListMiscellany, 5);
	return retval;
];
[_WAE_emptycontainer obj suppress retval;
	retval=(obj has container && obj has open or transparent && (child(obj)==0));
	if(suppress==false && retval==true) L__M(##ListMiscellany, 2);
	return retval;
];
!-----------------------------------------------------------------------------------------
object _waef_init LibraryExtensions
	with ext_messages[;
		ListMiscellany:switch(lm_n){
							1: print "closed";
								rtrue;
							2: print "empty";
								rtrue;
							3: print "providing light";
								rtrue;
							4: print "being worn";
								rtrue;
							5: print "locked";
								rtrue;
							6: print "open";
								rtrue;
						}
					]
;

!======================================================================================
#endif; #ifnot; #ifndef ORWAE_Formatting_DONEGRAMMAR; constant ORWAE_Formatting_DONEGRAMMAR; #ifdef ORWAE_Formatting_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================