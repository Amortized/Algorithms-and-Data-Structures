#include <vector>
#include <set>

using namespace std;

template<typename T1>
class Node {
public:
   Node()  { wmarker_ = false;  bfsmarker_ = false; bfsmarker_1 = false; }
  ~Node()  { } 
  typedef vector<Node<T1>*> childrenType;

  T1 getContent() { return content_; }
  void setContent(T1 c) { content_ = c; }

  bool wordMarker() { return wmarker_; }
  void setWordMarker() { wmarker_ = true; }

  void setBFSMarker() { bfsmarker_ = true; }
  bool getBFSMarker() { return bfsmarker_; }

  void setBFSMarker1() { bfsmarker_1 = true; }
  bool getBFSMarker1() { return bfsmarker_1; }


  childrenType getChildrens() { return children_; }
  Node<T1>* findChild(T1);
  void addChild(Node<T1>* child) { children_.push_back(child); child->setParent(this); }
  void removeChild(Node<T1>* child);

  void setKeywordIndexes(set<int> kIndexes) { keywordIndexes = kIndexes; }
  set<int> getKeywordIndexes() { return keywordIndexes; }

  void setParent(Node<T1>* p) { parent.insert(p); }
  Node<T1>* getParent() { return *(parent.begin()); }

  void setSupply(Node<T1>* s) { supply = s; }
  Node<T1>* getSupply() { return supply; }



private:
  vector<Node<T1>*> children_;
  bool wmarker_; //used to indicate the end of the word
  set<int> keywordIndexes; //This is used to hold the set of indexes of keywords to which this substring belongs to
  T1 content_; 
  bool bfsmarker_;
  bool bfsmarker_1;

  set<Node<T1>*> parent;
  Node<T1>* supply;

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
	Trie() {  root = new Node<T1>(); root->setParent(NULL);  };
  ~Trie() {
      vector<Node<T1>*> children = root->getChildrens();
      for(int i=0;i<children.size();i++)
       delete children.at(i);
  }
  void addWord(T,set<int> keywordIndexes);
  bool searchWord(T);
  Node<T1>* getRoot() { return root; } 
private:
  Node<T1>* root;
};


template<typename T,typename T1>
void Trie<T,T1>::addWord(T word,set<int> keywordIndexes) {

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

  if(i == word.length() - 1) {
    current->setWordMarker();
    current->setKeywordIndexes(keywordIndexes);
  } 
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


