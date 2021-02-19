!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORInsultCompli_KT	[Z, GLULX]
! This module creates specialized ORKnowlegeTopics to implement Insults and Compliments
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
! AutoDep:	ORKnowledgeTopic
!			ORPronoun
!			ORDynaString
!--------------------------------------------------------------------------------------
! This module provides the verbs insult and complement and generate text accordingly.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORInsultCompli_KT";					!Constant USE_ORInsultCompli_KT; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORInsultCompli_KT; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.01.10 	Initial Creation (not really, but this is the earliest date I could 
!				find without searching my code arcive.)
! 2002.02.26	Fit to new framework.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORInsultCompli_KT...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORInsultCompli_KT forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORInsultCompli_KT has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORDynaString_DONEREPLACE; default USE_ORDynaString 0; message "          [ORInsultCompli_KT forcing inclusion of ORDynaString]"; #include "ORDynaString"; #endif; 
	#ifndef ORKnowledgeTopic_DONEREPLACE; default USE_ORKnowledgeTopic 0; message "          [ORInsultCompli_KT forcing inclusion of ORKnowledgeTopic]"; #include "ORKnowledgeTopic"; #endif; 
	#ifndef USE_ORProunoun;Constant USE_ORProunoun;message "          [ORInsultCompli_KT forcing inclusion of ORPronoun]"; #include "ORPronoun";#endif;
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORInsultCompli_KT_DONEREPLACE; constant ORInsultCompli_KT_DONEREPLACE; #ifdef ORInsultCompli_KT_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORInsultCompli_KT_DONEMESSAGE; constant ORInsultCompli_KT_DONEMESSAGE;  #ifdef ORInsultCompli_KT_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORInsultCompli_KT requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORInsultCompli_KT_DONECODE;constant ORInsultCompli_KT_DONECODE; #ifdef ORInsultCompli_KT_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	ORDynaString insultcomplimentstr;

	ORKnowledgeTopic insult_t 
		with	topicinformation 0 0 0 0 
		,		TellAbout[to from; 
					new_line;!outside of text
					insultcomplimentstr.capture();
					print "You ",(string)random( 
										 "abysmal"
										,"abominable"
										,"annoying"
										,"appalling"
										,"atrocious"
										,"brainless"
										,"despicable"
										,"disgusting"
										,"dismal"
										,"dreadful"
										,"dreary"
										,"foul"
										,"grotesque"
										,"horrendous"
										,"horrific"
										,"idiotic"
										,"ignorant"
										,"miserable"
										,"nasty"
										,"putrid"
										,"sickening"
										,"stupid"
										,"vile"
										,"vulgar"
										,"worthless"
										)," ";
					print (string)random(	 
										 "barrel of"
										,"bucket of"
										,"embodiment of"
										,"excuse for"
										,"apology for"
										,"lump of"
										,"hunk of"
										,"mound of"
										,"pile of"
										,"vat of"
										,"cart of"
										,"plate of"
										,"heap of"
										,"mountain of"
										,"mass of"
										,"stack of"
										,"sack of"
										,"loaf of"
										,"tower of"
										,"tub of"
										,"glob of"
										,"drop of"
										,"blob of"
										,"load of"
										)," ";
							print (string)random(	 
										 "bloody"
										,"gooey"
										,"icy"
										,"fermenting"
										,"slimy"
										,"steaming"
										,"sticky"
										,"stinking"
										,"slimy"
										,"greasy"
										,"oily"
										,"gory"
										,"bleeding"
										,"blood-spattered"
										,"sweltering"
										,"scorching"
										,"roasted"
										,"blistered"
										,"sticky"
										,"reeking"
										,"moldy"
										,"rotten"
										,"decaying"
										,"decomposed"
										,"festering"
										)," ";
							print (string)random(	 
										 "elephant crap"
										,"dog excrement"
										,"monkey filth"
										,"maggots"
										,"muck"
										,"donkey puke"
										,"vomit"
										,"squirl waste"
										,"tentacles"
										,"grime"
										,"gunk"
										,"rhino droppings"
										,"rubbish" 
										,"refuse"
										,"garbage"
										,"trash"
										,"throw up"
										,"urine"
										,"fertilizer"
										,"goop"
										,"mucus"
										,"gonad glop"
										,"lizard snot"
										,"pig phlegm"
										,"maggot fodder"
										);
					
					insultcomplimentstr.release();
					self.&topicinformation-->0=insultcomplimentstr;
					self.&topicinformation-->1="!";
					self.ORKnowledgeTopic::TellAbout(to,from); 
					if(to provides agitate) to.agitate(); 
					return true;
				]
		,	agitate !--for no error
	;
	ORKnowledgeTopic complement_t 
		with	topicinformation 0 0 0 0 
		,		TellAbout[to from; 
					new_line; !outside of text				
					insultcomplimentstr.capture();
					
					print "You ",(string)random( 
										 "fabulous"
										,"wonderful"
										,"tremendous"
										,"magnificent"
										,"marvelous"
										,"great"
										,"remarkable"
										,"extraordinary"
										,"amazing"
										,"spiffy"
										,"mesmirizing" 
										,"enviable"
										,"popular"
										,"sought-after"
										,"wanted"
										,"charming"
										,"delightful"
										,"attractive"
										,"appealing"
										,"pleasant"
										,"valuable"
										,"fascinating"
										,"astute"
!										,""
!										,""
										)," ";
					print (string)random(	 
										 "barrel of"
										,"bucket of"
										,"embodiment of"
										,"paragon of"
										,"model of"
										,"example of"
										,"hunk of"
										,"mountain of"
										,"tower of"
										,"bundle of"
										,"package of"
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""

										)," ";
							print (string)random(	 
										 "enticing"
										,"charismatic"
										,"oozing"
										,"burning"
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
										)," ";
							print (string)random(	 
										 "quintessence"
										,"perfection"
										,"love"
										,"disirablity"
										,"intelligence"
										,"excellence"
										,"faultlessness"
										,"precision"
										,"aptitude"
										,"achievement"
										,"triumph"
										,"accomplishment"
										,"skill"
										,"ability"
										,"talent"
										,"cleverness"
										,"proficiency"
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""
!										,""

										);
					insultcomplimentstr.release();
					self.&topicinformation-->0=insultcomplimentstr;
					self.&topicinformation-->1="!";

					self.ORKnowledgeTopic::TellAbout(to,from); 
					if(to provides cheerup) to.cheerup(); 
					return true;
				]
			,	cheerup !--for no error
	;
!======================================================================================
   #endif; #ifnot; #ifndef ORInsultCompli_KT_DONEGRAMMAR; constant ORInsultCompli_KT_DONEGRAMMAR; #ifdef ORInsultCompli_KT_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	[ComplimentSub;
		<<TellTopic noun complement_t>>;
	];
	[InsultSub;
		<<TellTopic noun insult_t>>;
	];
	Verb "compliment" 
			* creature -> Compliment;
	Verb "insult" 
			* creature -> Insult;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================