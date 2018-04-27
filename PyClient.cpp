
#include "StdAfx.h"
#include "PyClient.h"
#include "EClientSocket.h"
#include "Contract.h"
#include "Order.h"
#include "OrderState.h"
#include "Execution.h"
#include "CommissionReport.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <ctime>
#include <boost/python.hpp>
#include <map>

using namespace boost::python;

const int PING_DEADLINE = 2; // seconds
const int SLEEP_BETWEEN_PINGS = 30; // seconds

///////////////////////////////////////////////////////////
// member funcs
//! [socket_init]
PyClientBase::PyClientBase() :
    m_osSignal(2000)//2-seconds timeout
    , m_pClient(new EClientSocket(this, &m_osSignal))
    , m_state(ST_CONNECT)
    , m_sleepDeadline(0)
    , m_orderId(0)
    , m_pReader(0)
    , m_extraAuth(false)
{
}
    
PyClientBase::~PyClientBase()
{
    if (m_pReader)
        delete m_pReader;

    delete m_pClient;
}

bool PyClientBase::connect(const char *host, unsigned int port, int clientId)
{
    // trying to connect
    printf( "Connecting to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientId);

    bool bRes = m_pClient->eConnect( host, port, clientId, m_extraAuth);

    if (bRes) {
        printf( "Connected to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);

        m_pReader = new EReader(m_pClient, &m_osSignal);

        m_pReader->start();
    }
    else
        printf( "Cannot connect to %s:%d clientId:%d\n", m_pClient->host().c_str(), m_pClient->port(), clientId);

    return bRes;
}

void PyClientBase::disconnect() const
{
    m_pClient->eDisconnect();

    printf ( "Disconnected\n");
}

bool PyClientBase::isConnected() const
{
    return m_pClient->isConnected();
}

void PyClientBase::setConnectOptions(const std::string& connectOptions)
{
    m_pClient->setConnectOptions(connectOptions);
}

void PyClientBase::processMessages() {
	fd_set readSet, writeSet, errorSet;

	struct timeval tval;
	tval.tv_usec = 0;
	tval.tv_sec = 0;

	time_t now = time(NULL);

	/*****************************************************************/
    /* Below are few quick-to-test examples on the IB API functions grouped by functionality. Uncomment the relevant methods. */
    /*****************************************************************/
    //std::cout << "processMessages : state = " << m_state <<std::endl;
	switch (m_state) {
		case ST_TICKDATAOPERATION:
			tickDataOperation();
			break;
		case ST_TICKDATAOPERATION_ACK:
			break;
		case ST_MARKETDEPTHOPERATION:
			marketDepthOperations();
			break;
		case ST_MARKETDEPTHOPERATION_ACK:
			break;
		case ST_REALTIMEBARS:
			realTimeBars();
			break;
		case ST_REALTIMEBARS_ACK:
			break;
		case ST_MARKETDATATYPE:
            std::cout << "processMessages : state = ST_MARKETDATATYPE" <<std::endl;
			mktDataType();
			break;
		case ST_MARKETDATATYPE_ACK:
            std::cout << "processMessages : state = ST_MARKETDATATYPE_ACK" <<std::endl;
			break;
		case ST_HISTORICALDATAREQUESTS:
            std::cout << "processMessages : state = ST_HISTORICALDATAREQUESTS" <<std::endl;
			historicalDataRequests();
			break;
		case ST_HISTORICALDATAREQUESTS_ACK:
            std::cout << "processMessages : state = ST_HISTORICALDATAREQUESTS_ACK" <<std::endl;
			break;
		case ST_OPTIONSOPERATIONS:
            std::cout << "processMessages : state = ST_OPTIONSOPERATIONS" <<std::endl;
			optionsOperations();
			break;
		case ST_OPTIONSOPERATIONS_ACK:
            std::cout << "processMessages : state = ST_OPTIONSOPERATIONS_ACK" <<std::endl;
			break;
		case ST_CONTRACTOPERATION:
            std::cout << "processMessages : state = ST_CONTRACTOPERATION" <<std::endl;
			contractOperations();
			break;
		case ST_CONTRACTOPERATION_ACK:
            std::cout << "processMessages : state = ST_CONTRACTOPERATION_ACK" <<std::endl;
			break;
		case ST_MARKETSCANNERS:
            std::cout << "processMessages : state = ST_MARKETSCANNERS" <<std::endl;
			marketScanners();
			break;
		case ST_MARKETSCANNERS_ACK:
            std::cout << "processMessages : state = ST_MARKETSCANNERS_ACK" <<std::endl;
			break;
		case ST_REUTERSFUNDAMENTALS:
			reutersFundamentals();
			break;
		case ST_REUTERSFUNDAMENTALS_ACK:
			break;
		case ST_BULLETINS:
            std::cout << "processMessages : state = ST_BULLETINS" <<std::endl;
			bulletins();
			break;
		case ST_BULLETINS_ACK:
            std::cout << "processMessages : state = ST_BULLETINS_ACK" <<std::endl;
			break;
		case ST_ACCOUNTOPERATIONS:
			accountOperations();
			break;
		case ST_ACCOUNTOPERATIONS_ACK:
			break;
		case ST_ORDEROPERATIONS:
			orderOperations();
			break;
		case ST_ORDEROPERATIONS_ACK:
			break;
		case ST_OCASAMPLES:
            std::cout << "processMessages : state = ST_OCASAMPLES" <<std::endl;
			ocaSamples();
			break;
		case ST_OCASAMPLES_ACK:
            std::cout << "processMessages : state = ST_OCASAMPLES_ACK" <<std::endl;
			break;
		case ST_CONDITIONSAMPLES:
            std::cout << "processMessages : state = ST_CONDITIONSAMPLES" <<std::endl;
			conditionSamples();
			break;
		case ST_CONDITIONSAMPLES_ACK:
            std::cout << "processMessages : state = ST_CONDITIONSAMPLES_ACK" <<std::endl;
			break;
		case ST_BRACKETSAMPLES:
            std::cout << "processMessages : state = ST_BRACKETSAMPLES" <<std::endl;
			bracketSample();
			break;
		case ST_BRACKETSAMPLES_ACK:
            std::cout << "processMessages : state = ST_BRACKETSAMPLES_ACK" <<std::endl;
			break;
		case ST_HEDGESAMPLES:
            std::cout << "processMessages : state = ST_HEDGESAMPLES" <<std::endl;
			hedgeSample();
			break;
		case ST_HEDGESAMPLES_ACK:
            std::cout << "processMessages : state = ST_HEDGESAMPLES_ACK" <<std::endl;
			break;
		case ST_TESTALGOSAMPLES:
            std::cout << "processMessages : state = ST_TESTALGOSAMPLES" <<std::endl;
			testAlgoSamples();
			break;
		case ST_TESTALGOSAMPLES_ACK:
            std::cout << "processMessages : state = ST_TESTALGOSAMPLES_ACK" <<std::endl;
			break;
		case ST_FAORDERSAMPLES:
            std::cout << "processMessages : state = ST_FAORDERSAMPLES" <<std::endl;
			financialAdvisorOrderSamples();
			break;
		case ST_FAORDERSAMPLES_ACK:
            std::cout << "processMessages : state = ST_FAORDERSAMPLES_ACK" <<std::endl;
			break;
		case ST_FAOPERATIONS:
            std::cout << "processMessages : state = ST_FAOPERATIONS" <<std::endl;
			financialAdvisorOperations();
			break;
		case ST_FAOPERATIONS_ACK:
            std::cout << "processMessages : state = ST_FAOPERATIONS_ACK" <<std::endl;
			break;
		case ST_DISPLAYGROUPS:
            std::cout << "processMessages : state = ST_DISPLAYGROUPS" <<std::endl;
			testDisplayGroups();
			break;
		case ST_DISPLAYGROUPS_ACK:
            std::cout << "processMessages : state = ST_DISPLAYGROUPS_ACK" <<std::endl;
			break;
		case ST_MISCELANEOUS:
            std::cout << "processMessages : state = ST_MISCELANEOUS" <<std::endl;
			miscelaneous();
			break;
		case ST_MISCELANEOUS_ACK:
            std::cout << "processMessages : state = ST_MISCELANEOUS_ACK" <<std::endl;
			break;
		case ST_PING:
            std::cout << "processMessages : state = ST_PING" <<std::endl;
			reqCurrentTime();
			break;
		case ST_PING_ACK:
            std::cout << "processMessages : state = ST_PING_ACK" <<std::endl;
			if( m_sleepDeadline < now) {
				disconnect();
				return;
			}
			break;
		case ST_IDLE:
            std::cout << "processMessages : state = ST_IDLE" <<std::endl;
			if( m_sleepDeadline < now) {
				m_state = ST_PING;
				return;
			}
			break;
	}

	if( m_sleepDeadline > 0) {
		// initialize timeout with m_sleepDeadline - now
		tval.tv_sec = m_sleepDeadline - now;
	}

    if( m_pReader )
    {
        m_pReader->checkClient();
        m_osSignal.waitForSignal();
        m_pReader->processMsgs();
    }
}

void PyClientBase::reqAccountSummary( int reqId, const std::string& groupName, const std::string& tags) 
{ 
    m_pClient->reqAccountSummary( reqId, groupName, tags ); 
}                 

void PyClientBase::reqAccountUpdates( bool subscribe, const std::string& acctCode )
{ 
    m_pClient->reqAccountUpdates(subscribe, acctCode); 
}

void PyClientBase::reqMktData( TickerId id, const Contract& contract, 
                               const std::string& genericTicks, bool snapshot )
{
    m_pClient->reqMktData( id, contract, genericTicks, snapshot, TagValueListSPtr() );
}

void PyClientBase::reqMktDepth( TickerId tickerId, const Contract& contract, int numRows )
{
    m_pClient->reqMktDepth( tickerId, contract, numRows, TagValueListSPtr() );
}

void PyClientBase::reqFundamentalData( TickerId reqId, const Contract& contract,  
                                       const std::string& reportType) 
{
    m_pClient->reqFundamentalData( reqId, contract, reportType );
}

void PyClientBase::reqRealTimeBars(TickerId tickerId, const Contract& contract,
                                   int barSize, const std::string& whatToShow, bool useRTH)
{
    m_pClient->reqRealTimeBars( tickerId, contract, barSize, whatToShow, useRTH, TagValueListSPtr() );
}

void PyClientBase::placeOrder( OrderId id, const Contract& contract, const Order& order)
{
    m_pClient->placeOrder( id, contract, order );
}

void PyClientBase::cancelMktData( TickerId id )
{
    m_pClient->cancelMktData(id);
}

void PyClientBase::cancelMktDepth( TickerId id )
{
    m_pClient->cancelMktDepth(id);
}

struct acquire_gil_call_policy : default_call_policies
{
    static bool precall(PyObject*)
    { 
        PyGILState_STATE gstate = PyGILState_Ensure();
        std::thread::id curThreadId = std::this_thread::get_id();
        acquire_gil_call_policy::state_map.insert( 
            std::pair<std::thread::id, PyGILState_STATE>(curThreadId, gstate) );
        
        std::cout << "Thread " << curThreadId << " acquiring GIL" << std::endl;
        return true;
    }

    static PyObject* postcall(PyObject*, PyObject* result)
    {
        std::thread::id curThreadId = std::this_thread::get_id();
        PyGILState_STATE gstate = acquire_gil_call_policy::state_map[ curThreadId ];
        PyGILState_Release(gstate);
        std::cout << "Thread " << curThreadId << " releasing GIL" << std::endl;
        return result;
    }
    static std::map<std::thread::id, PyGILState_STATE> state_map; 
};

std::map<std::thread::id, PyGILState_STATE> acquire_gil_call_policy::state_map;

void PyClientBaseWrap::export_to_python()
{
    enum_<State>("State")
        .value("ST_CONNECT", ST_CONNECT )
        .value("ST_TICKDATAOPERATION", ST_TICKDATAOPERATION)
        .value("ST_TICKDATAOPERATION_ACK", ST_TICKDATAOPERATION_ACK)
        .value("ST_MARKETDEPTHOPERATION", ST_MARKETDEPTHOPERATION)
        .value("ST_MARKETDEPTHOPERATION_ACK", ST_MARKETDEPTHOPERATION_ACK)
        .value("ST_REALTIMEBARS", ST_REALTIMEBARS)
        .value("ST_REALTIMEBARS_ACK", ST_REALTIMEBARS_ACK)
        .value("ST_MARKETDATATYPE", ST_MARKETDATATYPE)
        .value("ST_MARKETDATATYPE_ACK", ST_MARKETDATATYPE_ACK)
        .value("ST_HISTORICALDATAREQUESTS", ST_HISTORICALDATAREQUESTS)
        .value("ST_HISTORICALDATAREQUESTS_ACK", ST_HISTORICALDATAREQUESTS_ACK)
        .value("ST_OPTIONSOPERATIONS", ST_OPTIONSOPERATIONS)
        .value("ST_OPTIONSOPERATIONS_ACK", ST_OPTIONSOPERATIONS_ACK)
        .value("ST_CONTRACTOPERATION", ST_CONTRACTOPERATION)
        .value("ST_CONTRACTOPERATION_ACK", ST_CONTRACTOPERATION_ACK)
        .value("ST_MARKETSCANNERS", ST_MARKETSCANNERS)
        .value("ST_MARKETSCANNERS_ACK", ST_MARKETSCANNERS_ACK)
        .value("ST_REUTERSFUNDAMENTALS", ST_REUTERSFUNDAMENTALS)
        .value("ST_REUTERSFUNDAMENTALS_ACK", ST_REUTERSFUNDAMENTALS_ACK)
        .value("ST_BULLETINS", ST_BULLETINS)
        .value("ST_BULLETINS_ACK", ST_BULLETINS_ACK)
        .value("ST_ACCOUNTOPERATIONS", ST_ACCOUNTOPERATIONS)
        .value("ST_ACCOUNTOPERATIONS_ACK", ST_ACCOUNTOPERATIONS_ACK)
        .value("ST_ORDEROPERATIONS", ST_ORDEROPERATIONS)
        .value("ST_ORDEROPERATIONS_ACK", ST_ORDEROPERATIONS_ACK)
        .value("ST_OCASAMPLES", ST_OCASAMPLES)
        .value("ST_OCASAMPLES_ACK", ST_OCASAMPLES_ACK)
        .value("ST_CONDITIONSAMPLES", ST_CONDITIONSAMPLES)
        .value("ST_CONDITIONSAMPLES_ACK", ST_CONDITIONSAMPLES_ACK)
        .value("ST_BRACKETSAMPLES", ST_BRACKETSAMPLES)
        .value("ST_BRACKETSAMPLES_ACK", ST_BRACKETSAMPLES_ACK)
        .value("ST_HEDGESAMPLES", ST_HEDGESAMPLES)
        .value("ST_HEDGESAMPLES_ACK", ST_HEDGESAMPLES_ACK)
        .value("ST_TESTALGOSAMPLES", ST_TESTALGOSAMPLES)
        .value("ST_TESTALGOSAMPLES_ACK", ST_TESTALGOSAMPLES_ACK)
        .value("ST_FAORDERSAMPLES", ST_FAORDERSAMPLES)
        .value("ST_FAORDERSAMPLES_ACK", ST_FAORDERSAMPLES_ACK)
        .value("ST_FAOPERATIONS", ST_FAOPERATIONS)
        .value("ST_FAOPERATIONS_ACK", ST_FAOPERATIONS_ACK)
        .value("ST_DISPLAYGROUPS", ST_DISPLAYGROUPS)
        .value("ST_DISPLAYGROUPS_ACK", ST_DISPLAYGROUPS_ACK)
        .value("ST_MISCELANEOUS", ST_MISCELANEOUS)
        .value("ST_MISCELANEOUS_ACK", ST_MISCELANEOUS_ACK)
        .value("ST_CANCELORDER", ST_CANCELORDER)
        .value("ST_CANCELORDER_ACK", ST_CANCELORDER_ACK)
        .value("ST_PING", ST_PING)
        .value("ST_PING_ACK", ST_PING_ACK)
        .value("ST_IDLE", ST_IDLE)
    ;

    class_<PyClientBaseWrap, boost::noncopyable>("PyClientBaseWrap", init<>())

        .add_property("state", &PyClientBaseWrap::get_state, &PyClientBaseWrap::set_state )
        .add_property("orderId", &PyClientBaseWrap::get_orderid, &PyClientBaseWrap::set_orderid )

        .def( "connect", &PyClientBaseWrap::connect )
        .def( "disconnect",&PyClientBaseWrap::disconnect )
        .def( "isConnected", &PyClientBaseWrap::isConnected )
        .def( "setConnectOptions", &PyClientBaseWrap::setConnectOptions )
        .def( "processMessages", &PyClientBaseWrap::processMessages )
        
        // EClientSocket interface methods
        .def( "reqAccountSummary", &PyClientBaseWrap::reqAccountSummary )
        .def( "reqAccountUpdates", &PyClientBaseWrap::reqAccountUpdates )
        .def( "reqMktData", &PyClientBaseWrap::reqMktData )
        .def( "reqMktDepth", &PyClientBaseWrap::reqMktDepth )
        .def( "reqFundamentalData", &PyClientBaseWrap::reqFundamentalData )
        .def( "reqRealTimeBars", &PyClientBaseWrap::reqRealTimeBars )
        .def( "cancelMktData", &PyClientBaseWrap::cancelMktData )
        .def( "cancelMktDepth", &PyClientBaseWrap::cancelMktDepth )
        .def( "placeOrder", &PyClientBaseWrap::placeOrder )

        // EWrapper interface methods.
        .def( "tickPrice", pure_virtual( &PyClientBase::tickPrice ) )
        .def( "tickSize", pure_virtual( &PyClientBase::tickSize ) )
        .def( "tickOptionComputation", pure_virtual( &PyClientBase::tickOptionComputation ) )
        .def( "tickGeneric", pure_virtual( &PyClientBase::tickGeneric ) )
        .def( "tickString", pure_virtual( &PyClientBase::tickString ) )
        .def( "tickEFP", pure_virtual( &PyClientBase::tickEFP ) )
        .def( "orderStatus", pure_virtual( &PyClientBase::orderStatus ) )
        .def( "openOrder", pure_virtual( &PyClientBase::openOrder ) )
        .def( "openOrderEnd", pure_virtual( &PyClientBase::openOrderEnd ) )
        .def( "winError", pure_virtual( &PyClientBase::winError ) )
        .def( "connectionClosed", pure_virtual( &PyClientBase::connectionClosed ) )
        .def( "updateAccountValue", pure_virtual( &PyClientBase::updateAccountValue ) )
        .def( "updatePortfolio", pure_virtual( &PyClientBase::updatePortfolio ) )
        .def( "updateAccountTime", pure_virtual( &PyClientBase::updateAccountTime ) )
        .def( "accountDownloadEnd", pure_virtual( &PyClientBase::accountDownloadEnd ) )
        .def( "nextValidId", pure_virtual( &PyClientBase::nextValidId ) )
        .def( "contractDetails", pure_virtual( &PyClientBase::contractDetails ) )
        .def( "bondContractDetails", pure_virtual( &PyClientBase::bondContractDetails ) )
        .def( "contractDetailsEnd", pure_virtual( &PyClientBase::contractDetailsEnd ) )
        .def( "execDetails", pure_virtual( &PyClientBase::execDetails ) )
        .def( "execDetailsEnd", pure_virtual( &PyClientBase::execDetailsEnd ) )
        .def( "error", pure_virtual( &PyClientBase::error ) )
        .def( "updateMktDepth", pure_virtual( &PyClientBase::updateMktDepth ) )
        .def( "updateMktDepthL2", pure_virtual( &PyClientBase::updateMktDepthL2 ) )
        .def( "updateNewsBulletin", pure_virtual( &PyClientBase::updateNewsBulletin ) )
        .def( "managedAccounts", pure_virtual( &PyClientBase::managedAccounts ) )
        .def( "receiveFA", pure_virtual( &PyClientBase::receiveFA ) )
        .def( "historicalData", pure_virtual( &PyClientBase::historicalData ) )
        .def( "scannerParameters", pure_virtual( &PyClientBase::scannerParameters ) )
        .def( "scannerData", pure_virtual( &PyClientBase::scannerData ) )
        .def( "scannerDataEnd", pure_virtual( &PyClientBase::scannerDataEnd ) )
        .def( "realtimeBar", pure_virtual( &PyClientBase::realtimeBar ) )
        .def( "currentTime", pure_virtual( &PyClientBase::currentTime ) )
        .def( "fundamentalData", pure_virtual( &PyClientBase::fundamentalData ) )
        .def( "deltaNeutralValidation", pure_virtual( &PyClientBase::deltaNeutralValidation ) )
        .def( "tickSnapshotEnd", pure_virtual( &PyClientBase::tickSnapshotEnd ) )
        //            .def( "marketDataType", pure_virtual( static_cast<void (PyClientBase::*)(TickerId reqId, int marketDataType) >( &PyClientBase::marketDataType ) ) )
        .def( "marketDataType", pure_virtual( &PyClientBase::marketDataType ) )
        .def( "commissionReport", pure_virtual( &PyClientBase::commissionReport ) )
        .def( "position", pure_virtual( &PyClientBase::position ) )
        .def( "positionEnd", pure_virtual( &PyClientBase::positionEnd ) )
        .def( "accountSummary", pure_virtual( &PyClientBase::accountSummary ) )
        .def( "accountSummaryEnd", pure_virtual( &PyClientBase::accountSummaryEnd ) )
        .def( "verifyMessageAPI", pure_virtual( &PyClientBase::verifyMessageAPI ) )
        .def( "verifyCompleted", pure_virtual( &PyClientBase::verifyCompleted ) )
        .def( "displayGroupList", pure_virtual( &PyClientBase::displayGroupList ) )
        .def( "displayGroupUpdated", pure_virtual( &PyClientBase::displayGroupUpdated ) )
        .def( "verifyAndAuthMessageAPI", pure_virtual( &PyClientBase::verifyAndAuthMessageAPI ) )
        .def( "verifyAndAuthCompleted", pure_virtual( &PyClientBase::verifyAndAuthCompleted ) )
        .def( "connectAck", pure_virtual( &PyClientBase::connectAck ) )
        .def( "positionMulti", pure_virtual( &PyClientBase::positionMulti ) )
        .def( "positionMultiEnd", pure_virtual( &PyClientBase::positionMultiEnd ) )
            .def( "accountUpdateMulti", pure_virtual( &PyClientBase::accountUpdateMulti ) )
        .def( "accountUpdateMultiEnd", pure_virtual( &PyClientBase::accountUpdateMultiEnd ) )
        .def( "securityDefinitionOptionalParameter", pure_virtual( &PyClientBase::securityDefinitionOptionalParameter ) )
        .def( "securityDefinitionOptionalParameterEnd", pure_virtual( &PyClientBase::securityDefinitionOptionalParameterEnd ) )
        .def( "softDollarTiers", pure_virtual( &PyClientBase::softDollarTiers ) )
    ;
}


