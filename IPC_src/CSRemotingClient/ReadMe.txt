========================================================================
    CONSOLE APPLICATION : CSRemotingClient Project Overview
========================================================================

/////////////////////////////////////////////////////////////////////////////
Use:

.NET Remoting is a mechanism for one-way inter-process communication and RPC  
between .NET applications in the local machine or across the computers in the 
intranet and internet.

.NET Remoting allows an application to make an remotable object available 
across remoting boundaries, which includes different appdomains, processes or 
even different computers connected by a network. .NET Remoting makes a 
reference of a remotable object available to a client application, which then 
instantiates and uses a remotable object as if it were a local object. 
However, the actual code execution happens at the server-side. Any requests 
to the remotable object are proxied by the .NET Remoting runtime over Channel 
objects, that encapsulate the actual transport mode, including TCP streams, 
HTTP streams and named pipes. As a result, by instantiating proper Channel 
objects, a .NET Remoting application can be made to support different 
communication protocols without recompiling the application. The runtime 
itself manages the act of serialization and marshalling of objects across the 
client and server appdomains.

CSRemotingClient is a .NET Remoting client end. It accesses the remote 
objects exposed by the .NET Remoting server end, CSRemotingServer.

There are generally two ways to create the .NET Remoting client: using a 
configuration file or writing codes. The RemotingClientByConfig method 
demonstrates the former, and the RemotingClientByCode method illustrates the 
latter method.


/////////////////////////////////////////////////////////////////////////////
Project Relation:

CSRemotingClient -> CSRemotingServer
CSRemotingClient is the client end of the CSRemotingServer server end.

CSRemotingClient -> CSRemotingSharedLibrary
CSRemotingClient references a shared library for the client-activated 
remoting types.

CSRemotingClient - VBRemotingClient
CSRemotingClient and VBRemotingClient are the same .NET Remoting client 
implemented in different .NET languages.


/////////////////////////////////////////////////////////////////////////////
Creation:

A. Adding remotable types on the client end 

------
For client-activated types, they must be defined in an assembly shared by 
both client and server ends, because client-activated types require not only 
the same namespace/class name on both sides, but also the same assembly.

Step1. Add the reference to the .NET class library shared by the .NET 
Remoting server. The class library exposes the client-activated type 
(ClientActivatedObject) that inherits MarshalByRefObject.

------
For server-activated types, they can be either defined in a shared assembly, 
or defined in the server end and have en empty proxy of the type in the 
client ends. Please make sure that the server type and the proxy type have 
the same namespace/class name though it is not necessary to place them in the 
same assembly.

Step1. Add the proxy of the server-activated types (SingleCallObject, 
SingletonObject) that inherits MarshalByRefObject to the client project. 
There is no need to implement the body of the types.

B. Accessing .NET Remoting server using configuration file

Step1. Add an application configuration file to the project.

Step2. Define the channel to transport message.

<system.runtime.remoting>
	<application>
		<channels>
			<channel ref="tcp">
				<clientProviders>
					<formatter ref="binary" />
				</clientProviders>
				<serverProviders>
					<formatter ref="binary" typeFilterLevel="Full" />
				</serverProviders>
			</channel>
		</channels>
	</application>
</system.runtime.remoting>

The above configuration registers a TCP channel whose formatter is binary on 
both server and client sides.

Step3. Register the remotable types.

------
For client-activated types

<system.runtime.remoting>
	<application>
		<client url="tcp://localhost:6100/RemotingService">
			<activated type="RemotingShared.ClientActivatedObject, CSRemotingSharedLibrary">
			</activated>
		</client>
	</application>
</system.runtime.remoting>

The url attribute in <client> is only required for client-activated types. It 
specifies the URL that will be used to locate the activator for the attribute.
"tcp://localhost:6100/RemotingService" corresponds to the application name of 
the server end (<application name="RemotingService">).

------
For server-activated types

<system.runtime.remoting>
	<application>
		<client>
			<wellknown type="RemotingShared.SingleCallObject, CSRemotingClient"
                   url="tcp://localhost:6100/SingleCallService">
			</wellknown>
			<wellknown type="RemotingShared.SingletonObject, CSRemotingClient"
					   url="tcp://localhost:6100/SingletonService">
			</wellknown>
		</client>
	</application>
</system.runtime.remoting>

Step4. Read the configuration file and configure the remoting infrastructure 
for the client end. (RemotingConfiguration.Configure)

Step5. Create the remotable object.

	// Create a SingleCall server-activated object
	SingleCallObject remoteObj = new SingleCallObject();

	// [-or-] a Singleton server-activated object
	SingletonObject remoteObj = new SingletonObject();
	        
	// [-or-] a client-activated object
	ClientActivatedObject remoteObj = new ClientActivatedObject();

Step6. Use the remotable object as if it were a local object. For example,

	Console.WriteLine(remoteObj.GetRemoteObjectType());

C. Accessing .NET Remoting server using code

Step1. Specify the formatter of the messages for delivery. 
(BinaryClientFormatterSinkProvider, BinaryServerFormatterSinkProvider)
Once message has been formatted, it is transported to other application 
domains through the appropriate channel. .NET comes with the SOAP formatter 
(System.Runtime.Serialization.Formatters.Soap) and Binary formatter 
(System.Runtime.Serialization.Formatters.Binary).

Step2. Create and register the channel to transport message according to the 
channel on the server end. (TcpChannel/HttpChannel/IpcChannel, 
ChannelServices.RegisterChannel)

Step3. Create the remotable object.

------
For client-activated types

	RemotingConfiguration.RegisterActivatedClientType(
		typeof(ClientActivatedObject),
		"tcp://localhost:6100/RemotingService");
	ClientActivatedObject remoteObj = new ClientActivatedObject();

------
For server-activated types

	SingletonObject remoteObj = (SingletonObject)Activator.GetObject(
		typeof(SingletonObject),
		"tcp://localhost:6100/SingletonService");

Step4. Use the remotable object as if it were a local object. For example,

	Console.WriteLine(remoteObj.GetRemoteObjectType());


/////////////////////////////////////////////////////////////////////////////
References:

.NET Framework Remoting Architecture  
http://msdn.microsoft.com/en-us/library/2e7z38xb(VS.85).aspx

.NET Framework Remoting Overview  
http://msdn.microsoft.com/en-us/library/kwdt6w2k(VS.85).aspx

.NET Remoting
http://en.wikipedia.org/wiki/.NET_Remoting


/////////////////////////////////////////////////////////////////////////////