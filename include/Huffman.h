#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <cstdint>
#include <string>

class HuffmanNode
{
public:
	HuffmanNode(uint64_t freq, char symb);
	HuffmanNode(HuffmanNode* l, HuffmanNode* r, int symb);

	std::vector<bool> getCode() const;
	uint64_t getFreq() const;
	int getSymb() const;
	HuffmanNode* getLeftChild() const;
	HuffmanNode* getRightChild() const;

	void setCode(std::vector<bool> code);

	void update();

	bool operator==(const HuffmanNode& other) const;
private:
	std::vector<bool> code_ = {};
	uint64_t freq_ = 0;
	int symb_;
	HuffmanNode *l_ = nullptr,  *r_ = nullptr;
};

class HuffmanTree
{
public:
	HuffmanTree(const std::unordered_map<char, uint64_t>& freqs);
	std::unordered_map<char, std::vector<bool> > getCodes() const;
	std::unordered_map<std::vector<bool>, char> getSymbols() const;
	HuffmanNode getRoot() const;
private:
	std::vector<HuffmanNode> huffmanTree_;
	std::size_t alphabet_;
};
