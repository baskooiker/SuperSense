/* 
 * File:   newmain.cpp
 * Author: Bas
 *
 * Created on 13 januari 2014, 14:13
 */

//#include <test_gesture_set.h>
//#include <test_on_interval.h>
//#include <test_sync.h>
#include "GVFTester.h"
#include "SyncDataEvaluation.h"
//#include <test_mp.h>
#include "GestureSetEvaluation.h"
#include "AlphaFilter.h"

#include <string>
#include <vector>
#include <fstream>
#include <process.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void writeToFile(string s) {
    FILE* f;
    do {
        f = fopen("results/test.txt", "a");
    } while (!f);
    fstream os;
    os.open("results/test.txt", fstream::app | fstream::out);
    os << s << "\n";
//    os.close();
//    fprintf(f, "%s\n", s.c_str());
//    fclose(f);
}

void threadOne(void* arg) {
        vector<string> filenames;
        filenames.push_back("data/makifree_1.dat");
    
        SyncDataEvaluation sde = SyncDataEvaluation(filenames);
        sde.setOutputFilename("results/free.csv");
        sde.setTotalNrGest(4);
        sde.evaluate();
//    writeToFile("this is thread 1");
}

void threadTwo(void* arg) {
    //    //    GestureSetEvaluation* gse = new GestureSetEvaluation();
    //    //    gse->setFilename("data/gestureSet20.dat");
    //    //    gse->setOutputFilename("gestureset20PairsOutput83.csv");
    //    //    gse->setTotalNrGest(20);
    //    //    gse->setSkip(3);
    //    //    gse->evaluatePairs();
        vector<string> filenames;
        filenames.push_back("data/makifree_2.dat");
    
        SyncDataEvaluation sde = SyncDataEvaluation(filenames);
        sde.setOutputFilename("results/free.csv");
        sde.setTotalNrGest(4);
        sde.evaluate();
//    writeToFile("this is thread 2");
}

void threadThree(void* arg) {
    //    //    GestureSetEvaluation* gse = new GestureSetEvaluation();
    //    //    gse->setFilename("data/gestureSet20.dat");
    //    //    gse->setOutputFilename("gestureset20PairsOutput85.csv");
    //    //    gse->setTotalNrGest(20);
    //    //    gse->setSkip(5);
    //    //    gse->evaluatePairs();
    
        vector<string> filenames;
        filenames.push_back("data/makifree_3.dat");

        SyncDataEvaluation sde = SyncDataEvaluation(filenames);
        sde.setOutputFilename("results/free.csv");
        sde.setTotalNrGest(4);
        sde.evaluate();
    
//    writeToFile("this is thread 3");
}

/*
 * 
 */
int main(int argc, char** argv) {
    vector<string> filenames;
    //    filenames.push_back("joyce8_2.dat");
    //    filenames.push_back("joyce8_3.dat");
    //    filenames.push_back("bas8_2.dat");
    //    filenames.push_back("bas8_3.dat");
    //    filenames.push_back("bas8_4.dat");
    //    filenames.push_back("nico8.dat");
    //    filenames.push_back("nico8_2.dat");
    //    filenames.push_back("nico8_3.dat");
    //    filenames.push_back("test4noresampling.dat");
    //    filenames.push_back("gestureSet20.dat");

    //    SyncDataEvaluation* sde = new SyncDataEvaluation(filenames);
    //    sde->evaluate();
    //    sde->optimizeGVFParameters();

    HANDLE handles[3];
    handles[0] = (HANDLE) _beginthread(threadOne, 0, (void*) 0);
    handles[1] = (HANDLE) _beginthread(threadTwo, 0, (void*) 1);
    handles[2] = (HANDLE) _beginthread(threadThree, 0, (void*) 2);
    
    for (int j = 0; j < 3; j++) 
        WaitForSingleObject(handles[j], INFINITE);
   

    return 0;
}
