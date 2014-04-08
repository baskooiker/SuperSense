/* 
 * File:   GestureSetEvaluation.cpp
 * Author: Bas
 * 
 * Created on 6 maart 2014, 16:00
 */

#include "GestureSetEvaluation.h"

#include <vector>
#include <string>
#include "init.h"
#include <fstream>

using namespace std;

GestureSetEvaluation::GestureSetEvaluation() {
    initValues();
}

GestureSetEvaluation::GestureSetEvaluation(const GestureSetEvaluation& orig) {
}

GestureSetEvaluation::~GestureSetEvaluation() {
}

vector<int> GestureSetEvaluation::gesturesInData(vector<vector<float> > data) {
    vector<int> gests;
    for (int i = 0; i < data.size(); i++) {
        int g = data[i][3];
        if (!contains(gests, g) && g != 0)
            gests.push_back(g);
    }
    std::sort(gests.begin(), gests.end());
    return gests;
}

void GestureSetEvaluation::setSigs() {
    sigs = Eigen::VectorXf(pdim);
    sigs << sigPos, sigVel, sigSca, sigSca, sigSca, sigOff, sigOff, sigOff, sigRot, sigRot, sigRot;
    vector<float> sigs2 = vector<float>(pdim);
    sigs2.push_back(sigPos);
    sigs2.push_back(sigVel);
    sigs2.push_back(sigSca);
    sigs2.push_back(sigSca);
    sigs2.push_back(sigSca);
    sigs2.push_back(sigOff);
    sigs2.push_back(sigOff);
    sigs2.push_back(sigOff);
    sigs2.push_back(sigRot);
    sigs2.push_back(sigRot);
    sigs2.push_back(sigRot);
    gvf->setAdaptSpeed(sigs2);
}

void GestureSetEvaluation::initValues() {
    pdim = 11;

    spreadMeanPos = 0.05;
    spreadMeanVel = 1.0;
    spreadMeanSca = 1.0;
    spreadMeanOff = .1;
    spreadMeanRot = .0; // new dataset (1) .809 not highest, but is logical

    mpvrs = Eigen::VectorXf(pdim);
    mpvrs << spreadMeanPos, spreadMeanVel, spreadMeanSca, spreadMeanSca, spreadMeanSca, spreadMeanOff, spreadMeanOff, spreadMeanOff, spreadMeanRot, spreadMeanRot, spreadMeanRot;

    spreadRangePos = 1.2; // (5) .93
    spreadRangeVel = .3; // (4) .93
    spreadRangeSca = .3; // (3)was .5 .93        (4)
    spreadRangeOff = 0.; // (2) was .5           (3) .2 -> .881
    spreadRangeRot = 2.; // (1) .9 change   (2) change .4 -> .877

    rpvrs = Eigen::VectorXf(pdim);
    rpvrs << spreadRangePos, spreadRangeVel, spreadRangeSca, spreadRangeSca, spreadRangeSca, spreadRangeOff, spreadRangeOff, spreadRangeOff, spreadRangeRot, spreadRangeRot, spreadRangeRot;

    sigPos = 0.00001; // (10) .936
    sigVel = 0.0001; // (9) .936
    sigSca = 0.0001; // (8) .936
    sigOff = 0.0001; // (7) .936
    sigRot = 0.0001; // (6) .923

    sigs = Eigen::VectorXf(pdim);
    sigs << sigPos, sigVel, sigSca, sigSca, sigSca, sigOff, sigOff, sigOff, sigRot, sigRot, sigRot;

    resmapleThreshold = 800; // 70 -> .822
    numberOfParticles = 4000;

    icov = 0.75; // .6 -> .825

    gvf = new GestureVariationFollower(numberOfParticles, sigs, 1. / (icov * icov), resmapleThreshold, 0.);

    filename = "realDataBas14times5gestures.dat";
    gestNumber = 0;

    alpha = 1.;
    multiplier = 50.;

    outputFilename = "GestureSetTest4.csv";
    totalNrGest = 20;

    skip = 0;

    from = -1;
    to = -1;
    
    everySample = false;
}

