#include <stack>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#define DEBUG
using namespace std;

vector<string> get_all_name(fstream&fs);
void get_method_name_from_string(string te, vector<string>&method);

int main()
{
    string title = "";
#ifdef DEBUG
    title = "test//";
#endif // DEBUG


    //读取文件
    fstream fs( title + "main.cpp");
    //最终要加入源文件的函数体字符串数组
    vector<string>add;
    char head[1000];
    fs >> head;
    //获取include
    string include(head);

    string include_end = "std;";
    //获取include列表
    vector<string>include_list;
    while ( include != include_end )
    {
        //找到"包裹的文件名
        if ( include[0] == '"' ) include_list.push_back(include.substr(1, include.length() - 2));
        fs >> head;
        include = string(head);

    }
    vector<string>method = get_all_name(fs);
    fs.clear();
    fs.seekg(0,ios::beg);

    char buff[1000];
    fs.getline(buff, 1000);
    string line(buff);
    while ( line != "using namespace std;" )
    {
        line += "\n";
        add.push_back(line);
        fs.getline(buff, 1000);
        line = string(buff);
    }

    string alls="";
    while ( !fs.eof() )
    {
        fs.getline(buff, 1000);
        string a(buff);
        a += "\n";
        alls += a;
    }
 
    for each ( string ins in include_list )
    {
        fstream re(title + ins);
        //#pragma region  #pragma endregion
        char tagc[1000];

        while ( re >> tagc )
        {
            string localName;
            //获取一个region中的所有函数，region名为函数所定义的cpp文件
            if ( strcmp(tagc, "region") == 0 )
            {
                //这个region中的所有函数
                vector<string>method_in_region;
                //这个region中的所有被调用的函数
                vector<string>method_will_used;
                re >> tagc;
                //这个region代表的文件名
                localName = string(tagc);
                while ( strcmp(tagc, "#pragma") )
                {
                    string tag(tagc);
                    get_method_name_from_string(tag, method_in_region);
                    re >> tagc;
                }
                for each ( string inside in method_in_region )
                {
                    vector<string>::iterator ins_str = method.begin();
                    vector<string>::iterator now;
                    while ( (now = find(ins_str, method.end(), inside)) !=method.end() )
                    {
                        method_will_used.push_back(*now);
                        ins_str = now+1;
                    }
                }
                //取出这个region的函数实现
                //TODO：嵌套使用函数的抽取
                fstream f_re(title + localName + ".cpp");
                //将整个代码文件通过空格分隔开

                char code_pice[1000];
                while ( f_re.getline(code_pice,1000))
                {
                    vector<string>te_a;
                    string sfinal;
                    get_method_name_from_string(string(code_pice), te_a);
                    if ( te_a.size() == 0 ) continue;
                    if ( find(method_will_used.begin(), method_will_used.end(), te_a[0]) != method_will_used.end() )
                    {
                        sfinal = string(code_pice);
                        //如果找到了那么就取出这个函数
                        stack<string>hkh;
                        f_re.getline(code_pice, 1000);
                        string cos(code_pice);
                        cos += "\n";
                        sfinal += cos;
                        hkh.push("{");
                        while ( !hkh.empty() )
                        {
                            f_re.getline(code_pice, 1000);
                            cos = string(code_pice);
                            if ( cos.size() == 0 ) break;
                            if ( cos.back() == ';' || cos.back() == '}' || cos.back() == '{' ) cos += "\n";
                            sfinal += cos;

                            if ( cos == "{" )
                            {
                                hkh.push(cos);
                            }
                            else if ( cos == "}" )
                            {
                                if ( hkh.top() == "{" )
                                {
                                    hkh.pop();
                                }
                                else
                                {
                                    hkh.push("}");
                                }
                            }
                        }
                        add.push_back(sfinal);
                    }
                }
                f_re.close();
            }
        }
        re.close();
    }
    add.push_back(alls);
    fs.close();
    ofstream fw(title + "main.cpp", ios::out);
    for each ( string ins in add )
    {
        fw.write(ins.c_str(),ins.size());
        //fw.put('\n');
    }
    return 0;
}

//获取文件流中所有函数名称
vector<string> get_all_name(fstream&fs)
{
    char head[1000];
    vector<string>method;
    while ( fs.getline(head, 1000) )
    {
        string te(head);
        int inde = 0;
        while ( true )
        {
            //查找这行是否存在'('
            int psn = te.find('(', inde);

            if ( psn == string::npos ) break;        //没有
            else
            {
                int lsn = psn;
                for ( int i = psn - 1; i >= 0; i-- )
                {
                    if ( te[i] >= 'a'&&te[i] <= 'z' || te[i] >= 'A'&&te[i] <= 'Z' || te[i] == '_' ) lsn = i;
                    else break;
                }
                string worldName(&head[lsn], &head[psn]);
                //如果不为空且不存在才加入数组
                if ( worldName != "" && find(method.begin(), method.end(), worldName) == method.end() )
                {
                    method.push_back(worldName);
                }
                inde = psn + 1;
            }
        }

    }
    return method;
}

void get_method_name_from_string(string te, vector<string>&method)
{

    int inde = 0;
    while ( true )
    {
        //查找这行是否存在'('
        int psn = te.find('(', inde);

        if ( psn == string::npos ) break;        //没有
        else
        {
            int lsn = psn;
            for ( int i = psn - 1; i >= 0; i-- )
            {
                if ( te[i] >= 'a'&&te[i] <= 'z' || te[i] >= 'A'&&te[i] <= 'Z' || te[i] == '_' ) lsn = i;
                else break;
            }
            string worldName = te.substr(lsn, psn-lsn);
            //如果不为空且不存在才加入数组
            if ( worldName != "" && find(method.begin(), method.end(), worldName) == method.end() )
            {
                method.push_back(worldName);
            }
            inde = psn + 1;
        }
    }

}