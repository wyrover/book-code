package com.javapatterns.simplefactory.dateformat;

import java.util.Locale;
import java.util.Date;
import java.text.DateFormat;
import java.text.ParseException;

public class DateTester
{
    public static void main(String[] args)
    {
		Locale locale = Locale.FRENCH;
		Date date = new Date();

        String now = DateFormat.getTimeInstance(DateFormat.DEFAULT, locale)
            .format(date);

		System.out.println(now);

        try
        {
			date = DateFormat.getDateInstance(DateFormat.DEFAULT, locale)
                .parse("16 nov. 01");
            System.out.println(date);
        }
        catch(ParseException e)
        {
            System.out.println("Parsing exception:"+e);
        }
    }
}
