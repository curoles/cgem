TEST_DIR := $(BUILD_DIR)/test/C/Trie
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp trie.h $(INSTALL_INCLUDE_DIR)/C/trie.h
	mkdir -p $(TEST_DIR)
	$(CC) $(CFLAGS) -c trie.c -o $(BUILD_DIR)/obj/c/trie.c.o
	$(AR) -rs $(CGEM_LIB) $(BUILD_DIR)/obj/c/trie.c.o
	$(CC) $(CFLAGS) -L$(INSTALL_LIB_DIR) test.c -lcgem -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)

