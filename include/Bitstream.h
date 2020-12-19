#pragma once

#include <vector>
#include <iostream>
#include <fstream>

class BitIostream
{
public:
	uint64_t getSeenBytes() const;

protected:
	uint64_t seenBytes_ = 0;
	std::size_t curPos_ = 0, curBit_ = 0;
	std::vector<char> bytes_ = {};
};

class BitIstream : public BitIostream
{
public:
	BitIstream(std::istream& in);
	bool readBit();
	void freeBuf();
	std::size_t getBytes() const;
private:
	std::istream& in_;
};

class BitOstream : public BitIostream
{
public:
	BitOstream(std::ostream& out);
	~BitOstream();
	BitOstream& operator<<(bool bit);
	BitOstream& operator<<(const std::vector<bool> &bits);
private:
	std::ostream& out_;
	void flush();
};

