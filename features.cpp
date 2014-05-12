//#include "GestureVariationFollower.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include "../lbimproved/vectordtw.h"
#include <stdio.h>
#include <features.h>

using namespace std;

float length(vector<float> d) {
    float volumeLength = 0;
    for (int i = 0; i < d.size(); i++) {
        volumeLength += d[i] * d[i];
    }
    return (float) sqrt(volumeLength);
}

float phi(float x1, float y1, float x2, float y2, float x3, float y3) {
    float v1 = (x3 - x2) * (y2 - y1) - (x2 - x1) * (y3 - y2);
    float v2 = (x3 - x2) * (x2 - x1) + (y3 - y2) * (y2 - y1);
    return atan(v1 / v2);
}

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

float boundingVolumeLength(vector< vector<float> > data) {
    vector<float> maximum = max(data);
    vector<float> minimum = min(data);
    vector<float> boundingBox;

    for (int i = 0; i < maximum.size(); i++)
        boundingBox.push_back(maximum[i] - minimum[i]);

    return length(boundingBox);
}

vector<float> boundingAngle(vector< vector<float> > data) {
    vector<float> maximum = max(data);
    vector<float> minimum = min(data);
    vector<float> boundingBox;

    for (int i = 0; i < maximum.size(); i++)
        boundingBox.push_back(maximum[i] - minimum[i]);

    vector<float> angle;
    angle.push_back((float) atan(boundingBox[0] / boundingBox[1]));
    angle.push_back((float) atan(boundingBox[0] / boundingBox[2]));

    return angle;
}

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
        for (int j = 0; j < data.size(); j++)
            copy.push_back(data[j][i]);

        size_t n = copy.size() / 2;
        nth_element(copy.begin(), copy.begin() + n, copy.end());
        medians.push_back(copy[n]);
    }
    return medians;
}

// ========================== Error ==============================

vector<float> totalAngleTraversed(vector< vector<float> > data) {
    vector<float> tat;
    tat.push_back(0);
    tat.push_back(0);

    for (int i = 2; i < data.size(); i++) {
        tat[0] += phi(data[i - 2][0], data[i - 2][1], data[i - 1][0], data[i - 1][1], data[i][0], data[i][1]);
        tat[1] += phi(data[i - 2][0], data[i - 2][2], data[i - 1][0], data[i - 1][2], data[i][0], data[i][2]);
    }

    return tat;
}

vector<float> totalAngleTraversedAbsolute(vector< vector<float> > data) {
    vector<float> tat;
    tat.push_back(0);
    tat.push_back(0);

    for (int i = 2; i < data.size(); i++) {
        tat[0] += sqrt(pow(phi(data[i - 2][0], data[i - 2][1], data[i - 1][0], data[i - 1][1], data[i][0], data[i][1]), 2));
        tat[1] += sqrt(pow(phi(data[i - 2][0], data[i - 2][2], data[i - 1][0], data[i - 1][2], data[i][0], data[i][2]), 2));
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

vector<float> boundingBox(vector<vector<float> > data) {
    vector<float> maximum = max(data);
    vector<float> minimum = min(data);
    vector<float> boundingBox;

    for (int i = 0; i < maximum.size(); i++)
        boundingBox.push_back(maximum[i] - minimum[i]);

    return boundingBox;
}

vector<float> column(vector<vector<float> > data, int index) {
    vector<float> dim;
    for (int i = 0; i < data.size(); i++)
        dim.push_back(data[i][index]);
    return dim;
}
//AVERAGE

float average(vector<float> v) {
    float sum = 0.;
    for (int i = 0; i < v.size(); i++)
        sum += v[i];
    return sum / ((float) v.size());
}
//DEVIATION

float deviation(vector<float> v, float ave) {
    float E = 0.;
    for (int i = 0; i < v.size(); i++)
        E += (v[i] - ave)*(v[i] - ave);
    return (float) sqrt(1. / ((float) v.size()) * E);
}

float standardDeviation(vector<float> data) {
    return deviation(data, average(data));
}

vector<float> stdDev(vector<vector<float> > data) {
    vector<float> stdDev;

    for (int i = 0; i < data[0].size(); i++) {
        vector<float> dim = column(data, i);
        stdDev.push_back(standardDeviation(dim));
    }

    return stdDev;
}

/* ================================= vector<Point> =============================== */
vector<vector<float> > toVectors(vector<Point> data) {
    vector<vector<float> > vectors;
    for (int i = 0; i < data.size(); i++) {
        vector<float> sample;
        sample.push_back(data[i].x);
        sample.push_back(data[i].y);
        sample.push_back(data[i].z);
        vectors.push_back(sample);
    }
    return vectors;
}

float magnitude(vector<float> v) {
    float sum = 0.;
    for (int i = 0; i < v.size(); i++)
        sum += v[i] * v[i];
    return (float) sqrt(sum);
}

vector<vector<float> > toLengths(vector<vector<float> > data) {
    vector<vector<float> > vectors;
    for (int i = 0; i < data.size(); i++) {
        vector<float> lengths;
        lengths.push_back(magnitude(data[i]));
        vectors.push_back(lengths);
    }
    return vectors;
}

vector<vector<float> > toDifs(vector<vector<float> > data) {
    vector<vector<float> > vectors;
    for (int i = 1; i < data.size(); i++) {
        vector<float> sample;
        for (int j = 0; j < data[i].size(); j++)
            sample.push_back(data[i][j] - data[i - 1][j]);
        vectors.push_back(sample);
    }
    return vectors;
}

vector<float> calcFeatures(vector<vector<float> >& data) {
    vector<float> features;
    vector<float> f;

    f = boundingBox(data);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);
    printf("boundingbox %d\n", f.size());

    if (data[0].size() > 2) {
        f = boundingAngle(data);
        for (int i = 0; i < f.size(); i++)
            features.push_back(f[i]);
        printf("bounding angle %d\n", f.size());
    }

    if (data[0].size() > 2) {
        f = totalAngleTraversed(data);
        for (int i = 0; i < f.size(); i++)
            features.push_back(f[i]);
        printf("angle traversed %d\n", f.size());
    }

    if (data[0].size() > 2) {
        f = totalAngleTraversedAbsolute(data);
        for (int i = 0; i < f.size(); i++)
            features.push_back(f[i]);
        printf("angle travrsed absolute %d\n", f.size());
    }

    f = mean(data);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);
    printf("mean %d\n", f.size());

    f = stdDev(data);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);
    printf("stddev %d\n", f.size());

    features.push_back(totalGestureDistance(data));
    printf("total gest dist %d\n", f.size());

    return features;
}

vector<float> getFeatures(vector<Point> data) {
    vector<float> features;
    vector<float> f;

    vector<vector<float> > originalData = toVectors(data);
    vector<vector<float> > lengths = toLengths(originalData);
    vector<vector<float> > difs = toDifs(originalData);
    vector<vector<float> > difLengths = toLengths(difs);

    f = calcFeatures(originalData);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);
    printf("\noriginal data %d\n", f.size());

    f = calcFeatures(lengths);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);
    printf("\nlengths %d\n", f.size());

    f = calcFeatures(difs);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);
    printf("\ndifs %d\n", f.size());

    f = calcFeatures(difLengths);
    for (int i = 0; i < f.size(); i++)
        features.push_back(f[i]);
    printf("\ndifLengths %d\n", f.size());

    return features;
}
