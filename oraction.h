!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.06.08 ORAction [Z, GLULX]
! A base class to represent actions and detect whether or not they are available.
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
! A more detailed description of this file, what it does, and how to use it goes here.
!--------------------------------------------------------------------------------------
! AutoDep:	
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORAction; #include "ORAction"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORAction; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2004.02.15	Initial Creation
! 2004.02.27	CanAttempt for Take check if object is held by another character.
! 2004.02.28	Removed debug code and logic flaw in isdone and canattempt routines.
! 2004.03.07	Refined CanAttempt to take Untouchable state into consideration. 
!--------------------------------------------------------------------------------------
message "          Processing library extension ORAction...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORAction forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORAction has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORAction_DONEREPLACE; constant ORAction_DONEREPLACE; #ifdef ORAction_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORAction_DONEMESSAGE; constant ORAction_DONEMESSAGE;  #ifdef ORAction_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORAction requires the OREnglish file.";#endif; 
	class ORAction
		with create[a n s;
				self._action=a;
				self._noun=n;
				self._second=s;
				self.performer=self.ORAction::performer; !--reset to base routine
				self.CanAttempt=self.ORAction::CanAttempt; !--reset to default in case overridden in a previous instance...
			]
		,	IsMeta false
		,	 performer[; return actor;]
		,	_action 0
		,	_noun 0
		,	_second 0
		,	format_command[n s a;
				a=ValueOrRun(self,_action);
				print (ActionName)a;
				n=valueorrun(self,_noun);
				s=valueorrun(self,_second);
				if(n~=0) print " ",(name)n;
				if(s~=0) print " ",(name)s;
			]
		,	IsDone[a n s actn;
				if(LibraryExtensions.RunUntil(ext_IsCompletedAction,true,self)~=true) {
					a=valueorrun(self,performer);
					n=valueorrun(self,_noun);
					s=valueorrun(self,_second);
					actn=ValueOrRun(self,_action);
					!--handle no-noun verbs here
					switch(actn){
						!--add default handler here.
					}
					switch(actn){
						##Take: if(n in a) rtrue;
						##Drop: if(~~(n in a)) rtrue;
						##Open: if(n has open) rtrue;
						##Close: if(n hasnt open) rtrue;
						##Wear: if(n in a && n has worn) rtrue;
						##Disrobe: if(~~(n in a) || n hasnt worn) rtrue;
						##Eat: if(parent(n)==0) rtrue;
						##Enter: if(a in n) rtrue;
						##SwitchOn: if(n has switchable && n has on) rtrue;
						##SwitchOff:if(n has switchable && n hasnt on) rtrue;
					}
					switch(actn){
						##PutOn: if(n in a && s has supporter) rtrue;
						##Give: if(n in a && s has animate) rtrue;
						##Insert: if(n in a && s has container) rtrue;
						##Unlock: if(n hasnt locked) rtrue;
						##Lock:if(n has locked) rtrue;
					}
				}
				rfalse;
				
			]
		,	CanAttempt[a n s actn matchscope touch_n touch_s;
				matchscope=true; !--default to in scope
				touch_n=true;  !--and touchable
				touch_s=true;  !--and touchable
				if(LibraryExtensions.RunUntil(ext_CanAttemptAction,false,self)~=false) {
					a=valueorrun(self,performer);
					n=valueorrun(self,_noun);
					s=valueorrun(self,_second);
					actn=ValueOrRun(self,_action);
					!--handle no-noun verbs here
					switch(actn){
						!--add default handler here.
					}
					!--ensure noun in scope from here on...
					if(n==0 || testscope(a,n)==false) {
						touch_n=matchscope=false; !--not in scope
					}
					else touch_n=(objectisuntouchable(n,true)==false);
					
					switch(actn){
						##Examine: if(~~(matchscope)) rfalse;
						##Take: if(~~(touch_n && n hasnt scenery or static && parent(n) hasnt animate && parent(n)~=player)) rfalse;
						##Drop: if(~~(n in a)) rfalse;
						##Open: if(~~(touch_n && n has openable && n hasnt open && n hasnt locked)) rfalse;
						##Close: if(~~(touch_n && n has openable && n has open)) rfalse;
						##Wear: if(~~(n in a && n has clothing && n hasnt worn)) rfalse;
						##Disrobe: if(~~(n in a && n has worn)) rfalse;
						##Eat: if(~~(touch_n && n has edible)) rfalse;
						##Enter: if(~~(touch_n && n has enterable or door && (n hasnt openable || n has open))) rfalse;
						##SwitchOn: if(~~(touch_n && n has switchable && n hasnt on)) rfalse;
						##SwitchOff:if(~~(touch_n && n has switchable && n has on)) rfalse;
					}
					!--ensure second in scope from here on (and noun too)...
					if(s==0 || testscope(a,s)==false) {
						touch_s=matchscope=false; !--not in scope
					}
					else touch_s=(objectisuntouchable(s,true)==false);

					switch(actn){
						##PutOn: if(~~(touch_s && n in a && s has supporter)) rfalse;
						##Give: if(~~(touch_s && n in a && s has animate)) rfalse;
						##Insert: if(~~(touch_s && n in a && s has container)) rfalse;
						##Unlock: if(~~(touch_n && n has locked && s in a)) rfalse;
						##Lock:if(~~(touch_n && n hasnt locked && n has lockable && s in a)) rfalse;
					}
				}
				rtrue; !--default to possible if we've nothing to say it isn't
			]
		,	Execute[saveactor;
				saveactor=actor;
				actor=ValueOrRun(self,performer); 
				action=valueorrun(self,_action);	!--action
				noun=valueorrun(self,_noun);		!--noun of the action
				second=valueorrun(self,_second);	!--second of the action
				ActionPrimitive();	
				actor=saveactor;
			]
	;
	object	with	ext_CanAttemptAction 0 !--define for use...
		,	ext_IsCompletedAction 0
	;

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORAction_DONECODE;constant ORAction_DONECODE; #ifdef ORAction_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	[ ActionName a str;
		#ifdef TARGET_GLULX;
			str = #identifiers_table-->6;
		#ifnot;
			str = #identifiers_table;
			str = str + 2*(str-->0) + 2*48;
		#endif;
			print (string) str-->a;
	];
!======================================================================================
#endif; #ifnot; #ifndef ORAction_DONEGRAMMAR; constant ORAction_DONEGRAMMAR; #ifdef ORAction_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================