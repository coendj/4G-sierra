# common/gdb/ci.gdb
source common/gdb/common.gdb
set remotetimeout 30
source .gdb/target
monitor reset halt
source .gdb/load
monitor reset run
