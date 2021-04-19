add_executable(mymclient mymclient.c)
target_link_libraries(mymclient mymhorse)

add_executable(mymserver mymserver.c)
target_link_libraries(mymserver mymhorse)
