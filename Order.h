/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#pragma once
#ifndef order_def
#define order_def

#include "TagValue.h"
#include "OrderCondition.h"
#include "SoftDollarTier.h"

#include <float.h>
#include <limits.h>
#include <boost/python.hpp>

namespace bp = boost::python;

#define UNSET_DOUBLE DBL_MAX
#define UNSET_INTEGER INT_MAX

enum Origin { CUSTOMER,
              FIRM,
              UNKNOWN };

enum AuctionStrategy { AUCTION_UNSET = 0,
                       AUCTION_MATCH = 1,
                       AUCTION_IMPROVEMENT = 2,
                       AUCTION_TRANSPARENT = 3 };

struct OrderComboLeg
{
	OrderComboLeg()
	{
		price = UNSET_DOUBLE;
	}

	double price;

	bool operator==( const OrderComboLeg& other) const
	{
		return (price == other.price);
	}

    static void export_to_python()
    {
        bp::class_<OrderComboLeg>("OrderComboLeg", bp::init<>() )
            .def_readwrite("price", &OrderComboLeg::price )
            .def( bp::self == bp::self )
        ;
    }
};

typedef ibapi::shared_ptr<OrderComboLeg> OrderComboLegSPtr;

struct Order
{
	Order() :
		softDollarTier("", "", "")
	{
		// order identifier
		orderId  = 0;
		clientId = 0;
		permId   = 0;

		// main order fields
		totalQuantity = 0;
		lmtPrice      = UNSET_DOUBLE;
		auxPrice      = UNSET_DOUBLE;

		// extended order fields
		activeStartTime = "";
		activeStopTime = "";
		ocaType        = 0;
		transmit       = true;
		parentId       = 0;
		blockOrder     = false;
		sweepToFill    = false;
		displaySize    = 0;
		triggerMethod  = 0;
		outsideRth     = false;
		hidden         = false;
		allOrNone      = false;
		minQty         = UNSET_INTEGER;
		percentOffset  = UNSET_DOUBLE;
		overridePercentageConstraints = false;
		trailStopPrice = UNSET_DOUBLE;
		trailingPercent = UNSET_DOUBLE;

		// institutional (ie non-cleared) only
		openClose     = "O";
		origin        = CUSTOMER;
		shortSaleSlot = 0;
		exemptCode    = -1;

		// SMART routing only
		discretionaryAmt = 0;
		eTradeOnly       = true;
		firmQuoteOnly    = true;
		nbboPriceCap     = UNSET_DOUBLE;
		optOutSmartRouting = false;

		// BOX exchange orders only
		auctionStrategy = AUCTION_UNSET;
		startingPrice   = UNSET_DOUBLE;
		stockRefPrice   = UNSET_DOUBLE;
		delta           = UNSET_DOUBLE;

		// pegged to stock and VOL orders only
		stockRangeLower = UNSET_DOUBLE;
		stockRangeUpper = UNSET_DOUBLE;

        randomizePrice = false;
        randomizeSize = false;

		// VOLATILITY ORDERS ONLY
		volatility            = UNSET_DOUBLE;
		volatilityType        = UNSET_INTEGER;     // 1=daily, 2=annual
		deltaNeutralOrderType = "";
		deltaNeutralAuxPrice  = UNSET_DOUBLE;
		deltaNeutralConId     = 0;
		deltaNeutralSettlingFirm = "";
		deltaNeutralClearingAccount = "";
		deltaNeutralClearingIntent = "";
		deltaNeutralOpenClose = "";
		deltaNeutralShortSale = false;
		deltaNeutralShortSaleSlot = 0;
		deltaNeutralDesignatedLocation = "";
		continuousUpdate      = false;
		referencePriceType    = UNSET_INTEGER; // 1=Average, 2 = BidOrAsk

		// COMBO ORDERS ONLY
		basisPoints     = UNSET_DOUBLE;  // EFP orders only
		basisPointsType = UNSET_INTEGER; // EFP orders only

		// SCALE ORDERS ONLY
		scaleInitLevelSize  = UNSET_INTEGER;
		scaleSubsLevelSize  = UNSET_INTEGER;
		scalePriceIncrement = UNSET_DOUBLE;
		scalePriceAdjustValue = UNSET_DOUBLE;
		scalePriceAdjustInterval = UNSET_INTEGER;
		scaleProfitOffset = UNSET_DOUBLE;
		scaleAutoReset = false;
		scaleInitPosition = UNSET_INTEGER;
		scaleInitFillQty = UNSET_INTEGER;
		scaleRandomPercent = false;
		scaleTable = "";

		// What-if
		whatIf = false;

		// Not Held
		notHeld = false;
		solicited = false;

		triggerPrice = UNSET_DOUBLE;
		adjustedStopPrice = UNSET_DOUBLE;
		adjustedStopLimitPrice = UNSET_DOUBLE;
		adjustedTrailingAmount = UNSET_DOUBLE;
		lmtPriceOffset = UNSET_DOUBLE;
		extOperator = "";
	}

