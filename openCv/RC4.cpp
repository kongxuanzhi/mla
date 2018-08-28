#include "RC4.h"

	
string RC4::encrypt(string express, string key) {
	string res = "";
	int e_len = express.size();
	int k_len = key.size();
	int i = 0, j = -1;
	while (i < e_len) {
		if (express[i]==' ') {
			i++;
		}
		j = (j + 1) % k_len;
		if (key[j]==' ') {
			j++;
		}
		char c = (express[i++] - 'a' + key[j] - 'a') % 26 + 'a';
		res += c;
	}
	return res;
}
string RC4::decrypt(string express, string output) {
	int len = express.size();
	string key = "";
	for (int i = 0;i < len;i++) {
		if (express[i] == ' ')continue;
		int x = output[i] - express[i];
		x = x < 0 ? x + 26 : x;
	    key += (char)(x+'a');
	}
	return key;
}
