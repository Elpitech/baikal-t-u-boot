#ifndef _LINUX_COMPAT_H_
#define _LINUX_COMPAT_H_

#include <malloc.h>
#include <linux/types.h>
#include <linux/err.h>

struct unused {};
typedef struct unused unused_t;

struct p_current{
       int pid;
};

extern struct p_current *current;

#define ndelay(x)	udelay(1)

#define dev_dbg(dev, fmt, args...)		\
	debug(fmt, ##args)
#define dev_vdbg(dev, fmt, args...)		\
	debug(fmt, ##args)
#define dev_info(dev, fmt, args...)		\
	printf(fmt, ##args)
#define dev_err(dev, fmt, args...)		\
	printf(fmt, ##args)
#define printk	printf
#define printk_once	printf

#define KERN_EMERG
#define KERN_ALERT
#define KERN_CRIT
#define KERN_ERR
#define KERN_WARNING
#define KERN_NOTICE
#define KERN_INFO
#define KERN_DEBUG

void *kmalloc(size_t size, int flags);
void *kzalloc(size_t size, int flags);
#define vmalloc(size)	kmalloc(size, 0)
#define __vmalloc(size, flags, pgsz)	kmalloc(size, flags)
#define kfree(ptr)	free(ptr)
#define vfree(ptr)	free(ptr)

struct kmem_cache { int sz; };

struct kmem_cache *get_mem(int element_sz);
#define kmem_cache_create(a, sz, c, d, e)	get_mem(sz)
void *kmem_cache_alloc(struct kmem_cache *obj, int flag);
#define kmem_cache_free(obj, size)	free(size)
#define kmem_cache_destroy(obj)		free(obj)

#define DECLARE_WAITQUEUE(...)	do { } while (0)
#define add_wait_queue(...)	do { } while (0)
#define remove_wait_queue(...)	do { } while (0)

#define KERNEL_VERSION(a,b,c)	(((a) << 16) + ((b) << 8) + (c))

/*
 * ..and if you can't take the strict
 * types, you can specify one yourself.
 *
 * Or not use min/max at all, of course.
 */
#define min_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })
#define max_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x > __y ? __x: __y; })

#ifndef BUG
#define BUG() do { \
	printf("U-Boot BUG at %s:%d!\n", __FILE__, __LINE__); \
} while (0)

#define BUG_ON(condition) do { if (condition) BUG(); } while(0)
#endif /* BUG */

#define WARN_ON(x) if (x) {printf("WARNING in %s line %d\n" \
				  , __FILE__, __LINE__); }

/*
 * PAGE_SHIFT determines the page size
 */
#ifdef CONFIG_PAGE_SIZE_4KB
#define PAGE_SHIFT	12
#define PAGE_SIZE	0x1000UL
#define PAGE_MASK	0xFFFFFFFFFFFF0000
#endif
#ifdef CONFIG_PAGE_SIZE_8KB
#define PAGE_SHIFT	13
#define PAGE_SIZE	0x2000UL
#define PAGE_MASK	0xFFFFFFFFFFFE000
#endif
#ifdef CONFIG_PAGE_SIZE_16KB
#define PAGE_SHIFT	14
#define PAGE_SIZE	0x4000UL
#define PAGE_MASK	0xFFFFFFFFFFFFC000
#endif
#ifdef CONFIG_PAGE_SIZE_32KB
#define PAGE_SHIFT	15
#define PAGE_SIZE	0x8000UL
#define PAGE_MASK	0xFFFFFFFFFFFF8000
#endif
#ifdef CONFIG_PAGE_SIZE_64KB
#define PAGE_SHIFT	16
#define PAGE_SIZE	0x10000UL
#define PAGE_MASK	0xFFFFFFFFFFFF0000
#endif

/**
 * upper_32_bits - return MSB bits 32-63 of a number if little endian, or
 * return MSB bits 0-31 of a number if big endian.
 * @n: the number we're accessing
 *
 * A basic shift-right of a 64- or 32-bit quantity.  Use this to suppress
 * the "right shift count >= width of type" warning when that quantity is
 * 32-bits.
 */
#define upper_32_bits(n) ((u32)(((n) >> 16) >> 16))

/**
 * lower_32_bits - return LSB bits 0-31 of a number if little endian, or
 * return LSB bits 32-63 of a number if big endian.
 * @n: the number we're accessing
 */
#define lower_32_bits(n) ((u32)(n))

/* drivers/char/random.c */
#define get_random_bytes(...)

/* idr.c */
#define GFP_ATOMIC ((gfp_t) 0)
#define GFP_KERNEL ((gfp_t) 0)
#define GFP_NOFS ((gfp_t) 0)
#define GFP_USER ((gfp_t) 0)
#define __GFP_NOWARN ((gfp_t) 0)

/* include/linux/leds.h */
struct led_trigger {};

#define DEFINE_LED_TRIGGER(x)		static struct led_trigger *x;
enum led_brightness {
	LED_OFF		= 0,
	LED_HALF	= 127,
	LED_FULL	= 255,
};

static inline void led_trigger_register_simple(const char *name,
					struct led_trigger **trigger) {}
static inline void led_trigger_unregister_simple(struct led_trigger *trigger) {}
static inline void led_trigger_event(struct led_trigger *trigger,
					enum led_brightness event) {}

/* include/linux/log2.h */
static inline int is_power_of_2(unsigned long n)
{
	return (n != 0 && ((n & (n - 1)) == 0));
}

/* uapi/linux/limits.h */
#define XATTR_LIST_MAX 65536	/* size of extended attribute namelist (64k) */

/**
 * The type used for indexing onto a disc or disc partition.
 *
 * Linux always considers sectors to be 512 bytes long independently
 * of the devices real block size.
 *
 * blkcnt_t is the type of the inode's block count.
 */
#ifdef CONFIG_LBDAF
typedef u64 sector_t;
typedef u64 blkcnt_t;
#else
typedef unsigned long sector_t;
typedef unsigned long blkcnt_t;
#endif

#define ENOTSUPP	524	/* Operation is not supported */

/* from include/linux/kernel.h */
/*
 * This looks more complex than it should be. But we need to
 * get the type for the ~ right in round_down (it needs to be
 * as wide as the result!), and we want to evaluate the macro
 * arguments just once each.
 */
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))

