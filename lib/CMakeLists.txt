set(mymhorse_lib
        tcp_server.c
        tcp_client.c
        )


# 生成静态库
add_library(mymhorse ${mymhorse_lib})
# target_link_libraries(mymhorse pthread)

# 安装add_library定义的目标文件mymhorse到lib文件夹
install(TARGETS mymhorse DESTINATION lib)
