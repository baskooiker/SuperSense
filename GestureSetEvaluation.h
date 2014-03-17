/* 
 * File:   GestureSetEvaluation.h
 * Author: Bas
 *
 * Created on 6 maart 2014, 16:00
 */

#ifndef GESTURESETEVALUATION_H
#define	GESTURESETEVALUATION_H

#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "init.h"

using namespace std;

class GestureSetEvaluation {
public:
    GestureSetEvaluation();
    GestureSetEvaluation(const GestureSetEvaluation& orig);
    virtual ~GestureSetEvaluation();
    
        
//    int numberOfGestureSets;
//    int numberOfGestures;
    virtual void evaluate();
    
protected:
    GestureVariationFollower* trainClassifier(vector<vector<float> > data, vector<int> gs, int skip = 0);
    float testProcedure(GestureVariationFollower* gvf, vector<vector<float> > data, vector<int> gestureSet);
    string gestureSetString(vector<int> v);
    bool contains(vector<int> v, int e);
    void initValues();
    
    void setSigs();
    
    vector<vector<int> > getGestureSets();
    vector<vector<int> > setsOf2();
    vector<vector<int> > setsOf3();
    vector<vector<int> > add1(vector<int> v);
    
    Eigen::VectorXf mpvrs;
    Eigen::VectorXf rpvrs;
    Eigen::VectorXf sigs;
    
    int pdim;

    float spreadMeanPos;
    float spreadMeanVel;
    float spreadMeanSca;
    float spreadMeanOff;
    float spreadMeanRot;

    float spreadRangePos;
    float spreadRangeVel;
    float spreadRangeSca;
    float spreadRangeOff;
    float spreadRangeRot;

    float sigPos;
    float sigVel;
    float sigSca;
    float sigOff;
    float sigRot;

    int resmapleThreshold;
    int numberOfParticles;

    float icov;

    GestureVariationFollower* gvf;
    
    string filename;
    int gestNumber;
    ofstream out;
};

#endif	/* GESTURESETEVALUATION_H */

