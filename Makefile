obj-m += driver.o

KID := /lib/modules/`uname -r`/build
PWD := $(shell pwd)

all:
	make -C $(KID) M=$(PWD) modules -Werror
	gcc -c app.c -o app -Werror

clean:
	rm -rf *.o .cmd *.ko *.mod.c .tmp_versions
