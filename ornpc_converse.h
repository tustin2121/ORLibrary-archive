!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.03.08 ORNPC_converse	[Z, GLULX]
! A module which anables NPCs to converse with the PC or other NPCs.  Implements a 
! generic TALK TO CHARACTER verb which picks a subject to converse about.
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
! AutoDep :	ORNPC_interact
!			ORNPC_doverb
!			ORNPC_asktelllearn
!			ORNPCVerb
!			ORNPC
!--------------------------------------------------------------------------------------
! NPC Componant Priority: 15  
! The lower the number, the further down the "class" line.  The greater the number, 
! the earlier it needs to come on the class line. Modules with the same priority can
! effectively be exchanged in order without effect.
!--------------------------------------------------------------------------------------
! Similar to the ORNPC_asktelllearn componant, this module extends the NPC's ability
! to react to ORKnowledgeTopic-related commands and enables him/her to actually initiate
! conversations with the player or other NPCs. ORKnowledgeTopics known by the NPC that
! have an "initiatable" property value of true are valid candidates for NPC initiated 
! conversation. Additionally, this module also adds support for "related topics" if the NPC is also 
! derived from the OROptionList class. 
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORNPC_converse";					!Constant USE_ORNPC_converse; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORNPC_converse; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.09 	Initial Creation (not really that this is the earliest date I could 
!				find without searching my code archive.)
! 2002.02.26	Fit to new template
! 2002.03.21	Made modifications to support the new ORNPC_CoverseWeb module.
!				Also added verb definition for talk sub (which magically disappeared!?!)
! 2002.04.08	Added test to ensure no random 0 call.  Also modified Talk sub to utilize 
!				a message rather than hard code text in the routine.
! 2002.04.09	Fixed random selection of topics pool bug.
! 2002.04.14	Fixed issue caused by .h removal.
! 2002.05.29	Modified message generated when someone tries to talk to themselves.
! 2002.07.07	Added menu documention
! 2002.07.13	Addessed code that prevented the "interacting_with" property from being 
!				handled programatically.
! 2002.07.15	Addessed code to prefer recent topics over older topics.
! 2002.09.02	Converted attributes previously defined in this module to properties.
! 2002.12.29	Added linefeeds to seperate NPC output from rest of text.
! 2003.01.18	Modified code to give message if player is trying to talk but has 
!				nothing to say.
! 2003.01.31	Reworking the whole linefeed concept in light of a posting to RAIF
!				from Andrew Plotkin. Also filled in some gaps in the functionality of 
!				"TalkSub."
! 2003.02.08	Made modification to self documentation. Reworking the whole linefeed 
!				concept in light of a posting to RAIF.
! 2003.02.16	Fixed typo in self documentation.
! 2003.02.19	Added better ifdef for self docs
! 2003.03.02	Fixed another typo in self documentation.
! 2004.01.30	Updated Auto Doc.
! 2004.02.01	Added autodep code for ORNPC.
! 2004.02.09	Optimized objectloops
! 2004.02.28	Fixed problems introduced with objectloops were optimized.
! 2004.03.08 	Removed unneeded reference to ORRoutineList.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORNPC_converse...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORNPC_converse forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORNPC_converse has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORReverseDirection_DONEREPLACE; default USE_ORReverseDirection 0; message "          [ORNPC_converse forcing inclusion of ORReverseDirection]"; #include "ORReverseDirection"; #endif; 
	#ifndef ORNPC_interact_DONEREPLACE; default USE_ORNPC_interact 0; message "          [ORNPC_converse forcing inclusion of ORNPC_interact]"; #include "ORNPC_interact"; #endif; 
	#ifndef ORNPC_doverb_DONEREPLACE; default USE_ORNPC_doverb 0; message "          [ORNPC_converse forcing inclusion of ORNPC_doverb]"; #include "ORNPC_doverb"; #endif; 
	#ifndef ORNPC_asktelllearn_DONEREPLACE; default USE_ORNPC_asktelllearn 0; message "          [ORNPC_converse forcing inclusion of ORNPC_asktelllearn]"; #include "ORNPC_asktelllearn"; #endif; 
	#ifndef ORNPCVerb_DONEREPLACE; default USE_ORNPCVerb 0; message "          [ORNPC_converse forcing inclusion of ORNPCVerb]"; #include "ORNPCVerb"; #endif; 
	#ifndef ORNPC_DONEREPLACE; default USE_ORNPC 0; message "          [ORNPC_converse forcing inclusion of ORNPC]"; #include "ORNPC"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORNPC_converse_DONEREPLACE; constant ORNPC_converse_DONEREPLACE; #ifdef ORNPC_converse_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORNPC_converse_DONEMESSAGE; constant ORNPC_converse_DONEMESSAGE;  #ifdef ORNPC_converse_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORNPC_converse requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNPC_converse_DONECODE;constant ORNPC_converse_DONECODE; #ifdef ORNPC_converse_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
