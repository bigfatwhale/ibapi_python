#include "StdAfx.h"
#include "TimeCondition.h"
#include <boost/python.hpp>

using namespace boost::python;

std::string TimeCondition::valueToString() const {
	return m_time;
}

void TimeCondition::valueFromString(const std::string & v) {
	m_time = v;
}

std::string TimeCondition::toString() {
	return "time" + OperatorCondition::toString();
}

std::string TimeCondition::time() {
	return m_time;
}

void TimeCondition::time(const std::string & time) {
	m_time = time;
}

void TimeCondition::export_to_python()
{
    class_<TimeCondition>("TimeCondition", no_init )
        .add_property("rotime", 
                      static_cast<std::string (TimeCondition::*)(void)>( &TimeCondition::time ) )
        .add_property("time",   
                      static_cast<std::string (TimeCondition::*)(void)>( &TimeCondition::time ),            
                      static_cast<void (TimeCondition::*)(const std::string &)>( &TimeCondition::time ) )  
    ;
}
