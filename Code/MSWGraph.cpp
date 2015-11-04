#include "MSWGraph.h"
#ifdef KDEBUG
    #include <assert.h>
#endif
MSWGraph::MSWGraph() {
    root = new MSWNode();
    root->SetAsRootNode();
    currentPosition = root;
}

MSWGraph::~MSWGraph() {
    delete(root);
}

MSWNode * MSWGraph::addPattern(const char* word, int wordLength) {
    MSWNode *aux = root;
    return aux->MSWGraphInsertNode(word, wordLength);
}

void MSWGraph::scanValue(char block[], size_t bytCount) {
    for (Uint blockIndex = 0; blockIndex < bytCount; blockIndex++) {
        currentPosition = currentPosition->transitions[ordChar[(int)block[blockIndex]]];
        currentPosition->count++;
    }
}

void MSWGraph::MakeFSM() {
    for (int alphabetIndex = 0; alphabetIndex < ALPHABET_SIZE; alphabetIndex++) {
        root->traversed[alphabetIndex] = true;
        if (root->children[alphabetIndex]) {
            fastMode = false;
            Verify(root->children[alphabetIndex]);
        }
    }
    MSWNode *aux = root;
    MSWNode *F[ALPHABET_SIZE];
    for (int alphabetIndex = 0; alphabetIndex < ALPHABET_SIZE; alphabetIndex++) {
        F[alphabetIndex] = root;
    }
    PropagateTransitions(root, F);
    root = aux;
}

void MSWGraph::PropagateTransitions(MSWNode *w, MSWNode* F [ALPHABET_SIZE]) {
    for (int alphabetIndex = 0; alphabetIndex < ALPHABET_SIZE; alphabetIndex++) {
        if (!w->transitions[alphabetIndex]) {
            w->transitions[alphabetIndex] = F[alphabetIndex];
        }
    }
    for (int alphabetIndex = 0; alphabetIndex < ALPHABET_SIZE; alphabetIndex++) {
        MSWNode *child = w->children[alphabetIndex];
        if (child) {
            PropagateTransitions(child, w->transitions);
        }
    }
}

void MSWGraph::Verify(MSWNode *w) {
    char head = w->head;
    //busco el tail
    MSWNode *xTail;
    
    Uint nodeLength = w->subsequenceLength;
    MSWNode *rnode = w->father->tail;
    Uint currentNodeWordIndex;
    !w->father->subsequence ? currentNodeWordIndex = 1 : currentNodeWordIndex = 0;
#ifdef KDEBUG
    assert(rnode);
    assert(w->subsequence);
#endif
    MSWNode *vnode = NULL;
    MSWNode *unode = NULL;
    MSWNode *child;
    //El primer caracter estoy seguro que coincide por GetSonByIndex
    Uint childNodeWordIndex = 1;
    Uint childLength;
    //Este if se coloca fuera del while por motivos de eficiencia
    if (fastMode) {
        while (currentNodeWordIndex < nodeLength && rnode) {
            child = rnode->children[ordChar[(int) w->subsequence[currentNodeWordIndex]]];
            if (child) {
                childLength = child->subsequenceLength;
                if (childLength <= (nodeLength - currentNodeWordIndex)) {
                    currentNodeWordIndex += childLength;
                    rnode = child;
                    continue;
                }
                unode = new MSWNode();
                unode->subsequence = child->subsequence;
                unode->subsequenceLength = nodeLength - currentNodeWordIndex;
                xTail = unode;
                break;
            }
            //En este caso se cae directamente desde "r", es decir no hay nodo u
            else {
                vnode = new MSWNode();
                #ifdef KDEBUG
                    assert(w->subsequenceLength - currentNodeWordIndex > 0);
                #endif
                vnode->subsequence = w->subsequence + currentNodeWordIndex;
                vnode->subsequenceLength = nodeLength - currentNodeWordIndex;
                xTail = vnode;
                break;
            }
        }
    }
    else {
        while (currentNodeWordIndex < nodeLength && rnode) {
            child = rnode->children[ordChar[(int) w->subsequence[currentNodeWordIndex]]];
            if (child) {
                currentNodeWordIndex++;
                //Cambiar con el length
                while (childNodeWordIndex < child->subsequenceLength && currentNodeWordIndex < w->subsequenceLength
                      && child->subsequence[childNodeWordIndex] == w->subsequence[currentNodeWordIndex]) {
                    childNodeWordIndex++; currentNodeWordIndex++;
                }
                //Existe una diferencia entre el nodo child y el que se esta comparando,
                //pero aun restan elementos en "child". En este caso tengo que splitear
                if (childNodeWordIndex < child->subsequenceLength) {
                    unode = new MSWNode();
                    unode->subsequence = child->subsequence;
                    unode->subsequenceLength = childNodeWordIndex;
                    //Verifico si hay una parte de v
                    if (currentNodeWordIndex < nodeLength) {
                        vnode = new MSWNode();
                        vnode->subsequence = w->subsequence + currentNodeWordIndex;
                        vnode->subsequenceLength = nodeLength - currentNodeWordIndex;
                        xTail = vnode;
                    }
                    else {
                        xTail = unode;
                    }
                    break;
                }
                rnode = child;
            }
            //En este caso se cae directamente desde "r", es decir no hay nodo u
            else {
                vnode = new MSWNode();
                #ifdef KDEBUG
                    assert(w->subsequenceLength - currentNodeWordIndex > 0);
                #endif
                vnode->subsequence = w->subsequence + currentNodeWordIndex;
                vnode->subsequenceLength = nodeLength - currentNodeWordIndex;
                xTail = vnode;
                break;
            }
        }
    }
    if (unode || vnode) {
        FSMInsert(rnode, unode, vnode);
        if (unode) {
            if (rnode->traversed[ordChar[(int)unode->subsequence[0]]]) {
                fastMode = true;
                Verify(unode);
            }
        }
        else {
            //vnode no es necesario preguntar
            if (rnode->traversed[ordChar[(int)vnode->subsequence[0]]]) {
                fastMode = false;
                Verify(vnode);
            }
        }
    }
    else { //(!unode && !vnode)
        //En este caso toda la palabra ya se encuentra en el arbol en el nodo rnode
        xTail = rnode;
    }
#ifdef KDEBUG
    assert(xTail);
#endif
    w->tail = xTail;
    xTail->transitions[ordChar[(int)head]] = w;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (!w->traversed[i]) {
            w->traversed[i] = true;
            MSWNode* waz = w->children[i];
            if (waz) {
                fastMode = false;
                Verify(waz);
            }
        }
    }
}


