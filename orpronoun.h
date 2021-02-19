!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.22 ORPronoun	[Z, GLULX]
! This is a collection of pronoun print rules to handle just about every imaginable
! scenerio.  Build from principals discussed in the article "Pronouns on Steroids"
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
! AutoDep :	none
!--------------------------------------------------------------------------------------
! Things to remember about this implementation.
! Functions, such as civerb() and iverb() return a value after they have 
! printed.  When using these the (ig) printrule should be used.
!
! PrintRules:
!
!	I, Me, My, Mine, Myself, CI, CMe, CMy, CMine, CMyself, 
!	TheI, TheMe, TheMy, TheMine, TheMyself, CTheI, CTheMe, CTheMy, CTheMine, CTheMyself, 
!	IAm, CIAm
!
! Functions
!
!	IVerb, CIVerb
!
!	To add this module to your library, add the line:
!
!		#include "ORPronoun";					!Constant USE_ORPronoun; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORPronoun; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History 
! 2001.05.05	Initial Creation 
! 2001.09.09	?
! 2002.02.09	modified (AM) print rule.
! 2002.02.20	Added GLULX Support
! 2002.02.26	Fit to new template.
! 2002.03.07	Made modifications to support Third person text.
! 2002.03.08	Revised above modification form GLULX compatibility.
! 2002.03.11	Fixed blow up from vrb when object passed in is -1 (which is legal and should force a plural)
! 2002.05.18	Added additional defines to ensure that there is no requirement to use OREnglish
! 2002.07.15	Added additional TheVerb and CTheVerb routines to allow subject-noun agreement without pronouns
! 2003.01.05	Fixed bug with TheMy and TheMine (and caps versions too) concerning apostrophy "s"'s 
! 2004.01.19	Added constants SINGULAR_OBJ and PLURAL_OBJ to pass to the vrb routine instead of -1 or 0. 
! 2004.01.22	eliminated warning of unreferenced plural_obj constant; also fixed issue with actor and player
!				not being equal if player is assigned in initialise.
! 2004.02.22	Removed the code added on 2004.01.22 to handle pre-move actor state if 
!				player is assigned in initialise; this has been fixed in the 6.11
!				standard library final release.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORPronoun...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORPronoun forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORPronoun has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORPronoun_DONEREPLACE; constant ORPronoun_DONEREPLACE; #ifdef ORPronoun_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	system_file;
	Constant SINGULAR_OBJ 0;
	Constant PLURAL_OBJ -1;
	replace PSN__;
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORPronoun_DONEMESSAGE; constant ORPronoun_DONEMESSAGE;  #ifdef ORPronoun_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORPronoun requires the OREnglish file.";#endif; 
	Global MPronounObj;
	Global FPronounObj;
	Global NPronounObj;
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORPronoun_DONECODE;constant ORPronoun_DONECODE; #ifdef ORPronoun_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	!----------------------------------------------------------------------
	! Support OREnglish's features without creating a dependancy
	!----------------------------------------------------------------------
	Default FIRST_PERSON 1;
	Default SECOND_PERSON 2; 
	Default THIRD_PERSON 3; 
	Default PAST_TENSE 1;
	Default PRESENT_TENSE 2; 
	Default FUTURE_TENSE 3;

	Default NarativePerson SECOND_PERSON; 
	Default NarativeTense PRESENT_TENSE; 

	[ PSN__ o;
		if (o==0) { print (string) NOTHING__TX; rtrue; }
		switch(metaclass(o))
		{   Routine: print "<routine ", o, ">"; rtrue;
			String:  print "<string ~", (string) o, "~>"; rtrue;
			nothing: print "<illegal object number ", o, ">"; rtrue;
		}
		if (o==player && NarativePerson~=THIRD_PERSON) { print (string) YOURSELF__TX; rtrue; }
		#ifdef LanguagePrintShortName;
		if (LanguagePrintShortName(o)) rtrue;
		#endif;
		if (indef_mode && o.&short_name_indef~=0
			&& PrintOrRun(o, short_name_indef, 1)~=0) rtrue;
		if (o.&short_name~=0 && PrintOrRun(o,short_name,1)~=0) rtrue;
		print (object)o;
	];
	system_file;
	#ifndef ppf;[ppf f s l; f=l; print (string)s;]; #endif;
	#ifndef fst;[fst f s l; f=l; print (string)s;]; #endif;
	!----------------------------------------------------------------------
	! Special formating print routines go here
	!----------------------------------------------------------------------
	#ifndef ig; [ig o;o=null;]; #endif; !ignore print rule 
	!----------------------------------------------------------------------
	! All common code in the pronoun print routines is contined here
	!----------------------------------------------------------------------
	[CommonPronounCode12 obj i we you allofyou;
		if(obj==player && NarativePerson~=THIRD_PERSON)	!if we are talking about the player
		{
			if(NarativePerson==1) !first person?
			{
				if(obj hasnt pluralname) print(string)i;
				else print (string)we;
			}
			else 
			{
				if(obj hasnt pluralname) print (string)you; 
				else print (string)allofyou;
			}
			return true;
		}
		return false; !otherwise return false so that the other handler can be called
	];
	[CommonPronounCode3 obj they it she he;
		if(obj~=player || (obj==player && NarativePerson==THIRD_PERSON))	!if we are NOT talking about the player
		{
			if(obj has pluralname) 
				print (string)they; 
			else ! is singular
			{
				if (obj has neuter || obj hasnt animate) 
					print (string)it;
				else 
				{
					if (obj has female)  print (string)she;
					else print (string)he;
				}
			}
			return true;
		}
		return false;
	];
	!----------------------------------------------------------
	! lowercase versions of the five sets of pronouns.  Note: maximum of 7 params (using return of false to specify first person)
	!----------------------------------------------------------
	[I obj; if(CommonPronounCode12(obj, "I", "we","you","you")==false) CommonPronounCode3(obj,"they","it","she","he");];
	[Me obj; if(CommonPronounCode12(obj, "me", "us","you","you")==false) CommonPronounCode3(obj, "them","it","her","him");];
	[My obj; if(CommonPronounCode12(obj, "my","our","your","your")==false) CommonPronounCode3(obj,"their","its","her","his");];
	[Mine obj; if(CommonPronounCode12(obj, "mine", "ours","yours","yours")==false) CommonPronounCode3(obj,"theirs","its","hers","his");];
	[Myself obj; if(CommonPronounCode12(obj, "myself", "ourselves","yourself","yourselves")==false) CommonPronounCode3(obj,"their selves","itself","herself","himself");];
	!----------------------------------------------------------
	! UPPERCASE versions of the five sets of pronouns
	!----------------------------------------------------------
	[CI obj; if(CommonPronounCode12(obj, "I", "We","You","You")==false) CommonPronounCode3(obj,"They","It","She","He");];
	[CMe obj; if(CommonPronounCode12(obj, "Me", "Us","You","You")==false) CommonPronounCode3(obj, "Them","It","Her","Him");];
	[CMy obj; if(CommonPronounCode12(obj, "My","Our","Your","Your")==false) CommonPronounCode3(obj,"Their","Its","Her","His");];
	[CMine obj; if(CommonPronounCode12(obj, "Mine", "Ours","Yours","Yours")==false) CommonPronounCode3(obj,"Theirs","Its","Hers","His");];
	[CMyself obj; if(CommonPronounCode12(obj, "Myself", "Ourselves","Yourself","Yourselves")==false) CommonPronounCode3(obj,"Their selves","Itself","Herself","Himself");];
	!----------------------------------------------------------
	! All common code in the "The or pronoun" style routines 
	!----------------------------------------------------------
	[CommonTheORPronounCode obj cap checkobj;	
		if(obj hasnt animate || obj has neuter) !first locate the appropriate varaible to test
			checkobj=NPronounObj;
		else 
		{
			if(obj has female) 
				checkobj=FPronounObj;
			else 
				checkobj=MPronounObj;
		}
		if(obj==checkobj || (obj==player && NarativePerson~=THIRD_PERSON))  return false; !--if they are the same, then return false, nothing output
		if(cap) 
			print (The)obj;
		else	
			print (the)obj;
		if(checkobj==NULL && (obj~=player || NarativePerson==THIRD_PERSON)) !save the object for a later test
		{
			if(obj hasnt animate || obj has neuter)
				NPronounObj=obj;
			else 
			{
				if(obj has female) FPronounObj=obj;
				else MPronounObj=obj;
			}
		}
		return true; !printed the name
	];
	!----------------------------------------------------------
	! Reset the pronoun affinity
	!----------------------------------------------------------
	[ClearPronoun; 
		MPronounObj=FPronounObj=NPronounObj=null;
	];
	!----------------------------------------------------------
	! lowercase versions of the five sets of pronouns
	!----------------------------------------------------------
	[TheI obj; if(CommonTheORPronounCode(obj,false)==false) print (I)obj;];
	[TheMe obj; if(CommonTheORPronounCode(obj,false)==false) print (Me)obj;];
	[TheMy obj; if(CommonTheORPronounCode(obj,false)==false) print (My)obj; else print "'s";];
	[TheMine obj; if(CommonTheORPronounCode(obj,false)==false) print (Mine)obj; else print "'s";];
	[TheMyself obj; if(CommonTheORPronounCode(obj,false)==false) print (Myself)obj;];
	!----------------------------------------------------------
	! UPPERCASE versions of the five sets of pronouns
	!----------------------------------------------------------
	[CTheI obj; if(CommonTheORPronounCode(obj,true)==false) print (CI)obj;];
	[CTheMe obj; if(CommonTheORPronounCode(obj,true)==false) print (CMe)obj;];
	[CTheMy obj; if(CommonTheORPronounCode(obj,true)==false) print (CMy)obj; else print "'s";];
	[CTheMine obj; if(CommonTheORPronounCode(obj,true)==false) print (CMine)obj; else print "'s";];
	[CTheMyself obj; if(CommonTheORPronounCode(obj,true)==false) print (CMyself)obj;];
	!----------------------------------------------------------
	! A couple of routines to address the change in verb form 
	!	when dealing with singular nouns in the third-person, 
	!	present tense.
	!----------------------------------------------------------
	[Vrb obj pastverb vrb altverb futverb; !if no object, then obj can equal SINGULAR_OBJ or PLURAL_OBJ 
		!-- if(3rdPerson/PresentTense/Singular) then "runs" else "run"
		if(NarativeTense==PRESENT_TENSE && (obj~=player || NarativePerson==THIRD_PERSON) && (obj==SINGULAR_OBJ || (metaclass(obj)==object && obj hasnt pluralname))){
			if(altverb) print (string)altverb;
			else print (string)vrb,"s";
		}
		else {
			switch(NarativeTense){
				PAST_TENSE: print (string) pastverb;
				PRESENT_TENSE: print(string)vrb;
				FUTURE_TENSE: if(futverb)print_ret (string)futverb;
							else print "will ",(string)vrb;
			}
		}
	];
	[CommonVerbAdjustment cap obj pastverb verb altverb futverb forcename;	
		if(forcename){
			if(cap) print (The) obj," ";
			else  print (the) obj," ";
		}
		else{
			if(cap) print (CTheI) obj," ";
			else  print (TheI) obj," ";
		}
		Vrb(obj,pastverb,verb,altverb,futverb);
	];
	[IVerb obj pastverb verb altverb futverb; CommonVerbAdjustment(false,obj,pastverb, verb,altverb, futverb);];
	[CIVerb obj pastverb verb altverb futverb; CommonVerbAdjustment(true,obj,pastverb, verb,altverb, futverb);];
	[TheVerb obj pastverb verb altverb futverb; CommonVerbAdjustment(false,obj,pastverb, verb,altverb, futverb,true);];
	[CTheVerb obj pastverb verb altverb futverb; CommonVerbAdjustment(true,obj,pastverb, verb,altverb, futverb,true);];
	!----------------------------------------------------------
	! Print the appropriate "verb of being"
	!----------------------------------------------------------
	! pass false for generic singular object or true for generic plural object, otherwise pass in the object itself
	[Am obj; !was/am/will/were/are/is
		if(obj==player && NarativePerson~=THIRD_PERSON) !first or second person
		{
			switch(NarativePerson)
			{
			FIRST_PERSON: print (ig) ppf("was","am","will");
			SECOND_PERSON: print (ig) ppf("were","are","will");
			}
			return;
		}
		if(obj~=null && (obj == true || obj has pluralname)) 
			print (ig) ppf("were","are","will");
		else 
			print (ig) ppf("was","is","will");
	];
	[IAm obj; print (TheI)obj," ",(Am)obj;];
	[CIAm obj; print (CTheI)obj," ",(Am)obj;];
	[Have obj; !had/have/will have
		!--for present tense always use "have" except when speaking in singular third person
		if(obj hasnt pluralname && (obj~=player || (obj==player && NarativePerson==THIRD_PERSON))) {
			print (ig) ppf("had","has","will have had"); 
			return;
		}
		print (ig) ppf("had","have","will have had");
	];
	[IHave obj; print (TheI)obj," ",(Have)obj;];
	[CIHave obj; print (CTheI)obj," ",(Have)obj;];
	[ICan obj; IVerb(obj,"could","can","can");];
	[CICan obj; CIVerb(obj,"could","can","can");];
	[ICant obj; IVerb(obj,"could not","cannot","cannot");];
	[CICant obj; CIVerb(obj,"could not","cannot","cannot");];
!======================================================================================
#endif; #ifnot; #ifndef ORPronoun_DONEGRAMMAR; constant ORPronoun_DONEGRAMMAR; #ifdef ORPronoun_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================