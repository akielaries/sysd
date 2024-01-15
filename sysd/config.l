%{
#include "y.tab.h"
%}

DIGIT [0-9]
ID [a-zA-Z][a-zA-Z0-9]*
WS [ \t]

%%
{WS}+               /* skip whitespace */
{ID}                { yylval.strval = strdup(yytext); return ID; }
{DIGIT}+            { yylvalintval = atoi(yytext); return INTEGER; }
=                   { return EQUALS; }
\n                  { return NEWLINE; }
.                   { return yytext[0]; }
%%

int yywrap() {
    return 1;
}

