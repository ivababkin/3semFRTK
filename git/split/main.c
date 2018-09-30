#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void split(char * str, char ** resptr)
{
    char * ptr = str;
    int len = (strlen(str));
    int k = 0;
    for (int i = 0; i <= len; i++)
    {
        if ((*ptr == ' ') || (*ptr == '\0'))
        {
            *resptr = (char *)malloc(k * sizeof(char));
            strncpy((*resptr), str, k);
            char nullstr[] = {'\0'};
            strcat(*resptr, nullstr);
            resptr++;
            str = ptr + 1;
            k = 0;
        }
        ptr++;
        k++;
    }
}

// Если вы не хотите, чтобы исходная строка string изменилась, то можно сделать копию от неё.
void Split2 (char* string, char* delimeters, char*** tokens, int* tokensCount)//оно портит исходную строку, но так, вроде, можно
{
    int count = 0;
    char * ptrstr;
    char ** doubleptr = *tokens;
    ptrstr = strtok (string, delimeters);
    *doubleptr = ptrstr;

    while (ptrstr != NULL)
    {
       doubleptr++;
       ptrstr = strtok(NULL, delimeters);
       *doubleptr = ptrstr;
       count++;
    }
    *tokensCount = count;

}

int main()
{
    // FIXIT: 1) нужно вынести 100 в именованную константу 2) вывести результат с помощью цикла, а не копированием одной и той же строчки
    char** tokens = (char**)malloc(100 * sizeof(char));
    
    // Считывать с экрана строку можно с помощью fgets
    char string[] = {"da_i_chetvertiy-esly-chestno yz smotret by ne stal"};
    
    char delimeters[] = {" _-"};
    int tokensCount = 0;
    Split2 (string, delimeters, &tokens, &tokensCount);
    printf ("%d\n", tokensCount);
    printf("%s\n", (tokens)[0]);
    printf("%s\n", (tokens)[1]);
    printf("%s\n", (tokens)[2]);
    printf("%s\n", (tokens)[3]);
    printf("%s\n", (tokens)[4]);
    printf("%s\n", (tokens)[5]);
    printf("%s\n", (tokens)[6]);
    printf("%s\n", (tokens)[7]);
    printf("%s\n", (tokens)[8]);
    printf("%s\n", (tokens)[9]);
    printf("%s\n", (tokens)[10]);


    free((tokens));

    return 0;
}
