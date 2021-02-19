!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORLiquid	[Z, GLULX]
! Classes for implementing liquid behaving objects.  Like water, or sand.
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
! AutoDep:	ORReferByContents
!			ORDispensor	
!--------------------------------------------------------------------------------------
! Sample code.  More concise documentation will be implemented some time in the future.
!
!	class sand(20) 
!		class ORLiquid 
!		with	short_name "sand"
!		,		plural "handfuls of sand"
!	;
!
!	ORLiquidSource beachsand "beach" beach
!		has	supporter ~container
!		with	itemclass sand
!		,	conceal_dispensing_items true
!	;
!
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORLiquid";					!Constant USE_ORLiquid; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORLiquid; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.01.05	Initial Creation (not really, but this is the earliest date I could 
!				find without searching my code archive)
! 2002.02.26	Fit to new template
! 2002.05.12	Added code to the before rules that verifies objects are indeed 
!				touchable. Also added check to verify that player is present before 
!				printing messages. (These code fixes were sent in by Stephen Norris)
!				Additionally corrected a message "the water run though your fingers..."
! 2002.05.13	Added code to verify CanPlayerWitness values before checking if Object
!				is Untouchable.
! 2002.05.24	Leveraged the ORChooseObjects module to lessen the likelyhood that the 
!				water will be selected for a take or a drop, etc.
! 2002.08.31	Modified to reference the new OREntryPoints module.
! 2002.09.02	Converted attributes previously defined in this module to properties (not 
!				including wet and water_tight)
! 2002.10.15	Converted water_tight attributes to a property as well, leaving only wet
!				which is as it should be.
! 2003.01.25	Added more concise code to the chooseobjects property
! 2003.01.30	Cleaned up some destroy code which caused errors when an object 
!				was not created.
! 2003.02.17	Fixed bug in the CanContainDryLiquid routine.
! 2003.07.19	Added code to better pour a liquid into something else.
! 2003.12.15	Fixed a bug found by the 6.3 compiler
! 2004.01.11	Removed auto include of object_initialise
!--------------------------------------------------------------------------------------
message "          Processing library extension ORLiquid...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORLiquid forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORLiquid has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORRoutinesList_DONEREPLACE; default USE_ORRoutinesList 0; message "          [ORLiquid forcing inclusion of ORRoutinesList]"; #include "ORRoutinesList"; #endif; 
	#ifndef OREntryPoints_DONEREPLACE; default USE_OREntryPoints 0; message "          [ORLiquid forcing inclusion of OREntryPoints]"; #include "OREntryPoints"; #endif; 
	#ifndef ORReferByContents_DONEREPLACE; default USE_ORReferByContents 0; message "          [ORLiquid forcing inclusion of ORReferByContents]"; #include "ORReferByContents"; #endif; 
	#ifndef ORDispensor_DONEREPLACE; default USE_ORDispensor 0; message "          [ORLiquid forcing inclusion of ORDispensor]"; #include "ORDispensor"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORLiquid_DONEREPLACE; constant ORLiquid_DONEREPLACE; #ifdef ORLiquid_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	attribute	wet;
!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORLiquid_DONEMESSAGE; constant ORLiquid_DONEMESSAGE;  #ifdef ORLiquid_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORLiquid requires the OREnglish file.";#endif; 

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORLiquid_DONECODE;constant ORLiquid_DONECODE; #ifdef ORLiquid_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
#ifndef CanPlayerWitness;[CanPlayerWitness; rtrue; ]; #endif;

