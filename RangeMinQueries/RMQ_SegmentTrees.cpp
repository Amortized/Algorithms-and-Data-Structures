/*Uses Segment Trees Algo.  Preprocessing  - O(n), Query - O(logn). Memory - O(n) */
	
// If we want to update a data in array we can do it logn whereas if we use Sparse Table we need to reinitialize which takes nlogn Therefore, just by doing query in log time 
// we can easily use this for dynamic data

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <cstring>

using namespace std;


template<class T>
class SegmentTree
{
private:
  int* M,size;

public:
  SegmentTree(int N)
  {
    //Allocate Memory for the heap	
    int x = (int)(ceil(log2(N))) + 1;
    size = 2 * (int)pow(2,x);
    M = new int[size];
    memset(M,-1,sizeof(M)); //Initialize everyelement to -1	
  }

  void initialize(int node, int start, int end, T* array)
  {

    if(start == end)	
 	M[node] = start;
    else
    {
	int mid = (start+end)/2;
	initialize(2*node, start, mid, array);
	initialize(2*node+1, mid+1, end, array);
	
	if(array[M[2*node]] <= array[M[2*node+1]])	
	  M[node] = M[2*node];
	else
	  M[node] = M[2*node+1];
    }	
  }

  int query(int node, int start, int end, int i, int j, T* array)
  {

     int id1,id2;

     if(i>end || j<start)
	return -1;

     if(start>=i && end<=j) //Only Answer if u r in that range
	return M[node];   //Return Min Index for this range	

     int mid = (start+end)/2;

	     			
     id1 = query(2*node,start,mid,i,j,array); //Get Min for left son
     id2 = query(2*node+1,mid+1,end,i,j,array); //Get Min for right son

     if(id1 == -1)
	return id2;
	
     if(id2 == -1)
	return id1;

     if(array[id1] <= array[id2])
	return id1;
     else
	return id2;
  } 

  void update(int node, int start, int end, int pos, int val,T* array)
  {
     if(start == end) {	
	M[node] = start;
	array[pos] = val;
     }	
     else 
     {
	int mid = (start+end)/2;

	if(pos <= mid)
	  update(node*2,start,mid,pos,val,array);
	else
	  update(node*2+1,mid+1,end,pos,val,array);

	/*Set the New Min Depending on which child is the min*/
	if(array[M[node*2]] <=  array[M[node*2+1]])
	  M[node] = M[node*2];
	else
	  M[node] = M[node*2+1];	
     }	

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

  cout << "Enter the index to update:" ; 
  int pos; cin >> pos;
  s.update(1,0,no-1,pos,100,a);
	
  for(int i=0;i<no;i++)
    cout << a[i] << " ";
  cout << endl;
  int lowIndex,highIndex;
  cout << "Enter the Indexes:" << endl;
  while(scanf("%d %d",&lowIndex,&highIndex) != EOF)
    cout << a[s.query(1,0,no-1,lowIndex,highIndex,a)] << endl;

  return 0;
}
