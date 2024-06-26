ifeq ($(ZEPHYR_TOOLCHAIN_PREFIX),)
	$(error set ZEPHYR_TOOLCHAIN_PREFIX)
endif

GDB ?= $(ZEPHYR_TOOLCHAIN_PREFIX)/bin/arm-zephyr-eabi-gdb
COMMON_DIR?=common

ifeq ($(port),)
else
  GDB_TARGET_REMOTE='host.docker.internal:$(port)'
endif


gdb-interactive: .gdb
	$(GDB) --command=$(COMMON_DIR)/gdb/gdb.gdb

gdb-ci: .gdb
	$(GDB) --batch --command=$(COMMON_DIR)/gdb/ci.gdb

.gdb:
ifeq ($(GDB_TARGET_REMOTE),)
	$(error set GDB_TARGET_REMOTE to host:port of remote)
endif
ifeq ($(GDB_ELF_FILE),)
	$(error set GDB_ELF_FILE to the elf-file which must be debugged)
endif
	@rm -Rf $@
	@mkdir $@
	@echo 'Auto generated by $(COMMON_DIR)/mk/gdb.mk at' $(shell date) > $@/DONOTEDIT.txt
	@echo 'target extended-remote' $(GDB_TARGET_REMOTE) > $@/target
	@echo 'monitor arm semihosting enable'    >> $@/target
	@echo 'file' $(GDB_ELF_FILE) > $@/load
	@echo 'load' $(GDB_ELF_FILE) >> $@/load


.PHONY: .gdb gdb gdb-ci
