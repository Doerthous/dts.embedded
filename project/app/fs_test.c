#include <ff.h>
#include <dts/embedded/hal/uart.h>
#include <dts/embedded/hal/lib/uart_printf.h>
#include <string.h>

extern uart_t uart1;

int fs_test(void)
{
	static FATFS fs; /* Filesystem object */
    static FIL fil; /* File object */
    static BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
    FRESULT res; /* API result code */
    UINT cnt;
	BYTE mm[50];

	
	#define ff_check(cond, msg) \
		if (!(cond)) { \
			uart_printf(&uart1, msg" error\n"); \
			return 0; \
		} \
		else { \
			uart_printf(&uart1, msg" ok\n"); \
		}
	
	res = f_mount(&fs, "0:", 0);
	ff_check(res == FR_OK, "mount");
		
	res = f_mkfs("0:", NULL, work, sizeof(work)); //
	ff_check(res == FR_OK, "mkfs");

	res = f_open(&fil, "0:/a.txt", FA_CREATE_NEW|FA_WRITE|FA_READ);
	ff_check(res == FR_OK, "open");
	
	/* Write a message */
	res = f_write(&fil, "Hello,World!", 12, &cnt);
	ff_check(res == FR_OK && cnt == 12, "write");

	cnt = f_size(&fil);
	ff_check(cnt == 12, "size");

	memset(mm, 0x0, 50);
	f_lseek(&fil, 0);
	res = f_read(&fil, mm, 12, &cnt);
	ff_check(res == FR_OK && cnt == 12 && memcmp(mm, "Hello,World!", 12) == 0, "read");
	
	/* Close the file */
	f_close(&fil);

	f_mount(0, "0:", 0);
	
	return 1;
}
