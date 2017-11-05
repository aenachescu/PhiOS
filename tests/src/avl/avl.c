typedef long unsigned int size_t;
typedef int wchar_t;

typedef enum {
  P_ALL,
  P_PID,
  P_PGID
} idtype_t;
typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;
typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;
typedef long int __quad_t;
typedef unsigned long int __u_quad_t;
typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef long int __off64_t;
typedef int __pid_t;
typedef struct {
  int __val[2];
} __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef int __daddr_t;
typedef int __key_t;
typedef int __clockid_t;
typedef void *__timer_t;
typedef long int __blksize_t;
typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;
typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;
typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;
typedef long int __fsword_t;
typedef long int __ssize_t;
typedef long int __syscall_slong_t;
typedef unsigned long int __syscall_ulong_t;
typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;
typedef long int __intptr_t;
typedef unsigned int __socklen_t;
static __inline unsigned int __bswap_32(unsigned int __bsx) {
  return __builtin_bswap32(__bsx);
}
static __inline __uint64_t __bswap_64(__uint64_t __bsx) {
  return __builtin_bswap64(__bsx);
}
union wait {
  int w_status;
  struct {
    unsigned int __w_termsig : 7;
    unsigned int __w_coredump : 1;
    unsigned int __w_retcode : 8;
    unsigned int : 16;
  } __wait_terminated;
  struct {
    unsigned int __w_stopval : 8;
    unsigned int __w_stopsig : 8;
    unsigned int : 16;
  } __wait_stopped;
};
typedef union {
  union wait *__uptr;
  int *__iptr;
} __WAIT_STATUS __attribute__((__transparent_union__));

typedef struct {
  int quot;
  int rem;
} div_t;
typedef struct {
  long int quot;
  long int rem;
} ldiv_t;

__extension__ typedef struct {
  long long int quot;
  long long int rem;
} lldiv_t;

extern size_t __ctype_get_mb_cur_max(void)
    __attribute__((__nothrow__, __leaf__));

extern double atof(const char *__nptr) __attribute__((__nothrow__, __leaf__))
    __attribute__((__pure__)) __attribute__((__nonnull__(1)));
extern int atoi(const char *__nptr) __attribute__((__nothrow__, __leaf__))
    __attribute__((__pure__)) __attribute__((__nonnull__(1)));
extern long int atol(const char *__nptr) __attribute__((__nothrow__, __leaf__))
    __attribute__((__pure__)) __attribute__((__nonnull__(1)));

__extension__ extern long long int atoll(const char *__nptr)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__pure__))
    __attribute__((__nonnull__(1)));

extern double strtod(const char *__restrict __nptr, char **__restrict __endptr)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));

extern float strtof(const char *__restrict __nptr, char **__restrict __endptr)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern long double strtold(const char *__restrict __nptr,
                           char **__restrict __endptr)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));

extern long int strtol(const char *__restrict __nptr,
                       char **__restrict __endptr, int __base)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern unsigned long int strtoul(const char *__restrict __nptr,
                                 char **__restrict __endptr, int __base)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));

__extension__ extern long long int
strtoq(const char *__restrict __nptr, char **__restrict __endptr, int __base)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
__extension__ extern unsigned long long int
strtouq(const char *__restrict __nptr, char **__restrict __endptr, int __base)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));

__extension__ extern long long int
strtoll(const char *__restrict __nptr, char **__restrict __endptr, int __base)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
__extension__ extern unsigned long long int
strtoull(const char *__restrict __nptr, char **__restrict __endptr, int __base)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));

extern char *l64a(long int __n) __attribute__((__nothrow__, __leaf__));
extern long int a64l(const char *__s) __attribute__((__nothrow__, __leaf__))
    __attribute__((__pure__)) __attribute__((__nonnull__(1)));

typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;
typedef __loff_t loff_t;
typedef __ino_t ino_t;
typedef __dev_t dev_t;
typedef __gid_t gid_t;
typedef __mode_t mode_t;
typedef __nlink_t nlink_t;
typedef __uid_t uid_t;
typedef __off_t off_t;
typedef __pid_t pid_t;
typedef __id_t id_t;
typedef __ssize_t ssize_t;
typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;
typedef __key_t key_t;

typedef __clock_t clock_t;

typedef __time_t time_t;

typedef __clockid_t clockid_t;
typedef __timer_t timer_t;
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
typedef int int8_t __attribute__((__mode__(__QI__)));
typedef int int16_t __attribute__((__mode__(__HI__)));
typedef int int32_t __attribute__((__mode__(__SI__)));
typedef int int64_t __attribute__((__mode__(__DI__)));
typedef unsigned int u_int8_t __attribute__((__mode__(__QI__)));
typedef unsigned int u_int16_t __attribute__((__mode__(__HI__)));
typedef unsigned int u_int32_t __attribute__((__mode__(__SI__)));
typedef unsigned int u_int64_t __attribute__((__mode__(__DI__)));
typedef int register_t __attribute__((__mode__(__word__)));
typedef int __sig_atomic_t;
typedef struct {
  unsigned long int __val[(1024 / (8 * sizeof(unsigned long int)))];
} __sigset_t;
typedef __sigset_t sigset_t;
struct timespec {
  __time_t tv_sec;
  __syscall_slong_t tv_nsec;
};
struct timeval {
  __time_t tv_sec;
  __suseconds_t tv_usec;
};
typedef __suseconds_t suseconds_t;
typedef long int __fd_mask;
typedef struct {
  __fd_mask __fds_bits[1024 / (8 * (int)sizeof(__fd_mask))];
} fd_set;
typedef __fd_mask fd_mask;

