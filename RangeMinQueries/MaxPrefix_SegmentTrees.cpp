/*Uses Segment Trees Algo.  Preprocessing  - O(n), Query - O(logn). Memory - O(n) */
	
// If we want to update a data in array we can do it logn whereas if we use Sparse Table we need to reinitialize which takes nlogn Therefore, just by doing query in log time 
// we can easily use this for dynamic data

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <cstring>
#include <vector>
using namespace std;


template<class T>
class SegmentTree
{
private:
  struct Node
  {
    int maxPrefix;
    int totalSum;	
  };
  
  int size;
  vector<Node*> M;

public:
  SegmentTree(int N)
  {
    //Allocate Memory for the heap	
    int x = (int)(ceil(log2(N))) + 1;
    size = 2 * (int)pow(2,x);
    for(int i=0;i<size;i++) { 
     Node* m = new Node();
     m->totalSum = 0;
     m->maxPrefix = 0;
     M.push_back(m);
     //M.at(i) = NULL;
    }
  }

  void initialize(int node, int start, int end, T* array)
  {
    if(start == end) 
    {	
 	M.at(node)->maxPrefix = array[start];
	M.at(node)->totalSum  = array[start]; 
    }
    else
    {
	int mid = (start+end)/2;
	initialize(2*node, start, mid, array);
	initialize(2*node+1, mid+1, end, array);
	
	M.at(node)->totalSum = M.at(node*2)->totalSum + M.at(node*2+1)->totalSum;
	M.at(node)->maxPrefix = max(M.at(node*2)->maxPrefix, M.at(node*2)->totalSum + M.at(node*2+1)->maxPrefix);
    }		
  }

  int query(int node, int start, int end, int i, int j, int* totalSum)
  {
     int id1,id2;

     if(i>end || j<start)
	return -1;

     if(start>=i && end<=j) //Only Answer if u r in that range
      {	
	*totalSum = M.at(node)->totalSum;
	return M.at(node)->maxPrefix;
      }						

     int mid = (start+end)/2;

     int totalSum1,totalSum2;		     			
     id1 = query(2*node,start,mid,i,j,&totalSum1); //Get Min for left son
     id2 = query(2*node+1,mid+1,end,i,j,&totalSum2); //Get Min for right son

     if(id1 == -1)
	return id2;
	
     if(id2 == -1)
	return id1;

     return (max(id1, totalSum1 + id2));
  } 				


};

int main()
{
  int *a,no;
  cin >> no;
  a = new int[no];
  for(int i=0;i<no;i++)
   cin >> a[i];

  SegmentTree<int> s(no);
  s.initialize(1,0,no-1,a);

  int lowIndex,highIndex;
  cout << "Enter the Indexes:" << endl;
  int totalSum = 0;
  while(scanf("%d %d",&lowIndex,&highIndex) != EOF)
    cout << s.query(1,0,no-1,lowIndex,highIndex,&totalSum) << endl;

  return 0;
}
