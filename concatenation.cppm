export module concatenation;
import <string>;
import <stack>;
import <vector>;
import <iostream>;
import <array>;
import <queue>;
import <algorithm>;
import <cmath>;
import <iomanip>;
import <unordered_map>;
import utilities;

using namespace std;

enum HANDLER { none, quotes, variableormethod, adding, number };

HANDLER handling = HANDLER::none;

// (Full usage of AI) Function to ensure the vector is resized properly before accessing an element 
template <typename T> void ensureCapacity(std::vector<T>& vec, size_t index)
{
    if (index >= vec.size())
    {
        vec.resize(index + 1);
    }
}

enum CastType { INT, STR, FLOAT, CHAR };

unordered_map<string, CastType> castMap = {
    {"int", INT},
    {"str", STR},
    {"float", FLOAT},
    {"char", CHAR}
};

vector<string> methods = { "int", "str", "float" };

char reserved = 'x';

enum HANDLER2 { none, quotes, adding, number, variablemethodorinteger };
HANDLER2 handling2 = HANDLER2::variablemethodorinteger;

//Full usage of AI
static string replaceWithQueue(const string& str, queue<float>& q) {
    string result;
    for (char ch : str) {
        if (ch == reserved && !q.empty()) {
            string r = to_string(q.front());
            reverse(r.begin(), r.end());
            result += r;
            q.pop();
        }
        else {
            result += ch;
        }
    }
    return result;
}

// Made by me with the help of AI/research
static string replaceReserved(const string given, queue<float>& q) {
    queue<float> copy = q;
    stack<float> s;

    string str(given.rbegin(), given.rend());
    // Process the string and replace the reserved character with stack values
    str = replaceWithQueue(str, q);

    reverse(str.begin(), str.end());

    return str;
}

static bool isInteger(float value) {
    return floor(value) == value;
}

// AI + me
static float parseIntExpressionWithoutVariables(const string& expression) {
    float result = 0.0;
    float currentNumber = 0;
    float decimalMultiplier = 0;
    char lastOperation = '+'; // To keep track of the last operation

    for (char ch : expression) {
        if (isdigit(ch)) {
            if (decimalMultiplier > 0) {
                currentNumber += (ch - '0') * decimalMultiplier;
                decimalMultiplier *= 0.1;
            }
            else {
                currentNumber = currentNumber * 10 + (ch - '0');
            }
        }
        else if (ch == '.') {
            decimalMultiplier = 0.1;
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') {
            switch (lastOperation) {
            case '+': result += currentNumber; break;
            case '-': result -= currentNumber; break;
            case '*': result *= currentNumber; break;
            case '/': result /= currentNumber; break;
            case '%':
                if (isInteger(result) && isInteger(currentNumber)) {
                    result = static_cast<int>(result) % static_cast<int>(currentNumber);
                }
                break;
            }
            currentNumber = 0;
            decimalMultiplier = 0;
            lastOperation = ch;
        }
    }

    // Handle the last number
    switch (lastOperation) {
    case '+':
        result += currentNumber;
        break;
    case '-': result -= currentNumber; break;
    case '*': result *= currentNumber; break;
    case '/': result /= currentNumber; break;
    case '%':
        if (isInteger(result) && isInteger(currentNumber)) {
            result = static_cast<int>(result) % static_cast<int>(currentNumber);
        }
        break;
    }

    return result;
}

// Two ideas:
/*
One: Consider all + with an number as "constants" at depth = 0 (for optimization maybe?)
Two: Have a boolean like inParentheses or something and if depth is ever 0 while that is false (default value),
then that means you can just parse that chunk of the string and then add with other parentheses 
Three: If extra performance is neccessary, it could be better to put the HANDLER into compiler.cpp to work intertwining with EXPECT
*/

//int handleIntConcatenation(const string& str, int (*getVariable)(const string& variableName)) {
//	string currWord = "";
//	int depth = 0;
//	// This is the variable that will have the final value
//	int result = 0;
//	int firstNum = true;
//
//	for (char ch : str) {
//		if (handling == HANDLER::none) {
//			// Increase depth by 1
//			if (ch == '(') {
//				depth++;
//			}
//			// Why is there a space...
//			/*else if (ch == ' ') {
//				
//			}*/
//			// It's a string
//			else if (ch == '"') {
//
//			}
//			// It's a char
//			else if (ch == '\'') {
//
//			}
//			else { // That means we have a variable, method, number or float
//				if (isNumber(ch)) {
//					handling = HANDLER::numberorfloat;
//				}
//				else {
//					handling = HANDLER::variableormethod;
//				}
//			}
//		}
//		else {
//			if (handling == HANDLER::numberorfloat) {
//				// Then, we just wait until we see a space or the line abrubtly ends
//				if (ch == ' ') {
//					if (firstNum) {
//						result = stoi(currWord);
//					}
//					// Since there is a space, we can assume there will be an operation.
//					handling = HANDLER::operation;
//				}
//			}
//			else if (handling == HANDLER::operation) {
//
//			}
//		}
//	}
//	return result;
//}

