TARGET=$(n)

ifeq ($(n), )
$(error make n=xxx)
endif
S_SRC= asm.o
CROSS_TOOLCHAIN=aarch64-linux-gnu-
CC=$(CROSS_TOOLCHAIN)gcc
LD=$(CROSS_TOOLCHAIN)ld
OC=$(CROSS_TOOLCHAIN)objcopy
BH=$(CROSS_TOOLCHAIN)bin2hex
OD=$(CROSS_TOOLCHAIN)objdump
.PHONY : all
all: $(TARGET).axf
	$(OC) -O binary -S ./$(TARGET).axf ./$(TARGET).bin
	#$(BH) ./$(TARGET).bin 64 0 ./$(TARGET).hex
	$(OD) -DS ./$(TARGET).axf > ./$(TARGET).dump
$(TARGET).axf: $(S_SRC)
	$(LD) --script=./link.lds $(S_SRC) -o ./$@ -Map=./$(TARGET).map

$(S_SRC): %.o: %.S
	$(CC) -c $< -o $@

.PHONY : dump
dump: $(TARGET).dump


.PHONY : clean
clean:
	@rm -rf ./*.o
	@rm -rf ./*.axf
	@rm -rf ./*.map
	@rm -rf ./*.bin
	@rm -rf ./*.hex
	@rm -rf ./*.dump
