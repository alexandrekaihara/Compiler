#ifndef __AST_HPP__
#define __AST_HPP__

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <map>
#include <fstream>
using namespace std;

class Node;
class Program;
class StatementSequence;
class Statement;
class IfStmt;
class RepeatStmt;
class AssignStmt;
class ReadStmt;
class WriteStmt;
class Expression;
class SimpleExpression;
class Term;
class Factor;
struct Symbol{
    int line;
    int decl;
    char *text;
  };
struct Sym{
    vector<int> lines;
    bool declared;
};

enum Op {
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Assign,
    If,
    Less,
    Equal,
    Write,
    Read,
    Repeat
};
class AST{
    map<int,string> OperationMap = {
        {Add,"+"},
        {Sub,"-"},
        {Mul,"*"},
        {Div,"/"},
        {Mod,"%"},
        {Assign,":="},
        {If,"if"},
        {Less,"<"},
        {Equal,"="},
        {Write,"write"},
        {Read,"read"},
        {Repeat,"repeat"}};
    public:
        Node *root;
    
};

class Node{
    public:
    map<int,string> OperationMap = {
        {Add,"+"},
        {Sub,"-"},
        {Mul,"*"},
        {Div,"/"},
        {Mod,"%"},
        {Assign,":="},
        {If,"if"},
        {Less,"<"},
        {Equal,"="},
        {Write,"write"},
        {Read,"read"},
        {Repeat,"repeat"}};
        virtual void Print() = 0;
        // virtual string resolve() = 0;
};

class Program : public Node {
    public:
        void Print();
        StatementSequence *stmt_seq;
        Program() : stmt_seq(nullptr) {}
        string resolve();

};

class StatementSequence : public Node {
    public:
        void Print();
        Statement *stmt;
        StatementSequence *stmt_seq;
        StatementSequence() : stmt(nullptr), stmt_seq(nullptr) {}
        string resolve();
    private:
        bool isStmt() { if(this->stmt_seq == nullptr) return true; return false; };
        bool isStmtSeq() { if(this->stmt_seq != nullptr && this->stmt != nullptr) return true; return false; };
};
class Statement : public Node {
    public:
        void Print();
        IfStmt *ifStmt;
        RepeatStmt *repeatStmt;
        AssignStmt *assignStmt;
        ReadStmt *readStmt;
        WriteStmt *writeStmt;
        Statement() : ifStmt(nullptr), repeatStmt(nullptr), assignStmt(nullptr), readStmt(nullptr), writeStmt(nullptr) {}
        string resolve();
    private:
        bool isIf() { if(this->ifStmt != nullptr) return true; else return false; };
        bool isRepeat() { if(this->repeatStmt != nullptr) return true; else return false; };
        bool isAssign() { if(this->assignStmt != nullptr) return true; else return false; };
        bool isRead() { if(this->readStmt != nullptr) return true; else return false; };
        bool isWrite() { if(this->writeStmt != nullptr) return true; else return false; };
};
class IfStmt : public Node {
    public:
        void Print();
        Expression *expr;
        StatementSequence *ifStmt_seq;
        StatementSequence *elseStmt_seq;
        IfStmt() : expr(nullptr), ifStmt_seq(nullptr), elseStmt_seq(nullptr) {}
        string resolve();
    private:
        bool isIf() { if(this->ifStmt_seq != nullptr && this->elseStmt_seq == nullptr) return true; else return false; };
        bool isIfElse() { if(this->ifStmt_seq != nullptr && this->elseStmt_seq != nullptr) return true; else return false; };
};
class RepeatStmt : public Node {
    public:
        void Print();
        StatementSequence *stmt_seq;
        Expression *expr;
        RepeatStmt() : stmt_seq(nullptr), expr(nullptr) {}
        string resolve();
};
class AssignStmt : public Node {
    public:
        void Print();
        string id;
        Expression *expr;
        AssignStmt() : id(""), expr(nullptr) {}
        string resolve();

};
class ReadStmt : public Node {
    public:
        void Print();
        string id;
        ReadStmt() : id("") {}
        string resolve();

};
class WriteStmt : public Node {
    public:
        void Print();
        Expression *expr;
        WriteStmt() : expr(nullptr) {}
        string resolve();

};
class Expression : public Node {
    public:
        void Print();
        SimpleExpression *simple_expr;
        SimpleExpression *simple_expr2;
        Op op;
        Expression() : simple_expr(nullptr), simple_expr2(nullptr){};
        pair<string,string> resolve();
    private:
        bool isSimpleExp() {
            if(this->simple_expr != nullptr && this->simple_expr2 == nullptr) return true; else return false;}

