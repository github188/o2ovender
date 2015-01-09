AUX_SOURCE_DIRECTORY(. SERVER_SRCS)
ADD_EXECUTABLE(interface ${SERVER_SRCS})

TARGET_LINK_LIBRARIES(
    interface
    common mobile avm protocol
    pthread rt crypto
    /usr/local/lib/libprotobuf.a liblog4cplus.a
    mongoclient
    boost_thread
    boost_filesystem
    boost_system
    boost_regex-mt
    hiredis
    curl
)
