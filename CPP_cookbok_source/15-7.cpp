int prompt_user()
{
    cout << "give me an integer ... ";
    cin >> n;
    return n;
}

void f()
{
    vector<int> v;
    // The following could result in v being initialized out of
    // sequence
    intialize(v) = prompt_user(), prompt_user();
}