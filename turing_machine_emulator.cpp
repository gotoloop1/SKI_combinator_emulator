#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class transition {
public:
	int old_state, state;
	std::string old_character, character;
	bool move;
};

class transition2 {
public:
	int state, character;
	bool move;
};

int main(int argn, char** args) {
	// 状態遷移の規則をファイルから読み込む
	std::ifstream f(args[1]);
	std::string tmp;
	std::map<std::string, int> state;
	std::map<std::string, int> charset;
	std::vector<std::string> rcharset;
	charset["BLANK"] = 0;
	rcharset.push_back("");
	std::vector<transition> v;
	while(std::getline(f, tmp)) {
		if(tmp.empty() || tmp[0] == '#') {
			continue;
		}
		std::vector<std::string> ss;
		for(auto i: tmp) {
			if(i == ' ' || i == ',' || i == '\n' || i == '\r') {
				if(ss.empty() || ss.back() != "") {
					ss.push_back("");
				}
			}
			else {
				if(ss.empty()) {
					ss.push_back("");
				}
				ss.back() += i;
			}
		}
		if(ss.size() != 5) {
			continue;
		}
		if(state.find(ss[0]) == state.end()) {
			int s = state.size();
			state[ss[0]] = s;
		}
		if(state.find(ss[4]) == state.end()) {
			int s = state.size();
			state[ss[4]] = s;
		}
		if(ss[1] != "ANY" && charset.find(ss[1]) == charset.end()) {
			int s = charset.size();
			charset[ss[1]] = s;
			rcharset.push_back(ss[1]);
		}
		if(ss[2] != "NONE" && charset.find(ss[2]) == charset.end()) {
			int s = charset.size();
			charset[ss[2]] = s;
			rcharset.push_back(ss[2]);
		}
		v.push_back(transition{state[ss[0]], state[ss[4]], ss[1], ss[2], ss[3] == "R"});
	}
	// 高速に計算できるように状態遷移規則をテーブルに変換する
	std::vector<std::vector<transition2>> table(state.size(), std::vector<transition2>(charset.size(), transition2{-1, -1, false}));
	for(auto i: v) {
		if(i.old_character == "ANY") {
			for(int j = 1; j < charset.size(); j++) {
				if(table[i.old_state][j].state < 0) {
					if(i.character == "NONE") {
						table[i.old_state][j].state = i.state;
						table[i.old_state][j].character = j;
						table[i.old_state][j].move = i.move;
					}
					else {
						table[i.old_state][j].state = i.state;
						table[i.old_state][j].character = charset[i.character];
						table[i.old_state][j].move = i.move;
					}
				}
			}
		}
		else {
			int j = charset[i.old_character];
			if(i.character == "NONE") {
				table[i.old_state][j].state = i.state;
				table[i.old_state][j].character = j;
				table[i.old_state][j].move = i.move;
			}
			else {
				table[i.old_state][j].state = i.state;
				table[i.old_state][j].character = charset[i.character];
				table[i.old_state][j].move = i.move;
			}
		}
	}
	// 入力を読み込み、テープにセットする
	std::string input;
	std::cin >> input;
	int now = state["q_0"];
	int ac = state["q_accept"];
	int bl = charset["BLANK"];
	int nb = 0;
	std::vector<int> memory(input.size());
	for(int i = 0; i < input.size(); i++) {
		std::string s = "";
		s += input[i];
		if(charset.find(s) == charset.end()) {
			now = -1;
		}
		memory[i] = charset[s];
	}
	// 実際にチューリングマシンをシミュレートする
	while(now != ac && now >= 0) {
		int to = table[now][memory[nb]].state;
		int ch = table[now][memory[nb]].character;
		bool nex = table[now][memory[nb]].move;
		memory[nb] = ch;
		now = to;
		if(nex) {
			nb++;
		}
		else {
			nb--;
		}
		if(nb < 0) {
			now = -1;
		}
		if(nb >= memory.size()) {
			memory.push_back(bl);
		}
	}
	// 結果を出力する
	if(now == ac) {
		std::cout << "accepted" << std::endl;
		bool blank = false;
		for(auto i: memory) {
			if(i != bl) {
				if(blank) {
					std::cout << " ";
					blank = false;
				}
				std::cout << rcharset[i];
			}
			else {
				blank = true;
			}
		}
		std::cout << std::endl;
	}
	else {
		std::cout << "rejected" << std::endl;
	}
}
