!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.03.06	ORMagic	[Z, GLULX]
! The base class for magic spells.
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
! A more detailed description of this file, what it does, and how to use it goes here.
!
!--------------------------------------------------------------------------------------
! AutoDep:	
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORMagic; #include "ORMagic"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORMagic; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.06.20	Initial Creation
! 2002.07.03	Moved the actual casting code from my WIP to this module.
! 2002.07.05	Made the spells command meta.
! 2002.07.06	Corrected bug with the "spells" verb output formatting and spell 
!				selection.
! 2002.07.07	Added menu documentation
! 2002.08.28	Added code to support casting of spells without the need to specify the 
!				verb 'cast'
! 2002.08.31	Modified to reference the new OREntryPoints module.
! 2002.12.07	Removed references the "description" property in the spell and implemented 
!				a property named "text" to describe what a spell does (as displayed by the
!				"spells" verb)
! 2003.02.02	Modified the menu documentation.
! 2003.02.19	Added better ifdef for self docs
! 2004.02.09	Optimized objectloops.
! 2004.03.06	Minor modification to self documentation.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORMagic...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORMagic forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORMagic has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORRoutinesList_DONEREPLACE; default USE_ORRoutinesList 0; message "          [ORMagic forcing inclusion of ORRoutinesList]"; #include "ORRoutinesList"; #endif; 
!	#ifndef OREntryPoints_DONEREPLACE; default USE_OREntryPoints 0; message "          [ORMagic forcing inclusion of OREntryPoints]"; #include "OREntryPoints"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORMagic_DONEREPLACE; constant ORMagic_DONEREPLACE; #ifdef ORMagic_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORMagic_DONEMESSAGE; constant ORMagic_DONEMESSAGE;  #ifdef ORMagic_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORMagic requires the OREnglish file.";#endif; 
	class ORMagic 
		with cast[; 
				print (ig)CIVerb(actor,"cast","cast")," ",(the)self," spell with no affect.";
			]
		,	text 0
		,	active false
		,	cast_upon 0
		,	KnownBy 0
		,	IsKnownBy[o; return WordInProperty(o,self,KnownBy);]
		,	heartbeat 0
		,	priority 0
	;

	object ORMagicReserve;
	object _magicuv LibraryExtensions 
		with ext_initialise[o;
			ORUnknownVerb.add_routine(self.one);
			objectloop(o ofclass ORMagic) move o to ORMagicReserve;
		]
	,	one[word obj;
				objectloop (obj in ORMagicReserve){
					if (obj ofclass ORMagic){
					#ifdef USE_ORRecogName; 
						if(WordInProperty(word,obj,name)==true || MatchAgainstShortName(obj,verb_wordnum)==true){
					#ifnot;
						if(WordInProperty(word,obj,name)==true){
					#endif; 
							verb_wordnum=0;
							return 'cast';
						}
					}
				}
				rfalse;
			]
	;
!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORMagic_DONECODE;constant ORMagic_DONECODE; #ifdef ORMagic_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	!-----------------------------------------------------------------------------
	!---ORMagicControl  object
	!---This is essentially the same code (with a few modifications) as the 
	!---NPC control object.  It allows all active spells to function each turn
	!-----------------------------------------------------------------------------
	object ORMagicControl LibraryExtensions
		with pause false !--halt all continuous magic
		,	spells_acting false
		,	magiclist 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		,	description 0
		,	daemon 
			[spell t lowest_priority highest_priority outcount;
				if(self.pause==true) return;
				self.spells_acting=true;
				self.&magiclist-->0=0;
				objectloop(spell in ORMagicReserve){
					if (spell ofclass ORMagic) { 
						self.&magiclist-->0=(self.&magiclist-->0)+1;
						self.&magiclist-->(self.&magiclist-->0)=spell;
						if(spell.priority>highest_priority)highest_priority=spell.priority;
						if(spell.priority<lowest_priority)lowest_priority=spell.priority;
					}
				}
				for(outcount=lowest_priority:outcount<=highest_priority:outcount++){
					for(t=0:t<self.&magiclist-->0:t++){
						spell=self.&magiclist-->(t+1);
						if(spell.priority~=outcount) continue;
						if(parent(spell)~=0)
							actor=spell; !--so that floating objects are in the right place at the right time
						else
							actor=player;
						MoveFloatingObjects();
						if(spell.heartbeat~=0 && spell.active==true) spell.heartbeat();
					}						
				}
				actor=player;		!restore the actor
				MoveFloatingObjects();
				self.spells_acting=false;
			]
		,	ext_initialise[; StartDaemon(self);]
	;
!======================================================================================
   #endif; #ifnot; #ifndef ORMagic_DONEGRAMMAR; constant ORMagic_DONEGRAMMAR; #ifdef ORMagic_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	!--shows which spells the player knows (isknownby routine)
	Verb meta 'spells'*->ShowSpells;
	[ShowSpellsSub o count;
		objectloop(o in ORMagicReserve) if(o ofclass ORMagic && o.IsKnownBy(actor)==true) count++;
		print (CIHave)actor;
		if(count==0) print " no magic";
		else print " the following spell";
		if(count>1)print "s";
		print " at ",(my)actor," disposal";
		if(count==0) ".";
		print":^";
		objectloop(o in ORMagicReserve) {
			if(o ofclass ORMagic && o.IsKnownBy(actor)){
				print "     ",(name)o;
				if(o provides text && o.text~=0){
					print " ("; 
					printorrun(o,text,true);
					print ")"; 
				}
				print "^";
			}
		}
	];
	!--actually cast a spell
	[ActorSpell o; !isolate information known by the actor
		switch(scope_stage){
			1: return false;
			2: 	objectloop(o in ORMagicReserve) if(o ofclass ORMagic && o.isknownby(actor)==true) PlaceInScope(o); 
				return true;
			3: 	print_ret (bkt)"I am uncertain what you want to cast.";
		}
	];
	Verb 'cast' 
		* scope=ActorSpell -> CastSpell
		* scope=ActorSpell 'at'/'on' noun -> CastSpell		
		* scope=ActorSpell noun -> CastSpell		
	;
	[CastSpellSub;
		noun.cast();
	];

#ifdef ORLibDoc;
	!--Commands, document thyself.
	ORMenu ORHelpMagic "Magic" ORLibDoc with text "The system used for magic is fairly straightforward. Only two commands are used, ~Spells~ and ~Cast.~";
		ORMenu "Spells" ORHelpMagic with text "The ~Spells~ command will list all spells that the player character currently knows. Known spells can be cast with the ~Cast~ command.";
		ORMenu "Cast" ORHelpMagic with text[;
									print "~Cast~ is an optional command that is used in conjunction with a known spell. It can be used in two forms:^^";
									font off;
									spaces(4);print">CAST THONTAS^^";
									font on; print"or^^";
									font off;
									spaces(4);print">CAST FRIRQUIN AT SNOWMAN^^";
									font on;
									print"As was stated above, ~Cast~ is optional in that:^^";
									font off; 
									spaces(4);print">THONTAS^^";
									font on; print"or^^";
									font off;
									spaces(4);print">FRIRQUIN SNOWMAN^^";
									font on;
									print "will also work.^^";
									print (italics)"NOTE: These example spells do not necessarily appear in the game.";
		];
#endif;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================