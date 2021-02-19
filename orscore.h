!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORScore	[Z, GLULX]
! Extends the standard library's task score system, creating an auto-initialized
! repository for scores, which can be normal or randomly assigned (kind of like the
! scoring in Leather Goddesses of Phobos).
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!--------------------------------------------------------------------------------------
! Created by Mikael Segercrantz
! AutoDep:	None.
!--------------------------------------------------------------------------------------
! This is a simple extension to the standard library's task score system.  It replaces
! a few functions, adds some constants, properties and attributes.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORScore";					!Constant USE_ORScore; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORScore; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2003.07.31	Initial Creation
! 2003.08.01	Added Library Messages
! 2004.01.19	[Jim Fisher] Modified to translate library messages into standard 
!				library's new format; also replaced objectinitialise code with new
!				standard library functionality.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORScore...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORScore forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORScore has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORScore_DONEREPLACE; constant ORScore_DONEREPLACE; #ifdef ORScore_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	Replace Achieved;
	Replace FullscoreSub;
	Replace NoteObjectAcquisitions;
	Replace ScoreArrival;
	Replace ScoreSub;

	Property task_score;
	Property min_score;
	Property max_score;
	Property task;

	Attribute item;
	Attribute room;

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORScore_DONEMESSAGE; constant ORScore_DONEMESSAGE;  #ifdef ORScore_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORScore requires the OREnglish file.";#endif; 
	Global things_scored = 0;
	Global places_scored = 0;
	Global score_maximum = 0;

	Constant TASKS_PROVIDED;
	Ifndef NUMBER_TASKS;
		Constant NUMBER_TASKS = 1;
	Endif;

	Array task_minimum --> NUMBER_TASKS;
	Array task_maximum --> NUMBER_TASKS;
	Array task_scores --> NUMBER_TASKS;

	Ifndef MIN_OBJECT_SCORE;
		Constant MIN_OBJECT_SCORE = OBJECT_SCORE;
		Constant MAX_OBJECT_SCORE = OBJECT_SCORE;
	Ifnot;
		Ifndef MAX_OBJECT_SCORE;
			Constant MIN_OBJECT_SCORE = OBJECT_SCORE;
			Constant MAX_OBJECT_SCORE = OBJECT_SCORE;
		Endif;
	Endif;
	Ifndef MIN_ROOM_SCORE;
		Constant MIN_ROOM_SCORE = ROOM_SCORE;
		Constant MAX_ROOM_SCORE = ROOM_SCORE;
	Ifnot;
		Ifndef MAX_ROOM_SCORE;
			Constant MIN_ROOM_SCORE = ROOM_SCORE;
			Constant MAX_ROOM_SCORE = ROOM_SCORE;
		Endif;
	Endif;


	Class ORScoreHeader
		with short_name
			 [ taskn obj;
				objectloop(obj ofclass ORScore)
					if (obj.task == taskn)
						return obj.short_name();
				return "Unknown task";
			 ]
		,	 score_initialise
			 [ obj;
				for (obj = 0: obj < NUMBER_TASKS: obj++)
				{
					task_minimum-->obj = 0;
					task_maximum-->obj = 0;
				}

				objectloop (obj ofclass ORScore)
					if (obj.task >= 0 && obj.task < NUMBER_TASKS)
					{
						if (obj.task_score == 0)
						{
							if (obj.min_score > obj.max_score)
								obj.min_score = obj.max_score;
						}
						else
						{
							obj.min_score = obj.task_score;
							obj.max_score = obj.task_score;
						}
						task_minimum-->(obj.task) = obj.min_score;
						task_maximum-->(obj.task) = obj.max_score;
						score_maximum = score_maximum + obj.max_score;
					}
				
				objectloop (obj has scored && obj has room)
				{
					if (obj.task_score ~= 0)
					{
						obj.min_score = obj.task_score;
						obj.max_score = obj.task_score;
					}
					if (obj.min_score == 0 && obj.max_score == 0)
					{
						obj.min_score = MIN_ROOM_SCORE;
						obj.max_score = MAX_ROOM_SCORE;
					}
					if (obj.min_score > obj.max_score)
						obj.min_score = obj.max_score;
					score_maximum = score_maximum + obj.max_score;
				}

				objectloop (obj has scored && obj has item)
				{
					if (obj.task_score ~= 0)
					{
						obj.min_score = obj.task_score;
						obj.max_score = obj.task_score;
					}
					if (obj.min_score == 0 && obj.max_score == 0)
					{
						obj.min_score = MIN_OBJECT_SCORE;
						obj.max_score = MAX_OBJECT_SCORE;
					}
					if (obj.min_score > obj.max_score)
						obj.min_score = obj.max_score;
					score_maximum = score_maximum + obj.max_score;
				}
			 ]
		,    finalize
			 [ obj;
				self.done_finalizing = 1;

				objectloop (obj ofclass ORScore)
					if (obj.task >= 0 && obj.task < NUMBER_TASKS)
						if (task_done->(obj.task) == 0)
						{
							task_minimum-->(obj.task) = 0;
							task_maximum-->(obj.task) = 0;
						}
				
				objectloop (obj has scored && (obj has room or item))
				{
					obj.min_score = 0;
					obj.max_score = 0;
				}
			 ]
		,	 scores_finalized
			 [;
				return self.done_finalizing;
			 ]
		,	 done_finalizing 0
	;

	Class ORScore
		with task_score 0
		,	 min_score 0
		,	 max_score 0
		,	 task -1
		,	 short_name
			 [;
				print "for ", (object) self;
				if (self.task >= 0 || self.task < NUMBER_TASKS)
					if ((task_minimum-->self.task == 0) && (task_maximum-->self.task == 0))
						print " (UNSCORED)";
				print "^";
			 ]
	;

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORScore_DONECODE;constant ORScore_DONECODE; #ifdef ORScore_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	object _orscore_init LibrarExtensions 
		with ext_messages
			 [;
#Ifdef DEBUG;
				Achieve:
					switch (lm_n)
					{
						1:	bkt_o();
							print "Task number may be between 1 and ", NUMBER_TASKS, ".";
							bkt_c();
						2:	bkt_o();
							print "That task has already been achieved.";
							bkt_c();
						3:	bkt_o();
							print "Achieved.";
							bkt_c();
					}
#Endif;
				FullScore:
					if (lm_n == 4)
					{
						PANum(score);
						print "total (out of ", score_maximum; ")^";
					}
				Score:
					if (deadflag)
						print "In that game ", (ig)fst("I", "you"), " scored ";
					else
						print (ig)fst("I", "You"), " ", (ig)ppf("had", "have"), " so far scored ";
					
					print score, " out of a possible ", score_maximum, ", in ", turns, " turn";
					if (turns ~= 1)
						print "s";
					rtrue;
			 ]
			 , ext_initialise[o;
				objectloop(o ofclass ORScoreHeader){
					o.score_initialise();
				}
			]
	;

	ORScoreHeader ORScoreH;

	[ Achieved num sc;
		if (task_done->num == 0)
		{
			task_done->num = 1;
			if (ORScoreH.scores_finalized() == 0)
			{
				sc = random((1 + task_maximum-->num) - task_minimum-->num) + ((task_minimum-->num) - 1);
				score = score + sc;
				score_maximum = score_maximum - ((task_maximum-->num) - sc);
				task_scores-->num = sc;
			}
		}
	];

	[ NoteObjectAcquisitions obj sc;
		objectloop (obj in player && obj hasnt moved)
		{
			give obj moved;
			if (i has scored)
			{
				things_scored++;
				if (ORScoreH.scores_finalized() == 0)
				{
					sc = random((1 + obj.max_score) - obj.min_score) + (obj.min_score - 1);
					things_score = things_score + sc;
					score = score + sc;
					score_maximum = score_maximum - (obj.max_score - sc);
				}
			}
		}
	];

	[ ScoreArrival sc;
		if (location hasnt visited)
		{
			give location visited;
			if (location has scored)
			{
				places_scored++;
				if (ORScoreH.scores_finalized() == 0)
				{
					sc = random((1 + location.max_score) - location.min_score) + (location.min_score - 1);
					places_score = places_score + sc;
					score = score + sc;
					score_maximum = score_maximum - (location.max_score - sc);
				}
			}
		}
	];

