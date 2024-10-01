%{
    #include "poly.tab.h"
%}

%%

"+" { return PLUS;}
"-" { return MINUS;}
"^" { return POW;}
"1" { return ONE;}
"0" { return ZERO;}
[2-9] { yylval.charVal = yytext[0] ;return DIGIT;}
"x" {return x;}
[ \t\n]+ {}
. {}

%%

int yywrap(void) {
    return 1;
}