!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.03.05 ORActionQueue [Z, GLULX]
! Gives the player a queue of actions that will be executed sequentially without
! pause (unless interupted, or one of the next action becomes impossible).
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
! A hook into the player's input routines, enabling actions for multiple turns
! to be specified in advance.  An example for a use might be a path of GO commands
! populated using ORPathMaker, so that the player could type "Go to the Library" and
! the actions needed to get there could be generated and placed in the actions queue.
! Another, simpler, example might be a command like "Wait 10 turns" :
!
!	extend "wait" first
!		* number 'turns' -> WaitNum
!		* number -> WaitNum
!	;
!	[WaitNumSub t;
!		if(noun>ACTIONQUEUE_BUFFER) {
!			noun=ACTIONQUEUE_BUFFER; print "[Maximum number of queued actions is ",ACTIONQUEUE_BUFFER,".]^^";
!		}
!		for(t=0:t<noun-1:t++) ActionQueue.AddAction(ORQueuedAction.create(##Wait)); !--queue up all but the first one
!		<<WAIT>>; !--go ahead and do the first one now
!	];
!
!--------------------------------------------------------------------------------------
! AutoDep:	ORListManager
!			ORInformLibrary
!			ORKeyboardPrimitive
!			ORKeyboard
!			ORAction
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORActionQueue; #include "ORActionQueue"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORActionQueue; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2004.02.28	Initial Creation
! 2004.03.05	Replaced the InterruptQueuedActions variable with a property on the 
!				ActionQueue object itself; also fixed a couple of ORListManager calls 
!				that did not specify the property.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORActionQueue...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORActionQueue forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORActionQueue has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORListManager_DONEREPLACE; default USE_ORListManager 0; message "          [ORActionQueue forcing inclusion of ORListManager]"; #include "ORListManager"; #endif; 
	#ifndef ORInformLibrary_DONEREPLACE; default USE_ORInformLibrary 0; message "          [ORActionQueue forcing inclusion of ORInformLibrary]"; #include "ORInformLibrary"; #endif; 
	#ifndef ORKeyboardPrimitive_DONEREPLACE; default USE_ORKeyboardPrimitive 0; message "          [ORActionQueue forcing inclusion of ORKeyboardPrimitive]"; #include "ORKeyboardPrimitive"; #endif; 
	#ifndef ORKeyboard_DONEREPLACE; default USE_ORKeyboard 0; message "          [ORActionQueue forcing inclusion of ORKeyboard]"; #include "ORKeyboard"; #endif; 
	#ifndef ORAction_DONEREPLACE; default USE_ORAction 0; message "          [ORActionQueue forcing inclusion of ORAction]"; #include "ORAction"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORActionQueue_DONEREPLACE; constant ORActionQueue_DONEREPLACE; #ifdef ORActionQueue_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORActionQueue_DONEMESSAGE; constant ORActionQueue_DONEMESSAGE;  #ifdef ORActionQueue_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORActionQueue requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORActionQueue_DONECODE;constant ORActionQueue_DONECODE; #ifdef ORActionQueue_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	default ACTIONQUEUE_BUFFER 5;	

	class ORQueuedAction(ACTIONQUEUE_BUFFER)
		class ORAction
		with create[a b c;
				self.ORAction::create(a,b,c);
				self.selected_text=self.ORQueuedAction::selected_text;
    		]
		,	selected_text[;PrintOrRun(self,format_command,true);]
	;
	ORListManager ActionQueue LibraryExtensions
		with	queued_actions 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		,	AddAction[actn seltxt; 
				if(seltxt) actn.selected_text=seltxt;
				self.insert_end(queued_actions,actn); 
			]
		,	ClearActions[t h; 
				for(t=0:t<self.count(queued_actions):t++) {
					h=self.get(queued_actions,t);
					if(h~=0) ORQueuedAction.destroy(h);
				}
				self.clear(queued_actions); 
			]
		,	ext_initialise[; !--actually install the wedge...
				ORKBPreempt.add_routine(DoQueuedAction); 
			]
		,	ext_messages[; 
				Miscellany:switch(lm_n){
							60: print "[Queued actions interrupted by an event.]^";
							61: print "[Queued actions are no longer possible.]^";
						   }
			]
		,	interrupt false
	;
	[DoQueuedAction a iqa;
		if(ActionQueue.count(queued_actions)==0) rfalse;
		a=ActionQueue.pop(queued_actions);
		iqa=ValueOrRun(ActionQueue,interrupt);
		if(iqa || a.CanAttempt()==false){
			if(iqa==true) L__M(##Miscellany,60);
			else L__M(##Miscellany,61);
			ActionQueue.ClearActions();
			ORQueuedAction.destroy(a);
			if(metaclass(ActionQueue.interrupt)~=Routine) ActionQueue.interrupt=false;
			return -1;
		}
		PrintOrRun(a,selected_text);
		new_line;
		a.execute();
		ORQueuedAction.destroy(a);
		InformLibrary.end_turn_sequence(); !--and do all the normal stuff that follows an action
		DrawStatusLine();
		DisplayStatus();
		if(deadflag) __ORInformLibrary.play_handle_death();
		return -1; !--returning -1 to indicate that we usurped the input and processed this whole turn.  
	];
	
!======================================================================================
#endif; #ifnot; #ifndef ORActionQueue_DONEGRAMMAR; constant ORActionQueue_DONEGRAMMAR; #ifdef ORActionQueue_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================