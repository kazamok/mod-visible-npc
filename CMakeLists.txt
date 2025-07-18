set(MODULE_NAME "mod-visible-npc")
set(MODULE_SRCS
    src/mod_visible_npc.cpp
)

AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/mod_visible_npc.cpp")

# 설정 파일 설치
install(FILES
    conf/mod_visible_npc.conf.dist
    DESTINATION "${CMAKE_INSTALL_PREFIX}/conf"
)