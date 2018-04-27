#include "StdAfx.h"
#include "PriceCondition.h"
#include <sstream>
#include "EDecoder.h"
#include "EClient.h"
#include <boost/python.hpp>

using namespace boost::python;

std::string PriceCondition::valueToString() const {
	std::stringstream tmp;

	tmp << m_price;

	return tmp.str();
}

void PriceCondition::valueFromString(const std::string & v) {
	std::stringstream tmp;

	tmp << v;
	tmp >> m_price;
}

double PriceCondition::price() {
	return m_price;
}

void PriceCondition::price(double price) {
	m_price = price;
}

std::string PriceCondition::toString() {
	return strTriggerMethod() + " " + ContractCondition::toString();
}

PriceCondition::Method PriceCondition::triggerMethod() {
	return (Method)m_triggerMethod;
}

std::string PriceCondition::strTriggerMethod() {
	static std::string mthdNames[] = { "default", "double bid/ask", "last", "double last", "bid/ask", "", "", "last of bid/ask", "mid-point" };

	return mthdNames[triggerMethod()];
}

void PriceCondition::triggerMethod(Method triggerMethod) {
	m_triggerMethod = triggerMethod;
}

const char* PriceCondition::readExternal(const char* ptr, const char* endPtr) {
	if (!(ptr = ContractCondition::readExternal(ptr, endPtr)))
		return 0;

	DECODE_FIELD(m_triggerMethod)

	return ptr;
}

void PriceCondition::writeExternal(std::ostream & msg) const {
	ContractCondition::writeExternal(msg);

	ENCODE_FIELD(m_triggerMethod);
}

void PriceCondition::export_to_python()
{
    enum_<Method>("Method")
        .value("Default", PriceCondition::Method::Default)
        .value("DoubleBidAsk", PriceCondition::Method::DoubleBidAsk)
        .value("Last", PriceCondition::Method::Last)
        .value("DoubleLast", PriceCondition::Method::DoubleLast)
        .value("BidAsk", PriceCondition::Method::BidAsk)
        .value("LastBidAsk", PriceCondition::Method::LastBidAsk)
        .value("MidPoint", PriceCondition::Method::MidPoint)
    ;

    class_<PriceCondition>("PriceCondition", no_init )
        .add_property("price", 
                      static_cast<double (PriceCondition::*)()>( &PriceCondition::price ), 
                      static_cast<void (PriceCondition::*)(double)>( &PriceCondition::price ) )
        .add_property("triggerMethod", 
                      static_cast<Method (PriceCondition::*)()>( &PriceCondition::triggerMethod ),
                      static_cast<void (PriceCondition::*)(Method)>( &PriceCondition::triggerMethod ) )
        .def("toString", &PriceCondition::toString )
        .def("strTriggerMethod", &PriceCondition::strTriggerMethod )
        ; 

}
