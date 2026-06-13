import utilities;
import std;
import concatenation;

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

int main()
{

	try {
		ifstream file("code.txt");
		if (!file.is_open()) {
			println(clog, "Failed to open file.");
			println("Waiting on exit...");
			int t; cin >> t;
			return 1;
		}
		string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
		// artifically add \n to ensure that the last line will be executed
		code += "\n";

		file.close();

		for (char ch : code) {
			// ### THESE STATEMENTS ARE FOR SPECIAL CASES (like comments, #) ###

			// To check if the first character is a special symbol such as comment. At later states, firstChar is reset to be true.
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

			// ### THESE STATEMENTS ARE FOR AFTER WE HAVE ALREADY FIGURED OUT WHAT WE ARE PARSING (like variables, log, methods, etc) ###

			// Check for new lines, should be at the top of if statements!
			if (ch == '\n') {
				if (expect == EXPECT::logInputOrReturnString) {
					if (methodname == "log") {
						print("{}", concatenation::handleStrConcatenation(currWord, getVariable));
					}
					else if (methodname == "logn") {
						println("{}", concatenation::handleStrConcatenation(currWord, getVariable));
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
					if (varexpect == VAREXPECT::varvalue) {
						if (vartype == "str") {
							variables[varname] = Variable(concatenation::handleStrConcatenation(currWord, getVariable));
						}
						else if (vartype == "int") {
							variables[varname] = Variable(concatenation::handleExpression(currWord));
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
			// Check if declaration already known
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

			// It's definitely a variable declaration, for example in -> mystr: str = "test" -> we are ignoring mystr and just "waiting" until we find the colon signifying it must be a variable
			if (ch == ':') {
				varname = currWord;
				expect = EXPECT::waitTillNoSpace;
				// We need to find out the type of the variable and the value
				varexpect = VAREXPECT::vartype;
				currWord = "";
			}
			// It's definitely a log, input, return, for, while or class, because these are "special" keywords that have a space like log "test" or for (...) {}
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
			// It's a method call, for example, myfunction()
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
	catch (const exception& e) {
		println(std::clog, "Error: {}", e.what());
	}
	return 0;
}