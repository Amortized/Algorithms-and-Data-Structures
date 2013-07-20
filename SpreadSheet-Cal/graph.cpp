

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include <queue>

using namespace std;

#define MAXV            100             /* maximum number of vertices */

/*      DFS edge types          */

#define TREE            0               /* tree edge */
#define BACK            1               /* back edge */
#define CROSS           2               /* cross edge */
#define FORWARD         3               /* forward edge */

typedef struct edgenode {
        int y;                          /* adjancency info */
        int weight;                     /* edge weight, if any */
        struct edgenode *next;          /* next edge in list */
} edgenode;

typedef struct {
        edgenode *edges[MAXV+1];        /* adjacency info */
        int degree[MAXV+1];             /* outdegree of each vertex */
        int nvertices;                  /* number of vertices in the graph */
        int nedges;                     /* number of edges in the graph */
        int directed;                   /* is the graph directed? */
} graph;




void initialize_graph(graph *g, bool directed)
{
	int i;				/* counter */

	g -> nvertices = 0;
	g -> nedges = 0;
	g -> directed = directed;

	for (i=1; i<=MAXV; i++) g->degree[i] = 0;
	for (i=1; i<=MAXV; i++) g->edges[i] = NULL;
}


void insert_edge(graph *g, int x, int y, bool directed)
{
	edgenode *p;			/* temporary pointer */
       p = (edgenode*)malloc(sizeof(edgenode));	/* allocate storage for edgenode */

	p->weight = 0;
	p->y = y;
	p->next = g->edges[x];

	g->edges[x] = p;		/* insert at head of list */

	g->degree[x] ++;

	if (directed == false)
		insert_edge(g,y,x,true);
	else
		g->nedges ++;
}

void stringSplitter(string line,vector<string>& s)
{
	istringstream iss(line);
    do
    {
        string sub;
        iss >> sub;
        s.push_back(sub);
    } while (iss);
}

float stringToFloat(string s)
{
	float result;
	stringstream(s) >> result;
	return result;
}



int convertToRow(char c)
{
	switch(c)
	{
	case 'A': return 1; break;
	case 'B': return 2; break;
	case 'C': return 3; break;
	case 'D': return 4; break;
	case 'E': return 5; break;
	case 'F': return 6; break;
	case 'G': return 7; break;
	case 'H': return 8; break;
	case 'I': return 9; break;
	case 'J': return 10; break;
	case 'K': return 11; break;
	case 'L': return 12; break;
	case 'M': return 13; break;
	case 'N': return 14; break;
	case 'O': return 15; break;
	case 'P': return 16; break;
	case 'Q': return 17; break;
	case 'R': return 18; break;
	case 'S': return 19; break;
	case 'T': return 20; break;
	case 'U': return 21; break;
	case 'V': return 22; break;
	case 'W': return 23; break;
	case 'X': return 24; break;
	case 'Y': return 25; break;
	case 'Z': return 26; break;
	}

}

int stringToInt(string s)
{
	int result;
	stringstream(s) >> result;
	return result;
}



void read_graph(graph *g, bool directed, const vector< vector<string> > &spreadsheet)
{
        int i;                          /* counter */
        int m;                          /* number of edges */
        int x, y;                       /* vertices in edge (x,y) */

        initialize_graph(g, directed);

        //No of vertices
        g->nvertices = spreadsheet.size() * spreadsheet[0].size();
        //No of edges

        int row_temp = 0;
        int col_temp = 0;

	    for(int i = 0; i < spreadsheet.size(); i++)
	    {
	    	for(int j = 0; j < spreadsheet[i].size(); j++)
	    	{
	    		//If the string contains only numericals then there is no dependency hence no edge;
	    		if (spreadsheet[i][j].find_first_not_of("0123456789 +-/*") == string::npos)
	    		{
	    			continue;
	    		} else
	    		{
	    			//Insert edges
	    			vector<string> temp;
	    			stringSplitter(spreadsheet[i][j], temp);

	    			for(int k = 0; k < temp.size(); k++)
	    			{
	    				if(temp[k].find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos)
	    				{
	    					//Split
	    					row_temp = convertToRow(temp[k].at(0));
	    					col_temp = stringToInt(temp[k].substr(1));
	    					//Insert an edge
	    	                insert_edge(g, ( (i * spreadsheet[0].size()) + (j + 1) ) , ( (spreadsheet[0].size() * row_temp) - (spreadsheet[0].size() - col_temp) ), directed );
	    	                m++;
	    				}
	    			}
	    		}
	    	}
	    }

}

void delete_edge(graph *g, int x, int y, bool directed)
{
	int i;				/* counter */
	edgenode *p, *p_back;		/* temporary pointers */

	p = g->edges[x];
	p_back = NULL;

	while (p != NULL)
		if (p->y == y) {
			g->degree[x] --;
			if (p_back != NULL)
				p_back->next = p->next;
			else
				g->edges[x] = p->next;

			//free(p);

			if (directed == false)
				delete_edge(g,y,x,true);
			else
				g->nedges --;

			return;
		}
		else
			p = p->next;

	printf("Warning: deletion(%d,%d) not found in g.\n",x,y);
}


bool hasIncomingEdges(graph* g, int m)
{
	int i;				/* counter */
	edgenode *p;			/* temporary pointer */

	for (i=1; i<=g->nvertices; i++) {
		p = g->edges[i];
		while (p != NULL) {
			if(p->y == m)
				return true;
			p = p->next;
		}
	}
	return false;

}

void print_graph(graph *g)
{
	int i;				/* counter */
	edgenode *p;			/* temporary pointer */

	for (i=1; i<=g->nvertices; i++) {
		printf("%d: ",i);
		p = g->edges[i];
		while (p != NULL) {
			printf(" %d",p->y);
			p = p->next;
		}
		printf("\n");
	}
}



//Find the Topological sort of this graph
void topSort(graph* g, bool directed, vector<int>& sorted)
{
	set<int> initial;
	for(int i = 1; i<=g->nvertices; i++)
		initial.insert(i);

	//Find vertexes with no incoming edges
	edgenode *p;			/* temporary pointer */
	for (int i=1; i<=g->nvertices; i++) {
			p = g->edges[i];
			while (p != NULL) {
				if(initial.find(p->y) != initial.end())
				{
					initial.erase(p->y);
				}
				p = p->next;
			}
	}

	queue<int> no_inc;
	for(set<int>::iterator it = initial.begin(); it != initial.end(); it++)
	{
		no_inc.push(*it);
	}


    while(!no_inc.empty())
    {
    	int node = no_inc.front();
    	no_inc.pop();
    	sorted.push_back(node);
    	p = g->edges[node];
    	while (p != NULL) {
    		delete_edge(g, node, p->y, directed);

    		if(!hasIncomingEdges(g, p->y))
    		{
    			no_inc.push(p->y);
    		}
    		p = p->next;
    	 }

    }

	if(g->nedges > 0)
	{
		sorted.clear();
		sorted.push_back(-1);
	}

}




