!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORSuppressTakeAll [Z, GLULX]
! Allows suppression or altogther elimination of "TAKE ALL".
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
! There's been a fair amount of attention paid to the suppression of TAKE ALL in games.  
! The DM4 discusses a technique for suppression of scenery objects using TAKE ALL in 
! section 33.  Roger Firth demonstrates disabling TAKE ALL completely at the following 
! URL:
!
! http://www.firthworks.com/roger/informfaq/aa.html#7
!
! This module implements a similar implementation.  Simply include this module and 
! DM4's method of ignoring scenery objects from take all will be in effect. However, 
! if the developer would rather suppress TAKE ALL entirely, then this behavior can be 
! turned on by setting the "take_completely" property of the SuppressTakeAll object 
! (probably best done in Initialise):
!
!		SuppressTakeAll.take_completely=true;
!
! Note that this technique differs from Roger Firth's in that it still allows for 
! qualified lists. For example, "TAKE ALL APPLES" will still function as expected.
!
! Because TAKE ALL can take the form of the REMOVE command (e.g.: TAKE ALL FROM BOX),
! this command can also be suppressed in the same manner as TAKE. Similar to the 
! "take_completely" propery, so can the the unqualified REMOVE ALL command be 
! completely suppressed with the "remove_completely" property:
!
!		SuppressTakeAll.remove_completely=true;
!
! Again, qualified REMOVEs will continue to work (e.g.: REMOVE ALL MARBLES FROM BOX).
!--------------------------------------------------------------------------------------
! AutoDep:	
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
! 
!		#ifdef USE_ORSuppressTakeAll; #include "ORSuppressTakeAll"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORSuppressTakeAll; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2003.01.25	Initial Creation
! 2003.01.31	Modified message to utilize brackets if present.
! 2003.02.17	Modified code to utilize the remove_completely and take_completely 
!				properties.
! 2003.03.01	Further modifications have been introduced.  The code no longer mimics
!				Roger's functionality since the previous state of this module seemed 
!				to inhibit all forms of multi take.  For example, TAKE ORANGES would not 
!				work.  This module will now scan the input to differentiate between a 
!				simple TAKE ALL and a qualified TAKE ALL MARBLES. It will allow the 
!				latter to work but suppress the former.
! 2003.07.02	Made a change to NOT filter out scenery objects if qualifying words
!				are used.  This allows a player to attempt to take specific scenery 
!				objects in multiples.  That is, "take all tables"
!--------------------------------------------------------------------------------------
message "          Processing library extension ORSuppressTakeAll...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORSuppressTakeAll forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORSuppressTakeAll has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORRoutinesList_DONEREPLACE; default USE_ORRoutinesList 0; message "          [ORSuppressTakeAll forcing inclusion of ORRoutinesList]"; #include "ORRoutinesList"; #endif; 
	#ifndef OREntryPoints_DONEREPLACE; default USE_OREntryPoints 0; message "          [ORSuppressTakeAll forcing inclusion of OREntryPoints]"; #include "OREntryPoints"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORSuppressTakeAll_DONEREPLACE; constant ORSuppressTakeAll_DONEREPLACE; #ifdef ORSuppressTakeAll_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORSuppressTakeAll_DONEMESSAGE; constant ORSuppressTakeAll_DONEMESSAGE;  #ifdef ORSuppressTakeAll_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORSuppressTakeAll requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORSuppressTakeAll_DONECODE;constant ORSuppressTakeAll_DONECODE; #ifdef ORSuppressTakeAll_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	#ifndef bkt; [bkt m; print (string)m;]; #endif;
	object SuppressTakeAll LibraryExtensions with take_completely false , remove_completely false, ext_initialise[;ORChooseObjects.add_routine(__suppresstakeall);];
	[__suppresstakeall obj code compos count;
		if(code==2) return 1; !--return default for "assistance" check
		if(code==1){
			!--okay, trying to build a list...
			wn=1; !--lets begin by analyzing what was entered to see if any attempt was made to qualify the list, or if it was just a "take all"
			while(true){
				compos=NextWordStopped();
				if(compos==-1 or 'from' or 'off') break; !--we've either reached the end of the input, or the end of possible qualifiers, so lets exit
				if(compos~=verb_word or ALL1__WD or ALL2__WD or ALL3__WD or ALL4__WD or ALL5__WD) {
					count++;
				}
				if(compos==comma_word) count=0; !--reset after an order
				
			}
			if(count>0) return 0; !--use default parser result if qualifying words were used
			!--at this point we know that no attempt was made to qualify the "all".  Let's filter out all scenery objects..
			if(obj has scenery) return 2; !--remove all scenery objects from lists			
			!--And let's see if we are going to allow TAKE ALL or REMOVE ALL FROM
			if(action_to_be==##Take && valueorrun(SuppressTakeAll,take_completely)==true) return 2;  !--if setup to completely exclude "take all"
			if(action_to_be==##Remove && valueorrun(SuppressTakeAll,remove_completely)==true) return 2;  !--if setup to completely exclude "remove all"
			return 0;
		}
		return 0;
	];
	object _supta_init LibraryExtensions
		with ext_messages[;
		Miscellany:	if(lm_n==44){
						switch(action_to_be){
							##Take:	if(valueorrun(SuppressTakeAll,take_completely)==true)
									print_ret (bkt)"You must qualify ALL in this context (e.g.:TAKE ALL ORANGES).";
							##Remove:
									if(valueorrun(SuppressTakeAll,remove_completely)==true)
										print_ret (bkt)"You must qualify ALL in this context (e.g.:REMOVE ALL APPLES).";
						}
					}
					rfalse;
	];
!======================================================================================
#endif; #ifnot; #ifndef ORSuppressTakeAll_DONEGRAMMAR; constant ORSuppressTakeAll_DONEGRAMMAR; #ifdef ORSuppressTakeAll_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================