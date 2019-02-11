/* 	Membuat Calculator dengan mengimplementasikan Context-Free-Language/PushDown-Automata
	13517077 Dandi Agus Maulana
	13517080 Mgs. Muhammad Riandi Ramadhan
	13517104 Muhammad Fikri Hizbullah
*/
/*
	ALGORITMA
	1. Menerima input berupa string dari pengguna
	Revisi : menggunakan algoritma Recursive-Descent Parsing untuk menghitung dan menemukan error
	2. Mengecek apakah input dari pengguna sesuai dengan simbol terminal dan rules yang ada
		- bila tidak bisa tampilkan "SYNTAX ERROR"
		- kalau bisa lanjut ke langkah selanjutnya
	3. Mengecek apakah ekspresi bisa dioperasikan
		- bila tidak bisa tampilkan "MATH ERROR"
		- kalau bisa lanjut ke langkah selanjutnya
	4. Kalau keduanya valid, Lakukan operasi dan tampilkan hasil di layar
*/
/* Rules yang digunakan
S -> T | S+T | S-T --penjumlahan dan pengurangan
T -> F | T*F | T/F --perkalian dan pembagian
F -> G | G^F -- perpangkatan
G -> (S) | -A | A | A.A | -A.A --minus dan desimal
A -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | AA --angka-angka
*/

#include <stdio.h>
#include <string.h>
#include "mesinkar.h"
#include "boolean.h"
#include "math.h"
#include "stackt.h"

/**** KAMUS GLOBAL ****/
char terminal[18] = {'0','1','2','3','4','5','6','7','8','9','+','-','/','*','^','.','(',')'}; //simbol terminal
char ekspresi[50]; //Asumsi maksimum input user berupa 50 simbol terminal
boolean SyntaxValid = true; //Mengecek kevalidan syntax
boolean MathValid = true; //Mengecek kevalidan ekspresi

boolean IsTerminal(char C);
//Mengecek apakah symbol yang dibaca sebuah terminal atau bukan

boolean IsNumber(char C);
//Mengecek apakah symbol merupakan angka 1 digit

boolean IsOperator(char C);
//Mengecek apakah symbol merupakan sebuah operator

double parseAngka();
//Memparsing angka

double parseMinusDesimal();
//Memparsing angka minus dan/atau desimal

double parsePangkat();
//Memparsing sub-ekspresi pangkat

double parseKaliBagi();
//Memparsing sub-ekspresi kali dan/atau bagi

double parseEkspresi();
//Memparsing ekspresi keseluruhan

void main() {
	/**** ALGORITMA ****/
	/* Menerima input user */
	scanf("%s", ekspresi);
    START(ekspresi);
	while (strcmp(ekspresi,"exit")!=0) {
		/* Memvalidasi input */
		double result = parseEkspresi();

		/*Pengecekan syntax */
		if (!SyntaxValid) {
			printf("SYNTAX ERROR\n");
		}
		/*Pengecekan ekspresi */
		if (!MathValid) {
			printf("MATH ERROR\n");
		}
		/*Jika valid keduanya, hasil ekspresi akan ditampilkan*/
		if (SyntaxValid && MathValid) {
			printf("%lf\n", result);
		}

		SyntaxValid = true; MathValid = true;
		scanf("%s", ekspresi);
		START(ekspresi);
	}
}

boolean IsTerminal(char C) {
	/* KAMUS lOKAL */
	int i = 0;
	boolean exist = false;

	/* ALGORITMA */
	while ((i <= 17) && (!exist)) {
		if (C == terminal[i]) {
			exist = true;
		}
		else {
			i++;
		}
	}
	return exist;
}

boolean IsNumber(char C) {
	return (C == '0') || (C == '1') || (C == '2') || (C == '3') || (C == '4') || (C == '5') || (C == '6') || (C == '7') || (C == '8') || (C == '9');
}

boolean IsOperator(char C) {
	return (C == '*') || (C == '/') || (C == '-') || (C == '+') || (C == '^');
}

double parseAngka(){
	double factor;
	char CBefore;

	if (IsNumber(CC)) {
		factor = CC - '0';
		ADV(ekspresi);
		while (IsNumber(CC)) {
			factor = factor*10 + CC - '0';
			ADV(ekspresi);
		}
		return factor;
	}
	else {
		SyntaxValid = false;
	}
}

double parseMinusDesimal(){
	char CBefore;
	double factor;

	if (CC == '(') {
		ADV(ekspresi);
		factor = parseEkspresi();
		while (CC == '(') {
			ADV(ekspresi);
			factor *= parseKaliBagi();
		}
		if (CC == ')') {
			ADV(ekspresi);
		}
	}
	else if (CC == '-') {
		ADV(ekspresi);
		factor = -1*parseAngka();
		if (CC == '.') {
			ADV(ekspresi);
			double desimal = parseAngka();
			while (!(desimal > -1 && desimal < 1)) {
				desimal /= 10;
			}
			factor -= desimal;
			ADV(ekspresi);
		}
	}
	else {
		factor = parseAngka();
		if (CC == '.') {
			ADV(ekspresi);
			double desimal = (double) parseAngka();
			while (!(desimal > -1 && desimal < 1)) {
				desimal /= 10;
			}
			factor += desimal;
			ADV(ekspresi);
		}
	}
	return factor;
}

double parsePangkat() {
	Stack S;
	char CBefore;
	double factor1,factor2, result;
	CreateEmpty(&S);

	factor1 = parseMinusDesimal();
	Push(&S, factor1);
	while (CC == '^') {
		ADV(ekspresi);
		factor1 = parseMinusDesimal();
		Push(&S, factor1);
	}
	Push(&S, 1); //Pembatas aja jikalau yang dimasukkan tidak ada ^

	while (!IsEmpty(S) && MathValid) {
		Pop(&S, &factor2);
		Pop(&S, &factor1);
		if (factor1 >= 0) {
			result = pow(factor1, factor2);
			if (!IsEmpty(S)) {
				Push(&S,result);
			}
		}
		else if (factor1 < 0 && factor2 >= 1) {
			result = pow(factor1, factor2);
			if (!IsEmpty(S)) {
				Push(&S,result);
			}
		}
		else if (factor1 < 0 && factor2 > 0 && factor2 < 1) {
			MathValid = false;
		}
	}
	if (MathValid) {
		return result;
	}
}

double parseKaliBagi() {
	double factor1, factor2;

	factor1 = parsePangkat();
	while (CC == '*' || CC == '/') {
		if (CC == '*') {
			ADV(ekspresi);
			factor2 = parsePangkat();
			factor1 *= factor2;
		}
		else if (CC == '/') {
			ADV(ekspresi);
			factor2 = parsePangkat();
			if (factor2 != 0) {
				factor1 /= factor2;
			}
			else {
				MathValid = false;
			}
		}
	}

	if (MathValid) {
		return factor1;
	}
}

double parseEkspresi() {
	double factor1, factor2;

	factor1 = parseKaliBagi();
	while (CC == '+' || CC == '-') {
		if (CC == '+') {
			ADV(ekspresi);
			factor2 = parseKaliBagi();
			factor1 += factor2;
		}
		else if (CC == '-') {
			ADV(ekspresi);
			factor2 = parseKaliBagi();
			factor1 -= factor2;
		}
	}

	return factor1;
}



