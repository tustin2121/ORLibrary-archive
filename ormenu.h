!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORMenu [Z, GLULX]
! Full screen multi-layered menu system.
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
! AutoDep	(GLULX):	ORRockDispensor 
!			(both);		ORCenter
!--------------------------------------------------------------------------------------
! This is a fullscreen menu help system (that works for both Z Code and GLULX).  
!
! The following is an example of how a menu system might be implemented:
!
!	ORMenu toplevel_m "Help System";
!	ORMenu -> "About this Game" with text "In this game, you play a...^^^...Feel free to send any bug 
!				reports or thoughts on the game to:^^     Jim@@64OnyxRing.com^^I hope you enjoy the 
!				game.^^^-Jim Fisher";
!	ORMenu -> "Author's Disclaimer" with text "I've never...^^^...It's only a story.";
!	ORMenu -> "Hints";
!	ORMenu ->-> "Avoiding the White Demon" with text [; print (italics)"~The white demon keeps coming 
!				into my castle and grabbing me!  How do I avoid him?~","^^You ...^^^...best spent 
!				examining the world around you.";];
!	ORMenu ->-> "Who is this Bauefred, guy?" with text [; print (italics)"~This Baufred fellow 
!				keeps...","^^...later in the game.";];
!	ORMenu -> "About NPCs" with text "Blah, blah, blah....  Put NPC Text here.";
!	ORMenu -> "Special Commands" with text "There are various commands that are atypical to 
!				other IF games, or at least are infrequent enough to warrant mentioning.  These 
!				can be divided into two groups:";
!	ORMenu ->-> "Commands specific to this game";
!	ORMenu ->->-> "Mamberflop" with text[; print "Mamberflop is a spell which...";];
!	ORMenu ->->-> "Ducalolly" with text[; print "The Ducalolly spell can...";];
!	ORMenu ->-> "Commands derived from the ORLibrary" with text "The ORLibrary offers several 
!				modules which add new verbs for the player. ";
!	ORMenu ->->-> "Name" with text "The player has the ability to ~name~ an object and then refer 
!				to it by that name with a command such as:^^     name orderly ~Bob~^^or
!				^^     refer to the knife as a sword ";
!	ORMenu ->->-> "Talk" with text "A generic addidtion to the ASK/Tell/Paradigm, the player has 
!				the ability to ~talk~ to another NPC...";
!	ORMenu -> "About the ~ORLibrary~" with text "The ORLibrary, or the ~OnyxRing~ Library is a 
!				framework of extensions to the standard library.  It contains a broad spectrum 
!				of usable entries that do everything from centering text on the screen to enabling 
!				NPCs to walk around and talk with one another.^^If you are an Inform author and are 
!				interested in finding out more about the ORLibrary framework, point your 
!				browser to: www.OnyxRing.com.";
!	ORMenu -> "About the Author" with text "Jim Fisher, a software engineer by profession for close 
!				to a decade, has programmed in a dozen or more languages including Assembler, C/C++, 
!				C#, Delphi, Java, LISP, Perl, and SQL.  He has...";
!
! Calling the menu programmatically can be accomplished with a call to the global routine 
! DoORMenu():
!
!	DoORMenu(toplevel_m);
!
! Note that if a menu object is NOT specified to DoORMenu, then the routine checks the 
! variable ORMenuContext for a menu object. Changing this variable basied upon game state
! allows context sensitive menus to be implemented.  If this variable does not define 
! an ORMenu instance then the routine attempts to "guess" and find a toplevel menu 
! object on its own. 
!
! Additional note.  In a previous version of this module, the "menu" verb was implemented to 
! automatically call DoORMenu().  This has been removed, however, since verbs of this nature 
! are really better left to the developer's descretion.
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORMenu; #include "ORMenu"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORCenter; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.01.01	Initial Creation (not really, but earliest date found without 
!				searching code archive)
! 2002.02.20	Added support for GLULX, utilizing the ORRockRequestor object for 
!				GLULX rocks, and matching cosmetics of Glulx mode with ZCode mode.
! 2002.02.21	Fixed WORDSIZE reference when clearing half the screen.
! 2002.02.26	Fit to new template
! 2002.02.28	Added code to fill own dependancy ORRockDispensor in GLULX mode.
! 2002.03.27	Added autodependancy code and commenting.
! 2002.04.03	Entirely rewrote to leverage the new ORMiniMenu class.  Additionally 
!				removed the "menu" command and the keyboard interupt.  These really 
!				belong in other modules or, in the case of the "menu" command, should 
!				be left to the discretion of the implementor.
! 2002.07.07	Added an empty variable (ORMenuCatch) for use by other modules to 
!				define and place their specific documentation.
! 2002.10.19	Added autodep for ObjectInitialise 
! 2003.03.14	Modification to adjust for offset in GLULX mode
! 2004.01.11	Removed the ORMenuCatch variable and added support for the 6.11b2 stdlib.
! 2004.01.30	Removed the autodep for stylehints (6.3 compiler supports these built in)
!--------------------------------------------------------------------------------------
!--- sample forced include---   #ifndef ORMiniMenu_DONEREPLACE; default USE_ORMiniMenu 0; message "          [ORMenu forcing inclusion of ORMiniMenu]"; #include "ORMiniMenu"; #endif; 
message "          Processing library extension ORMenu...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORMenu forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORMenu has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORCenter_DONEREPLACE; default USE_ORCenter 0; message "          [ORMenu forcing inclusion of ORCenter]"; #include "ORCenter"; #endif; 
	#ifndef ORMiniMenu_DONEREPLACE; default USE_ORMiniMenu 0; message "          [ORMenu forcing inclusion of ORMiniMenu]"; #include "ORMiniMenu"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORMenu_DONEREPLACE; constant ORMenu_DONEREPLACE; #ifdef ORMenu_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	global ORMenuContext=0;
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORMenu_DONEMESSAGE; constant ORMenu_DONEMESSAGE;  #ifdef ORMenu_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORMenu requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORMenu_DONECODE;constant ORMenu_DONECODE; #ifdef ORMenu_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
system_file;
	class ORMenu
		class ORMiniMenu
		with menupos ORMENU_TOP !--default to top of screen 
		,	text 0 !---Override this to display text.
		,	show[t;
	#ifdef TARGET_GLULX; StatusLineHeight(2); #endif;
				self.ORMiniMenu::show();
				self._erase_screen(); 
				!--lets just position halfway down the screen for grins.
				for(t=0:t<(valueorrun(self,_screen_height)/2):t++) new_line; !-- the /2 used here is correct and should NOT be replaced with WORDSIZE (divides height in half)
	#ifndef TARGET_GLULX; @erase_window -2; #endif;
				DrawStatusLine();
				DisplayStatus();
				LookSub();
			]
		,	_status_height 2
		,	_status_bar[width v;
				self._setwindow(ORMENU_STATUS);
				width=valueorrun(self,_screen_width);
				style reverse;
				#ifndef TARGET_GLULX; @set_cursor 1 1; #ifnot; glk_window_move_cursor(gg_statuswin,0,0); #endif;
				spaces width;
				#ifndef TARGET_GLULX; @set_cursor 2 1; #ifnot; glk_window_move_cursor(gg_statuswin,0,1); #endif;
				spaces width;
				
				#ifndef TARGET_GLULX; @set_cursor 1 1; #ifnot; glk_window_move_cursor(gg_statuswin,0,0); #endif;
				center(self,0,2);
				#ifndef TARGET_GLULX; @set_cursor 2 1; #ifnot; glk_window_move_cursor(gg_statuswin,0,1); #endif;
				style reverse;
				!--upper left-----
				if(self.count==0)
					print "Any: ";
				else
					print " X : ";
				if(parent(self)==0)
					print "Exit";
				else
					print "Prev";
				!--bottom right-----
				if(self.count>0){
					v=width-14;
					#ifndef TARGET_GLULX; @set_cursor 2 v; #ifnot; glk_window_move_cursor(gg_statuswin,v-1,1); #endif;
					print "Enter : Select";
				}
				style roman;
				self._setwindow(ORMENU_MAIN);
			]
		,	_erase_screen[;
				#ifndef TARGET_GLULX; 
					@erase_window -1; 
					@set_cursor 1 1; 
				#ifnot; 
					glk_window_clear(gg_mainwin); 
					glk_window_move_cursor(gg_mainwin,0,0); 
				#endif;
			]
		,	_display[;
				self.ORMiniMenu::_display();
				if(ORTopMenu.result~=0){ !--There are no selectable items in this mode, lets go ahead and handle input
					ORTopMenu.result=0;
					self._update_display();
					self._user_input();				!--handle User Input
					self._dismiss();
				}
			]
		,	_update_display[t;t=t;
				self._erase_screen();
				self._status_bar();
#ifndef TARGET_GLULX;
				if(valueorrun(self,_menupos)~=ORMENU_BOTTOM){ !--don't try to print over the menu if it is on top
					for(t=0:t<self.count:t++)
						new_line;
				}
				for(t=0:t<valueorrun(ORTopMenu,_status_height):t++) new_line;!--adjust for statusline too
#endif;
				if(self.text~=0) PrintOrRun(self,text);
				self.ORMiniMenu::_update_display();
			]
	;
	[DoORMenu menu o;
		if(menu==0) menu=ORMenuContext;
		if(menu==0) objectloop(o ofclass ORMenu && parent(o)==0) if(menu==0) menu=o;
		if(menu==0) rfalse;
		menu.show();
	];
!======================================================================================
#endif; #ifnot; #ifndef ORMenu_DONEGRAMMAR; constant ORMenu_DONEGRAMMAR; #ifdef ORMenu_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================