TEST_DIR := $(BUILD_DIR)/test/File
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp File.h $(INSTALL_INCLUDE_DIR)/File.h
	mkdir -p $(TEST_DIR)
	$(CXX) $(CFLAGS) test.cpp -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)
