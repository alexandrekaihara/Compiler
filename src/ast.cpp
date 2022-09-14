#include "../include/ast.hpp"
#include <iostream>
#include <fstream>
using namespace std;
LabelGenerator labelGen;

//------------------------------------------------------------------------------
void Program::Print() {
    
    if( stmt_seq != nullptr){
        stmt_seq -> Print();
    }
    cout << endl;
}
string Program::resolve() {
    string program =
    this->stmt_seq->resolve()+
    "STOP\n"+
    string("ZERO: CONST 0\n")+
    "ONE: CONST 1\n"+
    "AUX: SPACE\n";
    vector<string> labelDeclarationList = labelGen.getLabelDeclarationList();
    ofstream outfile;
    outfile.open("program.asm", ios::out);
    for(int i = 0; i < labelDeclarationList.size(); i++){
        program += labelDeclarationList[i];
    }
    outfile << program;
    outfile.close();
    // labelGen.printSymbolTable();
    return program;
}
//------------------------------------------------------------------------------
void StatementSequence::Print() {
    if( stmt != nullptr){
        stmt -> Print();
    }
    cout << endl;
    if( stmt_seq != nullptr){
        stmt_seq -> Print();
    }
}
string StatementSequence::resolve() {
    if(isStmt()) return this->stmt->resolve();
    else if(isStmtSeq()) return this->stmt_seq->resolve()+this->stmt->resolve();
    return "ERROR couldn't identify if Statement Sequence is a Stmt or StmtSeq and Stmt\n";
}
//------------------------------------------------------------------------------
void Statement::Print() {
    if( ifStmt != nullptr){
        ifStmt -> Print();
    }
    if( repeatStmt != nullptr){
        repeatStmt -> Print();
    }
    if( assignStmt != nullptr){
        assignStmt -> Print();
    }
    if( readStmt != nullptr){
        readStmt -> Print();
    }
    if( writeStmt != nullptr){
        writeStmt -> Print();
    }
}
string Statement::resolve(){
    if(isIf()) return this->ifStmt->resolve();
    else if(isRepeat()) return this->repeatStmt->resolve();
    else if(isAssign()) return this->assignStmt->resolve();
    else if(isRead()) return this->readStmt->resolve();
    else if(isWrite()) return this->writeStmt->resolve();
    return "ERROR couldn't identify if Statement is a If, Assign, Read or Write statement\n";
}
//------------------------------------------------------------------------------
void IfStmt::Print() {
    cout << "if ";
    if( expr != nullptr){
        expr -> Print();
    }
    cout << " then" << endl;
    if( ifStmt_seq != nullptr){
        ifStmt_seq -> Print();
    }
    if( elseStmt_seq != nullptr){
        cout << "else" << endl;
        elseStmt_seq -> Print();
        cout << "end" << endl;
    }
}
string IfStmt::resolve() {
    string code1 = "";
    pair<string,string> exp = expr -> resolve();
    if(isIf()) {
        string endif = labelGen.getEndIf();
        return
            exp.first +                                     // Gets the evaluation of the condition
            "LOAD "+exp.second+"\n"+                        // Loads the value of the condition
            "JMPZ " + endif.substr(0,endif.size()-2) + "\n" +           // If the return if 0 (false), then jump the then
            this->ifStmt_seq->resolve() +                               // Gets the assembly of the stmt
            endif;                                                      // Add the label fo the end of the if
    }
    else if(isIfElse()){
        string endif = labelGen.getEndIf();
        string endelse = labelGen.getEndElse();
        return 
            exp.first +                                     // Gets the evaluation of the condition
            "LOAD "+exp.second+"\n"+                        // Loads the value of the condition
            "JMPZ " + endif.substr(0,endif.size()-2) + "\n" +           // If the return if 0 (false), then jump the then
            this->ifStmt_seq->resolve() +                               // Gets the assembly of the stmt of the then
            "JMP " + endelse.substr(0,endelse.size()-2) + "\n" +          // After executing the then, must jump to the end of the else
            endif+                                                      // Add the label fo the end of the if
            this->elseStmt_seq->resolve() +                             // Gets the assembly of the stmt of the else
            endelse;                                                
    }
    return "ERROR If Statement does not derives into if or if/else\n";
}
//------------------------------------------------------------------------------
void RepeatStmt::Print() {
    if( stmt_seq != nullptr){
        cout << "repeat" << endl;
        stmt_seq -> Print();
        cout << "until ";
    }
    if( expr != nullptr){
        expr -> Print();
    }
}
string RepeatStmt::resolve() {
    string startRepeat = labelGen.getRepeat(); 
    string endRepeat = labelGen.getEndRepeat();
    pair<string,string> exp = expr -> resolve();

    return 
        startRepeat+                                                  // LABEL:
        exp.first+                                              // acumulador = result expr
        "LOAD "+exp.second+"\n"+                                 // LOAD result expr
        "SUB ONE\n" +                                                 // If it returns 1(true), end the loop
        "JMPZ "+endRepeat.substr(0,endRepeat.size()-2)+"\n"+          // if acumulador == 0 goto EndLabel
        stmt_seq->resolve()+                                          // code to repeat
        "JMP "+startRepeat.substr(0,startRepeat.size()-2)+"\n"+       // goto LABEL
        endRepeat;                                                    // EndLabel:
}
//------------------------------------------------------------------------------
void AssignStmt::Print() {
    cout << id << " := ";
    if( expr != nullptr){
        expr -> Print();
    }
}
string AssignStmt::resolve() {
    pair<string,string> exp = expr -> resolve();
    return 
        labelGen.resolveId(id)+
        exp.first+
        "LOAD " +exp.second + "\n"+
        "STORE "+id+"\n";
}
//------------------------------------------------------------------------------
void ReadStmt::Print() {
    cout << "read " << id;
}
string ReadStmt::resolve() {
    return "INPUT "+id+"\n";
}
//------------------------------------------------------------------------------
void WriteStmt::Print() {
    cout << "write ";
    if( expr != nullptr){
        expr -> Print();
    }
}
string WriteStmt::resolve() {
    pair<string,string> exp = expr -> resolve();
    return 
        exp.first +
        "LOAD "+exp.second+"\n"+
        "STORE AUX\nOUTPUT AUX\n";
}
//------------------------------------------------------------------------------
void Expression::Print() {
    if( simple_expr != nullptr){
        simple_expr -> Print();
    }
    if( simple_expr2 != nullptr) {
        cout << OperationMap[op] << " ";
    }
    if( simple_expr2 != nullptr){
        simple_expr2 -> Print();
    }
}
pair<string,string> Expression::resolve(){
    if(isSimpleExp()) {
        return this->simple_expr->resolve();
    }
    else if(isComparison()){
        string labelNeg = labelGen.getEndComparison();
        string labelFim =  labelGen.getEndComparison();
        pair<string,string> simpleExp = this->simple_expr->resolve();
        pair<string,string> simpleExp2 = this->simple_expr2->resolve();
        if(this->op == Op::Less){
            string labelResult = labelGen.getResult();
            return make_pair(
                simpleExp.first+
                simpleExp2.first+
                "LOAD "+simpleExp2.second + "\n"+
                "STORE AUX\n"+
                "LOAD "+simpleExp.second + "\n"+
                "SUB AUX\n"+
                "JMPZ " + labelNeg.substr(0,labelNeg.size()-2) + "\n"+
                "JMPP " + labelNeg.substr(0,labelNeg.size()-2) + "\n"+
                "LOAD ONE\n" +
                "STORE " + labelResult + "\n"+
                "JMP " + labelFim.substr(0,labelFim.size()-2) + "\n"+
                labelNeg +
                "LOAD ZERO\n" +
                "STORE " + labelResult + "\n"+
                labelFim,labelResult);
        }
        else if(this->op == Op::Equal){
            string labelResult = labelGen.getResult();
            return make_pair(
                simpleExp.first+
                simpleExp2.first+
                "LOAD "+simpleExp2.second + "\n"+
                "STORE AUX\n"+
                "LOAD "+simpleExp.second + "\n"+
                "SUB AUX\n"+
                "JMPP " + labelNeg.substr(0,labelNeg.size()-2) + "\n"+
                "JMPN " + labelNeg.substr(0,labelNeg.size()-2) + "\n"+
                "LOAD ONE\n" +
                "STORE " + labelResult + "\n"+
                "JMP " + labelFim.substr(0,labelFim.size()-2) + "\n"+
                labelNeg +
                "LOAD ZERO\n" +
                "STORE " + labelResult + "\n"+
                labelFim,labelResult);   
        }
    }
    return make_pair("ERROR Expression does not derives into simple expression or comparison expression\n","");
}

