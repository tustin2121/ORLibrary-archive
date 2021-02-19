!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.03.08 ORNPC_doverb	[Z, GLULX]
! Gives an NPC the ability to perform actions as the player would.
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
! AutoDep :	ORNPCVerb
!--------------------------------------------------------------------------------------
! NPC Componant Priority: 5  
! The lower the number, the further down the "class" line.  The greater the number, 
! the earlier it needs to come on the class line. Modules with the same priority can
! effectively be exchanged in order without effect.
!--------------------------------------------------------------------------------------
! This module brings together the ORNPCVerb module and OREnglish to enable an NPC to 
! perform actions in exactly the same way as the PC. To get an NPC to perform an action,
! the DoVerb routine is exposed which takes the action, noun, and second as parameters:
!
!	troll.DoVerb(##Wave);
!	troll.DoVerb(##Take, Gem);
!	troll.DoVerb(##Insert, Gem, Pocket);
!
! Additionally, the "follow_orders" property can list all verbs that the NPC will obey.
! As an alternative, the "will_follow_all_orders" property can be set to true to get the 
! NPC to attempt to perform all actions except those specified in the "ignore_orders"
! property list.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORNPC_doverb";					!Constant USE_ORNPC_doverb; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORNPC_doverb; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.05	Initial Creation (Not really, but this was the earliest date I could 
!				find without searching my code archive.)
! 2002.02.26	Fit to new template.
! 2002.05.08	2 bugs and fixes, found by Stephen Robert Norris.
!				1) Before rules not being called by NPCs
!				2) Unrecognized orders attempted
! 2002.05.12	Additional code in DoVerb Routine to set and save the inp1 and inp2 
!				variables (again sent in by Stepen Norris.)
! 2002.09.02	Converted attributes previously defined in this module to properties.
! 2004.03.08	Added check to ensure OREnglish is being included.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORNPC_doverb...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORNPC_doverb forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORNPC_doverb has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORNPCVerb_DONEREPLACE; default USE_ORNPCVerb 0; message "          [ORNPC_doverb forcing inclusion of ORNPCVerb]"; #include "ORNPCVerb"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORNPC_doverb_DONEREPLACE; constant ORNPC_doverb_DONEREPLACE; #ifdef ORNPC_doverb_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORNPC_doverb_DONEMESSAGE; constant ORNPC_doverb_DONEMESSAGE;  #ifdef ORNPC_doverb_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
	#ifndef OREnglish; message fatalerror "ERROR!!!! ORNPC_doverb requires the OREnglish file.";#endif; 
	class ORNPC_doverb
		with will_follow_all_orders false
		,	ignore_orders 0
		,	follow_orders 0
		,	DoVerb[act n s svAL svActr svActn svN svS svInp1 svInp2 retval;
				!---1) save current information
				svAL=actors_location;
				svActr=actor;
				svActn=action;
				svN=noun;
				svS=second;
				svInp1 = inp1;
				svInp2 = inp2;
				!---2) specify new values
				actors_location=parent(self);
				actor=self;
				action=act;
				noun=n;
				second=s;
				inp1 = noun;
				inp2 = second;
				MoveFloatingObjects();
				!---3) perform action (assuming Before doesn't interupt it)
				retval=BeforeRoutines();
				if(retval==false) retval=ActionPrimitive();
				!---4) restore saved information
				actors_location=svAL;
				actor=svActr;
				action=svActn;
				noun=svN;
				second=svS;
				inp1 = svInp1;
				inp2 = svInp2;
				MoveFloatingObjects();
				return retval;
			]
		,   orders [;
				if(action==##NotUnderstood) rfalse;
				if(self==player && valueorrun(self,ownwill)==false) rfalse;
				if(self.will_follow_all_orders==true || WordInProperty(action,self,follow_orders)==true){
					if(WordInProperty(action,self,ignore_orders)==true) rfalse;
					return self.doVerb(action, noun, second);
				}
				rfalse;
			] 
		,	ownwill false
	;
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNPC_doverb_DONECODE;constant ORNPC_doverb_DONECODE; #ifdef ORNPC_doverb_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #ifnot; #ifndef ORNPC_doverb_DONEGRAMMAR; constant ORNPC_doverb_DONEGRAMMAR; #ifdef ORNPC_doverb_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================