!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORRoutinesList	[Z, GLULX]
! An object that can maintain a list of routines and run them in order like
! additive properties.
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
! ORLibDev: Note, that this module is not useful as a library extension alone.  By 
!     itself it provides no new functionality for the IF developer. Instead it provides
!     a basis for other module to be built upon and will primarily be useful to those 
!     who are developing ORLibrary entrys of their own.
!--------------------------------------------------------------------------------------
! Created primarily as a technique for avoiding conflicts when replacing routines that 
! may want to be replaced by other modules.  The ORRoutineList class exposes methods
! for adding and removing routines and calling them in different ways. For instance, 
! run_accumulate() will call all registered routines and add up their results.  
! run_until() will do the same thing but stop when the specified value is 
! returned by one of the registered routines.  This is similar by design to additive 
! properties.  Similarly, the run_until_not() routine will run the routines until one 
! of them does *not* return the specified value.
!--------------------------------------------------------------------------------------
! To add this module to your library, add the line:
!
!		#include "ORRoutinesList";					!Constant USE_ORRoutinesList; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORRoutinesList; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2001.12.10	Initial Creation
! 2002.02.07	Added Glulx support
! 2002.02.26	Fit to new template.
! 2002.05.08	Added run_until_not routine.
! 2002.05.14	Added an entry point for a routine to be defined that will be called 
!				every cycle int the routine execution list.
! 2003.02.06	Eliminated the default number of blank entries of the routine property.
!				This can be set to a better value in a derived class or instance of this 
!				class, but it is being reducedfrom this base class to keep from allocating 
!				readable memory needlessly.
! 2003.03.14	Added code to pass parameters to between_calls routine. 
! 2003.04.16	Removed useless call to between_calls from the remove routine method.
!				Also modified code which called to between_calls, because it didn't seem to 
!				function correctly.  Cause: Some strange overloading problem, where base class
!				couldn't correctly resolve overloaded definition.  Very strange, but worked around now.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORRoutinesList...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORRoutinesList forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORRoutinesList has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORRoutinesList_DONEREPLACE; constant ORRoutinesList_DONEREPLACE; #ifdef ORRoutinesList_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	class ORRoutinesList
		with	routines 0
		,	num_routines 0
		,	count_routines[t retval;
				for(t=0:t<self.num_routines:t++){
					if((self.&routines-->t)~=0) retval++;
				}
				return retval;
			]
		,	add_routine[rout;
				if(self.num_routines>=(self.#routines)/WORDSIZE) {
#ifdef DEBUG;
					print "[ERR: out of room in property ~routines~ in object ",(name)self," ]";
#endif;
					return;
				}
				(self.&routines-->(self.num_routines))=rout;
				self.num_routines++;
			]
		,	remove_routine[rout t;
				for(t=0:t<self.num_routines:t++){
					if((self.&routines-->t)==rout)
						(self.&routines-->t)=0;
				}
			]
		,	run_accumulate[one two three four five six retval rv t;
				retval=0;
				for(t=0:t<self.num_routines:t++){
					if((self.&routines-->t)~=0) rv=((self.&routines-->t)(one,two, three, four, five, six));
					retval=retval+rv;
					self.between_calls(rv, one, two, three, four);
				}
				return retval;
			]
		,	run_until[untilval one two three four five six retval t;
				retval=0;
				for(t=0:t<self.num_routines:t++){
					if((self.&routines-->t)~=0)
						retval=((self.&routines-->t)(one, two,three, four, five, six));
					if(retval==untilval) return retval;
					self.between_calls(retval, one, two, three, four);
				}
				return retval;
			]
		,	run_until_not[untilnotval one two three four five six retval t;
				retval=0;
				for(t=0:t<self.num_routines:t++){
					if((self.&routines-->t)~=0)
						retval=((self.&routines-->t)(one, two,three, four, five, six));
					if(retval~=untilnotval) return retval;
					self.between_calls(retval, one, two, three, four);
				}
				return retval;
			]
		,	between_calls[;return;]
	;
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORRoutinesList_DONEMESSAGE; constant ORRoutinesList_DONEMESSAGE;  #ifdef ORRoutinesList_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORRoutinesList requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORRoutinesList_DONECODE;constant ORRoutinesList_DONECODE; #ifdef ORRoutinesList_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #ifnot; #ifndef ORRoutinesList_DONEGRAMMAR; constant ORRoutinesList_DONEGRAMMAR; #ifdef ORRoutinesList_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================