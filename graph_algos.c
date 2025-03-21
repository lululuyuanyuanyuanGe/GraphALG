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

// Function prototypes for helper functions
MinHeap* initHeap(Graph* graph, int startVertex);
void printRecords(Records* records);

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

  int numVertices = graph->numVertices;
  newRecords->numVertices = numVertices;
  newRecords->heap = initHeap(graph, startVertex);
  
  newRecords->finished = (bool*)malloc(sizeof(bool) * numVertices);
  if (newRecords->finished == NULL) {
    printf("Error: Memory allocation failed for finished array\n");
    exit(1);
  }

  for (int i = 0; i < numVertices; i++) {
    newRecords->finished[i] = false;
  }
  
  newRecords->predecessors = (int*)malloc(sizeof(int) * numVertices);
  if (newRecords->predecessors == NULL) {
    printf("Error: Memory allocation failed for predecessors array\n");
    exit(1);
  }
    for (int i = 0; i < numVertices; i++) {
    newRecords->predecessors[i] = NOTHING;
  }
  
  newRecords->tree = (Edge*)malloc(sizeof(Edge) * numVertices);
  if (newRecords->tree == NULL) {
    printf("Error: Memory allocation failed for tree array\n");
    exit(1);
  }
  
  newRecords->numTreeEdges = 0;
  
  return newRecords;
}

/* Creates, populates, and returns a MinHeap to be used by Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
MinHeap* initHeap(Graph* graph, int startVertex) {
  MinHeap* minHeap = newHeap(graph->numVertices);
  
  int INF = INT_MAX;
  
  for (int i = 0; i < graph->numVertices; i++) {
    if (graph->vertices[i] == NULL)
      continue;
      
    int vertexId = graph->vertices[i]->id;
      
    if (vertexId == startVertex) {
      insert(minHeap, 0, vertexId);
    } else {
      insert(minHeap, INF, vertexId);
    }
  }
  
  return minHeap;
}

/* Returns true iff 'heap' is NULL or is empty. */
bool isEmpty(MinHeap* heap) {
  return (heap == NULL || heap->size == 0);
}

/* Add a new edge to records at index ind. */
void addTreeEdge(Records* records, int ind, int fromVertex, int toVertex,
                 int weight) {
  // Create edge directly in the tree array
  records->tree[ind].fromVertex = fromVertex;
  records->tree[ind].toVertex = toVertex;
  records->tree[ind].weight = weight;
}

/* Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */
EdgeList* makePath(Edge* distTree, int vertex, int startVertex) {
  // If we're already at the start vertex, return NULL (empty path)
  if (vertex == startVertex) {
    return NULL;
  }
  
  // Find the edge leading to the current vertex
  int i = 0;
  while (distTree[i].toVertex != vertex) {
    i++;
  }
  
  // Create new edge for the path
  Edge* edge = newEdge(distTree[i].fromVertex, distTree[i].toVertex, 
                      distTree[i].weight);
  
  // Recursively build the rest of the path
  EdgeList* rest = makePath(distTree, distTree[i].fromVertex, startVertex);
  
  // Create and return the new edge list
  return newEdgeList(edge, rest);
}

/*************************************************************************
 ** Required functions
 *************************************************************************/
Edge* getMSTprim(Graph* graph, int startVertex) {
  Records* records = initRecords(graph, startVertex);

  while (!isEmpty(records->heap)) {
    HeapNode minNode = extractMin(records->heap);
    int minVertex = minNode.id;

    records->finished[minVertex] = true;

    if (records->predecessors[minVertex] != NOTHING) {
      addTreeEdge(records, records->numTreeEdges, 
      records->predecessors[minVertex], minVertex,
      minNode.priority);
      records->numTreeEdges ++;
    }

    // Defensive programming in case there is a bug in the implementation of the graph.
    Vertex* vertex = graph->vertices[minVertex];
    if (vertex == NULL) {
      fprintf(stderr, "Critical error: Vertex %d from heap is NULL in graph\n", minVertex);
      abort();
    }

    EdgeList* adjList = vertex->adjList;
    while (adjList != NULL) {
      Edge* edge = adjList->edge;
      int toVertex = edge->toVertex;
      int weight = edge->weight;

      if (!records->finished[toVertex] &&
      weight < getPriority(records->heap, toVertex)) {
        decreasePriority(records->heap, toVertex, weight);
        records->predecessors[toVertex] = minVertex;
      }

      adjList = adjList->next;
    }

    if (DEBUG) {
      printf("After processing vertex %d:\n", minVertex);
      printRecords(records);
    }
  }

  Edge* mst = (Edge*)malloc(sizeof(Edge) * records->numTreeEdges);
  for (int i = 0; i < records->numTreeEdges; i++) {
    mst[i] = records->tree[i];
  }

  free(records->finished);
  free(records->predecessors);
  free(records->tree);
  deleteHeap(records->heap);
  free(records);

  return mst;
};

