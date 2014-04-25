/* 
 * File:   YINTester.h
 * Author: Bas
 *
 * Created on 24 april 2014, 11:20
 */

#ifndef YINTESTER_H
#define	YINTESTER_H

#include "DTWNNTester.h"

class YINTester : public DTWNNTester {
public:

    YINTester() {
        initValues();
        classifierType = "yin";
    }

    YINTester(const DTWNNTester& orig) {

    }

    virtual ~YINTester() {

    }

    float testPositive(YIN* yin, const vector<string>& filenames) {

        float sum = 0.;
        float counter = 0.;

        for (int trainFileNr = 0; trainFileNr < filenames.size(); trainFileNr++) {
            map<int, vector<vector<Point> > >* templates = createAnotatedTemplates(filenames[trainFileNr]);
            trainFilename = filename = filenames[trainFileNr];

            float correct[templates->size()];

            int index = 0;
            printf("Positive test: %s \n", filename.c_str());
            for (map<int, vector<vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
                std::vector<vector<Point> > gesture = it->second;

                float counter2;
                float sum2 = 0.;
                counter2 = 0.;

                for (int i = 0; i < gesture.size(); i++) {
                    for (int j = 0; j < gesture[i].size(); j++) {
                        vector<float> sample;
                        sample.push_back(gesture[i][j].x);
                        sample.push_back(gesture[i][j].y);
                        sample.push_back(gesture[i][j].z);
                        yin->process(sample);
                        if (yin->isSync()) {
                            sum2 += 1;
                            correct[index]++;
                        }
                        counter2++;
                    }
                }
                sum2 /= counter2;
                sum += sum2;
                counter++;
                correct[index++] /= (float) counter2;
                printf("%f \n", sum2);
            }
        }
        return sum / counter;
    }

    float testNegative(YIN* yin, const vector<string>& filenames) {

        float sum = 0.;
        float counter = 0.;

        for (int trainFileNr = 0; trainFileNr < filenames.size(); trainFileNr++) {
            map<int, vector<vector<Point> > >* templates = createAnotatedTemplates(filenames[trainFileNr]);
            trainFilename = filename = filenames[trainFileNr];

            float correct[templates->size()];

            int index = 0;
            printf("Negative test: %s \n", filename.c_str());


            for (int i = 0; i < NROFTRIALS; i++) {
                float counter2 = 0.;
                float sum2 = 0.;
                for (map<int, vector<vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
                    std::vector<vector<Point> > gesture = it->second;
                    for (int j = 0; j < gesture[i].size(); j++) {
                        vector<float> sample;
                        sample.push_back(gesture[i][j].x);
                        sample.push_back(gesture[i][j].y);
                        sample.push_back(gesture[i][j].z);
                        yin->process(sample);
                        if (!yin->isSync()) {
                            sum2 += 1;
                            correct[index]++;
                        }
                        counter2++;
                    }
                }
                sum2 /= counter2;
                sum += sum2;
                counter++;
                correct[index++] /= (float) counter2;
                printf("%f \n", sum2);
            }
        }
        return sum / counter;
    }

    float evaluate(const vector<string>& filenames) {
        YIN* yin = new YIN(3);
        yin->setAverageThreshold(avgThreshold); // .1
        yin->setDipThreshold(dipThreshold);
        yin->setMaxLength(maxLength);
        yin->setMaxDelay(maxDelay);
        yin->setMinDips(minDips);

        float truePos = testPositive(yin, filenames);
        float trueNeg = testNegative(yin, filenames);
        float perf = (truePos + trueNeg) / 2.;

        printf("truePos = %f, trueNeg = %f, average accuracy = %f\n", truePos, trueNeg, perf);
    }

    void printParams() {
        printf("avgThreshold = %f\n", avgThreshold);
        printf("dipThreshold = %f\n", dipThreshold);
        printf("maxLength = %d\n", maxLength);
        printf("maxDelay = %d\n", maxDelay);
        printf("minDips = %d\n", minDips);
    }

    void findBestAvgThreshold() {
        float bestPerf = 0.0;
        float bestVal = 0.0;

        for (float val = .05; val < .3; val += .05) {
            printf("Current avgThreshold = %.2f\n", val);
            avgThreshold = val;
            float perf = evaluate(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            avgThreshold = bestVal;
            printf("Best avgThreshold = %.2f with performance = %.3f\n\n", bestVal, bestPerf);
            printParams();
        }
    }

    void findBestDipThreshold() {
        float bestPerf = 0.0;
        float bestVal = 0.0;

        for (float val = .05; val < avgThreshold; val += .05) {
            printf("Current dipThreshold = %.2f\n", val);
            dipThreshold = val;
            float perf = evaluate(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            dipThreshold = bestVal;
            printf("Best dipThreshold = %.2f with performance = %.3f\n\n", bestVal, bestPerf);
            printParams();
        }
    }

    void findBestMaxLength() {
        float bestPerf = 0.0;
        float bestVal = 0.0;

        for (int val = 20; val < 100; val += 20) {
            maxLength = val;
            printf("Current MaxLength = %d\n", val);
            float perf = evaluate(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            maxLength = bestVal;
            printf("Best MaxLength = %d with performance = %.3f\n\n", bestVal, bestPerf);
            printParams();
        }
    }

    void findBestMaxDelay() {
        float bestPerf = 0.0;
        float bestVal = 0.0;

        for (int val = 50; val < 200; val += 25) {
            maxDelay = val;
            printf("Current maxDelay = %d\n", val);
            float perf = evaluate(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            maxDelay = bestVal;
            printf("Best maxDelay = %d with performance = %.3f\n\n", bestVal, bestPerf);
            printParams();
        }
    }

    void evaluateAllFiles(const std::vector<std::string> &filenames) {
        this->filenames = filenames;
        findBestMaxLength();
        findBestMaxDelay();
        findBestAvgThreshold();
        findBestDipThreshold();
        
    }

private:
    float avgThreshold = .1;
    float dipThreshold = .1;
    int maxLength = 20;
    int maxDelay = 120;
    int minDips = 2;
};

#endif	/* YINTESTER_H */