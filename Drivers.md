# Introduction #

Cake is a modular monolithic kernel, and as such needs a way for drivers to be hot-plugged into the kernel.

Several of the drivers need to be available at boot-time however, in order to load the other drivers. Drivers should be available for download, where they can be placed in a folder in the .img or .iso file, or where they can be loaded during run time.

The drivers that need to be available as GRUB modules (from boot) are:
  * (S)ATA
  * Floppy Driver Controller (For legacy)
  * CD/DVD (ATAPI)
  * FAT12 FS
  * RAM FS

The other drivers can be loaded from the boot medium in a /sys/drv folder.


# Driver Interface #

Drivers need to comply to a common interface.

Once loaded they will call a syscall with a pointer to the driver structure in memory. This structure has the locations of specific driver functions.

Required functions are:
  * IOCTL
  * INIT
  * DEINIT
  * OPEN
  * CLOSE

Optional functions are:
  * READ
  * WRITE

Drivers will be compiled as an ELF .SO, so that the functions may be called externally.

# Graphical Example #

![http://cakeos.googlecode.com/files/drivers%20interface.png](http://cakeos.googlecode.com/files/drivers%20interface.png)


---

code i might need for ideas:

---


enum devtype { Block, Char };

typedef struct device\_ops {
> int		 (**open)	(struct device**device, int flags);
> int		 (**close)	(struct device**device);
> ssize\_t		 (**read)	(struct device**device, void **buf, size\_t count, off\_t offset);
> ssize\_t		 (**write)	(struct device **device, const void**buf, size\_t count, off\_t offset);
> int		 (**stat)	(struct device**device, struct stat **statbuf);
> struct vm\_page**(**mmap)	(struct device**device, off\_t offset);
> int		 (**ioctl)	(struct device**device, int request, char **argp);
} device\_ops\_t;**

/**Defines a device**/
typedef struct device {
> list\_t header;
> const char **name;**

> refcount\_t count;
> enum devtype type;

> device\_ops\_t **ops;
> void**data;
} device\_t;