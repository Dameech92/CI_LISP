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

typedef struct symbol_table_node {
    char *ident;
    struct ast_node *val;
    struct symbol_table_node *next;
} SYMBOL_TABLE_NODE;


typedef enum {
    NUM_NODE_TYPE,
    FUNC_NODE_TYPE,
    SYMBOL_NODE_TYPE
} AST_NODE_TYPE;

// Types of numeric values
typedef enum {
    INT_TYPE = 0,
    DOUBLE_TYPE
} NUM_TYPE;

// Node to store a number.
typedef struct {
    NUM_TYPE type;

    double value;
} NUM_AST_NODE;

// Values returned by eval function will be numbers with a type.
// They have the same structure as a NUM_AST_NODE.
// The line below allows us to give this struct another name for readability.
typedef NUM_AST_NODE RET_VAL;
// num ast parse?
//eval retval?

// Node to store a function call with its inputs
typedef struct {
    OPER_TYPE oper;
    char* ident; // only needed for custom functions
    struct ast_node *op1;
    struct ast_node *op2;
} FUNC_AST_NODE;

typedef struct symbol_ast_node {
    char *ident;
} SYMBOL_AST_NODE;

// Generic Abstract Syntax Tree node. Stores the type of node,
// and reference to the corresponding specific node (initially a number or function call).
typedef struct ast_node {
    AST_NODE_TYPE type;
    SYMBOL_TABLE_NODE *symbolTable;
    struct ast_node *parent;
    union {
        NUM_AST_NODE number;
        SYMBOL_AST_NODE symbol;
        FUNC_AST_NODE function;
    } data;
} AST_NODE;

AST_NODE *createNumberNode(double value, NUM_TYPE type);

AST_NODE *createFunctionNode(char *funcName, AST_NODE *op1, AST_NODE *op2);

//adding new stuff below here
AST_NODE *createAST_SymbolNode (char *id);

AST_NODE *linkScopeToLetList(SYMBOL_TABLE_NODE *letLList, AST_NODE *scope);

SYMBOL_TABLE_NODE *createSymTableNode(char *id, AST_NODE *value);

SYMBOL_TABLE_NODE *linkSymbolTable(SYMBOL_TABLE_NODE *prevHead, SYMBOL_TABLE_NODE *newHead);



void freeNode(AST_NODE *node);

RET_VAL eval(AST_NODE *node);
RET_VAL evalNumNode(NUM_AST_NODE *numNode);
RET_VAL evalFuncNode(FUNC_AST_NODE *funcNode);
RET_VAL evalSymbolNode(AST_NODE *symNode);

//added helper methods for debugging assistance
RET_VAL neg_Helper(FUNC_AST_NODE *funcNode);
RET_VAL abs_Helper(FUNC_AST_NODE *funcNode);
RET_VAL exp_Helper(FUNC_AST_NODE *funcNode);
RET_VAL sqrt_Helper(FUNC_AST_NODE *funcNode);
RET_VAL add_Helper(FUNC_AST_NODE *funcNode);
RET_VAL subtract_Helper(FUNC_AST_NODE *funcNode);
RET_VAL multiply_Helper(FUNC_AST_NODE *funcNode);
RET_VAL divide_Helper(FUNC_AST_NODE *funcNode);
RET_VAL remainder_Helper(FUNC_AST_NODE *funcNode);
RET_VAL log_Helper(FUNC_AST_NODE *funcNode);
RET_VAL pow_Helper(FUNC_AST_NODE *funcNode);
RET_VAL max_Helper(FUNC_AST_NODE *funcNode);
RET_VAL min_Helper(FUNC_AST_NODE *funcNode);
RET_VAL exp2_Helper(FUNC_AST_NODE *funcNode);
RET_VAL cbrt_Helper(FUNC_AST_NODE *funcNode);
RET_VAL hypot_Helper(FUNC_AST_NODE *funcNode);



void printRetVal(RET_VAL val);

#endif
