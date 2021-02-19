!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2006.01.16 ORKnowledgeTopic	[Z, GLULX]
! This class provides the basis for ASK/TELL Topic-based conversations
! including learable conversations as well as scripted conversations, and 
! NPC initiatable conversations that do not repeat.
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
! AutoDep:	ORDynaString
!			OROptionList
!			ORPronoun
!--------------------------------------------------------------------------------------
! A more detailed description of this file, what it does, and how to use it goes here.
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORKnowledgeTopic; #include "ORKnowledgeTopic"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORKnowledgeTopic; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.10.01	Initial Creation (not really, but this is the earliest date that I can
!				find without searching my code archive)
! 2002.02.08	Added GLULX support
! 2002.02.26	Fit to new framework.  Also fixed several message related bugs.
! 2002.03.31	Added support for converstation webs.
! 2002.04.01	Added AutoDep code ORDynaString
! 2002.04.08	Modified to support OROptionList module rather than the previously 
!				considered and discarded ORNPC_ConverseWeb routine.
! 2002.04.14	Fixed issue caused by .h removal.
! 2002.04.19	Added additional return between Ask and Tell-Response
! 2002.04.21	Fixed bug with TellTopicSub where nothing topic gets through.
! 2002.04.23	Added AutoDependancy of ORLibaryMessages module
! 2002.07.07	Added menu documentation.
! 2002.07.13	Added IsSpeaking and IsSpokenTo members to make available who is 
!				actually speaking and who to. Additionally added code to better select
!				who is meant when using vague Ask/Tell, and Code to better support 
!				group conversations (and removing topics from consideration when someone
!				else has said it.)
! 2002.07.17	Fixed bug with HasBeenSpokenOfBy() test, causing "testee" to learn the topic.
! 2002.07.20	Modified documentation. 
! 2002.07.22	Added a new context property to ease simple context definitions
!				Also fixed bugs in the locate target code.
! 2002.08.23	Fixed bug related to NPC's being talked to are present
!				but not in the same location (e.g. sitting on a chair)
! 2002.09.02	Converted attributes previously defined in this module to properties.
! 2002.10.19	Added autodep for ObjectInitialise 
! 2002.12.29	Added linefeeds to seperate NPC output from rest of text, and removed 
!				it from the ASK code.
! 2003.01.18	Fixed issues very ugly issues with AnswerTopic grammar.
! 2003.01.31	Reworking the whole linefeed concept in light of a posting to RAIF
!				from Andrew Plotkin.
! 2003.02.04	Added additional code to utilize the isincontext functionality for 
!				asking and telling. Also fixed IDunno_msg bug.
! 2003.02.06	Eliminated the default number of blank entries of the properties 
!				KnownBy and ToldBy.  These can (and should) be set to better values 
!				in derived classes or instances of this class, but they are being 
!				eleminated from this base class to keep from allocating readable 
!				memory needlessly.
! 2003.02.08	Worked on some additional linefeed issues.
! 2003.02.19	Added better ifdef for self docs
! 2003.03.04	Revised the error printed when a topic is not recognized.
! 2003.04.19	Fixed bug that could possibly lead to run-on error messages (return -> rtrue).
! 2004.01.11	Modified to use the stdlib 6/11b2 new ext_initialise functionality
! 2004.01.30	Modified self documentation.
! 2004.02.09	Added objectloop optimizations.
! 2004.02.25	Added autodep code for ORPronoun 
! 2004.02.28	fixed bugs introduced in objectloop optimizations.
! 2006.01.08	Removed the reference to the DICT_PREP constant, introduced in the 
!				as-of-yet-un-released 6.31 version of the library, which caused the 
!				module to not compile under 6.30.
! 2006.01.16	Added additional code to allow for the program to detect to whom something 
!				is said, and thereby gain filterin ability earlier in the parsing process.
!				Making "Tell hello to alexander" and "Tell hello to sasha" work as expected.
!				Also, added code sent in by Jason Ermer to address specific occurances where the 
!				"I dunno" message is not called in non-topic specific instances.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORKnowledgeTopic...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORKnowledgeTopic forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORKnowledgeTopic has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef OREntryPoints_DONEREPLACE; default USE_OREntryPoints 0; message "          [ORKnowledgeTopic forcing inclusion of OREntryPoints]"; #include "OREntryPoints"; #endif; 
	#ifndef ORDynaString_DONEREPLACE; default USE_ORDynaString 0; message "          [ORKnowledgeTopic forcing inclusion of ORDynaString]"; #include "ORDynaString"; #endif; 
	#ifndef OROptionList_DONEREPLACE; default USE_OROptionList 0; message "          [ORKnowledgeTopic forcing inclusion of OROptionList]"; #include "OROptionList"; #endif;    	
    #ifndef ORPronoun_DONEREPLACE; default USE_ORPronoun 0; message "          [ORKnowledgeTopic forcing inclusion of ORPronoun]"; #include "ORPronoun"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORKnowledgeTopic_DONEREPLACE; constant ORKnowledgeTopic_DONEREPLACE; #ifdef ORKnowledgeTopic_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	global				playeristalkingto;
