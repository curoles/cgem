TEST_DIR := $(BUILD_DIR)/test/logging
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp logging.h $(INSTALL_INCLUDE_DIR)/logging.h
	mkdir -p $(TEST_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c logging.cpp -o $(BUILD_DIR)/obj/cpp/logging.cpp.o
	$(AR) -rs $(XGEM_LIB) $(BUILD_DIR)/obj/cpp/logging.cpp.o
	$(CXX) $(CFLAGS) $(CXXFLAGS) -L$(INSTALL_LIB_DIR) test.cpp -lxgem -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)
