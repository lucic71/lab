#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Util.h"
#include "Graph.h"

TGraphL* createGraphAdjList(int numberOfNodes) {
	TGraphL *g = calloc(1, sizeof(TGraphL));
	g->nn = numberOfNodes;
		
	g->adl = calloc(g->nn, sizeof(TNode*));
	return g;
}

void addEdgeList(TGraphL* graph, int v1, int v2) {
	ATNode first = calloc(1, sizeof(TNode));
	ATNode second = calloc(1, sizeof(TNode));

	first->c = 1;
	second->c = 1;

	first->v = v1;
	second->v = v2;

	first->next = graph->adl[v2];
	second->next = graph->adl[v1];

	graph->adl[v2] = first;
	graph->adl[v1] = second;
}

List* dfsIterative(TGraphL* graph, int s) {
		
	Stack *stack = createList();
	List *dfs = createList();	
	int *visited = calloc(graph->nn, sizeof(int));

	push(stack, s);
	visited[s] = 1;
	
	while(!isStackEmpty(stack)){
		int node = top(stack);
		pop(stack);
		
		for(ATNode listNode = graph->adl[node]; listNode;
			listNode = listNode->next){
			if(!visited[listNode->v]){
				push(stack, listNode->v);
				visited[listNode->v] = 1;
			}
		}

		enqueue(dfs, node);
	}

		
	free(visited);
	destroyStack(stack);
	return dfs;
}

void dfsRecHelper(TGraphL* graph, int* visited, List* path, int s) {

	visited[s] = 1;
	enqueue(path, s);
	for(ATNode current = graph->adl[s]; current;
		current = current->next){
		if(!visited[current->v]){
			dfsRecHelper(graph, visited, path, current->v);
		}
	}	

}

List* dfsRecursive(TGraphL* graph, int s) {
	List *path = createList();
	int *visited = calloc(graph->nn, sizeof(int));
	dfsRecHelper(graph, visited, path, s);
	free(visited);
	return path;
}

List* bfs(TGraphL* graph, int s){
	Queue *queue = createQueue();
	List *path = createList();
	int *visited = calloc(graph->nn, sizeof(int));
		
	enqueue(queue, s);
	visited[s] = 1;

	while(!isQueueEmpty(queue)){
		int node = front(queue);
		dequeue(queue);

		for(ATNode listNode = graph->adl[node]; listNode;
			listNode = listNode->next){
			if(!visited[listNode->v]){
				enqueue(queue, listNode->v);
				visited[listNode->v] = 1;
			}
		}	
		
		enqueue(path, node);
	}

	free(visited);
	destroyQueue(queue);
	return path;
}


void destroyGraphAdjList(TGraphL* graph){
	for(int i = 0; i < graph->nn; i++){
		for(ATNode current = graph->adl[i]; current;){
			ATNode next = current->next;
			free(current);
			current = next;	
		}
	}

	free(graph->adl);
	free(graph);

}

void removeEdgeList(TGraphL* graph, int v1, int v2){
	TNode* it = graph->adl[v1];
	TNode* prev = NULL;
	while(it != NULL && it->v != v2){
		prev = it;
		it = it->next;
	}

	if(it == NULL)return;

	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v1] = it->next;
	free(it);

	it = graph->adl[v2];
	prev = NULL;
	while(it != NULL && it->v != v1){
		prev = it;
		it = it->next;
	}
	if(it == NULL) return;
	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v2] = it->next;

	free(it);
}

void removeNodeList(TGraphL* graph, int v){
	for(int i = 0; i < graph->nn;i++){
		removeEdgeList(graph,v,i);
	}
}
