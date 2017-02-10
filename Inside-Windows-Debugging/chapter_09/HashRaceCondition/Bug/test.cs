//
// HashRaceCondition.cs
//
using System;
using System.Security.Cryptography;
using System.Text;
using System.Threading;

class Program
{
    private static void ThreadProc(
        object data
        )
    {
        string hashValue = Convert.ToBase64String(
            g_hashFunc.ComputeHash(data as byte[]));
        Console.WriteLine("Thread #{0} done processing. Hash value was {1}", Thread
            .CurrentThread.ManagedThreadId, hashValue);
    }

    private static void Main(
        string[] args
        )
    {
        int n, numThreads;
        Thread[] threads;

        numThreads = (args.Length == 0) ? 1 : Convert.ToInt32(args[0]);
        threads = new Thread[numThreads];
        g_hashFunc = new SHA1CryptoServiceProvider();

        //
        // Start multiple threads to use the shared hash object
        //
        for (n = 0; n < threads.Length; n++)
        {
            threads[n] = new Thread(ThreadProc);
            threads[n].Start(Encoding.UTF8.GetBytes("abc"));
        }

        //
        // Wait for all the threads to finish
        //
        for (n = 0; n < threads.Length; n++)
        {
            threads[n].Join();
        }
    }

    private static HashAlgorithm g_hashFunc;
}
