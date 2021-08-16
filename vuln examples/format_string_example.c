#include <stdio.h>

int main()
{
	//declaring a char array
	char mystr[30]; 
	printf("scanning a string: ");
	// getting input from user
	fgets(mystr, sizeof(mystr), stdin); 
	
	//safe use of printf
	printf("this is the string you entered printed safely: ");
	printf("%s\n", mystr); 
	
	// vulnarable use of printf
	printf("and this is the string you entered printed unsafely: ");
	printf(mystr); 
	
	return 0;
}