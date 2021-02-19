!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.09 ORMiniMenu [Z, GLULX]
! A sideline menu System (for both Z and GLULX)
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
!						ORDynaString
!--------------------------------------------------------------------------------------
! A menu system (that works for both Z Code and GLULX).  This menu does *not* clear the 
! screen so is ideal for CYOA style decisions that affect game play.  It additionally 
! allows for a parent/child relationship between menu items which enables the user to 
! traverse a tree of options to locate a selection.
!
! The following is an example of how a menu system might be implemented:
!
!	ORMiniMenu toplevel_m "Choose from the following";
!	ORMiniMenu -> "Do stuff with sticks.";
!	ORMiniMenu ->-> takesticks "Pick up sticks.";
!	ORMiniMenu ->-> burnsticks "Set sticks on fire.";
!	ORMiniMenu ->-> kicksticks "Kick sticks.";
!	ORMiniMenu -> "Look at stuff";
!	ORMiniMenu ->-> ash "Look closely at the fire ash.";
!	ORMiniMenu ->-> river "Look in the river.";
!	ORMiniMenu ->-> hole "Look into the hole.";
!	ORMiniMenu ->-> fish "Look at the fish.";
!	ORMiniMenu ->-> dog "Look around the tree.";
!
! Calling the menu programmatically can be accomplished with a call to the "show" property
! which also returns the user's selection (if any):
!
!	result=toplevel_m.show();
!
! The result is the actual menuobject.  It is determined when a menu option that has no 
! children is chosen.  The user's "choice" is also stored in the result parameter of the 
! parent menu item (that is, the menu item actually called):
!
!	print (name)toplevel_m.result;
!
! The above code will print the user's selection (provided there was a selection.  A zero 
! result indicates no user selection (the user backed out of the menu rather than make a 
! choice). Obviously the result should be checked for a zero value before attempting to 
! use it.
!
! The menupos property of the called menu object will determine the position of the menu at 
! either the top of the screen or the bottom of the screen.  The following are valid values 
! for this property:
!
!	ORMENU_BOTTOM
!	ORMENU_TOP
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORMiniMenu; #include "ORMiniMenu";	#endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORMiniMenu;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.04.03	Initial Creation (split apart from ORMenu)
! 2002.04.17	Supression of a "declared but not used" warning message for GLULX.
! 2002.07.19	Made modifications to support a function for count in derived classes.
! 2002.10.19	Added autodep for ObjectInitialise 
! 2002.11.10	Fixed intermitant redraw item error.
! 2003.03.14	Modification to adjust for offset in GLULX mode
! 2004.02.01	Removed reference to REMOVED ORStyleRvrsMonospace module.
! 2004.02.03	Added autodep for ORDynaString
! 2004.02.09	Optimized objectloop.
!--------------------------------------------------------------------------------------
!--- sample forced include---   #ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORMiniMenu forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
message "          Processing library extension ORMiniMenu...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORMiniMenu forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORMiniMenu has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
#ifdef TARGET_GLULX;
	#ifndef ORRockDispensor_DONEREPLACE; default USE_ORRockDispensor 0; message "          [ORMiniMenu forcing inclusion of ORRockDispensor]"; #include "ORRockDispensor"; #endif; 
