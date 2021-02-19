!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.06.08 OREnglish    [Z,GLULX]
! This is an alternate language definition file based upon the English file 
! included in the standard library.  It supports games written in past 
! tense as well as present tense, plus 1st, 2nd, and 3rd person.  It further 
! provides the ability to alternate between these two modes, mixing and 
! matching tense and person. It further enables support for NPCs performing 
! actions.
! 
! This is NOT a standard ORLibrary entry, and it cannot be included in the 
! same fashion as other ORLibrary entries.  Instead it must be specified as
! an ICL command (i.e. specified on the commandline, in the ICL
! file, or in the ICL section of the sourcecode).  
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!---------------------------------------------------------------------------
! Created by Graham Nelson (copyrights below all apply)
! Modified by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- - 
! License: 
! Matches that of Inform's standard library (see included header, below).
!--------------------------------------------------------------------------------------
! AutoDep :        ORTextFormatting
!                ORPronoun
!--------------------------------------------------------------------------------------
! This file is based upon the original Inform library source file English
! as released with the standard library.  The header of the file, which follows
! the OREnglish-specific notes, remains unchanged.
!---------------------------------------------------------------------------
! OREnglish-specific notes:
!
!    The modifications to this language file currently allow for changing the 
!    narative of the story between first, second, and third person, as well 
!    as past and present tenses.  This is accomplished by two global variables:
!
!        NarativePerson    = FIRST_PERSON, SECOND_PERSON, or THIRD_PERSON;
!        NarativeTense    = PAST_TENSE, or PRESENT_TENSE;
!
!    Additionally the NarativeTense variable can contain FUTURE_TENSE, but
!    this value is not yet fully supported.
!
!    The functions ppf(past, present, future) and fst(first, second,third)
!    have also been defined which accept strings of text and print the 
!    appropriate value based upon the value of NarativePerson or NarativeTense.
!   Note, that the (ig) printrule for ignore should be used with these.
!
!    Also the function SetPersonTense() can be called in the game to set
!    the appropriate values.  Without a call to this function, the default 
!    settings of SECOND_PERSON, PRESENT_TENSE are in affect, making this file 
!    imitate the original "English" file upon which it was based.
!
!    To use this file, pass:
!
!            +language_name=OREnglish
!
!    as an icl setting.
!
!---------------------------------------------------------------------------
!    N O T E S   O N   U S I N G   T H I R D   P E R S O N :
!    Third person is unique in that it is legal to use the actual name of the 
!    player.  That is, the name of the player is never appropriate in first 
!    or second person, but conversely, the name (or short_name) of the PC is 
!    3rd person by its very nature and may be used instead of the pronoun at 
!    times.  For this reason, when using THIRD_PERSON narative mode, the PC 
!    MUST HAVE A NAME OR SHORT_NAME SPECIFIED.  If not, then the self_obj's 
!    default short_name property will be used.  The standard library 
!    defines this to call the "myself" library message (18) which is 
!    inappropriate for third_person narative and will result in text similar
!    to the following:
!        
!        himself could see Dr. Bob there.
!
!    It is also worth mentioning that the selfobj is given the "proper" 
!    attribute by default, so nameless PCs (such as "the detective") might 
!    do well to remove this attribute in the initialise() routine, provided
!    an alternate PC is not being created.
!===========================================================================
! Original file header:
    ! ------------------------------------------------------------------------------
    !   Inform Language Definition File: English 040227
    !
    !   Supplied for use with Inform 6
    !
    !   Copyright (c) Graham Nelson 1993-2003
    !       but freely usable (see manuals)
    !
    !   This file is automatically Included in your game file by "parserm".
    !   Strictly, "parserm" includes the file named in the "language__" variable,
    !   whose contents can be defined by the +language_name=XXX compiler setting
    !   (with a default of "english").
    !
    !   Define the constant DIALECT_US before including "Parser" to
    !   obtain American English
    ! ------------------------------------------------------------------------------

System_file;

message "       ---Processing the OREnglish LDF (Once only)---";
    constant OREnglish;
	#ifndef ORTextFormatting_DONEREPLACE; default USE_ORTextFormatting 0; message "          [ORKnowledgeTopic forcing inclusion of ORTextFormatting]"; #include "ORTextFormatting"; #endif; 
	#ifndef ORPronoun_DONEREPLACE; default USE_ORPronoun 0; message "          [ORKnowledgeTopic forcing inclusion of ORPronoun]"; #include "ORPronoun"; #endif; 
! ------------------------------------------------------------------------------
!   ORLibrary specific section
! ------------------------------------------------------------------------------
    !----------------------------------------------------------------------------
    !    values and routines for person and tense
    !------------------------------------------------------------------------
    !    Add values to specify Person and Tense
    constant FIRST_PERSON 1;
    constant SECOND_PERSON 2; 
    constant THIRD_PERSON 3;
    constant PAST_TENSE 1;
    constant PRESENT_TENSE 2; 
    constant FUTURE_TENSE 3;
    global NarativePerson = SECOND_PERSON;
    global NarativeTense = PRESENT_TENSE;
    !------------------------------------------------------------------------
    ! print bracket around text
    default BracketedParserMsgs=false;
    #ifndef bkt; [bkt m; print (string)m;]; #endif;
    #stub bkt_o 0;
    #stub bkt_c 0;
    !------------------------------------------------------------------------
    ! ppf (past, present, future tense options)
    [ppf past present future;
        switch(NarativeTense)
        {
            PAST_TENSE: print (string) past;
            PRESENT_TENSE: print (string) present;
            FUTURE_TENSE:print (string) future;
        }
    ];
    !------------------------------------------------------------------------
    ! fst (First,Second,Third Person options)
    [fst first second third;
        switch(NarativePerson)
        {
            FIRST_PERSON:    print (string) first;
            SECOND_PERSON:    print (string) second;
            THIRD_PERSON:    print (string) third;
        }
    ];
! ---------------------------------------------------------------------------
!   Part I.   Preliminaries
! ---------------------------------------------------------------------------
Constant EnglishNaturalLanguage;   ! Needed to keep old pronouns mechanism

Class   CompassDirection
    with  number 0, article "the",
          description [;
              if (location provides compasslook && location.compasslook(self)) rtrue;
              print (ig)CIVerb(actor,"saw","see")," nothing special ";
              if (self ~= u_obj or d_obj) print "to the ";
              print_ret (name) self, ".";
              ],
    has   scenery;
Object Compass "compass" has concealed;
IFNDEF WITHOUT_DIRECTIONS;
CompassDirection -> n_obj "north"
                    with name 'n//' 'north',    door_dir n_to;
CompassDirection -> s_obj "south"
                    with name 's//' 'south',    door_dir s_to;
