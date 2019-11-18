#include "ciLisp.h"

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

OPER_TYPE resolveFunc(char *funcName)
{
    int i = 0;
    while (funcNames[i][0] != '\0')
    {
        if (strcmp(funcNames[i], funcName) == 0)
            return i;
        i++;
    }
    return CUSTOM_OPER;
}

// Called when an INT or DOUBLE token is encountered (see ciLisp.l and ciLisp.y).
// Creates an AST_NODE for the number.
// Sets the AST_NODE's type to number.
// Populates the value of the contained NUMBER_AST_NODE with the argument value.
// SEE: AST_NODE, NUM_AST_NODE, AST_NODE_TYPE.
AST_NODE *createNumberNode(double value, NUM_TYPE type)
{
    AST_NODE *node;
    size_t nodeSize;

    // allocate space for the fixed size and the variable part (union)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, assign values to contained NUM_AST_NODE DONE
    node->data.number.type = type;
    node->type = NUM_NODE_TYPE;
    //populate this with the type that was past in and the value
    switch(type){

        case INT_TYPE:
           node->data.number.value = value;
            break;
        case DOUBLE_TYPE:
            node->data.number.value = value;
            break;

    }

    return node;
}

// Called when an f_expr is created (see ciLisp.y).
// Creates an AST_NODE for a function call.
// Sets the created AST_NODE's type to function.
// Populates the contained FUNC_AST_NODE with:
//      - An OPER_TYPE (the enum identifying the specific function being called)
//      - 2 AST_NODEs, the operands
// SEE: AST_NODE, FUNC_AST_NODE, AST_NODE_TYPE.
AST_NODE *createFunctionNode(char *funcName, AST_NODE *op1, AST_NODE *op2) {
    AST_NODE *node;
    size_t nodeSize;

    // allocate space (or error)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, populate contained FUNC_AST_NODE DONE
    // NOTE: you do not need to populate the "ident" field unless the function is type CUSTOM_OPER.
    // When you do have a CUSTOM_OPER, you do NOT need to allocate and strcpy here.
    // The funcName will be a string identifier for which space should be allocated in the tokenizer.
    // For CUSTOM_OPER functions, you should simply assign the "ident" pointer to the passed in funcName.
    // For functions other than CUSTOM_OPER, you should free the funcName after you're assigned the OPER_TYPE.
    node->type = FUNC_NODE_TYPE;
    node->data.function.oper = resolveFunc(funcName);
    node->data.function.op1 = op1;
    op1->parent = node;
    if (op2 != NULL) {
        node->data.function.op2 = op2;
        op2->parent = node;
    }else{

        printf("error op2 is NULL\n");

    }

    return node;
}

AST_NODE *createAST_SymbolNode(char *id){
    AST_NODE *node;
    size_t nodeSize;

    // allocate space (or error)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    node->type = SYMBOL_NODE_TYPE;
    node->parent = NULL;

    //setting the ident pointer to the id pointers
    //ident is now the same address as id
    node->data.symbol.ident = id;

    return node;
}

