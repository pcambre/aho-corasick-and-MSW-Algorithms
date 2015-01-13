/*
 * MSWNode.h
 *
 *      Author: pcambre
 */

#ifndef MSWNode_H_
#define MSWNode_H_

#include "Const.h"
#include "types.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

class MSWNode {
    
public:
    //Posible mejora para reducir los fallos de memoria cache.
//    struct MergedInfo{
//        bool traversed;
//        MSWNode *child;
//        MSWNode* transition;
//    };
//    MergedInfo *info [ALPHABET_SIZE];
    /**
     * Atributos
     */
    char head;
    const char* subsequence;
    Uint subsequenceLength;
    // Podrian juntarse estos tres arreglos en uno solo con un struct adecuado,
    // eso podria ayudar a reducir los cache miss. En la practica, el efecto que produjo
    // fue el contrario
    bool traversed [ALPHABET_SIZE];
    MSWNode *children [ALPHABET_SIZE];
    MSWNode* transitions [ALPHABET_SIZE];
    
    //Elementos auxiliares para la creacion de la FSM y su recorrida posterior
    MSWNode *father;
    MSWNode *tail;
    
    //cuenta la cantidad de pasadas por el nodo, esta informacion se utilizara luego para contar apariciones de patrones
    Uint count;
    
    MSWNode();
    ~MSWNode();
    
    void LightNode();
    
    //Agrega un patron al arbol MSWGraph inicial.
    MSWNode* MSWGraphInsertNode(const char* pattern, Uint patternLength);

    //Indica que el nodo es el nodo root
    void SetAsRootNode();

    //Debug
#ifdef KDEBUG
    void PrintNodeInfo(bool fullInfo);
    int nodeId;
#endif
};
#endif /* MSWNode_H_ */