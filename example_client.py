
from   account import Account
from   ibapi import PyClientBaseWrap, State, Contract, Order, TickType
import logging
from   mktdata import MarketDataManager, Subscription
import thread
import time

logging.basicConfig()
logger = logging.getLogger('ibclient')
logger.setLevel(logging.DEBUG)

def makeContract( fields ):
    c = Contract()
    for k, v in fields.items():
        setattr( c, k, v )
    return c

class ContractSamples( object ):

    USStock = makeContract( dict(symbol="AAPL", secType="STK", currency="USD", exchange="ISLAND"))        

def makeOrder( fields ):
    o = Order()
    for k, v in fields.items():
        setattr( o, k, v )
    return o

class TestClient( PyClientBaseWrap ):
    """ Test client for python wrapper around the IB C++ api.""" 

    def __init__( self ):
        PyClientBaseWrap.__init__(self) # note that super( PyClientBase, self).__init__() won't work
        self.account = None
        self.mktDataMgr = MarketDataManager()
        self.orderSent = False

    def error(self, id, errorCode, errorString):
        logger.info( "TestClient : id=%s, errorCode=%s, errorString=%s", id, errorCode, errorString)

    def connectAck( self ):
        pass

    def nextValidId( self, orderId ):
        logger.info( 'nextValid called with orderId = %s', orderId )

        curState = self.state
        if not self.orderSent:
            newState = State.ST_ORDEROPERATIONS
            self.orderSent = True
        else:
            newState = State.TICKDATAOPERATION
        self.state = newState
        logger.info( 'Transition state from %s to %s', curState, newState )

        curOrderId = self.orderId
        newOrderId = orderId
        self.orderId = newOrderId
        logger.info( 'Transition orderId from %s to %s', curOrderId, newOrderId )

    def managedAccounts( self, accountsList ):
        self.account = Account( accountsList )

    # --------------- api request methods --------------
    def accountOperations( self ):
        logger.info( 'TestClient.accountOperations : request account update for %s', self.account )
        self.reqAccountUpdates( True, self.account.acctCode )
        self.state = State.ST_ACCOUNTOPERATIONS_ACK

    def tickDataOperation( self ):
        logger.info( 'Requesting market data for AAPL' )
        self.reqMktData( 9001, ContractSamples.USStock, '', False )
        subscription = Subscription( 9001, ContractSamples.USStock )
        self.mktDataMgr.add_subscription( subscription.id, subscription )
        self.state = State.ST_TICKDATAOPERATION_ACK

    def marketDepthOperations( self ):
        logger.info( 'Requesting market depth for AAPL' )
        self.reqMktDepth( 7001, ContractSamples.USStock, 5 ) # 5 rows
        self.state = State.ST_MARKETDEPTHOPERATION_ACK

    def reutersFundamentals( self ):
        logger.info( 'Requesting fundamentals data for AAPL' )
        self.reqFundamentalData( 8001, ContractSamples.USStock, 'ReportRatios' )
        self.state = State.ST_REUTERSFUNDAMENTALS_ACK

    def realTimeBars( self ):
        logger.info( 'Requesting realtime bars for AAPL' )
        self.reqRealTimeBars(3001, ContractSamples.USStock, 5, "MIDPOINT", True );
        self.state = State.ST_REALTIMEBARS_ACK
        
    def orderOperations( self ):
        logger.info( 'Sending order for 100 qty of AAPL' )
        curOrderId = self.orderId
        self.orderId += 1
        order = makeOrder( dict(action='BUY', orderType='MKT', totalQuantity=100) )
        self.placeOrder( curOrderId, ContractSamples.USStock, order ) 
        self.state = State.ST_ORDEROPERATIONS_ACK

    # -------------- call back functions --------------
    def updateAccountValue( self, key, val, currency, accountName):
        #logger.info( 'update account value : key = %s, val = %s, ccy = %s', key, val, currency )
        self.account.entries[key] = val

    def updateAccountTime( self, timestamp ):
        logger.info( '===== Acct Info as of : %s ======', timestamp )
        logger.info( 'Available funds = %s', self.account.entries['AvailableFunds'] )
        logger.info( 'TotalCashValue = %s', self.account.entries['TotalCashValue'] )

    def accountDownloadEnd( self, accountName ):
        logger.info( '===== Acct Info Download End ======' )
        logger.info('callback thread is running on thread id = %s', thread.get_ident() )


    def _tickDataUpdate( self, tickerId, key, value ):
        subscription = self.mktDataMgr.get_subscription( tickerId )
        subscription.update( key, value )
        return subscription

    def tickString( self, tickerId, tickTypeInt, value ):
        #logger.info('tickString called with tickerId=%s, tickTypeInt=%s, value=%s', tickerId, tickTypeInt, value )
        subscription = self._tickDataUpdate( tickerId, tickTypeInt, value )
        logger.info( subscription )

    def tickPrice( self, tickerId, field, price, canAutoExecute):
        #logger.info('tickPrice called with tickerId=%s, field=%s, price=%s, canAutoExecute=%s', 
        #            tickerId, field, price, canAutoExecute )
        subscription = self._tickDataUpdate( tickerId, field, price )
        logger.info( subscription )

    def tickSize( self, tickerId, field, size):
        #logger.info('tickSize called with tickerId=%s, field=%s, size=%s', tickerId, field, size )
        subscription = self._tickDataUpdate( tickerId, field, size )
        logger.info( subscription )

    def tickGeneric( self, tickerId, tickType, value ):
        logger.info('tickGeneric called with tickerId=%s, tickType=%s, value=%s', tickerId, tickType, value )

    def fundamentalData( self, reqId, data ):
        logger.info('fundamental data for AAPL,\n%s', data )

    def updateMktDepth( self, id, position, operation, side, price, ):
        logger.info('updateMktDepth:id=%s, position=%s, operation=%s, size=%s, price=%s, size=%s', 
                    id, position, operation, side, price, side)
    

    def updateMktDepthL2( self, id, position, marketMaker, operation, side, price, size) :
        logger.info('updateMktDepth:id=%s, position=%s, marketMaker=%s, operation=%s, size=%s, price=%s, size=%s',
                    id, position, marketMaker, operation, side, price, side)
    
    def realtimeBar( self, reqId, time, open, high, low, close, volume, wap, count):
        logger.info('realtimeBar:reqId=%s, time=%s, open=%s, high=%s, low=%s, close=%s, volume=%s, wap=%s, count=%s',
                    reqId, time, open, high, low, close, volume, wap, count)

    def orderStatus( self, orderId, status, filled, remaining, avgFillPrice, permId, parentId, lastFillPrice, 
                     clientId, whyHeld ): 
        logger.info('orderStatus:orderId=%s, status=%s, filled=%s, remaining=%s, avgFillPrice=%s, permId=%s, parentId=%s, lastFillPrice=%s, clientId=%s, whyHeld=%s', orderId, status, filled, remaining, avgFillPrice, permId, parentId, lastFillPrice, clientId, whyHeld )

    def openOrder( self, orderId, contract, order, orderstate ):
        logger.info( 'openOrder : orderId=%s, contract=%s, order=%s, orderstate=%s', 
                     orderId, contract, order, orderstate )

    def openOrderEnd( self ):
        logger.info( 'openOrderEnd' )

    def execDetails( self, reqId, contract, execution):
        logger.info( 'execDetails : reqId=%s, contract=%s, execution=%s', reqId, contract, execution )

    def commissionReport( self, commissionReport):
        logger.info( 'commissionReport=%s', commissionReport )
