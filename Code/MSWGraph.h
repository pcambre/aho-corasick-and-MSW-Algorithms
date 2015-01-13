/*
 * MSWGraph.h
 *
 *      Author: pcambre
 */

#include "MSWNode.h"
#include "Const.h"


#ifndef MSWGraph_H_
#define MSWGraph_H_

class MSWGraph {
private:
    bool fastMode;
    
    //Implementa el Verify del paper
    void Verify(MSWNode *w);
    
    //Implementa el PropagateTransitions del paper
    void PropagateTransitions(MSWNode *w, MSWNode* F [ALPHABET_SIZE]);
    
    MSWNode *GetCTTail(MSWNode *currentNode, bool &fastMode , MSWNode **iterNode);
    void FSMInsert(MSWNode *r, MSWNode *u, MSWNode *v);
public:
    MSWNode *root;
    MSWNode *currentPosition;
    
    MSWGraph();
    ~MSWGraph();
    
    
    //MSWNode *GetRoot();
    
    //Agrega un patron nuevo al arbol MSWGraph
    MSWNode * addPattern(const char* word, int wordLength);
    
    //Realiza el MakeFSM del paper
    void MakeFSM();
    
    void scanValue(char block[], size_t bytCount);
    static int countMatches(MSWNode *node);
    
    //metodos para debug
    
#ifdef KDEBUG
    void PrintCurrentNode();
    void PrintTree(MSWNode * patNode);
    void PrintAllTree();
    void TailsVerify(MSWNode* nodeToTest);
    void TestAllTails();
    void TestTails(MSWNode * patNode);
#endif
};

#endif /* MSWGraph_H_ */