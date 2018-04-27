#include "StdAfx.h"
#include "PercentChangeCondition.h"
#include <sstream>
#include <boost/python.hpp>

using namespace boost::python;

std::string PercentChangeCondition::valueToString() const {
	std::stringstream tmp;

	tmp << m_changePercent;

	return tmp.str();
}

void PercentChangeCondition::valueFromString(const std::string & v) {
	std::stringstream tmp;
	
	tmp << v;
	tmp >> m_changePercent;
}

double PercentChangeCondition::changePercent() {
	return m_changePercent;
}

void PercentChangeCondition::changePercent(double changePercent) {
	m_changePercent = changePercent;
}

void PercentChangeCondition::export_to_python()
{
    class_<PercentChangeCondition>("PercentChangeCondition", no_init )
        .add_property("rochangePercent", 
                      static_cast<double (PercentChangeCondition::*)(void)>( &PercentChangeCondition::changePercent ) )
        .add_property("changePercent",   
                      static_cast<double (PercentChangeCondition::*)(void)>( &PercentChangeCondition::changePercent ),            
                      static_cast<void (PercentChangeCondition::*)(double)>( &PercentChangeCondition::changePercent ) )
        .def("valueToString",  &PercentChangeCondition::valueToString )
        .def("valueFromString", &PercentChangeCondition::valueFromString )
        
    ;
}

