!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORParseToken [Z, GLULX]
! A rearangement of the ParserToken__ routine.  No actual code has been altered (at
! least not in any significant way) but the routine has been broken into smaller 
! componants which can be overriden by other modules.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- - 
! License: 
! Matches that of Inform's standard library (see included header, below).
!--------------------------------------------------------------------------------------
! AutoDep:	none
!--------------------------------------------------------------------------------------
! ORLibDev: Note, that this module is not useful as a library extension alone.  By 
!     itself it provides no new functionality for the IF developer. Instead it provides
!     a basis for other modules to be built upon and will primarily be useful to those 
!     who are developing ORLibrary entries of their own.
!--------------------------------------------------------------------------------------
! Although arranged differently, this code is nevertheless the same ParserToken__
! routine that comes from Graham Nelson's standard library (actually Andrew Plotkin's
! Biplatform library which is a modified version of Graham Nelson's standard library).  
! In any case, it falls under the same copyright as the code upon which it was based 
! which can be found in the standard library's parserm file.  The header to this file 
! is included now:
!--------------------------------------------------------------------------------------
	!  PARSERM:  Core of parser.
	!
	!  Supplied for use with Inform 6                         Serial number 991113
	!                                                                 Release 6/10
	!  (c) Graham Nelson 1993, 1994, 1995, 1996, 1997, 1998, 1999
	!      but freely usable (see manuals)
!--------------------------------------------------------------------------------------
! This module replaces the large and bulky original version of ParserToken__ with a 
! smaller version that does little more than make calls to routines that are 
! compartmentalized versions of the code originally making up the ParserToken__ routine. 
! These smaller routines can more selectively be replaced by other modules.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORParseToken; #include "ORParseToken"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORParseToken; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2003.01.03	Initial Creation
!--------------------------------------------------------------------------------------
message "          Processing library extension ORParseToken...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORParseToken forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORParseToken has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORParseToken_DONEREPLACE; constant ORParseToken_DONEREPLACE; #ifdef ORParseToken_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace ParseToken__;
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORParseToken_DONEMESSAGE; constant ORParseToken_DONEMESSAGE;  #ifdef ORParseToken_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORParseToken requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORParseToken_DONECODE;constant ORParseToken_DONECODE; #ifdef ORParseToken_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
global pt_return;	!-- 0:		continue on
					!-- true:	return return value
					!-- -1:		jump to FailToken
					!-- -2:		jump to ObjectList
					!-- -3:		jump to TryAgain