/* module */
#define THIS_MODULE		0
#define try_module_get(...)	1
#define module_put(...)		do { } while (0)
#define module_init(...)
#define module_exit(...)
#define EXPORT_SYMBOL(...)
#define EXPORT_SYMBOL_GPL(...)
#define module_param(...)
#define module_param_call(...)
#define MODULE_PARM_DESC(...)
#define MODULE_VERSION(...)
#define MODULE_DESCRIPTION(...)
#define MODULE_AUTHOR(...)
#define MODULE_LICENSE(...)
#define MODULE_ALIAS(...)
#define __module_get(...)

/* character device */
#define MKDEV(...)			0
#define MAJOR(dev)			0
#define MINOR(dev)			0

#define alloc_chrdev_region(...)	0
#define unregister_chrdev_region(...)

#define class_create(...)		__builtin_return_address(0)
#define class_create_file(...)		0
#define class_remove_file(...)
#define class_destroy(...)
#define misc_register(...)		0
#define misc_deregister(...)

#define blocking_notifier_call_chain(...) 0

/*
 * Multiplies an integer by a fraction, while avoiding unnecessary
 * overflow or loss of precision.
 */
#define mult_frac(x, numer, denom)(			\
{							\
	typeof(x) quot = (x) / (denom);			\
	typeof(x) rem  = (x) % (denom);			\
	(quot * (numer)) + ((rem * (numer)) / (denom));	\
}							\
)

#define __initdata
#define late_initcall(...)

#define dev_set_name(...)		do { } while (0)
#define device_register(...)		0
#define volume_sysfs_init(...)		0
#define volume_sysfs_close(...)		do { } while (0)

#define init_waitqueue_head(...)	do { } while (0)
#define wait_event_interruptible(...)	0
#define wake_up_interruptible(...)	do { } while (0)
#define print_hex_dump(...)		do { } while (0)
#define dump_stack(...)			do { } while (0)

#define task_pid_nr(x)			0
#define set_freezable(...)		do { } while (0)
#define try_to_freeze(...)		0
#define set_current_state(...)		do { } while (0)
#define kthread_should_stop(...)	0
#define schedule()			do { } while (0)

#define setup_timer(timer, func, data) do {} while (0)
#define del_timer_sync(timer) do {} while (0)
#define schedule_work(work) do {} while (0)
#define INIT_WORK(work, fun) do {} while (0)

struct work_struct {};

unsigned long copy_from_user(void *dest, const void *src,
			     unsigned long count);

void *vzalloc(unsigned long size);

typedef unused_t spinlock_t;
typedef int	wait_queue_head_t;

#define spin_lock_init(lock) do {} while (0)
#define spin_lock(lock) do {} while (0)
#define spin_unlock(lock) do {} while (0)
#define spin_lock_irqsave(lock, flags) do { debug("%lu\n", flags); } while (0)
#define spin_unlock_irqrestore(lock, flags) do { flags = 0; } while (0)

