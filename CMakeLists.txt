cmake_minimum_required(VERSION 3.10)
project(EasyLoginManager)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Add executable
add_executable(EasyLoginManager 
        main.cpp
        AccountDialog.cpp
        MainFrame.cpp
)

# Link wxWidgets
target_link_libraries(EasyLoginManager ${wxWidgets_LIBRARIES})

# Windows specific: remove console window
if(WIN32)
    set_target_properties(EasyLoginManager PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
