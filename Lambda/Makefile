TEST_DIR := $(BUILD_DIR)/test/Lambda
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp Lambda.h $(INSTALL_INCLUDE_DIR)/Lambda.h
	mkdir -p $(TEST_DIR)
	$(CXX) $(CFLAGS) test.cpp -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)
