idf_component_register(
    SRCS 
        "main.cpp"
        "camera.cpp"
    INCLUDE_DIRS 
        "."
        "../include"
    REQUIRES
        driver
        esp32-camera
        ssd1306
        spiffs

)