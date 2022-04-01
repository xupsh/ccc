set Project     "color.prj"
set Solution    solution1
set Device      "xcu50-fsvh2104-2-e"
set Flow        "vivado"
set Clock       10

open_project $Project -reset

set_top dut

add_files "top.cpp" -cflags "-I./"
add_files -tb "test.cpp" -cflags "-I./"
add_files -tb "./data/data-csr-offset.mtx"
add_files -tb "./data/data-csr-indicesweights.mtx"

open_solution -flow_target $Flow -reset $Solution
set_part $Device
create_clock -period $Clock

csim_design
csynth_design
cosim_design

exit