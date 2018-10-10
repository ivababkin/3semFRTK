#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// FIXIT: из названия констант должно быть ясно её предназначение, хотя бы примерно
// MAX_STRING_SIZE лучше
#define MAX_STRING_SIZE 100

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

void Split2 (char* string, char* delimeters, char*** tokens, int* tokensCount)
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
    char** tokens = (char**)malloc(MAX_STRING_SIZE * sizeof(char));

    // Считывать с экрана строку можно с помощью fgets
    char string[] = {"da_i_chetvertiy-esly-chestno yz smotret by ne stal"};

    char delimeters[] = {" _-"};
    int tokensCount = 0;
    Split2 (string, delimeters, &tokens, &tokensCount);

    for(int k = 0; k < tokensCount; k++)
    {
        //printf ("%d\n", tokensCount);
        printf("%s\n", tokens[k]);
       }

    free(tokens);

    return 0;
}
