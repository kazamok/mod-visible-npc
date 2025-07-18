# 모듈 이름 정의
set(MODULE_NAME "mod-visible-npc")

# 소스 파일 지정
set(MODULE_SRCS
    src/mod_visible_npc.cpp
)

# 빌드시 스크립트 추가
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/mod_visible_npc.cpp")

# 설정 파일 설치 (conf 디렉토리에 복사됨)
install(FILES
    conf/mod_visible_npc.conf.dist
    DESTINATION "${CMAKE_INSTALL_PREFIX}/conf"
)
