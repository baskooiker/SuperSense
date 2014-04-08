/* 
 * File:   DTWNNTester.h
 * Author: Bas
 *
 * Created on 3 april 2014, 11:59
 */

#ifndef DTWNNTESTER_H
#define	DTWNNTESTER_H

#include "../lbimproved/vectordtw.h"
#include "GestureSetEvaluation.h"

#include <string>
#include <stdio.h>
#include <vector>
#include <float.h>
#include <map>

using namespace std;

class DTWNNTester : public GestureSetEvaluation {
public:

    DTWNNTester() {
        initValues();
    }

    DTWNNTester(const DTWNNTester& orig) {

    }

    virtual ~DTWNNTester() {

    }

    static const int NROFTRIALS = 7;

    void evaluate(string filename) {
        vector<vector<vector<Point> > > trials = createTemplates(filename);
        printf("templates created\n");

        int conf[trials.size()][trials.size()];
        for (int i = 0; i < trials.size(); i++)
            for (int j = 0; j < trials.size(); j++)
                conf[i][j] = 0;

        // for every trial (1-8) create 20 templates and test all other trials on these templates.
        for (int trial = 0; trial < NROFTRIALS; trial--) {
            printf("trial = %d\n", trial);
            vector<VectorDTW> dtwBank;
            for (int i = 0; i < trials.size(); i++)
                dtwBank.push_back(VectorDTW(trials[i][trial].size(), 1));

            for (int gest = 0; gest < trials.size(); gest++) {
                for (int i = 0; i < NROFTRIALS; i++) {
                    printf("i = %d\n", i);
                    if (i != trial) {
                        int minIndex = -1;
                        double minVal = DBL_MAX;
                        for (int j = 0; j < trials.size(); j++) {
                            double d = dtwBank[j].fastdynamic(trials[gest][trial], trials[j][i]);
                            if (d < minVal) {
                                minVal = d;
                                minIndex = j;
                            }
                            //                            printf("gest %d on template %d: %f\n", j, gest, d);
                        }
                        conf[gest][minIndex] += 1;
                    }
                }
            }
        }
        for (int i = 0; i < trials.size(); i++) {
            for (int j = 0; j < trials.size(); j++) {
                printf("%d ", conf[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        printf("Done\n");
    }

    void evaluateOnFirst(vector<string> filenames) {
        map<int, vector<vector<Point> > > templates = createAnotatedTemplates(filenames[0]);

        vector<int> gests = gesturesInData(loadData2(filenames[0], 50., 1.));

        //                printf("gests.size() = %d\n", gests.size());
        //                for(int i = 0; i < gests.size(); i++)
        //                    printf("%d ", gests[i]);
        //                printf("\n");

        // init confusion matrix
        confusion = (float**) calloc(totalNrGest, sizeof (float*));
        for (int i = 0; i < totalNrGest; i++)
            confusion[i] = (float*) calloc(totalNrGest, sizeof (float));


        for (int gestIt = 0; gestIt < NROFTRIALS; gestIt++) {
            skip = gestIt;
            vector<VectorDTW> dtwBank;
            for (int i = 0; i < templates.size(); i++) {
                dtwBank.push_back(VectorDTW(templates[gests[i]][gestIt].size(), 1));
            }

            for (int i = 0; i < filenames.size(); i++) {
                filename = filenames[i];

                // reset confusion matrix
                for (int i = 0; i < totalNrGest; i++) {
                    for (int j = 0; j < totalNrGest; j++)
                        confusion[i][j] = 0.f;
                }

                map<int, vector<vector<Point> > > trials = createAnotatedTemplates(filenames[i]);
                for (int j = 0; j < NROFTRIALS; j++) {
                    //                                    printf("in trial %d\n", j);
                    for (int l = 0; l < gests.size(); l++) {
                        //                    printf("in gesture %d\n", l);
                        int minIndex = -1;
                        double minVal = DBL_MAX;
                        for (int k = 0; k < gests.size(); k++) {
                            //                        double d = 1.;
                            double d = dtwBank[k].fastdynamic(templates[gests[k]][gestIt], trials[gests[l]][j]);
                            if (d < minVal) {
                                minVal = d;
                                minIndex = k;
                            }
                        }
                        confusion[l][minIndex] += 1;
                    }
                }


                // Printf confusion
                for (int i = 0; i < templates.size(); i++) {
                    float rowTotal = 0.f;
                    for (int j = 0; j < templates.size(); j++) {
                        rowTotal += confusion[i][j];
                    }
                    //            printf("rowtotal = %f\n", rowTotal);
                    for (int j = 0; j < templates.size(); j++) {
                        if (rowTotal > 0.f)
                            confusion[i][j] /= rowTotal;
                    }
                    // print confusion matrix
                    printf("\n");
                    for (int j = 0; j < templates.size(); j++) {
                        printf("%.2f ", confusion[i][j]);
                    }
                    printf("\n");
                }

                printf("\n");

                // get average of diagonal for performance
                sum = 0.f;
                for (int i = 0; i < templates.size(); i++) {
                    sum += confusion[i][i];
                }
                sum /= (float) templates.size();
                printf("performance = %.3f \n\n", sum);

                writeResults(gests);

            }
        }

    }

private:

    vector<vector<vector<Point> > > createTemplates(string filename) {
        vector<vector<vector<Point> > > trials;
        vector<vector<float> > data = loadData2(filename);

        bool inTrial = false;
        int trialNr = -1;
        int gestNr = -1;
        for (int sampleNr = 0; sampleNr < data.size(); sampleNr++) {
            vector<float> sample = data[sampleNr];
            bool newTrial = (int) sample[4] == 1;
            int sampleGest = (int) sample[3];
            Point p = Point(sample[0], sample[1], sample[2]);
            if (inTrial) {
                if (newTrial) {
                    if (++trialNr < NROFTRIALS) {
                        trials[trials.size() - 1].push_back(vector<Point>());
                        trials[trials.size() - 1][trials[trials.size() - 1].size() - 1].push_back(p);
                    } else {
                        inTrial = false;
                    }
                } else {
                    trials[trials.size() - 1][trials[trials.size() - 1].size() - 1].push_back(p);
                }
            } else {
                if (newTrial) {
                    trialNr = 0;
                    inTrial = true;
                    trials.push_back(vector<vector<Point> >());
                    trials[trials.size() - 1].push_back(vector<Point>());
                    trials[trials.size() - 1][trials[trials.size() - 1].size() - 1].push_back(p);
                }
            }
        }
        printf("%d gestures were recorded\n", trials.size());
        for (int i = 0; i < trials.size(); i++) {
            printf("%d trials were recorded\n", trials[i].size());
            for (int j = 0; j < trials[i].size(); j++) {
                printf("%d samples were recorded\n", trials[i][j].size());
            }
        }
        printf("return trials\n");
        return trials;
    }

    map<int, vector<vector<Point> > > createAnotatedTemplates(string filename) {
        //        printf("creating templates\n");
        map<int, vector<vector<Point> > > trials;

        vector<vector<float> > data = loadData2(filename);

        bool inTrial = false;
        int trialNr = -1;
        //        int gestNr = -1;
        for (int sampleNr = 0; sampleNr < data.size(); sampleNr++) {
            vector<float> sample = data[sampleNr];
            bool newTrial = (int) sample[4] == 1;
            int sampleGest = (int) sample[3];
            Point p = Point(sample[0], sample[1], sample[2]);
            if (inTrial) {
                if (newTrial) {
                    if (++trialNr < NROFTRIALS) {
                        trials[sampleGest].push_back(vector<Point>());
                        trials[sampleGest][trials[sampleGest].size() - 1].push_back(p);
                    } else {
                        inTrial = false;
                    }
                } else {
                    trials[sampleGest][trials[sampleGest].size() - 1].push_back(p);
                }
            } else {
                if (newTrial) {
                    trialNr = 0;
                    inTrial = true;
                    //                    trials.insert(std::pair<int, vector<vector<Point> >(sampleGest, vector<vector<Point> >()));
                    trials[sampleGest].push_back(vector<Point>());
                    trials[sampleGest][trials[sampleGest].size() - 1].push_back(p);
                }
            }
        }

        //        printf("%d trials were recorded\n", trials.size());
        //        for (std::map<int, vector<vector<Point> > >::iterator it = trials.begin(); it != trials.end(); ++it) {
        //            printf("gestId = %d\n", it->first);
        //            printf("%d trials were recorded\n", trials[it->first].size());
        //            for (int j = 0; j < trials[it->first].size(); j++) {
        //                printf("%d samples were recorded\n", trials[it->first][j].size());
        //            }
        //        }

        return trials;
    }

};

#endif	/* DTWNNTESTER_H */

