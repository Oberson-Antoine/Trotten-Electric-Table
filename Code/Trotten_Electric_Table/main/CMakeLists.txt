idf_component_register(SRCS main.c wifi_app.c rgb_led.c http_server.c HCSR04.c i2c.c ssd1306.c
                    INCLUDE_DIRS "."
                    EMBED_FILES webpage/app.css webpage/app.js webpage/favicon.ico webpage/index.html webpage/jquery-3.3.1.min.js
                    )