/* 
 * File:   GVFTester.h
 * Author: Bas
 *
 * Created on 22 april 2014, 14:44
 */

#ifndef GVFTESTER_H
#define	GVFTESTER_H

#include <vector>
#include <string>
#include <cfloat>

#include "GestureSetEvaluation.h"
#include "DTWNNTester.h"

#include "GestureResampler.h"

class GVFTester : public DTWNNTester {
public:

    GVFTester() {
        initValues();
        classifierType = "gvf";
    }

    GVFTester(const DTWNNTester& orig) {

    }

    virtual ~GVFTester() {

    }

    void evaluateAllFiles(const std::vector<std::string> &filenames) {
        for (int trainFileNr = 0; trainFileNr < filenames.size(); trainFileNr++) {
            printf("TrainfileNr = %d\n", trainFileNr);
            trainFilename = filenames[trainFileNr];
            evaluateOnFirst(filenames, trainFileNr);
        }
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

    void evaluateOnFirst(const std::vector<std::string> &filenames, int trainFile = 0) {
        std::map<int, std::vector<std::vector<Point> > >* templates = createAnotatedTemplates(filenames[trainFile]);

        GestureVariationFollower *gvf = trainGVF(templates, gestureSet, trainFile);

        trainFilename = filenames[trainFile];

        initConfusion();

        for (int gestIt = 0; gestIt < NROFTRIALS; gestIt++) {
            printf("iter %d\n", gestIt);
            gestNumber = gestIt;

            for (int i = 0; i < filenames.size(); i++) {
                filename = filenames[i];

                initConfusion();

                testOneFile(filenames[i]);

                printConfusion();
                writeResults();

            }
        }

        freeConfusion();

        for (std::map<int, std::vector<std::vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
            for (int i = 0; i < it->second.size(); i++)
                it->second[i].clear();
            it->second.clear();
        }
        templates->clear();
        delete templates;
        delete gvf;
    }

protected:

    GestureVariationFollower* trainGVF(std::map<int, std::vector<std::vector<Point> > >* templates, std::vector<int> gs, int skip) {
        gestNumber = skip;

        pdim = 8;

        sigs = Eigen::VectorXf(pdim);
        sigs << sigPos, sigVel, sigSca, sigSca, sigSca, sigRot, sigRot, sigRot;

        mpvrs = Eigen::VectorXf(pdim);
        mpvrs << spreadMeanPos, spreadMeanVel, spreadMeanSca, spreadMeanSca, spreadMeanSca, spreadMeanRot, spreadMeanRot, spreadMeanRot;

        rpvrs = Eigen::VectorXf(pdim);
        rpvrs << spreadRangePos, spreadRangeVel, spreadRangeSca, spreadRangeSca, spreadRangeSca, spreadRangeRot, spreadRangeRot, spreadRangeRot;

        gvf = new GestureVariationFollower(numberOfParticles, sigs, 1. / (icov * icov), resmapleThreshold, 0.);

        for (int i = 0; i < gs.size(); i++) {
            gvf->addTemplate();
            //            printf("size = %d\n", templates->at(gs[i])[skip].size());
            for (int j = 0; j < templates->at(gs[i])[skip].size(); j++) {
                std::vector<float> sample;
                sample.push_back(templates->at(gs[i])[skip][j].x);
                sample.push_back(templates->at(gs[i])[skip][j].y);
                sample.push_back(templates->at(gs[i])[skip][j].z);
                //                printf("%f %f %f \n", sample[0], sample[1], sample[2]);
                gvf->fillTemplate(i, sample);
            }
            //            printf("\n");
        }
        //        
        //        for(int i = 0; i < gvf->getNbOfTemplates(); i++){
        //            printf("%d %d\n", i, gvf->getLengthOfTemplateByInd(i));
        //        }

        return gvf;
    }

    void testOneCase(string filename, int trialNr) {
        std::map<int, std::vector<std::vector<Point> > >* trials = createAnotatedTemplates(filename);
        for (int l = 0; l < gestureSet.size(); l++) {

            gvf->spreadParticles(mpvrs, rpvrs);
            for (int m = 0; m < trials->at(gestureSet[l])[skip].size(); m++) {
                std::vector<float> sample;
                sample.push_back(trials->at(gestureSet[l])[skip][m].x);
                sample.push_back(trials->at(gestureSet[l])[skip][m].y);
                sample.push_back(trials->at(gestureSet[l])[skip][m].z);
                //                            printf( "%f %f %f \n", sample[0], sample[1], sample[2]);
                gvf->infer(sample);
            }

            int maxIndex = FLT_MIN;
            float maxVal = 0.;
            Eigen::VectorXf stat = gvf->getGestureConditionnalProbabilities();
            for (int k = 0; k < gestureSet.size(); k++) {
                float d = stat(k);
                //                            printf("%d %f\n", k, d);
                if (d > maxVal) {
                    maxVal = d;
                    maxIndex = k;
                }
            }

            confusion[gestureSet[l] - 1][gestureSet[maxIndex] - 1] += 1.0f;
        }
    }

    void testOneFile(string filename) {
        std::map<int, std::vector<std::vector<Point> > >* trials = createAnotatedTemplates(filename);
        shiftTemplates(trials, shift);
        resampleVocabulary(*trials, interpolate, reduce);
        for (int j = 0; j < NROFTRIALS; j++) {

            for (int l = 0; l < gestureSet.size(); l++) {

                gvf->spreadParticles(mpvrs, rpvrs);
                for (int m = 0; m < trials->at(gestureSet[l])[j].size(); m++) {
                    std::vector<float> sample;
                    sample.push_back(trials->at(gestureSet[l])[j][m].x);
                    sample.push_back(trials->at(gestureSet[l])[j][m].y);
                    sample.push_back(trials->at(gestureSet[l])[j][m].z);
                    //                            printf( "%f %f %f \n", sample[0], sample[1], sample[2]);
                    gvf->infer(sample);
                }

                int maxIndex = FLT_MIN;
                float maxVal = 0.;
                Eigen::VectorXf stat = gvf->getGestureConditionnalProbabilities();
                for (int k = 0; k < gestureSet.size(); k++) {
                    float d = stat(k);
                    //                            printf("%d %f\n", k, d);
                    if (d > maxVal) {
                        maxVal = d;
                        maxIndex = k;
                    }
                }

                confusion[gestureSet[l] - 1][gestureSet[maxIndex] - 1] += 1.0f;
            }
        }

        for (std::map<int, std::vector<std::vector<Point> > >::iterator it = trials->begin(); it != trials->end(); it++) {
            for (int i = 0; i < it->second.size(); i++)
                it->second[i].clear();
            it->second.clear();
        }
        trials->clear();
        delete trials;
    }

};

#endif	/* GVFTESTER_H */
