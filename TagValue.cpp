
#include "StdAfx.h"
#include "TagValue.h"
#include <boost/python.hpp>

using namespace boost::python;

void TagValue::export_to_python()
{
    class_<TagValue>("TagValue")
        .def( init<const std::string &, const std::string &>() )
        .def_readwrite("tag", &TagValue::tag )
        .def_readwrite("value", &TagValue::value )
    ;
}
