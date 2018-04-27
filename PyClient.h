
#pragma once
#ifndef PYCLIENT_H_INCLUDED
#define PYCLIENT_H_INCLUDED


#include "EWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h"
#include <Python.h>
#include <boost/python.hpp>
#include <iostream>

namespace bp = boost::python;

class EClientSocket;

enum State {
    ST_CONNECT,
    ST_TICKDATAOPERATION,
    ST_TICKDATAOPERATION_ACK,
    ST_MARKETDEPTHOPERATION,
    ST_MARKETDEPTHOPERATION_ACK,
    ST_REALTIMEBARS,
    ST_REALTIMEBARS_ACK,
    ST_MARKETDATATYPE,
    ST_MARKETDATATYPE_ACK,
    ST_HISTORICALDATAREQUESTS,
    ST_HISTORICALDATAREQUESTS_ACK,
    ST_OPTIONSOPERATIONS,
    ST_OPTIONSOPERATIONS_ACK,
    ST_CONTRACTOPERATION,
    ST_CONTRACTOPERATION_ACK,
    ST_MARKETSCANNERS,
    ST_MARKETSCANNERS_ACK,
    ST_REUTERSFUNDAMENTALS,
    ST_REUTERSFUNDAMENTALS_ACK,
    ST_BULLETINS,
    ST_BULLETINS_ACK,
    ST_ACCOUNTOPERATIONS,
    ST_ACCOUNTOPERATIONS_ACK,
    ST_ORDEROPERATIONS,
    ST_ORDEROPERATIONS_ACK,
    ST_OCASAMPLES,
    ST_OCASAMPLES_ACK,
    ST_CONDITIONSAMPLES,
    ST_CONDITIONSAMPLES_ACK,
    ST_BRACKETSAMPLES,
    ST_BRACKETSAMPLES_ACK,
    ST_HEDGESAMPLES,
    ST_HEDGESAMPLES_ACK,
    ST_TESTALGOSAMPLES,
    ST_TESTALGOSAMPLES_ACK,
    ST_FAORDERSAMPLES,
    ST_FAORDERSAMPLES_ACK,
    ST_FAOPERATIONS,
    ST_FAOPERATIONS_ACK,
    ST_DISPLAYGROUPS,
    ST_DISPLAYGROUPS_ACK,
    ST_MISCELANEOUS,
    ST_MISCELANEOUS_ACK,
    ST_CANCELORDER,
    ST_CANCELORDER_ACK,
    ST_PING,
    ST_PING_ACK,
    ST_IDLE
};


class PyClientBase : public EWrapper
{
    // class only does connection establishment and low level plumbing. Extend with python  

public:

    PyClientBase();
    ~PyClientBase();

    void setConnectOptions(const std::string&);
    void processMessages();

    bool connect(const char * host, unsigned int port, int clientId = 0);
    void disconnect() const;
    bool isConnected() const;

    State state() { return m_state; }
    void state( State &s ) { m_state = s; }

    OrderId orderId() { return m_orderId; }
    void orderId( OrderId &o ) { m_orderId = o; }
    static void export_to_python();


    // main client api operations
    virtual void accountOperations() { std::cout << "PyClientBase.accountOperations" << std::endl; }
	virtual void tickDataOperation() { std::cout << "PyClientBase.tickDataOperation" << std::endl; }
	virtual void reutersFundamentals() {std::cout << "PyClientBase.reutersFundamentals" << std::endl; }
	virtual void marketDepthOperations() {std::cout << "PyClientBase.marketDepthOperations" << std::endl; }
	virtual void realTimeBars() {std::cout << "PyClientBase.realTimeBars" << std::endl; }
	virtual void orderOperations() {std::cout << "PyClientBase.orderOperations" << std::endl; }

    // forward api calls from python to EClientSocket
    void reqAccountSummary( int reqId, const std::string& groupName, const std::string& tags);
    void reqAccountUpdates( bool subscribe, const std::string& acctCode );
    void reqMktData( TickerId id, const Contract& contract, const std::string& genericTicks, 
                     bool snapshot 
                     /*, const TagValueListSPtr& mktDataOptions*/ // this is unused
                     );
    void reqMktDepth( TickerId tickerId, const Contract& contract, int numRows );
    void reqFundamentalData( TickerId reqId, const Contract& contract,                                         
                             const std::string& reportType);
    void reqRealTimeBars(TickerId tickerId, const Contract& contract,
                         int barSize, const std::string& whatToShow, bool useRTH );
    void placeOrder( OrderId id, const Contract& contract, const Order& order);                   

