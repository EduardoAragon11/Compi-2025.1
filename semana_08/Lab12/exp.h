#ifndef EXP_H
#define EXP_H

#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"
using namespace std;
enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP,LT_OP, LE_OP, EQ_OP, AND_OP, OR_OP};
enum UnaryOp { NOT_OP, NOTHING_OP };

class Body;

class ImpValue {
public:
  ImpValue(string tipo,int valor, bool bol){
    type = tipo;
    int_value = valor;
    bool_value = bol;
  };
  ImpValue(string tipo){
      type = tipo;
      int_value=0;
      bool_value=false;
  }
  ImpValue(){};
  ~ImpValue(){};
  string type;
  int int_value;
  bool bool_value;
};

class Exp {
public:
    static string unaryToChar(UnaryOp op);
    virtual ImpValue  accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;
    static string binopToChar(BinaryOp op);
};
class IFExp : public Exp {
public:
    Exp *cond,*left, *right;
    IFExp(Exp *cond, Exp* l, Exp* r);
    ImpValue accept(Visitor* visitor);
    ~IFExp();
};


class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ImpValue accept(Visitor* visitor);
    ~BinaryExp();
};

class UnaryExp : public Exp {
public:
    Exp *exp;
    UnaryOp op;
    UnaryExp(Exp* e, UnaryOp op);
    ImpValue accept(Visitor* visitor);
    ~UnaryExp(){
        delete exp;
    }
};

class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    ImpValue accept(Visitor* visitor);
    ~NumberExp();
};

class BoolExp : public Exp {
public:
    int value;
    BoolExp(bool v);
    ImpValue accept(Visitor* visitor);
    ~BoolExp();
};

class IdentifierExp : public Exp {
public:
    std::string name;
    IdentifierExp(const std::string& n);
    ImpValue accept(Visitor* visitor);
    ~IdentifierExp();
};

class Stm {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
};

class AssignStatement : public Stm {
public:
    std::string id;
    Exp* rhs;
    AssignStatement(std::string id, Exp* e);
    int accept(Visitor* visitor);
    ~AssignStatement();
};

class PrintStatement : public Stm {
public:
    Exp* e;
    PrintStatement(Exp* e);
    int accept(Visitor* visitor);
    ~PrintStatement();
};


class IfStatement : public Stm {
public:
    Exp* condition;
    Body* then;
    Body* els;
    IfStatement(Exp* condition, Body* then, Body* els);
    int accept(Visitor* visitor);
    ~IfStatement();
};
class WhileStatement : public Stm {
public:
    Exp* condition;
    Body* b;
    WhileStatement(Exp* condition, Body* b);
    int accept(Visitor* visitor);
    ~WhileStatement();
};

//FOR(int,int,int) stmlist endfor
class ForStatement : public Stm {
public:
    string id;
    Exp* start;
    Exp* end;
    Exp* step;
    Body* b;
    ForStatement(string id,Exp* start, Exp* end, Exp* step, Body* b);
    int accept(Visitor* visitor);
    ~ForStatement();
};

class VarDec {
public:
    string type;
    list<string> vars;
    VarDec(string type, list<string> vars);
    int accept(Visitor* visitor);
    ~VarDec();
};

class VarDecList{
public:
    list<VarDec*> vardecs;
    VarDecList();
    void add(VarDec* vardec);
    int accept(Visitor* visitor);
    ~VarDecList();
};

class StatementList {
public:
    list<Stm*> stms;
    StatementList();
    void add(Stm* stm);
    int accept(Visitor* visitor);
    ~StatementList();
};


class Body{
public:
    VarDecList* vardecs;
    StatementList* slist;
    Body(VarDecList* vardecs, StatementList* stms);
    int accept(Visitor* visitor);
    ~Body();
};

class Program {
public:
    Body* body;
    Program(Body* body);
    ~Program();
};



#endif // EXP_H