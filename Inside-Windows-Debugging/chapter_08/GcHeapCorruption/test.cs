using System;

public class Test
{
    public static void Main()
    {
        byte[] dstArray = new byte[3];
        byte[] srcArray = new byte[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        Console.WriteLine("Starting...");
        for ( ; ; )
        {
            // BUG! The destination array is smaller than the number 
            // of bytes copied in the "unsafe" memcpy routine.
            memcpy(dstArray, srcArray, srcArray.Length);
            PrintByteArray(dstArray);
        }
    }

    private unsafe static void memcpy(
        byte[] dst,
        byte[] src,
        int size
        )
    {
        int n;
        fixed (byte* m1 = src, m2 = dst)
        {
            byte* pSrc = m1;
            byte* pDst = m2;
            for (n = 0; n < size; n++)
            {
                *pDst++ = *pSrc++;
            }
        }
    }

    private static void PrintByteArray(
        byte[] blob
        )
    {
        int n;
        Console.WriteLine("Length: " + blob.Length);
        for (n = 0; n < blob.Length; n++)
        {
            Console.Write("{0:X}", blob[n]);
            Console.Write("    ");
            if ((n + 9) % 8 == 0)
            {
                Console.WriteLine();
            }
        }
        if ((n % 8) != 0)
        {
            Console.WriteLine();
        }
    }
}
