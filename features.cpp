//#include "GestureVariationFollower.h"
#include <math.h>
#include <vector>
#include <algorithm>

#include <features.h>

using namespace std;

float duration(vector< vector<float> > data) {
    return data.size();
}

vector<float> max(vector< vector<float> > data) {
    vector<float> f;
    for (int i = 0; i < data[0].size(); i++) {
        f.push_back(data[0][i]);
        for (int j = 1; j < data.size(); j++)
            if (data[j][i] > f[i])
                f[i] = data[j][i];
    }
    return f;
}

vector<float> min(vector< vector<float> > data) {
    vector<float> f;
    for (int i = 0; i < data[0].size(); i++) {
        f.push_back(data[0][i]);
        for (int j = 1; j < data.size(); j++)
            if (data[j][i] < f[i])
                f[i] = data[j][i];
    }
    return f;
}

float length(vector<float> d) {
    float volumeLength = 0;
    for (int i = 0; i < d.size(); i++) {
        volumeLength += d[i] * d[i];
    }
    return (float) sqrt(volumeLength);
}

float boundingVolumeLength(vector< vector<float> > data) {
    vector<float> maximum = max(data);
    vector<float> minimum = min(data);
    vector<float> boundingBox;

    for (int i = 0; i < maximum.size(); i++)
        boundingBox.push_back(maximum[i] - minimum[i]);

    return length(boundingBox);
}

//float diagonal(float l1, float l2){
//    return (float) sqrt(l1 * l1 + l2 * l2);
//}

vector<float> boundingAngle(vector< vector<float> > data) {
    vector<float> maximum = max(data);
    vector<float> minimum = min(data);
    vector<float> boundingBox;

    for (int i = 0; i < maximum.size(); i++)
        boundingBox.push_back(maximum[i] - minimum[i]);

    vector<float> angle;
    angle.push_back((float)atan(boundingBox[0] / boundingBox[1]));
    angle.push_back((float)atan(boundingBox[0] / boundingBox[2]));

    return angle;
}

//float firstLastPointDistance(vector< vector<float> > data) {
//    
//}

float totalGestureDistance(vector< vector<float> > data) {
    float gestureDistance = 0.0;

    for (int i = 1; i < data.size(); i++) {
        vector<float> dif;
        for (int j = 0; j < data[i].size(); j++)
            dif.push_back(data[i][j] - data[i - 1][j]);
        gestureDistance += length(dif);
    }

    return gestureDistance;
}

float maxAccelerationSquared(vector< vector<float> > data) {
    float maxAcc = 0.0;

    for (int i = 1; i < data.size(); i++) {
        vector<float> dif;
        for (int j = 0; j < data[i].size(); j++)
            dif.push_back(data[i][j] - data[i - 1][j]);
        float l = length(dif);
        float acc = l * l;
        if (acc > maxAcc)
            maxAcc = acc;
    }

    return maxAcc;
}

vector<float> mean(vector< vector<float> > data) {
    vector<float> f;
    for (int i = 0; i < data[0].size(); i++) {
        f.push_back(data[0][i]);
        for (int j = 1; j < data.size(); j++)
            f[i] += data[j][i];
    }

    for (int i = 0; i < data[0].size(); i++) {
        f[i] /= (float) data.size();
    }
    return f;
}

vector<float> median(vector< vector<float> > data) {
    vector<float> medians;
    for (int i = 0; i < data[0].size(); i++) {
        vector<float> copy;
        for(int j = 0; j < data.size(); j++)
            copy.push_back(data[j][i]);
        
        size_t n = copy.size() / 2;
        nth_element(copy.begin(), copy.begin() + n, copy.end());
        medians.push_back(copy[n]);
    }
    return medians;
}

//vector<float> startingAngle(vector< vector<float> > data){
//    
//}
//
//vector<float> firstLastAngle(vector< vector<float> > data){
//    
//}

float phi(float x1, float y1, float x2, float y2, float x3, float y3) {
    float v1 = (x3 - x2) * (y2 - y1) - (x2 - x1) * (y3 - y2);
    float v2 = (x3 - x2) * (x2 - x1) + (y3 - y2) * (y2 - y1);
    return atan(v1 / v2);
}

vector<float> totalAngleTraversed(vector< vector<float> > data) {
    vector<float> tat;
    tat.push_back(0);
    tat.push_back(0);

    for (int i = 2; i < data.size(); i++) {
        tat[0] += phi(data[0][i - 2], data[1][i - 2], data[0][i - 1], data[1][i - 1], data[0][i], data[1][i]);
        tat[1] += phi(data[0][i - 2], data[2][i - 2], data[0][i - 1], data[2][i - 1], data[0][i], data[2][i]);
    }

    return tat;
}

vector<float> totalAngleTraversedAbsolute(vector< vector<float> > data) {
    vector<float> tat;
    tat.push_back(0);
    tat.push_back(0);

    for (int i = 2; i < data.size(); i++) {
        tat[0] += sqrt(pow( phi(data[0][i - 2], data[1][i - 2], data[0][i - 1], data[1][i - 1], data[0][i], data[1][i]) ,2));
        tat[1] += sqrt(pow( phi(data[0][i - 2], data[2][i - 2], data[0][i - 1], data[2][i - 1], data[0][i], data[2][i]) ,2));
    }

    return tat;
}

vector<float> totalSquaredAngleTraversed(vector< vector<float> > data) {
    vector<float> tat;
    tat.push_back(0);
    tat.push_back(0);

    for (int i = 2; i < data.size(); i++) {
        tat[0] += (float) pow(phi(data[0][i - 2], data[1][i - 2], data[0][i - 1], data[1][i - 1], data[0][i], data[1][i]), 2);
        tat[1] += (float) pow(phi(data[0][i - 2], data[2][i - 2], data[0][i - 1], data[2][i - 1], data[0][i], data[2][i]), 2);
    }

    return tat;
}

vector<float> getFeatures(vector< vector<float> > data) {
    vector<float> features;
    vector<float> f;

    features.push_back(duration(data));

    f = max(data);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);

    f = min(data);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);

    f = mean(data);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);

    f = median(data);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);

    features.push_back(boundingVolumeLength(data));

    f = boundingAngle(data);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);

    f = totalAngleTraversed(data);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);

    f = totalAngleTraversedAbsolute(data);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);

    f = totalSquaredAngleTraversed(data);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);

    features.push_back(totalGestureDistance(data));

    features.push_back(maxAccelerationSquared(data));

    return features;
}
