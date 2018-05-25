TEST_DIR := $(BUILD_DIR)/test/Backtrace
TEST_PRG := $(TEST_DIR)/test.out

all:
	cp Backtrace.h $(INSTALL_INCLUDE_DIR)/Backtrace.h
	mkdir -p $(TEST_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c Backtrace.cpp -o $(BUILD_DIR)/obj/cpp/Backtrace.cpp.o
	$(AR) -rs $(XGEM_LIB) $(BUILD_DIR)/obj/cpp/Backtrace.cpp.o
	$(CXX) $(CFLAGS) $(CXXFLAGS) -L$(INSTALL_LIB_DIR) test.cpp -lxgem -o $(TEST_PRG) -rdynamic
	cd $(TEST_DIR); $(TEST_PRG)
