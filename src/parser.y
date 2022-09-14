%define parse.error verbose
%code requires{
  #include "./include/ast.hpp"
}
%{
  #include <string>
  #include <iostream>
  using namespace std; 
  #include "./include/ast.hpp"
  SymbolTable st;
  #include <stdio.h>
  int yylex(void);
  int yyerror(char const *s);
  int yyparse();
  extern FILE *yyin;
  int line = 1;
  int decl = 0;
%}
/* Bison declarations. */
%union
{
  int integer;
  Op op;
  char *text;
  Symbol sym;
  void *ptr;
}

%token COMMENT
%token IF
%token THEN
%token REPEAT
%token END
%token ELSE
%token READ
%token WRITE
%token LESS
%token ADD
%token STMT_SEP
%token MINUS
%token OPEN_EXP
%token CLOSE_EXP
%token IDENTIFIER
%token NUMBER
%token OTHER
%token EQUAL
%token ASSIGN
%token MUL
%token DIV
%token UNTIL


%type <ptr> factor term simple_exp NUMBER program exp statement write_stmt read_stmt assign_stmt if_stmt repeat_stmt stmt_sequence
%type <op> mulop addop comparison_op
%type <text> OPEN_EXP CLOSE_EXP
%type <sym> IDENTIFIER

%% /* The grammar follows. */

input:
| program  {
  Program *p = (Program *) $1;
  p -> resolve();
  st.OutputJson();
  ;YYACCEPT;};

program: stmt_sequence {
  Program *node = new Program();
  node->stmt_seq = (StatementSequence *)$1;
  $$ = node;
} // criar node 

stmt_sequence: stmt_sequence STMT_SEP statement  {
  StatementSequence *node = new StatementSequence();
  node -> stmt = (Statement *)$3;
  node -> stmt_seq = (StatementSequence *)$1;
  $$ = node;
}
| statement {
  StatementSequence *node = new StatementSequence();
  node -> stmt = (Statement *)$1;
  $$ = node;
};

statement: if_stmt {
  Statement *node = new Statement;
  node -> ifStmt = (IfStmt *) $1;
  $$ = node;
}
| repeat_stmt {
  Statement *node = new Statement;
  node -> repeatStmt = (RepeatStmt *) $1;
  $$ = node;
}
| assign_stmt {
  Statement *node = new Statement;
  node -> assignStmt = (AssignStmt *) $1;
  $$ = node;
}
| read_stmt {
  Statement *node = new Statement;
  node -> readStmt = (ReadStmt *) $1;
  $$ = node;
}
| write_stmt {
  Statement *node = new Statement;
  node -> writeStmt = (WriteStmt *) $1;
  $$ = node;
};

if_stmt: IF exp THEN stmt_sequence END {
  IfStmt *node = new IfStmt();
  node->expr = (Expression *)$2;
  node -> ifStmt_seq = (StatementSequence *)$4;
  $$ = node;
} // criar node
| IF exp THEN stmt_sequence ELSE stmt_sequence END {
    IfStmt *node = new IfStmt();
    node->expr = (Expression *)$2;
    node -> ifStmt_seq = (StatementSequence *)$4;
    node -> elseStmt_seq = (StatementSequence *)$6;
    $$ = node;

}; // criar node

repeat_stmt: REPEAT stmt_sequence UNTIL exp  {
  RepeatStmt * node = new RepeatStmt();
  node -> stmt_seq = (StatementSequence *)$2;
  node -> expr = (Expression *) $4;
  $$ = node;
}; // criar node

assign_stmt: IDENTIFIER ASSIGN exp  {
  st.updateSymbolTable($1.text, $1.line, true);
  AssignStmt *node = new AssignStmt();
  node->id = $1.text;
  node->expr = (Expression *)$3;
  $$ = node;
}; // criar node

read_stmt: READ IDENTIFIER  {
  st.updateSymbolTable($2.text, $2.line, false);
  ReadStmt *node = new ReadStmt();
  node->id = $2.text;
  $$ = node;
}; // criar node

write_stmt: WRITE exp  {
  WriteStmt *node = new WriteStmt();
  node->expr = (Expression *)$2;
  $$ = node;
}; // criar node

exp: simple_exp comparison_op simple_exp {
  Expression *node = new Expression();
  node -> simple_expr = (SimpleExpression *) $1;
  node -> simple_expr2 = (SimpleExpression *) $3;
  node -> op = $2;
  $$ = node;
} // criar node
| simple_exp {
  //create expression and assign its values
  Expression *node = new Expression();
  node -> simple_expr = (SimpleExpression *)$1;
  $$ = node;
};

comparison_op: LESS {$$ = Op::Less;}
| EQUAL {$$ = Op::Equal;};

simple_exp: simple_exp addop term {
  SimpleExpression *node = new SimpleExpression();
  node -> simple_expr = (SimpleExpression *) $1;
  node -> term = (Term *)$3;
  node -> op = $2;
  $$ = node;

} // criar node
| simple_exp NUMBER {

  SimpleExpression *node = new SimpleExpression();
  node -> simple_expr = (SimpleExpression *) $1;
  node -> term = new Term;
  node -> term -> factor = new Factor;
  node -> term -> factor -> val = *(int*)$2;
  node -> op = $2 < 0 ? Op::Sub : Op::Add;
  $$ = node;
} // criar node
| term {
  SimpleExpression *node = new SimpleExpression();
  node -> term = (Term *)$1;
  $$ = node;
};

addop: ADD {$$ = Op::Add;}
| MINUS {$$ = Op::Sub;};

term: term mulop factor {
  Term *term = new Term;
  term -> factor = (Factor *)$3;
  term -> term = (Term *)$1;
  term -> op = $2;

  $$ = term;
} // criar node
| factor {
    Term *term = new Term;
    term -> factor = (Factor *)$1;
    $$ = term;
  };

mulop: MUL {$$ = Op::Mul;}
| DIV  {$$ = Op::Div;};

factor: 
  NUMBER  {
    Factor *f = new Factor;
    f -> val = *(int*)$1;
    $$ = f;
  }
| IDENTIFIER {
    st.updateSymbolTable($1.text, $1.line, false);
    Factor *f = new Factor;
    f -> id = $1.text;
    $$ = f;
  }
| OPEN_EXP exp CLOSE_EXP {
    Factor *f = new Factor;
    f -> expr = (Expression *)$2;
    $$ = f;
  }

%%


int main (int argc, char *argv[]) {
  if(argc >= 2){
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
  }
  else{
    yyin = stdin;
    yyparse();
  }
}