export namespace concatenation {
    // The second parameter is just saying it expects the pointer to the function getVariable that takes a const string and returns a int
    string handleStrConcatenation(const string& str, string(*getVariable)(const string& variableName)) {
        string fullString = "";
        // This is just the seperate parts of the string str
        string currString = "";
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
            // If it's the first character, and we don't get a sign that there is a string, then it's should be a variable
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
            else if (handling == HANDLER::adding) {
                if (str.at(i) != ' ') {
                    handling = HANDLER::variableormethod;
                }
            }
            // Here, it's not quotes so it may be a method, variable or class
            else if (handling == HANDLER::variableormethod) {
                if (str.at(i) == ' ') {
                    handling = none;
                    // TODO: attempt to also find the method
                    fullString += getVariable(currString);
                    currString = "";
                    continue;
                }
                currString += str.at(i);
            }
            if (str.at(i) == '+') {
                handling = adding;
            }
        }
        // If it the concatenation abrubtly ends (which it will), then add the last currString
        // Or as a neat trick just add a space at the end or something to make the loop continue
        if (currString != "") {
            if (handling == HANDLER::variableormethod) {
                // TODO: attempt to also find the method
                fullString += getVariable(currString);
            }
        }
        handling = HANDLER::none;
        return fullString;
    }

    string handleConcatenationWithDifferentTypes(const string& str, string(*getVariable)(const string& variableName)) {
        string fullString = "";
        // This is just the seperate parts of the string str
        string currString = "";
        for (int i = 0; i < str.length(); i++) {
            if (str.at(i) == '"' || str.at(i) == '\'') {
                // If it's the end of the quotation mark (since handling at the start is NOT quotes, it will not run this if statement)
                if (handling2 == HANDLER2::quotes) {
                    // This means that the quotes ended and we are no longer within a string
                    fullString += currString;
                    //cout << currString << '\n';
                    handling2 = HANDLER2::none;
                    //currString = "";
                    continue;
                }
                // Set handling to quotes to ensure it will know when the quotes end
                handling2 = HANDLER2::quotes;
                continue;
            }
            // If it's the first character (and not a string), then it's probably a variable so print it
            else if (i == 0) {
                handling2 = HANDLER2::variablemethodorinteger;
            }
            if (handling2 == HANDLER2::none) {

            }
            else if (handling2 == HANDLER2::quotes) {
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
            else if (handling2 == HANDLER2::adding) {
                if (str.at(i) != ' ') {
                    if (isNumber(str.at(i))) {
                        handling2 = HANDLER2::number;
                    }
                    else {
                        handling2 = HANDLER2::variablemethodorinteger;
                    }
                }
            }
            // Here, it's not quotes so it may be a method, variable or class
            else if (handling2 == HANDLER2::variablemethodorinteger) {
                if (str.at(i) == ' ') {
                    handling2 = HANDLER2::none;
                    fullString += getVariable(currString);
                    currString = "";
                    continue;
                }
                currString += str.at(i);
            }
            if (str.at(i) == '+') {
                handling2 = HANDLER2::adding;
            }
        }
        // If it the concatenation abrubtly ends (which it will), then add the last currString
        // Or as a neat trick just add a space at the end or something to make the loop continue
        if (currString != "") {
            cout << currString << '\n';
            if (handling2 == HANDLER2::variablemethodorinteger) {
                if (all_of(currString.begin(), currString.end(), ::isdigit)) {
                    fullString += currString;
                }
                else {
                    fullString += getVariable(currString);
                }
            }
        }
        handling2 = HANDLER2::none;
        return fullString;
    }

    // Function created by me + research (see concatenation.svg to undertand this)
    float handleExpression(const string& str) {
        vector<vector<string>> order;
        vector<vector<CastType>> type;
        vector<int> clones;
        string currWord;
        int depth = 0;
        string depthZero = "";
        bool isString = false;
        handling == HANDLER::none;

        for (char ch : str) {
            //cout << ch << " " << depth << '\n';
            if (depth == 0) {
                if (ch == '(') {
                    depthZero += reserved;
                    depth++;
                }
                else if (ch == ')') {
                    depthZero += ch;
                    depth--;
                }
                else if (ch == '\'') {
                    // TODO: Is this really efficient?
                    type[depth][clones[depth]] = CastType::CHAR;
                }
                else if (ch == '"') {
                    type[depth][clones[depth]] = CastType::STR;
                }
                else {
                    depthZero += ch;
                }
                continue;
            }
            else if (ch == '(') {
                // TODO: This skips unneccessary computations but can confuse the compiler as well. See what works.
                // if (currWord == "") {
                //   continue;
                // }
                currWord += reserved;
                ensureCapacity(order, depth);
                ensureCapacity(clones, depth);
                int clone = clones[depth];
                ensureCapacity(order[depth], clone);
                order[depth][clone] += currWord;
                currWord = "";
                depth++;
            }
            else if (ch == ')') {
                ensureCapacity(order, depth);
                ensureCapacity(clones, depth);
                int clone = clones[depth];
                clones[depth] += 1;
                ensureCapacity(order[depth], clone);
                order[depth][clone] += currWord;
                currWord = "";
                depth--;
            }
            else {
                currWord += ch;
            }
        }

        // ## Actually calculating the parenthese values ##
        queue<float> previousValues;
        for (int i = order.size() - 1; i >= 0; i--) {
            for (int z = order[i].size() - 1; z >= 0; z--) {
                string filled = replaceReserved(order[i][z], previousValues);
                float val = parseIntExpressionWithoutVariables(filled);
                previousValues.push(val);
            }
        }

        // ## For the first depth ##
        string filled = replaceReserved(depthZero, previousValues);
        float valAtZero = parseIntExpressionWithoutVariables(filled);
        // Reset handler
        handling = HANDLER::none;
        return valAtZero;
    }
}