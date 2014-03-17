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

#include <string>
#include <vector>
#include <fstream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    vector<string> filenames;
    filenames.push_back("joyce8_2.dat");
    filenames.push_back("joyce8_3.dat");
    filenames.push_back("bas8_2.dat");
    filenames.push_back("bas8_3.dat");
    filenames.push_back("bas8_4.dat");
    filenames.push_back("nico8.dat");
    filenames.push_back("nico8_2.dat");
    filenames.push_back("nico8_3.dat");
    
    SyncDataEvaluation* sde = new SyncDataEvaluation(filenames);
    sde->optimizeGVFParameters();

    return 0;
}
