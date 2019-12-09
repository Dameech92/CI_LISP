/*
 * ciLisp task 1-8.5 done
 * Dimitri Cognata
 * version 8.5
 * Monday December 9, 2019
 *
 *
 * */

#include "ciLisp.h"
#include "limits.h"


#define DEFAULT_RET_VAL ((RET_VAL){INT_TYPE, NAN})

#define MAX_NARY_ARGS INT_MAX

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
RET_VAL rand_Helper(AST_NODE *node);
RET_VAL read_Helper(AST_NODE *node);
RET_VAL equal_Helper(FUNC_AST_NODE *funcNode);
RET_VAL less_Helper(FUNC_AST_NODE *funcNode);
RET_VAL greater_Helper(FUNC_AST_NODE *funcNode);
RET_VAL printHelper(FUNC_AST_NODE *funcNode);

//task5 addition for the opList and for seperating the math functions
RET_VAL evalZeroAry(AST_NODE *root);
RET_VAL evalUnary(FUNC_AST_NODE *funcNode);
RET_VAL evalBinary(FUNC_AST_NODE *funcNode);
RET_VAL evalNary(FUNC_AST_NODE *funcNode);

typedef enum arity_flag {
    NULLARY,
    ZEROARY,
    UNARY,
    BINARY,
    NARY
} FUNCTION_ARITY;


void yyerror(char *s) {
    fprintf(stderr, "\nERROR: %s\n", s);
    // note stderr that normally defaults to stdout, but can be redirected: ./src 2> src.log
    // CLion will display stderr in a different color from stdin and stdout
}

// Array of string values for operations.
// Must be in sync with funcs in the OPER_TYPE enum in order for resolveFunc to work.
char *funcNames[] = {
        "neg",
        "abs",
        "exp",
        "sqrt",
        "add",
        "sub",
        "mult",
        "div",
        "remainder",
        "log",
        "pow",
        "max",
        "min",
        "exp2",
        "cbrt",
        "hypot",
        "read",
        "rand",
        "print",
        "equal",
        "less",
        "greater",
        ""
};

OPER_TYPE resolveFunc(char *funcName) {
    int i = 0;
    while (funcNames[i][0] != '\0') {
        if (strcmp(funcNames[i], funcName) == 0)
            return i;
        i++;
    }
    return CUSTOM_OPER;
}

AST_NODE *createNewNode(AST_NODE_TYPE type) {
    AST_NODE *node = calloc(1, sizeof(AST_NODE));
    if (node == NULL) {
        yyerror("Memory allocation failed!");
        return NULL;
    }
    node->type = type;
    return node;
}

SYMBOL_TABLE_NODE *createNewSymbol() {
    SYMBOL_TABLE_NODE *node = calloc(1, sizeof(SYMBOL_TABLE_NODE));
    if (node == NULL) {
        yyerror("Memory allocation failed!");
        return NULL;
    }
    return node;
}

// Called when an INT or DOUBLE token is encountered (see ciLisp.l and ciLisp.y).
// Creates an AST_NODE for the number.
// Sets the AST_NODE's type to number.
// Populates the value of the contained NUMBER_AST_NODE with the argument value.
// SEE: AST_NODE, NUM_AST_NODE, AST_NODE_TYPE.
AST_NODE *createNumberNode(double value, NUM_TYPE type) {
    AST_NODE *node = createNewNode(NUM_NODE_TYPE);
    // TODO set the AST_NODE's type, assign values to contained NUM_AST_NODE DONE
    node->data.number.type = type;
    node->data.number.value = (type == INT_TYPE) ? floor(value) : value;
    return node;
}

// Called when an f_expr is created (see ciLisp.y).
// Creates an AST_NODE for a function call.
// Sets the created AST_NODE's type to function.
// Populates the contained FUNC_AST_NODE with:
//      - An OPER_TYPE (the enum identifying the specific function being called)
//      - 2 AST_NODEs, the operands
// SEE: AST_NODE, FUNC_AST_NODE, AST_NODE_TYPE.
AST_NODE *createFunctionNode(char *funcName, AST_NODE *opList) {
    AST_NODE *node = createNewNode(FUNC_NODE_TYPE);
    // TODO set the AST_NODE's type, populate contained FUNC_AST_NODE DONE
    node->data.function.ident = funcName;
    node->data.function.oper = resolveFunc(funcName);
    node->data.function.opList = opList;
    AST_NODE *elem = opList;
    while (elem != NULL) {
        elem->parent = node;
        elem = elem->next;
    }
    return node;
}

