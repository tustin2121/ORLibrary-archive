!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 OROptionListMenu [Z, GLULX]
! Defines an object which reads the current OptionList of an object that inherits from 
! OROptionList and displays an appropriate menu. Each object should 
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
!
!--------------------------------------------------------------------------------------
! AutoDep:	ORRockDispensor
!			ORCenter
!			ORMiniMenu
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_OROptionListMenu; #include "OROptionListMenu"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_OROptionListMenu; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.04.03	Initial Creation
! 2002.04.21	Added code to support options directions (by defining the Directions 
!				property with a value to be displayed)
! 2002.04.22	Made directions work in GLULX mode.
! 2002.04.24	Fixed clear problem with incorrect line number with using directions.
!				Also added OROptionList autodep code.
! 2002.05.01	Modified comments.
! 2002.07.14	Modified to allow menu options to *not* be included if they have the 
!				workflag attribute. This enables filtering of the menu prior to showing.
! 2004.02.01	Added support for source property to be a routine.
!--------------------------------------------------------------------------------------
message "          Processing library extension OROptionListMenu...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [OROptionListMenu forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "OROptionListMenu has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
#ifdef TARGET_GLULX;
	#ifndef ORRockDispensor_DONEREPLACE; default USE_ORRockDispensor 0; message "          [OROptionListMenu forcing inclusion of ORRockDispensor]"; #include "ORRockDispensor"; #endif; 
