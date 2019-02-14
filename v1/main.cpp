#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <cstring>
#include <map>
#include <vector>
#include <cctype>
#include <algorithm>

using namespace std;

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
            k = -1;
        }
        ptr++;
        k++;
    }
}

string prepare(string str)
{
    int i = 0;
    str.c_str();
    while (str[i])
    {
        str[i] = (char)tolower(str[i]);
        i++;
    }
    return (string)str;
}

string delMarks(string str)
{
    int i, found;
    char marks[] = {".,'\"0123456789-_()!;?"}; //вроде все
    int len = strlen(marks);
    for(i = 0; i < len; i++)
    {
        while ((int)str.find(marks[i]) != -1)
        {
            found = str.find(marks[i]);
            str.erase(str.begin() + found);
        }
    }
    return str;
}

void printmap(map <string,int> myFirstMap)
{
    for (auto it = myFirstMap.begin(); it != myFirstMap.end(); ++it)
    {
        cout << it->first << " : " << it->second << endl;
    }
}

bool comp(pair<string, int> firstOne, pair<string, int> secondOne)
{
    if (firstOne.second > secondOne.second)
        return true;
    else
        return false;
}

void printTopTen(map <string,int> myFirstMap)
{
    vector <pair<string, int>> myVect;
    for (auto it = myFirstMap.begin(); it != myFirstMap.end(); ++it)
    {
        myVect.push_back(*it);
    }
    void qsort ( void * first, size_t number, size_t size, int ( * comparator ) ( const void *, const void * ) );
    sort (myVect.begin(), myVect.end(), comp);

    cout <<"vector:" << endl;

    for (auto it = myVect.begin(); it != myVect.begin() + 10; ++it)
    {
        cout << it->first << " : " << it->second << endl;
    }
}

int main()
{
    //string str = {"t,..,.,.,r!sxcfghburc123fc2u12c3u12.,, d.d ,f.,df"};
    //cout << delMarks(str) << endl;
    map <string,int> myFirstMap;

    int i;
    string line;
    char** tokens = (char**)malloc(100 * sizeof(char*));


    ifstream in("/home/ivan/Рабочий стол/фртк/2 sem/v1/text1"); // окрываем файл для чтения
    if (in.is_open())
    {
        while (getline(in, line))
        {
            line = prepare(line); //сделать строчными

            line = delMarks(line);
            split((char*)line.c_str(), tokens); //распил

            i = 0;
            while (tokens[i] != NULL)
            {
                for (auto it = myFirstMap.begin(); it != myFirstMap.end(); ++it)
                {
                    if (strcmp((it->first).c_str(), tokens[i]) == 0)
                    {
                        it->second = it->second + 1;
                    }
                }
                if(strlen(tokens[i]) != 0)
                {
                    string strtemp = string(tokens[i]);
                    myFirstMap.insert(pair<string, int>(strtemp, 1));//добавляем
                }
                i++;
            }
        }
    }


    printTopTen(myFirstMap);
    cout << "-----------" << endl;
    printmap(myFirstMap);
    in.close();
    cout << "End of program" << endl;
    return 0;

}
