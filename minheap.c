/*
 * Our min-heap implementation.
 *
 * Author (starter code): A. Tafliovich.
 */

#include "minheap.h"

#define ROOT_INDEX 1
#define NOTHING -1

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/* Returns True if 'maybeIdx' is a valid index in minheap 'heap', and 'heap'
 * stores an element at that index. Returns False otherwise.
 */
bool isValidIndex(MinHeap* heap, int maybeIdx) {
       if (maybeIdx >= ROOT_INDEX && maybeIdx <= heap->size) {
              return true;
       }
       return false;
}

/* Swaps contents of heap->arr[index1] and heap->arr[index2] if both 'index1'
 * and 'index2' are valid indices for minheap 'heap'. Has no effect
 * otherwise.
 */
void swap(MinHeap* heap, int index1, int index2) {
    if (isValidIndex(heap, index1) && isValidIndex(heap, index2)) {
        HeapNode temp = heap->arr[index1];
        heap->arr[index1] = heap->arr[index2];
        heap->arr[index2] = temp;

        heap->indexMap[heap->arr[index1].id] = index1;
        heap->indexMap[heap->arr[index2].id] = index2;
    }
}

/* Returns the index of the parent of a node at index 'nodeIndex' in minheap
 * 'heap', if such exists. Returns NOTHING if there is no such parent.
 */
int parentIdx(MinHeap* heap, int nodeIndex) {
       if (isValidIndex(heap, nodeIndex) && nodeIndex > ROOT_INDEX) {
              return nodeIndex / 2;
       }
       return NOTHING;
}

/* Returns the index of the left child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists. Returns NOTHING if there is no such left
 * child.
 */
int leftIdx(MinHeap* heap, int nodeIndex) {
       int leftIndex = nodeIndex * 2;
       if (isValidIndex(heap, nodeIndex) && leftIndex <= heap->size) {
              return leftIndex;
       }
       return NOTHING;
}

/* Returns the index of the right child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists. Returns NOTHING if there is no such right
 * child.
 */
int rightIdx(MinHeap* heap, int nodeIndex) {
       int rightIndex = nodeIndex * 2 + 1;
       if (isValidIndex(heap, nodeIndex) && rightIndex <= heap->size) {
              return rightIndex;
       }
       return NOTHING;
}

/* Returns node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
HeapNode nodeAt(MinHeap* heap, int nodeIndex) {
       if (isValidIndex(heap, nodeIndex)) {
              return heap->arr[nodeIndex];
       }
       HeapNode invalid = {NOTHING, NOTHING};
       return invalid;
}

/* Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int priorityAt(MinHeap* heap, int nodeIndex) {
       if (isValidIndex(heap, nodeIndex)) {
              return heap->arr[nodeIndex].priority;
       }
       return NOTHING;
}

/* Returns ID of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int idAt(MinHeap* heap, int nodeIndex) {
       if (isValidIndex(heap, nodeIndex)) {
              return heap->arr[nodeIndex].id;
       }
       return NOTHING;
}

/* Returns index of node with ID 'id' in minheap 'heap'.
 * Precondition: 'id' is a valid ID in 'heap'
 *               'heap' is non-empty
 */
int indexOf(MinHeap* heap, int id) {
       if (id >= 0 && id < heap->capacity) {
              return heap->indexMap[id];
       }
       return NOTHING;
}

/* Bubbles up the element newly inserted into minheap 'heap' at index
 * 'nodeIndex', if 'nodeIndex' is a valid index for heap. Has no effect
 * otherwise.
 */
void bubbleUp(MinHeap* heap, int nodeIndex) {
       if (isValidIndex(heap, nodeIndex)) {
              int parentIndex = parentIdx(heap, nodeIndex);
              if (parentIndex != NOTHING && heap->arr[nodeIndex].priority < heap->arr[parentIndex].priority) {
                     swap(heap, nodeIndex, parentIndex);
                     bubbleUp(heap, parentIndex);
              }
       }
}

/* Bubbles down the element at index 'nodeIndex' in minheap 'heap',
 * if it exists. Has no effect otherwise.
 */
