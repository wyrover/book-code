___________________________________________________________________

                              README

                               for

                Microsoft Press Electronic Books 

            Copyright (c) 1999 by Microsoft Corporation

                       All Rights Reserved
___________________________________________________________________

README CONTENTS
 - HOW TO MAKE THIS PRODUCT AVAILABLE FOR NETWORK INSTALL
     - How to modify the type of installation
 - WHAT'S ON THIS CD?
     - Electronic Book
     - Setup
     - Microsoft Internet Explorer 5.0
 - REQUIREMENTS
 - YOUR GUIDE TO MICROSOFT TECHNICAL SUPPORT
   - Questions Regarding Microsoft Press Content
   - Self-Help Tools to Find Answers Yourself
   - Direct Assistance with a Microsoft Technical Support Engineer
 - KNOWN ISSUES 
   - With the Microsoft HTML Help ActiveX Control Version 1.2
   - For User's of Microsoft Windows NT Workstation Version 4.0 with SP5
 - MICROSOFT PRESS SUPPORT INFORMATION


HOW TO MAKE THIS PRODUCT AVAILABLE FOR NETWORK INSTALL
======================================================
1. Copy the contents of the ebook directory and all subdirectories
   to a network share.
2. Make the share available to users.
3. Instruct users to run the setup.exe file.

The setup for this electronic book can be configured by 
the network administrator either to run from the install 
point (network install) or to download all necessary files
to the user machine (local install). By default the installation
is configured to run from the installation point.

How to modify the type of installation
--------------------------------------
1. Open the file "bonline.ini" in a text editor.
2. Edit the InstallType parameter to read either: 

   InstallType=Local
          or
   InstallType=Network

3. Save and close the file. Note: If you receive an error when saving the
file, verify in the file properties that the read-only file attribute is
not checked.


WHAT'S INCLUDED IN THIS PRODUCT?
================================

Electronic Book 
---------------
The complete text of the original print book is contained in the 
electronic book.

Setup
-----
Setup for this product will check the user's machine for required
files, install required files, create a desktop icon and Start menu
item for the electronic book, and (depending on the configuration 
described above) possibly copy the electronic book files to the user's
machine.

Microsoft Internet Explorer 5.0
-------------------------------
Important Note: Setup does not install the full version of Internet Explorer. 
Only the components required to view the electronic book are installed. 


For up-to-date information on the Internet Explorer
consult Microsoft's Internet Explorer Web site:

    http://www.microsoft.com/ie/




SYSTEM REQUIREMENTS
===================

This electronic book (ebook) requires a system running:
* 486/66 or higher processor
* Operating System required:
	Microsoft Windows 95 or
	Microsoft Windows 98 or
	*Microsoft Window NT Workstation 4.0 SP3 or higher operating system
* Memory required:
	Microsoft Windows 95: 12 MB RAM or
	Microsoft Windows 98: 16 MB RAM or
	Microsoft Windows NT4 SP3:16 MB RAM
* Disk Space required: 
	To install and run an ebook from a network (network installation): 10 MB
	To install an ebook to the hard drive (local installation): 20 - 31 MB
	To install Microsoft Internet Explorer to the hard drive (local installation)
	    and install and run an ebook from a network (network installation): 110 MB
	To install Microsoft Internet Explorer to the hard drive (local installation)
	    and install and run the ebook from a hard drive (local installation): 120 - 131 MB

Microsoft Internet Explorer 5.0 is included on the CD and will be 
installed on the user's machine automatically if necessary. 
The Internet Explorer setup has been configured to install the 
minimum necessary files and will not change the user's current
settings or associations.

* If you are running Microsoft Window NT Workstation 4.0 SP5, please see
the KNOWN ISSUES section of this file.

YOUR GUIDE TO MICROSOFT TECHNICAL SUPPORT
=========================================

Microsoft offers technical support and services ranging from self-help 
tools to direct assistance with a Microsoft Technical Support Engineer. 
Note: The services and prices listed here are available in the United 
States and Canada only (see Technical Support Worldwide below).

Self-Help Tools to Find Answers Yourself
----------------------------------------
Support Online: Rather help yourself to answers? Support Online makes
it easy.  Simply select your product and type in your question. Because
the site's search engine uses a natural language query, you can even 
write your question in your own words.  Then click "Find" and Support 
Online does all the rest, employing the latest Microsoft technologies 
to instantly search Microsoft Technical Support's vast collection of 
problem-solving tools and technical information like the Knowledge Base 
and Frequently Asked Questions.  Next time you have a technical question, 
visit Support Online at 

	http://support.microsoft.com/support/ 

and see how easy it is to get the answers you need.


Direct Assistance with a Microsoft Technical Support Engineer
-------------------------------------------------------------
Microsoft offers Standard No-Charge Support for retail versions* 
of Microsoft Internet Explorer as follows:

	90 days no-charge support beginning with first contact with an engineer

To access Standard No-Charge Support for this product: 
In the U.S.: (425) 635-7123 
Monday-Friday, 6:00 a.m.- 6:00 p.m. Pacific time, excluding holidays 

In Canada: (905) 568-4494 
Monday-Friday, 8:00 a.m.- 8:00 p.m. Eastern time, excluding holidays 

*With the exception of the MSN online service, if your Microsoft product was
preinstalled or distributed with your personal computer dedicated system or Windows
CE-based device, or provided by an Internet Service Provider (ISP), the personal
computer manufacturer, the device/system manufacturer, or ISP is responsible for
providing your product support. Please contact the manufacturer or source of your
Microsoft product for support information.

Submitting questions via the Internet: In the U.S. and Canada, you can also submit 
your Standard No-Charge, Pay-Per-Incident or Priority Annual support questions via 
the Internet with Web Response.  For more details, go to Support Online at 

	http://support.microsoft.com/support
	


KNOWN ISSUES 
============

MICROSOFT HTML HELP ACTIVEX CONTROL VERSION 1.2
-----------------------------------------------

* The highlighting feature in Search (on the Help Viewer toolbar, 
  click Options/Search Highlight Off or On) requires reloading the topic 
  currently displayed to see a change.

* Printing a very large topic may fail.

* The HTML Help Installation and Update package (Hhupd.exe) will not 
  run on a Windows NT system unless you have administrative privileges. 

* HTML Help does not run on any version of Windows NT earlier than 
  Windows NT 4.0. 


MICROSOFT WINDOWS NT WORKSTATION 4.0 SP5
----------------------------------------

Setup may fail to automatically install necessary Internet Explorer 5.0
components on systems running Microsoft Windows NT Workstation version 4.0 SP5 
and Internet Explorer version 2.0. To install a full version of IE 5.0, 
select IE5SETUP.EXE from the IE5SETUP directory.




Microsoft Press Support Information
===================================
Every effort has been made to ensure the accuracy of this electronic book. 
Microsoft Press provides corrections for books at the following Web site:

    http://mspress.microsoft.com/support/

If you have comments, questions, or ideas regarding the product, please 
send them to Microsoft Press via e-mail to:

    MSPINPUT@MICROSOFT.COM

or via postal mail to:

    Microsoft Press
    Attn:  Microsoft Press Technical Support
    One Microsoft Way
    Redmond, WA  98052-6399

Please note that product support is not offered through the above
addresses. 



<end>