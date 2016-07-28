// Listing 66-4. The HashableString Class
class HashableString : public Hashable
{
public:
   HashableString() : string_{} {}
   ~HashableString() override;
   unsigned long hash() const override;
   bool equalTo(Hashable const&) const override;

    // Implement the entire interface of std::string ...
private:
   std::string string_;
};
