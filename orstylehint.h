!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.03.03 ORStyleHint [GLULX]
! An object wrapper to easily establish user-defined styles.
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
! AutoDep (Glulx-only) :	ORInfGlk
!--------------------------------------------------------------------------------------
! Without this module, in order to produce a style, for example [reverse, italic, centered, bold], 
! that would appear in both the status window and main window, you had to declare the 
! InitGlkWindow() routine and define it as follows:
!
!	[ InitGlkWindow winrock;
!		switch(winrock){
!			GG_MAINWIN_ROCK:
!				glk_stylehint_set(wintype_TextBuffer,Style_User1, stylehint_ReverseColor, 1);
!				glk_stylehint_set(wintype_TextBuffer,Style_User1, stylehint_Weight, 1);
!				glk_stylehint_set(wintype_TextBuffer,Style_User1, stylehint_Justification, stylehint_just_Centered);
!				glk_stylehint_set(wintype_TextBuffer,Style_User1, stylehint_Oblique, 1);
!			GG_STATUSWIN_ROCK:
!				glk_stylehint_set(wintype_TextGrid,Style_User1, stylehint_ReverseColor, 1);
!				glk_stylehint_set(wintype_TextGrid,Style_User1, stylehint_Weight, 1);
!				glk_stylehint_set(wintype_TextGrid,Style_User1, stylehint_Justification, stylehint_just_Centered);
!				glk_stylehint_set(wintype_TextGrid,Style_User1, stylehint_Oblique, 1);
!		}
!		rfalse;
!	];
!
! Note that this is just 1 style in two windows; double this for an additional style.  
! With the ORStyleHint module, the following will do the same and it can be defined 
! anywhere in your source code:
!
!	ORStyleHint	with windows wintype_TextGrid wintype_TextBuffer
!		,	hints stylehint_ReverseColor stylehint_Weight stylehint_Justification stylehint_Oblique
!		,	values 1 1 stylehint_just_Centered 1;
! 
! Also, the InitGlkWindow() routine need not be defined.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORStyleHint";					!Constant USE_ORStyleHint; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORStyleHint; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.11	Initial Creation
! 2002.02.26	Fit to new template.
! 2004.01.14	Modified to support new 6/11 stdlib.
! 2004.01.17	Fixed bug, apparently introduced in previous change, making module
!				comple even in Z-code mode.
! 2004.02.05	Aditional changes to better reflect stdlib enhancments.
! 2004.03.03	Removed references to the ExtensionRunRoutines... routines.
!--------------------------------------------------------------------------------------
	#ifndef TARGET_GLULX;
		message "          Processing library extension ORStyleHint...Ignoring (GLULX specific)";
	#ifnot;
		message "          Processing library extension ORStyleHint...";
	#endif;
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORStyleHint forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifdef TARGET_GLULX;
		#ifndef ORLibraryInclude; message fatalerror "ORStyleHint has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
		#ifndef ORInfGlk_DONEREPLACE; default USE_ORInfGlk 0; message "          [ORStyleHint forcing inclusion of ORInfGlk]"; #include "ORInfGlk"; #endif; 
	#endif;
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORStyleHint_DONEREPLACE; constant ORStyleHint_DONEREPLACE; #ifdef ORStyleHint_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	#ifdef TARGET_GLULX;
		replace GGInitialise;
		replace Box__Routine;
	#endif;
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORStyleHint_DONEMESSAGE; constant ORStyleHint_DONEMESSAGE;  #ifdef ORStyleHint_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORStyleHint requires the OREnglish file.";#endif; 
	#ifdef TARGET_GLULX;
		class ORStyleHint
				with style_initialise[rock o i; !--redundant calls.  Would be better to get type of window associated with "rock" and check type against the windows property...  For now, this doesn't do anything but waste a few cpu cycles on initialization.
					rock=rock; !--quash compiler warnings
					for(o=0:o<self.#windows/WORDSIZE:o++){
						for(i=0:i<self.#hints/WORDSIZE:i++){
							glk_stylehint_set(self.&windows-->o,self.user_num,self.&hints-->i,self.&values-->i);
						}
					}
				]
			,	user_num style_User1 !--can be changed
			,	windows  wintype_TextBuffer
			,	hints	stylehint_Weight
			,	values	0
		;
	#endif;
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORStyleHint_DONECODE;constant ORStyleHint_DONECODE; #ifdef ORStyleHint_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	#ifdef TARGET_GLULX;
		object orsh_init LibraryExtensions
		with ext_initglkwindow[rock obj;
			objectloop(obj ofclass(ORStyleHint)){
	#ifdef DEBUG;
				if(~~(obj provides style_initialise)) {
					print "^^^^WARNING: ORStyleHint ~",(name)obj,"~ does not provide ~style_initialise~ property.";
					continue;
				}
	#endif;			
				obj.style_initialise(rock);
			}
		]
	;

	[ Box__Routine maxwid arr ix lines lastnl parwin temp;
		maxwid = 0; ! squash compiler warning
		lines = arr-->0;

		if (gg_quotewin == 0) {
			gg_arguments-->0 = lines;
			temp = LibraryExtensions.RunAll(ext_initglkwindow, GG_QUOTEWIN_ROCK);
			ix = InitGlkWindow(GG_QUOTEWIN_ROCK);
			if(temp<ix)ix=temp;
			if (ix == 0)
				gg_quotewin = glk($0023, gg_mainwin, $12, lines, 3,
					GG_QUOTEWIN_ROCK); ! window_open
		}
		else {
			parwin = glk($0029, gg_quotewin); ! window_get_parent
			glk($0026, parwin, $12, lines, 0); ! window_set_arrangement
		}

		lastnl = true;
		if (gg_quotewin) {
			glk($002A, gg_quotewin); ! window_clear
			glk($002F, gg_quotewin); ! set_window
			lastnl = false;
		}

		! If gg_quotewin is zero here, the quote just appears in the story window.

		glk($0086, 7); ! set blockquote style
		for (ix=0 : ix<lines : ix++) {
			print (string) arr-->(ix+1);
			if (ix < lines-1 || lastnl) new_line;
		}
		glk($0086, 0); ! set normal style

		if (gg_quotewin) {
			glk($002F, gg_mainwin); ! set_window
		}
	];

	[ GGInitialise res temp;
		@gestalt 4 2 res; ! Test if this interpreter has Glk.
		if (res == 0) {
		! Without Glk, we're entirely screwed.
		quit;
		}
		! Set the VM's I/O system to be Glk.
		@setiosys 2 0;

		! First, we must go through all the Glk objects that exist, and see
		! if we created any of them. One might think this strange, since the
		! program has just started running, but remember that the player might
		! have just typed "restart".

		GGRecoverObjects();

		temp = LibraryExtensions.RunAll(ext_initglkwindow, 0);
		res = InitGlkWindow(0);
		if(temp<res)res=temp;
	    
		if (res ~= 0) return;

		! Now, gg_mainwin and gg_storywin might already be set. If not, set them.

		if (gg_mainwin == 0) {
			! Open the story window.

			temp = LibraryExtensions.RunAll(ext_initglkwindow, GG_MAINWIN_ROCK);
			res = InitGlkWindow(GG_MAINWIN_ROCK);
			if(temp<res)res=temp;
			
			if (res == 0)
				gg_mainwin = glk($0023, 0, 0, 0, 3, GG_MAINWIN_ROCK); ! window_open
			if (gg_mainwin == 0) {
				! If we can't even open one window, there's no point in going on.
				quit;
			}
		}
		else {
			! There was already a story window. We should erase it.
			glk($002A, gg_mainwin); ! window_clear
		}

		if (gg_statuswin == 0) {
			temp = LibraryExtensions.RunAll(ext_initglkwindow, GG_STATUSWIN_ROCK);
			res = InitGlkWindow(GG_STATUSWIN_ROCK);
			if(temp<res)res=temp;

			if (res == 0) {
				gg_statuswin_cursize = gg_statuswin_size;
				gg_statuswin = glk($0023, gg_mainwin, $12, gg_statuswin_cursize,
					4, GG_STATUSWIN_ROCK); ! window_open
			}
		}
		! It's possible that the status window couldn't be opened, in which case
		! gg_statuswin is now zero. We must allow for that later on.

		glk($002F, gg_mainwin); ! set_window

			temp = LibraryExtensions.RunAll(ext_initglkwindow, 1);

		InitGlkWindow(1);
	];
	#endif;
!======================================================================================
#endif; #ifnot; #ifndef ORStyleHint_DONEGRAMMAR; constant ORStyleHint_DONEGRAMMAR; #ifdef ORStyleHint_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================