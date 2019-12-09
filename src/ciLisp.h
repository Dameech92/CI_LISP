#ifndef __cilisp_h_
#define __cilisp_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "ciLispParser.h"

int yyparse(void);

int yylex(void);

void yyerror(char *);

// Enum of all operators.
// must be in sync with funcs in resolveFunc()
typedef enum oper {
    NEG_OPER, // 0
    ABS_OPER,//1
    EXP_OPER,//2
    SQRT_OPER,//3
    ADD_OPER,//4
    SUB_OPER,//5
    MULT_OPER,//6
    DIV_OPER,//7
    REMAINDER_OPER,//8
    LOG_OPER,//9
    POW_OPER,//10
    MAX_OPER,//11
    MIN_OPER,//12
    EXP2_OPER,//13
    CBRT_OPER,//14
    HYPOT_OPER,//15
    READ_OPER,//16
    RAND_OPER,//17
    PRINT_OPER,//18
    EQUAL_OPER,//19
    LESS_OPER,//20
    GREATER_OPER,//21
    CUSTOM_OPER =255//22
} OPER_TYPE;

OPER_TYPE resolveFunc(char *);


// Types of Abstract Syntax Tree nodes.
// Initially, there are only numbers and functions.
// You will expand this enum as you build the project.
// Types of numeric values
typedef enum {
    INT_TYPE,
    DOUBLE_TYPE,
    NULL_TYPE
} NUM_TYPE;

typedef enum { VARIABLE_TYPE, LAMBDA_TYPE, ARG_TYPE } SYMBOL_TYPE;

typedef struct stack_node {
    struct ast_node *val;
    struct stack_node *next;
} STACK_NODE;




typedef enum {
    NUM_NODE_TYPE,
    FUNC_NODE_TYPE,
    SYMBOL_NODE_TYPE,
    COND_NODE_TYPE
} AST_NODE_TYPE;

// Node to store a number.
typedef struct {
    NUM_TYPE type;
    double value;
} NUM_AST_NODE;

// Values returned by eval function will be numbers with a type.
// They have the same structure as a NUM_AST_NODE.
// The line below allows us to give this struct another name for readability.
typedef NUM_AST_NODE RET_VAL;

typedef struct symbol_table_node {
    SYMBOL_TYPE type;
    NUM_TYPE val_type;
    char * ident;
    struct ast_node * val;
    RET_VAL stack;
    struct symbol_table_node * next;
} SYMBOL_TABLE_NODE;
// num ast parse?
//eval retval?

// Node to store a function call with its inputs
typedef struct {
    OPER_TYPE oper;
    char* ident; // only needed for custom functions
    //HEY!! WHY DO I NEED TO DELETE THIS???
    struct ast_node *opList;
} FUNC_AST_NODE;

typedef struct symbol_ast_node {
    char *ident;
} SYMBOL_AST_NODE;


typedef struct {
    struct ast_node * cond;
    struct ast_node * doWhenTrue; // to eval if cond is nonzero
    struct ast_node * doWhenFalse; // to eval if cond is zero
} COND_AST_NODE;

// Generic Abstract Syntax Tree node. Stores the type of node,
// and reference to the corresponding specific node (initially a number or function call).
typedef struct ast_node {
    AST_NODE_TYPE type;
    SYMBOL_TABLE_NODE * symbolTable;
    struct ast_node * parent;
    union {
        NUM_AST_NODE number;
        SYMBOL_AST_NODE symbol;
        FUNC_AST_NODE function;
        COND_AST_NODE condition;
    } data;
    struct ast_node *next;
} AST_NODE;

typedef struct ret_val_holder {

    RET_VAL currentRetVal;
    struct ret_val_holder *next;


}RET_VAL_HOLDER;

AST_NODE * createNumberNode(double value, NUM_TYPE type);
AST_NODE * createFunctionNode(char * funcName, AST_NODE * opList);
AST_NODE * createAST_SymbolNode (char * id);
AST_NODE *linkSExprList(AST_NODE * sExpr, AST_NODE * sExprList);
AST_NODE *linkScopeToLetList(SYMBOL_TABLE_NODE * letList, AST_NODE * s_expr);
AST_NODE * createCondNode(AST_NODE * cond, AST_NODE * truth, AST_NODE * notTruth);

SYMBOL_TABLE_NODE *createArgNode(char * argSymbol, SYMBOL_TABLE_NODE * argList);
SYMBOL_TABLE_NODE *createCustomFuncNode(NUM_TYPE type, char * symbol, SYMBOL_TABLE_NODE * argList, AST_NODE * sExprList);
SYMBOL_TABLE_NODE *createSymTableNode(char * id, AST_NODE * value, NUM_TYPE type);
SYMBOL_TABLE_NODE *linkSymbolTable(SYMBOL_TABLE_NODE * prevHead, SYMBOL_TABLE_NODE * newHead);
SYMBOL_TABLE_NODE *findSymbolTable(char * ident, AST_NODE * s_expr);

RET_VAL eval(AST_NODE * node);
RET_VAL evalNumNode(NUM_AST_NODE * numNode);
RET_VAL evalFuncNode(AST_NODE * node);
RET_VAL evalSymbolNode(SYMBOL_AST_NODE * symNode, AST_NODE * parent);
RET_VAL evalCondNode(COND_AST_NODE * condNode);
RET_VAL evalArgTypeNode(SYMBOL_TABLE_NODE * argType);
RET_VAL evalVariableTypeNode(SYMBOL_TABLE_NODE * variableType);


void freeNode(AST_NODE * node);
void printRetVal(RET_VAL val);

#endif
