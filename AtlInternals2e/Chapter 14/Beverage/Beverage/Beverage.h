// Beverage.h : Defines the ATL Server request handler class
//
#pragma once

class CBeverageHandler
    : public CRequestHandlerT<CBeverageHandler>
{

public:
    BEGIN_REPLACEMENT_METHOD_MAP(CBeverageHandler)
    REPLACEMENT_METHOD_ENTRY("InputValid", OnInputValid)
    REPLACEMENT_METHOD_ENTRY("MoreDrinks", OnMoreDrinks)
    REPLACEMENT_METHOD_ENTRY("DrinkNumber", OnDrinkNumber)
    REPLACEMENT_METHOD_ENTRY("Beverage", OnBeverage)
    REPLACEMENT_METHOD_ENTRY("NextDrink", OnNextDrink)
    END_REPLACEMENT_METHOD_MAP()

    HTTP_CODE ValidateAndExchange()
    {
        m_numDrinks = 0;
        m_HttpRequest.GetQueryParams().Exchange("numdrinks", &m_numDrinks);
        m_beverage = m_HttpRequest.GetQueryParams().Lookup("beverage");
        m_HttpResponse.SetContentType("text/html");
        return HTTP_SUCCESS;
    }

protected:
    HTTP_CODE OnInputValid()
    {
        if (m_numDrinks == 0 || m_beverage.IsEmpty()) {
            return HTTP_S_FALSE;
        }

        return HTTP_SUCCESS;
    }

    HTTP_CODE OnMoreDrinks()
    {
        if (m_numDrinks > 0) {
            return HTTP_SUCCESS;
        }

        return HTTP_S_FALSE;
    }

    HTTP_CODE OnDrinkNumber()
    {
        m_HttpResponse << m_numDrinks;
        return HTTP_SUCCESS;
    }

    HTTP_CODE OnBeverage()
    {
        m_HttpResponse << m_beverage;
        return HTTP_SUCCESS;
    }

    HTTP_CODE OnNextDrink()
    {
        --m_numDrinks;

        if (m_numDrinks > 0) {
            m_HttpResponse << m_numDrinks;
        } else {
            m_HttpResponse << "No more";
        }

        return HTTP_SUCCESS;
    }

private:
    long m_numDrinks;
    CStringA m_beverage;
}; // class CBeverageHandler
