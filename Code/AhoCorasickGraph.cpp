
#include "AhoCorasickGraph.h"
#include "types.h"
#include "Queue.h"
#include "Const.h"
#include <stdio.h>
#include <iostream>

AhoCorasickGraph::AhoCorasickGraph(){
    root = new AhoCorasickNode();
    //Por AC75 el root debe tener todos los goto completados  i.e fail = null
    root->fail = root;
    currentPosition = root;
    //Esto distingue el nodo root del resto
    root->outputcount = -1;
}

AhoCorasickGraph::~AhoCorasickGraph(){
    delete(root);
    delete [] founds;
}

AhoCorasickNode* AhoCorasickGraph::addPattern(const char* word, int wordIndex) {
    AhoCorasickNode* currentNode = root;
    AhoCorasickNode* child = NULL;
    int index = 0;
    int aux;
    while (word[index] != '\0') {
        aux = ordChar[(int) word[index]];
        child = currentNode->children[aux];
        if (!child) {
            child = new AhoCorasickNode();
            currentNode->children[aux] = child;
        }
        currentNode = child;
        index++;
    }
    currentNode->addOutput(wordIndex);
    return currentNode;
}

//Implementaci—n de AC-Fail
void AhoCorasickGraph::setFailTransitions(int numberOfPatterns) {
    founds = (int*)calloc(numberOfPatterns, sizeof(int));
    Queue* queue = new Queue();
    //1) f(s) <- 0 para todo s con profundidad 1
    AhoCorasickNode* child = NULL;
    AhoCorasickNode* state = NULL;
    int currentSymbol;
    for (int childIndex = 0; childIndex < ALPHABET_SIZE; childIndex++) {
        currentSymbol = (int) ordChar[(int)alphabet[childIndex]];
        child = root->children[currentSymbol];
        if (child) {
            child->fail = root;
            queue->addNode(child);
        }
        else {
            root->children[currentSymbol] = root;
        }
    }
    //2) se configuran failure en los estados con profundidad d>1
    while (!queue->isEmpty()) {
        AhoCorasickNode* r = queue->getNextNode();
        for (int childIndex = 0; childIndex < ALPHABET_SIZE; childIndex++) {
            currentSymbol = (int) ordChar[(int)alphabet[childIndex]];
            child = r->children[currentSymbol];
            if (child) {
                queue->addNode(child);
                state = r->fail;
                while (!state->children[currentSymbol]) {
                    state = state->fail;
                }
                child->fail = state->children[currentSymbol];
                child->outputUnion();
            }
        }
    }
    delete queue;
}

void AhoCorasickGraph::updateGraph(char block[], size_t bytCount) {
    int alphabetIndex;
    for (Uint blockIndex = 0; blockIndex < bytCount; blockIndex++) {
    #ifdef KDEBUG
        assert(currentPosition);
    #endif
        alphabetIndex = ordChar[(int)block[blockIndex]];
        AhoCorasickNode* nextNode = currentPosition->children[alphabetIndex];
        while (!nextNode) {
            nextNode = currentPosition->fail;
            currentPosition = nextNode;
            nextNode = currentPosition->children[alphabetIndex];
        }
        currentPosition = nextNode;
        int oLength = currentPosition->outputcount;
        for (int outputIndex = 0; outputIndex < oLength; outputIndex++) {
            founds[currentPosition->output[outputIndex]] += 1;
        }
    }
}