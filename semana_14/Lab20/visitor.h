#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

class BinaryExp;
class NumberExp;
class BoolExp;
class IdentifierExp;
class AssignStatement;
class PrintStatement;
class IfStatement;
class WhileStatement;
class VarDec;
class VarDecList;
class StatementList;
class Body;
class FCallExp;
class FunDec;
class FunDecList;
class ReturnStatement;
class Program;


class Visitor {
public:
    // 
    virtual void visit(Program* p) = 0;
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(BoolExp* exp) = 0;
    virtual int visit(IdentifierExp* exp) = 0;
    virtual int visit(FCallExp* exp) = 0;
    virtual void visit(ReturnStatement* stm) = 0;
    virtual void visit(FunDec* f)=0;
    virtual void visit(FunDecList* f)=0;
    virtual void visit(AssignStatement* stm) = 0;
    virtual void visit(PrintStatement* stm) = 0;
    virtual void visit(IfStatement* stm) = 0;
    virtual void visit(WhileStatement* stm) = 0;
    virtual void visit(VarDec* stm) = 0;
    virtual void visit(VarDecList* stm) = 0;
    virtual void visit(StatementList* stm) = 0;
    virtual void visit(Body* b) = 0;
};



class GenCodeVisitor : public Visitor {
private:
    std::ostream& out;
public:
    GenCodeVisitor(std::ostream& out) : out(out) {}
    void generar(Program* program);
    unordered_map<string, int> memoria;
    unordered_map<string, bool> memoriaGlobal;
    int offset = -8;
    int labelcont = 0;
    bool entornoFuncion = false;
    string nombreFuncion;
    void visit(Program* p) override;
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(BoolExp* exp) override;
    int visit(IdentifierExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    int visit(FCallExp* exp) override;
    void visit(ReturnStatement* stm) override;
    void visit(FunDec* f) override;
    void visit(FunDecList* f) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    void visit(VarDec* stm) override;
    void visit(VarDecList* stm) override;
    void visit(StatementList* stm) override;
    void visit(Body* b) override;
};

class OptimizedVisitor : public Visitor{
public:
    //int visit(Program* program) override;
    OptimizedVisitor();
    void visit(Program* p) override{}
    int imprimir(Program* program);
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(BoolExp* exp) override;
    int visit(IdentifierExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(VarDec* stm) override;
    void visit(VarDecList* stm) override;
    void visit(StatementList* stm) override;
    void visit(Body* b) override;
    void visit(WhileStatement* stm) override;

    int visit(FCallExp* exp) override;
    void visit(ReturnStatement* stm) override;
    void visit(FunDec* f) override;
    void visit(FunDecList* e) override;

    // for
};

#endif // VISITOR_H