AST_NODE *createAST_SymbolNode(char *id) {
    //TODO create the AST symbolNode
    AST_NODE *node = createNewNode(SYMBOL_NODE_TYPE);
    node->data.symbol.ident = id;
    return node;
}

SYMBOL_TABLE_NODE *createSymTableNode(char *id, AST_NODE *value, NUM_TYPE number) {
    //TODO create the symbol table node here DONE
    SYMBOL_TABLE_NODE *node = createNewSymbol();
    node->val_type = number;
    node->ident = id;
    node->val = value;
    return node;
}

AST_NODE *linkSExprList(AST_NODE *sExpr, AST_NODE *sExprList) {
    //TODO links the ast sExpr to the letList DONE
    sExpr->next = sExprList;
    return sExpr;
}

//good example of how to use pointers
AST_NODE *linkScopeToLetList(SYMBOL_TABLE_NODE *letList, AST_NODE *s_expr) {
    //TODO links the scope AST_NODE to the letList DONE
    SYMBOL_TABLE_NODE * linker = letList;
    if (letList == NULL)
        return s_expr;
    SYMBOL_TABLE_NODE **scope = &(s_expr->symbolTable);
    while (*scope) {
        scope = &((*scope)->next);
    }
    (*scope) = letList;
    while(linker != NULL) {

        linker->val->parent = s_expr;
        linker = linker->next;
    }
    return s_expr;
}

AST_NODE *createCondNode(AST_NODE *cond, AST_NODE *truth, AST_NODE *notTruth) {

    AST_NODE *node = createNewNode(COND_NODE_TYPE);
    cond->parent = node;
    truth->parent = node;
    notTruth->parent = node;
    node->data.condition.cond = cond;
    node->data.condition.doWhenTrue = truth;
    node->data.condition.doWhenFalse = notTruth;

    return node;
}


SYMBOL_TABLE_NODE *linkSymbolTable(SYMBOL_TABLE_NODE *prevHead, SYMBOL_TABLE_NODE *newHead) {
    //TODO set the new head to the previous one DONE
    newHead->next = prevHead;
    return newHead;
}

void freeFuncOpList(AST_NODE *opList) {
    if (opList == NULL)
        return;
    freeFuncOpList(opList->next);
    freeNode(opList);
}

void freeFuncNode(AST_NODE *node) {
    free(node->data.function.ident);
    freeFuncOpList(node->data.function.opList);
}

void freeSymNode(AST_NODE *node) {
    free(node->data.symbol.ident);
}

void freeSymTableNode(SYMBOL_TABLE_NODE *node) {
    if (node == NULL)
        return;
    freeSymTableNode(node->next);
    free(node->ident);
    freeNode(node->val);
    free(node);
}

// Called after execution is done on the base of the tree.
// (see the program production in ciLisp.y)
// Recursively frees the whole abstract syntax tree.
// You'll need to update and expand freeNode as the project develops.
void freeNode(AST_NODE *node) {
    if (!node)
        return;

    if (node->type == FUNC_NODE_TYPE)
        freeFuncNode(node);

    if (node->type == SYMBOL_NODE_TYPE)
        freeSymNode(node);

    freeSymTableNode(node->symbolTable);

    free(node);
}

// Evaluates an AST_NODE.
// returns a RET_VAL storing the the resulting value and type.
// You'll need to update and expand eval (and the more specific eval functions below)
// as the project develops.
RET_VAL eval(AST_NODE *node) {
    if (!node)
        return (RET_VAL) {INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN}; // see NUM_AST_NODE, because RET_VAL is just an alternative name for it.

    // TODO complete the switch. DONE so far..
    // Make calls to other eval functions based on node type.
    // Use the results of those calls to populate result.
    switch (node->type) {

        case FUNC_NODE_TYPE:
            result = evalFuncNode(node);
            break;
        case NUM_NODE_TYPE:
            result = evalNumNode(&node->data.number);
            break;
        case SYMBOL_NODE_TYPE:
            result = evalSymbolNode(&node->data.symbol, node);
            break;
        case COND_NODE_TYPE:
            result = evalCondNode(&node->data.condition);
            break;
        default:
            yyerror("Invalid AST_NODE_TYPE, probably invalid writes somewhere!");
    }

    return result;
}

