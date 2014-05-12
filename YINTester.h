/* 
 * File:   YINTester.h
 * Author: Bas
 *
 * Created on 24 april 2014, 11:20
 */

#ifndef YINTESTER_H
#define	YINTESTER_H

#include "DTWNNTester.h"
#include "AlphaFilter.h"
#include "HistogramBuffer.h"

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

    float testPositive(YIN* yin, const vector<string>& filenames, bool print = true) {

        AlphaFilter ax = AlphaFilter(alpha);
        AlphaFilter ay = AlphaFilter(alpha);
        AlphaFilter az = AlphaFilter(alpha);

        HistogramBuffer hb = HistogramBuffer(hbSize);

        float sum = 0.;
        float counter = 0.;

        for (int trainFileNr = 0; trainFileNr < filenames.size(); trainFileNr++) {
            map<int, vector<vector<Point> > >* templates = createAnotatedTemplates(filenames[trainFileNr]);
            trainFilename = filename = filenames[trainFileNr];

            float correct[templates->size()];

            int index = 0;
            if (print)
                printf("Positive test: %s \n", filename.c_str());
            for (map<int, vector<vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
                std::vector<vector<Point> > gesture = it->second;

                float counter2;
                float sum2 = 0.;
                counter2 = 0.;

                for (int i = 0; i < gesture.size(); i++) {
                    for (int j = 0; j < gesture[i].size(); j++) {
                        vector<float> sample;
                        sample.push_back(ax.value(gesture[i][j].x));
                        sample.push_back(ay.value(gesture[i][j].y));
                        sample.push_back(az.value(gesture[i][j].z));
                        yin->process(sample);
                        if (i > 0) {
                            if (hb.putAndGet(yin->isSync() ? 1 : 0) == 1) {
                                sum2 += 1;
                                correct[index]++;
                            }
                            counter2++;
                        }
                    }
                }
                sum2 /= counter2;
                sum += sum2;
                counter++;
                correct[index++] /= (float) counter2;
                if (print)
                    printf("%f \n", sum2);
            }
        }
        return sum / counter;
    }

    float testNegative(YIN* yin, const vector<string>& filenames, bool print = true) {

        AlphaFilter ax = AlphaFilter(alpha);
        AlphaFilter ay = AlphaFilter(alpha);
        AlphaFilter az = AlphaFilter(alpha);

        HistogramBuffer hb = HistogramBuffer(hbSize);

        float sum = 0.;
        float counter = 0.;

        for (int trainFileNr = 0; trainFileNr < filenames.size(); trainFileNr++) {
            map<int, vector<vector<Point> > >* templates = createAnotatedTemplates(filenames[trainFileNr]);
            trainFilename = filename = filenames[trainFileNr];

            float correct[templates->size()];

            int index = 0;
            if (print)
                printf("Negative test: %s \n", filename.c_str());


            for (int i = 0; i < NROFTRIALS; i++) {
                float counter2 = 0.;
                float sum2 = 0.;
                for (map<int, vector<vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
                    std::vector<vector<Point> > gesture = it->second;
                    for (int j = 0; j < gesture[i].size(); j++) {
                        vector<float> sample;
                        sample.push_back(ax.value(gesture[i][j].x));
                        sample.push_back(ay.value(gesture[i][j].y));
                        sample.push_back(az.value(gesture[i][j].z));
                        yin->process(sample);
                        if (hb.putAndGet(yin->isSync() ? 1 : 0) != 1) {
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
                if (print)
                    printf("%f \n", sum2);
            }
        }
        return sum / counter;
    }

    float evaluate(const vector<string>& filenames, bool print = true) {
        YIN* yin = new YIN(3);
        yin->setAverageThreshold(avgThreshold); // .1
        yin->setDipThreshold(dipThreshold);
        yin->setMaxLength(maxLength);
        yin->setMaxDelay(maxDelay);
        yin->setMinDips(minDips);

        float truePos = testPositive(yin, filenames, print);
        float trueNeg = testNegative(yin, filenames, print);
        float perf = (truePos + trueNeg) / 2.;

        if (print)
            printf("\ntruePos = %f, trueNeg = %f, average accuracy = %f\n", truePos, trueNeg, perf);

        delete yin;

        return perf;
    }

    void printParams() {
        printf("avgThreshold = %f\n", avgThreshold);
        printf("dipThreshold = %f\n", dipThreshold);
        printf("maxLength = %d\n", maxLength);
        printf("maxDelay = %d\n", maxDelay);
        printf("minDips = %d\n", minDips);
        printf("alpha = %f\n", alpha);
        printf("hbSize = %d\n\n", hbSize);
    }

    void findBestAlpha() {
        float bestPerf = 0.0;
        float bestVal = 0.0;

        for (float val = .35; val > .00; val -= .05) {
            printf("Current alpha = %.2f\n", val);
            alpha = val;
            float perf = evaluate(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            alpha = bestVal;
            printf("Best alpha = %.2f with performance = %.3f\n\n", bestVal, bestPerf);
            printParams();
        }
    }

    void findBestAvgThreshold() {
        float bestPerf = 0.0;
        float bestVal = 0.0;

        for (float val = .04; val < .2; val += .04) {
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

        for (float val = .12; val <= .12; val += .02) {
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
        int bestVal = 0;

        for (int val = 20; val < 40; val += 5) {
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
        int bestVal = 0;

        for (int val = 100; val < 201; val += 5) {
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

    void findBestHBSize() {
        float bestPerf = 0.0;
        int bestVal = 0;

        for (int val = 1; val < 65; val *= 2) {
            hbSize = val;
            printf("Current hbSize = %d\n", val);
            float perf = evaluate(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            hbSize = bestVal;
            printf("Best hbSize = %d with performance = %.3f\n\n", bestVal, bestPerf);
            printParams();
        }
    }

    void findBestInt(int* parameter, int lowerBound, int upperBound, int stepSize) {
        float bestPerf = 0.0;
        int bestVal = 0;

        for (int val = lowerBound; val <= upperBound; val += stepSize) {
            *parameter = val;
            printf("Current value = %d\n", val);
            float perf = evaluate(filenames, false);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            *parameter = bestVal;
            printf("Best value = %d with performance = %.3f\n\n", bestVal, bestPerf);
            printParams();
        }
    }

    void findBestFloat(float* parameter, float lowerBound, float upperBound, float stepSize) {
        float bestPerf = 0.0;
        float bestVal = 0;

        for (float val = lowerBound; val <= upperBound; val += stepSize) {
            *parameter = val;
            printf("Current value = %f\n", val);
            float perf = evaluate(filenames, false);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            *parameter = bestVal;
            printf("Best value = %f with performance = %.3f\n\n", bestVal, bestPerf);
            printParams();
        }
    }

    void evaluateAllFiles(const std::vector<std::string> &filenames) {
        this->filenames = filenames;
        alpha = .3;

        //        findBestHBSize();
        //        findBestAlpha();
        //        findBestMaxLength();
        //                findBestMaxDelay();
        //                findBestAvgThreshold();
        //                findBestDipThreshold();
        //                findBestAlpha();
        
        printf("alpha\n");
        findBestFloat(&alpha, .15, .6, .05);
//        printf("AvgThreshold\n");
//        findBestFloat(&avgThreshold, .01, .04, .01);
//        printf("DipThreshold\n");
//        findBestFloat(&dipThreshold, .04, .2, .02);
//        printf("MaxLength\n");
//        findBestInt(&maxLength, 15, 60, 5);
//        printf("MaxDelay\n");
//        findBestInt(&maxDelay, 100, 200, 10);
//        printf("hbSize\n");
//        findBestInt(&hbSize, 55, 80, 5);
    }

private:
    float avgThreshold = .01;
    float dipThreshold = .12;
    int maxLength = 30;
    int maxDelay = 150;
    int minDips = 2;
    int hbSize = 55;
};

#endif	/* YINTESTER_H */