/* 
 * File:   SyncDataEvaluation.cpp
 * Author: Bas
 * 
 * Created on 13 maart 2014, 14:59
 */

#include "SyncDataEvaluation.h"
#include "../YIN/YIN.h"
#include "../utils/utils.h"

#include <vector>
#include <string>
#include <algorithm>

#include "init.h"

using namespace std;

SyncDataEvaluation::SyncDataEvaluation(vector<string> fn) {
    initValues();
    filenames = fn;
}

SyncDataEvaluation::SyncDataEvaluation(const SyncDataEvaluation& orig) {
}

SyncDataEvaluation::~SyncDataEvaluation() {
}

vector<int> SyncDataEvaluation::gesturesInData(vector<vector<float> > data) {
    vector<int> gests;
    for (int i = 0; i < data.size(); i++) {
        int g = data[i][3];
        if (!contains(gests, g) && g != 0)
            gests.push_back(g);
    }
    std::sort(gests.begin(),gests.end());
    return gests;
}

float SyncDataEvaluation::syncTestProcedure(vector<vector<float> > data) {
    vector<int> gests = gesturesInData(data);

    int conf[2][2][gests.size()];
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < gests.size(); k++)
                conf[i][j][k] = 0;

    int isSync = 0;
    int syncDetected = 0;
    yin->clear();
    int lastGest = (int) data[0][3];

    for (int i = 0; i < data.size(); i++) {
        vector<float> newSample;
        for (int j = 0; j < 3; j++)
            newSample.push_back(data[i][j]);

        if ((int) data[i][3] != lastGest) {
            isSync = 0;
            //            printf("off\n");
        }
        lastGest = data[i][3];

        if (data[i][4] > 0.) {
            isSync = 1;
            //            printf("click\n");
        }

        yin->process(newSample);
        syncDetected = yin->isSync() ? 1 : 0;

        conf[isSync][syncDetected][indexOf(lastGest, gests)]++;
    }

    for (int k = 0; k < gests.size(); k++) {
        for (int i = 0; i < 2; i++) {
            int sum = 0;
            for (int j = 0; j < 2; j++)
                sum += conf[i][j][k];
            for (int j = 0; j < 2; j++)
                printf("%f ", conf[i][j][k] / (float) sum);
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");
}

void SyncDataEvaluation::evaluate() {
    for (int i = 0; i < filenames.size(); i++) {
        filename = filenames[i];

        vector<vector<float> > data = loadData2(filenames[i], 50., 1.);

        vector<int> gests = gesturesInData(data);

//        yin = new YIN(3);
//        yin->setAverageThreshold(.2);
//        yin->setDipThreshold(.1);
//        yin->setMaxLength(60);
//        yin->setMaxDelay(150);
//        yin->setMinDips(2);
//
//        float perf = syncTestProcedure(data);

        int nrOfRuns = 7;

        for (int j = 0; j < nrOfRuns; j++) {
            printf("%s, run %d of %d\n", filenames[i].c_str(), j, nrOfRuns);
            gvf = trainClassifier(data, gests, j);
            for(int k = 0; k < gvf->getNbOfTemplates(); k++)
                printf("%d ", gvf->getLengthOfTemplateByInd(k));
            printf("\n");
            testProcedure(gvf, data, gests);
        }
    }
}

void SyncDataEvaluation::optimizeGVFParameters() {
    float bestPerf = 0.f;
    float bestVal = 0.;
    float sumPerf = 0;

    float s;
    for (float s = 1.; s > .95; s -= .01) {
        alpha = s;
        sumPerf = 0.f;
        for (int i = 0; i < filenames.size(); i++) {
            filename = filenames[i];
            vector<vector<float> > data = loadData2(filename, multiplier, alpha);
            vector<int> gests = gesturesInData(data);
            gests.clear();
            gests.push_back(1);
            gests.push_back(2);
            gests.push_back(8);
            gests.push_back(13);
            for (int j = 1; j < 7; j += 7) {
                printf("%s: alpha = %f, gestNr = %d\n", filename.c_str(), alpha, j);
                gvf = trainClassifier(data, gests, j);
                sumPerf += testProcedure(gvf, data, gests);
            }
        }
        if (sumPerf > bestPerf) {
            bestPerf = sumPerf;
            bestVal = s;
        }
    }
    alpha = bestVal;

    //    for (float _icov = .65; _icov < .9; _icov += .1) {
    //        icov = _icov;
    //        sumPerf = 0.f;
    //        for (int i = 0; i < filenames.size(); i++) {
    //            filename = filenames[i];
    //            vector<vector<float> > data = loadData2(filename, multiplier, alpha);
    //            vector<int> gests = gesturesInData(data);
    //            for (int j = 1; j < 7; j += 4) {
    //                printf("%s: icov = %f, gestNr = %d\n", filename.c_str(), icov, j);
    //                gvf = trainClassifier(data, gests, j);
    //                sumPerf += testProcedure(gvf, data, gests);
    //            }
    //        }
    //        if(sumPerf > bestPerf){
    //            bestPerf = sumPerf;
    //            bestVal = _icov;
    //        }
    //    }
    //    icov = bestVal;
    //    
    //    for (float _sigOff = .00001; _sigOff <= .001; _sigOff *= 10.) {
    //        sigOff = _sigOff;
    //        setSigs();
    //        sumPerf = 0.f;
    //        for (int i = 0; i < filenames.size(); i++) {
    //            filename = filenames[i];
    //            vector<vector<float> > data = loadData2(filename);
    //            vector<int> gests = gesturesInData(data);
    //            for (int j = 1; j < 7; j += 4) {
    //                printf("%s: sigOff = %f, gestNr = %d\n", filename.c_str(), sigOff, j);
    //                gvf = trainClassifier(data, gests, j);
    //                sumPerf += testProcedure(gvf, data, gests);
    //            }
    //        }
    //        if(sumPerf > bestPerf){
    //            bestPerf = sumPerf;
    //            bestVal = _sigOff;
    //        }
    //    }
    //    sigOff = bestVal;
    //    setSigs();
    //    
    //    float s;
    //    for (s = .00001; s <= .001; s *= 10.) {
    //        sigPos = s;
    //        setSigs();
    //        sumPerf = 0.f;
    //        for (int i = 0; i < filenames.size(); i++) {
    //            filename = filenames[i];
    //            vector<vector<float> > data = loadData2(filename);
    //            vector<int> gests = gesturesInData(data);
    //            for (int j = 1; j < 7; j += 4) {
    //                printf("%s: sigPos = %f, gestNr = %d\n", filename.c_str(), sigPos, j);
    //                gvf = trainClassifier(data, gests, j);
    //                sumPerf += testProcedure(gvf, data, gests);
    //            }
    //        }
    //        if(sumPerf > bestPerf){
    //            bestPerf = sumPerf;
    //            bestVal = s;
    //        }
    //    }
    //    sigPos = bestVal;
    //    setSigs();
    //    
    //    for (s = .00001; s <= .001; s *= 10.) {
    //        sigRot = s;
    //        setSigs();
    //        sumPerf = 0.f;
    //        for (int i = 0; i < filenames.size(); i++) {
    //            filename = filenames[i];
    //            vector<vector<float> > data = loadData2(filename);
    //            vector<int> gests = gesturesInData(data);
    //            for (int j = 1; j < 7; j += 4) {
    //                printf("%s: sigRot = %f, gestNr = %d\n", filename.c_str(), sigRot, j);
    //                gvf = trainClassifier(data, gests, j);
    //                sumPerf += testProcedure(gvf, data, gests);
    //            }
    //        }
    //        if(sumPerf > bestPerf){
    //            bestPerf = sumPerf;
    //            bestVal = s;
    //        }
    //    }
    //    sigRot = bestVal;
    //    setSigs();
    //    
    //    for (s = .00001; s <= .001; s *= 10.) {
    //        sigSca = s;
    //        setSigs();
    //        sumPerf = 0.f;
    //        for (int i = 0; i < filenames.size(); i++) {
    //            filename = filenames[i];
    //            vector<vector<float> > data = loadData2(filename);
    //            vector<int> gests = gesturesInData(data);
    //            for (int j = 1; j < 7; j += 4) {
    //                printf("%s: sigSca = %f, gestNr = %d\n", filename.c_str(), sigSca, j);
    //                gvf = trainClassifier(data, gests, j);
    //                sumPerf += testProcedure(gvf, data, gests);
    //            }
    //        }
    //        if(sumPerf > bestPerf){
    //            bestPerf = sumPerf;
    //            bestVal = s;
    //        }
    //    }
    //    sigSca = bestVal;
    //    setSigs();
    //    
    //    for (s = .00001; s <= .001; s *= 10.) {
    //        sigVel = s;
    //        setSigs();
    //        sumPerf = 0.f;
    //        for (int i = 0; i < filenames.size(); i++) {
    //            filename = filenames[i];
    //            vector<vector<float> > data = loadData2(filename);
    //            vector<int> gests = gesturesInData(data);
    //            for (int j = 1; j < 7; j += 4) {
    //                printf("%s: sigVel = %f, gestNr = %d\n", filename.c_str(), sigVel, j);
    //                gvf = trainClassifier(data, gests, j);
    //                sumPerf += testProcedure(gvf, data, gests);
    //            }
    //        }
    //        if(sumPerf > bestPerf){
    //            bestPerf = sumPerf;
    //            bestVal = s;
    //        }
    //    }
    //    sigVel = bestVal;
    //    setSigs();

}