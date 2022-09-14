%{
#include <stdio.h>
#include <stdlib.h> 
#include "parser.tab.h"
#include <string.h>
extern int line;
extern int decl;
%}
comment ("(*")([^*)])*("*)"{1})
if "if"
then "then"
repeat "repeat"
end "end"
else "else"
read "read"
write "write"
until "until"
less "<"
minus "-"
add "+"
stmt_sep ";"
identifier [[:alpha:]][[:alnum:]|_]*
number ([+|-]?)[[:digit:]]+("."[[:digit:]]*)?
white [[:blank:]]+
other "."
equal "="
assign ":="
mul "*"
div "/"
open_exp "("
close_exp ")"
newline [\n]

%%
{comment}	      {return COMMENT;}
{if}            {return IF;}
{then}          {return THEN;}
{repeat}        {return REPEAT;}
{end}           {return END;}
{else}          {return ELSE;}
{read}          {return READ;}
{write}         {return WRITE;}
{until}         {return UNTIL;}
{less}          {return LESS;}
{add}           {return ADD;}
{stmt_sep}      {return STMT_SEP;}
{minus}         {return MINUS;}
{open_exp}		  {return OPEN_EXP;}
{close_exp}		  {return CLOSE_EXP;}
{identifier}    {
  yylval.sym.line = line;
  yylval.sym.text = strdup(yytext);
  yylval.sym.decl = decl;
  decl++;
  return IDENTIFIER;}
{number}	      {
  int *num = new int();
  *num = atoi(yytext);
  yylval.ptr = num; return NUMBER;}
{white}	        {;}
{other}         {return OTHER;}
{equal}         {return EQUAL;}
{assign}        {return ASSIGN;}
{mul}           {return MUL;}
{div}           {return DIV;}
{newline}       {line++;}
%%
/* recomendavel declarar sempre funcao yywrap() */
int yywrap(){return 1;}
int yyerror(const char* s){fprintf(stderr, "%s\n", s); return 1;}