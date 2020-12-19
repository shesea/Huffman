#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <iostream>

#include "Huffman.h"
#include "HuffmanArchiver.h"
#include "Bitstream.h"

using std::ios_base;

TEST_CASE("test node")
{
    SUBCASE("init by frequency")
    {
        HuffmanNode node(67, 's');
        CHECK_EQ(node.getLeftChild(), nullptr);
        CHECK_EQ(node.getRightChild(), nullptr);
        CHECK_EQ(node.getSymb(), (int)'s');
        CHECK_EQ(node.getCode(), std::vector<bool>{});
        CHECK_EQ(node.getFreq(), 67);
    }
    SUBCASE("init by children")
    {
        HuffmanNode l(56, 'o');
        HuffmanNode r(4, 'i');
        HuffmanNode node(&l, &r, 10);
        CHECK_EQ(*node.getLeftChild(), l);
        CHECK_EQ(*node.getRightChild(), r);
        CHECK_EQ(node.getSymb(), 10);
        CHECK_EQ(node.getCode(), std::vector<bool>{});
        CHECK_EQ(node.getFreq(), 60);
    }
}

TEST_CASE("test tree")
{
    SUBCASE("one node")
    {
        std::unordered_map<char, uint64_t> freqs;
        freqs['u'] = 1;
        HuffmanTree tree(freqs);
        HuffmanNode node(1, 'u');
        std::unordered_map<char, std::vector<bool>> code = tree.getCodes();
        std::unordered_map<std::vector<bool>, char> symb = tree.getSymbols();
        CHECK_EQ(tree.getRoot(), node);
        CHECK_EQ(code['u'], std::vector<bool>{1});
        CHECK_EQ(symb[std::vector<bool>{1}], 'u');
    }
    SUBCASE("two nodes")
    {
        std::unordered_map<char, uint64_t> freqs;
        freqs['u'] = 1;
        freqs['i'] = 4;
        HuffmanTree tree(freqs);
        HuffmanNode node1(1, 'u');
        HuffmanNode node2(4, 'i');
        HuffmanNode node3(&node1, &node2, 300);
        std::unordered_map<char, std::vector<bool>> code = tree.getCodes();
        std::unordered_map<std::vector<bool>, char> symb = tree.getSymbols();
        CHECK_EQ(tree.getRoot(), node3);
        CHECK_EQ(code['i'], std::vector<bool>{1});
        CHECK_EQ(symb[std::vector<bool>{1}], 'i');
        CHECK_EQ(code['u'], std::vector<bool>{0});
        CHECK_EQ(symb[std::vector<bool>{0}], 'u');
    }
    SUBCASE("seven nodes")
    {
        std::unordered_map<char, uint64_t> freqs;
        freqs['b'] = 3;
        freqs['e'] = 4;
        freqs['p'] = 2;
        freqs[' '] = 2;
        freqs['o'] = 2;
        freqs['r'] = 1;
        freqs['!'] = 1;
        HuffmanTree tree(freqs);
        std::unordered_map<char, std::vector<bool>> code = tree.getCodes();
        std::unordered_map<std::vector<bool>, char> symb = tree.getSymbols();
        CHECK_EQ(code['b'].size(), 2);
        CHECK_EQ(code['e'].size(), 2);
        CHECK_EQ(code['p'].size(), 3);
        CHECK_EQ(code[' '].size(), 3);
        CHECK_EQ(code['o'].size(), 3);
        CHECK_EQ(code['r'].size(), 4);
        CHECK_EQ(code['!'].size(), 4);
    }
}
TEST_CASE("bin input")
{
    SUBCASE("null")
    {
        std::stringstream in;
        char mask = 0;
        in.write(&mask, sizeof(mask));
        BitIstream is(in);
        for (uint64_t i = 0; i < 8; i++) {
            CHECK_EQ(is.readBit(), 0);
        }
    }
    SUBCASE("1 byte")
    {
        std::stringstream in;
        char mask = (1 << 2) | (1 << 3) | (1 << 5);
        in.write(&mask, sizeof(mask));
        BitIstream is(in);
        for (uint64_t i = 0; i < 8; i++) {
            CHECK_EQ(is.readBit(), (i % 8 == 3) || (i % 8 == 2) || (i % 8 == 5));
        }
    }
}
TEST_CASE("bin output")
{
    SUBCASE("null")
    {
        std::stringstream out;
        {
            BitOstream os(out);
            os << std::vector<bool>{0, 0, 0, 0, 0, 0, 0, 0};
        }
        char mask = 0;
        char c;
        out.read(&c, sizeof(c));
        CHECK_EQ(c, static_cast<char>(mask));
    }
    SUBCASE("255")
    {
        std::stringstream out;
        {
            BitOstream os(out);
            os << std::vector<bool>{1, 1, 1, 1, 1, 1, 1, 1};
        }
        char mask = 255;
        char c;
        out.read(&c, sizeof(c));
        CHECK_EQ(c, static_cast<char>(mask));
    }
    SUBCASE("5 bytes")
    {
        std::stringstream out;
        {
            BitOstream os(out);
            std::vector<bool> data;
            os << std::vector<bool>{0, 1, 0, 0, 0, 0, 0, 1};
            os << std::vector<bool>{0, 0, 0, 1, 1, 0, 0, 0};
            os << std::vector<bool>{1, 1, 1, 0, 0, 0, 0, 0};
            os << std::vector<bool>{0, 0, 0, 0, 0, 0, 1, 1};
            os << std::vector<bool>{0, 1, 1, 0, 0, 0, 0, 0};
        }
        char c;
        unsigned char mask = (1 << 1) | (1 << 7);
        out.read(&c, sizeof(c));
        CHECK_EQ(c, static_cast<char>(mask));
        mask = (1 << 3) | (1 << 4);
        out.read(&c, sizeof(c));
        CHECK_EQ(c, static_cast<char>(mask));
        mask = (1 << 0) | (1 << 1) | (1 << 2);
        out.read(&c, sizeof(c));
        CHECK_EQ(c, static_cast<char>(mask));
        mask = (1 << 6) | (1 << 7);
        out.read(&c, sizeof(c));
        CHECK_EQ(c, static_cast<char>(mask));
        mask = (1 << 1) | (1 << 2);
        out.read(&c, sizeof(c));
        CHECK_EQ(c, static_cast<char>(mask));
    }
}
TEST_CASE("calculating frequencies")
{
    SUBCASE("empty")
    {
        std::string str("");
        std::stringstream in(str);
        HuffmanArchiver archiver;
        std::unordered_map<char, uint64_t> freqTable = archiver.calcFreqs(in);
        std::unordered_map<char, uint64_t> freqs = {};
        CHECK_EQ(freqTable, freqs);
    }
    SUBCASE("one kind of symbols")
    {
        std::string str("oooooooo");
        std::stringstream in(str);
        HuffmanArchiver archiver;
        std::unordered_map<char, uint64_t> freqTable = archiver.calcFreqs(in);
        std::unordered_map<char, uint64_t> freqs = {{'o', 8}};
        CHECK_EQ(freqTable, freqs);
    }
    SUBCASE("different symbols")
    {
        std::string str("help");
        std::stringstream in(str);
        HuffmanArchiver archiver;
        std::unordered_map<char, uint64_t> freqTable = archiver.calcFreqs(in);
        std::unordered_map<char, uint64_t> freqs = {{'e', 1}, {'h', 1},
        {'l', 1}, {'p', 1}};
        CHECK_EQ(freqTable, freqs);
    }

}
TEST_CASE("compress_sizes")
{
    SUBCASE("empty")
    {
        HuffmanArchiver archiver;
        FileSizeTracker track;
        std::string str("");
	    std::stringstream in(str);
        std::ofstream out;
	    track = archiver.compress(in, out);
        CHECK_EQ(track.getOriginalSize(), 0);
        CHECK_EQ(track.getCompressedSize(), 0);
        CHECK_EQ(track.getExtraSize(), 8);
    }
    SUBCASE("one word")
    {
        HuffmanArchiver archiver;
        FileSizeTracker track;
        std::string str("help");
	    std::stringstream in(str);
        std::ofstream out;
	    track = archiver.compress(in, out);
        CHECK_EQ(track.getOriginalSize(), 4);
        CHECK_EQ(track.getCompressedSize(), 2);
        CHECK_EQ(track.getExtraSize(), 44);
    }
}
TEST_CASE("extract_sizes")
{
    SUBCASE("empty")
    {
        HuffmanArchiver archiver;
        FileSizeTracker track;
        std::string str("");
	    std::stringstream in(str);
        std::ofstream out;
	    track = archiver.compress(in, out);
        CHECK_EQ(track.getOriginalSize(), 0);
        CHECK_EQ(track.getCompressedSize(), 0);
        CHECK_EQ(track.getExtraSize(), 8);
    }
    SUBCASE("one word")
    {
        HuffmanArchiver archiver;
        FileSizeTracker track;
        std::string str("help");
	    std::stringstream in(str);
        std::ofstream out;
	    track = archiver.compress(in, out);
        CHECK_EQ(track.getOriginalSize(), 4);
        CHECK_EQ(track.getCompressedSize(), 2);
        CHECK_EQ(track.getExtraSize(), 44);
    }
}
TEST_CASE("end-to-end")
{
    SUBCASE("empty")
    {
        std::stringstream in;
        std::stringstream compressed;
        std::stringstream extracted;
        HuffmanArchiver archiver;
        archiver.compress(in, compressed);
        archiver.extract(compressed, extracted);
        CHECK_EQ(extracted.str(), "");
    }
    SUBCASE("one letter")
    {
        HuffmanArchiver arc1;
        HuffmanArchiver arc2;
        std::stringstream in_file("oooooo");
        std::stringstream zipped_file;
        std::stringstream out_file;
        arc1.compress(in_file, zipped_file);
        arc2.extract(zipped_file, out_file);
        CHECK_EQ("oooooo", out_file.str());
    }
    SUBCASE("different letters")
    {
        HuffmanArchiver arc1;
        HuffmanArchiver arc2;
        std::stringstream in_file("oyaoyaoya");
        std::stringstream zipped_file;
        std::stringstream out_file;
        arc1.compress(in_file, zipped_file);
        arc2.extract(zipped_file, out_file);
        CHECK_EQ("oyaoyaoya", out_file.str());
    }
    SUBCASE("big file")
    {
        HuffmanArchiver arc1;
        HuffmanArchiver arc2;
        std::stringstream in_file;
        std::stringstream zipped_file;
        std::stringstream out_file;
        for (std::size_t i = 0; i < 8000; i++)
        {
            char cur = static_cast<char>(rand() % (1 << 8));
            in_file.write(&cur, sizeof(char));
        }
        arc1.compress(in_file, zipped_file);
        arc2.extract(zipped_file, out_file);
        CHECK_EQ(in_file.str(), out_file.str());
    }
}