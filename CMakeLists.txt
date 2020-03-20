cmake_minimum_required(VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE "D:/code_playground/gravitas/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_CXX_FLAGS "-std=c++17 -stdlib=libc++")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(gravitas)

set(PACKAGES_PATH "D:/code_playground/gravitas/vcpkg/packages")
set(SOURCES_PATH "src")
message(STATUS ${PACKAGES_PATH})

file(GLOB_RECURSE fileCollection "src/*.*")
add_executable(gravitas ${fileCollection})


find_package(PkgConfig)

find_package(GLEW REQUIRED)
find_package(CURL CONFIG REQUIRED)

foreach(FILE ${fileCollection}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

target_include_directories(gravitas PRIVATE 


    ${PACKAGES_PATH}/gtk_x64-windows/include
    ${PACKAGES_PATH}/atk_x64-windows/include   
    ${PACKAGES_PATH}/cairo_x64-windows/include
    ${PACKAGES_PATH}/gdk-pixbuf_x64-windows/include    
    ${PACKAGES_PATH}/gettext_x64-windows/include    
    ${PACKAGES_PATH}/glib_x64-windows/include    
    ${PACKAGES_PATH}/libepoxy_x64-windows/include  
    ${PACKAGES_PATH}/pango_x64-windows/include   

    ${PACKAGES_PATH}/freetype_x64-windows/include  
    ${PACKAGES_PATH}/glew_x64-windows/include  

    include/vendor/
    include/
    src/
    src/core
    

)

target_link_directories(gravitas PUBLIC
    ${PACKAGES_PATH}/gtk_x64-windows/lib
    ${PACKAGES_PATH}/atk_x64-windows/lib
    ${PACKAGES_PATH}/cairo_x64-windows/lib
    ${PACKAGES_PATH}/gdk-pixbuf_x64-windows/lib
    ${PACKAGES_PATH}/gettext_x64-windows/lib
    ${PACKAGES_PATH}/glib_x64-windows/lib
    ${PACKAGES_PATH}/libepoxy_x64-windows/lib
    ${PACKAGES_PATH}/pango_x64-windows/lib
    ${PACKAGES_PATH}/freetype_x64-windows/lib

)


target_link_libraries(gravitas

    
    
    GLEW::GLEW
    CURL::libcurl
    gtk-3.0.lib
    gdk-3.0.lib
    gailutil-3.0.lib
    
    atk-1.0.lib

    cairo.lib
    cairo-gobject.lib

    gdk_pixbuf-2.0.lib

    libintl.lib

    glib-2.0.lib
    gio-2.0.lib
    gmodule-2.0.lib
    gobject-2.0.lib
    gthread-2.0.lib
    
    epoxy.lib

    pango-1.0.lib
    pangocairo-1.0.lib
    pangoft2-1.0.lib
    pangowin32-1.0.lib

    freetype.lib



)



