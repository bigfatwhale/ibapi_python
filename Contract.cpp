
#include "Contract.h"
#include <boost/python.hpp>

using namespace boost::python;

void Contract::export_to_python()
{
    class_<Contract>("Contract")
        .def_readwrite("conId", &Contract::conId )
        .def_readwrite("symbol", &Contract::symbol )
        .def_readwrite("secType", &Contract::secType )
        .def_readwrite("lastTradeDateOrContractMonth", &Contract::lastTradeDateOrContractMonth )
        .def_readwrite("strike", &Contract::strike )
        .def_readwrite("right", &Contract::right )
        .def_readwrite("multiplier", &Contract::multiplier )
        .def_readwrite("exchange", &Contract::exchange )
        .def_readwrite("primaryExchange", &Contract::primaryExchange )
        .def_readwrite("currency", &Contract::currency )
        .def_readwrite("localSymbol", &Contract::localSymbol )
        .def_readwrite("tradingClass", &Contract::tradingClass )
        .def_readwrite("includeExpired", &Contract::includeExpired )
        .def_readwrite("secIdType", &Contract::secIdType )
        .def_readwrite("secId", &Contract::secId )
    ;
}

