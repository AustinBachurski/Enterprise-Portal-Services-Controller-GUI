cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 20)
project("PortalServicesController")

find_package(wxWidgets REQUIRED COMPONENTS richtext aui adv html core xml net base)

include(${wxWidgets_USE_FILE})

include_directories(src)

add_executable(${PROJECT_NAME}
	src/Application.cpp
	src/AboutWindow.cpp
	src/Configuration.cpp
	src/CustomRichTextCtrl.cpp
	src/EnterCredentials.cpp
	src/PortalServerControls.cpp
)
	
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
			 GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba)
			 # The commit hash for 1.10.x. 
			 # Replace with the latest from: https://github.com/libcpr/cpr/releases 
FetchContent_MakeAvailable(cpr)

target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr ${wxWidgets_LIBRARIES} ${wxWidgets_RICHTEXT_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE YES)
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
