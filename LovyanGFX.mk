
# source files
SRCS += $(wildcard $(LGFX_DIR)/src/lgfx/Fonts/efont/*.c)
SRCS += $(wildcard $(LGFX_DIR)/src/lgfx/Fonts/IPA/*.c)
SRCS += $(wildcard $(LGFX_DIR)/src/lgfx/utility/*.c)
SRCS += $(wildcard $(LGFX_DIR)/src/lgfx/v1/*.cpp)
SRCS += $(wildcard $(LGFX_DIR)/src/lgfx/v1/misc/*.cpp)
SRCS += $(wildcard $(LGFX_DIR)/src/lgfx/v1/panel/*.cpp)
SRCS += $(wildcard $(LGFX_DIR)/src/lgfx/v1/touch/*.cpp)
SRCS += $(wildcard $(LGFX_DIR)/src/lgfx/v1/platforms/sapphire/*.cpp)

# include path
CFLAGS += -I$(LGFX_DIR)/src
