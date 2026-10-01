# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\Husher_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Husher_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\cryptopp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\cryptopp_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\xxhash_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\xxhash_autogen.dir\\ParseCache.txt"
  "Husher_autogen"
  "cryptopp_autogen"
  "xxhash_autogen"
  )
endif()
