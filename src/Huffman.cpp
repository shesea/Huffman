#include "Huffman.h"

#include <iostream>
#include <fstream>

struct NodeComparator
{
	bool operator()(const HuffmanNode* l, const HuffmanNode* r)
	{
		if (l->getFreq() != r->getFreq())
		{
			return  l->getFreq() > r->getFreq();
		}
		return l->getSymb() > r->getSymb();
	}
};

HuffmanNode::HuffmanNode(HuffmanNode* l, HuffmanNode* r, int symb)
	: symb_(symb), l_(l), r_(r) 
{
	freq_ = l->freq_ + r->freq_;
}

HuffmanNode::HuffmanNode(uint64_t freq, char symb)
	: freq_(freq), symb_(symb) {}

std::vector<bool> HuffmanNode::getCode() const
{
	return code_;
}

uint64_t HuffmanNode::getFreq() const
{
	return freq_;
}

int HuffmanNode::getSymb() const
{
	return symb_;
}

HuffmanNode* HuffmanNode::getLeftChild() const
{
	return l_;
}

HuffmanNode* HuffmanNode::getRightChild() const
{
	return r_;
}

void HuffmanNode::setCode(std::vector<bool> code)
{
	code_ = code;
}

void HuffmanNode::update()
{
	if (!l_)
	{
		return;
	}
	l_->code_ = code_;
	l_->code_.push_back(0);
	l_->update();
	r_->code_ = code_;
	r_->code_.push_back(1);
	r_->update();
}

bool HuffmanNode::operator==(const HuffmanNode& other) const
{
	if (!l_)
	{
		return  other.symb_ == symb_ && other.freq_ == freq_ && !other.l_ ;

	}
	if (!other.l_)
	{
		return false;
	}
	return other.symb_ == symb_ && other.freq_ == freq_ && *other.l_ == *l_ &&  *other.r_ == *r_;
}


HuffmanTree::HuffmanTree(const std::unordered_map<char, uint64_t>& freqs) 
	:alphabet_(freqs.size())
{
	int char_id = 300;
	huffmanTree_.reserve(2 * freqs.size());
	for (std::pair<char, uint64_t> symbol : freqs)
	{
		HuffmanNode node(symbol.second, symbol.first);
		if (alphabet_ == 1)
		{
			node.setCode(std::vector<bool>{true});
		}
		huffmanTree_.push_back(node);
	}
	std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, NodeComparator> heap_nodes;
	for (std::size_t i = 0; i < alphabet_; i++)
	{
		heap_nodes.push(&huffmanTree_[i]);
	}
	while (heap_nodes.size())
	{
		HuffmanNode* f = heap_nodes.top();
		heap_nodes.pop();
		if (heap_nodes.empty()) {
			break;
		}
		HuffmanNode* s = heap_nodes.top();
		heap_nodes.pop();
		huffmanTree_.push_back(HuffmanNode(f, s, char_id++));
		heap_nodes.push(&huffmanTree_.back());
	}
	if(!huffmanTree_.empty())
	{
		(huffmanTree_.back()).update();
	}
}


std::unordered_map<char, std::vector<bool>> HuffmanTree::getCodes() const {
	std::unordered_map<char, std::vector<bool> > codes;
	for (std::size_t i = 0; i < alphabet_; i++) {
		codes[huffmanTree_[i].getSymb()] = huffmanTree_[i].getCode();
	}
	return codes;
}

std::unordered_map<std::vector<bool>, char> HuffmanTree::getSymbols() const {
	std::unordered_map<std::vector<bool>, char> keys;
	for (std::size_t i = 0; i < alphabet_; i++) {
		keys[huffmanTree_[i].getCode()] = huffmanTree_[i].getSymb();
	}
	return keys;
}

HuffmanNode HuffmanTree::getRoot() const
{
	return huffmanTree_.back();
}

