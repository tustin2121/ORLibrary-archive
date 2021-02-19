!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORReview [Z, Glulx]
! An implementation of the Review Command
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
! Canonically, the REVIEW command acts like a meta version of LOOK.  That is, it 
! enables the player to review the room description without the passages of time.
! This module implements is a slightly expanded version of REVIEW in that it also works 
! for objects that have already been examined. The following transcript, occuring 
! without the number of moves increasing, demonstrates:
!
!    >REVIEW 
!	 Room
!    A basic room.
!
!    You can see a table and chair here.
!
!    >REVIEW TABLE
!    The wooden table is old and stained.
!
!    >REVIEW CHAIR
!    You have not already examined the chair.
!
!--------------------------------------------------------------------------------------
! AutoDep:	
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORReview; #include "ORReview"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORReview; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2004.02.07	Initial Creation
!--------------------------------------------------------------------------------------
message "          Processing library extension ORReview...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORReview forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORReview has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORReview_DONEREPLACE; constant ORReview_DONEREPLACE; #ifdef ORReview_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace ActionPrimitive; 
	attribute been_examined; !--used for REVIEW command
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORReview_DONEMESSAGE; constant ORReview_DONEMESSAGE;  #ifdef ORReview_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORReview requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORReview_DONECODE;constant ORReview_DONECODE; #ifdef ORReview_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	!--add the been_examined attribute after examining
	[ ActionPrimitive;
#ifndef TARGET_GLULX;
		indirect(#actions_table-->action);
#ifnot;
		indirect(#actions_table-->(action+1));
#endif;
		if(action==##Examine) {
			give noun been_examined;
		}
	];
!======================================================================================
#endif; #ifnot; #ifndef ORReview_DONEGRAMMAR; constant ORReview_DONEGRAMMAR; #ifdef ORReview_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
#ifndef bkt_o; !--support for bracketed parser messages, without dependancies
	[bkt_c o; ];
	[bkt_o o; ];
#endif;
	Verb meta 'review' 
		* -> Review
		* noun -> ReviewObj
	;
	[ ReviewSub; 
#ifdef RelookSub;
		RelookSub();
#ifnot;
		LookSub();
#endif;
	];
	[ ReviewObjSub; 
		if(noun has been_examined) {
#ifdef ReExamineSub;
			ReExamineSub();
#ifnot;
			ExamineSub();
#endif;
		}
		else {
			bkt_o();
			print "You have not already examined ",(the)noun,".";
			bkt_c();
			print "^";
		}
	];
!--Commands, document thyselves.
#ifdef ORLibDoc;
	ORMenu ORHelpReview "Review" ORLibDoc
		with text[;
			print "The REVIEW command is provided so the player can look again at a previous description without taking up another turn. Used alone...^^";
			font off; 
				spaces(4);print "REVIEW^^";
			font on;
				print "...it will simply re-describe the current location, but it can also be used with an object that has been previously examined:^^";
			font off; 
				spaces(4);print "REVIEW CANDLE^^";
			font on;
				print "Obviously the command will not work with objects that have not already been described.";
		]
	;
#endif;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================