//
// OptionProbabilityExportedFunctions.cpp
#include "OptionsProbabilities.h"

extern "C" double __declspec(dllexport) optionProbFinishAboveStrike(double initialPrice,
        double strike, double avgStep, int nDays) {
	OptionsProbabilities optP(initialPrice, strike, avgStep, nDays);
	return optP.probFinishAboveStrike();
}

extern "C" double __declspec(dllexport) optionProbFinishBellowStrike(double initialPrice,
        double strike, double avgStep, int nDays) {
	OptionsProbabilities optP(initialPrice, strike, avgStep, nDays);
	return optP.probFinishBellowStrike();
}

extern "C" double __declspec(dllexport) optionProbFinishBetweenPrices(double initialPrice,
        double strike, double avgStep, int nDays, double lowPrice, double highPrice) {
	OptionsProbabilities optP(initialPrice, strike, avgStep, nDays);
	return optP.probFinalPriceBetweenPrices(lowPrice, highPrice);
}

