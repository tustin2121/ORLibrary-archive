!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORKnowledgeScript	[Z, GLULX]
! This class is derived from ORKnowledgeTopic.  It provides the basis for scripted
! NPC initiatable conversations that do not repeat, like a lecture.
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
!			ORKnowledgeTopic
!--------------------------------------------------------------------------------------
!	ORKnowledgeScript DocScript_t
!		with KnownBy AnNPC
!		, initiatable true
!		, TopicList 
!				"^~How are you feeling, Mr. Valentine?  I'm pleased to  see you've awakened,~ 
!					the man says to you. " 
!				doc_who_t
!				doc_me_t
!				hosp_where_t
!				local_routine !--this is a routine defined in this object
!				"~Okay then,~ says the doctor as he opens the door.  ~If you have anymore 
!					questions, feel free to buzz the nurse.~  He leaves and closes the door behind him." 
!		, local_routine[;
!				if(random(2)==1) "man doctor scratches his chin."; "~It's getting late,~ says the man.";
!			]
!	;
!
! Note that the TopicList property above can list 1)strings, 2)local routines, and 3) ORKnowledgeTopics.
! The three ORKnowledge topics listed above follow for the sake of completness:
!
!	ORKnowledgeTopic doc_who_t 
!		with name 'him' 'himself' 'man' 'doc' 'doctor' 'who'
!		,	KnownBy AnNPC
!		,	query  "^~So who are you?~ I asked the man. "
!		,	TopicInformation "^~My name is Doctor Harris.  I'm the resident Nuerological specialist.~ ^^
!				~Nuerological.~ You repeat, then touch your head probingly. No bandage.^^The doctor nods. "
!	;
!	ORKnowledgeTopic doc_me_t 
!		with name 'myself' 'self' 'me' 'head' 'damage' 'condition'
!		,	KnownBy AnNPC
!		,	query "^~So what's my condition, Doc?~ You ask."
!		,	TopicInformation "^^~You're the talk of the hospital right now,~ he replies. ~You've been comatose 
!				for several weeks. CAT scans showed extreme cerebral dysfunction. To be frank, the prognosis was 
!				bleak.^^~This morning, for no apparent reason, monitors registered a return to normal brain 
!				activity. Further examination showed burned tissue and broken ribs healed. I've never seen 
!				anything like it before.~^^Cerebral dysfunction... You think about that for a moment when it 
!				suddenly dawns on you that you cannot remember anything about yourself, including your own name!"
!	;
!	ORKnowledgeTopic hosp_where_t
!		with name 'where' 'hospital' 'location'
!		,	KnownBy AnNPC
!		,	query "^~So where am I?~ ,You ask the doctor. "
!		,	TopicInformation "^~You're at Saint Augustine Hospital.~^Saint Augustine. You don't recognize the name. "
!	;
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORKnowledgeScript; #include "ORKnowledgeScript"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORKnowledgeScript; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.2.08		Initial Creation
! 2001.2.25		Modified to Fit new framework
! 2002.02.27	Added missing #endif missed when new framework was added.
! 2002.04.09	Added support for string types being listed in the TopicList property.
! 2002.05.24	Added support for routine lists the TopicList property.
! 2002.09.02	Converted attributes previously defined in this module to properties.
! 2003.01.31	Reworking the whole linefeed concept in light of a posting to RAIF
!				from Andrew Plotkin.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORKnowledgeScript...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORKnowledgeScript forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORKnowledgeScript has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORDynaString_DONEREPLACE; default USE_ORDynaString 0; message "          [ORKnowledgeScript forcing inclusion of ORDynaString]"; #include "ORDynaString"; #endif; 
	#ifndef ORKnowledgeTopic_DONEREPLACE; default USE_ORKnowledgeTopic 0; message "          [ORKnowledgeScript forcing inclusion of ORKnowledgeTopic]"; #include "ORKnowledgeTopic"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORKnowledgeScript_DONEREPLACE; constant ORKnowledgeScript_DONEREPLACE; #ifdef ORKnowledgeScript_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORKnowledgeScript_DONEMESSAGE; constant ORKnowledgeScript_DONEMESSAGE;  #ifdef ORKnowledgeScript_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORKnowledgeScript requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORKnowledgeScript_DONECODE;constant ORKnowledgeScript_DONECODE; #ifdef ORKnowledgeScript_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	system_file;
	class ORKnowledgeScript 
		class ORKnowledgeTopic
		with	ToldState 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		, StateBy[from val;
				if(val~=0) self.&ToldState-->self.KnownAt(from)=val;
				return self.&ToldState-->self.KnownAt(from);
			]
		, TopicList
		, TellAbout[to from topic state;
				if(self.hasBeenSpokenOfBy(from)==true) return;
				state=self.StateBy(from);
				topic=self.&TopicList-->state;	!---get the next topic on the list
				state++;						!---increment to just before the next topic 
				if(state==(self.#TopicList/WORDSIZE)){		!---if surpassed topic count 
					state=0;					!---reset the state
					self.hasBeenSpokenOfBy(from,true); !---mark as has been told
				}
				self.StateBy(from, state);	!---record state pointer
				self.SayORRecurse(topic, to, from);
			]
		, SayORRecurse[topic to from;
				if(topic ofclass ORKnowledgeTopic){
					if(topic.hasBeenSpokenOfBy(from)) !---if the topic we selected has already been told
						self.TellAbout(to,from);		!---then lets call ourselves recursivly
					else
						topic.TellAbout(to,from);
					return;
				}
				if(metaclass(topic)==string){
					print "^",(string)topic,"^";
					return;
				}
				if(metaclass(topic)==routine){
					print"^";
					topic(to,from);
					print"^";
					return;
				}
				if(metaclass(topic)~=object){
					print"^";
					self.topic(to,from);
					print"^";
					return;
				}
				print "^[ERR:Invalid topic listed in TopicList property of ",(object)self,".]^";
		]
	;
!======================================================================================
   #endif; #ifnot; #ifndef ORKnowledgeScript_DONEGRAMMAR; constant ORKnowledgeScript_DONEGRAMMAR; #ifdef ORKnowledgeScript_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================