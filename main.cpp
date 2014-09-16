#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
#include<vector>
#include<time.h>
using namespace std;

string n_1gram = "1GramFreq.txt";
string n_2gram = "2GramFreq.txt";
string n_3gram = "3GramFreq.txt";

string title_1gram = "title_1GramFreq.txt";
string title_2gram = "title_2GramFreq.txt";
string testQueriesWithKnownTitle = "testQueriesWithKnownTitle.txt";
string title_testQueriesWithKnownTitle = "Title_testQueriesWithKnownTitle.txt";

string session_TestQuery = "query_testQuerySession.txt";
string sid_TestQuery = "sid_testQuerySession.txt";
string sessionIntentFile = "sessionIntent.txt";

string testFile = "test.txt";
string resultFile = "resultQuerySessionAllTitleW8TermWeighted.txt";

map<int, map<string, double> > table_1Gram;
map<int, map<int, map<string, double> > > table_2Gram;
map<int, map<int, map<int, map<string, double> > > > table_3Gram;
map<string, vector<string> > queryTitle;
//map<string, string> queryTitle;
map<int, map<string, double> > table_Title1Gram;
map<int, map<int, map<string, double> > > table_Title2Gram;

map<int, string> sessionIntent;
map<string, int> testQuerySession;

void ReadSessionIntent()
{
    ifstream fin(sessionIntentFile.c_str());
    int sid;
    string s;
    while(fin>>sid)
    {
        getline(fin,s);
        sessionIntent[sid] = s;
    }
    fin.close();
}

void ReadQuerySid()
{
    ifstream finQuery(session_TestQuery.c_str());
    ifstream finSid(sid_TestQuery.c_str());
    int sid;
    string s;
    while(getline(finQuery,s))
    {
        finSid>>sid;
        testQuerySession[s] = sid;
    }
    finQuery.close();
    finSid.close();
}

void Read1Grams()
{
    ifstream fin(n_1gram.c_str());
    string s, cl;
    int token;
    double prob;
    while(getline(fin, s))
    {
        istringstream sin(s);
        sin>>token>>cl>>prob;
        table_1Gram[token][cl] = prob;
    }
    fin.close();
}

void Read2Grams()
{
    ifstream fin(n_2gram.c_str());
    string s, cl;
    int token1, token2;
    double prob;
    while(getline(fin, s))
    {
        istringstream sin(s);
        sin>>token1>>token2>>cl>>prob;
        table_2Gram[token1][token2][cl] = prob;
    }
    fin.close();
}

void Read3Grams()
{
    ifstream fin(n_3gram.c_str());
    string s, cl;
    int token1, token2, token3;
    double prob;
    while(getline(fin, s))
    {
        istringstream sin(s);
        sin>>token1>>token2>>token3>>cl>>prob;
        table_3Gram[token1][token2][token3][cl] = prob;
    }
    fin.close();
}

