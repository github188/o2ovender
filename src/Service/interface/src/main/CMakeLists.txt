AUX_SOURCE_DIRECTORY(. SERVER_SRCS)
ADD_EXECUTABLE(interface ${SERVER_SRCS})

TARGET_LINK_LIBRARIES(
    interface
    common protocol
    liblog4cplus.a
    pthread rt
    /usr/local/lib/libprotobuf.a
    mongoclient
    boost_thread
    boost_filesystem
    boost_system
    boost_regex-mt
)
