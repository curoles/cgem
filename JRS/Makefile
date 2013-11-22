TEST_DIR := $(BUILD_DIR)/test/JRS
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp jrs.h $(INSTALL_INCLUDE_DIR)/jrs.h
	mkdir -p $(TEST_DIR)
	$(CXX) $(CFLAGS) -c jrs.cpp -o $(BUILD_DIR)/obj/cpp/jrs.cpp.o
	$(AR) -rs $(XGEM_LIB) $(BUILD_DIR)/obj/cpp/jrs.cpp.o
	#$(CXX) $(CFLAGS) -L$(INSTALL_LIB_DIR) test.cpp -lxgem -o $(TEST_PRG)
	#cd $(TEST_DIR); $(TEST_PRG)
