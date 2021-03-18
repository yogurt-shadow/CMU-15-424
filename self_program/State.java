import java.util.*;

public class State{
	Map<String, Integer> map;

	State(){
		map = new HashMap<>();
	}

	void put(String str, int x){
		map.put(str, x);
	}

	int getValue(String str){
		for(String ele: map.keySet()){
			if(str.equals(ele)){
				return map.get(ele);
			}
		}
		int res = 0;
		for(int i = 0; i < str.length(); i++){
			res = 10 * res + str.charAt(i) - '0';
		}
		return res;
	}
}
