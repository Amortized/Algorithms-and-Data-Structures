/*
Multi Pattern Matching Algorithm : 
Preprocessing: O(M) where M: total len of all the keywords
Search : Sublinear to the size of the text.
*/

#include <iostream>
#include <algorithm>
#include <string>
#include <queue>
#include "trie.cpp"
#include <map>


#define NOTDEFINED -1
#define XSIZE 256 //How many types of characters
#define LONGESTSIZE 10000

using namespace std;

class SetOracleBackwardMatching
{
private:
	  Trie<string,char>* oracle; 
	  void buildTrie();
	  int minKeyLen;
	  vector<string> patterns_;
	  vector<string> replacements_;
	  string text_;
	  string replacedText_;
	  bool checkIfKeywords(set<int> indexes,int pos);

public:
	void constructFactorOracleMulti(vector<string> patterns);
	void printOracle();
	string search(string text, vector<string> replacements);
	
	SetOracleBackwardMatching() {  oracle = new Trie<string,char>(); }
	~SetOracleBackwardMatching() { delete oracle; }
};



/* This function preprocess list of Patterns. Result is a Oracle a automata for representing 
   all the substrings possible from all combinations of patterns. This is constructed in O(M) where M is the sum of lengths of all the patterns.
   */
void SetOracleBackwardMatching::constructFactorOracleMulti(vector<string> patterns)
{
    //Need a Set of Reverse Patterns chopped off the min length.
	//Find the Shortest Pattern Length
	
	if(patterns.size() == 0)
		return;

	for(vector<string>::iterator it = patterns.begin(); it != patterns.end(); it++)
	{
		patterns_.push_back(*it);
	}

	int min = 10000;
	int len = 0;
	int totalLenSum = 0;

	for(int i=0;i<patterns.size();i++)
	{   len = patterns.at(i).length();
		totalLenSum += len;
		if(len < min)
			min = len;
	}
	
	//Record the min
	minKeyLen = min;

	//Cut the Patterns to min length; reverse them and form a set
	set<string> reverseOracles;
	map<string, set<int> > prefixesWords;

	for(int i=0;i<patterns.size();i++)
	{
		patterns.at(i) = patterns.at(i).substr(0,min); 
		reverse(patterns.at(i).begin(),patterns.at(i).end());
		pair<set<string>::iterator,bool> mypair = reverseOracles.insert(patterns.at(i));

		if(mypair.second == false) //Same Reverse Prefix
		{
			(prefixesWords[*mypair.first]).insert(i); //Push this Index
		}
		else
		{
			set<int> temp;
			temp.insert(i);
			prefixesWords[*mypair.first] = temp;
		}
		
	}

	//Add Words to the Trie
	for(set<string>::const_iterator it = reverseOracles.begin(); it != reverseOracles.end(); it++)
	{   
	   oracle->addWord(*it,prefixesWords[*it]);
	}
	
	//Now Traverse the Trie in BFS
	Node<char>* current = oracle->getRoot();
	queue<Node<char>*> bfsQueue;
	Node<char>* supplyNode;

	current->setSupply(NULL); //Set the Supply of Root Node to Null

	bfsQueue.push(current);
	while(!bfsQueue.empty())
	{
		current = bfsQueue.front();

		if(current->getParent()!=NULL) //Not to be done for the Root
		{
			Node<char>* k = current->getParent()->getSupply(); //Get the Supply of the Parent
			char c = current->getContent(); //Get the label from parent to current Node. This is stored in current node itself
			while ( (k != NULL) && (supplyNode=k->findChild(c))==NULL)
			{ 
				k->addChild(current);
				k = k->getSupply();
			}
			  
			if( k == NULL)
			  {
				current->setSupply(oracle->getRoot()); 
			  }
			else
			{ 
				current->setSupply(supplyNode);
			}

		}

		//Continue remaining operation of BFS Algorithm
		bfsQueue.pop();	
		vector<Node<char>*> childrens = current->getChildrens();
		for(int i = 0; i < childrens.size(); i++)
		{
			Node<char>* child = childrens.at(i);
			if(!child->getBFSMarker())
			{
				child->setBFSMarker();
				bfsQueue.push(child);
			}
		}
	}
}

