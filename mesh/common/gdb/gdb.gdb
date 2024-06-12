# common/gdb/gdb.gdb
source common/gdb/common.gdb
source .gdb/target
monitor reset halt
source .gdb/load
monitor reset init
