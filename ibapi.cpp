
#include "StdAfx.h"
#include "CommissionReport.h"
#include "EMessage.h"
#include "PercentChangeCondition.h"
#include "TagValue.h"
#include "VolumeCondition.h"
#include "TimeCondition.h"
#include "SoftDollarTier.h"
#include "TwsSocketClientErrors.h"
#include "ScannerSubscription.h"
#include "PriceCondition.h"
#include "OrderState.h"
#include "OrderCondition.h"
#include "Order.h"
#include "MarginCondition.h"
#include "executioncondition.h"
#include "Execution.h"
#include "PyClient.h"
#include "Contract.h"
#include "EWrapper.h"
#include <boost/python.hpp>

namespace bp = boost::python;

void TickType_export_to_python()
{
    bp::enum_<TickType>("TickType")
        .value("BID_SIZE",BID_SIZE)
        .value("BID",BID)
        .value("ASK",ASK)
        .value("ASK_SIZE",ASK_SIZE)
        .value("LAST",LAST)
        .value("LAST_SIZE",LAST_SIZE)
        .value("HIGH",HIGH)
        .value("LOW",LOW)
        .value("VOLUME",VOLUME)
        .value("CLOSE",CLOSE)
        .value("BID_OPTION_COMPUTATION",BID_OPTION_COMPUTATION)
        .value("ASK_OPTION_COMPUTATION",ASK_OPTION_COMPUTATION)
        .value("LAST_OPTION_COMPUTATION",LAST_OPTION_COMPUTATION)
        .value("MODEL_OPTION",MODEL_OPTION)
        .value("OPEN",OPEN)
        .value("LOW_13_WEEK",LOW_13_WEEK)
        .value("HIGH_13_WEEK",HIGH_13_WEEK)
        .value("LOW_26_WEEK",LOW_26_WEEK)
        .value("HIGH_26_WEEK",HIGH_26_WEEK)
        .value("LOW_52_WEEK",LOW_52_WEEK)
        .value("HIGH_52_WEEK",HIGH_52_WEEK)
        .value("AVG_VOLUME",AVG_VOLUME)
        .value("OPEN_INTEREST",OPEN_INTEREST)
        .value("OPTION_HISTORICAL_VOL",OPTION_HISTORICAL_VOL)
        .value("OPTION_IMPLIED_VOL",OPTION_IMPLIED_VOL)
        .value("OPTION_BID_EXCH",OPTION_BID_EXCH)
        .value("OPTION_ASK_EXCH",OPTION_ASK_EXCH)
        .value("OPTION_CALL_OPEN_INTEREST",OPTION_CALL_OPEN_INTEREST)
        .value("OPTION_PUT_OPEN_INTEREST",OPTION_PUT_OPEN_INTEREST)
        .value("OPTION_CALL_VOLUME",OPTION_CALL_VOLUME)
        .value("OPTION_PUT_VOLUME",OPTION_PUT_VOLUME)
        .value("INDEX_FUTURE_PREMIUM",INDEX_FUTURE_PREMIUM)
        .value("BID_EXCH",BID_EXCH)
        .value("ASK_EXCH",ASK_EXCH)
        .value("AUCTION_VOLUME",AUCTION_VOLUME)
        .value("AUCTION_PRICE",AUCTION_PRICE)
        .value("AUCTION_IMBALANCE",AUCTION_IMBALANCE)
        .value("MARK_PRICE",MARK_PRICE)
        .value("BID_EFP_COMPUTATION",BID_EFP_COMPUTATION)
        .value("ASK_EFP_COMPUTATION",ASK_EFP_COMPUTATION)
        .value("LAST_EFP_COMPUTATION",LAST_EFP_COMPUTATION)
        .value("OPEN_EFP_COMPUTATION",OPEN_EFP_COMPUTATION)
        .value("HIGH_EFP_COMPUTATION",HIGH_EFP_COMPUTATION)
        .value("LOW_EFP_COMPUTATION",LOW_EFP_COMPUTATION)
        .value("CLOSE_EFP_COMPUTATION",CLOSE_EFP_COMPUTATION)
        .value("LAST_TIMESTAMP",LAST_TIMESTAMP)
        .value("SHORTABLE",SHORTABLE)
        .value("FUNDAMENTAL_RATIOS",FUNDAMENTAL_RATIOS)
        .value("RT_VOLUME",RT_VOLUME)
        .value("HALTED",HALTED)
        .value("BID_YIELD",BID_YIELD)
        .value("ASK_YIELD",ASK_YIELD)
        .value("LAST_YIELD",LAST_YIELD)
        .value("CUST_OPTION_COMPUTATION",CUST_OPTION_COMPUTATION)
        .value("TRADE_COUNT",TRADE_COUNT)
        .value("TRADE_RATE",TRADE_RATE)
        .value("VOLUME_RATE",VOLUME_RATE)
        .value("LAST_RTH_TRADE",LAST_RTH_TRADE)
        .value("RT_HISTORICAL_VOL",RT_HISTORICAL_VOL)
        .value("IB_DIVIDENDS",IB_DIVIDENDS)
        .value("BOND_FACTOR_MULTIPLIER",BOND_FACTOR_MULTIPLIER)
        .value("REGULATORY_IMBALANCE",REGULATORY_IMBALANCE)
        .value("NEWS_TICK",NEWS_TICK)
        .value("SHORT_TERM_VOLUME_3_MIN",SHORT_TERM_VOLUME_3_MIN)
        .value("SHORT_TERM_VOLUME_5_MIN",SHORT_TERM_VOLUME_5_MIN)
        .value("SHORT_TERM_VOLUME_10_MIN",SHORT_TERM_VOLUME_10_MIN)
        .value("DELAYED_BID",DELAYED_BID)
        .value("DELAYED_ASK",DELAYED_ASK)
        .value("DELAYED_LAST",DELAYED_LAST)
        .value("DELAYED_BID_SIZE",DELAYED_BID_SIZE)
        .value("DELAYED_ASK_SIZE",DELAYED_ASK_SIZE)
        .value("DELAYED_LAST_SIZE",DELAYED_LAST_SIZE)
        .value("DELAYED_HIGH",DELAYED_HIGH)
        .value("DELAYED_LOW",DELAYED_LOW)
        .value("DELAYED_VOLUME",DELAYED_VOLUME)
        .value("DELAYED_CLOSE",DELAYED_CLOSE)
        .value("DELAYED_OPEN",DELAYED_OPEN)
        .value("RT_TRD_VOLUME",RT_TRD_VOLUME)
        .value("CREDITMAN_MARK_PRICE",CREDITMAN_MARK_PRICE)
        .value("CREDITMAN_SLOW_MARK_PRICE",CREDITMAN_SLOW_MARK_PRICE)
        .value("NOT_SET",NOT_SET)
    ;

}

BOOST_PYTHON_MODULE(ibapi)
{
    PyEval_InitThreads(); // need this to mess around with GIL

    TagValue::export_to_python();
    EMessage::export_to_python();
    CommissionReport::export_to_python();
    PercentChangeCondition::export_to_python();
    VolumeCondition::export_to_python();
    TimeCondition::export_to_python();
    SoftDollarTier::export_to_python();
    CodeMsgPair::export_to_python();
    ScannerSubscription::export_to_python();
    PriceCondition::export_to_python();
    OrderState::export_to_python();
    OrderCondition::export_to_python();
    OrderComboLeg::export_to_python();
    Order::export_to_python();
    MarginCondition::export_to_python();
    ExecutionCondition::export_to_python();
    Execution::export_to_python();
    ExecutionFilter::export_to_python();
    PyClientBaseWrap::export_to_python();
    Contract::export_to_python();
    TickType_export_to_python();
}
