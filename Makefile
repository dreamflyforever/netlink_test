obj-m += driver.o

KID := /lib/modules/`uname -r`/build
PWD := $(shell pwd)

all:
	make -C $(KID) M=$(PWD) modules -Werror
	gcc app.c -o app -Werror

clean:
	rm -f *.o app .* modules.order Module.symvers *.ko *.mod.c; rm -rf .tmp_versions
