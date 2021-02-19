!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! 2004.06.08 OR_Library_Include [Z, GLULX]
!!!	This file serves as a common entry point for all ORLibrary entries.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 
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
! The latest version of the OnyxRing Library for Inform can be found in its 
! entirety at:
!
!		http://www.onyxring.com/downloads/orlib/orlib_complete.zip
!		
! Alternativly, the basic ORLibrary framework and each of the entries can also be 
! downloaded seperately from:
!
!		http://www.OnyxRing.com/orlibrary.aspx
!	
! For compilation instructions, see the ORLibrary Developer's Guide on-line:
! 
!		http://www.onyxring.com/ORLibDocToc.aspx
!
! Or in the complete PDF format:
!
!		http://www.onyxring.com/downloads/orldg.pdf 
!
!--------------------------------------------------------------------------------------
system_file;
default ORLibVersion "1.3C";
default ORLibDateStamp "2004.06.08";
!---Verify we are using the 630 compiler or later
#ifndef VN_1630; message fatalerror "Beginning with version ORLib rev 1.2, the ORLibrary requires the Inform compiler version 3.0 or later. "; #endif;
!---Detect missing or extra inclusions and throw errors if needed
#ifndef LIBRARY_PARSER; #ifdef _PRE_PARSER;	message fatalerror "OR_Library_Include has been included multiple times prior to PARSER."; #ifnot; 
	Constant ORLibraryInclude;
	message "Compiling with the  O R L i b r a r y  framework ";
	#ifndef TARGET_ZCODE; message "[GLULX Native mode]"; #ifnot; message "[Z-Code mode]"; #endif;
	#ifndef ORBanner_DONEREPLACE; default USE_ORBanner 0; #endif; 	
	constant _PRE_PARSER; 
	message "Preparing to process extensions REPLACE section...";#endif;
#ifnot; #ifndef LIBRARY_VERBLIB; #ifdef _TWEEN_PARSER_VERBLIB;	message fatalerror "OR_Library_Include has been included multiple times between PARSER and VERBLIB."; #ifnot; 
	#ifndef _PRE_PARSER; message fatalerror "OR_Library_Include has not been included prior to PARSER."; #endif;
	constant _TWEEN_PARSER_VERBLIB; 
	message "Preparing to process extensions MESSAGE section...";#endif;
#ifnot; #ifndef LIBRARY_GRAMMAR; #ifdef _TWEEN_VERBLIB_GRAMMAR;	message fatalerror "OR_Library_Include has been included multiple times between VERBLIB and GRAMMAR."; #ifnot; 
	#ifndef _TWEEN_PARSER_VERBLIB; message fatalerror "OR_Library_Include has not been included between PARSER and VERBLIB."; #endif;
	constant _TWEEN_VERBLIB_GRAMMAR; 
	message "Preparing to process extensions CODE section...";#endif;
#ifnot; #ifdef _POST_GRAMMAR;	message fatalerror "OR_Library_Include has been included multiple times following GRAMMAR."; #ifnot; 
	#ifndef _TWEEN_VERBLIB_GRAMMAR; message fatalerror "OR_Library_Include has not been included between VERBLIB and GRAMMAR."; #endif;
	constant _POST_GRAMMAR; 
	message "Preparing to process extensions GRAMMAR section..."; #endif;
