#include <cstdlib>
#include <cstdio>
#include <assert.h>
#include "Vocabulary.h"
#include "structs.h"
#include "DependenceGraph.h"
#include <set>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <ctime>
#include <time.h>

#define SPLITTING
//#define APPLICATION

using namespace std;

FILE* summary;
FILE* unit;
extern FILE* yyin;
extern vector<Rule> G_NLP;
extern set<int> U;
extern int *atomState;
extern int *ruleState;
extern int yyparse();
FILE* result_out;
FILE* count_out;        // 保存所有输入文件中的“Atoms(b_U(P)) \ U”数量


void io(const char* iPathName, const char* oPathName, const char* sPathName) {
    yyin = fopen(iPathName, "r");
    unit = fopen(oPathName, "w+");
    summary = fopen(sPathName, "a+");
    
    if (! yyin) {
        printf("IO Error: cannot open the input file.\n" );
        assert(0);
    }
    if (!summary || !unit) {
        printf("IO Error: cannot open the output file.\n");
        assert(0);
    }
}

int main(int argc, char** argv) {

#ifdef SPLITTING
    
    count_out = fopen("IO/count_out.txt", "a");
    
//    int mPealSize = atoi(argv[3]);
    string filename(argv[2]);
    printf("%s\n", filename.c_str());
    string filenametmp = filename.substr(filename.find_last_of("/")+1);
    string numInFile;
    for(int i = 0; i < filenametmp.size(); i++) {
        if(filenametmp.at(i) != '.') {
            numInFile.push_back(filenametmp.at(i));
        }
        else {
            break;
        }
    }
    int mPealSize = atoi((char*)(numInFile.c_str()));
    printf("%d\n", mPealSize);

    yyin = fopen(argv[1], "r");   //--------------------------------------
//    int mPealSize = 9;
//    yyin = fopen("IO/input/nva2_9.txt.in", "r");
    
    if (!yyin) {
        printf("IO Error: cannot open the input file.\n" );
        assert(0);
    }

    string outPath("IO/output/output_");
    
    string tmp(argv[1]);        //--------------------------------------
//    string tmp = "IO/input/nva2_9.txt.in";
    
    outPath.append(tmp.substr(tmp.find_last_of("/")+1, tmp.size()-4));
//        printf("%s\n", outPath.c_str());
    result_out = fopen((outPath + ".txt").c_str(), "w");
    if (!result_out) {
        printf("IO Error: cannot open the output file.\n");
        assert(0);
    }

    printf("Start Parser\n");
    yyparse();
    printf("End Parser\n");

    // get U from IO/pealGraph/cuts.txt
    int BUFF_SIZE = 1024;
    
    FILE* fcuts = fopen(argv[2], "r");  //--------------------------------------
//    FILE* fcuts = fopen("IO/pealGraph/cuts/9.cut", "r");
    
    if(!fcuts) {
        printf("Open fcuts failed!\n");
        assert(0);
    }
    char lines[BUFF_SIZE];
    while(fgets(lines, BUFF_SIZE, fcuts)) {
        int last = strlen(lines);
        if(lines[last-1] == '\n')
            lines[last-1] = '\0';   // reduce the last '\n' per line
        string str_(lines);
//        printf("%s %d %d\n", str_.c_str(), str_.length(), str_.size());
        int index = Vocabulary::instance().queryAtom((char*)(str_.c_str()));
        if(index != -1)
            U.insert(index);
        else 
            printf("cuts Error!\n");
    }
//    printf("Us : \n");
//    for(set<int>::iterator it = U.begin(); it != U.end(); it++) {
//        printf("%s ", Vocabulary::instance().getAtom(*it));
//    }
//    printf("\n");
    fclose(fcuts);

//    fprintf(result_out, "Total Atom size = %d\n", Vocabulary::instance().apSize());
    DependenceGraph dpdg;
    dpdg.setMPealSize(mPealSize);
    long start = clock();
//    U = dpdg.calU();
     dpdg.splitting(result_out, argv[1]);     //-------------------------------------
//    dpdg.splitting(result_out, "IO/input/nva2_9.txt.in");
//    dpdg.testFunction(result_out);
//    dpdg.calbuU();
    long end = clock();

//    fprintf(result_out, "\nTime cost : %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    fclose(result_out);
    fclose(yyin);
#endif
    
    
#ifdef APPLICATION
    yyin = fopen("IO/app/input/sample2.txt", "r");
    if(!yyin) {
        printf("Open yyin fail!\n");
        assert(0);
    }
    FILE* result = fopen("IO/app/output/output.txt", "w");
    if(!result) {
        printf("Open result fail!\n");
        assert(0);
    }
    
    printf("Start Parser\n");
    yyparse();
    printf("End Parser\n");
    
    int u = Vocabulary::instance().queryAtom("a");
    if(u == -1) {
        printf("Get U error!\n");
        assert(0);
    }
    U.insert(u);
    
    DependenceGraph dpg;
    dpg.application(result);
    
    fclose(result);
    fclose(yyin);
#endif    
    
    return 0;
}