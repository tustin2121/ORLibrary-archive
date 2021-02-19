!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORDispensor	[Z, GLULX]
! An object which dispenses a seemingly endless supply of other objects, like a pot
! of gold.
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
! This is an object that is designed to dispense quantities of a specific object.  It is useful 
! for things like a pot of gold coins, where the pot may contain numerous gold coins, but the 
! developer does not want to actually create all coins at once.  The class of the items that
! the derived object dispenses must be defined as the property "itemclass".  When the Dispensor
! object is initialised (automatically), a number of objects of the itemclass type are created.
! This number is stored in the "initial_count" property.  When the items are removed, more are 
! generated to replace them, when the items are returned to the dispensor, they are destroyed.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORDispensor";					!Constant USE_ORDispensor; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORDispensor; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.01.04	Initial Creation (not really but this is the earliest date I could find
!				without searching my code archive)
! 2002.02.26	fit to new template.
! 2002.04.03	modified faulty WORDSIZE definition.
! 2002.05.12	Added check to verify that player is present before 
!				printing messages. 
! 2002.10.19	Added autodep for ObjectInitialise 
!--------------------------------------------------------------------------------------
message "          Processing library extension ORDispensor...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORDispensor forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORDispensor has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORDispensor_DONEREPLACE; constant ORDispensor_DONEREPLACE; #ifdef ORDispensor_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORDispensor_DONEMESSAGE; constant ORDispensor_DONEMESSAGE;  #ifdef ORDispensor_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORDispensor requires the OREnglish file.";#endif; 
	class ORDispensor
		with	itemclass
		,		itemobj 0
		,		conceal_dispensing_items false
		,		accept_alternate_items true
		,		initial_count 1
		,		initial 
				[;
					Locale(self);
				]
		,		newitem
				[numb o t;
					if(numb<1) numb=1;
					for(t=0:t<numb:t++)
					{
						o=(self.itemclass).create();
						move o to self;
						if(self.conceal_dispensing_items==true) give o concealed;
					}
					self.itemobj=o; !make sure that we have a real object reference
				]
		,		before
				[;
					LetGo: !create another object to replace the released one
						if(noun ofclass self.itemclass) {
							give noun ~concealed;
							self.newitem();
						}
					receive: !destroy a received itemobj if it is one of our items
						if(noun ofclass self.itemclass)
						{
							printorrun(self,receive_item_msg);
							self.itemclass.destroy(noun);
							return true;
						}
						if(self.accept_alternate_items==false){
							printorrun(self,cannot_receive_item_msg);
							return true;
						}
				]
		,		receive_item_msg[;  
					if(CanPlayerWitness()==false)rtrue;
					print (ig)CIVerb(actor,"put","put")," ",(the) noun," into ", (the) self, " with the rest.^";
				]
		,		cannot_receive_item_msg[;  
					if(CanPlayerWitness()==false)rtrue;
					print (ig)CICant(actor)," put ",(the) noun," into ", (the) self, ",.^";
				]
		has		container
	;
!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORDispensor_DONECODE;constant ORDispensor_DONECODE; #ifdef ORDispensor_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	#ifndef CanPlayerWitness;[CanPlayerWitness; rtrue; ]; #endif;
	object _dispensorinit LibraryExtensions
		with ext_initialise[o t; 
				objectloop(o ofclass ORDispensor){
					for(t=0:t<o.initial_count:t++)o.newitem();
				}
			]
	;
!======================================================================================
   #endif; #ifnot; #ifndef ORDispensor_DONEGRAMMAR; constant ORDispensor_DONEGRAMMAR; #ifdef ORDispensor_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================