!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORAdjective [Z, Glulx]
! Implementation of Adjectives.
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
! Two options are available to govern this module's behavior:
!
! 1) The DM4 Exercise 75 gives an example of a ParseNoun routine which distinguishes 
! between nouns and adjectives. This module implements a modified version of 
! that routine, but leverages the ORParseNoun object so that other ParseNoun-like 
! routines can co-exist without conflict.  Additionally, this version of ParseNoun will 
! also work with the ORRecogName module.
!
! 2) Alternatively, this module can be made to prefer objects when a noun word is 
! specifically used, but still allow reference by adjectives only. This method uses a 
! technique that is very very similar to Miron Schmidt's calyx_adjectives extension and 
! leverages the ORChooseObjects object to allow other ChooseObject routines to co-exist.
!
! This second mode is the default; however the first mode can be turned on by setting 
! the mode variable of the ORAdjective object, usually in Initialise():
!
!	ORAdjective.mode=REQUIRE_NOUN;
!
! This setting differs from the default value of PREFER_NOUN.
!
!--------------------------------------------------------------------------------------
! AutoDep:	OREntryPoints
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORAdjective; #include "ORAdjective"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORAdjective; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.05.08	Initial Creation
! 2002.07.07	Added menu documention.
! 2002.07.20	Added Auto Dependancy code for objectinitialise.
! 2002.08.31	Modified to reference the new OREntryPoints module.
! 2002.12.22	Modified the parsenoun algorithm to detect plural nouns.
! 2003.02.19	Added better ifdef for self docs.
! 2003.03.02	Fixed spelling issue in self docs.
! 2003.04.14	Added code to allow objects to be referenced by adjectives only, yet
!				still prefer nouns. 
! 2003.06.08	...And modified the above code slightly.
! 2003.12.23	Fixed bug where nouns were not preferred over adjectives.
! 2004.01.09	Fixed bug where empty adjective properties could lead to a never-ending
!				false positives
! 2004.01.11	Modified to take advantage of the standard libary's new ext_initialise 
!				functionality. Also made PREFER_NOUN the default setting and removed the 
!				Auto documentation, since it is no longer accurate.
! 2004.01.24	Added code to make the parsenoun routine give other parsenoun routines
!				a chance if it doesn't match any words.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORAdjective...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORAdjective forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORAdjective has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef OREntryPoints_DONEREPLACE; default USE_OREntryPoints 0; message "          [ORAdjective forcing inclusion of OREntryPoints]"; #include "OREntryPoints"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORAdjective_DONEREPLACE; constant ORAdjective_DONEREPLACE; #ifdef ORAdjective_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORAdjective_DONEMESSAGE; constant ORAdjective_DONEMESSAGE;  #ifdef ORAdjective_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
! #ifndef OREnglish; message fatalerror "ERROR!!!! ORAdjective requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORAdjective_DONECODE;constant ORAdjective_DONECODE; #ifdef ORAdjective_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	Constant	REQUIRE_NOUN 0;
	Constant	PREFER_NOUN 1;
	attribute	noun_ref;
	object ORAdjective LibraryExtensions 
		with mode PREFER_NOUN
		,	 ext_initialise[;
				ORChooseObjects.add_routine(__chooseobjectadjective); 
				ORParseNoun.add_routine(__parsenounadjective);
			]
		, adjective;
	[__parsenounadjective obj n m f;
		give obj ~noun_ref;
		while(true){
			f=AdjOrNoun(obj);
			if(f==0) break; !--current word does NOT reference the given object
			if(f==1 or 2) { !--current word is a noun, and not an adjective
				m++;			!--increment the noun count
				give obj noun_ref; !--this object was definately reference by a noun word
			}
			if(f==2) parser_action=##PLURALFOUND;
			if(f==3) n++; !--increment the adjective count
		}
		if(n+m==0) return -1; !--we didn't find anything that matched. lets defer to another parsenoun routine...
		if(m==0 && ValueOrRun(ORAdjective,mode)==REQUIRE_NOUN) return 0; !--in this mode, we don't allow referencing by adjectives alone
		return n+m;
	];
	[__chooseobjectadjective obj c;
		if(c<2) return 0; !--let the parser decide
		if(obj has noun_ref) return 2; !--a noun word was actually used
		if(obj hasnt noun_ref) { !--objects referred to only by adjectives are not rated as highly 
			if(number_matched==1) return 1;  !-- specifically, if it was the only object matched, give it an average score
			if(number_matched>1) return -1; !--but if other objects were matched too, then give it a less than average score in case one of the others was referred to by a noun
		}
	];
	!--used to determine if the CurrentWord is an adjective or a noun (singular or plural).  
	!--returns 1:Noun; 2:PluralNoun; 3: Adj; 0:Neither
	[AdjOrNoun obj wrd;
		wrd=NextWord();
		if(wrd~=0 && obj provides adjective && WordInProperty(wrd,obj,adjective)==true) return 3; !--adjective
		if(WordInProperty(wrd,obj,name)==true) {
			if(wrd~=0 && ((wrd->#dict_par1) & 4)~=0) 
				return 2; !--plural noun
			else
				return 1; !--singular noun
		}
	#ifdef USE_ORRecogName; 
		if(MatchAgainstShortName(obj,wn-1)==true) return 1; !--singular noun
	#endif;
		return 0; !--None of the above
	];
!======================================================================================
   #endif; #ifnot; #ifndef ORAdjective_DONEGRAMMAR; constant ORAdjective_DONEGRAMMAR; #ifdef ORAdjective_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================