TEST_DIR := $(BUILD_DIR)/test/C/Vector
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp vector.h $(INSTALL_INCLUDE_DIR)/C/vector.h
	mkdir -p $(TEST_DIR)
	$(CC) $(CFLAGS) test.c -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)

