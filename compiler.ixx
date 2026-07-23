import utilities;
import std;
import concatenation;
import fileloader;

using namespace std;

/*
Powerhouse
A language designed to be faster than C++, but with less lines of code.
It aims to have three compilers: one with only what you need for competitive programming (with error handling), one without error handling, and one with error handling and all other features
The syntax is a mix of Cpp, Rust and it's own syntax.
It uses Rust data types, uses ownership, and no error handling unless you run it with the error handler compiler.
However, it also has C++ features like struct and memory management from C++

NOTES:
- comments with "bad?" are possibly bad implementations
*/

class StringVariable {
public:
	string value;

	StringVariable(string v) : value(v) {}

	int toInt() const {
		// TODO
	}
};

//class Method {
//	// Method implementation
//};

//class ClassInfo {
//public:
//	unordered_map<string, Variable> privateMembers;
//	unordered_map<string, Variable> publicMembers;
//	unordered_map<string, Method> privateMethods;
//	unordered_map<string, Method> publicMethods;
//};

unordered_map<string, int32_t> i32Mem;
unordered_map<string, int64_t> i64Mem;
unordered_map<string, float> floatMem;
unordered_map<string, char> charMem;
unordered_map<string, string> stringMem;

// State enums
// Generally used in reading lines
enum class EXPECT { none, variable, variablename, variablevalue, method, waitForEndOfLine, comment, logInputOrReturnString, logInputOrReturnInt, waitTillNoSpace, classmethodOrVariableOrBuiltinfunction};
EXPECT expect = EXPECT::none;
// Used to process variables
//enum class VAREXPECT { vartype, varassignment, varvalue };
//VAREXPECT varexpect = VAREXPECT::vartype;
//bool isInClass = false;
// the class we are in currently
//string currentClass = "";

// Variables
string currWord = "";
string varname = "";
string vartype = "";
string methodname = "";
bool firstChar = true;
const unordered_set<string> builtInSpecialMethods = { "log", "logn", "return", "input" };


// vartype -> varassignment -> varvalue
// x: int = 5 -> go past the ":", get the type and then go past the assignment and get the value
//static void checkForVariable(const char& ch) {
//	//cout << ch << '\n';
//	if (varexpect == VAREXPECT::vartype) {
//		// If we expect assignment, go past the = sign and find the expected value
//		if (ch != ' ') {
//			currWord += ch;
//		}
//		else {
//			vartype = currWord;
//			currWord = "";
//			varexpect = VAREXPECT::varassignment;
//		}
//	}
//	// This is to skip the space and equal sign to get to the value
//	else if (varexpect == VAREXPECT::varassignment) {
//		if (ch == ' ' || ch == '=') {
//			return;
//		}
//		else {
//			currWord += ch;
//			varexpect = VAREXPECT::varvalue;
//		}
//	}
//	// Add the characters to currWord. In the '\n' check in int main(), it will actually assign the variable in memory
//	else if (varexpect == VAREXPECT::varvalue) {
//		currWord += ch;
//	}
//}

//static string getVariable(const string& variableName) {
//	if (currentScopeDepth == 0) {
//		if (variables.find(variableName) != variables.end()) {
//			return variables[variableName].toString();
//		}
//	}
//	else if (currentScopeDepth == 1) {
//		// TODO
//	}
//	else if (isInClass) {
//		// TODO
//	}
//	return "undefined";
//}

