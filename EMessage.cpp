/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#include "StdAfx.h"
#include "EMessage.h"
#include <boost/python.hpp>
using namespace boost::python;

EMessage::EMessage(const std::vector<char> &data) {
    this->init(data);
}

EMessage::EMessage(std::string str) {
    std::vector<char> data(str.begin(), str.end());
    this->init(data);
}

void EMessage::init( const std::vector<char> &data ) {
    this->data = data;
}

const char* EMessage::begin(void) const
{
    return data.data();
}

const char* EMessage::end(void) const
{
    return data.data() + data.size();
}

void EMessage::export_to_python()
{
  class_<EMessage>("EMessage", boost::python::init<std::string>() )
        .def("begin", &EMessage::begin)
        .def("end", &EMessage::end)
    ;
}
