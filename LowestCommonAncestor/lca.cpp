/* Preprocessing Time - O(nlogn); Query - O(logn) */

// P[i][j] - It represents the 2^j ancestor of i.

#include <iostream>
#include <cmath>

#define MAXN 100

#define logmaxn 7

int P[MAXN][logmaxn];
int Levels[MAXN];

using namespace std;

void preprocess(int N, int* T)
{                     		
  int i,j;                        
  //Initialize all the ancestors as -1
  for(i = 0; i < N; i++)
    for(j = 0; /*Till 2^j is less than N*/ 1<<j < N;j++)
	P[i][j] = -1;

   //If j = 0; then their ancestors are the original ones in T
   for(i=0;i<N;i++)
     P[i][0] = T[i];	
  

   //Compute Ancestors Bottom up using DP
   for(j=1; 1<<j <N; j++) {
     for(i=0; i < N; i++)  {
	if(P[i][j-1] != -1) /*If it has a j-1 ancestor*/ 
	 {
	  P[i][j] = P[P[i][j-1]-1][j-1];		
	 }
      }
    }

}			

int query(int N, int* T, int node1, int node2)
{
  //Assume node1 is bottom to node. If not swap
  int tmp;
  int logg;


  if(Levels[node1] < Levels[node2])   	   
  {
    tmp = node1;
    node1 = node2;
    node2 = tmp;
  } 	

  //Node1 is lower. First Get how many 2^j level it is from the root
  for(logg = 1; 1<<logg <= Levels[node1]; logg++);
  
  logg--; //Crosses One Level,so reduce

  //Now we Know what level node1  in 2^j format
  
 
  //We find the ancestor of node1 situtuated on the same level as node2 first
  int i;
  for(i = logg; i>=0 ; 	i--)
    if((Levels[node1] - (1<<i)) >= Levels[node2]) {
	node1 = P[node1][i];
    }


  //If node1 and node2 are same; 
  if(T[node1] == T[node2])
    return T[node1];


  //Just work your way up till their parents match. Previous step was required to ensure that node2 is not ancestor of node1. 
  for(i = logg; i>=0; i--) {
   std::cout <<  P[node1][i] << P[node2][i] << std::endl;
   if(P[node1][i] != -1 /*Ancestor Exists*/ && P[node1][i] != P[node2][i] /*Only if 2^i was successful.then change*/) 
    {   
  	node1 = P[node1][i];
	node2 = P[node2][i];
    }
  }
 
  return T[node1]; //Remember we don't stop at the common ancestor,just a level below that

}    



/*
        1
      /  \
     2    3
    / \   / \ 
   4   5  6 7

T[] = {0,1,1,2,2,3,3}	
*/
int main()
{
  int no;
  cout << "Enter the no of Nodes:" << endl;
  cin >> no;
  cout << "Tree in depth first manner.  Enter their parents. Root Node's parent is -1. Nodes at Level 1 will have parent as root node i.e 1 so their entry is 1. " << endl;

  int* T = new int[no];
  for(int i=0;i<no;i++)
    cin >> T[i];

  cout << "Enter the corresponding levels.Root starts with Level 1" << endl;
  for(int i=0;i<no;i++)
   cin >> Levels[i];

  preprocess(no,T);  

  //Get the LCA of node 
  cout << "Enter two node whose LCA is to be found:" << endl;
  int n1,n2;
  cin >> n1 >> n2;
  cout << "LCA is Node:" << query(no,T,n1-1,n2-1) << endl;
  return 0;
}