GestureVariationFollower* GestureSetEvaluation::trainClassifier(vector<vector<float> > data, vector<int> gs, int skip) {
    gestNumber = skip;
    delete gvf;

    sigs = Eigen::VectorXf(pdim);
    sigs << sigPos, sigVel, sigSca, sigSca, sigSca, sigOff, sigOff, sigOff, sigRot, sigRot, sigRot;

    mpvrs = Eigen::VectorXf(pdim);
    mpvrs << spreadMeanPos, spreadMeanVel, spreadMeanSca, spreadMeanSca, spreadMeanSca, spreadMeanOff, spreadMeanOff, spreadMeanOff, spreadMeanRot, spreadMeanRot, spreadMeanRot;

    rpvrs = Eigen::VectorXf(pdim);
    rpvrs << spreadRangePos, spreadRangeVel, spreadRangeSca, spreadRangeSca, spreadRangeSca, spreadRangeOff, spreadRangeOff, spreadRangeOff, spreadRangeRot, spreadRangeRot, spreadRangeRot;

    gvf = new GestureVariationFollower(numberOfParticles, sigs, 1. / (icov * icov), resmapleThreshold, 0.);
    for (int i = 0; i < gs.size(); i++) {
        gvf->addTemplate();
        bool gestureStarted = false;
        bool gestureDone = false;
        int count = 0;
        for (int j = 0; j < data.size() && !gestureDone; j++) {
            vector<float> sample = data[j];
            if (gestureStarted) {
                if (((int) sample[4]) == 1) {
                    gestureDone = true;
                    count = 0;
                    break;
                }
                vector<float> newSample;
                for (int k = 0; k < 3; k++)
                    newSample.push_back(sample[k]);
                gvf->fillTemplate(i, newSample);
            } else {
                if (((int) sample[3]) == gs[i] && ((int) sample[4]) == 1) {
                    if (count < skip)
                        count++;
                    else {
                        vector<float> newSample;
                        for (int k = 0; k < 3; k++)
                            newSample.push_back(sample[k]);
                        gvf->fillTemplate(i, newSample);
                        gestureStarted = true;
                    }
                }
            }
        }
    }
    return gvf;
}

string GestureSetEvaluation::gestureSetString(vector<int> v) {
    string returnstring = "";
    char buffer [33];
    for (int i = 0; i < v.size(); i++) {
        if (i > 0)
            returnstring += " ";
        returnstring += itoa(v[i], buffer, 10);
    }
    return returnstring;
}

int GestureSetEvaluation::getClassifiedGesture(int nr) {
    Eigen::VectorXf stat = gvf->getGestureConditionnalProbabilities();
    int index = 0;
    float val = -1.0f;
    for (int k = 0; k < nr; k++) {
        if (stat(k) > val) {
            index = k;
            val = stat(k);
        }
    }
    return index;
}

void GestureSetEvaluation::inferSample(vector<vector<float> > data, int j) {
    vector<float> newSample;
    for (int i = 0; i < 3; i++)
        newSample.push_back(data[j][i]);
    gvf->infer(newSample);
}

float GestureSetEvaluation::testProcedure(GestureVariationFollower* _gvf, vector<vector<float> > data, vector<int> gestureSet) {
    int nr = gestureSet.size();
//    float confusion[totalNrGest][totalNrGest];
//    confusion = float[totalNrGest][totalNrGest];
    confusion = (float**)calloc(totalNrGest, sizeof(float*));
    for (int i = 0; i < totalNrGest; i++){
        confusion[i] = (float*)calloc(totalNrGest,sizeof(float));
        for (int j = 0; j < totalNrGest; j++)
            confusion[i][j] = 0.f;
    }

    gvf = _gvf;
    gvf->spreadParticles(mpvrs, rpvrs);

    int currentGest = 0;
    // ===================== The actual loop =====================
    for (int j = 0; j < data.size(); j++) {

        // if gesture starts, spread particles
        int indicator = (int) data.at(j).at(4);
        if (indicator == 0) {
            if (currentGest > 0) {
                if (everySample) {
                    int index = getClassifiedGesture(nr);
                    confusion[currentGest - 1][gestureSet[index] - 1] += 1;
                }
                inferSample(data, j);
            }
        } else {
            if (currentGest > 0) {
                int index = getClassifiedGesture(nr);
                confusion[currentGest - 1][gestureSet[index] - 1] += 1;
            }

            int gestVal = (int) data.at(j).at(3);
            if (contains(gestureSet, gestVal)) {
                gvf->spreadParticles(mpvrs, rpvrs);
                currentGest = gestVal;
                inferSample(data, j);
            }
        }

        if (j > 0) {
            if (((int) data.at(j).at(3)) != ((int) data.at(j - 1).at(3))) {
                currentGest = 0;
            }
        }
    }

    // Printf gvf stuff
        for (int i = 0; i < totalNrGest; i++) {
            //         normalize confusion matrix
            if (contains(gestureSet, i + 1)) {
                float rowTotal = 0.0;
                for (int j = 0; j < totalNrGest; j++) {
                    rowTotal += confusion[i][j];
                }
                for (int j = 0; j < totalNrGest; j++) {
                    if (rowTotal > 0)
                        confusion[i][j] /= rowTotal;
                } // print confusion matrix
                printf("\n");
                for (int j = 0; j < gestureSet.size(); j++) {
                    printf("%.2f ", confusion[i][gestureSet[j] - 1]);
                }
                printf("\n");
            }

        }
        printf("\n");

    // get average of diagonal for performance
    sum = 0.0f;
    for (int i = 0; i < totalNrGest; i++) {
        if (contains(gestureSet, i + 1));
        sum += confusion[i][i];
    }
    sum /= (float) nr;
    printf("performance = %.3f \n\n", sum);

    writeResults(gestureSet);
    return sum;
}

