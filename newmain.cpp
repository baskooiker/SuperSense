/* 
 * File:   newmain.cpp
 * Author: Bas
 *
 * Created on 13 januari 2014, 14:13
 */

#include "SyncDataEvaluation.h"
#include "GestureSetEvaluation.h"
#include "DataChecker.h"
#include "DTWNNTester.h"
#include "GVFTester.h"
#include "YINTester.h"
#include "FeatureCalculator.h"
#include "DTWExtendedTester.h"

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
    dtw.setOutputFilename("results/rotationlien2.csv");
    dtw.setTotalNrGest(20);
    dtw.evaluateAllFiles(filenames);
    
    return;
    
    SyncDataEvaluation sde2 = SyncDataEvaluation(filenames);
    sde2.setTotalNrGest(20);
    sde2.setOutputFilename("results/intersubject.csv");
    sde2.evaluateAllFiles();
}

void threadTwo(void* arg) {
    std::vector<string> filenames;

//    filenames.push_back("data/maki_0.dat");
//    filenames.push_back("data/maki_45.dat");
//    filenames.push_back("data/maki_90.dat");
//    filenames.push_back("data/lienfree_1.dat");
//    filenames.push_back("data/lienfree_2.dat");
//    filenames.push_back("data/lienfree_3.dat");

    filenames.push_back("data/maki_0.dat");
    filenames.push_back("data/lien_0.dat");
    filenames.push_back("data/nico_0.dat");
    filenames.push_back("data/leonie_0.dat");
    filenames.push_back("data/jan_0.dat");
    filenames.push_back("data/suzanne_0.dat");
    
//    DTWNNTester dtw = DTWNNTester();
    YINTester dtw = YINTester();
//    dtw.setOutputFilename("results/shifttest.csv");
//    dtw.setTotalNrGest(4);
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

vector<string> freeFiles(){
    vector<string> filenames;
    
    filenames.push_back("data/danfree_1.dat");
    filenames.push_back("data/danfree_2.dat");
    filenames.push_back("data/danfree_3.dat");
    filenames.push_back("data/janfree_1.dat");
    filenames.push_back("data/janfree_2.dat");
    filenames.push_back("data/janfree_3.dat");
    filenames.push_back("data/lienfree_1.dat");
    filenames.push_back("data/lienfree_2.dat");
    filenames.push_back("data/lienfree_3.dat");
    filenames.push_back("data/makifree_1.dat");
    filenames.push_back("data/makifree_2.dat");
    filenames.push_back("data/makifree_3.dat");
    filenames.push_back("data/marcofree_1.dat");
    filenames.push_back("data/marcofree_2.dat");
    filenames.push_back("data/marcofree_3.dat");
    filenames.push_back("data/nicofree_1.dat");
    filenames.push_back("data/nicofree_2.dat");
    filenames.push_back("data/nicofree_3.dat");
    filenames.push_back("data/suzannefree_1.dat");
    filenames.push_back("data/suzannefree_2.dat");
    filenames.push_back("data/suzannefree_3.dat");
    filenames.push_back("data/mirjamfree_1.dat");
    filenames.push_back("data/mirjamfree_2.dat");
    filenames.push_back("data/mirjamfree_3.dat");
    
    return filenames;
}

vector<string> fixedFiles(){
    vector<string> filenames;
    
    filenames.push_back("data/dan_0.dat");
    filenames.push_back("data/dan_45.dat");
    filenames.push_back("data/dan_90.dat");
    filenames.push_back("data/jan_0.dat");
    filenames.push_back("data/jan_45.dat");
    filenames.push_back("data/jan_90.dat");
    filenames.push_back("data/lien_0.dat");
    filenames.push_back("data/lien_45.dat");
    filenames.push_back("data/lien_90.dat");
    filenames.push_back("data/maki_0.dat");
    filenames.push_back("data/maki_45.dat");
    filenames.push_back("data/maki_90.dat");
    filenames.push_back("data/marco_0.dat");
    filenames.push_back("data/marco_45.dat");
    filenames.push_back("data/marco_90.dat");
    filenames.push_back("data/nico_0.dat");
    filenames.push_back("data/nico_45.dat");
    filenames.push_back("data/nico_90.dat");
    filenames.push_back("data/suzanne_0.dat");
    filenames.push_back("data/suzanne_45.dat");
    filenames.push_back("data/suzanne_90.dat");
    filenames.push_back("data/mirjam_0.dat");
    filenames.push_back("data/mirjam_45.dat");
    filenames.push_back("data/mirjam_90.dat");
    
    return filenames;
}

void writeFeatureFile(){
    FeatureCalculator fc = FeatureCalculator();
    vector<string> filenames = freeFiles();
    fc.calculateFeatures(filenames, "results/featureCalculations2.csv");
}

void testDTWExtended(){
    vector<string> filenames = fixedFiles();
    
    DTWExtendedTester dtw = DTWExtendedTester();
    dtw.setOutputFilename("results/dtwExtendedTest.csv");
    dtw.setTotalNrGest(20);
    dtw.evaluateIndividual(filenames);
}

void testDTWNN(){
    vector<string> filenames = fixedFiles();
    
    DTWNNTester dtw = DTWNNTester();
    dtw.setOutputFilename("results/dtwTest.csv");
    dtw.setTotalNrGest(20);
    dtw.evaluateIndividual(filenames);
}

/*
 * 
 */
int main(int argc, char** argv) {
    
//    writeFeatureFile();
    testDTWExtended();
//    testDTWNN();
    return 0;
    
    HANDLE handles[2];
//    handles[0] = (HANDLE) _beginthread(threadOne, 0, (void*) 0);
    handles[1] = (HANDLE) _beginthread(threadTwo, 0, (void*) 1);
    //        handles[2] = (HANDLE) _beginthread(threadThree, 0, (void*) 2);

    
//    WaitForSingleObject(handles[0], INFINITE);
    WaitForSingleObject(handles[1], INFINITE);
    //        WaitForSingleObject(handles[2], INFINITE);

    return 0;
}