void SolveTest()
{
    ifstream fin(testFile.c_str());
    ofstream fout(resultFile.c_str());
    string s;
    string answerClass;
    map<string, double>::iterator it;
    int thisToken;
    int count = 0;
    while(getline(fin, s))
    {
        if(count%1000 == 0)
            cout<<"1000 lines\n";
        count++;
        istringstream sin(s);
        vector<int> tokens;

        while(sin>>thisToken)
        {
            tokens.push_back(thisToken);
        }

        map<string, double> probOfThisClass;
        //map<string, int> countOfThisClass;
        double denom = 0;

        for(int i = 0; i < tokens.size(); i++)
        {
            for(it = table_1Gram[tokens[i]].begin(); it != table_1Gram[tokens[i]].end(); ++it)
            {
                //if(it->second > maxProb)
                //{
                    probOfThisClass[it->first] += it->second;//(probOfThisClass[it->first] * countOfThisClass[it->first]) / (1 + countOfThisClass[it->first]);
                    denom += it->second;
                    //countOfThisClass[it->first]++;
                //}
            }
        }

        for(int i = 0; i < tokens.size(); i++)
        {
            for(int j = i+1; j < tokens.size(); j++)
            {
                for(it = table_2Gram[tokens[i]][tokens[j]].begin(); it != table_2Gram[tokens[i]][tokens[j]].end(); ++it)
                {
                    //if(it->second > maxProb)
                    //{
                        probOfThisClass[it->first] += it->second;//(probOfThisClass[it->first] * countOfThisClass[it->first]) / (1 + countOfThisClass[it->first]);
                        denom += it->second;
                    //}
                }
            }
        }

        /************************/
        for(int i = 0; i < tokens.size(); i++)
        {
            for(int j = i+1; j < tokens.size(); j++)
            {
                for(int k = j+1; k < tokens.size(); k++)
                {
                    for(it = table_3Gram[tokens[i]][tokens[j]][tokens[k]].begin(); it != table_3Gram[tokens[i]][tokens[j]][tokens[k]].end(); ++it)
                    {
                        //if(it->second > maxProb)
                        //{
                            probOfThisClass[it->first] += it->second;//(probOfThisClass[it->first] * countOfThisClass[it->first]) / (1 + countOfThisClass[it->first]);
                            denom += it->second;
                        //}
                    }
                }
            }
        }
        /************************/

        if(queryTitle.find(s) != queryTitle.end())
        {
            for(int si = 0; si < queryTitle[s].size(); si++)
            {
                string EarlierTitleForThis = queryTitle[s][si];

                vector<int> titleTokens;

                istringstream sinTitle(EarlierTitleForThis);
                int thisTitleToken;
                while(sinTitle >> thisTitleToken)
                {
                    titleTokens.push_back(thisTitleToken);
                }

                if(titleTokens.size() > 8)
                    continue;

                for(int i = 0; i < titleTokens.size(); i++)
                {
                    for(it = table_Title1Gram[titleTokens[i]].begin(); it != table_Title1Gram[titleTokens[i]].end(); ++it)
                    {
                        probOfThisClass[it->first] += it->second;
                        denom += it->second;
                    }
                }

                for(int i = 0; i < titleTokens.size(); i++)
                {
                    for(int j = i+1; j < titleTokens.size(); j++)
                    {
                        for(it = table_Title2Gram[titleTokens[i]][titleTokens[j]].begin(); it != table_Title2Gram[titleTokens[i]][titleTokens[j]].end(); ++it)
                        {
                            probOfThisClass[it->first] += it->second;
                            denom += it->second;
                        }
                    }
                }
            }
        }

        /************************/

        /**context consideration*/

        if(testQuerySession.find(s) != testQuerySession.end())
        {
            int sid = testQuerySession[s];
            if(sessionIntent.find(sid) != sessionIntent.end())
            {
            	vector<string> classes;
            	string s = sessionIntent[sid];
            	istringstream sin(s);
            	string thisClass;
				while(sin >> thisClass)
				{
					if(thisClass[0] == 'C')
						classes.push_back(thisClass);
				}

				for(int class_i = 0; class_i < classes.size(); class_i++)
				{
					probOfThisClass[classes[class_i]] += 0.8;
                	denom += 0.8;
				}
            }
        }

        /************************/
        map<string, double>::iterator it1;

        for(it1 = probOfThisClass.begin(); it1 != probOfThisClass.end(); ++it1)
        {
            it1->second = (it1->second)/denom;
        }

        double maxProb = 0;
        for(it1 = probOfThisClass.begin(); it1 != probOfThisClass.end(); ++it1)
        {
            if(it1->second > maxProb)
            {
                maxProb = it1->second;
                answerClass = it1->first;
            }
        }

        double secondMaxProb = 0;
        string secondAnswerClass;
        for(it1 = probOfThisClass.begin(); it1 != probOfThisClass.end(); ++it1)
        {
            if(it1->second > secondMaxProb && it1->second != maxProb)
            {
                secondMaxProb = it1->second;
                secondAnswerClass = it1->first;
            }
        }

        if(maxProb <= 1.6 * secondMaxProb)
            answerClass += " | " + secondAnswerClass;

        fout<<s<<"\t"<<answerClass<<endl;
    }
    fin.close();
    fout.flush();
    fout.close();
}

void ReadQueryTitle()
{
    ifstream finQueries(testQueriesWithKnownTitle.c_str());
    ifstream finTitle(title_testQueriesWithKnownTitle.c_str());
    string query, title;
    while(getline(finQueries, query))
    {
        getline(finTitle, title);
        //if(queryTitle.find(query) == queryTitle.end())
        queryTitle[query].push_back(title);
    }
}

void ReadTitle1Grams()
{
    ifstream fin(title_1gram.c_str());
    string s, cl;
    int token;
    double prob;
    while(getline(fin, s))
    {
        istringstream sin(s);
        sin>>token>>cl>>prob;
        table_Title1Gram[token][cl] = prob;
    }
    fin.close();
}

void ReadTitle2Grams()
{
    ifstream fin(title_2gram.c_str());
    string s, cl;
    int token1, token2;
    double prob;
    while(getline(fin, s))
    {
        istringstream sin(s);
        sin>>token1>>token2>>cl>>prob;
        table_Title2Gram[token1][token2][cl] = prob;
    }
    fin.close();
}

int main()
{
    //Session
    ReadQuerySid();
    cout<<"read query->sid\n";
    ReadSessionIntent();
    cout<<"read sid->intent\n";

    //Title
    ReadQueryTitle();
    cout<<"read query->title\n";
    ReadTitle1Grams();
    cout<<"read title 1 grams\n";
    ReadTitle2Grams();
    cout<<"read title 2 grams\n";

    //Queries
    Read1Grams();
    cout<<"read query 1 grams\n";
    Read2Grams();
    cout<<"read query 2 grams\n";
    Read3Grams();
    cout<<"read query 3 grams\n";

    SolveTest();
}
