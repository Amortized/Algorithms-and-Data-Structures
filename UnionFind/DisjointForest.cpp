#include "DisjointForest.h"

using namespace std;


DisjointForest::DisjointForest()
{
  numElements = 0;
  numSets = 0;
}

DisjointForest::~DisjointForest()
{
  for(int i=0;i<numElements;i++)
     delete numNodes.at(i);
  numNodes.clear();
  numElements = 0;
  numSets = 0;
}

DisjointForest::DisjointForest(int count)
{
  numElements = 0;
  numSets = 0;
  addElements(count);	
}


DisjointForest::DisjointForest(const DisjointForest& s)
{
  this->numElements = s.getNumElements();
  this->numSets = s.getNumSets();

  numNodes.resize(numElements);
  for(int i=0;i<numElements;i++) {	
   numNodes[i] = new Node();
   numNodes[i]->parent = s.numNodes[i]->parent;
   numNodes[i]->rank = s.numNodes[i]->rank;
   numNodes[i]->index = s.numNodes[i]->index;	
  }
}

void DisjointForest::addElements(int count) 
{
  //Push at the end
  numNodes.insert(numNodes.end(), count, (Node*)NULL);
  for(int i = numElements; i < numElements + count; i++) 
  {
	numNodes[i] = new Node();
	numNodes[i]->parent = NULL; //No Parent as of now
	numNodes[i]->rank = 0; //There are 0 nodes beneath this node.ie no one has this as parent
	numNodes[i]->index = i; //Id of the Node

  }

  numElements+=count;
  numSets+=count;
}

int DisjointForest::getNumElements() const
{
  return numElements;
}

int DisjointForest::getNumSets() const
{
 return numSets;
}

//Find the set to which this element belongs
int DisjointForest::findSet(int elementId) const
{
  Node* current = numNodes[elementId];

  while(current->parent!=NULL) { 	
    current = current->parent;	
  }
  //Parent Found for this elementId
  Node* root = current;

  //Now go up again set their parent along the path; This is amortization step
  current = numNodes[elementId];
  while(current->parent!=NULL) {
    Node* oldparent = current->parent;
    current->parent = root; //Set the New Parent
    current = oldparent;	
  }

  //Return Id of the Parent
  return root->index;
}


//Does Union of two sets. always a tree with lower lank is added to higher rank. this aids in faster findSet
void DisjointForest::unionSet(int set1, int set2)
{
  //Parent of two nodes are same; no need to union
  if(set1 == set2)
    return;	

  //Get the ranks of two parents
  int xrank = numNodes[set1]->rank;
  int yrank = numNodes[set2]->rank;

  if(xrank < yrank) 
  {
    numNodes[set1]->parent = numNodes[set2]; //Set set1's parent to set2	
  }  
  else if (xrank > yrank) 
  { 
    numNodes[set2]->parent = numNodes[set1]; //Set set2's parent to set1	
  }
  else
  {
    numNodes[set2]->parent = numNodes[set1];
    ++numNodes[set1]->rank; //There is one more element which has set1 as its parent; so increase this info in set1
  }	

  --numSets;
}



