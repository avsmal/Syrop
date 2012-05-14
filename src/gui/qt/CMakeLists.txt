project( SyropGui )

set(CMAKE_BUILD_TYPE Release )
set(SYROP_LIBRARIES syroputils)

add_definitions ( -Wall )
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )
include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}
	../../includes)
set ( SyropGui_HDRS)

set ( SyropGui_SRCS
	main.cpp
	mainwindow.cpp
	dialogaddnetwork.cpp
	applicationsettingstab.cpp
	dialogabout.cpp
	datamodel.cpp
	dialogaddapp.cpp
	)

set ( SyropGui_UIS
	mainwindow.ui
	dialogaddnetwork.ui
	applicationsettingstab.ui
	dialogabout.ui
	dialogaddapp.ui
	)
QT4_WRAP_UI(UIS ${SyropGui_UIS})

set ( SyropGui_MOCS
	mainwindow.h
	dialogaddnetwork.h
	applicationsettingstab.h
	datamodel.h
	dialogabout.h
	dialogaddapp.h
	)
QT4_WRAP_CPP(MOCS ${SyropGui_MOCS})

add_executable ( SyropGui ${SyropGui_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( SyropGui  
    ${QT_QTCORE_LIBRARY} 
    ${QT_QTGUI_LIBRARY} 
    syroputils   
)
