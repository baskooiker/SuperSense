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
    spreadRangeOff = 0.2; // (2) was .5           (3) .2 -> .881
    spreadRangeRot = .4; // (1) .9 change   (2) change .4 -> .877

    rpvrs = Eigen::VectorXf(pdim);
    rpvrs << spreadRangePos, spreadRangeVel, spreadRangeSca, spreadRangeSca, spreadRangeSca, spreadRangeOff, spreadRangeOff, spreadRangeOff, spreadRangeRot, spreadRangeRot, spreadRangeRot;

    sigPos = 0.00001; // (10) .936
    sigVel = 0.0001; // (9) .936
    sigSca = .0001; // (8) .936
    sigOff = 0.0001; // (7) .936
    sigRot = 0.0001; // (6) .923

    sigs = Eigen::VectorXf(pdim);
    sigs << sigPos, sigVel, sigSca, sigSca, sigSca, sigOff, sigOff, sigOff, sigRot, sigRot, sigRot;

    resmapleThreshold = 800; // 70 -> .822
    numberOfParticles = 4000;

    icov = 0.75; // .6 -> .825

    gvf = new GestureVariationFollower(numberOfParticles, sigs, 1. / (icov * icov), resmapleThreshold, 0.);

    filename = "";
    gestNumber = 0;
}

GestureVariationFollower* GestureSetEvaluation::trainClassifier(vector<vector<float> > data, vector<int> gs, int skip) {
    gestNumber = skip;
    delete gvf;
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
                    newSample.push_back(sample[k] * 50. - 25.);
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

float GestureSetEvaluation::testProcedure(GestureVariationFollower* gvf, vector<vector<float> > data, vector<int> gestureSet) {
    int nr = gestureSet.size();
    int totalNrGest = 14;
    float confusion[totalNrGest][totalNrGest];
    for (int i = 0; i < totalNrGest; i++)
        for (int j = 0; j < totalNrGest; j++)
            confusion[i][j] = 0.f;

    gvf->spreadParticles(mpvrs, rpvrs);

    for (int j = 0; j < 50; j++)
        printf("=");
    printf("\n");
    int processBar = 0;
    int currentGest = 0;
    //    int currentIndex = -1;

    // ===================== The actual loop =====================
    for (int j = 0; j < data.size(); j++) {
        int var = j / (data.size() / 50);
        if (var > processBar) {
            processBar = var;
            printf("=");
        }

        vector<float> newSample;
        for (int i = 0; i < 3; i++)
            newSample.push_back(data[j][i] * 50. - 25.);
        gvf->infer(newSample);

        // if gesture starts, spread particles
        int indicator = (int) data.at(j).at(4);
        if (indicator == 0) {
            if (currentGest > 0) {

                Eigen::VectorXf stat = gvf->getGestureConditionnalProbabilities();
                int index = 0;
                float val = -1.0f;
                for (int k = 0; k < nr; k++) {
                    if (stat(k) > val) {
                        index = k;
                        val = stat(k);
                    }
                }
                confusion[currentGest - 1][gestureSet[index] - 1] += 1;
            }
        } else {
            gvf->spreadParticles(mpvrs, rpvrs);
            currentGest = (int) data.at(j).at(3);

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
    float sum = 0.0f;
    for (int i = 0; i < totalNrGest; i++) {
        if (contains(gestureSet, i + 1));
        sum += confusion[i][i];
    }
    sum /= (float) nr;
    printf("performance = %.3f \n\n", sum);

    // write results to file
    out.open("GestureSetTest3.csv", fstream::out | fstream::app);
    out << "\"" << filename << "\"" << ";";
    out << gestNumber << ";";
    out << "\"" << gestureSetString(gestureSet) << "\"" << ";" << gestureSet.size() << ";" << sum;

    out << ";" << numberOfParticles << ";" << resmapleThreshold << ";" << pdim << ";" << icov << ";";
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

vector<vector<int> > GestureSetEvaluation::setsOf3() {
    vector<vector<int> > gestureSets;

    for (int i = 1; i <= 14; i++) {
        for (int j = i + 1; j <= 14; j++) {
            for (int k = j + 1; k <= 14; k++) {
                vector<int>* gs = new vector<int>;
                gs->push_back(i);
                gs->push_back(j);
                gs->push_back(k);
                gestureSets.push_back(*gs);
            }
        }
    }

    return gestureSets;
}

vector<vector<int> > GestureSetEvaluation::setsOf2() {
    vector<vector<int> > gestureSets;
    for (int i = 1; i <= 14; i++) {
        for (int j = i + 1; j <= 14; j++) {
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
    for (int i = 1; i <= 14; i++) {
        bool isInV = false;
        for (int j = 0; j < v.size() && !isInV; j++) {
            isInV = i == v[j];
        }
        if (!isInV) {
            vector<int> newV;
            for (int j = 0; j < v.size(); j++)
                newV.push_back(v[j]);
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

void GestureSetEvaluation::evaluate() {
    vector<vector<int> > gs;
    vector<int> s1;
    s1.push_back(3);
    s1.push_back(4);
    filename = "realDataBas14times5gestures.dat";

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

