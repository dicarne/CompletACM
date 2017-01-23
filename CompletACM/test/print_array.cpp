#include <stdio.h>
#include <vector>
#include <string>
using namespace std;
void print_array_in_one_line_int(vector<int>list)
{
    int n = list.size();
    for ( int i = 0; i < n - 1; i++ )
    {
        printf("%d ", list[i]);
    }
    printf("%d\n", list[n - 1]);
}

void print_array_in_one_line_string(vector<string>list)
{
    int n = list.size();
    for ( int i = 0; i < n - 1; i++ )
    {
        printf("%s ", list[i].c_str());
    }
    printf("%d\n", list[n - 1].c_str());
}