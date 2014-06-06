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

vector<string> getNames();
void testGVF(float from, float to, float interval, string filename);
void testDTWNN(float from, float to, float interval);
void testGVFInterUser();
void testDTWNNInterUser();
void testGVFFree();
void testDTWNNFree();
vector<string> fixedFiles(int offset = 0);
vector<string> firstFiles();

void threadOne(void* arg) {
    vector<string> filenames = firstFiles();
    for (int res = 15; res < 26; res += 1) {
        for (int fileNr = 0; fileNr < filenames.size(); fileNr++) {
            vector<string> file;
            file.push_back(filenames[fileNr]);
            GVFTester gvf = GVFTester();
            gvf.setInterpolate(20);
            gvf.setReduce(res);
            gvf.setOutputFilename("results/resampleGVF.csv");
            gvf.setTotalNrGest(20);
            gvf.evaluateAllFiles(file);
        }
    }
}

void threadTwo(void* arg) {
        vector<string> filenames = firstFiles();
    for (int res = 15; res < 26; res += 1) {
        for (int fileNr = 0; fileNr < filenames.size(); fileNr++) {
            vector<string> file;
            file.push_back(filenames[fileNr]);
            DTWNNTester gvf = DTWNNTester();
            gvf.setInterpolate(20);
            gvf.setReduce(res);
            gvf.setOutputFilename("results/resampleDTW.csv");
            gvf.setTotalNrGest(20);
            gvf.evaluateAllFiles(file);
        }
    }
}

void threadThree(void* arg) {
        vector<string> filenames = firstFiles();
    for (int res = 15; res < 26; res += 1) {
        for (int fileNr = 0; fileNr < filenames.size(); fileNr++) {
            vector<string> file;
            file.push_back(filenames[fileNr]);
            DTWExtendedTester gvf = DTWExtendedTester();
            gvf.setInterpolate(20);
            gvf.setReduce(res);
            gvf.setOutputFilename("results/resampleDTWExt.csv");
            gvf.setTotalNrGest(20);
            gvf.evaluateAllFiles(file);
        }
    }
}

vector<string> freeFiles() {
    vector<string> names = getNames();
    vector<string> filenames;
    for (int i = 0; i < names.size(); i++) {
        filenames.push_back("data/" + names[i] + "free_1.dat");
        filenames.push_back("data/" + names[i] + "free_2.dat");
        filenames.push_back("data/" + names[i] + "free_3.dat");
    }
    return filenames;
}

vector<vector<string> > freeSubjects() {
    vector<vector<string> > filenames;
    vector<string> names = getNames();
    for (int i = 0; i < names.size(); i++) {
        vector<string> subject;
        subject.push_back("data/" + names[i] + "free_1.dat");
        subject.push_back("data/" + names[i] + "free_2.dat");
        subject.push_back("data/" + names[i] + "free_3.dat");
        filenames.push_back(subject);
    }
    return filenames;
}

vector<vector<string> > fixedSubjects() {
    vector<vector<string> > filenames;
    vector<string> names = getNames();
    for (int i = 0; i < names.size(); i++) {
        vector<string> subject;
        subject.push_back("data/" + names[i] + "_0.dat");
        subject.push_back("data/" + names[i] + "_45.dat");
        subject.push_back("data/" + names[i] + "_90.dat");
        filenames.push_back(subject);
    }
    return filenames;
}

vector<vector<string> > interUserSet() {
    vector<vector<string> > subjects = fixedSubjects();
    vector<vector<string> > interSubjects;
    for (int i = 0; i < subjects[0].size(); i++) {
        vector<string> set;
        for (int j = 0; j < subjects.size(); j++)
            set.push_back(subjects[j][i]);
        interSubjects.push_back(set);
    }
    return interSubjects;
}

vector<string> fixedFiles(int fileoffset) {
    vector<string> names = getNames();
    vector<string> filenames;
    for (int i = fileoffset; i < names.size(); i++) {
        filenames.push_back("data/" + names[i] + "_0.dat");
        filenames.push_back("data/" + names[i] + "_45.dat");
        filenames.push_back("data/" + names[i] + "_90.dat");
    }
    return filenames;
}

