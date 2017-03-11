# Calculator by Parse Tree

The program parses the input into a parse tree, then prints the computed result.

 ## Parenthesization
 
 Each '(' creates a new tree and is pushed onto top of the trees stack. Any digit read or operand read from that point on is appened to the tree at the top of the stack.
 
 At the end of the parse loop, trees are attached to one another to create one main tree.
 
 Users need not close parentheses with ')'
 
## Operands
Currently these operands are computable
 * '+'
 * '-'
 * '/
 * '*'

## Example Inputs

Calculation is done from left to right

* (3+4)*(99-4)+(2+2+2)
* (((((((3*3
* 4+(9-6) -> produces 10
* 3+3+(2 -> produces 9 (invalid Parenthesization of '(2' )
 