#ifdef USE_OROptionList;
	OROptionList _olconverse;
#endif;
	[SelectRandomKnownTopicForActor o j opt;
#ifdef USE_OROptionList;
		if(actor ofclass OROptionList && actor.count()>0){
			_olconverse.ClearOptionList();
			for(j=actor.count()-1:j>=0:j--) { !--proceed backwards, applying a filter to all of the entries
				opt=actor.GetOption(j);
				if(opt provides LocateTarget && opt.LocateTarget(actor)~=0) {
					_olconverse.PushOption(opt,actor.GetAssociation(j));
				}
			}
			if(_olconverse.Count()==0) return -1;
			!--now we have a list of speakable topics in reverse order
			o=actor.GetAssociation(0);
			for(j=0:j<_olconverse.count():j++){
				if(actor.GetAssociation(j)~=o or -o) break;
			}
			j=random(j)-1; !--translate j (the number of possible topics of equal priority) to a specific topic #
			o=_olconverse.GetOption(j); !--get that specific option
			o=actor.LocateOption(o); !--lookup its position in the original list
			return o;
		}
#endif;
		return -1;
	];
	[FillTempArray o c;
		if(o ofclass ORKnowledgeTopic 
				&& o.isknownby(actor) 
				&& o.isincontext(noun)
				&& o provides initiatable 
				&& valueorrun(o,initiatable)==true
				&& (o.hasBeenSpokenOfBy(actor)==false || (o provides repeatable && valueorrun(o,repeatable)==true))){
			npctemparray-->0=(npctemparray-->0)+1;
			npctemparray-->(npctemparray-->0)=o;
		}
		objectloop(c in o) FillTempArray(c);
	];
	[TalkSub o tosay isask; !routine to continue telling about a scripted topic, or to pick a new topic 
		tosay=0;
		isask=false;
		!--- if already talking about something, then just continue
		if(tosay==0 && actor provides current_subject && actor.current_subject~=0){
			tosay=actor.current_subject;
		}
		!--if have not chosen a topic, then let us see if we have a pool of topics and if so, choose one...
		if(tosay==0) {
			o=SelectRandomKnownTopicForActor();
			if(o>-1){
				tosay=actor.GetOption(o);
				if(actor.GetAssociation(o)<0) isask=true;
			}
		}
		!--- if we still have not selected a topic, then come up with something random
		if(tosay==0){
			npctemparray-->0=0;
			objectloop(o in ORKnowledgeTopicReserve){
				FillTempArray(o);
			}
			if(npctemparray-->0>0) 
				tosay=npctemparray-->(random(npctemparray-->0));
			else {!--character has run out of things to say, so let him forget he said anything and start over (after he thinks about it this turn)
				objectloop(o ofclass ORKnowledgeTopic !--not efficient, but should be seldom done, so probably okay.
							&& o.isknownby(actor)
							&& o provides initiatable 
							&& valueorrun(o,initiatable)==true) {
						o.hasBeenSpokenOfBy(actor,0);
					}
			}
		}

		if(tosay~=0) {
			if(isask==true)
				<AskTopic noun tosay>;
			else
				<TellTopic noun tosay>;
				if(tosay.hasBeenSpokenOfBy(actor)) actor.current_subject=0; !--if completed talking, then don't repeat
		}
		else if(CanPlayerWitness()) {
			if(actor provides speechless && actor.speechless~=0){ !--if the speechless property is defined then print it.
				if(actor==player) print "^";
				PrintOrRun(actor,speechless);
				print "^";
			}else{
				if((~~(actor provides speechless))||actor==player){ !--print the default message for 1) the player (who doesn't define speechless) because he is trying to talk and needs a response, and 2) NPCs that don't even provide speechless, such as NPCs not derived from ORNPC_converse.  We'll try to talk to them too.
					L__M(##Talk,1);
				}
			}
			!--All that is left are NPCs the provide speechless and define it as 0.  This is effectly the same as turning off the speechless message so we'll do nothing here.
		}
		return;
	];
	object _onc_init LibraryExtensions
		with ext_messages[;
			Talk: print (ig)CIVerb(actor,"started","start")," to say something to ";
				if(noun==actor)
					print (Myself)noun;
				else 
					print (TheMe)noun;
				print ", but words ",(ig)ppf("failed ","fail "),(TheMe)actor,".^";
		]
		,	initiatable !--dummy definition
		,	repeatable !--dummy definition
	;

	class ORNPC_converse
		with	can_converse true
		,		current_subject	0
		,		heartbeat[;
					if(self.current_subject~=0){ !---if currenly talking to someone...
						if(testscope(self,valueorrun(self,interacting_with))==false){ !---person we were talking to has walked away
							self.disengage();
							self.current_subject=0;
							rfalse;
						}
						if(self.has_already_interacted()==false) self.act_converse();
						rtrue;
					}
					rfalse;
				]
		,	act_converse[abletest;
				if(abletest==true) {
					if(valueorrun(self,has_already_interacted)==true) rfalse;
					if(self.engage_person(true)==0) rfalse;
					return ValueOrRun(self,can_converse);
				}
				if(valueorrun(self,interacting_with)==0) self.engage_person();
				self.DoVerb(##Talk,valueorrun(self,interacting_with));
			]
		,	ProcessDialog[talkto procedure knobj info s1 p1 s2 p2;
				if((self provides procedure)==false) rfalse;
				s1=(knobj.&info-->0);
				p1=knobj.&info-->1;
				s2=0; !default to nothing
				if((knobj.#info/WORDSIZE)>2){
					s2=knobj.&info-->2;
					p2=knobj.&info-->3;
				}
				self.procedure(talkto, s1,p1,s2,p2);
				knobj.FlushDialog(info,4); !print out remaining text
				rtrue;
			]	
		,	TellLine[before s1 canreplace p1 after;!print a single portion of text
				if(s1 && CanPlayerWitness(self)){
					print (dynastring)before;
					print (dynastring)s1;
					if(canreplace)print (dynastring)p1; !can replace punctuation?
					print (dynastring)after;
					return true;
				}
				return false;
			]
		,	PersonalizeTell[talkto s1 p1 s2 p2; !override for npc formating
				if(self.TellLine("~",s1,p1,",","~ "))
					print (ig)CIVerb(self,"said","say")," to ",(me)talkto,". ";
				self.TellLine("~",s2,p2,".","~ ");
			]
		,		PersonalizeAsk [talkto s1 p1 s2 p2; !over-ride for npc formatting
				if(self.TellLine("~",s1,p1,",","~ "))
					print (ig)CIVerb(self,"asked","ask")," ",(me)talkto,". ";
				self.TellLine("~",s2,p2,".","~ ");
			]
		,	Speechless[; !--by default, just print the default message
					!--print "^^";
					L__M(##Talk,1);
			]
	;
	object _npcconverseinit LibraryExtensions
		with ext_initialise[o; 
				objectloop(o ofclass ORNPC_converse){
					if((o ofclass ORNPC_doverb)==false) print_ret "^[ERROR: ORNPC_converse behavior requires ORNPC_doverb behavior.]^";
					if((o ofclass ORNPC_asktelllearn)==false) print_ret "^[ERROR: ORNPC_converse behavior requires ORNPC_asktelllearn behavior.]^";
					if((o ofclass ORNPC_interact)==false) print_ret "^[ERROR: ORNPC_converse behavior requires ORNPC_interact behavior.]^";
					o.register_action(act_converse);
				}
			]
	;

!======================================================================================
#endif; #ifnot; #ifndef ORNPC_converse_DONEGRAMMAR; constant ORNPC_converse_DONEGRAMMAR; #ifdef ORNPC_converse_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	Verb 'talk' 
		* 'to' creature ->Talk
		* creature ->Talk
	;
	extend 't' first
		* 'to' creature ->Talk
		* creature ->Talk
	;
#ifdef ORLibDoc;
	!--Commands, document thyselves.
		ORMenu ORHelpTalk "Talk" ORLibDoc
			with text[;
				print "The ~Talk~ command has been implemented. Be forewarned, that unlike other games, where ~Talk~ is the sole method of conversing, this command is meant to augment the Ask/Tell paradigm.  This command is not guaranteed to further a script. ~Talk~ simply chooses a random topic from the player's running pool of potential subjects. It can nevertheless be useful when you are uncertain what to say:^^";
				font off; 
				spaces(4);print ">TALK TO GIRL";
				font on;
			]
	;
#endif;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================