// returns a pointer to the NUM_AST_NODE (aka RET_VAL) referenced by node.
// DOES NOT allocate space for a new RET_VAL.
RET_VAL evalNumNode(NUM_AST_NODE *numNode) {
    if (!numNode)
        return (RET_VAL) {INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    // TODO populate result with the values stored in the node. DONE
    // SEE: AST_NODE, AST_NODE_TYPE, NUM_AST_NODE
    result.type = numNode->type;
    result.value = numNode->value;

    return result;
}

//task 5
int getArity(FUNC_AST_NODE *node) {
    switch (node->oper) {

        case READ_OPER:
        case RAND_OPER:
            return ZEROARY;

        case NEG_OPER:
        case ABS_OPER:
        case EXP_OPER:
        case SQRT_OPER:
        case CBRT_OPER:
        case EXP2_OPER:
            return UNARY;

        case POW_OPER:
        case MAX_OPER:
        case MIN_OPER:
        case HYPOT_OPER:
       // case READ_OPER:
        //case RAND_OPER:
        case EQUAL_OPER:
        case LESS_OPER:
        case LOG_OPER:
        case GREATER_OPER:
        case DIV_OPER:
        case REMAINDER_OPER:
        case SUB_OPER:
            return BINARY;

        case PRINT_OPER:
        case MULT_OPER:
        case ADD_OPER:
            return NARY;

        default:
            return NULLARY;
    }
}

RET_VAL evalZeroAry(AST_NODE *root){
    RET_VAL result = DEFAULT_RET_VAL;

    switch(root->data.function.oper){
        case READ_OPER:
            result = read_Helper(root);
            break;
        case RAND_OPER:
            result = rand_Helper(root);
            break;
        default:
            break;
    }
    return result;
}

RET_VAL evalUnary(FUNC_AST_NODE *funcNode) {
    RET_VAL result = DEFAULT_RET_VAL;
    if (funcNode->opList == NULL) {
        printf(" ERROR: too few parameters for the function %s", funcNode->ident);
        return result;
    }
    if (funcNode->opList->next != NULL) {
        printf("WARNING: too many parameters for the function %s\n", funcNode->ident);
    }

    switch (funcNode->oper) {
        case NEG_OPER:
            result = neg_Helper(funcNode);
            break;
        case ABS_OPER:
            result = abs_Helper(funcNode);
            break;
        case EXP_OPER:
            result = exp_Helper(funcNode);
            break;
        case SQRT_OPER:
            result = sqrt_Helper(funcNode);
            break;
        case LOG_OPER:
            result = log_Helper(funcNode);
            break;
        case EXP2_OPER:
            result = exp2_Helper(funcNode);
            break;
        case CBRT_OPER:
            result = cbrt_Helper(funcNode);
            break;
        default:
            break;
    }
    return result;
}


RET_VAL evalBinary(FUNC_AST_NODE *funcNode) {

    RET_VAL result = DEFAULT_RET_VAL;
    if (funcNode->opList == NULL) {
        printf(" ERROR: too few parameters for the function %s", funcNode->ident);
        return result;
    }
    if (funcNode->opList->next->next != NULL) {
        printf("WARNING: too many parameters for the function %s\n", funcNode->ident);
    }
    switch (funcNode->oper) {

        case SUB_OPER:
            result = subtract_Helper(funcNode);
            break;
        case DIV_OPER:
            result = divide_Helper(funcNode);
            break;
        case REMAINDER_OPER:
            result = remainder_Helper(funcNode);
            break;
        case LOG_OPER:
            result = log_Helper(funcNode);
            break;
        case POW_OPER:
            result = pow_Helper(funcNode);
            break;
        case MAX_OPER:
            result = max_Helper(funcNode);
            break;
        case MIN_OPER:
            result = min_Helper(funcNode);
            break;
        case HYPOT_OPER:
            result = hypot_Helper(funcNode);
            break;
        case EQUAL_OPER:
            result = equal_Helper(funcNode);
            break;
        case LESS_OPER:
            result = less_Helper(funcNode);
            break;
        case GREATER_OPER:
            result = greater_Helper(funcNode);
            break;

        default:
            break;
    }

    return result;

}

RET_VAL evalNary(FUNC_AST_NODE *funcNode) {
    RET_VAL result = DEFAULT_RET_VAL;
    if (funcNode->opList == NULL) {
        printf(" ERROR: too few parameters for the function %s", funcNode->ident);
        return result;
    }


    switch (funcNode->oper) {
        case ADD_OPER:
            result = add_Helper(funcNode);
            break;
        case MULT_OPER:
            result = multiply_Helper(funcNode);
            break;
        case PRINT_OPER:
            result = printHelper(funcNode);
            break;
        default:
            printf("error in evalNary wrong OPER");
            break;
    }

    return result;
}

RET_VAL evalFuncNode(AST_NODE *node) {
    if (!node)
        return (RET_VAL) {INT_TYPE, NAN};

    FUNC_AST_NODE *funcNode = &(node->data.function);
    RET_VAL result = {INT_TYPE, NAN};

    if(funcNode->oper == CUSTOM_OPER){

        SYMBOL_TABLE_NODE * function = findSymbolTable(funcNode->ident,node);
        AST_NODE * currentVar = funcNode->opList;
        SYMBOL_TABLE_NODE * someVar = function->val->symbolTable;

        RET_VAL_HOLDER * evalOperands = calloc(sizeof(RET_VAL_HOLDER),1);
        RET_VAL_HOLDER * head = evalOperands;
        while(currentVar != NULL){

            head->currentRetVal = eval(currentVar);
            currentVar = currentVar->next;
            if(currentVar != NULL){
                head->next = calloc(sizeof(RET_VAL_HOLDER),1);
                head = head->next;
            }
        }
        while(evalOperands != NULL){

            someVar->stack = evalOperands->currentRetVal;
            someVar = someVar->next;
            evalOperands = evalOperands->next;

        }

        result = eval(function->val);
    }else {

        int arity = getArity(funcNode);
        switch (arity) {
            case ZEROARY:
                result = evalZeroAry(node);
                break;
            case UNARY:
                result = evalUnary(funcNode);
                break;
            case BINARY:
                result = evalBinary(funcNode);
                break;
            case NARY:
                result = evalNary(funcNode);
                break;
            default:
                break;
        }
    }
    //Maybe some flooring for int result type 
    return result;
}

NUM_TYPE resolveType(NUM_TYPE a, NUM_TYPE b) {
    if (a == DOUBLE_TYPE || b == DOUBLE_TYPE)
        return DOUBLE_TYPE;
    if (a == INT_TYPE || b == INT_TYPE)
        return INT_TYPE;
    return NULL_TYPE;
}

RET_VAL evalSymbolNode(SYMBOL_AST_NODE * symbolNode, AST_NODE * parent) {
    //Must have node to do work
    if (!symbolNode)
        return DEFAULT_RET_VAL;

    //Must find symbol to eval
    SYMBOL_TABLE_NODE *symbol = findSymbolTable(symbolNode->ident, parent);
    if (symbol == NULL) {
        yyerror("Unexpected identifier");
        return DEFAULT_RET_VAL;
    }
//add here
    RET_VAL result ;

    switch(symbol->type){

        case VARIABLE_TYPE:
            result = evalVariableTypeNode(symbol);
            break;
        case ARG_TYPE:
            result = evalArgTypeNode(symbol);
            break;
    }

    //Do you lose precision after eval?
    if (symbol->val_type == INT_TYPE && result.type == DOUBLE_TYPE) {
        fprintf(stdout, "WARNING: precision loss in the assignment for variable %s\n", symbol->ident);
        result.value = floor(result.value);
    }

    //Determine resulting type
    result.type = resolveType(result.type, symbol->val_type);

    return result;
}



RET_VAL evalArgTypeNode(SYMBOL_TABLE_NODE * argType){

    if(!argType)
        return DEFAULT_RET_VAL;

    return argType->stack;


}

RET_VAL evalVariableTypeNode(SYMBOL_TABLE_NODE * variableType){
    if(!variableType)
        return DEFAULT_RET_VAL;
    RET_VAL result = eval(variableType->val);

    variableType->val->type = NUM_NODE_TYPE;
    variableType->val->data.number.value = result.value;
    variableType->val->data.number.type = result.type;

    return result;
}


RET_VAL evalCondNode(COND_AST_NODE *condNode) {

    if (!condNode)
        return DEFAULT_RET_VAL;

    RET_VAL result = eval(condNode->cond);

    if (result.value) {
        result = eval(condNode->doWhenTrue);
    } else {
        result = eval(condNode->doWhenFalse);

    }
    return result;
}

SYMBOL_TABLE_NODE *findSymbolTable(char *ident, AST_NODE * current) {

    //Had daddy issues
    if (current == NULL)
        return NULL;

    SYMBOL_TABLE_NODE *scope = current->symbolTable;
    while (scope != NULL) {
        if (strcmp(scope->ident, ident) == 0)
            return scope;
        scope = scope->next;
    }

    return findSymbolTable(ident, current->parent);
}

SYMBOL_TABLE_NODE *createCustomFuncNode(NUM_TYPE type, char * symbol, SYMBOL_TABLE_NODE * argList, AST_NODE * sExprList ){

    SYMBOL_TABLE_NODE * customFuncNode = createNewSymbol();
    customFuncNode->type = LAMBDA_TYPE;
    customFuncNode->val_type = type;
    customFuncNode->ident = symbol;
    customFuncNode->val = sExprList;
    sExprList->symbolTable = argList;


    return customFuncNode;
}

SYMBOL_TABLE_NODE *createArgNode(char * argSymbol, SYMBOL_TABLE_NODE * argList){

    SYMBOL_TABLE_NODE * customArgNode = createNewSymbol();
    customArgNode->val_type = NULL_TYPE;
    customArgNode->type =  ARG_TYPE;
    customArgNode->ident = argSymbol;
    customArgNode->next = argList;
//    while( argList != NULL){
//
//        customArgNode->next = argList;
//        argList = argList->next;
//
//    }

    return  customArgNode;
}


RET_VAL neg_Helper(FUNC_AST_NODE *funcNode) {
    //TODO negate the negOp DONE
    RET_VAL negResult = {INT_TYPE, NAN};
    RET_VAL negOp = eval(funcNode->opList);

    //set the type to the negOp
    negResult.type = negOp.type;
    //now set the value to a negative number
    negResult.value = -negOp.value;

    return negResult;
}

RET_VAL abs_Helper(FUNC_AST_NODE *funcNode) {
    // TODO implement abs() for double and int types DONE
    RET_VAL absResult = {INT_TYPE, NAN};
    RET_VAL absOp = eval(funcNode->opList);
    //access and set the type
    absResult.type = absOp.type;
    //now set the values do the math then round if its and int
    if (absResult.value == INT_TYPE) {
        absResult.value = round(fabs(absOp.value));
    } else {
        absResult.value = fabs(absOp.value);
    }

    return absResult;
}

RET_VAL exp_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement exp() for double and int types DONE
    RET_VAL expResult = {INT_TYPE, NAN};
    RET_VAL expOp = eval(funcNode->opList);

    //access and set the type
    expResult.type = DOUBLE_TYPE;
    //now set the value and do the calculations
    expResult.value = exp(expOp.value);

    return expResult;
}

