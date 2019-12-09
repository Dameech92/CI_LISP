# CILISP README

This is my read me for all changes that I have done on my project, CI-LIsp
### Tasks that I have fully completed
* TASK 1
* TASK 2
* TASK 3
* TASK 4
* TASK 5
* TASK 6
* TASK 7
* TASK 8
* TASK 8 and a half  

## TASK 1 implementation

*Finished implementing the functions that were given to us initially such as 

1. **createNumberNode**
	* which populates the type that was passed in either being a INT or DOUBLE
2. **createFunctionNode**
	* populate the node with the function node type
	* Then check both nodes data for the AST node op1 and op2
	* dont forget that if op2 is null don't do anything 
	* this only will happen if there is a math function being used that only needs one operand like abs()
3. **eval** 
 	* makes calls to other eval functions based on the node type 
 	* as of now it can only be a FUNCTION or NUM_NODE TYPE
4. **evalNumNode** 
	* have a RET_VAL that returns the appropriate numNode Type and Value.  
5. **evalFunctionNode** 
	* implemented a switch statement for all the mathmatical functions being used in cilisp
	* many changed were done in this method
	* first off for every case I made a seperate helper method and called upon it in the case
	* examples of these helper are as follows "someName_Helper" I keep this format for all cases
	* all of these helpers make sure to round if there are two operands
	* if there are two then we check if its a INT_TYPE and if it is then we use the round() function

**Testing functionality code output**

* here are the various things I tested to make sure that task one was fully functional 
* after testing I changed precision to two now it only shows two significant figures 



*   (add 1 1)
2
* (add 1.1 2.2)
3.300000
* (add 1.3 4)
5.300000
* (add 6 3.3)
9.300000
* (neg -1)
1
* (neg 1) **-1**
* (neg 4.5678989)
-4.567899
* (neg 2.0008907876235)
-20008907876235
* (sub 20 3.0)
17.000000
* (sub 3.0 4.0)
-1.000000
* (sub 3.2 4.3)
-1.100000
* (sub 12345 234.45)
12110.550000
* (sub 250.60 5050)
-4799.400000
* (abs -2.2)
2.200000
* (abs -20000000)
20000000
* (abs -200.000)
200.000000
* (exp 5)
148.413159
* (exp 5.0)
148.413159
* (sqrt 25)
5.000000
* (sqrt 36.0)
6.000000
* (sqrt 25.0)
5.000000
* (add 4 2)
6
* (add 4.2 2.2)
6.400000
* (add 34 64)
98
* (add 66.1 77.2)
143.300000
* (add 77.2 60)
137.200000
* (add 60 77.2)
137.200000
* (mult 6 6)
36
* (mult 6.2 6)
37.200000
* (mult 2.2 3.3)
7.260000
* (mult 3 2.3)
6.900000
* (div 2 8)
0
* (div 8 2)
4
* (div 8.2 2)
4.100000
* (div 2 2.2)
0.909091
* (div 3.2 3.2)
1.000000
* (remainder 8 2)
0
* (remainder 5.0 5.0)
0.000000
* (remainder 8.4 4.1) 
ERROR: invalid character: >><<
*  (remainder 8 3)
-1
* (remainder 3 2)
-1
 (remainder 2 3)
-1
 (remainder 3.0 2.0)
-1.000000
 (remainder 8.4 4.1)
0.200000 
* (log 10)
2
* (log 100)
4
*  (log 20.2)
3.005683
* (log 20)
2
* (pow 2 2)
4
* (pow 2.0 3)
8.000000
* (pow 4 2)
16
* (pow 2.0 2.0)
4.000000
* (pow 2 2.3)
5
* (max  100 10)
100
* (max 10 1000)
1000
* (max 100.00 200.00)
200.000000
* (max 2.2 4.4)
4.400000
* (max 2.2 40)
40.000000
* (min 4 8)
4
* (min 8 4)
4
* (min 40 40.2)
40.000000
* (min 40.2 40)
40.000000 
* (exp2 2 2)
4
* (exp2 2)
4
* (exp2 4)
16
* (exp2 2.0)
4.000000
* (exp2 4.2)
18.379174
* (exp2 5.2)
36.758347
*   (cbrt 4)
2
* (cbrt 4.0)
1.587401
* (cbrt 8)
2
* (cbrt 8.0)
2.000000
* (cbrt 8.2)
2.016530 


	
## TASK 2 Implementation
Finished implementing all parts of task two so far I have added the grammar that was asked extending parts of task one as well along the way.adding the grammar to  Flex and Bison too.

1. **lex file** - added symbol and let tokens

	* also added to the **regex** included letters and symbols that accepts 0 or 1 letters 
	* edited the double regex
	* fixed the regex to include all functions needed for task two up to "hypot"
