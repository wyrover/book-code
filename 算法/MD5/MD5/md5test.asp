<HTML>
<%
' This VBScript ASP file tests the accompanying MD5.asp for generating
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
<!--#include file="md5.asp"-->
<%

Dim sDigest

response.write "<FONT FACE='COURIER' SIZE=3>"

sDigest = md5("")
response.write "MD5("""")<BR>"
response.write "   Expected: d41d8cd98f00b204e9800998ecf8427e<BR>"
response.write "   Actual:&nbsp;&nbsp; " & sDigest & "<BR><BR>"

sDigest = md5("a")
response.write "MD5(""a"")<BR>"
response.write "   Expected: 0cc175b9c0f1b6a831c399e269772661<BR>"
response.write "   Actual:&nbsp;&nbsp; " & sDigest & "<BR><BR>"

sDigest = md5("abc")
response.write "MD5(""abc"")<BR>"
response.write "   Expected: 900150983cd24fb0d6963f7d28e17f72<BR>"
response.write "   Actual:&nbsp;&nbsp; " & sDigest & "<BR><BR>"

sDigest = md5("message digest")
response.write "MD5(""message digest"")<BR>"
response.write "   Expected: f96b697d7cb7938d525a2f31aaf161d0<BR>"
response.write "   Actual:&nbsp;&nbsp; " & sDigest & "<BR><BR>"

sDigest = md5("abcdefghijklmnopqrstuvwxyz")
response.write "MD5(""abcdefghijklmnopqrstuvwxyz"")<BR>"
response.write "   Expected: c3fcd3d76192e4007dfb496cca67e13b<BR>"
response.write "   Actual:&nbsp;&nbsp; " & sDigest & "<BR><BR>"

sDigest = md5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
response.write "MD5(""ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"")<BR>"
response.write "   Expected: d174ab98d277d9f5a5611c2c9f419d9f<BR>"
response.write "   Actual:&nbsp;&nbsp; " & sDigest & "<BR><BR>"

sDigest = md5("12345678901234567890123456789012345678901234567890123456789012345678901234567890")
response.write "MD5(""12345678901234567890123456789012345678901234567890123456789012345678901234567890"")<BR>"
response.write "   Expected: 57edf4a22be3c955ac49da2e2107b67a<BR>"
response.write "   Actual:&nbsp;&nbsp; " & sDigest & "<BR><BR>"

response.write "</FONT>"
%>
</HTML>
