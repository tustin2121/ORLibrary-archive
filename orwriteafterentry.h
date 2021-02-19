!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORWriteAfterEntry [Z, GLULX]
! A rearangement of the WriteAfterEntry routine.  No actual code has been altered (at
! least not in any significant way) but the routine has been broken into smaller 
! componants which can be overriden by other modules.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- - 
! License: 
! Matches that of Inform's standard library.
!--------------------------------------------------------------------------------------
! AutoDep:	none
!--------------------------------------------------------------------------------------
! ORLibDev: Note, that this module is not useful as a library extension alone.  By 
!     itself it provides no new functionality for the IF developer. Instead it provides
!     a basis for other modules to be built upon and will primarily be useful to those 
!     who are developing ORLibrary entries of their own.
!--------------------------------------------------------------------------------------
! Although arranged differently, this code is nevertheless the same WriteAfterEntry
! routine that comes from the Inform standard library 6.10.  
! It falls under the same copyright as the code upon which it was based 
! which can be found in the standard library's verblibm file.  
!--------------------------------------------------------------------------------------
! This module replaces the large and bulky original version of WriteAfterEntry with a 
! smaller version that does little more than make calls to routines that are 
! compartmentalizations of the code originally making up the WriteAfterEntry routine. 
! These smaller routines can more selectively be replaced by other modules.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORWriteAfterEntry; #include "ORWriteAfterEntry"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORWriteAfterEntry; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2003.02.21	Initial Creation
! 2003.12.26	Resynced with the new standard library 6.11
! 2004.01.17	Corrected accidental code modification that was not caught before 
!				release.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORWriteAfterEntry...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORWriteAfterEntry forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORWriteAfterEntry has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORWriteAfterEntry_DONEREPLACE; constant ORWriteAfterEntry_DONEREPLACE; #ifdef ORWriteAfterEntry_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace WriteAfterEntry;
	global _WAE_recurse_flag=0; !-
	global _WAE_parenth_flag=0; !-
	global _WAE_child_count=0;  !- flag3
	global _WAE_eldest_child=0; !-
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORWriteAfterEntry_DONEMESSAGE; constant ORWriteAfterEntry_DONEMESSAGE;  #ifdef ORWriteAfterEntry_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORWriteAfterEntry requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORWriteAfterEntry_DONECODE;constant ORWriteAfterEntry_DONECODE; #ifdef ORWriteAfterEntry_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
[WriteAfterEntry o depth stack_p recurse_flag parenth_flag eldest_child child_count;
	eldest_child=_WAE_eldest_child;	recurse_flag=_WAE_recurse_flag;	parenth_flag=_WAE_parenth_flag;	child_count=_WAE_child_count; !--this routine is called recursively.  Lets save off the values of the flag variables that we made global.
		_WAE_eldest_child=0;_WAE_recurse_flag=0;_WAE_parenth_flag=0;_WAE_child_count=0;
		if(WAE_PartInv(o,depth,stack_p)==true) jump exit_wae;
		if(WAE_FullInv(o,depth,stack_p)==true) jump exit_wae;
		if(WAE_Conceal(o,depth,stack_p)==true) jump exit_wae;
		if(WAE_Always(o,depth,stack_p)==true) jump exit_wae;
		if(WAE_Recurse(o,depth,stack_p)==true) jump exit_wae;
		if(WAE_IsOrAre(o,depth,stack_p)==true) jump exit_wae;
		if(WAE_Newline(o,depth,stack_p)==true) jump exit_wae;
		if(WAE_DoList(o,depth,stack_p)==true) jump exit_wae;
.exit_wae;
	_WAE_eldest_child=eldest_child;_WAE_recurse_flag=recurse_flag;_WAE_parenth_flag=parenth_flag;_WAE_child_count=child_count;
];
!======================================================================================
#endif; #ifnot; #ifndef ORWriteAfterEntry_DONEGRAMMAR; constant ORWriteAfterEntry_DONEGRAMMAR; #ifdef ORWriteAfterEntry_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
system_file;
#ifndef WAE_PartInv;
[ WAE_PartInv o combo depth stack_p;depth=depth; stack_p=stack_p;
    inventory_stage = 2;
    if (c_style & PARTINV_BIT) {
        if (o.invent && RunRoutines(o, invent))                 ! These lines
            if (c_style & NEWLINE_BIT) ""; else rtrue;          ! added

        combo = 0;
        if (o has light && location hasnt light) combo=combo+1;
        if (o has container && o hasnt open)     combo=combo+2;
        if ((o has container && (o has open || o has transparent))
            && (child(o)==0))                    combo=combo+4;
        if (combo) L__M(##ListMiscellany, combo, o);
    }   ! end of PARTINV_BIT processing
	rfalse;
];
#endif;
#ifndef WAE_FullInv;
[WAE_FullInv o depth stack_p;depth=depth; stack_p=stack_p;
    if (c_style & FULLINV_BIT) {
        if (o.invent && RunRoutines(o, invent))
            if (c_style & NEWLINE_BIT) ""; else rtrue;

        if (o has light && o has worn) { L__M(##ListMiscellany, 8);    _WAE_parenth_flag = true; }
        else {
            if (o has light)           { L__M(##ListMiscellany, 9, o); _WAE_parenth_flag = true; }
            if (o has worn)            { L__M(##ListMiscellany, 10, o);_WAE_parenth_flag = true; }
        }

        if (o has container)
            if (o has openable) {
                if (_WAE_parenth_flag) print (string) AND__TX;
                else              L__M(##ListMiscellany, 11, o);
                if (o has open)
                    if (child(o)) L__M(##ListMiscellany, 12, o);
                    else          L__M(##ListMiscellany, 13, o);
                else
                    if (o has lockable && o has locked) L__M(##ListMiscellany, 15, o);
                    else                                L__M(##ListMiscellany, 14, o);
               _WAE_parenth_flag = true;
            }
            else
                if (child(o)==0 && o has transparent)
                    if (_WAE_parenth_flag) L__M(##ListMiscellany, 16, o);
                    else              L__M(##ListMiscellany, 17, o);

        if (_WAE_parenth_flag) print ")";
    }   ! end of FULLINV_BIT processing
	rfalse;
];
#endif;
#ifndef WAE_Conceal;
[WAE_Conceal o p depth stack_p; depth=depth; stack_p=stack_p;
    if (c_style & CONCEAL_BIT) {
        _WAE_child_count = 0;
        objectloop (p in o)
            if (p hasnt concealed && p hasnt scenery) { _WAE_child_count++; _WAE_eldest_child = p; }
    }
    else { _WAE_child_count = children(o); _WAE_eldest_child = child(o);}
	rfalse;
];
#endif;
#ifndef WAE_Always;
[WAE_Always o depth stack_p; depth=depth; stack_p=stack_p;
    if (_WAE_child_count && (c_style & ALWAYS_BIT)) {
        if (c_style & ENGLISH_BIT) L__M(##ListMiscellany, 18, o);
        _WAE_recurse_flag=true;
    }
	rfalse;
];
#endif;
#ifndef WAE_Recurse;
[WAE_Recurse o depth stack_p; depth=depth; stack_p=stack_p;
    if (_WAE_child_count && (c_style & RECURSE_BIT)) {
        if (o has supporter) {
            if (c_style & ENGLISH_BIT) {
                if (c_style & TERSE_BIT) L__M(##ListMiscellany, 19, o);
                else                     L__M(##ListMiscellany, 20, o);
                if (o has animate)       print (string) WHOM__TX;
                else                     print (string) WHICH__TX;
            }
            _WAE_recurse_flag = true;
        }
        if (o has container && (o has open || o has transparent)) {
            if (c_style & ENGLISH_BIT) {
                if (c_style & TERSE_BIT) L__M(##ListMiscellany, 21, o);
                else                     L__M(##ListMiscellany, 22, o);
                if (o has animate)       print (string) WHOM__TX;
                else                     print (string) WHICH__TX;
                }
            _WAE_recurse_flag = true;
        }
    }
	rfalse;
];
#endif;
#ifndef WAE_IsOrAre;
[WAE_IsOrAre o depth stack_p; o=o; depth=depth; stack_p=stack_p;
	if (_WAE_recurse_flag && (c_style & ENGLISH_BIT)){
        if (_WAE_child_count > 1 || _WAE_eldest_child has pluralname)	
			print (string) ARE2__TX;
        else
			print (string) IS2__TX;
	}
  rfalse;
];
#endif;
#ifndef WAE_Newline;
[WAE_Newline o depth stack_p; o=o; depth=depth; stack_p=stack_p;
	if (c_style & NEWLINE_BIT ~= 0) new_line;	
	rfalse;
];
#endif;
#ifndef WAE_DoList;
[WAE_DoList o depth stack_p; depth=depth; stack_p=stack_p;
    if (_WAE_recurse_flag) {
        o = child(o);
        #Ifdef TARGET_ZCODE;
        @push lt_value; @push listing_together; @push listing_size;
        #Ifnot; ! TARGET_GLULX;
        @copy lt_value sp; @copy listing_together sp; @copy listing_size sp;
        #Endif;
        lt_value = 0;   listing_together = 0;   listing_size = 0;
        WriteListR(o, depth+1, stack_p);
        #Ifdef TARGET_ZCODE;
        @pull listing_size; @pull listing_together; @pull lt_value;
        #Ifnot; ! TARGET_GLULX;
        @copy sp listing_size; @copy sp listing_together; @copy sp lt_value;
        #Endif;
        if (c_style & TERSE_BIT) print ")";
    }
  rfalse;
];
#endif;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================