#include <iostream>
#include <cmath>

#define MAXN 100

using namespace std;



class RangeMinQuery
{
private:
  int M[MAXN][MAXN];

public:
  /*uses Dynamic Programming. Preprocessing -  O(n^2) , Query - O(1), Memory - O(n^2) */
  void trivialPreprocessing(int* A, int N);

  /*Uses Sparse Table Algo.  Preprocessing  - O(nlogn), Query - O(1). Memory - O(nlogn) */
  void sparseTable(int* A, int N);

  /*Answer Query  : Index of the min element*/
  int getMinIndexTrivial(int i, int j);

  int getMinIndexSparse(int* A, int i, int j);
};

void RangeMinQuery::trivialPreprocessing(int* A,int N)
{
  //Initialise the Array
  for(int i=0;i<N;i++)
    M[i][i] = i;
 
  //Do Calulcations
  for(int i=0;i<N;i++) 
  {
    for(int j= i+1; j<N; j++)
    {
	if(A[M[i][j-1]] < A[j])
	  M[i][j] = M[i][j-1];
	else
	  M[i][j] = j;
    }
  }
}


void RangeMinQuery::sparseTable(int* A,int N) 
{
  //Initialize the Array
  for(int i=0;i<N;i++)
    M[i][0] = i;

  //Compute from smallest range to largest range
  for(int j=1; 1<<j <= N; j++)
    for(int i=0; i + (1<<j) - 1 < N; i++)
	if(A[M[i][j-1]] < A[M[i + (1<<(j-1))][j-1]])
	  M[i][j] = M[i][j-1];
	else
	 M[i][j] = M[i + (1<<(j-1))][j-1];
  
}


int RangeMinQuery::getMinIndexTrivial(int i, int j)
{
  return M[i][j];

}

int RangeMinQuery::getMinIndexSparse(int* A, int i, int j)
{
  // Get a Chunk Size
  int k = (int)log2(j-i+1);
  int p = 1 << k;
  if(A[M[i][k]] < A[M[j-p+1][k]])
    return M[i][k];
  else
   return M[j-p+1][k];		
}


int main()
{
 int a[MAXN],n;
 cin >> n;
 for(int i=0;i<n;i++)
   cin >> a[i];

 std::cout << "HereL:" << std::endl;
 RangeMinQuery rmq,rmq1;
 rmq.trivialPreprocessing(a,n);
 std::cout<< rmq.getMinIndexTrivial(0,4);

 rmq1.sparseTable(a,n);
 std::cout << rmq1.getMinIndexSparse(a,0,4);

 return 0;
}
