#pragma once
#include "DeEncode.h"

class Vigenere :public DeEncode {
public:
	Vigenere() {
	}
	Vigenere(string express, string key) :DeEncode(express, key) {
	}
	virtual string encrypt(string express, string key);
	virtual string decrypt(string express, string output);
};