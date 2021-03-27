#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

using namespace std;

class Unimplemented: public exception {
public:
  Unimplemented() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// Terms of Differential Dynamic Logic:
// e,i ::= c | x | -e | e + i | e * i
// where c is a constant and x is a variable. For our purposes, variables
// names should be C++ strings..
////////////////////////////////////////////////////////////////////////////////////////////////////

class Term {
public:
  virtual ~Term() {};
};

class Number : public Term {
public:
  float x;

  Number(float x) {
    this->x = x;
  }
};

class Plus : public Term {
public:
  Term* left;
  Term* right;

  Plus(Term* left, Term* right) {
    this->left = left;
    this->right = right;
  }
};

class Times : public Term {
public:
  Term* left;
  Term* right;

  Times(Term* left, Term* right) {
    this->left = left;
    this->right = right;
  }
};

class Variable : public Term {
public:
  string name;

  Variable(string name) {
    this->name = name;
  }
};

//Task 1: implement the rest of the cases.

/* The term interpreter.
 * state is a mapping from variables names to values.
 *
 */
float interpret_term(map<string, float>* state, Term* term) {
  if(Number* n = dynamic_cast<Number*>(term)) {
    return n->x;
  } else if(Variable* x = dynamic_cast<Variable*>(term)) {
    throw Unimplemented();
  } else if(Plus* p = dynamic_cast<Plus*>(term)) {
    return interpret_term(state, p->left) + interpret_term(state, p->right);
  } else if(Times* t = dynamic_cast<Times*>(term)) {
    throw Unimplemented();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Formulas of Differential Dynamic Logic:
// P,Q ::= !P | P && Q | P || Q | \forall x. P | \exists x. Q | e ~ f
// where x is a variable, e and f are terms, and ~ is > or < or ==.
////////////////////////////////////////////////////////////////////////////////////////////////////
class Formula {
public:
  virtual ~Formula() {};
};

class GreaterEqual : public Formula {
public:
  Term* left;
  Term* right;

  GreaterEqual(Term* left, Term* right) {
    this->left = left;
    this->right = right;
  }
};

class Equal : public Formula {
public:
  Term* left;
  Term* right;

  Equal(Term* left, Term* right) {
    this->left = left;
    this->right = right;
  }
};

class Not : public Formula {
public:
  Formula* child;

  Not(Formula* child) {
    this->child = child;
  }
};

class And : public Formula {
public:
  Formula* left;
  Formula* right;

  And(Formula* left, Formula* right) {
    this->left = left;
    this->right = right;
  }
};

class Or : public Formula {
public:
  Formula* left;
  Formula* right;

  Or(Formula* left, Formula* right) {
    this->left = left;
    this->right = right;
  }
};

class Implies : public Formula {
public:
  Formula* left;
  Formula* right;

  Implies(Formula* left, Formula* right) {
    this->left = left;
    this->right = right;
  }
};

class Equiv : public Formula {
public:
  Formula* left;
  Formula* right;

  Equiv(Formula* left, Formula* right) {
    this->left = left;
    this->right = right;
  }
};

class Forall : public Formula {
public:
  string name;
  Formula* child;

  Forall(string name, Formula* child) {
    this->name = name;
    this->child = child;
  }
};

class Exists : public Formula {
public:
  string name;
  Formula* child;

  Exists(string name, Formula* child) {
    this->name = name;
    this->child = child;
  }
};

// Task 2: implement the rest of the cases.
bool interpret_formula(map<string, float>* state, Formula* formula) {
  if(GreaterEqual* gt = dynamic_cast<GreaterEqual*>(formula)){
    return interpret_term(state, gt->left) >= interpret_term(state, gt->right);
  } else if(Equal* gt = dynamic_cast<Equal*>(formula)) {
    throw Unimplemented();
  } else if(Not* n = dynamic_cast<Not*>(formula)) {
    throw Unimplemented();
  } else if(And* a = dynamic_cast<And*>(formula)) {
    throw Unimplemented();
  } else if(Or* o = dynamic_cast<Or*>(formula)) {
    throw Unimplemented();
  } else if(Implies* i = dynamic_cast<Implies*>(formula)) {
    throw Unimplemented();
  } else if(Equiv* e = dynamic_cast<Equiv*>(formula)) {
    throw Unimplemented();
  } else if(Forall* f = dynamic_cast<Forall*>(formula)) {
    throw Unimplemented();
  } else if(Exists* e = dynamic_cast<Exists*>(formula)) {
    throw Unimplemented();
  }
}

// Some nice output for booleans.
string btos(bool b) {
  return b ? "true" : "false";
}

int main() {
  //Example of an empty state.
  map<string, float>* empty_state = new map<string,float>();

  //Example of a context with some variables.
  map<string, float>* some_state = new map<string,float>();
  some_state->insert(some_state->begin(), pair<string,float>("x", 6));
  some_state->insert(some_state->begin(), pair<string,float>("y", 36));

  //Example of some terms.
  Term* one = new Number(1.0);
  Term* two = new Number(2.0);
  Term* sum = new Plus(one, two);

  //Example of a formula.
  Formula* twoGTone = new GreaterEqual(two, one);

  //Some debugging output.
  std::cout << "1+2 = " << interpret_term(empty_state, sum)
            << std::endl
            << "2 > 1 is " << btos( interpret_formula(empty_state, twoGTone) );

  //Task 3 add some more test cases.

  cout << "\n";
  return 0;
}
