
from   datetime import datetime
from   ibapi import TickType
import pandas as pd

class MarketDataManager( object ):

    def __init__( self ):
        self.subscriptions_by_id = {}

    def add_subscription( self, id, subscription ):
        self.subscriptions_by_id[id] = subscription

    def get_subscription( self, id ):
        return self.subscriptions_by_id[id]

class Subscription( object ):
    
    def __init__( self, id, contract ):
        self.id = id
        self.contract = contract
        self.data = {}
        self.trades = {'time':[], 'price':[], 'size':[] }

    def update( self, field, value ):
        if field == TickType.LAST:
            self.trades['price'].append( value )
            self.trades['size'].append( 0 )
        elif field == TickType.LAST_SIZE:
            self.trades['size'][-1] +=  value 
        elif field == TickType.LAST_TIMESTAMP:
            self.trades['time'].append( datetime.fromtimestamp(float(value)) )
        else:
            self.data[str(field)] = value

    def __repr__( self ):
        s = 'no data'

        if self.data:
            cols, data = zip(*sorted(self.data.items()))
            quotes = pd.DataFrame([data], columns=cols)
            lens = map( len, self.trades.values() ) 
            s = 'MktDataUpdate\n==== %s quote ====\n%s\n' % \
                ( self.contract.symbol, quotes.to_string(index=False) )

        if ( len( self.trades['time']) == len( self.trades['price'] ) ) and len( self.trades['price'] ) > 0:
            trades = pd.DataFrame(self.trades)   
            s += '\n\n==== %s trades ====\n%s\n\n' % ( self.contract.symbol, trades.to_string(index=False))

        return s
