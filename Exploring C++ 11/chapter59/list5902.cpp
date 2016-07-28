// Listing 59-2. Implementing list::push_back and list::pop_back
template<class T>
void list<T>::push_back(T const& x)
{
    node* n{new node{x}};
    if (tail_ == nullptr)
        head_ = tail_ = n;
    else
    {
        n->prev_ = tail_;
        tail_->next = n;
    }
    ++size_;
}

template<class T>
void list<T>::pop_back()
{
    node* n{tail_};
    if (head_ == tail_)
        head_ = tail_ = nullptr;
    else
    {
        tail_ = tail_->prev_;
        tail_->next_ = nullptr;
    }
    --size_;
    delete n;
}
