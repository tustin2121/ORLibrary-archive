!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.22 ORNPC	[Z, GLULX]
! A base class and controlling daemon for NPCs.
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
! AutoDep:	
!--------------------------------------------------------------------------------------
! NPC Componant Priority: 0  
! The lower the number, the further down the "class" line.  The greater the number, 
! the earlier it needs to come on the class line. Modules with the same priority can
! effectively be exchanged in order without effect.
!--------------------------------------------------------------------------------------
! This is the base class which lays the groundwork for all NPC componant classes in the 
! ORLibrary.  Even without any componant classes, this modules enables NPCs that 
! have their heartbeat methods called every turn.
!
! To register this module with your library, add the line:
!
!		#include "ORNPC";					!Constant USE_ORNPC; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORNPC; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.08	Initial Creation (not really, but this is the earliest date that I could 
!				find without searching my code archive)
! 2002.02.26	fit to new template
! 2002.03.28	Added code to indicate whether the NPC daemon is currently running.
! 2002.04.14	Fixed issue caused by .h removal.
! 2002.05.24	Added the heartbeat_post property which is called after the additive 
!				heartbeat process runs.
! 2002.08.21	Added support for players entering a floating object (A strange thing
!				to do, but the standard library seems to allow this if simply by virtue 
!				of the fact that MoveFloatingObjects is called so infrequently)
! 2002.09.02	Converted attributes previously defined in this module to properties.
! 2003.01.31	Making NPCs transparent by default.
! 2003.02.06	Reduced number of blank slots reserved for actions.  Can always increase 
!				in derived classes.
! 2003.03.04	Modified the CanPlayerWitness routine to NOT test the scope of the noun
!				and second objects. This caused inappropriate messages when the object 
!				was in scope but the npc was not.
! 2004.01.27	Added code to handle pre-move actor state.
! 2004.02.15	Rearranged the action select code, so that it can be replaced by other 
!				componants.
! 2004.02.22	Removed the code added on 2004.01.15 to handle pre-move actor state; 
!				this has been fixed in the 6.11 standard library final release.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORNPC...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
! #ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORNPC forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORNPC has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
#ifndef ORPronoun_DONEREPLACE; default USE_ORPronoun 0; message "          [ORNPC forcing inclusion of ORPronoun]"; #include "ORPronoun"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORNPC_DONEREPLACE; constant ORNPC_DONEREPLACE; #ifdef ORNPC_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace MoveFloatingObjects;
	property additive	heartbeat;
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORNPC_DONEMESSAGE; constant ORNPC_DONEMESSAGE;  #ifdef ORNPC_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORNPC requires the OREnglish file.";#endif; 
	system_file;
	!---Can the player see what the current actor is doing? (current actor may be overriden with a passed in object)
	!- - - - - - - - - - - - - - - - - - - - - - - -
	!---	Checking the scope of the noun and second objects is no longer done. When it was, 
	!---	NPCs that continuously tried to refer to an object that was not in scope for them, but
	!---	WAS in scope for the, player generated library messages that were inappropriate.
	[CanPlayerWitness one scp; 
		scp=ScopeCeiling(player);
		if(one==0) one=actor;
		return scp==ScopeCeiling(one);
!--		if(scp==ScopeCeiling(actor) || (noun && scp==ScopeCeiling(noun)) || (second && scp==ScopeCeiling(second)) ) return true; 
!--		return false; 
	];
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNPC_DONECODE;constant ORNPC_DONECODE; #ifdef ORNPC_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	[ MoveFloatingObjects i k l m address flag;
		objectloop (i)
		{   
			if(i==parent(actor)) continue; !--lets not try to place an object inside of itself or remove the actors current location from the game world (This can only happen if an actor enters a floating object)
			address=i.&found_in;
			if (address~=0 && i hasnt absent)
			{   
				if (ZRegion(address-->0)==2)
				{   if (i.found_in() ~= 0) move i to parent(actor); else remove i;
				}
				else
				{   k=i.#found_in;
					for (l=0: l<k/WORDSIZE: l++)
					{   m=address-->l;
						if (m==parent(actor) || m in parent(actor))
						{   
							if (i notin parent(actor)) move i to parent(actor);
							flag = true;
						}
					}
					if (flag == false) { if (parent(i)) remove i; }
				}
			}
		}
	];

	array		npctemparray-->0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0; !---this is a generic array for use by various npc behaviors
	array		npctemparraybaselevel-->0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0; !---this is a generic array by base level object only

	class ORNPC
		has		animate transparent
		with	actions				0 0 0 0 0
		,		priority			0
		,		DoNothing_msg		0 !---message to print when npc takes no action
		,		continued_action	0
		,		heartbeat_post	0
		,		heartbeat[actn;
					npctemparray-->0=npctemparray-->0; !--suppress warning
					if(self.continued_action~=0){
						if(self.continued_action(true)~=true)
							self.continued_action=0;
						else{
							self.continued_action();
							return;
						}
					}
					actn=self._select_action();
					if(actn~=0)
						self.actn();
					else
						if(self.DoNothing_msg~=0) printorrun(self,DoNothing_msg);
				]
		,		_select_action[t action; !--select a random, action which is doable...
					npctemparraybaselevel-->0=0;
					for(t=1:t<=self.&actions-->0:t++){ !---cycle through all registered actions
						action=self.&actions-->t;
						if(self.action(true)==true){ !---consider the the current action?
							npctemparraybaselevel-->0=(npctemparraybaselevel-->0)+1; !--- then add it to the list
							npctemparraybaselevel-->(npctemparraybaselevel-->0)=action;	
						}
					}
					if(npctemparraybaselevel-->0>0) return npctemparraybaselevel-->(random(npctemparraybaselevel-->0));
					return 0;

				]
		,		register_action[prop;
					if((self.&actions-->0)>=(self.#actions/WORDSIZE)) {
						print "[ERROR: No room to register additional actions.]";
						return;
					}
					self.&actions-->0=(self.&actions-->0)+1; !---increment action counter
					self.&actions-->(self.&actions-->0)=prop;
				]
		,		ownwill false	!---True For NPCs that are "possessed" by the PC (or visa-versa).  That is, they 
							!---  will try to perform NPC actions even when they are the player
	;
	!---ORNPCControl  object
	object ORNPCControl LibraryExtensions
		with pause false
		,	npcs_acting false
		,	npclist 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		,	daemon
			[npc t lowest_priority highest_priority outcount savedparent savedparentloc;
				if(self.pause==true) return;
				
				!--in the weird case that the player is inside of a floating object...
				savedparent=parent(player); 
				savedparentloc=parent(parent(player));
				
				self.npcs_acting=true;
				self.&npclist-->0=0;
				objectloop(npc ofclass ORNPC && npc has animate && metaclass(parent(npc))==object) { 
					self.&npclist-->0=(self.&npclist-->0)+1;
					self.&npclist-->(self.&npclist-->0)=npc;
					if(npc.priority>highest_priority)highest_priority=npc.priority;
					if(npc.priority<lowest_priority)lowest_priority=npc.priority;
				}
				for(outcount=lowest_priority:outcount<=highest_priority:outcount++){
					for(t=0:t<self.&npclist-->0:t++){
						npc=self.&npclist-->(t+1);
						if(npc==player && valueorrun(npc,ownwill)==false) continue;
						if(npc.priority~=outcount) continue;
						ClearPronoun(); 
						actor=npc;
						MoveFloatingObjects();
						npc.heartbeat();
						if(npc.heartbeat_post~=0) npc.heartbeat_post();
						!if(CanPlayerWitness(npc)==true) PronounNotice(npc);
					}						
				}
				ClearPronoun();
				actor=player;		!restore the actor
				MoveFloatingObjects();
				self.npcs_acting=false;

				!--handle the odd case that the player is within a floating object...
				if(parent(player)==savedparent && parent(parent(player))~=savedparentloc){ !-- if the player wasn't moved by an NPC...
					move parent(player) to savedparentloc; !--then lets make sure that players location wasn't move around by non-typical use of MoveFloatingObjects
				}

			]
		,	ext_initialise[; StartDaemon(self);]
	;
!======================================================================================
#endif; #ifnot; #ifndef ORNPC_DONEGRAMMAR; constant ORNPC_DONEGRAMMAR; #ifdef ORNPC_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================