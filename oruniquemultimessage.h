!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORUniqueMultiMessage	[Z, GLULX]
! Consolidates identical messages for actions that accept multiple objects. For example:
!          >TAKE ALL MARBLES FROM BOWL
!          6 marbles: Removed.
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
! AutoDep :	ORDynaString
!			ORRedefineObject
!			ORInformLibrary
!--------------------------------------------------------------------------------------
! Several identical messages are often generated when a player performs an action that 
! affects multiple objects.  Consider the following normal transcript:
!
!	>TAKE ALL MARBLES FROM BOWL
!	marble: Removed.
!	marble: Removed.
!	marble: Removed.
!	marble: Removed.
!	marble: Removed.
!	marble: Removed.
!
! The ORUniqueMultiMessage module will analyze the responses made by each of the 
! commands and attempt to consolidate them (provided they occur sequentially).  
! By leveraging this entry, the above example would appear as:
!
!	>TAKE ALL MARBLES FROM BOWL
!	6 marbles: Removed.
!
! No additional code need be written to make this functionality occur.  Simply include
! this module and consolidated messages will occur.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#include "ORUniqueMultiMessage";					!Constant USE_ORUniqueMultiMessage; 
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORUniqueMultiMessage; to the game file;
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.08	Initial Creation
! 2002.02.13	Added Glulx support and modified to be derived from ORRedefineObject.
! 2002.02.26	Fit to new template.
! 2002.12.22	Added an addtional buffer to keep other modules from stomping over the 
!				current buffer.
! 2003.01.02	Adjusted extra carriage return in formating.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORUniqueMultiMessage...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORUniqueMultiMessage forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
	#ifndef ORLibraryInclude; message fatalerror "ORUniqueMultiMessage has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
	#ifndef ORDynaString_DONEREPLACE; default USE_ORDynaString 0; message "          [ORUniqueMultiMessage forcing inclusion of ORDynaString]"; #include "ORDynaString"; #endif; 
	#ifndef ORRedefineObject_DONEREPLACE; default USE_ORRedefineObject 0; message "          [ORUniqueMultiMessage forcing inclusion of ORRedefineObject]"; #include "ORRedefineObject"; #endif; 
	#ifndef ORInformLibrary_DONEREPLACE; default USE_ORInformLibrary 0; message "          [ORUniqueMultiMessage forcing inclusion of ORInformLibrary]"; #include "ORInformLibrary"; #endif; 
!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORUniqueMultiMessage_DONEREPLACE; constant ORUniqueMultiMessage_DONEREPLACE; #ifdef ORUniqueMultiMessage_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================

!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORUniqueMultiMessage_DONEMESSAGE; constant ORUniqueMultiMessage_DONEMESSAGE;  #ifdef ORUniqueMultiMessage_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORUniqueMultiMessage requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORUniqueMultiMessage_DONECODE;constant ORUniqueMultiMessage_DONECODE; #ifdef ORUniqueMultiMessage_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	array __uniqmultarr1 -> 500; 
	array __uniqmultarr2 -> 500; 
	ORDynaString __uniqmultmsg;
	
	ORRedefineObject ORUniqueMultiMessage
		with	dest_object __ORInformLibrary
		,	play_loop_handle_multiple[j i k l previous_obj previous_buf count t severe_error;
				previous_obj=0;
				if (toomany_flag){   
					toomany_flag = false; 
					L__M(##Miscellany,1); 
				}
				i=location;
				
				__uniqmultarr1->0=0;
				__uniqmultarr1->1=0;
				__uniqmultarr2->0=0;
				__uniqmultarr2->1=0;

				__uniqmultmsg.buf=__uniqmultarr1;
				previous_buf=__uniqmultarr2;
				
				for (k=1:k<=j:k++){   
					if (deadflag) break;
					if (location ~= i){   
						severe_error=true;
						break;
					}
					l = multiple_object-->k;
					PronounNotice(l);
					__uniqmultmsg.capture();
					if (inp1 == 0){   
						inp1 = l; 
						InformLibrary.begin_action(action, l, second, 0); 
						inp1 = 0;
					}
					else{   
						inp2 = l; 
						InformLibrary.begin_action(action, noun, l, 0); 
						inp2 = 0;
					}
					__uniqmultmsg.release();
					if(identical(previous_obj,l)==false || __uniqmultmsg.matches(previous_buf)==false){
						if(previous_obj~=0) PrintMultiAction(previous_obj,previous_buf,count);
						t=__uniqmultmsg.buf;
						__uniqmultmsg.buf=previous_buf;
						previous_buf=t;
						previous_obj=l;
						count=1;
					}else{
						count++;
					}
				}
				PrintMultiAction(previous_obj,previous_buf,count);
				if(severe_error==true) L__M(##Miscellany, 51);
			]
	;
	[PrintMultiAction obj buf count;
		if(count>1)print count," ";
		if(count>1 && obj provides plural)
			print (string)obj.plural;
		else	
			print (name)obj;
		print " : ";
		print (arraystring) buf; !--,"^";
	];
!======================================================================================
#endif; #ifnot; #ifndef ORUniqueMultiMessage_DONEGRAMMAR; constant ORUniqueMultiMessage_DONEGRAMMAR; #ifdef ORUniqueMultiMessage_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================