!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.03.05 ORListManager [Z, GLULX]
! A small class for managing property lists; supports stacks, queues, and generic 
! list access methods.
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
! ORListManager
! A small class for managing property lists; supports stacks, queues, and generic 
! list access methods. This class is similar to the OROptionList class, which it was
! written to replace, however it does not encapsulate the managed list itself.
! All methods now require the property list as a parameter. This enables the use of 
! multiple lists.
!--------------------------------------------------------------------------------------
! AutoDep:	
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORListManager; #include "ORListManager"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORListManager; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2004.02.28	Initial Creation
! 2004.03.05	Fixed small assignment bug in the insert routine.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORListManager...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORListManager forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORListManager has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORListManager_DONEREPLACE; constant ORListManager_DONEREPLACE; #ifdef ORListManager_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	class ORListManager
		with Count[prop t;
				for(t=0:t<(self.#prop/WORDSIZE):t++){
					if(self.&prop-->t==0) return t;
				}
				return (self.#prop/WORDSIZE);
			]
!		,	dump[prop t; print "^"; 
!				for(t=0:t<(self.#prop/WORDSIZE):t++) print self.&prop-->t;
!			]
		,	Clear[prop t cnt; !--remove everything from the list
				cnt=self.count(prop);
				for(t=0:t<cnt:t++) {
					self.Remove(prop,0);
				}
			]	
 		,	Get[prop n;
				if(n>=self.count(prop)) "Error (ORList): index out of range (Get)";
				return self.&prop-->n;
			]
		,	Set[prop n val;
				if(n>=self.count(prop)) "Error (ORList): index out of range (Set)";
				self.&prop-->n=val;
			]
		,	Find[prop Option t;
				for(t=0:t<self.#prop/WORDSIZE:t++){
					if(self.&prop-->t==Option){
						return t;
					}
				}
				return -1;
			]
		,	Remove[prop number t pos; !--eliminate a position and shift everything forward
				pos=((self.#prop)/WORDSIZE)-1;
				for(t=number:t<pos:t++){
					self.&prop-->t=self.&prop-->(t+1);
				}
				self.&prop-->pos=0;
			]
		,	RemoveEntry[prop Option n; !--Locate an option and remove it
				n=self.Find(prop, Option);
				if(n~=-1) self.Remove(prop, n);
			]
		,	insert[prop newOption pos t; !--add an option and associated value to the first position
				if(self.count(prop)>=(self.#prop/WORDSIZE)) rfalse; !--already filled up, no room left

				for(t=self.#prop/WORDSIZE-1:t>pos:t--){
					self.&prop-->t=self.&prop-->(t-1);
				}
				self.&prop-->pos=newOption;
				rtrue;
			]
		,	insert_end[prop newOption; !--add an option and associated value to the first unused position
				return self.insert(prop,newOption,self.count(prop));
			]
		,	pop[prop pos retval;
				retval=self.Get(prop,pos);
				self.Remove(prop,pos);
				return retval;
			]
		,	pop_end[prop end retval;
				end=self.count(prop); 
				if(end<1) return 0;
				retval=self.Get(prop,end-1);
				self.Remove(prop,end-1);
				return retval;
			]
	;

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORListManager_DONEMESSAGE; constant ORListManager_DONEMESSAGE;  #ifdef ORListManager_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORListManager requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORListManager_DONECODE;constant ORListManager_DONECODE; #ifdef ORListManager_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #ifnot; #ifndef ORListManager_DONEGRAMMAR; constant ORListManager_DONEGRAMMAR; #ifdef ORListManager_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================