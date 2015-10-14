/*
Implemented by Rahul(rrm333@nyu.edu)

AVL Trees: Self Balancing BST used to implement associative arrays, priority queues , ordered sets
   and mutable lists

Worst case time for insert, delete and get is O(log n)

This is good for lists because avoids linear time access which aids in delete also*/



#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

template<typename Key, typename Value>
class AvlTree {
public:
  struct AvlNode {
    AvlNode(const Key& key, const Value& val) :
     parent_node(NULL), left_node(NULL), right_node(NULL), left_max_depth(0),
     right_max_depth(0), key(key), val(val) {
    } 	
   
    AvlNode* parent_node; 
    AvlNode* left_node;
    AvlNode* right_node;
    int left_max_depth;
    int right_max_depth;		
    Key key;
    Value val;	
  };        

  AvlNode* root_node;
  int size;
  bool doRebalancing;

  AvlTree();
  virtual ~AvlTree();

  //Remove the Node;deallocate the memory all its children 
  void deleteNode(AvlNode*);

  //Balance a Node 
  bool balanceNode(AvlNode*);

  //Insert a Node
  void insert(const Key& key, const Value& val);


  //Remove a Node
  bool remove(const Key& key);

  //Single Left rotate
  void slr(AvlNode*); 
  //Single Right rotate
  void srr(AvlNode*);

  //Lookup a Node
  Value lookupNode(const Key&) const;
  //Contains a Node
  bool contains(const Key&) const;
  //Get the Node
  AvlNode* getNode(const Key&) const;

  //Get the leftMost Node 
  AvlNode* getLeftMostNode(AvlNode* current) const;


  //Propogate new max children size to parents, does balancing
  void propogate_max_children(AvlNode*,AvlNode*,int,bool);

  //Get the Balance 
  int getBalance(AvlNode* node) const {
    return node->right_max_depth - node->left_max_depth;
  }

  //Returns the Max Children Size
  int getMaxChildren(AvlNode* node) const {
    return std::max(node->left_max_depth, node->right_max_depth);
  }

};


template <typename Key,typename Val>
AvlTree<Key,Val>::AvlTree() 
: root_node(NULL), size(0), doRebalancing(true) 
{}

template <typename Key,typename Val>
AvlTree<Key,Val>::~AvlTree()
{
  deleteNode(root_node); 
}


//Single Right Rotate
template <typename Key,typename Val>
void AvlTree<Key,Val>::srr(AvlNode* node) 
{
  
  node->left_node->parent_node = node->parent_node;
  AvlNode* Oldright = node->left_node->right_node;
  
  node->left_node->right_node = node;
  node->parent_node = node->left_node;
  
  node->left_node = Oldright; //Change ; Need to check to adjust weights	
  node->left_max_depth = Oldright ? (getMaxChildren(Oldright)) : 0;  //Adjust left max depth 
   
  if(Oldright)
  {
    Oldright->parent_node = node; //Set the Down link
    node->left_max_depth++; //Increase the Depth 	
  }  
  
  node->parent_node->right_max_depth++; //Because there was a change in branch 


  //Now the final Uplink
  if(node == root_node) {
    root_node = node->parent_node;
  } else 
  {
    if(node->parent_node->parent_node->left_node == node) 
	node->parent_node->parent_node->left_node = node->parent_node;
    else 	
	node->parent_node->parent_node->right_node = node->parent_node;
  }
}


//Single Left Rotate
template <typename Key,typename Val>
void AvlTree<Key,Val>::slr(AvlNode* node)
{
  node->right_node->parent_node = node->parent_node;
  AvlNode* Oldleft = node->right_node->left_node;
  
  node->right_node->left_node = node;
  node->parent_node = node->right_node;

  node->right_node = Oldleft;
  node->right_max_depth = (Oldleft) ? (getMaxChildren(Oldleft)) : 0;

  if(Oldleft) {
    Oldleft->parent_node = node;
    node->right_max_depth++;
  }
  
  node->parent_node->left_max_depth++;

  if(root_node == node) 
    root_node = node->parent_node;
  else {
    if(node->parent_node->parent_node->left_node == node)  
       node->parent_node->parent_node->left_node = node->parent_node;
    else
       node->parent_node->parent_node->right_node = node->parent_node;
  }
}

