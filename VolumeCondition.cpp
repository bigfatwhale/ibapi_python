#include "StdAfx.h"
#include "VolumeCondition.h"
#include <sstream>
#include <boost/python.hpp>

using namespace boost::python;

std::string VolumeCondition::valueToString() const {
	std::stringstream tmp;

	tmp << m_volume;

	return tmp.str();
}

void VolumeCondition::valueFromString(const std::string & v) {
	std::stringstream tmp;

	tmp << v;
	tmp >> m_volume;
}

int VolumeCondition::volume() {
	return m_volume;
}

void VolumeCondition::volume(int volume) {
	m_volume = volume;
}

void VolumeCondition::export_to_python()
{
    class_<VolumeCondition>("VolumeCondition", no_init )
        .add_property("volume",   
                      static_cast<int (VolumeCondition::*)(void)>( &VolumeCondition::volume ),            
                      static_cast<void (VolumeCondition::*)(int)>( &VolumeCondition::volume ) )  
    ;
}
