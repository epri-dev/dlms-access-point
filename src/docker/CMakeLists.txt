# configure a docker-compose file 
configure_file (
  "${CMAKE_CURRENT_LIST_DIR}/docker-compose.yml.in"
  "${PROJECT_BINARY_DIR}/docker-compose.yml"
)

configure_file (
  "${CMAKE_CURRENT_LIST_DIR}/Dockerfile.in"
  "${PROJECT_BINARY_DIR}/src/Dockerfile"
)

configure_file (
  "${CMAKE_CURRENT_LIST_DIR}/Dockerfile-doc.in"
  "${PROJECT_BINARY_DIR}/doc/html/Dockerfile"
)

configure_file (
  "${CMAKE_CURRENT_LIST_DIR}/docserver/default.conf"
  "${PROJECT_BINARY_DIR}/doc/html/default.conf"
)

configure_file (
  "${CMAKE_CURRENT_LIST_DIR}/demo/docker-entrypoint.sh"
  "${PROJECT_BINARY_DIR}/src/docker-entrypoint.sh"
  COPYONLY
)

add_custom_target(
    shared_container
    "docker-compose"
    "build"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Creating container with shared volume..." VERBATIM
)