#endif;
	#ifndef ORCenter_DONEREPLACE; default USE_ORCenter 0; message "          [OROptionListMenu forcing inclusion of ORCenter]"; #include "ORCenter"; #endif; 
	#ifndef ORMiniMenu_DONEREPLACE; default USE_ORMiniMenu 0; message "          [OROptionListMenu forcing inclusion of ORMiniMenu]"; #include "ORMiniMenu"; #endif; 
	#ifndef OROptionList_DONEREPLACE; default USE_OROptionList 0; message "          [OROptionListMenu forcing inclusion of OROptionList]"; #include "OROptionList"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef OROptionListMenu_DONEREPLACE; constant OROptionListMenu_DONEREPLACE; #ifdef OROptionListMenu_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef OROptionListMenu_DONEMESSAGE; constant OROptionListMenu_DONEMESSAGE;  #ifdef OROptionListMenu_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! OROptionListMenu requires the OREnglish file.";#endif; 
	class OROptionListMenu
		class ORMiniMenu
		with source 0
		,	count[src t c o;
				src=ValueOrRun(self,source);
				if(src==0 || (src ofclass OROptionList)==false) return 0;
				for(t=0:t<src.count():t++){
					o=src.GetOption(t);
					if(o hasnt workflag) c++;
				}
				return c;
			]
		,	_init[;
				return self.count();
			]
		,	_update_display[t src o pos c; c=c; !--quash warning
				src=ValueOrRun(self,source);
#ifdef TARGET_GLULX;
				if(gg_menuwin~=0){
					glk_window_close(gg_menuwin,gg_arguments);
					gg_menuwin=0;
				}
				if(self.count()~=0){
					c=self.count();
					if(self.directions~=0) c++;
					if(valueorrun(self,_menupos)==ORMENU_BOTTOM)
						gg_menuwin=glk_window_open(gg_mainwin,winmethod_Below+winmethod_Fixed,c,wintype_TextGrid,gg_menuwin_rock);
					else
						gg_menuwin=glk_window_open(gg_mainwin,winmethod_Above+winmethod_Fixed,c,wintype_TextGrid,gg_menuwin_rock);
				}
#endif;
				if(self.count()==0) return;
				DrawStatusLine();
				DisplayStatus();
				if(self.Directions~=0) 	{
					self._setwindow(ORMENU_MENU);
				#ifndef TARGET_GLULX; 
					pos=(valueorrun(self,_status_height))+ORTopMenu._adjustment; 
					if(self.directions~=0) pos++; 
				#endif;
				#ifndef TARGET_GLULX;
					@set_cursor pos 1; 
				#ifnot; 
					glk_window_move_cursor(gg_menuwin,0,pos); 
				#endif;
					spaces valueorrun(self,_screen_width);
					#ifndef TARGET_GLULX; @set_cursor pos 1; #ifnot; glk_window_move_cursor(gg_menuwin,0,pos); #endif;
					Center(self.Directions,0,true);
				}
				for(t=0:t<self.count():t++){
					o=src.GetOption(self.TranslatePositionToAbsolute(t));
					self.DrawItem(o);
				}
				if(self.cur_sel<self.count()) 
					self._select(self.cur_sel);
				else
					self._select(0); 

			]
		,	GetItem[num;
				num=self.TranslatePositionToAbsolute(num);
				return ValueOrRun(self,source).GetOption(num);
			]
		,	Directions	0
		,	_status_height 1
		,	TranslatePositionToRelative[number t c src;
				src=ValueOrRun(self,source);
				for(t=0:t<number:t++) if((src.&OptionList-->t) hasnt workflag) c++;
				if((src.&OptionList-->t) hasnt workflag) return c;
				return -1;
			]
		,	TranslatePositionToAbsolute[number t c src;
				src=ValueOrRun(self,source);
				for(t=0:t<src.count():t++){
					if((src.&OptionList-->t) hasnt workflag) c++;
					if(number<c) return t;
				}
				return -1;
			]
		,	DrawItem[obj hilight pos prop src number x;
				if(obj==0) return;
				src=ValueOrRun(self,source);
				number=src.LocateOption(obj);
				prop=menu_display;
				if(src.GetAssociation(number)<0 && obj provides menu_display_alt) prop=menu_display_alt;
				self._setwindow(ORMENU_MENU);
				pos=valueorrun(self,_menupos);
#ifdef TARGET_GLULX;
				if(pos==ORMENU_BOTTOM) pos=0;
#endif;
				number=self.TranslatePositionToRelative(number); !--adjust for all hidden menu items
				pos=number+pos;

				if(self.Directions~=0) pos++;

				#ifndef TARGET_GLULX; pos=pos+ORTopMenu._adjustment; @set_cursor pos 1; #ifnot; glk_window_move_cursor(gg_menuwin,0,pos); #endif;
				spaces valueorrun(self,_screen_width);
				#ifndef TARGET_GLULX; @set_cursor pos 1; #ifnot; glk_window_move_cursor(gg_menuwin,0,pos); #endif;

				orminimenu_ds.capture();
				if(hilight>0)
					print (string)self.pre_select;
				else
					print (string)self.pre_norm;
				if(obj provides prop)
					printorrun(obj,prop,true);
				else
					print(name)obj; !--just use the name then

				x=src.GetAssociation2(number);
				if(x~=0){
					print " [";
					switch(metaclass(x)){
						object: print (name)x;
						string: print (string)x;
						default: print "ERROR";
					}
					print "]";
				}
				if(hilight>0)
					print (string)self.post_select;
				else
					print (string)self.post_norm;
				orminimenu_ds.release();
				Center(orminimenu_ds,0,hilight);
			]
		,	_clear[pos tpos t c;!--clear the exiting lines
					self._setwindow(ORMENU_MENU);
					tpos=valueorrun(self,_menupos);
	#ifdef TARGET_GLULX;
					if(tpos==ORMENU_BOTTOM) tpos=0;
	#endif;
					c=self.count();
					if(self.directions~=0)c++;
					for(t=0:t<c:t++){
						pos=t+tpos;
						#ifndef TARGET_GLULX; pos=pos+ORTopMenu._adjustment; @set_cursor pos 1; #ifnot;glk_window_move_cursor(gg_menuwin,0,pos); #endif;
						spaces valueorrun(self,_screen_width);
					}
				]
		,	menu_display		!--to be defined by the objects contained in the list (how it will be displayed)
		,	menu_display_alt	!--to be defined by the objects contained in the list (how it will be displayed if alternate value is selected)
	;
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef OROptionListMenu_DONECODE;constant OROptionListMenu_DONECODE; #ifdef OROptionListMenu_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #ifnot; #ifndef OROptionListMenu_DONEGRAMMAR; constant OROptionListMenu_DONEGRAMMAR; #ifdef OROptionListMenu_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================