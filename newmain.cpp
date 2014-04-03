/* 
 * File:   newmain.cpp
 * Author: Bas
 *
 * Created on 13 januari 2014, 14:13
 */

//#include "GVFTester.h"
#include "SyncDataEvaluation.h"
#include "GestureSetEvaluation.h"
#include "AlphaFilter.h"
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
    //        filenames.push_back("data/makifree_1.dat");
    filenames.push_back("data/maki_1.dat");

    SyncDataEvaluation sde = SyncDataEvaluation(filenames);
    //        sde.setOutputFilename("results/free.csv");
    sde.setOutputFilename("results/gestureSet.csv");
    sde.setTotalNrGest(20);
    sde.evaluate();

    filenames.clear();
    //        filenames.push_back("data/makifree_1.dat");
    filenames.push_back("data/makifree_1.dat");

    SyncDataEvaluation sde2 = SyncDataEvaluation(filenames);
    //        sde.setOutputFilename("results/free.csv");
    sde2.setOutputFilename("results/free.csv");
    sde2.setTotalNrGest(4);
    sde2.evaluate();
}

void threadTwo(void* arg) {
    vector<string> filenames;
    //        filenames.push_back("data/makifree_2.dat");
    filenames.push_back("data/maki_2.dat");

    SyncDataEvaluation sde = SyncDataEvaluation(filenames);
    //        sde.setOutputFilename("results/free.csv");
    sde.setOutputFilename("results/gestureSet.csv");
    sde.setTotalNrGest(20);
    sde.evaluate();

    filenames.clear();
    //        filenames.push_back("data/makifree_2.dat");
    filenames.push_back("data/makifree_2.dat");

    SyncDataEvaluation sde2 = SyncDataEvaluation(filenames);
    //        sde.setOutputFilename("results/free.csv");
    sde2.setOutputFilename("results/free.csv");
    sde2.setTotalNrGest(4);
    sde2.evaluate();
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
    vector<string> filenames;
    filenames.push_back("data/bas_rotation2.dat");

    DataChecker dc;
    if (!dc.checkCorrectness(filenames))
        return -1;

//    DTWNNTester dtw = DTWNNTester();
//    dtw.evaluate("data/bas_rotation2.dat");

    SyncDataEvaluation sde2 = SyncDataEvaluation(filenames);
    sde2.setTotalNrGest(20);
    sde2.setOutputFilename("results/rotation.csv");
    sde2.optimizeGVFParameters();

    return 0;


    //    filenames.clear();
    //    filenames.push_back("data/maki_1.dat");
    //    filenames.push_back("data/maki_2.dat");
    //    filenames.push_back("data/maki_3.dat");
    //    filenames.push_back("data/makifree_1.dat");
    //    filenames.push_back("data/makifree_2.dat");
    //    filenames.push_back("data/makifree_3.dat");
    //    DataChecker dc = DataChecker();
    //    if(!dc.checkCorrectness(filenames))
    //        return -1;
    //    
    //    HANDLE handles[3];
    //    handles[0] = (HANDLE) _beginthread(threadOne, 0, (void*) 0);
    //    handles[1] = (HANDLE) _beginthread(threadTwo, 0, (void*) 1);
    //    handles[2] = (HANDLE) _beginthread(threadThree, 0, (void*) 2);
    //    
    //    for (int j = 0; j < 3; j++) 
    //        WaitForSingleObject(handles[j], INFINITE);

    return 0;
}
