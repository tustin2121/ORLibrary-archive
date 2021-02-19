!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.03.03 ORInformLibrary	[Z, GLULX]
! A rearangement of the ORInformLibrary object.  No actual code has been altered (at
! least not in any significant way) but the routines have been broken into smaller 
! componants which can be overriden by other modules.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- - 
! License: 
! Matches that of Inform's standard library (see included header, below).
!--------------------------------------------------------------------------------------
! This code was derived from the standard library's InformLibrary object
! that comes from Graham Nelson's standard library (actually Andrew Plotkin's
! Biplatform library which is a modified version of Graham Nelson's standard library).  
! It falls under the same copyright as the code upon which it was based 
! which can be found in the standard library's parserm file.  The header to this file 
! is included now:
!--------------------------------------------------------------------------------------
	!  PARSERM:  Core of parser.
	!
	!  Supplied for use with Inform 6                         Serial number 991113
	!                                                                 Release 6/10
	!  (c) Graham Nelson 1993, 1994, 1995, 1996, 1997, 1998, 1999
	!      but freely usable (see manuals)
!--------------------------------------------------------------------------------------
! AutoDep:	none
!--------------------------------------------------------------------------------------
! ORLibDev: Note, that this module is not useful as a library extension alone.  By 
!     itself it provides no new functionality for the IF developer. Instead it provides
!     a basis for other module to be built upon and will primarily be useful to those 
!     who are developing ORLibrary entries of their own.
!--------------------------------------------------------------------------------------
!--------------------------------------------------------------------------------------
! This module leverages modifies the standard library's InformLibrary
! object.  No code has been changed and no new functionality has been added to the 
! InformLibrary object, however the excessively long play() routine
! has been broken into several smaller properties which can now be overridden by 
! subsequent ORLibrary modules to apply modifications.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORInformLibrary";					!Constant USE_ORInformLibrary; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORInformLibrary; 
!	
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.07	Initial Creation
! 2002.02.13	Modified to instantiate the __ORInformLibrary object rather than force 
!				developers to do that. Overrides to this object can be made with the 
!				ORRedefineObject module, just as it is used here.
! 2002.02.26	Fit to new template
! 2003.12.26	Verified against new standard library 6.11's beta 1.
! 2004.01.11	Verified against new standard library 6.11's beta 2.
! 2004.01.17	Fixed bug introduced by most recent conversion: ORRedefineObject
!				is now run within play, therefore redefining play has no affect.
!				This method no longer uses ORRedefineObject, rather it replaces main
!				and does the InformLibrary modifications there. 
! 2004.02.05	Aditional changes to better reflect stdlib enhancments.
! 2004.02.11	Change to support problems when ORInformLibrary is auto-included.
! 2004.03.03	Removed references to the ExtensionRunRoutines... routines.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORInformLibrary...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORInformLibrary forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORInformLibrary has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORInformLibrary_DONEREPLACE; constant ORInformLibrary_DONEREPLACE; #ifdef ORInformLibrary_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	replace main;
!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORInformLibrary_DONEMESSAGE; constant ORInformLibrary_DONEMESSAGE;  #ifdef ORInformLibrary_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORInformLibrary requires the OREnglish file.";#endif; 
	[ Main; 
		InformLibrary.play=__ORInformLibrary.play;	
		InformLibrary.play(); 
	];
