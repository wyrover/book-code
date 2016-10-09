package com.javapatterns.observersax;

import org.xml.sax.*;
import org.xml.sax.helpers.XMLReaderFactory;
import java.io.*;

public class MyReaderShort 
{
	private static XMLReader parser = null;
    private static ContentHandler handler = null;

    public static void main(String[] args) throws ClassNotFoundException
    {
        try 
        {
            parser = XMLReaderFactory.createXMLReader();
            
            Writer out = new OutputStreamWriter(System.out);
            handler = new MyHandlerShort(out);
            parser.setContentHandler(handler);
        
            parser.parse("myxml.xml");
            
            out.flush();
        }
        catch (Exception e) 
        {
            System.err.println(e); 
        }
    } 
}

