OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)
SECTIONS
{
	. = 0x100000;
	__TEXT_START = .;
	. = ALIGN(8);
	.text :
	{
		*(.text.first)
		*(.text)
	}
	. = ALIGN(8);
	.rodata :
	{ 
		*(.rodata)
	}
	. = ALIGN(8);
	.data :
	{ 
		*(.data)
	} 
	. = ALIGN(8);
	_bss_start = .;
	.bss :
	{
		*(.bss)
	}
	. = ALIGN(8);
	_bss_end = .;
}
