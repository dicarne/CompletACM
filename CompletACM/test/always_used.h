#include <vector>

using namespace std;


#pragma region print_array
void print_array_in_one_line_int(vector<int>list)
{
    int n = list.size();
    for ( int i = 0; i < n - 1; i++ )
    {
        printf("%d ", list[i]);
    }
    printf("%d\n", list[n - 1]);
}
void print_array_in_one_line_string(vector<string>list);
#pragma endregion

#pragma region print
void test();
#pragma endregion

