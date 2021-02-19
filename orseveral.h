!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORSeveral [Z, GLULX]
!	This module allows multiple objects of the same class to be described with an adjective 
!	rather than an exact number.  For instance, It may be preferable that the description 
!	of 22 gold coins read as "several gold coins."
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- - 
! License: 
! Matches that of Inform's standard library because the majority of ths module is a 
! simply a revised version of WriteListR (see parserm for specifics).
!--------------------------------------------------------------------------------------
! AutoDep :	none
!--------------------------------------------------------------------------------------
!	Two properties and one attribute have been introduced to implement this behavior:
!		
!		plural_many			Set this property to the actual text to be displayed instead of 
!							the number. If this is not defined, then the default library 
!							behavior takes presidence
!		many_number			Set this to the number which must be surpassed in order for the 
!							new behavior to occur (Defaults to 3)
!		specific_number		Given to a parent object (or container) to force the old behavior.
!							This is often given to the player so that 22 gold coins reads as 
!							such in the player's inventory (instead of "several gold coins")
!
!	sample code:
!
!		Class ChocolateMarbles(100) 
!			with	name "chocolate" "covered" "marble" "marbles//p"
!			,		short_name "chocolate covered marble"
!			,		plural "chocolate covered marbles"
!			,		plural_many "several" 
!			,		many_number 5
!			,		description "This is a single chocolate covered marble.";
!
!	
!	In the Initialise function add this to show exactly how many marbles the player 
!	is holding:
!
!		give player specific_number;
!
!--------------------------------------------------------------------------------------
! Revision History
! 2001.04.20	Initial Creation
! 2002.02.17	Added GLULX support.
! 2004.01.17	Corrected bug referencing a property named "number" rather than the 
!				actual count.
!--------------------------------------------------------------------------------------
message "          Processing library extension ORSeveral...";
!======================================================================================
! D E P E N D A N C I E S   section   (for bringing in dependant modules)
!======================================================================================
!	#ifndef <REPLACEWITHINCLUDENAME>_DONEREPLACE; default USE_<REPLACEWITHINCLUDENAME> 0; message "          [ORSeveral forcing inclusion of <REPLACEWITHINCLUDENAME>]"; #include "<REPLACEWITHINCLUDENAME>"; #endif; 
!	#ifndef ORLibraryInclude; message fatalerror "ORSeveral has dependences and cannot stand alone. Leverage the OR_Library_Include file to automatically meet module prerequisites."; #endif;
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

!======================================================================================
#ifndef LIBRARY_PARSER; #ifndef ORSeveral_DONEREPLACE; constant ORSeveral_DONEREPLACE; #ifdef ORSeveral_DONEREPLACE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! R E P L A C E   section (for code that preceeds the inclusion of PARSER)
!======================================================================================
	Replace WriteListR;
	Property plural_many;		!The actual text to be displayed instead of the number. If not
								!	defined, then the default library behavior takes presidence
	property many_number;		!The number which must be surpassed in order for the new behavior 
								!	to occur (Defaults to 3)
	Attribute specific_number;	!Given to a parent object (or container) to force the old behavior.
								!	This is often given to the player so that 22 gold coins reads as 
								!	such in the player's inventory (instead of several gold coins)
!======================================================================================
#ifnot; message "               ...Already Included. Skipping."; #endif; #ifnot; #ifndef LIBRARY_VERBLIB; #ifndef ORSeveral_DONEMESSAGE; constant ORSeveral_DONEMESSAGE;  #ifdef ORSeveral_DONEMESSAGE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! M E S S A G E   section (for code that falls between PARSER and VERBLIB )
!======================================================================================
!	#ifndef OREnglish; message fatalerror "ERROR!!!! ORSeveral requires the OREnglish file.";#endif; 

