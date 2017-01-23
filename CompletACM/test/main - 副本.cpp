#include <stdio.h>
#include <vector>
#include <algorithm>
#include "always_used.h"
using namespace std;
bool base_abs(int&lhs, int&rhs)
{
    return abs(lhs) > abs(rhs);
}
int t_main()
{
    int n;
    while ( scanf("%d", &n) != EOF )
    {
        if ( n == 0 ) break;
        vector<int>list;
        int te;
        for ( int i = 0; i < n; i++ )
        {
            scanf("%d", &te);
            list.push_back(te);
        }
        sort(list.begin(), list.end(), base_abs);
        print_array_in_one_line_int(list);
    }
    return 0;
}