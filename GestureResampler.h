/* 
 * File:   GestureResampler.h
 * Author: Bas
 *
 * Created on 3 juni 2014, 10:18
 */

#ifndef GESTURERESAMPLER_H
#define	GESTURERESAMPLER_H

void interpolateTrial(std::vector<Point> &trial, int interpolate) {
    for (int i = trial.size() - 1; i > 0; i--) {
        float dx, dy, dz;
        dx = trial[i].x - trial[i - 1].x;
        dy = trial[i].y - trial[i - 1].y;
        dz = trial[i].z - trial[i - 1].z;

        for (int j = 0; j < interpolate; j++) {
            float nx, ny, nz, factor;
            factor = (float) j / (float) interpolate;
            nx = trial[i - 1].x + dx * factor;
            ny = trial[i - 1].y + dy * factor;
            nz = trial[i - 1].z + dz * factor;
            trial.insert(trial.begin() + i + j - 1, Point(nx, ny, nz));
        }
    }
}

void reduceTrial(std::vector<Point> &trial, int reduction) {
    for (int i = trial.size() - 1; i >= 0; i--) {
        if (i % reduction != 0)
            trial.erase(trial.begin() + i);
    }
}

void resampleTrial(std::vector<Point> &trial, int interpolate, int resample) {
    interpolateTrial(trial, interpolate);
    reduceTrial(trial, resample);
}


void resampleVocabulary(std::map<int, std::vector<std::vector<Point> > > &voc, int interpolate, int resample) {
    for (std::map<int, std::vector<std::vector<Point> > >::iterator it = voc.begin(); it != voc.end(); it++) {
        for (int i = 0; i < it->second.size(); i++) {
            resampleTrial(it->second[i], interpolate, resample);
        }
    }
}

void resizeTrial(std::vector<Point>& trial, int size){
    int resample = trial.size();
    interpolateTrial(trial, size-1);
    reduceTrial(trial, resample);
}

void resizeVocabulary(std::map<int, std::vector<std::vector<Point> > > &voc, int size) {
//    for(std::map<int, std::vector<std::vector<Point> > >::iterator it = voc.begin(); it != voc.end(); it++){
//        printf("\ngesture %d\n", it->first);
//        for(int i = 0; i < it->second.size(); i++){
//            printf("size of %d: %d", i, it->second[i].size());
//        }
//    }
    
    for (std::map<int, std::vector<std::vector<Point> > >::iterator it = voc.begin(); it != voc.end(); it++) {
        for (int i = 0; i < it->second.size(); i++) {
            resizeTrial(it->second[i], size);
        }
    }
    
    for(std::map<int, std::vector<std::vector<Point> > >::iterator it = voc.begin(); it != voc.end(); it++){
        printf("\ngesture %d\n", it->first);
        for(int i = 0; i < it->second.size(); i++){
            printf("\nsize of %d: %d\n", i, it->second[i].size());
            for(int j = 0; j < it->second[i].size(); j++){
                printf("%.3f %.3f %.3f\n",it->second[i][j].x,it->second[i][j].y,it->second[i][j].z);
            }
        }
    }
    
}


#endif	/* GESTURERESAMPLER_H */

