
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/mman.h>
#include "Const.h"
#include "AhoCorasickGraph.h"
#include "MSWGraph.h"
#include <time.h>
#include <string>
#include "Memory.c"
#ifdef KDEBUG
#include "test.h"
#endif
//Argumentos principales
char *fileName = NULL;
char *patternsFileName = NULL;
Uint numberOfPatterns = 0;
int mode = 0;

//Opciones extra
int blockSize = 1024;
bool printPreTime = false;
bool printPostTime = false;
bool printReadTime = false;
bool printBlockReadTime = false;
bool printTotalTime = false;
bool printMemory = false;
bool printFinds = false;
bool printComments = false;


//************************************************ CLOCK *********************************************

static clock_t startclock,
stopclock;

/*
 Esta funcion inicializa el reloj
 */

void initclock(void) {
    startclock = clock();
}

/*
 Esta funcion retorna el tiempo entre la inicializacion 
 del reloj medido en segundos
 */

double getruntime(void) {
    stopclock = clock();
    return (stopclock-startclock) / (double) CLOCKS_PER_SEC;
}

/* Esta funcion retorna los ticks de reloj entre
  \texttt{startclock} to \texttt{stopclock}
 */

Uint getclockticks(void) {
    stopclock = clock();
    return (Uint) (stopclock-startclock);
}

//************************************************ MAIN *********************************************

void ahoCorasick() {
    
    clock_t startPreTime;
    clock_t startReadTime;
    clock_t startBlockTime;
    clock_t startTotalTime;
    
    if (printTotalTime) {
        startTotalTime = clock();
    }
    
    AhoCorasickGraph *acg = new AhoCorasickGraph();
    AhoCorasickNode *patternsNodes [numberOfPatterns];
    
    std::ifstream stream(patternsFileName);
    
    unsigned int patternIndex = 0;
    std::string line;
    
    if (printPreTime) {
        startPreTime = clock();
    }
    while(std::getline( stream, line ) && patternIndex < numberOfPatterns) {
        char *ww = (char*)malloc(sizeof(char) * (line.length() + 1));
        strcpy(ww, line.c_str());
        patternsNodes[patternIndex] = acg->addPattern(ww, patternIndex);
        free(ww);
        patternIndex++;
    }
    
    acg->setFailTransitions(numberOfPatterns);
    if (printPreTime) {
        clock_t finalPreTimeClock = clock();
        float preTimeVal = (finalPreTimeClock - startPreTime) / (double) CLOCKS_PER_SEC;
        if (printComments) {
            printf("# Total pre process time\n");
            printf("%f\n", preTimeVal);
        }
        else {
            printf("%f ", preTimeVal);
        }
    }
    
    //Recorro el archivo
    FILE *sourceFile = fopen(fileName, "r");
    if(sourceFile) {
        long pageSize = sysconf(_SC_PAGE_SIZE) * blockSize;
        char block[pageSize];
        size_t bytCount;
        if (printReadTime) {
            startReadTime = clock();
        }
        if (printBlockReadTime) {
            if (printComments) {
                printf("# Read Block by block Times\n");
            }
            startBlockTime = clock();
            float blockTimeVal = 0;
            clock_t finalBlockTimeClock ;
            while (!feof(sourceFile)) {
                bytCount = fread (block, 1, sizeof(block), sourceFile);
                acg->updateGraph(block, bytCount);
                finalBlockTimeClock = clock();
                blockTimeVal = (finalBlockTimeClock - startBlockTime) / (double) CLOCKS_PER_SEC;
                printf("%f ", blockTimeVal);
                startBlockTime = finalBlockTimeClock;
            }
        }
        else {
            while (!feof(sourceFile)) {
                bytCount = fread (block, 1, sizeof(block), sourceFile);
                acg->updateGraph(block, bytCount);
            }
        }

        if (printReadTime) {
            clock_t finalReadTimeClock = clock();
            float readTimeVal = (finalReadTimeClock - startReadTime) / (double) CLOCKS_PER_SEC;
            if (printComments) {
                printf("# Total read time\n");
                printf("%f\n", readTimeVal);
            }
            else {
                printf("%f ", readTimeVal);
            }
        }
        fclose(sourceFile);
        
        if (printFinds) {
            for (Uint i = 0; i < numberOfPatterns; i++) {
                printf("%d-%d \n", i, acg->founds[i]);
            }
        }
        if (printTotalTime) {
            clock_t finalTotalTimeClock = clock();
            float totalTimeVal = (finalTotalTimeClock - startTotalTime) / (double) CLOCKS_PER_SEC;
            if (printComments) {
                printf("# Total read time\n");
                printf("%f\n", totalTimeVal);
            }
            else {
                printf("%f ", totalTimeVal);
            }
        }
        if (printMemory) {
            if (printComments) {
                printf("# RSS and VSZ respectively \n");
                printf("%zu %zu\n", getCurrentRSS() / 1024, getCurrentVSZ() / 1024);
            }
            else {
                printf("%zu %zu", getCurrentRSS() / 1024, getCurrentVSZ() / 1024);
            }
        }
        printf("\n");
        delete(acg);
    }
    else {
        printf("Error al abrir el archivo \n");
    }

}

