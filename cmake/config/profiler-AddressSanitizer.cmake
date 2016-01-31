# Add options for clang LeakSanitizer
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -fno-inline-functions -fno-inline")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer -fno-inline-functions -fno-inline")

 #-fsanitize=memory -fno-optimize-sibling-calls -fsanitize-memory-track-origins=2