void bubbleDownAtIndex(MinHeap* heap, int nodeIndex) {
       if (isValidIndex(heap, nodeIndex)) {
              int leftIndex = leftIdx(heap, nodeIndex);
              int rightIndex = rightIdx(heap, nodeIndex);
              int smallestIndex = nodeIndex;
              
              if (leftIndex != NOTHING && heap->arr[leftIndex].priority < heap->arr[smallestIndex].priority) {
                     smallestIndex = leftIndex;
              }
              if (rightIndex != NOTHING && heap->arr[rightIndex].priority < heap->arr[smallestIndex].priority) {
                     smallestIndex = rightIndex;
              }
              if (smallestIndex != nodeIndex) {
                     swap(heap, nodeIndex, smallestIndex);
                     bubbleDownAtIndex(heap, smallestIndex);
              }
       }
}

/* Bubbles down the element newly inserted into minheap 'heap' at the root,
 * if it exists. Has no effect otherwise.
 */
void bubbleDown(MinHeap* heap) {
       bubbleDownAtIndex(heap, ROOT_INDEX);
}

/*********************************************************************
 * Required functions
 ********************************************************************/


HeapNode getMin(MinHeap* heap) {
       return heap->arr[ROOT_INDEX];
}

HeapNode extractMin(MinHeap* heap) {
       HeapNode min = getMin(heap);
       
       heap->arr[ROOT_INDEX] = heap->arr[heap->size];
       heap->indexMap[heap->arr[ROOT_INDEX].id] = ROOT_INDEX;
       
       heap->size--;
       
       bubbleDown(heap);
       
       return min;
}

void insert(MinHeap* heap, int priority, int id) {

       heap->size++;
       heap->arr[heap->size].priority = priority;
       heap->arr[heap->size].id = id;
       
       heap->indexMap[id] = heap->size;
       
       bubbleUp(heap, heap->size);
}


int getPriority(MinHeap* heap, int id) {
       int index = indexOf(heap, id);
       if (index != NOTHING && isValidIndex(heap, index)) {
              return heap->arr[index].priority;
       }
       return NOTHING;
}


bool decreasePriority(MinHeap* heap, int id, int newPriority) {
       int index = indexOf(heap, id);
       
       if (index != NOTHING && isValidIndex(heap, index) && 
           newPriority < heap->arr[index].priority) {
              
              heap->arr[index].priority = newPriority;
              
              bubbleUp(heap, index);
              
              return true;
       }
       
       return false;
}

MinHeap* newHeap(int capacity) {
       MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
       if (heap == NULL) {
              printf("Error: Memory allocation failed for heap\n");
              exit(1);
       }
       
       heap->size = 0;
       heap->capacity = capacity;
       
       heap->arr = (HeapNode*)malloc((capacity + 1) * sizeof(HeapNode));
       if (heap->arr == NULL) {
              printf("Error: Memory allocation failed for heap array\n");
              free(heap);
              exit(1);
       }
       
       heap->indexMap = (int*)malloc(capacity * sizeof(int));
       if (heap->indexMap == NULL) {
              printf("Error: Memory allocation failed for index map\n");
              free(heap->arr);
              free(heap);
              exit(1);
       }
       
       for (int i = 0; i < capacity; i++) {
              heap->indexMap[i] = NOTHING;
       }
       
       return heap;
}

/* Frees all memory allocated for minheap 'heap'.
 */
void deleteHeap(MinHeap* heap) {
       free(heap->arr);
       free(heap->indexMap);
       free(heap);
}

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/
void printHeap(MinHeap* heap) {
  printf("MinHeap with size: %d\n\tcapacity: %d\n\n", heap->size,
         heap->capacity);
  printf("index: priority [ID]\t ID: index\n");
  for (int i = 0; i < heap->capacity; i++)
    printf("%d: %d [%d]\t\t%d: %d\n", i, priorityAt(heap, i), idAt(heap, i), i,
           indexOf(heap, i));
  printf("%d: %d [%d]\t\t\n", heap->capacity, priorityAt(heap, heap->capacity),
         idAt(heap, heap->capacity));
  printf("\n\n");
}
