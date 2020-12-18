#include "correctionfactor.h"

long double CorrectionFactor::calcThetaSin(long double thetaCenterAngle) const{
    return atan(earthRadius * sin(thetaCenterAngle) / (satelliteAltitude + earthRadius * (1 - cos(thetaCenterAngle))));
}
long double CorrectionFactor::calcThetaCos(long double thetaSin) const{
    long double delta_sqrt = sqrt(pow(earthRadius, 2) + pow(tan(thetaSin), 2) * (pow(earthRadius, 2) - pow(satelliteOrbitRadius, 2)));
    return acos((pow(tan(thetaSin), 2) * satelliteOrbitRadius + delta_sqrt) / (earthRadius * (pow(tan(thetaSin), 2) + 1)));
}

long double CorrectionFactor::calcCorrectionFactor(long double thetaCenterAngle) const{
    long double norm_factor = earthRadius / satelliteAltitude;
    long double tan_derivative_recip = (1 + pow((earthRadius * sin(thetaCenterAngle) / (satelliteAltitude + earthRadius * (1 - cos(thetaCenterAngle)))), 2));
    long double arg_derivative_recip = (pow((satelliteAltitude + earthRadius * (1 - cos(thetaCenterAngle))), 2) / (earthRadius * cos(thetaCenterAngle) * (satelliteAltitude + earthRadius * (1 - cos(thetaCenterAngle))) - pow(earthRadius, 2) * pow(sin(thetaCenterAngle), 2)));
    return norm_factor * tan_derivative_recip * arg_derivative_recip;
}

long double CorrectionFactor::calcThetaCenter(int imgWidth, int imgColumn) const{
    long double theta_sin = calcThetaSin(thetaCenter / 2.0) * (abs(imgColumn - imgWidth / 2.0) / (imgWidth / 2.0));
    return calcThetaCos(theta_sin);
}

void CorrectionFactor::calcRectifiedWidth(){
    this->rectifiedWidth = ceil(accumulate(this->correctionFactors.begin(), this->correctionFactors.end(), 0.0));
}

void CorrectionFactor::calcCorrectionVector(){
    correctionFactors.clear();
    for(int imgColumn = 0; imgColumn <= this->imageWidth; imgColumn++){
        correctionFactors.push_back(calcCorrectionFactor(this->calcThetaCenter(this->imageWidth, imgColumn)));
    }
}

CorrectionFactor::CorrectionFactor(int imgWidth){
    this->imageWidth = imgWidth;
    calcCorrectionVector();
    calcRectifiedWidth();
}

void CorrectionFactor::setEarthRadius(double earthRadius){
    this->earthRadius = earthRadius;
    this->calcCorrectionVector();
}

void CorrectionFactor::setSatelliteAltitude(double satelliteAltitude){
    this->satelliteAltitude = satelliteAltitude;
    this->calcCorrectionVector();
}

void CorrectionFactor::setSatelliteSwath(int satelliteSwath){
    this->satelliteSwath = satelliteSwath;
    this->calcCorrectionVector();
}

int CorrectionFactor::getRectifiedWidth() const{
    return this->rectifiedWidth;
}

const vector<long double> &CorrectionFactor::getVector() const{
    return this->correctionFactors;
}

//long double CorrectionFactor::operator [](int index) const{
//    return this->correctionFactors[index];
//}

//long double &CorrectionFactor::operator [](int index){
//    return this->correctionFactors[index];
//}
