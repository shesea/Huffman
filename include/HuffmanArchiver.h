#pragma once

#include "Huffman.h"
#include "Bitstream.h"

class FileSizeTracker
{
public:
	uint64_t getOriginalSize() const;
	uint64_t getCompressedSize() const;
	uint64_t getExtraSize() const;

	void setOriginalSize(uint64_t original);
	void setCompressedSize(uint64_t compressed);
	void setExtraSize(uint64_t extra);

private:
	uint64_t originalSize_ = 0, compressedSize_ = 0, extraSize_ = 0;
};

class HuffmanArchiver
{
public:
	FileSizeTracker compress(std::istream& in, std::ostream& out);
	FileSizeTracker extract(std::istream& in, std::ostream& out);

	std::unordered_map<char, uint64_t> calcFreqs(std::istream& in);

private:
	FileSizeTracker fileSizes_;

};

