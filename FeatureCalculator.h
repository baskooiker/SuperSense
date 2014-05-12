/* 
 * File:   FeatureCalculator.h
 * Author: Bas
 *
 * Created on 2 mei 2014, 11:50
 */

#ifndef FEATURECALCULATOR_H
#define	FEATURECALCULATOR_H

#include "DTWNNTester.h"
#include "GVFTester.h"
#include "features.h"

class FeatureCalculator : protected GVFTester {
public:

    FeatureCalculator(){
        initValues();
    }
    
    void calculateFeatures(vector<string> filenames, string outfilename) {
        for (int j = 0; j < filenames.size(); j++) {
            for (int i = 0; i < NROFTRIALS; i++) {
                initConfusion();
                map<int, vector<vector<Point> > >* templates = createAnotatedTemplates(filenames[j]);
                gvf = trainGVF(templates, gestureSet, i);
                testOneFile(filenames[j]);
                printConfusion(true);
                printf("perf = %f\n", sum);
                vector<vector<float> > features;
                for (int k = 0; k < gestureSet.size(); k++) 
                    features.push_back(getFeatures(templates->at(gestureSet[k])[i]));
                vector<float> finalFeatures = meanAndStdDev(features);
                
                FILE* f;
                do {
                    f = fopen(outfilename.c_str(), "a");
                    Sleep(10);
                } while (!f);
                out.open(outfilename.c_str(), fstream::out | fstream::app);

                out << filenames[j].c_str();
                out << ";" << i;
                out << ";" << sum;
                for (int k = 0; k < finalFeatures.size(); k++)
                    out << ";" << finalFeatures[k];
                out << "\n";

                out.close();
                fclose(f);

            }
        }
    }

private:

    vector<float> meanAndStdDev(vector<vector<float> > data){
        vector<float> features;
        for(int i = 0; i < data[0].size(); i++){
            vector<float> v = column(data, i);
            features.push_back(average(v));
            features.push_back(standardDeviation(v));
        }
        return features;
    }


};

#endif	/* FEATURECALCULATOR_H */

