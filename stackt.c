/* 	NAMA/NIM	: Muhammad Fikri Hizbullah/13517104
	TANGGAL		: 13-10-2018
	TOPIK		: ADT STACK
*/

#include "stackt.h"

void CreateEmpty (Stack *S) {
	Top(*S) = Nil;
}

boolean IsEmpty (Stack S) {
	return Top(S) == Nil;
}

boolean IsFull (Stack S) {
	return Top(S) == MaxEl;
}

void Push (Stack * S, infotype X) {
	Top(*S)++;
	InfoTop(*S) = X;
}

void Pop (Stack * S, infotype* X) {
	*X = InfoTop(*S);
	Top(*S)--;
}

