#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
#include<vector>

using namespace std;

string testFile = "test.txt";
string trainFile = "train";
string sessionInfoFile = "sessionInfo.txt";

int main()
{
    ifstream fin(trainFile.c_str());
    ofstream fout(sessionInfoFile.c_str());
    string s;
    int count = 0;
    int sessionId = 1;
    while(getline(fin,s))
    {
        if(count%10000000 == 0)
            cout<<"10000000 lines\n";
        count++;

        if(s=="")
        {
            sessionId++;
            continue;
        }

        fout<<sessionId<<"\t"<<s<<"\n";
    }
    return 0;
}
