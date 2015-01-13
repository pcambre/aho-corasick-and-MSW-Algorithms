
#include "QueueNode.h"

QueueNode::QueueNode() {
	item = NULL;
	nextNode = NULL;
}

QueueNode::QueueNode(AhoCorasickNode* itemNode) {
	item = itemNode;
	nextNode = NULL;
}

QueueNode::~QueueNode() {
	item = NULL;
	nextNode = NULL;
}