set Project     "svm_predict_test.prj"
set Solution    solution1
set Device      "xcu50-fsvh2104-2-e"
set Flow        "vivado"
set Clock       10

open_project $Project -reset

set_top dut

add_files "top.cpp" -cflags "-I./"
add_files -tb "test.cpp 1000.csv" -cflags "-I./"

open_solution -flow_target $Flow -reset $Solution
set_part $Device
create_clock -period $Clock

csim_design
csynth_design
cosim_design

exit