!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORKnowledgeTopic_DONEMESSAGE; constant ORKnowledgeTopic_DONEMESSAGE;  #ifdef ORKnowledgeTopic_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORKnowledgeTopic requires the OREnglish file.";#endif; 
! peek ahead at what will likely be in the second variable, once the parser gets their...
	[ GuessNounAfterPrep savewn retval highest_score current_score o l slon;
		slon=number_matched; !length_of_noun;
		highest_score=0;
		savewn=wn; 
		objectloop(o in actors_location){
			current_score=0;
			wn=1;
			while (wn < num_words) { l=NextWord(); !position wn to follow a
				if (l && (l->#dict_par1) & $08) !preposition
					current_score=TryGivenObject(o); !then get its score
			}
			if(current_score==highest_score) retval=0; !we can't tell the difference between two objects, so lets not assume either
			if(current_score>highest_score) {
				highest_score=current_score;
				retval=o;
			}
		}
		wn=savewn;
		number_matched=slon;
		return retval;
	];
	class ORKnowledgeTopic has proper
		with TopicInformation "~Yes, I know about that but I think I'll keep it to myself.~"
		, query 0
		, AskedAgain 0
		, KnownBy 0 0 
		, ToldBy 0 0 
		, IsSpeaking 0
		, IsSpokenTo 0
		, KnownAt[who t;
				if(self.IsKnownBy(who)==false && valueorrun(self,learnable)==true) self.MemorizeFor(who);
				for(t=0:t<(self.#KnownBy)/WORDSIZE:t++){
					if(self.&KnownBy-->t==who) return t;
				}
				return 0;
			]
		, chooseobject[code targetnpc; !TODO: Do we really need this, the scope rule to this, I believe
			if(code<2)return 0;
				if(noun==0){ !noun has not been identified yet, so we're in the the "tell hello to mat" mode
					targetnpc=GuessNounAfterPrep();	!to whom are we probably talking?
					if(self.IsInContext(targetnpc, actor)==true) return 2; 
				}else{ !noun is identified, so we're in the the "tell mat hello" mode
					if(self.IsInContext(noun, actor)==true) return 2; 
				}
				return 1;
			]
			, HasBeenSpokenOfBy[o value; 
				if(self.isknownby(o)==false) return false;
				if(value==true) self.&ToldBy-->(self.KnownAt(o))=true;
				value=self.&ToldBy-->(self.KnownAt(o));
				return value;
			]
		, IsInContext[to from retval; 
				retval=-1; !--default to not yet determined
				if(self.&context-->0~=0) retval=wordinproperty(to,self,context); !--if there are values in the context property list
				if(retval==-1 && parent(self) provides IsInContext) retval=parent(self).IsInContext(to,from);  !--if no context has been determined, ask parent
				if(retval==-1) retval=true; !--if nothing told us a context either way, then assume true
				return retval;
			]
		, Context
		, _matchtargetcriteria[to from;
				if(self.isknownby(to)==false && self.isknownby(from)==true && self.isincontext(to,from)==true ) rtrue; !--yeah, we can tell them this
				if(self.isknownby(to)==true && self.isknownby(from)==false && self.isincontext(from,to)==true ) rtrue; !--yeah, we can ask about this information
				rfalse;
			]
		, LocateTarget[from o; 
					if(from==0) from=actor;
					objectloop(o near from){
						if(o has animate or talkable && o~=from && self._matchtargetcriteria(o,from)==true) return o; !--return the first response in the objectloop for which the contexts match
					}
					return 0;
				]
		, LocateTargetCount[from o c; 
					if(from==0) from=actor;
					objectloop(o near from){
						if(o has animate or talkable && o~=from && self._matchtargetcriteria(o,from)==true) c++;
					}
					return c;
				]
		, IsKnownBy[npc t el parnt;
				if(parent(self)==CommonKnowledge) return true;
				for(t=0:t<(self.#KnownBy)/WORDSIZE:t++) {
					el=self.&KnownBy-->t; !current element in list
					if(el==npc) return true; !if the current element in list is the npc param
					parnt=parent(self);
					if(parnt ~=0 
						&&	parnt provides IsKnownBy 
						&&	parnt.IsKnownBy(npc)==true) 
						return true; !or the npc is contained by the current element
				}
				return false;]
		, MemorizeFor[obj t;
				if(self.IsKnownBy(obj)) return true; !already known
				for(t=0:t<(self.#KnownBy)/WORDSIZE:t++) {
					if(self.&KnownBy -->t==0){
						self.&KnownBy -->t=obj;
						rtrue;
					}
				}
#ifdef DEBUG;
					print "[ERR: out of room in property ~KnownBy~ in object ",(name)self," ]";
#endif;
				return false;]
		, TellAbout[to from forcetellagain prop suppresstext;
				self.isspeaking=from;
				self.isspokento=to;
				prop=TopicInformation;
				if(forcetellagain==false){
					if(self.hasBeenSpokenOfBy(from)){ !---have we already talked about this? Does this topic offer an alternate output for previously stated topics?
						if(valueorrun(self,learnable)==false || (valueorrun(self,learnable)==true && self.IsKnownBy(to)==true)){ !-- is this already known to the tellee? (ignore this check if the topic cannot be learned)
							if(self.AskedAgain~=0){
								prop=AskedAgain;
							}
						}
					}
				}
				if(from provides interacted ) from.interacted();
				if(WillPlayerHear(to,from)){
					if((self.#prop/WORDSIZE)>1){
						suppresstext=false;
						if(from provides ProcessDialog) suppresstext=from.ProcessDialog(to, PersonalizeTell, self, prop);
						if(suppresstext==false){
#ifdef ORNPCControl;
						if(ORNPCControl.ornpcs_action==true || actor~=player) print "^"; !--if an NPC is either 1) initiating this action, or 2) performing an automatic responce to something the player did (like answering a question) then provide a linefeed.
#ifnot;
						if(actor~=player) print "^"; !--if an NPC then must be performing an automatic responce to something the player did (like answering a question) then provide a linefeed.
#endif;
							print (ig)CIVerb(from,"said","say");
							if(from has animate)print " to ",(theme)to;
							print ", ";
							self.FlushDialog(prop,0);
							print "^";
						}
					}
					else {
#ifdef ORNPCControl;
						if(ORNPCControl.ornpcs_action==true || actor~=player) print "^"; !--if an NPC is either 1) initiating this action, or 2) performing an automatic responce to something the player did (like answering a question) then provide a linefeed.
#ifnot;
						if(actor~=player) print "^"; !--if an NPC then must be performing an automatic responce to something the player did (like answering a question) then provide a linefeed.
#endif;

						PrintOrRun(self,prop, true);
						print"^";
					}
					self.hasBeenSpokenOfBy(from,true);
				}
				if(valueorrun(self,learnable)==true)  Self.MemorizeFor(to);
			]
		, AskAbout[askwho askby;
				self.isspeaking=askby;
				self.isspokento=askwho;
				if(self.query==0) return;
				if(WillPlayerHear(askwho,askby)){
					if((self.#query/WORDSIZE)>1 && askwho has animate or talkable){
						if(askby provides ProcessDialog) 
							askby.ProcessDialog(askwho, PersonalizeAsk,self,query);
						else{

							if(actor~=player) print "^"; !--if an NPC then provide a linefeed.
							print (ig)CIVerb(askby,"asked","ask"),", ";
							self.FlushDialog(query,0);
							print "^";
						} 
					}
					else {
						if(actor~=player) print "^"; !--if an NPC then provide a linefeed.
						PrintOrRun(self,query, true);
						print"^";
					}
				}
			]
		, FlushDialog[info start finalpunct t ;
				if(finalpunct==0) finalpunct=".";
				if((self.#info/WORDSIZE)<=start) return;
				if(CanPlayerWitness()){
					print"~";
					for(t=start:t<(self.#info/WORDSIZE):t=t+2){
						if(t>start && self.&info-->t~=0) print " ";
						if(self.&info-->t~=0){
							print (dynastring) self.&info-->t;
							if(self.&info-->(t+1)~=0) print (dynastring) self.&info-->(t+1);
							else print (dynastring)finalpunct;
						}
					}
					
					print"~ ";
				}
				
			]
		,	ProcessDialog  !---default implementations so that ORNPC is not required to be implemented
		,	PersonalizeTell
		,	PersonalizeAsk
		,	interacted
		,	idunno_msg
		,	already_knows
		,	learnable false
		,	rhetorical false
	;

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORKnowledgeTopic_DONECODE;constant ORKnowledgeTopic_DONECODE; #ifdef ORKnowledgeTopic_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	Object CommonKnowledge with interacting_with; !--just provide dummy instance for interacting_with
!======================================================================================
   #endif; #ifnot; #ifndef ORKnowledgeTopic_DONEGRAMMAR; constant ORKnowledgeTopic_DONEGRAMMAR; #ifdef ORKnowledgeTopic_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	#ifndef WillPlayerHear; [WillPlayerHear; rtrue;]; #endif;
	#ifndef CanPlayerWitness; [CanPlayerWitness; rtrue;]; #endif;

	object ORKnowledgeTopicReserve;

	object _okntop_init LibraryExtensions
		with ext_messages[;
			Tell: if(lm_n==2){ 
					print (ig)CIVerb(actor,"did","do","does")," not know anything about that.^"; 
					rtrue;
				  }
			TellTopic:
				if(lm_n==1){
					print "That topic ",(ig)ppf("did","does")," not seem appropriate for the given context.^"; 
					rtrue;
				}
			Ask: 
				if(lm_o provides IDunno_msg)		!-- Added JLE 2006.1.8
				    printorrun(lm_o,Idunno_msg);	!-- Added JLE 2006.1.8
				else								!-- Added JLE 2006.1.8
					print "~I don't know anything about that,~ ", (ig)CIVerb(lm_o,"said","say"),".^"; 
				rtrue;
			]
		,	ext_initialise[o;
				objectloop(o ofclass ORKnowledgeTopic && parent(o)==0) move o to ORKnowledgeTopicReserve; !--consolodate all locationless topics in a single location so that searches will be more efficient
			]
	;

	[NotCreature; if(noun has animate or talkable) rfalse; rtrue;];

	[TopicAndChildrenInTargetTest o knwn c; !isolate information known by the target
		if(o ofclass ORKnowledgeTopic && o.IsKnownBy(knwn)==true) {!inputobjs-->2 == noun...
				PlaceInScope(o); 
		}
		objectloop(c in o) TopicAndChildrenInTargetTest(c,knwn);
	];
	[TopicAndChildrenInActorTest o targetnpc c; !isolate information known by the target
 		if(o ofclass ORKnowledgeTopic && o.IsKnownBy(actor)==true && (targetnpc==0 || o.isincontext(targetnpc)==true)) {
				PlaceInScope(o);
		}
		objectloop(c in o) TopicAndChildrenInActorTest(c,targetnpc);
	];
	[TopicInTargetScope o; !isolate information known by the target
		switch(scope_stage){
			1: return false;
			2:	objectloop(o in ORKnowledgeTopicReserve){
					TopicAndChildrenInTargetTest(o,inputobjs-->2);
				}
				rtrue;
			3: print_ret "Who ",(italics)"exactly"," do you want me to talk to?^";
		}
	];

	[TopicInVagueTarget o targ; !isolate information known by the target
		targ=ResolveActorTalkingTo();	
		switch(scope_stage){
			1: return false;
			2: if(targ>0){
					objectloop(o in ORKnowledgeTopicReserve){
						TopicAndChildrenInTargetTest(o,targ);
					}
				}
				rtrue;
			3: 	if(targ>0){
					if(targ provides IDunno_msg)
						printorrun(targ,Idunno_msg);
					else print (ig)CIVerb(targ,"did","do","does"),"not appear to know the answer to that.^";
				}
				else print "Who ",(italics)"exactly"," do you want me to talk to?^";
		}
	];
	[TopicInActor o val targetnpc; !isolate information known by the actor
		switch(scope_stage){
			1: return false;
			2: 	for(val=0:val<pcount:val++) if(metaclass(pattern-->val)==object && ((pattern-->val) has animate)) targetnpc=(pattern-->val); !--scan the pattern parsed so far for the NPC being addressed
				if(targetnpc==0)targetnpc=GuessNounAfterPrep(); !--if not found yet, then lets peek ahead and assume we are using the "say blah to npc" format

				objectloop(o in ORKnowledgeTopicReserve) 
					TopicAndChildrenInActorTest(o,targetnpc);
				return true;
			3: 	print_ret "What topic did you mean",(italics)" exactly","?";
		}
	];
	[AskTopicSub;
		if(noun==actor) {
			L__M(##Tell, 1);
			rtrue;
		}
		else {
			second.AskAbout(noun, actor);
			if(actor provides interacting_with) actor.interacting_with=noun;
			else if(actor==player) playeristalkingto=noun;
			RunLife(noun,##AskTopic);
		}
		rtrue;
	];  
	[AnswerTopicSub;
		if(second==actor) {
			L__M(##Tell, 1);
		}
		else {
			noun.TellAbout(second, actor);
			if(actor provides interacting_with) actor.interacting_with=second;
			else if(actor==player) playeristalkingto=second;
			RunLife(second,##AnswerTopic);
		}
		
		rtrue;
	];  
	[ConsultTopicSub; second.TellAbout(actor,noun,true); ]; 
	[TellTopicSub; 
		if(second==0) {
			L__M(##Tell, 2);
			return;
		}
		print "a^";
		if(noun==actor) 
			L__M(##Tell, 1);
		else {
			print "b^";
			if(second.IsInContext(noun)==false){
#ifdef ORDEBUG;
				print"return not in context for ",(object)second, " to ",(object)noun,".  Actor is ",(object)actor,"^";
#endif;
				L__M(##TellTopic, 1);
				return;
			}
			print "c^";
			if(noun provides already_knows) noun.already_knows=second.IsKnownBy(noun);
			print "d^";
			if(action_to_be==##TellTopic or ##VagueTellTopic) {
					second.TellAbout(noun,actor,true); 
			}
			else {!--otherwise, a redirection from another verb.  Mostlikely the response to "ask"
				second.TellAbout(noun,actor);  
			}
			print "e^";
			if(actor provides interacting_with) actor.interacting_with=noun;
			else if(actor==player) playeristalkingto=noun;
			
		}
		RunLife(noun,##TellTopic);
	]; 
	OROptionList _ol with optionlist 0 0 0 0 0 0 0;
	[ResolveActorTalkingTo obj o t cnt;
		
		if(actor provides interacting_with) obj=valueorrun(actor,interacting_with);
		if(actor==player && playeristalkingto~=0) obj=playeristalkingto;
		if(obj>0 && ScopeCeiling(obj)==ScopeCeiling(actor)) return obj; !--we're already talking to someone, lets assume them.
		playeristalkingto=0;
		obj=0;
		
		_ol.ClearOptionList();
		!--lets try and deduce who we are talking to...
		objectloop(o near actor){
			if(o has animate && actor~=o) _ol.PushOption(o);
		}
		if(_ol.Count()==0) return 0; !--nobody here to talk to, error out.
		if(_ol.count()==1) return _ol.GetOption(0); !--only one person here, so lets talk to them.
		cnt=_ol.count(); !--lets save off the original number of possibilities, before we begin to eliminate possibilities
		!--okay, there's more than one person here... Let's use some fuzzy logic and see if one of the people meets better criteria than the others
		!--does the topic context fit the person we are talking to?
		if(noun ofclass ORKnowledgeTopic && noun provides isincontext) {
			for(t=0:t<_ol.count():t++){
				o=_ol.GetOption(t);
				if(noun.IsInContext(o,actor)==false) {
					_ol.RemoveOption(o);
					t--;
				}
			}
		}
		if(_ol.count()==1) return _ol.GetOption(0); !--only one person left, so lets talk to them.
		!--does the person already know what we are trying to say?
		if(noun ofclass ORKnowledgeTopic && noun provides isknownby) {
			for(t=0:t<_ol.count():t++){
				o=_ol.GetOption(t);
				if(noun.IsKnownBy(o)==true) {
					_ol.RemoveOption(o);
					t--;
				}
			}
		}
		if(_ol.count()==1) return _ol.GetOption(0); !--only one person left, so lets talk to them.
		!--are any of the people already talking to us?
		for(t=0:t<_ol.count():t++){
			o=_ol.GetOption(t);
			if((o provides interacting_with)==false || valueorrun(o,interacting_with)~=actor) {
				_ol.RemoveOption(o);
				t--;
			}
		}
		if(_ol.count()==1) return _ol.GetOption(0); !--only one person left, so lets talk to them.
		return -cnt; !--unable to narrow the list down to one and only one, so return negative number of possible people
	];
	[VagueAskTopicSub o; 
		o=ResolveActorTalkingTo();
		if(o==0) print_ret "There ",(ig)ppf("was","is","will be")," nothing to speak with.";
		if(o<0) print_ret "You'll need to be more specific. Who ",(italics)"exactly"," did you want me to speak with?";
		second=noun;
		noun=o;
		print "(Asking ",(name)o,")^";
		<<AskTopic noun second>>;
	]; 
	[VagueTellTopicSub o; 
		o=ResolveActorTalkingTo();
		if(o==0) print_ret "There ",(ig)ppf("was","is","will be")," nothing to speak with.";
		if(o<0) print_ret "You'll need to be more specific. Who ",(italics)"exactly"," did you want me to speak with?";
		second=noun;
		noun=o;
		print "(Telling ",(name)o,")^";
		<<TellTopic noun second>>;
	]; 
	[VagueAskSub o; 
		o=ResolveActorTalkingTo();
		if(o==0) print_ret "There ",(ig)ppf("was","is","will be")," nothing to speak with.";
		if(o<0) print_ret "You'll need to be more specific. Who ",(italics)"exactly"," did you want me to speak with?";
		second=noun;
		noun=o;
		print "(Asking ",(name)o,")^";
		<<Ask noun second>>;
	]; 
	[VagueTellSub o; 
		o=ResolveActorTalkingTo();
		if(o==0) print_ret "There ",(ig)ppf("was","is","will be")," nothing to speak with.";
		if(o<0) print_ret "You'll need to be more specific. Who ",(italics)"exactly"," did you want me to speak with?";
		second=noun;
		noun=o;
		print "(Telling ",(name)o,")^";
		<<Tell noun second>>;
	]; 
	extend 'consult' first 
		* noun=notcreature 'on'/'about' scope=TopicInTargetScope-> ConsultTopic
		* creature 'about'/'on' scope=TopicInTargetScope -> AskTopic
	;

	extend 'ask' replace
		* creature 'about' scope=TopicInTargetScope	-> AskTopic
		* creature 'about' topic					-> Ask
		* creature scope=TopicInTargetScope			-> AskTopic
		* creature topic							-> Ask
		* 'about' scope=TopicInVagueTarget			-> VagueAskTopic
		* 'about' topic								-> VagueAsk	
		* scope=TopicInVagueTarget					-> VagueAskTopic
		* topic										-> VagueAsk
	;
	Extend 'tell' replace 
		* creature 'about' scope=TopicInActor	-> TellTopic
		* creature 'about' topic				-> Tell
		* creature scope=TopicInActor			-> TellTopic 
		* creature topic						-> Tell
		* 'about' scope=TopicInActor			-> VagueTellTopic
		* 'about' topic							-> VagueTell
		* scope=TopicInActor					-> VagueTellTopic
		* scope=TopicInActor 'to' creature		-> TellTopic reverse
		* topic	'to' creature 					-> TellTopic reverse
		* topic									-> VagueTell
	;
	verb 't' ='tell';
	verb  'a' ='ask';
	Extend 'answer' first
		* 'to' creature  scope=TopicInActor		-> AnswerTopic reverse
		* 'to' creature  topic					-> Answer reverse
		* scope=TopicInActor 'to' creature		-> AnswerTopic 
	;
#ifdef ORLibDoc;
	!--Commands, document thyself.
		ORMenu ORHelpAskTell "Ask/Tell" ORLibDoc 
		  with text[;
			print "This game employs the Ask/Tell model for conversation. It should be noted, however, that slight improvements have been made to the system traditionally employed. Of particular noteworthiness is the attempt to determine the person that is being addressed if unspecified. For example, the command:^^";
			font off; 
			spaces(4);print">TELL ABOUT BREATHMINTS^^";
			font on; 
			print"will cause the parser to make some suppositions and could possibly result in a command like:^^";
			font off; 
			spaces(4);print">TELL TROLL ABOUT BREATHMINTS.^^";
			font on;
			print "This same behavior applies to the ~Ask~ command as well.^^In addition, the abbreviations ~A~ and ~T~ have been implemented for ~Ask~ and ~Tell~ respectively, so the above example could also be worded as:^^";
			font off; 
			spaces(4);print">T BREATHMINTS";
			font on;
		]
	;
#endif;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================