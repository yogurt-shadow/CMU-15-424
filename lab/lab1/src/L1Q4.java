import java.util.HashMap;
import java.lang.Exception;

class Term {}

class Number extends Term {
    public double x;
    Number(double x) {
	this.x = x;
    }
}

class Variable extends Term {
    public String x;
    Variable(String x) {
	this.x = x;
    }
}

class Plus extends Term {
    public Term t1;
    public Term t2;
    Plus(Term t1, Term t2) {
	this.t1 = t1;
	this.t2 = t2;
    }
}

class Times extends Term {
    public Term t1;
    public Term t2;
    Times(Term t1, Term t2) {
	this.t1 = t1;
	this.t2 = t2;
    }
}

class Formula {}

class GreaterEqual extends Formula {
    public Term t1;
    public Term t2;
    GreaterEqual(Term t1, Term t2) {
	this.t1 = t1;
	this.t2 = t2;
    }
}

class Equal extends Formula {
    public Term t1;
    public Term t2;
    Equal(Term t1, Term t2) {
	this.t1 = t1;
	this.t2 = t2;
    }
}

class Not extends Formula {
    public Formula p;
    Not(Formula p) {
	this.p = p;
    }
}

class And extends Formula {
    public Formula p;
    public Formula q;
    And(Formula p, Formula q) {
	this.p = p;
	this.q = q;
    }
}

class Or extends Formula {
    public Formula p;
    public Formula q;
    Or(Formula p, Formula q) {
	this.p = p;
	this.q = q;
    }
}

class Implies extends Formula {
    public Formula p;
    public Formula q;
    Implies(Formula p, Formula q) {
	this.p = p;
	this.q = q;
    }
}

class Equiv extends Formula {
    public Formula p;
    public Formula q;
    Equiv(Formula p, Formula q) {
	this.p = p;
	this.q = q;
    }
}

class Forall extends Formula {
    public String x;
    public Formula p;
    Forall(String x, Formula p) {
	this.x = x;
	this.p = p;
    }
}

class Exists extends Formula {
    public String x;
    public Formula p;
    Exists(String x, Formula p) {
	this.x = x;
	this.p = p;
    }
}

class BadPatternMatch extends Exception {
    BadPatternMatch(){}
}

class Unimplemented extends Exception {
    Unimplemented(){
    	System.out.println("variable not implemented in this state!");
    }
}

/* For readablity */
class State extends HashMap<String, Double> {}

class Interpreter {
    static Double interpret(State state, Term term) throws Exception {
	if (term instanceof Number) {
	    Number n = (Number)term;
	    return n.x;
	} else if (term instanceof Plus) {
	    Plus p = (Plus)term;
	    return interpret(state, p.t1) + interpret(state,p.t2);
	} else if (term instanceof Variable) {
	    Variable x = (Variable)term;
	    if (!state.containsKey(x.x)){
	    	throw new Unimplemented();
	    }
	    else {
	    	return state.get(x.x);
	    }
	} else if (term instanceof Times) {
	    Times p = (Times)term;
	    return interpret(state, p.t1) * interpret(state, p.t2);
	} else {
	    throw new BadPatternMatch();
	}
    }

    static boolean interpret(State state, Formula formula) throws Exception {
	if (formula instanceof GreaterEqual) {
	    GreaterEqual gr = (GreaterEqual)formula;
	    return interpret(state, gr.t1) >= interpret(state, gr.t2);
	} else if (formula instanceof Equal) {
	    Equal n = (Equal)formula;
	    return interpret(state, n.t1).equals(interpret(state, n.t2));
	} else if (formula instanceof Not) {
	    Not n = (Not)formula;
	    return !interpret(state, n.p);
	} else if (formula instanceof And) {
	    And n = (And)formula;
	    return interpret(state, n.p) && interpret(state, n.q);
	} else if (formula instanceof Or) {
	    Or n = (Or)formula;
	    return interpret(state, n.p) || interpret(state, n.q);
	} else if (formula instanceof Implies) {
	    Implies n = (Implies)formula;
	    return !interpret(state, n.p) || interpret(state, n.q);
	} else if (formula instanceof Equiv) {
	    Equiv n = (Equiv)formula;
	    return interpret(state, n.p) == interpret(state, n.q);
	} else if (formula instanceof Forall) {
	    Forall n = (Forall)formula;
	    boolean have = state.containsKey(n.x);
	    double copy = have ? state.get(n.x) : 0;
	    for(double value = 0; value <= 10; value += 0.5){
	    	state.put(n.x, value);
	    	if(!interpret(state, n.p)){
	    		state.put(n.x, copy);
	    		if(!have){
	    			state.remove(n.x);
	    		}
	    		return false;
	    	}
	    }
	    state.put(n.x, copy);
		if(!have){
			state.remove(n.x);
		}
	    return true;
	} else if (formula instanceof Exists) {
	    Exists n = (Exists)formula;
	    boolean have = state.containsKey(n.x);
	    double copy = have ? state.get(n.x) : 0;
	    for(double value = 0; value <= 10; value += 0.5){
	    	state.put(n.x, value);
	    	if(interpret(state, n.p)){
	    		state.put(n.x, copy);
	    		if(!have){
	    			state.remove(n.x);
	    		}
	    		return true;
	    	}
	    }
	    state.put(n.x, copy);
		if(!have){
			state.remove(n.x);
		}
	    return false;
	} else {
	    throw new BadPatternMatch();
	}
    }
}

