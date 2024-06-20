%{
	#include <stdio.h>
	#include <string.h>
    #include <stdlib.h>
    extern FILE *yyin;

	int yylex(); 
	int yyerror(const char *error){
		printf("Syntax Error\n");
		exit(1);
	}
 
    char * functionArray[20][20];
    int functionCount = 0;

    int getFirstInt(char *a){
        int loc = -1;
        for (int i = 0; i < strlen(a); i++) {
            if (a[i] == 'b') {
                loc = i;
                break;
            }
        }
        char *temp = malloc(loc);
        for (int i = 0; i < loc; i++) {
            temp[i] = a[i];
        }
        int convertedInt = atoi(temp);
        return convertedInt;
    }
    int getSecondInt(char *a){
        int loc = -1;
        for (int i = 0; i < strlen(a); i++) {
            if (a[i] == 'b') {
                loc = i;
                break;
            }
        }
        char *temp = malloc(strlen(a) - loc);
        int j = 0;
        for (int i = loc + 1; i < strlen(a); i++) {
            temp[j] = a[i];
            j++;
        }
        int convertedInt = atoi(temp);
        return convertedInt;
    }
    void isFunctionParametherCountIsCorrect(int index, int parametherCount){
        int i = 0;
        while (strcmp(functionArray[index][i], "end") != 0) {
            i++;
        }
        if (i-2 == parametherCount) {
            return;
        }
        printf("Syntax Error! Function paramether count is not correct!\n");
        exit(1);
    }
    char * doOperation(char * a, char * b, char op){
        int aFirstInt = getFirstInt(a);
        int aSecondInt = getSecondInt(a);

        int bFirstInt = getFirstInt(b);
        int bSecondInt = getSecondInt(b);

        
        int num = (aFirstInt * bSecondInt) + (aSecondInt * bFirstInt);
        int denom = aSecondInt * bSecondInt;

        switch(op){
            case '+':
                num = (aFirstInt * bSecondInt) + (aSecondInt * bFirstInt);
                denom = aSecondInt * bSecondInt;
                break;
            case '-':
                num = (aFirstInt * bSecondInt) - (aSecondInt * bFirstInt);
                denom = aSecondInt * bSecondInt;
                break;
            case '*':
                num = aFirstInt * bFirstInt;
                denom = aSecondInt * bSecondInt;
                break;
            case '/':
                num = aFirstInt * bSecondInt;
                denom = aSecondInt * bFirstInt;
                if (denom == 0) {
                    printf("Syntax Error! Division by zero!\n");
                    exit(1);
                }
                break;
        }
        // simplify
        for (int i = 2; i <= num && i <= denom; i++) {
            if (num % i == 0 && denom % i == 0) {
                num = num / i;
                denom = denom / i;
            }
        }
        char * target = malloc(1000);
        sprintf(target, "%db%d", num, denom);
        target[strlen(target)] = '\0';
        return target;
    }
    void holdFunctionNoParameter(char * identifier, char * expression){
        // add function to functionList array
        functionArray[functionCount][0] = identifier;
        functionArray[functionCount][1] = expression;
        functionArray[functionCount][2] = "end";
        functionCount++;
    }
    void holdFunctionOneParameter(char * identifier, char * firstParameter, char * expression){
        functionArray[functionCount][0] = identifier;
        functionArray[functionCount][1] = firstParameter;
        functionArray[functionCount][2] = expression;
        functionArray[functionCount][3] = "end";
        functionCount++;
    }
    void holdFunctionTwoParameter(char * identifier, char * firstParameter, char * secondParameter, char * expression){
        functionArray[functionCount][0] = identifier;
        functionArray[functionCount][1] = firstParameter;
        functionArray[functionCount][2] = secondParameter;
        functionArray[functionCount][3] = expression;
        functionArray[functionCount][4] = "end";
        functionCount++;
    }
    void printResult(void* s){
        printf("Result: ");
        
        char **temp = (char*)s;
        char *str = (char*)temp[0];
        while (*str != '\0')
        {
            printf("%c", *str);
            str++;
        }
        printf("\n");
    }
    char * addString(char * a, char * b, char * c){
        char * target = malloc(1000);
        sprintf(target, "%s %s %s )", a, b, c);
        target[strlen(target)] = '\0';
        return target;
    }
    char * executeNoParametherFunction(char * identifier){
        char * target = malloc(1000);
        int loc = -1;
        for (int i = 0; i < functionCount; i++) {
            if (strcmp(functionArray[i][0], identifier) == 0) {
                target = functionArray[i][1];
                loc = i;
                break;
            }
        }
        if (loc == -1) {
            printf("Syntax Error! Function not found!\n");
            exit(1);
        }
        isFunctionParametherCountIsCorrect(loc, 0);
        return target;
    }
    char * executeOneParametherFunction(char * identifier, char * paramether){
        char target[1000];
        int loc = -1;
        for (int i = 0; i < functionCount; i++) {
            if (strcmp(functionArray[i][0], identifier) == 0) {
                strcpy(target, functionArray[i][2]);
                loc = i;
                break;
            }
        }
        if (loc == -1) {
            printf("Syntax Error! Function not found!\n");
            exit(1);
        }
        isFunctionParametherCountIsCorrect(loc, 1);
        char * temp = malloc(1000);
        char * token = strtok(target, " ");
        // split target and replace parameter functionArray[loc][0]
        while (token != NULL) {
            if (strcmp(token, functionArray[loc][1]) == 0) {
                token = paramether;
            }
            strcat(temp, token);
            strcat(temp, " ");
            token = strtok(NULL, " ");
        }
        temp[strlen(temp)] = '\0';

        return temp;
    }
    char * executeTwoParametherFunction(char * identifier, char * parametherFirst, char * parametherSecond){
        char target[1000];
        
        int loc = -1;
        for (int i = 0; i < functionCount; i++) {
            if (strcmp(functionArray[i][0], identifier) == 0) {
                strcpy(target, functionArray[i][3]);
                loc = i;
                break;
            }
        }
        if (loc == -1) {
            printf("Syntax Error! Function not found!\n");
            exit(1);
        }
        isFunctionParametherCountIsCorrect(loc, 2);
        char * temp = malloc(1000);
        char * token = strtok(target, " ");
        // split target and replace parameter functionArray[loc][0]
        while (token != NULL) {
            if (strcmp(token, functionArray[loc][1]) == 0) {
                token = parametherFirst;
            }
            if (strcmp(token, functionArray[loc][2]) == 0) {
                token = parametherSecond;
            }
            strcat(temp, token);
            strcat(temp, " ");
            token = strtok(NULL, " ");
        }
        temp[strlen(temp)] = '\0';
        return temp;
    }
    int countParathesis(char * expression){
        int count = 0;
        for (int i = 0; i < strlen(expression); i++) {
            if (expression[i] == '(') {
                count++;
            }
        }
        return count;
    }
    char * evaluateFunction(char * expression){
        // get first char
        char op = expression[0];
        // remove first char
        char * temp = malloc(strlen(expression) - 1);
        int j = 0;
        for (int i = 2; i < strlen(expression); i++) {
            temp[j] = expression[i];
            j++;
        }
        char * first = malloc(1000);
        char * second = malloc(1000);
        int k = -1;
        for(int l = 0; l < strlen(temp); l++){
            if(temp[l] == ' '){
                k++;
                continue;
            }
            if(k == -1){
                first[l] = temp[l];
            }else{
                second[k] = temp[l];
                k++;
            }   
        }
        first[strlen(first)] = '\0';
        second[strlen(second)] = '\0';
        char * result = doOperation(first, second, op);
        return result;
    }
    char * doOperationFunction(char * expression){
        // prepare expression for doOperation
        while (countParathesis(expression) > 0) {
            int lastOpenParathesis = -1;
            for(int idx = 0; idx < strlen(expression);idx++){
                if(expression[idx] == ')'){
                    // get string between lastOpenParathesis and idx
                    char * temp = malloc(idx - lastOpenParathesis);
                    int j = 0;
                    for (int i = lastOpenParathesis + 1; i < idx; i++) {
                        temp[j] = expression[i];
                        j++;
                    }
                    temp[strlen(temp)] = '\0';
                    char * result = evaluateFunction(temp);
                    char * temp2 = malloc(strlen(expression) - strlen(temp) + strlen(result));
                    int k = 0;
                    for (int i = 0; i < lastOpenParathesis; i++) {
                        temp2[k] = expression[i];
                        k++;
                    }
                    for (int i = 0; i < strlen(result); i++) {
                        temp2[k] = result[i];
                        k++;
                    }
                    for (int i = idx + 1; i < strlen(expression); i++) {
                        temp2[k] = expression[i];
                        k++;
                    }
                    temp2[strlen(temp2)] = '\0';
                    expression = temp2;
                }
                if(expression[idx] == '('){
                    lastOpenParathesis = idx;
                }
            }
        }
        return expression;
    }
    
%}
%union 
{
	char *string;
	char * fraction;
	int values[1000];
}

