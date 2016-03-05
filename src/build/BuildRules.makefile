
$(BUILD_DIR)/%.md.html : $(SOURCE_DIR)/%.md
	mkdir -p $(@D)
	$(MARKDOWN) $< > $@

$(BUILD_DIR)/%.sv.vsim : $(SOURCE_DIR)/%.sv
	mkdir -p $(@D)
	$(VLOG) $(VLOGFLAGS) -work $(BUILD_DIR)/default_vlog_build/work $<
	touch $@

$(BUILD_DIR)/%.v.vsim : $(SOURCE_DIR)/%.v
	mkdir -p $(@D)
	$(VLOG) $(VLOGFLAGS) -work $(BUILD_DIR)/default_vlog_build/work $<
	touch $@

$(BUILD_DIR)/%.vhdl.vsim : $(SOURCE_DIR)/%.vhdl
	mkdir -p $(@D)
	$(VCOM) $(VHDLFLAGS) -work $(BUILD_DIR)/default_vlog_build/work $<
	touch $@

