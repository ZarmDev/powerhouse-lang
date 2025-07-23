import utilities;
import <iostream>;
import <fstream>;
import <string>;
import <vector>;
import <unordered_map>;
import <stack>;
import <unordered_set>;
import <sstream>;
import <queue>;

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

// AI usage
class Variable {
public:
	enum Type { INT, FLOAT, STRING, UNDEFINED } type;
	int intValue;
	float floatValue;
	string stringValue;

	Variable() : type(UNDEFINED) {}
	Variable(int value) : type(INT), intValue(value) {}
	Variable(float value) : type(FLOAT), floatValue(value) {}
	Variable(string value) : type(STRING), stringValue(value) {}

	string toString() const {
		switch (type) {
		case INT: return to_string(intValue);
		case FLOAT: return to_string(floatValue);
		case STRING: return stringValue;
		default: return "undefined";
		}
	}
};

class Method {
	// Method implementation
};

class ClassInfo {
public:
	std::unordered_map<std::string, Variable> privateMembers;
	std::unordered_map<std::string, Variable> publicMembers;
	std::unordered_map<std::string, Method> privateMethods;
	std::unordered_map<std::string, Method> publicMethods;
};

// Storage
std::unordered_map<std::string, ClassInfo> classDefinitions;
std::unordered_map<std::string, Variable> variables;
// It increases each time you go deeper into a method
int currentScopeDepth = 0;

// State enums
// Generally used in reading lines
enum class EXPECT { none, variable, method, waitForEndOfLine, comment, logInputOrReturnString, logInputOrReturnInt, waitTillNoSpace };
EXPECT expect = EXPECT::none;
// Used to process variables
enum class VAREXPECT { vartype, varassignment, varvalue };
VAREXPECT varexpect = VAREXPECT::vartype;
bool isInClass = false;
// the class we are in currently
string currentClass = "";

// Variables
string currWord = "";
string varname = "";
string vartype = "";
string methodname = "";
bool firstChar = true;
const unordered_set<string> builtInMethods = { "" };
const unordered_set<string> builtInSpecialMethods = { "log", "logn", "return", "input" };


// vartype -> varassignment -> varvalue
// x: int = 5 -> go past the ":", get the type and then go past the assignment and get the value
static void checkForVariable(const char& ch) {
	//cout << ch << '\n';
	if (varexpect == VAREXPECT::vartype) {
		// If we expect assignment, go past the = sign and find the expected value
		if (ch != ' ') {
			currWord += ch;
		}
		else {
			vartype = currWord;
			currWord = "";
			varexpect = VAREXPECT::varassignment;
		}
	}
	// This is to skip the space and equal sign to get to the value
	else if (varexpect == VAREXPECT::varassignment) {
		if (ch == ' ' || ch == '=') {
			return;
		}
		else {
			currWord += ch;
			varexpect = VAREXPECT::varvalue;
		}
	}
	// Add the characters to currWord. In the '\n' check in int main(), it will actually assign the variable in memory
	else if (varexpect == VAREXPECT::varvalue) {
		currWord += ch;
	}
}

static void parseClass(const std::string& className, const std::string& code) {
	bool inStaticMethod = false;
}

// AI Usage
static string getVariable(const string& variableName) {
	if (currentScopeDepth == 0) {
		if (variables.find(variableName) != variables.end()) {
			return variables[variableName].toString();
		}
	}
	else if (currentScopeDepth == 1) {
		// TODO
	}
	else if (isInClass) {
		// TODO
	}
	return "undefined";
}

// (AI) Function to ensure the vector is resized properly before accessing an element 
template <typename T> void ensureCapacity(std::vector<T>& vec, size_t index)
{
	if (index >= vec.size())
	{
		vec.resize(index + 1);
	}
}

//static void print(string str) {
//	cout << str;
//}
//
//static void println(string str) {
//	cout << str << '\n';
//}