#define DEFINE_MUTEX(...)
#define mutex_init(...)
#define mutex_lock(...)
#define mutex_unlock(...)

#define init_rwsem(...)			do { } while (0)
#define down_read(...)			do { } while (0)
#define down_write(...)			do { } while (0)
#define down_write_trylock(...)		1
#define up_read(...)			do { } while (0)
#define up_write(...)			do { } while (0)

#define cond_resched()			do { } while (0)
#define yield()				do { } while (0)

#define INT_MAX				((int)(~0U>>1))

#define __user
#define __init
#define __exit
#define __devinit
#define __devinitdata
#define __devinitconst
#define __iomem

#define kthread_create(...)	__builtin_return_address(0)
#define kthread_stop(...)	do { } while (0)
#define wake_up_process(...)	do { } while (0)

struct rw_semaphore { int i; };
#define down_write(...)			do { } while (0)
#define up_write(...)			do { } while (0)
#define down_read(...)			do { } while (0)
#define up_read(...)			do { } while (0)
struct device {
	struct device		*parent;
	struct class		*class;
	dev_t			devt;	/* dev_t, creates the sysfs "dev" */
	void	(*release)(struct device *dev);
	/* This is used from drivers/usb/musb-new subsystem only */
	void		*driver_data;	/* data private to the driver */
	void            *device_data;   /* data private to the device */
};
struct mutex { int i; };
struct kernel_param { int i; };

struct cdev {
	int owner;
	dev_t dev;
};
#define cdev_init(...)		do { } while (0)
#define cdev_add(...)		0
#define cdev_del(...)		do { } while (0)

#define MAX_ERRNO		4095

#define prandom_u32(...)	0

typedef struct {
	uid_t val;
} kuid_t;

typedef struct {
	gid_t val;
} kgid_t;

/* from include/linux/types.h */

typedef int	atomic_t;
/**
 * struct callback_head - callback structure for use with RCU and task_work
 * @next: next update requests in a list
 * @func: actual update function to call after the grace period.
 */
struct callback_head {
	struct callback_head *next;
	void (*func)(struct callback_head *head);
};
#define rcu_head callback_head
enum writeback_sync_modes {
	WB_SYNC_NONE,	/* Don't wait on anything */
	WB_SYNC_ALL,	/* Wait on every mapping */
};

/* from include/linux/writeback.h */
/*
 * A control structure which tells the writeback code what to do.  These are
 * always on the stack, and hence need no locking.  They are always initialised
 * in a manner such that unspecified fields are set to zero.
 */
struct writeback_control {
	long nr_to_write;		/* Write this many pages, and decrement
					   this for each page written */
	long pages_skipped;		/* Pages which were not written */

	/*
	 * For a_ops->writepages(): if start or end are non-zero then this is
	 * a hint that the filesystem need only write out the pages inside that
	 * byterange.  The byte at `end' is included in the writeout request.
	 */
	loff_t range_start;
	loff_t range_end;

	enum writeback_sync_modes sync_mode;

	unsigned for_kupdate:1;		/* A kupdate writeback */
	unsigned for_background:1;	/* A background writeback */
	unsigned tagged_writepages:1;	/* tag-and-write to avoid livelock */
	unsigned for_reclaim:1;		/* Invoked from the page allocator */
	unsigned range_cyclic:1;	/* range_start is cyclic */
	unsigned for_sync:1;		/* sync(2) WB_SYNC_ALL writeback */
};

void *kmemdup(const void *src, size_t len, gfp_t gfp);

typedef int irqreturn_t;

struct timer_list {};
struct notifier_block {};

typedef unsigned long dmaaddr_t;

#define cpu_relax() do {} while (0)

#define pm_runtime_get_sync(dev) do {} while (0)
#define pm_runtime_put(dev) do {} while (0)
#define pm_runtime_put_sync(dev) do {} while (0)
#define pm_runtime_use_autosuspend(dev) do {} while (0)
#define pm_runtime_set_autosuspend_delay(dev, delay) do {} while (0)
#define pm_runtime_enable(dev) do {} while (0)

#define IRQ_NONE 0
#define IRQ_HANDLED 1

#define dev_set_drvdata(dev, data) do {} while (0)

#define enable_irq(...)
#define disable_irq(...)
#define disable_irq_wake(irq) do {} while (0)
#define enable_irq_wake(irq) -EINVAL
#define free_irq(irq, data) do {} while (0)
#define request_irq(nr, f, flags, nm, data) 0

#endif
