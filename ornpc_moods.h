!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORNPC_moods	[Z, GLULX]
! Adds a "mood" or "mind-set" framework to an NPC.
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
! NPC Componant Priority: 5  
! The lower the number, the further down the "class" line.  The greater the number, 
! the earlier it needs to come on the class line. Modules with the same priority can
! effectively be exchanged in order without effect.
!--------------------------------------------------------------------------------------
! This is the base class, used by other modules to add support for an NPC to have 
! moods, become irritable or happy, and to remember these impressions. 
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORNPC_moods";					!Constant USE_ORNPC_moods; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORNPC_moods; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.08	Initial Creation (Not really, but this is the earliest date I could 
!				find without searching my code archives.)
! 2002.02.25	Fixed bug in messages and fit to new framework.
! 2002.02.27	Added missing #endif missed when new framework was added.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORNPC_moods...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORNPC_moods forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORNPC_moods has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORNPC_moods_DONEREPLACE; constant ORNPC_moods_DONEREPLACE; #ifdef ORNPC_moods_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORNPC_moods_DONEMESSAGE; constant ORNPC_moods_DONEMESSAGE;  #ifdef ORNPC_moods_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORNPC_moods requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNPC_moods_DONECODE;constant ORNPC_moods_DONECODE; #ifdef ORNPC_moods_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	constant NPCLivid			0;
	constant NPCAngry			1;
	constant NPCNormal			2;
	constant NPCHappy			3;
	constant NPCEcstatic		4;

	system_file;
	class ORNPC_moods
		with	mind_set	0						!current frame of mind of the npc
		,		ImpressionsOf	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0		!list of things the npc likes or dislikes
		,		Impressions		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0		!actual list of how he feels about the above subjects
		,		Locate_Impression[obj t;
					for(t=0:t<(self.#ImpressionsOf)/WORDSIZE:t++) if(self.&ImpressionsOf-->t==obj) return t;
					!--if not already remembered, then lets choose the first blank
					for(t=0:t<(self.#ImpressionsOf)/WORDSIZE:t++) if(self.&ImpressionsOf-->t==0) return t;
					!--if no blank spots left, then NPC is out of memory slots and suffering for Alziemers.  Pick a random thing to forget
					t=random((self.#ImpressionsOf)/WORDSIZE);
					self.&ImpressionsOf-->t=0;
					return t;
				]
		,		affect_from[obj;
					if(self.FrameOfMind(self.Recall_Impression(obj))<NPCNormal) self.agitate(true); !if doesn't like person, then become slightly irritated.
					if(self.FrameOfMind(self.Recall_Impression(obj))<NPCAngry) self.agitate(true);  !if REALLY doesn't like person, then become even more irritated.
					if(self.FrameOfMind(self.Recall_Impression(obj))>NPCNormal) self.cheerup(true); !if likes person, then become slightly cheered up 
					if(self.FrameOfMind(self.Recall_Impression(obj))>NPCHappy) self.cheerup(true);  !if REALLY likes person, then become even more cheered up 
				]
		,		Record_Impression[obj; !--associate the way the npc currently feels with the given npc
					if(obj==nothing) return 0;				
					self.&Impressions-->(self.Locate_Impression(obj))=self.mind_set;
				]
		,		Recall_Impression[obj; !--returns how the npc feels about the given object
					if(obj==nothing) return 0;				
					return self.&Impressions-->(self.Locate_Impression(obj));
				]
		,		MentalState[;
					return self.FrameOfMind(self.mind_set);
				]
		,		FrameOfMind[checkval r;
					switch(checkval){
						-9999 to -10:r=NPCLivid;
						-9 to -4: r=NPCAngry;
						4 to 9: r=NPCHappy;
						10 to 9999: r=NPCEcstatic;
						default:r=NPCNormal;
					}
					return r;
				]
		,		agitate_msg[;
					if(self==player) print_ret (ig)CIVerb(self,"became","become")," more irritated. ";
					else if(CanPlayerWitness(self)) print (CTheI) self," appear",(ig)ppf("ed","s")," to become more irritated. ";
				]
		,		agitate [silent; !call to increase agitation of the npc
					self.mind_set=self.mind_set-random(3);
					if(silent) return;
					printorrun(self,agitate_msg);
				]
		,		cheerup_msg[;
					if(CanPlayerWitness(self)) print (CTheI) self," appear",(ig)ppf("ed","s")," to soften somewhat. ";
				]
		,		cheerup [silent;!call to increase happiness of the npc
					self.mind_set=self.mind_set+random(3);
					if(silent) return;
					printorrun(self,cheerup_msg);
				]
	;
!======================================================================================
#endif; #ifnot; #ifndef ORNPC_moods_DONEGRAMMAR; constant ORNPC_moods_DONEGRAMMAR; #ifdef ORNPC_moods_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================