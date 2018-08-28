 
#include "DeEncode.h"
#include "Vigenere.h"
#include "AES.h"
#include "RC4.h"
#define MAXLEN 1000



int main() {
	char express[MAXLEN] = "scientists try to answer questions about the world around us";
	char key[1000] = "zgtpbamdeg avj ec hrdhsy ufpgamzyg hfzfh alp hcypo lfvyyo iz";


	int a[12] = { 1,3,5,7,9,11,15,17,19,21,23,25 };
	int ia[12] = { 1,9,21,15,3,19,7,23,11,5,17,25 };
	char *s = "fmxvedkaphferbndkrxrsrefmorudsdkdvshvufedkaprkdlyevlrhhrh";
	int n = strlen(s);
	/*for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 26; j++) {
			cout << a[i] << "  " << j << endl;
			for (int k = 0; k < n; k++) {
				int tt = s[k] - 'a';
				char t = (ia[i] * tt + j) % 26 + 'a';
				cout << t;
			}
			cout << endl;
		}
	}*/

	char *str = "algorithmsarequitegeneraldefinitionsofarithmeticprocesses";
	n = strlen(str);
	for (int j = 0; j < 26; j++) {
		for (int i = 0; i < n; i++)
		{
				int tt = str[i] - 'a';
				char t = (3 * tt + 5) % 26 + 'a';
				cout << t;
		}
		cout << endl;
	}

	getchar();
	getchar();


	//Aes *aes = new Aes("fd434","ee43");
	//aes->decrypt("","");
	//cout << "end" << endl;
	
	/*RC4 *rc4 = new RC4("Attackatdawn", "fdf");
	getchar();
	getchar();*/


	while (1) {
	    gets_s(express);
		gets_s(key);
		Vigenere decode;
		//¼ÓÃÜ
		//cout << decode.encrypt(express, key)<< endl;
		////½âÃÜ
		cout << decode.decrypt(express, key);
		getchar();    
	}
	getchar();
	return 0;
}

//attack begins at five
//ci pher

//are you going to scarborough fair
//paul simon

//without no seams nor needlework
//thyme

//scientists try to answer questions about the world around us
//zgtpbamdeg avj ec hrdhsy ufpgamzyg hfzfh alp hcypo lfvyyo iz

//hellU
//NeRRU
//NeRRU
//heRRo
//heRlU
//NellU
//helRU
//NelRU
//hellU
//hellU
//h
//
//Z]VVm
//t]ppm
//t]ppm
//Z]ppS
//Z]pVm
//t]VVm
//Z]Vpm
//t]Vpm
//Z]VVm
//Z]VVm
//Z



//²âÊÔ¶þ
//char exp[2000] = "ALPGLECTALZHLASDHVPPZOTCNXSTKIGDAIPHVJNXCMWGPKSIZASTUATASCZJIIDPAMDUPIOLLGLCUIGTYFPHHXTHMMPSHWWDUKLHALPCLKCDPWEWLZTRAMXDMXSTBRDELEVPIPPWVVCDYWZUWSWXJIMGBXLAPXJLLGLCUIGTYFPHHXTHMMPSHWWDUKLHVYCQVHTTZLPPCCHXALEWLJLIPKFTVJEGHZPAJEYCVXRPPRWDKKTCNMYIOIXDAIWHVJEWLLTVOALNZEYSALPWVXPAZSQIOINXAMPHDINPURZIIIDPAMDUPIOPZPZCNEDPUIRGVMYBPWDXZWTEWMNPURZICSETHROPUIRGVMYCLAJDYOMTSMPKLWSTOEDCVXSXUKQDYASXJLEDCSETUSYDDILGLRZIZEEXZJTTKEYSDIHXSPYDAFPHHXTHMMPSBREXSNFHAMNTYSWAZHZLUPTZLALILVDPUHCXNLETVYDCLWDAPOPPTMRWACDIYILBPEXCVXFCTMYSMYWIOEEHVQPDMCZJOEGTJSXTOICTVYEDMKCTHXEGPEWHHROIYMMJSEEXVRDHVQPDMCZJOEGTJSXTMVPHOJCDTRLGYSHYHMWRLPWHHROHVQPDMCZJOEGTJSXTMVZBHVPPZASTYIJDBVBJLWEFBIDIMSCUYIPSVQWTMXJDBFLIAICTKFJIOIDIVVXHVJATYWPRBXTDUEYSZXLVNICTKFJIOIHXUHDDMTZAPGPQYYEPSMENFSFWHZPQLIYIOIGTAICPUWZUJVPPAMGTZYQULVTCNGZCAMYJLXZLVVVLPXSIOIQPPXSIOEEJUILGUIOHBJQTYMYVPWCTKIXEAMGTNSMPJOEDTMDHPWDXWTTVVFLRRXZPSEMPTERDIENZASDDBXSRHVZAPRLVVFLRRXZVLSCVPERDIENZASWDBMDXHRLVVFLRRXZIOIDABQDPUHRWLXEDZSQDBVYDYXSTYRNXAMPHRRZLPRRIOEEHVQPWVAEWPWDXAYLIPSYRHRLCKATASFPROEYVLH";
//int ap[26] = { 0 };
//double percent[26] = { 0.0 };
//int len = strlen(exp);
//for (int i = 0; i < len; i++) {
//	ap[exp[i] - 'A']++;
//}
//
//for (int i = 0; i < 26; i++) {
//	percent[i] = (double)ap[i] / len;
//	cout << (char)(i + 'A') << " " << percent[i] << endl;
//}
///*sort(percent,percent+26);
//for (int i = 0; i < 26; i++) {
//cout << percent[i] << endl;
//}*/