void MSW() {
    
    clock_t startPreTime;
    clock_t startReadTime;
    clock_t startBlockTime;
    clock_t startTotalTime;
    clock_t startPostTime;
    
    if (printTotalTime) {
        startTotalTime = clock();
    }
    
    MSWGraph *fsmClosure = new MSWGraph();
    char *patterns [numberOfPatterns];
    MSWNode *patternsNodes [numberOfPatterns];
    
    std::ifstream stream(patternsFileName);
    
    size_t patternIndex = 0;
    std::string line;
    if (printPreTime) {
        startPreTime = clock();
    }
    while(std::getline( stream, line ) && patternIndex < numberOfPatterns) {
        char *ww = (char*)malloc(sizeof(char) * (line.length() + 1));
        strcpy(ww, line.c_str());
        patterns[patternIndex] = ww;
        patternsNodes[patternIndex] = fsmClosure->addPattern(ww, (unsigned int) line.length());
        patternIndex++;
    }
    //Sanity
    if (patternIndex < numberOfPatterns) {
        std::cerr << "Pattern file not found or numberOfPatterns wrong." << std::endl;
        exit(1);
    }
    //Proceso el MakeFSM
    fsmClosure->MakeFSM();
    if (printPreTime) {
        clock_t finalPreTimeClock = clock();
        float preTimeVal = (finalPreTimeClock - startPreTime) / (double) CLOCKS_PER_SEC;
        if (printComments) {
            printf("# Total pre process time\n");
            printf("%f\n", preTimeVal);
        }
        else {
            printf("%f ",preTimeVal);
        }
    }

    //Recorro el archivo
    FILE *sourceFile = fopen(fileName, "r");
    if (sourceFile) {
        long pageSize = sysconf(_SC_PAGE_SIZE) * blockSize;
        char block[pageSize];
        size_t bytCount;
        if (printReadTime) {
            startReadTime = clock();
        }
        if (printBlockReadTime) {
            if (printComments) {
                printf("# Read Block by block Times\n");
            }
            startBlockTime = clock();
            float blockTimeVal = 0;
            clock_t finalBlockTimeClock;
            while (!feof(sourceFile)) {
                bytCount = fread (block, 1, sizeof(block), sourceFile);
                fsmClosure->scanValue(block, bytCount);
                finalBlockTimeClock = clock();
                blockTimeVal = (finalBlockTimeClock - startBlockTime) / (double) CLOCKS_PER_SEC;
                printf("%f ", blockTimeVal);
                startBlockTime = finalBlockTimeClock;
            }
        }
        else {
            while (!feof(sourceFile)) {
                bytCount = fread (block, 1, sizeof(block), sourceFile);
                fsmClosure->scanValue(block, bytCount);
            }
        }
        if (printReadTime) {
            clock_t finalReadTimeClock = clock();
            float readTimeVal = (finalReadTimeClock - startReadTime) / (double) CLOCKS_PER_SEC;
            if (printComments) {
                printf("# Total read time\n");
                printf("%f ", readTimeVal);
            }
            else {
                printf("%f ", readTimeVal);
            }
        }
        fclose(sourceFile);

        if (printPostTime) {
            startPostTime = clock();
        }
        if (printFinds) {
            int quantity;
            for (Uint index = 0; index < numberOfPatterns; index++) {
                quantity = fsmClosure->countMatches(patternsNodes[index]);
                printf("%d-%d \n", index, quantity);
            }
        }
        else {
            for (Uint index = 0; index < numberOfPatterns; index++) {
                fsmClosure->countMatches(patternsNodes[index]);
            }
        }
        if (printPostTime) {
            clock_t finalPostTimeClock = clock();
            float postTimeVal = (finalPostTimeClock - startPostTime) / (double) CLOCKS_PER_SEC;
            if (printComments) {
                printf("# Post time\n");
                printf("%f\n", postTimeVal);
            }
            else {
                printf("%f ", postTimeVal);
            }
        }
        if (printTotalTime) {
            clock_t finalTotalTimeClock = clock();
            float totalTimeVal = (finalTotalTimeClock - startTotalTime) / (double) CLOCKS_PER_SEC;
            if (printComments) {
                printf("# Total time\n");
                printf("%f\n", totalTimeVal);
            }
            else {
                printf("%f ", totalTimeVal);
            }
        }
        if (printMemory) {
            if (printComments) {
                printf("# RSS and VSZ respectively \n");
                printf("%zu %zu\n", getCurrentRSS() / 1024, getCurrentVSZ() / 1024);
            }
            else {
                printf("%zu %zu", getCurrentRSS() / 1024, getCurrentVSZ() / 1024);
            }
        }
        printf("\n");
        delete(fsmClosure);
    }
    else {
        printf("Error al abrir el archivo \n");
    }
}

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " <option(s)> SOURCES\n"
    << "Options:\n"
    << "\t-h,--help\t\tShow this help message\n"
    << "\t-p,--patternFileName Patterns\tSpecify pattern file path\n"
    << "\t-c,--corpusFileName Corpus\tSpecify corpus file path\n"
    << "\t-q,--patternNumber Quantity\tSpecify how much patterns are in pattern file\n"
    << "\t-m,--runMode Mode\tSpecify the run mode\n"<< "\tOptions:\n"
    <<"\t\t1 Custom Aho Corasick implementation\n"
    <<"\t\t2 Custom MSW implementation\n"
    << std::endl;
}

