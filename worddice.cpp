#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<fstream>
#include<sstream>
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

void print(map<string, vector<string> >mygraph)
{
	for(map<string, vector<string> >::iterator mit = mygraph.begin(); mit != mygraph.end(); ++mit)
	{
		cout << mit->first << endl;
		for(int j = 0; j < mit->second.size(); j++)
		{
			cout << "\t" << mit->second[j] << endl;
		}
	}
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
	//print(mygraph);
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
	//print(myRgraph);
}

struct sol 
{
	string value;
	string prev;
};

void solution(vector<string> dice, string word, map<string, vector<string> >&mygraph, map<string, vector<string> >&myRgraph)
{
	int solutionCheck = -1;




	while(mygraph["source"].size() != 0)
	{

		// cout << "source size: " << mygraph["source"].size() << endl;
		solutionCheck = mygraph["source"].size();

		set<string> v; //visited set
		vector<sol> queue;
		sol first;

		first.value = "source";
		first.prev = "null";
		queue.push_back(first);
		bool sinkCheck = false;
		for(int i = 0; i < queue.size(); i++)
		{
			for(int j = 0; j < mygraph[queue[i].value].size(); j++)
			{
				if(v.insert(mygraph[queue[i].value][j]).second)
				{
					sol temp;
					temp.value = mygraph[queue[i].value][j];
					temp.prev = queue[i].value;
					queue.push_back(temp);
				}
				if(mygraph[queue[i].value][j] == "sink")
				{
					sinkCheck = true;
					break;
				}
			}
			if(sinkCheck) break;
		}


		string to = "sink";
		string from;
		//print(mygraph);
		for(int i = queue.size() - 1; i > -1; i--)
		{
			from = queue[i].value;
			if(from == to && from != "source")
			{
				to = queue[i].prev;
				for(int j = 0; j < mygraph[to].size(); j++)
				{
					if(mygraph[to][j] == from)
					{
						mygraph[to].erase(mygraph[to].begin()+j);
						break;
					}
				}
				mygraph[from].push_back(to);
				bool clearCheck = false;
				if(myRgraph[to].size() > 0)
				{
					for(int k = 0; k < myRgraph[to].size(); k++)
					{
						if(myRgraph[to][k] == from)
						{
							mygraph[to].erase(mygraph[to].begin()+k);
							clearCheck = true;
							break;
						}
					}
				}
				if(!clearCheck) myRgraph[to].push_back(from);
			}
		}

		for(int i = 0; i < queue.size(); i++)
		{
		//	cout << queue[i].value << " " << queue[i].prev << endl;
		}
		//print(mygraph);
		//print(myRgraph);

		if(solutionCheck == mygraph["source"].size()) {
			break;
		}

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
	bool solutionExists;

	readFile(file1, file2, dice, words);

	for(int currWord = 0; currWord < words.size(); currWord++) {
		
		original(dice, words[currWord], mygraph);
		residual(dice, words[currWord], myRgraph);

		solution(dice, words[currWord], mygraph, myRgraph);

		vector<int> sol;

		sol.resize(dice.size());


		// if(solutionExists) {
			if(myRgraph["source"].size() == dice.size())
			{

				for(int i = 0; i < dice.size(); i++)
				{
					int x;
					string location = myRgraph[dice[i]][0];
					stringstream ss(location);
					ss >> x;
					sol[x] = i;
				}

				cout << sol[0];
				for(int i = 1; i < sol.size(); i++)
				{
					cout << "," << sol[i];
				}

			cout << ": " << words[currWord] << endl;
			}

		else {
			cout << "Cannot spell " << words[currWord] <<  endl;
		}

		// print(myRgraph);
		// cout << "src size: " << myRgraph["source"].size() << endl;
		// cout << "sink size: " << myRgraph["sink"].size() << endl;
		// cout << "dice size: " << dice.size() << endl;
		// cout << "curr word size: " << words[currWord].size() << endl;
		// cout << endl;

		mygraph.clear();
		myRgraph.clear();
		sol.clear();
	}

}
