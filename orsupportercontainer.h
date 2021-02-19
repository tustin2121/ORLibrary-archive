!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.23 ORSupporterContainer [Z, GLULX]
! Allows an object to be both a supporter and and container at the same time.
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
! This module allows an object to be both a container and a supporter (that is have 
! both the container and supporter attributes). The following code demonstrates:
!
!	object key "key" room with name 'key', description "a shiny key";
!	object dormfridge "dorm fridge" room 
!		with name 'fridge' 'dorm'
!		has supporter container open openable;
!
! Placing this code in a game will allow the player to put the key either inside or on
! top of the fridge.  As one would expect, objects can be place in or on an object 
! independantly, that is some can be in while others are on top.  Also, closeing the 
! fridge hides the contained objects while the objects on top remain in scope.
!--------------------------------------------------------------------------------------
! AutoDep:	ORWriteAfterEntry
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORSupporterContainer; #include "ORSupporterContainer"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORSupporterContainer; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.10.13	Initial Creation
! 2002.10.14	Removed dependancy upon ORNPC_Verb and added the missing "contained"
!				attribute.
! 2002.12.22	Added code to not require the contained attribute for contained objects
!				that are not also supporters.
! 2003.01.02	Extracted code to display contents in "on the" "in the" format in a reusable
!				function called "DescribeContents";
!				Also added a redefinition of the RemoveSub to better support containment.
! 2003.02.12	Added code to handle the message displayed when "opening" containers 
!				and initial description when container is not static.
! 2004.01.02	Modified the "on the" message to use two returns.
! 2004.01.17	Matched to new standard library 6/11
! 2004.02.09	optimized objectloops.
! 2004.02.23	fixed bug reported by user, where incomplete message is generated from
!				open if an object is "on" the container, but nothing is "in" the 
!				container (i.e. "You open the dresser, revealing .";
!--------------------------------------------------------------------------------------
message "          Processing library extension ORSupporterContainer...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORSupporterContainer forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORSupporterContainer has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORWriteAfterEntry_DONEREPLACE; default USE_ORWriteAfterEntry 0; message "          [ORSupporterContainer forcing inclusion of ORWriteAfterEntry]"; #include "ORWriteAfterEntry"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORSupporterContainer_DONEREPLACE; constant ORSupporterContainer_DONEREPLACE; #ifdef ORSupporterContainer_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	attribute contained; 
	
	replace InsertSub;
	replace DropSub;
	replace ExitSub;
	replace EnterSub;
	replace SearchSub;
	replace RemoveSub;
	replace OpenSub;

	replace AttemptToTakeObject;
	replace ScopeWithin;
	replace SayWhatsOn;
	replace ObjectIsUntouchable;
	replace WAE_Recurse;
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORSupporterContainer_DONEMESSAGE; constant ORSupporterContainer_DONEMESSAGE;  #ifdef ORSupporterContainer_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORSupporterContainer requires the OREnglish file.";#endif; 
	[WAE_Recurse o; 
		if (c_style & RECURSE_BIT ~= 0 && _WAE_child_count>0){
			if(SetContainedChildrenWorkFlag(o,-1)>0){
				print " (";
				DescribeContents(o,true);
				print ") ";
				rtrue;
			}
		}
		rfalse;
	];
	[ RemoveSub i;
		i=parent(noun);
		if (IsContained(noun) && i hasnt open) return L__M(##Remove,1,noun);
		if (i~=second) return L__M(##Remove,2,noun);
		if (i has animate) return L__M(##Take,6,i);
		if (AttemptToTakeObject(noun)) rtrue;
		action=##Remove; if (AfterRoutines()==1) rtrue;
		action=##Take;   if (AfterRoutines()==1) rtrue;

		if (keep_silent==1) rtrue;
		return L__M(##Remove,3,noun);
	];
	[ InsertSub ancestor;
		receive_action = ##Insert;
		if (second==d_obj || actor in second) <<Drop noun>>;
		if (parent(noun)~=actor) return L__M(##Insert,1,noun);

		ancestor = CommonAncestor(noun, second);
		if (ancestor == noun) return L__M(##Insert, 5, noun);
		if (ObjectIsUntouchable(second)) return;

		if (second ~= ancestor)
		{   
			action=##Receive;
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
		give noun contained;											!----------------------contained code

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
		if(actor has contained) give noun contained;					!----------------------contained code
		if (AfterRoutines()==1) rtrue;
		if (keep_silent==1) rtrue;
		return L__M(##Drop,4,noun);
	];
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

		! Are actor and item in totally different places?

		if (ancestor == 0) return L__M(##Take,8,item);

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
		give item ~contained;							!----------------------contained code

		! Send "after" message to the object letting go of the item, if any.

		if (after_recipient~=0)
		{   k=action; action=##LetGo;
			if (RunRoutines(after_recipient,after)~=0) { action=k; rtrue; }
			action=k;
		}
		rfalse;
	];
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
														!----------------------contained code (begin)
		if(parent(actor) has container) give actor contained;
		else give actor ~contained;
														!----------------------contained code (end)
		if (AfterRoutines()==1) rtrue;
		if (keep_silent==1) rtrue;
		L__M(##Exit,3,p); LookSub(1);
	];
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
														!----------------------contained code (begin)
		if(parent(actor) has container) give actor contained;
		else give actor ~contained;
														!----------------------contained code (end)
		if (AfterRoutines()==1) rtrue;
		if (keep_silent==1) rtrue;
		L__M(##Enter,5,noun);
		if(CanPlayerWitness()) Locale(noun);
	];

	[ SayWhatsOn descon f;
		if (descon==parent(actor)) rfalse;
		f=SetContainedChildrenWorkFlag(descon,true);
		if (f==0) rfalse;
		L__M(##Look, 4, descon); 
		SetContainedChildrenWorkFlag(descon,3); !--will reset the flag
		rtrue;
	];
	!--this routine determines if on object is contained.  This is really needed because the contained attribute
	!--is an addition to the library.  Simple containers that are not also supporters will likely not have been coded
	!--with the attribute.
	[IsContained obj;
		if(obj has contained) rtrue;
		if(parent(obj) has container && parent(obj) hasnt supporter) rtrue;
		rfalse;
	];
	[ OpenSub f i;
		if (ObjectIsUntouchable(noun)) return;
		if (noun hasnt openable) return L__M(##Open,1,noun);
		if (noun has locked)     return L__M(##Open,2,noun);
		if (noun has open)       return L__M(##Open,3,noun);
		give noun open;
		if (AfterRoutines()==1) rtrue;
		if (keep_silent==1) rtrue;
		objectloop (i in noun) if (i hasnt concealed && i hasnt scenery && IsContained(i)==true) f=1;
		if (noun has container && noun hasnt transparent 
			&& IndirectlyContains(noun,actor)==0 && f==1)
			return L__M(##Open,4,noun);
		L__M(##Open,5,noun);
	];
	[ SearchSub i f;
		if (location==thedark) return L__M(##Search,1,noun);
		if (ObjectIsUntouchable(noun)) return;
		if (noun hasnt container && noun hasnt supporter) return L__M(##Search,4,noun); !--find nothing of interest
		!--at this point we are guaranteed at least one form of containment..
		objectloop (i in noun) if (i hasnt concealed && i hasnt scenery && (IsContained(i)==false || parent(i) has open || parent(i) has transparent)) f=1;
		if(f==0){ !--no children, so which message?
				if(noun has container){ !--prefer the container messages if possible
					if(noun hasnt transparent && noun hasnt open) return L__M(##Search,5,noun); !--"can't see inside"
					return L__M(##Search,6,noun); !--"is empty"
				}
				return L__M(##Search,2,noun); !--at this point we know we have a supporter 
		}
		if (AfterRoutines()==1) rtrue;
		!--actually contains something(s)
		DescribeContents(noun);
		print "^";
		rtrue;
	];
	

	[ ObjectIsUntouchable item flag1 flag2 ancestor i oldi;
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
		{   
			oldi=actor;
			i = parent(actor);
			while (i~=ancestor)
			{   if (i has container && i hasnt open && IsContained(oldi)==true)
				{   if (flag1) rtrue;
					return L__M(##Take,9,i);
				}
				oldi=i;
				i = parent(i);
			}
		}

		! Second, a barrier between the item and the ancestor.  The item can
		! be carried by someone, part of a piece of machinery, in or on top
		! of something and so on.

		if (item ~= ancestor)
		{   oldi=item;
			i = parent(item);
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
				if (i has container && i hasnt open && IsContained(oldi)==true)
				{   if (flag1) rtrue;
					return L__M(##Take,9,i);
				}
				oldi=i;
				i = parent(i);
			}
		}
		rfalse;
	];
	[ ScopeWithin domain nosearch context x y;
		if (domain==0) rtrue;
		!  Special rule: the directions (interpreted as the 12 walls of a room) are
		!  always in context.  (So, e.g., "examine north wall" is always legal.)
		!  (Unless we're parsing something like "all", because it would just slow
		!  things down then, or unless the context is "creature".)
		if (indef_mode==0 && domain==actors_location
			&& scope_reason==PARSING_REASON && context~=CREATURE_TOKEN)
				ScopeWithin(compass);
		!  Look through the objects in the domain, avoiding "objectloop" in case
		!  movements occur, e.g. when trying each_turn.
		x = child(domain);
		while (x ~= 0)
		{   y = sibling(x);
			if(IsContained(x)==false || domain has open || domain has transparent) 
				ScopeWithin_O(x, nosearch, context);
			x = y;
		}
	];
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORSupporterContainer_DONECODE;constant ORSupporterContainer_DONECODE; #ifdef ORSupporterContainer_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	object _sc_init LibraryExtensions
		with ext_messages[;
			Look:	switch(lm_n){
						4: print "^^On ", (the) lm_o;
							WriteListFrom(child(lm_o),
								ENGLISH_BIT + WORKFLAG_BIT + RECURSE_BIT + PARTINV_BIT
								+ TERSE_BIT + ISARE_BIT + CONCEAL_BIT);
							".";
					}
			Search: switch(lm_n){
						3: 	print "On ", (the) lm_o;
							WriteListFrom(child(lm_o),
								TERSE_BIT + WORKFLAG_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT + PARTINV_BIT + RECURSE_BIT );
							print ".";
							rtrue;
						7: 	print "In ", (the) lm_o;
							WriteListFrom(child(lm_o),
								TERSE_BIT + WORKFLAG_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT + PARTINV_BIT + RECURSE_BIT );
							print ".";
							rtrue;
						8: 	print "supporting ";
							WriteListFrom(child(lm_o),
								TERSE_BIT + WORKFLAG_BIT + ENGLISH_BIT + CONCEAL_BIT + PARTINV_BIT + RECURSE_BIT );
							rtrue;
						9: 	print "containing ";
							WriteListFrom(child(lm_o),
								TERSE_BIT + WORKFLAG_BIT + ENGLISH_BIT + CONCEAL_BIT + PARTINV_BIT + RECURSE_BIT );
							rtrue;

					}
			Open:	if(lm_n==4){
						print (ig)CIVerb(actor,"opened","open"), " ",(the) lm_o, ", revealing ";
						SetContainedChildrenWorkFlag(lm_o,true);
						if(WriteListFrom(child(lm_o),
					      ENGLISH_BIT + WORKFLAG_BIT + TERSE_BIT + CONCEAL_BIT)==0) "nothing.";
	                  ".";
					}
								
		];
system_file;
	[DescribeContents obj lcase retval;
		if(SetContainedChildrenWorkFlag(obj,false)>0){
				if(lcase==true) 
					L__M(##Search,8,obj);
				else
					L__M(##Search,3,obj);
				if(SetContainedChildrenWorkFlag(obj,true)>0){
					if(lcase==true) print "; and";
					print " ";
				}
				retval=true;
		}
		if(SetContainedChildrenWorkFlag(obj,true)>0){
			if(lcase==true)
				L__M(##Search,9,obj);
			else
				L__M(##Search,7,obj);
			retval=true;
		}
		return retval;
	];
	!--for_contained true: containment; false: supporting; -1: both
	[SetContainedChildrenWorkFlag par_obj for_contained obj retval;
		objectloop(obj in par_obj){
			if(obj hasnt concealed && obj hasnt scenery){
				if(for_contained==IsContained(obj) || for_contained==-1){
					if(IsContained(obj)==false || parent(obj) has open || parent(obj) has transparent){
						give obj workflag;
						retval++;
					}
				}else give obj ~workflag;
			}
		}
		return retval;
	];

!======================================================================================
#endif; #ifnot; #ifndef ORSupporterContainer_DONEGRAMMAR; constant ORSupporterContainer_DONEGRAMMAR; #ifdef ORSupporterContainer_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
#ifndef CanPlayerWitness;
	[CanPlayerWitness; rtrue;];
#endif;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================