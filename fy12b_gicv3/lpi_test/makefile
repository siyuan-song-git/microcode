TARGET=$(n)
C_SRC= main.o
S_SRC= asm.o exceptions.o
CROSS_TOOLCHAIN=aarch64-linux-gnu-
CC=$(CROSS_TOOLCHAIN)gcc
LD=$(CROSS_TOOLCHAIN)ld
OC=$(CROSS_TOOLCHAIN)objcopy
BH=$(CROSS_TOOLCHAIN)bin2hex

.PHONY : all
all: $(TARGET).axf
 
$(TARGET).axf: $(C_SRC) $(S_SRC)
	$(LD) --script=./link.lds $(S_SRC) $(C_SRC) -o ./$@ -Map=./$(TARGET).map
$(C_SRC): %.o: %.c
	$(CC) -c -g $< -o $@
$(S_SRC): %.o: %.S
	$(CC) -c -g $< -o $@

.PHONY : clean
clean:
	@rm -rf ./*.o
	@rm -rf ./*.axf
	@rm -rf ./*.map