RET_VAL sqrt_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement sqrt() for double and int types DONE
    RET_VAL sqrtResult = {INT_TYPE, NAN};
    RET_VAL sqrtOp = eval(funcNode->opList);
    //set result to double
    sqrtResult.type = DOUBLE_TYPE;
    //then set thh result value to a sqrt
    sqrtResult.value = sqrt(sqrtOp.value);

    return sqrtResult;
}

RET_VAL add_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement add operation DONE
    RET_VAL addResult = {NULL_TYPE, 0.0};
    AST_NODE *args = funcNode->opList;
    do {
        RET_VAL temp = eval(args);
        addResult.value += temp.value;
        addResult.type = resolveType(addResult.type, temp.type);
        args = args->next;
    } while (args != NULL);

    return addResult;
}

RET_VAL subtract_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement subtract operation DONE
    RET_VAL subResult = {INT_TYPE, NAN};
    RET_VAL minusOp1 = eval(funcNode->opList);
    RET_VAL minusOp2 = eval(funcNode->opList->next);

    subResult.type = minusOp1.type || minusOp2.type;
    //if its an int make sure its not a double by rounding
    if (subResult.type == INT_TYPE) {

        subResult.value = round(minusOp1.value - minusOp2.value);
    } else {
        subResult.value = minusOp1.value - minusOp2.value;
    }
    return subResult;
}

