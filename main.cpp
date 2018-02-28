#include<iostream>
#include <stdlib.h>
#include <stdio.h>

#include "armsim.cpp"

using namespace std;

int main()
{
	readMemFile("test.mem");
	run();
	return 0;
}
