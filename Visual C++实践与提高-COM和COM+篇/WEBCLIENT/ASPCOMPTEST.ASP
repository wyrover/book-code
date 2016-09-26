<%@ Language=VBScript %>
<HTML>
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Visual Studio 6.0">
</HEAD>
<BODY>

<H2>Here's what ASP and COM Components can do</H2>
<%
dim obj
set obj = Server.CreateObject("ASPComSamp.ASPComp")
obj.ProcessRequest ()
%>
</BODY>
</HTML>
