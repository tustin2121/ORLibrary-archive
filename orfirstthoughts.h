!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORFirstThoughts	[Z, GLULX]
! Often it is desirable, when the player is first examining an object, to describe 
! first impressions that are not repeated afterwards.
! This is a simple base class that allows for a complex first description.  
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
! A more detailed description of this file, what it does, and how to use it goes here.
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORFirstThoughts";					!Constant USE_ORFirstThoughts; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORFirstThoughts; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.08.13	Initial Creation
! 2002.02.26	Applied to new template.
! 2002.05.12	Added support for generic message when no description properties are 
!				defined. (sent in by Stephen Robert Norris)
! 2002.06.16	Added the synonyms "rex" and "rel" for "reexamine" and "relook"
! 2002.08.15	Added Self documenting code.
! 2002.10.19	Added autodep for ObjectInitialise 
! 2003.02.16	Added code to abort a description if one of the calls moves the character.
! 2003.02.19	Added better ifdef for self docs
! 2003.04.19	Fixed code which suppressed look from within an object.
! 2003.07.01	Made the REX and REL commands META.
! 2004.01.11	Modified to use the stdlib 6/11b2 new ext_initialise functionality
!--------------------------------------------------------------------------------------
message "          Processing library extension ORFirstThoughts...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORFirstThoughts forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORFirstThoughts has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORFirstThoughts_DONEREPLACE; constant ORFirstThoughts_DONEREPLACE; #ifdef ORFirstThoughts_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	global SuppressRoomDescriptionNewLine;
!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORFirstThoughts_DONEMESSAGE; constant ORFirstThoughts_DONEMESSAGE;  #ifdef ORFirstThoughts_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORFirstThoughts requires the OREnglish file.";#endif; 
system_file;
	class ORFirstThoughts
		with beentold false
		,	description[printed;
				if(self.firsttime && self.beentold==false){
					PrintOrRun(self,firsttime,true);
					printed = true;
				}
				if(TestScope(self)==false && ScopeCeiling(actor)~=self) return; 
				if(self.descrip){
					PrintOrRun(self,descrip,true);
					printed = true;
				}
				if(TestScope(self)==false && ~~actor in self) return;
				if(self.firsttimepost && self.beentold==false){
					PrintOrRun(self,firsttimepost,true);
					printed = true;
				}
				self.beentold=true;
				if (action == ##Examine && printed == false){
					L__M(##Examine, 2, self);
					rtrue;
				}
				if(TestScope(self)==false && ~~actor in self) return;
				if(SuppressRoomDescriptionNewLine==false) new_line;
			]
		,	firsttime 0
		,	firsttimepost 0
		,	descrip		0
	;
!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORFirstThoughts_DONECODE;constant ORFirstThoughts_DONECODE; #ifdef ORFirstThoughts_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
   #endif; #ifnot; #ifndef ORFirstThoughts_DONEGRAMMAR; constant ORFirstThoughts_DONEGRAMMAR; #ifdef ORFirstThoughts_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
#ifdef ORLibDoc;
	!--Commands, document thyselves.
		ORMenu ORHelpFirstThought "REX and REL" ORLibDoc with text "In several places, the protagonist's thoughts will be intermingled with the first description on an object.  For example, looking at a golf ball might cause the player character to reflect upon his experience at golfing.  By convention, these ~first thoughts~ are only shown during the initial examination, however a player wishing to reread this initial text can access it with the REEXAMINE (REX) and RELOOK (REL) verbs.";
#endif;

	[ReExamineSub;
		if(noun provides beentold) noun.beentold=false;
		<<examine noun>>;
	];
	[RelookSub;
		if(parent(actor) provides beentold) parent(actor).beentold=false;
		<<look>>;
	];

	Verb meta "reexamine" "rex"
		*	'room'	-> Relook
		*	noun	-> ReExamine
	; 
	Verb meta "relook" "rel"
		*	-> Relook
		*	'around' -> Relook
		*	'at' noun ->ReExamine
	;
	Extend "examine" last
		*	'room' 'again'	-> Relook
		*	noun 'again'	-> ReExamine
		*	'again'	noun	-> ReExamine
	;
	Extend "look" last
		*	'around' 'again'	-> Relook
		*	'at' noun 'again'	->ReExamine
		*	'again' 'at' noun	->ReExamine
	;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================