Edge* getDistanceTreeDijkstra(Graph* graph, int startVertex) {
  if (startVertex < 0 || startVertex >= graph->numVertices ||
      graph->vertices[startVertex] == NULL) {
        return NULL;
  }

  Records* records = initRecords(graph, startVertex);

  int* distances = (int*)malloc(sizeof(int) * graph->numVertices);
  if (distances == NULL) {
    printf("Error: Memory allocation failed for distances array\n");
    exit(1);
  }

  for (int i = 0; i < graph->numVertices; i++) {
    distances[i] = INT_MAX;
  }
  distances[startVertex] = 0;

  while (!isEmpty(records->heap)) {
    HeapNode nodeMin = extractMin(records->heap);
    int vertexMin = nodeMin.id;
    int currDis = nodeMin.priority;

    records->finished[vertexMin] = true;

    if (records->predecessors[vertexMin] != NOTHING) {

      int edgeweight = 0;
      
      Vertex* preVertex = graph->vertices[records->predecessors[vertexMin]];
      EdgeList* adjList = preVertex->adjList;

      while(adjList != NULL) {
        if (adjList->edge->toVertex == vertexMin) {
          edgeweight = adjList->edge->weight;
          break;
        }
        adjList = adjList->next;
      }

      addTreeEdge(records, records->numTreeEdges,
      records->predecessors[vertexMin], vertexMin,
      edgeweight);
      records->numTreeEdges++; 
  }

  Vertex* vertex = graph->vertices[vertexMin];
  if (vertex == NULL) {
    fprintf(stderr, "Critical error: Vertex %d from heap is NULL in graph\n", vertexMin);
    abort();
  }

  EdgeList* adjList = vertex->adjList;
  while (adjList != NULL) {
    Edge* edge = adjList->edge;
    int toVertex = edge->toVertex;
    int weight = edge->weight;
    
    int newDist = currDis + weight;

    if(!records->finished[toVertex] &&
       newDist < getPriority(records->heap, toVertex)) {
        
        decreasePriority(records->heap, toVertex, newDist);
        records->predecessors[toVertex] = vertexMin;
        distances[toVertex] = newDist;
       }
    adjList = adjList->next;
  }
  if (DEBUG) {
      printf("After processing vertex %d:\n", vertexMin);
      printRecords(records);
    }
  }
  
  Edge* distTree = (Edge*)malloc(sizeof(Edge) * records->numTreeEdges);
  if (distTree == NULL) {
    printf("Error: Memory allocation failed for distance tree\n");
    exit(1);
  }

  for (int i = 0; i < records->numTreeEdges; i++) {
    distTree[i] = records->tree[i];
  }

  free(distances);
  free(records->finished);
  free(records->predecessors);
  free(records->tree);
  deleteHeap(records->heap);
  free(records);
  
  return distTree;
};

EdgeList** getShortestPaths(Edge* distTree, int numVertices, int startVertex) {
  if (startVertex < 0 || startVertex >= numVertices) {
    return NULL;
  }

  EdgeList** paths = (EdgeList**)malloc(sizeof(EdgeList*) * numVertices);
  if (paths == NULL) {
    printf("Error: Memory allocation failed for paths array\n");
    exit(1);
  }

  for (int i = 0; i < numVertices; i++) {
    if (i == startVertex) {
      paths[i] = NULL;
    } else {
      paths[i] = makePath(distTree, i, startVertex);
      if (paths[i] == NULL && i != startVertex) {
        printf("Warning: No path found from vertex %d to start vertex %d\n", 
               i, startVertex);
      }
    }
  }
  return paths;
};


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