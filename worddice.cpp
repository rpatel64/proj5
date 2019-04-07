#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<fstream>
using namespace std;

void readFile(string file1, string file2, vector<string>&dice, vector<string>&words)
{
	ifstream ifs;
	ifs.open(file1.c_str());
	string line;
	while(getline(ifs, line))
	{
		dice.push_back(line);
	}
//	for(int i = 0; i < dice.size(); i++)
//	{
//		cout << dice[i] << endl;
//	}
	ifs.close();
	ifs.open(file2.c_str());
	while(getline(ifs, line))
	{
		words.push_back(line);
	}
//	for(int i = 0; i < words.size(); i++)
//	{
//		cout << words[i] << endl;
//	}
	ifs.close();	
}

void original(vector<string> dice, string word,map<string, vector<string> >&mygraph)
{
	mygraph.insert(pair<string, vector<string> >("source", dice));
	vector<string> sink;
	sink.push_back("sink");
	vector<string> emptyV;
	for(int i = 0; i < dice.size(); i++)
	{
		mygraph.insert(pair<string, vector<string> >(dice[i], emptyV));
	}
	for(int i = 0; i < word.size(); i++)
	{
		mygraph.insert(pair<string, vector<string> >(to_string(i), sink));
		for(int j = 0; j < dice.size(); j++)
		{
			int loc = dice[j].find(word[i]);
			if(loc == -1) {}
			else
			{
				mygraph[dice[j]].push_back(to_string(i));
			}
		}
	}
	for(map<string, vector<string> >::iterator mit = mygraph.begin(); mit != mygraph.end(); ++mit)
	{
		cout << mit->first << endl;
		for(int j = 0; j < mit->second.size(); j++)
		{
			cout << "\t" << mit->second[j] << endl;
		}
	}
}

void residual(vector<string> dice, string word,map<string, vector<string> >&myRgraph)
{
	vector<string> emptyV;
	myRgraph["source"] = emptyV;
	for(int i = 0; i < dice.size(); i++)
	{
		myRgraph[dice[i]] = emptyV;
	}
	for(int i = 0; i < word.size(); i++)
	{
		myRgraph[to_string(i)] = emptyV;
	}
	myRgraph["sink"] = emptyV;
	for(map<string, vector<string> >::iterator mit = myRgraph.begin(); mit != myRgraph.end(); ++mit)
	{
		cout << mit->first << endl;
		for(int j = 0; j < mit->second.size(); j++)
		{
			cout << "\t" << mit->second[j] << endl;
		}
	}
}

struct sol 
{
	string value;
	string prev;
};

void solution(vector<string> dice, string word, map<string, vector<string> >&mygraph, map<string, vector<string> >&myRgraph)
{
	set<string> v; //visited set
	vector<sol> queue;
	sol first;
	first.value = "source";
	first.prev = "null";
	queue.push_back(first);
	for(int i = 0; i < queue.size(); i++)
	{
		for(int j = 0; j < mygraph[queue[i].value].size(); j++)
		{
			sol temp;
			temp.value = mygraph[queue[i].value][j];
			temp.prev = queue[i].value;
			queue.push_back(temp);
		}
	}
	for(int i = 0; i < queue.size(); i++)
	{
		cout << queue[i].value << " " << queue[i].prev << endl;
	}	

}

int main(int argc, char **argv)
{
	vector<string> dice;
	vector<string> words;
	string file1 = argv[1];
	string file2 = argv[2];
	map<string, vector<string> >mygraph;
	map<string, vector<string> >myRgraph;
	readFile(file1, file2, dice, words);
	original(dice, words[1], mygraph);
	residual(dice, words[1], myRgraph);
	solution(dice, words[1], mygraph, myRgraph);
}
