idf_component_register(
    SRCS 
        "main.cpp"
        "camera.cpp"
        "lcd.cpp"
        #"microcv.cpp"
        "microcv2.cpp"
        "pipico.cpp"
    INCLUDE_DIRS 
        "."
        "../include"
        "./opencv"
    REQUIRES
        driver
        esp32-camera
        ssd1306
        spiffs
        fatfs
        sdmmc
        esp_timer
)
            
add_prebuilt_library(opencv_imgcodecs "opencv/libopencv_imgcodecs.a")
add_prebuilt_library(libpng "opencv/3rdparty/liblibpng.a")
add_prebuilt_library(libzlib "opencv/3rdparty/libzlib.a")
add_prebuilt_library(opencv_imgproc "opencv/libopencv_imgproc.a")
add_prebuilt_library(opencv_core "opencv/libopencv_core.a")
add_prebuilt_library(ade "opencv/libade.a")

set(IMGCODEC_DEP libpng libzlib)
target_link_libraries(opencv_imgcodecs INTERFACE ${IMGCODEC_DEP})

set(CORE_DEP libzlib)
target_link_libraries(opencv_core INTERFACE ${CORE_DEP})

set(OPENCV_DEP opencv_imgcodecs opencv_imgproc opencv_core)
target_link_libraries(${COMPONENT_LIB} ${OPENCV_DEP})
