!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORGibberish	[Z, GLULX]
! Generate and print a random, human pronouncable word.
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
! AutoDep:	none
!--------------------------------------------------------------------------------------
! Some modules are fun, but have little real practical purpose.  ORGibberish is just 
! such a module. ORGibberish is an object that defines a method called MakeWord.
! MakeWord takes a single number as a parameter which defines the number of requested 
! vowel sounds.  It then generates a random word based upon rules derived from 
! phonetic principals of pronunciation and then prints it.
!
! Possible uses? Passwords, Monster's Names, spell incantations...
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORGibberish";					!Constant USE_ORGibberish; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORGibberish; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.12.15	Initial Creation
! 2002.02.17	added Glulx support
! 2002.02.26	fit into new template
!--------------------------------------------------------------------------------------
message "          Processing library extension ORGibberish...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORGibberish forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORGibberish has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORGibberish_DONEREPLACE; constant ORGibberish_DONEREPLACE; #ifdef ORGibberish_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORGibberish_DONEMESSAGE; constant ORGibberish_DONEMESSAGE;  #ifdef ORGibberish_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORGibberish requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORGibberish_DONECODE;constant ORGibberish_DONECODE; #ifdef ORGibberish_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	object ORGibberish !---Print random pronouncable words
		with phonechoice 0 0 0 0 0 0 
		,	vowels		"a" "e" "i" "o" "u" "oo" "ou" "ae" "ei" "ia" "ee" !---vowels
		!---consonant phones------------------------------------------------------------
		,	tenuecon	"c" "k" "p" "qu" "t"			!---tenue mutes
		,	mutecon		"b" "d" "g" "j" "ph" "th" "ch"	!---mediae & aspirates mutes
		,	liquidcon	"l" "r"							!---liquids
		,	frictivecon	"f" "s" "z" "v"					!---frictives
		,	gencon		"h" "m" "n" "w" "x" "y"			!---others
		,	Makeword[numvowels t;
				if(numvowels==0)return;
				if(random(true,false)==true) self.ChooseConsonant();
				for(t=0:t<numvowels:t++){
					self.ChooseVowel();	
					if(t<(numvowels-1)) self.ChooseConsonant(); !--skip final
				}
				if(random(true,false)==true) self.ChooseConsonant(true);]!--true: no liquid end
		,	ChooseVowel[; self.PrintOne(vowels);]
		,	ChooseConsonant[disallowliquids value;
				if(disallowliquids)
					value=random(8)+6; !--ignore the first six liquid ending possibilities
				else
					value=random(15);
				!---5 simple rules to determine complex consonant phones:
				!---	a) a liquid can follow any mute						(1,2)
				!---	b) a liquid can follow any frictive					(3,4)
				!---	c) a liquid can follow an 's'						(5)
				!---	d) a liquid following a tenue can follow an 's'		(6)
				!---	e) a tenue can follow an 's'						(7)
				!---but most of the time non-complex consonants are used	(8-15)
				switch(value){
					1, 2:	self.PrintOne(tenuecon,mutecon);			!--- 2/15 (probability)
							self.PrintOne(liquidcon);			
					3, 4:	self.PrintOne(frictivecon);					!--- 2/15
							self.PrintOne(liquidcon);
					5:	print (string)"s"; self.PrintOne(liquidcon);	!--- 1/15
					6:	print (string)"s"; self.PrintOne(tenuecon);		!--- 1/15
						self.PrintOne(liquidcon);
					7:	print (string)"s"; self.PrintOne(tenuecon);		!--- 1/15
					default: self.PrintOne(tenuecon,liquidcon,			!--- 8/15
										frictivecon,mutecon,gencon);
				}]
		,	AddPhoneChoice[o;
				self.&phonechoice-->0=(self.&phonechoice-->0)+1;
				self.&phonechoice-->(self.&phonechoice-->0)=o;]
		,	PrintOne[one two three four five choice val;
				self.&phonechoice-->0=0;
				if(one)self.AddPhoneChoice(one);
				if(two)self.AddPhoneChoice(two);
				if(three)self.AddPhoneChoice(three);
				if(four)self.AddPhoneChoice(four);
				if(five)self.AddPhoneChoice(five);
				val=random(self.&phonechoice-->0);
				choice=self.&phonechoice-->(val);
				print (string)(self.&choice-->(random(self.#choice/WORDSIZE)-1));]
	;
!======================================================================================
   #endif; #ifnot; #ifndef ORGibberish_DONEGRAMMAR; constant ORGibberish_DONEGRAMMAR; #ifdef ORGibberish_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================