CompassDirection -> e_obj "east"
                    with name 'e//' 'east',     door_dir e_to;
CompassDirection -> w_obj "west"
                    with name 'w//' 'west',     door_dir w_to;
CompassDirection -> ne_obj "northeast"
                    with name 'ne' 'northeast', door_dir ne_to;
CompassDirection -> nw_obj "northwest"
                    with name 'nw' 'northwest', door_dir nw_to;
CompassDirection -> se_obj "southeast"
                    with name 'se' 'southeast', door_dir se_to;
CompassDirection -> sw_obj "southwest"
                    with name 'sw' 'southwest', door_dir sw_to;
CompassDirection -> u_obj "above"
                    with name 'u//' 'up' 'ceiling' 'above' 'sky',     door_dir u_to;
CompassDirection -> d_obj "below"
                    with name 'd//' 'down' 'floor' 'below' 'ground',  door_dir d_to;
ENDIF;
CompassDirection -> out_obj "outside"
                    with                               door_dir out_to,
                    compasslook 0;  ! use it somewhere just to satisfy the compiler
CompassDirection -> in_obj "inside"
                    with                               door_dir in_to;

! ---------------------------------------------------------------------------
!   Part II.   Vocabulary
! ---------------------------------------------------------------------------

Constant AGAIN1__WD   = 'again';
Constant AGAIN2__WD   = 'g//';
Constant AGAIN3__WD   = 'again';
Constant OOPS1__WD    = 'oops';
Constant OOPS2__WD    = 'o//';
Constant OOPS3__WD    = 'oops';
Constant UNDO1__WD    = 'undo';
Constant UNDO2__WD    = 'undo';
Constant UNDO3__WD    = 'undo';

Constant ALL1__WD     = 'all';
Constant ALL2__WD     = 'each';
Constant ALL3__WD     = 'every';
Constant ALL4__WD     = 'everything';
Constant ALL5__WD     = 'both';
Constant AND1__WD     = 'and';
Constant AND2__WD     = 'and';
Constant AND3__WD     = 'and';
Constant BUT1__WD     = 'but';
Constant BUT2__WD     = 'except';
Constant BUT3__WD     = 'but';
Constant ME1__WD      = 'me';
Constant ME2__WD      = 'myself';
Constant ME3__WD      = 'self';
Constant OF1__WD      = 'of';
Constant OF2__WD      = 'of';
Constant OF3__WD      = 'of';
Constant OF4__WD      = 'of';
Constant OTHER1__WD   = 'another';
Constant OTHER2__WD   = 'other';
Constant OTHER3__WD   = 'other';
Constant THEN1__WD    = 'then';
Constant THEN2__WD    = 'then';
Constant THEN3__WD    = 'then';

Constant NO1__WD      = 'n//';
Constant NO2__WD      = 'no';
Constant NO3__WD      = 'no';
Constant YES1__WD     = 'y//';
Constant YES2__WD     = 'yes';
Constant YES3__WD     = 'yes';

Constant AMUSING__WD  = 'amusing';
Constant FULLSCORE1__WD = 'fullscore';
Constant FULLSCORE2__WD = 'full';
Constant QUIT1__WD    = 'q//';
Constant QUIT2__WD    = 'quit';
Constant RESTART__WD  = 'restart';
Constant RESTORE__WD  = 'restore';

Array LanguagePronouns table

   !  word       possible GNAs                   connected
   !             to follow:                      to:
   !             a     i
   !             s  p  s  p
   !             mfnmfnmfnmfn                 

      'it'     $$001000111000                    NULL
      'him'    $$100000000000                    NULL
      'her'    $$010000000000                    NULL
      'them'   $$000111000111                    NULL;

Array LanguageDescriptors table

   !  word       possible GNAs   descriptor      connected
   !             to follow:      type:           to:
   !             a     i
   !             s  p  s  p
   !             mfnmfnmfnmfn                 

      'my'     $$111111111111    POSSESS_PK      0
      'this'   $$111111111111    POSSESS_PK      0
      'these'  $$000111000111    POSSESS_PK      0
      'that'   $$111111111111    POSSESS_PK      1
      'those'  $$000111000111    POSSESS_PK      1
      'his'    $$111111111111    POSSESS_PK      'him'
      'her'    $$111111111111    POSSESS_PK      'her'
      'their'  $$111111111111    POSSESS_PK      'them'
      'its'    $$111111111111    POSSESS_PK      'it'
      'the'    $$111111111111    DEFART_PK       NULL
      'a//'    $$111000111000    INDEFART_PK     NULL
      'an'     $$111000111000    INDEFART_PK     NULL
      'some'   $$000111000111    INDEFART_PK     NULL
      'lit'    $$111111111111    light           NULL
     'lighted' $$111111111111    light           NULL
      'unlit'  $$111111111111    (-light)        NULL;

Array LanguageNumbers table
    'one' 1 'two' 2 'three' 3 'four' 4 'five' 5
    'six' 6 'seven' 7 'eight' 8 'nine' 9 'ten' 10
    'eleven' 11 'twelve' 12 'thirteen' 13 'fourteen' 14 'fifteen' 15
    'sixteen' 16 'seventeen' 17 'eighteen' 18 'nineteen' 19 'twenty' 20;

! ---------------------------------------------------------------------------
!   Part III.   Translation
! ---------------------------------------------------------------------------

[ LanguageToInformese;
];

! ---------------------------------------------------------------------------
!   Part IV.   Printing
! ---------------------------------------------------------------------------

Constant LanguageAnimateGender   = male;
Constant LanguageInanimateGender = neuter;

Constant LanguageContractionForms = 2;     ! English has two:
                                           ! 0 = starting with a consonant
                                           ! 1 = starting with a vowel

[ LanguageContraction text;
  if (text->0 == 'a' or 'e' or 'i' or 'o' or 'u'
                 or 'A' or 'E' or 'I' or 'O' or 'U') return 1;
  return 0;
];

Array LanguageArticles -->

 !   Contraction form 0:     Contraction form 1:
 !   Cdef   Def    Indef     Cdef   Def    Indef

     "The " "the " "a "      "The " "the " "an "          ! Articles 0
     "The " "the " "some "   "The " "the " "some ";       ! Articles 1

                   !             a           i
                   !             s     p     s     p
                   !             m f n m f n m f n m f n                 

Array LanguageGNAsToArticles --> 0 0 0 1 1 1 0 0 0 1 1 1;

[ LanguageDirection d;
   switch(d)
   {   n_to: print "north";
       s_to: print "south";
       e_to: print "east";
       w_to: print "west";
       ne_to: print "northeast";
       nw_to: print "northwest";
       se_to: print "southeast";
       sw_to: print "southwest";
       u_to: print "up";
       d_to: print "down";
       in_to: print "in";
       out_to: print "out";
       default: return RunTimeError(9,d);
   }
];

