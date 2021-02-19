!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORReferByContents	[Z, GLULX]
! An object that can be refered to by the name of what it contains.  For instance a 
! glass of water can be taken by the command "get water".
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
! AutoDep :	none
!--------------------------------------------------------------------------------------
! A more detailed description of this file, what it does, and how to use it goes here.
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORReferByContents";					!Constant USE_ORReferByContents; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORReferByContents; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.22	Initial Creation (not really, but this is the earliest date I could find
!				without searching my code archive)
! 2002.02.26	Fit to new template
! 2003.02.18	Made modifications to support ORAdjective, also fixed a signed comparsion problem
!				that occurs in large games.
! 2003.06.08	Made modifications to stop the parse_name routine from signalling "NO MATCH",
!				rather it now passes control back to the parser in the event that it cannot 
!				validate the object against the input.  Other routines, such as ORAdjective, 
!				might be more successful.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORReferByContents...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORReferByContents forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORReferByContents has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORReferByContents_DONEREPLACE; constant ORReferByContents_DONEREPLACE; #ifdef ORReferByContents_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORReferByContents_DONEMESSAGE; constant ORReferByContents_DONEMESSAGE;  #ifdef ORReferByContents_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORReferByContents requires the OREnglish file.";#endif; 
	class ORReferByContents
		with	refer_actions 0 
		,		ignore_actions 0
		,		match_name[w o;
					if(WordInProperty(w,self,name)==true && self.discount(w,self)==false)
						rtrue;
					else{ 
						objectloop(o in self) {
							if(WordInProperty(w,o,name)==true && self.discount(w,o)==false)
								rtrue;
						}
					}
					rfalse;
				]
		,		discount[w o i;w=w;o=o;i=i;
	#ifdef USE_ORAdjective;
					if((o provides adjective)==false) rfalse;
					i=WordInProperty(w,o,adjective);
					return i;
	#ifnot;
					rfalse;
	#endif;
				]
		,		match_short_name[w swn o; w=w;o=o;
	#ifdef USE_ORRecogName;
						if(MatchAgainstShortName(self,swn)==true && self.discount(w,self)==false)
							rtrue;
						else{ 
							objectloop(o in self) {
								if(MatchAgainstShortName(o,swn)==true && self.discount(w,o)==false)
									rtrue;
							}
						}
						rfalse;
	#ifnot;
						rfalse;
	#endif;
				]
		,		parse_name[w found count svdwn passedinwn;
					if(parser_action==##TheSame) return 0; !--don't try to differentiate between similar objects
					
					if (found_ttype==HELD_TOKEN && (self in actor)==false && self has static or scenery) return -1;
					if(self.refer_actions~=0 && WordInProperty(action_to_be,self,refer_actions)==false) return -1;
					if(self.ignore_actions~=0 && WordInProperty(action_to_be,self,ignore_actions)==true) return -1;
					passedinwn=wn;
					do{
						found=false;
						svdwn=wn;
						w=NextWord();
						if(w~=0) found=self.match_name(w);
						if(found==false || w==0) found=self.match_short_name(w, svdwn);
						if(found==true) count++; 
					}until(found==false);
					if(count>0) return count;
					wn=passedinwn; !--restore the passed in wn
					return -1; 
				]
	;
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORReferByContents_DONECODE;constant ORReferByContents_DONECODE; #ifdef ORReferByContents_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #ifnot; #ifndef ORReferByContents_DONEGRAMMAR; constant ORReferByContents_DONEGRAMMAR; #ifdef ORReferByContents_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================