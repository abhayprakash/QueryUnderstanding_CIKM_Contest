#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
#include<vector>
using namespace std;
map<int, map<int, map<string, int> > > biGram;
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
        //      cout<<tokens<<endl;
                vector<int> tokenNumbers;
                vector<string> classNames;
                string thisClassName;
                int thisTokenNumber;
                while(sinClass >> thisClassName)
                {
                        if(thisClassName[0] == 'C')
                                classNames.push_back(thisClassName);
                }
                while(sinTokens >> thisTokenNumber)
                {
                        tokenNumbers.push_back(thisTokenNumber);
                }

        //      cout<<tokenNumbers.size()<<endl;
                for(int i = 0; i < tokenNumbers.size(); i++)
                {
                        for(int j = i+1; j < tokenNumbers.size(); j++)
                        {
        //                      cout<<"here\n";
        //                      cout<<classNames.size()<<endl;
                                for(int k = 0; k < classNames.size(); k++)
                                {
        //                              cout<<tokenNumbers[i]<<" "<<tokenNumbers[j]<<" "<<classNames[k]<<endl;
                                        biGram[tokenNumbers[i]][tokenNumbers[j]][classNames[k]]++;
                                }
                        }
                }
        }

        map<int, map<int, map<string, int> > >::iterator it;
        ofstream fout2GramFreq("title_2GramFreq.txt");
        for(it = biGram.begin(); it!=biGram.end(); ++it)
        {
                map<int, map<string, int> >::iterator it2;
                for(it2 = (it->second).begin(); it2 != (it->second).end(); ++it2)
                {
                        map<string, int>::iterator it3;
                        int sumTotal = 0;
                        for(it3 = (it2->second).begin(); it3 != (it2->second).end(); ++it3)
                        {
                                sumTotal += it3->second;
                        }

                        for(it3 = (it2->second).begin(); it3 != (it2->second).end(); ++it3)
                        {
                //              cout<<it3->first<<endl;
                                fout2GramFreq<<it->first<<"\t"<<it2->first<<"\t"<<it3->first<<"\t"<<(double)(it3->second)/(double)(sumTotal)<<endl;
                        }
                }
        }

        fout2GramFreq.flush();
        fout2GramFreq.close();
        return 0;
}
