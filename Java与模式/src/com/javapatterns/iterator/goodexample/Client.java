package com.javapatterns.iterator.goodexample;

import java.util.Enumeration;
import java.util.Vector;
import java.util.List;
import java.util.Hashtable;

public class Client
{
    private Vector v = new Vector();
    private Hashtable ht = new Hashtable();
    private Display disp = new Display();

    public void test()
    {
        System.out.println("Before testing...");
        disp.initList( v.elements() );
        disp.initList( ht.elements() );
        System.out.println("After testing...");
    }
    public static void main(String[] args)
    {
        Client client = new Client();

        client.test();
    }
}
