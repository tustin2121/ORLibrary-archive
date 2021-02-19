!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.09 ORNPC_interact	[Z, GLULX]
! A base class for NPC objects to derive from.  Add support for interacting with other
! NPCs of the PC.
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
! NPC Componant Priority: 10  
! The lower the number, the further down the "class" line.  The greater the number, 
! the earlier it needs to come on the class line. Modules with the same priority can
! effectively be exchanged in order without effect.
!--------------------------------------------------------------------------------------
! This is the base class, used by other modules to add support to an NPC to interact 
! with other characters.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORNPC_interact";					!Constant USE_ORNPC_interact; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORNPC_interact; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.01.20	Initial Creation (not really, but this was the earliest date I could 
!				find without searching my code archive.
! 2002.02.26	Fit to new framework.
! 2002.03.28	Modified interacted routines to differentiate between response to other
!				npcs (when moving the following turn should be allowed) and response to 
!				player actions (when the npc should wait and let the player drive)
! 2002.07.13	Addessed code that prevented the "interacting_with" property from being 
!				handled programatically.
! 2004.02.09	Optimized objectlooops.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORNPC_interact...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORNPC_interact forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORNPC_interact has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORNPC_interact_DONEREPLACE; constant ORNPC_interact_DONEREPLACE; #ifdef ORNPC_interact_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORNPC_interact_DONEMESSAGE; constant ORNPC_interact_DONEMESSAGE;  #ifdef ORNPC_interact_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORNPC_interact requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNPC_interact_DONECODE;constant ORNPC_interact_DONECODE; #ifdef ORNPC_interact_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	class ORNPC_interact
		with	interacting_with 0
		,		will_engage_player true
		,		last_move_interacted 0
		,		interacted[marker; 
					marker=turns;
					if(ORNPCControl.npcs_acting==false) marker++; !--if reacting to a player action then skip next turn
					if(self.last_move_interacted<marker) self.last_move_interacted=marker;
				]
		,		has_already_interacted[; if(self.last_move_interacted<turns) rfalse; rtrue;]
		,		engage_person[count o;
					npctemparray-->0=0;
					objectloop(o near self){
						if(o has talkable or animate && o~=self or player 
							&& o provides interacting_with && valueorrun(o,interacting_with)==0 or self){
								npctemparray-->0=(npctemparray-->0)+1;
								npctemparray-->((npctemparray-->0))=o;
						}
					}
 					if(testscope(player,self) && valueorrun(self,will_engage_player)==true) {!---if the player is here, then add him/her to the list too
						npctemparray-->0=(npctemparray-->0)+1;
						npctemparray-->((npctemparray-->0))=player;
					}
					if(count==true) return npctemparray-->0;
					self.interacting_with=npctemparray-->(random(npctemparray-->0));
					return self.interacting_with;
				]
		,	disengage[;
				self.interacting_with=0;
		]
	;
!======================================================================================
#endif; #ifnot; #ifndef ORNPC_interact_DONEGRAMMAR; constant ORNPC_interact_DONEGRAMMAR; #ifdef ORNPC_interact_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================