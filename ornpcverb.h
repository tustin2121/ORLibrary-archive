!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2007.08.03 ORNPCVerb  [Z, GLULX]
! ORNPCVerb enables NPCs to act and react in much the same way as the PC.
! Much of this module is a replacment for many library verbs.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- - 
! License: 
! Matches that of Inform's standard library.
!--------------------------------------------------------------------------------------
! AutoDep :	ORReverseDirection
!--------------------------------------------------------------------------------------
! A more detailed description of this file, what it does, and how to use it goes here.
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORNPCVerb";					!Constant USE_ORNPCVerb; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORNPCVerb; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.17	Initial Creation (not really, but this was the earliest date I could 
!				find without searching my code archive)
! 2002.02.26	Fit to new template.
! 2002.05.12	Added versions of the BeforeRoutines() and AfterRoutines() routines.
! 2002.05.22	Fixed a problem with a nested call to the Orders routine from the 
!				BeforeRoutines routine.
! 2002.08.18	Wrapped all definitions in #ifndef pragmas so that they can be redifined
!				elsewhere.
! 2002.08.30	Restored Darkness test to GoSub.  Still trying to remember why it was 
!				removed at all...
! 2002.09.17	Added missed #ifndef pragma around ObjectIsUntouchable
! 2002.10.13	Added missed #ifndef pragma around PutOnSub and InsertSub
! 2002.12.24	Found bug in BeforeRoutines and AfterRoutines that caused the before/after 
!				method to be called on an object that contains an actor rather than the 
!				method attached to the top level room.
! 2003.02.28	Fixed bad representation of NPC wanderings up and down (e.g.: "Dan entered 
!				from the up.")
! 2003.03.01	Added code to the print message routine to prefix a newline the actor is 
!				not the player. Also made a minor modification to the TopLocation routine
!				to assume the actor if not parameter is provided.
! 2003.03.04	Fixed a little bug in the modification made 2/28/2003 which referenced
!				directions objects instead of direction properties (d_obj instead of d_to).
! 2003.03.11	Fixed some missing return values in the ORLibMessage object.
! 2003.03.13	Removed the diagnostic text that I put in and forgot to remove.
! 2004.01.11	Modified to match new stdlib 6.11
! 2004.02.05	Additional changes to better reflect stdlib enhancments.
! 2004.02.09	Even more additional changes to better reflect stdlib enhancments.
! 2004.02.27	Removed autodep code for ORRoutinesList, since it is no longer a requisite.
! 2004.03.03	Removed references to the ExtensionRunRoutines... routines.
! 2007.08.03    Howard Sherman added responses player should see when NPC wanders in or out
!               of a room (e.g. Billy wanders in from the out..." and so forth.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORNPCVerb...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORNPCVerb forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORNPCVerb has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORReverseDirection_DONEREPLACE; default USE_ORReverseDirection 0; message "          [ORNPCVerb forcing inclusion of ORReverseDirection]"; #include "ORReverseDirection"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORNPCVerb_DONEREPLACE; constant ORNPCVerb_DONEREPLACE; #ifdef ORNPCVerb_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace PlayerTo;
	replace GetOffSub;
	replace ExitSub;
	replace GoSub;
	replace SayWhatsOn;
	replace Locale;
	replace EatSub;
	replace TakeSub;
	replace ObjectIsUntouchable;
	replace AttemptToTakeObject;
	replace FindVisibilityLevels;
	replace LookSub;
	replace DropSub;
	replace PutOnSub;
	replace InsertSub;
	replace TransferSub;
	replace GiveSub;
	replace ShowSub;
	replace EnterSub;
	replace OpenSub;
	replace WearSub;
	replace TouchSub;
	replace WaveSub;
	replace AllowPushDir;
	replace ThrowAtSub;
	replace KissSub;
	replace TellSub;
	replace AskForSub;
	replace L___M;
	replace InvSub;
	Replace BeforeRoutines;
	Replace AfterRoutines;
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORNPCVerb_DONEMESSAGE; constant ORNPCVerb_DONEMESSAGE;  #ifdef ORNPCVerb_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORNPCVerb requires the OREnglish file.";#endif; 
	default ORSuppressRoomDescriptionNewLine=false;
	global ORSuppressLibraryMessages=false;
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNPCVerb_DONECODE;constant ORNPCVerb_DONECODE; #ifdef ORNPCVerb_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #ifnot; #ifndef ORNPCVerb_DONEGRAMMAR; constant ORNPCVerb_DONEGRAMMAR; #ifdef ORNPCVerb_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
[TopLocation act;
	if(act==0) act=actor;
	while (parent(act)~=0) act=parent(act);
	return act;
];
#ifndef CanPlayerWitness;
	[CanPlayerWitness; return true;];
#endif;
#ifndef BeforeRoutines;
	[ BeforeRoutines;
		if (GamePreRoutine()~=0) rtrue;
		if (actor==player && RunRoutines(player,orders)~=0) rtrue; !--go ahead and let any orders routines for the player work as normal.  Don't do this if the actor is not the player, however, because it will generally mean a recursive loop (if the player told the npc what to do) or a pointless call (if the npc decided to do it on its own)
		if (TopLocation(actor)~=0 && RunRoutines(TopLocation(actor),before)~=0) rtrue;
		scope_reason=REACT_BEFORE_REASON; parser_one=0;
		SearchScope(ScopeCeiling(actor),actor,0); scope_reason=PARSING_REASON;
		if (parser_one~=0) rtrue;
		if (inp1>1 && RunRoutines(inp1,before)~=0) rtrue;
		rfalse;
	];
#endif;
#ifndef AfterRoutines;
	[ AfterRoutines;
		scope_reason=REACT_AFTER_REASON; parser_one=0;
		SearchScope(ScopeCeiling(actor),actor,0); scope_reason=PARSING_REASON;
		if (parser_one~=0) rtrue;
		if (TopLocation(actor)~=0 && RunRoutines(TopLocation(actor),after)~=0) rtrue;
		if (inp1>1 && RunRoutines(inp1,after)~=0) rtrue;
		return GamePostRoutine();
	];
#endif;
#ifndef GetOffSub;
	[ GetOffSub;
		if (parent(actor)==noun) <<Exit>>;
		L__M(##GetOff,1,noun);
	];
#endif;
#ifndef ExitSub;
	[ ExitSub p;
		p=parent(actor);
		if (p==location || (location==thedark && p==real_location))
		{   if ((location.out_to~=0)
				|| (location==thedark && real_location.out_to~=0)) <<Go out_obj>>;
			return L__M(##Exit,1);
		}
		if (p has container && p hasnt open)
			return L__M(##Exit,2,p);

		move actor to parent(p);

		if (AfterRoutines()==1) rtrue;
		if (keep_silent==1) rtrue;
		L__M(##Exit,3,p); LookSub(1);
	];
#endif;
#ifndef SayWhatsOn;
	[ SayWhatsOn descon j f;
		if (descon==parent(actor)) rfalse;
		objectloop (j in descon)
			if (j hasnt concealed && j hasnt scenery) f=1;
		if (f==0) rfalse;
		L__M(##Look, 4, descon); rtrue;
	];
#endif;
#ifndef NotSupportingTheActor;
	[ NotSupportingTheActor o i;
		i=parent(actor);
		while (i~=0 && i~=visibility_ceiling)
		{   if (i==o) rfalse;
			i = parent(i);
			if (i~=0 && i hasnt supporter) rtrue;
		}
		rtrue;
	];
#endif;
#ifndef Locale;
	[ Locale descin text1 text2 o k p j f2 flag;
		objectloop (o in descin) give o ~workflag;
		k=0;
		objectloop (o in descin)
			if (o hasnt concealed && NotSupportingTheActor(o))
			{  #IFNDEF MANUAL_PRONOUNS;
				PronounNotice(o);
				#ENDIF;
				if (o hasnt scenery)
				{   give o workflag; k++;
					p=initial; f2=0;
					if ((o has door || o has container)
						&& o has open && o provides when_open)
					{   p = when_open; f2 = 1; jump Prop_Chosen; }
					if ((o has door || o has container)
						&& o hasnt open && o provides when_closed)
					{   p = when_closed; f2 = 1; jump Prop_Chosen; }
					if (o has switchable
						&& o has on && o provides when_on)
					{   p = when_on; f2 = 1; jump Prop_Chosen; }
					if (o has switchable
						&& o hasnt on && o provides when_off)
					{   p = when_off; f2 = 1; }

					.Prop_Chosen;

					if (o hasnt moved || o.describe~=NULL || f2==1)
					{   if (o.describe~=NULL && RunRoutines(o,describe)~=0)
						{   flag=1;
							give o ~workflag; k--;
						}    
						else
						{   j=o.p;
							if (j~=0)
							{   new_line;
								PrintOrRun(o,p);
								flag=1;
								give o ~workflag; k--;
								if (o has supporter && child(o)~=0) SayWhatsOn(o);
							}
						}
					}
				}
				else
					if (o has supporter && child(o)~=0) SayWhatsOn(o);
			}
		if (k==0) return 0;
		if (text1~=0)
		{   new_line;
			if (flag==1) text1=text2;
			print (string) text1, " ";
			WriteListFrom(child(descin),
				ENGLISH_BIT + WORKFLAG_BIT + RECURSE_BIT
				+ PARTINV_BIT + TERSE_BIT + CONCEAL_BIT);
			return k;
		}
		
		if (flag==1) L__M(##Look,5,descin); else L__M(##Look,6,descin);
		
	];
#endif;
#ifndef EatSub;
	[ EatSub;
		if (ObjectIsUntouchable(noun)) return;
		if (noun hasnt edible) return L__M(##Eat,1,noun);
		if (noun has worn)
		{   L__M(##Drop,3,noun);
			<Disrobe noun>;
			if (noun has worn && noun in actor) rtrue;
		}
		remove noun;
		if (AfterRoutines()==1) rtrue;
		if (keep_silent==1) rtrue;
		L__M(##Eat,2,noun);
	];
#endif;
#ifndef TakeSub;
	[ TakeSub;
		if(onotheld_mode==0 || noun notin actor){
			if (AttemptToTakeObject(noun)==true) rtrue;
		}
		if (AfterRoutines()==1) rtrue;
		notheld_mode=onotheld_mode;
		if (notheld_mode==1 || keep_silent==1) rtrue;
		L__M(##Take,1);
	];
#endif;
#ifndef ObjectIsUntouchable;
	[ ObjectIsUntouchable item flag1 flag2 ancestor i;
		! Determine if there's any barrier preventing the player from moving
		! things to "item".  Return false if no barrier; otherwise print a
		! suitable message and return true.
		! If flag1 is set, do not print any message.
		! If flag2 is set, also apply Take/Remove restrictions.

		! If the item has been added to scope by something, it's first necessary
		! for that something to be touchable.

		ancestor = CommonAncestor(actor, item);
		if (ancestor == 0) {
			ancestor = item;
			while (ancestor && (i = ObjectScopedBySomething(ancestor)) == 0)
				ancestor = parent(ancestor);
			if (i ~= 0) {
				if (ObjectIsUntouchable(i, flag1, flag2)) return;
				! An item immediately added to scope
			}
		}
		else

		! First, a barrier between the actor and the ancestor.  The actor
		! can only be in a sequence of enterable objects, and only closed
		! containers form a barrier.
		if (actor ~= ancestor)
		{   i = parent(actor);
			while (i~=ancestor)
			{   if (i has container && i hasnt open)
				{   if (flag1) rtrue;
					return L__M(##Take,9,i);
				}
				i = parent(i);
			}
		}

		! Second, a barrier between the item and the ancestor.  The item can
		! be carried by someone, part of a piece of machinery, in or on top
		! of something and so on.

		if (item ~= ancestor)
		{   i = parent(item);
			while (i~=ancestor)
			{   if (flag2 && i hasnt container && i hasnt supporter)
				{   if (i has animate)
					{   if (flag1) rtrue;
						return L__M(##Take,6,i);
					}
					if (i has transparent)
					{   if (flag1) rtrue;
						return L__M(##Take,7,i);
					}
					if (flag1) rtrue;
					return L__M(##Take,8,item);
				}
				if (i has container && i hasnt open)
				{   if (flag1) rtrue;
					return L__M(##Take,9,i);
				}
				i = parent(i);
			}
		}
		rfalse;
	];
#endif;
#ifndef AttemptToTakeObject;
	[ AttemptToTakeObject item     ancestor after_recipient i j k;
		! Try to transfer the given item to the actor: return false
		! if successful, true if unsuccessful, printing a suitable message
		! in the latter case.
		! People cannot ordinarily be taken.
		if (item == actor) return L__M(##Take,2);
		if (item has animate) return L__M(##Take,3,item);

		ancestor = CommonAncestor(actor, item);

		if (ancestor == 0)
		{   i = ObjectScopedBySomething(item);
			if (i ~= 0) ancestor = CommonAncestor(actor, i);
		}

		! Is the actor indirectly inside the item?
		if (ancestor == item) return L__M(##Take,4,item);

		! Does the actor already directly contain the item?
		if (item in actor) return L__M(##Take,5,item);

		! Can the actor touch the item, or is there (e.g.) a closed container
		! in the way?
		if (ObjectIsUntouchable(item,false,true)) return;

		! The item is now known to be accessible.

		! Consult the immediate possessor of the item, if it's in a container
		! which the actor is not in.

		i=parent(item);
		if (i ~= ancestor && (i has container || i has supporter))
		{   after_recipient=i;
			k=action; action=##LetGo;
			if (RunRoutines(i,before)~=0) { action=k; rtrue; }
			action=k;
		}

		if (item has scenery) return L__M(##Take,10,item);
		if (item has static)  return L__M(##Take,11,item);

		! The item is now known to be available for taking.  Is the actor
		! carrying too much?  If so, possibly juggle items into the rucksack
		! to make room.

		k=0; objectloop (j in actor) if (j hasnt worn) k++;

		if (k >= ValueOrRun(actor,capacity))
		{   if (SACK_OBJECT~=0)
			{   if (parent(SACK_OBJECT)~=actor)
					return L__M(##Take,12);
				j=0;
				objectloop (k in actor) 
					if (k~=SACK_OBJECT && k hasnt worn && k hasnt light) j=k;

				if (j~=0)
				{   L__M(##Take,13,j);
					keep_silent = 1; <Insert j SACK_OBJECT>; keep_silent = 0;
					if (j notin SACK_OBJECT) rtrue;
				}
				else return L__M(##Take,12);
			}
			else return L__M(##Take,12);
		}

		! Transfer the item.

		move item to actor;

		! Send "after" message to the object letting go of the item, if any.

		if (after_recipient~=0)
		{   k=action; action=##LetGo;
			if (RunRoutines(after_recipient,after)~=0) { action=k; rtrue; }
			action=k;
		}
		rfalse;
	];
#endif;
#ifndef FindVisibilityLevels;
	[ FindVisibilityLevels visibility_levels;
		visibility_levels = 1;
		visibility_ceiling = parent(actor);
		while ((parent(visibility_ceiling) ~= 0)
				&& (visibility_ceiling hasnt container
					|| visibility_ceiling has open
					|| visibility_ceiling has transparent))
		{   visibility_ceiling = parent(visibility_ceiling);
			visibility_levels++;
		}      
		return visibility_levels;
	];
#endif;
#ifndef LookSub;
	[ LookSub allow_abbrev  visibility_levels i j k;
			if(player~=actor){
				if (AfterRoutines()==1) rtrue;
				rfalse;
			}

		if (parent(actor)==0) return RunTimeError(10);
		.MovedByInitial;
		if (location == thedark) { visibility_ceiling = thedark; NoteArrival(); }
		else
		{   visibility_levels = FindVisibilityLevels();
			if (visibility_ceiling == location)
			{   NoteArrival();
				if (visibility_ceiling ~= location) jump MovedByInitial;
			}
		}
		!   Printing the top line: e.g.
		!   Octagonal Room (on the table) (as Frodo)

		new_line;
		#ifdef TARGET_GLULX;
			glk($0086, 4); #ifnot;  style bold; 
		#endif; 
		if (visibility_levels == 0) print (name) thedark;
		else
		{   if (visibility_ceiling ~= location) print (The) visibility_ceiling;
			else print (name) visibility_ceiling;
		}
		#ifdef TARGET_GLULX;
			glk($0086, 0); #ifnot;  style roman; 
		#endif; 


		for (j=1, i=parent(actor):j<visibility_levels:j++, i=parent(i))
			if (i has supporter) L__M(##Look,1,i);
							else L__M(##Look,2,i);

		if (print_player_flag==1) L__M(##Look,3,actor);
		new_line;
		!   The room description (if visible)

		if (lookmode<3 && visibility_ceiling==location)
		{   if ((allow_abbrev~=1) || (lookmode==2) || (location hasnt visited))
			{   if (location.describe~=NULL) RunRoutines(location,describe);
				else
				{   if (location.description==0) RunTimeError(11,location);
					else PrintOrRun(location,description,ORSuppressRoomDescriptionNewLine);
				}
			}
		}

		if (visibility_levels == 0) Locale(thedark);
		else
		{   for (i=actor, j=visibility_levels: j>0: j--, i=parent(i))
				give i workflag;
		      
			for (j=visibility_levels: j>0: j--)
			{   for (i=actor, k=0: k<j: k++) i=parent(i);
				if (i.inside_description~=0)
				{   new_line; PrintOrRun(i,inside_description,ORSuppressRoomDescriptionNewLine); }
				Locale(i);
			}
		}

		LookRoutine();
		ScoreArrival();

		action=##Look;
		if (AfterRoutines()==1) rtrue;
	];
#endif;
#ifndef DropSub;
	[ DropSub;
		if (noun == actor) return L__M(##PutOn, 4);
		if (noun in parent(actor)) return L__M(##Drop,1,noun);
		if (noun notin actor) return L__M(##Drop,2,noun);
		if (noun has worn)
		{   L__M(##Drop,3,noun);
			<Disrobe noun>;
			if (noun has worn && noun in actor) rtrue;
		}
		move noun to parent(actor);
		if (AfterRoutines()==1) rtrue;
		if (keep_silent==1) rtrue;
		return L__M(##Drop,4,noun);
		];
#endif;
#ifndef PutOnSub; 
	[ PutOnSub ancestor;
		receive_action=##PutOn; 
		if (second == d_obj || actor in second) <<Drop noun>>;
		if (parent(noun)~=actor) return L__M(##PutOn,1,noun);

		ancestor = CommonAncestor(noun, second);
		if (ancestor == noun) return L__M(##PutOn,2,noun);
		if (ObjectIsUntouchable(second)) return;

		if (second ~= ancestor)
		{   action=##Receive;
			if (RunRoutines(second,before)~=0) { action=##PutOn; return; }
			action=##PutOn;
		}
		if (second hasnt supporter) return L__M(##PutOn,3,second);
		if (ancestor == actor) return L__M(##PutOn,4);
		if (noun has worn)
		{   L__M(##PutOn,5,noun); <Disrobe noun>; if (noun has worn) return;
		}

		if (children(second)>=ValueOrRun(second,capacity))
			return L__M(##PutOn,6,second);

		move noun to second;

		if (AfterRoutines()==1) return;

		if (second ~= ancestor)
		{   action=##Receive;
			if (RunRoutines(second,after)~=0) { action=##PutOn; return; }
			action=##PutOn;
		}
		if (keep_silent==1) return;
		if (multiflag==1) return L__M(##PutOn,7);
		L__M(##PutOn,8,noun);
		];
#endif;
#ifndef InsertSub;
	[ InsertSub ancestor;
		receive_action = ##Insert;
		if (second==d_obj || actor in second) <<Drop noun>>;
		if (parent(noun)~=actor) return L__M(##Insert,1,noun);

		ancestor = CommonAncestor(noun, second);
		if (ancestor == noun) return L__M(##Insert, 5, noun);
		if (ObjectIsUntouchable(second)) return;

		if (second ~= ancestor)
		{   action=##Receive;
			if (RunRoutines(second,before)~=0) { action=##Insert; rtrue; }
			action=##Insert;
			if (second has container && second hasnt open)
				return L__M(##Insert,3,second);
		}
		if (second hasnt container) return L__M(##Insert,2,second);

		if (noun has worn)
		{   L__M(##Insert,6,noun); <Disrobe noun>; if (noun has worn) return;
		}

		if (children(second) >= ValueOrRun(second,capacity))
			return L__M(##Insert,7,second);

		move noun to second;

		if (AfterRoutines()==1) rtrue;

		if (second ~= ancestor)
		{   action=##Receive;
			if (RunRoutines(second,after)~=0) { action=##Insert; rtrue; }
			action=##Insert;
		}
		if (keep_silent==1) rtrue;
		if (multiflag==1) return L__M(##Insert,8,noun);
		L__M(##Insert,9,noun);
	];
#endif;
#ifndef TransferSub;
	[ TransferSub;
		if (noun notin actor && AttemptToTakeObject(noun)) return;
		if (second has supporter) <<PutOn noun second>>;
		if (second == d_obj) <<Drop noun>>;
		<<Insert noun second>>;
	];
#endif;
#ifndef GiveSub;
	[ GiveSub;
		if (parent(noun)~=actor) return L__M(##Give,1,noun);
		if (second==actor)  return L__M(##Give,2,noun);
		if (RunLife(second,##Give)~=0) rfalse;
		L__M(##Give,3,second);
	];
#endif;
#ifndef ShowSub;
	[ ShowSub;
		if (parent(noun)~=actor) return L__M(##Show,1,noun);
		if (second==actor) <<Examine noun>>;
		if (RunLife(second,##Show)~=0) rfalse;
		L__M(##Show,2,second);
	];
#endif;
#ifndef EnterSub;
	[ EnterSub ancestor j k;
		if (noun has door || noun in compass) <<Go noun>>;

		if (actor in noun) return L__M(##Enter,1,noun);
		if (noun hasnt enterable) return L__M(##Enter,2,noun);
		if (noun has container && noun hasnt open) return L__M(##Enter,3,noun);

		if (parent(actor) ~= parent(noun))
		{   ancestor = CommonAncestor(actor, noun);
			if (ancestor == actor or 0) return L__M(##Enter,4,noun);
			while (actor notin ancestor)
			{   j = parent(actor);
				k = keep_silent; 
				if (parent(j) ~= ancestor || noun ~= ancestor)
				{   L__M(##Enter,6,j);
					keep_silent = 1;
				}
				<Exit>;
				keep_silent = k;
				if (actor in j) return;
			}
			if (actor in noun) return;
			if (noun notin ancestor)
			{   j = parent(noun);
				while (parent(j) ~= ancestor) j = parent(j);
				L__M(##Enter,7,j);
				k = keep_silent; keep_silent = 1;
				<Enter j>;
				keep_silent = k;
				if (actor notin j) return;
				<<Enter noun>>;
			}
		}

		move actor to noun;
		if (AfterRoutines()==1) rtrue;
		if (keep_silent==1) rtrue;
		L__M(##Enter,5,noun);
		if(CanPlayerWitness()) Locale(noun);
	];
#endif;
#ifndef OpenSub;
	[ OpenSub;
		if (ObjectIsUntouchable(noun)) return;
		if (noun hasnt openable) return L__M(##Open,1,noun);
		if (noun has locked)     return L__M(##Open,2,noun);
		if (noun has open)       return L__M(##Open,3,noun);
		give noun open;
		if (AfterRoutines()==1) rtrue;
		if (keep_silent==1) rtrue;
		if (noun has container && noun hasnt transparent && child(noun)~=0
			&& IndirectlyContains(noun,actor)==0)
			return L__M(##Open,4,noun);
		L__M(##Open,5,noun);
	];
#endif;
#ifndef WearSub;
	[ WearSub;
		if (ObjectIsUntouchable(noun)) return;
		if (noun hasnt clothing)  return L__M(##Wear,1,noun);
		if (parent(noun)~=actor) return L__M(##Wear,2,noun);
		if (noun has worn)        return L__M(##Wear,3,noun);
		give noun worn;
		if (AfterRoutines()==1) rtrue;
		if (keep_silent==1) rtrue;
		L__M(##Wear,4,noun);
	];
#endif;
#ifndef TouchSub;
	[ TouchSub;
		if (noun==actor) return L__M(##Touch,3,noun);
		if (ObjectIsUntouchable(noun)) return;
		if (noun has animate) return L__M(##Touch,1,noun);
		L__M(##Touch,2,noun); 
	];
#endif;
#ifndef WaveSub;
	[ WaveSub;
		if (parent(noun)~=actor) return L__M(##Wave,1,noun);
		L__M(##Wave,2,noun); 
	];
#endif;
#ifndef AllowPushDir;
	[ AllowPushDir i;
		if (parent(second)~=compass) return L__M(##PushDir,2,noun);
		if (second==u_obj or d_obj)  return L__M(##PushDir,3,noun);
		AfterRoutines(); i=noun; move i to actor;
		<Go second>;
		if (location==thedark) move i to real_location;
		else move i to location;
	];
#endif;
#ifndef ThrowAtSub;
	[ ThrowAtSub;
		if (ObjectIsUntouchable(noun)) return;
		if (second>1)
		{   action=##ThrownAt;
			if (RunRoutines(second,before)~=0) { action=##ThrowAt; rtrue; }
			action=##ThrowAt;
		}
		if (noun has worn)
		{   L__M(##Drop,3,noun);
			<Disrobe noun>;
			if (noun has worn && noun in actor) rtrue;
		}
		if (second hasnt animate) return L__M(##ThrowAt,1);
		if (RunLife(second,##ThrowAt)~=0) rfalse;
		L__M(##ThrowAt,2,noun);
	];
#endif;
#ifndef KissSub;
	[ KissSub;
		if (ObjectIsUntouchable(noun)) return;
		if (RunLife(noun,##Kiss)~=0) rfalse;
		if (noun==actor) return L__M(##Touch,3,noun);
		L__M(##Kiss,1,noun);
	];
#endif;
#ifndef TellSub;
	[ TellSub;
		if (noun==actor) return L__M(##Tell,1,noun);
		if (RunLife(noun,##Tell)~=0) rfalse;
		L__M(##Tell,2,noun);
	];  
#endif;
#ifndef AskForSub;
	[ AskForSub;
		if (noun==actor) <<Inv>>;
		L__M(##Order,1,noun);
	];
#endif;

#ifndef L___M;
	[ L___M n x1 s;
		s=action;
		lm_n=n; lm_o=x1;
		action=sw__var;
		if(CanPlayerWitness() && ORSuppressLibraryMessages==false){
			if(actor~=player) print"^";
			if (RunRoutines(LibraryMessages,before)~=0) { action=s; rfalse; }
			if (LibraryExtensions.RunWhile(ext_messages,0) ~=0 ) { action = s; rfalse;}
		}
		action=s;
		if(CanPlayerWitness()  && ORSuppressLibraryMessages==false) LanguageLM(n, x1);
	];
#endif;
#ifndef GoSub;
	[ GoSub i j k df movewith thedir old_loc;
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
			if(CanPlayerWitness()){
				if(actor==player){
					if(i.cant_go ~= 0) PrintOrRun(i, cant_go);
				}else{
					return L__M(##Go,10,thedir);
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
		!TODO: needs work!!!  npcs should have to deal with darktodark...
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
#endif;
#ifndef InvSub;
	[ InvSub x;
		if (child(actor)==0) return L__M(##Inv,1);
		if (inventory_style==0) return InvTallSub();

		L__M(##Inv,2);
		if (inventory_style & NEWLINE_BIT ~= 0) L__M(##Inv, 3); else print " ";

		WriteListFrom(child(actor), inventory_style, 1);
		if (inventory_style & ENGLISH_BIT ~= 0) L__M(##Inv, 4);

		#IFNDEF MANUAL_PRONOUNS;
		objectloop(x in actor) PronounNotice(x);
		#ENDIF;
		x = 0; ! To prevent a "not used" error
		AfterRoutines();
	];
#endif;
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
	object _onpcvrb_init LibraryExtensions
		with ext_messages[;			  
		if(action==##Go){
			switch(lm_n)	{
				10: print(ig)CIVerb(actor,"tried","try","tries")," to go ",(LanguageDirection)lm_o," but ",(ig)vrb(actor,"could not","can not","will not be able to")," go that way.^";
					rtrue;
				11: print (ig)CIVerb(actor,"entered","enter")," from ";
						switch(lm_o){
							d_to: print "above"; !--reverse...
                                                        u_to: print "below"; !--reverse...
                                                       in_to: print "outside"; ! Fixes the same logic as above.
                                                      out_to: print "inside"; ! Ditto -- player enters from the out??

							default:print "the ",(LanguageDirection)ReverseDirection(lm_o);
						}
	#ifdef USE_ORNPC_movement;
					if(actor provides follow_object && actor.follow_object~=0) print " (following ",(TheMe)actor.follow_object,")";
	#endif;
					print".^";
					rtrue;
				12: print (ig)CIVerb(actor,"wandered","wander")," away ";
						switch(lm_o){
							u_to: print "upwards"; 
							d_to: print "downwards";
                                                      out_to: print "out of the room";
                                                       in_to: print "into another area";
							default:print "to the ",(LanguageDirection)lm_o;
						}
	#ifdef USE_ORNPC_movement;
					if(actor provides follow_object && actor.follow_object~=0) print " (following ",(TheMe)actor.follow_object,")";
	#endif;
					print ".^";
					rtrue;
			}
		}
	];
#ifndef PlayerTo;
	[ PlayerTo newplace flag sv;
		sv=actor;actor=player;
		move player to newplace;
		while (parent(newplace)~=0) newplace=parent(newplace);
		location=newplace;
		real_location=location; MoveFloatingObjects();
		AdjustLight(1);
		if (flag==0) <<Look>>;
		if (flag==1) { NoteArrival(); ScoreArrival(); }
		if (flag==2) LookSub(1);
		actor=sv;
	];
#endif;
	system_file;
	[ ActorTo newplace;
		if(actor==player) return playerto(newplace);
		move actor to newplace;
		MoveFloatingObjects();
	];
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================