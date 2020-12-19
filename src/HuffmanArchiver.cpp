#include "HuffmanArchiver.h"

#include <iostream>

const std::size_t CHAR_SIZE = sizeof(char);
const std::size_t UINT_SIZE = sizeof(uint64_t);

uint64_t FileSizeTracker::getOriginalSize() const
{
	return originalSize_;
}

uint64_t FileSizeTracker::getCompressedSize() const
{
	return compressedSize_;
}

uint64_t FileSizeTracker::getExtraSize() const
{
	return extraSize_;
}

void FileSizeTracker::setOriginalSize(uint64_t original)
{
	originalSize_ = original;
}

void FileSizeTracker::setCompressedSize(uint64_t compressed)
{
	compressedSize_ = compressed;
}

void FileSizeTracker::setExtraSize(uint64_t extra)
{
	extraSize_ = extra;
}

std::unordered_map<char, uint64_t> HuffmanArchiver::calcFreqs(std::istream& in)
{
	std::unordered_map<char, uint64_t> freqTable;
	while (in)
	{
		char symbol;
		in.read(&symbol, sizeof(symbol));
		for (int i = 0; i < in.gcount(); i++)
		{
			freqTable[symbol]++;
		}
	}
	return freqTable;
}

FileSizeTracker HuffmanArchiver::compress(std::istream& in, std::ostream& out)
{
	if (!in)
	{
		throw std::runtime_error("Unable to find input file");
	}
	std::unordered_map<char, uint64_t> freqTable = calcFreqs(in);
	uint64_t size = freqTable.size();
	out.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (std::pair<char, uint64_t> x : freqTable)
	{
		char symbol = x.first;
		out.write((&symbol), sizeof(symbol));
		uint64_t freq = x.second;
		out.write(reinterpret_cast<char*>(&freq), sizeof(freq));
	}
	BitOstream bitOut(out);
	HuffmanTree tree(freqTable);
	std::unordered_map<char, std::vector<bool>> codeTable = tree.getCodes();
	in.clear();
	in.seekg(0, in.beg);
	while (in)
	{
		char symbol;
		in.read(&symbol, sizeof(symbol));
		bitOut << codeTable[symbol];
	}
	for (std::pair<char, uint64_t> x : freqTable)
	{
		fileSizes_.setOriginalSize(fileSizes_.getOriginalSize() + x.second);
	}
	fileSizes_.setCompressedSize(bitOut.getSeenBytes());
	fileSizes_.setExtraSize(freqTable.size() * (UINT_SIZE + CHAR_SIZE) + UINT_SIZE);
	return fileSizes_;
}


FileSizeTracker HuffmanArchiver::extract(std::istream& in, std::ostream& out)
{
	if (!in)
	{
		std::runtime_error("Unable to find input file");
	}
	std::unordered_map<char, uint64_t> freqTable;
	uint64_t size;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (uint64_t i = 0; i < size; i++) {
		char symbol;
		in.read(&symbol, sizeof(symbol));
		uint64_t freq;
		in.read(reinterpret_cast<char*>(&freq), sizeof(freq));
		freqTable[symbol] = freq;
	}
	BitIstream bitIn(in);
	HuffmanTree tree(freqTable);
	for (std::pair<char, uint64_t> x : freqTable)
	{
		fileSizes_.setOriginalSize(fileSizes_.getOriginalSize() + x.second);
	}
	std::unordered_map<std::vector<bool>, char> symbs = tree.getSymbols();
	if (freqTable.size() == 1)
	{
		char symbol = (*freqTable.begin()).first;
		for (std::size_t i = 0; i < fileSizes_.getOriginalSize(); i++)
		{
			out.write(&symbol, sizeof(symbol));
		}
	}
	else {
		uint64_t written_bytes = 0;
		while (written_bytes < fileSizes_.getOriginalSize())
		{
			std::vector<bool> curCode;
			while (symbs.find(curCode) == symbs.end())
			{
				curCode.push_back(bitIn.readBit());
			}
			out.write(&symbs[curCode], CHAR_SIZE);
			written_bytes++;
		}
	}
	fileSizes_.setCompressedSize(bitIn.getSeenBytes());
	fileSizes_.setExtraSize(freqTable.size() * (UINT_SIZE + CHAR_SIZE) + UINT_SIZE);
	return fileSizes_;
}