RET_VAL multiply_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement add operation DONE
    RET_VAL multResult = {NULL_TYPE, 1.0};
    AST_NODE *args = funcNode->opList;
    do {
        RET_VAL temp = eval(args);
        multResult.value *= temp.value;
        multResult.type = resolveType(multResult.type, temp.type);
        args = args->next;
    } while (args != NULL);

    return multResult;
}

RET_VAL divide_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement divide operation DONE
    RET_VAL divResult = {INT_TYPE, NAN};
    RET_VAL divOp1 = eval(funcNode->opList);
    RET_VAL divOp2 = eval(funcNode->opList->next);

    divResult.type = divOp1.type || divOp2.type;
    //if its an int make sure its not a double by rounding
    if (divResult.type == INT_TYPE) {
        divResult.value = round(divOp1.value / divOp2.value);
    } else {
        divResult.value = divOp1.value / divOp2.value;
    }
    return divResult;
}

RET_VAL remainder_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement mod operation DONE
    RET_VAL modResult = {INT_TYPE, NAN};
    RET_VAL modOp1 = eval(funcNode->opList);
    RET_VAL modOp2 = eval(funcNode->opList->next);

    modResult.type = modOp1.type || modOp2.type;
    //if its an int make sure its not a double by rounding
    if (modResult.type == INT_TYPE) {
        modResult.value = round(remainder(modOp1.value, modOp2.value));
    } else {
        modResult.value = remainder(modOp1.value, modOp2.value);
    }
    return modResult;
}

