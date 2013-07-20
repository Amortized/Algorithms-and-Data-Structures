

#include <iostream>
#include "DisjointForest.h"

using namespace std;

void printElements(const DisjointForest& s)
{
  for(int i=0;i<s.getNumElements();i++) 
     cout << "i:" << i << "Parent:" << s.findSet(i) << endl;
  cout << endl;
}

int main()
{
  DisjointForest s(10);
  printElements(s);
  s.unionSet(s.findSet(5),s.findSet(8));
  printElements(s);
  s.unionSet(s.findSet(3),s.findSet(8));
  printElements(s);
  s.addElements(3);
  s.unionSet(s.findSet(11),s.findSet(8));
  printElements(s);
return 0;
}
