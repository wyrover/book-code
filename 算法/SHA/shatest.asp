<HTML>
<%
' This VBScript ASP file tests the accompanying SHA256.asp for generating
' digests of strings.
'
' See the VB6 project that accompanies this sample for full code comments on how
' it works.
'
' This is 'free' software with the following restrictions:
'
' You may not redistribute this code as a 'sample' or 'demo'. However, you are free
' to use the source code in your own code, but you may not claim that you created
' the sample code. It is expressly forbidden to sell or profit from this source code
' other than by the knowledge gained or the enhanced value added by your own code.
'
' Use of this software is also done so at your own risk. The code is supplied as
' is without warranty or guarantee of any kind.
'
' Should you wish to commission some derivative work based on this code provided
' here, or any consultancy work, please do not hesitate to contact us.
'
' Web Site:  http://www.frez.co.uk
' E-mail:    sales@frez.co.uk
%>
<!--#include file="sha256.asp"-->
<%

Dim sDigest

response.write "<FONT FACE='COURIER' SIZE=3>"

sDigest = sha256("abc")
response.write "SHA256(""abc"")<BR>"
response.write "   Expected: ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad<BR>"
response.write "   Actual:&nbsp;&nbsp; " & sDigest & "<BR><BR>"

sDigest = sha256("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq")
response.write "SHA256(""abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"")<BR>"
response.write "   Expected: 248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1<BR>"
response.write "   Actual:&nbsp;&nbsp; " & sDigest & "<BR><BR>"

response.write "</FONT>"
%>
</HTML>