2. **Yacc/Bison** file 

	* extended the token to include the LET %token
	* added symbol to the ast node %type 
	* added let_section let_list let_elem to the symNode %type
	* extended the grammar in the s-expr to include symbol and let_section
	* let_section, let_list, let_elem and symbol were also added to the grammar in their own sections

## ciLisp.h file and ciLisp.c extensions
3. 
	* everthing added to the .h file is in the same order as the .c file to keep things organized
	*  extended the ciLisp.h file making sure everything I added in the yacc and flex files was also in this .h file.
	*  also added 4 new functions that were also added to the .y file  
	*  4 methods used in the .y 
	*  linkScopeToLetList(), linkSymbolTable(), createSymTableNode(), createAST_SymbolNode()
	
4. **createAST _SymbolNode**
	* creates the AST node and allocates space for it with calloc and settimg it to size 1 
	* then we set the node type to make it a SYMBOL_ NODE_ TYPE
	* then dont forget to set its parent to NULL 
	* now set the nodes ident pointer to the id  pointer 
5. **linkScopeToLetList**
	* we set the scopes symbolTable node to the letLisTS symbolTable node
	* then we make sure while the  let list isnt null to populate the scope to include the value of the parent
	* then iterate the letList
6. **createSymTableNode** 
	* set the string name 
	* set the nodes value
	* then set the next node to NULL
7. **linkSymbolTable** 
	* links the newHead.next  to the previous one
8.  **evalSymbolNode**
	* implement the evaluation function that executes this and similar programs the code will have to perform symbol table lookups
	
#### Added changes from task that were made in task two 
	
1. **createFunctionNode**
	* had errors during testing when op2 was Null so added condition, making sure to only do stuff to op2 if its not null
	* set op1 and op2's parent to equal an AST node whcih only is set if its being used 
	* op1 is always being used tho
2. **eval**
	* added a case in the switch statement to include SYMBOL_NODE_TYPE 

## **Testing functionality code output**

```
* here are the various things I tested to make sure that task two was fully functional 

>  ((let (a ((let (c 3) (d 4)) (mult c d)))) (sqrt a))
Double Value 3.46

> (add ((let (abcd 1)) (sub 3 abcd)) 4)
Integer value is 6

> (mult ((let (a 1) (b 2)) (add a b)) (sqrt 2))
Double Value 4.24

>(add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))
Double Value 5.40

> ((let (first (sub 5 1)) (second 2)) (add (pow 2 first) (sqrt second)))
Double Value 17.41
```



##Task 3/4 
I am putting task three and four into one section. This makes most sence due to the way I implemented them.

. During this phase of my programming I ran into a few speed bumps and this really slowed down my progress. After extending the grammar and adding the the type to the let section changing names from double and int to int_literal and double_literal things got very confusing for me 

1. **Yacc file**


	* Extended let_elem to accept an int symbol and and double symbol in the yacc file
	
	* heavily changed the number ast node type to include all cases of int and double literals
	
	* Extended all tokens in the yacc file so that the yacc and lex files could communicate correctly this part confused me due to the fact that my naming convention was too closely related
	* including- the INT_ LITERAL and DOUBLE_ LITERAL < dval> tokens & the INT and DOUBLE tokens. This name convention ended up being the best i could think of at this point all other names confused me and made it very hard to debug when i ran into issues 
	


2. **Lex file**


	* In the regex of the program I changed the names of the double and int regex to int _ literal, and double _literal.
	* Extended the INT and DOUBLE tokens to the lex file now the lex and yacc files are using the same tokens because i am passing them into the lex file
	
	
## ciLisp.h and ciLisp.c file extensions 
3. **evalSymbolNode**


	* By the time I added the parts to make the precison warning loss in this function I didnt have a clue why it wasnt working and was running into many debugging issues. part of the issue was that I needed to fully refract my function it was starting to look like the rolling hills of camarillo.
	* kieth in the tutor center at this point helped me fully rewrite my evalSymbolNode and we went through all the issues I was running into in the function and made helper methods that made things much easier to understand 
	* this helper method is called resolveType it helps determine the resulting type that is for precision loss




4.    **Refractoring code and adding helper methods**


	* now that I finally got task 4 and 5 working with the guidance of kieth I fully refractored code from all previous sections and cleaned up all parts of the code making my project much easier to see and use
	* made two different helper methods helping with callocing/ memory allocation one for callocing space for the symbol table and one for callocing for an ast node. this code was used in a few spots and made more sense to just make a helper function and just call upon this function when needed instead of rewriting the same code everytime we need to allocate space
	* after making createNewNode and createNewSymbol I went through all my code and used these helpers in all places i was callocing space
	* fully refractored linkSymbolToLetList 
	* also extended the print and got task 4 working in about 5 minutes 



