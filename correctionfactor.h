#ifndef CORRECTIONFACTOR_H
#define CORRECTIONFACTOR_H
#include <math.h>
#include <numeric>
#include <vector>

using namespace std;

class CorrectionFactor{
private:
    vector<long double> correctionFactors;
    double earthRadius = 6371.0;
    double satelliteAltitude = 822.5;
    const double satelliteOrbitRadius = earthRadius + satelliteAltitude;
    int satelliteSwath = 2800;
    const double thetaCenter = satelliteSwath / earthRadius;
    int imageWidth;
    int rectifiedWidth;
    long double calcThetaSin(long double thetaCenterAngle) const; //Satellite angle for given center angle
    long double calcThetaCos(long double thetaSin) const; //Inverse of theta Sin
    long double calcCorrectionFactor(long double thetaCenterAngle) const; //Calculate the needed correction factor for given center angle
    long double calcThetaCenter(int imgWidth, int imgColumn) const; //Calculate the center angle given the image column and the overall width
    void calcRectifiedWidth();
    void calcCorrectionVector();
public:
    CorrectionFactor(int imgWidth = 1368);
    void setEarthRadius(double earthRadius);
    void setSatelliteAltitude(double satelliteAltitude);
    void setSatelliteSwath(int satelliteSwath);
    int getRectifiedWidth() const;
    const vector<long double> &getVector() const;
//    long double operator [](int index) const;
//    long double &operator [](int index);
};

#endif // CORRECTIONFACTOR_H
