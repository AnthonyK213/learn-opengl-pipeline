aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(mygl
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    mygl.ui
  )
  # Define target properties for Android with Qt 6 as:
  #    set_property(TARGET mygl APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
  # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(mygl SHARED
      ${PROJECT_SOURCES}
    )
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(mygl
      ${PROJECT_SOURCES}
    )
  endif()

  qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_include_directories(mygl
  PUBLIC
  ${objl_SOURCE_DIR}
  )

target_link_libraries(mygl PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(mygl PROPERTIES
  MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
  )

install(TARGETS mygl
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(WIN32)
  add_custom_command(
    TARGET mygl POST_BUILD
    COMMAND $ENV{QT_DIR}/bin/windeployqt.exe
        $<TARGET_PROPERTY:mygl,BINARY_DIR>/$<CONFIG>/mygl.exe
    )
endif()

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(mygl)
endif()
