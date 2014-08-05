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
#include "../YIN/YIN.h"

class YINTester : public DTWNNTester {
private:
    int nroftrialinrow = 2;

public:

    YINTester() {
        initValues();
        classifierType = "yin";

        avgThreshold = .01;
        dipThreshold = .12;
        maxLength = 30;
        maxDelay = 150;
        minDips = 2;
        hbSize = 55;

        avgThreshold = .01f;
        dipThreshold = .2f;
        maxLength = 20;
        maxDelay = 150;
        minDips = 2;
        alpha = 1.f;
        hbSize = 1;

        totalNrGest = 0;

        outputFilename = "results/YINResults.csv";
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
        sum = (truePos + trueNeg) / 2.;

        if (print)
            printf("\ntruePos = %f, trueNeg = %f, average accuracy = %f\n", truePos, trueNeg, sum);

        delete yin;

        writeResults();
        return sum;
    }

    float evaluate2(const vector<string>& filenames, bool print = true) {
        YIN* yin = new YIN(3);
        yin->setAverageThreshold(avgThreshold); // .1
        yin->setDipThreshold(dipThreshold);
        yin->setMaxLength(maxLength);
        yin->setMaxDelay(maxDelay);
        yin->setMinDips(minDips);


        AlphaFilter ax = AlphaFilter(alpha);
        AlphaFilter ay = AlphaFilter(alpha);
        AlphaFilter az = AlphaFilter(alpha);

        HistogramBuffer hb = HistogramBuffer(hbSize);

        for (int trainFileNr = 0; trainFileNr < filenames.size(); trainFileNr++) {

            sum = 0.;
            float counter = 0.;

            map<int, vector<vector<Point> > >* templates = createAnotatedTemplates(filenames[trainFileNr]);
            trainFilename = filename = filenames[trainFileNr];

            float correct[templates->size()];

            int index = 0;
            if (print)
                printf("Testfile: %s \n", filename.c_str());

            int nrOfTrials = 0;
            for (map<int, vector<vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
                nrOfTrials = nrOfTrials > it->second.size() ? nrOfTrials : it->second.size();
            }

            for (int k = 0; k < nrOfTrials - nroftrialinrow + 1; k++) {

                float counter2;
                float sum2 = 0.;
                counter2 = 0.;

                for (map<int, vector<vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
                    std::vector<vector<Point> > gesture = it->second;

                    for (int i = 0; i < nroftrialinrow; i++) {
                        int ki = k + i;
                        for (int j = 0; j < gesture[ki].size(); j++) {

                            vector<float> sample;
                            sample.push_back(ax.value(gesture[ki][j].x));
                            sample.push_back(ay.value(gesture[ki][j].y));
                            sample.push_back(az.value(gesture[ki][j].z));

//                            printf("%f %f %f\n", ax.value(), ay.value(), az.value());

                            yin->process(sample);
                            hb.put(yin->isSync() ? 1 : 0);
                            //                            printf("%s\n", yin->isSync() ? "TRUE" : "FALSE");
                            //                            printf("%f %f %f\n", sample[0], sample[1], sample[2]);

                            if (hb.get() == i || (hb.get() == 1 && i > 0)) {
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
                printf("%f\n", sum2);
            }


            sum = sum / counter;
            printf("Total = %f\n", sum);

            writeResults();
        }

        delete yin;

        return sum;
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

        for (float val = .05; val < 1.005; val += .05) {
            printf("Current alpha = %.2f\n", val);
            alpha = val;
            float perf = evaluate(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            alpha = bestVal;
            //            printf("Best alpha = %.2f with performance = %.3f\n\n", bestVal, bestPerf);
            //            printParams();
        }
    }

    void findBestAlpha2(float from, float to, float with) {
        float bestPerf = 0.0;
        float bestVal = 0.0;

        for (float val = from; val < to; val += with) {
            printf("Current alpha = %.2f\n", val);
            alpha = val;
            float perf = evaluate2(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            alpha = bestVal;
            //            printf("Best alpha = %.2f with performance = %.3f\n\n", bestVal, bestPerf);
            //            printParams();
        }
    }

    void findBestAvgThreshold() {
        float bestPerf = 0.0;
        float bestVal = 0.0;

        for (float val = .1; val < 1.05; val += .1) {
            printf("Current avgThreshold = %.2f\n", val);
            avgThreshold = val;
            float perf = evaluate(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            avgThreshold = bestVal;
            //            printf("Best avgThreshold = %.2f with performance = %.3f\n\n", bestVal, bestPerf);
            //            printParams();
        }
    }

    void findBestDipThreshold() {
        float bestPerf = 0.0;
        float bestVal = 0.0;

        for (float val = .04; val <= .22; val += .04) {
            printf("Current dipThreshold = %.2f\n", val);
            dipThreshold = val;
            float perf = evaluate(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            dipThreshold = bestVal;
            //            printf("Best dipThreshold = %.2f with performance = %.3f\n\n", bestVal, bestPerf);
            //            printParams();
        }
    }

    void findBestDipThreshold2(float from, float to, float with) {
        float bestPerf = 0.0;
        float bestVal = 0.0;

        for (float val = from; val <= to; val += with) {
            printf("Current dipThreshold = %.2f\n", val);
            dipThreshold = val;
            float perf = evaluate2(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            dipThreshold = bestVal;
        }
    }

    void findBestMaxLength() {
        float bestPerf = 0.0;
        int bestVal = 0;

        for (int val = 1; val < 15; val += 1) {
            maxLength = val;
            printf("Current MaxLength = %d\n", val);
            float perf = evaluate(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            maxLength = bestVal;
            //            printf("Best MaxLength = %d with performance = %.3f\n\n", bestVal, bestPerf);
            //            printParams();
        }
    }

    void findBestTau2(int from, int to, int with) {
        float bestPerf = 0.0;
        int bestVal = 0;

        for (int val = from; val < to; val += with) {
            maxDelay = val;
            printf("Current tau = %d\n", val);
            float perf = evaluate2(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            maxDelay = bestVal;
        }
    }

    void findBestHBSize(int from = 1, int to = 65, int with = 2) {
        float bestPerf = 0.0;
        int bestVal = 0;

        for (int val = from; val < to; val += with) {
            hbSize = val;
            printf("Current hbSize = %d\n", val);
            float perf = evaluate(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            hbSize = bestVal;
            //            printf("Best hbSize = %d with performance = %.3f\n\n", bestVal, bestPerf);
            //            printParams();
        }
    }

    void findBestHBSize2(int from = 1, int to = 65, int with = 2) {
        float bestPerf = 0.0;
        int bestVal = 0;

        for (int val = from; val < to; val += with) {
            hbSize = val;
            printf("Current hbSize = %d\n", val);
            float perf = evaluate2(filenames);
            if (perf > bestPerf) {
                bestPerf = perf;
                bestVal = val;
            }
            hbSize = bestVal;
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
            //            printf("Best value = %d with performance = %.3f\n\n", bestVal, bestPerf);
            //            printParams();
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
            //            printf("Best value = %f with performance = %.3f\n\n", bestVal, bestPerf);
            //            printParams();
        }
    }

    void evaluateAllFiles(const std::vector<std::string> &filenames) {
        this->filenames = filenames;
        alpha = .3;
        printf("alpha\n");
        findBestFloat(&alpha, .15, .6, .05);
    }

    void setFilenames(vector<string> filenms) {
        filenames = filenms;
    }

    void setTau(float f) {
        maxLength = f;
    }

    void setAlpha(float al) {
        alpha = al;
    }

    void setNrOfTrialsInRow(int i) {
        nroftrialinrow = i;
    }

    void setAbsoluteThreshold(float f) {
        dipThreshold = f;
    }
};

#endif	/* YINTESTER_H */