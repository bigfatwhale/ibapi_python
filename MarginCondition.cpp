#include "StdAfx.h"
#include "OrderCondition.h"
#include "MarginCondition.h"
#include <sstream>
#include <boost/python.hpp>

using namespace boost::python;

const OrderCondition::OrderConditionType MarginCondition::conditionType;

std::string MarginCondition::valueToString() const {
	std::stringstream tmp;

	tmp << m_percent;

	return tmp.str();
}

void MarginCondition::valueFromString(const std::string & v) {
	std::stringstream tmp;
	
	tmp << v;
	tmp >> m_percent;
}

std::string MarginCondition::toString() {
	return "the margin cushion percent" + OperatorCondition::toString();
}

int MarginCondition::percent() {
	return m_percent;
}

void MarginCondition::percent(int percent) {
	m_percent = percent;
}

void MarginCondition::export_to_python()
{
    class_<MarginCondition>("MarginCondition", no_init)
        .add_property("percent", 
                      static_cast<int (MarginCondition::*)(void)>( &MarginCondition::percent), 
                      static_cast<void (MarginCondition::*)(int)>( &MarginCondition::percent))
        .def("toString", &MarginCondition::toString)
        .add_static_property("conditionType", make_getter( &MarginCondition::conditionType ))
    ;


}