## **Testing functionality code output**

```
>((let (int a 1.25))(add a 1))
WARNING: precision loss in the assignment for variable a
Double Value 2.00

> (mult ((let (int a (print(add 4 5.5)))) (sub (sub 21 a) a)) 4)
=> Double Value 9.50

WARNING: precision loss in the assignment for variable a
=> Double Value 9.50

WARNING: precision loss in the assignment for variable a
Double Value 12.00

> (mult ((let (double a (print(add 4 5)))) (sub (sub 21 a) a)) 4)
=> Integer value is 9

=> Integer value is 9

Double Value 12.00

 
```


## Task 5
This task for me was straight forward and I knew intuitivley what I needed to change to make this work. 

1. **Yacc file**

	*At this point I am starting to get the hang of how to add code to Lisp now. I have been always adding to the yacc or lex files first so this is how I started with this one too
	 
	* First checking the changed grammar given to us then adding those changes to the yacc file 
	
	* **f_expr** - Changed the fexpr to only accept a FUNC and a s_expr_list now it only takes two parameters instead of 3 
	* **s _ expr_list**	- Made a new ast type called s_expr _list and using the directions given in task 5 fully extended the grammar for this type
2. **lex files**

	* no changes were made to this during task 5
	
## ciLisp.h file extensions
3. **FUNC_ AST_NODE**
	
	* We were asked to change this struct, it was first only supposed to take in two operands but we were asked to change that and to put in just one ast node called *opList.
	* From this extension to the FUNC_ AST_ NODE i had to then go through all segments of code that previously used op1 and op2 and change them to opList and opList.next 
4. **AST_ NODE**

	* To allow creations of lists of s-expressions, a link next is added to AST_NODE
	
## ciLisp.c file extensions 	
	
5. **Finding Arity**

	* At this point we find out that we have three operation functions that are different than the other 20 or so that we are dealing with for this task. MULT_ OPER, ADD_ OPER AND PRINT _OPER
	* To make things a little easier kieth and I came to a conclusion that seperating all the math functions that only have one operand to one pile and setting the math functions that take in two opoerands into another pile and setting the other three into their own place as well would make things easier to read
	* this was accomplished with the helper function called **getArity()** this function sepereates all OPER_ TYPE to different FUNCTION_ ARITY enums calls
6. **evalFuncNode**
	* this function was heavily refractored and does not call the same thing it did previously
	* now there are three cases in evalFuncNode a Unary, Binary and Nary case which all call other subroutines that are called evalUnary, evalBinary, and evalNary 
7. **evalUnary**
	
	* In this helper method we go check if there are too few parameters and too many parameters before going into a switch statement 
	* In this switch it segragates all math operations that only take in one operand.
	* ther are 7 math functions so far that only take in one operand
	* within each case we call more helper functions where all the math is actually going on
8. **evalBinary**

	* In this helper method we go check if there are too few parameters and too many parameters before going into a switch statement 
	* In the switch it segragates all math operations that only take in two operands.
	* there are 10 math functions so far that only take in two operands
	* within each case we call more helper functions where all the math is actually going on

9. **evalNary**
	
	* In this helper method we go check if there are too few parameters before going into a switch statement 
	* In the switch it segragates all math operations that more than two operands.
	* there are 3 math functions so far that take in more than 2
	* within each case we call more helper functions where all the math is actually going on

10. **add mult print**

 Now finally at this point i needed to fix the three functions that take in more than two operands they essentially are all the same code for each of the three as well. These three again are **MULT, ADD** and **PRINT** I go into there respective helper functions and change how the math works for each of them

11. **freeNode**

	* now that task 5 finally started working before moving onto the next task I free all allocated space for freeNode
	* for this function I made about 4 small helper methods 
	* freeSymTableNode, freeSymNode, freeFuncNode, freeFuncOpList which are all used within the freeNode() function this part is necessary to help free all the allocated space that we have been using this entire project.
	* they recurively free the entore syntax tree and is only called after the execution of the code

## **Testing functionality code output for task 5**

```
(mult ((let (double a (print(add 4 5 2)))) (sub (sub 100 a) a 5)) 4 2.5)
WARNING: too many parameters for the function sub
=> Integer value is 11

=> Integer value is 11

Double Value 780.00

> (add 1 2 3 4 5)
Integer value is 15

> ((let (int a 1)(double b 2))(print a b 3))
=> Integer value is 1
Double Value 2.00
Integer value is 3

> (add 1 2 3(sub 5 2))
Integer value is 9

```


##Task 6 


1. **yacc files** 
	* went though and added the new COND token
	* and also added a new ast node called s_expr
	* after this I went on and extended the sexpr to include a COND node and three s_expr setting the this extended grammar calls a new function that i created to be able to handle this called createCondNode
	
	
