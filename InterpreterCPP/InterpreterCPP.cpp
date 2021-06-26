// InterpreterCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "testCode.h"
#include "OpObj.h"
#include "ExternalDef.h"
#include <iostream>
#include <ctime>


//Define our functions we are going to import into the interpreter
OpObj* printFn( OpObj*(*popFn) () ){
	StringObj* str = static_cast<StringObj*>(popFn());
	cout << "printFn: " << *str->value << "\n";
	delete str;
	return new BoolObj(false, false);
}
OpObj* timeFn(OpObj*(*popFn) ()){
	time_t now = time(nullptr);
	return new NumberObj(now, false);
}
OpObj* notFn(OpObj*(*popFn) ()) {
	BoolObj* obj = static_cast<BoolObj*>(popFn());
	optional<bool> value = false;
	if (obj->value == nullopt) {
		value = true;
	} else {
		value = !*obj->value;
	}
	delete obj;
	return new BoolObj(value, false);
}
OpObj* negFn(OpObj*(*popFn) ()){
	NumberObj* obj = static_cast<NumberObj*>(popFn());
	optional<double> value = obj->value;
	delete obj;
	return new NumberObj(0 - *value, false);
}

optional<string> checkCompile(vector<ExternalDef>externs, const char* code) {
	try {
		Tokenizer tokenizer;
		try {
			tokenizer.tokenize(code);
			Parser parser(tokenizer.tokens);
			try {
				parser.parse(externs, IdentityType::String);
			} catch (char e) {
				return "Parser error: " + parser.errorMsg;
			}
		} catch (char e) {
			return "Tokenizer error: " + tokenizer.errorMsg;
		}
	} catch (...) {
		return "Unknown error";
	}
	return nullopt;
}

optional<string> runCode(vector<ExternalDef>externs, const char* code, bool* errorOccured, string* errorOut) {
	try {
		Tokenizer tokenizer;
		optional<string> returnValue = nullopt;

		if (errorOccured) *errorOccured = false;

		try {
			tokenizer.tokenize(code);
			Parser parser(tokenizer.tokens);
			try {
				parser.parse(externs, IdentityType::String);

				OpObj* retObj = parser.program.execute(externs);
				if (retObj) {
					if (retObj->objType == OpObjType::String) {
						returnValue = static_cast<StringObj*>(retObj)->value;
					}
					delete retObj;
				}

			} catch (char e) {
				if (e == 'P') {
					if (errorOccured) *errorOccured = true;
					if (errorOut) *errorOut = "Parser error: " + parser.errorMsg;
				} else if (e == 'E') {
					if (errorOccured) *errorOccured = true;
					if (errorOut) *errorOut = "Execution error: " + parser.program.errorMsg;
				} else {
					if (errorOccured) *errorOccured = true;
					if (errorOut) *errorOut = "Unknown error";
				}
			}
		} catch (char e) {
			if (errorOccured) *errorOccured = true;
			if (errorOut) "Tokenizer error: " + tokenizer.errorMsg;
		}
		return returnValue;
	} catch (...) {
		if (errorOccured) *errorOccured = true;
		if (errorOut) *errorOut = "Unknown catch all error";
	}
	return nullopt;
}

int main(){
	auto authorName = StringObj("Dan Teel", true);
	auto publicationYear = NumberObj(2020, true);
	auto isInterpreted = BoolObj(true, true);
	auto numberOfTestsPassed = NumberObj(0, false);

	auto externList = {
		ExternalDef("print", IdentityType::Bool, { IdentityType::String }, &printFn),
		ExternalDef("time", IdentityType::Double, {}, &timeFn),
		ExternalDef("not", IdentityType::Bool, { IdentityType::Bool }, &notFn),
		ExternalDef("neg", IdentityType::Double, { IdentityType::Double }, &negFn),

		ExternalDef("authorName", IdentityType::String, &authorName),
		ExternalDef("publicationYear", IdentityType::Double, &publicationYear),
		ExternalDef("isInterpreted", IdentityType::Bool, &isInterpreted),
		ExternalDef("numberOfTestsPassed", IdentityType::Double, &numberOfTestsPassed),
	};

	bool errorOccured;
	string errorMsg;
		
	optional<string> value = runCode(externList, testCode, &errorOccured, &errorMsg);

	if (value == nullopt) {
		cout << errorMsg << "\n";
	} else {
		cout << *value << "\n";
	}
}
 