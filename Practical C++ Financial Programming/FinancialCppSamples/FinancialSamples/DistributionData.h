//
// DistributionData.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net


#ifndef DISTRIBUTIONDATA_H_
#define DISTRIBUTIONDATA_H_

#include <vector>

class DistributionData {
public:
	DistributionData();
	~DistributionData();
	std::vector<double> gaussianData(int nPoints, double mean, double sigma);
	std::vector<double> exponentialData(int nPoints, double rate);
	std::vector<double> chiSquaredData(int nPoints, int degreesOfFreedom);
    std::vector<double> logNormalData(int nPoints, double mean, double sigma);

	double gaussianQuantile(double x, double mean, double sigma);
	double chiSquaredQuantile(double x, int degreesOfFreedom);
    double exponentialQuantile(double x, double rate);
    double logNormalQuantile(double x, double mean, double sigma);
};

#endif /* DISTRIBUTIONDATA_H_ */
