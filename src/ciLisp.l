%option noyywrap
%option nounput
%option noinput

%{
    #include "ciLisp.h"
%}
letter [a-zA-Z]
symbol {letter}+
digit [0-9]
int_literal [+-]?{digit}+
double_literal [+-]?{digit}+\.{digit}*
func "neg"|"abs"|"exp"|"sqrt"|"add"|"sub"|"mult"|"div"|"remainder"|"log"|"pow"|"max"|"min"|"exp2"|"cbrt"|"hypot"|"print"|"rand"|"read"|"equal"|"less"|"greater"
%%

"(" {
    fprintf(stderr, "lex: LPAREN\n");
    return LPAREN;
    }

")" {
    fprintf(stderr, "lex: RPAREN\n");
    return RPAREN;
    }
"quit" {
    return QUIT;
    }

"let" {
    fprintf(stderr, "lex: LET\n");
    return LET;
    }

"int" {
    fprintf(stderr, "lex: INT_TYPE\n");
    return INT;
    }

"double" {
    fprintf(stderr, "lex: DOUBLE_TYPE\n");
    return DOUBLE;
    }
"cond"  {
    fprintf(stderr, "lex: COND\n");
    return COND;
    }
"lambda" {
    fprintf(stderr, "lex: LAMBDA\n");
    return LAMBDA;
}
{int_literal} {
    yylval.dval = strtod(yytext, NULL);
    fprintf(stderr, "lex: INT dval = %lf\n", yylval.dval);
    return INT_LITERAL;
}

{double_literal} {
    yylval.dval = strtod(yytext, NULL);
    fprintf(stderr, "lex: DOUBLE dval = %lf\n", yylval.dval);
    return DOUBLE_LITERAL;
}

{func} {
    yylval.sval = (char *) malloc(strlen(yytext)*sizeof(char)+1);
    strcpy(yylval.sval, yytext);
    fprintf(stderr, "lex: FUNC sval = %s\n", yylval.sval);
    return FUNC;
    }

{symbol} {
    yylval.sval = (char *) malloc(strlen(yytext)*sizeof(char)+1);
    strcpy(yylval.sval, yytext);
    fprintf(stderr, "lex: SYMBOL sval = %s\n", yylval.sval);
    return SYMBOL;
    }

[\n] {
    fprintf(stderr, "lex: EOL\n");
    YY_FLUSH_BUFFER;
    return EOL;
    }

[ |\t] ; /* skip whitespace */

. { // anything else
    printf("ERROR: invalid character: >>%s<<\n", yytext);
    }

%%

/*
 * DO NOT CHANGE THE FOLLOWING CODE!
 */
int main(void) {

       freopen("/dev/null", "w", stderr); // except for this line that can be uncommented to throw away debug printouts

    char *s_expr_str = NULL;
    size_t s_expr_str_len = 0;
    YY_BUFFER_STATE buffer;
    while (true) {
        printf("\n> ");
        getline(&s_expr_str, &s_expr_str_len, stdin);
        s_expr_str[s_expr_str_len++] = '\0';
        s_expr_str[s_expr_str_len++] = '\0';
        buffer = yy_scan_buffer(s_expr_str, s_expr_str_len);
        yyparse();
        yy_delete_buffer(buffer);
    }
}