!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORInformLibrary_DONECODE;constant ORInformLibrary_DONECODE; #ifdef ORInformLibrary_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	object __ORInformLibrary
		with play[; return __ORInformLibrary.new_play();]
		!---------------------------------------
		,	new_play 
			[j;
				j=__ORInformLibrary.play_pre_initialise();
				__ORInformLibrary.play_post_initialise();
				if(j~=2) Banner();
				__ORInformLibrary.play_post_banner();
	.very__late__error;
				__ORInformLibrary.play_loop_until_dead();
				if (deadflag~=2) AfterLife();
				if (deadflag==0) jump very__late__error;
				__ORInformLibrary.play_handle_death();
		]
		!---------------------------------------
		,  play_pre_initialise[;
				#ifndef TARGET_GLULX;
					standard_interpreter = $32-->0;
					transcript_mode = ((0-->8) & 1);
				#ifnot; ! TARGET_GLULX
					GGInitialise();
				#endif; ! TARGET_

					ChangeDefault(cant_go, CANTGO__TX);

				#ifndef TARGET_GLULX;
					dict_start = 0-->4;
					dict_entry_size = dict_start->(dict_start->0 + 1);
					dict_start = dict_start + dict_start->0 + 4;
					dict_end = (dict_start - 2)-->0 * dict_entry_size;
					#Ifdef DEBUG;
					if (dict_start > 0 && dict_end < 0 &&
					((-dict_start) - dict_end) % dict_entry_size == 0)
						print "** Warning: grammar properties might not work
							correctly **^";
					#Endif; ! DEBUG

					buffer->0 = INPUT_BUFFER_LEN;
					buffer2->0 = INPUT_BUFFER_LEN;
					buffer3->0 = INPUT_BUFFER_LEN;
					parse->0 = 15;
					parse2->0 = 15;
				#endif; ! TARGET_ZCODE
				       
					real_location = thedark;
					player = selfobj; actor = player;
				    
				#ifndef TARGET_GLULX;
					top_object = #largest_object-255;
				#endif; ! TARGET_ZCODE
					selfobj.capacity = MAX_CARRIED; ! ### change?
					#ifdef LanguageInitialise;
					LanguageInitialise();
					#endif;
					new_line;
					LibraryExtensions.RunAll(ext_initialise);
					return Initialise();
			]
		!---------------------------------------
		,	play_post_initialise[i;
				last_score = score;
				move player to location;
				while (parent(location)~=0) location=parent(location);
				real_location = location;
				objectloop (i in player) give i moved ~concealed;
			]
		!---------------------------------------
		,	play_post_banner[i j;
				MoveFloatingObjects();
				lightflag=OffersLight(parent(player));
				if (lightflag==0) { real_location=location; location=thedark; }
				<Look>;
				for (i=1:i<=100:i++) j=random(i);
	#ifdef EnglishNaturalLanguage;
				old_itobj = itobj; old_himobj = himobj; old_herobj = herobj;
	#endif;
			]
		!---------------------------------------
		,	play_loop_until_dead[ i j;
				while(~~deadflag){   
			.late__error;
					__ORInformLibrary.play_loop_pre_parse();
					__ORInformLibrary.play_loop_post_parse();
			.begin__action;
					i=__ORInformLibrary.play_loop_begin_action();
					!  --------------------------------------------------------------
					!  The player's "orders" property can refuse to allow conversation
					!  here, by returning true.  If not, the order is sent to the
					!  other person's "orders" property.  If that also returns false,
					!  then: if it was a misunderstood command anyway, it is converted
					!  to an Answer action (thus "floyd, grrr" ends up as
					!  "say grrr to floyd").  If it was a good command, it is finally
					!  offered to the Order: part of the other person's "life"
					!  property, the old-fashioned way of dealing with conversation.
					if(actor~=player)
					{   
						j=RunRoutines(player,orders);
						if (j==0)
						{   j=RunRoutines(actor,orders);
							if (j==0)
							{   if (action==##NotUnderstood)
								{   inputobjs-->3=actor; actor=player; action=##Answer;
									jump begin__action;
								}
								if (RunLife(actor,##Order)==0) L__M(##Order,1,actor);
							}
						}
					}
					else{ !--the player is not the actor...
						!--verify everything was resolved correctly
						if ((i==0) || (i==1 && inp1 ~= 0)|| (i==2 && inp1 ~= 0 && inp2 ~= 0)){
							InformLibrary.begin_action(action, noun, second, 0);
						}
						else{ !--if not, then check for a multiple object...
							! check the multiple list isn't empty;
							multiflag = true;
							j=multiple_object-->0;
							if(j==0){ 
								L__M(##Miscellany,2); 
								jump late__error; !--go back and start over
							}
							__ORInformLibrary.play_loop_handle_multiple(j); !--definately handling a multiple, so do that
						}
					}
			.turn__end;
					!  No time passes if either (i) the verb was meta, or
					!  (ii) we've only had the implicit take before the "real"
					!  action to follow.
					if (notheld_mode==1) { NoteObjectAcquisitions(); continue; }
					if (meta) continue;
					if (~~deadflag) InformLibrary.end_turn_sequence();
					else if (START_MOVE ~= 1) turns++;
				}
		]
		!---------------------------------------
		,	play_loop_pre_parse[;
	#ifdef EnglishNaturalLanguage;
				PronounOldEnglish();
				old_itobj = PronounValue('it');
				old_himobj = PronounValue('him');
				old_herobj = PronounValue('her');
	#endif;
				if (score ~= last_score)
				{   
					if (notify_mode==1) NotifyTheScore(); 
					last_score=score; 
				}
				inputobjs-->0 = 0; inputobjs-->1 = 0;
				inputobjs-->2 = 0; inputobjs-->3 = 0; meta=false;
				!  The Parser writes its results into inputobjs and meta,
				!  a flag indicating a "meta-verb".  This can only be set for
				!  commands by the player, not for orders to others.
				InformParser.parse_input(inputobjs);
			]
		!---------------------------------------
		,	play_loop_post_parse[i;
				action=inputobjs-->0;
				!  Reverse "give fred biscuit" into "give biscuit to fred"
				if (action==##GiveR or ##ShowR)
				{   
					i=inputobjs-->2; 
					inputobjs-->2=inputobjs-->3; 
					inputobjs-->3=i;
					if (action==##GiveR) 
						action=##Give; 
					else 
						action=##Show;
				}
				!  Convert "P, tell me about X" to "ask P about X"
				if (action==##Tell && inputobjs-->2==player && actor~=player){   
					inputobjs-->2=actor; actor=player; action=##Ask;
				}
				!  Convert "ask P for X" to "P, give X to me"
				if (action==##AskFor && inputobjs-->2~=player && actor==player){   
					actor=inputobjs-->2; 
					inputobjs-->2=inputobjs-->3;
					inputobjs-->3=player; action=##Give;
				}
				!  For old, obsolete code: special_word contains the topic word
				!  in conversation
				if (action==##Ask or ##Tell or ##Answer)
					special_word = special_number1;

				multiflag=false; 
				onotheld_mode=notheld_mode; 
				notheld_mode=false;
			]
		!---------------------------------------
		,	play_loop_begin_action[i;		
				inp1 = 0; inp2 = 0; i=inputobjs-->1;
				if (i>=1) inp1=inputobjs-->2;
				if (i>=2) inp2=inputobjs-->3;
		    
				!  inp1 and inp2 hold: object numbers, or 0 for "multiple object",
				!  or 1 for "a number or dictionary address"
		    
				if (inp1 == 1) 
					noun = special_number1; 
				else 
					noun = inp1;
				if (inp2 == 1){   
					if (inp1 == 1) 
						second = special_number2;
					else 
						second = special_number1;
				} 
				else second = inp2;
				return i;
			]
		!---------------------------------------
		,	play_loop_handle_multiple[j i k l;
				!  (b) warn the player if it has been cut short because too long;
				!  (c) generate a sequence of actions from the list
				!      (stopping in the event of death or movement away).
				if (toomany_flag)
				{   toomany_flag = false; L__M(##Miscellany,1); }
				i=location;
				for (k=1:k<=j:k++)
				{   if (deadflag) break;
					if (location ~= i)
					{   L__M(##Miscellany, 51);
						break;
					}
					l = multiple_object-->k;
					PronounNotice(l);
					print (name) l, ": ";
					if (inp1 == 0)
					{   inp1 = l; InformLibrary.begin_action(action, l, second, 0); inp1 = 0;
					}
					else
					{   inp2 = l; InformLibrary.begin_action(action, noun, l, 0); inp2 = 0;
					}
				}
			]
		!---------------------------------------
		,	play_handle_death[;
				print "^^    ";
				#Ifdef TARGET_ZCODE;
				#IfV5; style bold; #Endif; ! V5
				#Ifnot; ! TARGET_GLULX
				glk($0086, 5); ! set alert style
				#Endif; ! TARGET_
				print "***";
				if (deadflag == 1) L__M(##Miscellany, 3);
				if (deadflag == 2) L__M(##Miscellany, 4);
				if (deadflag > 2)  {
					print " ";
					DeathMessage();
					print " ";
				}
				print "***";
				#Ifdef TARGET_ZCODE;
				#IfV5; style roman; #Endif; ! V5
				#Ifnot; ! TARGET_GLULX
				glk($0086, 0); ! set normal style
				#Endif; ! TARGET_

	#Ifdef NO_SCORE;
				print "^^";
	#Ifnot;
				print "^^^";
	#Endif;
				ScoreSub();
				DisplayStatus();
				AfterGameOver();
			]
	;
!======================================================================================
   #endif; #ifnot; #ifndef ORInformLibrary_DONEGRAMMAR; constant ORInformLibrary_DONEGRAMMAR; #ifdef ORInformLibrary_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================