global pt_many_flag;
! ----------------------------------------------------------------------------
!  ParseToken(type, data):
!      Parses the given token, from the current word number wn, with exactly
!      the specification of a general parsing routine.
!      (Except that for "topic" tokens and prepositions, you need to supply
!      a position in a valid grammar line as third argument.)
!
!  Returns:
!    GPR_REPARSE  for "reconstructed input, please re-parse from scratch"
!    GPR_PREPOSITION  for "token accepted with no result"
!    $ff00 + x    for "please parse ParseToken(ELEMENTARY_TT, x) instead"
!    0            for "token accepted, result is the multiple object list"
!    1            for "token accepted, result is the number in parsed_number"
!    object num   for "token accepted with this object as result"
!    -1           for "token rejected"
!
!  (A)            Analyse the token; handle all tokens not involving
!                 object lists and break down others into elementary tokens
!  (B)            Begin parsing an object list
!  (C)            Parse descriptors (articles, pronouns, etc.) in the list
!  (D)            Parse an object name
!  (E)            Parse connectives ("and", "but", etc.) and go back to (C)
!  (F)            Return the conclusion of parsing an object list
! ----------------------------------------------------------------------------
[ ParseToken__ given_ttype given_tdata token_n
             token l and_parity single_object desc_wn 
             token_allows_multiple;
	pt_return=false;
	pt_many_flag = false; and_parity = true; dont_infer = false;
!  **** (A) ****
	token=PT_Analyse__(given_ttype, given_tdata, token_n, token);
		if(pt_return==true) return token;
!  **** (B) ****	
	token_allows_multiple=PT_BeginList__(token);
!  **** (C) ****
!  We expect to find a list of objects next in what the player's typed.
.ObjectList;
	desc_wn=pt_descriptors__();
		if(pt_return==true) return desc_wn;
.TryAgain;
	l=PT_LoopStart__(token_allows_multiple);
		if(pt_return==true) return l;
!  **** (D) ****
	single_object=PT_ObjectName__(token,and_parity, desc_wn);
		if(pt_return==true) return single_object;
		if(pt_return==-1) jump FailToken;
!  **** (E) ****
	and_parity=PT_Connectives__(and_parity,token_allows_multiple,single_object);
		if(pt_return==-1) jump FailToken;
		if(pt_return==-2) jump ObjectList;
!  **** (F) ****
!  Happy or unhappy endings:
	single_object=PT_PassToken__(single_object,token);
		if(pt_return==-1) jump FailToken;
    return single_object;
.FailToken;
	l=PT_FailToken__(desc_wn);
		if(pt_return==-3) jump TryAgain;
	return -1;
];
system_file; !--allow the components to be replaced
[PT_Analyse__ given_ttype given_tdata token_n
             token l o;
	pt_return=true; !--default this to true so that all returns except the last exit main routine.
!  **** (A) ****
   token_filter = 0;

   switch(given_ttype)
   {   ELEMENTARY_TT:
           switch(given_tdata)
           {   SPECIAL_TOKEN:
                   l=TryNumber(wn);
                   special_word=NextWord();
                   #ifdef DEBUG;
                   if (l~=-1000)
                       if (parser_trace>=3)
                           print "  [Read special as the number ", l, "]^";
                   #endif;
                   if (l==-1000)
                   {   #ifdef DEBUG;
                       if (parser_trace>=3)
                         print "  [Read special word at word number ", wn, "]^";
                       #endif;
                       l = special_word;
                   }
                   parsed_number = l; return GPR_NUMBER;

               NUMBER_TOKEN:
                   l=TryNumber(wn++);
                   if (l==-1000) { etype=NUMBER_PE; return GPR_FAIL; } 
                   #ifdef DEBUG;
                   if (parser_trace>=3) print "  [Read number as ", l, "]^";
                   #endif;
                   parsed_number = l; return GPR_NUMBER;

               CREATURE_TOKEN:
                   if (action_to_be==##Answer or ##Ask or ##AskFor or ##Tell)
                       scope_reason = TALKING_REASON;

               TOPIC_TOKEN:
                   consult_from = wn;
                   if ((line_ttype-->(token_n+1) ~= PREPOSITION_TT)
                       && (line_token-->(token_n+1) ~= ENDIT_TOKEN))
                       RunTimeError(13);
                   do o=NextWordStopped();
                   until (o==-1 || PrepositionChain(o, token_n+1) ~= -1);
                   wn--;
                   consult_words = wn-consult_from;
                   if (consult_words==0) return GPR_FAIL;
                   if (action_to_be==##Ask or ##Answer or ##Tell)
                   {   o=wn; wn=consult_from; parsed_number=NextWord();
                       #IFDEF EnglishNaturalLanguage;
                       if (parsed_number=='the' && consult_words>1)
                           parsed_number=NextWord();
                       #ENDIF;
                       wn=o; return 1;
                   }
                   return GPR_PREPOSITION;
           }

       PREPOSITION_TT:
           #Iffalse Grammar__Version==1;
!  Is it an unnecessary alternative preposition, when a previous choice
!  has already been matched?
           if ((token->0) & $10) return GPR_PREPOSITION;
           #Endif;

!  If we've run out of the player's input, but still have parameters to
!  specify, we go into "infer" mode, remembering where we are and the
!  preposition we are inferring...

           if (wn > num_words)
           {   if (inferfrom==0 && parameters<params_wanted)
               {   inferfrom = pcount; inferword = token;
                   pattern-->pcount = REPARSE_CODE + Dword__No(given_tdata);
               }

!  If we are not inferring, then the line is wrong...

               if (inferfrom==0) return -1;

!  If not, then the line is right but we mark in the preposition...

               pattern-->pcount = REPARSE_CODE + Dword__No(given_tdata);
               return GPR_PREPOSITION;
           }

           o = NextWord();

           pattern-->pcount = REPARSE_CODE + Dword__No(o);

!  Whereas, if the player has typed something here, see if it is the
!  required preposition... if it's wrong, the line must be wrong,
!  but if it's right, the token is passed (jump to finish this token).

           if (o == given_tdata) return GPR_PREPOSITION;
           #Iffalse Grammar__Version==1;
           if (PrepositionChain(o, token_n) ~= -1)
               return GPR_PREPOSITION;
           #Endif;
           return -1;

       GPR_TT:
           l=indirect(given_tdata);
           #ifdef DEBUG;
           if (parser_trace>=3)
               print "  [Outside parsing routine returned ", l, "]^";
           #endif;
           return l;

       SCOPE_TT:
           scope_token = given_tdata;
           scope_stage = 1;
           l = indirect(scope_token);
           #ifdef DEBUG;
           if (parser_trace>=3)
               print "  [Scope routine returned multiple-flag of ", l, "]^";
           #endif;
           if (l==1) given_tdata = MULTI_TOKEN; else given_tdata = NOUN_TOKEN;

       ATTR_FILTER_TT:
           token_filter = 1 + given_tdata;
           given_tdata = NOUN_TOKEN;

       ROUTINE_FILTER_TT:
           token_filter = given_tdata;
           given_tdata = NOUN_TOKEN;
   }
	pt_return=false;
	return given_tdata;
];
[PT_BeginList__ token;
	!  There are now three possible ways we can be here:
	!      parsing an elementary token other than "special" or "number";
	!      parsing a scope token;
	!      parsing a noun-filter token (either by routine or attribute).
	!
	!  In each case, token holds the type of elementary parse to
	!  perform in matching one or more objects, and
	!  token_filter is 0 (default), an attribute + 1 for an attribute filter
	!  or a routine address for a routine filter.
	if(token == MULTI_TOKEN or MULTIHELD_TOKEN or MULTIEXCEPT_TOKEN or MULTIINSIDE_TOKEN) rtrue;
	rfalse;
];


[PT_Descriptors__ o l;
	pt_return=true; !--assume exit
	!  **** (C) ****
	#ifdef DEBUG;
	if (parser_trace>=3) print "  [Object list from word ", wn, "]^";
	#endif;
!  Take an advance look at the next word: if it's "it" or "them", and these
!  are unset, set the appropriate error number and give up on the line
!  (if not, these are still parsed in the usual way - it is not assumed
!  that they still refer to something in scope)
    o=NextWord(); wn--;
    pronoun_word = NULL; pronoun_obj = NULL;
    l = PronounValue(o);
    if (l ~= 0)
    {   pronoun_word = o; pronoun_obj = l;
        if (l == NULL)
        {   !   Don't assume this is a use of an unset pronoun until the
            !   descriptors have been checked, because it might be an
            !   article (or some such) instead

            for (l=1:l<=LanguageDescriptors-->0:l=l+4)
                if (o == LanguageDescriptors-->l) jump AssumeDescriptor;
            pronoun__word=pronoun_word; pronoun__obj=pronoun_obj;
            etype=VAGUE_PE; return GPR_FAIL; 
        }
    }
.AssumeDescriptor;
	PT_HandleMeWords__(o);
	allow_plurals = true; ! --desc_wn = wn;
	pt_return=false;
	return wn;
];

[PT_HandleMeWords__ o;
    if (o==ME1__WD or ME2__WD or ME3__WD){   
		pronoun_word = o; pronoun_obj = player;
    }
];

[PT_LoopStart__ token_allows_multiple l;
	pt_return=true; !--assume exit
	!   First, we parse any descriptive words (like "the", "five" or "every"):
    l = Descriptors(token_allows_multiple); 
    if (l~=0) { etype=l; return GPR_FAIL; }
	pt_return=false; 
];
[PT_ObjectName__ token and_parity desc_wn l i j k o single_object;
	pt_return=true; !--assume exit
	!  **** (D) ****
.TryAgain2;
	!  This is an actual specified object, and is therefore where a typing error
	!  is most likely to occur, so we set:
	
		oops_from = wn;

	!  So, two cases.  Case 1: token not equal to "held" (so, no implicit takes)
	!  but we may well be dealing with multiple objects

	!  In either case below we use NounDomain, giving it the token number as
	!  context, and two places to look: among the actor's possessions, and in the
	!  present location.  (Note that the order depends on which is likeliest.)

		if (token ~= HELD_TOKEN)
		{   i=multiple_object-->0;
			#ifdef DEBUG;
			if (parser_trace>=3)
				print "  [Calling NounDomain on location and actor]^";
			#endif;
			l=NounDomain(actors_location, actor, token);
			if (l==REPARSE_CODE) return l;                  ! Reparse after Q&A
			if (l==0) {   if (indef_possambig)
						{   ResetDescriptors(); wn = desc_wn; jump TryAgain2; }
						etype=CantSee(); pt_return=-1; return; } ! Choose best error

			#ifdef DEBUG;
			if (parser_trace>=3)
			{   if (l>1)
					print "  [ND returned ", (the) l, "]^";
				else
				{   print "  [ND appended to the multiple object list:^";
					k=multiple_object-->0;
					for (j=i+1:j<=k:j++)
						print "  Entry ", j, ": ", (The) multiple_object-->j,
							" (", multiple_object-->j, ")^";
					print "  List now has size ", k, "]^";
				}
			}
			#endif;

			if (l==1)
			{   if (~~pt_many_flag)
				{   pt_many_flag = true;
				}
				else                                  ! Merge with earlier ones
				{   k=multiple_object-->0;            ! (with either parity)
					multiple_object-->0 = i;
					for (j=i+1:j<=k:j++)
					{   if (and_parity) MultiAdd(multiple_object-->j);
						else MultiSub(multiple_object-->j);
					}
					#ifdef DEBUG;
					if (parser_trace>=3)
						print "  [Merging ", k-i, " new objects to the ",
							i, " old ones]^";
					#endif;
				}
			}
			else
			{   ! A single object was indeed found

				if (match_length == 0 && indef_possambig)
				{   !   So the answer had to be inferred from no textual data,
					!   and we know that there was an ambiguity in the descriptor
					!   stage (such as a word which could be a pronoun being
					!   parsed as an article or possessive).  It's worth having
					!   another go.

					ResetDescriptors(); wn = desc_wn; jump TryAgain2;
				}
	        
				if (token==CREATURE_TOKEN && CreatureTest(l)==0)
				{   etype=ANIMA_PE; pt_return=-1; return; } !  Animation is required

				if (~~pt_many_flag)
					single_object = l;
				else
				{   if (and_parity) MultiAdd(l); else MultiSub(l);
					#ifdef DEBUG;
					if (parser_trace>=3)
						print "  [Combining ", (the) l, " with list]^";
					#endif;
				}
			}
		}

	!  Case 2: token is "held" (which fortunately can't take multiple objects)
	!  and may generate an implicit take

		else

		{   l=NounDomain(actor,actors_location,token);       ! Same as above...
			if (l==REPARSE_CODE) return GPR_REPARSE;
			if (l==0)
			{   if (indef_possambig)
				{   ResetDescriptors(); wn = desc_wn; jump TryAgain2; }
				etype=CantSee(); return GPR_FAIL;            ! Choose best error
			}

	!  ...until it produces something not held by the actor.  Then an implicit
	!  take must be tried.  If this is already happening anyway, things are too
	!  confused and we have to give up (but saving the oops marker so as to get
	!  it on the right word afterwards).
	!  The point of this last rule is that a sequence like
	!
	!      > read newspaper
	!      (taking the newspaper first)
	!      The dwarf unexpectedly prevents you from taking the newspaper!
	!
	!  should not be allowed to go into an infinite repeat - read becomes
	!  take then read, but take has no effect, so read becomes take then read...
	!  Anyway for now all we do is record the number of the object to take.

			o=parent(l);
			if (o~=actor)
			{   if (notheld_mode==1)
				{   saved_oops=oops_from; etype=NOTHELD_PE; pt_return=-1; return; 
				}
				not_holding = l;
				#ifdef DEBUG;
				if (parser_trace>=3)
					print "  [Allowing object ", (the) l, " for now]^";
				#endif;
			}
			single_object = l;
		}

	!  The following moves the word marker to just past the named object...

		wn = oops_from + match_length;

		pt_return=false;
		return single_object;
];

[PT_Connectives__ and_parity token_allows_multiple single_object o;
!  **** (E) ****
		pt_return=true;
!  Object(s) specified now: is that the end of the list, or have we reached
!  "and", "but" and so on?  If so, create a multiple-object list if we
!  haven't already (and are allowed to).

    .NextInList; !--a label that is never used?

    o=NextWord();

    if (o==AND1__WD or AND2__WD or AND3__WD or BUT1__WD or BUT2__WD or BUT3__WD
           or comma_word)
    {
        #ifdef DEBUG;
        if (parser_trace>=3) print "  [Read connective '", (address) o, "']^";
        #endif;

        if (~~token_allows_multiple)
        {   etype=MULTI_PE; 
			pt_return=-1; !--FailToken;
			return and_parity;
        }

        if (o==BUT1__WD or BUT2__WD or BUT3__WD) and_parity = 1-and_parity;

        if (~~pt_many_flag)
        {   multiple_object-->0 = 1;
            multiple_object-->1 = single_object;
            pt_many_flag = true;
            #ifdef DEBUG;
            if (parser_trace>=3)
                print "  [Making new list from ", (the) single_object, "]^";
            #endif;
        }
        dont_infer = true; inferfrom=0;           ! Don't print (inferences)
        pt_return=-2; !--objectlist
		return and_parity;                          ! And back around
    }
    wn--;   ! Word marker back to first not-understood word
	pt_return=false;
	return and_parity;
];
[PT_PassToken__ single_object token;
	pt_return=true;
	.PassToken; !--another label not used

    if (pt_many_flag)
    {   single_object = GPR_MULTIPLE;
        multi_context = token;
    }
    else
    {   if (indef_mode==1 && indef_type & PLURAL_BIT ~= 0)
        {   if (indef_wanted<100 && indef_wanted>1)
            {   multi_had=1; multi_wanted=indef_wanted;
                etype=TOOFEW_PE;
                pt_return=-1;
				return single_object;
            }
        }
    }
	pt_return=false;
	return single_object;
];
[PT_FailToken__ desc_wn;
!  If we were only guessing about it being a plural, try again but only
!  allowing singulars (so that words like "six" are not swallowed up as
!  Descriptors)
    if (allow_plurals && indef_guess_p==1)
    {   allow_plurals=false; wn=desc_wn; return -3; !--jump TryAgain;
    }
];
!======================================================================================
#endif; #ifnot; #ifndef ORParseToken_DONEGRAMMAR; constant ORParseToken_DONEGRAMMAR; #ifdef ORParseToken_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================