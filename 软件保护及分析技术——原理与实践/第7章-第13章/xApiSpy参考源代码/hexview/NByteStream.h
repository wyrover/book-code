#ifndef BYTESTREAM_20060628_H_
#define BYTESTREAM_20060628_H_

#include <cstddef>	// for std::size_t
#include <QtGlobal>

class NByteStream {
public:
    virtual ~NByteStream() {}
	
public:
	typedef std::size_t	size_type;
	typedef quint8		value_type;
	
private:
	class reference {
        friend class NByteStream;
	protected:
        reference(NByteStream &s, size_type i) : stream_(s), index_(i) {}
		
	public:
		~reference() {}
		
		reference &operator=(value_type value) {
			stream_.write(index_, value);
			return *this;
		}
	private:
        NByteStream	&stream_;
		size_type	index_;
	};
	
	class const_iterator {
        friend class NByteStream;
	protected:
		const_iterator(const const_iterator &it) : stream_(it.stream_), index_(it.index_) {
		}
		
        const_iterator(const NByteStream &s, size_type index) : stream_(s), index_(index) {
		}

	public:
		const_iterator operator++() { ++index_; return *this; }
		const_iterator operator--() { --index_; return *this; }
		
		const_iterator operator++(int) {
			const_iterator t(*this);
			++index_;
			return t;
		}

		const_iterator operator--(int) {
			const_iterator t(*this);
			--index_;
			return t;
		}
		
		value_type operator*() {
			return stream_[index_];
		}

	private:
        const NByteStream	&stream_;
		size_type			index_;
		
	};

public:	// element access	
	value_type operator[](size_type i) const {
		return read(i);
	}
	
	reference operator[](size_type i) {
		return reference(*this, i);
	}
	
	const_iterator begin() const	{ return const_iterator(*this, 0); }
	const_iterator end() const		{ return const_iterator(*this, size()); }
		
public:	// container information
	virtual size_type size() const = 0;
	virtual size_type max_size() const = 0;
	virtual bool empty() const = 0;
	
protected:
	virtual value_type read(size_type i) const = 0;
	virtual void write(size_type i, const value_type &v) = 0;
	
public:
	virtual void clear() = 0;
};

#endif

