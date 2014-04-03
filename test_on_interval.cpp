/* 
 * File:   newmain.cpp
 * Author: Bas
 *
 * Created on 13 januari 2014, 14:13
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

#include "GestureVariationFollower.h"
#include "utils.h"
#include <features.h>
#include <utils.h>
#include <init.h>

#include <test_on_interval.h>

#include<list>

using namespace std;

/*
 * 
 */
int test_on_interval() {

    //    float sp = 0.00001;
    //    float sv = 0.0001;
    //    float ss = 0.0001;
    //    float sr = 0.0000001;
    //    float so = 0.75;
    int pdim = 4;
    //
    Eigen::VectorXf mpvrs = Eigen::VectorXf(pdim);
    Eigen::VectorXf rpvrs = Eigen::VectorXf(pdim);
    mpvrs << 0.05, 1.0, 1.0, 0.0;
    rpvrs << 0.1, 0.4, 0.3, 0.0;
    //
    //    int rt = 800;
    //
    //    Eigen::VectorXf sigs(pdim);
    //    sigs << sp, sv, ss, sr;
    //
    //    int num_particles = 4000;
    //
    //    GestureVariationFollower* myGVF = new GestureVariationFollower(num_particles, sigs, 1. / (so * so), rt, 0.);
    //
    //    printf("Great! GVF opened!\n\n");

    GestureVariationFollower* myGVF = getGVF();

    vector<string> filenames;
    filenames.push_back("record1.mov.dat");
    filenames.push_back("record2.mov.dat");
    filenames.push_back("record3.mov.dat");
    filenames.push_back("record4.mov.dat");
    filenames.push_back("record5.mov.dat");
    filenames.push_back("record6.mov.dat");
    filenames.push_back("record7.mov.dat");
    filenames.push_back("record8.mov.dat");
    //    filenames.push_back("record9.mov.dat");

    int nr = filenames.size();

    // gesture - trial - sample - value
    //    vector < vector < vector< float >> > gesture_data;

    // Open 9 gesture files and store in gesture data
    vector <vector <float> > gesture_data[nr];
    for (int i = 0; i < nr; i++) {
        int ones = -1;

        vector <vector <float> > data;
        ifstream infile(filenames[i].c_str());

        while (infile) {
            string s;
            if (!getline(infile, s))
                break;

            istringstream ss(s);
            vector <float> record;

            while (ss) {
                string s;
                if (!getline(ss, s, ','))
                    break;
                record.push_back((float) atof(s.c_str()));
            }
            if (record[4] == 1)
                ones++;

            if (ones >= 0 && ones < 11)
                data.push_back(record);
        }

        printf("ones = %d\n", ones);
        printf("data length = %d x %d\n", data.size(), data[0].size());

        gesture_data[i] = data;
    }

    printf("\n");



    // Add template for every first gesture
    for (int i = 0; i < nr; i++) {
        myGVF->addTemplate();
        for (int j = 0; true; j++) {
            vector<float> sample = gesture_data[i].at(j);
            vector<float> new_sam;
            for (int k = 0; k < 3; k++) {
                new_sam.push_back(sample[k + 1] * 50.0f - 25.0f);
            }
            if (j > 0 && sample[4] == 1)
                break;
            myGVF->fillTemplate(i, new_sam);
        }
    }

    list<int> buffer;

    //    int buffersize = 100;

    // Open file for writing features
    ofstream myfile;
    myfile.open("features_interval.csv");
    ofstream results_stream;
    results_stream.open("results.csv");

    for (int buffersize = 1; buffersize <= 1; buffersize *= 2) {

        for (int prtcls = 4000; prtcls <= 4000; prtcls += 2000) {
            myGVF->setNumberOfParticles(prtcls);

            for (int thrshld = 800; thrshld <= 800; thrshld += 400) {
                myGVF->setResamplingThreshold(thrshld);

                int count = 0;
                
                // Test all data on the model
                float confusion[nr][nr];
                for (int i = 0; i < nr; i++)
                    for (int j = 0; j < nr; j++)
                        confusion[i][j] = 0;


                myGVF->spreadParticles(mpvrs, rpvrs);
                buffer.clear();
                for (int i = 0; i < nr; i++) {

                    //                vector< vector<float> > copy;
//                    myGVF->spreadParticles(mpvrs, rpvrs);

                    for (int j = 0; j < gesture_data[i].size(); j++) {
                        count++;
                        vector<float> new_sam;

                        // if new trial, get prediction and restart
                        if (j % 1 == 0 && j > 0) {
                            Eigen::VectorXf stat = myGVF->getGestureConditionnalProbabilities();

                            // find recognized gesture
                            int index = 0;
                            float val = -1.0f;
                            for (int j = 0; j < nr; j++) {
                                if (stat(j) > val) {
                                    index = j;
                                    val = stat(j);
                                }
                            }

                            // add new index to buffer. Delete if over buffersize
                            buffer.push_back(index);
                            if (buffer.size() > buffersize)
                                buffer.pop_front();

                            if (buffer.size() >= buffersize) {
                                // find max index
                                index = maxOccurences(buffer);

                                confusion[i][index] += 1;

                            }

                        }

                        // Copy data to 3 dimensional vector and infer in model
                        for (int k = 0; k < 3; k++) {
                            new_sam.push_back(gesture_data[i].at(j)[k + 1] * 50.0f - 25.0f);
                        }
                        myGVF->infer(new_sam);
                        //                    copy.push_back(new_sam);
                    }

                    // normalize confusion matrix
                    float rowTotal = 0.0;
                    for (int j = 0; j < nr; j++) {
                        rowTotal += confusion[i][j];
                    }
                    for (int j = 0; j < nr; j++) {
                        confusion[i][j] /= rowTotal;
                    }

                    // print confusion matrix
                    for (int j = 0; j < nr; j++) {
                        printf("%.2f ", confusion[i][j]);
                    }
                    printf("\n");

                }
                printf("\n");
                printf("count = %d\n\n", count);

                float sum = 0.0f;
                for (int i = 0; i < nr; i++) {
                    sum += confusion[i][i];
                }
                sum /= (float) nr;
                printf("num particles = %d, threshold = %d, buffersize = %d, performance = %.3f \n", prtcls, thrshld, buffersize, sum);
                results_stream << prtcls << "," << thrshld << "," << buffersize << "," << sum << "\n";
            }
        }
    }
    myfile.close();
    results_stream.close();

    return 0;
}