int main()
{
	auto start = chrono::steady_clock::now();
	try {
		// TODO: artifically add \n to ensure that the last line will be executed
		//code += "\n";

		// Lets just read stuff faster because we don't need any extra features iostream provides
		auto buffer = fileloader::load_file_into_buffer("D:\\Visual Studio Projects\\powerhouse\\code.ph");

		for (char ch : buffer) {
			// ### THESE STATEMENTS ARE FOR SPECIAL CASES (like comments, #) ###

			// To check if the first character is a special keyword or symbol such as comment, functions, etc. At later states, firstChar is reset to be true.
			if (firstChar) {
				switch (ch) {
				case '/':
					// comments
					expect = EXPECT::comment;
					continue;
				case 'f':
					// function
					break;
				case '!':
					// const variable
					break;
				case 's':
					// struct
					break;
				case 'e':
					// enum
					break;
				}
				// We have a global declaration
				//else if (ch == '#') {
				//	// TODO: implement global logic
				//}
				firstChar = false;
			}

			// ### THESE STATEMENTS ARE FOR AFTER WE HAVE ALREADY FIGURED OUT WHAT WE ARE PARSING (like variables, log, methods, etc) ###

			// Check for new lines, should be at the top of if statements!
			if (ch == '\n') {
				if (expect == EXPECT::logInputOrReturnString) {
					if (methodname == "log") {
						//print("{}", concatenation::handleStrConcatenation(currWord, getVariable));
					}
					else if (methodname == "logn") {
						//println("{}", concatenation::handleStrConcatenation(currWord, getVariable));
					}
				} else if (expect == EXPECT::logInputOrReturnInt) {
					if (methodname == "log") {
						print("{}", concatenation::handleExpression(currWord));
					}
					else if (methodname == "logn") {
						print("{}\n", concatenation::handleExpression(currWord));
					}
				}
				// This must be at the end because otherwise it outputs true always
				else if (expect == EXPECT::variable) {
					// If we had a variable declaration, we can finish up here
					cout << "vartype: " << vartype << '\n';
					//if (varexpect == VAREXPECT::varvalue) {
					//	if (vartype == "str") {
					//		stringMem[varname] = concatenation::handleStrConcatenation(currWord, stringMem);
					//	}
					//	else if (vartype == "i32") {
					//		i32Mem[varname] = concatenation::handleExpression(currWord);
					//	}
					//	else if (vartype == "i64") {
					//		cout << "found\n";
					//		i64Mem[varname] = concatenation::handleExpression(currWord);
					//	}
					//	else if (vartype == "float") {
					//		//variables[varname] = Variable(stof(currWord));
					//	}
					//}
				}
				expect = EXPECT::none;
				currWord = "";
				// Reset first char since there is a \n
				firstChar = true;
				continue;
			}


			// ## THESE STATEMENTS ARE FOR LO
			// Check if declaration already known
			if (expect != EXPECT::none) {
				if (expect == EXPECT::classmethodOrVariableOrBuiltinfunction) {
					// It's definitely a log, input, return, for, while or class, because these are "special" keywords that have a space like log "test" or for (...) {}
					if (ch == ' ') {
						// TODO: Replace this code to assume it's a built in special method, since for loops, while class, need parentheses around them
						//if (builtInSpecialMethods.find(currWord) != builtInSpecialMethods.end()) {
						//	expect = EXPECT::logInputOrReturnString;
						//	methodname = currWord;
						//	currWord = "";
						//}
						//else {
						//	// check if it's a for loop, while, class, etc
						//}
					}
					// It's definitely a variable declaration, for example in -> mystr: str = "test" -> we are ignoring mystr and just "waiting" until we find the colon signifying it must be a variable
					else if (ch == ':') {
						cout << "varname: " << currWord << '\n';
						varname = currWord;
						//println("varname: {}", varname);
						expect = EXPECT::variablename;
						// We need to find out the type of the variable and the value
						//varexpect = VAREXPECT::vartype;
						currWord = "";
					}
					// it's calling a class like server -> start("8080"), probably a bad idea to support chaining like server.response().tostring()
					else if (ch == '-') {

					}
					// It's a method call, for example, myfunction()
					else if (ch == '(') {
						// check if it's a built in method call
						//if (builtInMethods.find(currWord) != builtInMethods.end()) {
						methodname = currWord;
						currWord = "";
						expect = EXPECT::method;
						//}
						// TODO: or check if it's a user created method
					}
				}
				else if (expect == EXPECT::variablename) {
					if (ch == ' ') {
						vartype = currWord;
					}
					currWord = "";
				}
				else if (expect == EXPECT::waitForEndOfLine) {
					continue;
				}
				else if (expect == EXPECT::logInputOrReturnString) {
					currWord += ch;
					// We automatically assume the log is printing a string/variable unless we see parentheses, which means integer evaluation
					if (ch == '(') {
						expect = EXPECT::logInputOrReturnInt;
					}
				}
				else if (expect == EXPECT::logInputOrReturnInt) {
					currWord += ch;
				}
				else if (expect == EXPECT::waitTillNoSpace) {
					//if (ch == ' ') {
					// Reset currword
					currWord = "";
					expect = EXPECT::variable;
					//}
				}
				else if (expect == EXPECT::variable) {
					//checkForVariable(ch);
				}
				else if (expect == EXPECT::method) {
					// ## WHERE METHODS ARE RUN ##
					if (methodname == "log") {
					    if (ch == ')') {
					        expect = EXPECT::waitForEndOfLine;
					    }
					    else {
					        currWord += ch;
					    }
					}
				}
				continue;
			}
			else if (expect == EXPECT::comment) {
				continue;
			}

			// ### ALL STATEMENTS BELOW ARE BEFORE WE HAVE FIGURED OUT WHAT WE ARE PARSING ####

			// If the first character didn't find a "pattern" this is the last character that signifies any pattern
			if (ch == ' ') {
				cout << currWord << '\n';
				expect = EXPECT::classmethodOrVariableOrBuiltinfunction;
			}
			// Otherwise just add the characters to currWord 
			else {
				currWord += ch;
			}
		}
	}
	catch (const exception& e) {
		println(clog, "Error: {}", e.what());
	}
	auto end = chrono::steady_clock::now();
	auto ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
	println("Program took {}ms", ms);
	return 0;
}