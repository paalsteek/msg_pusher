# -*- mode: cmake; -*-
set(msg_pusher_src main.cpp msg_pusher.cpp)
link_directories(${SQLITE3_LIBRARY_DIRS})

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${SQLITE3_INCLUDE_DIR}
                    ${CURL_INCLUDE_DIR}
                    ${LIBKLIO_INCLUDE_DIR}
                    ${HXB_INCLUDE_DIR}
)

add_executable(msg_pusher ${msg_pusher_src})
target_link_libraries(msg_pusher
                      ${LIBKLIO_LIBRARY}
                      ${HXB_LIBRARIES}
)

# sensor programs to the install target
INSTALL(PROGRAMS 
  ${CMAKE_CURRENT_BINARY_DIR}/msg_pusher
  DESTINATION bin)
