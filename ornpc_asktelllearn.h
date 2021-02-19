!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORNPC_AskTellLearn	[Z, GLULX]
! A module which endows an NPC with the ability to answer questions and learn 
! new information that has been told to it.  This module leverages the ORKnowledgeTopic
! module.
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
! AutoDep :	ORKnowledgeTopic
!--------------------------------------------------------------------------------------
! NPC Componant Priority: 10  
! The lower the number, the further down the "class" line.  The greater the number, 
! the earlier it needs to come on the class line. Modules with the same priority can
! effectively be exchanged in order without effect.
!--------------------------------------------------------------------------------------
! When derived from this componant, NPCs can answer asked questions, respond to topics 
! being told, and learn new topics. In short, this enables NPCs to respond to 
! ORKnowledgeTopic-related commands.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORNPC_AskTellLearn";					!Constant USE_ORNPC_AskTellLearn; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORNPC_AskTellLearn; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.05	Initial Creation (not really, but this is the earliest date I could 
!				find without searching my code archive)
! 2002.02.26	fit to new template
! 2002.03.23	added property to quash generic responses
! 2002.04.14	Fixed issue caused by .h removal.
! 2002.08.21	Added Auto dependancies for OROptionList and ORLibraryMessages 
!				(which were just recently added to ORKnowledgeTopic)
! 2002.09.02	Converted attributes previously defined in this module to properties.
! 2003.01.18	Fixed an ancient bug in the life routine which handled AnswerTopic.
!				Specifically, the code to swap noun and second assigned a zero.
! 2003.01.31	Reworking the whole linefeed concept in light of a posting to RAIF
!				from Andrew Plotkin.
! 2004.01.12	Removed Auto dependancy code for orlibrarymessages, since similar 
!				functionality is now included in the stdlib, 6/11.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORNPC_AskTellLearn...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORNPC_AskTellLearn forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORNPC_AskTellLearn has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef OROptionList_DONEREPLACE; default USE_OROptionList 0; message "          [ORNPC_AskTellLearn forcing inclusion of OROptionList]"; #include "OROptionList"; #endif;    	
	#ifndef ORDynaString_DONEREPLACE; default USE_ORDynaString 0; message "          [ORNPC_AskTellLearn forcing inclusion of ORDynaString]"; #include "ORDynaString"; #endif; 
	#ifndef ORKnowledgeTopic_DONEREPLACE; default USE_ORKnowledgeTopic 0; message "          [ORNPC_AskTellLearn forcing inclusion of ORKnowledgeTopic]"; #include "ORKnowledgeTopic"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORNPC_AskTellLearn_DONEREPLACE; constant ORNPC_AskTellLearn_DONEREPLACE; #ifdef ORNPC_AskTellLearn_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	global ignore_dialog=false;
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORNPC_AskTellLearn_DONEMESSAGE; constant ORNPC_AskTellLearn_DONEMESSAGE;  #ifdef ORNPC_AskTellLearn_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORNPC_AskTellLearn requires the OREnglish file.";#endif; 
	[WillPlayerHear one two; 
		if(one==0)one=actor;
		if(two==0)two=actor;
		if(CanPlayerWitness(one)==false && CanPlayerWitness(two)==false) rfalse;
		if(player==one or two) rtrue;
		if(ignore_dialog==false && one has animate or talkable && two has animate or talkable) rtrue;
		rfalse; 
	];
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNPC_AskTellLearn_DONECODE;constant ORNPC_AskTellLearn_DONECODE; #ifdef ORNPC_AskTellLearn_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	class ORNPC_AskTellLearn
		with	acknowledge_conversation true
			,	AlreadyKnewResponse_msg [;
						if(ValueOrRun(self,acknowledge_conversation)==true) print "^~I knew that already,~ ",(ig)IVerb(self,"said","say"),".^";						
					]
			,	LearnedResponse_msg [;
						if(ValueOrRun(self,acknowledge_conversation)==true) print "^~Really?~ ",(ig)CIVerb(self,"said","say"), ".  ~I'll have to remember that.~^";
					]
			,	UnlearnedResponse_msg [;
						if(ValueOrRun(self,acknowledge_conversation)==true) print "^",(ig)CIVerb(self,"nodded","nod")," ",(My)self," head in acknowledgment.^";	
					]
			,	IDunno_msg [; 
						print "^~I'm afraid I don't know anything about that,~ ",(TheI)self," ",(ig)ppf("said","says"),".^";
					]	
			,	already_knows false
			,	generic_tell_topic !--respond if talked to. Implemented once here rather that twice in "life" 
				[;
					if(WillPlayerHear(self)==true){
						if((second provides rhetorical)==false||valueorrun(second,rhetorical)==false) {
							if(self provides already_knows  && self.already_knows==true) printorrun(self,AlreadyKnewResponse_msg);
							else {
								if(second.IsKnownBy(self)==true) printorrun(self,learnedResponse_msg);
								else printorrun(self,UnlearnedResponse_msg);
							}
						}
					}
					rtrue;
				]
			,	life [o;	
					ASKTopic:
						if((second==0) || ((second ofclass ORKnowledgeTopic)==false) || (second.isknownby(self)==false)){
							if(WillPlayerHear(self)==true) printorrun(self,IDunno_msg);
						}
						else {
							if(self==player)  !--if it is the player that was asked a question, then let us *not* automatically answer it as we would for an NPC
								self.PushOption(second); !--instead, we'll add it to the list of good things to say
							else{
								!---at this point, we can go ahead and answer the question
								o=actor;
								actor=self;
								<TellTopic o second>;
								actor=o;
	#ifdef USE_ORNPC_Interact;
								if(self provides interacted) self.interacted();
	#endif;
							}
						}
						rtrue;
					Answer, Ask, Tell: if(actor==player) playeristalkingto=self;
					TellTopic: 
						return self.generic_tell_topic();
					AnswerTopic:	o=noun; noun=second; second=o;
					return self.generic_tell_topic();
			]
		,	rhetorical false 	!for knowlege topics, used to suppress response by NPCs when topic is learned
	;
!======================================================================================
#endif; #ifnot; #ifndef ORNPC_AskTellLearn_DONEGRAMMAR; constant ORNPC_AskTellLearn_DONEGRAMMAR; #ifdef ORNPC_AskTellLearn_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================