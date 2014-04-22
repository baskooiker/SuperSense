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
    void evaluatePairs();
    void setFilename(string fn);
    void setOutputFilename(string fn);
    void setTotalNrGest(int t);
    void setSkip(int t);
    void setTo(int t);
    void setFrom(int t);
    void printConfusion();

    bool everySample;

protected:
    vector<string> filenames;
    GestureVariationFollower* trainClassifier(vector<vector<float> > data, vector<int> gs, int skip = 0);
    float testProcedure(GestureVariationFollower* gvf, vector<vector<float> > data, vector<int> gestureSet);
    string gestureSetString(vector<int> v);
    bool contains(vector<int> v, int e);
    int getClassifiedGesture(int nr);
    void inferSample(vector<vector<float> > data, int j);
    void initValues();
    void writeResults();

    void setSigs();

    vector<vector<int> > getGestureSets();
    vector<vector<int> > setsOf2(int from, int to);
    vector<vector<int> > setsOf3(int from, int to);
    vector<vector<int> > add1(vector<int> v);
    vector<int> newPair(int i, int j);
    vector<int> newTri(int i, int j, int k);
    vector<int> newQuad(int i, int j, int k, int l);
    vector<int> newSet(int i, int j, int k, int l, int m);
    vector<int> newSet(int i, int j, int k, int l, int m, int n);
    vector<int> newSet(int i, int j, int k, int l, int m, int n, int o);
    void addPairs(vector<vector<int> >* pairs, vector<int> pair);
    vector<vector<int> > getBestPairsExtended();
    vector<int> gesturesInData(vector<vector<float> > data);

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

    float multiplier;
    float alpha;

    string outputFilename;
    int totalNrGest;
    int skip;
    int from;
    int to;

    float** confusion;
    
    float sum;
    
    string classifierType;
    string trainFilename;
    
    vector<int> gestureSet;
};

#endif	/* GESTURESETEVALUATION_H */

