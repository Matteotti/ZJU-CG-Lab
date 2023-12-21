add_library(glad glad/include/glad/glad.h glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)
set_target_properties(glad PROPERTIES FOLDER ${third_party_folder}/glad)
