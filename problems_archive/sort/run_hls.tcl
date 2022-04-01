set Project     "insert_sort.prj"
set Solution    solution1
set Device      "xcu50-fsvh2104-2-e"
set Flow        "vitis"
set Clock       10

open_project $Project -reset

set_top hls_db_insert_sort_function

add_files "top.cpp" -cflags "-I."
add_files -tb "insert_sort_test.cpp" -cflags "-I."

open_solution -flow_target $Flow -reset $Solution
set_part $Device
create_clock -period $Clock

csim_design
csynth_design
cosim_design

exit
