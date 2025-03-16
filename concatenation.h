#ifndef CONCATENATION_H
#define CONCATENATION_H

#include <string>
using namespace std;

string handleStrConcatenation(const string& str, int (*getVariable)(int, int), string& currString);

#endif