#endif; #endif; #endif; 
!--------------------------------------------------------------------------------------
!!! Includes begin here... Add additional library entries to the bottom of this list
!  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
!--JustEnoughGlulx initializes itself based upon the existence of constants that may be defined in modules, so it is best to include it first...
#ifdef USE_ORJustEnoughGlulx; #include "ORJustEnoughGlulx"; #endif; 
!  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
#ifdef USE_ORAction; #include "ORAction"; #endif;
#ifdef USE_ORActionMenu; #include "ORActionMenu"; #endif;
#ifdef USE_ORActionQueue; #include "ORActionQueue"; #endif;
#ifdef USE_ORAdjective; #include "ORAdjective"; #endif;
#ifdef USE_ORBanner; #include "ORBanner";	#endif;
#ifdef USE_ORBetterChoice; #include "ORBetterChoice"; #endif;
#ifdef USE_ORBracketParserMsgs; #include "ORBracketParserMsgs"; #endif;
#ifdef USE_ORCantGoOdd; #include "ORCantGoOdd"; #endif;
#ifdef USE_ORCenter; #include "ORCenter"; #endif;
#ifdef USE_ORConverseMenu; #include "ORConverseMenu"; #endif;
#ifdef USE_ORDispensor; #include "ORDispensor"; #endif;
#ifdef USE_ORDistinctMeSelf; #include "ORDistinctMeSelf"; #endif;
#ifdef USE_ORDistinctRead; #include "ORDistinctRead"; #endif;
#ifdef USE_ORDoor; #include "ORDoor"; #endif;
#ifdef USE_ORDynaMap; #include "ORDynaMap"; #endif;
#ifdef USE_ORDynaString; #include "ORDynaString"; #endif;
#ifdef USE_OREntryPoints; #include "OREntryPoints"; #endif;
#ifdef USE_ORExamWithContents; #include "ORExamWithContents"; #endif;
#ifdef USE_ORExits; #include "ORExits"; #endif;
#ifdef USE_ORFirstThoughts; #include "ORFirstThoughts"; #endif;
#ifdef USE_ORGibberish; #include "ORGibberish"; #endif;
#ifdef USE_ORInfGlk; #include "ORInfGlk"; #endif;
#ifdef USE_ORInformLibrary; #include "ORInformLibrary"; #endif;
#ifdef USE_ORInsultCompli_KT; #include "ORInsultCompli_KT"; #endif;
#ifdef USE_ORKeyboard; #include "ORKeyboard"; #endif;
#ifdef USE_ORKeyboardPrimitive; #include "ORKeyboardPrimitive"; #endif;
#ifdef USE_ORKnowledgeScript; #include "ORKnowledgeScript"; #endif;
#ifdef USE_ORKnowledgeTopic; #include "ORKnowledgeTopic"; #endif;
#ifdef USE_ORKnowledgeWeb; #include "ORKnowledgeWeb"; #endif;
#ifdef USE_ORListManager; #include "ORListManager"; #endif;
#ifdef USE_ORLiquid; #include "ORLiquid"; #endif;
#ifdef USE_ORLookRoom; #include "ORLookRoom"; #endif;
#ifdef USE_ORMagic; #include "ORMagic"; #endif;
#ifdef USE_ORMenu; #include "ORMenu"; #endif;
#ifdef USE_ORMiniMenu; #include "ORMiniMenu"; #endif;
#ifdef USE_ORNameable; #include "ORNameable"; #endif;
#ifdef USE_ORNPC; #include "ORNPC"; #endif;
#ifdef USE_ORNPC_AskTellLearn; #include "ORNPC_AskTellLearn"; #endif;
#ifdef USE_ORNPC_Converse; #include "ORNPC_Converse"; #endif;
#ifdef USE_ORNPC_DoVerb; #include "ORNPC_DoVerb"; #endif;
#ifdef USE_ORNPC_GoalDriven; #include "ORNPC_GoalDriven"; #endif;
#ifdef USE_ORNPC_Interact; #include "ORNPC_Interact"; #endif;
#ifdef USE_ORNPC_MapKnown; #include "ORNPC_MapKnown"; #endif;
#ifdef USE_ORNPC_Moods; #include "ORNPC_Moods"; #endif;
#ifdef USE_ORNPC_Movement; #include "ORNPC_Movement"; #endif;
#ifdef USE_ORNPCVerb; #include "ORNPCVerb"; #endif;
#ifdef USE_ORNumberedContainer; #include "ORNumberedContainer"; #endif;
#ifdef USE_OROptionList; #include "OROptionList";	#endif;
#ifdef USE_OROptionListMenu; #include "OROptionListMenu"; #endif;
#ifdef USE_ORParser; #include "ORParser"; #endif;
#ifdef USE_ORParseToken; #include "ORParseToken"; #endif;
#ifdef USE_ORPathMaker; #include "ORPathMaker";	 #endif;
#ifdef USE_ORPrefixSuffix; #include "ORPrefixSuffix"; #endif;
#ifdef USE_ORPronoun; #include "ORPronoun"; #endif;
#ifdef USE_ORProp; #include "ORProp"; #endif;
#ifdef USE_ORRecogName; #include "ORRecogName"; #endif;
#ifdef USE_ORRedefineObject; #include "ORRedefineObject"; #endif;
#ifdef USE_ORReferByContents; #include "ORReferByContents"; #endif;
#ifdef USE_ORReverseDirection; #include "ORReverseDirection"; #endif;
#ifdef USE_ORReview; #include "ORReview"; #endif;
#ifdef USE_ORRockDispensor; #include "ORRockDispensor"; #endif;
#ifdef USE_ORRoutinesList; #include "ORRoutinesList"; #endif;
#ifdef USE_ORSeveral; #include "ORSeveral"; #endif;
#ifdef USE_ORSpecializedExit; #include "ORSpecializedExit"; #endif;
#ifdef USE_ORStyleHint; #include "ORStyleHint"; #endif;
#ifdef USE_ORSupporterContainer; #include "ORSupporterContainer"; #endif;
#ifdef USE_ORSuppressTakeAll; #include "ORSuppressTakeAll"; #endif;
#ifdef USE_ORTextFormatting; #include "ORTextFormatting"; #endif;
#ifdef USE_ORTransition; #include "ORTransition"; #endif;
#ifdef USE_ORUniqueMultiMessage; #include "ORUniqueMultiMessage"; #endif;
#ifdef USE_ORWAE_Formatting; #include "ORWAE_Formatting"; #endif;
#ifdef USE_ORWriteAfterEntry; #include "ORWriteAfterEntry"; #endif;

#ifdef USE_PersonalReuse; #include ">Reuse.h"; #endif;		!---personal library additions (user-defined)