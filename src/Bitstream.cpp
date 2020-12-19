#include "Bitstream.h"

#include <algorithm>
#include <vector>
#include <cstdint>
#include <string.h>
#include <assert.h>

const std::size_t BYTES = 1 << 15;
const std::size_t BYTE_SIZE = 8;

BitIstream::BitIstream(std::istream& in)
	:in_(in)
{
	bytes_.resize(BYTES);
	freeBuf();
}

void BitIstream::freeBuf()
{
	curBit_ = 0;
	curPos_ = 0;
	in_.read(&bytes_[0], bytes_.size());
	seenBytes_ += static_cast<uint64_t>(in_.gcount());
}

bool BitIstream::readBit()
{
	if (!getBytes()) {
		throw std::runtime_error("Unable to get input access");
	}
	bool readBit = (bytes_[curPos_] >> curBit_) & 1;
	if (curBit_ != BYTE_SIZE - 1)
	{
		curBit_++;
		return readBit;
	}
	curBit_ = 0;
	curPos_++;
	if (curPos_ >= getBytes())
	{
		freeBuf();
	}
	return readBit;
}

std::size_t BitIstream::getBytes() const
{
	return static_cast<int>(in_.gcount());
}

uint64_t BitIostream::getSeenBytes() const
{
	return seenBytes_;
}

BitOstream::BitOstream(std::ostream& out)
	: out_(out)
{
	bytes_.resize(BYTES);
}

BitOstream& BitOstream::operator<<(bool bit)
{
	if (bit)
	{
		bytes_[curPos_] |= (1 << curBit_);
	}
	if (curBit_ != BYTE_SIZE - 1)
	{
		if (curBit_ == 0)
		{
			seenBytes_++;
		}
		curBit_++;
		return *this;
	}
	curBit_ = 0;
	curPos_++;
	if (curPos_ == bytes_.size())
	{
		flush();
	}
	return *this;
}

BitOstream& BitOstream::operator<<(const std::vector<bool>& bits)
{
	for (bool bit : bits)
	{
		*this << bit;
	}
	return *this;
}


void BitOstream::flush()
{
	if (curBit_)
	{
		curPos_++;
		curBit_ = 0;
	}
    out_.write(&bytes_[0], curPos_);
	std::fill(bytes_.begin(), bytes_.end(), 0);
	curPos_ = 0;
}

BitOstream::~BitOstream()
{
	flush();
}
