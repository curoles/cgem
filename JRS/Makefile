TEST_DIR := $(BUILD_DIR)/test/JRS
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp jrs.h $(INSTALL_INCLUDE_DIR)/jrs.h
	mkdir -p $(TEST_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c jrs.cpp -o $(BUILD_DIR)/obj/cpp/jrs.cpp.o
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c jrs2txt.cpp -o $(BUILD_DIR)/obj/cpp/jrs2txt.cpp.o
	$(AR) -rs $(XGEM_LIB) $(BUILD_DIR)/obj/cpp/jrs.cpp.o $(BUILD_DIR)/obj/cpp/jrs2txt.cpp.o
	$(CXX) $(CFLAGS) $(CXXFLAGS) -L$(INSTALL_LIB_DIR) test.cpp -lxgem -o $(TEST_PRG)
	cd $(TEST_DIR); $(TEST_PRG)
