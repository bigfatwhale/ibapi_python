
from ibapi import OrderComboLeg

def main():
    o1 = OrderComboLeg()
    o2 = OrderComboLeg()
    o1.price = 10
    o2.price = 10

    assert( o1 == o2 )

main()
