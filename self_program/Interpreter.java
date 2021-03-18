public class Interpreter{
	State state;

	Interpreter(State state){
		this.state = state;
	}

	int value(String str){
		for(int i = 0; i < str.length(); i++){
			if(str.charAt(i) == '('){
				int j = i + 1;
				int ans = 1;
				while(j < str.length()){
					if(str.charAt(j) == '('){
						ans++;
					}
					if(str.charAt(j) == ')'){
						ans--;
						if(ans == 0){
							break;
						}
					}
					j++;
				}
				String copy = str.substring(i + 1, j);
				int val = value(copy);
				String str2 = str.substring(0, i) + String.valueOf(val) + str.substring(j + 1, str.length());
				return value(str2);
			}
		}
		for(int i = 0; i < str.length(); i++){
			if(str.charAt(i) == '+' || str.charAt(i) == '-'){
				int sign = (str.charAt(i) == '+') ? 1 : -1;
				return value(str.substring(0, i)) + sign * value(str.substring(i + 1, str.length()));
			}
		}

		for(int i = 0; i < str.length(); i++){
			if(str.charAt(i) == '*'){
				return value(str.substring(0, i)) * value(str.substring(i + 1, str.length()));
			}
		}
		return state.getValue(str);
	}

	public static void main(String[] args){
		State state = new State();
		state.put("a", 12);
		state.put("b", 24);
		Interpreter inter = new Interpreter(state);
		System.out.println(inter.value("(a+123)*(13+b*2)"));
	}
}