!======================================================================================
#endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORSeveral_DONECODE;constant ORSeveral_DONECODE; #ifdef ORSeveral_DONECODE; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
!======================================================================================
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	! WriteListR: pulled from VERBLIBM (Rel 6/10 sn 991113)
	!	Modified on 2000.07.20
	!
	!	Note: 
	!		Changes to this procedure should be transparent.  That is, if the appropriate
	!	properties/attributes have not been defined, then WriteListR should function
	!	exactly as it did prior to the change.
	!	
	!	Purpose:
	!		Multiple object listings which normally come out in the form of "10 gold coins", 
	!	are conditionally changed to display as "several gold coins".  This is accomplished by 
	!	the count value with the property value "plural_many" 
	!	This is done if :					1) the object provides "plural_many"
	!										2) the count is greater than the object's property
	!											"many_number", or 3 if it doesn't provide 
	!											that property (generally speaking, three doesn't
	!											equate to "many" or "several")
	!										3) the object's parent does not have the 
	!											attribute "specific_number"
	!
	[WriteListR o depth stack_pointer  classes_p sizes_p i j k k2 l m n q senc mr;

	  if (depth>0 && o==child(parent(o)))
	  {   SortOutList(o); o=child(parent(o)); }
	  for (::)
	  {   if (o==0) rfalse;
		  if (c_style & WORKFLAG_BIT ~= 0 && depth==0 && o hasnt workflag)
		  {   o = sibling(o); continue; }
		  if (c_style & CONCEAL_BIT ~= 0
			  && (o has concealed || o has scenery))
		  {   o=sibling(o); continue; }
		  break;
	  }

	  classes_p = match_classes + stack_pointer;
	  sizes_p   = match_list + stack_pointer;

	  for (i=o,j=0:i~=0 && (j+stack_pointer)<128:i=NextEntry(i,depth),j++)
	  {   classes_p->j=0;
		  if (i.plural~=0) k++;
	  }

	  if (c_style & ISARE_BIT ~= 0)
	  {   if (j==1 && o hasnt pluralname)
			  print (string) IS__TX; else print (string) ARE__TX;
		  if (c_style & NEWLINE_BIT ~= 0) print ":^"; else print (char) ' ';
		  c_style = c_style - ISARE_BIT;
	  }

	  stack_pointer = stack_pointer+j+1;

	  if (k<2) jump EconomyVersion;   ! It takes two to plural
	  n=1;
	  for (i=o,k=0:k<j:i=NextEntry(i,depth),k++)
		  if (classes_p->k==0)
		  {   classes_p->k=n; sizes_p->n=1;
			  for (l=NextEntry(i,depth), m=k+1:l~=0 && m<j:
				   l=NextEntry(l,depth), m++)
				  if (classes_p->m==0 && i.plural~=0 && l.plural~=0)
				  {   if (ListEqual(i,l)==1)
					  {   sizes_p->n = sizes_p->n + 1;
						  classes_p->m = n;
					  }
				  }
			  n++;
		  }
	  n--;

	  for (i=1, j=o, k=0: i<=n: i++, senc++)
	  {   while (((classes_p->k) ~= i)
				 && ((classes_p->k) ~= -i)) { k++; j=NextEntry(j,depth); }
		  m=sizes_p->i;
		  if (j==0) mr = 0;
		  else
		  {   if (j.list_together~=0 or lt_value
				  && ZRegion(j.list_together)==2 or 3
				  && j.list_together==mr) senc--;
			  mr=j.list_together;
		  }
	  }
	  senc--;

	  for (i=1, j=o, k=0, mr=0: senc>=0: i++, senc--)
	  {   while (((classes_p->k) ~= i)
				 && ((classes_p->k) ~= -i)) { k++; j=NextEntry(j,depth); }
		  if (j.list_together~=0 or lt_value)
		  {   if (j.list_together==mr) { senc++; jump Omit_FL2; }
			  k2=NextEntry(j,depth);
			  if (k2==0 || k2.list_together~=j.list_together) jump Omit_WL2;
			  k2=ZRegion(j.list_together);
			  if (k2==2 or 3)
			  {   q=j; listing_size=1; l=k; m=i;
				  while (m<n && q.list_together==j.list_together)
				  {   m++;
					  while (((classes_p->l) ~= m)
							 && ((classes_p->l) ~= -m))
					  {   l++; q=NextEntry(q,depth); }
					  if (q.list_together==j.list_together) listing_size++;
				  }
	!              print " [", listing_size, "] ";
				  if (listing_size==1) jump Omit_WL2;
				  if (c_style & INDENT_BIT ~= 0)
					  Print__Spaces(2*(depth+wlf_indent));
              
				  if (k2==3)
				  {   q=0; for (l=0:l<listing_size:l++) q=q+sizes_p->(l+i);

	! ModA begin !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						if(j provides plural_many && parent(j) hasnt specific_number && ( (j provides many_number && q > j.many_number) || ((j provides many_number)==false && q>3) ) )
						{
							if(PrintOrRun(j,plural_many,1)==false) EnglishNumber(q); 
						}
						else EnglishNumber(q); 
						print " ";
	! ModA end !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					  print (string) j.list_together;
					  if (c_style & ENGLISH_BIT ~= 0) print " (";
					  if (c_style & INDENT_BIT ~= 0) print ":^";
				  }
				  q=c_style;
				  if (k2~=3)
				  {   inventory_stage=1;
					  parser_one=j; parser_two=depth+wlf_indent;
					  if (RunRoutines(j,list_together)==1) jump Omit__Sublist2;
				  }

#ifndef TARGET_GLULX;
              @push lt_value; @push listing_together; @push listing_size;
#ifnot; ! TARGET_GLULX;
              @copy lt_value sp; 
              @copy listing_together sp; 
              @copy listing_size sp;
#endif; ! TARGET_;
              lt_value=j.list_together; listing_together=j; wlf_indent++;
              WriteListR(j,depth,stack_pointer); wlf_indent--;
#ifndef TARGET_GLULX;
              @pull listing_size; @pull listing_together; @pull lt_value;
#ifnot; ! TARGET_GLULX;
              @copy sp listing_size; 
              @copy sp listing_together; 
              @copy sp lt_value;
#endif; ! TARGET_;

				  if (k2==3)
				  {   if (q & ENGLISH_BIT ~= 0) print ")";
				  }
				  else
				  {   inventory_stage=2;
					  parser_one=j; parser_two=depth+wlf_indent;
					  RunRoutines(j,list_together);
				  }
				 .Omit__Sublist2;
				  if (q & NEWLINE_BIT ~= 0 && c_style & NEWLINE_BIT == 0)
					  new_line;
				  c_style=q;
				  mr=j.list_together;
				  jump Omit_EL2;
			  }
		  }

		 .Omit_WL2;
		  if (WriteBeforeEntry(j,depth,-senc)==1) jump Omit_FL2;
		  if (sizes_p->i == 1)
		  {   if (c_style & NOARTICLE_BIT ~= 0) print (name) j;
			  else
			  {   if (c_style & DEFART_BIT ~= 0) print (the) j; else print (a) j;
			  }
		  }
		  else
		  {   if (c_style & DEFART_BIT ~= 0)
				  PrefaceByArticle(j, 1, sizes_p->i);
	! ModA begin !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				if(j provides plural_many && parent(j) hasnt specific_number && ( (j provides many_number && (sizes_p->i) > j.many_number) || ((j provides many_number)==false && (sizes_p->i)>3) ) )
				{
					if(PrintOrRun(j,plural_many,1)==false) print (number) sizes_p->i; 
				}
				else print (number) sizes_p->i; 
				print " ";
	! ModA end !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			PrintOrRun(j,plural,1);
		  }
		  WriteAfterEntry(j,depth,stack_pointer);

		 .Omit_EL2;
		  if (c_style & ENGLISH_BIT ~= 0)
		  {   if (senc==1) print (string) AND__TX;
			  if (senc>1) print ", ";
		  }
		 .Omit_FL2;
	  }
	  rtrue;

	  .EconomyVersion;

	  n=j;

	  for (i=1, j=o: i<=n: j=NextEntry(j,depth), i++, senc++)
	  {   if (j.list_together~=0 or lt_value
			  && ZRegion(j.list_together)==2 or 3
			  && j.list_together==mr) senc--;
		  mr=j.list_together;
	  }

	  for (i=1, j=o, mr=0: i<=senc: j=NextEntry(j,depth), i++)
	  {   if (j.list_together~=0 or lt_value)
		  {   if (j.list_together==mr) { i--; jump Omit_FL; }
			  k=NextEntry(j,depth);
			  if (k==0 || k.list_together~=j.list_together) jump Omit_WL;
			  k=ZRegion(j.list_together);
			  if (k==2 or 3)
			  {   if (c_style & INDENT_BIT ~= 0)
					  Print__Spaces(2*(depth+wlf_indent));
				  if (k==3)
				  {   q=j; l=0;
					  do
					  {   q=NextEntry(q,depth); l++;
					  } until (q==0 || q.list_together~=j.list_together);
					  EnglishNumber(l); print " ";
					  print (string) j.list_together;
					  if (c_style & ENGLISH_BIT ~= 0) print " (";
					  if (c_style & INDENT_BIT ~= 0) print ":^";
				  }
				  q=c_style;
				  if (k~=3)
				  {   inventory_stage=1;
					  parser_one=j; parser_two=depth+wlf_indent;
					  if (RunRoutines(j,list_together)==1) jump Omit__Sublist;
				  }

#ifndef TARGET_GLULX;
              @push lt_value; @push listing_together; @push listing_size;
#ifnot; ! TARGET_GLULX;
              @copy lt_value sp; 
              @copy listing_together sp; 
              @copy listing_size sp;
#endif; ! TARGET_;
              lt_value=j.list_together; listing_together=j; wlf_indent++;
              WriteListR(j,depth,stack_pointer); wlf_indent--;
#ifndef TARGET_GLULX;
              @pull listing_size; @pull listing_together; @pull lt_value;
#ifnot; ! TARGET_GLULX;
              @copy sp listing_size; 
              @copy sp listing_together; 
              @copy sp lt_value;
#endif; ! TARGET_;

				  if (k==3)
				  {   if (q & ENGLISH_BIT ~= 0) print ")";
				  }
				  else
				  {   inventory_stage=2;
					  parser_one=j; parser_two=depth+wlf_indent;
					  RunRoutines(j,list_together);
				  }
				 .Omit__Sublist;
				  if (q & NEWLINE_BIT ~= 0 && c_style & NEWLINE_BIT == 0) new_line;
				  c_style=q;
				  mr=j.list_together;
				  jump Omit_EL;
			  }
		  }
		 .Omit_WL;
		  if (WriteBeforeEntry(j,depth,i-senc)==1) jump Omit_FL;
		  if (c_style & NOARTICLE_BIT ~= 0) print (name) j;
		  else
		  {   if (c_style & DEFART_BIT ~= 0) print (the) j; else print (a) j;
		  }
		  WriteAfterEntry(j,depth,stack_pointer);

		 .Omit_EL;
		  if (c_style & ENGLISH_BIT ~= 0)
		  {   if (i==senc-1) print (string) AND__TX;
			  if (i<senc-1) print ", ";
		  }
		 .Omit_FL;
	  }
	];
!======================================================================================
#endif; #ifnot; #ifndef ORSeveral_DONEGRAMMAR; constant ORSeveral_DONEGRAMMAR; #ifdef ORSeveral_DONEGRAMMAR; #endif; !--suppress warning
!  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
!======================================================================================

!======================================================================================
#endif; #endif; #endif;#endif;
!======================================================================================