# K
A little attempt at making the C language more perfect
A lexer and parser combo to try and add some features / syntax changes to C
It takes a K file (C with the extras) and generates a C file that can then be compiled
Trying to hit C11

DONE:
  lexer

TODO:
  parser
  add 'deref' keyword (puts the specified function call at the end of the current scope) (Zig inspired)
  add functions to structs (make an implicit parameter that is a pointer to the struct that called it) (C++ inspired)
