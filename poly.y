%{
    #include <stdio.h>
    void yyerror(const char *s);
    int yylex();    
%}

%code requires {
    #include "polyutils.h"
    extern Node* treeroot;
}

%union{
    char charVal;
    Node *node;
}

%token <charVal> DIGIT
%token PLUS MINUS POW ONE ZERO x

%type <node> S P T X N M

%start S

%%

S: 
    P {
        $$ = createNode('S');
        addChild($$, $1);
        treeroot = $$;     
    }
    |PLUS P {
        $$ = createNode('S');
        addChild($$, createNode('+'));
        addChild($$, $2);
        treeroot = $$;  
        
    }
    |MINUS P {
        $$ = createNode('S');
        addChild($$, createNode('-'));
        addChild($$, $2);
        treeroot = $$;  
        
    }
    ;

P:
    T {
        $$ = createNode('P');
        addChild($$, $1);
    }
    | T PLUS P {
        $$ = createNode('P');
        addChild($$, $1);
        addChild($$, createNode('+'));
        addChild($$, $3);
    }
    | T MINUS P {
        $$ = createNode('P');
        addChild($$, $1);
        addChild($$, createNode('-'));
        addChild($$, $3);
    }
    ;

T:
    ONE {
        $$ = createNode('T');
        addChild($$, createNode('1'));
    }
    | N {
        $$ = createNode('T');
        addChild($$, $1);
    }
    | X {
        $$ = createNode('T');
        addChild($$, $1);
    }
    | N X {
        $$ = createNode('T');
        addChild($$, $1);
        addChild($$, $2);
    }
    ;

X:
    x {
        $$ = createNode('X');
        addChild($$, createNode('x'));
    }
    | x POW N {
        $$ = createNode('X');
        addChild($$, createNode('x'));
        addChild($$, createNode('^'));
        addChild($$, $3);
    }
    ;

N:
    DIGIT {
        $$ = createNode('N');
        addChild($$, createNode($1));
    }
    | ONE M {
        $$ = createNode('N');
        addChild($$, createNode('1'));
        addChild($$, $2);
    }
    | DIGIT M {
        $$ = createNode('N');
        addChild($$, createNode($1));
        addChild($$, $2);
    }
    ;

M:
    ZERO {
        $$ = createNode('M');
        addChild($$, createNode('0'));
    }
    | ONE {
        $$ = createNode('M');
        addChild($$, createNode('1'));
    }
    | DIGIT {
        $$ = createNode('M');
        addChild($$, createNode($1));
    }
    | ZERO M {
        $$ = createNode('M');
        addChild($$, createNode('0'));
        addChild($$, $2);
    }
    | ONE M {
        $$ = createNode('M');
        addChild($$, createNode('1'));
        addChild($$, $2);
    }
    | DIGIT M {
        $$ = createNode('M');
        addChild($$, createNode($1));
        addChild($$, $2);
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parser Error : %s\n", s);
}