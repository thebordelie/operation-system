obj-m += my_vmstat.o

build:
  make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
  make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install: build
  sudo insmod my_vmstat.ko
