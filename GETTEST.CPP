#include <iomanip.h>
#include <stdio.h>
#include <malloc.h>



void main()
{
	char* dude=(char*)malloc(100);
	cin.getline(dude,100);
	cout << dude;
	cin.getline(dude,100);
	cout << dude;
}