	// order identifier
	long     orderId;
	long     clientId;
	long     permId;

	// main order fields
	std::string action;
	double     totalQuantity;
	std::string orderType;
	double   lmtPrice;
	double   auxPrice;

	// extended order fields
	std::string tif;           // "Time in Force" - DAY, GTC, etc.
	std::string activeStartTime;	// for GTC orders
	std::string activeStopTime;	// for GTC orders
	std::string ocaGroup;      // one cancels all group name
	int      ocaType;       // 1 = CANCEL_WITH_BLOCK, 2 = REDUCE_WITH_BLOCK, 3 = REDUCE_NON_BLOCK
	std::string orderRef;      // order reference
	bool     transmit;      // if false, order will be created but not transmited
	long     parentId;      // Parent order Id, to associate Auto STP or TRAIL orders with the original order.
	bool     blockOrder;
	bool     sweepToFill;
	int      displaySize;
	int      triggerMethod; // 0=Default, 1=Double_Bid_Ask, 2=Last, 3=Double_Last, 4=Bid_Ask, 7=Last_or_Bid_Ask, 8=Mid-point
	bool     outsideRth;
	bool     hidden;
	std::string goodAfterTime;    // Format: 20060505 08:00:00 {time zone}
	std::string goodTillDate;     // Format: 20060505 08:00:00 {time zone}
	std::string rule80A; // Individual = 'I', Agency = 'A', AgentOtherMember = 'W', IndividualPTIA = 'J', AgencyPTIA = 'U', AgentOtherMemberPTIA = 'M', IndividualPT = 'K', AgencyPT = 'Y', AgentOtherMemberPT = 'N'
	bool     allOrNone;
	int      minQty;
	double   percentOffset; // REL orders only
	bool     overridePercentageConstraints;
	double   trailStopPrice; // TRAILLIMIT orders only
	double   trailingPercent;

	// financial advisors only
	std::string faGroup;
	std::string faProfile;
	std::string faMethod;
	std::string faPercentage;

	// institutional (ie non-cleared) only
	std::string openClose; // O=Open, C=Close
	Origin   origin;    // 0=Customer, 1=Firm
	int      shortSaleSlot; // 1 if you hold the shares, 2 if they will be delivered from elsewhere.  Only for Action="SSHORT
	std::string designatedLocation; // set when slot=2 only.
	int      exemptCode;

	// SMART routing only
	double   discretionaryAmt;
	bool     eTradeOnly;
	bool     firmQuoteOnly;
	double   nbboPriceCap;
	bool     optOutSmartRouting;

	// BOX exchange orders only
	int      auctionStrategy; // AUCTION_MATCH, AUCTION_IMPROVEMENT, AUCTION_TRANSPARENT
	double   startingPrice;
	double   stockRefPrice;
	double   delta;

	// pegged to stock and VOL orders only
	double   stockRangeLower;
	double   stockRangeUpper;

    bool randomizeSize;
    bool randomizePrice;

	// VOLATILITY ORDERS ONLY
	double   volatility;
	int      volatilityType;     // 1=daily, 2=annual
	std::string deltaNeutralOrderType;
	double   deltaNeutralAuxPrice;
	long     deltaNeutralConId;
	std::string deltaNeutralSettlingFirm;
	std::string deltaNeutralClearingAccount;
	std::string deltaNeutralClearingIntent;
	std::string deltaNeutralOpenClose;
	bool     deltaNeutralShortSale;
	int      deltaNeutralShortSaleSlot;
	std::string deltaNeutralDesignatedLocation;
	bool     continuousUpdate;
	int      referencePriceType; // 1=Average, 2 = BidOrAsk