    void cancelMktData( TickerId id );
    void cancelMktDepth( TickerId id );

private:
	void mktDataType() {}
	void historicalDataRequests(){}
	void optionsOperations(){}
	void ocaSamples(){}
	void conditionSamples(){}
	void bracketSample(){}
	void hedgeSample(){}
	void contractOperations(){}
	void marketScanners(){}
	void bulletins(){}
	void testAlgoSamples(){}
	void financialAdvisorOrderSamples(){}
	void financialAdvisorOperations(){}
	void testDisplayGroups(){}
	void miscelaneous(){}

	void reqCurrentTime(){}

private:
    //! [socket_declare]
    EReaderOSSignal m_osSignal;
    EClientSocket * const m_pClient;
    //! [socket_declare]
    State m_state;
    time_t m_sleepDeadline;

    OrderId m_orderId;
    EReader *m_pReader;
    bool m_extraAuth;

};

struct PyLockGIL
{
    PyLockGIL() : gstate(PyGILState_Ensure()) { }
    ~PyLockGIL() { PyGILState_Release(gstate); }
    PyGILState_STATE gstate;
};


// wrapper class to make virtual function that's going to be overridden in Python and 
// called polymorphically from C++ work properly
struct PyClientBaseWrap : PyClientBase, bp::wrapper<PyClientBase>
{

    PyClientBaseWrap() : PyClientBase() {}

    // ------------ get/set methods ------------- //
    State get_state(){ return state(); }
    void set_state( State s ) { state(s); }
    OrderId get_orderid(){ return orderId(); }
    void set_orderid( OrderId o ) { orderId(o); }

    // ------------ Setup test client methods for python override --------------------// 
    void accountOperations()
    {
        this->get_override("accountOperations")();
    }

    void tickDataOperation()
    {
        this->get_override("tickDataOperation")();
    }

    void reutersFundamentals()
    {
        this->get_override("reutersFundamentals")();
    }

    void marketDepthOperations()
    {
        this->get_override("marketDepthOperations")();
    }

    void realTimeBars()
    {
        this->get_override("realTimeBars")();
    }

    void orderOperations()
    {
        this->get_override("orderOperations")();
    }

    // ----------------- EWrapper interface methods begin here ---------------------- //
    void tickPrice( TickerId tickerId, TickType field, double price, int canAutoExecute) 
    { 
        this->get_override("tickPrice")( tickerId, field, price, canAutoExecute);
    }

    void tickSize( TickerId tickerId, TickType field, int size) 
    {
        this->get_override("tickSize")( tickerId, field, size);
    }

    void tickOptionComputation( TickerId tickerId, TickType tickType, double impliedVol, 
                                double delta, double optPrice, double pvDividend, double gamma, 
                                double vega, double theta, double undPrice) 
    {
        this->get_override("tickOptionComputation")( tickerId, tickType, impliedVol, delta, 
                                                     optPrice, pvDividend, gamma, vega, theta, 
                                                     undPrice);
    }

    void tickGeneric(TickerId tickerId, TickType tickType, double value)
    {
        this->get_override("tickGeneric")( tickerId, tickType, value ); 
    }

    void tickString(TickerId tickerId, TickType tickType, const std::string& value)
    {
        this->get_override("tickString")( tickerId, tickType, value ); 
    }

    void tickEFP(TickerId tickerId, TickType tickType, double basisPoints, 
                 const std::string& formattedBasisPoints, double totalDividends, int holdDays, 
                 const std::string& futureLastTradeDate, double dividendImpact, 
                 double dividendsToLastTradeDate)
    {
        this->get_override("tickEFP")(tickerId, tickType, basisPoints, formattedBasisPoints, 
                                      totalDividends, holdDays, futureLastTradeDate, dividendImpact, 
                                      dividendsToLastTradeDate);
    }

