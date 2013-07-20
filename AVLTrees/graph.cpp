

#include <stdio.h>
#include <stdlib.h>

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

void read_graph(graph *g, bool directed)
{
        int i;                          /* counter */
        int m;                          /* number of edges */
        int x, y;                       /* vertices in edge (x,y) */

        initialize_graph(g, directed);

	printf("Enter the no of vertices and no of edges:\n");
        scanf("%d %d",&(g->nvertices),&m);

		
        for (i=1; i<=m; i++) {
		printf("Enter Edge:\n");
                scanf("%d %d",&x,&y);
                insert_edge(g,x,y,directed);
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

			free(p);

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


