/* 
 * File:   GVFTester.cpp
 * Author: Bas
 * 
 * Created on 24 februari 2014, 11:42
 */

#include "GVFTester.h"
#include "init.h"
#include <vector>
#include "utils.h"

GVFTester::GVFTester() {
}

GVFTester::GVFTester(const GVFTester& orig) {
}

GVFTester::~GVFTester() {
}

void GVFTester::trainGesturesById(GestureVariationFollower* gvf, vector<vector<float> > data, int nrGestures) {
    for (int i = 0; i < nrGestures; i++) {
        gvf->addTemplate();
        bool recording = false;
        for (int j = 0; j < data.size(); j++) {
            vector<float> sample = data[j];
            if (sample[4] == i + 1) {
                recording = true;
                vector<float> new_sam = takeSample(data[j]);
                gvf->fillTemplate(i, new_sam);
            } else if (recording)
                break;
        }
    }
    gvf->saveTemplates("gvfTemplates.gvf");
}

bool GVFTester::contains(vector<int> vec, int val) {
    for (int i = 0; i < vec.size(); i++)
        if (vec[i] == val)
            return true;
    return false;
}

vector<int> GVFTester::gestureIndices(vector <vector <float> > data) {
    int nr = 0;
    vector<int> indices;
    for (int i = 0; i < data.size(); i++) {
        int ges = data[i][4];
        if (!contains(indices, ges))
            indices.push_back(ges);
    }
    return indices;
}

void GVFTester::testProcedure(string filename, ofstream& resultsOut) {
    float confusion[nr][nr];
    for (int i = 0; i < nr; i++)
        for (int j = 0; j < nr; j++)
            confusion[i][j] = 0;

    float positives = 0.0f, truePositives = 0.0f, trueNegatives = 0.0f;

    //    ofstream gvfOut;
    //    gvfOut.open((filename + "_results.csv").c_str());

    Eigen::VectorXf mpvrs(pdim);
    Eigen::VectorXf rpvrs(pdim);
    Eigen::VectorXf sigs(pdim);
    if (pdim == 11) {
        mpvrs << spreadMeanPos, spreadMeanVel, spreadMeanSca, spreadMeanSca, spreadMeanSca, spreadMeanOff, spreadMeanOff, spreadMeanOff, spreadMeanRot, spreadMeanRot, spreadMeanRot;
        rpvrs << spreadRangePos, spreadRangeVel, spreadRangeSca, spreadRangeSca, spreadRangeSca, spreadRangeOff, spreadRangeOff, spreadRangeOff, spreadRangeRot, spreadRangeRot, spreadRangeRot;
        sigs << sigPos, sigVel, sigSca, sigSca, sigSca, sigOff, sigOff, sigOff, sigRot, sigRot, sigRot;

    } else if (pdim == 4) {
        mpvrs << spreadMeanPos, spreadMeanVel, spreadMeanSca, spreadMeanRot;
        rpvrs << spreadRangePos, spreadRangeVel, spreadRangeSca, spreadRangeRot;
        sigs << sigPos, sigVel, sigSca, sigRot;
    }

    GestureVariationFollower* myGVF = new GestureVariationFollower(numberOfParticles, sigs, 1. / (icov * icov), resmapleThreshold, 0.);

    trainGesturesById(myGVF, data, nr);

    myGVF->spreadParticles(mpvrs, rpvrs);

    for (int j = 0; j < 50; j++)
        printf("=");
    printf("\n");
    int processBar = 0;
    int prev = 0;
    // ===================== The actual loop =====================
    for (int j = 0; j < data.size(); j++) {
        int var = j / (data.size() / 50);
        if (var > processBar) {
            processBar = var;
            printf("=");
        }


        // if gesture starts, spread particles
        int i = data.at(j).at(4);
        //        if (prev == 0 && i != 0)
        //            myGVF->spreadParticles(mpvrs, rpvrs);
        //        prev = i;

        vector<float> new_sam = takeSample(data.at(j));

        // find recognized gesture
        myGVF->infer(new_sam);
        Eigen::VectorXf stat = myGVF->getGestureConditionnalProbabilities();
        int index = 0;
        float val = -1.0f;
        for (int k = 0; k < nr; k++) {
            if (stat(k) > val) {
                index = k;
                val = stat(k);
            }
        }
        confusion[i - 1][index] += 1;

        // Get mp output
        yin->process(new_sam);
        bool sync = yin->isSync();
        if (data[j][4] > 0) {
            positives += 1.0;
            if (sync)
                truePositives += 1.0;
        } else {
            if (!sync) {
                trueNegatives += 1.0;
            }
        }

        // Printf gvf stuff
        for (int i = 0; i < nr; i++) {
            // normalize confusion matrix
            float rowTotal = 0.0;
            for (int j = 0; j < nr; j++) {
                rowTotal += confusion[i][j];
            }
            for (int j = 0; j < nr; j++) {
                if (rowTotal > 0)
                    confusion[i][j] /= rowTotal;
            }

            // print confusion matrix
            printf("\n");
            for (int j = 0; j < nr; j++) {
                printf("%.2f ", confusion[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        // get average of diagonal for performance
        float sum = 0.0f;
        for (int i = 0; i < nr; i++) {
            sum += confusion[i][i];
        }
        sum /= (float) nr;
        printf("performance = %.3f \n\n", sum);

        // write stuff to log file
        resultsOut << ";" << filename << ";" << numberOfParticles << ";" << resmapleThreshold << ";" << pdim << ";" << icov << ";";
        resultsOut << spreadMeanPos << ";" << spreadMeanVel << ";" << spreadMeanSca << ";" << spreadMeanOff << ";" << spreadMeanRot << ";";
        resultsOut << spreadRangePos << ";" << spreadRangeVel << ";" << spreadRangeSca << ";" << spreadRangeOff << ";" << spreadRangeRot << ";";
        resultsOut << sigPos << ";" << sigVel << ";" << sigSca << ";" << sigOff << ";" << sigRot << ";" << sum << "\n";
    }
}

int GVFTester::test_sync(string filename) {

    data = loadData2(filename);
    gestureVec = gestureIndices(data);
    nr = gestureVec.size();
    yin = new YIN();
    yin->setMaxDelay(150);
    yin->setMaxLength(60);
    yin->setAverageThreshold(0.7);
    yin->setDipThreshold(0.3);
    yin->clear();

    pdim = 11;

    // These all make sense, don't test
    spreadMeanPos = 0.05;
    spreadMeanVel = 1.0;
    spreadMeanSca = 1.0;
    spreadMeanOff = .1;
    spreadMeanRot = .0; // new dataset (1) .809 not highest, but is logical

    spreadRangePos = 1.2; // (5) .93
    spreadRangeVel = .3; // (4) .93
    spreadRangeSca = .3; // (3)was .5 .93        (4)
    spreadRangeOff = .2; // (2) was .5           (3) .2 -> .881
    spreadRangeRot = .4; // (1) .9 change   (2) change .4 -> .877

    sigPos = 0.00001; // (10) .936
    sigVel = 0.0001; // (9) .936
    sigSca = 0.0001; // (8) .936
    sigOff = 0.0001; // (7) .936
    sigRot = 0.0001; // (6) .923

    resmapleThreshold = 70; // 70 -> .822
    numberOfParticles = 4000;

    icov = 0.6; // .6 -> .825

    ofstream resultsOut;
    resultsOut.open("gvfOut.csv", ios::out | ios::app);
    printf("\n%s\n", filename.c_str());
    testProcedure(filename, resultsOut);
    resultsOut.close();

    return 0;
}