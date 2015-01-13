
#include "Queue.h"

Queue::Queue() {
	firstNode = NULL;
	lastNode = NULL;
}

//Destruyo la cola sin remover los items (pues estos estan en la estructura arborecente y deben ser removidos desde ahi)
Queue::~Queue() {
	QueueNode *aux = NULL;
	while (firstNode) {
		aux = firstNode->nextNode;
		delete(firstNode);
		firstNode = aux;
	}
}

void Queue::addNode(AhoCorasickNode *node) {
#ifdef KDEBUG
	assert(node);
#endif
	QueueNode *qn = new QueueNode(node);
	if (lastNode) {
		lastNode->nextNode = qn;
	}
	lastNode = qn;
	if (!firstNode) {
		firstNode = qn;
	}
}