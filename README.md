# Calculator by Parse Tree
The program parses the input into a parse tree, then prints the result

## Operands
Currently these operands computable
 * '+'
 * '-'
 * '/
 * '*'
 
 ## Parenthesization
 
 Each '(' create a new tree and is pushed onto top of the trees stack. Any digit read or operand read from that point on is appened to the tree at the top of the stack.
 
 At the end of the parse loop, trees are attached to one another to create one main tree.
 
 Users need not close parentheses with ')'
 