[ LanguageNumber n f;
  if (n==0)    { print "zero"; rfalse; }
  if (n<0)     { print "minus "; n=-n; }
  if (n>=1000) { print (LanguageNumber) n/1000, " thousand"; n=n%1000; f=1; }
    if (n >= 100)  {
        if (f == 1) print ", ";
        print (LanguageNumber) n/100, " hundred"; n = n%100; f = 1;
    }
  if (n==0) rfalse;
  #ifndef DIALECT_US;
  if (f==1) print " and ";
  #ifnot;
  if (f==1) print " ";
  #endif;
  switch(n)
  {   1:  print "one";
      2:  print "two";
      3:  print "three";
      4:  print "four";
      5:  print "five";
      6:  print "six";
      7:  print "seven";
      8:  print "eight";
      9:  print "nine";
      10: print "ten";
      11: print "eleven";
      12: print "twelve";
      13: print "thirteen";
      14: print "fourteen";
      15: print "fifteen";
      16: print "sixteen";
      17: print "seventeen";
      18: print "eighteen";
      19: print "nineteen";
      20 to 99:
          switch(n/10)
          {  2: print "twenty";
             3: print "thirty";
             4: print "forty";
             5: print "fifty";
             6: print "sixty";
             7: print "seventy";
             8: print "eighty";
             9: print "ninety";
          }
          if (n%10 ~= 0) print "-", (LanguageNumber) n%10;
  }
];

[ LanguageTimeOfDay hours mins i;
   i=hours%12;
   if (i==0) i=12;
   if (i<10) print " ";
   print i, ":", mins/10, mins%10;
   if ((hours/12) > 0) print " pm"; else print " am";
];

