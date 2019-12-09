%{
    #include "ciLisp.h"
%}

%union {
    double dval;
    char *sval;
    struct ast_node *astNode;
    struct symbol_table_node *symNode;
}

%token <sval> FUNC SYMBOL
%token <dval> INT_LITERAL DOUBLE_LITERAL

%token LPAREN RPAREN EOL QUIT LET INT DOUBLE COND LAMBDA

%type <astNode> s_expr f_expr symbol number s_expr_list
%type <symNode> let_section let_list let_elem arg_list

%%

program:
    s_expr EOL {
        fprintf(stderr, "yacc: program ::= s_expr EOL\n");
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
        }
    };

s_expr:
    number {
        fprintf(stderr, "yacc: s_expr ::= number\n");
        $$ = $1;
    }
    | symbol{
        fprintf(stderr,"yacc s_expr :: = SYMBOL\n");
        $$ = $1;
    }
    | f_expr {
    	fprintf(stderr,"yacc s_expr :: = f_expr\n");
        $$ = $1;
    }
    | QUIT {
        fprintf(stderr, "yacc: s_expr ::= QUIT\n");
        exit(EXIT_SUCCESS);
    }
    | error {
        fprintf(stderr, "yacc: s_expr ::= error\n");
        yyerror("unexpected token");
        $$ = NULL;
    }
    | LPAREN let_section s_expr RPAREN{
    	fprintf(stderr, "yacc: s_expr ::= LPAREN let_section s_expr RPAREN \n");
        $$ = linkScopeToLetList($2, $3);
    }
    | LPAREN COND s_expr s_expr s_expr RPAREN{
	fprintf(stderr, "yacc: s_expr ::= LPAREN s_expr s_expr s_expr RPAREN \n");
    	$$ = createCondNode($3, $4, $5);
    }
    | LPAREN s_expr RPAREN{
    	fprintf(stderr, "yacc: s_expr ::= LPAREN s_expr RPAREN \n");
    	$$ = $2;
    };


number:
    INT_LITERAL {
        fprintf(stderr, "yacc: number ::= INT\n");
        $$ = createNumberNode($1, INT_TYPE);
    }
    | INT INT_LITERAL {
    	fprintf(stderr, "yacc: number ::= INT_TYPE INT\n");
    	$$ = createNumberNode($2, INT_TYPE);
    }
    | DOUBLE INT_LITERAL {
	fprintf(stderr, "yacc: number ::= DOUBLE_TYPE INT\n");
	$$ = createNumberNode($2, DOUBLE_TYPE);
    }
    | DOUBLE_LITERAL {
   	 fprintf(stderr, "yacc: number ::= DOUBLE\n");
         $$ = createNumberNode($1, DOUBLE_TYPE);
    }
    | INT DOUBLE_LITERAL {
    	fprintf(stderr, "yacc: number ::= INT_TYPE DOUBLE\n");
	$$ = createNumberNode($2, INT_TYPE);
    }
    | DOUBLE DOUBLE_LITERAL {
	fprintf(stderr, "yacc: number ::= DOUBLE_TYPE DOUBLE\n");
    	$$ = createNumberNode($2, DOUBLE_TYPE);
    };

s_expr_list:
	s_expr s_expr_list{
		fprintf(stderr,"yacc: s_expr_list ::= s_expr s_expr_list\n");
		$$ = linkSExprList($1,$2);
	}
	| s_expr {
		fprintf(stderr,"yacc: s_expr_list ::= s_expr \n");
		$$ = $1;
	}
	| /* EMPTY */ {
        		fprintf(stderr,"yacc: s_expr_list ::= EMPTY\n");
        		$$ = NULL;
        	};


f_expr:
    LPAREN FUNC s_expr_list RPAREN {
        fprintf(stderr, "yacc: f_expr ::= LPAREN FUNC s_expr_list RPAREN\n");
        $$ = createFunctionNode($2, $3);
    }
    | LPAREN FUNC RPAREN{
    	fprintf(stderr, "yacc: f_expr ::= LPAREN FUNC RPAREN\n");
    	$$ = createFunctionNode($2, NULL);
    }
    | LPAREN SYMBOL s_expr_list RPAREN {
    	fprintf(stderr, "yacc: f_expr ::= LPAREN SYMBOL s_expr_list RPAREN\n");
    	$$ = createFunctionNode($2, $3);
    };

let_section:
    LPAREN LET let_list RPAREN {
         fprintf(stderr," yacc: let_section ::= LPAREN LET let_list RPAREN \n");
        $$ = $3;
    }
    | /* EMPTY */ {
	fprintf(stderr, "yacc: let_section ::= <EMPTY> \n");
	$$ = NULL;
     };

let_list:
    let_elem {
        fprintf(stderr," yacc: let_list ::= let_elem\n");
        $$ = $1;
    }
    | let_list let_elem{
        fprintf(stderr," yacc: let_list ::= let_list let_elem\n");
        $$ = linkSymbolTable($1, $2);
    };

let_elem:
    LPAREN INT SYMBOL s_expr RPAREN {
        fprintf(stderr," yacc:  let_elem ::= LPAREN INT_TYPE SYMBOL s_expr RPAREN \n");
        $$ = createSymTableNode($3, $4, INT_TYPE);
    }
    | LPAREN DOUBLE SYMBOL s_expr RPAREN {
	fprintf(stderr," yacc:  let_elem ::= LPAREN DOUBLE_TYPE SYMBOL s_expr RPAREN \n");
	$$ = createSymTableNode($3, $4, DOUBLE_TYPE);
    }
    | LPAREN SYMBOL s_expr RPAREN {
    	fprintf(stderr," yacc:  let_elem ::= LPAREN SYMBOL s_expr RPAREN \n");
    	$$ = createSymTableNode($2, $3, NULL_TYPE);
    }
    | LPAREN SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr_list RPAREN {
        fprintf(stderr," yacc:  let_elem ::= LPAREN SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN \n");
        $$ = createCustomFuncNode(NULL_TYPE,$2,$5,$7);
    }
    | LPAREN INT SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr_list RPAREN {
	fprintf(stderr," yacc:  let_elem ::= LPAREN TYPE SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr_list RPAREN \n");
        $$ = createCustomFuncNode(INT_TYPE,$3,$6,$8);
    }
    | LPAREN DOUBLE SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr_list RPAREN {
     	fprintf(stderr," yacc:  let_elem ::= LPAREN TYPE SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr_list RPAREN \n");
        $$ = createCustomFuncNode(DOUBLE_TYPE,$3,$6,$8);
    };
arg_list:
	SYMBOL arg_list{
	$$ = createArgNode($1,$2);
	}
	| SYMBOL{
	$$ = createArgNode($1,NULL);
	};

symbol:
    SYMBOL {
        fprintf(stderr," yacc: symbol ::= SYMBOL\n");
        $$ = createAST_SymbolNode($1);
    };

%%

