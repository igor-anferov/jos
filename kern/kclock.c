/* See COPYRIGHT for copyright information. */

#include <inc/x86.h>
#include <inc/time.h>
#include <kern/kclock.h>
#include <inc/stdio.h>

int gettime(void)
{
	nmi_disable();
	// LAB 12: your code here
    struct tm time;
    int uip, res_time[2], i;
    
    do {
        outb(IO_RTC_CMND, RTC_AREG);
        do {
            uip = inb(IO_RTC_DATA) & RTC_UPDATE_IN_PROGRESS;
        } while (uip);
        
        for (i=0; i<2; i++) {
            outb(IO_RTC_CMND, RTC_SEC);
            time.tm_sec = BCD2BIN(inb(IO_RTC_DATA));
            outb(IO_RTC_CMND, RTC_MIN);
            time.tm_min = BCD2BIN(inb(IO_RTC_DATA));
            outb(IO_RTC_CMND, RTC_HOUR);
            time.tm_hour = BCD2BIN(inb(IO_RTC_DATA));
            outb(IO_RTC_CMND, RTC_DAY);
            time.tm_mday = BCD2BIN(inb(IO_RTC_DATA));
            outb(IO_RTC_CMND, RTC_MON);
            time.tm_mon = BCD2BIN(inb(IO_RTC_DATA)) - 1;
            outb(IO_RTC_CMND, RTC_YEAR);
            time.tm_year = BCD2BIN(inb(IO_RTC_DATA));
            res_time[i] = timestamp( &time );
        }
    } while (res_time[0] != res_time[1]);

	nmi_enable();
	return res_time[0];
}

void
rtc_init(void)
{
    uint8_t reg;
	nmi_disable();
	// LAB 4: your code here
    
    outb(IO_RTC_CMND, RTC_AREG);
    reg = inb(IO_RTC_DATA);
    reg &= ~15;
    reg |= 3;
    outb(IO_RTC_DATA, reg);
    
    outb(IO_RTC_CMND, RTC_BREG);
    reg = inb(IO_RTC_DATA);
    reg |= RTC_PIE;
    outb(IO_RTC_DATA, reg);
    
	nmi_enable();
}

uint8_t
rtc_check_status(void)
{
	uint8_t status = 0;
	// LAB 4: your code here
    outb(IO_RTC_CMND, RTC_CREG);
    status = inb(IO_RTC_DATA);

	return status;
}

unsigned
mc146818_read(unsigned reg)
{
	outb(IO_RTC_CMND, reg);
	return inb(IO_RTC_DATA);
}

void
mc146818_write(unsigned reg, unsigned datum)
{
	outb(IO_RTC_CMND, reg);
	outb(IO_RTC_DATA, datum);
}

