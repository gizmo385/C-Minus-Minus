%{
#include <stdio.h>
#include <stdlib.h>
#include "myhtml2txt.tab.h"
extern int mylineno;
extern int mycolno;
extern char *yytext;
%}

/* tokens */

%token DOC_START DOC_END
%token HEAD_START HEAD_END
%token BODY_START BODY_END
%token BF_START BF_END
%token IT_START IT_END
%token UL_START UL_END
%token OL_START OL_END
%token LI_START LI_END

%token TAG
%token SPACE
%token TEXT

%%

Doc     : Wspace DOC_START Wspace Head Wspace Body Wspace DOC_END Wspace;
Head    : HEAD_START Html HEAD_END;
Body    : BODY_START Html BODY_END;
Wspace  : SPACE
        |
        ;
Html    : Item Html
        |
        ;
Item    : BF_START Html BF_END
        | IT_START Html IT_END
        | List
        | Other
        ;
List    : UL_START Wspace ItemList Wspace UL_END
        | OL_START Wspace ItemList Wspace OL_END
        ;
ItemList: ItemList Wspace OneItem
        | OneItem
        ;
OneItem : LI_START Html LI_END;
Other   : TAG
        | TEXT                      { printf("%s", yytext); }
        | SPACE                     { printf("%s", yytext); }
        ;

%%

int main(int argc, char **argv){
   yyparse();
   return 0;
}

int yyerror() {
    fprintf(stderr, "%d:%d - Encountered error while parsing: \"%s\"\n", mylineno, mycolno, yytext);
    exit(1);
}
