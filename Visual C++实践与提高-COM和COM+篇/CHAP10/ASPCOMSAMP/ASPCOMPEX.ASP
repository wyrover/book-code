<HTML>

<HEAD>
<TITLE>ASPCompEx</TITLE> 
</HEAD> 

<H2>Here's what ASP and COM Components can do</H2>
<SCRIPT LANGUAGE=vbscript RUNAT=Server>
	Set ASPObj = Server.CreateObject("ASPComSamp.CASPComp") 
	ASPObj.ProcessRequest()
</SCRIPT>

</HTML>
