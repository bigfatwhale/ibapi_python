/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef execution_def
#define execution_def
#include <boost/python.hpp>

namespace bp = boost::python;

struct Execution
{
	Execution()
	{
		shares = 0;
		price = 0;
		permId = 0;
		clientId = 0;
		orderId = 0;
		cumQty = 0;
		avgPrice = 0;
		evMultiplier = 0;
	}

    static void export_to_python()
    {
        bp::class_<Execution>( "Execution", bp::init<>() )
            .def_readwrite("time", &Execution::time)
            .def_readwrite("acctNumber", &Execution::acctNumber)
            .def_readwrite("exchange", &Execution::exchange)
            .def_readwrite("side", &Execution::side)
            .def_readwrite("shares", &Execution::shares)
            .def_readwrite("price", &Execution::price)
            .def_readwrite("permId", &Execution::permId)
            .def_readwrite("clientId", &Execution::clientId)
            .def_readwrite("orderId", &Execution::orderId)
            .def_readwrite("liquidation", &Execution::liquidation)
            .def_readwrite("cumQty", &Execution::cumQty)
            .def_readwrite("avgPrice", &Execution::avgPrice)
            .def_readwrite("orderRef", &Execution::orderRef)
            .def_readwrite("evRule", &Execution::evRule)
            .def_readwrite("evMultiplier", &Execution::evMultiplier)
            .def_readwrite("modelCode", &Execution::modelCode)
        ;
    }

	std::string	execId;
	std::string	time;
	std::string	acctNumber;
	std::string	exchange;
	std::string	side;
	double		shares;
	double		price;
	int			permId;
	long		clientId;
	long		orderId;
	int			liquidation;
	int			cumQty;
	double		avgPrice;
	std::string	orderRef;
	std::string	evRule;
	double		evMultiplier;
	std::string modelCode;
};

struct ExecutionFilter
{
	ExecutionFilter()
		: m_clientId(0)
	{
	}

    static void export_to_python()
    {
        bp::class_<ExecutionFilter>("ExecutionFilter", bp::init<>())
            .def_readwrite("clientId", &ExecutionFilter::m_clientId)
            .def_readwrite("acctCode", &ExecutionFilter::m_acctCode)
            .def_readwrite("time", &ExecutionFilter::m_time)
            .def_readwrite("symbol", &ExecutionFilter::m_symbol)
            .def_readwrite("secType", &ExecutionFilter::m_secType)
            .def_readwrite("exchange", &ExecutionFilter::m_exchange)
            .def_readwrite("side", &ExecutionFilter::m_side)
        ;
    }

	// Filter fields
	long		m_clientId;
	std::string	m_acctCode;
	std::string	m_time;
	std::string	m_symbol;
	std::string	m_secType;
	std::string	m_exchange;
	std::string	m_side;
};

#endif // execution_def
