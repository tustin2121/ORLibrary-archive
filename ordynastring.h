!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2005.01.08 ORDynaString	[Z, GLULX]
! ORDynaString is a quick class to facilitate creation and printing of dynamic text.
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
! AutoDep:	
!	GLULX:	ORInfGlk
!	Both:	ORTextFormatting
!--------------------------------------------------------------------------------------
! A more detailed description of this file, what it does, and how to use it goes here.
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORDynaString";					!Constant USE_ORDynaString; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORDynaString; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.12.26	Initial Creation (not really, but this was the earliest date I could 
!				find without searching my code archive)
! 2002.02.13	Added Glulx support 
! 2002.02.25	Finalized Glulx support.  Also made objects utilize zero-based index.
! 2002.02.26	Fit to new template.
! 2003.01.31	Added routine to print partial strings (substring)
! 2003.12.13	renamed the length property to "strlen" in order to avoid warning 
!				message in new 6.3 compiler
! 2004.02.03	Moved code to Replace section.
! 2004.02.11	Added autodepend for TextFormatting
! 2004.02.11	Added autodepend for InfGlk
! 2005.01.08	Implemented substring fix submitted by Michael Roy
!--------------------------------------------------------------------------------------
message "          Processing library extension ORDynaString...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
! #ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORSupporterContainer forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 	
	#ifndef ORLibraryInclude; message fatalerror "ORDynaString has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORTextFormatting_DONEREPLACE; default USE_ORTextFormatting 0; message "          [ORDynaString forcing inclusion of ORTextFormatting]"; #include "ORTextFormatting"; #endif; 
#ifdef TARGET_GLULX;
	#ifndef ORInfGlk_DONEREPLACE; default USE_ORInfGlk 0; message "          [ORSupporterContainer forcing inclusion of ORInfGlk]"; #include "ORInfGlk"; #endif; 
#endif;
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORDynaString_DONEREPLACE; constant ORDynaString_DONEREPLACE; #ifdef ORDynaString_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	system_file;
	
	#ifndef ORDynaBuf; array ORDynaBuf -> 1000; #endif;
	class ORDynaString
	with	buf ORDynaBuf
		,	sv 0
		,	matches[o t;
				if(o ofclass object){
					if(~~(o ofclass ORDynaBuf)) rfalse;
					o=o.buf; !--is a dynabuf, so get the actual buffer...
				}
				if(o==0)rfalse; !--if not initialized, then error 
				for(t=0:t<self.strlen()+WORDSIZE:t++){  !--check all including length
					if(self.buf->(t)~=o->(t)) rfalse;
				}
				rtrue;
			]
		,	capture[a;
				a=self.buf;
	#ifndef TARGET_GLULX;
			@output_stream 3 a;	!capture output to an array
	#ifnot;
			self.sv=glk_stream_get_current();
			glk_stream_set_current(glk_stream_open_memory(a+WORDSIZE, 1000, filemode_Write, 662));
	#endif;
			]
		,	release[;
		#ifndef TARGET_GLULX;
			@output_stream -3;		!release output back to screen
		#ifnot;
			glk_stream_close(glk_stream_get_current(), gg_arguments);
			glk_stream_set_current(self.sv);
			(self.buf-->0)=gg_arguments-->1;
		#endif;
			]
		,	substring[start count i len;
				len=(self.buf-->0);
				if(start+count>len)count=len-start;
				for(i=start:i<count+start:i++) 
					print (char) (self.buf)->(i+WORDSIZE); 
			]
		,	print[o;
				if(o==0) o=self.buf;
				print (arraystring) o;
			]
		,	islower[v;
#ifndef TARGET_GLULX;
				if(v>96 && v<123) rtrue; rfalse;
#ifnot;
				if(v==glk_char_to_lower(v)) rtrue; rfalse;
#endif;
			]
		,	isupper[v;
#ifndef TARGET_GLULX;
				if(v>64 && v<91) rtrue; rfalse;
#ifnot;
				if(v==glk_char_to_upper(v)) rtrue; rfalse;
#endif;
			]
		,	get_char[n;
				n=n+WORDSIZE;
				return ((self.buf)->n);
			]
		,	set_char[n val;
				n=n+WORDSIZE;
				((self.buf)->n)=val;
			]
		,	strlen [;return (self.buf-->0);]
		,	upper[n t;
				n=n+WORDSIZE;
				t=((self.buf)->n);
				if(self.islower(t)) {
#ifndef TARGET_GLULX;
					((self.buf)->n)=t-32;
#ifnot;
					((self.buf)->n)=glk_char_to_upper(t);
#endif;
				}
			]
		,	lower[n t;
				n=n+WORDSIZE;
				t=((self.buf)->n);
				if(self.isupper(t)) {
#ifndef TARGET_GLULX;
					((self.buf)->n)=t+32;	
#ifnot;
					((self.buf)->n)=glk_char_to_lower(t);
#endif;
				}
			]
		,	upper_all[t;
				for(t=0:t<self.strlen():t++){
					self.upper(t);
				}
			]
		,	lower_all[t;
				for(t=0:t<self.strlen():t++){
					self.lower(t);
				}
			]
	;
	[dynastring o;	o.print();]; !works with both strings and dynastrings
!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORDynaString_DONEMESSAGE; constant ORDynaString_DONEMESSAGE;  #ifdef ORDynaString_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORDynaString requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORDynaString_DONECODE;constant ORDynaString_DONECODE; #ifdef ORDynaString_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
   #endif; #ifnot; #ifndef ORDynaString_DONEGRAMMAR; constant ORDynaString_DONEGRAMMAR; #ifdef ORDynaString_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================