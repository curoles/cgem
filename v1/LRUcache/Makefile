TEST_DIR := $(BUILD_DIR)/test/LRUcache
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp LRUcache.h $(INSTALL_INCLUDE_DIR)/LRUcache.h
	mkdir -p $(TEST_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS) test.cpp -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)