!======================================================================================
#endif; #ifnot; #ifndef ORScore_DONEGRAMMAR; constant ORScore_DONEGRAMMAR; #ifdef ORScore_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================
#ifdef DEBUG;
	[ AchieveSub num;
		num = noun;
		if (num < 1 || num > NUMBER_TASKS)
		{
			L__M(##Achieve, 1);
			return;
		}

		num = num - 1;
		if (task_done->num == 1)
		{
			L__M(##Achieve, 2);
			return;
		}

		Achieved(num);
		L__M(##Achieve, 3);
	];
#endif;

	[ FullScoreSub i;
		ScoreSub();

		if (score == 0 || TASKS_PROVIDED == 1)
			rfalse;
		
		new_line;
		L__M(##FullScore, 1);

		for (i = 0: i < NUMBER_TASKS: i++)
			if (task_done->i == 1)
			{
				font off;
				PANum(task_scores-->i);
				font on;
				ORScoreH.short_name(i);
			}
		
		if (things_scored ~= 0)
		{
			font off;
			PANum(things_score);
			font on;
			L__M(##FullScore, 2);
		}

		if (places_scored ~= 0)
		{
			font off;
			PANum(places_score);
			font on;
			L__M(##FullScore, 3);
		}

		new_line;
		L__M(##FullScore, 4);
	];

	[ ScoreSub;
		L__M(##Score);
		PrintRank();
	];

#ifdef DEBUG;
Verb meta 'achieve'
	* number						-> Achieve
	* 'task' number					-> Achieve
	* number 'task'					-> Achieve;

Verb meta 'task'
	* number						-> Achieve
	* 'achieve' number				-> Achieve
	* number 'achieve'				-> Achieve;
#endif;
!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================