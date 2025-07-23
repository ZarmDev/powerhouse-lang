#ifndef CONCATENATION_H
#define CONCATENATION_H

#include <string>
using namespace std;

string handleStrConcatenation(const string& str, string(*getVariable)(const string& variableName));
float handleExpression(const string& str);
string handleConcatenationWithDifferentTypes(const string& str, string(*getVariable)(const string& variableName));

#endif