%token <string> IDENTIFIER FILENAME VALUEF
%token KW_AND KW_OR KW_NOT KW_EQUAL KW_LESS KW_NIL KW_LIST KW_APPEND KW_CONCAT KW_SET KW_DEF KW_FOR KW_IF KW_EXIT KW_LOAD KW_DISP KW_TRUE KW_FALSE
%token OP_PLUS OP_MINUS OP_DIV OP_MULT OP_OP OP_CP OP_COMMA COMMENT 

%start START

%type <fraction> INPUT
%type <fraction> EXP
%type <fraction> FUNCTION
%type <fraction> FUNCTION_INFO

%%
START: | START INPUT {};

INPUT: EXP { printResult(&$$); $$ = $1; }
    | FUNCTION { printf("Function definition parsed!\n"); }
    | OP_OP KW_EXIT OP_CP { printf("Exit command parsed!\n"); exit(1); }
    ;

EXP : OP_OP OP_PLUS EXP EXP OP_CP { $$ = doOperation($3, $4, '+'); }
    | OP_OP OP_MINUS EXP EXP OP_CP { $$ = doOperation($3, $4, '-'); }
    | OP_OP OP_MULT EXP EXP OP_CP { $$ = doOperation($3, $4, '*'); }
    | OP_OP OP_DIV EXP EXP OP_CP { $$ = doOperation($3, $4, '/'); }
    | OP_OP IDENTIFIER OP_CP { $$ = doOperationFunction(executeNoParametherFunction($2)); }
    | OP_OP IDENTIFIER EXP OP_CP { $$ = doOperationFunction(executeOneParametherFunction($2, $3)); }
    | OP_OP IDENTIFIER EXP EXP OP_CP { $$ = doOperationFunction(executeTwoParametherFunction($2, $3, $4));}
    | IDENTIFIER {  }
    | VALUEF { }
    ;

