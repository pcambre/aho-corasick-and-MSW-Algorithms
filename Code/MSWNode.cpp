/*
 * MSWNode.cpp
 *
 *      Author: pcambre
 */

#include "MSWNode.h"
#include <typeinfo>


#ifdef KDEBUG
    static int CantNodes = 0;
#endif

MSWNode::MSWNode() {
    subsequence = "";
    subsequenceLength = 0;
    count = 0;
    //Estructuras auxiliares, podran ser removidas una vez culmine el MakeFSM
    tail = NULL;
    for (int alphabetIndex = 0; alphabetIndex < ALPHABET_SIZE; alphabetIndex++) {
        traversed[alphabetIndex] = false;
        transitions[alphabetIndex] = NULL;
        children[alphabetIndex] = NULL;
    }
    father = NULL;
    //Informacion util unicamente para debug
    #ifdef KDEBUG
        nodeId = CantNodes;
        CantNodes++;
    #endif
}

MSWNode::~MSWNode() {
    int alphabetIndex = 0;
    tail = NULL;
    for (; alphabetIndex < ALPHABET_SIZE; alphabetIndex++) {
        traversed[alphabetIndex] = false;
        transitions[alphabetIndex] = NULL;
    }
    father = NULL;
    alphabetIndex = 0;
    for (; alphabetIndex < ALPHABET_SIZE; alphabetIndex++) {
        MSWNode *child = children[alphabetIndex];
        if (child) {
            delete(child);
            children[alphabetIndex] = NULL;
        }
    }
}

void MSWNode::LightNode() {
    tail = NULL;
    father = NULL;
    int alphabetIndex = 0;
    subsequence = NULL;
    for (; alphabetIndex < ALPHABET_SIZE; alphabetIndex++) {
        MSWNode *child = children[alphabetIndex];
        if (child) {
            child->LightNode();
        }
    }
}

void MSWNode::SetAsRootNode() {
    subsequence = NULL;
    subsequenceLength = 0;
    father = this;
    //Por motivos de implementacion el tail del root debe ser el mismo root
    tail = this;
    for (int alphabetIndex = 0; alphabetIndex < ALPHABET_SIZE; alphabetIndex++) {
        transitions[alphabetIndex] = NULL;
        traversed[alphabetIndex] = false;
    }
}

MSWNode * MSWNode::MSWGraphInsertNode(const char* pattern, Uint patternLength) {
    Uint patternIndex = 0;
    MSWNode *currentNode = this;
    MSWNode *child = children[ordChar[(int)pattern[0]]];
    while (1) {
        if (!child) {
            MSWNode *newChild = new MSWNode();
            newChild->head = pattern[0];
            newChild->subsequence = pattern + patternIndex;
            newChild->subsequenceLength = patternLength - patternIndex;
            newChild->father = currentNode;
            currentNode->children[ordChar[(int)pattern[patternIndex]]] = newChild;
            return newChild;
        }
        else {
            Uint nodeIndex = 0;
            while (patternIndex < patternLength && nodeIndex < child->subsequenceLength
                  && child->subsequence[nodeIndex] == pattern[patternIndex]) {
                patternIndex++;
                nodeIndex++;
            }
            //Hubo un fallo en la comparacion
            if (nodeIndex < child->subsequenceLength) {
                Uint wordRightLength = child->subsequenceLength - nodeIndex;
                const char* trimLeftWord = child->subsequence;
                const char* trimRightWord = child->subsequence + nodeIndex;
                
                MSWNode *father = child->father;
                
                //Antes que nada debo avisarle a mi padre que no sera mas mi padre
                father->children[ordChar[(int)child->subsequence[0]]] = NULL;
                
                child->subsequence = trimRightWord;
                child->subsequenceLength = wordRightLength;
                MSWNode *newChild = new MSWNode();
                newChild->head = pattern[0];
                newChild->subsequence = trimLeftWord;
                newChild->subsequenceLength = nodeIndex;
                newChild->children[ordChar[(int)trimRightWord[0]]] = child;
                newChild->father = father;
                //El padre debe conocer a su nuevo hijo...
                father->children[ordChar[(int)newChild->subsequence[0]]] = newChild;
                
                child->father = newChild;
                
                //Ahora agrego lo que falta del pattern ...
                if (patternIndex < patternLength) {
                    MSWNode *patternRest = new MSWNode();
                    patternRest->head = pattern[0];
                    patternRest->subsequence = pattern + patternIndex;
                    patternRest->subsequenceLength = patternLength - patternIndex;
                    patternRest->father = newChild;
                    newChild->children[ordChar[(int)pattern[patternIndex]]] = patternRest;
                    return patternRest;
                }
                return newChild;
            }
            if (patternIndex == patternLength) {
                return child; //El nodo en cuestion ya esta insertado, retorno el mismo
            }
            currentNode = child;
            child = child->children[ordChar[(int)pattern[patternIndex]]];
        }
    }
}

#ifdef KDEBUG
void MSWNode::PrintNodeInfo(bool fullInfo) {
    printf("\n %s", "==========================  ");
    printf("Informacion del nodo %d", nodeId);
    printf("%s \n", "  ======================");
    printf("El nodo actual tiene %d caracteres \n", subsequenceLength);
    printf("%s\n", "Su secuencia es: ");
    for (Uint i = 0; i < subsequenceLength; i++) {
        printf("==> %c \n", subsequence[i]);
    }
    printf("La cantidad de visitas de este nodo es de: %d \n", count);
    
    if (fullInfo) {
        printf("%s \n", "Sus hijos son: ");
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            MSWNode* child =children[ordChar[(int)alphabet[i]]];
            if (child) {
                printf("=> %c con id %d \n", alphabet[i], child->nodeId);
            }
        }
        MSWNode* patNode = father;
        
        if (patNode && father->subsequence) {
            printf("%s \n", "Su padre es: ");
            printf("=> %c con id %d \n", father->subsequence[0], father->nodeId);
        }
        if (tail && tail->subsequence) {
            printf("%s \n", "Su Tail es: ");
            printf("=> %c con id %d \n", tail->subsequence[0], tail->nodeId);
        }
        printf("%s \n", "Sus transiciones son: ");
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            MSWNode *trans = transitions[i];
            if(trans){
                printf("=> %c con id %d \n", alphabet[i], trans->nodeId);
            }
        }
        printf("%s \n", "Sus traversed son: ");
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (traversed[i]){
                printf("=> %c \n", alphabet[i]);
            }
        }
        
    }
    printf("\n %s \n", "===========================================================");
}
#endif