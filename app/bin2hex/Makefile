NAME=bin2hex

ifeq ($(ARCH), )
$(error make ARCH=arm/arm64/x86)
endif


ifeq ($(ARCH),arm64)
TOOLTAIN=aarch64-linux-gnu-
else

ifeq ($(ARCH),arm)
TOOLTAIN=arm-mol-linux-uclibcgnueabihf-
else

ifeq ($(ARCH),x86)
else
$(error ARCH=arm/arm64/x86 error)
endif

endif
endif

CC=$(TOOLTAIN)gcc

all:
	$(CC) -o $(NAME) $(NAME).c

clean:
	rm -rf $(NAME)
