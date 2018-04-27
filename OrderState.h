/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef ORDER_STATE_H__INCLUDED
#define ORDER_STATE_H__INCLUDED

#include "Order.h"
#include <boost/python.hpp>

struct OrderState {

	explicit OrderState()
		:
		commission(UNSET_DOUBLE),
		minCommission(UNSET_DOUBLE),
		maxCommission(UNSET_DOUBLE)
	{}

	std::string status;

	std::string initMargin;
	std::string maintMargin;
	std::string equityWithLoan;

	double  commission;
	double  minCommission;
	double  maxCommission;
	std::string commissionCurrency;

	std::string warningText;

    static void export_to_python()
    {
        boost::python::class_<OrderState>( "OrderState", boost::python::init<>() )
            .def_readwrite( "status", &OrderState::status )
            .def_readwrite( "initMargin", &OrderState::initMargin )
            .def_readwrite( "maintMargin", &OrderState::maintMargin )
            .def_readwrite( "equityWithLoan", &OrderState::equityWithLoan )
            .def_readwrite( "commission", &OrderState::commission )
            .def_readwrite( "minCommission", &OrderState::minCommission )
            .def_readwrite( "maxCommission", &OrderState::maxCommission )
            .def_readwrite( "commissionCurrency", &OrderState::commissionCurrency )
            .def_readwrite( "warningText", &OrderState::warningText )
        ;
    }
};

#endif
