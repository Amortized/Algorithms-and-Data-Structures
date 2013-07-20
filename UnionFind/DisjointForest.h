#include <vector>
#include <cstddef>


using namespace std;
class DisjointForest 
{
  private:
    struct Node     	 
      {
	int rank; //No of elements in this tree
	Node* parent; //Root Node of this Node
	int index; //Id of the Node to identify it 
      };
    
    int numElements;
    int numSets;	
 
   std::vector<Node*> numNodes;
  public:
	int findSet(int elementid) const;
	void unionSet(int set1, int set2);
	int getNumElements() const;	
	int getNumSets() const;	
	//Adds the elements with their ids going from existing id to count
	void addElements(int count);

	DisjointForest();
	//Adds the Elements
	DisjointForest(int count);
	DisjointForest(const DisjointForest& s);
	~DisjointForest();
};



