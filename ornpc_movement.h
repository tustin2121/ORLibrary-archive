!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.03.08 ORNPC_movement	[Z, GLULX]
! Adds movement capabilities to an ORNPC object
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Note: THIS MODULE WILL CHANGE 
! The code in this module, having been largely written before the original ORLibrary 
! in late 2000, lacks the modularity of typical ORLibrary modules. For this reason and 
! in part because the ORLibrary NPC components have become a bit more refined, 
! ORNPC_movement has been slated for revision. 
!
! Most of the changes will be invisible to the typical ORLibrary user; however some 
! of the features not really at home in the ORNPC_movement module, such as the 
! FollowNPCSub, will be moved to modules of their own.
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
! AutoDep :	ORRoutinesList
!			ORNPC
!			ORNPCVerb
!			ORNPC_doverb
!			ORReverseDirection
!--------------------------------------------------------------------------------------
! NPC Componant Priority: 10  
! The lower the number, the further down the "class" line.  The greater the number, 
! the earlier it needs to come on the class line. Modules with the same priority can
! effectively be exchanged in order without effect.
!--------------------------------------------------------------------------------------
! Add to the class section of an ORNPC dervived object to enable an NPC to take action
! and move around, either by wandering, following a path, or following someone else.
!--------------------------------------------------------------------------------------
! To add this module to your library, add the line:
!
!		#include "ORNPC_movement";					!Constant USE_ORNPC_movement; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORNPC_movement; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.12.01	Initial Creation
! 2002.02.13	Added Glulx Support
! 2002.02.20	Modified a few messages
! 2002.02.26	Fit to new template
! 2002.05.13	fixed two missing .h occurances brought about by global removal of
!				of the .h file extensions.
! 2002.05.14	Added code to leverage the ORPathfinder module if provided.  Also implemented
!				a new attribute call "tracker" which implements ORPathFinder in a similar 
!				manner as ORNPC_MapKnown. For followed objects.
! 2002.05.18	Added additional defines to ensure that there is no requirement to use OREnglish
! 2002.09.02	Converted attributes previously defined in this module to properties.
! 2003.03.08	Changed the default cant_move_save to true so that characters that are not 
!				able to move initially can be told to move and will start.
! 2003.03.11	Fixed some missing return values in the ORLibMessage object.
! 2004.01.11	Modified to conform to new stdlib 6.11 features.
! 2004.03.07	Added a message to the FollowNPC routine to print what direction is actually
!				being taken.
! 2004.03.08	Removed reference to unneeded ORRoutineList.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORNPC_movement...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORNPC_movement forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORNPC_movement has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORNPC_DONEREPLACE; default USE_ORNPC 0; message "          [ORNPC_movement forcing inclusion of ORNPC]"; #include "ORNPC"; #endif; 
	#ifndef ORNPCVerb_DONEREPLACE; default USE_ORNPCVerb 0; message "          [ORNPC_movement forcing inclusion of ORNPCVerb]"; #include "ORNPCVerb"; #endif; 
	#ifndef ORNPC_doverb_DONEREPLACE; default USE_ORNPC_doverb 0; message "          [ORNPC_movement forcing inclusion of ORNPC_doverb]"; #include "ORNPC_doverb"; #endif; 
	#ifndef ORReverseDirection_DONEREPLACE; default USE_ORReverseDirection 0; message "          [ORNPC_movement forcing inclusion of ORReverseDirection]"; #include "ORReverseDirection"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORNPC_movement_DONEREPLACE; constant ORNPC_movement_DONEREPLACE; #ifdef ORNPC_movement_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORNPC_movement_DONEMESSAGE; constant ORNPC_movement_DONEMESSAGE;  #ifdef ORNPC_movement_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORNPC_movement requires the OREnglish file.";#endif; 



	class ORNPC_movement
		with	act_move[abletest d;
					if(abletest==true) {
						if(self.follow_object~=0 && ScopeCeiling(self.follow_object)==ScopeCeiling(self)) rfalse;
						return ValueOrRun(self,can_move);
					}
					if(self.follow_object~=0){
						d=self.ajoining_room_to_direction(ScopeCeiling(self.follow_object));
#ifdef USE_ORPathMaker;
						if(d==0 && valueorrun(self,tracker)==true){
							d=ORPathMaker.Determine_Path(self,self.follow_object);
							if(d<0)
								d=0;
							else
								d=self.ajoining_room_to_direction(d);	
						}
#endif;							
						if(d~=0) {
							self.move_to(d); 
							return;
						}
					}
					if(self.#path>0 && self.path~=0) 
						self.move_path();
					else								
						self.move_random();
				]
		,		tracker false
		,		can_move		true
		,		can_move_save	true
		,		priority_save	0
		,		can_move_to[obj; !--can move to or through given object
					if(metaclass(obj)~=object) return false;
					if(obj hasnt locked || (obj provides with_key && parent(obj.with_key)==self)) return true;
					return false;
				]
		,		halt[;
					!---Note, the below line should not call for the value of can move (as a routine)
					!---   since we are checking only to see if it is turned all the way off
					if(self.can_move~=false){
						self.can_move_save=self.can_move;
						self.can_move=false;
					}
				]
		,		unhalt[;
					!---Note, the below line should not call for the value of can move (as a routine)
					!---   since we are checking only to see if it is turned all the way off
					if(self.can_move==false){
						self.can_move=self.can_move_save;
						self.can_move_save=false;
					}
				]
		,		start_following[obj;
					self.unhalt();
					if(obj provides priority) {
						self.priority_save=self.priority;
						self.priority=obj.priority+1;
					}
					self.follow_object=obj;
				]
		,		stop_following[; 
					self.priority=self.priority_save;
					self.follow_object=0;
				]
		,		follow_object	0
		,		path			0
		,		path_ptr		0
		,		path_ptr_inc	1
		,		reverse_at_path_end false
		,		last_location	0
		,		no_where_to_go	0
		,		missing_path_msg[;
					print (ig)CIVerb(self,"looked","look")," a little disoriented for a moment. ";
				]
		,		ajoining_room_to_direction[room t o; !---given an ajoining room, return the dir_obj the npc needs to travel to get there
					objectloop(t in compass) { !---t is the direction object
						o=parent(self).(t.door_dir);
						if(o~=0 && (o==room ||(o provides door_to && valueorrun(o,door_to)==room))) return (t.door_dir);
					}
					return 0;
				]
		,		FindLocationInPath[loc t;
					for(t=0:t<self.#path/WORDSIZE:t++){
						if(self.&path-->t==loc) return t;
					}
					return -1;
				]
		,		move_path [t; !called when an NPC is following a path rather than wandering at random
					!-- Get next item in list, ignore if needed
					t=self.&path-->(self.path_ptr);	
					self.path_ptr=self.path_ptr+self.path_ptr_inc;	!-- move pointer to next position
					if(t==0 || t==scopeceiling(self)) return; !--return if empty element or the current location
					
					!-- translate to appropriate direction
					if(scopeceiling(t)~=compass) !if not a direction, then really is a room, so turn it into a direction 
						t=self.ajoining_room_to_direction(t);	
					else{ !--is a direction
						t=t.door_dir;
						if(self.path_ptr_inc<0) t=ReverseDirection(t);
					}
					
					if(t==0) !-- problem with path?
						self.missing_path();
					else
						self.move_to(t); !-- move in determined direction 

					!-- handle if reached end of list
					if(self.path_ptr>=(self.#path/WORDSIZE) || self.path_ptr<0 ) 
						self.handle_end_of_path();
				]
		,		handle_end_of_path[; 
					if(self.reverse_at_path_end==false) { !--option 1, wrap around to the begining (if not in retreace steps mode)
						self.path_ptr=0; 
					}
					else{ !--option 2, reverse direction
						self.path_ptr_inc=self.path_ptr_inc*-1; !reverse direction
						self.path_ptr=self.path_ptr+(2*self.path_ptr_inc); !do twice (to undo the previous adjustemnt)
					}
				]
		,		missing_path[t; !--there is a problem with the path...
					if(self.missing_path_msg~=0 && CanPlayerWitness(self)==true) printorrun(self,missing_path_msg); !--print a message to indicate a problem with the path
					self.move_random(); !--wander instead
					t=self.FindLocationInPath(parent(self)); !--see if we can find the new location in the path we have been following
					if(t~=-1)self.path_ptr=t;
					return;
				]
		,		move_random !when wandering/exploring the npc will pick a random direction, that he did not just come from, and head that way
				[togo t o;
					!step one: get list of possible directions to travel
					npctemparray-->0=0;
					objectloop(t in compass) {
						o=parent(self).(t.door_dir);
						if(o==0 || metaclass(o)~=object) continue;
						if(o has door) o=valueorrun(o,door_to);
						if(	self.last_location~=o && self.can_move_to(o)==true) {
							npctemparray-->0=(npctemparray-->0)+1;
							npctemparray-->(npctemparray-->0)=t; !--save dir_obj that must be traveled
						}
					}
					!if no place left to go then try going back the way we came in
					if(npctemparray-->0==0) 
						togo=self.ajoining_room_to_direction(self.last_location);
					else
						togo=(npctemparray-->random(npctemparray-->0)).door_dir;
					!---at this point, togo is either equal to the destination-direction, or is nothing
					if(togo)
						return self.move_to(togo);
					else
						if(self.no_where_to_go~=0) printorrun(self,no_where_to_go);

				]
		,		move_to [ direction obj; !---is called to actually generate the actions to move the NPC in the given direction 
					self.last_location=parent(self);
					obj=valueorrun(parent(self),direction); !---get the destination 
					if(obj has door) {
						if(obj has locked && obj.with_key in self) self.doverb(##Unlock, obj,obj.with_key);
						if(obj hasnt open) self.doverb(##Open, obj);
						obj=valueorrun(obj,door_to);
					}
					self.doverb(##go,DirectionObjFromDirection(direction));
					if(parent(self)==self.last_location) {
						!print "[ERROR:  npc was not able to travel in direction ",(ig)LanguageDirection(direction),".]";
						if(self.missing_path_msg~=0 && CanPlayerWitness(self)==true) printorrun(self,missing_path_msg); !--print a message to indicate a problem with the path
						return; !---didn't go anywhere...
					}
				]
	;

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNPC_movement_DONECODE;constant ORNPC_movement_DONECODE; #ifdef ORNPC_movement_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
#ifndef bkt; !--support for bracketed parser messages, without dependancies
	[bkt o; print o;];
	[bkt_c o; ];
	[bkt_o o; ];
#endif;

	object _onm_init LibraryExtensions
		with ext_messages[; 
		FollowNPC: 
			switch(lm_n){
				1: print (ig)CIVerb(actor,"began","begin")," to follow ",(TheMe)noun,", but ",(ig)vrb(actor,"seemed","seem")," to have lost ",(me)noun,". ";
					rtrue;
			}
		ContinueFollowingObject:
			switch(lm_n){
				1:	if(noun==actor){
						print (ig)CICant(actor)," follow ",(Myself)noun,". ";
					}
					else{
						print (ig)CICant(actor)," follow ",(TheMe)noun,". ";
					}
					print_ret (ig)CIHave(noun)," not gone anywhere. ";
				2: print_ret (ig)CIVerb(actor,"looked","look")," at ",(TheMe)noun," and then ",(ig)vrb(actor,"nodded","nod")," agreeably. ";
				3: print_ret (ig)CIVerb(actor,"shook","shake")," ",(my)actor," head. ~I don't follow,~ ",(ig)CIVerb(actor,"said","say"),". ";
			}
StopVague:	
					bkt_o(); 
					print "You'll need to be more specific about what you would like stopped.";
					bkt_c();
					new_line;
					rtrue;
		StopFollowing:
			switch(lm_n){
				1:	print_ret (ig)CIVerb(actor,"stopped","stop")," paying attention to ",(TheMe)noun," and ",(ig)vrb(actor,"looked","look")," away.";
				2:  print_ret (CIAm)actor," not following anything. ";
			}
		Halt:
			switch(lm_n){
				1:print_ret (CIAm)actor," not moving around. ";
				2:print_ret (ig)CIVerb(actor,"stood","stand")," still.";
			}
		Unhalt:
			switch(lm_n){
				1:print_ret (CIAm)actor," not standing particularly still. ";
				2:print_ret (ig)CIVerb(actor,"resumed","resume")," a more mobile posture. ";
			}
	];
	[DirectionObjFromDirection d;
		switch(d){
			in_to: d=in_obj;
			out_to: d=out_obj;
			u_to:d=u_obj;
			d_to:d=d_obj;
			n_to: d=n_obj;
			s_to: d=s_obj;
			e_to: d=e_obj;
			w_to: d=w_obj;
			nw_to: d=nw_obj;
			se_to: d=se_obj;
			ne_to: d=ne_obj;
			sw_to: d=sw_obj;
		}
		return d;
	];
	[FollowNPCSub t dir;
		t=DirectionFromActor(ScopeCeiling(noun));
		if(t~=0) {
			dir=DirectionObjFromDirection(t);
			if(actor==player)print "(Go ",(name)dir,")^";
			<Go dir>;
			if(actor~=player)ContinueFollowingObjectSub();
			return;
		}
		L__M(##FollowNPC,1); !--I don't see him
	];
	[ContinueFollowingObjectSub;
		if(actor==player || actor==noun){ 
			L__M(##ContinueFollowingObject,1); !--but the noun is right here 
			return;
		}
		if(actor provides follow_object) {
			actor.start_following(noun);
			L__M(##ContinueFollowingObject,2); !--okay, I'll follow that...
			return;
		}
		L__M(##ContinueFollowingObject,3); !--no, I'm not going to follow that...
	];
	[NPCInNeighboringRoom o; !isolate NPCs in neighboring rooms that just left the actor's locations
		switch(scope_stage) 
		{
			1: return false;
			2:	objectloop(o has animate && DirectionFromActor(ScopeCeiling(o))~=0) PlaceInScope(o); 
				rtrue;
			3: "It is unclear who you want me to follow. ";
		} 
	];
	[StopVagueSub;
		L__M(##StopVague,1);
		return false;
	];
	[StopFollowingSub;
		if(actor provides follow_object && actor.follow_object~=0){
			L__M(##StopFollowing,1,actor.follow_object);!--okay
			actor.stop_following();
		}
		else L__M(##StopFollowing,2);!--I'm not following anything

	];
	[HaltSub;
		if(actor==player || (actor provides can_move && valueorrun(actor,can_move)==0) || (actor provides halt)==false) return L__M(##Halt,1); !--not doing anything
		actor.halt();
		L__M(##Halt,2);
	];
	[UnHaltSub;
		if(actor==player || (actor provides can_move && valueorrun(actor,can_move)~=0) || (actor provides unhalt)==false) return L__M(##Unhalt,1); !--not really standing still as such
		actor.unhalt();
		L__M(##Unhalt,2);
	];
	!----------------------------------------------------------------------------------------
	!This routine determines if a given room is adjacent to the actor's current location.
	!If it is, then the direction the actor must travel to reach it is returned
	[DirectionFromActor room t o sc; 
		objectloop(t in compass){
			sc=ScopeCeiling(actor);
			o=sc.(t.door_dir);
			if(o~=0 && metaclass(o)==object && o has door)o=valueorrun(o,door_to);
			if(o==room) return t.door_dir; 
		}
		return 0;
	];
	system_file;
	object _npcmoveinit LibraryExtensions
		with ext_initialise[o; 
					objectloop(o ofclass ORNPC_movement){
						if((o ofclass ORNPC_doverb)==false) print "^[ERROR: ORNPC_movement behavior requires ORNPC_doverb behavior. (object",(name)o,")]^";
						o.register_action(act_move);
					}
				]
	;
!======================================================================================
#endif; #ifnot; #ifndef ORNPC_movement_DONEGRAMMAR; constant ORNPC_movement_DONEGRAMMAR; #ifdef ORNPC_movement_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	Verb "halt" 
			*  -> Halt
	;
	Verb "remain" 
			*  -> Halt
			* 'here' -> Halt
	;
	Verb "stay" 
			*  -> Halt
			* 'here' -> Halt
			* 'still' -> Halt
	;
	Extend "stand" first
			* 'still'  -> Halt
			* 'here' -> Halt
	;
	Verb "unhalt" 
			*  -> unhalt
	;
	Verb "follow" 
			* scope=NPCInNeighboringRoom -> FollowNPC
			* noun -> ContinueFollowingObject
	;
	Verb "stop" 
			*  -> StopVague
			* 'follow'/'following' -> StopFollowing
			* 'follow'/'following' creature -> StopFollowing
			* 'halt'/'halting'/'standing'/'remaining'/'staying' ->UnHalt
			* 'standing'/'remaining'/'staying' 'here'/'still' ->UnHalt
	;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================