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


    //��ȡ�ļ�
    fstream fs( title + "main.cpp");
    //����Ҫ����Դ�ļ��ĺ������ַ�������
    vector<string>add;
    char head[1000];
    fs >> head;
    //��ȡinclude
    string include(head);

    string include_end = "std;";
    //��ȡinclude�б�
    vector<string>include_list;
    while ( include != include_end )
    {
        //�ҵ�"�������ļ���
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
            //��ȡһ��region�е����к�����region��Ϊ�����������cpp�ļ�
            if ( strcmp(tagc, "region") == 0 )
            {
                //���region�е����к���
                vector<string>method_in_region;
                //���region�е����б����õĺ���
                vector<string>method_will_used;
                re >> tagc;
                //���region������ļ���
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
                //ȡ�����region�ĺ���ʵ��
                //TODO��Ƕ��ʹ�ú����ĳ�ȡ
                fstream f_re(title + localName + ".cpp");
                //�����������ļ�ͨ���ո�ָ���

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
                        //����ҵ�����ô��ȡ���������
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

//��ȡ�ļ��������к�������
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
            //���������Ƿ����'('
            int psn = te.find('(', inde);

            if ( psn == string::npos ) break;        //û��
            else
            {
                int lsn = psn;
                for ( int i = psn - 1; i >= 0; i-- )
                {
                    if ( te[i] >= 'a'&&te[i] <= 'z' || te[i] >= 'A'&&te[i] <= 'Z' || te[i] == '_' ) lsn = i;
                    else break;
                }
                string worldName(&head[lsn], &head[psn]);
                //�����Ϊ���Ҳ����ڲż�������
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
        //���������Ƿ����'('
        int psn = te.find('(', inde);

        if ( psn == string::npos ) break;        //û��
        else
        {
            int lsn = psn;
            for ( int i = psn - 1; i >= 0; i-- )
            {
                if ( te[i] >= 'a'&&te[i] <= 'z' || te[i] >= 'A'&&te[i] <= 'Z' || te[i] == '_' ) lsn = i;
                else break;
            }
            string worldName = te.substr(lsn, psn-lsn);
            //�����Ϊ���Ҳ����ڲż�������
            if ( worldName != "" && find(method.begin(), method.end(), worldName) == method.end() )
            {
                method.push_back(worldName);
            }
            inde = psn + 1;
        }
    }

}