[ LanguageVerb i;
   if (i==#n$l)        { print "look";              rtrue; }
   if (i==#n$z)        { print "wait";              rtrue; }
   if (i==#n$x)        { print "examine";           rtrue; }
   if (i==#n$i or 'inv' or 'inventory')
                       { print "inventory";         rtrue; }
   rfalse;
];

! ----------------------------------------------------------------------------
!  LanguageVerbIsDebugging is called by SearchScope.  It should return true 
!  if word w is a debugging verb which needs all objects to be in scope.
! ----------------------------------------------------------------------------

#Ifdef DEBUG;
[ LanguageVerbIsDebugging w;
    if (w == 'purloin' or 'tree' or 'abstract'
                       or 'gonear' or 'scope' or 'showobj')
        rtrue;
    rfalse;
];
#Endif;

! ----------------------------------------------------------------------------
!  LanguageVerbLikesAdverb is called by PrintCommand when printing an UPTO_PE
!  error or an inference message.  Words which are intransitive verbs, i.e.,
!  which require a direction name as an adverb ('walk west'), not a noun
!  ('I only understood you as far as wanting to touch /the/ ground'), should
!  cause the routine to return true.
! ----------------------------------------------------------------------------

[ LanguageVerbLikesAdverb w;
    if (w == 'look' or 'go' or 'push' or 'walk')
        rtrue;
    rfalse;
];

! ----------------------------------------------------------------------------
!  LanguageVerbMayBeName is called by NounDomain when dealing with the 
!  player's reply to a "Which do you mean, the short stick or the long
!  stick?" prompt from the parser. If the reply is another verb (for example,
!  LOOK) then then previous ambiguous command is discarded /unless/
!  it is one of these words which could be both a verb /and/ an
!  adjective in a 'name' property.
! ----------------------------------------------------------------------------

[ LanguageVerbMayBeName w;
    if (w == 'long' or 'short' or 'normal'
                    or 'brief' or 'full' or 'verbose')
        rtrue;
    rfalse;
];

Constant NKEY__TX     = "N = next subject";
Constant PKEY__TX     = "P = previous";
Constant QKEY1__TX    = "  Q = resume game";
Constant QKEY2__TX    = "Q = previous menu";
Constant RKEY__TX     = "RETURN = read subject";

Constant NKEY1__KY    = 'N';
Constant NKEY2__KY    = 'n';
Constant PKEY1__KY    = 'P';
Constant PKEY2__KY    = 'p';
Constant QKEY1__KY    = 'Q';
Constant QKEY2__KY    = 'q';

Constant SCORE__TX      = "Score: ";
Constant MOVES__TX      = "Moves: ";
Constant TIME__TX       = "Time: ";
    global CANTGO__TX    = "You can't go that way.";
    global FORMER__TX    = "your former self";
    global YOURSELF__TX    = "yourself";
    global YOU__TX        = "You";
Constant DARKNESS__TX   = "Darkness";

Constant THOSET__TX     = "those things";
Constant THAT__TX       = "that";
Constant OR__TX         = " or ";
Constant NOTHING__TX    = "nothing";
    global IS__TX       = " is";
    global ARE__TX      = " are";
    global IS2__TX      = "is ";
    global ARE2__TX     = "are ";
Constant AND__TX        = " and ";
Constant WHOM__TX       = "whom ";
Constant WHICH__TX      = "which ";

Constant COMMA__TX      = ", ";

[ ThatorThose obj;  
    if(obj == player || obj has animate) 
        print(Me)obj;
    else{
        if (obj has pluralname) print "those"; 
        else print "that";
    }
];
[ ItorThem obj; print (Me)obj;];
[ IsorAre obj; print (am)obj;];

[CThatorThose obj;  
        if(obj == player || obj has animate) 
            print(CI)obj;
        else{
            if (obj has pluralname) print "Those"; 
            print "That";
        }
    ];
[ CTheyreorThats obj; print (ig)CIAm(obj);];
[ TheyreorThats obj; print (ig)IAm(obj);];


[ LanguageLM n x1;
  Answer, Ask:    "There",(string) IS__TX," no reply.";
  ! Ask:      see Answer
  Attack:         print_ret (ig)CIVerb(actor,"considered","consider")," attacking ",(theMe)noun," but ",(ig)vrb(actor,"decided","decide")," against it. Violence",(string) IS__TX," not the answer to this.";
  Blow:           print_ret (ig) CICant(actor)," usefully blow ", (thatorthose) x1, ".";
  Burn:           print_ret "This dangerous act ",(ig)ppf("would have achieved","would achieve","will achieve")," little.";
  Buy:            "Nothing",(string) IS__TX," on sale.";
  Climb:          print_ret (ig)CIVerb(actor,"did","do")," not think much",(string) IS__TX," to be achieved by that.";
  Close:   switch(n)
           {   1: if(actor~=player) print (ig)CIVerb(actor,"tried","try","tries")," to close ",(the)x1,", but ", (theyreorthats) x1;
                    else print (ctheyreorthats) x1;
                    print_ret " not something ",(ig)ICan(actor)," close.";
               2: if(actor~=player) print (ig)CIVerb(actor,"tried","try","tries")," to close ",(the)x1,", but ", (theyreorthats) x1;
                    else print (ctheyreorthats) x1;
                  " already closed.";
               3: print_ret (ig)CIVerb(actor,"closed","close"), " ",(the) x1, ".";
           }
  CommandsOn:
    #Ifdef TARGET_GLULX;
    switch(n) {
      2: "[Commands are currently replaying.]";
      3: "[Command recording already on.]";
      4: "[Command recording failed.]";
      }
    #Endif;
    "[Command recording on.]";
  CommandsOff:
    #Ifdef TARGET_GLULX;
    switch(n) {
      2: "[Command recording already off.]";
      }
    #Endif;
    "[Command recording off.]";
  CommandsRead:
    #Ifdef TARGET_GLULX;
    switch(n) {
      2: "[Commands are already replaying.]";
      3: "[Command replay failed.  Command recording is on.]";
      4: "[Command replay failed.]";
      5: "[Command replay complete.]";
      }
    #Endif;
    "[Replaying commands.]";
  Consult:        print_ret (ig)CIVerb(actor,"discovered","discover")," nothing of interest in ", (the) x1, ".";
  Cut:            print_ret "Cutting ", (thatorthose) x1, " up ",(ig) ppf("would have","would","will")," achieve",(ig) ppf("d","")," little.";
  Dig:            print_ret "Digging ",(ig) ppf("would have","would","will")," achieve",(ig) ppf("d","")," nothing here.";
  Disrobe: switch(n)
           {   1: print_ret (CTheyreorThats) actor," not wearing ", (thatorthose) x1, ".";
               2: print_ret (ig)CIVerb(actor,"took","take")," off ", (the) x1, ".";
           }
  Drink:          print_ret "There",(string) IS__TX," nothing suitable to drink here.";
  Drop:    switch(n)
           {   1: print_ret (ig)CIAm(noun), " already ",(ig) ppf("there.","here.");
               2: print_ret (CI)actor,(ig)ppf(" did"," do"," will")," not have ",(ig)ppf("",""," had "), (thatorthose) x1, ".";
               3: "(first taking ", (the) x1, " off)";
               4: print_ret "Dropped.";
           }
  Eat:     switch(n)
           {   1: print_ret (ctheyreorthats) x1,
                  " plainly inedible.";
               2: print (ig)CIVerb(actor,"ate","eat"), " ",(the) x1, ". ";
                    if(actor==player) print"Not bad.";
                    new_line;
                    return;
           }
  EmptyT:  switch(n)
           {   1: print_ret (ig) CICant(x1)," contain things.";
               2: print_ret (The) x1, " ", (isorare) x1, " closed.";
               3: print_ret (The) x1, " ", (isorare) x1, " empty already.";
               4: "That ",(ig)ppf("would ","would ","will "),"scarcely ",(ig) ppf("have emptied","empty","empty")," anything.";
           }
  Enter:   switch(n)
           {   1: print "But ",(ig)IAm(actor)," already ";
                  if (x1 has supporter) print "on "; else print "in ";
                  print_ret (the) x1, ".";
               2: print (ig)CIam(x1)," not something ",(ig)ICan(actor);
                  switch (verb_word) {
                      'stand': " stand on.";
                      'sit': " sit down on.";
                      'lie': " lie down on.";
                      default: " enter.";
                  }
               3: print_ret (ig)CICant(actor)," get into the closed ", (name) x1, ".";
               4: print_ret (ig)CICan(actor)," only get into something free-standing.";
               5: print (ig)CIVerb(actor,"got","get");
                  if (x1 has supporter) print " onto "; else print " into ";
                  print_ret (the) x1, ".";
               6: print "(getting ";
                  if (x1 has supporter) print " off "; else print " out of ";
                  print (the) x1; ")";
               7: if (x1 has supporter) "(getting onto ", (the) x1, ")^";
                  if (x1 has container) "(getting into ", (the) x1, ")^";
                  "(entering ", (the) x1, ")^";
           }
  Examine: switch(n)
           {   1: "Darkness, noun.  An absence of light to see by.";
               2: print_ret (ig)CIVerb(actor,"saw","see")," nothing special about ", (the) x1, ".";
               3: print (ig)CIAm(x1), " currently switched ";
                  if (x1 has on) "on."; else "off.";
           }
  Exit:    switch(n)
           {   
               1: print_ret "But ",(ig)IAm(actor)," not in anything at ",(ig)ppf("that","the","that")," moment.";
               2: print_ret (ig)CICant(actor)," get out of the closed ", (name) x1, ".";
               3: print (ig)CIVerb(actor,"got","get");
                  if (x1 has supporter) print " off "; else print " out of ";
                  print_ret (the) x1, ".";
           }
  Fill:           print_ret "But there",(string) IS__TX," no water here to carry.";
  FullScore: switch(n)
           {   
			   1:	bkt_o();
					if (deadflag) print "The score was ";
					else          print "The score is ";
					"made up as follows:^";
               2: "finding sundry items";
               3: "visiting various places";
               4: print "total (out of ", MAX_SCORE; print ")"; new_line; bkt_c();
           }
  GetOff:         "But ",(ig)IAm(actor)," not on ", (the) x1, " at ",(ig)ppf("that","the","that")," moment.";
  Give:    switch(n)
           {   1: print_ret (ig)CIAm(actor)," not holding ", (the) noun, ".";
               2: print_ret (ig)CIVerb(actor,"juggled","juggle"), " ",(the) x1,
                  " for a while, but ",(ig) ppf("did","do","will")," not achieve much.";
               3: print_ret (ig)CIVerb(x1,"did","do","does", "will"), " not seem interested.";
           }
  Go:      switch(n)
           {   1: print (CTheI) actor,(ig) ppf(" would have to have gotten"," will have to get"," will have to get");
                  if (x1 has supporter) print " off "; else print " out of ";
                  print_ret (the) x1, " first.";
               2: print_ret (ig)CICant(actor)," go that way.";
               3: print_ret (ig)CIAm(actor)," unable to climb ", (the) x1, ".";
               4: print_ret (ig)CIAm(actor)," unable to descend by ", (the) x1, ".";
               5: print_ret (ig)CICant(actor),", since ", (the) x1, " ", (isorare) x1," in the way.";
               6: print_ret (ig)CICant(actor),", since ", (ig)IVerb(x1,"led","lead")," nowhere.";
           }
  Insert:  switch(n)
           {   1: print_ret (ig)CIVerb(actor,"needed","need")," to be holding ", (the) x1,
                  " before ",(ig)ICan(actor)," put ", (itorthem) x1,
                  " into something else.";
               2: print_ret (ig)CICant(x1), " contain things.";
               3: print_ret (ig)CIAm(x1), " closed.";
               4: print_ret (ig)CIVerb(actor,"needed","need")," to take ", (itorthem) x1, " off first.";
               5: print_ret (ig)CICant(actor)," put something inside itself.";
               6: "(first taking ", (itorthem) x1, " off)^";
               7: "There",(string)IS__TX," no more room in ", (the) x1, ".";
               8: "Done.";
               9: print_ret (ig)CIVerb(actor,"put","put")," ",(the) x1, " into ", (the) second, ".";
           }
  Inv:     switch(n){   
                1: print_ret (ig)CIAm(actor), " carrying nothing.";    
                2: print (ig)CIAm(actor), " carrying";        
                3:  print ":^";
                4:  print ".^";
           }
  Jump:           print_ret (ig)CIVerb(actor,"jumped","jump")," on the spot, fruitlessly.";
  JumpOver, Tie:  print_ret (CThatorthose) actor," ",(ig) ppf("would have","would","will")," achieve",(ig) ppf("d","")," nothing by this.";
  Kiss:           print_ret (bkt)"Keep your mind on the game.";
  Listen:         print_ret (ig)CIVerb(actor,"heard","hear")," nothing unexpected.";
  ListMiscellany:
           switch(n)
           {   1: print " (providing light)";
               2: print " (which ", (isorare) x1, " closed)";
               3: print " (closed and providing light)";
               4: print " (which ", (isorare) x1, " empty)";
               5: print " (empty and providing light)";
               6: print " (which ", (isorare) x1, " closed and empty)";
               7: print " (closed, empty and providing light)";
               8: print " (providing light and being worn";
               9: print " (providing light";
              10: print " (being worn";
              11: print " (which ", (isorare) x1, " ";
              12: print "open";
              13: print "open but empty";
              14: print "closed";
              15: print "closed and locked";
              16: print " and empty";
              17: print " (which ", (isorare) x1, " empty)";
              18: print " containing ";
              19: print " (on ";
              20: print ", on top of ";
              21: print " (in ";
              22: print ", inside ";
           }
  LMode1:    print " is now in its ~brief~ printing mode, which gives
                   long descriptions of places never before visited and short
                   descriptions otherwise.";
  LMode2:   print " is now in its ~verbose~ mode, which always gives long
                   descriptions of locations
                   (even if you've been there before).";
  LMode3:   print " is now in its ~superbrief~ mode, which always gives short
                   descriptions of locations
                   (even if you haven't been there before).";
  Lock:    switch(n)
           {   1: if(actor~=player) print (ig)CIVerb(actor,"tried","try","tries")," to unlock ",(the)x1,", but ",(thatorthose)x1," ",(ig)vrb(x1,"did","do","does","will")," not";
                    else print (cthatorthose)x1," ",(ig)vrb(x1,"did","do","does","will")," not";
                  " seem to be something ",(ig) ICan(actor)," lock.";
               2: if(actor~=player) print (ig)CIVerb(actor,"tried","try","tries")," to unlock ",(the)x1,", but ", (theyreorthats) x1;
                    else print (ctheyreorthats) x1;
                  " locked.";
               3: "First ",(ig)CIVerb(actor," would"," will"," will"," will")," have to close ", (the) x1, ".";
               4: if(actor~=player) print (ig)CIVerb(actor,"tried","try","tries")," to unlock ",(the)noun,", but ",(thatorthose)x1," ",(ig)vrb(x1,"did","do","does","will")," not";
                    else print (thatorthose)x1," ",(ig)vrb(x1,"did","do","does","will")," not";
                  " seem to fit the lock.";
               5: print_ret (ig)CIVerb(actor,"locked","lock")," ", (the) x1, "."; 
           }
  Look:    switch(n)
           {   1: print " (on ", (the) x1, ")";
               2: print " (in ", (the) x1, ")";
               3: print " (as ", (object) x1, ")";
               4: print "^On ", (the) x1;
                    WriteListFrom(child(x1),
                      ENGLISH_BIT+RECURSE_BIT+PARTINV_BIT+TERSE_BIT+CONCEAL_BIT+ISARE_BIT);
                    ".";
                5,6:
                    if (x1~=location)
                    {   if (x1 has supporter) print "^On "; else print "^In ";
                        print (the) x1, " ",(ig)ICan(actor);
                    }
                    else print "^",(ig) CICan(actor);
                    if (n==5) print " also"; print " see ";
                    WriteListFrom(child(x1),
                        ENGLISH_BIT + WORKFLAG_BIT + RECURSE_BIT
                        + PARTINV_BIT + TERSE_BIT + CONCEAL_BIT);
                    if (x1~=location) ".";
                    print_ret " ",(ig)ppf("there","here","there"),".";                
                7:  print "^",(ig) CIVerb(actor,"saw","see")," nothing unexpected in that direction.";

           }
  LookUnder: switch(n)
           {   1: "But it",(string)IS__TX," dark.";
               2: print_ret (ig)CIVerb(actor,"found","find")," nothing of interest.";
           }
  Mild:           "Quite.";
  Miscellany:
           switch(n)
           {   1: "(considering the first sixteen objects only)^";
               2: print_ret (bkt)"Nothing to do!","^";
               3: print " ",(ig)IHave(player)," died "; 
               4: print " ",(ig)IHave(player)," won "; 
               5: print "^";
                  bkt_o();
                  print "Would you like to RESTART, RESTORE a saved game";
                  #IFDEF DEATH_MENTION_UNDO;
                  print ", UNDO your last move";
                  #ENDIF;
                  if (TASKS_PROVIDED==0)
                      print ", give the FULL score for that game";
                  if (deadflag==2 && AMUSING_PROVIDED==0)
                      print ", see some suggestions for AMUSING things to do";
                  print " or QUIT?";
                  bkt_c();
                  print"^";
               6: print_ret (bkt)"Your interpreter does not provide ~undo~.  Sorry!";
               7: print_ret (bkt)"~Undo~ failed.  Not all interpreters provide it.";
               8: print_ret (bkt)"Please give one of the answers above.";
               9: print_ret "^It ",(ig) ppf("was then","is now")," pitch dark in ",(ig) ppf("there","here"),"!"; 
              10: print_ret (bkt)"I beg your pardon?";
              11:  print_ret (bkt)"You can't ~undo~ what hasn't been done!";
              12:  print_ret (bkt)"Can't ~undo~ twice in succession. Sorry!";
              13:  print_ret (bkt)"Previous turn undone.";
              14:  print_ret (bkt)"Sorry, that can't be corrected.";
              15:  print_ret (bkt)"Think nothing of it.";
              16:  print_ret (bkt)"~Oops~ can only correct a single word.";
              17: print_ret "It",(string) IS__TX," pitch dark, and ",(ig)ICant(actor)," see a thing."; 
              18: print (ig)myself(actor); 
              19: "As good-looking as ever.";           
              20:  print_ret (bkt)"To repeat a command like ~frog, jump~, just say
                   ~again~, not ~frog, again~.";
              21: print_ret (ig)CICan(actor)," hardly repeat that."; 
              22:  print_ret (bkt)"You can't begin with a comma.";
              23:    bkt_o();
                    print "You seem to want ",(ig) fst("me ",""),"to talk to someone, but I can't see whom."; 
                    bkt_c();
                    print "^";
              24: print_ret (ig)CICant(actor)," talk to ", (the) x1, "."; 
              25:  print_ret (bkt) "To talk to someone, try ~someone, hello~ or some such.";
              26: "(first taking ", (the) not_holding, ")";
              27:  print_ret (bkt) "I didn't understand that sentence.";
              28:   bkt_o();
                    print "I only understood you as far as wanting to ";
              29:  print_ret (bkt)"I didn't understand that number.";
              30: print_ret (ig)CICant(actor)," see any such thing.";
              31:  print_ret (bkt)"You seem to have said too little!";
              32: print_ret (ig)CIAm(actor)," holding that!"; 
              32:  print_ret (ig)CIAm(actor)," not holding that! "; 
              33:  print_ret (bkt) "You can't use multiple objects with that verb.";
              34:  print_ret  (bkt)"You can only use multiple objects once on a line.";
              35:    bkt_o();
                    print "I'm not sure what ~", (address) pronoun_word,"~ refers to.";
                    bkt_c();
                    print"^";
              36:    bkt_o();
                    print "You excepted something not included anyway! "; 
                    bkt_c();
                    print "^";
              37:    bkt_o();
                    print "You can only do that to something animate."; 
                    bkt_c();
                    print "^";
              38: #ifdef DIALECT_US;
                  print_ret  (bkt)"That's not a verb I recognize.";
                  #ifnot;
                  print_ret  (bkt)"That's not a verb I recognise.";
                  #endif;
              39:    bkt_o();
                    print "That's not something you need to refer to in the course of this game.";
                    bkt_c();
                    print "^";
              40: print_ret  (ig)CICant(actor)," see ~", (address) pronoun_word,
                  "~ (", (the) pronoun_obj, ") at ",(ig)ppf("that","the","that")," moment.";
              41:  print_ret  (bkt)"I didn't understand the way that finished.";
              42: if (x1==0) print "None";
                  else print "Only ", (number) x1;
                  print " of those ";
                  if (x1==1) print "is"; else print "are";
                  " available.";
              43:  print_ret  (bkt)"Nothing to do!";
              44:  print_ret  (bkt)"There are none at all available!";
              45: bkt_o(); print "Who do you mean, ";
              46: bkt_o(); print "Which do you mean, ";
              47:    bkt_o();
                    print "Sorry, ",(ig)ICan(actor)," only have one item here. Which exactly?";
                    bkt_c();
                    new_line;
              48:    bkt_o();
                    print "Whom do you want";
                    if (actor~=player) print " ", (the) actor; print " to ";
                    PrintCommand(); print "?";
                    bkt_c();
                    print "^";
            49:        bkt_o();
                    print "What do you want";
                    if (actor~=player) print " ", (the) actor; print " to ";
                    PrintCommand(); print "?";
                    bkt_c();
                    print "^";
              50:     print "Your score has just gone ";
                  if (x1 > 0) print "up"; else { x1 = -x1; print "down"; }
                  print " by ", (number) x1, " point";
                  if (x1 > 1) print "s";
              51: print_ret (bkt)"Since something dramatic has happened,
                   your list of commands has been cut short.";
              52: "^Type a number from 1 to ", x1,
                  ", 0 to redisplay or press ENTER.";
              53: print_ret "^",(bkt)"Please press SPACE.";
              54: "[Comment recorded.]";
              55: "[Comment NOT recorded.]";
              56: print "."; bkt_c(); new_line;
              57: print "?"; bkt_c(); new_line;
           }
  No,Yes:        "That was a rhetorical question.";
  NotifyOff:       print_ret (bkt)"Score notification off.";
  NotifyOn:        print_ret (bkt)"Score notification on.";
  Objects: switch(n)
           {   1: print "Objects ",(ig)ihave(player)," handled:^";
               2: "None.";
               3: print "   (worn)";
               4: print "   (held)";
               5: print "   (given away)";
               6: print "   (in ", (name) x1, ")";
               7: print "   (in ", (the) x1, ")";
               8: print "   (inside ", (the) x1, ")";
               9: print "   (on ", (the) x1, ")";
              10: print "   (lost)";
           }
  Open:    switch(n)
           {   1: if(actor~=player) print (ig)CIVerb(actor,"tried","try","tries")," to open ",(the)x1,", but ", (theyreorthats) x1;
                    else print (ctheyreorthats) x1;
                print_ret " not something ",(ig)ICan(actor)," open.";
               2: if(actor~=player) print (ig)CIVerb(actor,"tried","try","tries")," to open ",(the)x1,", but it";
                    else print (The)x1;
                    print_ret " ",(ig)vrb(x1,"seemed","seem"),
                  " to be locked.";
               3: if(actor~=player) print (ig)CIVerb(actor,"tried","try","tries")," to open ",(the)x1,", but ", (theyreorthats) x1;
                    else print (ctheyreorthats) x1;
                    print_ret " already open.";
               4: print (ig)CIVerb(actor,"opened","open"), " ",(the) x1, ", revealing ";
                  if (WriteListFrom(child(x1),
                      ENGLISH_BIT + TERSE_BIT + CONCEAL_BIT)==0) "nothing.";
                  ".";
               5: print_ret (ig)CIVerb(actor,"opened","open"), " ",(the) x1, ".";
           }
  Order:          print (The) x1;
                  if (x1 has pluralname) print " have"; else print " has";
                  " better things to do.";
  Places:         
    switch (n) {   
        1: print (ig)CIHave(player)," visited: ";
        2: print ".^";
    }

  Pray:           print_ret "Nothing practical ",(ig)ppf("resulted","results","will result")," from ",(my) actor," prayer.";
  Prompt:  print "^>";
  Pronouns: switch(n)
           {   1: print "At the moment,";
               2: print "means ";
               3: print "is unset";
               4: "no pronouns are known to the game.";
               5:  ".";
           }
  Pull,Push,Turn:
           switch(n)
           {   1: print_ret (ig)CIAm(x1), " fixed in place.";
               2:     print_ret (CTheyreorThats)actor," unable to.";
               3: "Nothing obvious ",(ig)ppf("happened","happens","will happen"),".";
               4: "That ",(ig) ppf("would have been","would be","will be")," less than courteous.";
           }
! Push: see Pull
  PushDir: switch(n)
           {   1: print_ret (bkt)"Is that the best you can think of?";
               2: print_ret "That",(string) IS__TX," not a direction.";
               3: print_ret "Not that way ",(ICant) actor,".";
           }
  PutOn:   switch(n)
           {   1: print_ret (ig)CIVerb(actor,"needed","need")," to be holding ", (the) x1,
                  " before ",(ig)ICan(actor)," put ", (itorthem) x1,
                  " on top of something else.";
               2: print_ret (ig)CICant(actor)," put something on top of itself.";
               3: print_ret "Putting things on ", (the) x1, (ig) ppf(" would have achieved"," would achieve"," will achieve")," nothing.";
               4: print_ret (ig)CIVerb(actor,"lacked","lack")," the dexterity.";
               5: print_ret "(first taking ", (itorthem) x1, " off)^";
               6: print_ret "There",(string)IS__TX," no more room on ", (the) x1, ".";
               7: print_ret "Done.";
               8: print_ret (ig)CIVerb(actor,"put","put"), " ",(the) x1, " on ", (the) second, ".";
           } 
  Quit:    switch(n)
           {   1: print (bkt)"Please answer yes or no.";
               2: print (bkt)"Are you sure you want to quit? ";
           }
  Remove:  switch(n)
           {   
               !--1: print_ret (ig)CIAm(x1)," unfortunately closed."; !--although this is the actual translation, I believe that it is incorrect since it is actually checking the plurality of the noun (which is passed in) and not the container
               1: print_ret (ig)CIAm(parent(x1))," unfortunately closed.";
               2: print_ret "But ",(ig)IAm(x1)," not there now.";
               3: print_ret "Removed.";
           }
  Restart: switch(n)
           {   1: print (bkt)"Are you sure you want to restart? ";
               2: print_ret (bkt)"Failed.";
           }
  Restore: switch(n)
           {   1:  print_ret (bkt)"Restore failed.";
               2:  print_ret (bkt)"Ok.";
           }
  Rub:            print_ret (ig)CIVerb(actor,"achieved","achieve")," nothing by this.";
  Save:    switch(n)
           {   1:  print_ret (bkt)"Save failed.";
               2:  print_ret (bkt)"Ok.";
           }
  Score: switch (n) {
			1:  if (deadflag) print "In that game you scored "; else print "You have so far scored ";
				print score, " out of a possible ", MAX_SCORE, ", in ", turns, " turn";
			    if (turns ~= 1) print "s";
		        return;
	       2:  bkt_o(); print "There is no score in this story."; bkt_c(); new_line;
		}
  ScriptOff: switch(n)
           {   1:  print_ret (bkt)"Transcripting is already off.";
               2:    print_ret "^", (bkt)"End of transcript.";
               3:  print_ret (bkt)"Attempt to end transcript failed.";
           }
  ScriptOn: switch(n)
           {   1:  print_ret (bkt)"Transcripting is already on.";
               2:  print_ret (bkt)"Start of a transcript.";
               3:  print_ret (bkt)"Attempt to begin transcript failed.";
           }
  Search:  switch(n)
           {   1: "But it",(string)IS__TX," dark.";
               2: "There ",(ig) ppf("was","is","will be")," nothing on ", (the) x1, ".";
               3: print "On ", (the) x1;
                  WriteListFrom(child(x1),
                      TERSE_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT);
                  ".";
               4: print_ret (ig) CIVerb(actor,"found","find")," nothing of interest.";
               5: print_ret (ig) CICant(actor)," see inside, since ", (the) x1, " ",
                  (isorare) x1, " closed.";
               6: print_ret (The) x1, " ", (isorare) x1, " empty.";
               7: print "In ", (the) x1;
                  WriteListFrom(child(x1),
                      TERSE_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT);
                  ".";
           }
  Set:            print_ret "No, ", (ig) CICant(actor)," set ", (thatorthose) x1, ".";
  SetTo:          print_ret "No, ", (ig) CICant(actor)," set ", (thatorthose) x1, " to anything.";
  Show:    switch(n)
           {   1: print_ret (ig)CIAm(actor)," not holding ", (the) noun, ".";
               2: print_ret (ig)CIAm(x1)," unimpressed.";
           }
  Sing:           print_ret (CMy)actor," singing",(string) IS__TX," abominable.";
  Sleep:          print_ret (ig)CIAm(actor)," not feeling especially drowsy.";
  Smell:          print_ret (ig)CIVerb(actor,"smelled","smell")," nothing unexpected.";
  Sorry:          #ifdef DIALECT_US;
                  "Oh, don't apologize.";
                  #ifnot;
                  "Oh, don't apologise.";
                  #endif;
  Squeeze: switch(n)
           {   1: print_ret (bkt)"Keep your hands to yourself.";
               2: print_ret (ig)CIVerb(actor,"achieved","achieve")," nothing by this.";
           }
  Strong:         "Real adventurers do not use such language.";
  Swim:           print_ret "There",(string) IS__TX," not enough water to swim in.";
  Swing:          print_ret "There",(string) IS__TX," nothing sensible to swing here.";
  SwitchOff: switch(n)
           {   1: print_ret (ctheyreorthats) x1,
                  " not something ",(ig) ICan(actor)," switch.";
               2: print_ret (ctheyreorthats) x1,
                  " already off.";
               3: print_ret (ig)CIVerb(actor,"switched", "switch","switches")," ",(the) x1, " off.";
           }
  SwitchOn: switch(n)
           {   1: print_ret (ctheyreorthats) x1,
                  " not something ",(ig)ICan(actor)," switch.";
               2: print_ret (ctheyreorthats) x1,
                  " already on.";
               3: print_ret (ig)CIVerb(actor,"switched","switch","switches"), " ",(the) x1, " on.";
           }
  Take:    switch(n)
           {   1:	if(actor==player) print_ret "Taken."; 
					else print_ret (ig)CIVerb(actor,"took","take"), " ",(the)noun,"."; 
               2: print_ret (ig)CIAm(actor), " always self-possessed."; 
               3: print_ret (ig)CIVerb(actor,"did","do","does")," not suppose ", (the) x1, " would care for that.";
               4: print (CTheI) actor," would have ",(ig)ppf("had ",""),"to get ";
                  if (x1 has supporter) print "off "; else print "out of ";
                  print_ret (the) x1, " first.";
               5: print_ret (CIHave) actor," ",(thatorthose) x1, " already.";
               6: print (ig)CIVerb(noun,"seemed","seem");
                  " to belong to ", (the) x1, ".";
               7: print (ig)CIVerb(noun,"seemed","seem");
                  " to be a part of ", (the) x1, ".";
               8: print_ret (ig)CIAm(x1),
                  " not available.";
               9: print_ret (ig)CIAm(x1), " not open.";
              10: print_ret (ig)CIAm(x1), " hardly portable.";
              11: print_ret (ig)CIAm(x1), " fixed in place.";
              12: print_ret (ig)CIAm(actor), " carrying too many things already.";
              13: "(putting ", (the) x1, " into ", (the) SACK_OBJECT,
                  " to make room)";
           }
  Taste:          print_ret (ig)CIVerb(actor,"tasted","taste")," nothing unexpected.";
  Tell:    switch(n)
           {   1: print_ret (ig) CIVerb(actor,"talked","talk")," to ",(myself) actor," for a while.";
               2: "This ",(ig) ppf("provoked","provokes","will provoke")," no reaction.";
           }
  Think:          "What a good idea.";
  ThrowAt: switch(n)
           {   1: "Futile.";
               2: print_ret (ig)CIVerb(actor,"lacked","lack")," the nerve when it ",(ig) ppf("came","comes","comes")," to the crucial moment.";
           }
! Tie:  see JumpOver.
  Touch:   switch(n)
           {   1: print_ret (bkt)"Keep your hands to yourself!";
               2: print_ret (ig)CIVerb(actor,"felt","feel")," nothing unexpected.";
               3:    bkt_o();
                    print "If you think that",(ig) ppf(" would have helped.","'ll help.","'ll help.");
                    bkt_c();
                    print "^";
           }
! Turn: see Pull.
  Unlock:  switch(n)
           {   1: if(actor~=player) print (ig)CIVerb(actor,"tried","try","tries")," to unlock ",(the)x1,", but ", (thatorthose) x1;
                    else print (cthatorthose) x1;
                    print " ",(ig)vrb(x1,"did","do","does","will")," not";
                  " seem to be something that ",(ig)ppf("could","can")," be unlocked.";
               2: if(actor~=player) print (ig)CIVerb(actor,"tried","try","tries")," to unlock ",(the)x1,", but ", (theyreorthats) x1;
                    else print (ctheyreorthats) x1;
                  " unlocked.";
               3: if(actor~=player) print (ig)CIVerb(actor,"tried","try","tries")," to unlock ",(the)x1,", but ",(ig)IVerb(x1,"did","do","does","will")," not ";
                    else print (ig)CIVerb(x1,"did","do","does","will")," not ";
                  "seem to fit the lock.";
               4: print_ret (ig)CIVerb(actor,"unlocked","unlock")," ", (the) x1, "."; 
           }
  VagueGo:        print_ret (bkt)"You'll have to say which compass direction to go in.";
  Verify:  switch(n)
           {   1:  print_ret (bkt)"The game file has verified as intact.";
               2:  print_ret (bkt)"The game file did not verify as intact,
                   and may be corrupt.";
           }
  Wait:           "Time ",(ig) ppf("passed","passes","will pass"),".";
  Wake:           print_ret "The dreadful truth ",(ig) ppf("was","is","will be"),", ",(ig) IAm(actor)," not a dreaming.";
  WakeOther:      print_ret "That seem",(ig) ppf("ed","s")," unnecessary.";
  Wave:    switch(n)
           {   1:    print_ret "But ",(CTheyreorThats) actor," not holding ", (thatorthose) x1, ".";
               2:    print_ret (ig) CIVerb(actor,"looked","look")," ridiculous waving ", (the) x1, ".";
           }
  WaveHands:      print_ret (ig) CIVerb(actor,"waved","wave"),", feeling foolish.";
  Wear:    switch(n)
           {   1: print_ret (ig)CICant(actor)," wear ", (thatorthose) x1, "! ";
               2: print_ret (CTheyreorThats) actor," not holding ", (thatorthose) x1, "! ";
               3: print_ret (CTheyreorThats) actor," already wearing ", (thatorthose) x1, "! ";
               4: print_ret (ig)CIVerb(actor,"put","put")," on ", (the) x1, ".";
           }
];
! ------------------------------------------------------------------------------
!   ORLibrary specific section 2
! ------------------------------------------------------------------------------
    [SetPersonTense person tense;
        if(person~=0) NarativePerson=person;    
        if(tense~=0) NarativeTense=tense;
        switch(NarativePerson){
            FIRST_PERSON:
                FORMER__TX   = "my former self";
                YOURSELF__TX = "myself";
                YOU__TX = "I";
                switch(NarativeTense)
                {
                    PAST_TENSE:CANTGO__TX="I couldn't go that way.";
                    PRESENT_TENSE:CANTGO__TX="I can't go that way.";
                }
            SECOND_PERSON:
                FORMER__TX   = "your former self";
                YOURSELF__TX = "yourself";
                switch(NarativeTense)
                {
                    PAST_TENSE:CANTGO__TX="You couldn't go that way.";
                    PRESENT_TENSE:CANTGO__TX="You can't go that way.";
                }
                YOU__TX = "You";
            THIRD_PERSON:
                if(player has neuter){
                    FORMER__TX   = "its former self";
                    YOURSELF__TX = "itself";
                    YOU__TX = "It";
                    switch(NarativeTense){
                        PAST_TENSE:CANTGO__TX="It couldn't go that way.";
                        PRESENT_TENSE:CANTGO__TX="It can't go that way.";
                    }
                }else{
                    if(player has female){
                        FORMER__TX   = "her former self";
                        YOURSELF__TX = "herself";
                        YOU__TX = "She";
                        switch(NarativeTense){
                            PAST_TENSE:CANTGO__TX="She couldn't go that way.";
                            PRESENT_TENSE:CANTGO__TX="She can't go that way.";
                        }
                    }
                    else{
                        FORMER__TX   = "his former self";
                        YOURSELF__TX = "himself";
                        YOU__TX = "He";
                        switch(NarativeTense){
                            PAST_TENSE:CANTGO__TX="He couldn't go that way.";
                            PRESENT_TENSE:CANTGO__TX="He can't go that way.";
                        }
                    }
                }
        }
        switch(NarativeTense)
        {
            PAST_TENSE:
                IS__TX       = " was";
                ARE__TX      = " were";
                IS2__TX      = "was ";
                ARE2__TX     = "were ";
            PRESENT_TENSE:
                IS__TX       = " is";
                ARE__TX      = " are";
                IS2__TX      = "is ";
                ARE2__TX     = "are ";
            FUTURE_TENSE:
                IS__TX       = " will be";
                ARE__TX      = " will be";
                IS2__TX      = "will be ";
                ARE2__TX     = "will be ";
        }
        ChangeDefault(cant_go, CANTGO__TX);
    ];