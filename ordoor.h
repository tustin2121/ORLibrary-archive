!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORDoor	[Z, GLULX]
! The ORDoor class represents a simple door, but utilizes generic code for the door_to, 
! door_dir properties so that objects derived from ORDoor class need not define these
! properties.
!
! Additionally, this module will probe the world map for directions that lead to 
! ORDoor objects and attempt to fill in the found_in property.  This effectively 
! reduces door creation to:
!
!		ORDoor RedDoor "red door" 
!			with	name 'door' 'red';
!
! ...assuming that two rooms point to the RedDoor object.
!
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
!
! The following sample conventional door:
!
!		Object RedDoor "red door"
!			 with	name "door" "red"
!			 ,     door_dir 
!					[; 
!						if (location == LivingRoom) 
!							return n_to; 
!						else
!							return s_to;
!					]
!	        ,     door_to 
!					[; 
!						if (location == LivingRoom) 
!							return DiningRoom; 
!						else 
!							return LivingRoom;
!					]
!	        ,     found_in LivingRoom DiningRoom
!			has openable door static scenery;
!
! Is reduced to:
!
!		ORDoor RedDoor "red door" with	name "door" "red";
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORDoor";					!Constant USE_ORDoor; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORDoor; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.04.20	Initial Creation
! 2001.05.10	Made modifications to the door to look at the current actor's location
!				rather than player's location. This will make the doors work for NPCs as well.
! 2002.02.02	Made modification to default an empty found_in property for the benifit of 
!				programatic initialization.
! 2002.02.26	Fit to new template
! 2003.12.23	Combined ORDoorInit and ORDoor
! 2004.01.11	Modified to use the stdlib 6/11b2 new ext_initialise functionality
!--------------------------------------------------------------------------------------
message "          Processing library extension ORDoor...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORDoor forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORDoor has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORDynaString_DONEREPLACE; default USE_ORDynaString 0; message "          [ORDoor forcing inclusion of ORDynaString]"; #include "ORDynaString"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORDoor_DONEREPLACE; constant ORDoor_DONEREPLACE; #ifdef ORDoor_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORDoor_DONEMESSAGE; constant ORDoor_DONEMESSAGE;  #ifdef ORDoor_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORDoor requires the OREnglish file.";#endif; 
	system_file;

	class ORDoor
		has static door openable scenery
		with	door_to 
				[;
					if((self.&found_in-->0)==parent(actor)) return self.&found_in-->1;
					return self.&found_in-->0;
				]
		,		door_dir
				[;
					if(parent(actor) provides n_to && parent(actor).n_to==self) return n_to;
					if(parent(actor) provides s_to && parent(actor).s_to==self) return s_to;
					if(parent(actor) provides e_to && parent(actor).e_to==self) return e_to;
					if(parent(actor) provides w_to && parent(actor).w_to==self) return w_to;
					if(parent(actor) provides ne_to && parent(actor).ne_to==self) return ne_to;
					if(parent(actor) provides se_to && parent(actor).se_to==self) return se_to;
					if(parent(actor) provides nw_to && parent(actor).nw_to==self) return nw_to;
					if(parent(actor) provides sw_to && parent(actor).sw_to==self) return sw_to;
					if(parent(actor) provides u_to && parent(actor).u_to==self) return u_to;
					if(parent(actor) provides d_to && parent(actor).d_to==self) return d_to;
					return self;
				]
		,		found_in 0 0
	;
!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORDoor_DONECODE;constant ORDoor_DONECODE; #ifdef ORDoor_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	ORDynaString __ords LibraryExtensions
	!--attaching the initialization code here, because it is already an existing object. 
	with	ext_initialise[room dir dest; 
					objectloop(room provides	n_to or s_to or e_to or w_to
									or	nw_to or sw_to or ne_to or se_to
									or	u_to or d_to){
						objectloop(dir in compass){
							if(room provides (dir.door_dir)){
								if(metaclass(room.(dir.door_dir))~=object) continue; !--don't do anything with a string...
								__ords.capture(); !--suppress any messages that the direction routines may spit out...
								dest=valueorrun(room,(dir.door_dir));
								__ords.release();
								if(dest ofclass ORDoor && WordInProperty(room,dest,found_in)==false){
									if((dest.&found_in-->0)==0)
										(dest.&found_in-->0)=room;
									else{
										if((dest.&found_in-->1)==0)  
											(dest.&found_in-->1)=room;
										else{
	#ifdef DEBUG;
											print "Warning. Inconsistency found with map.  Door ~",(object)dest,"~ has too many found_in entries detected.";
	#endif;
										}
									}
								}
							}
						}
					}
	#ifdef DEBUG;
					objectloop(dest ofclass ORDoor && dest provides found_in && dest.#found_in>1){
						if(dest.&found_in-->0==0 || dest.&found_in-->1==0) {
							print "^ERROR found.  Door ",(object)dest," does not connect two locations.  An unmodified version of the library routine MoveFloatingObjects() will generate errors.^";
						}
					}
	#endif;
			]
	;
!======================================================================================
   #endif; #ifnot; #ifndef ORDoor_DONEGRAMMAR; constant ORDoor_DONEGRAMMAR; #ifdef ORDoor_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================