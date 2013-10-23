#include <iostream>
#include "srcparse.h"
#include "..\..\..\file\file.h"
using namespace std;
int main(void)
{
	srcparse s;
	file f;
	f.setpath("C:\\test.txt");
	cout << s.parse((char*)f.load()).linecode;
	getchar();
}