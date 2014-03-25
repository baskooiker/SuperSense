/* 
 * File:   SyncDataEvaluation.h
 * Author: Bas
 *
 * Created on 13 maart 2014, 14:59
 */

#ifndef SYNCDATAEVALUATION_H
#define	SYNCDATAEVALUATION_H

#include "GestureSetEvaluation.h"
#include "../YIN/YIN.h"

#include <vector>
#include <string>

using namespace std;

class SyncDataEvaluation : public GestureSetEvaluation {
public:
    SyncDataEvaluation(vector<string> fn);
    SyncDataEvaluation(const SyncDataEvaluation& orig);
    virtual ~SyncDataEvaluation();
    void evaluate();
    void optimizeGVFParameters();
    
protected:
    YIN* yin;
    float syncTestProcedure(vector<vector<float> > data);
    
    vector<int> gesturesInData(vector<vector<float> > data);
};

#endif	/* SYNCDATAEVALUATION_H */

