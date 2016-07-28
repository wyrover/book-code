#ifndef __LIST__
#define __LIST__

#include "main.h"
#include "CThread.h"

class CNode
{
public:
    CNode(CThread* thread) : thread(thread), next(0) { }
    CThread* Thread() const
    {
        return thread;
    }
    CNode*& Next()
    {
        return next;
    }
private:
    CThread* thread;
    CNode* next;
};

class CList
{
public:
    CList() : dwCount(0), head(0) { }
    virtual ~CList() { }
    void Insert(CNode*& node, CThread* thread);
    CThread* GetFirst();
    CThread* GetNext(CThread* thread);
    CThread* Iterate(CThread* thread);
    bool Remove(DWORD dwThreadId);
    DWORD Count() const
    {
        return dwCount;
    }
    CNode*& Head()
    {
        return head;
    }
protected:
    CList(const CList& list);
    CList& operator = (const CList& list);
private:
    CNode* head;
    DWORD dwCount;
};

void CList::Insert(CNode*& node, CThread* thread)
{
    if (node == NULL) {
        dwCount++;
        node = new CNode(thread);
        return;
    }

    Insert(node->Next(), thread);
}

inline CThread* CList::GetFirst()
{
    if (head == NULL) {
        return NULL;
    }

    return head->Thread();
}

CThread* CList::GetNext(CThread* thread)
{
    if (head == NULL) {
        return NULL;
    }

    if (thread == NULL) {
        return GetFirst();
    }

    if (head->Thread()->ThreadId() == thread->ThreadId()) {
        if (head->Next() != NULL) {
            return head->Next() != NULL ? head->Next()->Thread() : NULL;
        }
    }

    CNode* lst = head->Next();

    while (lst != NULL) {
        if (lst->Thread()->ThreadId() == thread->ThreadId()) {
            return lst->Next() != NULL ? lst->Next()->Thread() : NULL;
        }

        lst = lst->Next();
    }

    return NULL;
}

CThread* CList::Iterate(CThread* thread)
{
    if (head == NULL) {
        return NULL;
    }

    if (thread == NULL) {
        return GetFirst();
    }

    if (head->Thread()->ThreadId() == thread->ThreadId()) {
        return head->Next() != NULL ? head->Next()->Thread() : head->Thread();
    }

    CNode* lst = head->Next();

    while (lst != NULL) {
        if (lst->Thread()->ThreadId() == thread->ThreadId()) {
            return lst->Next() != NULL ? lst->Next()->Thread() : head->Thread();
        }

        lst = lst->Next();
    }

    return head->Thread();
}

bool CList::Remove(DWORD dwThreadId)
{
    if (head == NULL) {
        return NULL;
    }

    if (head->Thread()->ThreadId() == dwThreadId) {
        CNode* tmp = head;
        head = head->Next();
        delete tmp;
        dwCount--;
        return true;
    }

    CNode* tmp = head;
    CNode* lst = head->Next();

    while (lst != NULL) {
        if (lst->Thread()->ThreadId() == dwThreadId) {
            tmp->Next() = lst->Next();
            delete lst;
            dwCount--;
            return true;
        }

        lst = lst->Next();
        tmp = tmp->Next();
    }

    return false;
}

#endif
