package com.javapatterns.adapter.iterenum;

import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.Enumeration;

public class Enuterator implements Iterator
{
	Enumeration enum;

    public Enuterator(Enumeration enum)
    {
		this.enum = enum;
    }

    public boolean hasNext()
    {
		return enum.hasMoreElements();
    }

    public Object next() throws NoSuchElementException
    {
        return enum.nextElement();
    }

    public void remove()
    {
		throw new UnsupportedOperationException();
    }

}
