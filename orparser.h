!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.09 ORParser [Z, GLULX]
! A rearangement of the Parser__parse routine.  No actual code has been altered (at
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
! Although arranged differently, this code is nevertheless the same Parser__parse 
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
! This module replaces the large and bulky original version of Parser__parse with a 
! smaller version that does little more than make calls to routines that are 
! compartmentalized versions of the code originally making up the Parser__parse routine. 
! These smaller routines can more selectively be replaced by other modules.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORParser; #include "ORParser"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORParser; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.04.20	Initial Creation
! 2002.05.01	Modified comments.
! 2003.01.19	Fixed Bug the appeared in DEBUG mode only (missing line number in 
!				debug output.
! 2003.01.24	modified to include the INPUT_BUFFER_LEN constant if it was not already 
!				defined. (This constant is defined in Andrew Plotkin's version of the 
!				standard library for Z-Code but not in the standard library's.)
! 2004.01.11	A quick modification to bring in line with the new standard library's
!				6/11 beta 2 revision.
! 2004.01.18	Added code to suppress a warning.
! 2004.02.09	Added references to new library messages that occur in new 6/11 library.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORParser...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORParser forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORParser has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORParser_DONEREPLACE; constant ORParser_DONEREPLACE; #ifdef ORParser_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace Parser__parse;
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORParser_DONEMESSAGE; constant ORParser_DONEMESSAGE;  #ifdef ORParser_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORParser requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORParser_DONECODE;constant ORParser_DONECODE; #ifdef ORParser_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
#ifndef INPUT_BUFFER_LEN;
	Constant INPUT_BUFFER_LEN = 120;
#endif;
	[ Parser__parse  results retval;
		if((retval=ParseInit(results))<0) jump ParserBranch;
	.ReType;
		if((retval=Keyboard(buffer,parse))<0) jump ParserBranch;
	.ReParse;
		if((retval=ParserReParse(results))<0) jump ParserBranch;
	.AlmostReParse;
		scope_token = 0;
		action_to_be = NULL;
	.BeginCommand;
		if((retval=ParserHandleAgain(results))<0) jump ParserBranch;
		if((retval=ParserBeginCommand(results))<0) jump ParserBranch;
		#ifdef LanguageIsVerb;
			if (verb_word==0){   
				i = wn; verb_word=LanguageIsVerb(buffer, parse, verb_wordnum);
				wn = i;
			}
		#endif;
		!  If the first word is not listed as a verb, it must be a direction
		!  or the name of someone to talk to
		if (verb_word==0 || ((verb_word->#dict_par1) & 1) == 0)
		{   
			if((retval=ParserDirection(results))<0) jump ParserBranch;
			if((retval=ParserCommandToNPC(results))<0) jump ParserBranch;
		}
	.VerbAccepted;
		if((retval=ParserCheckVerbSyntax(results))<0) jump ParserBranch;	
		if((retval=ParserUnpackGrammar(results))<0) jump ParserBranch;	
	.GiveError;
		if((retval=ParserParseUnrecogConv(results))<0) jump ParserBranch;	
		if((retval=ParserShowError(results))<0) jump ParserBranch;	
		jump ReType;
	.LookForMore;
		if((retval=ParserLookForMore(results))<0) jump ParserBranch;
		jump GiveError;
	!--special section setup to handle all of the return codes possible by the broken out pieces of code
	.ParserBranch;
		switch(retval){
			-1:jump ReType;
			-2:jump ReParse;
			-3:jump GiveError;
			-4:jump LookForMore;
			-5:jump VerbAccepted;
			-6:jump BeginCommand;
			-7:jump AlmostReParse;
			-8:rtrue;
			-9:rfalse;
		}
		return; !--shouldn't ever actually get here.
	];
system_file; !----------the following "componants" can be replaced...
	[ ParseInit results i; 
		!  Firstly, in "not held" mode, we still have a command left over from last
		!  time (eg, the user typed "eat biscuit", which was parsed as "take biscuit"
		!  last time, with "eat biscuit" tucked away until now).  So we return that.

		if (notheld_mode==1)
		{   for (i=0:i<8:i++) results-->i=kept_results-->i;
			notheld_mode=0; 
			return -8;
		}

		if (held_back_mode==1)
		{   held_back_mode=0;
			Tokenise__(buffer,parse);
			return -2;
		}
		
	];
	[ParserReparse results i j k l m; i=j=l=m; results=results;!--squash warnings.
		parser_inflection = name;
		!  Initially assume the command is aimed at the player, and the verb
		!  is the first word
		#ifndef TARGET_GLULX;
			num_words=parse->1;
		#ifnot; ! TARGET_GLULX
			num_words=parse-->0;
		#endif; ! TARGET_
			wn=1;
		#ifdef LanguageToInformese;
			LanguageToInformese();
			#ifv5;
			!   Re-tokenise:
				Tokenise__(buffer,parse);
			#endif;
		#endif;
			BeforeParsing();
		#ifndef TARGET_GLULX;
			num_words=parse->1;
		#ifnot; ! TARGET_GLULX
			num_words=parse-->0;
		#endif; ! TARGET_
			k=0;
		#ifdef DEBUG;
			if (parser_trace>=2)
			{   print "[ ";
				for (i=0:i<num_words:i++)
				{
		#ifndef TARGET_GLULX;
					j=parse-->(i*2 + 1);
		#ifnot; ! TARGET_GLULX
					j=parse-->(i*3 + 1);
		#endif; ! TARGET_
					k=WordAddress(i+1);
					l=WordLength(i+1);
					print "~"; for (m=0:m<l:m++) print (char) k->m; print "~ ";

					if (j == 0) print "?";
					else
					{
		#ifndef TARGET_GLULX;
						if (UnsignedCompare(j, 0-->4)>=0
							&& UnsignedCompare(j, 0-->2)<0) print (address) j;
						else print j;
		#ifnot; ! TARGET_GLULX
						if (j->0 == $60) print (address) j;
						else print j;
		#endif; ! TARGET_
					}
					if (i ~= num_words-1) print " / ";
				}
				print " ]^";
			}
		#endif;
			verb_wordnum=1;
			actor=player;
			actors_location = ScopeCeiling(player);
			usual_grammar_after = 0;
	];
	[ParserBeginCommand results i;
		!  Save the present input in case of an "again" next time
		if (verb_word~=AGAIN1__WD)
			for (i=0:i<INPUT_BUFFER_LEN:i++) buffer3->i=buffer->i;
		if (usual_grammar_after==0)
		{   i = RunRoutines(actor, grammar);
			#ifdef DEBUG;
			if (parser_trace>=2 && actor.grammar~=0 or NULL)
				print " [Grammar property returned ", i, "]^";
			#endif;
			if (i<0) { usual_grammar_after = verb_wordnum; i=-i; }
			if (i==1)
			{   results-->0 = action;
				results-->1 = noun;
				results-->2 = second;
				return -8;
			}
			if (i~=0) { verb_word = i; wn--; verb_wordnum--; }
			else
			{   wn = verb_wordnum; verb_word=NextWord();
			}
		}
		else usual_grammar_after=0;
	];
	!--handle input when the player types "again" or "g"
	[ParserHandleAgain results i; results=results;
		!  Begin from what we currently think is the verb word
		wn=verb_wordnum;
		verb_word = NextWordStopped();
		!  If there's no input here, we must have something like
		!  "person,".
		if (verb_word==-1) {   
			best_etype = STUCK_PE; 
			return -3; 
		}
	!  Now try for "again" or "g", which are special cases:
	!  don't allow "again" if nothing has previously been typed;
	!  simply copy the previous text across
		if (verb_word==AGAIN2__WD or AGAIN3__WD) verb_word=AGAIN1__WD;
		if (verb_word==AGAIN1__WD)
		{   if (actor~=player)
			{   L__M(##Miscellany,20); return -1; }
	#ifndef TARGET_GLULX;
			if (buffer3->1==0)
			{   L__M(##Miscellany,21); return -1; }
	#ifnot; ! TARGET_GLULX
			if (buffer3-->0==0)
			{   L__M(##Miscellany,21); return -1; }
	#endif; ! TARGET_
			for (i=0:i<INPUT_BUFFER_LEN:i++) buffer->i=buffer3->i;
			return -2;
		}
	];
	[ParserDirection results l; 
		!  So is the first word an object contained in the special object "compass"
		!  (i.e., a direction)?  This needs use of NounDomain, a routine which
		!  does the object matching, returning the object number, or 0 if none found,
		!  or REPARSE_CODE if it has restructured the parse table so the whole parse
		!  must be begun again...
				wn=verb_wordnum; 
				indef_mode = false; 
				token_filter = 0;
				l=NounDomain(compass,0,0); 
				if (l==REPARSE_CODE) return -2;
		!  If it is a direction, send back the results:
		!  action=GoSub, no of arguments=1, argument 1=the direction.
				if (l~=0)
				{   results-->0 = ##Go;
					action_to_be = ##Go;
					results-->1 = 1;
					results-->2 = l;
					return -4;
				}
	];
	
	[ParserCommandToNPC results i j l; results=results;
		!  Only check for a comma (a "someone, do something" command) if we are
		!  not already in the middle of one.  (This simplification stops us from
		!  worrying about "robot, wizard, you are an idiot", telling the robot to
		!  tell the wizard that she is an idiot.)
				if (actor==player)
				{   for (j=2:j<=num_words:j++)
					{   i=NextWord(); if (i==comma_word) jump Conversation;
					}
					verb_word=UnknownVerb(verb_word);
					if (verb_word~=0) return -5;
				}
				best_etype=VERB_PE; return -3;
		!  NextWord nudges the word number wn on by one each time, so we've now
		!  advanced past a comma.  (A comma is a word all on its own in the table.)
			.Conversation;
				j=wn-1;
				if (j==1) { L__M(##Miscellany,22); return -1;}

		!  Use NounDomain (in the context of "animate creature") to see if the
		!  words make sense as the name of someone held or nearby

				wn=1; lookahead=HELD_TOKEN;
				scope_reason = TALKING_REASON;
				l=NounDomain(player,actors_location,6);
				scope_reason = PARSING_REASON;
				if (l==REPARSE_CODE) 
					return -2;
				if (l==0) { 
					L__M(##Miscellany,23); 
					return -1; 
				}
		!  The object addressed must at least be "talkable" if not actually "animate"
		!  (the distinction allows, for instance, a microphone to be spoken to,
		!  without the parser thinking that the microphone is human).

				if (l hasnt animate && l hasnt talkable)
				{   L__M(##Miscellany, 24, l); return -1; }

		!  Check that there aren't any mystery words between the end of the person's
		!  name and the comma (eg, throw out "dwarf sdfgsdgs, go north").

				if (wn~=j)
				{   L__M(##Miscellany, 25); return -1; }

		!  The player has now successfully named someone.  Adjust "him", "her", "it":

				PronounNotice(l);

		!  Set the global variable "actor", adjust the number of the first word,
		!  and begin parsing again from there.

				verb_wordnum=j+1;

		!  Stop things like "me, again":

				if (l == player)
				{   wn = verb_wordnum;
					if (NextWordStopped() == AGAIN1__WD or AGAIN2__WD or AGAIN3__WD)
					{   L__M(##Miscellany,20); return -1;
					}
				}

				actor=l;
				actors_location=ScopeCeiling(l);
				#ifdef DEBUG;
				if (parser_trace>=1)
					print "[Actor is ", (the) actor, " in ",
						(name) actors_location, "]^";
				#endif;
				return -6;

	];
	[ParserCheckVerbSyntax results; results=results;
		!  We now definitely have a verb, not a direction, whether we got here by the
		!  "take ..." or "person, take ..." method.  Get the meta flag for this verb:
			meta=((verb_word->#dict_par1) & 2)/2;
		!  You can't order other people to "full score" for you, and so on...
			if (meta==1 && actor~=player)
			{   best_etype=VERB_PE; meta=0; return -3; }
	];
	[ParserLookAhead i l m;
		not_holding=0;
        inferfrom=0;
        parameters=0;
        nsns=0; special_word=0; special_number=0;
        multiple_object-->0 = 0;
        multi_context = 0;
        etype=STUCK_PE;
        wn=verb_wordnum+1;
        advance_warning = NULL; 
		indef_mode = false;
        for (i=0,m=false,pcount=0:line_token-->pcount ~= ENDIT_TOKEN:pcount++){   
			scope_token = 0;
            if (line_ttype-->pcount ~= PREPOSITION_TT) i++;
            if (line_ttype-->pcount == ELEMENTARY_TT)
            {   if (line_tdata-->pcount == MULTI_TOKEN) m=true;
                if (line_tdata-->pcount == MULTIEXCEPT_TOKEN or MULTIINSIDE_TOKEN  && i==1){   
                    #ifdef DEBUG;
                    if (parser_trace>=2) print " [Trying look-ahead]^";
                    #endif;
                    pcount++;
                    if (line_ttype-->pcount == PREPOSITION_TT){   
						while (line_ttype-->pcount == PREPOSITION_TT) pcount++;
                        if ((line_ttype-->pcount == ELEMENTARY_TT)&& (line_tdata-->pcount == NOUN_TOKEN))
                        {
                            while (wn <= num_words)
                            {   if (NextWord() == line_tdata-->(pcount-1))
                                {   l = NounDomain(actors_location, actor,
                                            NOUN_TOKEN);
                                    #ifdef DEBUG;
                                    if (parser_trace>=2)
                                    {   print " [Advanced to ~noun~ token: ";
                                        if (l==REPARSE_CODE)
                                            print "re-parse request]^";
                                        if (l==1) print "but multiple found]^";
                                        if (l==0) print "error ", etype, "]^";
                                        if (l>=2) print (the) l, "]^";
                                    }
                                    #endif;
                                    if (l==REPARSE_CODE) return -2;
                                    if (l>=2) advance_warning = l;
                                }
                            }
                        }
					}
                    break;
				}
			}
		}
        take_all_rule = 0;
        if (m && params_wanted==1 && action_to_be==##Take) take_all_rule = 1;
        not_holding=0;
        inferfrom=0;
        parameters=0;
        nsns=0; special_word=0; special_number=0;
        multiple_object-->0 = 0;
        etype=STUCK_PE;
        wn=verb_wordnum+1;
	];


[ ParserUnpackGrammar results retval syntax line num_lines line_address i;
	i=$ff-(verb_word->#dict_par2);
	#ifndef TARGET_GLULX;
    syntax=(0-->7)-->i;
	#ifnot; ! TARGET_GLULX
    syntax=(#grammar_table)-->(i+1);
	#endif; ! TARGET_
    num_lines=(syntax->0)-1;
	pronoun_word=NULL; pronoun_obj=NULL;
	#ifdef DEBUG;
	if (parser_trace>=1){    
	   print "[Parsing for the verb '", (address) verb_word,
              "' (", num_lines+1, " lines)]^";
	}
	#endif;
	best_etype=STUCK_PE; nextbest_etype=STUCK_PE;
    line_address = syntax + 1;

	for (line=0:line<=num_lines:line++)
    {   
        for (i = 0 : i < 32 : i++)
        {   line_token-->i = ENDIT_TOKEN;
            line_ttype-->i = ELEMENTARY_TT;
            line_tdata-->i = ENDIT_TOKEN;
        }
        line_address = UnpackGrammarLine(line_address);
        #ifdef DEBUG;
        if (parser_trace >= 1)
        {   if (parser_trace >= 2) new_line;
            print "[line ", line; DebugGrammarLine();
            print "]^";
        }
        #endif;
		if((retval=ParserLookAhead(results))<0) return retval;
		if((retval=ParserParseToken(results,line))<0) return retval;
		if (take_all_rule == 2 && etype==NOTHING_PE) break;
   }
];
	[ ParserParseToken results line token l i m; line=line;
		for (pcount=1::pcount++)
        {   
			pattern-->pcount = PATTERN_NULL; scope_token=0;
            token = line_token-->(pcount-1);
            lookahead = line_token-->pcount;
            #ifdef DEBUG;
            if (parser_trace >= 2)
               print " [line ", line, " token ", pcount, " word ", wn, " : ",
                     (DebugToken) token, "]^";
            #endif;
            if (token ~= ENDIT_TOKEN){   
				scope_reason = PARSING_REASON;
                parser_inflection = name;
                AnalyseToken(token);
                l = ParseToken__(found_ttype, found_tdata, pcount-1, token);
                while (l<-200) 
					l = ParseToken__(ELEMENTARY_TT, l + 256);
                scope_reason = PARSING_REASON;
                if (l==GPR_PREPOSITION){   
					if (found_ttype~=PREPOSITION_TT && (found_ttype~=ELEMENTARY_TT || found_tdata~=TOPIC_TOKEN)) 
						params_wanted--;
                    l = true;
                }
                else if (l<0) 
					l = false;
				else if (l~=GPR_REPARSE)
                {   
					if (l==GPR_NUMBER)
                    {   if (nsns==0) special_number1=parsed_number;
                        else special_number2=parsed_number;
                        nsns++; l = 1;
                    }
                    if (l==GPR_MULTIPLE) 
						l = 0;
                    results-->(parameters+2) = l;
                    parameters++;
                    pattern-->pcount = l;
                    l = true;
                }
                #ifdef DEBUG;
                if (parser_trace >= 3)
                {   print "  [token resulted in ";
                    if (l==REPARSE_CODE) print "re-parse request]^";
                    if (l==0) print "failure with error type ", etype, "]^";
                    if (l==1) print "success]^";
                }
                #endif;
                if (l==REPARSE_CODE) return -2;
                if (l==false) break;
            }
            else
            {
                if (wn <= num_words)
                {   l=NextWord();
                    if (l==THEN1__WD or THEN2__WD or THEN3__WD or comma_word)
                    {   held_back_mode=1; hb_wn=wn-1; }
                    else
                    {   for (m=0:m<32:m++) pattern2-->m=pattern-->m;
                       pcount2=pcount;
                        etype=UPTO_PE; break;
                    }
                }
                if (parameters>=1 && results-->2 == 0)
                {   l=ReviseMulti(results-->3);
                    if (l~=0) { etype=l; break; }
                }
                if (parameters>=2 && results-->3 == 0)
                {   l=ReviseMulti(results-->2);
                    if (l~=0) { etype=l; break; }
                }
                if (take_all_rule==2 && results-->2 == actor)
                {   best_etype = NOTHING_PE; return -3;
                }
                #ifdef DEBUG;
                if (parser_trace>=1)
                    print "[Line successfully parsed]^";
                #endif;
                oops_from = 0;
                if (inferfrom~=0)
                {   
					print "("; PrintCommand(inferfrom); print ")^";
                }
                results-->0 = action_to_be;
                results-->1 = parameters;
                if (action_reversed && parameters==2)
                {   i = results-->2; results-->2 = results-->3;
                    results-->3 = i;
                    if (nsns == 2)
                    {   i = special_number1; special_number1=special_number2;
                        special_number2=i;
                    }
                }
                if (parameters > 0 && results-->2 >= 2)
                    PronounNotice(results-->2);
                if (not_holding~=0 && actor==player)
                {   notheld_mode=1;
                    for (i=0:i<8:i++) kept_results-->i = results-->i;
                    results-->0 = ##Take;
                    results-->1 = 1;
                    results-->2 = not_holding;
                   L__M(##Miscellany, 26, not_holding);
                }
                if (held_back_mode==1) { wn=hb_wn; return -4; }
                return -8;
            }
        }
        if (etype>best_etype) best_etype=etype;
        if (etype~=ASKSCOPE_PE && etype>nextbest_etype) nextbest_etype=etype;
	];
	[ParserParseUnrecogConv results;
		etype=best_etype;

	!  Errors are handled differently depending on who was talking.

	!  If the command was addressed to somebody else (eg, "dwarf, sfgh") then
	!  it is taken as conversation which the parser has no business in disallowing.

		if (actor~=player)
		{   if (usual_grammar_after>0)
			{   verb_wordnum = usual_grammar_after;
				return -7;
			}
			wn=verb_wordnum;
			special_word=NextWord();
			if (special_word==comma_word)
			{   special_word=NextWord();
				verb_wordnum++;
			}
			special_number=TryNumber(verb_wordnum);
			results-->0=##NotUnderstood;
			results-->1=2;
			results-->2=1; special_number1=special_word;
			results-->3=actor;
			consult_from = verb_wordnum; consult_words = num_words-consult_from+1;
			return -8;
		}
	];
	[ ParserShowError results m; results=results;
	!  If the player was the actor (eg, in "take dfghh") the error must be printed,
	!  and fresh input called for.  In three cases the oops word must be jiggled.

	if (ParserError(etype)~=0) return -1;

	pronoun_word = pronoun__word; pronoun_obj = pronoun__obj;

		if (etype==STUCK_PE)   { L__M(##Miscellany, 27); oops_from=1; }
		if (etype==UPTO_PE)    { L__M(##Miscellany, 28);
								for (m=0:m<32:m++) pattern-->m = pattern2-->m;
								pcount = pcount2; PrintCommand(0);L__M(##Miscellany, 56);
							}
		if (etype==NUMBER_PE)  L__M(##Miscellany, 29);
		if (etype==CANTSEE_PE) { L__M(##Miscellany, 30); oops_from=saved_oops; }
		if (etype==TOOLIT_PE)  L__M(##Miscellany, 31);
		if (etype==NOTHELD_PE) { L__M(##Miscellany, 32); oops_from=saved_oops; }
		if (etype==MULTI_PE)   L__M(##Miscellany, 33);
		if (etype==MMULTI_PE)  L__M(##Miscellany, 34);
		if (etype==VAGUE_PE)   L__M(##Miscellany, 35);
		if (etype==EXCEPT_PE)  L__M(##Miscellany, 36);
		if (etype==ANIMA_PE)   L__M(##Miscellany, 37);
		if (etype==VERB_PE)    L__M(##Miscellany, 38);
		if (etype==SCENERY_PE) L__M(##Miscellany, 39);
		if (etype==ITGONE_PE)
		{   if (pronoun_obj == NULL) L__M(##Miscellany, 35);
								else L__M(##Miscellany, 40);
		}
		if (etype==JUNKAFTER_PE) L__M(##Miscellany, 41);
		if (etype==TOOFEW_PE)  L__M(##Miscellany, 42, multi_had);
		if (etype==NOTHING_PE) { if (multi_wanted==100) L__M(##Miscellany, 43);
								else L__M(##Miscellany, 44);  }

		if (etype==ASKSCOPE_PE)
		{   scope_stage=3;
			if (indirect(scope_error)==-1)
			{   best_etype=nextbest_etype; return -3;  }
		}
	];
	[ParserLookForMore results i j; results=results;
		if (wn>num_words) return -8;
		i=NextWord();
		if (i==THEN1__WD or THEN2__WD or THEN3__WD or comma_word)
		{   if (wn>num_words)
			{   held_back_mode = false; return -9; }
			i = WordAddress(verb_wordnum);
			j = WordAddress(wn);
			for (:i<j:i++) i->0 = ' ';
			i = NextWord();
			if (i==AGAIN1__WD or AGAIN2__WD or AGAIN3__WD)
			{   !   Delete the words "then again" from the again buffer,
				!   in which we have just realised that it must occur:
				!   prevents an infinite loop on "i. again"

				i = WordAddress(wn-2)-buffer;
				if (wn > num_words) j = INPUT_BUFFER_LEN-1; 
				else j = WordAddress(wn)-buffer;
				for (:i<j:i++) buffer3->i = ' ';
			}
			Tokenise__(buffer,parse); held_back_mode = true; return -8;
		}
		best_etype=UPTO_PE; return -3;
	];
!======================================================================================
#endif; #ifnot; #ifndef ORParser_DONEGRAMMAR; constant ORParser_DONEGRAMMAR; #ifdef ORParser_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================