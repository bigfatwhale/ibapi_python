/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef commissionreport_def
#define commissionreport_def

#include<boost/python.hpp>

using namespace boost::python;

struct CommissionReport
{
	CommissionReport()
	{
		commission = 0;
		realizedPNL = 0;
		yield = 0;
		yieldRedemptionDate = 0;
	}

    static void export_to_python()
    {
        class_<CommissionReport>("CommissionReport" )
            .def_readwrite("execId",              &CommissionReport::execId)
            .def_readwrite("commission",          &CommissionReport::commission)
            .def_readwrite("currency",            &CommissionReport::currency)
            .def_readwrite("realizedPNL",         &CommissionReport::realizedPNL)
            .def_readwrite("yield",               &CommissionReport::yield)
            .def_readwrite("yieldRedemptionDate", &CommissionReport::yieldRedemptionDate)
        ;
    }

	// commission report fields
	std::string	execId;
	double		commission;
	std::string	currency;
	double		realizedPNL;
	double		yield;
	int			yieldRedemptionDate; // YYYYMMDD format
};

#endif // commissionreport_def
