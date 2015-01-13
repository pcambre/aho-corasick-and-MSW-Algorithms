
#include "AhoCorasickNode.h"

#ifndef QUEUENODE_H_
#define QUEUENODE_H_

class QueueNode {
	public:
        AhoCorasickNode* item;
        QueueNode *nextNode;
		QueueNode();
		QueueNode(AhoCorasickNode* itemNode);
		~QueueNode();		
};

#endif /* QUEUENODE_H_ */
