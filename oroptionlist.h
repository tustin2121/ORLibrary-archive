!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 OROptionList [Z, GLULX]
! A class which maintains a list of value pairs.
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
! A more detailed description of this file, what it does, and how to use it goes here.
!
!--------------------------------------------------------------------------------------
! AutoDep:	
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_OROptionList; #include "OROptionList";	#endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_OROptionList; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.04.03	Initial Creation
! 2002.05.01	Modified comments.
! 2002.07.15	Moved class definition to the messages section. Additionally created 
!				an field to hold an addition association and methods to set the various 
!				componants too.
! 2003.02.06	Eliminated the default number of blank entries of the properties 
!				OptionList, AssociationList, and AssociationList2.  This can (and should)
!				be set to better values in derived classes or instances of this class, 
!				but is being eleminated from the base class to keep from allocating 
!				readable memory needlessly.
! 2004.01.26	Reconsidered previous change.  Although the fifteen items at a time 
!				was too wasteful, one item renders this class unusable without defining
!				a property that is already defined.  Changing the default size of 
!				OptionList, AssociationList, and AssociationList2 to four.
!--------------------------------------------------------------------------------------
message "          Processing library extension OROptionList...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [OROptionList forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "OROptionList has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef OROptionList_DONEREPLACE; constant OROptionList_DONEREPLACE; #ifdef OROptionList_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef OROptionList_DONEMESSAGE; constant OROptionList_DONEMESSAGE;  #ifdef OROptionList_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! OROptionList requires the OREnglish file.";#endif; 
	class OROptionList
		with OptionList			0 0 0 0
		,	AssociationList		0 0 0 0
		,	AssociationList2	0 0 0 0
		,	GetOption[n;
				if(n>=self.count()) "Error (OROptionList): index out of range (GetOption)";
				return self.&OptionList-->n;
			]
		,	GetAssociation[n;
				if(n>=self.count()) "Error (OROptionList): index out of range (GetAssociation)";
				if(((self.#AssociationList)/WORDSIZE)>n) return self.&AssociationList-->n;
				return 0;
			]
		,	GetAssociation2[n;
				if(n>=self.count()) "Error (OROptionList): index out of range (GetAssociation2)";
				if(n>=self.count()) return -1;
				if(((self.#AssociationList2)/WORDSIZE)>n) return self.&AssociationList2-->n;
				return 0;
			]
		,	SetOption[n val;
				if(n>=self.count()) "Error (OROptionList): index out of range (SetOption)";
				self.&OptionList-->n=val;
			]
		,	SetAssociation[n val;
				if(n>=self.count()) "Error (OROptionList): index out of range (SetAssociation)";
				self.&AssociationList-->n=val;
			]
		,	SetAssociation2[n val;
				if(n>=self.count()) "Error (OROptionList): index out of range (GetAssociation2)";
				self.&AssociationList2-->n=val;
			]
		,	Count[t;
				for(t=0:t<(self.#OptionList/WORDSIZE):t++){
					if(self.&OptionList-->t==0) return t;
				}
				return (self.#OptionList/WORDSIZE);
			]
		,	ClearOptionList[t; !--remove everything from the list
				for(t=0:t<self.#OptionList/WORDSIZE:t++){
					self.&OptionList-->t=0;
					if(((self.#AssociationList)/WORDSIZE)>t)self.&AssociationList-->t=0;
					if(((self.#AssociationList2)/WORDSIZE)>t)self.&AssociationList2-->t=0;
				}
			]	
		,	RemoveOptionByNumber[number t pos; !--eliminate a position and shift everything forward
				pos=((self.#OptionList)/WORDSIZE)-1;
				for(t=number:t<pos:t++){
					self.&OptionList-->t=self.&OptionList-->(t+1);
					if(((self.#AssociationList)/WORDSIZE)>t+1) self.&AssociationList-->t=self.&AssociationList-->(t+1); 
					if(((self.#AssociationList2)/WORDSIZE)>t+1)self.&AssociationList2-->t=self.&AssociationList2-->(t+1); 
				}
				self.&OptionList-->pos=0;
				if(((self.#AssociationList)/WORDSIZE)>pos) self.&AssociationList-->pos=0;
				if(((self.#AssociationList2)/WORDSIZE)>pos) self.&AssociationList2-->pos=0;
			]
		,	RemoveOption[Option n; !--Locate an option and remove it
				n=self.LocateOption(Option);
				if(n~=-1) self.RemoveOptionByNumber(n);
			]
		,	LocateOption[Option t;
				for(t=0:t<self.#OptionList/WORDSIZE:t++){
					if(self.&OptionList-->t==Option){
						return t;
					}
				}
				return -1;
			]
		,	PushOption[newOption AltValue AltValue2 t; !--add an option and associated value to the first unused position
				if(self.LocateOption(newOption)>=0) rtrue; !--already is considering that Option
				for(t=0:t<self.#OptionList/WORDSIZE:t++){
					if(self.&OptionList-->t==0){
						self.&OptionList-->t=newOption;
						if(self.#AssociationList/WORDSIZE>t)self.&AssociationList-->t=AltValue;
						if(self.#AssociationList2/WORDSIZE>t)self.&AssociationList2-->t=AltValue2;
						rtrue;
					}
				}
				rfalse; !--not enough room to record new option
			]
	;
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef OROptionList_DONECODE;constant OROptionList_DONECODE; #ifdef OROptionList_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #ifnot; #ifndef OROptionList_DONEGRAMMAR; constant OROptionList_DONEGRAMMAR; #ifdef OROptionList_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================