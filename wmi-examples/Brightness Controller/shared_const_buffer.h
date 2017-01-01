template<class T>
class shared_const_buffer
{
public:
	// Construct from a `shared_ptr`
	explicit shared_const_buffer(std::shared_ptr<std::vector<T>> const& p_data) :
		data_(p_data), buffer_(boost::asio::buffer(*data_)) {}

	// Implement the ConstBufferSequence requirements.
	using value_type = boost::asio::const_buffer;
	using const_iterator = boost::asio::const_buffer const*;

	boost::asio::const_buffer const* begin() const { return &buffer_; }
	boost::asio::const_buffer const* end()	 const { return &buffer_ + 1; }

private:
	std::shared_ptr<std::vector<T>> data_;
	boost::asio::const_buffer buffer_;
};