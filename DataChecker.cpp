/* 
 * File:   DataChecker.cpp
 * Author: Bas
 * 
 * Created on 1 april 2014, 19:07
 */

#include "DataChecker.h"

#include <vector>
#include <string>
#include <stdio.h>

DataChecker::DataChecker() {
    initValues();
}

DataChecker::DataChecker(const DataChecker& orig) {
}

DataChecker::~DataChecker() {
}

bool DataChecker::checkCorrectness(vector<string> filenames) {
    const int NROFGESTURES = 7;

    bool correct = true;
    for (int i = 0; i < filenames.size(); i++) {
        for (int skip = 0; skip < NROFGESTURES; skip++) {
            vector<vector<float> > data = loadData2(filenames[i]);
            vector<int> gestures = gesturesInData(data);
            GestureVariationFollower* gvf = trainClassifier(data, gestures, skip);
            for(int gest = 0; gest < gvf->getNbOfTemplates(); gest++){
                int length = gvf->getLengthOfTemplateByInd(gest);
                printf("filename %s: classifier %d gesture %d has length %d\n", filenames[i].c_str(), skip, gest, length);
                if(length < 10){
                    printf("template too short!\n");
                    correct = false;
                }
                printf("\n");
            }
        }
        printf("\n");
    }
    return correct;
} 