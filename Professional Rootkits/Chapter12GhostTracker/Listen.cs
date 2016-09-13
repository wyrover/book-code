// Listen.cs
// Copyright Ric Vieler, 2006
// Spawns a TargetController for each new connection

using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace GhostTracker
{
	/// <summary>
	/// Summary description for Listen.
	/// </summary>
	public class Listen
	{
		MainForm mainFormPtr;
		TargetController myTarget = null;
		Thread myThread = null;
		TcpListener myServer = null;
		bool listening = false;

		public Listen( MainForm parent )
		{
			mainFormPtr = parent;
		}

		/// <summary>
		/// Listen for Ghost connections
		/// </summary>
		public void Start()
		{
			try
			{
				// Set the TcpListener on port 80.
				Int32 port = 80;
				// Use "0" to let the IP Stack figure out the IP Address
				IPAddress localAddr = IPAddress.Parse( "0" );
				myServer = new TcpListener( localAddr, port );

				// Start listening for client requests.
				myServer.Start();
				listening = true;

				// Enter the listening loop.
				while ( listening ) 
				{
					// See if a connection request is pending
					if ( !myServer.Pending() )
					{
						// Wait 50 milliseconds and then try again
						Thread.Sleep( 50 );
						if ( !listening )
							break;
						continue;
					}
					// Perform a blocking call to accept requests.
					TcpClient client = myServer.AcceptTcpClient();
					
					// Spawn a new TargetController for each new connection
					myTarget = new TargetController( mainFormPtr, client );
					myThread = new Thread( new ThreadStart( myTarget.Start ) );

					// Start the TARGET thread.
					myThread.Start();
				}
			}
			catch( SocketException )
			{
				string message	= "Could not get a port 80 socket.\n"
								+ "Make sure the port is not in use.\n"
								+ "(IIS uses port 80 on most servers)";
				mainFormPtr.Alert( null, message );
			}
			catch( ThreadAbortException )
			{
				// Application is shutting down
			}
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		public void Stop()
		{
			// Stop listening for client requests.
			listening = false;
			Thread.Sleep( 100 );
			myServer.Stop();

            	if ( myThread != null )
			{
				myTarget.Stop();
				// Stop the target thread.
				myThread.Abort();
				myThread.Join();
			}
		}
	}
}
