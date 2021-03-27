#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

using namespace std;

class Unimplemented: public exception {
public:
  Unimplemented() {
    cout << "variable not implemented in this state!" << endl;
  }
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
    if(state -> find(x -> name) == state -> end()){
      throw Unimplemented();
    }
    else{
      return state -> at(x -> name);
    }
  } else if(Plus* p = dynamic_cast<Plus*>(term)) {
    return interpret_term(state, p->left) + interpret_term(state, p->right);
  } else if(Times* t = dynamic_cast<Times*>(term)) {
    return interpret_term(state, t -> left) * interpret_term(state, t -> right);
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
    return interpret_term(state, gt -> left) == interpret_term(state, gt -> right);
  } else if(Not* n = dynamic_cast<Not*>(formula)) {
    return !interpret_formula(state, n -> child);
  } else if(And* a = dynamic_cast<And*>(formula)) {
    return interpret_formula(state, a -> left) && interpret_formula(state, a -> right);
  } else if(Or* o = dynamic_cast<Or*>(formula)) {
    return interpret_formula(state, o -> left) || interpret_formula(state, o -> right);
  } else if(Implies* i = dynamic_cast<Implies*>(formula)) {
    return !interpret_formula(state, i ->left) || interpret_formula(state, i -> right);
  } else if(Equiv* e = dynamic_cast<Equiv*>(formula)) {
    return interpret_formula(state, e -> left) == interpret_formula(state, e -> right);
  } else if(Forall* f = dynamic_cast<Forall*>(formula)) {
    bool have = (state -> find(f ->name) != state -> end());
    float copy = have ? state -> at(f -> name) : 0;
    if(have){
      state -> erase(f -> name);
    }
    for(float value = 0; value <= 10; value += 0.5){
      state -> insert(state -> begin(), pair<string, float>(f -> name, value));
      if(!interpret_formula(state, f -> child)){
        state -> insert(state -> begin(), pair<string, float>(f -> name, copy));
        if(!have){
          state -> erase(f -> name);
        }
        return false;
      }
      state -> erase(f -> name);
    }
    state -> insert(state -> begin(), pair<string, float>(f -> name, copy));
    if(!have){
      state -> erase(f -> name);
    }
    return true;
  } else if(Exists* e = dynamic_cast<Exists*>(formula)) {
    bool have = (state -> find(e -> name) != state -> end());
    float copy = have ? state -> at(e -> name) : 0;
    if(have){
      state -> erase(e -> name);
    }
    for(float value = 0; value <= 10; value += 0.5){
      state -> insert(state -> begin(), pair<string, float>(e -> name, value));
      if(interpret_formula(state, e -> child)){
        state -> insert(state -> begin(), pair<string, float>(e -> name, copy));
        if(!have){
          state -> erase(e -> name);
        }
        return true;
      }
      state -> erase(e -> name);
    }
    state -> insert(state -> begin(), pair<string, float>(e -> name, copy));
      if(!have){
        state -> erase(e -> name);
      }
      return false;
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
            << "2 > 1 is " << btos( interpret_formula(empty_state, twoGTone) )
            << endl;
  //Task 3 add some more test cases.
  
  Term* x = new Variable("x");
  Term* y = new Variable("y");
  Term* plus1 = new Plus(x, y);
  Term* time1 = new Times(x, y);
  Term* plus_then_time = new Times(plus1, plus1);
  cout << "in state {x -> 6, y -> 36}, (x + y) =  " << interpret_term(some_state, plus1) << endl;
  cout << "in state {x -> 6, y -> 36}, (x * y) =  " << interpret_term(some_state, time1) << endl;
  cout << "in state {x -> 6, y -> 36}, (x * y) ^ 2 =  " << interpret_term(some_state, plus_then_time) << endl;
  
  /* Formula Test. */
  cout << "---------Formula Test---------" << endl;
  cout << endl;
  map<string, float>* some_state2 = new map<string, float>();
  some_state2 -> insert(some_state2 -> begin(), pair<string, float>("x", 1.0));
  some_state2 -> insert(some_state2 -> begin(), pair<string, float>("y", 2.0));

  /* Equal */
  cout << "---------Equal Test---------" << endl;
  Formula* one_one = new Equal(one, one);
  Term* y_times_2 = new Times(y, two);
  Term* x_times_4 = new Times(x, new Number(4.0));
  Formula* x_times_4_y_times_2 = new Equal(x_times_4, y_times_2);

  cout << "(1 = 1) = " << btos(interpret_formula(empty_state, one_one)) << endl;
  cout << "in state {x -> 1, y -> 2}, (x * 4 = 2 * y) = " << btos(interpret_formula(some_state2, x_times_4_y_times_2)) << endl;
  cout << endl;

  /* Not */
  cout << "---------Not Test---------" << endl;
  Formula* not1 = new Not(one_one);
  Formula* not2 = new Not(x_times_4_y_times_2);

  cout << "(1 = 1) != " << btos(interpret_formula(empty_state, not1)) << endl;
  cout << "in state {x -> 1, y -> 2}, (x * 4 = 2 * y) != " << btos(interpret_formula(some_state2, not2)) << endl;
  cout << endl;
  
  /* And */
  cout << "---------And Test---------" << endl;
  Formula* one_three = new Equal(one, new Number(3.0));
  Formula* and1 = new And(one_one, one_three);
  Formula* x_plus_y_3 = new Equal(new Number(3.0), new Plus(x, y));
  Formula* and2 = new And(x_times_4_y_times_2, x_plus_y_3);
  cout << "(1 = 1) & (1 = 3) = " << btos(interpret_formula(empty_state, and1)) << endl;
  cout << "in state {x -> 1, y -> 2}, (x * 4 = 2 * y) & (x + y = 3) = " << btos(interpret_formula(some_state2, and2)) << endl;
  cout << endl;

  /* Or */
  cout << "---------Or Test---------" << endl;
  Formula* orr = new Or(one_three, one_one);
  cout << "(1 = 1) | (1 = 3) = " << btos(interpret_formula(empty_state, orr)) << endl;
  cout << endl;

  /* implies */
  cout << "---------Implies Test---------" << endl;
  Formula* imply1 = new Implies(one_one, one_three);
  Formula* imply2 = new Implies(one_three, one_one);
  cout << "(1 = 1) -> (1 = 3) = " << btos(interpret_formula(empty_state, imply1)) << endl;
  cout << "(1 = 3) -> (1 = 1) = " << btos(interpret_formula(empty_state, imply2)) << endl;
  cout << endl;

  /* Equiv */ 
  cout << "---------Equiv Test---------" << endl;
  Formula* equ1 = new Equiv(one_one, one_three);
	Formula* equ2 = new Equiv(x_plus_y_3, x_times_4_y_times_2);
  cout << "(1 = 1) <-> (1 = 3) = " << btos(interpret_formula(empty_state, equ1)) << endl;
  cout << "in state {x -> 1, y -> 2}, (x + y = 3) <-> (x * 4 = 2 * y) = " << btos(interpret_formula(some_state2, equ2)) << endl;
  cout << endl;

  /* Forall */
  cout << "---------Forall Test---------" << endl;
  Formula* forall1 = new Forall("y", new GreaterEqual(new Times(y, y), new Number(0.0)));
	Formula* forall2 = new Forall("y", new Equal(new Times(y, y), new Number(1.0)));
  cout << "size: " << empty_state -> size() << endl;

  cout << "forall y, (y * y >= 0) = " << btos(interpret_formula(empty_state, forall1)) << endl; 
  cout << "size: " << empty_state -> size() << endl;
  
  cout << "forall y, (y * y = 1) = " << btos(interpret_formula(empty_state, forall2)) << endl;
  cout << "size: " << empty_state -> size() << endl;
  cout << endl;

  /* Exists */
  cout << "---------Exists Test---------" << endl;
  Term* z = new Variable("z");
  Formula* exists1 = new Exists("y", new Equal(new Times(y, y), new Number(1.0)));
	Formula* exists2 = new Exists("z", new Equal(new Variable("z"), new Plus(x, y)));
	Formula* exists3 = new Exists("z", new Equal(new Number(-1), new Times(z, z)));
  cout << "size: " << empty_state -> size() << endl;  
  cout << "Exists y, (y * y = 1) = " << btos(interpret_formula(empty_state, exists1)) << endl;

  cout << "size: " << empty_state -> size() << endl;
  cout << "in state {x -> 1, y -> 2}, exists z, (z = x + y) = " << btos(interpret_formula(some_state2, exists2)) << endl;

  cout << "size: " << empty_state -> size() << endl;
  cout << "in state {x -> 1, y -> 2}, exists z, (z * z = -1) = " << btos(interpret_formula(some_state2, exists3)) << endl;
  cout << "size: " << empty_state -> size() << endl;
  cout << endl;
  return 0;
}