vector<vector<int> > GestureSetEvaluation::getGestureSets() {
    vector<vector<int> > gestureSets;
    vector<int>* gs1 = new vector<int>;
    gs1->push_back(1);
    gs1->push_back(2);
    gs1->push_back(3);
    gs1->push_back(4);
    //    gestureSets.push_back(*gs1);

    gs1 = new vector<int>;
    gs1->push_back(5);
    gs1->push_back(6);
    gs1->push_back(7);
    gs1->push_back(8);
    gestureSets.push_back(*gs1);

    gs1 = new vector<int>;
    gs1->push_back(9);
    gs1->push_back(10);
    gs1->push_back(11);
    gs1->push_back(12);
    gestureSets.push_back(*gs1);

    gs1 = new vector<int>;
    gs1->push_back(3);
    gs1->push_back(4);
    gs1->push_back(13);
    gs1->push_back(14);
    //    gestureSets.push_back(*gs1);

    gs1 = new vector<int>;
    gs1->push_back(1);
    gs1->push_back(2);
    gs1->push_back(3);
    gs1->push_back(4);
    gs1->push_back(5);
    gs1->push_back(6);
    gs1->push_back(7);
    gs1->push_back(8);
    gs1->push_back(9);
    gs1->push_back(10);
    gs1->push_back(11);
    gs1->push_back(12);
    gs1->push_back(13);
    gs1->push_back(14);
    gestureSets.push_back(*gs1);

    return gestureSets;
}

vector<vector<int> > GestureSetEvaluation::setsOf3(int from, int to) {
    vector<vector<int> > gestureSets;

    for (int i = from; i <= to; i++) {
        for (int j = i + 1; j <= to; j++) {
            for (int k = j + 1; k <= to; k++) {
                vector<int> gs = vector<int>();
                gs.push_back(i);
                gs.push_back(j);
                gs.push_back(k);
                gestureSets.push_back(gs);
            }
        }
    }

    return gestureSets;
}

vector<vector<int> > GestureSetEvaluation::setsOf2(int from, int to) {
    vector<vector<int> > gestureSets;
    for (int i = from; i <= to; i++) {
        for (int j = i + 1; j <= to; j++) {
            vector<int>* gs = new vector<int>;
            gs->push_back(i);
            gs->push_back(j);
            gestureSets.push_back(*gs);
        }
    }
    return gestureSets;
}

vector<vector<int> > GestureSetEvaluation::add1(vector<int> v) {
    vector<vector<int> > gs;
    for (int i = 1; i <= totalNrGest; i++) {
        if (!contains(v, i)) {
            bool pushed = false;
            vector<int> newV;
            for (int j = 0; j < v.size(); j++) {
                if (v[j] > i && !pushed) {
                    newV.push_back(i);
                    pushed = true;
                }
                newV.push_back(v[j]);
            }
            if (!pushed)
                newV.push_back(i);
            gs.push_back(newV);
        }
    }
    return gs;
}

bool GestureSetEvaluation::contains(vector<int> v, int e) {
    for (int i = 0; i < v.size(); i++)
        if (v[i] == e)
            return true;
    return false;
}

vector<int> GestureSetEvaluation::newPair(int i, int j) {
    vector<int> pair = vector<int>();
    pair.push_back(i);
    pair.push_back(j);
    return pair;
}

vector<int> GestureSetEvaluation::newTri(int i, int j, int k) {
    vector<int> pair = vector<int>();
    pair.push_back(i);
    pair.push_back(j);
    pair.push_back(k);
    return pair;
}

