#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
#include<vector>
using namespace std;
map<int, map<string, int> > uniGram;
int main()
{
        ifstream finClass("uniqueTitleClass.txt");
        ifstream finTokens("uniqueTitleTokens.txt");
        string className, tokens;
        int count = 0;
        while(getline(finClass,className))
        {
                if(count%100000 == 0)
                        cout<<"100000 lines\n";
                count++;
                getline(finTokens,tokens);
                /*****************************************/
                istringstream sinClass(className);
                istringstream sinTokens(tokens);
                int tokenNumber;
                vector<string> classNames;
                string thisClassName;
                while(sinClass >> thisClassName)
                {
                        if(thisClassName[0] == 'C')
                                classNames.push_back(thisClassName);
                }
                while(sinTokens >> tokenNumber)
                {
                        for(int i = 0; i < classNames.size(); i++)
                        {
                                uniGram[tokenNumber][classNames[i]]++;
                        }
                }
        }

        map<int, map<string, int> >::iterator it;
        ofstream fout1GramFreq("title_1GramFreq.txt");
        for(it = uniGram.begin(); it!=uniGram.end(); ++it)
        {
                map<string, int>::iterator it2;
                int sumTotal = 0;
                for(it2 =  (it->second).begin(); it2!=(it->second).end(); ++it2)
                {
                        sumTotal += it2->second;
                }
                for(it2 = (it->second).begin(); it2 != (it->second).end(); ++it2)
                {
                        fout1GramFreq<<it->first<<"\t"<<it2->first<<"\t"<<(double)(it2->second)/(double)(sumTotal)<<endl;
                }
        }
        fout1GramFreq.flush();
        fout1GramFreq.close();
        return 0;
}
