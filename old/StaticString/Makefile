TEST_DIR := $(BUILD_DIR)/test/StaticString
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp StaticString.h $(INSTALL_INCLUDE_DIR)/StaticString.h
	mkdir -p $(TEST_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS) test.cpp -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)