extern int select(int __nfds, fd_set *__restrict __readfds,
                  fd_set *__restrict __writefds, fd_set *__restrict __exceptfds,
                  struct timeval *__restrict __timeout);
extern int pselect(int __nfds, fd_set *__restrict __readfds,
                   fd_set *__restrict __writefds,
                   fd_set *__restrict __exceptfds,
                   const struct timespec *__restrict __timeout,
                   const __sigset_t *__restrict __sigmask);

__extension__ extern unsigned int gnu_dev_major(unsigned long long int __dev)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
__extension__ extern unsigned int gnu_dev_minor(unsigned long long int __dev)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
__extension__ extern unsigned long long int
gnu_dev_makedev(unsigned int __major, unsigned int __minor)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));

typedef __blksize_t blksize_t;
typedef __blkcnt_t blkcnt_t;
typedef __fsblkcnt_t fsblkcnt_t;
typedef __fsfilcnt_t fsfilcnt_t;
typedef unsigned long int pthread_t;
union pthread_attr_t {
  char __size[56];
  long int __align;
};
typedef union pthread_attr_t pthread_attr_t;
typedef struct __pthread_internal_list {
  struct __pthread_internal_list *__prev;
  struct __pthread_internal_list *__next;
} __pthread_list_t;
typedef union {
  struct __pthread_mutex_s {
    int __lock;
    unsigned int __count;
    int __owner;
    unsigned int __nusers;
    int __kind;
    short __spins;
    short __elision;
    __pthread_list_t __list;
  } __data;
  char __size[40];
  long int __align;
} pthread_mutex_t;
typedef union {
  char __size[4];
  int __align;
} pthread_mutexattr_t;
typedef union {
  struct {
    int __lock;
    unsigned int __futex;
    __extension__ unsigned long long int __total_seq;
    __extension__ unsigned long long int __wakeup_seq;
    __extension__ unsigned long long int __woken_seq;
    void *__mutex;
    unsigned int __nwaiters;
    unsigned int __broadcast_seq;
  } __data;
  char __size[48];
  __extension__ long long int __align;
} pthread_cond_t;
typedef union {
  char __size[4];
  int __align;
} pthread_condattr_t;
typedef unsigned int pthread_key_t;
typedef int pthread_once_t;
typedef union {
  struct {
    int __lock;
    unsigned int __nr_readers;
    unsigned int __readers_wakeup;
    unsigned int __writer_wakeup;
    unsigned int __nr_readers_queued;
    unsigned int __nr_writers_queued;
    int __writer;
    int __shared;
    unsigned long int __pad1;
    unsigned long int __pad2;
    unsigned int __flags;
  } __data;
  char __size[56];
  long int __align;
} pthread_rwlock_t;
typedef union {
  char __size[8];
  long int __align;
} pthread_rwlockattr_t;
typedef volatile int pthread_spinlock_t;
typedef union {
  char __size[32];
  long int __align;
} pthread_barrier_t;
typedef union {
  char __size[4];
  int __align;
} pthread_barrierattr_t;

extern long int random(void) __attribute__((__nothrow__, __leaf__));
extern void srandom(unsigned int __seed) __attribute__((__nothrow__, __leaf__));
extern char *initstate(unsigned int __seed, char *__statebuf, size_t __statelen)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern char *setstate(char *__statebuf) __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(1)));
struct random_data {
  int32_t *fptr;
  int32_t *rptr;
  int32_t *state;
  int rand_type;
  int rand_deg;
  int rand_sep;
  int32_t *end_ptr;
};
extern int random_r(struct random_data *__restrict __buf,
                    int32_t *__restrict __result)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int srandom_r(unsigned int __seed, struct random_data *__buf)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int initstate_r(unsigned int __seed, char *__restrict __statebuf,
                       size_t __statelen, struct random_data *__restrict __buf)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2, 4)));
extern int setstate_r(char *__restrict __statebuf,
                      struct random_data *__restrict __buf)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));

extern int rand(void) __attribute__((__nothrow__, __leaf__));
extern void srand(unsigned int __seed) __attribute__((__nothrow__, __leaf__));

