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

using namespace std;

void threadOne(void* arg) {
    GestureSetEvaluation* gse = new GestureSetEvaluation();
    gse->setFilename("gestureSet20.dat");
    gse->setOutputFilename("gestureset20PairsOutput61.csv");
    gse->setTotalNrGest(20);
    gse->setSkip(1);
    gse->evaluatePairs();
}

void threadTwo(void* arg) {
    GestureSetEvaluation* gse = new GestureSetEvaluation();
    gse->setFilename("gestureSet20.dat");
    gse->setOutputFilename("gestureset20PairsOutput63.csv");
    gse->setTotalNrGest(20);
    gse->setSkip(3);
    gse->evaluatePairs();
}

void threadThree(void* arg) {
    GestureSetEvaluation* gse = new GestureSetEvaluation();
    gse->setFilename("gestureSet20.dat");
    gse->setOutputFilename("gestureset20PairsOutput65.csv");
    gse->setTotalNrGest(20);
    gse->setSkip(5);
    gse->evaluatePairs();
}

void threadFour(void* arg) {
    GestureSetEvaluation* gse = new GestureSetEvaluation();
    gse->setFilename("gestureSet20.dat");
    gse->setOutputFilename("gestureset20PairsOutput67.csv");
    gse->setTotalNrGest(20);
    gse->setSkip(7);
    gse->evaluatePairs();
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

    HANDLE t1 = (HANDLE) _beginthread(threadOne, 0, (void*) 1);
    HANDLE t2 = (HANDLE) _beginthread(threadTwo, 0, (void*) 3);
    HANDLE t3 = (HANDLE) _beginthread(threadThree, 0, (void*) 5);
//    HANDLE t4 = (HANDLE) _beginthread(threadFour, 0, (void*) 7);

    WaitForSingleObject(t1, INFINITE);
    WaitForSingleObject(t2, INFINITE);
    WaitForSingleObject(t3, INFINITE);
//    WaitForSingleObject(t4, INFINITE);

    return 0;
}
