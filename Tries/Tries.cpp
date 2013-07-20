/*

1. Tries are used to implement Associative Arrays(key-value) where keys are strings.  

Only value is stored in the node. Key is defined by its position in the tree. 
Complexity.  0(m) - lookup for a word of length m. BST takes log(n); Compared to Hash table in worst case with collisions, its look up will be faster. 
		    because in hash table o(m) time is spent in calculating the hash.If there are collisions then it gets screwed.
         	     Can answer queries like which key is closest to asked query which BST cant do. 

Insert and Delete are also faster. Tries are faster than Hash table for inserts because no need to rebuild indexes.

2. Tries support ordered iteration. that means u can access they keys in a particular order . This will help you to sort the keys. 
   Insert all they keys in Trie. Perform Pre order traversal gives lexico sorted keys.
   
3. Tries can also be useful for finding the longest prefix match. 
										     	
*/

#include<iostream>
#include<vector>



using namespace std;



template<typename T1>
class Node {
public:
  Node()  { wmarker_ = false; }
  ~Node()  { } 
  typedef vector<Node<T1>*> childrenType;
  T1 getContent() { return content_; }
  void setContent(T1 c) { content_ = c; }
  bool wordMarker() { return wmarker_; }
  void setWordMarker() { wmarker_ = true; }
  childrenType getChildrens() { return children_; }
  Node<T1>* findChild(T1);
  void addChild(Node<T1>* child) { children_.push_back(child); }
  void removeChild(Node<T1>* child);
private:
  int prefixes_; //No of prefixes with this vertex
  int words_; //No of words that exactly match this vertex
  vector<Node<T1>*> children_;
  bool wmarker_; //used to indicate the end of the word
  T1 content_; 

};

template<typename T1>
Node<T1>* Node<T1>::findChild(T1 c) {
  for(int i=0;i<children_.size();i++) {
    Node<T1>* tmp = children_.at(i);
    if(tmp->content_ == c) 
      return tmp;
  }
  return NULL;
}


template<typename T1>
void Node<T1>::removeChild(Node<T1>* child) {

  for(int i=0;i<children_.size();i++) {
    Node<T1>* tmp = children_.at(i);
    if(tmp->content_ == child->content_) {
      children_.erase(children_.begin()+i);
      delete tmp;    
    }
  }
} 

template<typename T,typename T1>
class Trie {
public:
  Trie() {  root = new Node<T1>();  };
  ~Trie() {
      vector<Node<T1>*> children = root->getChildrens();
      for(int i=0;i<children.size();i++)
       delete children.at(i);
  }
  void addWord(T);
  bool searchWord(T);
  void deleteWord(T);
  T longestPrefixMatch(T);
  int countWordsCharDeleted(Node<T1>*,T,int,int);
  Node<T1>* getRoot() { return root; } 
private:
  Node<T1>* root;
};


template<typename T,typename T1>
void Trie<T,T1>::addWord(T word) {
 Node<T1>* current = root;

 if(word.length() == 0) { 
   current->setWordMarker();
   return;
 }	

 for(int i=0; i<word.length(); i++) {
   Node<T1>* child = current->findChild(word[i]);
    
   if(child!=NULL) {
     current = child;
   } else {
     Node<T1>* mynode = new Node<T1>();
     mynode->setContent(word[i]);		
     current->addChild(mynode);
     current = mynode;
    }

  if(i == word.length() - 1)  
    current->setWordMarker();
  } 
}


template<typename T,typename T1>
T Trie<T,T1>::longestPrefixMatch(T word)
{
  Node<T1>* current = root;

  if(current==NULL)
    return "";

  while(current!=NULL) {
    for(int i=0;i<word.length();i++) {	
          Node<T1>* tmp = current->findChild(word[i]);	
	  if(tmp == NULL) { //No more Match
	    if(i==0) //First Character Didnt Macth
	     return "";
	    else	
	    return word.substr(0,i);	
	  }
         else
 	  current = tmp;	  
     }


    //Whole word Matched
	return word.substr(0,word.length());
  }

}

template<typename T,typename T1>
bool Trie<T,T1>::searchWord(T word) {
  Node<T1>* current = root;
  
  while(current!=NULL) {
    for(int i=0; i<word.length(); i++) {
      Node<T1>* tmp = current->findChild(word[i]);		
      if(tmp == NULL)
	return false;
      else
	current = tmp; 		
    }

    if(current->wordMarker())
     return true;
    else
     return false;			
  }
  return false;
}

/*
Need to fix this

template<typename T,typename T1>
int Trie<T,T1>::countWordsCharDeleted(Node<T1>* vertex,T word,int missingLetters,int currentCount)
{
   std::cout << "Word:" << word << std::endl;
   if(missingLetters<0)
	return currentCount;

   int total = currentCount;
   if(word.empty()) //Word is completed
   {
	if(vertex->wordMarker())
	  return (total+1) ;//No more Word
	else
	  return 0;
   }
     
   else {

	Node<T1>* tmp = vertex->findChild(word.at(0));
	if(tmp == NULL) { //Word Does not exist 
	   if(missingLetters == 0)	 
	  	return 0;
	   else
		return countWordsCharDeleted(vertex,word.substr(1,word.length()),missingLetters-1,total); 
        }
	else 
	{  
	   total = countWordsCharDeleted(vertex, word, missingLetters-1,total); //Keep the word as it is but assume the first character was deleted
	   total += countWordsCharDeleted(tmp,word.substr(1,word.length()),missingLetters,total); //First Character was present
	}	

   } 	

  return total; 			 	   
			   	
}
*/


template<typename T,typename T1>
void Trie<T,T1>::deleteWord(T word) {
  Node<T1>* parent = NULL;
  Node<T1>* current = root;
  
  while(current!=NULL) {
    for(int i=0; i<word.length(); i++) {
      Node<T1>* tmp = current->findChild(word[i]);
      if(tmp == NULL) {
         cout << "Word does not exist:" << endl; 
       	return;
      }
      else {
         parent = current;
         current = tmp;
      }
    } 
 
    parent->removeChild(current);
    cout << word << " has been deleted:" << endl;
    return;
     
  }

  cout << "Word does not exist:" << endl;

}


int main() {
    Trie<string,char>* trie = new Trie<string,char>();
    trie->addWord("Hello");
    trie->addWord("Balloon");
    trie->addWord("Ball");

    if ( trie->searchWord("Hell") )
        cout << "Found Hell" << endl;
    
    if ( trie->searchWord("Hello") )
        cout << "Found Hello" << endl;

    if ( trie->searchWord("Helloo") )
        cout << "Found Helloo" << endl;

    if ( trie->searchWord("Ball") )
        cout << "Found Ball" << endl;

    if ( trie->searchWord("Balloon") )
        cout << "Found Balloon" << endl; 

    trie->deleteWord("Balloon"); 
    trie->deleteWord("Ball");
    trie->deleteWord("Hello");

    if (! trie->searchWord("Balloon") )
        cout << "Did not Found Balloon" << endl;
    if (! trie->searchWord("Hello") )
        cout << "Did not Found Balloon" << endl;
    if (! trie->searchWord("Ball") )
        cout << "Did not Found Ball" << endl;


    //Longest Prefix Match
    trie->addWord("HelloSirk");
    trie->addWord("HelloSiririlm");
    trie->addWord("HelloSirij");

    cout << trie->longestPrefixMatch("HelloSirirk") << endl;
    cout << trie->countWordsCharDeleted(trie->getRoot(),"HeelloSirk",2,0) << endl;
    //Sorting Keys using Tries
	

    delete trie;

return 0;
}

