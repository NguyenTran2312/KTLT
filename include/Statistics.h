#ifndef STATISTICS_H
#define STATISTICS_H
#include "Defines.h"

double calculateMean(const SampleVector& arr);
double calculateStd(const SampleVector& arr, double mean);
double calculateMin(const SampleVector& arr);
double calculateMax(const SampleVector& arr);
SampleVector standardizeZScore(const SampleVector& arr);

#endif