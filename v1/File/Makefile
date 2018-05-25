TEST_DIR := $(BUILD_DIR)/test/File
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp File.h $(INSTALL_INCLUDE_DIR)/File.h
	cp Dir.h $(INSTALL_INCLUDE_DIR)/Dir.h
	mkdir -p $(TEST_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c Dir.cpp -o $(BUILD_DIR)/obj/cpp/Dir.cpp.o
	$(AR) -rs $(XGEM_LIB) $(BUILD_DIR)/obj/cpp/Dir.cpp.o
	$(CXX) $(CFLAGS) $(CXXFLAGS) test.cpp -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)
