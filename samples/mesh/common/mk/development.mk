include common/mk/common.mk
include common/mk/gdb.mk

ifeq ($(PROJECT_PATH),)
	$(error set PROJECT_PATH in Makefile)
endif

GDB_ELF_FILE_DEBUG=$(PROJECT_PATH)/build/zephyr/zephyr.elf

gdb:    debug
	make gdb-interactive GDB_ELF_FILE=$(GDB_ELF_FILE_DEBUG)
flash:  debug
	make gdb-ci GDB_ELF_FILE=$(GDB_ELF_FILE_DEBUG)