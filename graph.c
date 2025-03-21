/*
 * Our graph implementation.
 *
 * Author: A. Tafliovich.
 */

#include "graph.h"

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/

void printEdge(Edge* edge) {
  if (edge == NULL)
    printf("NULL");
  else
    printf("(%d -- %d, %d)", edge->fromVertex, edge->toVertex, edge->weight);
}

void printEdgeList(EdgeList* head) {
  while (head != NULL) {
    printEdge(head->edge);
    printf(" --> ");
    head = head->next;
  }
  printf("NULL");
}

void printVertex(Vertex* vertex) {
  if (vertex == NULL) {
    printf("NULL");
  } else {
    printf("%d: ", vertex->id);
    printEdgeList(vertex->adjList);
  }
}

void printGraph(Graph* graph) {
  if (graph == NULL) {
    printf("NULL");
    return;
  }
  printf("Number of vertices: %d. Number of edges: %d.\n\n", graph->numVertices,
         graph->numEdges);

  for (int i = 0; i < graph->numVertices; i++) {
    printVertex(graph->vertices[i]);
    printf("\n");
  }
  printf("\n");
}

/*********************************************************************
 ** Required functions
 *********************************************************************/

Edge* newEdge(int fromVertex, int toVertex, int weight) {
  Edge* newEdge = (Edge*)malloc(sizeof(Edge));
  if (newEdge == NULL) {
    printf("Error: Memory allocation failed for new edge\n");
    exit(1);
  }

  newEdge->fromVertex = fromVertex;
  newEdge->toVertex = toVertex;
  newEdge->weight = weight;

  return newEdge;
}

EdgeList* newEdgeList(Edge* edge, EdgeList* next) {
  EdgeList* newEdgeList = (EdgeList*)malloc(sizeof(EdgeList));
  if (newEdgeList == NULL) {
    printf("Error: Memory allocation failed for new edge list\n");
    exit(1);
  }

  newEdgeList->edge = edge;
  newEdgeList->next = next;

  return newEdgeList;
}

Vertex* newVertex(int id, void* value, EdgeList* adjList) {
  Vertex* newVertex = (Vertex*)malloc(sizeof(Vertex));
  if (newVertex == NULL) {
    printf("Error: Memory allocation failed for new vertex\n");
    exit(1);
  }

  newVertex->id = id;
  newVertex->value = value;
  newVertex->adjList = adjList;

  return newVertex;
}

Graph* newGraph(int numVertices) {
  Graph* newGraph = (Graph*)malloc(sizeof(Graph));
  if (newGraph == NULL) {
    printf("Error: Memory allocation failed for new graph\n");
    exit(1);
  }
  
  newGraph->numVertices = numVertices;
  newGraph->numEdges = 0;
  newGraph->vertices = (Vertex**)malloc(numVertices * sizeof(Vertex*));
  
  if (newGraph->vertices == NULL) {
    printf("Error: Memory allocation failed for vertices array\n");
    free(newGraph);
    exit(1);
  }
  
  // Initialize all vertices to NULL
  for (int i = 0; i < numVertices; i++) {
    newGraph->vertices[i] = NULL;
  }
  
  return newGraph;
}

void deleteEdgeList(EdgeList* head) {
  if (head == NULL) {
    return;
  }

  EdgeList* current = head;
  EdgeList* next;
  
  while(current != NULL) {
    next = current->next;
    free(current->edge);
    free(current);
    current = next;
  }
}

void deleteVertex(Vertex* vertex) {
  if (vertex == NULL) {
    return;
  }

  deleteEdgeList(vertex->adjList);
  free(vertex);
}

void deleteGraph(Graph* graph) {
  if (graph == NULL) {
    return;
  }

  // Delete all vertices
  for (int i = 0; i < graph->numVertices; i++) {
    deleteVertex(graph->vertices[i]);
  }
  
  // Free the vertices array
  free(graph->vertices);
  free(graph);
}