RET_VAL log_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement log operation DONE
    RET_VAL logResult = {INT_TYPE, NAN};
    RET_VAL logOp1 = eval(funcNode->opList);
    //set the results type to the operands type
    logResult.type = logOp1.type;
    //now calculate its value and set its result
    logResult.value = log(logOp1.value);

    return logResult;
}

//pow(double x, double y)
RET_VAL pow_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement POW operation DONE
    RET_VAL powResult = {INT_TYPE, NAN};
    RET_VAL powOp1 = eval(funcNode->opList);
    RET_VAL powOp2 = eval(funcNode->opList->next);
    //set the type to the x.type
    powResult.type = powOp1.type;
    if (powResult.type == INT_TYPE) {
        //pow(double x, double y)
        // x raised to the power y
        //so powOp1raised to the power of powOp2
        powResult.value = round(pow(powOp1.value, powOp2.value));
    } else {
        powResult.value = pow(powOp1.value, powOp2.value);
    }

    return powResult;
}


//the fmax() functions return x or y, whichever is bigger.
RET_VAL max_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement max operation DONE
    RET_VAL maxResult = {INT_TYPE, NAN};
    RET_VAL maxOp1 = eval(funcNode->opList);
    RET_VAL maxOp2 = eval(funcNode->opList);

    maxResult.type = maxOp1.type || maxOp2.type;

    if (maxResult.type == INT_TYPE) {
        //the fmax() functions return x or y, whichever is bigger.
        //set the result to the max
        maxResult.value = round(fmax(maxOp1.value, maxOp2.value));
    } else {
        //the fmax() functions return x or y, whichever is bigger.
        //set the result to the max
        maxResult.value = fmax(maxOp1.value, maxOp2.value);

    }

    return maxResult;
}

//the fmin() functions return x or y, whichever is smaller.
RET_VAL min_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement min operation DONE
    RET_VAL minResult = {INT_TYPE, NAN};
    RET_VAL minOp1 = eval(funcNode->opList);
    RET_VAL minOp2 = eval(funcNode->opList->next);

    minResult.type = minOp1.type || minOp2.type;

    if (minResult.type == INT_TYPE) {
        //the fmin() functions return x or y, whichever is smaller.
        minResult.value = round(fmin(minOp1.value, minOp2.value));
    } else {
        //the fmin() functions return x or y, whichever is smaller.
        minResult.value = fmin(minOp1.value, minOp2.value);
    }

    return minResult;
}

//The exp2() function computes 2**x, the base-2 exponential of x.
RET_VAL exp2_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement exp2 operation DONE
    RET_VAL exp2Result = {INT_TYPE, NAN};
    RET_VAL exp2Op1 = eval(funcNode->opList);

    exp2Result.type = exp2Op1.type;
    if (exp2Result.type == INT_TYPE) {
        exp2Result.value = round(exp2(exp2Op1.value));
    } else {
        exp2Result.value = exp2(exp2Op1.value);
    }

    return exp2Result;
}

