!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.09 ORLookRoom	[Z, GLULX]
! This module adds the functionality of examining a neighboring room.
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
! Using this module, a player who is presented with the description of a room similar
! to the following:
!
!     You are in the Dining Room.  To the North is the Laboratory.
!
! can have the ability to use any of the following:
!		
!     look at laboratory
!     examine laboratory	
!		
! The description of a neighboring room is determined by the following logic:
!     
!	1) if the current room has a direction-description property defined, then 
!		 that is used. The following are direction-description properties:
!		
!				n_look
!				s_look
!				e_look
!				w_look
!				nw_look
!				sw_look
!				ne_look
!				se_look
!				u_look
!				d_look
!
!	2) Failing the above, the room being looked at is searched for the		
!		'remote_description' property.  If found, then that is used.
!
!	3) Alternately, the room being looked at is checked for the property 
!		'describe_as_if_present' to be set to true.  If so, then the normal 
!		room descriptions are called as though the character were actually 
!		present in the room.
!
!	4) Finally, if the "Look <Direction>" form of this command was used, and 
!		all of the above attempts to determine a remote description fail, then
!		current room is checked for the "cant_look" property which is printed.
!	
! Additionally, for completness, the commands "examine room" and "look around" have 
! been implimented along with minor variations.
! 
! Room visibility is generally determined by a direct (or indirect, in the case of 
! a door) connection in the game's map, but an additional property "visible_from"
! can be defined for a room to list other rooms from which it can be seen.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORLookRoom";					!Constant USE_ORLookRoom; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORLookRoom; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.01.03	Initial Creation (actually earlier, but this is the only datestamp still 
!					available without having to search archived code)
! 2002.02.09	1. Added support for room visibility when it is not actually accessible 
!				(i.e.: A fastfood resturante's floor room may be visible from behind the 
!				counter even though there may not be a direct pathway connecting the two.
!				2. Bug Fix: Add support for floating objects to the DescribeRoomRemotely 
!				routine.
! 2002.02.26	Fit to new framework
! 2002.03.11	Fix to the "What lies in that direction is unclear" message.
! 2002.07.07	Added menu documentation.
! 2002.07.20	Fixed naming conflict with up-and-coming menu speak system (against 
!				documentation menu) 
! 2002.09.02	Converted attributes previously defined in this module to properties.
! 2002.10.19	Added autodep for ObjectInitialise 
! 2003.01.07	Added new cant_look property.  Also placed all messages actual message format
!				so that they can be overriden by devleopers in the LibraryMessages object.
! 2003.01.20	Added minor extension to the LookDoor grammer.
! 2003.02.02	Fixed bug with cant_look property. Also added code to look at current room by name.
! 2003.02.02	Modified the documentation some.
! 2003.02.15	Fixed some line-space issues.
! 2003.02.19	Added better ifdef for self docs
! 2003.02.19	moved grammar for look room to "last" instead of first.  This caused problems with
!				objects contained in a room with a similar name (e.g.: an altar in an altar room)
! 2003.03.11	Modified grammar for examine to treat X NORTH as LOOK NORTH rather than
!				X NORTH WALL.
! 2003.12.23	Modified to check for the 6.11 standard library's new direction functionallity, 
!				if everything the look_dir properties step fails. Also removed the WORDSIZE 
!				definition, since new compiler defines this by default.
! 2004.01.11	Modified to use the stdlib 6/11b2 new ext_initialise functionality
! 2004.02.09	Optimized objectloops.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORLookRoom...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORLookRoom forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORLookRoom has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORLookRoom_DONEREPLACE; constant ORLookRoom_DONEREPLACE; #ifdef ORLookRoom_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	system_file;
	!--- just to allocate properties without declaring them as common
	class orlookroomnothing
		with	remote_description
		,		n_look
		,		s_look
		,		e_look
		,		w_look
		,		nw_look
		,		sw_look
		,		ne_look
		,		se_look
		,		u_look
		,		d_look
		,		cant_look
		,		visible_from
		,		describe_as_if_present	
	;
!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORLookRoom_DONEMESSAGE; constant ORLookRoom_DONEMESSAGE;  #ifdef ORLookRoom_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORLookRoom requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORLookRoom_DONECODE;constant ORLookRoom_DONECODE; #ifdef ORLookRoom_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

	object _olr_init LibraryExtensions
		with ext_messages[;
		LookDirection: 
			switch(lm_n){
				1: print "It",(string)IS__TX," too dark to see that way.^";
				2: print "What ",(ig)vrb(SINGULAR_OBJ,"lay","lie")," in that direction ",(ig)isorare(null)," unclear.^";
				3: print (The)lm_o," ",(isorare)lm_o," closed and ",(ig)ICant(actor)," see through it.^";
				4: print (ig)CICant(actor)," see anything in that direction.^[Warning: obsolete message referenced (LookDirecton#4 from ORLookRoom).]^^";	
				5: print (ig)CIVerb(actor,"did","do","does")," not see anything like that.^";
			}
	];

	[DescribeRoomRemotely o SuppressCantSee sva svl; ! Describe a given room (Look without being in the room)
		if(o hasnt light) {
			if(SuppressCantSee==false) L__M(##LookDirection,1);
			rfalse;
		}
		if(o provides remote_description){
			printorrun(o,remote_description);
			rtrue;
		}
		if(o provides describe_as_if_present && valueorrun(describe_as_if_present)==true){
			printorrun(o,description);
			svl=location;
			sva=parent(actor);
			location=o;
			move actor to o;
			MoveFloatingObjects();
			Locale(o); !now describe the contents of the room
			location=svl;
			move actor to sva;
			MoveFloatingObjects();
			rtrue;
		}
		if(SuppressCantSee==false) L__M(##LookDirection,2);
		rfalse;
	];

!======================================================================================
   #endif; #ifnot; #ifndef ORLookRoom_DONEGRAMMAR; constant ORLookRoom_DONEGRAMMAR; #ifdef ORLookRoom_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	[AdjoiningRoom	d r;
		switch(scope_stage)
		{
			1:	rfalse;
			2:	objectloop(d in compass) !for each possible direction 
				{
					if(parent(actor) provides d.door_dir && metaclass(parent(actor).(d.door_dir))==object)
					{
						r=parent(actor).(d.door_dir);
						if(r has door)
						{
							if(r has open or transparent) 
								r=valueorrun(r,door_to);
							else
								r=nothing;
						}
						if(r~=nothing) PlaceInScope(r); 
					}
				}
				!--An Adjoining room may not actually be reachable, just nearby and therefore visible...
				objectloop(r provides visible_from){
					for(d=0:d<(r.#visible_from/WORDSIZE):d++){
						if(r.&visible_from-->d==scopeceiling(actor)) 
							PlaceInScope(r);
					}
				}
				rtrue;
			3: L__M(##LookDirection,5);
		}
	];
	[DetermineRoomDirection rm d r;
		objectloop(d in compass) !for each possible direction 
		{
			if(parent(actor) provides (d.door_dir)) 
			{
				r=valueorrun(parent(actor),(d.door_dir));
				if(r has door)
				{
					if(r has open or transparent) 
						r=valueorrun(r,door_to);
					else
						r=nothing;
				}
				if(r==rm) return d;
			}
		}
		return 0;
	];
	[LookDoorSub o;
		o=ValueORRun(noun,door_to);
		noun=DetermineRoomDirection(o);
		LookDirectionSub();
		rtrue;
	];
	[LookRoomSub o;
		if(parent(actor)~=noun)
		{
			o=DetermineRoomDirection(noun);
			if(o~=0){ !--if a neighboring room, then describe via direction sub (which checks line of sight)
				noun=o;
				LookDirectionSub();
				rtrue;
			} !--otherwise, room must be visible but inaccessible (like a play's stage as seen from a balcony).  Fall through and describe.
		}
		DescribeRoomRemotely(noun);  !describe the specified room
	];

	[LookDirectionSub o prop;
		!---1) If current room provides a distance description, then use that
		prop=-1;
		switch(noun)
		{
			n_obj:prop=n_look;
			s_obj:prop=s_look;
			e_obj:prop=e_look;
			w_obj:prop=w_look;
			nw_obj:prop=nw_look;
			sw_obj:prop=sw_look;
			ne_obj:prop=ne_look;
			se_obj:prop=se_look;
			u_obj:prop=u_look;
			d_obj:prop=d_look;
		}
		if(prop~=-1){
			if(parent(actor) provides prop && parent(actor).prop~=0) {
				printorrun(parent(actor),prop);
				return;
			}
		}
		!---2) use the standard library's compasslook property if it exists...
		if (location provides compasslook && location.compasslook(noun)) rtrue;
		!---3) Verify that the destination of the direction looked is a room and not a string or a routine (we don't want to risk running code that usually runs only when a player moves and may print text)
		if(prop~=-1 && parent(actor) provides (noun.door_dir) && metaclass((parent(actor)).(noun.door_dir))==object) {
			o=(parent(actor)).(noun.door_dir);
			if(o has door){ !---if actually a door then verify that we can see the room behind
				if(o has open or transparent){ !--- open door?  made of glass?
					o=valueorrun(o,door_to);   
				}
				else {
					L__M(##LookDirection,3,o);
					return;
				}
			}
			!--- 3) at this point we are assured a room, so try to describe it
			prop=false;
			if(parent(actor) provides cant_look && parent(actor).cant_look~=0) prop=true; !--we should suppress any default messages if we cannot describe the room because we have an alternate default message supplied
			if(DescribeRoomRemotely(o,prop)==true) return; !Describe the room, (suppressing any errors if we have a cant_look option left...)
			!--failed to describe the room remotely.  
			if(prop==true) printorrun(parent(actor),cant_look); !--If we have a cant_look option availible, then we supressed error messsages show should run it now.
			return;
		}
		!--check the cant_look property
		if(parent(actor) provides cant_look && parent(actor).cant_look~=0) {
			printorrun(parent(actor),cant_look); 
			return;
		}
		!--failing all of the above, use the standard library's version...
		noun.description(); !--this will perform the same check as step 2 above, but if we got this far then it shouldn't make a difference
	];

	[CurrentRoom;
		if(scope_stage~=2)rfalse;
		PlaceInScope(parent(actor));
		rtrue;
	];
	Extend "examine" first
		*	-> Look
		*	noun=ADirection -> LookDirection
	;
	Extend "examine" last
		*	'room'	-> Look
		*	scope=CurrentRoom -> Look
		*	scope=AdjoiningRoom -> LookRoom
	;

	Extend "look" first !--needed to usurp the new compasslook functionality in the standard library
		*	-> Look
		*	noun=ADirection -> LookDirection
		*	'to' noun=ADirection -> LookDirection
	;
	Extend "look" last
		*	'around' -> Look
		*	'at' 'room' -> Look
		*	'outside'/'inside'/'out'/'in'/'through'/'thru' door ->LookDoor
		*	'at' scope=CurrentRoom -> Look
		*	'at' scope=AdjoiningRoom ->LookRoom
	;
	Extend "search" last
		*	scope=CurrentRoom -> Look
		*	'room' -> Look
		*	topic -> SearchNothing
	;
	[SearchNothingSub; L__M(##Miscellany,30);];

#ifdef ORLibDoc;
	!--Commands, document thyself.
	ORMenu ORHelpLookRoom "Looking at Scenery" ORLibDoc 
	  with text[;
			print "The traditional ~Look~ command has been extended to allow a player to look in a direction, at a nearby location, or through a door or window. For example, consider the following:^^";
			font off; 
			spaces(4);print "You are in the Dining Room. To the North is the Laboratory.";
			font on;
			print"^^Any of the following commands are legal:^^";
			font off; 
			spaces(4);print">look north^";
			spaces(4);print">look at laboratory^";
			spaces(4);print">examine laboratory.";
		]
	;
#endif;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================