int extraOptions(std::string arg, char **argv, int argc, int index) {
    if ((arg == "-s" || (arg == "--readBlockSize"))) {
        if (index + 1 < argc) {
            blockSize = atoi(argv[index + 1]);
            return 2;
        }
        else {
            std::cerr << "-s or --readBlockSize option requires one argument." << std::endl;
            exit(1);
        }
    }
    if (arg == "--printMemory") {
        printMemory = true;
        return 1;
    }
    if (arg == "--readBlockTime") {
        printBlockReadTime = true;
        return 1;
    }
    if (arg == "--preTime") {
        printPreTime = true;
        return 1;
    }
    if (arg == "--readTime") {
        printReadTime = true;
        return 1;
    }
    if (arg == "--postTime") {
        printPostTime = true;
        return 1;
    }
    if (arg == "--totalTime") {
        printTotalTime = true;
        return 1;
    }
    if (arg == "--finds") {
        printFinds = true;
        return 1;
    }
    if (arg == "--printComments") {
        printComments = true;
        return 1;
    }
    return 0;
}

int mainOptions(std::string arg, char **argv, int argc, int index) {
    if ((arg == "-p") || (arg == "--patternFileName")) {
        if (index + 1 < argc) {
            patternsFileName = argv[index + 1];
            return 2;
        }
        else {
            std::cerr << "--patternFileName option requires one argument." << std::endl;
            exit(1);
        }
    }
    if ((arg == "-q") || (arg == "--patternNumber")) {
        if (index + 1 < argc) {
            numberOfPatterns = atoi(argv[index + 1]);
            return 2;
        }
        else {
            std::cerr << "--patternNumber option requires one argument." << std::endl;
            exit(1);
        }
    }
    if ((arg == "-c") || (arg == "--corpusFileName")) {
        if (index + 1 < argc) {
            fileName = argv[index + 1];
            return 2;
        }
        else {
            std::cerr << "--corpusFileName option requires one argument." << std::endl;
            exit(1);
        }
    }
    if ((arg == "-m") || (arg == "--runMode")) {
        if (index + 1 < argc) {
            mode = atoi(argv[index + 1]);
            if(mode != 1 && mode != 2) {
                std::cerr << "--runMode is " << mode << " but should be 1 or 2." << std::endl;
                exit(1);
            }
            return 2;
        }
        else {
            std::cerr << "--runMode option requires one argument." << std::endl;
            return 1;
            exit(1);
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 8) {
        show_usage(argv[0]);
        exit(1);
    }

    int index = 1;
    int prevIndex =index;
    while (index < argc) {
        std::string arg = argv[index];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            exit(1);
        }
        index += mainOptions(arg, argv, argc, index);
        index += extraOptions(arg, argv, argc, index);
        if (index == prevIndex) {
            std::cerr << "--runMode option requires one argument." << std::endl;
            show_usage(argv[index]);
            exit(1);
        }
        prevIndex = index;
    }
    bool badParameters = false;
    if (numberOfPatterns <= 0) {
        badParameters = true;
        std::cerr << "Number of pattern is not setup." << std::endl;
    }
    if (fileName == NULL) {
        badParameters = true;
        std::cerr << "FileName is not setup." << std::endl;
    }
    if (patternsFileName == NULL) {
        badParameters = true;
        std::cerr << "Pattern file is not setup." << std::endl;
    }
    if (mode == 0) {
        badParameters = true;
        std::cerr << "Mode is not setup." << std::endl;
    }
    if (badParameters) {
        exit(1);
    }
    if (mode == 1) {
        ahoCorasick();
    }
    else {
        MSW();
    }
   exit(0);
}