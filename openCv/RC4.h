#pragma once
#pragma warning(disable:4996)
#include "DeEncode.h"

class RC4 :public DeEncode {
public:
	RC4() {
	}
	RC4(string express, string key) :DeEncode(express, key) {
		char key_str[4] = { 0X0F,0X01,0X0E,0X02 };
		char *exp = "Attackatdawn";
		int k_len = strlen(key_str);
		int e_len = strlen(exp);

		unsigned char *ret_K = (unsigned char*)malloc(sizeof(unsigned char)*e_len);
		for (int i = 0; i < 256; i++) {
			S[i] = i;
			T[i] = key_str[i%k_len];
		}
		int j = 0;
		for (int i = 0; i < 256; i++) {
			j = (j + S[i] + T[i]) % 256;
			unsigned char temp = S[i];
			S[i] = S[j];
			S[j] = temp;
		}

		int i = 0, t = 0;
		j = 0;
		for (int k = 0; k < e_len; k++) {
			i = (i + 1) % 256;
			j = (j + S[i]) % 256;
			unsigned char temp = S[i];
			S[i] = S[j];
			S[j] = temp;
			t = (S[i] + S[j]) % 256;
			ret_K[k] = S[k] ^ express.at(k);
			printf("%X ", ret_K[k]);
		}
		
		/*generkeys(len);*/
	}
	virtual string encrypt(string express, string key);
	virtual string decrypt(string express, string output);
	unsigned char S[256]; 
	unsigned char T[256];
};

