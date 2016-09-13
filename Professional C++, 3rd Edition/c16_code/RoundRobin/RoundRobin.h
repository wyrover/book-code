#pragma once

#include <stdexcept>
#include <vector>

// class template RoundRobin
// Provides simple round-robin semantics for a list of elements.
template <typename T>
class RoundRobin
{
public:
    // Client can give a hint as to the number of expected elements for
    // increased efficiency.
    RoundRobin(int numExpected = 0);
    virtual ~RoundRobin();

    // prevent assignment and pass-by-value
    RoundRobin(const RoundRobin& src) = delete;
    RoundRobin& operator=(const RoundRobin& rhs) = delete;

    // Appends elem to the end of the list. May be called
    // between calls to getNext().
    void add(const T& elem);

    // Removes the first (and only the first) element
    // in the list that is equal (with operator==) to elem.
    // May be called between calls to getNext().
    void remove(const T& elem);

    // Returns the next element in the list, starting with the first,
    // and cycling back to the first when the end of the list is
    // reached, taking into account elements that are added or removed.
    T& getNext();

private:
    std::vector<T> mElems;
    typename std::vector<T>::iterator mCurElem;
};

template <typename T>
RoundRobin<T>::RoundRobin(int numExpected)
{
    // If the client gave a guideline, reserve that much space.
    mElems.reserve(numExpected);
    // Initialize mCurElem even though it isn't used until
    // there's at least one element.
    mCurElem = end(mElems);
}

template <typename T>
RoundRobin<T>::~RoundRobin()
{
    // nothing to do here -- the vector will delete all the elements
}

// Always add the new element at the end
template <typename T>
void RoundRobin<T>::add(const T& elem)
{
    // Even though we add the element at the end, the vector could
    // reallocate and invalidate the iterator with the push_back() call.
    // Take advantage of the random access iterator features to save our
    // spot. When getNext() hasn't been called yet, mCurElem is equal
    // to end(mElems) (see constructor), in which case pos is set to -1.
    int pos = (mCurElem == end(mElems) ? -1 : mCurElem - begin(mElems));
    // Add the element.
    mElems.push_back(elem);
    // Reset our iterator to make sure it is valid.
    // If getNext() hasn't been called yet, reset mCurElem to end(mElems).
    mCurElem = (pos == -1 ? end(mElems) : begin(mElems) + pos);
}

template <typename T>
void RoundRobin<T>::remove(const T& elem)
{
    for (auto it = begin(mElems); it != end(mElems); ++it) {
        if (*it == elem) {
            // Removing an element will invalidate our mCurElem iterator if
            // it refers to an element past the point of the removal.
            // Take advantage of the random access features of the iterator
            // to track the position of the current element after removal.
            int newPos;

            // If current iterator is before or at the one we're removing,
            // the new position is the same as before.
            if (mCurElem <= it) {
                newPos = mCurElem - begin(mElems);
            } else {
                // otherwise, it's one less than before
                newPos = mCurElem - begin(mElems) - 1;
            }

            // Erase the element (and ignore the return value).
            mElems.erase(it);
            // Now reset our iterator to make sure it is valid.
            mCurElem = begin(mElems) + newPos;
            return;
        }
    }
}

template <typename T>
T& RoundRobin<T>::getNext()
{
    // First, make sure there are any elements.
    if (mElems.empty()) {
        throw std::out_of_range("No elements in the list");
    }

    // If getNext() hasn't been called yet, mCurElem is equal to end(mElems)
    // (see constructor). In that case wrap to the beginning.
    if (mCurElem == end(mElems)) {
        mCurElem = begin(mElems);
    } else {
        // getNext() has been called before.
        // Increment the iterator modulo the number of elements.
        ++mCurElem;

        if (mCurElem == end(mElems)) {
            mCurElem = begin(mElems);
        }
    }

    // Return a reference to the element.
    return *mCurElem;
}
