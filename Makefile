GDB_TARGET_REMOTE='host.docker.internal:1234'
SUPPORTED_BOARD=dual_inventive_stm32f407
ZEPHYR_TOOLCHAIN_PREFIX = /root/zephyr-sdk-0.16.4/arm-zephyr-eabi
PROJECT_PATH = $(shell pwd)

ZEPHYR_VERSION = v3.5.0
ZEPHYR_PROJECT_PATH = /root/zephyr/$(ZEPHYR_VERSION)

include common/mk/development.mk