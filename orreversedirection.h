!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORReverseDirection	[Z, GLULX]
! A simple routine, shared by multiple objects to reverse a direction.
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
! This is a short, simple routine, not at all worthy of its own module but used by 
! multiple library entries and so is placed in its own module for the sake of sharing.
!
! ORReverse direction simply takes a direction property as a parameter (such as nw_to) 
! and returns the reverse direction (in this case, se_to). It is used by several 
! ORLibrary modules, such as ORNPCVerb, ORNPC_movement, and ORDynaMap.
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORReverseDirection";					!Constant USE_ORReverseDirection; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORReverseDirection; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.17	Initial Creation
! 2002.02.20	Added support for direction objects as well as direction properties
! 2002.02.26	Fit to new template.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORReverseDirection...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORReverseDirection forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORReverseDirection has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORReverseDirection_DONEREPLACE; constant ORReverseDirection_DONEREPLACE; #ifdef ORReverseDirection_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORReverseDirection_DONEMESSAGE; constant ORReverseDirection_DONEMESSAGE;  #ifdef ORReverseDirection_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORReverseDirection requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORReverseDirection_DONECODE;constant ORReverseDirection_DONECODE; #ifdef ORReverseDirection_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	[ReverseDirection d; 
		switch(d){ 
		!--are we reversing a property?
			in_to: d=out_to; 
			out_to: d=in_to; 
			u_to:d=d_to; 
			d_to:d=u_to; 
			n_to: d=s_to; 
			s_to: d=n_to; 
			e_to: d=w_to; 
			w_to: d=e_to; 
			nw_to: d=se_to; 
			se_to: d=nw_to; 
			ne_to: d=sw_to; 
			sw_to: d=ne_to; 
		!--otherwise reversing an object
			in_obj: d=out_obj; 
			out_obj: d=in_obj; 
			u_obj:d=d_obj; 
			d_obj:d=u_obj; 
			n_obj: d=s_obj; 
			s_obj: d=n_obj; 
			e_obj: d=w_obj; 
			w_obj: d=e_obj; 
			nw_obj: d=se_obj; 
			se_obj: d=nw_obj; 
			ne_obj: d=sw_obj; 
			sw_obj: d=ne_obj; 
		}
		return d;
	];
!======================================================================================
#endif; #ifnot; #ifndef ORReverseDirection_DONEGRAMMAR; constant ORReverseDirection_DONEGRAMMAR; #ifdef ORReverseDirection_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================