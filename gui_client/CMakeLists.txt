set(SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
)

add_executable(weather_gui ${SOURCES})

if(Qt5_FOUND)
    target_link_libraries(weather_gui Qt5::Widgets Qt5::Network Qt5::Charts)
else()
    target_link_libraries(weather_gui Qt6::Widgets Qt6::Network Qt6::Charts)
endif()