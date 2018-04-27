
class Account( object ):

    def __init__( self, acctCode ):
        self.acctCode = acctCode
        self.entries = dict()
