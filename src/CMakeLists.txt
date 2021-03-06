include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})

# add Qt modules here, the include will setup QT_LIBRARIES
set(QT_USE_QT TRUE)
set(QT_USE_QTGUI TRUE)
#set(QT_USE_QTNETWORK TRUE)
#set(QT_USE_QTWEBKIT TRUE)

message(STATUS "Use file is: ${QT_USE_FILE}")
include(${QT_USE_FILE})

# hardcode path to mojodb libraries

#add_definitions(-DMOJ_LINUX -DMOJ_INTERNAL -D_REENTRANT)

# .cxx sources
set(SRCS_CXX
	MojClientHandler.cpp
	MojAppMain.cpp
	error.cpp
	mainwindow.cpp
	main.cpp
	KindsModel.cpp
)

# files which need to be moc'd by Qt
set(MOC_SRCS
    #	settings.h
    #	networkmanager.h
	mainwindow.h
	MojWorker.h
	KindsModel.h
)


set(RCCS mojoview.qrc)

set(UI
	settings.ui
)

# build ui_XXX files from the XML-style .ui files
QT4_WRAP_UI(SRCS_CXX ${UI})

# this moc's the above variable and appends to the cxx sources
QT4_WRAP_CPP(SRCS_CXX ${MOC_SRCS})

# generate rules for building source files from the resources
QT4_ADD_RESOURCES(RCC_SRCS ${RCCS})

add_executable(${PROJECT_NAME} ${SRCS_CXX} ${RCC_SRCS} ${SRCS_HEADERS})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${LIBS})