//The cbrt() function computes the cube root of x.
RET_VAL cbrt_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement cbrt aka cubed root  operation DONE
    RET_VAL cbrtResult = {INT_TYPE, NAN};
    RET_VAL cbrtOp1 = eval(funcNode->opList);

    cbrtResult.type = cbrtOp1.type;

    if (cbrtResult.type == INT_TYPE) {
        cbrtResult.value = round(cbrt(cbrtOp1.value));
    } else {
        cbrtResult.value = cbrt(cbrtOp1.value);
    }

    return cbrtResult;
}

RET_VAL hypot_Helper(FUNC_AST_NODE *funcNode) {
    //TODO implement subtract operation DONE
    RET_VAL hypotResult = {INT_TYPE, NAN};
    RET_VAL hypotOp1 = eval(funcNode->opList);
    RET_VAL hypotOp2 = eval(funcNode->opList->next);

    hypotResult.type = hypotOp1.type || hypotOp2.type;

    if (hypotResult.type == INT_TYPE) {

        hypotResult.value = round(hypot(hypotOp1.value, hypotOp2.value));
    } else {

        hypotResult.value = hypot(hypotOp1.value, hypotOp2.value);
    }

    return hypotResult;
}

RET_VAL printHelper(FUNC_AST_NODE *funcNode) {
    printf("=> ");
    RET_VAL retVal = DEFAULT_RET_VAL;
    AST_NODE *list = funcNode->opList;
    do {
        retVal = eval(list);

        if (retVal.type == INT_TYPE)
            printRetVal(retVal);
        else
            printRetVal(retVal);
            list = list->next;
    } while (list != NULL);
    printf("\n");
    return retVal;
}

RET_VAL rand_Helper(AST_NODE *node){

    RET_VAL result = {DOUBLE_TYPE,(double)rand()/RAND_MAX};
    node->type = NUM_NODE_TYPE;
    node->data.number = result;

    return  result;
}

RET_VAL read_Helper(AST_NODE *node){

    RET_VAL readResult = {DOUBLE_TYPE,NAN};

    //char buffer
    char buffer[128];

    printf("read := ");
    scanf("%s", buffer);
    getchar();
    bool isDouble = false;

    for(int i = 0; buffer[i] != '\0'; ++i){

        switch(buffer[i]){

            case'0'...'9':
                break;
            case '.':
                if(!isDouble){
                    isDouble = true;
                    break;
                }
            case'-':
                if(i==0){
                    break;
                }
                break;
            default:
                yyerror("invalid, error during input of expression!");
                node->type = NUM_NODE_TYPE;
                node->data.number = readResult;
                return readResult;
        }
    }
    readResult.value = strtod(buffer,NULL);
    if(!isDouble){
        readResult.type = INT_TYPE ;
    }
    node->type = NUM_NODE_TYPE;
    node->data.number = readResult;

    return readResult;
}

RET_VAL equal_Helper(FUNC_AST_NODE *funcNode){
    RET_VAL equalResult = {INT_TYPE, NAN};
    RET_VAL equalOp1 = eval(funcNode->opList);
    RET_VAL equalOp2 = eval(funcNode->opList->next);

    equalResult.value = (equalOp1.value ==  equalOp2.value);

    return equalResult;

}

RET_VAL less_Helper(FUNC_AST_NODE *funcNode){
    RET_VAL lessResult = {INT_TYPE, NAN};
    RET_VAL lessOp1 = eval(funcNode->opList);
    RET_VAL lessOp2 = eval(funcNode->opList->next);

    lessResult.value = (lessOp1.value < lessOp2.value);

    return lessResult;
}

RET_VAL greater_Helper(FUNC_AST_NODE *funcNode){
    RET_VAL greaterResult = {INT_TYPE, NAN};
    RET_VAL greaterOp1 = eval(funcNode->opList);
    RET_VAL greaterOp2 = eval(funcNode->opList->next);

    greaterResult.value = greaterOp1.value > greaterOp2.value;

    return greaterResult;

}

// prints the type and value of a RET_VAL
void printRetVal(RET_VAL val) {
    // TODO if val type is int or double cast accordingly and then print
    switch (val.type) {

        case INT_TYPE:
            //if its an in case it to a long to make sure its value isnt a double
            printf("Integer value is %ld\n", (long) val.value);
            break;
        case NULL_TYPE:
            printf("Null Value %.2lf\n", val.value);
            break;
        case DOUBLE_TYPE:
            printf("Double Value %.2lf\n", val.value);
            break;
    }
}