    void orderStatus( OrderId orderId, const std::string& status, double filled,
                      double remaining, double avgFillPrice, int permId, int parentId,
                      double lastFillPrice, int clientId, const std::string& whyHeld) 
    {
        this->get_override("orderStatus")( orderId, status, filled, remaining, avgFillPrice, 
                                           permId, parentId, lastFillPrice, clientId, whyHeld);
    }
  
    void openOrder( OrderId orderId, const Contract& c, const Order& o, const OrderState& os)
    {
        this->get_override("openOrder")(orderId, c, o, os);
    }

    void openOrderEnd() 
    {
        this->get_override("openOrderEnd")();
    }

    void winError( const std::string& str, int lastError)
    {
        this->get_override("winError")(str, lastError);
    }

    void connectionClosed() 
    {
        this-> get_override("connectionClosed")();
    }

    void updateAccountValue(const std::string& key, const std::string& val,
                            const std::string& currency, const std::string& accountName)
    {
        this->get_override("updateAccountValue")(key, val, currency, accountName);
    }

    void updatePortfolio( const Contract& contract, double position,
                          double marketPrice, double marketValue, double averageCost,
                          double unrealizedPNL, double realizedPNL, const std::string& accountName)
    {
        this->get_override("updatePortfolio")(contract, position, marketPrice, marketValue, 
                                              averageCost, unrealizedPNL, realizedPNL, accountName);
    }

    void updateAccountTime(const std::string& timeStamp)
    {
        this->get_override("updateAccountTime")(timeStamp);
    }

    void accountDownloadEnd(const std::string& accountName) 
    {
        this->get_override("accountDownloadEnd")(accountName);
    }

    void nextValidId( OrderId orderId) 
    {    
        this->get_override("nextValidId")(orderId);
    }
    
    void contractDetails( int reqId, const ContractDetails& contractDetails) 
    {  
        this->get_override("contractDetails")(reqId, contractDetails);
    }

    void bondContractDetails( int reqId, const ContractDetails& contractDetails) 
    {  
        this->get_override("bondContractDetails")(reqId, contractDetails);
    }
    
    void contractDetailsEnd( int reqId) 
    { 
        this->get_override("contractDetailsEnd")(reqId);
    }
    
    void execDetails( int reqId, const Contract& contract, const Execution& execution)
    {
        this->get_override("execDetails")(reqId, contract, execution);
    }

    void execDetailsEnd( int reqId)
    { 
        this->get_override("execDetailsEnd")(reqId);
    }

    void error(const int id, const int errorCode, const std::string errorString)
    {
        PyLockGIL lockGil;
        this->get_override("error")(id, errorCode, errorString);
    }

    void updateMktDepth(TickerId id, int position, int operation, int side, double price, int size) 
    {
        this->get_override("updateMktDepth")(id, position, operation, side, price, side);
    }

    void updateMktDepthL2(TickerId id, int position, std::string marketMaker, int operation,
                          int side, double price, int size) 
    {
        this->get_override("updateMktDepthL2")(id, position, marketMaker, operation, side, price, side);
    }

