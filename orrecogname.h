!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORRecogName [Z, GLULX]
!	This modification eliminates the need to specify the name property.  That is,
!	it allows for object recognition without the use of dictionary words. Without 
!	this modification, the name property needs to contain all possible words that can
!	be used to match against a noun, even if already listed in object the declaration
!	(short_name).
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
! AutoDep (Glulx-only) :	ORInfGlk
! AutoDep (Z-code) :		none
!--------------------------------------------------------------------------------------
!	Note the need to repeat words in the following conventional code samples:
!
!		Object redbox "big red box"
!			 with	name "big" "red" "box";
!	
!		object SmellyCat "dirty, smelly cat"
!			 with	name "dirty" "smelly" "cat";
!
!	This is a common, but annoying redundancy.  With this library entry, the name 
!	property is no longer necessary, but IS still optional.  The following code 
!	demonstrates:
!
!		Object RedDoor "big red box";
!
!		Object SmellyCat "dirty, smelly cat";
!
!		Object browndog "great brown down" with name "Spot" "canine";
!
!	Note that it is now only necessary to specify synonyms and words not appearing
!	in the display name (short_name) property.
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORRecogName";					!Constant USE_ORRecogName; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORRecogName; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.04.28	Initial Creation
! 2001.05.14	Forced check to be case insensitive (thereby allowing a 
!				match against proper names)
! 2001.05.21	Fixed bug where shorter names were matching.  
!				e.g. ME matched for MEMORY
! 2002.01.10	Added the return of a generic word to the NOUNWORD routine
!				to eliminate inconsistencies.
! 2002.02.11	Added GLULX support.
! 2002.02.22	Corrected bug intoduced with GLULX support which prevented "middle" 
!				words in the name from being recognized.
! 2002.02.24	MAJOR optimizations made to NounWord() by reducing number of calls to 
!				MatchAgainstShortName (from thirty or more to just one per turn).
! 2002.02.26	Fit to new template and removed some scope-specific code made previously.
! 2002.03.11	wrapped meeting of Autodependancy InfGlk in GLULX-only directives
! 2002.05.18	Corrected issue of no named objects being treated as identical objects
! 2002.05.28	Adjustment for check-name generation.
! 2003.01.02	Added support for ME4__WD, if defined.
! 2003.02.02	Supressed matching of directions objects by name.
! 2003.04.27	Eliminated reuse of buffers.  Caused incorrect matching from the indentical
!				routine.
! 2003.12.15	Modified to use new buffer arrays from the 6.3 compiler. Also made minor 
!				modifications to avoid new 6.3 warnings.
! 2003.12.20	Buffer overflow affecting only GLULX
!--------------------------------------------------------------------------------------
message "          Processing library extension ORRecogName...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORRecogName forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORRecogName has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
#ifdef TARGET_GLULX;
	#ifndef ORInfGlk_DONEREPLACE; default USE_ORInfGlk 0; message "          [ORRecogName forcing inclusion of ORInfGlk]"; #include "ORInfGlk"; #endif; 
