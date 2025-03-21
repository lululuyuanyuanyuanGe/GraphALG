/*
 * Graph algorithms.
 *
 * Author (of starter code): A. Tafliovich.
 */

#include <limits.h>

#include "graph.h"
#include "minheap.h"

#define NOTHING -1
#define DEBUG 0

typedef struct records {
  int numVertices;    // total number of vertices in the graph
                      // vertex IDs are 0, 1, ..., numVertices-1
  MinHeap* heap;      // priority queue
  bool* finished;     // finished[id] is true iff vertex id is finished
                      //   i.e. no longer in the PQ
  int* predecessors;  // predecessors[id] is the predecessor of vertex id
  Edge* tree;         // keeps edges for the resulting tree
  int numTreeEdges;   // current number of edges in mst
} Records;

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/* Creates, populates, and returns all records needed to run Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
Records* initRecords(Graph* graph, int startVertex) {
  Records* newRecords = (Records*)malloc(sizeof(Records));
  if (newRecords == NULL) {
    printf("Error: Memory allocation failed for new records\n");
    exit(1);
  }

  newRecords->numVertices = graph->numVertices;
  newRecords
};

/* Creates, populates, and returns a MinHeap to be used by Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
MinHeap* initHeap(Graph* graph, int startVertex) {
  MinHeap* minHeap = newHeap(graph->numVertices);
  
  // Define "infinity" as a very large value
  int INF = INT_MAX;
  
  // Insert all vertices into the heap
  // Start vertex gets priority 0, all others get "infinity"
  for (int i = 0; i < graph->numVertices; i++) {
    // Skip vertices that don't exist in the graph
    if (graph->vertices[i] == NULL)
      continue;
      
    if (i == startVertex) {
      // Start vertex gets priority 0
      insert(minHeap, 0, i);
    } else {
      // All other vertices get "infinity" priority
      insert(minHeap, INF, i);
    }
  }
  
  return minHeap;
}

/* Returns true iff 'heap' is NULL or is empty. */
bool isEmpty(MinHeap* heap) {
  return (heap == NULL || heap->size == 0);
}

/* Prints the status of all current algorithm data: good for debugging. */
void printRecords(Records* records);

/* Add a new edge to records at index ind. */
void addTreeEdge(Records* records, int ind, int fromVertex, int toVertex,
                 int weight);

/* Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */
EdgeList* makePath(Edge* distTree, int vertex, int startVertex);

/*************************************************************************
 ** Required functions
 *************************************************************************/


/*************************************************************************
 ** Provided helper functions -- part of starter code to help you debug!
 *************************************************************************/
void printRecords(Records* records) {
  if (records == NULL) return;

  int numVertices = records->numVertices;
  printf("Reporting on algorithm's records on %d vertices...\n", numVertices);

  printf("The PQ is:\n");
  printHeap(records->heap);

  printf("The finished array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->finished[i]);

  printf("The predecessors array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->predecessors[i]);

  printf("The TREE edges are:\n");
  for (int i = 0; i < records->numTreeEdges; i++) printEdge(&records->tree[i]);

  printf("... done.\n");
}
