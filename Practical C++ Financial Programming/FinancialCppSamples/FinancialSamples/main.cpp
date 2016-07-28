//
//  main.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

//#include "IntRateCalculator.h"
//#include "CompoundIntRateCalculator.h"
#include "CashFlowCalculator.h"
#include "BondCalculator.h"
#include "MACalculator.h"
#include "CorrelationCalculator.h"
#include "TimeSeries.h"
#include "VolatilityCalculator.h"
#include "FundamentalsCalc.h"
#include "FinancialStatement.h"
#include "InvestmentInstrument.h"
#include "IntRateEngine.h"
#include "TransactionHandler.h"
#include "FactorialTemplate.h"

#include <iostream>
#include <cstdlib>
#include <memory>

// the main function receives parameters passed to the program
int amain(int argc, const char * argv[])
{
    if (argc != 4)
    {
        std::cout << "usage: progName <interest rate> <value> <num periods>" << std::endl;
        return 1;
    }

    double rate = atof(argv[1]);
    //double value = atof(argv[2]);
    //int num_periods = atoi(argv[3]);

    //CompoundIntRateCalculator cIRCalc(rate);
    //double res = cIRCalc.multiplePeriod(value, num_periods);

    CashFlowCalculator cfc(rate);
    do {
        int period;
        std::cin >> period;
        if (period == -1) {
            break;
        }
        double value;
        std::cin >> value;
        cfc.addCashPayment(value, period);

    } while (1);

    double result = cfc.presentValue();
    std::cout << " The present value is " << result << std::endl;


    //IntRateCalculator irCalculator(rate);
    //double res = irCalculator.singlePeriod(value);
    //double contRes = cIRCalc.continuousCompounding(value, num_periods);
    //std::cout << " future value for multiple period compounding is " << res << std::endl;
    //std::cout << " future value for continuous compounding is " << contRes << std::endl;

    return 0;
}

// the main function receives parameters passed to the program
int main_A(int argc, const char * argv[])
{
    if (argc != 5)
    {
        std::cout << "usage: progName <institution> <principal> <coupon> <num periods>" << std::endl;
        return 1;
    }

    std::string issuer = argv[1];
    double principal = atof(argv[2]);
    double coupon = atof(argv[3]);
    int num_periods = atoi(argv[4]);

    BondCalculator bc(issuer, num_periods, principal, coupon);
    std::cout << "reading information for bond issued by " << issuer << std::endl;
    std::cout << " the internal rate of return is " << bc.interestRate() << std::endl;
    return 0;
}

// the main function receives parameters passed to the program
int main_B(int argc, const char * argv[])
{
    if (argc != 2)
    {
        std::cout << "usage: progName <num periods>" << std::endl;
        return 1;
    }

    int num_periods = atoi(argv[1]);


    double price;
    MACalculator calculator(num_periods);
    for (;;) {
        std::cin >> price;
        if (price == -1)
            break;
        calculator.addPriceQuote(price);
    }

    std::vector<double> ma = calculator.calculateMA();

    for (unsigned i=0; i<ma.size(); ++i)
    {
        std::cout << "average value " << i << " = " << ma[i] << std::endl;
    }

    std::vector<double> ema = calculator.calculateEMA();
    for (unsigned i=0; i<ma.size(); ++i)
    {
        std::cout << "exponential average value " << i << " = " << ema[i] << std::endl;
    }

    return 0;
}


// the main function receives parameters passed to the program
int main_C(int argc, const char * argv[])
{
    double price;

    TimeSeries tsa;
    TimeSeries tsb;
    for (;;) {
        std::cin >> price;
        if (price == -1)
        {
            break;
        }
        tsa.addValue(price);
        std::cin >> price;
        tsb.addValue(price);
    }

    CorrelationCalculator cCalc(tsa, tsb);

    auto correlation = cCalc.correlation();

    std::cout << "correlation is " <<  correlation  << std::endl;
    return 0;
}

// the main function receives parameters passed to the program
int main_D(int argc, const char * argv[])
{
    double price;

    VolatilityCalculator vc;
    for (;;) {
        std::cin >> price;
        if (price == -1)
        {
            break;
        }
        vc.addPrice(price);
    }


    std::cout << "range volatility is " <<  vc.rangeVolatility()  << std::endl;
    std::cout << "average daily range is " <<  vc.avgDailyRange()  << std::endl;
    std::cout << "standard deviation is " <<  vc.stdDev()  << std::endl;
    return 0;
}

// the main function receives parameters passed to the program
int maind(int argc, const char * argv[])
{
    FundamentalsCalculator fc("AAPL", 543.99, 12.20);

    // values are in millions
    fc.setAssets(243139);
    fc.setBookValue(165234);
    fc.setEarnings(35885);
    fc.setEpsGrowth(0.22);
    fc.setExpectedEarnings(39435);
    fc.setLiabilitiesAndIntangibles(124642);
    fc.setNetIncome(37235);
    fc.setNumOfShares(891990);
    fc.setShareHoldersEquity(123549);

    std::cout << "P/E: " <<  fc.PE()/1000  << std::endl;  // prices in thousands
    std::cout << "forward P/E: " <<  fc.forwardPE()/1000  << std::endl;
    std::cout << "book value: " <<  fc.bookValue()  << std::endl;
    std::cout << "price to book: " <<  fc.priceToBookRatio()  << std::endl;
    std::cout << "price earnings to growth: " <<  fc.priceEarningsToGrowth()  << std::endl;
    std::cout << "return on equity: " <<  fc.returnOnEquity()  << std::endl;
    std::cout << "dividend: " <<  fc.getDividend()  << std::endl;
    return 0;
}

int main_E()
{
    std::auto_ptr<FinancialStatement> fs = getSampleStatement();
    transferFinancialStatement(fs);

    // the auto_ptr object is invalid here, the next access can crash the program
    std::cout << fs->getReturn() << "\n";

    return 0;
}

int main_F()
{
    IntRateEngine<BondInstrument> engineA;
    IntRateEngine<MortgageInstrument> engineB;

    BondInstrument bond(40000, 250);
    MortgageInstrument mortgage(250, 50000, 5000);
    engineA.setInstrument(bond);
    engineB.setInstrument(mortgage);


    std::cout << " bond annual int rate: " << engineA.getAnnualIntRate()*100 << "%" << std::endl;
    std::cout << " mortgage annual int rate: " << engineB.getAnnualIntRate()*100 << "%" << std::endl;

    return 0;
}

int main_G(int argc, const char **argv)
{
    std::cout << Factorial<6>::result;
    std::cout << "\n" << ChoiceNumber<6,2>::result;
    exit(0);
    if (argc < 2)
    {
        std::cerr << "usage: <progName> <fileName>\n";
        return 1;
    }

    std::string fileName = argv[1];
    try
    {
        TransactionHandler handler(fileName);
    }
    catch (FileError &e)
    {
        std::cerr << "received a file error: " << e.what() << std::endl;
    }
    catch (TransactionTypeError &e)
    {
        std::cerr << "received a transaction error: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "received an unknown error\n";
    }
    return 0;
}
