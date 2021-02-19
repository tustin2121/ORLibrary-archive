!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.04.28 ORActionMenu
! Adds the ability to list possible player actions in a menu.  Although reminiscent of 
! the CYOA format (it can certainly be leveraged to make stories of that type), this is 
! actually an alternate front end to the parser (as opposed to the traditional 
! keyboard input prompt).
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
! To use, ORMenuAction objects (which encode actions) can be added to the ActMenu object
! along with a description:
!
!	ActMenu.add(ORMenuAction.create(##Examine, player),"Take a look at yourself");
!
! The description parameter is optional.  If not provided, then an attempt will be made to 
! generate one. The above example, minus the description, prints as:
!
!	Examine yourself
!
! Use of Inform's pseudo-dynamic arrays makes creating potential actions mostly painless 
! and enables reuse, but it is not necessary to instantiate ORMenuAction in this way.  
! ORMenuAction objects can also be defined in the traditional manner:
!
!		ORMenuAction OMA_PutWallnutOnTable
!		,	_action ##PutOn
!		,	_noun wallnet
!		,	_second table;
!
!		...
!		
!		ActMenu.add(OMA_PutWallnutOnTable);
!
! Additionally, the size of the pre-defined array of ORMenuActions (defaulted to five) can 
! be changed by defining the ACTION_BUFFER constant before the inclusion of VERBLIB.
!--------------------------------------------------------------------------------------
! AutoDep:	ORInformLibrary
!			OROptionListMenu
!			ORKeyboardPrimitive
!			ORKeyboard
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORActionMenu; #include "ORActionMenu"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORActionMenu; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2004.02.01	Initial Creation
! 2004.02.03	Minor code modifications for performer resolution; also added code
!				to allow for thirty possible menu items (expecting most to be filtered).
!				Created a default can_show routine and added a filter properties
!				to house new SCOPE_FILTER_ constants.
! 2004.02.12	Moved some of the create parameters of the ORAction class into the add 
!				routine to avoid maximum number of parameters allowed in create method.
! 2004.02.15	Revised all references to the ORAction class to be ORMenuAction.
!				ORAction is now a base class from which ORMenuAction derives.  Code
!				in ORAction will be used in other modules.
!				Also threw away all of the SCOPE_FILTER rules, using instead the 
!				generic code in ORAction's CanAttempt routine to these rules. 
! 2004.02.28	Minor modifications to add compatibility with recent ORKeyboard and 
!				ORKeyboardPrimitive modifications. Also added code to utilize ORAction's
!				new Execute() property.
! 2004.03.06	Minor modification to self documentation.
! 2004.04.28	Added support for Meta verbs
!--------------------------------------------------------------------------------------
message "          Processing library extension ORActionMenu...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORActionMenu forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORActionMenu has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORInformLibrary_DONEREPLACE; default USE_ORInformLibrary 0; message "          [ORActionMenu forcing inclusion of ORInformLibrary]"; #include "ORInformLibrary"; #endif; 
	#ifndef OROptionListMenu_DONEREPLACE; default USE_OROptionListMenu 0; message "          [ORActionMenu forcing inclusion of OROptionListMenu]"; #include "OROptionListMenu"; #endif; 
	#ifndef ORKeyboardPrimitive_DONEREPLACE; default USE_ORKeyboardPrimitive 0; message "          [ORActionMenu forcing inclusion of ORKeyboardPrimitive]"; #include "ORKeyboardPrimitive"; #endif; 
	#ifndef ORKeyboard_DONEREPLACE; default USE_ORKeyboard 0; message "          [ORActionMenu forcing inclusion of ORKeyboard]"; #include "ORKeyboard"; #endif; 
	#ifndef ORAction_DONEREPLACE; default USE_ORAction 0; message "          [ORActionMenu forcing inclusion of ORAction]"; #include "ORAction"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORActionMenu_DONEREPLACE; constant ORActionMenu_DONEREPLACE; #ifdef ORActionMenu_DONEREPLACE;#endif;!--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
!	replace KeyboardGetInput; !--replace the ORLibrary-defined componant to Keyboard
!	replace KeyboardPrepInput; !--add code to exit on game over
!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORActionMenu_DONEMESSAGE; constant ORActionMenu_DONEMESSAGE; #ifdef ORActionMenu_DONEMESSAGE;#endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
! #ifndef OREnglish; message fatalerror "ERROR!!!! ORActionMenu requires the OREnglish file.";#endif; 
	!--class used to create actions to display
	default ACTION_BUFFER 5;
	class ORMenuAction(ACTION_BUFFER)
		class ORAction
		with create[a b c;
				self.ORAction::create(a,b,c);
				self.HasBeenSelected=false;
				self.RemoveAfterSelect=false;
				self.silent=false;
				self.autorunifonly=false;
				self.selected_text=self.ORMenuAction::selected_text;
				self.menu_display=self.ORMenuAction::menu_display;
    		]
		,	HasBeenSelected false
		,	RemoveAfterSelect false
		,	silent false		!--true:hide the text at the prompt
		,	autorunifonly false !--true:if this is the only option, then automatically select without displaying the menu
		,	selected_text[;PrintOrRun(self,format_command,true);]
		,	menu_display[;PrintOrRun(self,selected_text,true);]
	;

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORActionMenu_DONECODE;constant ORActionMenu_DONECODE; #ifdef ORActionMenu_DONECODE; #endif;!--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	global AutoShowActionMenu=false;
	!--Define the OptionListMenu which will actually serve as our menu.  
	!--Make this menu install the wedge as well. 
	OROptionListMenu ActMenu LibraryExtensions 
		with source _actionoptions
		,	AllowExit true
		,	showcount 4
		,	add[actn seltxt des ras;
				if(~~(actn ofclass ORMenuAction)) {
	#ifdef DEBUG; 
					print"ERROR: Attempting to add a NON-ORMenuAction object to Actmenu.";
	#endif;				
					return;
				}
				if(self.contains(actn)==true) return; !--don't add the same item twice
				if(des) actn.menu_display=des;
				if(seltxt) actn.selected_text=seltxt;
				actn.RemoveAfterSelect=ras;
				_actionoptions.PushOption(actn);
			]
		,	contains[obj o t;
				o=self.source;
				for(t=0:t<o.count():t++){
					if(o.GetOption(t)==obj) rtrue;
				}
				rfalse;
			]
		,	clear[t o op;
				o=self.source;
				for(t=0:t<o.count():t++){
					op=o.GetOption(t);
					if(op~=0){
						o.RemoveOption(op);
						if(op provides destroy) op.destroy();
						t--;
					}
				}
			]
		,	Instantiated false 
		,	directions "----------Press X to exit----------"
		,	ext_initialise[; !--actually install the wedge...
				ORKBPreempt.add_routine(self.ORDoActionMenuInstead); 
			]
		,	ORDoActionMenuInstead[; !--the wedge itself...
				return DoActionMenu(false); !--wedge does nothing but call another routine (which possibly displays the menu or not)
			]
		,	FilterActions[a i o;
				a=ActMenu.source; !--just for ease of referencing below
				if(a==0)rfalse;
				PreFilterActionMenu();
				LibraryExtensions.RunAll(ext_PreFilterActionMenu);
				for(i=0:i<a.count():i++){ !--cycle through all actions...
					o=a.GetOption(i); !--get the action
					if(valueorrun(o,CanAttempt)==true)  give o ~workflag; 
					else give o workflag; 
				}
				if(ActMenu.count()>0) rtrue; 
				rfalse;
			]
	;
	OROptionList _actionoptions !--options that the menu will display
		with OptionList			0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		,	AssociationList		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		,	AssociationList2	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		,	ext_PreFilterActionMenu 0
	; 

	global _menu_last_shown=START_MOVE-1;
	!--The routine called by the installed wedge; It is also called by the MenuAction command.  It displays the menu, and issues the command if one was chosen
	[DoActionMenu fromcommand canshow o a canexit retval; !--actually show the menu and perform the action if selected...pass in true to force a menu regardless of the state of the AutoShowActionMenu variable
		if(fromcommand==false && _menu_last_shown==turns) rfalse; !--don't do this again, in the same turn it was canceled from, make them restart the menu, or do something else
		_menu_last_shown=turns;
		if(deadflag~=0) return -1; !--don't bother, we're already dead
		!--------------------------------------------
		!--first, massage the data in the pool...
	.domenuloc;
		canshow=ActMenu.FilterActions(); 
		canexit=ActMenu.AllowExit;
		if(fromcommand==true || canshow==false) canexit=true; !--allow an exit regardless of allowexit value if this menu was initiated from the prompt or there's simply nothing to show

		a=ActMenu.source; !--just for ease of referencing below
		!--show the menu if supposed to...
		o=0; !--this will be the selected menu option 
		if((AutoShowActionMenu==true || fromcommand==true) &&  a ofclass OROptionList && canshow==true) {
			if(a.count()==1 && valueorrun(a.GetOption(0),autorunifonly)==true){
				o=a.GetOption(0);
			}else{
				o=ActMenu.show();
				if(o~=0 && ActMenu.showcount>0) {
					ActMenu.showcount=ActMenu.showcount-1 ;
					if(ActMenu.showcount==0) ActMenu.directions=0; !--turn off the directions after four showings
				}
			}
		}
		!--we actually had no result but require one? 
		if(o==0 && canexit==false) jump domenuloc; !--then retry the menu
		
		DrawStatusLine();
		DisplayStatus();
		if(o==0) {  !--if no action was selected then lets just exit out, returning false to indicate input should continue as normal
			if(fromcommand==true) print "[Menu Aborted]";
			rfalse; 
		}
		!--otherwise, an action WAS selected from the menu. SO lets do that... 
		if(valueorrun(o,silent)==false){ !--print the text if needed
			printorrun(o,selected_text);
			new_line;
		}
		ActMenu.Instantiated=true;
		retval=o.execute();
		ActMenu.Instantiated=false;

		if(retval==true) rfalse; 

		
		o.HasBeenSelected=true; !--just so that we know if we have already shown this before
		if(valueorrun(o,RemoveAfterSelect)==true) a.RemoveOption(o);
		
		if(valueorrun(o,ismeta)==false) InformLibrary.end_turn_sequence(); !--and do all the normal stuff that follows an action
		DrawStatusLine();
		DisplayStatus();

		if(deadflag==0) return -1;	
		
		!---Ulp! Game Over caused by a Daemon or each_turn event. 
		__ORInformLibrary.play_handle_death();
		return -1;
	];
!======================================================================================
   #endif; #ifnot; #ifndef ORActionMenu_DONEGRAMMAR; constant ORActionMenu_DONEGRAMMAR; #ifdef ORActionMenu_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	system_file;
	#ifndef bkt; [bkt m; print (string)m;]; #endif;
		verb meta "AutoActionsMenu" "aam" "autoactm"
			* -> AutoAError
			* 'on' -> AutoAOn
			* 'off' -> AutoAOff
		;
		[AutoAErrorSub;
			print "This command turns on or off automatic display of the ActionsMenu.  The syntax for this is:^^     AutoActionsMenu|AAM";
			spaces(5);
			print_ret "On|Off^^For example:^^     >AAM OFF^^or^^     >AUTOACTIONSMENU ON";
		];
		[AutoAOffSub;
			if(AutoShowActionMenu==false) print_ret (bkt)"Automatic display of the ActionsMenu system is already turned off.";
			AutoShowActionMenu=false;
			print_ret (bkt)"Automatic display of the ActionsMenu system is now turned off.";
		];
		[AutoAOnSub;
			if(AutoShowActionMenu==true) print_ret (bkt)"Automatic display of the ActionsMenu system is already turned on.";
			AutoShowActionMenu=true;
			print_ret (bkt)"Automatic display of the ActionsMenu system is now turned on.";
		];
	!--MenuSpeak : show conversation menu system
		verb meta "ActionsMenu" "ActMenu" "am" * -> ShowActionsMenu;
		[ShowActionsMenuSub; 
			if(ActMenu.FilterActions()==true) {
				L__M(##Prompt);
				DoActionMenu(true);
			}else
				print_ret (bkt)"There are no actions currently in the menu system.";
		];
	stub PreFilterActionMenu 0;
	#ifdef ORLibDoc;
	!--Commands, document thyselves.
	ORMenu _oramdoc "The ActionsMenu System" ORLibDoc with text "The ActionsMenu system allows the player to select possible actions from a list. Rather than a simple CYOA implementation, the ActionsMenu system is an alternate input interface that avoids keyboard input and parsing steps. Two commands are available to the user: ~ActionsMenu~ and ~AutoActionsMenu.~";
		ORMenu "ActionsMenu" _oramdoc with text "The ~ActionsMenu~ command, or ~AM~ for short, turns on the actions menu if there are any choices available to be made. ^^This list can be made to display automatically with the ~AutoActionsMenu~ command.";
		ORMenu "AutoActionsMenu" _oramdoc with text "The ~AutoActionsMenu~ command, or ~AAM~ for short, allows the player to choose whether or not the menu will appear automatically when options are available. The syntax to turn this mode on or off is simply: ^^AAM ON^^or^^AAM OFF";
	#endif;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================+