
#include <iostream>
#include <vector>
#include <array>
#include <string>
using namespace std;

enum HANDLER { none, quotes, variableormethod, numberorfloat, operation, additionorconcatenation };

HANDLER handling = HANDLER::none;

const char digits[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
//// bytes in digits / bytes in one element of the array = total num of elements in array (idk why don't ask me)
//// sizeof(digits) / sizeof(digits[0])
static bool isNumber(string str) {
	char c = str.at(0);
	for (int i = 0; i < sizeof(digits) / sizeof(digits[0]); i++) {
		if (c == digits[i]) {
			return true;
		}
	}
	return false;
}

//dummy function
string getVariable(const string& str) {
	return "VAR";
}

string handleStrConcatenation(const string& str, string(*getVariable)(const string& variableName)) {
	string fullString = "";
	// This is just the seperate parts of the string str
	string currString = "";
	int val = 0;
	bool num = false;
	for (int i = 0; i < str.length(); i++) {
		if (str.at(i) == '"' || str.at(i) == '\'') {
			// If it's the end of the quotation mark
			if (handling == quotes) {
				fullString += currString;
				handling = none;
				currString = "";
				continue;
			}
			handling = quotes;
			continue;
		}
		// If it's the first character, then it's probably a variable so print it
		else if (i == 0) {
			handling = HANDLER::variableormethod;
		}
		if (handling == quotes) {
			//cout << str.at(i);
			// If it's a special character (\)
			if (str.at(i) == '\\') {
				// Then, add that character to the string
				i++;
				if (str.at(i) == 'n') {
					currString += '\n';
				}
				continue;
			}
			currString += str.at(i);
			continue;
		}
		else if (handling == HANDLER::additionorconcatenation) {
			if (str.at(i) != ' ') {
				handling = HANDLER::variableormethod;
			}
		}
		// Here, it's not quotes so it may be a method, variable or class
		else if (handling == HANDLER::variableormethod) {
			if (str.at(i) == ' ') {
				handling = none;
				cout << currString << "test\n";
				if (isNumber(currString)) {
					// cout << currString << " \n";
					// fullString += currString;
					val += stoi(currString);
					num = true;
				}
				else {
					fullString += getVariable(currString);
				}
				currString = "";
				continue;
			}
			currString += str.at(i);
		}
		if (str.at(i) == '+') {
			handling = HANDLER::additionorconcatenation;
		}
	}
	// If it the concatenation abrubtly ends (which it will), then add the last currString
	// Or as a neat trick just add a space at the end or something to make the loop continue
	if (currString != "") {
		if (handling == HANDLER::variableormethod) {
			if (isNumber(currString)) {
				fullString += currString;
			}
			else {
				fullString += getVariable(currString);
			}
		}
	}
	handling = HANDLER::none;
	if (num) {
		fullString = to_string(val);
	}
	return fullString;
}

array<vector<string>, 8> handleParentheses(const string& str) {
	// vector<string> order;
	array<vector<string>, 8> order;
	// max of 10 parentheses deep
	// order.reserve(10);
	string currWord;
	int depth = 0;

	for (char ch : str) {
		// cout << depth << " -> " << ch << '\n';
		if (ch == ')') {
			//   cout << currWord << depth << '\n';
			order[depth].push_back(currWord);
			currWord = "";
			depth--;
		}
		else if (ch == '(') {
			// reserved character
			// currWord += '\t';
			currWord += '■';
			order[depth].push_back(currWord);
			currWord = "";
			depth++;
		}
		else {
			currWord += ch;
		}
	}
	for (int i = order.size(); i >= 0; i--) {
		if (!order[i].empty()) {
			for (int z = 0; z < order.at(i).size() - 1; z++) {
				cout << "i: " << i << " z: " << z << " -> " << order.at(i)[z] << '\n';
				order.at(i)[z] = handleStrConcatenation(order.at(i)[z], &getVariable);
			}
		}
	}
	cout << "------\n";

	// If there's any remaining word after the loop, handle it
	// if (!currWord.empty()) {
	//   order.push_back(currWord);
	// }
	return order;
}

string parseParentheses(const string& str, string(*getVariable)(const string& variableName)) {
	string fullString = "";
	// This is just the seperate parts of the string str
	string currString = "";
	for (char ch : str) {
		if (handling == HANDLER::none) {
			currString += ch;
			if (ch == '+') {

			}
		}
	}
	// If it the concatenation abrubtly ends (which it will), then add the last currString
	// Or as a neat trick just add a space at the end or something to make the loop continue
	// if (currString != "") {
	// 	if (handling == HANDLER::variableormethod) {
	// 		// Just check if first character of the string is a number
	// 		if (isNumber(currString.at(0))) {
	// 			fullString += currString;
	// 		}
	// 		else {
	// 			fullString += getVariable(currString);
	// 		}
	// 	}
	// }
	handling = HANDLER::none;
	return fullString;
}

//int main() {
//	array<vector<string>, 8> vec = handleParentheses("5 + (5 + 5) + int(str('5' + '5'))");
//	for (int i = vec.size() - 1; i >= 0; i--) {
//		if (vec[i + 1].size() != 0) {
//			vector<string> vec2 = vec.at(i);
//			// cout << i << " -> " << parseParentheses(vec.at(i), &getVariable) << '\n';
//			for (int z = 0; z < vec2.size(); z++) {
//				cout << "i: " << i << " z: " << z << " -> " << vec2[z] << '\n';
//			}
//		}
//	}
//}
