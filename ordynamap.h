!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORDynaMap [Z, GLULX]
! This module dynamically creates the ties between certain rooms based upon movements 
! by the player, similar to an effect in the "mars" scene of Adam Cadre's "Photopia".
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
! The use is fairly straightforward: Simply create an instance of the ORDynaMap class
! with the found_in property containing a list of the rooms that are to be arranged, 
! in the order that they are to appear.  The first entry in the list should already 
! be accessable by the player.
! 
! As for the rooms that are to be created, they should initially provide all directions.
! The cant_go property on the ORDynaMap object, if defined, will be propogated down to 
! the rearanged rooms which also provide cant_go, but that do not define it.
!
! To simplify the creation of the rearangable rooms, a simple class is provided 
! that defines all directons and the cant_go property as zero.  Note the following example
! which allows the player to explore in virtually any direction from the water's edge:
!
!	ORDynaMap with cant_go "The trees are too dense to travel in that direction."
!		,	found_in wateredge sparseforest forestmidst forestclearing treasuretrove
!	;
!	ORDynaMapRoom wateredge "Edge of water/forest"
!		with s_to "Your boat is that way, but you can't go home until the treasure is found."
!		,	description "This is a small strip of land separating the southern inlet
!				of ocean from the vast forest in all other directions."
!	;
!	ORDynaMapRoom sparseforest "Sparse Forest"
!		with description "This is the start of a vast forest.  The trees here are thin
!				and sparse."
!	;
!	ORDynaMapRoom forestmidst "Midst of Forest"
!		with description "Smack dab in the middle of the forest. Trees are everywhere."
!	;
!	ORDynaMapRoom forestclearing "Forest Clearing"
!		with description "Suddenly clear of trees.  You feel yourself draw closer to the target."
!	;
!	ORDynaMapRoom treasuretrove "Pile of Treasure"
!		with description "At last! The long sought-after pile of treaure.  You are victorious."
!	;
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORDynaMap";					!Constant USE_ORDynaMap; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORDynaMap; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.01.01	Initial Creation
! 2002.02.17	Added GLULX Support
! 2002.02.26	Fit to new template
! 2002.04.27	Stopped incrementing through rooms when a room cannot be traveled to.
! 2003.02.16	Added a property "complete" to show when the iteractions are done.
!				Also added the in_to and out_to properties.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORDynaMap...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORDynaMap forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORDynaMap has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORReverseDirection_DONEREPLACE; default USE_ORReverseDirection 0; message "          [ORDynaMap forcing inclusion of ORReverseDirection]"; #include "ORReverseDirection"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORDynaMap_DONEREPLACE; constant ORDynaMap_DONEREPLACE; #ifdef ORDynaMap_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORDynaMap_DONEMESSAGE; constant ORDynaMap_DONEMESSAGE;  #ifdef ORDynaMap_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORDynaMap requires the OREnglish file.";#endif; 
	system_file;
	class	ORDynaMapRoom
		with	n_to , s_to, e_to, w_to
		,		ne_to, nw_to, se_to, sw_to
		,		u_to, d_to
		,		in_to, out_to
		,		cant_go 
	;
	class ORDynaMap 
		has		concealed
		with	found_in 0
		,		cant_go	0
		,		fi_ptr 0
		,		complete false
		,		react_before[curloc destroom;
					go: if(actor~=player) rfalse;
						curloc=scopeceiling(player);
						do{
							!if(self.fi_ptr>=(self.#found_in/WORDSIZE)) rfalse;
							if(self.complete==true) rfalse;
							destroom=self.&found_in-->(self.fi_ptr);
							self.fi_ptr++;
							if(self.cant_go~=0 && destroom provides cant_go && destroom.cant_go==0) destroom.cant_go=self.cant_go;
						}until(destroom~=curloc);
						if(curloc provides (noun.door_dir) && curloc.(noun.door_dir)==0){
							curloc.(noun.door_dir)=destroom;
							destroom.(ReverseDirection(noun.door_dir))=curloc;
						}
						else self.fi_ptr--; !--wasn't able to go that way, decrement for next attempt
						if(self.fi_ptr>=(self.#found_in/WORDSIZE)) self.complete=true;
				]
	;
!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORDynaMap_DONECODE;constant ORDynaMap_DONECODE; #ifdef ORDynaMap_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
   #endif; #ifnot; #ifndef ORDynaMap_DONEGRAMMAR; constant ORDynaMap_DONEGRAMMAR; #ifdef ORDynaMap_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================