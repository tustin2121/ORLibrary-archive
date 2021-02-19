!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.28 ORKeyboard [Z, GLULX]
! A rearangement of the Keyboard routine.  No actual code has been altered (at
! least not in any significant way) but the routine has been broken into smaller 
! componants which can be overriden by other modules.
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- - 
! License: 
! Matches that of Inform's standard library (see included header, below).
!--------------------------------------------------------------------------------------
! AutoDep:	none
!--------------------------------------------------------------------------------------
! ORLibDev: Note, that this module is not useful as a library extension alone.  By 
!     itself it provides no new functionality for the IF developer. Instead it provides
!     a basis for other module to be built upon and will primarily be useful to those 
!     who are developing ORLibrary entries of their own.
!--------------------------------------------------------------------------------------
! Although arranged differently, this code is nevertheless the same Keyboard 
! routine that comes from Graham Nelson's standard library (actually Andrew Plotkin's
! Biplatform library which is a modified version of Graham Nelson's standard library).  
! In any case, it falls under the same copyright as the code upon which it was based 
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
! This module replaces the large and bulky original version of Keyboard with a smaller
! version that does little more than make calls to routines that are compartmentalized 
! versions of the code originally making up the Keyboard routine. These smaller
! routines can more selectively be replaced by other modules.
!--------------------------------------------------------------------------------------
! To register this module with your library, add the line:
!
!		#ifdef USE_ORKeyboard; #include "ORKeyboard"; #endif;
!	
!	to the library header file (OR_Library_Include).  To use in a game, add the line:
!
!		Constant USE_ORKeyboard; 
!
!	to the game file;
!--------------------------------------------------------------------------------------
! Revision History
! 2002.04.05 Initial Creation
! 2004.01.11 Made modifications to match new stdlib 6.11 changes.
! 2004.01.18 Implemented missing changes from 6.11 changes.
! 2004.02.28 Minor change to KeyboardGetInput, to return the return value of 
!				KeyboardPrimitive routine.  This enables Keyboard primitive 
!				overloads to cancel processing.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORKeyboard...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORKeyboard forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORKeyboard has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORKeyboard_DONEREPLACE; constant ORKeyboard_DONEREPLACE; #ifdef ORKeyboard_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
replace keyboard;
!======================================================================================
   #ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORKeyboard_DONEMESSAGE; constant ORKeyboard_DONEMESSAGE;  #ifdef ORKeyboard_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORKeyboard requires the OREnglish file.";#endif; 
	[ Keyboard  a_buffer a_table  nw;	
		DisplayStatus();
		.FreshInput;
		
		if(KeyboardGetInput(a_buffer, a_table)==-1) jump FreshInput;
		if((nw=KeyboardPrepInput(a_buffer, a_table))==-1) jump FreshInput;
		if (a_table-->1 == OOPS1__WD or OOPS2__WD or OOPS3__WD) {
			if((nw=KeyboardDoOops(a_buffer, a_table, nw))==-1) jump FreshInput;
			return nw;
		}
		if(KeyboardDoComment(a_buffer, a_table,nw)==-1) jump FreshInput;
		if(KeyboardDoUndo(a_buffer, a_table,nw)==-1) jump FreshInput;
		return nw;
	];

	system_file; !---methods from here down can be replaced
	#ifndef INPUT_BUFFER_LEN; Constant INPUT_BUFFER_LEN = 120;     #endif; ! Length of buffer array 
	[KeyboardGetInput a_buffer a_table i; !--return -1 to get freshinput
		!  Save the start of the buffer, in case "oops" needs to restore it
		!  to the previous time's buffer

			for (i=0:i<64:i++) oops_workspace->i = a_buffer->i;

		!  In case of an array entry corruption that shouldn't happen, but would be
		!  disastrous if it did:

		#Ifdef TARGET_ZCODE;
		a_buffer->0 = INPUT_BUFFER_LEN;
		a_table->0 = 15;  ! Allow to split input into this many words
		#Endif; ! TARGET_

		!  Print the prompt, and read in the words and dictionary addresses
	    L__M(##Prompt);
		AfterPrompt();

		#IfV5;
		DrawStatusLine();
		#Endif; ! V5
		return KeyboardPrimitive(a_buffer, a_table);
	];
	[KeyboardPrepInput a_buffer a_table nw; !--return -1 to get freshinput	
		a_buffer=a_buffer;
		#Ifdef TARGET_ZCODE;
		nw = a_table->1;
		#Ifnot; ! TARGET_GLULX
		nw = a_table-->0;
		#Endif; ! TARGET_
		!  If the line was blank, get a fresh line
		if (nw == 0){ 
			L__M(##Miscellany,10); 
			return -1;
		}
		rtrue;
	];

	[KeyboardDoComment a_buffer a_table;!--return -1 to get freshinput
		a_table=a_table; !--suppress warning
		if (a_buffer->WORDSIZE == COMMENT_CHARACTER) {
			#Ifdef TARGET_ZCODE;
			if ((HDR_GAMEFLAGS-->0) & 1 || xcommsdir)
											L__M(##Miscellany, 54);
			else                               L__M(##Miscellany, 55);
			#Ifnot; ! TARGET_GLULX
			if (gg_scriptstr || gg_commandstr) L__M(##Miscellany, 54);
			else                               L__M(##Miscellany, 55);
			#Endif; ! TARGET_

			return -1;
		}
		rtrue;
	];
	[KeyboardDoUndo a_buffer a_table nw i w;!--return -1 to get freshinput
	a_buffer=a_buffer;
	#IFV5;
	!  Undo handling

		w=a_table-->1;
		if ((w == UNDO1__WD or UNDO2__WD or UNDO3__WD) && (nw==1))
		{   if (turns==START_MOVE){   
				L__M(##Miscellany,11); 
				return -1;
			}
			if (undo_flag==0){   
				L__M(##Miscellany,6); 
				return -1;
			}
			if (undo_flag==1) jump UndoFailed;
        #Ifdef TARGET_ZCODE;
        @restore_undo i;
        #Ifnot; ! TARGET_GLULX
        @restoreundo i;
        i = (~~i);
        #Endif; ! TARGET_
			if (i==0){   
				.UndoFailed;
				L__M(##Miscellany,7);
			}
			return -1;
		}
	#ifndef TARGET_GLULX;
		@save_undo i;
	#ifnot; ! TARGET_GLULX
		@saveundo i;
		if (i == -1) {
			GGRecoverObjects();
			i = 2;
		}
		else {
			i = (~~i);
		}
	#endif; ! TARGET_
		just_undone=0;
		undo_flag=2;
		if (i==-1) undo_flag=0;
		if (i==0) undo_flag=1;
		if (i==2)
		{
			RestoreColours();
			#Ifdef TARGET_ZCODE;
			style bold;
			#Ifnot; ! TARGET_GLULX
			glk($0086, 4); ! set subheader style
			#Endif; ! TARGET_
			print (name) location, "^";
			#Ifdef TARGET_ZCODE;
			style roman;
			#Ifnot; ! TARGET_GLULX
			glk($0086, 0); ! set normal style
			#Endif; ! TARGET_
			L__M(##Miscellany, 13);
			just_undone = 1;
			return -1;
		}
	#ENDIF;
	];
	[KeyboardDoOops a_buffer a_table nw i x1 x2 w w2;!--return false to get freshinput
		if (oops_from == 0){   
			L__M(##Miscellany,14); 
			return -1;
		}
		if (nw == 1){   
			L__M(##Miscellany,15); 
			return -1;
		}
		if (nw > 2){   
			L__M(##Miscellany,16); 
			return -1;
		}

	!  So now we know: there was a previous mistake, and the player has
	!  attempted to correct a single word of it.

		for (i=0:i<INPUT_BUFFER_LEN:i++) buffer2->i = a_buffer->i;
	#ifndef TARGET_GLULX;
		x1 = a_table->9; ! Start of word following "oops"
		x2 = a_table->8; ! Length of word following "oops"
	#ifnot; ! TARGET_GLULX
		x1 = a_table-->6; ! Start of word following "oops"
		x2 = a_table-->5; ! Length of word following "oops"
	#endif; ! TARGET_

	!  Repair the buffer to the text that was in it before the "oops"
	!  was typed:

		for (i=0:i<64:i++) a_buffer->i = oops_workspace->i;
		Tokenise__(a_buffer,a_table);

	!  Work out the position in the buffer of the word to be corrected:

	#ifndef TARGET_GLULX;
		w = a_table->(4*oops_from + 1); ! Start of word to go
		w2 = a_table->(4*oops_from);    ! Length of word to go
	#ifnot; ! TARGET_GLULX
		w = a_table-->(3*oops_from);      ! Start of word to go
		w2 = a_table-->(3*oops_from - 1); ! Length of word to go
	#endif; ! TARGET_

	!  Write spaces over the word to be corrected:

		for (i=0:i<w2:i++) a_buffer->(i+w) = ' ';

		if (w2 < x2)
		{   ! If the replacement is longer than the original, move up...

			for (i=INPUT_BUFFER_LEN-1:i>=w+x2:i--)
				a_buffer->i = a_buffer->(i-x2+w2);

			! ...increasing buffer size accordingly.

	#ifndef TARGET_GLULX;
			a_buffer->1 = (a_buffer->1) + (x2-w2);
	#ifnot; ! TARGET_GLULX
			a_buffer-->0 = (a_buffer-->0) + (x2-w2);
	#endif; ! TARGET_

		}

	!  Write the correction in:

		for (i=0:i<x2:i++) a_buffer->(i+w) = buffer2->(i+x1);

		Tokenise__(a_buffer,a_table);
	#ifndef TARGET_GLULX;
		nw=a_table->1;
	#ifnot; ! TARGET_GLULX
		nw=a_table-->0;
	#endif; ! TARGET_

		return nw;

	];

!======================================================================================
   #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORKeyboard_DONECODE;constant ORKeyboard_DONECODE; #ifdef ORKeyboard_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================

!======================================================================================
   #endif; #ifnot; #ifndef ORKeyboard_DONEGRAMMAR; constant ORKeyboard_DONEGRAMMAR; #ifdef ORKeyboard_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================