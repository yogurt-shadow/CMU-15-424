(* Implementation of maps: Provided *)
structure Map = struct
  type (''k,'v) t = ''k -> ('v option)
  fun empty () k = NONE
  fun insert M (k,v) k' = if k = k' then SOME v else M k'
end

datatype term =
  Number of real
| Plus of term * term
| Times of term * term
| Variable of string

datatype formula =
  GreaterEqual of term * term
| Equal of term * term
| Not of formula
| And of formula * formula
| Or of formula * formula
| Implies of formula * formula
| Equiv of formula * formula
| Forall of string * formula
| Exists of string * formula

type state = (string, real) Map.t

exception Unimplemented

fun interpret_term (state: state, term:term):real =
  case term of
     Number r => r
   | Plus (t1, t2) => interpret_term (state, t1) + interpret_term (state, t2)
   | Times (t1, t2) => raise Unimplemented
   | Variable (x) => raise Unimplemented
(* Task 1: Add remaining cases below *)

fun interpret_formula (state: state, formula:formula):bool =
  case formula of
  GreaterEqual(t1,t2) => interpret_term (state, t1) >= interpret_term (state, t2)
| Equal(t1,t2) => raise Unimplemented
| Not(p) => raise Unimplemented
| And(p,q) => raise Unimplemented
| Or(p,q) => raise Unimplemented
| Implies(p,q) => raise Unimplemented
| Equiv(p,q) => raise Unimplemented
| Forall(x,p) => raise Unimplemented
| Exists(x,p) => raise Unimplemented
(* Task 2: Add remaining cases below *)

val r2s = Real.toString
val b2s = Bool.toString

structure Examples = struct
  (* Example states *)
  val empty    : state = Map.empty ()
  val nonempty : state = Map.insert (Map.insert (Map.empty ()) ("x", 6.0)) ("y", 36.0)
  (* Example terms *)
  val t1       : term = Number 1.0
  val t2       : term = Number 2.0
  val sum      : term = Plus(t1,t2)
  (* Example formulas *)
  val f        : formula = GreaterEqual(t2, t1)
  (* Tests *)
  val () = print ("1+2 = " ^ (r2s (interpret_term(empty, sum))) ^ "\n")
  val () = print ("2>1 = " ^ (b2s (interpret_formula(empty, f))) ^ "\n")
  (* Task 3: Add more tests here *)
end
