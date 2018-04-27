
from   example_client import TestClient
from   optparse import OptionParser
import logging
import thread

logging.basicConfig()
logger = logging.getLogger('ibclient')
logger.setLevel(logging.DEBUG)


def main():

    clientId = 168
    host = '127.0.0.1'

    parser = OptionParser()
    parser.add_option("-g", "--gateway", dest="gateway", default='ibgw',
                      help=" gateway to use. either 'tws' or 'ibgw'")
    options, args = parser.parse_args()

    logger.info( 'connecting to gateway = %s', options.gateway )

    if options.gateway == 'tws':
        port = 7497 
    elif options.gateway =='ibgw':
        port = 4002

    logger.info('main python thread is running on thread id = %s', thread.get_ident() )

    client = TestClient()

    logger.info('TestClient state = %s', client.state )
    res = client.connect( host, port, clientId )   
    
    try:
        while True:
            client.processMessages()
    except KeyboardInterrupt:
        logger.info("Ctrl-C pressed, disconnecting...")
        client.disconnect()

if __name__ == "__main__":
    main()

