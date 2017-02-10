using System;
using System.Collections.Generic;
using System.Threading;

class ObjectPool<T>
    where T : class, new()
{
    public ObjectPool(
        int capacity
        )
    {
        int n;

        if (capacity <= 0)
        {
            throw new ArgumentException();
        }

        m_objects = new Stack<T>(capacity);

        //
        // objects will be lazily created only as needed
        //
        for (n = 0; n < capacity; n++)
        {
            m_objects.Push(null);
        }

        m_objectsLock = new object();
        m_semaphore = new Semaphore(capacity, capacity);
    }

    public T GetObject()
    {
        T obj;

        m_semaphore.WaitOne();

        lock (m_objectsLock)
        {
            obj = m_objects.Pop();
        }

        if (obj == null)
        {
            //
            // delay-create the object
            //
            obj = new T();
        }

        return obj;
    }

    public void ReleaseObject(
        T obj
        )
    {
        if (obj == null)
        {
            throw new ArgumentException();
        }

        lock (m_objectsLock)
        {
            m_objects.Push(obj);
        }

        //
        // Signal that one more object is available in the pool
        //
        m_semaphore.Release();
    }

    private Stack<T> m_objects;
    private object m_objectsLock;
    private Semaphore m_semaphore;
}