AST_NODE *linkScopeToLetList(SYMBOL_TABLE_NODE *letList, AST_NODE *scope){

    scope->symbolTable = letList;
    SYMBOL_TABLE_NODE *temp=letList;
    while(temp != NULL){

        temp->val->parent = scope;
        temp = temp->next;
    }

    return  scope;
}
SYMBOL_TABLE_NODE *createSymTableNode(char *id, AST_NODE *value){
    SYMBOL_TABLE_NODE *node;
    size_t nodeSize;

    // allocate space (or error)
    nodeSize = sizeof(SYMBOL_TABLE_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    //set the string name
    node->ident = id;

    //now set the nodes value
    node->val = value;
    //set the node.next to null to make sure its not pointing to garbage data
    node->next = NULL;


    return node;
}

SYMBOL_TABLE_NODE *linkSymbolTable(SYMBOL_TABLE_NODE *prevHead, SYMBOL_TABLE_NODE *newHead){
    //set the new head to the previous one
    newHead->next = prevHead;

    return newHead;

}

// Called after execution is done on the base of the tree.
// (see the program production in ciLisp.y)
// Recursively frees the whole abstract syntax tree.
// You'll need to update and expand freeNode as the project develops.
void freeNode(AST_NODE *node)
{
    if (!node)
        return;

    if (node->type == FUNC_NODE_TYPE)
    {
        // Recursive calls to free child nodes
        freeNode(node->data.function.op1);
        freeNode(node->data.function.op2);

        // Free up identifier string if necessary
        if (node->data.function.oper == CUSTOM_OPER)
        {
            free(node->data.function.ident);
        }
    }

    free(node);
}

// Evaluates an AST_NODE.
// returns a RET_VAL storing the the resulting value and type.
// You'll need to update and expand eval (and the more specific eval functions below)
// as the project develops.
RET_VAL eval(AST_NODE *node)
{
    if (!node)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN}; // see NUM_AST_NODE, because RET_VAL is just an alternative name for it.

    // TODO complete the switch. DONE
    // Make calls to other eval functions based on node type.
    // Use the results of those calls to populate result.
    switch (node->type){

        case FUNC_NODE_TYPE:
            result = evalFuncNode(&node->data.function);
            break;
        case NUM_NODE_TYPE:
            result = evalNumNode(&node->data.number);
            break;
        case SYMBOL_NODE_TYPE:
            result = evalSymbolNode(node);
            break;
        default:
            yyerror("Invalid AST_NODE_TYPE, probably invalid writes somewhere!");
    }

    return result;
}  

// returns a pointer to the NUM_AST_NODE (aka RET_VAL) referenced by node.
// DOES NOT allocate space for a new RET_VAL.
RET_VAL evalNumNode(NUM_AST_NODE *numNode)
{
    if (!numNode)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};

    // TODO populate result with the values stored in the node. DONE
    // SEE: AST_NODE, AST_NODE_TYPE, NUM_AST_NODE
    result.type = numNode->type;
    result.value = numNode->value;

    return result;
}

RET_VAL evalFuncNode(FUNC_AST_NODE *funcNode)
{
    if (!funcNode)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};


    // TODO populate result with the result of running the function on its operands. DONE
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE
    switch(funcNode->oper){

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
        case ADD_OPER:
            result = add_Helper(funcNode);
            break;
        case SUB_OPER:
            result = subtract_Helper(funcNode);
            break;
        case MULT_OPER:
            result = multiply_Helper(funcNode);
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
        case EXP2_OPER:
            result = exp2_Helper(funcNode);
            break;
        case CBRT_OPER:
            result = cbrt_Helper(funcNode);
            break;
        case HYPOT_OPER:
            result = hypot_Helper(funcNode);
            break;

    }

    return result;
}

RET_VAL evalSymbolNode(AST_NODE *symNode){
    if (!symNode)
        return (RET_VAL){INT_TYPE, NAN};

    RET_VAL result = {INT_TYPE, NAN};
    AST_NODE *tempAST_Node = symNode;
    SYMBOL_TABLE_NODE *tempSymbolTableNode = symNode->symbolTable;
    char *symbol = symNode->data.symbol.ident;

    while(tempAST_Node != NULL){
        tempSymbolTableNode = tempAST_Node->symbolTable;
        while(tempSymbolTableNode != NULL){

            if(strcmp(symbol,tempSymbolTableNode->ident) == 0){

                result = eval(tempSymbolTableNode->val);
                return  result;
            }
            tempSymbolTableNode = tempSymbolTableNode->next;

        }
        tempAST_Node = tempAST_Node->parent;
    }

    return result;
}



RET_VAL neg_Helper(FUNC_AST_NODE *funcNode){
    //TODO negate the negOp DONE
    RET_VAL negResult = {INT_TYPE, NAN};
    RET_VAL negOp = eval(funcNode->op1);

    //set the type to the negOp
    negResult.type = negOp.type;
    //now set the value to a negative number
    negResult.value = -negOp.value;

    return negResult;
}

