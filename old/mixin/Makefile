TEST_DIR := $(BUILD_DIR)/test/mixin
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp mixin.h $(INSTALL_INCLUDE_DIR)/mixin.h
	mkdir -p $(TEST_DIR)
	$(CXX) $(CFLAGS) test.cpp -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)
