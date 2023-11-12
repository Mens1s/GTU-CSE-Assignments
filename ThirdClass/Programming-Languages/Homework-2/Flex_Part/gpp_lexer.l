/* Definition Section */

%{
    /* int isKeywordUsed = 0;  This line for lisp syntax */
    /* int isEqualUsed = 0; this line for lisp syntax */
%}
      
/* End of Definition Section */

/* Pattern Action / Rules */ 
%%
    
    /* Keyword Definition */

and     {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_AND\n");}
or      {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_OR\n");}
not     {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_NOT\n");}
equal   {/*isKeywordUsed = 1; isEqualUsed = 1;This line for lisp syntax */ printf("KW_EQUAL\n");}
less    {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_LESS\n");}
nil     {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_NIL\n");}
list    {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_LIST\n");}
append  {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_APPEND\n");}
concat  {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_TCONCAT\n");}
set     {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_SET\n");}
deffun  {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_FUN\n");}
for     {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_FOR\n");}
if      {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_IF\n");}
exit    {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_EXIT\n"); return 0;}
EXIT    {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_EXIT\n"); return 0;}
Exit    {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_EXIT\n"); return 0;}
load    {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_LOAD\n");}
disp    {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_DISP\n");}
true    {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_TRUE\n");}
false   {/*isKeywordUsed = 1;This line for lisp syntax */ printf("KW_FALSE\n");}

    /* Operators Definition */

[+]     {printf("OP_PLUS\n");/* isKeywordUsed = 1;*/}
[-]     {printf("OP_MINUS\n");/* isKeywordUsed = 1;*/}
[/]     {printf("OP_DIV\n"); /*isKeywordUsed = 1;*/}
[*]     {printf("OP_MULT\n");/* isKeywordUsed = 1;*/}
[(]     {printf("OP_OP\n"); }
[)]     {printf("OP_CP\n");/* isKeywordUsed = 0;*/}
[,]     {printf("OP_COMMA\n");/* isKeywordUsed = 1;*/}

  /* Comment Line Definition */

[;]{2}.* { printf("COMMENT\n"); }                   /* ;; double ; mean that it is comment section */

    /* LITERALS: Special Case of Float Number */

[0-9]*+[b]+[0-9]*    {printf("VALUEF\n");}          /* Literals section from pdf */
  
    /* Other Definitions */

[a-zA-Z][a-zA-Z0-9]* {/*if(isKeywordUsed == 1) */ printf("IDENTIFIER\n");/* else{ printf("Syntax Error. '%s' is not suitiable for tokenize \n", yytext); return 0;}*/}    /* token is valid statement */
[0-9]+[a-zA-Z]*      {/*if(isKeywordUsed == 1)*/ printf("NOT_VALID\nSyntax Error. '%s' is not suitiable for tokenize \n", yytext); return 0; /*else{ printf("Syntax Error. '%s' is not suitiable for tokenize \n", yytext); return 0;}*/}   /* token is not valid if it contains number and character    */

    /* do not care space tabs newlines */

[\t \n] {;}

    /* Syntax Error */
. {printf("Syntax Error. '%s' is not suitiable for tokenize \n", yytext); return 0;}


%%
/* end of pattern section */

int yywrap() {}

int main(int argc, char**argv)
{
    if (argc == 1) {
        printf("Type exit to finish...\n");
        printf("Enter your string:");
    }
    else if (argc == 2) {
        FILE *filePointer;
        filePointer = fopen(argv[1], "r");
        yyin = filePointer;
    }
    else {
        printf("Not permitted arguments.\n");
        return 0;
    }

    yylex();
    return 0;
}