#endif;
	#ifndef ORCenter_DONEREPLACE; default USE_ORCenter 0; message "          [ORMiniMenu forcing inclusion of ORCenter]"; #include "ORCenter"; #endif; 
	#ifndef ORDynaString_DONEREPLACE; default USE_ORDynaString 0; message "          [ORMiniMenu forcing inclusion of ORDynaString]"; #include "ORDynaString"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORMiniMenu_DONEREPLACE; constant ORMiniMenu_DONEREPLACE; #ifdef ORMiniMenu_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	#ifdef TARGET_GLULX;	
		global gg_menuwin=0;
		global gg_menuwin_rock=0;
	#endif;
	global ORTopMenu=0;
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORMiniMenu_DONEMESSAGE; constant ORMiniMenu_DONEMESSAGE;  #ifdef ORMiniMenu_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORMiniMenu requires the OREnglish file.";#endif; 
	constant ORMENU_BOTTOM	-1;
	constant ORMENU_TOP		1;
	constant ORMENU_MAIN	0;
	constant ORMENU_MENU	1;
	constant ORMENU_STATUS	2;
	#ifndef TARGET_GLULX;
		Array ORMENU_arg -->0 0;
	#endif;
	ORDynaString orminimenu_ds;
	class ORMiniMenu
		with menupos ORMENU_TOP !--default to TOPtop of screen since BOTTOM will only work for GLULX and Frotz-based interpreters (not Nitfol or Zip)
		,	pre_select ">"
		,	post_select "<"
		,	pre_norm " "
		,	post_norm " "
		,	show[;				!--Only called to begin menu session.
				ORTopMenu=self;	!--remember this entry point for the duration of this call
				self.result=0;
				DisplayStatus();
				self._display();	!--_display this menu option
				if(self.result==self) self.result=0;
				ORTopMenu._offset=0;
				ORTopMenu=0;	!--reset entry point
				self._setwindow(ORMENU_MAIN);
				return self.result;
			]
		,	_setwindow[w;!--set the current window
				#ifdef TARGET_GLULX; 
					switch(w){
						ORMENU_MAIN:
							 glk_set_window(gg_mainwin); 
						ORMENU_MENU:
							glk_set_window(gg_menuwin); 
						ORMENU_STATUS:
							glk_set_window(gg_statuswin); 

					}
				#ifnot;
					switch(w){
						ORMENU_MAIN:
							@set_window 0;
						ORMENU_MENU:
							@set_window 1;
						ORMENU_STATUS:
							@set_window 1;
					}
				#endif;
					
			]
		,	_offset 0
		,	_menupos[rv; rv=ORMENU_TOP; !--suppress warning
				if(ORTopMenu~=0)
					rv=valueorrun(ORTopMenu,menupos);
				else
					rv=ORMENU_BOTTOM;
	#ifndef TARGET_GLULX;
			if(rv~=ORMENU_BOTTOM) 
				rv=valueorrun(self,_status_height)+1;
			else
				rv=(valueorrun(ORTopMenu,_screen_height)-valueorrun(self,count))+1; 
	#ifnot;
			if(rv==ORMENU_TOP) rv=0; !--valueorrun(ORTopMenu,_status_height);
	#endif;
				return rv;
			]
		,	_screen_width[width; !--helper function to return width
				#ifdef TARGET_GLULX;
					glk_window_get_size(gg_statuswin, gg_arguments, gg_arguments+WORDSIZE);
					width = gg_arguments-->0;
				#ifnot;
					@loadb $21 0 -> width;				!first get the width of the _display
				#endif;
				return width;		
			]
		,	_status_height[;
		#ifndef TARGET_GLULX; 
				return 1;
		#ifnot; 
				return 0; !--in GLULX mode, the status line appears in another window...
		#endif;
			]
		,	_screen_height[height;
				#ifdef TARGET_GLULX;
					glk_window_get_size(gg_mainwin, gg_arguments, gg_arguments+WORDSIZE);
					height = gg_arguments-->1;
				#ifnot;
					@loadb $20 0 -> height;
				#endif;
				return height;
			]
		,	_update_display[o;
	#ifdef TARGET_GLULX;
				if(gg_menuwin~=0){
					glk_window_close(gg_menuwin,gg_arguments);
					gg_menuwin=0;
				}
				if(valueorrun(self,count)~=0){
					if(valueorrun(self,_menupos)==ORMENU_BOTTOM)
						gg_menuwin=glk_window_open(gg_mainwin,winmethod_Below+winmethod_Fixed,valueorrun(self,count),wintype_TextGrid,gg_menuwin_rock);
					else
						gg_menuwin=glk_window_open(gg_mainwin,winmethod_Above+winmethod_Fixed,valueorrun(self,count),wintype_TextGrid,gg_menuwin_rock);
				}
	#endif;
				if(valueorrun(self,count)==0) return;
				objectloop(o in self) if(o ofclass ORMiniMenu) self.DrawItem(o);
				if(self.cur_sel<valueorrun(self,count)) 
					self._select(self.cur_sel);
				else
					self._select(0);
			]
		,	_display[t newadj; t=newadj;  !--supress warning
				if(self._init()==0) {
					ORTopMenu.result=self;
					return; !--setup this menu item for display (exiting if nothing to display)
				}
	#ifndef TARGET_GLULX;
				self._setwindow(ORMENU_MAIN);
				if(valueorrun(ORTopMenu,menupos)==ORMENU_BOTTOM && valueorrun(self,count)>ORTopMenu._offset){
					if(ORTopMenu._offset==0){
						for(t=0:t<valueorrun(self,count)-1:t++) !-- one less than count to allow for cr/lf generated by interpreters
							new_line; 
						ORTopMenu._offset=valueorrun(self,count);
					}
					if(ORTopMenu._offset~=0){
						for(t=(ORTopMenu._offset+ORTopMenu._adjustment):t<valueorrun(self,count):t++) new_line; 
						@get_cursor ORMENU_arg;
						t=(ORTopMenu._screen_height()-ORMENU_arg-->0)-valueorrun(self,_status_height);
						newadj=(valueorrun(self,count)-ORTopMenu._offset)-t-1; !--calculate the new remmember number of lines added to screen height this scroll
						if(newadj>0) {
							ORTopMenu._adjustment=newadj;
						}
					}
				}
				self._setwindow(ORMENU_MAIN);
				@split_window 0;
				self._setwindow(ORMENU_MENU);
	#ifnot;
				if(self.menupos~=ORMENU_BOTTOM) self.menupos=0;
	#endif;
				self._update_display();
				self._user_input();				!--handle User Input
				self._dismiss();
				self._adjustment=0;
				return;
			]
		,	_init[o;
				self.count=0;						!-- 1)determine items in list to _display
				objectloop(o in self) {
					if(o ofclass ORMiniMenu){
						o.number=self.count;
						self.count++;
					}
				}
				return self.count;
			]
		,	_dismiss[o;
				self._clear();
				objectloop(o in self) if(o ofclass ORMiniMenu)o.number=0; !--4) Clear list
				self._setwindow(ORMENU_MAIN);
	#ifdef TARGET_GLULX;
				if(gg_menuwin~=0){
					glk_window_close(gg_menuwin,gg_arguments);
					gg_menuwin=0;
				}
	#endif;
		]
		,	_select[num obj;
				obj=self.getitem(self.cur_sel);
				if(metaclass(obj)==object) self.DrawItem(obj);
				self.cur_sel=num;
				obj=self.getitem(self.cur_sel);
				self.DrawItem(obj,true);
			]
			!---translate a number into the actual menu object 
		,	GetItem[num o obj;
				objectloop(o in self) if(o ofclass ORMiniMenu && o.number==num) obj=o;
				return obj;
			]
			!---actually write out the name of the given object
		,	DrawItem[obj hilight pos;
				if(obj==0) return;
				pos=valueorrun(self,_menupos);
	#ifdef TARGET_GLULX;
				!--if(pos==ORMENU_BOTTOM) 
				!--	pos=0;
				!--else
				!--	pos=valueorrun(ORTopMenu,_status_height);
				pos=0;
	#endif;
				pos=obj.number+pos;
				self._setwindow(ORMENU_MENU);
				#ifndef TARGET_GLULX; pos=pos+ORTopMenu._adjustment; @set_cursor pos 1; #ifnot; glk_window_move_cursor(gg_menuwin,0,pos); #endif;
				spaces valueorrun(self,_screen_width);
				#ifndef TARGET_GLULX; @set_cursor pos 1; #ifnot; glk_window_move_cursor(gg_menuwin,0,pos); #endif;
				orminimenu_ds.capture();
				if(hilight>0)
					print (string)self.pre_select;
				else
					print (string)self.pre_norm;
				print (name)obj;
				if(hilight>0)
					print (string)self.post_select;
				else
					print (string)self.post_norm;
				orminimenu_ds.release();
				Center(orminimenu_ds,0,hilight);
			]
		,	_clear[o pos;!--clear the exiting lines
				self._setwindow(ORMENU_MENU);
				objectloop(o in self) {
					if(o ofclass ORMiniMenu){
						pos=valueorrun(self,_menupos);
	#ifdef TARGET_GLULX;
						if(pos==ORMENU_BOTTOM) pos=0; 
	#endif;
						pos=o.number+pos;
	#ifndef TARGET_GLULX; 
						pos=pos+ORTopMenu._adjustment; 
						@set_cursor pos 1; 
	#ifnot;
						glk_window_move_cursor(gg_menuwin,0,pos); 
	#endif;
						spaces valueorrun(self,_screen_width);
					}
				}
			]
			!---handle user input
		,	_handle_selection[ch i;
				if(valueorrun(self,count)==0) rtrue;
				switch(ch){
					'p','P',129, -4:if(self.cur_sel-1>=0 && valueorrun(self,count)>0) self._select(self.cur_sel-1);
					'n','N',130, -5:if(self.cur_sel+1<valueorrun(self,count) && valueorrun(self,count)>0) self._select(self.cur_sel+1);
					' ',10,13, -6 :if(valueorrun(self,count)>0){
										self._clear();!--hide the current menu screen
										i=self.GetItem(self.cur_sel);
										if(i provides _display) 
											i._display();  !--pass off control to selected item
										else{
											ORTopMenu.result=i;
											rtrue;
										}
										if(ORTopMenu.result~=0) rtrue;
										self._update_display(); !--when returns, re_display this one again
									}
					'`','~','q','Q',133,'x','X': 
						ORTopMenu.result=0;		
						rtrue;
				}
				rfalse;
			]
		,	_user_input[ch win; win=win;
	#ifndef TARGET_GLULX; 
				while(true){
					@read_char 1 -> ch;
					if(valueorrun(self,count)==0) return;
					if(self._handle_selection(ch)==true) return;
				}
	#ifnot;
				ch=false;
				while(ch==false){
					win=gg_menuwin; !--recalculate window, because can be different if selection was chosen, and then exited from
					if(win==0)win=gg_mainwin;
					glk_request_char_event(win);
					glk_select(gg_event); ! select
					if(gg_event-->0==evtype_CharInput) ch=self._handle_selection(gg_event-->2);
					HandleGlkEvent(gg_event, 1);
				}
	#endif;
			]
		,	_adjustment 0
		,	result  0 !--result of a selected item
		,	number	0 !--the index for children objects
		,	count	0 !--total number of child ORMiniMenu objects
		,	cur_sel 0 !--the current selection for the running object
	;
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORMiniMenu_DONECODE;constant ORMiniMenu_DONECODE; #ifdef ORMiniMenu_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	#ifdef TARGET_GLULX;
		object _gmm LibraryExtensions with ext_initialise[;gg_menuwin_rock=ORRockDispensor.get_window();];
	#endif;
!======================================================================================
#endif; #ifnot; #ifndef ORMiniMenu_DONEGRAMMAR; constant ORMiniMenu_DONEGRAMMAR; #ifdef ORMiniMenu_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================