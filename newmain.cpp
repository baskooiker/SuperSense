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
