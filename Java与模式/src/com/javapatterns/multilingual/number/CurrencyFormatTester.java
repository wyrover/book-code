package com.javapatterns.multilingual.number;

import java.util.Locale;
import java.text.NumberFormat;

public class CurrencyFormatTester
{
    static public void displayCurrency(Double amount, Locale currentLocale)
    {

       NumberFormat formatter;
       String amountOut;

       formatter = NumberFormat.getCurrencyInstance(currentLocale);
       amountOut = formatter.format(amount);

       System.out.println(amountOut + "   " + currentLocale.toString());
    }

    static public void main(String[] args)
    {
       displayCurrency(new Double(1234567.89), new Locale("en", "US"));
       displayCurrency(new Double(1234567.89), new Locale("de", "DE"));
       displayCurrency(new Double(1234567.89), new Locale("fr", "FR"));
    }
}

