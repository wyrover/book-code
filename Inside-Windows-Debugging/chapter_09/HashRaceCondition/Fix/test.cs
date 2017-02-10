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
        SHA1CryptoServiceProvider hashObj;
        string hashValue;

        //
        // Fetch a free object from the pool (could block!)
        //
        hashObj = g_hashObjPool.GetObject();
        hashValue = Convert.ToBase64String(
            hashObj.ComputeHash(data as byte[]));

        //
        // Release the object back to the pool
        //
        g_hashObjPool.ReleaseObject(hashObj);

        Console.WriteLine("Thread #{0} done processing. Hash value was {1}", Thread
            .CurrentThread.ManagedThreadId, hashValue);
    }

    private static void Main(
        string[] args
        )
    {
        DateTime start;
        double totalTime;
        int n, numThreads;
        Thread[] threads;

        start = DateTime.Now;

        numThreads = (args.Length == 0) ? 1 : Convert.ToInt32(args[0]);
        g_hashObjPool = new ObjectPool<SHA1CryptoServiceProvider>(g_poolSize);
        threads = new Thread[numThreads];

        //
        // Start n threads
        //
        for (n = 0; n < threads.Length; n++)
        {
            threads[n] = new Thread(ThreadProc);
            threads[n].Start(Encoding.UTF8.GetBytes("abc"));
        }

        //
        // Wait for them to finish
        //
        for (n = 0; n < threads.Length; n++)
        {
            threads[n].Join();
        }

        totalTime = (DateTime.Now - start).TotalMilliseconds;
        Console.WriteLine("Test completed in {0} ms", totalTime);
    }

    private static ObjectPool<SHA1CryptoServiceProvider> g_hashObjPool;
    private const int g_poolSize = 5;
}
