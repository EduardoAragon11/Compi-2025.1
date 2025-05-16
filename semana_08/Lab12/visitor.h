
#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"
#include "environment.h"
#include <list>

class BinaryExp;
class NumberExp;
class BoolExp;
class IFExp;
class IdentifierExp;
class AssignStatement;
class PrintStatement;
class IfStatement;
class WhileStatement;
class ForStatement;
class VarDec;
class VarDecList;
class StatementList;
class Body;
class Program;
class ImpValue;
class UnaryExp;

class Visitor {
public:
    virtual ImpValue visit(IFExp* exp) = 0;
    virtual ImpValue visit(BinaryExp* exp) = 0;
    virtual ImpValue visit(NumberExp* exp) = 0;
    virtual ImpValue visit(BoolExp* exp) = 0;
    virtual ImpValue visit(IdentifierExp* exp) = 0;
    virtual ImpValue visit(UnaryExp* exp) = 0;
    virtual void  visit(AssignStatement* stm) = 0;
    virtual void visit(PrintStatement* stm) = 0;
    virtual void visit(IfStatement* stm) = 0;
    virtual void visit(WhileStatement* stm) = 0;
    virtual void visit(ForStatement* stm) = 0;
    virtual void visit(VarDec* stm) = 0;
    virtual void visit(VarDecList* stm) = 0;
    virtual void visit(StatementList* stm) = 0;
    virtual void visit(Body* b) = 0;
};

class PrintVisitor : public Visitor {
public:
    int iden = 0;
    void imprimir(Program* program);
    ImpValue visit(IFExp* exp) override;
    ImpValue visit(BinaryExp* exp) override;
    ImpValue visit(NumberExp* exp) override;
    ImpValue visit(BoolExp* exp) override;
    ImpValue visit(IdentifierExp* exp) override;
    ImpValue visit(UnaryExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    void visit(ForStatement* stm) override;
    void visit(VarDec* stm) override;
    void visit(VarDecList* stm) override;
    void visit(StatementList* stm) override;
    void visit(Body* b) override;
};

class EVALVisitor : public Visitor {
    Environment env;
public:
    void ejecutar(Program* program);
    ImpValue visit(IFExp* exp) override;
    ImpValue visit(BinaryExp* exp) override;
    ImpValue visit(NumberExp* exp) override;
    ImpValue visit(BoolExp* exp) override;
    ImpValue visit(IdentifierExp* exp) override;
    ImpValue visit(UnaryExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    void visit(ForStatement* stm) override;
    void visit(VarDec* stm) override;
    void visit(VarDecList* stm) override;
    void visit(StatementList* stm) override;
    void visit(Body* b) override;
};

class TypeVisitor : public Visitor {
    Environment env;
public:
    void check(Program* program);
    ImpValue visit(IFExp* exp) override;
    ImpValue visit(BinaryExp* exp) override;
    ImpValue visit(NumberExp* exp) override;
    ImpValue visit(BoolExp* exp) override;
    ImpValue visit(IdentifierExp* exp) override;
    ImpValue visit(UnaryExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    void visit(ForStatement* stm) override;
    void visit(VarDec* stm) override;
    void visit(VarDecList* stm) override;
    void visit(StatementList* stm) override;
    void visit(Body* b) override;
};

#endif// VISITOR_H