int main()
{

	try {
		ifstream file("test.txt");
		if (!file.is_open()) {
			cerr << "Failed to open file." << endl;
			cout << "Waiting on exit...";
			int t; cin >> t;
			return 1;
		}
		string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
		// artifically add \n to ensure that the last line will be executed
		code += "\n";

		file.close();

		for (char ch : code) {
			//cout << ch << '\n';
			// To check if the first character is a special symbol such as comment
			if (firstChar) {
				if (ch == '/') {
					expect = EXPECT::comment;
					continue;
				}
				// We have a global declaration
				else if (ch == '#') {
					// TODO: implement global logic
				}
				firstChar = false;
			}
			// ## CHECK FOR NEW LINES. THIS SHOULD BE AT THE TOP. ##
			if (ch == '\n') {
				if (expect == EXPECT::logInputOrReturnString) {
					// TODO: Just testing...
					cout << "TESTING\n";
					// This must be at the end because otherwise it outputs true always
					handleConcatenationWithDifferentTypes(currWord, getVariable);
					continue;
					if (methodname == "log") {
						cout << handleStrConcatenation(currWord, getVariable);
					}
					else if (methodname == "logn") {
						cout << handleStrConcatenation(currWord, getVariable) << '\n';
					}
				} else if (expect == EXPECT::logInputOrReturnInt) {
					// TODO: Just testing...
					cout << "TESTING\n";
					// This must be at the end because otherwise it outputs true always
					handleConcatenationWithDifferentTypes(currWord, getVariable);
					continue;
					//cout << currWord;
					if (methodname == "log") {
						cout << handleExpression(currWord);
					}
					else if (methodname == "logn") {
						cout << handleExpression(currWord) << '\n';
					}
				}
				// This must be at the end because otherwise it outputs true always
				else if (expect == EXPECT::variable) {
					// If we had a variable declaration, we can finish up here
					if (varexpect == VAREXPECT::varvalue) {
						//cout << varname << ": " << vartype << " = " << currWord << '\n';
						// AI Usage
						if (vartype == "str") {
							//variables[varname] = Variable(handleStrConcatenation(currWord));
							variables[varname] = Variable(currWord);
						}
						else if (vartype == "int") {
							variables[varname] = Variable(handleExpression(currWord));
						}
						else if (vartype == "float") {
							variables[varname] = Variable(stof(currWord));
						}
					}
				}
				expect = EXPECT::none;
				currWord = "";
				// Reset first char since there is a \n
				firstChar = true;
				continue;
			}
			// ## CHECK IF DECLARATION ALREADY KNOWN ##
			if (expect != EXPECT::none) {
				if (expect == EXPECT::waitForEndOfLine) {
					continue;
				}
				else if (expect == EXPECT::logInputOrReturnString) {
					currWord += ch;
					// We automatically assume the log is printing a string/variable unless we see parentheses, which means integer evaluation
					if (ch == '(') {
						expect = EXPECT::logInputOrReturnInt;
					}
					continue;
				}
				else if (expect == EXPECT::logInputOrReturnInt) {
					currWord += ch;
				}
				else if (expect == EXPECT::waitTillNoSpace) {
					if (ch == ' ') {
						// Reset currword
						currWord = "";
						expect = EXPECT::variable;
					}
					continue;
				}
				else if (expect == EXPECT::variable) {
					checkForVariable(ch);
				}
				else if (expect == EXPECT::method) {
					// ## WHERE METHODS ARE RUN ##
					if (methodname == "log") {
					    if (ch == ')') {
					        expect = EXPECT::waitForEndOfLine;
					        // Run the method
					        //print(currWord);
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
			// It's definitely a variable declaration
			if (ch == ':') {
				// bad?
				//currWord = currWord.substr(0, currWord.length() - 1);
				varname = currWord;
				expect = EXPECT::waitTillNoSpace;
				// We need to find out the type of the variable and the value
				varexpect = VAREXPECT::vartype;
				currWord = "";
			}
			// It's definitely a log, input, return, for, while or class
			if (ch == ' ') {
				if (builtInSpecialMethods.find(currWord) != builtInSpecialMethods.end()) {
					expect = EXPECT::logInputOrReturnString;
					methodname = currWord;
					currWord = "";
				}
				else {
					// check if it's a for loop, while, class, etc
				}
			}
			// It's a method call
			else if (ch == '(') {
				// check if it's a built in method call
				if (builtInMethods.find(currWord) != builtInMethods.end()) {
					methodname = currWord;
					currWord = "";
					expect = EXPECT::method;
				}
				// TODO: or check if it's a user created method
			}
			// Otherwise just add the characters to currWord
			else {
				currWord += ch;
			}
		}
	}
	catch (const std::exception& e) {
		cout << e.what();
	}
	cout << "Waiting on exit...";
	int t; cin >> t;
	return 0;
}