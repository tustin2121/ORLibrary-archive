!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORPathMaker	[Z, GLULX]
! Object which can determine a path between two rooms (or objects)
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
! The ORPathMaker object will determine a path between two rooms.  Two starting  objects 
! are taken by the determine_path routine. These can be rooms or objects or characters. 
! Passage through doors is resolved, however if the doors require keys, then the starting 
! object (first passed in) must be an object that containes the needed keys. This allows 
! an NPC with the appropriate key to successfully take a path that an NPC without the 
! appropriate key could not.
!
! Sample use:
!
!	ORPathMaker.determine_path(player,bathroom);
!
!	ORPathMaker.determine_path(bathroom,bedroom);
!
! The return values for this routine are:
!	
!	-1 : There is no connecting path between the two given objects.
!	-2 : Ran out of workspace while trying to calculate path.  Define the constant
!			PATHDEPTH with a larger than default value.
!	-3 : Although there WAS a path correctly determined, there was NOT enough space in 
!			the path property to contain it. 
!	room obj : The first connected room that must be traveled to in order to finally reach
!			the destination.
!
! Additionally, once the path has been correctly determined, the size property contains 
! the size and the path property contains the final path.
!
! There is a limited amount of scratch workspace used to determine a correct path.  If 
! there is not enough space, the path will not be findable.  The constant PATHDEPTH
! is used to allocate this scratch space.  By default it is set to 30, however it can be 
! defined at a greater value in the main program's source.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORPathMaker";					!Constant USE_ORPathMaker; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORPathMaker; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.09	Initial Creation
! 2002.02.18	Fixed bug in false assignment of self.npc (and verified GLULX support)
! 2002.02.26	Fit to new template.
! 2002.05.27	Fixed bug where strings in mapping caused fatal error under some 
!				interpreters.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORPathMaker...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORPathMaker forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORPathMaker has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORPathMaker_DONEREPLACE; constant ORPathMaker_DONEREPLACE; #ifdef ORPathMaker_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORPathMaker_DONEMESSAGE; constant ORPathMaker_DONEMESSAGE;  #ifdef ORPathMaker_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORPathMaker requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORPathMaker_DONECODE;constant ORPathMaker_DONECODE; #ifdef ORPathMaker_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	#ifndef PATHDEPTH;constant PATHDEPTH 30; #endif;
	array __orpathmaker_room-->PATHDEPTH;
	array __orpathmaker_distance->PATHDEPTH;
	object ORPathMaker
		with	path 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		,		end_ptr	0
		,		cur_ptr	0
		,		size 0
		,		npc	ORPathMaker !--self if not defined...strange, but it will work
		,		find_room_distance[room t;
					for(t=0:t<self.end_ptr:t++){
						if(__orpathmaker_room-->t==room) 
							return __orpathmaker_distance->t;
					}
					return -1;
				]
		,		add[room level;
					if(self.find_room_distance(room)==-1) {
						__orpathmaker_room-->self.end_ptr=room;
						__orpathmaker_distance->self.end_ptr=level;
						self.end_ptr++;
					}
				]
		,		determine_path[npc_cntx endroom t startroom; 
					if(parent(npc_cntx)==0){
						startroom=npc_cntx; !--room was passed in instead of a character
						self.npc=self;
					}
					else{
						startroom=scopeceiling(npc_cntx);
						self.npc=npc_cntx;
					}
					if(parent(endroom)~=0)endroom=scopeceiling(endroom); !--an object was specified instead of a room... determine
					if(startroom==endroom) return 0;
					for(t=0:t<self.end_ptr:t++){ 	!-- first lets clear everything out 
						__orpathmaker_room-->t=0;
						__orpathmaker_distance->t=0;
					}
					self.end_ptr=self.cur_ptr=0;
					self.add(startroom,0); 
					t=self.process(endroom);
					if(t<0) return t; !--- Errors? -1: not reachable; -2: out of workspace
					return self.unwind_path(); !---all was good, lets fill in the path property and return (either an error of the first room in the list...)
				]
		,		resolve_door[obj last_room a p l al n retval;
					if(obj hasnt door) return obj;
					if(obj has locked && ((obj provides with_key)==false || (obj.with_key in self.npc)==false)) return 0; !--can it be opened by the character in question?
					!--depending on who wrote it, the door might look at parent(actor), parent(player), location, or actors_location. 
					a=actor; p=player; l=location; al=actors_location; n=parent(self.npc); !--save old
					actor=self.npc; player=self.npc; location=last_room; actors_location=last_room; move self.npc to last_room;	!--set new
					retval=valueorrun(obj,door_to);			!--determine door
					actor=a; player=p; location=l; actors_location=al; 
					if(n==nothing or 0)
						remove self.npc;
					else
						move self.npc to n;!--restore the old
					return retval;
				]
		,		process[endroom r cr cl t;
					while(self.cur_ptr<self.end_ptr){
						cr=__orpathmaker_room-->(self.cur_ptr);
						cl=self.find_room_distance(cr); 
						objectloop(t in compass){
							if(cr provides (t.door_dir)){
								r=cr.(t.door_dir);
								if(metaclass(r)==object) {
									if(r has door) r=self.resolve_door(r,cr);
									if(r~=0){
										self.add(r,cl+1);
										if(r==endroom) return cl+1; !--success.  return needed size for path
										if(self.end_ptr>PATHDEPTH) return -2; !--Out of space
									}
								}
							}
						}
						self.cur_ptr++;
					}
					return -1; !--completed search, with no possible path found
				]
		,		unwind_path[pos d room r t; !--called once the search algorith has completed successfully, to find the shortest path and put it in  the property pointed to by self.path
					room=__orpathmaker_room-->(self.end_ptr-1); !--locate the endroom
					self.size=pos=self.find_room_distance(room); !--size of path 
					
					if((self.#path/WORDSIZE)<pos) return -3; !--path too small
					while(pos>0){
						self.&path-->(pos-1)=room;
						objectloop(d in compass){
							if(room provides (d.door_dir)){
								r=room.(d.door_dir);
								if(r has door) r=self.resolve_door(r,room);
								if(r~=0){
									t=self.find_room_distance(r);
									if(t>-1 && t<pos) break;
								}
							}
						}
						room=r;
						pos--;
					}
					return self.&path-->0;
				]
	;
!======================================================================================
#endif; #ifnot; #ifndef ORPathMaker_DONEGRAMMAR; constant ORPathMaker_DONEGRAMMAR; #ifdef ORPathMaker_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================