extern int rand_r(unsigned int *__seed) __attribute__((__nothrow__, __leaf__));
extern double drand48(void) __attribute__((__nothrow__, __leaf__));
extern double erand48(unsigned short int __xsubi[3])
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern long int lrand48(void) __attribute__((__nothrow__, __leaf__));
extern long int nrand48(unsigned short int __xsubi[3])
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern long int mrand48(void) __attribute__((__nothrow__, __leaf__));
extern long int jrand48(unsigned short int __xsubi[3])
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern void srand48(long int __seedval) __attribute__((__nothrow__, __leaf__));
extern unsigned short int *seed48(unsigned short int __seed16v[3])
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern void lcong48(unsigned short int __param[7])
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
struct drand48_data {
  unsigned short int __x[3];
  unsigned short int __old_x[3];
  unsigned short int __c;
  unsigned short int __init;
  __extension__ unsigned long long int __a;
};
extern int drand48_r(struct drand48_data *__restrict __buffer,
                     double *__restrict __result)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int erand48_r(unsigned short int __xsubi[3],
                     struct drand48_data *__restrict __buffer,
                     double *__restrict __result)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int lrand48_r(struct drand48_data *__restrict __buffer,
                     long int *__restrict __result)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int nrand48_r(unsigned short int __xsubi[3],
                     struct drand48_data *__restrict __buffer,
                     long int *__restrict __result)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int mrand48_r(struct drand48_data *__restrict __buffer,
                     long int *__restrict __result)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int jrand48_r(unsigned short int __xsubi[3],
                     struct drand48_data *__restrict __buffer,
                     long int *__restrict __result)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int srand48_r(long int __seedval, struct drand48_data *__buffer)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int seed48_r(unsigned short int __seed16v[3],
                    struct drand48_data *__buffer)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));
extern int lcong48_r(unsigned short int __param[7],
                     struct drand48_data *__buffer)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1, 2)));

extern void *malloc(size_t __size) __attribute__((__nothrow__, __leaf__))
    __attribute__((__malloc__));
extern void *calloc(size_t __nmemb, size_t __size)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__malloc__));

extern void *realloc(void *__ptr, size_t __size)
    __attribute__((__nothrow__, __leaf__))
    __attribute__((__warn_unused_result__));
extern void free(void *__ptr) __attribute__((__nothrow__, __leaf__));

extern void cfree(void *__ptr) __attribute__((__nothrow__, __leaf__));

extern void *alloca(size_t __size) __attribute__((__nothrow__, __leaf__));

extern void *valloc(size_t __size) __attribute__((__nothrow__, __leaf__))
    __attribute__((__malloc__));
extern int posix_memalign(void **__memptr, size_t __alignment, size_t __size)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));

extern void abort(void) __attribute__((__nothrow__, __leaf__))
    __attribute__((__noreturn__));
extern int atexit(void (*__func)(void)) __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(1)));

extern int on_exit(void (*__func)(int __status, void *__arg), void *__arg)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));

extern void exit(int __status) __attribute__((__nothrow__, __leaf__))
    __attribute__((__noreturn__));

extern void _Exit(int __status) __attribute__((__nothrow__, __leaf__))
    __attribute__((__noreturn__));

extern char *getenv(const char *__name) __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(1)));

extern int putenv(char *__string) __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(1)));
extern int setenv(const char *__name, const char *__value, int __replace)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(2)));
extern int unsetenv(const char *__name) __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(1)));
extern int clearenv(void) __attribute__((__nothrow__, __leaf__));
extern char *mktemp(char *__template) __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(1)));
extern int mkstemp(char *__template) __attribute__((__nonnull__(1)));
extern int mkstemps(char *__template, int __suffixlen)
    __attribute__((__nonnull__(1)));
extern char *mkdtemp(char *__template) __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(1)));

extern int system(const char *__command);

extern char *realpath(const char *__restrict __name,
                      char *__restrict __resolved)
    __attribute__((__nothrow__, __leaf__));
typedef int (*__compar_fn_t)(const void *, const void *);

extern void *bsearch(const void *__key, const void *__base, size_t __nmemb,
                     size_t __size, __compar_fn_t __compar)
    __attribute__((__nonnull__(1, 2, 5)));
extern void qsort(void *__base, size_t __nmemb, size_t __size,
                  __compar_fn_t __compar) __attribute__((__nonnull__(1, 4)));
extern int abs(int __x) __attribute__((__nothrow__, __leaf__))
    __attribute__((__const__));
extern long int labs(long int __x) __attribute__((__nothrow__, __leaf__))
    __attribute__((__const__));

__extension__ extern long long int llabs(long long int __x)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));

extern div_t div(int __numer, int __denom)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));
extern ldiv_t ldiv(long int __numer, long int __denom)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));

__extension__ extern lldiv_t lldiv(long long int __numer, long long int __denom)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__const__));

extern char *ecvt(double __value, int __ndigit, int *__restrict __decpt,
                  int *__restrict __sign) __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(3, 4)));
extern char *fcvt(double __value, int __ndigit, int *__restrict __decpt,
                  int *__restrict __sign) __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(3, 4)));
extern char *gcvt(double __value, int __ndigit, char *__buf)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3)));
extern char *qecvt(long double __value, int __ndigit, int *__restrict __decpt,
                   int *__restrict __sign)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3, 4)));
extern char *qfcvt(long double __value, int __ndigit, int *__restrict __decpt,
                   int *__restrict __sign)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3, 4)));
extern char *qgcvt(long double __value, int __ndigit, char *__buf)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(3)));
extern int ecvt_r(double __value, int __ndigit, int *__restrict __decpt,
                  int *__restrict __sign, char *__restrict __buf, size_t __len)
    __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(3, 4, 5)));
extern int fcvt_r(double __value, int __ndigit, int *__restrict __decpt,
                  int *__restrict __sign, char *__restrict __buf, size_t __len)
    __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(3, 4, 5)));
