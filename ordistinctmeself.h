!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORDistinctMeSelf [Z, GLULX]
! Makes the subtle distinction between 'self' and 'me', having them 
! equate to 'actor' and 'player' repectively.
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
! This module is most useful when ordering NPC's around.  The default functionality 
! of the standard library is to equate all "me" words with the player.  This results 
! in the following two orders being the same:
!
!		"Bob, pour coffee on me" 
!		"Bob, pour coffee on self" 
!
! Both forms would normally result in an order for Bob to pour coffee on the player. 
! This module will make the appropriate distinction where the second form will instruct
! "Bob" to pour coffee on himself.  Additionally, it adds the word "yourself" as a "me"
! word that equals the actor.
!--------------------------------------------------------------------------------------
! AutoDep:	ORParseToken
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORDistinctMeSelf; #include "ORDistinctMeSelf"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORDistinctMeSelf; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2003.01.03	Initial Creation
! 2003.01.05	Um... Minor bug fixed.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORDistinctMeSelf...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORDistinctMeSelf forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORDistinctMeSelf has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	 #ifndef ORParseToken_DONEREPLACE; default USE_ORParseToken 0; message "          [ORDistinctMeSelf forcing inclusion of ORParseToken]"; #include "ORParseToken"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORDistinctMeSelf_DONEREPLACE; constant ORDistinctMeSelf_DONEREPLACE; #ifdef ORDistinctMeSelf_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace PT_HandleMeWords__;
	replace TryGivenObject;
	Constant ME4__WD 'yourself';
!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORDistinctMeSelf_DONEMESSAGE; constant ORDistinctMeSelf_DONEMESSAGE;  #ifdef ORDistinctMeSelf_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORDistinctMeSelf requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORDistinctMeSelf_DONECODE;constant ORDistinctMeSelf_DONECODE; #ifdef ORDistinctMeSelf_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	[PT_HandleMeWords__ o;
		if (o==ME1__WD or ME2__WD or ME3__WD or ME4__WD){   
			pronoun__word = pronoun_word = o; 
			if (o==ME1__WD or ME2__WD) pronoun__obj = pronoun_obj = player;
			else pronoun__obj =pronoun_obj = actor;
		}
	];

	[ TryGivenObject obj threshold k w j z;

	#ifdef DEBUG;
	if (parser_trace>=5)
		print "    Trying ", (the) obj, " (", obj, ") at word ", wn, "^";
	#endif;

	dict_flags_of_noun = 0;

	!  If input has run out then always match, with only quality 0 (this saves
	!  time).

	if (wn > num_words)
	{   if (indef_mode ~= 0)
			dict_flags_of_noun = $$01110000;  ! Reject "plural" bit
		MakeMatch(obj,0);
		#ifdef DEBUG;
		if (parser_trace>=5)
		print "    Matched (0)^";
		#endif;
		return 1;
	}

	!  Ask the object to parse itself if necessary, sitting up and taking notice
	!  if it says the plural was used:

	if (obj.parse_name~=0)
	{   parser_action = NULL; j=wn;
		k=RunRoutines(obj,parse_name);
		if (k>0)
		{   wn=j+k;
			.MMbyPN;

			if (parser_action == ##PluralFound)
				dict_flags_of_noun = dict_flags_of_noun | 4;

			if (dict_flags_of_noun & 4)
			{   if (~~allow_plurals) k=0;
				else
				{   if (indef_mode==0)
					{   indef_mode=1; indef_type=0; indef_wanted=0; }
					indef_type = indef_type | PLURAL_BIT;
					if (indef_wanted==0) indef_wanted=100;
				}
			}

			#ifdef DEBUG;
				if (parser_trace>=5)
				{   print "    Matched (", k, ")^";
				}
			#endif;
			MakeMatch(obj,k);
			return k;
		}
		if (k==0) jump NoWordsMatch;
	}

	!  The default algorithm is simply to count up how many words pass the
	!  Refers test:

	parser_action = NULL;

	w = NounWord();
!-----------------modified code to support actor/player differences (Begin)--------------
	wn=wn-1;
	z=NextWord();
	if (w==1 || z==ME4__WD){ !--if a me word
			if(player==actor && obj==player) { k=1; jump MMbyPN; } !--if player and actor are the same then all "me" words will work
			if(z==ME1__WD or ME2__WD){
				if(obj==player) { k=1; jump MMbyPN; } !--me and myself reference player 
			}
			else{
				if(obj==actor) { k=1; jump MMbyPN; } !--self and yourself reference actor
			}
	}
!-----------------modified code to support actor/player differences (END)  --------------

	if (w>=2 && w<128 && (LanguagePronouns-->w == obj))
	{   k=1; jump MMbyPN; }

	j=--wn;
	threshold = ParseNoun(obj);
	#ifdef DEBUG;
	if (threshold>=0 && parser_trace>=5)
		print "    ParseNoun returned ", threshold, "^";
	#endif;
	if (threshold<0) wn++;
	if (threshold>0) { k=threshold; jump MMbyPN; }

	if (threshold==0 || Refers(obj,wn-1)==0)
	{   .NoWordsMatch;
		if (indef_mode~=0)
		{   k=0; parser_action=NULL; jump MMbyPN;
		}
		rfalse;
	}

	if (threshold<0)
	{   threshold=1;
		dict_flags_of_noun = (w->#dict_par1) & $$01110100;
		w = NextWord();
		while (Refers(obj, wn-1))
		{   threshold++;
			if (w)
				dict_flags_of_noun = dict_flags_of_noun
										| ((w->#dict_par1) & $$01110100);
			w = NextWord();
		}
	}

	k = threshold; jump MMbyPN;
	];

!======================================================================================
   #endif; #ifnot; #ifndef ORDistinctMeSelf_DONEGRAMMAR; constant ORDistinctMeSelf_DONEGRAMMAR; #ifdef ORDistinctMeSelf_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================