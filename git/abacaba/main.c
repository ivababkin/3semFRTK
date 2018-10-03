#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void generateString(int n, char* mass)
{
    mass[0] = 'a';
    char * temp = (char*)malloc(((1 << n) - 1) * sizeof(char));
    char * temp2[2];
    
    for (int i = 0; i < n; i++)
    {
        temp2[0] = 'a' + i + 1;
        temp2[1] = '\0';
        strcpy(temp, mass);
        strcat(mass, temp2);
        strcat(mass, temp);
    }
}

int main()
{
    int n = 0;
    scanf("%d", &n);
    char * mass = (char*)malloc(((1 << n) + 1) * sizeof(char));
    generateString(n, mass);
    printf("%s\n", mass);
    return 0;
}
