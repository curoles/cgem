TEST_DIR := $(BUILD_DIR)/test/C/Trie
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp trie.h $(INSTALL_INCLUDE_DIR)/C/trie.h
	mkdir -p $(TEST_DIR)
	$(CC) $(CFLAGS) test.c -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)

