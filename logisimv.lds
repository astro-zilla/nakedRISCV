	/* vim: set filetype=ld : */
  PROVIDE(__heap_size    = 8M );  /* required amount of heap */
  PROVIDE( __stack_size  = 0x400 );  /* required amount of stack */

	ENTRY(_start);

  MEMORY
  {
    ROM (rx)  : ORIGIN = 0x00000000, LENGTH = 16M /* 16MB ROM */
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 16M /* 16MB RAM (could be less but using max 24bit addr line) */
		IO  (rw)  : ORIGIN = 0xfffff500, LENGTH = 256 /* 256 bytes, 64 words */
  }
  SECTIONS
  {
    .text :
    {
      *(.boot)
      *(.text .text.*)
      *(.text)
      *(.rodata*)
    } > ROM

    .data :
    {
      *(.data .data.*)
			__global_pointer$ = . + 0x800;
			*(.shdata .sdata)
    } > RAM

		.bss :
		{
			. = ALIGN(4);
			PROVIDE( __bss_start = . );
			*(.shbss .sbss)
			*(.bss .bss.*)
			*(.rela*)
			*(COMMON)
		} > RAM

    .heap :
    {
      . = ALIGN(4);
      _end = .;
      PROVIDE( _sheap = . );
      . = . + __heap_size;
      . = ALIGN(4);
      PROVIDE( _eheap = . );
    } >RAM

    .stack :
    {
      . = ALIGN(4);
	    PROVIDE( _estack = . );
      . = . + __stack_size;
      . = ALIGN(4);
      PROVIDE( _sstack = . );
    } >RAM
  }
