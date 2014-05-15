/* 
 * File:   DTWExtendedTester.h
 * Author: Bas
 *
 * Created on 13 mei 2014, 15:09
 */

#ifndef DTWEXTENDEDTESTER_H
#define	DTWEXTENDEDTESTER_H

#include "DTWNNTester.h"

class DTWExtendedTester : public DTWNNTester {
public:

    DTWExtendedTester() {
        initValues();
        classifierType = "dtwext";
    }

    DTWExtendedTester(const DTWExtendedTester& orig) {
    }

    virtual ~DTWExtendedTester() {
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

    vector<Point> lastTrial(deque<Point> buf, int length){
        vector<Point> trial;
        for(int i = length; i > 0; i--)
            trial.push_back(buf[buf.size()-i]);
        return trial;
    }
    
    void evaluateOnFirst(const vector<string> &filenames, int trainFile = 0) {
        map<int, vector<vector<Point> > >* templates = createAnotatedTemplates(filenames[trainFile]);
        shiftTemplates(templates, shift);

        trainFilename = filenames[trainFile];

        initConfusion();

        for (int gestIt = 0; gestIt < NROFTRIALS; gestIt++) {
            printf("iter %d\n", gestIt);
            gestNumber = gestIt;

            int maxSize = 0;

            vector<VectorDTW> dtwBank;
            for (int i = 0; i < templates->size(); i++) {
                int size = templates->at(gestureSet[i])[gestIt].size();
                dtwBank.push_back(VectorDTW(size, 1.));
                if (size > maxSize)
                    maxSize = size;
            }

            for (int i = 0; i < filenames.size(); i++) {
                filename = filenames[i];

                initConfusion();

                deque<Point> buffer;

                map<int, vector<vector<Point> > >* trials = createAnotatedTemplates(filenames[i]);
                for (int l = 0; l < gestureSet.size(); l++) {
                    for (int j = 0; j < NROFTRIALS; j++) {
                        for (int m = 0; m < trials->at(gestureSet[l])[j].size(); m++) {
                            buffer.push_back(trials->at(gestureSet[l])[j][m]);
                            if (buffer.size() > maxSize)
                                buffer.pop_front();


                            // find best
                            int minIndex = -1;
                            double minVal = DBL_MAX;
                            for (int k = 0; k < gestureSet.size(); k++) {
                                double d = dtwBank[k].fastdynamic(templates->at(gestureSet[k])[gestIt], lastTrial(buffer, dtwBank[k].getMn()));
                                if (d < minVal) {
                                    minVal = d;
                                    minIndex = k;
                                }
                            }
                            confusion[gestureSet[l] - 1][gestureSet[minIndex] - 1] += 1.0;



                        }
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
        freeConfusion();

        for (map<int, vector<vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
            for (int i = 0; i < it->second.size(); i++)
                it->second[i].clear();
            it->second.clear();
        }
        templates->clear();
        delete templates;
    }

    void evaluateAllFiles(const vector<string> &filenames) {
        for (int trainFileNr = 0; trainFileNr < filenames.size(); trainFileNr++) {
            printf("TrainfileNr = %d\n", trainFileNr);
            trainFilename = filenames[trainFileNr];
            evaluateOnFirst(filenames, trainFileNr);
        }
    }

};

#endif	/* DTWEXTENDEDTESTER_H */

