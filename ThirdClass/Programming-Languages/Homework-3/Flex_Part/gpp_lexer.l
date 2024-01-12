
%{

    #include <stdio.h>
    #include <stdlib.h>

    #include "gpp_interpreter.h" // interpreter generates this file

    int is_op_cc = 0;  // keep track open or close parenthesis
     
    /* int isKeywordUsed = 0;  This line for lisp syntax */
    /* int isEqualUsed = 0; this line for lisp syntax */
%}

/* Tokens */
COMMENT     [;]{2}.*
VALUEF      [-]*[0-9]+[0-9]*+[b]+[0-9]+[0-9]*
IDENTIFIER  [a-zA-Z][a-zA-Z0-9]*
FILENAME    [a-zA-Z0-9_.+]+

%%
    /* KEYWORDS */

and     {return KW_AND;}
or      {return KW_OR;}
not     {return KW_NOT;}
equal   {return KW_EQUAL;}
less    {return KW_LESS;}
nil     {return KW_NIL;}
list    {return KW_LIST;}
append  {return KW_APPEND;}
concat  {return KW_CONCAT;}
set     {return KW_SET;}
def     {return KW_DEF;} /* in pdf it checks def but deffun given to*/
deffun  {return KW_DEF;} /* in pdf it checks def but deffun given to*/
for     {return KW_FOR;}
if      {return KW_IF;}
exit    {return KW_EXIT;}
EXIT    {return KW_EXIT;}
Exit    {return KW_EXIT;}
load    {return KW_LOAD;}
disp    {return KW_DISP;}
true    {return KW_TRUE;}
false   {return KW_FALSE;}

    /* OPERATORS */

[+]     {return OP_PLUS;}
[-]     {return OP_MINUS;}
[/]     {return OP_DIV;}
[*]     {return OP_MULT;}
[(]     {return OP_OP;}
[)]     {return OP_CP;}
[,]     {return OP_COMMA;}


    /* VALUE - COMMENT - IDENTIFIER */

    /* Comment Line Definition */
{COMMENT}     { return COMMENT; }
    /* LITERALS: Special Case of Float Number */
{VALUEF}     {yylval.string = strdup(yytext); return VALUEF;}

    /* Other Definitions */
{IDENTIFIER}   { yylval.string = strdup(yytext); return IDENTIFIER; }

    /* String Definition */
{FILENAME}     { yylval.string = strdup(yytext); return FILENAME; }

    /* do not care space tabs newlines */
[\t \n] {;}

    /* Syntax Error */
. {printf("Syntax Error. '%s' is not suitiable for tokenize \n", yytext); exit(1);}


%%