vector<int> GestureSetEvaluation::newQuad(int i, int j, int k, int l) {
    vector<int> pair = vector<int>();
    pair.push_back(i);
    pair.push_back(j);
    pair.push_back(k);
    pair.push_back(l);
    return pair;
}

vector<int> GestureSetEvaluation::newSet(int i, int j, int k, int l, int m) {
    vector<int> pair = vector<int>();
    pair.push_back(i);
    pair.push_back(j);
    pair.push_back(k);
    pair.push_back(l);
    pair.push_back(m);
    return pair;
}

vector<int> GestureSetEvaluation::newSet(int i, int j, int k, int l, int m, int n) {
    vector<int> pair = vector<int>();
    pair.push_back(i);
    pair.push_back(j);
    pair.push_back(k);
    pair.push_back(l);
    pair.push_back(m);
    pair.push_back(n);
    return pair;
}

vector<int> GestureSetEvaluation::newSet(int i, int j, int k, int l, int m, int n, int o) {
    vector<int> pair = vector<int>();
    pair.push_back(i);
    pair.push_back(j);
    pair.push_back(k);
    pair.push_back(l);
    pair.push_back(m);
    pair.push_back(n);
    pair.push_back(o);
    return pair;
}

void GestureSetEvaluation::addPairs(vector<vector<int> >* pairs, vector<int> pair) {
    vector<vector<int> > tris = add1(pair);
    for (int i = 0; i < tris.size(); i++)
        pairs->push_back(tris[i]);
}

vector<vector<int> > GestureSetEvaluation::getBestPairsExtended() {
    vector<vector<int> >* pairs = new vector<vector<int> >();
    //    addPairs(pairs, newSet(11,12,18,19,20));
    //    addPairs(pairs, newSet(8, 12, 18, 19, 20));
    //    addPairs(pairs, newSet(9, 14, 18, 19, 20));
    //    addPairs(pairs, newSet(9, 11, 12, 19, 20));
    //    addPairs(pairs, newSet(12, 17, 18, 19, 20));
    //    addPairs(pairs, newSet(9, 17, 18, 19, 20));
    //    addPairs(pairs, newSet(10, 11, 12, 19, 20));
    //    addPairs(pairs, newSet(12, 14, 18, 19, 20));
    //    addPairs(pairs, newSet(9, 12, 18, 19, 20));
    //    addPairs(pairs, newSet(10, 12, 18, 19, 20));


    //    addPairs(pairs, newSet(10, 12, 14, 18, 19, 20));
    //    addPairs(pairs, newSet(9, 14, 17, 18, 19, 20));
    //    addPairs(pairs, newSet(8, 12, 17, 18, 19, 20));
    //    addPairs(pairs, newSet(9, 10, 11, 12, 19, 20));
    //    addPairs(pairs, newSet(10, 12, 17, 18, 19, 20));
    //    addPairs(pairs, newSet(9, 10, 12, 18, 19, 20));
    //    addPairs(pairs, newSet(10, 11, 12, 18, 19, 20));
    //    addPairs(pairs, newSet(12, 14, 17, 18, 19, 20));
    //    addPairs(pairs, newSet(9, 12, 14, 18, 19, 20));
    //    addPairs(pairs, newSet(9, 12, 17, 18, 19, 20));

    addPairs(pairs, newSet(1, 9, 12, 17, 18, 19, 20));
    addPairs(pairs, newSet(9, 12, 16, 17, 18, 19, 20));
    addPairs(pairs, newSet(9, 11, 12, 17, 18, 19, 20));
    addPairs(pairs, newSet(3, 10, 12, 14, 18, 19, 20));
    addPairs(pairs, newSet(10, 12, 14, 17, 18, 19, 20));
    addPairs(pairs, newSet(6, 9, 12, 14, 18, 19, 20));
    addPairs(pairs, newSet(3, 9, 12, 14, 18, 19, 20));
    addPairs(pairs, newSet(9, 10, 11, 12, 18, 19, 20));
    addPairs(pairs, newSet(9, 10, 12, 14, 18, 19, 20));
    addPairs(pairs, newSet(9, 12, 14, 17, 18, 19, 20));

    return *pairs;
}

void removeDuplicates(vector<vector<int> >* sets) {
    for (int i = 0; i < sets->size(); i++) {
        for (int j = i + 1; j < sets->size(); j++) {
            bool dif = false;
            for (int k = 0; (k < sets->at(i).size()) && !dif; k++) {
                if (sets->at(i)[k] != sets->at(j)[k]) {
                    dif = true;
                }
            }
            if (!dif) {
                sets->erase((sets->begin()) + j);
                j--;
            }
        }
    }
}