//------------------------------------------------------------------------------
void SimpleExpression::Print() {
    if( simple_expr != nullptr){
        simple_expr -> Print();
    }
    if( simple_expr != nullptr){
        cout << OperationMap[op] << " ";
    }
    if( term != nullptr){
        term -> Print();
    }
}
pair<string,string> SimpleExpression::resolve() {
    if(isTerm()) { return this->term->resolve(); }
    else if(isAddOp()) { 
        pair<string,string> simpleExp = this->simple_expr->resolve();
        pair<string,string> term = this->term->resolve();
        if     (this->op == Op::Add) {
            string labelResult = labelGen.getResult();
            return make_pair(
                simpleExp.first +
                term.first +
                "LOAD " +term.second + "\n"              // Evaluate the second operator
                "STORE AUX\n" +     // Save the evaluation value 
                "LOAD " + simpleExp.second + "\n"       // Evaluate the first operator
                "ADD AUX\n" +         // Sum both operators
                "STORE " + labelResult +"\n"       // Save the result
                ,labelResult);
        }
        else if(this->op == Op::Sub) {
            string labelResult = labelGen.getResult();
            return make_pair(
                simpleExp.first +
                term.first +
                "LOAD " +term.second + "\n"              // Evaluate the second operator
                "STORE AUX\n" +     // Save the evaluation value 
                "LOAD " + simpleExp.second + "\n"       // Evaluate the first operator
                "SUB AUX\n"+         // Subtract both operators
                "STORE " + labelResult +"\n"       // Save the result
                ,labelResult);
        }

    }
    return make_pair("ERROR Simple expression does not derives into simple expression or Add Operation\n","");
}
//------------------------------------------------------------------------------
void Term::Print() {
    if( term != nullptr){
        term -> Print();
    }
    if( term != nullptr){
        cout << OperationMap[op] << " ";
    }
    if( factor != nullptr){
        factor -> Print();
    }
}
pair<string,string> Term::resolve(){
    if(isFactor()) return this->factor->resolve();
    else if(isMulOp()) {
        pair<string,string> term = this->term->resolve();
        pair<string,string> factor = this->factor->resolve();
        if (this->op == Op::Mul){
            string resultLabel = labelGen.getResult();
            return
                make_pair(
                factor.first +                // Evaluate the second operator
                term.first +                  // Evaluate the first operator
                "LOAD " + factor.second+"\n"+    // Load the evaluation value
                "STORE AUX\n" +         // Store the result of the seconf operator
                "LOAD "+term.second+"\n"+ // Load the first operator
                "MULT AUX\n"            // Multiply both operators
                "STORE "+resultLabel+"\n" // Store the result of the multiplication
                ,resultLabel);
        }
        else if (this->op == Op::Div){
            string resultLabel = labelGen.getResult();
            return
                make_pair(
                factor.first +                // Evaluate the second operator
                term.first +                  // Evaluate the first operator
                "LOAD " + factor.second+"\n"+    // Load the evaluation value
                "STORE AUX\n" +         // Store the result of the seconf operator
                "LOAD "+term.second+"\n"+ // Load the first operator
                "DIV AUX\n"            // Multiply both operators
                "STORE "+resultLabel+"\n" // Store the result of the multiplication
                ,resultLabel);
        }
    }
    return make_pair("ERROR Term does not derives into Factor or Mul Operation\n","");
}
//------------------------------------------------------------------------------
void Factor::Print() {
    if( expr != nullptr){
        expr -> Print();
    }
    if( id != ""){
        cout << id << " ";
    }
    if( expr == nullptr && id == ""){
        cout << val << " ";
    }
}
pair<string,string> Factor::resolve(){
    if (isIdentifier()) return make_pair("",getIdentifier());               // Loads the value of the identifier into the accumulator
    else if (isNumber()) {
            return 
                make_pair("",labelGen.resolveNum(getNumber()));
        }       // Loads the value of the number into the accumulator
    else if (isExpression()) 
    return getExpression()->resolve();          // Return the assembly of the resolved expression
    return make_pair("ERROR Factor does not derives into Identifier, Number or Expression\n","");
}
//------------------------------------------------------------------------------

void SymbolTable::updateSymbolTable(string id, int line, bool declared){
    Sym sym;
    vector<int> lines;
    lines.push_back(line);
    sym.lines = lines;
    sym.declared = declared;
    auto it = symbol_table.find(id);
    if(it != symbol_table.end()){
        it -> second.lines.push_back(line);
        if(it -> second.declared == false){
            it -> second.declared = declared;
        }
    }
    else{
        symbol_table[id] = sym;
    }
}
void SymbolTable::OutputJson(){
    fstream file;
    file.open("symbolstable.json",ios::out);
    file << "{\n";
    for(auto it = symbol_table.begin(); it != symbol_table.end(); it++){
        file << "\t\"" << it->first << "\": {\n";
        file << "\t\t\"lines\": [";
        for(auto it2 = it->second.lines.begin(); it2 != it->second.lines.end(); it2++){
            file << *it2;
            if(it2 != it->second.lines.end()-1){
                file << ",";
            }
        }
        file << "],\n";
        file << "\t\t\"declared\": " << it->second.declared << "\n";
        file << "\t}";
        if(std::next(it) != symbol_table.end()){
            file << ",";
        }
        file << "\n";
    }
    file << "}\n";
    file.close();
}