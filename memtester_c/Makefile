TARGET=$(n)

ifeq ($(n), )
$(error make n=xxx)
endif
C_SRC= main.o memtester.o iram_test.o
S_SRC= start.o exceptions.o mmus.o 
#CROSS_TOOLCHAIN=../aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-
CROSS_TOOLCHAIN=aarch64-linux-gnu-
CC=$(CROSS_TOOLCHAIN)gcc
LD=$(CROSS_TOOLCHAIN)ld
OC=$(CROSS_TOOLCHAIN)objcopy
BH=$(CROSS_TOOLCHAIN)bin2hex
OD=$(CROSS_TOOLCHAIN)objdump
.PHONY : all
all: $(TARGET).axf
	$(OC) -O binary -S ./$(TARGET).axf ./$(TARGET).bin
	$(BH) ./$(TARGET).bin 64 0 ./$(TARGET).hex
	$(OD) -DS ./$(TARGET).axf > ./$(TARGET).dump
$(TARGET).axf: $(S_SRC) $(C_SRC)
	$(LD) --script=./link.lds $(S_SRC) $(C_SRC) -o ./$@ -Map=./$(TARGET).map

$(C_SRC): %.o: %.c
	$(CC) -c -g $< -o $@
$(S_SRC): %.o: %.S
	$(CC) -c -g $< -o $@

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
