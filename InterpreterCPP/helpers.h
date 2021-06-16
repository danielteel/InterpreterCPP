#pragma once
#include <string>
bool isSpace(int chr);
bool isDigit(int chr);
bool isAlpha(int chr);
bool isAlNum(int chr);

bool compare_float(double x, double y, double epsilon = 0.000001f);

std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v");
std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v");
std::string& trim(std::string& s, const char* t = " \t\n\r\f\v");
std::string stringToLowerCopy(std::string str);
void stringToLower(std::string& str);
std::string doubleToString(double value, int decimalPlaces);
std::string trimmedDoubleToString(double value, int mostDecimalPlaces);
std::string trimmedDoubleToString(double value);

std::string encodeString(std::string str);
std::string decodeString(std::string str);

std::string stripTrailingNumber(std::string str);