void GestureSetEvaluation::evaluatePairs() {
    vector<vector<int> > sets = getBestPairsExtended();
    printf("there are %d pairs\n", sets.size());
    removeDuplicates(&sets);
    printf("there are %d pairs\n", sets.size());
    //    for (int i = 0; i < sets.size(); i++) {
    //        for (int j = 0; j < sets[i].size(); j++)
    //            printf("%d ", sets[i][j]);
    //        printf("\n");
    //    }
    //    printf("there are %d pairs\n", sets.size());

    vector<vector<float> > data = loadData2(filename);

    from = from == -1 ? 0 : from;
    from = from > sets.size() - 1 ? sets.size() - 1 : from;
    to = to == -1 ? sets.size() - 1 : to;
    to = to > sets.size() - 1 ? sets.size() - 1 : to;

    //    printf("from = %d to = %d\n", from, to);

    //    for (skip = 1; skip < 8; skip += 2) {
    for (int i = from; i <= to; i++) {
        printf("gestureset %d of %d, %s, skip = %d\n", i, to, gestureSetString(sets[i]).c_str(), skip);
        GestureVariationFollower* gvf = trainClassifier(data, sets[i], skip);
        printf("\n");
        testProcedure(gvf, data, sets[i]);
    }
    //    }

}

void GestureSetEvaluation::evaluate() {
    vector<vector<int> > gs;
    vector<int> s1;
    s1.push_back(3);
    s1.push_back(4);

    vector<vector<float> > data = loadData2(filename);

    float bestP = 0.f;
    vector<int> best = s1;

    for (int adds = 0; adds < 6; adds++) {
        bestP = 0.f;
        gs = add1(best);

        for (int i = 0; i < gs.size(); i++) {
            printf("gesture set %d, iteration %d\n", i, adds);
            GestureVariationFollower* gvf = trainClassifier(data, gs[i]);
            for (int j = 0; j < gvf->getNbOfTemplates(); j++) {
                printf("gesture length %d = %d\n", gs[i][j], gvf->getLengthOfTemplateByInd(j));
            }
            printf("\n");
            float p = testProcedure(gvf, data, gs[i]);

            if (p > bestP) {
                bestP = p;
                best = gs[i];
            }
        }
    }

}

void GestureSetEvaluation::setFilename(string fn) {
    filename = fn;
}

void GestureSetEvaluation::setOutputFilename(string fn) {
    outputFilename = fn;
}

void GestureSetEvaluation::setTotalNrGest(int t) {
    totalNrGest = t;
}

void GestureSetEvaluation::setSkip(int t) {
    skip = t;
}

void GestureSetEvaluation::setFrom(int t) {
    from = t;
}

void GestureSetEvaluation::setTo(int t) {
    to = t;
}

void GestureSetEvaluation::writeResults(vector<int> gestureSet){
    
    // write results to file
    FILE* f;
    do {
        f = fopen(outputFilename.c_str(), "a");
    } while (!f);
    out.open(outputFilename.c_str(), fstream::out | fstream::app);
    out << "\"" << filename << "\"" << ";";
    out << gestNumber << ";";
    out << "\"" << gestureSetString(gestureSet) << "\"" << ";" << gestureSet.size() << ";" << sum;

    out << ";" << numberOfParticles << ";" << resmapleThreshold << ";" << pdim << ";" << icov << ";";
    out << alpha << ";" << multiplier << ";";
    out << spreadMeanPos << ";" << spreadMeanVel << ";" << spreadMeanSca << ";" << spreadMeanOff << ";" << spreadMeanRot << ";";
    out << spreadRangePos << ";" << spreadRangeVel << ";" << spreadRangeSca << ";" << spreadRangeOff << ";" << spreadRangeRot << ";";
    out << sigPos << ";" << sigVel << ";" << sigSca << ";" << sigOff << ";" << sigRot << ";" << sum;

    for (int i = 0; i < totalNrGest; i++) {
        if (contains(gestureSet, i + 1))
            out << ";" << 1;
        else
            out << ";" << 0;
    }
    for (int i = 0; i < totalNrGest; i++) {
        for (int j = 0; j < totalNrGest; j++) {
            out << ";" << confusion[i][j];
        }
    }
    out << "\n";
    out.close();
    fclose(f);

}

