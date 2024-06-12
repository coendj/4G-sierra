# common/gdb/common.gdb
set width 0
set height 0
set verbose off

# Turn of prompt only for quit command and not for breakpoints
define hook-quit
    set confirm off
end