    void updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch) 
    {
        this->get_override("updateNewsBulletin")(msgId, msgType, newsMessage, originExch);
    }

    void managedAccounts( const std::string& accountsList)
    {
        this->get_override("managedAccounts")(accountsList);
    }

    void receiveFA(faDataType pFaDataType, const std::string& cxml)
    {
        this->get_override("receiveFA")(pFaDataType, cxml);
    }

    void historicalData(TickerId reqId, const std::string& date, double open, double high,
                        double low, double close, int volume, int barCount, double WAP, int hasGaps)
    {
        this->get_override("historicalData")(reqId, date, open, high, low, close, volume, 
                                             barCount, WAP, hasGaps);
    }

    void scannerParameters(const std::string& xml) 
    {
        this->get_override("scannerParameters")(xml);
    }
   
    void scannerData(int reqId, int rank, const ContractDetails& contractDetails,
                     const std::string& distance, const std::string& benchmark, 
                     const std::string& projection, const std::string& legsStr)
    {
        this->get_override("scannerData")(reqId, rank, contractDetails, distance, benchmark, 
                                          projection, legsStr);
    }

    void scannerDataEnd(int reqId)
    {
        this->get_override("scannerDataEnd")(reqId);
    }

    void realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
                     long volume, double wap, int count) 
    {
        this->get_override("realtimeBar")(reqId, time, open, high, low, close, volume, wap, count);
    }

    void currentTime(long time)
    {
        this->get_override("currentTime")(time);
    }
    
    void fundamentalData(TickerId reqId, const std::string& data)
    {
        this->get_override("fundamentalData")(reqId, data);
    }
    
    void deltaNeutralValidation(int reqId, const UnderComp& underComp) 
    {
        this->get_override("deltaNeutralValidation")(reqId, underComp);
    }


    void tickSnapshotEnd( int reqId)
    {
        this->get_override("tickSnapshotEnd")(reqId);
    }

    void marketDataType( TickerId reqId, int marketDataType)
    {
        this->get_override("marketDataType")(reqId, marketDataType);
    }

    void commissionReport( const CommissionReport& commissionReport)
    {
        this->get_override("commissionReport")(commissionReport);
    }

    void position( const std::string& account, const Contract& contract, double position, 
                   double avgCost) 
    {
        this->get_override("position")(account, contract, position, avgCost);
    }

    void positionEnd()
    {
        this->get_override("positionEnd")();
    }

    void accountSummary( int reqId, const std::string& account, const std::string& tag, 
                         const std::string& value, const std::string& curency)
    {
        this->get_override("accountSummary")(reqId, account, tag, value, curency);
    }

    void accountSummaryEnd( int reqId)
    {
        this->get_override("accountSummaryEnd")(reqId);
    }

    void verifyMessageAPI( const std::string& apiData) 
    {
        this->get_override("verifyMessageAPI")(apiData);
    }

    void verifyCompleted( bool isSuccessful, const std::string& errorText) 
    {
        this->get_override("verifyCompleted")(isSuccessful, errorText);
    }

    void displayGroupList( int reqId, const std::string& groups) 
    {
        this->get_override("displayGroupList")(reqId, groups);
    }

    void displayGroupUpdated( int reqId, const std::string& contractInfo) 
    {
        this->get_override("displayGroupUpdated")(reqId, contractInfo);
    }

    void verifyAndAuthMessageAPI( const std::string& apiData, const std::string& xyzChallange) 
    {
        this->get_override("verifyAndAuthMessageAPI")(apiData, xyzChallange);
    }

    void verifyAndAuthCompleted( bool isSuccessful, const std::string& errorText) 
    {
        this->get_override("verifyAndAuthCompleted")(isSuccessful, errorText);
    }

    void connectAck() 
    {
        this->get_override("connectAck")();
    }

    void positionMulti( int reqId, const std::string& account,const std::string& modelCode, 
                        const Contract& contract, double pos, double avgCost) 
    {
        this->get_override("positionMulti")(reqId, account, modelCode, contract, pos, avgCost);
    }

    void positionMultiEnd( int reqId) 
    {
        this->get_override("positionMultiEnd")(reqId);
    }

    void accountUpdateMulti( int reqId, const std::string& account, const std::string& modelCode, 
                             const std::string& key, const std::string& value, 
                             const std::string& currency) 
    {
        this->get_override("accountUpdateMulti")(reqId, account, modelCode, key, value, currency);
    }

    void accountUpdateMultiEnd( int reqId) 
    {
        this->get_override("accountUpdateMultiEnd")(reqId);
    }
     
    void securityDefinitionOptionalParameter(int reqId, const std::string& exchange, 
                                             int underlyingConId, const std::string& tradingClass, 
                                             const std::string& multiplier, 
                                             std::set<std::string> expirations, 
                                             std::set<double> strikes) 
    {
        this->get_override("securityDefinitionOptionalParameter")(reqId, expirations, 
                                                                  underlyingConId, tradingClass, 
                                                                  multiplier, expirations, strikes);
    }

    void securityDefinitionOptionalParameterEnd(int reqId) 
    {
        this->get_override("securityDefinitionOptionalParameterEnd")(reqId);
    }
    
    void softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers) 
    {
        this->get_override("softDollarTiers")(reqId, tiers);
    }

    static void export_to_python();

};



#endif
