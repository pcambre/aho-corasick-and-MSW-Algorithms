#include "AhoCorasickNode.h"


#ifndef AHOCORASICKGRAPH_H_
#define AHOCORASICKGRAPH_H_

class AhoCorasickGraph {
	public:
        int* founds;
        AhoCorasickNode* root;
        AhoCorasickNode* currentPosition;
        //Podria usarse estos atributos para evitar un post procesamiento
        //int* found;
        //int patterns;
		AhoCorasickGraph();
		~AhoCorasickGraph();
    
		//agrega patrones a la estructura inicial de AC
		AhoCorasickNode* addPattern(const char* word, int wordIndex);
    
        //Crea la funcion de fallos
		void setFailTransitions(int numberOfPatterns);
    
        //Dado un  bloque de caracteres leido desde el corpus
        //mueve el puntero actual dentro de la estrucutra de AC
		void updateGraph(char block[], size_t bytCount);
};

#endif /* AHOCORASICKGRAPH_H_ */
