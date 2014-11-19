#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
#include<vector>
using namespace std;
map<int, map<int, map<int, map<string, int> > > > triGram;
int main()
{
	ifstream finClass("ngramClass.txt");
	ifstream finTokens("ngramTokens.txt");
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
	//	cout<<tokens<<endl;
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
	//	cout<<tokenNumbers.size()<<endl;
		for(int i = 0; i < tokenNumbers.size(); i++)
		{
			for(int j = i+1; j < tokenNumbers.size(); j++)
			{
				for(int l = j + 1; l < tokenNumbers.size(); l++)
				{
					for(int k = 0; k < classNames.size(); k++)
					{
						triGram[tokenNumbers[i]][tokenNumbers[j]][tokenNumbers[l]][classNames[k]]++;
					}
				}
			}
		}
	}
	
	map<int, map<int, map<int, map<string, int> > > >::iterator it;
	ofstream fout3GramFreq("3GramFreq.txt");
	for(it = triGram.begin(); it!=triGram.end(); ++it)
	{
		map<int, map<int, map<string, int> > >::iterator it2;
		for(it2 = (it->second).begin(); it2 != (it->second).end(); ++it2)
		{
			map<int, map<string, int> >::iterator it3;
			for(it3 = (it2->second).begin(); it3 != (it2->second).end(); ++it3)
			{
				map<string, int>::iterator it4;
				int sumTotal = 0;
				for(it4 = (it3->second).begin(); it4 != (it3->second).end(); ++it4)
				{
					sumTotal += it4->second;
				}
		        
				for(it4 = (it3->second).begin(); it4 != (it3->second).end(); ++it4)
                        	{
		                	fout3GramFreq<<it->first<<"\t"<<it2->first<<"\t"<<it3->first<<"\t"<<it4->first<<"\t"<<(double)(it4->second)/(double)(sumTotal)<<endl;
				}
                        } 	
		}
	}

	fout3GramFreq.flush();
	fout3GramFreq.close();
	return 0;
}