        bool isComparison() {
            if(this->simple_expr != nullptr && this->simple_expr2 != nullptr) return true; else return false;}

};

class SimpleExpression : public Node {
    public:
        void Print();
        Term *term;
        SimpleExpression *simple_expr;
        Op op;
        SimpleExpression() : term(nullptr), simple_expr(nullptr){};
        pair<string,string> resolve();
    private:
        bool isTerm() { if(this->term != nullptr && this->simple_expr == nullptr) return true; else return false; };
        bool isAddOp() { if(this->term != nullptr && this->simple_expr != nullptr) return true; else return false; };

};
class Term : public Node {
    public:
        void Print();
        Factor *factor;
        Term *term;
        Op op;
        Term() : factor(nullptr), term(nullptr){};
        pair<string,string> resolve();
    private:
        bool isFactor() { if(this->term == nullptr && this->factor != nullptr) return true; else return false; };
        bool isMulOp() { if(this->term != nullptr && this->factor != nullptr) return true; else return false; };
};
class Factor : public Node {
    public:
        void Print();
        string id;
        int val;
        Expression *expr;
        Factor() : val(0), expr(nullptr),id(""){};
        pair<string,string> resolve();
    private:
        string getIdentifier(){ return this->id; };
        int getNumber(){ return this->val; };
        Expression *getExpression(){ return this->expr; };
        bool isIdentifier() { if (getIdentifier() != "") return true; else return false; };
        bool isNumber() { if (getIdentifier() == "" && expr == nullptr) return true; else return false; };
        bool isExpression() { if (getExpression() != nullptr) return true; else return false; }
};

class LabelGenerator{
    public:
        string getEndIf() { return "endif"+to_string(ifcounter++)+":\n"; };
        string getRepeat() { return "repeat"+to_string(repeatcounter++)+":\n"; };
        string getEndRepeat() { return "endrepeat"+to_string(endrepeatcounter++)+":\n"; };
        string getEndElse() { return "endelse"+to_string(elsecounter++)+":\n";};
        string getEndComparison(){return "endcomp"+to_string(compcounter++)+":\n";};
        string resolveId(string id){
            auto it = symbolTable.find(id+":");
            if(it == symbolTable.end()){
                symbolTable[id+":"] = cont++;
                labelDeclarationList.push_back(id+": SPACE\n");
                return 
                    "";
            }
            else{
                return "";
            }
        }
        string getResult(){
            labelDeclarationList.push_back("RESULT"+to_string(resultcounter++)+": SPACE\n");
            return labelDeclarationList.back().substr(0,labelDeclarationList.back().size()-8);
        }
        string resolveNum(int num){
            string key = "SYSTEMNUM" + ((num  >= 0) ? to_string(num)+":" : "NEG"+to_string(-num) + ":"); 
            auto it = symbolTable.find(key);
            if(it == symbolTable.end()){
                symbolTable[key] = -1;
                labelDeclarationList.push_back(key+" CONST "+to_string(num)+"\n");
                return key.substr(0,key.size()-1);
            }
            else{
                return key.substr(0,key.size()-1);
            }
        }
        vector<string> getLabelDeclarationList(){ return labelDeclarationList; };
        void printSymbolTable();
    private:
        int ifcounter = 0;
        int repeatcounter = 0;
        int endrepeatcounter = 0;
        int elsecounter = 0;
        int compcounter = 0;
        int resultcounter = 0;
        int cont = 0;
        map<string, int> symbolTable;
        vector<string> labelDeclarationList;
};
class SymbolTable {
    public:
        void updateSymbolTable(string id, int line, bool declared);
        void OutputJson();
    private:
        unordered_map<string, Sym> symbol_table;
};

#endif