extern int qecvt_r(long double __value, int __ndigit, int *__restrict __decpt,
                   int *__restrict __sign, char *__restrict __buf, size_t __len)
    __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(3, 4, 5)));
extern int qfcvt_r(long double __value, int __ndigit, int *__restrict __decpt,
                   int *__restrict __sign, char *__restrict __buf, size_t __len)
    __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(3, 4, 5)));

extern int mblen(const char *__s, size_t __n)
    __attribute__((__nothrow__, __leaf__));
extern int mbtowc(wchar_t *__restrict __pwc, const char *__restrict __s,
                  size_t __n) __attribute__((__nothrow__, __leaf__));
extern int wctomb(char *__s, wchar_t __wchar)
    __attribute__((__nothrow__, __leaf__));
extern size_t mbstowcs(wchar_t *__restrict __pwcs, const char *__restrict __s,
                       size_t __n) __attribute__((__nothrow__, __leaf__));
extern size_t wcstombs(char *__restrict __s, const wchar_t *__restrict __pwcs,
                       size_t __n) __attribute__((__nothrow__, __leaf__));

extern int rpmatch(const char *__response)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));
extern int getsubopt(char **__restrict __optionp,
                     char *const *__restrict __tokens,
                     char **__restrict __valuep)
    __attribute__((__nothrow__, __leaf__))
    __attribute__((__nonnull__(1, 2, 3)));
extern int getloadavg(double __loadavg[], int __nelem)
    __attribute__((__nothrow__, __leaf__)) __attribute__((__nonnull__(1)));

typedef unsigned int clib_error_code_t;
typedef unsigned char clib_bool_t;
typedef struct _Data {
  unsigned int start;
  unsigned int end;
} Data;
void AVLAllocNodeSetError();
void *AVLAllocNode(size_t a_size);
void AVLFreeNode(void *a_node);
void AVLDestroyData(Data *a_data);
void AVLCopyDataSetError();
clib_error_code_t AVLCopyData(Data *dest, const Data *src);
size_t GetMemoryInUsage();
void ResetMemoryInUsage();
size_t GetObjectsInUsage();
void ResetObjectsInUsage();
size_t GetAllocCalls();
void ResetAllocCalls();
size_t GetFreeCalls();
void ResetFreeCalls();
typedef struct UTDataAVLNode_t {
  struct UTDataAVLNode_t *left;
  struct UTDataAVLNode_t *right;
  unsigned int height;
  Data data;
} UTDataAVLNode;
typedef struct UTDataAVL_t {
  UTDataAVLNode *root;
} UTDataAVL;
typedef void (*UTDataAVLForeachCbk_t)(const Data *a_value);
typedef clib_bool_t (*UTDataAVLFindIfCbk_t)(const Data *a_value,
                                            void *a_context);
clib_error_code_t UTDataAVLNode_init(UTDataAVLNode *a_node, const Data *a_data);
clib_error_code_t UTDataAVLNode_uninit(UTDataAVLNode *a_node);
clib_error_code_t UTDataAVLNode_create(UTDataAVLNode **a_node,
                                       const Data *a_data);
clib_error_code_t UTDataAVLNode_free(UTDataAVLNode *a_node);
clib_error_code_t UTDataAVL_init(UTDataAVL *a_avl);
clib_error_code_t UTDataAVL_free(UTDataAVL *a_avl);
clib_error_code_t UTDataAVL_getHeight(const UTDataAVL *a_avl,
                                      unsigned int *a_res);
clib_error_code_t UTDataAVL_insertNode(UTDataAVL *a_avl, UTDataAVLNode *a_node);
clib_error_code_t UTDataAVL_insert(UTDataAVL *a_avl, const Data *a_value);
clib_error_code_t UTDataAVL_find(const UTDataAVL *a_avl, const Data *a_value,
                                 const UTDataAVLNode **a_res);
clib_error_code_t UTDataAVL_findType(const UTDataAVL *a_avl,
                                     const Data *a_value, const Data **a_res);
clib_error_code_t UTDataAVL_findGreaterOrEqual(const UTDataAVL *a_avl,
                                               const Data *a_value,
                                               const UTDataAVLNode **a_res);
clib_error_code_t UTDataAVL_findLessOrEqual(const UTDataAVL *a_avl,
                                            const Data *a_value,
                                            const UTDataAVLNode **a_res);
clib_error_code_t UTDataAVL_foreachInorder(const UTDataAVL *a_avl,
                                           UTDataAVLForeachCbk_t a_cbk);
clib_error_code_t UTDataAVL_foreachPreorder(const UTDataAVL *a_avl,
                                            UTDataAVLForeachCbk_t a_cbk);
clib_error_code_t UTDataAVL_foreachPostorder(const UTDataAVL *a_avl,
                                             UTDataAVLForeachCbk_t a_cbk);
clib_error_code_t UTDataAVL_isBalanced(const UTDataAVL *a_avl,
                                       clib_bool_t *a_result);
clib_error_code_t UTDataAVL_remove(UTDataAVL *a_avl, const Data *a_value,
                                   UTDataAVLNode **a_result);
