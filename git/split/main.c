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

int main()
{
    char ** res = (char**)malloc(100 * sizeof(char));
    char mass[] = {"gruppa krovi na rukave moi poradkoviy nomer na rukave"};
    split(mass, res);
    printf("strlen(mass) = %d\n", strlen(mass));
    printf("%s\n", res[0]);
    printf("%s\n", res[1]);
    printf("%s\n", res[2]);
    printf("%s\n", res[3]);
    printf("%s\n", res[4]);
    printf("%s\n", res[5]);
    printf("%s\n", res[6]);
    printf("%s\n", res[7]);
    printf("%s\n", res[8]);
    printf("%s\n", res[9]);
    printf("%s\n", res[10]);


    for (int i = 0; i <= (strlen(res)); i++)
    {
        free(res[i]);
    }
    free(res);

    return 0;
}