	// COMBO ORDERS ONLY
	double   basisPoints;      // EFP orders only
	int      basisPointsType;  // EFP orders only

	// SCALE ORDERS ONLY
	int      scaleInitLevelSize;
	int      scaleSubsLevelSize;
	double   scalePriceIncrement;
	double   scalePriceAdjustValue;
	int      scalePriceAdjustInterval;
	double   scaleProfitOffset;
	bool     scaleAutoReset;
	int      scaleInitPosition;
	int      scaleInitFillQty;
	bool     scaleRandomPercent;
	std::string scaleTable;

	// HEDGE ORDERS
	std::string hedgeType;  // 'D' - delta, 'B' - beta, 'F' - FX, 'P' - pair
	std::string hedgeParam; // 'beta=X' value for beta hedge, 'ratio=Y' for pair hedge

	// Clearing info
	std::string account; // IB account
	std::string settlingFirm;
	std::string clearingAccount; // True beneficiary of the order
	std::string clearingIntent; // "" (Default), "IB", "Away", "PTA" (PostTrade)

	// ALGO ORDERS ONLY
	std::string algoStrategy;

	TagValueListSPtr algoParams;
	TagValueListSPtr smartComboRoutingParams;

	std::string algoId;

	// What-if
	bool     whatIf;

	// Not Held
	bool     notHeld;
	bool     solicited;

	// models
	std::string modelCode;

	// order combo legs
	typedef std::vector<OrderComboLegSPtr> OrderComboLegList;
	typedef ibapi::shared_ptr<OrderComboLegList> OrderComboLegListSPtr;

	OrderComboLegListSPtr orderComboLegs;

	TagValueListSPtr orderMiscOptions;

	//VER PEG2BENCH fields:
	int referenceContractId;
	double peggedChangeAmount;
	bool isPeggedChangeAmountDecrease;
	double referenceChangeAmount;
	std::string referenceExchangeId;
	std::string adjustedOrderType;
	double triggerPrice;
	double adjustedStopPrice;
	double adjustedStopLimitPrice;
	double adjustedTrailingAmount;
	int adjustableTrailingUnit;
	double lmtPriceOffset;

	std::vector<ibapi::shared_ptr<OrderCondition>> conditions;
	bool conditionsCancelOrder;
	bool conditionsIgnoreRth;

	// ext operator
	std::string extOperator;

	SoftDollarTier softDollarTier;

public:

	// Helpers
	static void CloneOrderComboLegs(OrderComboLegListSPtr& dst, const OrderComboLegListSPtr& src);

