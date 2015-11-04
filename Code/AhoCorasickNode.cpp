

#include "AhoCorasickNode.h"

AhoCorasickNode::AhoCorasickNode() {
    outputcount = 0;
    int alphabetIndex = 0;
    for (; alphabetIndex < ALPHABET_SIZE; alphabetIndex++) {
        children[alphabetIndex] = NULL;
    }
    fail = NULL;
    output = NULL;
}

AhoCorasickNode::~AhoCorasickNode() {
    for (int alphabetIndex = 0; alphabetIndex < ALPHABET_SIZE; alphabetIndex++) {
        AhoCorasickNode *child = children[alphabetIndex];
        if (child && child->outputcount != -1) {
            delete(child);
            child = NULL;
        }
    }
    fail = NULL;
    delete [] output;
    output = NULL;
}

bool AhoCorasickNode::isItemInOutput(int wordIndex) {
    if (outputcount == 0) {
        return false;
    }
    int index = 0;
    while (index < outputcount && output[index] != wordIndex) {
        index++;
    }
    return index < outputcount ;
}

void AhoCorasickNode::addOutput(int wordIndex) {
    if (!isItemInOutput(wordIndex)) {
        outputcount++;
        int *temp = (int*)realloc(output, outputcount * sizeof(int));
        output = temp;
        output[outputcount - 1] = wordIndex;
    }
}

void AhoCorasickNode::outputUnion() {
    int count = fail->outputcount;
    int i = 0;
    for (; i < count; i++) {
        addOutput(fail->output[i]);
    }
}