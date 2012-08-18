/********************************************************** DRYTEST for VECTOR
 *	Copyright (C) 2012- project talos (http://talos-kernel.sf.net/)
 *	check LICENSE.txt. If you don't have the file, mail us.
 */

#include <stdio.h>
#include "../include/vector.h"

static class VECTORTEST{
public:
	VECTORTEST(){
		printf("VECTOR...");
		printf("OK.\n");
	};
private:
	class C{};
	VECTOR<C> vector;
}test;

extern "C"{
	int main(void){
		return 0;
	}
}
