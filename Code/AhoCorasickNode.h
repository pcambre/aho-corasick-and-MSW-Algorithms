
#ifndef AhoCorasickNode_H_
#define AhoCorasickNode_H_
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include "Const.h"

#include "AhoCorasickNode.h"

class AhoCorasickNode {
	private:
		bool isItemInOutput(int wordIndex);
	public:
        AhoCorasickNode *children [ALPHABET_SIZE];
        AhoCorasickNode *fail;
    // http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/ otra posibilidad seria std::vector<int> output pero esta resulto ser mas costosa en la practica
        int* output;
        int outputcount;
		AhoCorasickNode();
		~AhoCorasickNode();
		void addOutput(int wordIndex);
        void outputUnion();
};

#endif /* AhoCorasickNode_H_ */