template <typename Key,typename Val>
Val AvlTree<Key,Val>::lookupNode(const Key& key) const
{
  AvlNode* ret = getNode(key);
  return ret ? ret->val : 0;	
}


template <typename Key,typename Val>
bool AvlTree<Key,Val>::contains(const Key& key) const 
{
  AvlNode* ret = getNode(key);
  return ret ? true : false;
}

template <typename Key,typename Val>
typename AvlTree<Key,Val>::AvlNode* AvlTree<Key,Val>::getNode(const Key& key) const
{
  //Check if root exist
  AvlNode* current = this->root_node;

  if(!current) 
    return 0;

  for(;;) {
  if(current->key == key)
    return current;
  else if(current->key > key) 
  {
    if(current->left_node)	
     current = current->left_node;
    else
     return 0;       	
  }	
  else {
     if(current->right_node)	
	current = current->right_node;
     else
	return 0;
   } 
  }
}


template <typename Key,typename Val>
void AvlTree<Key,Val>::deleteNode(AvlNode* current)
{
  if(!current) 
    return;

  if(current->left_node)
    deleteNode(current->left_node);
  
  if(current->right_node)
    deleteNode(current->right_node);

  this->size--; 

  if(current->parent_node) 
  {
     if(current->parent_node->left_node == current) 
     {
	current->parent_node->left_node = NULL;
	current->parent_node->left_max_depth--;
     } else
     {
        current->parent_node->right_node = NULL;
        current->parent_node->right_max_depth--;     
     }	
  } 
  else 
  {
    root_node = NULL;	 	
  }
    
  delete current;
}


template <typename Key,typename Val>
bool AvlTree<Key,Val>::balanceNode(AvlNode* current)
{
  int balancing = getBalance(current);
  if(balancing < -1) //Left Heavy. Need to Right Rotate
  {
    int childBalance = getBalance(current->left_node);
    if(childBalance <= 1) { //Left Heavy
      slr(current->left_node);
      srr(current);
    }
    else {  
      srr(current); 	
    }

  } else if (balancing > 1) //Right Heavy; Need to left rotate
  {
     int childBalance = getBalance(current->right_node);
     if(childBalance >= 1) {
	srr(current->right_node);
	slr(current);
     } else 
     {	
 	slr(current);
     }
  }
  return true;
}

template <typename Key,typename Val>
void AvlTree<Key,Val>::insert(const Key& key, const Val& val)
{
  AvlNode* current = root_node;
  if(!current) {
     AvlNode* newNode = new AvlNode(key,val);
     root_node = newNode;
     size++;
     return;		
  }  

  for(;;) 
  {
    if(current->key == key) 
    {
       current->val = val;	
       return;	
    }	

    if(current->key > key) {
	if(!current->left_node) {
	  current->left_node = new AvlNode(key,val);	
	  current->left_node->parent_node = current;
	  size++;
	  propogate_max_children(current,current->left_node,0,false);
	  return;
        }
	current = current->left_node;
    } else 
    {
	if(!current->right_node) {
	  current->right_node = new AvlNode(key,val);
	  current->right_node->parent_node = current;
	  size++;
	  propogate_max_children(current,current->right_node,0,false);
	  return;
	}
	current = current->right_node;
    }
  }

}


