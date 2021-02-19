!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORCantGoOdd [Z, GLUILX]
! A modification to the library's cant_go behavior to distinguish between "normal"
! directions and "odd" directions (such as up, down, in, and out).
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
! CantGoOdd is a modification to the regular GoSub routine which causes it to ignore the
! cant_go property if the direction is up, down, in, or out. This seems to be one of 
! the more common oddities in Inform adventures. For example...
!
!	You are in a small forest, surrounded by trees.  Only to the north 
!	is there a passage.
!	
!	>s
!	Trees block travel in that direction.
!	
!	>in
!	Trees block travel in that direction.
!
!	>down
!	Trees block travel in that direction.
!
!	>up
!	Trees block travel in that direction.
!
! Alternatively, a new property has been defined "cant_go_odd" which will be used 
! instead with these directions.  A default library message has also been created
! for generic cant_go responses in these odd directions.  (##Go,13) 
!
!--------------------------------------------------------------------------------------
! AutoDep:	
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORCantGoOdd; #include "ORCantGoOdd"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORCantGoOdd; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2003.02.08	Initial Creation
! 2003.02.28	Added code to not depend upon ORNPC_DoVerb;
!--------------------------------------------------------------------------------------
message "          Processing library extension ORCantGoOdd...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORCantGoOdd forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORCantGoOdd has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORCantGoOdd_DONEREPLACE; constant ORCantGoOdd_DONEREPLACE; #ifdef ORCantGoOdd_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	Replace GoSub;
!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORCantGoOdd_DONEMESSAGE; constant ORCantGoOdd_DONEMESSAGE;  #ifdef ORCantGoOdd_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
! #ifndef OREnglish; message fatalerror "ERROR!!!! ORCantGoOdd requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORCantGoOdd_DONECODE;constant ORCantGoOdd_DONECODE; #ifdef ORCantGoOdd_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	object _cgoinit LibraryExtensions
		with ext_messages[;
				Go: if(lm_n==13) print_ret "It",(string)IS__TX," not possible to go that way.";
			]
		,	cant_go_odd
	;
	[ GoSub i j k df movewith thedir old_loc x;
		if (second ~= 0 && second notin Compass
			&& ObjectIsUntouchable(second)) return;
		old_loc = actors_location; !save where we are
		movewith=0;
		i=parent(actor); !get where we are 

		if (actors_location~=i or thedark)
		{   j=actors_location;
			k=RunRoutines(i,before); if (k~=3) actors_location=j;
			if (k==1)
			{   movewith=i; i=parent(i);
			}
			else
			{   if (k==0) L__M(##Go,1,i);
				rtrue;
			}
		}
		thedir=noun.door_dir;

		if (ZRegion(thedir)==2) thedir=RunRoutines(noun,door_dir);

		j=i.thedir; k=ZRegion(j);

		if (k==3) { if(actor==player)print (string) j; new_line; rfalse; }
		if (k==2) { j=RunRoutines(i,thedir);
					if (j==1) rtrue;
					}

		if (k==0 || j==0)
		{ 
#ifdef CanPlayerWitness;
			x=CanPlayerWitness();
#ifnot;
			x=true;
#endif;
			if(x){
				if(noun==u_obj or d_obj or in_obj or out_obj){ !--if one of the odd directions
					 if(actor==player && i provides cant_go_odd){
						if(i.cant_go_odd ~= 0) PrintOrRun(i, cant_go_odd);
					}else return L__M(##Go,13,thedir);
				}else{
					if(actor==player){
						if(i.cant_go ~= 0) PrintOrRun(i, cant_go);
					}else return L__M(##Go,10,thedir);
				}
			}		
			rfalse;
		}

		if (j has door)
		{   if (j has concealed) return L__M(##Go,2);
			if (j hasnt open)
			{   if (noun==u_obj) return L__M(##Go,3,j);
				if (noun==d_obj) return L__M(##Go,4,j);
				return L__M(##Go,5,j);
			}
			k=RunRoutines(j,door_to);
			if (k==0) return L__M(##Go,6,j);
			if (k==1) rtrue;
			j = k;
		}
		if(actor~=player && ScopeCeiling(actor)==ScopeCeiling(player)) L__M(##Go,12,thedir);
		if (movewith==0) move actor to j; else move movewith to j;
		if(actor~=player && ScopeCeiling(actor)==ScopeCeiling(player)) L__M(##Go,11,thedir);
		actors_location=j; 
		MoveFloatingObjects();
		if(actor==player){ 
			df=OffersLight(j);
			if (df~=0) { location=j; real_location=j; lightflag=1; }
			else
			{   if (old_loc == thedark)
				{   DarkToDark();
					if (deadflag~=0) rtrue;
				}
				real_location=j;
				location=thedark; lightflag=0;
			}
			if (AfterRoutines()==1) rtrue;
			if (keep_silent==1) rtrue;
			LookSub(1);
		}
	];
!======================================================================================
   #endif; #ifnot; #ifndef ORCantGoOdd_DONEGRAMMAR; constant ORCantGoOdd_DONEGRAMMAR; #ifdef ORCantGoOdd_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================