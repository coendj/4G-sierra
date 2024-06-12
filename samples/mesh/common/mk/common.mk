# Be more quiet
ifneq ($(V),1)
Q    := @
NQMAKE := $(MAKE) --no-print-directory
MAKE := $(Q)$(MAKE) --no-print-directory
NULL := 2>/dev/null
endif

ifeq ($(ZEPHYR_PROJECT_PATH),)
	$(error set ZEPHYR_PROJECT_PATH in Makefile)
endif
ifeq ($(ZEPHYR_VERSION),)
	$(error set ZEPHYR_VERSION in )
endif

ifdef SUPPORTED_BOARD
LOAD_BOARD = -b $(SUPPORTED_BOARD)
endif

ifdef MODULE
EXTRA_OVERLAY =  -- -DDTC_OVERLAY_FILE="app.overlay app_$(MODULE).overlay" -DMODULE=$(MODULE)
endif

debug: os
	ZEPHYR_BASE=$(ZEPHYR_PROJECT_PATH)/zephyr west build -p auto $(LOAD_BOARD) $(EXTRA_OVERLAY)

os:
ifeq ($(wildcard $(ZEPHYR_PROJECT_PATH)),)
	west init -m https://github.com/zephyrproject-rtos/zephyr --mr $(ZEPHYR_VERSION) $(ZEPHYR_PROJECT_PATH)
	cd $(ZEPHYR_PROJECT_PATH) && west update
	cd $(ZEPHYR_PROJECT_PATH) && pip3 install --user -r $(ZEPHYR_PROJECT_PATH)/zephyr/scripts/requirements.txt
	cd $(ZEPHYR_PROJECT_PATH) && west zephyr-export
endif

configure:
	ZEPHYR_BASE=$(ZEPHYR_PROJECT_PATH)/zephyr west build -t menuconfig

memory: debug
	ZEPHYR_BASE=$(ZEPHYR_PROJECT_PATH)/zephyr west build -t ram_report
	ZEPHYR_BASE=$(ZEPHYR_PROJECT_PATH)/zephyr west build -t rom_report

test: flash
	pytest -v -ra test

help:
	$Qecho "clean:      Remove build files"
	$Qecho "configure:  Configure Zephyr OS (store the output in the workspace/prj.conf)"
	$Qecho "memory: Creates report of both ram and rom usage"
	$Qecho "debug:      Build the debug version of the application"
	$Qecho "flash:      Flash the debug version of the application"
	$Qecho "gdb:        Run the debug version of the application in GDB"
	$Qecho "help:       Display this help message"
	$Qecho "os:         Download Zephyr OS"
	$Qecho "test:       Runs pytest"

clean:
	ZEPHYR_BASE=$(ZEPHYR_PROJECT_PATH)/zephyr west build -t clean
