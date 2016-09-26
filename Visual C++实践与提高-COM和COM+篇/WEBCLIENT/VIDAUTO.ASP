<%@ Language=VBScript %>
<HTML style="width=300;height=220;">
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Visual Studio 6.0">
</HEAD>
<BODY  bgColor=silver>
<P >&nbsp;</P>
<P >  组件返回结果：</P>
<%
dim sInput
sInput=Request.QueryString("input") 

dim obj
set obj = Server.CreateObject("ATLSample.Account")
dim sRes
sRes=obj.Post (Clng(sInput))
Response.Write("<P align=center>")
Response.Write(sRes)	
Response.Write("</P>")		


%>

<P></P>
</BODY>
</HTML>