//This Function prints the Oracle After it is constructed.
void SetOracleBackwardMatching::printOracle()
{
	cout << "----------------------------------------" << endl;
	//Now Traverse the Trie in BFS
	Node<char>* current = oracle->getRoot();
	queue<Node<char>*> bfsQueue;

	bfsQueue.push(current);
	while(!bfsQueue.empty())
	{
		current = bfsQueue.front();


	    /////
		cout << "Node: " << current->getContent();
		if(current->wordMarker() == true)
		{
			//Output the Indices
		//	cout << " Found it "; 
			set<int> indexes = current->getKeywordIndexes();
	        for(set<int>::iterator it1 = indexes.begin(); it1 != indexes.end(); it1++){
				cout <<  *it1 << " ";
			}
		}
		cout << "Child: ";
		////////////


		bfsQueue.pop();


	
		vector<Node<char>*> childrens = current->getChildrens();
		for(int i = 0; i < childrens.size(); i++)
		{
			Node<char>* child = childrens.at(i);
			std::cout << child->getContent() << " -" << child->getBFSMarker1() << "  ";
			if(!child->getBFSMarker1())
			{
				child->setBFSMarker1();
				bfsQueue.push(child);
			}
		}
		cout << endl;

	}
}
 
/* Helper function: It used when the search window on the text is accepted by automata. Every terminal node in the algorithm maintains a
set of patterns that it represents.Initially the window in the text is checked whether it is a prefix of any of keywords stored at this 
termimal.Only when that is successful will it start comparing patterns with the text starting from that position */
bool SetOracleBackwardMatching::checkIfKeywords(set<int> indexes,int pos)
{
	string prefixToMatch = text_.substr(pos,minKeyLen);
	string keyword;
	bool prefixFound = false;
	int i;
	int index = 0;

	//Check if prefixToMatch is a prefix in a keyword
	for(set<int>::iterator it1 = indexes.begin(); it1 != indexes.end(); it1++){
		index = *it1;
		keyword = patterns_.at(index);
		for(i=0;i<prefixToMatch.length();i++)
		{  
			if(prefixToMatch.at(i) != keyword.at(i))
				break;  //Check the Next Keyword
		}

		if(i == prefixToMatch.length())
		{
			//Prefix Matched with one of the keywords
			prefixFound = true;
			break; //Check No more Prefix Matches
		}
	}

	if(prefixFound)
	{   
		for(set<int>::iterator it1 = indexes.begin(); it1 != indexes.end(); it1++)       
		{
			keyword = patterns_.at(*it1);
			if(keyword.compare(text_.substr(pos,keyword.length())) == 0 )
			{
				replacedText_.replace(pos,keyword.length(),replacements_.at(*it1));
			}
		}
	}
	return prefixFound;
}

/*Searches the text against the pattern. Window of the search is length of the shortest len pattern.*/
string SetOracleBackwardMatching::search(string text, vector<string> replacements)
{
	if(text.empty())
		return text;

	text_ = text;
	replacedText_ = text;

	//Copy replacements
	for(vector<string>::iterator it = replacements.begin(); it != replacements.end(); it++)
		replacements_.push_back(*it);

	int pos = 0;
	int j;
	while (pos <= text.size() - minKeyLen)
	{  
		Node<char>* current = oracle->getRoot();
		j = minKeyLen-1;

		while(j>=0 && current!=NULL)
		{ 
			current = current->findChild(text.at(pos+j));
			j--;
		}

		if(current!=NULL && j == -1)
		{
			if(checkIfKeywords(current->getKeywordIndexes(),pos) == true )
			{    
				j = -1;
			}
		}			

		pos = pos + j + 2;	
	}
	
	return replacedText_;
}


int main()
{

	SetOracleBackwardMatching* SOBM;
	vector<string> patterns;
    vector<string> replacements;
	


	patterns.push_back("we");
	patterns.push_back("SFO");
	patterns.push_back("meet");
	patterns.push_back("meete");
	patterns.push_back("jeeter");
	patterns.push_back("beer");
	patterns.push_back("cokerre");
	patterns.push_back("www.facebook.com");
	patterns.push_back("www.yahoo.com");
	patterns.push_back("jaily");
	patterns.push_back("MONEY");

	replacements.push_back("be");
	replacements.push_back("JFK");
	replacements.push_back("talk");
	replacements.push_back("keete");
	replacements.push_back("deeter");
	replacements.push_back("coca");
	replacements.push_back("joke");
	replacements.push_back("social");
	replacements.push_back("WEBLOOSER");
	replacements.push_back("daily");
	replacements.push_back("HONEY");


	vector<string> texts;
	texts.push_back("Hi can we meet tomorrow at SFO. Get some beer");
	texts.push_back("Today is jeeter day. There will be beer and coke");
	texts.push_back("Can we this web page at www.yahoo.com");
	texts.push_back("Today is jaily party.Event at abc.com");
	texts.push_back("I am flying tomorrow from JFK");
	texts.push_back("I need my MONEY back");
	texts.push_back("You are the best jaily");
	texts.push_back("Get the money. we will party");
	texts.push_back("abc.com is website where we crack MONEY ");

	for(int i= 0; i < texts.size(); i++)
	{
		SOBM = new SetOracleBackwardMatching();
		SOBM->constructFactorOracleMulti(patterns);
		cout << SOBM->search(texts.at(i),replacements) << endl;
		delete SOBM;
	}
	
	return 0;
}
