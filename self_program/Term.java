public class Term{
	boolean isConst;
	int val;
	String var;

	Term(int val){
		this.val = val;
		isConst = true;
	}

	Term(String var){
		this.var = var;
		isConst = false;
	}
}