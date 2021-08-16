#include <stdio.h>

int main()
{
    //creating the buffer
    char overflowme[20];
    printf("now overflow me!!\n");
    //overflowing using the unsafety of gets
    gets(overflowme);
    return 0;
}
