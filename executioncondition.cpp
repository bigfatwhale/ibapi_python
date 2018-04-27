#include "StdAfx.h"
#include "executioncondition.h"
#include "EDecoder.h"
#include "EClient.h"
#include <boost/python.hpp>

using namespace boost::python;

const char* ExecutionCondition::readExternal(const char* ptr, const char* endPtr) {
	if (!(ptr = OrderCondition::readExternal(ptr, endPtr)))
		return 0;

	DECODE_FIELD(m_secType)
	DECODE_FIELD(m_exchange);
	DECODE_FIELD(m_symbol);

	return ptr;
}

std::string ExecutionCondition::toString() {
	return "trade occurs for " + m_symbol + " symbol on " + m_exchange + " exchange for " + m_secType + " security type";
}

void ExecutionCondition::writeExternal(std::ostream & msg) const {
	OrderCondition::writeExternal(msg);

	ENCODE_FIELD(m_secType);
	ENCODE_FIELD(m_exchange);
	ENCODE_FIELD(m_symbol);
}

std::string ExecutionCondition::exchange() {
	return m_exchange;
}

void ExecutionCondition::exchange(const std::string &exchange) {
	m_exchange = exchange;
}

std::string ExecutionCondition::secType() {
	return m_secType;
}

void ExecutionCondition::secType(const std::string &secType) {
	m_secType = secType;
}

std::string ExecutionCondition::symbol() {
	return m_symbol;
}

void ExecutionCondition::symbol(const std::string &symbol) {
	m_symbol = symbol;
}

void ExecutionCondition::export_to_python()
{

    class_<ExecutionCondition>("ExecutionCondition", no_init)
        .add_property("exchange", 
                      static_cast<std::string (ExecutionCondition::*)(void)>(&ExecutionCondition::exchange), 
                      static_cast<void (ExecutionCondition::*)(const std::string &)>(&ExecutionCondition::exchange) )
        .add_property("secType", 
                      static_cast<std::string (ExecutionCondition::*)(void)>(&ExecutionCondition::secType), 
                      static_cast<void (ExecutionCondition::*)(const std::string &)>(&ExecutionCondition::secType) )
        .add_property("symbol", 
                      static_cast<std::string (ExecutionCondition::*)(void)>(&ExecutionCondition::symbol), 
                      static_cast<void (ExecutionCondition::*)(const std::string &)>(&ExecutionCondition::symbol) )
        .def("toString", &ExecutionCondition::toString )
        ;


}
