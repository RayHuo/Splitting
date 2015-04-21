/* 
 * File:   main.cpp
 * Author: ray
 *
 * Created on September 9, 2014, 10:47 AM
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <set>
#include <sstream>

using namespace std;

void createPealGraph(int PEALNUM, int PEALSIZE) {
    string fileName = "nva";
    stringstream ss1;    
    string tmp1 = "";   
    ss1 << PEALNUM;
    tmp1 = ss1.str();
    fileName += tmp1 + "_";
    stringstream ss2;
    ss2 << PEALSIZE;
    string tmp2 = "";
    tmp2 = ss2.str();
    fileName += tmp2 + ".txt";

    string openFile = "IO/pealGraph/";
    openFile += fileName;

    string cutFile = "IO/pealGraph/cuts/";
    cutFile += fileName;

    FILE* fout; 
    FILE* fcuts;
    fout = fopen(openFile.c_str(), "w");
    if(!fout) {
        printf("Fail to open output file.\n");
        return;
    }

    // fcuts = fopen("IO/pealGraph/cuts.txt", "w");
    fcuts = fopen(cutFile.c_str(), "w");
    if(!fcuts) {
        printf("Fail to open cuts file.\n");
        return;
    }

    srand(time(0));
    int total = PEALNUM*PEALSIZE;
    bool visited[PEALNUM*PEALSIZE];
    memset(visited, false, sizeof(visited));

    // print all vertexs in file
    for(int i = 0; i < total; i++) {
        fprintf(fout, "vertex(%d).\n", i);      fflush(fout);
    }
    visited[0] = true;  // while visited[i] = true, vertex[i] can not be choose as a connect node

    // print all arcs in the each peal Test
    for(int t = 0; t < PEALNUM; t++)
        for(int i = 0; i< PEALSIZE; i++)
            for(int j = i+1; j< PEALSIZE; j++) {
                fprintf(fout, "arc(%d, %d).\n", i+t*PEALSIZE, j+t*PEALSIZE);    fflush(fout);
                fprintf(fout, "arc(%d, %d).\n", j+t*PEALSIZE, i+t*PEALSIZE);    fflush(fout);
            }

    // generate connect edge for every two nodes
    set<int> cuts;              // hold all cut vertexs             
    int st = -1, ed = -1;

    // first node to second node
    do {
        st = rand() % PEALSIZE;
    }while(visited[st]);
    visited[st] = true;     cuts.insert(st);
    do {
        ed = rand() % PEALSIZE + PEALSIZE;
    }while(visited[ed]);
    visited[ed] = true;     cuts.insert(ed);
    fprintf(fout, "arc(%d, %d).\n", st, ed);    fflush(fout);

    // second node to first node
    do {
        st = rand() % PEALSIZE + PEALSIZE;
    }while(visited[st]);
    visited[st] = true;     cuts.insert(st);
    do {
        ed = rand() % PEALSIZE;
    }while(visited[ed]);
    visited[ed] = true;     cuts.insert(ed);
    fprintf(fout, "arc(%d, %d).\n", st, ed);    fflush(fout);

    // print out total line num to judge whether a correct output
    // int lines = ((PEALSIZE-1)*PEALSIZE) * PEALNUM + PEALNUM + 2*PEALSIZE;
    // fprintf(fout, "Total lines should be %d\n", lines);      fflush(fout);

    // print out all cut vertexs with reached(i)
    for(set<int>::iterator it = cuts.begin(); it != cuts.end(); it++) {
        fprintf(fcuts, "reached(%d)\n", *it);   fflush(fout);
    }

    fclose(fcuts);
    fclose(fout);


    // then call gringo to ground the hc.lp with output file created above.
    // 管道调用 RUN_CMD 计算并将其结果写进 OUTPUT_FILE
    int BUFF_SIZE = 1024;
    string OUTPUT_FILE = "IO/input/";
    OUTPUT_FILE += fileName + ".in";
    string cmd = "gringo -t IO/pealGraph/hc.lp " + openFile + " > " + OUTPUT_FILE;
    char buff[BUFF_SIZE];
    FILE *pipe_file = popen(cmd.c_str(), "r");
    FILE *output_file = fopen(OUTPUT_FILE.c_str(), "w");
    while (fgets(buff, BUFF_SIZE, pipe_file)) {
        fprintf(output_file, "%s", buff);
    }
    pclose(pipe_file);
    fclose(output_file);

    // get U from IO/pealGraph/cuts.txt
    set<string> U;
//    FILE* fcuts2 = fopen("IO/pealGraph/cuts.txt", "r");
    FILE* fcuts2 = fopen(cutFile.c_str(), "r");
    char lines[BUFF_SIZE];
    while(fgets(lines, BUFF_SIZE, fcuts2)) {
        // int index = Vocabulary::instance().queryAtom((char*)lines.c_str());
        // if(index != -1)
        int last = strlen(lines);
        if(lines[last-1] == '\n')
            lines[last-1] = '\0';   // reduce the last '\n' per line
        string str(lines);
            U.insert(str);
        // else 
        //     printf("cuts Error!\n");
    }
    fclose(fcuts2);
    // printf("U : \n");
    // for(set<string>::iterator it = U.begin(); it != U.end(); it++) {
    //     printf("%s ", (*it).c_str());
    // }
    // printf("\n");
}


int main(int argc, char** argv) {
    createPealGraph(2, 3);
    createPealGraph(2, 4);
    createPealGraph(2, 5);
    createPealGraph(2, 6);
    createPealGraph(2, 7);
    createPealGraph(2, 8);
    createPealGraph(2, 9);
    return 0;
}