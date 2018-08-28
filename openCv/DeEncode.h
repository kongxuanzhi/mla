#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <algorithm>
using namespace std;

class DeEncode {
public:
	DeEncode() {

	}
	DeEncode(string express,string key) {
		this->express = express;
		this->key = key;
	}
public:
	virtual string encrypt(string express, string key) = 0;
	virtual string decrypt(string express, string output) = 0;
protected:
	string express;
	string key;
};