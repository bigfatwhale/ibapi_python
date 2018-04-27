#include "StdAfx.h"
#include "SoftDollarTier.h"
#include <boost/python.hpp>

using namespace boost::python;

SoftDollarTier::SoftDollarTier(const std::string& name, const std::string& val, const std::string& displayName) :
	m_name(name), m_val(val), m_displayName(displayName)
{
}

std::string SoftDollarTier::name() const
{
	return m_name;
}

std::string SoftDollarTier::val() const
{
	return m_val;
}

std::string SoftDollarTier::displayName() const
{
	return m_displayName;
}

void SoftDollarTier::export_to_python()
{
    class_<SoftDollarTier>("SoftDollarTier", init<const std::string &, const std::string &, const std::string &>() )
        .add_property("name", &SoftDollarTier::name )
        .add_property("val", &SoftDollarTier::val )
        .add_property("displayName", &SoftDollarTier::displayName )

    ;
}
