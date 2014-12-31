AUX_SOURCE_DIRECTORY(. SERVER_SRCS)
ADD_EXECUTABLE(interface ${SERVER_SRCS})

TARGET_LINK_LIBRARIES(
    interface
    common protocol
    liblog4cplus.a
    pthread rt
    /usr/local/lib/libprotobuf.a
)