RET_VAL abs_Helper(FUNC_AST_NODE *funcNode){
    // TODO implement abs() for double and int types DONE
    RET_VAL absResult = {INT_TYPE,NAN};
    RET_VAL absOp = eval(funcNode->op1);
    //access and set the type
    absResult.type = absOp.type;
    //now set the values
    if(absResult.value == INT_TYPE) {
        absResult.value = round(fabs(absOp.value));
    }else{
        absResult.value = fabs(absOp.value);
    }

    return absResult;
}

RET_VAL exp_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement exp() for double and int types DONE
    RET_VAL expResult = {INT_TYPE,NAN};
    RET_VAL expOp = eval(funcNode->op1);

    //
    expResult.type = DOUBLE_TYPE;
    //
    expResult.value = exp(expOp.value);

    return expResult;
}

RET_VAL sqrt_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement sqrt() for double and int types DONE
    RET_VAL sqrtResult = {INT_TYPE,NAN};
    RET_VAL sqrtOp = eval(funcNode->op1);
    //
    sqrtResult.type = DOUBLE_TYPE;
    //
    sqrtResult.value = sqrt(sqrtOp.value);

    return sqrtResult;
}

RET_VAL add_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement add operation DONE
    RET_VAL addResult = {INT_TYPE,NAN};
    RET_VAL addOp1 = eval(funcNode->op1);
    RET_VAL addOp2 = eval(funcNode->op2);

    addResult.type = addOp1.type || addOp2.type;

    if(addResult.type == INT_TYPE){
        //if its an int make sure its not a double by rounding
        addResult.value = round(addOp1.value + addOp2.value);
        //addResult.value = round(addOp1.value) + round(addOp2.value);
    }else{
        addResult.value = addOp1.value + addOp2.value;
    }

    return addResult;
}
RET_VAL subtract_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement subtract operation DONE
    RET_VAL subResult = {INT_TYPE,NAN};
    RET_VAL minusOp1 = eval(funcNode->op1);
    RET_VAL minusOp2 = eval(funcNode->op2);

    subResult.type = minusOp1.type || minusOp2.type;

    if(subResult.type == INT_TYPE){

        subResult.value = round(minusOp1.value - minusOp2.value);
    }else{
        subResult.value = minusOp1.value - minusOp2.value;
    }
    return subResult;
}

RET_VAL multiply_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement multiply operation DONE
    RET_VAL multResult = {INT_TYPE,NAN};
    RET_VAL multOp1 = eval(funcNode->op1);
    RET_VAL multOp2 = eval(funcNode->op2);

    multResult.type = multOp1.type || multOp2.type;
    //if the type isn't a double
    if(multResult.type == INT_TYPE){
        //make sure that we round to a whole number not  double
        multResult.value = round(multOp1.value * multOp2.value);
    }else{
        //else the result is a double so dont worry about it
        multResult.value = multOp1.value * multOp2.value;
    }
    return multResult;
}

RET_VAL divide_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement divide operation DONE
    RET_VAL divResult = {INT_TYPE,NAN};
    RET_VAL divOp1 = eval(funcNode->op1);
    RET_VAL divOp2 = eval(funcNode->op2);

    divResult.type = divOp1.type || divOp2.type;

    if(divResult.type == INT_TYPE){
        divResult.value = round(divOp1.value/divOp2.value);
    }else{
        divResult.value = divOp1.value/divOp2.value;
    }
    return divResult;
}

RET_VAL remainder_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement mod operation DONE
    RET_VAL modResult = {INT_TYPE,NAN};
    RET_VAL modOp1 = eval(funcNode->op1);
    RET_VAL modOp2 = eval(funcNode->op2);

    modResult.type = modOp1.type || modOp2.type;
    if(modResult.type == INT_TYPE) {
        modResult.value = round(remainder(modOp1.value, modOp2.value));
    }else{
        modResult.value = remainder(modOp1.value,modOp2.value);
    }
    return modResult;
}

