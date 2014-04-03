/* 
 * File:   DTWNNTester.h
 * Author: Bas
 *
 * Created on 3 april 2014, 11:59
 */

#ifndef DTWNNTESTER_H
#define	DTWNNTESTER_H

#include "../lbimproved/vectordtw.h"
#include "init.h"

#include <string>
#include <stdio.h>
#include <vector>
#include <float.h>

using namespace std;

class DTWNNTester {
public:

    DTWNNTester() {

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
                for (int trial = 0; trial < NROFTRIALS; trial++) {
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
        };

#endif	/* DTWNNTESTER_H */

