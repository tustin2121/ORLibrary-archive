!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORNameable [Z, GLULX]
! Gives an object the ability to be named and then be referred to by that name.
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
! AutoDep :	None.
!--------------------------------------------------------------------------------------
! To create a nameable object, simple derive it from ORNameable:
!
!	object golem "Golem"
!		has	animate  
!		class ORNameable
!		with description "The creature looks dirty and pathetic."
!		,	name 'golem'
!	;
!
! And during game play you can name the golem with a command similar to:
!
!	>name golem "bob"
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORNameable";					!Constant USE_ORNameable; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORNameable; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.12	Initial Creation
! 2002.02.26	Fit to new template
! 2002.02.27	Fixed search and replace error when applying new framework.
! 2002.07.07	Added menu documentation.
! 2002.10.19	Added autodep for ObjectInitialise 
! 2003.02.19	Added better ifdef for self docs
! 2004.01.14	Made compatible with new standard library extensions.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORNameable...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORNameable forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORNameable has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORNameable_DONEREPLACE; constant ORNameable_DONEREPLACE; #ifdef ORNameable_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORNameable_DONEMESSAGE; constant ORNameable_DONEMESSAGE;  #ifdef ORNameable_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORNameable requires the OREnglish file.";#endif; 
	global named_wn;
	global named_article;
	#ifndef MAX_SIZE; constant MAX_SIZE=9;#endif;
system_file;
	class ORNameable 
		with	custom_name	0 0 0 0 0 0 0 0 0 0 !--up to 20 characters in length
		,	after_description false !--used to passthrough the following short_name (used with recursion)
		,	short_name[; if(self.after_description==false){
							self.after_description=true;
							print (name)self;
							if(self.&custom_name->0~=0){
								print " (or ";
								self.named_name();
								print ")";
							}
							self.after_description=false;
							rtrue;
						}
				]
		,	custom_article 0
		,	before[;
				Name:	if(noun==self) {
							self.set_name(named_wn);
							rtrue;
						}
			]
		,	parse_name[w t count o swn add length;
				length=WordLength(wn);
				if(length>MAX_SIZE) length=MAX_SIZE;
				if(parser_action==##TheSame){   
					!--I think this was a copy and paste error...
					!--for(t=0:t<self.#custom_name:t++) if(parser_one.&custom_name->t~=parser_two.&number->i) return -2; !--different
					for(t=0:t<self.#custom_name:t++) if(parser_one.&custom_name->t~=parser_two.&custom_name->t) return -2; !--different
					return -1; !--indistinguishable
				}
				while(true){
					swn=wn;
					w=NextWord();
					t=WordInProperty(w,self,name);
	#ifdef USE_ORRecogName;
					if(t<1 || w==0) t=MatchAgainstShortName(self,swn);
	#endif;				
					if(t<1){
						add=WordAddress(swn);
						t=true;
						for(o=0:o<length:o++){
							if(add->o~=self.&custom_name->o){
								t=false;
								break;
							}
						}
					}
					if(t<1) {
						if(count>0) return count;
						return -1;
					}
					count++;
				}
			]
		,	named_name[t temp;
				switch(self.custom_article){
					'a':print "a ";
					'an': print "an ";
					'the': print "the ";
				}
				for(t=0:t<self.#custom_name:t++){
					temp=(self.&custom_name->t);
					if(temp==0) continue;
					if(self.custom_article==0 && t==0) temp=temp-32;
					print (char)temp;
				}
			]
		,	set_name[addr len t;
				if(named_wn==0) rfalse;
				addr=WordAddress(named_wn);
				len=WordLength(named_wn);
				if(len==0) rfalse;
				if(len>self.#custom_name)len=self.#custom_name;
				for(t=0:t<self.#custom_name:t++) self.&custom_name->t=0; !-zero out name first
				for(t=0:t<len:t++) self.&custom_name->t=addr->t; !--copy new name in
				self.custom_article=named_article;
				L__M(##Name,1,self);
				rtrue;
			]	
	;
	object _onam_init LibraryExtensions
		with ext_messages[;
			Name:	
				switch(lm_n){
					1 :	noun.after_description=true;
						print (ig)CIVerb(actor,"began","begin")," to think of ",(the)noun," as ";
						if(noun.custom_article==0) print "~";
						noun.named_name();
						print".";
						if(named_article==0)print"~";
						noun.after_description=false;
					2:	bkt_o(); 
						print (CICant)noun," be named."; 
						bkt_c();
				}
		];
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNameable_DONECODE;constant ORNameable_DONECODE; #ifdef ORNameable_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #ifnot; #ifndef ORNameable_DONEGRAMMAR; constant ORNameable_DONEGRAMMAR; #ifdef ORNameable_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	!-- isolate a single word, possibly quoted (name knife "stinger")
	!-- and possibly beginning with an article (refer to the knife as a sword)
	[single_word addr; 
		named_wn=wn++; 
		named_article=0;
		if(WordAddress(named_wn)->0=='"')named_wn=wn++; !--strip out quotes
		addr=WordAddress(named_wn);
		if(WordLength(named_wn)==3 && addr->0=='t' && addr->1=='h' && addr->2=='e') named_article='the';
		if(WordLength(named_wn)==2 && addr->0=='a' && addr->1=='n') named_article='an';
		if(WordLength(named_wn)==1 && addr->0=='a') named_article='a';
		if(named_article~=0) named_wn=wn++; !--strip out article
		if(WordAddress(named_wn+1)->0=='"') wn++; 
		rtrue; 
	];
	[NameSub; 	L__M(##Name,2,self);];
	Verb 'name'
		* noun single_word->Name
		* noun 'as'/'to' single_word ->Name	
	;
	Verb 'refer'
		* 'to' noun 'as' single_word ->Name
	;
	verb 'dub' = 'name';
	verb 'call' = 'name';
#ifdef ORLibDoc;
	!--Commands, document thyselves.
	ORMenu ORHelpName "Name" ORLibDoc with text "Some objects can be named. That is, certain objects can be named by the player and then referred to by that given name. For example:^^NAME SWORD BOB^^can then be followed with:^^PUT BOB IN SHEATH.";
#endif;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================