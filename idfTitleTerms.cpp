#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
#include<vector>
#include<math.h>
using namespace std;
map<int, int> NumOfQueriesHavingThisTerm;
int main()
{
    cout<<log(10)<<endl;
	ifstream finTokens("allUniqueQueries.txt");
	string className, tokens;
	int count = 0;
	while(getline(finTokens,tokens))
	{
		if(count%100000 == 0)
			cout<<"100000 lines\n";
		count++;
		/*****************************************/
		istringstream sinTokens(tokens);
		int tokenNumber;
		map<int, bool> visited;
		while(sinTokens >> tokenNumber)
		{
			if(!visited[tokenNumber])
                NumOfQueriesHavingThisTerm[tokenNumber]++;
            visited[tokenNumber] = true;
		}
	}

    ofstream fout("newUniqueIdfTerm.txt");

    map<int, int>::iterator it;
    for(it = NumOfQueriesHavingThisTerm.begin(); it!=NumOfQueriesHavingThisTerm.end(); ++it)
    {
    	//cout<<it->second<<endl;
        fout<<it->first<<"\t"<<log(4904603/double(it->second))<<endl;
    }

	return 0;
}