class L1Q4 {
    public static final void main (String[] args) throws Exception {
	State empty = new State();

	//Example of a context with some variables.
	State nonempty = new State();
	nonempty.put("x", 6.0);
	nonempty.put("y", 36.0);

	//Example of some terms.
	Term one = new Number(1.0);
	Term two = new Number(2.0);
	Term sum = new Plus(one, two);

	//Example of a formula.
	Formula twoGTone = new GreaterEqual(two, one);

	//Some debugging output.
	System.out.println("(1 + 2) = " + Interpreter.interpret(empty, sum));
	System.out.println("(2 > 1) = " + Interpreter.interpret(nonempty, twoGTone));
	//TODO add some more test cases.

	/* Varaible Term in given state. */
	Term x = new Variable("x");
	Term y = new Variable("y");
	Term plus1 = new Plus(x, y);
	Term time1 = new Times(x, y);
	Term plus_then_time = new Times(plus1, plus1);
	System.out.println("in state {x -> 6, y -> 36}, (x + y) =  " + Interpreter.interpret(nonempty, plus1));
	System.out.println("in state {x -> 6, y -> 36}, (x * y) =  " + Interpreter.interpret(nonempty, time1));
	System.out.println("in state {x -> 6, y -> 36}, (x + y) ^ 2 =  " + Interpreter.interpret(nonempty, plus_then_time));

	/* Formula Test. */
	System.out.println("---------Formula Test---------");
	System.out.println();
	State nonempty2 = new State();
	nonempty2.put("x", 1.0);
	nonempty2.put("y", 2.0);
	// Equal
	System.out.println("---------Equal Test---------");
	Formula one_one = new Equal(one, one);
	Term y_times_2 = new Times(y, two);
	Term x_times_4 = new Times(x, new Number(4.0));
	Formula x_times_4_y_times_2 = new Equal(x_times_4, y_times_2);

	System.out.println("(1 = 1) = " + Interpreter.interpret(empty, one_one));
	System.out.println("in state {x -> 1, y -> 2}, (x * 4 = 2 * y) = " + Interpreter.interpret(nonempty2, x_times_4_y_times_2));
	System.out.println();
	// Not
	System.out.println("---------Not Test---------");
	Formula not1 = new Not(one_one);
	Formula not2 = new Not(x_times_4_y_times_2);

	System.out.println("(1 = 1) != " + Interpreter.interpret(empty, not1));
	System.out.println("in state {x -> 1, y -> 2}, (x * 4 = 2 * y) != " + Interpreter.interpret(nonempty2, not2));
	System.out.println();
	// And
	System.out.println("---------And Test---------");
	Formula one_three = new Equal(one, new Number(3.0));
	Formula and1 = new And(one_one, one_three);
	Formula x_plus_y_3 = new Equal(new Number(3.0), new Plus(x, y));
	Formula and2 = new And(x_times_4_y_times_2, x_plus_y_3);
	System.out.println("(1 = 1) & (1 = 3) = " + Interpreter.interpret(empty, and1));
	System.out.println("in state {x -> 1, y -> 2}, (x * 4 = 2 * y) & (x + y = 3) = " + Interpreter.interpret(nonempty2, and2));
	System.out.println();
	// Or
	System.out.println("---------Or Test---------");
	Formula or = new Or(one_three, one_one);
	System.out.println("(1 = 1) | (1 = 3) = " + Interpreter.interpret(empty, or));
	System.out.println();
	// Implies
	System.out.println("---------Implies Test---------");
	Formula imply1 = new Implies(one_one, one_three);
	Formula imply2 = new Implies(one_three, one_one);
	System.out.println("(1 = 1) -> (1 = 3) = " + Interpreter.interpret(empty, imply1));
	System.out.println("(1 = 3) -> (1 = 1) = " + Interpreter.interpret(empty, imply2));
	System.out.println();
	// Equiv 
	System.out.println("---------Equiv Test---------");
	Formula equ1 = new Equiv(one_one, one_three);
	Formula equ2 = new Equiv(x_plus_y_3, x_times_4_y_times_2);
	System.out.println("(1 = 1) <-> (1 = 3) = " + Interpreter.interpret(empty, equ1));
	System.out.println("in state {x -> 1, y -> 2}, (x + y = 3) <-> (x * 4 = 2 * y) = " + Interpreter.interpret(nonempty2, equ2));
	System.out.println();
	// Forall
	System.out.println("---------Forall Test---------");
	Formula forall1 = new Forall("y", new GreaterEqual(new Times(y, y), new Number(0.0)));
	Formula forall2 = new Forall("y", new Equal(new Times(y, y), new Number(1.0)));
	System.out.println("size: " + empty.size());

	System.out.println("forall y, (y * y >= 0) = " + Interpreter.interpret(empty, forall1));
	System.out.println("size: " + empty.size());

	System.out.println("forall y, (y * y = 1) = " + Interpreter.interpret(empty, forall2));
	System.out.println("size: " + empty.size());
	System.out.println();

	// Exists
	System.out.println("---------Exists Test---------");
	Term z = new Variable("z");
	Formula exists1 = new Exists("y", new Equal(new Times(y, y), new Number(1.0)));
	Formula exists2 = new Exists("z", new Equal(new Variable("z"), new Plus(x, y)));
	Formula exists3 = new Exists("z", new Equal(new Number(-1), new Times(z, z)));
	System.out.println("size: " + empty.size());
	System.out.println("Exists y, (y * y = 1) = " + Interpreter.interpret(empty, exists1));
	System.out.println("size: " + empty.size());		
	System.out.println("in state {x -> 1, y -> 2}, exists z, (z = x + y) = " + Interpreter.interpret(nonempty2, exists2));
	System.out.println("size: " + empty.size());
	System.out.println("in state {x -> 1, y -> 2}, exists z, (z * z = -1) = " + Interpreter.interpret(nonempty2, exists3));
	System.out.println("size: " + empty.size());
	System.out.println();
    }
}