void MSWGraph::FSMInsert(MSWNode *r, MSWNode *u, MSWNode *v) {
    if (!u || !u->subsequence) {
        v->father = r;
        v->head = r->subsequence ? r->head : v->subsequence[0];
        r->children[ordChar[(int)v->subsequence[0]]] = v;
    }
    else {
        MSWNode *ruy = r->children[ordChar[(int)u->subsequence[0]]];
        if (ruy->subsequenceLength > u->subsequenceLength) {
            ruy->subsequence = ruy->subsequence + u->subsequenceLength ;
            ruy->subsequenceLength = ruy->subsequenceLength - u->subsequenceLength;
            ruy->father = u;
        }
        u->children[ordChar[(int)ruy->subsequence[0]]] = ruy;
        u->father = r;
        u->head = r->subsequence ? r->head : u->subsequence[0];
        r->children[ordChar[(int)u->subsequence[0]]] = u;
        u->traversed[ordChar[(int)ruy->subsequence[0]]] = r->traversed[ordChar[(int)u->subsequence[0]]];
        if (v && v->subsequence) {
            u->children[ordChar[(int)v->subsequence[0]]] = v;
            v->father = u;
            v->head = u->subsequence ? u->head : v->subsequence[0];
        }
    }
}


//Este metodo debe ser llamado una vez se recorrio completamente el corpus de busqueda
//Se espera que todos los tail sean no nulos, por lo que se marcara con tail nulo aquel nodo que ha sido procesado.
//El proceso suma todas las ocurrencias de los child de los nodos y pone la suma en el nodo padre (de forma recursiva)
int MSWGraph::countMatches(MSWNode *node) {
    int patMach = node->count;
    if (!node->tail) {
        return patMach;
    }
    for (int alphabetIndex = 0; alphabetIndex < ALPHABET_SIZE; alphabetIndex++) {
        MSWNode* child = node->children[alphabetIndex];
        if (child) {
            patMach+= countMatches(child);
        }
    }
    node->count = patMach;
    node->tail = NULL;
    return patMach;
}


#ifdef KDEBUG

void MSWGraph::PrintCurrentNode() {
    currentPosition->PrintNodeInfo(true);
}

void MSWGraph::PrintAllTree() {
    PrintTree(root);
}

void MSWGraph::PrintTree(MSWNode * patNode) {
    patNode->PrintNodeInfo(true);
    for(int i = 0; i < ALPHABET_SIZE ; i ++ ) {
        MSWNode* child = patNode->children[i];
        if(child)
            PrintTree(child);
    }
}

//Chequeo de tails

void MSWGraph::TestAllTails() {
    TestTails(root);
}

void MSWGraph::TestTails(MSWNode * patNode) {
    for(int i = 0; i < ALPHABET_SIZE ; i ++ ) {
        MSWNode* child = patNode->children[i];
        if(child) {
            TailsVerify(child);
        }
    }
}

void MSWGraph::TailsVerify(MSWNode* nodeToTest) {
    MSWNode* iterNode = nodeToTest;
    Uint nodeTotalLength = 0;
    while (iterNode->subsequence) {
        nodeTotalLength += iterNode->subsequenceLength;
        iterNode = iterNode->father;
    }
    MSWNode* iterTailNode = nodeToTest->tail;
    Uint tailTotalLength = 0;
    while (iterTailNode->subsequence) {
        tailTotalLength += iterTailNode->subsequenceLength;
        iterTailNode = iterTailNode->father;
    }
    assert(nodeTotalLength == tailTotalLength + 1);
    Uint index = tailTotalLength;
    for(; index != 0; index -- ) {
        assert(nodeToTest->tail->subsequence - index == nodeToTest->subsequence - index);
    }
}
#endif