template <typename Key,typename Val>
bool AvlTree<Key,Val>::remove(const Key& key) 
{
  AvlNode* node = getNode(key);
  if(!node) {
    return false;	
  }
 
  this->size--;

  //Get extra Details
  AvlNode* parent_node = node->parent_node;
  bool isRootNode = node->parent_node ? false : true;
  bool toRightParent = !isRootNode ? (parent_node->right_node == node) : false;
  bool has_left =  node->right_node ? true : false;
  bool has_right = node->left_node ? true : false;

  //Deleted Node has no leaves
  if(!has_left && !has_right) {
     if(!isRootNode) {

	if(toRightParent) {
	   parent_node->right_node = NULL;
	   parent_node->right_max_depth--;
         } else {
	   parent_node->left_node = NULL;
	   parent_node->left_max_depth--;
	 }
     }
	
     delete node;	
	    	
     if(isRootNode) {
	root_node = 0;
	return true;
     }	

  } 

  //Deleted Node has exactly one leaf
  else if ( (has_left && !has_right) || (!has_left && has_right) ) {

    //What leaf it is
    bool detachRight = node->right_node ? true : false;
	
    if( (!toRightParent) && (!isRootNode) ) {
      parent_node->left_node = detachRight ? node->right_node : node->left_node;
      parent_node->left_max_depth = (detachRight ? getMaxChildren(node->right_node) : getMaxChildren(node->left_node)) + 1;	
      	
     } else if(!isRootNode) {
	parent_node->right_node = detachRight ? node->right_node : node->left_node;
	parent_node->right_max_depth = (detachRight ? getMaxChildren(node->right_node) : getMaxChildren(node->left_node)) + 1;
     }	
     	
     
     if(detachRight) {
	node->right_node->parent_node = parent_node;
     }	
     else {	
       node->left_node->parent_node = parent_node;	
     }	

     if(isRootNode) {
	root_node = detachRight ? node->right_node : node->left_node;
	parent_node = root_node;
     }	

    delete node;
  } 
  
  //Deleted Node has both the leaves
  else if ( (has_left && has_right) ) {
	
    AvlNode* leftMost = getLeftMostNode(node->right_node);
    AvlNode* parentleftMost = leftMost->parent_node;

    node->key = leftMost->key;
    node->val = leftMost->val;

    if(node->right_node = leftMost) {
      node->right_node = leftMost->right_node;	
      node->right_max_depth = leftMost->right_node ? leftMost->right_node->right_max_depth+1 : 0;

      if(leftMost->right_node) {
	leftMost->right_node->parent_node = node;
      }			
      parent_node = node; //This Node goes out of balance	
    } else 
    { 			     	
	leftMost->parent_node->left_max_depth = 0;
	leftMost->parent_node->left_node = NULL;
	parent_node = leftMost->parent_node; 
    }

    delete leftMost;
  }

  balanceNode(parent_node); //balance the Node
  parent_node = parent_node->parent_node; //Set the Correct Parent


  //If parent and its father exist 
  if(parent_node && parent_node->parent_node) {
    propogate_max_children(parent_node->parent_node, parent_node, getMaxChildren(parent_node), true);
  }
	
  return true;
}


template <typename Key,typename Val>
typename AvlTree<Key,Val>::AvlNode* AvlTree<Key,Val>::getLeftMostNode(AvlNode* current) const
{
  AvlNode* node = current;
  if(node->left_node) {
    getLeftMostNode(node->left_node);	
  } else {
    return node;	
  }
}


template <typename Key,typename Val>
void AvlTree<Key,Val>::propogate_max_children(AvlNode* notifiedNode,AvlNode* senderNode,int childSize,bool isDeletion)
{
  
  //Adjust right or left depth for parent

  int oldMaxSize = getMaxChildren(notifiedNode);

  int maxSize = childSize + 1;
  if(notifiedNode->right_node == senderNode) 
     notifiedNode->right_max_depth = maxSize;
  else 
     notifiedNode->left_max_depth = maxSize;

  int newMaxSize = getMaxChildren(notifiedNode);
	
  if(balanceNode(notifiedNode)) {
    if(isDeletion)
    {
	notifiedNode = notifiedNode->parent_node;				
    } 
    else
      return; //Insert ends here //As subtree height does not change in case of inserts, one rotation is enough
  } 

  //Balance More
  if(notifiedNode->parent_node) {
     if(isDeletion ? newMaxSize!=oldMaxSize/*isdeletion caused an imbalance*/ : newMaxSize > oldMaxSize/*Insert Caused a imbalance*/)	
	propogate_max_children(notifiedNode->parent_node,notifiedNode,newMaxSize, isDeletion);
  }

}


int main()
{

AvlTree<int, int> k;
for(int i=1;i<100;i++)
k.insert(i,i*10);

for(int i=1;i<100;i++) 
cout << k.lookupNode(i) << endl;

for(int i=100;i<200;i++)
k.insert(i,i*10);


for(int i=1;i<200;i++) 
cout << k.lookupNode(i) << endl;

for(int i=1;i<100;i++)  
k.remove(i);

for(int i=1;i<200;i++)
cout << k.lookupNode(i) << endl;


return 0;
}
