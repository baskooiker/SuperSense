/* 
 * File:   newmain.cpp
 * Author: Bas
 *
 * Created on 13 januari 2014, 14:13
 */

//#include "GVFTester.h"
#include "SyncDataEvaluation.h"
#include "GestureSetEvaluation.h"
#include "DataChecker.h"
#include "DTWNNTester.h"

#include <string>
#include <vector>
#include <fstream>
#include <process.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void threadOne(void* arg) {
    vector<string> filenames;

    filenames.push_back("data/lien_0.dat");
    filenames.push_back("data/lien_45.dat");
    filenames.push_back("data/lien_90.dat");
//    filenames.push_back("data/nico_0.dat");
//    filenames.push_back("data/leonie_0.dat");
//    filenames.push_back("data/maki_1.dat");

    DTWNNTester dtw = DTWNNTester();
    dtw.setOutputFilename("results/rotationlien3.csv");
    dtw.setTotalNrGest(20);
    dtw.evaluateAllFiles(filenames);
    
    return;
    
    SyncDataEvaluation sde2 = SyncDataEvaluation(filenames);
    sde2.setTotalNrGest(20);
    sde2.setOutputFilename("results/intersubject.csv");
    sde2.evaluateAllFiles();
}

void threadTwo(void* arg) {
    vector<string> filenames;

    filenames.push_back("data/lienfree_1.dat");
    filenames.push_back("data/lienfree_2.dat");
    filenames.push_back("data/lienfree_3.dat");

    DTWNNTester dtw = DTWNNTester();
    dtw.setOutputFilename("results/lienfree3.csv");
    dtw.setTotalNrGest(4);
    dtw.evaluateAllFiles(filenames);
    return;
    
    SyncDataEvaluation sde2 = SyncDataEvaluation(filenames);
    sde2.setTotalNrGest(4);
    sde2.setOutputFilename("results/lienfree.csv");
    sde2.evaluateOnFirst();
}

void threadThree(void* arg) {
    vector<string> filenames;
    //        filenames.push_back("data/makifree_3.dat");
    filenames.push_back("data/maki_3.dat");

    SyncDataEvaluation sde = SyncDataEvaluation(filenames);
    //        sde.setOutputFilename("results/free.csv");
    sde.setOutputFilename("results/gestureSet.csv");
    sde.setTotalNrGest(20);
    sde.evaluate();

    filenames.clear();
    //        filenames.push_back("data/makifree_3.dat");
    filenames.push_back("data/makifree_3.dat");

    SyncDataEvaluation sde2 = SyncDataEvaluation(filenames);
    //        sde.setOutputFilename("results/free.csv");
    sde2.setOutputFilename("results/free.csv");
    sde2.setTotalNrGest(4);
    sde2.evaluate();
}

/*
 * 
 */
int main(int argc, char** argv) {
//    vector<string> filenames;
//    filenames.push_back("data/lien_0.dat");
//    filenames.push_back("data/lien_45.dat");
//    filenames.push_back("data/lien_90.dat");
//    DataChecker dc = DataChecker();
//    if(!dc.checkCorrectness(filenames))
//        return -1;
//    return 1;
    
    HANDLE handles[2];
    handles[0] = (HANDLE) _beginthread(threadOne, 0, (void*) 0);
//    handles[1] = (HANDLE) _beginthread(threadTwo, 0, (void*) 1);
    //        handles[2] = (HANDLE) _beginthread(threadThree, 0, (void*) 2);

    WaitForSingleObject(handles[0], INFINITE);
//    WaitForSingleObject(handles[1], INFINITE);
    //        WaitForSingleObject(handles[2], INFINITE);

    return 0;
}
