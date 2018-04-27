/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef scanner_def
#define scanner_def

#include <float.h>
#include <limits.h>
#include <boost/python.hpp>

#define UNSET_DOUBLE DBL_MAX
#define UNSET_INTEGER INT_MAX
#define NO_ROW_NUMBER_SPECIFIED -1;

struct ScannerSubscription {
	ScannerSubscription() {
		numberOfRows = NO_ROW_NUMBER_SPECIFIED;
		abovePrice = DBL_MAX;
		belowPrice = DBL_MAX;
		aboveVolume = INT_MAX;
		marketCapAbove = DBL_MAX;
		marketCapBelow = DBL_MAX;
		couponRateAbove = DBL_MAX;
		couponRateBelow = DBL_MAX;
		excludeConvertible = 0;
		averageOptionVolumeAbove = 0;
	}
    int numberOfRows;
    std::string instrument;
    std::string locationCode;
    std::string scanCode;
    double abovePrice;
    double belowPrice;
    int aboveVolume;
    double marketCapAbove;
    double marketCapBelow;
    std::string moodyRatingAbove;
    std::string moodyRatingBelow;
    std::string spRatingAbove;
    std::string spRatingBelow;
    std::string maturityDateAbove;
    std::string maturityDateBelow;
    double couponRateAbove;
    double couponRateBelow;
    int excludeConvertible;
	int averageOptionVolumeAbove;
	std::string scannerSettingPairs;
	std::string stockTypeFilter;

    static void export_to_python()
    {
        boost::python::class_<ScannerSubscription>("ScannerSubscription", boost::python::init<>() )
            .def_readwrite("numberOfRows", &ScannerSubscription::numberOfRows )
            .def_readwrite("instrument", &ScannerSubscription::instrument )
            .def_readwrite("locationCode", &ScannerSubscription::locationCode )
            .def_readwrite("scanCode", &ScannerSubscription::scanCode )
            .def_readwrite("abovePrice", &ScannerSubscription::abovePrice )
            .def_readwrite("belowPrice", &ScannerSubscription::belowPrice )
            .def_readwrite("aboveVolume", &ScannerSubscription::aboveVolume )
            .def_readwrite("marketCapAbove", &ScannerSubscription::marketCapAbove )
            .def_readwrite("marketCapBelow", &ScannerSubscription::marketCapBelow )
            .def_readwrite("moodyRatingAbove", &ScannerSubscription::moodyRatingAbove )
            .def_readwrite("moodyRatingBelow", &ScannerSubscription::moodyRatingBelow )
            .def_readwrite("spRatingAbove", &ScannerSubscription::spRatingAbove )
            .def_readwrite("spRatingBelow", &ScannerSubscription::spRatingBelow )
            .def_readwrite("maturityDateAbove", &ScannerSubscription::maturityDateAbove )
            .def_readwrite("maturityDateBelow", &ScannerSubscription::maturityDateBelow )
            .def_readwrite("couponRateAbove", &ScannerSubscription::couponRateAbove )
            .def_readwrite("couponRateBelow", &ScannerSubscription::couponRateBelow )
            .def_readwrite("excludeConvertible", &ScannerSubscription::excludeConvertible )
            .def_readwrite("averageOptionVolumeAbove", &ScannerSubscription::averageOptionVolumeAbove )
            .def_readwrite("scannerSettingPairs", &ScannerSubscription::scannerSettingPairs )
            .def_readwrite("stockTypeFilter", &ScannerSubscription::stockTypeFilter )
        ;
    }
};

#endif
