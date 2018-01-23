SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/libs)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

FIND_PACKAGE(MySQL)

IF(MYSQL_FOUND)
    INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})
ENDIF(MYSQL_FOUND)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/inc)
INCLUDE_DIRECTORIES(/usr/local/include/hiredis)
INCLUDE_DIRECTORIES(/usr/include/hiredis)
INCLUDE_DIRECTORIES(/usr/include/mysql)

SET(LIBS ${PROJECT_SOURCE_DIR}/libs)

SET(LIBCJSON_SRC ${LIBS}/cJSON.c)
ADD_LIBRARY(cjson SHARED ${LIBCJSON_SRC})

SET(LIBHIMYSQL_SRC ${LIBS}/himysql.c)
ADD_LIBRARY(himysql SHARED ${LIBHIMYSQL_SRC})

SET(LIBHASHMAP_SRC ${LIBS}/hashmap.c)
ADD_LIBRARY(hashmap SHARED ${LIBHASHMAP_SRC})


INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/inc)
INCLUDE_DIRECTORIES(/usr/local/include/hiredis)
INCLUDE_DIRECTORIES(/usr/include/hiredis)
INCLUDE_DIRECTORIES(/usr/include/mysql)

ADD_LIBRARY(lis ../libs/lis.c)
ADD_LIBRARY(lislog ../libs/lis_log.c)
ADD_LIBRARY(lisread ../libs/lis_read.c)
ADD_LIBRARY(lismatch ../libs/lis_match.c)
ADD_LIBRARY(lismarket ../libs/lis_market.c)
ADD_LIBRARY(lisleague ../libs/lis_league.c)
ADD_LIBRARY(liscache ../libs/lis_cache.c)



ADD_EXECUTABLE(test_read_file test_read_file.c)
TARGET_LINK_LIBRARIES(test_read_file lisread)

ADD_EXECUTABLE(test_match test_match.c)
TARGET_LINK_LIBRARIES(test_match lis)
TARGET_LINK_LIBRARIES(test_match lismatch)
TARGET_LINK_LIBRARIES(test_match lisread)
TARGET_LINK_LIBRARIES(test_match liscache)
TARGET_LINK_LIBRARIES(test_match cjson)
TARGET_LINK_LIBRARIES(test_match himysql)
TARGET_LINK_LIBRARIES(test_match hashmap)
TARGET_LINK_LIBRARIES(test_match hiredis)
TARGET_LINK_LIBRARIES(test_match ${MYSQL_LIBRARIES})

ADD_EXECUTABLE(test_league test_league.c)
TARGET_LINK_LIBRARIES(test_league lis)
TARGET_LINK_LIBRARIES(test_league lisleague)
TARGET_LINK_LIBRARIES(test_league lislog)
TARGET_LINK_LIBRARIES(test_league lisread)
TARGET_LINK_LIBRARIES(test_league liscache)
TARGET_LINK_LIBRARIES(test_league cjson)
TARGET_LINK_LIBRARIES(test_league himysql)
TARGET_LINK_LIBRARIES(test_league hashmap)
TARGET_LINK_LIBRARIES(test_league hiredis)
TARGET_LINK_LIBRARIES(test_league ${MYSQL_LIBRARIES})

ADD_EXECUTABLE(test_lis test_lis.c)
TARGET_LINK_LIBRARIES(test_lis lis)
TARGET_LINK_LIBRARIES(test_lis cjson)
TARGET_LINK_LIBRARIES(test_lis himysql)
TARGET_LINK_LIBRARIES(test_lis hashmap)
TARGET_LINK_LIBRARIES(test_lis hiredis)
TARGET_LINK_LIBRARIES(test_lis ${MYSQL_LIBRARIES})

ADD_EXECUTABLE(test_log test_log.c)
TARGET_LINK_LIBRARIES(test_log lislog)

ADD_EXECUTABLE(test_market test_market.c)
TARGET_LINK_LIBRARIES(test_market lis)
TARGET_LINK_LIBRARIES(test_market lismarket)
TARGET_LINK_LIBRARIES(test_market lislog)
TARGET_LINK_LIBRARIES(test_market lisread)
TARGET_LINK_LIBRARIES(test_market liscache)
TARGET_LINK_LIBRARIES(test_market cjson)
TARGET_LINK_LIBRARIES(test_market himysql)
TARGET_LINK_LIBRARIES(test_market hashmap)
TARGET_LINK_LIBRARIES(test_market hiredis)
TARGET_LINK_LIBRARIES(test_market ${MYSQL_LIBRARIES})