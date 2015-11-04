
#include "QueueNode.h"
#ifndef QUEUE_H_
#define QUEUE_H_

class Queue {
    private:
        QueueNode *firstNode;
        QueueNode *lastNode;
    public :
        Queue();
        ~Queue();
        
        //Funciones inline 
        
        //Retorna un nodo del arbol AC, eliminando el mismo de la cola.
        AhoCorasickNode* getNextNode() {
            AhoCorasickNode* nodeToReturn = firstNode->item;
            QueueNode* aux = firstNode;
            firstNode = firstNode->nextNode;
            if (!firstNode) {
                lastNode = NULL;
            }
            delete(aux);
            return nodeToReturn;
        }

        bool isEmpty() {
            return(!firstNode);
        }
        
        void addNode(AhoCorasickNode *node);

};

#endif /* QUEUE_H_ */
