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
#include <windows.h>
#include <algorithm>
#include "init.h"

#include "GestureResampler.h"

using namespace std;

class DTWNNTester : public GestureSetEvaluation {
public:

    DTWNNTester() {
        initValues();
        classifierType = "dtw";
    }

    DTWNNTester(const DTWNNTester& orig) {

    }

    virtual ~DTWNNTester() {

    }

    static const int NROFTRIALS = 7;

    void evaluateIndividual(const vector<string> &filenames) {
        for (int trainFileNr = 0; trainFileNr < filenames.size(); trainFileNr++) {
            printf("TrainfileNr = %d\n", trainFileNr);
            trainFilename = filenames[trainFileNr];
            vector<string> filenameVector;
            filenameVector.push_back(trainFilename);
            evaluateOnFirst(filenameVector);
        }
    }

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

    void shiftTemplates(map<int, vector<vector<Point> > >* templates, float shift) {
        shift = (float) min(1., max(0., (double) shift));
        for (std::map<int, vector<vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
            boolean deleteOne = false;
            for (int i = 0; i < it->second.size() - 1; i++) {
                int shiftAmount = (int) (it->second[i].size() * shift);
                if (shiftAmount > 0) {
                    it->second[i].erase(it->second[i].begin(), it->second[i].begin() + shiftAmount);
                    deleteOne = true;
                }

                for (int j = 0; j < shiftAmount; j++)
                    it->second[i].push_back(it->second[i + 1][it->second[i + 1].size() - 1 - j]);

            }
            if (deleteOne) {
                it->second.pop_back();
            }
        }
        // print
        for (std::map<int, vector<vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
            printf("gesture %d\n", it->first);
            for (int i = 0; i < it->second.size(); i++) {
                printf("length = %d\n", it->second[i].size());
            }
            printf("\n");
        }
    }

    void evaluateOnFirst(const vector<string> &filenames, int trainFile = 0) {
        map<int, vector<vector<Point> > >* templates = createAnotatedTemplates(filenames[trainFile]);

        trainFilename = filenames[trainFile];

        int minus = 0;
        if (shift > .01)
            minus = 1;

        for (int gestIt = 0; gestIt < NROFTRIALS - minus; gestIt++) {
            printf("iter %d\n", gestIt);
            gestNumber = gestIt;
            vector<VectorDTW> dtwBank;
            for (int i = 0; i < templates->size(); i++)
                dtwBank.push_back(VectorDTW(templates->at(gestureSet[i])[gestIt].size(), 1.));

            for (int i = 0; i < filenames.size(); i++) {
                printf("test %d of %d\n", i, filenames.size());
                filename = filenames[i];

                initConfusion();
                Sleep(1);

                map<int, vector<vector<Point> > >* trials = createAnotatedTemplates(filenames[i]);
                shiftTemplates(trials, shift);
                resampleVocabulary(*trials, interpolate, reduce);

                for (int j = 0; j < NROFTRIALS; j++) {
                    for (int l = 0; l < gestureSet.size(); l++) {
                        int minIndex = -1;
                        double minVal = DBL_MAX;
                        for (int k = 0; k < gestureSet.size(); k++) {
                            double d = dtwBank[k].fastdynamic(templates->at(gestureSet[k])[gestIt], trials->at(gestureSet[l])[j]);
                            if (d < minVal) {
                                minVal = d;
                                minIndex = k;
                            }
                        }
                        confusion[gestureSet[l] - 1][gestureSet[minIndex] - 1] += 1.0;
                    }
                }
                printConfusion();
                writeResults();

                for (map<int, vector<vector<Point> > >::iterator it = trials->begin(); it != trials->end(); it++) {
                    for (int i = 0; i < it->second.size(); i++)
                        it->second[i].clear();
                    it->second.clear();
                }
                trials->clear();
                delete trials;

            }
            dtwBank.clear();
        }

        Sleep(1);
        freeConfusion();
        Sleep(1);

        for (map<int, vector<vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
            for (int i = 0; i < it->second.size(); i++)
                it->second[i].clear();
            it->second.clear();
        }
        templates->clear();
        delete templates;

        printf("evaluateOnFirst done\n");
    }

    void evaluateAllFiles(const vector<string> &filenames) {
        for (int trainFileNr = 0; trainFileNr < filenames.size(); trainFileNr++) {
            printf("TrainfileNr = %d\n", trainFileNr);
            trainFilename = filenames[trainFileNr];
            evaluateOnFirst(filenames, trainFileNr);
        }
    }

protected:

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

    map<int, vector<vector<Point> > >* createAnotatedTemplates(string filename) {
        map<int, vector<vector<Point> > >* trials = new map<int, vector<vector<Point> > >();

        vector<vector<float> > data = loadData2(filename);
        gestureSet = gesturesInData(data);

        bool inTrial = false;
        int trialNr = -1;

        // push samples in trials in gestures in gesture set
        for (int sampleNr = 0; sampleNr < data.size(); sampleNr++) {
            vector<float> sample = data[sampleNr];
            bool newTrial = (int) data[sampleNr][4] == 1;
            int sampleGest = (int) sample[3];
            Point p = Point(sample[0], sample[1], sample[2]);
            if (inTrial) {
                if (newTrial) {
                    if (++trialNr < NROFTRIALS) {
                        trials->at(sampleGest).push_back(vector<Point>());
                        trials->at(sampleGest)[trials->at(sampleGest).size() - 1].push_back(p);
                    } else {
                        inTrial = false;
                    }
                } else {
                    trials->at(sampleGest)[trials->at(sampleGest).size() - 1].push_back(p);
                }
            } else {
                if (newTrial) {
                    trialNr = 0;
                    inTrial = true;
                    trials->insert(std::make_pair(sampleGest, vector<vector<Point> >()));
                    trials->at(sampleGest).push_back(vector<Point>());
                    trials->at(sampleGest)[trials->at(sampleGest).size() - 1].push_back(p);
                }
            }
        }

        //         print
        //        printf("printf~\n");
        //                for (std::map<int, vector<vector<Point> > >::iterator it = trials->begin(); it != trials->end(); it++) {
        //                    printf("gesture %d\n", it->first);
        //                    for (int i = 0; i < it->second.size(); i++) {
        //                        printf("length = %d\n", it->second[i].size());
        //                    }
        //                    printf("\n");
        //                }

        for (int i = 0; i < data.size(); i++) {
            data[i].clear();
        }
        data.clear();

        return trials;
    }

};

#endif	/* DTWNNTESTER_H */

