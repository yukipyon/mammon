cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra")
set(CMAKE_CXX_COMPILER_ID "Clang")
project(mammon)

add_executable(
    json_test
    main.cpp
    chan_post.cpp
    fourchan_parser_json.cpp)

#add_executable(
#    fourchan_parser_json_ut
#    unit_test/fourchan_parser_json_ut.cpp
#    unit_test/common_ut.cpp
#    chan_post.cpp
#    fourchan_parser_json.cpp)

add_executable(
    ext_ut
    unit_test/ext_test.cpp
    ext.cpp
    )
target_link_libraries(ext_ut chibi-scheme)

add_executable(
    chan_db_ut
    unit_test/chan_db_ut.cpp
    unit_test/common_ut.cpp
    chan_db.cpp
    ../base_db.cpp
    chan_post.cpp
    )
target_link_libraries(chan_db_ut sqlite3)
#
#add_executable(
#    kon_ut
#    unit_test/kon_ut.cpp
#    kyukon/kon.cpp
#    kyukon/task.cpp
#    )
#target_link_libraries(kon_ut curl)
#
##add_executable(
##    fourchan_proc_ut
##    fourchan_proc.cpp
##    unit_test/fourchan_proc_ut.cpp
##    )
#
#add_executable(
#    mkgold
#    unit_test/mkgold.cpp
#    unit_test/common_ut.cpp
#    chan_post.cpp
#    fourchan_parser_json.cpp)

add_executable(
    filesystem_ut
    ../filesystem.cpp
    unit_test/filesystem_ut.cpp
    )

add_executable(
    mammon
    mammon.cpp
    chan_proc.cpp
    score.cpp
    ext.cpp
    hash.cpp
    fourchan_proc.cpp
    eightchan_proc.cpp
    kyukon/kon.cpp
    kyukon/task.cpp
    chan_db.cpp
    ../base_db.cpp
    ../filesystem.cpp
    fourchan_parser_json.cpp
    chan_post.cpp
    fourchan_parser_json.cpp)
target_link_libraries(mammon sqlite3)
target_link_libraries(mammon curl)
target_link_libraries(mammon crypto)
target_link_libraries(mammon chibi-scheme)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
endif(DOXYGEN_FOUND)