class ORliquid
		with	article "some"
		,		short_name "liquid"
		,		plural "measures of liquid"
		,		was_created false
		,		create[;self.was_created=true;]
		,		before
				[value cpw;
					cpw=CanPlayerWitness();
					remove, take:
						if(cpw==false) rtrue;
						if(ObjectIsUntouchable(noun, false, true)) rtrue;
						print (ig)CIVerb(actor,"scooped","scoop")," up ",(the)self," in ",(my)actor," hand, but it ",(ig)vrb(SINGULAR_OBJ,"ran","run")," through ",(my)actor," fingers.";
						rtrue;
					Eat, Drink: 
						if(ObjectIsUntouchable(noun, ~cpw, true)) rtrue;
						if(self has edible){
							if((parent(self) ofclass ORLiquidSource)==false || 
								(parent(self) provides itemclass)==false || 
								(self ofclass (parent(self).itemclass))==false){
									if(self.was_created==true)
										ORLiquid.destroy(self);
									else
										remove self;
							}
							if(AfterRoutines()~=1){
								if(cpw==false) rtrue;
								print (ig)CIVerb(actor,"drank","drink")," ",(the)self,".  Very satisfying.^";
							}
							rtrue;
						}
					Drop:
						printorrun(self,liquid_vanishes);
						if(self.was_created==true)
							ORLiquid.destroy(self);
						else
							remove self;

						rtrue;
					ScoopInto,Insert,PutOn:
						if(ObjectIsUntouchable(noun, ~cpw, true)) rtrue;
						if(ObjectIsUntouchable(second, ~cpw, true)) rtrue;
						if(valueorrun(self,wetliquid)==true) value=CanContainWetLiquid(second);
						else value=CanContainDryLiquid(second);
						if(value==-1 && (second ofclass ORLiquidSource)==false){
							if(cpw==false) rtrue;
							print (ig)CIAm(second)," too full to hold more ",(name)self,".^";
							rtrue;
						}
						else if(value){
							if(cpw==true) {
								if(action==##ScoopInto)
									print (ig)CIVerb(actor,"scooped","scoop");
								else print (ig)CIVerb(actor,"poured","pour");
								print" ",(the)self;
								if(second has supporter) print" onto ";	
								else print " into ";
								print (the)second,".^";
							}
							move self to second;
						}else{
							print "test";
							printorrun(self,cant_puton_msg);
						}
						rtrue;
				]
		,		wetliquid false
		,		ChooseObject[code;
					if(code<2)return 0;
					if(action_to_be==##EmptyT or ##remove or ##take or ##insert or ##puton or ##drop) return 0;
					return 1;
				]
		,		cant_puton_msg[;
					if(CanPlayerWitness()==false) rtrue;
					print (ig)CICant(actor)," put ",(the)noun," ";
					if(action==##Insert || second has container) print "in"; else print "on";
					print " ",(the) second,".^";
				]
		,		liquid_vanishes[;
					if(CanPlayerWitness()==false) rtrue;
					print (ig)CIVerb(self,"fell","fall")," to the ground, effectively vanishing.^";
				]

	;
	[CanContainWetLiquid obj val;
		val=CanContainDryLiquid(obj);
		if(val==false or -1) return val;
		if(obj provides water_tight && valueorrun(obj,water_tight)==true) rtrue; 
		rfalse;
	];
	[CanContainDryLiquid obj o count;
		if(obj hasnt container or supporter) rfalse;
		if(obj has openable && obj hasnt open) rfalse;
		if((obj provides liquid_measures)==false) rfalse;
		count=obj.liquid_measures;
		objectloop(o in obj) if(o provides liquid_measures) count=count-o.liquid_measures;
		if(count<=0) return -1;
		rtrue;
	];
	class ORLiquidSource
		class	ORReferByContents ORDispensor
		has		container open ~openable static
		with	liquid_measures 1 ! just two really needed, as new objects are destroyed as they are inserted
		,		water_tight true
		,		ignore_actions ##Drink
		,		before
				[f n count value cpw;
							cpw=CanPlayerWitness();
							ScoopInto:	
								if(ObjectIsUntouchable(noun, ~cpw, true)) rtrue;
								if(ObjectIsUntouchable(second, ~cpw, true)) rtrue;
								if(valueorrun((self.itemobj),wetliquid)==true) value=CanContainWetLiquid(second);
								else value=CanContainDryLiquid(second);
								if(value==-1){
									if(cpw==false) rtrue;
									print (ig)CIAm(second)," too full to hold more ",(name)self.itemobj,".^";
									rtrue;
								}
								else if(value){
									if(cpw==true){
										print (ig)CIVerb(actor,"scooped","scoop")," ",(the)self.itemobj;
										if(second has supporter) print" onto ";	
										else print " into ";
										print (the)second,", filling it.^";
									}
									self.fill_container(second);
									if(valueorrun((self.itemobj),wetliquid)==true) give noun wet;
								}else{
									if(cpw==false) rtrue;
									printorrun(self.itemobj,cant_puton_msg);
								}
								rtrue;
							Receive:
								if((noun ofclass self.itemclass)==true) {
									if(noun.was_created==true)
										self.itemobj.destroy(noun);
									else
										remove noun;
								}
								else {
									if(valueorrun((self.itemobj),wetliquid)==true && CanContainWetLiquid(noun)==true) {
										!--first, any wet liquid is destroyed, diluted beyond recognition or sopped up by dry liquids
										f=child(noun);
										while(f~=0){
											n=sibling(f);
											if(f ofclass ORLiquid && valueorrun(f,wetliquid)==true) {
												if(f.was_created==true)
													f.destroy(f);
												else 
													remove f;

											}
											f=n;
										}
										!--now lets fill the container up
										self.fill_container(noun);
										if(valueorrun((self.itemobj),wetliquid)==true) give noun wet;
									}
								}
					LetGo:	
								count=0;
								objectloop(f in noun) if(f ofclass ORLiquid) count++;
								move noun to actor;
								if(valueorrun((self.itemobj),wetliquid)==true) give noun wet;
								if(cpw==false) rtrue;
								print (ig)CIVerb(actor,"pulled","pull")," ",(the)noun;
								if(count>0 && noun has open) print " (full of ",(name)self.itemobj,")";
								print " from ",(the)self,".^";
								rtrue;
				]
		,		fill_container
				[obj count o w t value;
					if(valueorrun((self.itemobj),wetliquid)==true) value=CanContainWetLiquid(obj);
					else value=CanContainDryLiquid(obj);
					if(value) { !--if fillable
						count=obj.liquid_measures; !---calculate room to fill
						objectloop(o in obj) if(o provides liquid_measures) count=count-o.liquid_measures;
						if(count>0){ 
							for(t=0:t<count:t++){
								w=self.itemclass.create();
								move w to obj;
							}
							return count;
						}
					}
					return 0;
				]
		,		receive_item_msg[;  
						if(CanPlayerWitness()==false) rtrue;
						if(noun ofclass ORLiquid){
							print (ig)CIVerb(actor,"poured","pour")," ",(the) noun," into ", (the) self, ".^";
						}
						else{
							print (ig)CIVerb(actor,"dropped","drop")," ",(the) noun," into ", (the) self, ".^";
						}
					]

		;

	object _oliq_init LibraryExtensions
		with ext_messages[;
				switch(action){
					##Drink: print (ig)CICant(actor)," drink ",(the)noun,".^";
						rtrue;
				}
			rfalse;];
!======================================================================================
   #endif; #ifnot; #ifndef ORLiquid_DONEGRAMMAR; constant ORLiquid_DONEGRAMMAR; #ifdef ORLiquid_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
	!--- Is the object in the normal scope of the actor?  That is, a 
	!--- scope that is unmodified by specialized scope rules.  This object 
	!--- may by untouchable, in the player's possession, or just nearby.
	[IsObjectInNormalScope obj a;
		a=CommonAncestor(actor, obj);
		if(a==0){
			a=ObjectScopedBySomething(obj);
			if(a~=0) a=CommonAncestor(actor,a);
		}
		if(a==0 or obj) rfalse;
		rtrue;
	];

	[ContainerSupporterScope o;
		switch(scope_stage){
			1:rfalse; !allow multiple objects
			2:objectloop(o has container && IsObjectInNormalScope(o)==true) PlaceInScope(o);
				rtrue;
		}
	];
	[MultiLiquidScope o;
		switch(scope_stage){
			1:rtrue; !allow multiple objects
			2:objectloop(o ofclass ORLiquid && IsObjectInNormalScope(o)==true) PlaceInScope(o);
				rtrue;
		}
	];
	[liquidsource; if(noun ofclass ORLiquidSource) rtrue; rfalse;];
	verb 'scoop'
		* 'up' noun=liquidsource -> Take
		* 'up' scope=MultiLiquidScope -> Take
		* noun=liquidsource -> Take
		* scope=MultiLiquidScope -> Take
		* 'up' noun=liquidsource 'with'/'in'/'into'/'onto' held -> ScoopInto
		* 'up' scope=MultiLiquidScope 'with'/'in'/'into'/'onto' held -> ScoopInto
		* noun=liquidsource 'up' 'with'/'in'/'into'/'onto' held -> ScoopInto
		* scope=MultiLiquidScope 'up' 'with'/'in'/'into'/'onto' held -> ScoopInto
		* noun=liquidsource 'with'/'in'/'into'/'onto' held -> ScoopInto
		* scope=MultiLiquidScope 'with'/'in'/'into'/'onto' held -> ScoopInto
		* held 'in'/'into' noun=liquidsource -> ScoopInto reverse
		* held 'in'/'into' scope=MultiLiquidScope -> ScoopInto reverse
	;
	extend 'fill' replace
		* held 'from'/'with' noun=liquidsource -> ScoopInto reverse
		* held 'from'/'with' scope=MultiLiquidScope -> ScoopInto reverse
	;
	verb 'pour'
		* 'out' held ->	Empty
		* held 'out' ->	Empty
		* multiinside 'out' 'of' held ->	Empty reverse
		* multiinside 'from' held ->	Empty reverse
		* multiinside 'on'/'onto'/'into'/'in' scope=ContainerSupporterScope ->	Pour
		* multiinside 'on'/'onto'/'into'/'in' held ->	Pour
		* held 'on'/'onto'/'into'/'in' scope=ContainerSupporterScope ->	EmptyT 
		* held 'on'/'onto'/'into'/'in' held ->	EmptyT
	;
	[ScoopIntoSub;
		print (ig)CICant(actor)," scoop up ",(the)second,".";
	];
	[PourSub o; o=parent(noun);
	<EmptyT o second>;];
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================