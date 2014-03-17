/* 
 * File:   GVFTester.h
 * Author: Bas
 *
 * Created on 24 februari 2014, 11:42
 */

#ifndef GVFTESTER_H
#define	GVFTESTER_H

#include <vector>
#include "../yin/YIN.h"
#include <fstream>
#include "GestureVariationFollower.h"

using namespace std;

class GVFTester {
public:
    GVFTester();
    GVFTester(const GVFTester& orig);
    virtual ~GVFTester();
    void testProcedure(string filename, ofstream& resultsOut);
    int test_sync(string filename);
    void trainGesturesById(GestureVariationFollower* gvf, vector<vector<float> > data, int nrGestures);
    
    vector <vector <float> > data;
    int nr;

    YIN* yin;

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

private:
    vector<int> gestureVec;
    
    bool contains(vector<int> vec, int val);
    vector<int> gestureIndices(vector <vector <float> > data);
};

#endif	/* GVFTESTER_H */

