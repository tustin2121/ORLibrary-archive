!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.03.08 ORNPC_GoalDriven [Z, GLULX]
! Adds the ability for NPCs to perform goals.
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
! NPCs that derive from ORNPC_GoalDriven will perform specific actions when they 
! become possible.  The following is a short example of an NPC that will take the rock 
! when it becomes takeable, and open a chest when it becomes openable:
!
!	object bob "Bob" room has proper
!		class ORNPC_GoalDriven ORNPC_doverb ORNPC
!		with description "Bob looks the same frontwards and backwards."
!		,	name 'bob'
!	;
!
!	[Initialise;
!		location=room;
!		bob.NewGoal(##Take, rock);
!		bob.NewGoal(##Open, chest);		
!	]; 
!--------------------------------------------------------------------------------------
! AutoDep:	ORAction
!			ORNPC
!			ORNPC_DoVerb
!--------------------------------------------------------------------------------------
! NPC Componant Priority: 50  
! The lower the number, the further down the "class" line.  The greater the number, 
! the earlier it needs to come on the class line. Modules with the same priority can
! effectively be exchanged in order without effect.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORNPC_GoalDriven; #include "ORNPC_GoalDriven"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORNPC_GoalDriven; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2004.02.27	Initial Creation
! 2004.03.07	Added step-action system, so the NPC can take obvious (and in some cases
!				not obvious steps towards completing a goal)
! 2004.03.08	Added the ability to supress goal-step creation on the NPC and Action 
!				levels; turned off auto goal-step creation for NPCs. Added ability to 
!				flag Actions as repeatable.  That is, if undone, they will be retried.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORNPC_GoalDriven...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
! #ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORNPC_GoalDriven forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORNPC_GoalDriven has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
#ifndef ORNPC_DONEREPLACE; default USE_ORNPC 0; message "          [ORNPC_GoalDriven forcing inclusion of ORNPC]"; #include "ORNPC"; #endif; 
#ifndef ORNPC_DoVerb_DONEREPLACE; default USE_ORNPC_DoVerb 0; message "          [ORNPC_GoalDriven forcing inclusion of ORNPC_DoVerb]"; #include "ORNPC_DoVerb"; #endif; 
#ifndef ORAction_DONEREPLACE; default USE_ORAction 0; message "          [ORNPC_GoalDriven forcing inclusion of ORAction]"; #include "ORAction"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORNPC_GoalDriven_DONEREPLACE; constant ORNPC_GoalDriven_DONEREPLACE; #ifdef ORNPC_GoalDriven_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORNPC_GoalDriven_DONEMESSAGE; constant ORNPC_GoalDriven_DONEMESSAGE;  #ifdef ORNPC_GoalDriven_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORNPC_GoalDriven requires the OREnglish file.";#endif; 
system_file;
	!--class used to create actions to display
	default GOAL_BUFFER 5;	
	object with ext_GenerateGoalSteps false;
	class ORGoalAction(GOAL_BUFFER)
		class ORAction
		with create[a b c;
				self.ORAction::create(a,b,c);
				give self ~general;
    		]
		,	foreknowledge false
		,	generate_goal_steps true
		,	force false
		,	repeat false
	;
	[GetEarliestPossibleAction a o e;
		if(valueorrun(a,IsDone)==true && valueorrun(a,force)==false) return 0; !--don't iterate down goals that are already done unless we are forcing this
		if(valueorrun(a,CanAttempt)==true) return a; !--this goal is not completed but is possible, so return it
		!--at this point we are guaranteed to have a goal that needs to be done, but is not currently possible
		if(children(a)==0 && valueorrun(a,generate_goal_steps)==true && valueorrun(actor,generate_goal_steps)==true) GenerateGoalSteps(a);  !--Only try to piece together sub-actions if: none already exist and the character is able to generate steps and the action lends itself to this 
		objectloop(o in a) {
			e=GetEarliestPossibleAction(o); !--recursively call for children
			if(e) return e;
		}
		return 0;
	];
	[GenerateGoalSteps obj n s o;
		if(~~(obj ofclass ORGoalAction)) return;
		if(LibraryExtensions.RunUntil(ext_GenerateGoalSteps,true,obj)==true) return;
		n=valueorrun(obj,_noun);
		s=valueorrun(obj,_second);

		switch(valueorrun(obj,_action)){
			##Examine:	GGS_Visible(obj,n);
			##Enter,
			##SwitchOn,
			##SwitchOff,
			##Take,
			##Close:	GGS_Touchable(obj,n);
			##Open:		GGS_Touchable(obj,n); 
						if(n has locked && n provides with_key) {
							o=ORGoalAction.create(##Unlock, n, valueorrun(n,with_key));
							if(o==0) return;
							give o general;
							if(obj provides foreknowledge) o.foreknowledge=obj.foreknowledge;
							move o to obj;
						}
			##Wear,
			##Eat:		GGS_Held(obj,n); 
			##PutOn,	
			##Give,
			##Insert:	GGS_Held(obj,n); 
						GGS_Touchable(obj,s);
			##Unlock,
			##Lock:		GGS_Touchable(obj,n);
						GGS_Held(obj,s); 
		}
	];
	!--	actor has foreknowledge of world or foreknowledge of this action
	[HasForeknowledge actn;
		return ((actor provides foreknowledge && valueorrun(actor,foreknowledge)==true) || (actn provides foreknowledge && valueorrun(actn,foreknowledge)==true));
	];
	[ClearGeneratedGoalSteps topactn selactn o n;
!		objectloop(o in topactn) ClearGeneratedGoalSteps(o,selactn);

		for(o=child(topactn):o~=0: o=n){
			n=sibling(o);
			ClearGeneratedGoalSteps(o,selactn);	
		}
		if(topactn has general && topactn~=selactn) ORGoalAction.destroy(topactn);
	];
	!--three game states that often prevent actions from being possible
	[GGS_Visible actn obj; !--make sure something is in scope 
		actn=obj; !--suppress warnings
		! if(HasForeknowledge(actn)) { TODO: go find the location of the object if npc has foreknowledge or tracker}
	];
	[GGS_Touchable actn obj s o; !--make sure something is touchable 
		GGS_Visible(actn,obj);
		s=parent(obj);
		while(s~=0){
			!--contained, and the actor knows about it...
			if(s has openable && s hasnt open && (s has transparent || HasForeknowledge(actn))) {
				o=ORGoalAction.create(##Open, s);
				if(o==0) return;
				give o general; !--automatically created
				if(actn provides foreknowledge) o.foreknowledge=actn.foreknowledge;
				move o to actn;
			}
			s=parent(s);
		}
	];
	[GGS_Held actn obj o; obj=obj;!--make sure something is being held.  if we get here, then we definately aren't holding it, so take is ALWAYS something to consider 
		o=ORGoalAction.create(##Take, obj);
		if(o==0) return;
		give o general;
		if(actn provides foreknowledge) o.foreknowledge=actn.foreknowledge;
		move o to actn;
	];

	
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNPC_GoalDriven_DONECODE;constant ORNPC_GoalDriven_DONECODE; #ifdef ORNPC_GoalDriven_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
system_file;
	class ORNPC_GoalDriven
		with _select_action[t sgs; 
			for(t=0:t<self.GoalCount():t++){
				sgs=false;
				if(valueorrun(self,generate_goal_steps)==false) sgs=true;
				self.selected_goal=GetEarliestPossibleAction(self.&_goallist-->t, sgs);
				ClearGeneratedGoalSteps(self.&_goallist-->t,self.selected_goal);
				if(self.selected_goal) {
					return _act_goal;
				}
			}
			return self.ORNPC::_select_action();
		]
		,	generate_goal_steps false
		,	foreknowledge false
		,	selected_goal 0
		,	_act_goal[g;
				g=self.selected_goal;
				self.DoVerb(g._action,g._noun,g._second);
				if(valueorrun(g,isdone)==true && valueorrun(g,repeat)==false) g.CanAttempt=false; !--not a repeatable action.  Once done, do not attempt again
				if(g has general) ORGoalAction.destroy(g);
			]
		,	GoalCount[t;
				for(t=0:t<(self.#_goallist/WORDSIZE):t++){
					if(self.&_goallist-->t==0) return t;
				}
				return (self.#_goallist/WORDSIZE);
			]
		,	_goallist 0 0 0 0 0 0 0 0 0 0
		,	RemoveGoal[number t pos g; !--eliminate a position and shift everything forward
				pos=self.GoalCount()-1;
				g=self.#_goallist-->number;
				ORGoalAction.destroy(g);
				for(t=number:t<pos:t++) self.&_goallist-->t=self.&_goallist-->(t+1);
				self.&_goallist-->pos=0;
			]
		,	ClearCompletedGoals[t;
				for(t=0:t<self.GoalCount():t++){
					if(self.&_goallist-->t==0) continue;
					if((self.&_goallist-->t).IsDone()==true) {
						self.RemoveGoal(t);
						t--;
					}
				}
			]
		,	AddGoal[newgoal t;
				if(~~(newgoal ofclass ORGoalAction)){
					"Error: Trying to add a non-ORGoalAction object to the goallist.";
				}
				for(t=0:t<self.#_goallist/WORDSIZE:t++){
					if(self.&_goallist-->t==0){
						self.&_goallist-->t=newgoal ;
						rtrue;
					}
				}
				print "Error: Out or goal Room. Failed to add new goal.";
				rfalse;
			]
		,	NewGoal[a n s o;
				o=ORGoalAction.create(a,n,s);
				self.AddGoal(o);
				return o;
			]
	;

#ifdef DEBUG; 
	!-- validate that all ORNPC_GoalDriven derived NPCs are also derived from ORNP_DoVerb.
	object _npcgoalinit LibraryExtensions
		with ext_initialise[o; 
					objectloop(o ofclass ORNPC_GoalDriven){
						if((o ofclass ORNPC_doverb)==false) print "^[ERROR: ORNPC_GoalDriven behavior requires ORNPC_doverb behavior. (object",(name)o,")]^";
					}
				]
	;
#endif;
!======================================================================================
#endif; #ifnot; #ifndef ORNPC_GoalDriven_DONEGRAMMAR; constant ORNPC_GoalDriven_DONEGRAMMAR; #ifdef ORNPC_GoalDriven_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================