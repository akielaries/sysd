%{
#include <stdio.h>
#include <stdlib.h>
extern int yylex();
extern FILE* yyin;
void yyerror(const char *s);

%}

%union {
    int intval;
    char* strval;
}

%token <intval> INTEGER
%token <strval> ID
%token EQUALS NEWLINE

%%

config: /* empty */
      | config statement
      ;

statement: ID EQUALS value NEWLINE {
    printf("%s = ", $1);
    if ($3.intval)
        printf("%d\n", $3.intval);
    else
        printf("%s\n", $3.strval);
    free($3.strval);
}
;

value: INTEGER { $$ = $1; }
    | ID { $$ = $1; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    yyin = fopen("test.conf", "r");
    if (!yyin) {
        perror("test.conf");
        return 1;
    }

    yyparse();

    fclose(yyin);
    return 0;
}

