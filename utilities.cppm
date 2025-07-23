export module utilities;
import <string>;
import <algorithm>;

using namespace std;

const char digits[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

export namespace utilities {
    bool isPrimitiveType(const string& word) {
        return (word == "int" || word == "str" || word == "bool" || word == "float");
    }
    bool isNumber(char c) {
        for (int i = 0; i < sizeof(digits) / sizeof(digits[0]); i++) {
            if (c == digits[i]) {
                return true;
            }
        }
        return false;
    }
};



//// bytes in digits / bytes in one element of the array = total num of elements in array (idk why don't ask me)
//// sizeof(digits) / sizeof(digits[0])


//bool isNumber(const string& str) {
//    return all_of(str.begin(), str.end(), ::isdigit);
//}