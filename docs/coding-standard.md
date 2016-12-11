PhiOS Coding Standard
=========

Please read this coding standard and respect it if you want to contribute to
this project.

Identation
------

* The indentations are 4 characters.
* Every statement inside a block (a block is delimited by braces) should be
indented.
* Do not put multiple statements on the same line. Also, avoid placing multiple
assignments on the same line. The basic rule you should follow is common sense:
avoid tricky expressions.

Long lines
------

The lines should have at most 80 characters. Statements longer should be:
* Broken into shorter, simpler statements.
* Splitted into more lines, with double indentation.
* Splitted into more lines, aligning similar elements.

Placing braces
-----

* The braces should be placed at the beginning of the next line. The braces
should be used even if this is not necessary, for example:
```
if (a == 0)
{
    return 0;
}
```
* When the braces are used for the variable initialization, then should be placed
on same line. For example:
```
char str[10] = { 0 };
```

Functions
-----

Functions should be as modular as possible. Every function should do one thing,
and do it well. Please document your functions and test them before making a
pull request. Keep your functions as short and simple as possible.

Spaces
-----

* Use a space after the most keywords, except for unary operators like
```sizeof``` .
* Do not add spaces arround (inside) parenthesized expressions. This example is
wrong: ```s = sizeof( variable )``` .
* When you declare a pointer variable, the '*' should be placed beside variable
name. This rule applies and to function parameters. When a function returns a
pointer, the '*' should be placed beside data type name. For example:
```int *varName = NULL;```
```int* foo(char *a_functionParameter)```. 
* Use spaces around most of the binary and ternary operators, but not after unary
operators (or before, in case of postfix operators).
* Do not use spaces around structure member access operators, like ```->``` and
```.``` .  
* Do not leave trailing whitespaces at the end of the lines.

Naming
-----

* Function argument names should start with ```a_```.
* Global variable names should start with ```g_```.
* Static function names should start with ```helper_```.
* Variable and function names should be ```camelCase```.
* Function names should start with module name, except static functions.
* Struct names and typedefs should start with upper character.

Commenting
-----

* Every file should have a comment header. It must include a description of its
content.
* Every function should have a documentation before the its declaration.
It must include the function prototype, a function description, parameters
and return value.
* Before declaration, every structure should have a documentation. Global
variables should have a documentation too.
* Keep the documentation as clear and precise as possible.
* For documentation, Doxygen should be used. Please read the following
instructions about documenting the code:
http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html.


Here is an example of a function documentation:
```
/**
 * int foo(int a)
 *
 * The function description
 *
 * @param a an integer value
 * @return an integer value
 */
int foo(int a);
``` 
* Feel free to add short comments in your code if you think they are needed (to
explain a certain expression for example). Make your code as easy to read
and understand as possible.

Miscellaneous
-----
* After you free a memory location, make sure you set the pointer to NULL.
* Make sure that all pointers are freed.
