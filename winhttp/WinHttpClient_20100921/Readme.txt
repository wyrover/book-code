Test
	VS 2008 sample project.

Common
	Common header files, WinHttpClient.h is in this folder.

ThirdParty
	Third party files, atlrx.h is in this folder.



History:

2010-9-21
1. Provide a method RequireValidSslCertificates() to let user select whether to accept any certificate while performing HTTPS request.
2. Support keeping the session open across multiple requests.
Thanks Scott Leckie from AXIOS who provides the changes and gives me the examples.  Thank you!

2010-4-29
1. Fix 1 memory leak.
2. Fix the problem "the proxy code do not work if I do not have a pac file and I am setting the proxy in IE, not using auto configuration."
