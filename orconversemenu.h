!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.04.28 ORConverseMenu [Z, Glulx]
! A conversation menu which sits on top of the Ask/Tell system.
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
! The "MenuSpeak" conversation system, this is an implementaton of an ORMenuAction object 
! which leverages the ORActionMenu module. This module presupposes that the player 
! object derives from OROptionList and leverages the ORKnowledgeWeb classes for 
! conversation. 
!--------------------------------------------------------------------------------------
! AutoDep:	ORActionMenu
!			ORKnowledgeWeb
!----------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORConverseMenu; #include "ORConverseMenu"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORConverseMenu; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.07.21	Initial Creation
! 2002.08.23	Fixed bug when menu does not open when NPC's being talked to are present
!				but not in the same location (e.g. sitting on a chair)
! 2002.09.02	Converted attributes previously defined in this module to properties.
! 2002.10.19	Added autodep for ObjectInitialise.  Also fixed NOABOUT reversal bug.
! 2003.01.11	Rearranged some code to filter the conversation menu from a stand-alone
!				routine named FilterConverseMenu() so that it can be determined whether 
!				or not their are possible conversation topics prior to showing the menu. 
!				Also added a few additional synonyms to the menuspeak and automenuspeak
!				commands.
!				Also added filtering out of name of person being spoken to if 
!				nonspecifictarget property is true.
! 2003.01.16	Changed the default setting of AutoShowConverseMenu to false rather 
!				true.
! 2003.01.31	Modified the MenuSpeak to respond with an appropriate message if there 
!				is nothing to display.
! 2003.02.16	Fixed bug with ~~ preventing name of person talking to...
! 2003.02.17	Added a property to the ConMenu object to allow an action to determine
!				if it was called from the menu or not.
! 2003.02.19	Added better ifdef for self docs
! 2003.02.27	Another battle with newlines.
! 2003.03.12	modified AutoDoc text.
! 2004.01.11	Modified to use the stdlib 6/11b2 new ext_initialise functionality
! 2004.01.30	Minor code change to support change in ORKeyboardPrimitive module.
! 2004.02.09	Added objectloop optimizations.
! 2004.02.12	Minor code change to utilize ORActionMenu's new extendable entry point
!				ext_PreFilterActionMenu rather than the standard library's ext_initialise.
! 2004.02.15	Modifications to support changes in ORActionMenu (using ORMenuAction object
!				instead of ORAction)
! 2004.02.28	Minor modifications to match recent ORKeyboard, ORKeyboardPrimitive
!				and ORActionMenu changes
! 2004.04.28	Implemented the new meta functionality...
!--------------------------------------------------------------------------------------
message "          Processing library extension ORConverseMenu...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORConverseMenu forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORConverseMenu has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORActionMenu_DONEREPLACE; default USE_ORActionMenu 0; message "          [ORConverseMenu forcing inclusion of ORActionMenu]"; #include "ORActionMenu"; #endif;  
	#ifndef ORKnowledgeWeb_DONEREPLACE; default USE_ORKnowledgeWeb 0; message "          [ORConverseMenu forcing inclusion of ORKnowledgeWeb]"; #include "ORKnowledgeWeb"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORConverseMenu_DONEREPLACE; constant ORConverseMenu_DONEREPLACE; #ifdef ORConverseMenu_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORConverseMenu_DONEMESSAGE; constant ORConverseMenu_DONEMESSAGE;  #ifdef ORConverseMenu_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORConverseMenu requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORConverseMenu_DONECODE;constant ORConverseMenu_DONECODE; #ifdef ORConverseMenu_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	default MSMulti "Multi"; !--used to indicate a topic that can be spoken to multiple people (and needs to be clarified)
	OROptionList _menuol; !--used to prompt the user for clarification of topic target
	!--The OptionListMenu that will actually display Conversation Topics
	OROptionListMenu ConMenu LibraryExtensions 
		with source[; return player;]
		,	directions[; return ValueOrRun(ActMenu,directions);]
		,	FilterTopics[ o a i c x; !--after filtering, return true or false to indicate whether or not the menu can be displayed.
				!--------------------------------------------
				! Here we need to filter the menu and modify
				! the way it displays
				!--------------------------------------------
				a=ValueORRun(self,source); !--just for ease of referencing below
				if(a==0)rfalse;
				if(~~(a ofclass OROptionList)) rfalse; !--the talker needs to be derived from OROptionList
				for(i=0:i<a.count():i++)  give a.GetOption(i) workflag; !--clear everything
				objectloop(o near a){
						if(o has animate or talkable && o~=a) c++; !--number of people present
				}
				if(c~=0) { !--if there are actually people present (one or more) 
					for(i=0:i<a.count():i++){ !--cycle through all topics...
						o=a.GetOption(i); !--get the topic 
						if(c==1){ !--only one person to talk to...
							a.SetAssociation2(i,0); !--... so clear the second association (we don't want to qualify this in the menu)
							if(o.locatetargetcount(a)==1) give o ~workflag; !--since topic can be spoken to 1 person then must be the same so make this topic displayable
						}
						else{ !--here we have multiple people to talk to 
							x=o.locatetargetcount(a);
							if(x==0) continue;
							give o ~workflag; !--otherwise we can talk about this...
							if(o provides nonspecifictarget && valueorrun(o,nonspecifictarget)==true) 
								a.SetAssociation2(i,0); !--clear 
							else {
								if(x==1)
									a.SetAssociation2(i,o.locatetarget(a)); !--set Association2 to the one person
								else
									a.SetAssociation2(i,MSMulti);!--set Association2 to the "MULTI" indicator
							}
						}
					}
				}
				if(self.count()>0) rtrue; 
				rfalse;
		]
		,	ext_PreFilterActionMenu[;
				ActMenu.add(oa_menuspeak,"Speak"); ActMenu.add(oa_menuspeak,"Speak");
			]
		,	noabout !--just to define the property
	;
	!--The action object which generates the command to show the convesation menu
	ORMenuAction oa_menuspeak
		with _action ##ShowConverseMenu
		,	_noun -1
		,	silent true
		,	autorunifonly true
		,	RemoveAfterSelect false
		,	CanAttempt[;
				return ConMenu.FilterTopics();
			]	
		,	execute[;
				self.ORAction::execute();
				return valueorrun(self,IsMeta);
			]
	;
	

	!--Actually show the conversation menu.  It displays the menu, and issues the command if one was chosen
	[DoConverseMenu fromcommand r o a i t ss; !--actually show the menu and perform the action if selected...pass in true to force a menu regardless of the state of the AutoShowActionMenu variable
		if(deadflag~=0) rtrue;

		!--first, massage the data in the pool...
		oa_menuspeak.IsMeta=false; !--reset, in case it was set during a previous turn
		
		r=ConMenu.FilterTopics();
		a=ValueORRun(ConMenu,source); !--the return value, just for ease of referencing below
		!--------------------------------------------
		!--show the menu if supposed to...
.domenuloc;
		o=0; !--this will be the selected menu option (the ORKnowledgeTopic object)
		if((AutoShowActionMenu==true || fromcommand==true) &&  a ofclass OROptionList && r==true) {
			o=ConMenu.show();
			if(o~=0 && ActMenu.showcount>0) {
				ActMenu.showcount=ActMenu.showcount-1 ; !--use the action direction count down 
				if(ActMenu.showcount==0) ConMenu.directions=0; !--to turn off the directions for conversations too
			}
		}
		if(o~=0){ !--if we actually had a result
			t=a.GetAssociation2(a.LocateOption(o));
			if(o provides nonspecifictarget && valueorrun(o,nonspecifictarget)==true) t=0;
			switch(t){  !--get the association2 of the selected item (person to whom we can speak this to)
				MSMulti	: _menuol.clearoptionlist(); !--we need to clarify to whom we are speaking
						objectloop(t near a){
							if(t has animate or talkable && t~=a && o.isincontext(t,a)==true && o.isknownby(t)~=o.isknownby(a)) 
								_menuol.pushoption(t);
						}
						ss=ConMenu.source; !--the actual value, whether a routine or object
						ConMenu.source=_menuol;
						i=conmenu.show();
						ConMenu.source=ss;
				default	: i=o.locatetarget(a); !--works for either zero or the single option
			}
			if(i==0) jump domenuloc; !--chose a topic, but couldn't find a target; go back and redisplay the begining menu
		}
		DrawStatusLine();
		DisplayStatus();
		if(o==0) {
			oa_menuspeak.IsMeta=true; !--used to indicate that nothing was done, so don't do the end turn sequence; and continue as normal
			return;
		}
		!--otherwise, a knowledge topic WAS selected from the menu. SO lets create and perform the talking command.  Set...
		actor=a;			!--actor to the person whose decision pool the menu is focused on
		noun=i;				!--noun to the person we are talking to
		second=o;			!--second to the actual topic chosen
		if(o.isknownby(actor)==true){	!--if known by the person talking then assume tell mode
			print "tell ";
			action=##TellTopic;	
		}
		else{							!--otherwise we must be asking a question
			print "ask ";
			action=##AskTopic;
		}
		!--We've already printed ASK/TELL.  Lets finish the "imaginary" command text output.  For example: Tell...
		if((~~(o provides nonspecifictarget)) || (o provides nonspecifictarget && valueorrun(o,nonspecifictarget)==false)) print (name)noun," ";			!--GINGER...
		if((~~(o provides noabout)) || (o provides noabout && valueorrun(o,NoAbout)==false)) print "about "; !--ABOUT... 
		print (name)second,"^";				!--DINOSAUR...
		if(fromcommand) ActMenu.Instantiated=true;
		ActionPrimitive();				!--Now let us actually perform the action
		if(fromcommand) ActMenu.Instantiated=false;
		if(deadflag~=0) __ORInformLibrary.play_handle_death(); !---Ulp! Game Over caused by a Daemon or each_turn event. 
		return;
	];
!======================================================================================
   #endif; #ifnot; #ifndef ORConverseMenu_DONEGRAMMAR; constant ORConverseMenu_DONEGRAMMAR; #ifdef ORConverseMenu_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	#ifndef bkt; [bkt m; print (string)m;]; #endif;
	!--MenuSpeak : show conversation menu system
		verb meta "MenuSpeak" "ms" "mspk" * -> ShowConverseMenu;
		[ShowConverseMenuSub; 
			if(ConMenu.FilterTopics()==true) {
				if(noun~=-1) L__M(##Prompt); !--print the prompt only if not called from the menu
				DoConverseMenu(true);
			}
			else
				print_ret (bkt)"There are no conversation topics currently in the menu system.";
		];

	#ifdef ORLibDoc;
		!--Commands, document thyselves.
		ORMenu "MenuSpeak" ORLibDoc with text "The ~MenuSpeak~ command, or ~MS~ for short, turns on the conversation menu if there are any choices available to be made. It should be noted that the absence of a choice from the menu does not necessarily mean that the player cannot ask it manually by exiting the menu and using the traditional ASK/Tell method. In fact, this is often the method that is employed for changing the ~topic~ of conversation.^^In short, the MenuSpeak system is a method of representing a relation between topics in an easy to view manner and allows the topics to be selected from a list.^^This list can be made to display automatically with the ~AutoActionsMenu~ command.";
	#endif;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================