#endif;
	#ifndef ORDynaString_DONEREPLACE; default USE_ORDynaString 0; message "          [ORRecogName forcing inclusion of ORDynaString]"; #include "ORDynaString"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORRecogName_DONEREPLACE; constant ORRecogName_DONEREPLACE; #ifdef ORRecogName_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	Replace NounWord;
	Replace Identical;
	[LanguageRefers obj wordnumber;
		return MatchAgainstShortName(obj,wordnumber);
	];
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORRecogName_DONEMESSAGE; constant ORRecogName_DONEMESSAGE;  #ifdef ORRecogName_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORRecogName requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORRecogName_DONECODE;constant ORRecogName_DONECODE; #ifdef ORRecogName_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	Constant _RECOGBUFLEN 100;
	ORDynaString _recname_dyna;

	array _namebuffer1 buffer _RECOGBUFLEN;
	array _namebuffer2 buffer _RECOGBUFLEN;

	#ifndef MAX_SIZE; constant MAX_SIZE=9;#endif;
	
	[MatchAgainstShortName obj wordnumber o i count count2 markerend markerstart check chara sv len;
		sv=0;
		if(obj ofclass CompassDirection) return -1;
	#ifndef TARGET_GLULX;
			@output_stream 3 _namebuffer1;	!capture short_name in array
	#ifnot;
			sv=glk_stream_get_current();
			glk_stream_set_current(glk_stream_open_memory(_namebuffer1+WORDSIZE, _RECOGBUFLEN, filemode_Write, 661));
	#endif;
		print(object) obj," ",(name) obj;
	#ifndef TARGET_GLULX;
			@output_stream -3;		!release output back to screen
			len=_namebuffer1-->0;
	#ifnot;
			glk_stream_close(glk_stream_get_current(), gg_arguments);
			glk_stream_set_current(sv);
			len=gg_arguments-->1;
	#endif;
		if(len>_RECOGBUFLEN) len=_RECOGBUFLEN;
		len=len+WORDSIZE; 

		check=WordAddress(wordnumber);							!locate the text
		count2=WordLength(wordnumber);							!get the size of it
		if(count2>MAX_SIZE) count2=MAX_SIZE;					!and recalculate to the first few pertinant letters
		for(markerend=markerstart=o=WORDSIZE:o<len:o++)		!search for words in short_name until match is found
		{
			if((o+1)==len)o=len; !--if reached end of line, then go ahead and adjust the pointer as though an word break was found
			if(o==len || _namebuffer1->o==' ' or ',' or '.')		!if found a whole word, or end of line...
			{
				if(o==markerstart) { !--if this word break is the first char, then this will be empty, so ignore
					markerstart++;
					continue;
				}
				markerend=o;									!adjust pointer and attempt to match...
				count=(markerend-markerstart);					
				
				#ifdef ORDEBUG; print"^For object ",(name)obj," testing word..."; for(i=0:i<count:i++) print(char)_namebuffer1->(i+markerstart);print"(length of ",count,")^";  #endif;
				
				if(count>MAX_SIZE) count=MAX_SIZE;				
					
				i=0;
				if(count==count2)
				{
					do 
					{
						chara=_namebuffer1->(i+markerstart);
	#ifndef TARGET_GLULX;
						if(chara>64 && chara <91) chara=chara+32;
	#ifnot;
						chara=glk_char_to_lower(chara);
	#endif;
						if(chara~=check->i) {
							break;
						}
						i++;
					}until(i==count);
				}
				if(i==count) {
					#ifdef ORDEBUG; print"^TRUE";  #endif;
					rtrue;
				}
				markerstart=o+1;
			}
		}
		return -1; 
		#ifdef ORDEBUG; print"^FALSE";  #endif;
	];

	object __checknounword
		with	word1	0 0 0 0 0 
		,		word2	0 0 0 0 0
		,		word3	0 0 0 0 0
		,		word4	0 0 0 0 0
		,		word5	0 0 0 0 0
		,		word6	0 0 0 0 0
		,		value	0 0 0 0 0 0
		,		ptr		0
		,		lastmove 0
		,		check_word[w p l a t o;
					
					if(self.lastmove~=turns){
						self.clear();
						return -1; !--not found
					}
					l=WordLength(w);
					if(l>MAX_SIZE)l=MAX_SIZE;
					a=WordAddress(w);
					for(o=0:o<self.ptr:o++){
						p=self.get_prop(o);	
						if(self.&p->0==l){
							for(t=0:t<l:t++){
								if(self.&p->(t+1)~=a->t) break;
							}
							if(t==l) return self.&value-->o; !--found, return value
						}
					}
					return -1; !--not found
				]
		,		get_prop[p retval;
					switch(p){
						0: retval=word1;	
						1: retval=word2;
						2: retval=word3;
						3: retval=word4;
						4: retval=word5;
						5: retval=word6;
					}
					return retval;
				]
		,		add_word[w val p l a t;
					p=self.get_prop(self.ptr);
					l=WordLength(w);
					a=WordAddress(w);
					if(l>MAX_SIZE)l=MAX_SIZE;
					self.&p->0=l;
					for(t=0:t<l:t++) self.&p->(t+1)=a->t;
					self.&value-->(self.ptr)=val;

					self.ptr++;
				]
		,		clear[t;
					self.ptr=0;
					self.&word1->0=0;
					self.&word2->0=0;
					self.&word3->0=0;
					self.&word4->0=0;
					self.&word5->0=0;
					self.&word6->0=0;
					for(t=0:t<self.ptr:t++) self.&value-->t=false;
					self.lastmove=turns;


				]
	;
	[ NounWord i j s o r;
		i=NextWord();
		if (i==0) { !--if not a recognised word
			r=__checknounword.check_word(wn-1); !--lets do a little hocus pocus to ensure that we only scan the short_names of nearby objects once per turn (instead of the thirty or so that this routine is called.)
			if(r==true) return '.FALSEWORD';
			if(r==-1){
				objectloop(o~=actor){ !--can this be made better????
					if(MatchAgainstShortName(o,wn-1)==true) {
						__checknounword.add_word(wn-1,true);
						return '.FALSEWORD';
					}
				}
				__checknounword.add_word(wn-1,false);
			}
			rfalse;
		}

		if (i==ME1__WD or ME2__WD or ME3__WD) return 1;
#ifdef ME4__WD;
		if (i==ME4__WD) return 1;
#endif;
		s = LanguagePronouns-->0;
		for (j=1 : j<=s : j=j+3)
			if (i == LanguagePronouns-->j)
				return j+2;
		if ((i->#dict_par1)&128 == 0) rfalse;
		return i;
	];
	!--this is the same Identical routine from parserm 6/10, however blocks of original comments have been removed (for clarity as to purpose of this modification)
	[ Identical o1 o2 p1 p2 n1 n2 i j flag;
		if (o1==o2) rtrue;  ! This should never happen, but to be on the safe side
		if (o1==0 || o2==0) rfalse;  ! Similarly
		if (parent(o1)==compass || parent(o2)==compass) rfalse; ! Saves time

		!----------------------------------------------------
		! New Code here... Matches names text
		!----------------------------------------------------
		!--capture name of first object to first buffer
		_recname_dyna.buf=_namebuffer1;
		_recname_dyna.capture();
		print(name)o1;
		_recname_dyna.release();
		!--capture name of second object to second buffer
		_recname_dyna.buf=_namebuffer2;
		_recname_dyna.capture();
		print(name)o2;
		_recname_dyna.release();
		!--make match
#ifdef ORDEBUG; print "ORRecogName: Testing ",(name)o1," against ",(name)o2,"."; #endif;
			if(_recname_dyna.matches(_namebuffer1)==false) {
#ifdef ORDEBUG; print "names don't match."; #endif;
				rfalse;
			}
#ifdef ORDEBUG; print "names DO match."; #endif;
		!----------------------------------------------------
		! end new Code
		!----------------------------------------------------
		if (o1.parse_name~=0 || o2.parse_name~=0)
		{   if (o1.parse_name ~= o2.parse_name) rfalse;
			parser_action=##TheSame; parser_one=o1; parser_two=o2;
			j=wn; i=RunRoutines(o1,parse_name); wn=j;
			if (i==-1) rtrue; if (i==-2) rfalse;
		}
		p1 = o1.&1; n1 = (o1.#1)/2;
		p2 = o2.&1; n2 = (o2.#1)/2;
		for (i=0:i<n1:i++)
		{   flag=0;
			for (j=0:j<n2:j++)
				if (p1-->i == p2-->j) flag=1;
			if (flag==0) rfalse;
		}
		for (j=0:j<n2:j++)
		{   flag=0;
			for (i=0:i<n1:i++)
				if (p1-->i == p2-->j) flag=1;
			if (flag==0) rfalse;
		}
		rtrue;
	];

!======================================================================================
#endif; #ifnot; #ifndef ORRecogName_DONEGRAMMAR; constant ORRecogName_DONEGRAMMAR; #ifdef ORRecogName_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================