clib_error_code_t UTDataAVL_findIf(const UTDataAVL *a_avl,
                                   UTDataAVLFindIfCbk_t a_cbk, void *a_context,
                                   const UTDataAVLNode **a_result);
;
static inline unsigned int
UTDataAVLNode_getHeight(const UTDataAVLNode *a_node) {
  return a_node ? a_node->height : 0;
}
static inline unsigned int
UTDataAVLNode_getMaxHeight(const UTDataAVLNode *a_node1,
                           const UTDataAVLNode *a_node2) {
  unsigned int height1 = UTDataAVLNode_getHeight(a_node1);
  unsigned int height2 = UTDataAVLNode_getHeight(a_node2);
  return height1 > height2 ? height1 : height2;
}
static inline void UTDataAVLNode_calculateHeight(UTDataAVLNode *a_node) {
  a_node->height = 1 + UTDataAVLNode_getMaxHeight(a_node->left, a_node->right);
}
static inline clib_bool_t
UTDataAVLNode_isHeightGreater(const UTDataAVLNode *a_node1,
                              const UTDataAVLNode *a_node2) {
  if (UTDataAVLNode_getHeight(a_node1) > UTDataAVLNode_getHeight(a_node2)) {
    return ((clib_bool_t)1);
  }
  return ((clib_bool_t)0);
}
static inline unsigned int UTDataAVLNode_diffHeight(UTDataAVLNode *a_node1,
                                                    UTDataAVLNode *a_node2) {
  unsigned int height1 = UTDataAVLNode_getHeight(a_node1);
  unsigned int height2 = UTDataAVLNode_getHeight(a_node2);
  if (height1 > height2) {
    return height1 - height2;
  }
  return 0;
}
static UTDataAVLNode *UTDataAVLNode_rotateLeft(UTDataAVLNode *a_parent) {
  UTDataAVLNode *newParent = a_parent->right;
  a_parent->right = newParent->left;
  newParent->left = a_parent;
  UTDataAVLNode_calculateHeight(a_parent);
  UTDataAVLNode_calculateHeight(newParent);
  return newParent;
}
static UTDataAVLNode *UTDataAVLNode_rotateRight(UTDataAVLNode *a_parent) {
  UTDataAVLNode *newParent = a_parent->left;
  a_parent->left = newParent->right;
  newParent->right = a_parent;
  UTDataAVLNode_calculateHeight(a_parent);
  UTDataAVLNode_calculateHeight(newParent);
  return newParent;
}
static UTDataAVLNode *UTDataAVLNode_balance(UTDataAVLNode *a_parent) {
  UTDataAVLNode_calculateHeight(a_parent);
  if (UTDataAVLNode_diffHeight(a_parent->left, a_parent->right) == 2) {
    if (UTDataAVLNode_isHeightGreater(a_parent->left->right,
                                      a_parent->left->left)) {
      a_parent->left = UTDataAVLNode_rotateLeft(a_parent->left);
    }
    return UTDataAVLNode_rotateRight(a_parent);
  }
  if (UTDataAVLNode_diffHeight(a_parent->right, a_parent->left) == 2) {
    if (UTDataAVLNode_isHeightGreater(a_parent->right->left,
                                      a_parent->right->right)) {
      a_parent->right = UTDataAVLNode_rotateRight(a_parent->right);
    }
    return UTDataAVLNode_rotateLeft(a_parent);
  }
  return a_parent;
}
static UTDataAVLNode *UTDataAVLNode_insert(UTDataAVLNode *a_parent,
                                           UTDataAVLNode *a_node) {
  if (a_parent == ((void *)0)) {
    return a_node;
  }
  if (((&a_node->data)->start < (&a_parent->data)->start)) {
    a_parent->left = UTDataAVLNode_insert(a_parent->left, a_node);
  } else {
    a_parent->right = UTDataAVLNode_insert(a_parent->right, a_node);
  }
  return UTDataAVLNode_balance(a_parent);
}
clib_error_code_t UTDataAVLNode_init(UTDataAVLNode *a_node,
                                     const Data *a_data) {
  if (a_node == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_data == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  a_node->left = ((void *)0);
  a_node->right = ((void *)0);
  a_node->height = 1;
  clib_error_code_t err = AVLCopyData((&a_node->data), a_data);
  if (err != ((clib_error_code_t)0)) {
    return err;
  }
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVLNode_uninit(UTDataAVLNode *a_node) {
  if (a_node == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  a_node->left = ((void *)0);
  a_node->right = ((void *)0);
  a_node->height = 0;
  AVLDestroyData((&a_node->data));
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVLNode_create(UTDataAVLNode **a_node,
                                       const Data *a_data) {
  if (a_node == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_data == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  *a_node = (UTDataAVLNode *)AVLAllocNode((sizeof(**a_node)));
  if (*a_node == ((void *)0)) {
    return ((clib_error_code_t)3);
  }
  (*a_node)->left = ((void *)0);
  (*a_node)->right = ((void *)0);
  (*a_node)->height = 1;
  clib_error_code_t err = AVLCopyData((&(*a_node)->data), a_data);
  if (err != ((clib_error_code_t)0)) {
    AVLFreeNode((*a_node));
    return err;
  }
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVLNode_free(UTDataAVLNode *a_node) {
  if (a_node == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  UTDataAVLNode_free(a_node->left);
  UTDataAVLNode_free(a_node->right);
  AVLDestroyData((&a_node->data));
  AVLFreeNode(a_node);
  return ((clib_error_code_t)0);
}
static const UTDataAVLNode *UTDataAVLNode_find(const UTDataAVLNode *a_parent,
                                               const Data *a_value) {
  if (a_parent == ((void *)0)) {
    return ((void *)0);
  }
  if ((a_value->start < (&a_parent->data)->start)) {
    return UTDataAVLNode_find(a_parent->left, a_value);
  }
  if ((a_value->start > (&a_parent->data)->start)) {
    return UTDataAVLNode_find(a_parent->right, a_value);
  }
  return a_parent;
}
static const UTDataAVLNode *
UTDataAVLNode_findGreaterOrEqual(const UTDataAVLNode *a_parent,
                                 const Data *a_value,
                                 unsigned int *a_currentScore) {
  if (((*a_currentScore) == 0)) {
    return a_parent;
  }
  if (((*a_currentScore) == 0xFFFFFFFF)) {
    if (a_parent->right == ((void *)0)) {
      return ((void *)0);
    }
    (&a_parent->right->data)->start >= a_value->start
        ? (*a_currentScore = (&a_parent->right->data)->start - a_value->start)
        : (*a_currentScore = 0xFFFFFFFF);
    return UTDataAVLNode_findGreaterOrEqual(a_parent->right, a_value,
                                            a_currentScore);
  }
  if (a_parent->left == ((void *)0)) {
    return a_parent;
  }
  unsigned int scoreLeft;
  (&a_parent->left->data)->start >= a_value->start
      ? (*(&scoreLeft) = (&a_parent->left->data)->start - a_value->start)
      : (*(&scoreLeft) = 0xFFFFFFFF);
  const UTDataAVLNode *result =
      UTDataAVLNode_findGreaterOrEqual(a_parent->left, a_value, (&scoreLeft));
  if (((*(&scoreLeft)) < (*a_currentScore))) {
    ((*a_currentScore) = (*(&scoreLeft)));
    return result;
  }
  return a_parent;
}
static const UTDataAVLNode *
UTDataAVLNode_findLessOrEqual(const UTDataAVLNode *a_parent,
                              const Data *a_value,
                              unsigned int *a_currentScore) {
  if (((*a_currentScore) == 0)) {
    return a_parent;
  }
  if (((*a_currentScore) == 0xFFFFFFFF)) {
    if (a_parent->left == ((void *)0)) {
      return ((void *)0);
    }
    (&a_parent->left->data)->start <= a_value->start
        ? (*a_currentScore = a_value->start - (&a_parent->left->data)->start)
        : (*a_currentScore = 0xFFFFFFFF);
    return UTDataAVLNode_findLessOrEqual(a_parent->left, a_value,
                                         a_currentScore);
  }
  if (a_parent->right == ((void *)0)) {
    return a_parent;
  }
  unsigned int scoreRight;
  (&a_parent->right->data)->start <= a_value->start
      ? (*(&scoreRight) = a_value->start - (&a_parent->right->data)->start)
      : (*(&scoreRight) = 0xFFFFFFFF);
  const UTDataAVLNode *result =
      UTDataAVLNode_findLessOrEqual(a_parent->right, a_value, (&scoreRight));
  if (((*(&scoreRight)) < (*a_currentScore))) {
    ((*a_currentScore) = (*(&scoreRight)));
    return result;
  }
  return a_parent;
}
static void UTDataAVLNode_foreachInorder(const UTDataAVLNode *a_parent,
                                         UTDataAVLForeachCbk_t a_cbk) {
  if (a_parent == ((void *)0)) {
    return;
  }
  UTDataAVLNode_foreachInorder(a_parent->left, a_cbk);
  a_cbk((&a_parent->data));
  UTDataAVLNode_foreachInorder(a_parent->right, a_cbk);
}
static void UTDataAVLNode_foreachPreorder(const UTDataAVLNode *a_parent,
                                          UTDataAVLForeachCbk_t a_cbk) {
  if (a_parent == ((void *)0)) {
    return;
  }
  a_cbk((&a_parent->data));
  UTDataAVLNode_foreachPreorder(a_parent->left, a_cbk);
  UTDataAVLNode_foreachPreorder(a_parent->right, a_cbk);
}
static void UTDataAVLNode_foreachPostorder(const UTDataAVLNode *a_parent,
                                           UTDataAVLForeachCbk_t a_cbk) {
  if (a_parent == ((void *)0)) {
    return;
  }
  UTDataAVLNode_foreachPostorder(a_parent->left, a_cbk);
  UTDataAVLNode_foreachPostorder(a_parent->right, a_cbk);
  a_cbk((&a_parent->data));
}
static clib_bool_t UTDataAVLNode_isBalanced(const UTDataAVLNode *a_parent) {
  if (a_parent == ((void *)0)) {
    return ((clib_bool_t)1);
  }
  if (UTDataAVLNode_diffHeight(a_parent->left, a_parent->right) > 1) {
    return ((clib_bool_t)0);
  }
  if (UTDataAVLNode_diffHeight(a_parent->right, a_parent->left) > 1) {
    return ((clib_bool_t)0);
  }
  if (UTDataAVLNode_isBalanced(a_parent->left) == ((clib_bool_t)0)) {
    return ((clib_bool_t)0);
  }
  if (UTDataAVLNode_isBalanced(a_parent->right) == ((clib_bool_t)0)) {
    return ((clib_bool_t)0);
  }
  return ((clib_bool_t)1);
}
static UTDataAVLNode *UTDataAVLNode_removeMin(UTDataAVLNode *a_parent,
                                              UTDataAVLNode **a_min) {
  if (a_parent->left == ((void *)0)) {
    *a_min = a_parent;
    return a_parent->right;
  }
  a_parent->left = UTDataAVLNode_removeMin(a_parent->left, a_min);
  return UTDataAVLNode_balance(a_parent);
}
static UTDataAVLNode *UTDataAVLNode_remove(UTDataAVLNode *a_parent,
                                           const Data *a_value,
                                           UTDataAVLNode **a_result) {
  if ((a_value->start < (&a_parent->data)->start)) {
    if (a_parent->left == ((void *)0)) {
      return a_parent;
    }
    a_parent->left = UTDataAVLNode_remove(a_parent->left, a_value, a_result);
    if (*a_result == ((void *)0)) {
      return a_parent;
    }
    return UTDataAVLNode_balance(a_parent);
  }
  if ((a_value->start > (&a_parent->data)->start)) {
    if (a_parent->right == ((void *)0)) {
      return a_parent;
    }
    a_parent->right = UTDataAVLNode_remove(a_parent->right, a_value, a_result);
    if (*a_result == ((void *)0)) {
      return a_parent;
    }
    return UTDataAVLNode_balance(a_parent);
  }
  *a_result = a_parent;
  UTDataAVLNode *auxNode;
  if (a_parent->right == ((void *)0)) {
    auxNode = a_parent->left;
    a_parent->left = ((void *)0);
    return auxNode;
  }
  UTDataAVLNode *minNode = ((void *)0);
  auxNode = UTDataAVLNode_removeMin(a_parent->right, &minNode);
  minNode->right = auxNode;
  minNode->left = a_parent->left;
  a_parent->left = ((void *)0);
  a_parent->right = ((void *)0);
  return UTDataAVLNode_balance(minNode);
}
static const UTDataAVLNode *UTDataAVLNode_findIf(const UTDataAVLNode *a_parent,
                                                 UTDataAVLFindIfCbk_t a_cbk,
                                                 void *a_context) {
  const UTDataAVLNode *result;
  if (a_parent->left != ((void *)0)) {
    result = UTDataAVLNode_findIf(a_parent->left, a_cbk, a_context);
    if (result != ((void *)0)) {
      return result;
    }
  }
  if (a_cbk(&a_parent->data, a_context) == ((clib_bool_t)1)) {
    return a_parent;
  }
  if (a_parent->right != ((void *)0)) {
    result = UTDataAVLNode_findIf(a_parent->right, a_cbk, a_context);
    if (result != ((void *)0)) {
      return result;
    }
  }
  return ((void *)0);
}
clib_error_code_t UTDataAVL_init(UTDataAVL *a_avl) {
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  a_avl->root = ((void *)0);
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_free(UTDataAVL *a_avl) {
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_avl->root != ((void *)0)) {
    UTDataAVLNode_free(a_avl->root);
    a_avl->root = ((void *)0);
  }
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_getHeight(const UTDataAVL *a_avl,
                                      unsigned int *a_res) {
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_res == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  *a_res = UTDataAVLNode_getHeight(a_avl->root);
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_insertNode(UTDataAVL *a_avl,
                                       UTDataAVLNode *a_node) {
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_node == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  a_avl->root = UTDataAVLNode_insert(a_avl->root, a_node);
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_insert(UTDataAVL *a_avl, const Data *a_value) {
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_value == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  UTDataAVLNode *node = ((void *)0);
  clib_error_code_t err = UTDataAVLNode_create(&node, a_value);
  if (err != ((clib_error_code_t)0)) {
    return err;
  }
  a_avl->root = UTDataAVLNode_insert(a_avl->root, node);
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_find(const UTDataAVL *a_avl, const Data *a_value,
                                 const UTDataAVLNode **a_res) {
  if (a_res == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  *a_res = ((void *)0);
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_value == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  *a_res = UTDataAVLNode_find(a_avl->root, a_value);
  if (*a_res == ((void *)0)) {
    return ((clib_error_code_t)2);
  }
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_findType(const UTDataAVL *a_avl,
                                     const Data *a_value, const Data **a_res) {
  if (a_res == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  *a_res = ((void *)0);
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_value == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  const UTDataAVLNode *node = UTDataAVLNode_find(a_avl->root, a_value);
  if (node == ((void *)0)) {
    return ((clib_error_code_t)2);
  }
  *a_res = &node->data;
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_findGreaterOrEqual(const UTDataAVL *a_avl,
                                               const Data *a_value,
                                               const UTDataAVLNode **a_res) {
  if (a_res == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  *a_res = ((void *)0);
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_value == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_avl->root == ((void *)0)) {
    return ((clib_error_code_t)2);
  }
  unsigned int score;
  (&a_avl->root->data)->start >= a_value->start
      ? (*(&score) = (&a_avl->root->data)->start - a_value->start)
      : (*(&score) = 0xFFFFFFFF);
  *a_res = UTDataAVLNode_findGreaterOrEqual(a_avl->root, a_value, &score);
  if (*a_res == ((void *)0)) {
    return ((clib_error_code_t)2);
  }
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_findLessOrEqual(const UTDataAVL *a_avl,
                                            const Data *a_value,
                                            const UTDataAVLNode **a_res) {
  if (a_res == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  *a_res = ((void *)0);
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_value == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_avl->root == ((void *)0)) {
    return ((clib_error_code_t)2);
  }
  unsigned int score;
  (&a_avl->root->data)->start <= a_value->start
      ? (*(&score) = a_value->start - (&a_avl->root->data)->start)
      : (*(&score) = 0xFFFFFFFF);
  *a_res = UTDataAVLNode_findLessOrEqual(a_avl->root, a_value, &score);
  if (*a_res == ((void *)0)) {
    return ((clib_error_code_t)2);
  }
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_foreachInorder(const UTDataAVL *a_avl,
                                           UTDataAVLForeachCbk_t a_cbk) {
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_cbk == ((void *)0)) {
    return ((clib_error_code_t)6);
  }
  UTDataAVLNode_foreachInorder(a_avl->root, a_cbk);
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_foreachPreorder(const UTDataAVL *a_avl,
                                            UTDataAVLForeachCbk_t a_cbk) {
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_cbk == ((void *)0)) {
    return ((clib_error_code_t)6);
  }
  UTDataAVLNode_foreachPreorder(a_avl->root, a_cbk);
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_foreachPostorder(const UTDataAVL *a_avl,
                                             UTDataAVLForeachCbk_t a_cbk) {
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_cbk == ((void *)0)) {
    return ((clib_error_code_t)6);
  }
  UTDataAVLNode_foreachPostorder(a_avl->root, a_cbk);
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_isBalanced(const UTDataAVL *a_avl,
                                       clib_bool_t *a_result) {
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_result == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  *a_result = UTDataAVLNode_isBalanced(a_avl->root);
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_remove(UTDataAVL *a_avl, const Data *a_value,
                                   UTDataAVLNode **a_result) {
  if (a_result == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  *a_result = ((void *)0);
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_value == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_avl->root == ((void *)0)) {
    return ((clib_error_code_t)2);
  }
  a_avl->root = UTDataAVLNode_remove(a_avl->root, a_value, a_result);
  if (*a_result == ((void *)0)) {
    return ((clib_error_code_t)2);
  }
  return ((clib_error_code_t)0);
}
clib_error_code_t UTDataAVL_findIf(const UTDataAVL *a_avl,
                                   UTDataAVLFindIfCbk_t a_cbk, void *a_context,
                                   const UTDataAVLNode **a_result) {
  if (a_result == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  *a_result = ((void *)0);
  if (a_avl == ((void *)0)) {
    return ((clib_error_code_t)1);
  }
  if (a_cbk == ((void *)0)) {
    return ((clib_error_code_t)6);
  }
  if (a_avl->root != ((void *)0)) {
    *a_result = UTDataAVLNode_findIf(a_avl->root, a_cbk, a_context);
    if (*a_result != ((void *)0)) {
      return ((clib_error_code_t)0);
    }
  }
  return ((clib_error_code_t)2);
};
static size_t g_memInUsage = 0;
static size_t g_objectsInUsage = 0;
static size_t g_allocCalls = 0;
static size_t g_freeCalls = 0;
static int g_allocError = 0;
static int g_copyError = 0;
void AVLAllocNodeSetError() { g_allocError = 1; }
void *AVLAllocNode(size_t a_size) {
  if (g_allocError == 1) {
    g_allocError = 0;
    return ((void *)0);
  }
  g_memInUsage += a_size;
  g_allocCalls++;
  return malloc(a_size);
}
void AVLFreeNode(void *a_node) {
  g_memInUsage -= sizeof(UTDataAVLNode);
  g_freeCalls++;
  return free(a_node);
}
void AVLDestroyData(Data *a_data) {
  g_objectsInUsage--;
  a_data->start = 0;
  a_data->end = 0;
}
void AVLCopyDataSetError() { g_copyError = 1; }
clib_error_code_t AVLCopyData(Data *dest, const Data *src) {
  if (g_copyError == 1) {
    g_copyError = 0;
    return ((clib_error_code_t)4);
  }
  g_objectsInUsage++;
  dest->start = src->start;
  dest->end = src->end;
  return ((clib_error_code_t)0);
}
size_t GetMemoryInUsage() { return g_memInUsage; }
void ResetMemoryInUsage() { g_memInUsage = 0; }
size_t GetObjectsInUsage() { return g_objectsInUsage; }
void ResetObjectsInUsage() { g_objectsInUsage = 0; }
size_t GetAllocCalls() { return g_allocCalls; }
void ResetAllocCalls() { g_allocCalls = 0; }
size_t GetFreeCalls() { return g_freeCalls; }
void ResetFreeCalls() { g_freeCalls = 0; }