FUNCTION: OP_OP KW_DEF IDENTIFIER FUNCTION_INFO OP_CP { holdFunctionNoParameter($3, $4); $$="FUNCTION";}
        | OP_OP KW_DEF IDENTIFIER IDENTIFIER FUNCTION_INFO OP_CP {  holdFunctionOneParameter($3, $4, $5); $$="FUNCTION"; }
        | OP_OP KW_DEF IDENTIFIER IDENTIFIER IDENTIFIER FUNCTION_INFO OP_CP {  holdFunctionTwoParameter($3, $4, $5, $6); $$="FUNCTION"; }
        ;

FUNCTION_INFO:    OP_OP OP_PLUS FUNCTION_INFO FUNCTION_INFO OP_CP { $$ = addString("(+", $3, $4); }
                | OP_OP OP_MINUS FUNCTION_INFO FUNCTION_INFO OP_CP { $$ = addString("(-", $3, $4 );  }
                | OP_OP OP_MULT FUNCTION_INFO FUNCTION_INFO OP_CP { $$ = addString("(*", $3, $4 );  }
                | OP_OP OP_DIV FUNCTION_INFO FUNCTION_INFO OP_CP { $$ = addString("(/", $3, $4 );   }
                | IDENTIFIER {  }
                | VALUEF { }
                ;
%%

int main(int argc, char *argv[])
{
    
    if (argc == 1)
    {
        printf("Type (exit) for exit\n");
        printf("Enter your input\n");
    }
    else if (argc == 2)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL)
        {
            printf("File not found\n");
            return 0;
        }
    }
    else 
    {
        printf("Undefined start call... Please look at the documentation\n");
        return 0;
    }
    yyparse(); // Start parsing
    return 0;
}