vector<string> firstFiles() {
    vector<string> filenames;

    filenames.push_back("data/dan_0.dat");
    filenames.push_back("data/jan_0.dat");
    filenames.push_back("data/lien_0.dat");
    filenames.push_back("data/maki_0.dat");
    filenames.push_back("data/marco_0.dat");
    filenames.push_back("data/nico_0.dat");
    filenames.push_back("data/suzanne_0.dat");
    filenames.push_back("data/mirjam_0.dat");
    filenames.push_back("data/tijs_0.dat");
    filenames.push_back("data/marije_0.dat");

    return filenames;
}

vector<string> getNames() {
    vector<string> names;
    names.push_back("dan");
    names.push_back("jan");
    names.push_back("lien");
    names.push_back("maki");
    names.push_back("marco");
    names.push_back("nico");
    names.push_back("suzanne");
    names.push_back("mirjam");
    names.push_back("tijs");
    names.push_back("marije");
    return names;
}

void writeFeatureFile() {
    FeatureCalculator fc = FeatureCalculator();
    vector<string> filenames = freeFiles();
    fc.calculateFeatures(filenames, "results/featureCalculations2.csv");
}

void testDTWExtended() {
    vector<string> filenames = fixedFiles();

    DTWExtendedTester dtw = DTWExtendedTester();
    dtw.setOutputFilename("results/dtwExtendedTest.csv");
    dtw.setTotalNrGest(20);
    dtw.evaluateIndividual(filenames);
}

void testDTWNN(float from, float to, float interval, string filename, int fileoffset) {
    vector<string> filenames = fixedFiles(fileoffset);
    for (float rot = from; rot <= to; rot += interval) {
        DTWNNTester dtw = DTWNNTester();
        dtw.setShift(rot);
        dtw.setOutputFilename(filename);
        dtw.setTotalNrGest(20);
        dtw.evaluateIndividual(filenames);
    }
}

void testGVF(float from, float to, float interval, string filename = "results/orientationGVF.csv") {
    vector<string> filenames = fixedFiles();
    for (float rot = from; rot <= to; rot += interval) {
        for (int fileNr = 0; fileNr < filenames.size(); fileNr++) {
            vector<string> file;
            file.push_back(filenames[fileNr]);
            GVFTester gvf = GVFTester();
            gvf.setShift(rot);
            gvf.setOutputFilename(filename);
            gvf.setTotalNrGest(20);
            gvf.evaluateAllFiles(file);
        }
    }
}

void testDTWNNFree() {
    vector<string> filenames = freeFiles();
    DTWNNTester dtw = DTWNNTester();
    dtw.setOutputFilename("results/freeDTW.csv");
    dtw.setTotalNrGest(4);
    dtw.evaluateIndividual(filenames);

}

void testGVFFree() {
    vector<string> filenames = freeFiles();
    for (int fileNr = 0; fileNr < filenames.size(); fileNr++) {
        vector<string> file;
        file.push_back(filenames[fileNr]);
        GVFTester gvf = GVFTester();
        gvf.setOutputFilename("results/freeGVF.csv");
        gvf.setTotalNrGest(20);
        gvf.evaluateAllFiles(file);

    }
}

void testGVFInterUser() {
    vector<vector<string> > filenames = interUserSet();
    for (int fileNr = 0; fileNr < filenames.size(); fileNr++) {
        GVFTester gvf = GVFTester();
        gvf.setOutputFilename("results/interUserGVF.csv");
        gvf.setTotalNrGest(20);
        gvf.evaluateAllFiles(filenames[fileNr]);
    }
}

void testDTWNNInterUser() {
    vector<vector<string> > filenames = interUserSet();
    for (int fileNr = 0; fileNr < filenames.size(); fileNr++) {
        DTWNNTester dtw = DTWNNTester();
        dtw.setOutputFilename("results/interUserDTW.csv");
        dtw.setTotalNrGest(20);
        dtw.evaluateAllFiles(filenames[fileNr]);
    }
}

int main(int argc, char** argv) {


    HANDLE handles[2];
    handles[0] = (HANDLE) _beginthread(threadOne, 0, (void*) 0);
    Sleep(100);
    handles[1] = (HANDLE) _beginthread(threadTwo, 0, (void*) 1);
    Sleep(100);
    handles[2] = (HANDLE) _beginthread(threadThree, 0, (void*) 2);


    WaitForSingleObject(handles[0], INFINITE);
    WaitForSingleObject(handles[1], INFINITE);
    WaitForSingleObject(handles[2], INFINITE);

    return 0;
}
