cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake)


#Change to your path
SET(OpenCV_DIR "O:/OpenCV/OpenCV-2.4.11_vs14_x64/build" CACHE PATH "OpenCV directory")


set(TARGET_NAME LabelTool)
project(${TARGET_NAME})
message(STATUS "AT " ${TARGET_NAME})

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui PrintSupport REQUIRED)
set(QT_MODULES Qt5::Core Qt5::Widgets Qt5::Gui Qt5::PrintSupport)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)



find_package(OpenCV REQUIRED core imgproc highgui video)

# Include files
FILE( GLOB SOURCE_FILES *.cpp *.h *.ui) 

add_executable(${TARGET_NAME} ${SOURCE_FILES})

# Organize target into a group in Visual Studio IDE
IF (DEFINED TARGET_FOLDER)
	set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER ${TARGET_FOLDER})
ENDIF()


target_link_libraries(${TARGET_NAME} PUBLIC ${QT_MODULES})
target_link_libraries(${TARGET_NAME} PUBLIC ${OpenCV_LIBS})



