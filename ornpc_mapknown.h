!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORNPC_MapKnown	[Z, GLULX]
! For NPCs that know their surroundings.  Decreases the likelyhood that the NPC will
! become "lost" and have to "wander".
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
! AutoDep :	ORPathMaker
!			ORNPC_Movement
!--------------------------------------------------------------------------------------
! NPC Componant Priority: 15  
! The lower the number, the further down the "class" line.  The greater the number, 
! the earlier it needs to come on the class line. Modules with the same priority can
! effectively be exchanged in order without effect.
!--------------------------------------------------------------------------------------
! This NPC componant class modifies the behavior of ORNPC_Movement.  It is required 
! that NPCs which derive from this class also derive from ORNPC_Movement.
!
! What does this module do?
! The ORNPC_Movement class allows NPCs to follow a path.  There are requirements to this 
! path such as the rooms in the path need to be contiguous.  If the NPC ever arrives in 
! a room which is not adjacent to the next room in his path, the NPC can become "lost" 
! and wander around until he "recognizes" his location (wanders into a location that 
! appears in his path).  
!
! This module removes the need for rooms in the path to be contiguous. It leverages the
! ORPathMaker module to generate the shortest path between the NPC's current location
! and target location thereby enabling the NPC to arrive at his path and never wander
! around lost (assuming the destination is actually reachable and falls within the 
! limitations of the ORPathMaker object.)
!
! Other than adding the ORNPC_MapKnown class to the NPC's inheritence chain (and 
! prerequisits) no additional code need be implemented to make this functionality occur.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORNPC_MapKnown";					!Constant USE_ORNPC_MapKnown; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORNPC_MapKnown; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.09	Initial Creation
! 2002.02.26	Fit to new template
!--------------------------------------------------------------------------------------
message "          Processing library extension ORNPC_MapKnown...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORNPC_MapKnown forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORNPC_MapKnown has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORPathMaker_DONEREPLACE; default USE_ORPathMaker 0; message "          [ORNPC_MapKnown forcing inclusion of ORPathMaker]"; #include "ORPathMaker"; #endif; 
	#ifndef ORNPC_Movement_DONEREPLACE; default USE_ORNPC_Movement 0; message "          [ORNPC_MapKnown forcing inclusion of ORNPC_Movement]"; #include "ORNPC_Movement"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORNPC_MapKnown_DONEREPLACE; constant ORNPC_MapKnown_DONEREPLACE; #ifdef ORNPC_MapKnown_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORNPC_MapKnown_DONEMESSAGE; constant ORNPC_MapKnown_DONEMESSAGE;  #ifdef ORNPC_MapKnown_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORNPC_MapKnown requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNPC_MapKnown_DONECODE;constant ORNPC_MapKnown_DONECODE; #ifdef ORNPC_MapKnown_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	class ORNPC_MapKnown
		with missing_path[t; 
				self.path_ptr=self.path_ptr-self.path_ptr_inc;	!-- adjust pointer back to previous position
				t=self.&path-->(self.path_ptr);	
				t=ORPathMaker.Determine_Path(self,t);
				if(t>0){
					t=self.ajoining_room_to_direction(t);	
					return self.move_to(t); !-- found a way to the needed path, so DO NOT adjust pointer back again
				}
				self.path_ptr=self.path_ptr-self.path_ptr_inc;	!-- adjust pointer back to previous position
				return self.ORNPC_Movement::missing_path(); !--do old stuff
			]
	;
	object _npcmapknowninit LibraryExtensions
		with ext_initialise[o; 
					objectloop(o ofclass ORNPC_MapKnown){
						if((o ofclass ORNPC_Movement)==false) print "^[ERROR: ORNPC_Movement behavior requires ORNPC_DoVerb behavior. (object",(name)o,")]^";
					}
				]
	;
!======================================================================================
#endif; #ifnot; #ifndef ORNPC_MapKnown_DONEGRAMMAR; constant ORNPC_MapKnown_DONEGRAMMAR; #ifdef ORNPC_MapKnown_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================