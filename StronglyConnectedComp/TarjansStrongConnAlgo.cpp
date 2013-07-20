/*       Tarjan's Algorithm Based on DFS
  indexes are maintained for each vertex which indicates when they are were 
  decovered during search

  lowlink is the root of strongly connected component
////////////////////////////////////
  1. Start with any Node n
  2. for each adjacent vertex w for Node n
	If that vertex is not discovered yet
	     goto step 1
	     n.lowlink = min(n.lowlink, w.lowlink) //If w or any of its successors goes back to a node behind n then the root needs to go back further 	   
	elseif w is on Stack  //This was Back link
	     n.lowlink = min(n.lowlink ,w.index)  
     
  3. If n.lowlink is same as n.index which means this is the root
	Pop from the Stack till u get 'n'. Thats ur vertices for a Strong Component
	Print it

//////////////////////////////
*/





#include "graph.cpp"
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

int globalIndex = 0;





vector<int> indexes;
vector<int> lowlinks;
vector<int> testIfStack;


stack<int> mystack;

void strongConnect(graph& g,int vertex)
{

   indexes.at(vertex-1) = globalIndex;
   lowlinks.at(vertex-1) = globalIndex;
   globalIndex++;
   mystack.push(vertex); //Push the Vertex
   testIfStack.at(vertex-1) = 1; //To Indicate on the Stack


 
  //Consider Successors for v
  edgenode* p = g.edges[vertex];

  while(p!=NULL) {
    	
    if(indexes.at(p->y - 1) == -1) {
       strongConnect(g, p->y);
       lowlinks.at(vertex-1) = min(lowlinks.at(vertex-1), lowlinks.at(p->y - 1)); 
    } else if ( testIfStack.at(vertex-1) == 1) { //One on the stack
       lowlinks.at(vertex-1) = min(lowlinks.at(vertex-1), indexes.at(p->y - 1));
    } 
   
    p = p->next; //New Vertex
  }
  
  // If vertex is a root node, pop the stack and generate an SCC
  if(indexes.at(vertex-1) == lowlinks.at(vertex-1)) {
    //Create a New SCC
    vector<int> newComp;	
    printf( "--------------New component:\n");
    int w = mystack.top();
    mystack.pop();
    newComp.push_back(w); 	


    while(w!=vertex) {
     w = mystack.top();	
     mystack.pop();
     newComp.push_back(w);	
    }

    //Print a New SCC
    for (int i=0; i<(int)newComp.size(); i++) {
	       
                printf("%d: ", newComp.at(i));
                //Print All the Outgoing Edges for that vertex
                p = g.edges[newComp.at(i)];
	        
                while (p != NULL) {
                        printf(" %d",p->y);
                        p = p->next;
                }
                printf("\n");
     }

  printf("-----------------------");
  }


}


void TarjanAlgo(graph& g) {

  //Initialize
  for(int i=0;i<g.nvertices;i++) {
    indexes.push_back(-1);
    lowlinks.push_back(-1);
    testIfStack.push_back(-1); //Not on Stack
  } 
  
  //Go throu each vertex
  for(int i=0;i<g.nvertices;i++) {
     if(indexes.at(i) == -1)  //If Vertex hasn't been explored yet
     { 
       strongConnect(g,i+1);  //Vertex Nos start from 1,2...
     }
  }

}



int main()
{
  graph g;
  read_graph(&g,true);
  print_graph(&g);
  TarjanAlgo(g);

return 0;
}