    static void export_to_python()
    {
        bp::class_<Order>("Order", bp::init<>())
            .def_readwrite("orderId", &Order::orderId ) // order identifier
            .def_readwrite("clientId", &Order::clientId )
            .def_readwrite("permId", &Order::permId )

            .def_readwrite("action", &Order::action ) // main order fields
            .def_readwrite("totalQuantity", &Order::totalQuantity )
            .def_readwrite("orderType", &Order::orderType )
            .def_readwrite("lmtPrice", &Order::lmtPrice )
            .def_readwrite("auxPrice", &Order::auxPrice )

            .def_readwrite("tif", &Order::tif )
            .def_readwrite("activeStartTime", &Order::activeStartTime )
            .def_readwrite("activeStopTime", &Order::activeStopTime )
            .def_readwrite("ocaGroup", &Order::ocaGroup )
            .def_readwrite("ocaType", &Order::ocaType )
            .def_readwrite("orderRef", &Order::orderRef )
            .def_readwrite("transmit", &Order::transmit )
            .def_readwrite("parentId", &Order::parentId )
            .def_readwrite("blockOrder", &Order::blockOrder )
            .def_readwrite("sweepToFill", &Order::sweepToFill )
            .def_readwrite("displaySize", &Order::displaySize )
            .def_readwrite("triggerMethod", &Order::triggerMethod )
            .def_readwrite("outsideRth", &Order::outsideRth )
            .def_readwrite("hidden", &Order::hidden )
            .def_readwrite("goodAfterTime", &Order::goodAfterTime )
            .def_readwrite("goodTillDate", &Order::goodTillDate )
            .def_readwrite("rule80A", &Order::rule80A )
            .def_readwrite("allOrNone", &Order::allOrNone )
            .def_readwrite("minQty", &Order::minQty )
            .def_readwrite("percentOffset", &Order::percentOffset )
            .def_readwrite("overridePercentageConstraints", &Order::overridePercentageConstraints )
            .def_readwrite("trailStopPrice", &Order::trailStopPrice )
            .def_readwrite("trailingPercent", &Order::trailingPercent )

            .def_readwrite("faGroup", &Order::faGroup ) // financial advisors only
            .def_readwrite("faProfile", &Order::faProfile )
            .def_readwrite("faMethod", &Order::faMethod )
            .def_readwrite("faPercentage", &Order::faPercentage )

            .def_readwrite("openClose", &Order::openClose ) // institutional only
            .def_readwrite("origin", &Order::origin )
            .def_readwrite("shortSaleSlot", &Order::shortSaleSlot )
            .def_readwrite("designatedLocation", &Order::designatedLocation )
            .def_readwrite("exemptCode", &Order::exemptCode )

            .def_readwrite("discretionaryAmt", &Order::discretionaryAmt ) //SMART routing only
            .def_readwrite("eTradeOnly", &Order::eTradeOnly )
            .def_readwrite("firmQuoteOnly", &Order::firmQuoteOnly )
            .def_readwrite("nbboPriceCap", &Order::nbboPriceCap )
            .def_readwrite("optOutSmartRouting", &Order::optOutSmartRouting )

            .def_readwrite("auctionStrategy", &Order::auctionStrategy ) // BOX exchange orders only
            .def_readwrite("startingPrice", &Order::startingPrice )
            .def_readwrite("stockRefPrice", &Order::stockRefPrice )
            .def_readwrite("delta", &Order::delta )


            .def_readwrite("stockRangeLower", &Order::stockRangeLower ) // pegged to stock and VOL orders only
            .def_readwrite("stockRangeUpper", &Order::stockRangeUpper )
            .def_readwrite("randomizeSize", &Order::randomizeSize )
            .def_readwrite("randomizePrice", &Order::randomizePrice )

            .def_readwrite("volatility", &Order::volatility ) // VOLATITLIY ORDERS ONLY 
            .def_readwrite("volatilityType", &Order::volatilityType )
            .def_readwrite("deltaNeutralOrderType", &Order::deltaNeutralOrderType )
            .def_readwrite("deltaNeutralAuxPrice", &Order::deltaNeutralAuxPrice )
            .def_readwrite("deltaNeutralConId", &Order::deltaNeutralConId )
            .def_readwrite("deltaNeutralSettlingFirm", &Order::deltaNeutralSettlingFirm )
            .def_readwrite("deltaNeutralClearingAccount", &Order::deltaNeutralClearingAccount )
            .def_readwrite("deltaNeutralClearingIntent", &Order::deltaNeutralClearingIntent )
            .def_readwrite("deltaNeutralOpenClose", &Order::deltaNeutralOpenClose )
            .def_readwrite("deltaNeutralShortSale", &Order::deltaNeutralShortSale )
            .def_readwrite("deltaNeutralShortSaleSlot", &Order::deltaNeutralShortSaleSlot )
            .def_readwrite("deltaNeutralDesignatedLocation", &Order::deltaNeutralDesignatedLocation )
            .def_readwrite("continuousUpdate", &Order::continuousUpdate )
            .def_readwrite("referencePriceType", &Order::referencePriceType )

            .def_readwrite("basisPoints", &Order::basisPoints ) // COMBO ORDERS ONLY 
            .def_readwrite("basisPointsType", &Order::basisPointsType )

            .def_readwrite("scaleInitLevelSize", &Order::scaleInitLevelSize ) //SCALE ORDERS ONLY
            .def_readwrite("scaleSubsLevelSize", &Order::scaleSubsLevelSize )
            .def_readwrite("scalePriceIncrement", &Order::scalePriceIncrement )
            .def_readwrite("scalePriceAdjustValue", &Order::scalePriceAdjustValue )
            .def_readwrite("scalePriceAdjustInterval", &Order::scalePriceAdjustInterval )
            .def_readwrite("scaleProfitOffset", &Order::scaleProfitOffset )
            .def_readwrite("scaleAutoReset", &Order::scaleAutoReset )
            .def_readwrite("scaleInitPosition", &Order::scaleInitPosition )
            .def_readwrite("scaleInitFillQty", &Order::scaleInitFillQty )
            .def_readwrite("scaleRandomPercent", &Order::scaleRandomPercent )
            .def_readwrite("scaleTable", &Order::scaleTable )

            .def_readwrite("hedgeType", &Order::hedgeType ) // HEDGE ORDERS
            .def_readwrite("hedgeParam", &Order::hedgeParam )

            .def_readwrite("account", &Order::account ) // Clearing info
            .def_readwrite("settlingFirm", &Order::settlingFirm )
            .def_readwrite("clearingAccount", &Order::clearingAccount )
            .def_readwrite("clearingIntent", &Order::clearingIntent )

            .def_readwrite("algoStrategy", &Order::algoStrategy ) // ALGO ORDERS ONLY
            .def_readwrite("algoParams", &Order::algoParams )
            .def_readwrite("smartComboRoutingParams", &Order::smartComboRoutingParams )
            .def_readwrite("algoId", &Order::algoId )

            .def_readwrite("whatIf", &Order::whatIf )
            .def_readwrite("notHeld", &Order::notHeld )
            .def_readwrite("solicited", &Order::solicited )
            .def_readwrite("modelCode", &Order::modelCode )

            .def_readwrite("orderComboLegs", &Order::orderComboLegs )
            .def_readwrite("orderMiscOptions", &Order::orderMiscOptions )

            .def_readwrite("referenceContractId", &Order::referenceContractId ) // VER PEG2BENCH fields
            .def_readwrite("peggedChangeAmount", &Order::peggedChangeAmount )
            .def_readwrite("isPeggedChangeAmountDecrease", &Order::isPeggedChangeAmountDecrease )
            .def_readwrite("referenceChangeAmount", &Order::referenceChangeAmount )
            .def_readwrite("referenceExchangeId", &Order::referenceExchangeId )
            .def_readwrite("adjustedOrderType", &Order::adjustedOrderType )
            .def_readwrite("triggerPrice", &Order::triggerPrice )
            .def_readwrite("adjustedStopPrice", &Order::adjustedStopPrice )
            .def_readwrite("adjustedStopLimitPrice", &Order::adjustedStopLimitPrice )
            .def_readwrite("adjustedTrailingAmount", &Order::adjustedTrailingAmount )
            .def_readwrite("adjustableTrailingUnit", &Order::adjustableTrailingUnit )
            .def_readwrite("lmtPriceOffset", &Order::lmtPriceOffset )

            .def_readwrite("conditions", &Order::conditions )
            .def_readwrite("conditionsCancelOrder", &Order::conditionsCancelOrder )
            .def_readwrite("conditionsIgnoreRth", &Order::conditionsIgnoreRth )
            .def_readwrite("extOperator", &Order::extOperator )
            .def_readwrite("softDollarTier", &Order::softDollarTier )

            .def("CloneOrderComboLegs", &Order::CloneOrderComboLegs)
            .staticmethod("CloneOrderComboLegs")
        ;

    }
};

inline void
Order::CloneOrderComboLegs(OrderComboLegListSPtr& dst, const OrderComboLegListSPtr& src)
{
	if (!src.get())
		return;

	dst->reserve(src->size());

	OrderComboLegList::const_iterator iter = src->begin();
	const OrderComboLegList::const_iterator iterEnd = src->end();

	for (; iter != iterEnd; ++iter) {
		const OrderComboLeg* leg = iter->get();
		if (!leg)
			continue;
		dst->push_back(OrderComboLegSPtr(new OrderComboLeg(*leg)));
	}
}

#endif
