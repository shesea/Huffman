all: hw_03 test_hw_03

hw_03: obj/Bitstream.o obj/CLI.o obj/Huffman.o obj/HuffmanArchiver.o obj/main.o
	gcc obj/Bitstream.o obj/CLI.o obj/Huffman.o obj/HuffmanArchiver.o obj/main.o -o hw_03 -lstdc++

test_hw_03: obj/Bitstream.o obj/CLI.o obj/Huffman.o obj/HuffmanArchiver.o obj/TestMain.o
	gcc obj/Bitstream.o obj/CLI.o obj/Huffman.o obj/HuffmanArchiver.o obj/TestMain.o -o test_hw_03 -lstdc++

obj/TestMain.o: test/TestMain.cpp | obj
	gcc -Iinclude -Wall -Wextra -Werror -c test/TestMain.cpp -o obj/TestMain.o

obj/Bitstream.o: src/Bitstream.cpp include/Bitstream.h |obj
	gcc -Iinclude -Wall -Wextra -Werror -c src/Bitstream.cpp -o obj/Bitstream.o

obj/CLI.o: src/CLI.cpp include/CLI.h |obj
	gcc -Iinclude -Wall -Wextra -Werror -c src/CLI.cpp -o obj/CLI.o

obj/Huffman.o: src/Huffman.cpp include/Huffman.h |obj
	gcc -Iinclude -Wall -Wextra -Werror -c src/Huffman.cpp -o obj/Huffman.o

obj/HuffmanArchiver.o: src/HuffmanArchiver.cpp include/HuffmanArchiver.h |obj
	gcc -Iinclude -Wall -Wextra -Werror -c src/HuffmanArchiver.cpp -o obj/HuffmanArchiver.o

obj/main.o: src/main.cpp |obj
	gcc -Iinclude -Wall -Wextra -Werror -c src/main.cpp -o obj/main.o

obj:
	mkdir obj
clean:
	rm -rf hw_03 obj test_hw_03
