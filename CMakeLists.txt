# Make sure to have i2cdev as described in https://github.com/martinberlin/HM3301-idf/wiki
set(srcs 
    # Seeed dust sensor HHM330x (our own take from Arduino library)
    "hm3301.cpp"
)

idf_component_register(SRCS ${srcs}
                    REQUIRES driver
                    INCLUDE_DIRS "."
)