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

    vector<Point> lastTrial(deque<Point> buf, int length) {
        vector<Point> trial;
        for (int i = length; i > 0; i--)
            trial.push_back(buf[buf.size() - i]);
        return trial;
    }

    void evaluateIndividual(const vector<string> &filenames) {
        for (int trainFileNr = 0; trainFileNr < filenames.size(); trainFileNr++) {
            printf("TrainfileNr = %d\n", trainFileNr);
            trainFilename = filenames[trainFileNr];
            vector<string> filenameVector;
            filenameVector.push_back(trainFilename);
            evaluateOnFirst(filenameVector);
        }
    }

    vector<Point> rotate(vector<Point>& trial, float rotation) {
        vector<Point> newTrial;
        int rotationInt = (int) ((float) trial.size() * rotation);
        //        printf("rotation int = %d\n", rotationInt);
        for (int i = 0; i < trial.size(); i++) {
            newTrial.push_back(trial[(i + rotationInt) % trial.size()]);
        }
        return newTrial;
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
                            if (buffer.size() > maxSize) {
                                buffer.pop_front();


                                // find best
                                int minIndex = -1;
                                float minRot = 0.;
                                double minVal = DBL_MAX;
                                for (int k = 0; k < gestureSet.size(); k++) {
                                    vector<Point> currentTrial = lastTrial(buffer, dtwBank[k].getMn());
                                    for (float rotation = 0.; rotation < .975; rotation += .05) {
                                        vector<Point> rotatedTemplate = rotate(templates->at(gestureSet[k])[gestIt], rotation);
                                        double d = dtwBank[k].fastdynamic(rotatedTemplate, currentTrial);
                                        if (d < minVal) {
                                            minVal = d;
                                            minIndex = k;
                                            minRot = rotation;
                                        }
                                    }
                                }
                                printf("maxD = %.2f - %d\n", minRot, minIndex);
                                confusion[gestureSet[l] - 1][gestureSet[minIndex] - 1] += 1.0;
                            }


                        }
                    }
                    buffer.clear();
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
        Sleep(1);

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