2. **lex files**

	* added to the regex the rest of the functions needed for lisps lex to be fully extended now 
	* including, rand | read | equal | less | greater
	* extended the cond node in the lex file to return the COND node from the yacc file 
	
## ciLisp.h and ciLisp.c extensions

1.  **AST_ NODE_ TYPE**
	* added a new ast node to this struct called COND_ NODE_TYPE for use to extend grammar in the eval() so that our grammar can read conditions 

2. **COND_AST _NODE**
	* added entire new struct type called called  COND_ AST_NODE 
	* - contains three fields:
		- **cond**
			* conditional expression to decide which of the following expressions is evaluated
		- **doWhenTrue**
			* expression to evaluate if the conditional is true.
		- **doWhenFalse**
			* expression to evaluate if the conditional is false.
3. 	- **createCondNode()**
			- creates an ast_node of COND _TYPE and assigns the provided expressions to the appropriate fields. Sets the parents of the expressions this node. 
4.  **evalCondNode()**
			- creates a new RET_ VAL and uses a switch statement, based on the evaluation of the conditional expression either the ifTrue expression will be evaluated and the value assigned to RET_VAL or the ifFalse expression will be.
			
5. **eval()** 
	* extended the eval to accept COND_ NODE_TYPE which calls the evalCondNode when necessary 

6. **helper functions**

	* finally extended the last few functions that are needed for ciLisp these 5 functions are less,greater,rand and equals are straight forward and do exactly that 
	
		- **read**- read was very confusing for me and I still am a little confused on this part but from what ive done it reads a value from the user, changes the ast_node from a function node to a number node, in the number field of the data union the read in value is assigned to the value field and the type is dependent on whether or not the read value contained a decimal point.

		- **rand**- generates a random number , changes the ast_node from a function node to a NUMBER _ NODE_TYPE, in the number field  random value is assigned to the value  and the type is set as a double.

		- **less**- less compares two operands and spits out the one  thats less than the other 
		- **greater**- grester compares two operands and outputs the one that is greatwr than the other
		- **equals**- equals compares two operands and sees if they are the same if they are the same if so out puts the value if not does not do anything 


## **Testing functionality code output fot task 6**

	
```
> ((let (int a (read)) (double b (read)) (c (read)) (d (read))) (print a b c d))
=> read := 3
Integer value is 3
read := 5.0
Double Value 5.00
read := 10
Integer value is 10
read := 5.175
Double Value 5.17

Double Value 5.17

> ((let (a 0)) (cond (less (rand) 0.5) (add a 1) (sub a 1)))
Integer value is 1

> ((let (myA (read))(myB (rand)))(cond (less myA myB) (print myA) (print myB)))
read := -1
=> Integer value is -1

Integer value is -1


``` 		
 	
 	
##Task 7,8 and half of 9 
Now the Final strech of the program, instead of treating the last few parts of this program as sepereate entities I tried to think about the end goal when I was implementing this part. The reason I did this was to not have to go back and refractor code that I already did once. 

1. **yacc files** 
	* added the new token called LAMBDA 
	* extended the let_elem to take in LAMBDA expressions 
	* created a new symnode called arg_list and extended the grammar inside it  
	
	
2. **lex files**
	* added support for a new keyword "lambda" and the return of a new LAMBDA token.
	
	
## ciLisp.h and ciLisp.c extensions

	* 





## **Testing functionality code output fot task 7,8 and half of 9**

```
> ((let (double myFunc lambda (x y) (mult (add x 5) (sub y 2)))) (sub (myFunc 3 5) 2))
Integer value is 22

> ((let (f lambda (x y) (add x y)))(f (sub 5 2) (mult 2 3))) 
Integer value is 9

> ((let (int a 1)(f lambda (x y) (add x y)))(f 2 (f a 3)))
Integer value is 6

> ((let (countdown lambda (x) (cond (greater x 0) (countdown (print (sub x 1))) (print x)))) (countdown 10))
=> Integer value is 9

=> Integer value is 8

=> Integer value is 7

=> Integer value is 6

=> Integer value is 5

=> Integer value is 4

=> Integer value is 3

=> Integer value is 2

=> Integer value is 1

=> Integer value is 0

=> Integer value is 0

Integer value is 0

> ((let (f lambda (w x y z) (pow (add (read) (add 1 (read) (mult 3 (abs -2)))) 2)))(f 1.2 999.987 63.2 0.0013))
read := 3
read := 4
Integer value is 196

> ((let (f lambda (w x y z) (pow (add (read) (add 1 (read) (mult 3 (abs -2)))) 2)))(f 1.2 999.987 63.2 0.0013))
read := 1
read := 2
Integer value is 100

> 


```	
