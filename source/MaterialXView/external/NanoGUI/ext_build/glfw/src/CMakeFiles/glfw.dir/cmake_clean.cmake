file(REMOVE_RECURSE
  "../../../../../../../lib/libglfw3.a"
  "../../../../../../../lib/libglfw3.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/glfw.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