RET_VAL log_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement log operation DONE
    RET_VAL logResult = {INT_TYPE,NAN};
    RET_VAL logOp1 = eval(funcNode->op1);
   // RET_VAL logOp2 = eval(funcNode->op2);

   logResult.type = logOp1.type;
   logResult.value = log(logOp1.value);

    return logResult;
}
//pow(double x, double y)
RET_VAL pow_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement POW operation DONE
    RET_VAL powResult = {INT_TYPE,NAN};
    RET_VAL powOp1 = eval(funcNode->op1);
    RET_VAL powOp2 = eval(funcNode->op2);
    //set the type to the x.type
    powResult.type = powOp1.type;
    if(powResult.type == INT_TYPE){
        //pow(double x, double y)
        // x raised to the power y
        //so powOp1raised to the power of powOp2
        powResult.value = round(pow(powOp1.value,powOp2.value));
    }else{
            powResult.value = pow(powOp1.value,powOp2.value);
    }

    return powResult;
}


//the fmax() functions return x or y, whichever is bigger.
RET_VAL max_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement max operation DONE
    RET_VAL maxResult = {INT_TYPE,NAN};
    RET_VAL maxOp1 = eval(funcNode->op1);
    RET_VAL maxOp2 = eval(funcNode->op2);

    maxResult.type = maxOp1.type || maxOp2.type;

    if(maxResult.type == INT_TYPE){
        //the fmax() functions return x or y, whichever is bigger.
        //set the result to the max
        maxResult.value = round(fmax(maxOp1.value,maxOp2.value));
    }else{
        //the fmax() functions return x or y, whichever is bigger.
        //set the result to the max
        maxResult.value = fmax(maxOp1.value,maxOp2.value);

    }

    return maxResult;
}

//the fmin() functions return x or y, whichever is smaller.
RET_VAL min_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement min operation DONE
    RET_VAL minResult = {INT_TYPE,NAN};
    RET_VAL minOp1 = eval(funcNode->op1);
    RET_VAL minOp2 = eval(funcNode->op2);

    minResult.type = minOp1.type || minOp2.type;

    if(minResult.type == INT_TYPE){
        //the fmin() functions return x or y, whichever is smaller.
        minResult.value = round(fmin(minOp1.value,minOp2.value));
    }else{
        //the fmin() functions return x or y, whichever is smaller.
        minResult.value = fmin(minOp1.value,minOp2.value);
    }

    return minResult;
}
//The exp2() function computes 2**x, the base-2 exponential of x.
RET_VAL exp2_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement exp2 operation DONE
    RET_VAL exp2Result = {INT_TYPE,NAN};
    RET_VAL exp2Op1 = eval(funcNode->op1);

    exp2Result.type = exp2Op1.type;
    if(exp2Result.type == INT_TYPE){
        exp2Result.value = round(exp2(exp2Op1.value));
    }else{
        exp2Result.value = exp2(exp2Op1.value);
    }

    return exp2Result;
}
//The cbrt() function computes the cube root of x.
RET_VAL cbrt_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement cbrt aka cubed root  operation DONE
    RET_VAL cbrtResult = {INT_TYPE,NAN};
    RET_VAL cbrtOp1 = eval(funcNode->op1);

    cbrtResult.type = cbrtOp1.type;

    if(cbrtResult.type==INT_TYPE){
        cbrtResult.value = round(cbrt(cbrtOp1.value));
    }else{
        cbrtResult.value = cbrt(cbrtOp1.value);
    }

    return cbrtResult;
}
RET_VAL hypot_Helper(FUNC_AST_NODE *funcNode){
    //TODO implement subtract operation DONE
    RET_VAL hypotResult = {INT_TYPE,NAN};
    RET_VAL hypotOp1 = eval(funcNode->op1);
    RET_VAL hypotOp2 = eval(funcNode->op2);

    hypotResult.type = hypotOp1.type || hypotOp2.type;

    if(hypotResult.type == INT_TYPE){

        hypotResult.value = round(hypot(hypotOp1.value,hypotOp2.value));
    }else{

        hypotResult.value = hypot(hypotOp1.value,hypotOp2.value);
    }

    return hypotResult;
}


// prints the type and value of a RET_VAL
void printRetVal(RET_VAL val)
{
    // TODO print the type and value of the value passed in. DONE
    switch(val.type){

        case INT_TYPE:
            printf("%ld",(long)val.value);
            break;
        case DOUBLE_TYPE:
            printf("%f",val.value);
            break;

    }

}
