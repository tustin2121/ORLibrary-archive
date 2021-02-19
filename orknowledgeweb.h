!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.04.27 ORKnowledgeWeb [Z, GLULX]
! Used to create a network of interrelating knowledge topics
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
! Derivitived from ORKnowledgeTopic, this class has the added support for NPCs
! which maintain an OptionList for possible conversation topics.
!--------------------------------------------------------------------------------------
! AutoDep:	ORKnowledgeTopic
!			OROptionList
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORKnowledgeWeb";					!Constant USE_ORKnowledgeWeb; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORKnowledgeWeb; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.03.23	Initial Creation
! 2002.03.31	Split out NPC class into its own module.
! 2002.04.03	Setup to leverage OROptionList 
! 2002.04.09	Added code to eliminate old topics if running out of room
! 2002.04.14	Fixed issue caused by .h removal.
! 2002.04.15	Fixed incorrect algorithm for adding topics
! 2002.04.17	Removed unused variable from TellAbout routine (and the associated warning)
! 2002.05.01	Modified comments.
! 2002.07.15	Modified the mechanism in which "asking" in stored.  Previously the "associations"
!				of an entry was "true" if the topic needed to be "asked" and "false" it it needed
!				to be "told". Now, "asked" topics are stored as negative numbers and "told" topics
!				are stored as positive one.  the actual number is the turn in which the topic was 
!				added, allowing more recent topics to be given greater priority.
! 2002.08.23	Fixed bug related to NPC's being talked to are present
!				but not in the same location (e.g. sitting on a chair)
! 2002.09.02	Converted attributes previously defined in this module to properties.
! 2004.02.09	objectloop optimizations.
! 2004.04.28	Fixed a bug that caused flawed conversation selection by NPC.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORKnowledgeWeb...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORKnowledgeWeb forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORKnowledgeWeb has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORDynaString_DONEREPLACE; default USE_ORDynaString 0; message "          [ORKnowledgeWeb forcing inclusion of ORDynaString]"; #include "ORDynaString"; #endif; 
	#ifndef ORKnowledgeTopic_DONEREPLACE; default USE_ORKnowledgeTopic 0; message "          [ORKnowledgeWeb forcing inclusion of ORKnowledgeTopic]"; #include "ORKnowledgeTopic"; #endif; 
	#ifndef OROptionList_DONEREPLACE; default USE_OROptionList 0; message "          [ORKnowledgeWeb forcing inclusion of OROptionList]"; #include "OROptionList"; #endif;   
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORKnowledgeWeb_DONEREPLACE; constant ORKnowledgeWeb_DONEREPLACE; #ifdef ORKnowledgeWeb_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORKnowledgeWeb_DONEMESSAGE; constant ORKnowledgeWeb_DONEMESSAGE;  #ifdef ORKnowledgeWeb_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORKnowledgeWeb requires the OREnglish file.";#endif; 
system_file;
	class ORKnowledgeWeb
		class ORKnowledgeTopic
		with TellAbout[to from forcetellagain t rt o;
				!--clear all topics if needed
				if(valueorrun(self,ResetOptionList)==true){
					objectloop(o near from){
						if(o provides ClearOptionList && (valueorrun(self, nonspecifictarget)==true || o==from or to)){ 
							o.ClearOptionList();
						}
					}
				}
				!--clear this specific topic
				objectloop(o near from) {
					if(o provides removeoption && (valueorrun(self,nonspecifictarget)==true || o==from or to))
						o.removeoption(self);
				}
				!--add all related topics
				for(t=0:t<self.#RelatedTopics/WORDSIZE:t++){ !--for each related topic that the listener might think to ask
					rt=self.&RelatedTopics-->t;
					if(rt==0) continue;
					objectloop(o near from){
						if(o provides PushOption && (valueorrun(self,nonspecifictarget)==true  || o==from or to)) {
							!--something to say?
							if(rt.IsKnownBy(o)==true && rt.hasBeenSpokenOfBy(o)==false && ((rt provides response)==false || valueorrun(rt,response)==false)){
								self.addtopic(o,rt,false); !--add as something to say
							}
							!--something to ask?
							if(rt.IsKnownBy(o)==false && rt provides query && rt.query~=0){
								self.addtopic(o,rt,true); !--add as something to ask
							}
						}
					}
				}
				self.ORKnowledgeTopic::TellAbout(to,from,forcetellagain);
			]
		,	AskAbout[askwho askby o;
				objectloop(o near askby){
					if(((valueorrun(self,nonspecifictarget)==true) ) || o==askwho){
						if(self.isknownby(o)==true && o provides PushOption && o~=askby) 
							o.PushOption(self,turns);					
					}
				}
				self.ORKnowledgeTopic::AskAbout(askwho,askby);
			]
		,	addtopic[po rt altval adj;
				if(rt==0)rt=self;
				adj=turns;
				if(altval==true) adj=adj*-1;
				if(po~=0){
					if(po.PushOption(rt,adj)==false){ !--generally adding an option fails because of a lack of room
						po.RemoveOptionByNumber(0); !--lets forget about older topics
						po.PushOption(rt,adj); !--and try to add it again
					}
				}
			]
		,	RelatedTopics
		,	Response false !--npc will not talk of this topic unless asked about it
		,	ResetOptionList false
		,	NonSpecificTarget false !--this topic effects everyone present, not just the character being talked to
	;
!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORKnowledgeWeb_DONECODE;constant ORKnowledgeWeb_DONECODE; #ifdef ORKnowledgeWeb_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
   #endif; #ifnot; #ifndef ORKnowledgeWeb_DONEGRAMMAR; constant ORKnowledgeWeb_DONEGRAMMAR; #ifdef ORKnowledgeWeb_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================