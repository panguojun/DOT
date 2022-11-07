#include "pngDec.hpp"
/* inflate.c -- zlib decompression
 * Copyright (C) 1995-2005 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

 /*
  * Change history:
  *
  * 1.2.beta0    24 Nov 2002
  * - First version -- complete rewrite of inflate to simplify code, avoid
  *   creation of window when not needed, minimize use of window when it is
  *   needed, make inffast.c even faster, implement gzip decoding, and to
  *   improve code readability and style over the previous zlib inflate code
  *
  * 1.2.beta1    25 Nov 2002
  * - Use pointers for available input and output checking in inffast.c
  * - Remove input and output counters in inffast.c
  * - Change inffast.c entry and loop from avail_in >= 7 to >= 6
  * - Remove unnecessary second byte pull from length extra in inffast.c
  * - Unroll direct copy to three copies per loop in inffast.c
  *
  * 1.2.beta2    4 Dec 2002
  * - Change external routine names to reduce potential conflicts
  * - Correct filename to inffixed.h for fixed tables in inflate.c
  * - Make hbuf[] unsigned char to match parameter type in inflate.c
  * - Change strm->next_out[-state->offset] to *(strm->next_out - state->offset)
  *   to avoid negation problem on Alphas (64 bit) in inflate.c
  *
  * 1.2.beta3    22 Dec 2002
  * - Add comments on state->bits assertion in inffast.c
  * - Add comments on op field in inftrees.h
  * - Fix bug in reuse of allocated window after inflateReset()
  * - Remove bit fields--back to byte structure for speed
  * - Remove distance extra == 0 check in inflate_fast()--only helps for lengths
  * - Change post-increments to pre-increments in inflate_fast(), PPC biased?
  * - Add compile time option, POSTINC, to use post-increments instead (Intel?)
  * - Make MATCH copy in inflate() much faster for when inflate_fast() not used
  * - Use local copies of stream next and avail values, as well as local bit
  *   buffer and bit count in inflate()--for speed when inflate_fast() not used
  *
  * 1.2.beta4    1 Jan 2003
  * - Split ptr - 257 statements in inflate_table() to avoid compiler warnings
  * - Move a comment on output buffer sizes from inffast.c to inflate.c
  * - Add comments in inffast.c to introduce the inflate_fast() routine
  * - Rearrange window copies in inflate_fast() for speed and simplification
  * - Unroll last copy for window match in inflate_fast()
  * - Use local copies of window variables in inflate_fast() for speed
  * - Pull out common write == 0 case for speed in inflate_fast()
  * - Make op and len in inflate_fast() unsigned for consistency
  * - Add to lcode and dcode declarations in inflate_fast()
  * - Simplified bad distance check in inflate_fast()
  * - Added inflateBackInit(), inflateBack(), and inflateBackEnd() in new
  *   source file infback.c to provide a call-back interface to inflate for
  *   programs like gzip and unzip -- uses window as output buffer to avoid
  *   window copying
  *
  * 1.2.beta5    1 Jan 2003
  * - Improved inflateBack() interface to allow the caller to provide initial
  *   input in strm.
  * - Fixed stored blocks bug in inflateBack()
  *
  * 1.2.beta6    4 Jan 2003
  * - Added comments in inffast.c on effectiveness of POSTINC
  * - Typecasting all around to reduce compiler warnings
  * - Changed loops from while (1) or do {} while (1) to for (;;), again to
  *   make compilers happy
  * - Changed type of window in inflateBackInit() to unsigned char *
  *
  * 1.2.beta7    27 Jan 2003
  * - Changed many types to unsigned or unsigned short to avoid warnings
  * - Added inflateCopy() function
  *
  * 1.2.0        9 Mar 2003
  * - Changed inflateBack() interface to provide separate opaque descriptors
  *   for the in() and out() functions
  * - Changed inflateBack() argument and in_func typedef to swap the length
  *   and buffer address return values for the input function
  * - Check next_in and next_out for Z_NULL on entry to inflate()
  *
  * The history for versions after 1.2.0 are in ChangeLog in zlib distribution.
  */
#define Bytef unsigned char
#define uInt unsigned int
#define uLong unsigned long
#define voidpf void*
#define Z_NULL 0
#define Z_OK            0
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define Z_ERRNO        (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define Z_BUF_ERROR    (-5)
#define Z_VERSION_ERROR (-6)
#define ENOUGH 2048
#define MAXD 592
#define ZLIB_VERSION "1.2.3"
#define DEF_WBITS 15
#define MAXBITS 15
#define Z_DEFLATED   8
#define BASE 65521UL    /* largest prime smaller than 65536 */
#define MOD4(a) a %= BASE
#define NMAX 5552
#define DO1(buf,i)  {adler += (buf)[i]; sum2 += adler;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);
#define MOD(a) a %= BASE
#define Z_NO_FLUSH      0
#define Z_PARTIAL_FLUSH 1 /* will be removed, use Z_SYNC_FLUSH instead */
#define Z_SYNC_FLUSH    2
#define Z_FULL_FLUSH    3
#define Z_FINISH        4
#define Z_BLOCK         5
#define OFF 1
#define PUP(a) *++(a)

void zmemzero(void* pMem, int data_len)
{
    int i;
    for (i = 0; i < data_len; i++)
        ((char*)pMem)[i] = 0;
}
void zmemcpy(void* p1, const void* p2, int data_len)
{
    for (int i = 0; i < data_len; i++)
        ((char*)p1)[i] = ((char*)p2)[i];
}
voidpf zcalloc(voidpf opaque, unsigned items, unsigned size)
{
    //    if (opaque) items += size - size; /* make compiler happy */
    //    return sizeof(uInt) > 2 ? (voidpf)malloc(items * size) :
    //                              (voidpf)calloc(items, size);
    return new char[items * size];
}

void  zcfree(voidpf opaque, voidpf ptr)
{
    //    free(ptr);
    //    if (opaque) return; /* make compiler happy */
    delete[] ptr;
}
#define ZALLOC(strm, items, size) \
           zcalloc((strm)->opaque, (items), (size))
#define ZFREE(strm, addr)  zcfree((strm)->opaque, (voidpf)(addr))
#define TRY_FREE(s, p) {if (p) ZFREE(s, p);}



struct internal_state { int dummy; }; /* hack for buggy compilers */
typedef struct z_stream_s {
    Bytef* next_in;  /* next input byte */
    uInt     avail_in;  /* number of bytes available at next_in */
    uLong    total_in;  /* total nb of input bytes read so */

    Bytef* next_out; /* next output byte should be put there */
    uInt     avail_out; /* remaining free space at next_out */
    uLong    total_out; /* total nb of bytes output so */

    char* msg;      /* last error message, NULL if no error */
    struct internal_state* state; /* not visible by applications */

    voidpf     opaque;  /* private data object passed to zalloc and zfree */

    int     data_type;  /* best guess about the data type: binary or text */
    uLong   adler;      /* adler32 value of the uncompressed data */
    uLong   reserved;   /* reserved for future use */
} z_stream;

typedef z_stream* z_streamp;

/* Possible inflate modes between inflate() calls */
typedef enum {
    HEAD,       /* i: waiting for magic header */
    FLAGS,      /* i: waiting for method and flags (gzip) */
    TIME,       /* i: waiting for modification time (gzip) */
    OS,         /* i: waiting for extra flags and operating system (gzip) */
    EXLEN,      /* i: waiting for extra length (gzip) */
    EXTRA,      /* i: waiting for extra bytes (gzip) */
    NAME,       /* i: waiting for end of file name (gzip) */
    COMMENT,    /* i: waiting for end of comment (gzip) */
    HCRC,       /* i: waiting for header crc (gzip) */
    DICTID,     /* i: waiting for dictionary check value */
    DICT,       /* waiting for inflateSetDictionary() call */
    TYPE,       /* i: waiting for type bits, including last-flag bit */
    TYPEDO,     /* i: same, but skip check to exit inflate on new block */
    STORED,     /* i: waiting for stored size (length and complement) */
    COPY,       /* i/o: waiting for input or output to copy stored block */
    TABLE,      /* i: waiting for dynamic block table lengths */
    LENLENS,    /* i: waiting for code length code lengths */
    CODELENS,   /* i: waiting for length/lit and distance code lengths */
    LEN,        /* i: waiting for length/lit code */
    LENEXT,     /* i: waiting for length extra bits */
    DIST,       /* i: waiting for distance code */
    DISTEXT,    /* i: waiting for distance extra bits */
    MATCH,      /* o: waiting for output space to copy string */
    LIT,        /* o: waiting for output space to write literal */
    CHECK,      /* i: waiting for 32-bit check value */
    LENGTH,     /* i: waiting for 32-bit length (gzip) */
    DONE,       /* finished check, done -- remain here until reset */
    BAD,        /* got a data error -- remain here until reset */
    MEM,        /* got an inflate() memory error -- remain here until reset */
    SYNC        /* looking for synchronization bytes to restart inflate() */
} inflate_mode;

typedef struct gz_header_s {
    int     text;       /* true if compressed data believed to be text */
    uLong   time;       /* modification time */
    int     xflags;     /* extra flags (not used when writing a gzip file) */
    int     os;         /* operating system */
    Bytef* extra;     /* pointer to extra field or Z_NULL if none */
    uInt    extra_len;  /* extra field length (valid if extra != Z_NULL) */
    uInt    extra_max;  /* space at extra (only when reading header) */
    Bytef* name;      /* pointer to zero-terminated file name or Z_NULL */
    uInt    name_max;   /* space at name (only when reading header) */
    Bytef* comment;   /* pointer to zero-terminated comment or Z_NULL */
    uInt    comm_max;   /* space at comment (only when reading header) */
    int     hcrc;       /* true if there was or will be a header crc */
    int     done;       /* true when done reading gzip header (not used
                           when writing a gzip file) */
} gz_header;

typedef gz_header* gz_headerp;

typedef struct {
    unsigned char op;           /* operation, extra bits, table bits */
    unsigned char bits;         /* bits in this part of the code */
    unsigned short val;         /* offset in table or code value */
} code;

struct inflate_state {
    inflate_mode mode;          /* current inflate mode */
    int last;                   /* true if processing last block */
    int wrap;                   /* bit 0 true for zlib, bit 1 true for gzip */
    int havedict;               /* true if dictionary provided */
    int flags;                  /* gzip header method and flags (0 if zlib) */
    unsigned dmax;              /* zlib header max distance (INFLATE_STRICT) */
    unsigned long check;        /* protected copy of check value */
    unsigned long total;        /* protected copy of output count */
    gz_headerp head;            /* where to save gzip header information */
        /* sliding window */
    unsigned wbits;             /* log base 2 of requested window size */
    unsigned wsize;             /* window size or zero if not using window */
    unsigned whave;             /* valid bytes in the window */
    unsigned write;             /* window write index */
    unsigned char* window;  /* allocated sliding window, if needed */
        /* bit accumulator */
    unsigned long hold;         /* input bit accumulator */
    unsigned bits;              /* number of bits in "in" */
        /* for string and stored block copying */
    unsigned length;            /* literal or length of data to copy */
    unsigned offset;            /* distance back to copy string from */
        /* for table and code decoding */
    unsigned extra;             /* extra bits needed */
        /* fixed and dynamic code tables */
    code const* lencode;    /* starting table for length/literal codes */
    code const* distcode;   /* starting table for distance codes */
    unsigned lenbits;           /* index bits for lencode */
    unsigned distbits;          /* index bits for distcode */
        /* dynamic table building */
    unsigned ncode;             /* number of code length code lengths */
    unsigned nlen;              /* number of length code lengths */
    unsigned ndist;             /* number of distance code lengths */
    unsigned have;              /* number of code lengths in lens[] */
    code* next;             /* next available space in codes[] */
    unsigned short lens[320];   /* temporary storage for code lengths */
    unsigned short work[288];   /* work area for code table building */
    code codes[ENOUGH];         /* space for code tables */
};
typedef enum {
    CODES,
    LENS,
    DISTS
} codetype;

void inflate_fast(z_streamp strm, unsigned start)
{
    struct inflate_state* state;
    unsigned char* in;      /* local strm->next_in */
    unsigned char* last;    /* while in < last, enough input available */
    unsigned char* out;     /* local strm->next_out */
    unsigned char* beg;     /* inflate()'s initial strm->next_out */
    unsigned char* end;     /* while out < end, enough space available */
#ifdef INFLATE_STRICT
    unsigned dmax;              /* maximum distance from zlib header */
#endif
    unsigned wsize;             /* window size or zero if not using window */
    unsigned whave;             /* valid bytes in the window */
    unsigned write;             /* window write index */
    unsigned char* window;  /* allocated sliding window, if wsize != 0 */
    unsigned long hold;         /* local strm->hold */
    unsigned bits;              /* local strm->bits */
    code const* lcode;      /* local strm->lencode */
    code const* dcode;      /* local strm->distcode */
    unsigned lmask;             /* mask for first level of length codes */
    unsigned dmask;             /* mask for first level of distance codes */
    code _this;                  /* retrieved table entry */
    unsigned op;                /* code bits, operation, extra bits, or */
                                /*  window position, window bytes to copy */
    unsigned len;               /* match length, unused bytes */
    unsigned dist;              /* match distance */
    unsigned char* from;    /* where to copy match from */

    /* copy state to local variables */
    state = (struct inflate_state*)strm->state;
    in = strm->next_in - OFF;
    last = in + (strm->avail_in - 5);
    out = strm->next_out - OFF;
    beg = out - (start - strm->avail_out);
    end = out + (strm->avail_out - 257);
#ifdef INFLATE_STRICT
    dmax = state->dmax;
#endif
    wsize = state->wsize;
    whave = state->whave;
    write = state->write;
    window = state->window;
    hold = state->hold;
    bits = state->bits;
    lcode = state->lencode;
    dcode = state->distcode;
    lmask = (1U << state->lenbits) - 1;
    dmask = (1U << state->distbits) - 1;

    /* decode literals and length/distances until end-of-block or not enough
       input data or output space */
    do {
        if (bits < 15) {
            hold += (unsigned long)(PUP(in)) << bits;
            bits += 8;
            hold += (unsigned long)(PUP(in)) << bits;
            bits += 8;
        }
        _this = lcode[hold & lmask];
    dolen:
        op = (unsigned)(_this.bits);
        hold >>= op;
        bits -= op;
        op = (unsigned)(_this.op);
        if (op == 0) {                          /* literal */
            PUP(out) = (unsigned char)(_this.val);
        }
        else if (op & 16) {                     /* length base */
            len = (unsigned)(_this.val);
            op &= 15;                           /* number of extra bits */
            if (op) {
                if (bits < op) {
                    hold += (unsigned long)(PUP(in)) << bits;
                    bits += 8;
                }
                len += (unsigned)hold & ((1U << op) - 1);
                hold >>= op;
                bits -= op;
            }
            if (bits < 15) {
                hold += (unsigned long)(PUP(in)) << bits;
                bits += 8;
                hold += (unsigned long)(PUP(in)) << bits;
                bits += 8;
            }
            _this = dcode[hold & dmask];
        dodist:
            op = (unsigned)(_this.bits);
            hold >>= op;
            bits -= op;
            op = (unsigned)(_this.op);
            if (op & 16) {                      /* distance base */
                dist = (unsigned)(_this.val);
                op &= 15;                       /* number of extra bits */
                if (bits < op) {
                    hold += (unsigned long)(PUP(in)) << bits;
                    bits += 8;
                    if (bits < op) {
                        hold += (unsigned long)(PUP(in)) << bits;
                        bits += 8;
                    }
                }
                dist += (unsigned)hold & ((1U << op) - 1);
#ifdef INFLATE_STRICT
                if (dist > dmax) {
                    strm->msg = (char*)"invalid distance too back";
                    state->mode = BAD;
                    break;
                }
#endif
                hold >>= op;
                bits -= op;
                op = (unsigned)(out - beg);     /* max distance in output */
                if (dist > op) {                /* see if copy from window */
                    op = dist - op;             /* distance back in window */
                    if (op > whave) {
                        strm->msg = (char*)"invalid distance too back";
                        state->mode = BAD;
                        break;
                    }
                    from = window - OFF;
                    if (write == 0) {           /* very common case */
                        from += wsize - op;
                        if (op < len) {         /* some from window */
                            len -= op;
                            do {
                                PUP(out) = PUP(from);
                            } while (--op);
                            from = out - dist;  /* rest from output */
                        }
                    }
                    else if (write < op) {      /* wrap around window */
                        from += wsize + write - op;
                        op -= write;
                        if (op < len) {         /* some from end of window */
                            len -= op;
                            do {
                                PUP(out) = PUP(from);
                            } while (--op);
                            from = window - OFF;
                            if (write < len) {  /* some from start of window */
                                op = write;
                                len -= op;
                                do {
                                    PUP(out) = PUP(from);
                                } while (--op);
                                from = out - dist;      /* rest from output */
                            }
                        }
                    }
                    else {                      /* contiguous in window */
                        from += write - op;
                        if (op < len) {         /* some from window */
                            len -= op;
                            do {
                                PUP(out) = PUP(from);
                            } while (--op);
                            from = out - dist;  /* rest from output */
                        }
                    }
                    while (len > 2) {
                        PUP(out) = PUP(from);
                        PUP(out) = PUP(from);
                        PUP(out) = PUP(from);
                        len -= 3;
                    }
                    if (len) {
                        PUP(out) = PUP(from);
                        if (len > 1)
                            PUP(out) = PUP(from);
                    }
                }
                else {
                    from = out - dist;          /* copy direct from output */
                    do {                        /* minimum length is three */
                        PUP(out) = PUP(from);
                        PUP(out) = PUP(from);
                        PUP(out) = PUP(from);
                        len -= 3;
                    } while (len > 2);
                    if (len) {
                        PUP(out) = PUP(from);
                        if (len > 1)
                            PUP(out) = PUP(from);
                    }
                }
            }
            else if ((op & 64) == 0) {          /* 2nd level distance code */
                _this = dcode[_this.val + (hold & ((1U << op) - 1))];
                goto dodist;
            }
            else {
                strm->msg = (char*)"invalid distance code";
                state->mode = BAD;
                break;
            }
        }
        else if ((op & 64) == 0) {              /* 2nd level length code */
            _this = lcode[_this.val + (hold & ((1U << op) - 1))];
            goto dolen;
        }
        else if (op & 32) {                     /* end-of-block */
            state->mode = TYPE;
            break;
        }
        else {
            strm->msg = (char*)"invalid literal/length code";
            state->mode = BAD;
            break;
        }
    } while (in < last && out < end);

    /* return unused bytes (on entry, bits < 8, so in won't go too back) */
    len = bits >> 3;
    in -= len;
    bits -= len << 3;
    hold &= (1U << bits) - 1;

    /* update state and return */
    strm->next_in = in + OFF;
    strm->next_out = out + OFF;
    strm->avail_in = (unsigned)(in < last ? 5 + (last - in) : 5 - (in - last));
    strm->avail_out = (unsigned)(out < end ?
        257 + (end - out) : 257 - (out - end));
    state->hold = hold;
    state->bits = bits;
    return;
}

uLong adler32(uLong adler, const Bytef* buf, uInt len)
{
    unsigned long sum2;
    unsigned n;

    /* split Adler-32 into component sums */
    sum2 = (adler >> 16) & 0xffff;
    adler &= 0xffff;

    /* in case user likes doing a byte at a time, keep it fast */
    if (len == 1) {
        adler += buf[0];
        if (adler >= BASE)
            adler -= BASE;
        sum2 += adler;
        if (sum2 >= BASE)
            sum2 -= BASE;
        return adler | (sum2 << 16);
    }

    /* initial Adler-32 value (deferred check for len == 1 speed) */
    if (buf == Z_NULL)
        return 1L;

    /* in case short lengths are provided, keep it somewhat fast */
    if (len < 16) {
        while (len--) {
            adler += *buf++;
            sum2 += adler;
        }
        if (adler >= BASE)
            adler -= BASE;
        MOD4(sum2);             /* only added so many BASE's */
        return adler | (sum2 << 16);
    }

    /* do length NMAX blocks -- requires just one modulo operation */
    while (len >= NMAX) {
        len -= NMAX;
        n = NMAX / 16;          /* NMAX is divisible by 16 */
        do {
            DO16(buf);          /* 16 sums unrolled */
            buf += 16;
        } while (--n);
        MOD(adler);
        MOD(sum2);
    }

    /* do remaining bytes (less than NMAX, still just one modulo) */
    if (len) {                  /* avoid modulos if none remaining */
        while (len >= 16) {
            len -= 16;
            DO16(buf);
            buf += 16;
        }
        while (len--) {
            adler += *buf++;
            sum2 += adler;
        }
        MOD(adler);
        MOD(sum2);
    }

    /* return recombined sums */
    return adler | (sum2 << 16);
}

const char inflate_copyright[] =
" inflate 1.2.3 Copyright 1995-2005 Mark Adler ";
/*
  If you use the zlib library in a product, an acknowledgment is welcome
  in the documentation of your product. If for some reason you cannot
  include such an acknowledgment, I would appreciate that you keep this
  copyright string in the executable of your product.
 */

 /*
    Build a set of tables to decode the provided canonical Huffman code.
    The code lengths are lens[0..codes-1].  The result starts at *table,
    whose indices are 0..2^bits-1.  work is a writable array of at least
    lens shorts, which is used as a work area.  type is the type of code
    to be generated, CODES, LENS, or DISTS.  On return, zero is success,
    -1 is an invalid code, and +1 means that ENOUGH isn't enough.  table
    on return points to the next available entry's address.  bits is the
    requested root table index bits, and on return it is the actual root
    table index bits.  It will differ if the request is greater than the
    longest code or if it is less than the shortest code.
  */
int inflate_table(codetype type, unsigned short* lens, unsigned codes, code** table, unsigned* bits, unsigned short* work)
{
    unsigned len;               /* a code's length in bits */
    unsigned sym;               /* index of code symbols */
    unsigned min, max;          /* minimum and maximum code lengths */
    unsigned root;              /* number of index bits for root table */
    unsigned curr;              /* number of index bits for current table */
    unsigned drop;              /* code bits to drop for sub-table */
    int left;                   /* number of prefix codes available */
    unsigned used;              /* code entries in table used */
    unsigned huff;              /* Huffman code */
    unsigned incr;              /* for incrementing code, index */
    unsigned fill;              /* index for replicating entries */
    unsigned low;               /* low bits for current root entry */
    unsigned mask;              /* mask for low root bits */
    code _this;                  /* table entry for duplication */
    code* next;             /* next available space in table */
    const unsigned short* base;     /* base value table to use */
    const unsigned short* extra;    /* extra bits table to use */
    int end;                    /* use base and extra for symbol > end */
    unsigned short count[MAXBITS + 1];    /* number of codes of each length */
    unsigned short offs[MAXBITS + 1];     /* offsets in table for each length */
    static const unsigned short lbase[31] = { /* Length codes 257..285 base */
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0 };
    static const unsigned short lext[31] = { /* Length codes 257..285 extra */
        16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18,
        19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 16, 201, 196 };
    static const unsigned short dbase[32] = { /* Distance codes 0..29 base */
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
        8193, 12289, 16385, 24577, 0, 0 };
    static const unsigned short dext[32] = { /* Distance codes 0..29 extra */
        16, 16, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22,
        23, 23, 24, 24, 25, 25, 26, 26, 27, 27,
        28, 28, 29, 29, 64, 64 };

    /*
       Process a set of code lengths to create a canonical Huffman code.  The
       code lengths are lens[0..codes-1].  Each length corresponds to the
       symbols 0..codes-1.  The Huffman code is generated by first sorting the
       symbols by length from short to long, and retaining the symbol order
       for codes with equal lengths.  Then the code starts with all zero bits
       for the first code of the shortest length, and the codes are integer
       increments for the same length, and zeros are appended as the length
       increases.  For the deflate format, these bits are stored backwards
       from their more natural integer increment ordering, and so when the
       decoding tables are built in the large loop below, the integer codes
       are incremented backwards.

       This routine assumes, but does not check, that all of the entries in
       lens[] are in the range 0..MAXBITS.  The caller must assure this.
       1..MAXBITS is interpreted as that code length.  zero means that that
       symbol does not occur in this code.

       The codes are sorted by computing a count of codes for each length,
       creating from that a table of starting indices for each length in the
       sorted table, and then entering the symbols in order in the sorted
       table.  The sorted table is work[], with that space being provided by
       the caller.

       The length counts are used for other purposes as well, i.e. finding
       the minimum and maximum length codes, determining if there are any
       codes at all, checking for a valid set of lengths, and looking ahead
       at length counts to determine sub-table sizes when building the
       decoding tables.
     */

     /* accumulate lengths for codes (assumes lens[] all in 0..MAXBITS) */
    for (len = 0; len <= MAXBITS; len++)
        count[len] = 0;
    for (sym = 0; sym < codes; sym++)
        count[lens[sym]]++;

    /* bound code lengths, force root to be within code lengths */
    root = *bits;
    for (max = MAXBITS; max >= 1; max--)
        if (count[max] != 0) break;
    if (root > max) root = max;
    if (max == 0) {                     /* no symbols to code at all */
        _this.op = (unsigned char)64;    /* invalid code marker */
        _this.bits = (unsigned char)1;
        _this.val = (unsigned short)0;
        *(*table)++ = _this;             /* make a table to force an error */
        *(*table)++ = _this;
        *bits = 1;
        return 0;     /* no symbols, but wait for decoding to report error */
    }
    for (min = 1; min <= MAXBITS; min++)
        if (count[min] != 0) break;
    if (root < min) root = min;

    /* check for an over-subscribed or incomplete set of lengths */
    left = 1;
    for (len = 1; len <= MAXBITS; len++) {
        left <<= 1;
        left -= count[len];
        if (left < 0) return -1;        /* over-subscribed */
    }
    if (left > 0 && (type == CODES || max != 1))
        return -1;                      /* incomplete set */

    /* generate offsets into symbol table for each length for sorting */
    offs[1] = 0;
    for (len = 1; len < MAXBITS; len++)
        offs[len + 1] = offs[len] + count[len];

    /* sort symbols by length, by symbol order within each length */
    for (sym = 0; sym < codes; sym++)
        if (lens[sym] != 0) work[offs[lens[sym]]++] = (unsigned short)sym;

    /*
       Create and fill in decoding tables.  In this loop, the table being
       filled is at next and has curr index bits.  The code being used is huff
       with length len.  That code is converted to an index by dropping drop
       bits off of the bottom.  For codes where len is less than drop + curr,
       those top drop + curr - len bits are incremented through all values to
       fill the table with replicated entries.

       root is the number of index bits for the root table.  When len exceeds
       root, sub-tables are created pointed to by the root entry with an index
       of the low root bits of huff.  This is saved in low to check for when a
       new sub-table should be started.  drop is zero when the root table is
       being filled, and drop is root when sub-tables are being filled.

       When a new sub-table is needed, it is necessary to look ahead in the
       code lengths to determine what size sub-table is needed.  The length
       counts are used for this, and so count[] is decremented as codes are
       entered in the tables.

       used keeps track of how many table entries have been allocated from the
       provided *table space.  It is checked when a LENS table is being made
       against the space in *table, ENOUGH, minus the maximum space needed by
       the worst case distance code, MAXD.  This should never happen, but the
       sufficiency of ENOUGH has not been proven exhaustively, hence the check.
       This assumes that when type == LENS, bits == 9.

       sym increments through all symbols, and the loop terminates when
       all codes of length max, i.e. all codes, have been processed.  This
       routine permits incomplete codes, so another loop after this one fills
       in the rest of the decoding tables with invalid code markers.
     */

     /* set up for code type */
    switch (type) {
    case CODES:
        base = extra = work;    /* dummy value--not used */
        end = 19;
        break;
    case LENS:
        base = lbase;
        base -= 257;
        extra = lext;
        extra -= 257;
        end = 256;
        break;
    default:            /* DISTS */
        base = dbase;
        extra = dext;
        end = -1;
    }

    /* initialize state for loop */
    huff = 0;                   /* starting code */
    sym = 0;                    /* starting code symbol */
    len = min;                  /* starting code length */
    next = *table;              /* current table to fill in */
    curr = root;                /* current table index bits */
    drop = 0;                   /* current bits to drop from code for index */
    low = (unsigned)(-1);       /* trigger new sub-table when len > root */
    used = 1U << root;          /* use root table entries */
    mask = used - 1;            /* mask for comparing low */

    /* check available table space */
    if (type == LENS && used >= ENOUGH - MAXD)
        return 1;

    /* process all codes and make table entries */
    for (;;) {
        /* create table entry */
        _this.bits = (unsigned char)(len - drop);
        if ((int)(work[sym]) < end) {
            _this.op = (unsigned char)0;
            _this.val = work[sym];
        }
        else if ((int)(work[sym]) > end) {
            _this.op = (unsigned char)(extra[work[sym]]);
            _this.val = base[work[sym]];
        }
        else {
            _this.op = (unsigned char)(32 + 64);         /* end of block */
            _this.val = 0;
        }

        /* replicate for those indices with low len bits equal to huff */
        incr = 1U << (len - drop);
        fill = 1U << curr;
        min = fill;                 /* save offset to next table */
        do {
            fill -= incr;
            next[(huff >> drop) + fill] = _this;
        } while (fill != 0);

        /* backwards increment the len-bit code huff */
        incr = 1U << (len - 1);
        while (huff & incr)
            incr >>= 1;
        if (incr != 0) {
            huff &= incr - 1;
            huff += incr;
        }
        else
            huff = 0;

        /* go to next symbol, update count, len */
        sym++;
        if (--(count[len]) == 0) {
            if (len == max) break;
            len = lens[work[sym]];
        }

        /* create new sub-table if needed */
        if (len > root && (huff & mask) != low) {
            /* if first time, transition to sub-tables */
            if (drop == 0)
                drop = root;

            /* increment past last table */
            next += min;            /* here min is 1 << curr */

            /* determine length of next table */
            curr = len - drop;
            left = (int)(1 << curr);
            while (curr + drop < max) {
                left -= count[curr + drop];
                if (left <= 0) break;
                curr++;
                left <<= 1;
            }

            /* check for enough space */
            used += 1U << curr;
            if (type == LENS && used >= ENOUGH - MAXD)
                return 1;

            /* point entry in root table to sub-table */
            low = huff & mask;
            (*table)[low].op = (unsigned char)curr;
            (*table)[low].bits = (unsigned char)root;
            (*table)[low].val = (unsigned short)(next - *table);
        }
    }

    /*
       Fill in rest of table for incomplete codes.  This loop is similar to the
       loop above in incrementing huff for table indices.  It is assumed that
       len is equal to curr + drop, so there is no loop needed to increment
       through high index bits.  When the current sub-table is filled, the loop
       drops back to the root table to fill in any remaining entries there.
     */
    _this.op = (unsigned char)64;                /* invalid code marker */
    _this.bits = (unsigned char)(len - drop);
    _this.val = (unsigned short)0;
    while (huff != 0) {
        /* when done with sub-table, drop back to root table */
        if (drop != 0 && (huff & mask) != low) {
            drop = 0;
            len = root;
            next = *table;
            _this.bits = (unsigned char)len;
        }

        /* put invalid code marker in table */
        next[huff >> drop] = _this;

        /* backwards increment the len-bit code huff */
        incr = 1U << (len - 1);
        while (huff & incr)
            incr >>= 1;
        if (incr != 0) {
            huff &= incr - 1;
            huff += incr;
        }
        else
            huff = 0;
    }

    /* set return parameters */
    *table += used;
    *bits = root;
    return 0;
}


/* function prototypes */
int updatewindow(z_streamp strm, unsigned out);
unsigned syncsearch(unsigned* have, unsigned char* buf,
    unsigned len);

int inflateReset(z_streamp strm)
{
    struct inflate_state* state;

    if (strm == Z_NULL || strm->state == Z_NULL) return Z_STREAM_ERROR;
    state = (struct inflate_state*)strm->state;
    strm->total_in = strm->total_out = state->total = 0;
    strm->msg = Z_NULL;
    strm->adler = 1;        /* to support ill-conceived Java test suite */
    state->mode = HEAD;
    state->last = 0;
    state->havedict = 0;
    state->dmax = 32768U;
    state->head = Z_NULL;
    state->wsize = 0;
    state->whave = 0;
    state->write = 0;
    state->hold = 0;
    state->bits = 0;
    state->lencode = state->distcode = state->next = state->codes;
    return Z_OK;
}

int inflatePrime(z_streamp strm, int bits, int value)
{
    struct inflate_state* state;

    if (strm == Z_NULL || strm->state == Z_NULL) return Z_STREAM_ERROR;
    state = (struct inflate_state*)strm->state;
    if (bits > 16 || state->bits + bits > 32) return Z_STREAM_ERROR;
    value &= (1L << bits) - 1;
    state->hold += value << state->bits;
    state->bits += bits;
    return Z_OK;
}

int inflateInit2_(z_streamp strm, int windowBits, const char* version, int stream_size)
{
    struct inflate_state* state;

    if (version == Z_NULL || version[0] != ZLIB_VERSION[0] ||
        stream_size != (int)(sizeof(z_stream)))
        return Z_VERSION_ERROR;
    if (strm == Z_NULL) return Z_STREAM_ERROR;
    strm->msg = Z_NULL;                 /* in case we return an error */
    strm->opaque = (voidpf)0;
    state = (struct inflate_state*)
        ZALLOC(strm, 1, sizeof(struct inflate_state));
    if (state == Z_NULL) return Z_MEM_ERROR;
    strm->state = (struct internal_state*)state;
    if (windowBits < 0) {
        state->wrap = 0;
        windowBits = -windowBits;
    }
    else {
        state->wrap = (windowBits >> 4) + 1;
#ifdef GUNZIP
        if (windowBits < 48) windowBits &= 15;
#endif
    }
    if (windowBits < 8 || windowBits > 15) {
        ZFREE(strm, state);
        strm->state = Z_NULL;
        return Z_STREAM_ERROR;
    }
    state->wbits = (unsigned)windowBits;
    state->window = Z_NULL;
    return inflateReset(strm);
}

int inflateInit_(z_streamp strm, const char* version, int stream_size)
{
    return inflateInit2_(strm, DEF_WBITS, version, stream_size);
}

/*
   Return state with length and distance decoding tables and index sizes set to
   fixed code decoding.  Normally this returns fixed tables from inffixed.h.
   If BUILDFIXED is defined, then instead this routine builds the tables the
   first time it's called, and returns those tables the first time and
   thereafter.  This reduces the size of the code by about 2K bytes, in
   exchange for a little execution time.  However, BUILDFIXED should not be
   used for threaded applications, since the rewriting of the tables and virgin
   may not be thread-safe.
 */
void fixedtables(struct inflate_state* state)
{
    static int virgin = 1;
    static code* lenfix, * distfix;
    static code fixed[544];

    /* build fixed huffman tables if first call (may not be thread safe) */
    if (virgin) {
        unsigned sym, bits;
        static code* next;

        /* literal/length table */
        sym = 0;
        while (sym < 144) state->lens[sym++] = 8;
        while (sym < 256) state->lens[sym++] = 9;
        while (sym < 280) state->lens[sym++] = 7;
        while (sym < 288) state->lens[sym++] = 8;
        next = fixed;
        lenfix = next;
        bits = 9;
        inflate_table(LENS, state->lens, 288, &(next), &(bits), state->work);

        /* distance table */
        sym = 0;
        while (sym < 32) state->lens[sym++] = 5;
        distfix = next;
        bits = 5;
        inflate_table(DISTS, state->lens, 32, &(next), &(bits), state->work);

        /* do this just once */
        virgin = 0;
    }

    state->lencode = lenfix;
    state->lenbits = 9;
    state->distcode = distfix;
    state->distbits = 5;
}

#ifdef MAKEFIXED
#include <stdio.h>

/*
   Write out the inffixed.h that is #include'd above.  Defining MAKEFIXED also
   defines BUILDFIXED, so the tables are built on the fly.  makefixed() writes
   those tables to stdout, which would be piped to inffixed.h.  A small program
   can simply call makefixed to do this:

    void makefixed(void);

    int main(void)
    {
        makefixed();
        return 0;
    }

   Then that can be linked with zlib built with MAKEFIXED defined and run:

    a.out > inffixed.h
 */
void makefixed()
{
    unsigned low, size;
    struct inflate_state state;

    fixedtables(&state);
    puts("    /* inffixed.h -- table for decoding fixed codes");
    puts("     * Generated automatically by makefixed().");
    puts("     */");
    puts("");
    puts("    /* WARNING: this file should *not* be used by applications.");
    puts("       It is part of the implementation of this library and is");
    puts("       subject to change. Applications should only use zlib.h.");
    puts("     */");
    puts("");
    size = 1U << 9;
    printf("    static const code lenfix[%u] = {", size);
    low = 0;
    for (;;) {
        if ((low % 7) == 0) printf("\n        ");
        printf("{%u,%u,%d}", state.lencode[low].op, state.lencode[low].bits,
            state.lencode[low].val);
        if (++low == size) break;
        putchar(',');
    }
    puts("\n    };");
    size = 1U << 5;
    printf("\n    static const code distfix[%u] = {", size);
    low = 0;
    for (;;) {
        if ((low % 6) == 0) printf("\n        ");
        printf("{%u,%u,%d}", state.distcode[low].op, state.distcode[low].bits,
            state.distcode[low].val);
        if (++low == size) break;
        putchar(',');
    }
    puts("\n    };");
}
#endif /* MAKEFIXED */

/*
   Update the window with the last wsize (normally 32K) bytes written before
   returning.  If window does not exist yet, create it.  This is only called
   when a window is already in use, or when output has been written during this
   inflate call, but the end of the deflate stream has not been reached yet.
   It is also called to create a window for dictionary data when a dictionary
   is loaded.

   Providing output buffers larger than 32K to inflate() should provide a speed
   advantage, since only the last 32K of output is copied to the sliding window
   upon return from inflate(), and since all distances after the first 32K of
   output will fall in the output data, making match copies simpler and faster.
   The advantage may be dependent on the size of the processor's data caches.
 */
int updatewindow(z_streamp strm, unsigned out)
{
    struct inflate_state* state;
    unsigned copy, dist;

    state = (struct inflate_state*)strm->state;

    /* if it hasn't been done already, allocate space for the window */
    if (state->window == Z_NULL) {
        state->window = (unsigned char*)
            ZALLOC(strm, 1U << state->wbits,
                sizeof(unsigned char));
        if (state->window == Z_NULL) return 1;
    }

    /* if window not in use yet, initialize */
    if (state->wsize == 0) {
        state->wsize = 1U << state->wbits;
        state->write = 0;
        state->whave = 0;
    }

    /* copy state->wsize or less output bytes into the circular window */
    copy = out - strm->avail_out;
    if (copy >= state->wsize) {
        zmemcpy(state->window, strm->next_out - state->wsize, state->wsize);
        state->write = 0;
        state->whave = state->wsize;
    }
    else {
        dist = state->wsize - state->write;
        if (dist > copy) dist = copy;
        zmemcpy(state->window + state->write, strm->next_out - copy, dist);
        copy -= dist;
        if (copy) {
            zmemcpy(state->window, strm->next_out - copy, copy);
            state->write = copy;
            state->whave = state->wsize;
        }
        else {
            state->write += dist;
            if (state->write == state->wsize) state->write = 0;
            if (state->whave < state->wsize) state->whave += dist;
        }
    }
    return 0;
}

/* Macros for inflate(): */

/* check function to use adler32() for zlib or crc32() for gzip */
#ifdef GUNZIP
#  define UPDATE(check, buf, len) \
    (state->flags ? crc32(check, buf, len) : adler32(check, buf, len))
#else
#  define UPDATE(check, buf, len) adler32(check, buf, len)
#endif

/* check macros for header crc */
#ifdef GUNZIP
#  define CRC2(check, word) \
    do { \
        hbuf[0] = (unsigned char)(word); \
        hbuf[1] = (unsigned char)((word) >> 8); \
        check = crc32(check, hbuf, 2); \
    } while (0)

#  define CRC4(check, word) \
    do { \
        hbuf[0] = (unsigned char)(word); \
        hbuf[1] = (unsigned char)((word) >> 8); \
        hbuf[2] = (unsigned char)((word) >> 16); \
        hbuf[3] = (unsigned char)((word) >> 24); \
        check = crc32(check, hbuf, 4); \
    } while (0)
#endif

/* Load registers with state in inflate() for speed */
#define LOAD() \
    do { \
        put = strm->next_out; \
        left = strm->avail_out; \
        next = strm->next_in; \
        have = strm->avail_in; \
        hold = state->hold; \
        bits = state->bits; \
    } while (0)

/* Restore state from registers in inflate() */
#define RESTORE() \
    do { \
        strm->next_out = put; \
        strm->avail_out = left; \
        strm->next_in = next; \
        strm->avail_in = have; \
        state->hold = hold; \
        state->bits = bits; \
    } while (0)

/* Clear the input bit accumulator */
#define INITBITS() \
    do { \
        hold = 0; \
        bits = 0; \
    } while (0)

/* Get a byte of input into the bit accumulator, or return from inflate()
   if there is no input available. */
#define PULLBYTE() \
    do { \
        if (have == 0) goto inf_leave; \
        have--; \
        hold += (unsigned long)(*next++) << bits; \
        bits += 8; \
    } while (0)

   /* Assure that there are at least n bits in the bit accumulator.  If there is
      not enough available input to do that, then return from inflate(). */
#define NEEDBITS(n) \
    do { \
        while (bits < (unsigned)(n)) \
            PULLBYTE(); \
    } while (0)

      /* Return the low n bits of the bit accumulator (n < 16) */
#define BITS(n) \
    ((unsigned)hold & ((1U << (n)) - 1))

/* Remove n bits from the bit accumulator */
#define DROPBITS(n) \
    do { \
        hold >>= (n); \
        bits -= (unsigned)(n); \
    } while (0)

/* Remove zero to seven bits as needed to go to a byte boundary */
#define BYTEBITS() \
    do { \
        hold >>= bits & 7; \
        bits -= bits & 7; \
    } while (0)

/* Reverse the bytes in a 32-bit value */
#define REVERSE(q) \
    ((((q) >> 24) & 0xff) + (((q) >> 8) & 0xff00) + \
     (((q) & 0xff00) << 8) + (((q) & 0xff) << 24))

/*
   inflate() uses a state machine to process as much input data and generate as
   much output data as possible before returning.  The state machine is
   structured roughly as follows:

    for (;;) switch (state) {
    ...
    case STATEn:
        if (not enough input data or output space to make progress)
            return;
        ... make progress ...
        state = STATEm;
        break;
    ...
    }

   so when inflate() is called again, the same case is attempted again, and
   if the appropriate resources are provided, the machine proceeds to the
   next state.  The NEEDBITS() macro is usually the way the state evaluates
   whether it can proceed or should return.  NEEDBITS() does the return if
   the requested bits are not available.  The typical use of the BITS macros
   is:

        NEEDBITS(n);
        ... do something with BITS(n) ...
        DROPBITS(n);

   where NEEDBITS(n) either returns from inflate() if there isn't enough
   input left to load n bits into the accumulator, or it continues.  BITS(n)
   gives the low n bits in the accumulator.  When done, DROPBITS(n) drops
   the low n bits off the accumulator.  INITBITS() clears the accumulator
   and sets the number of available bits to zero.  BYTEBITS() discards just
   enough bits to put the accumulator on a byte boundary.  After BYTEBITS()
   and a NEEDBITS(8), then BITS(8) would return the next byte in the stream.

   NEEDBITS(n) uses PULLBYTE() to get an available byte of input, or to return
   if there is no input available.  The decoding of variable length codes uses
   PULLBYTE() directly in order to pull just enough bytes to decode the next
   code, and no more.

   Some states loop until they get enough input, making sure that enough
   state information is maintained to continue the loop where it left off
   if NEEDBITS() returns in the loop.  For example, want, need, and keep
   would all have to actually be part of the saved state in case NEEDBITS()
   returns:

    case STATEw:
        while (want < need) {
            NEEDBITS(n);
            keep[want++] = BITS(n);
            DROPBITS(n);
        }
        state = STATEx;
    case STATEx:

   As shown above, if the next state is also the next case, then the break
   is omitted.

   A state may also return if there is not enough output space available to
   complete that state.  Those states are copying stored data, writing a
   literal byte, and copying a matching string.

   When returning, a "goto inf_leave" is used to update the total counters,
   update the check value, and determine whether any progress has been made
   during that inflate() call in order to return the proper return code.
   Progress is defined as a change in either strm->avail_in or strm->avail_out.
   When there is a window, goto inf_leave will update the window with the last
   output written.  If a goto inf_leave occurs in the middle of decompression
   and there is no window currently, goto inf_leave will create one and copy
   output to the window for the next call of inflate().

   In this implementation, the flush parameter of inflate() only affects the
   return code (per zlib.h).  inflate() always writes as much as possible to
   strm->next_out, given the space available and the provided input--the effect
   documented in zlib.h of Z_SYNC_FLUSH.  Furthermore, inflate() always defers
   the allocation of and copying into a sliding window until necessary, which
   provides the effect documented in zlib.h for Z_FINISH when the entire input
   stream available.  So the only thing the flush parameter actually does is:
   when flush is set to Z_FINISH, inflate() cannot return Z_OK.  Instead it
   will return Z_BUF_ERROR if it has not reached the end of the stream.
 */

int inflate2(z_streamp strm, int flush)
{
    struct inflate_state* state;
    unsigned char* next;    /* next input */
    unsigned char* put;     /* next output */
    unsigned have, left;        /* available input and output */
    unsigned long hold;         /* bit buffer */
    unsigned bits;              /* bits in bit buffer */
    unsigned in, out;           /* save starting available input and output */
    unsigned copy;              /* number of stored or match bytes to copy */
    unsigned char* from;    /* where to copy match bytes from */
    code _this;                  /* current decoding table entry */
    code last;                  /* parent table entry */
    unsigned len;               /* length to copy for repeats, bits to drop */
    int ret;                    /* return code */
#ifdef GUNZIP
    unsigned char hbuf[4];      /* buffer for gzip header crc calculation */
#endif
    static const unsigned short order[19] = /* permutation of code lengths */
    { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

    if (strm == Z_NULL || strm->state == Z_NULL || strm->next_out == Z_NULL ||
        (strm->next_in == Z_NULL && strm->avail_in != 0))
        return Z_STREAM_ERROR;

    state = (struct inflate_state*)strm->state;
    if (state->mode == TYPE) state->mode = TYPEDO;      /* skip check */
    LOAD();
    in = have;
    out = left;
    ret = Z_OK;
    for (;;)
        switch (state->mode) {
        case HEAD:
            if (state->wrap == 0) {
                state->mode = TYPEDO;
                break;
            }
            NEEDBITS(16);
#ifdef GUNZIP
            if ((state->wrap & 2) && hold == 0x8b1f) {  /* gzip header */
                state->check = crc32(0L, Z_NULL, 0);
                CRC2(state->check, hold);
                INITBITS();
                state->mode = FLAGS;
                break;
            }
            state->flags = 0;           /* expect zlib header */
            if (state->head != Z_NULL)
                state->head->done = -1;
            if (!(state->wrap & 1) ||   /* check if zlib header allowed */
#else
            if (
#endif
                ((BITS(8) << 8) + (hold >> 8)) % 31) {
                strm->msg = (char*)"incorrect header check";
                    state->mode = BAD;
                    break;
            }
            if (BITS(4) != Z_DEFLATED) {
                strm->msg = (char*)"unknown compression method";
                    state->mode = BAD;
                break;
            }
            DROPBITS(4);
            len = BITS(4) + 8;
            if (len > state->wbits) {
                strm->msg = (char*)"invalid window size";
                state->mode = BAD;
                break;
            }
            state->dmax = 1U << len;
            strm->adler = state->check = adler32(0L, Z_NULL, 0);
            state->mode = hold & 0x200 ? DICTID : TYPE;
            INITBITS();
            break;
#ifdef GUNZIP
        case FLAGS:
            NEEDBITS(16);
            state->flags = (int)(hold);
            if ((state->flags & 0xff) != Z_DEFLATED) {
                strm->msg = (char*)"unknown compression method";
                state->mode = BAD;
                break;
            }
            if (state->flags & 0xe000) {
                strm->msg = (char*)"unknown header flags set";
                state->mode = BAD;
                break;
            }
            if (state->head != Z_NULL)
                state->head->text = (int)((hold >> 8) & 1);
            if (state->flags & 0x0200) CRC2(state->check, hold);
            INITBITS();
            state->mode = TIME;
        case TIME:
            NEEDBITS(32);
            if (state->head != Z_NULL)
                state->head->time = hold;
            if (state->flags & 0x0200) CRC4(state->check, hold);
            INITBITS();
            state->mode = OS;
        case OS:
            NEEDBITS(16);
            if (state->head != Z_NULL) {
                state->head->xflags = (int)(hold & 0xff);
                state->head->os = (int)(hold >> 8);
            }
            if (state->flags & 0x0200) CRC2(state->check, hold);
            INITBITS();
            state->mode = EXLEN;
        case EXLEN:
            if (state->flags & 0x0400) {
                NEEDBITS(16);
                state->length = (unsigned)(hold);
                if (state->head != Z_NULL)
                    state->head->extra_len = (unsigned)hold;
                if (state->flags & 0x0200) CRC2(state->check, hold);
                INITBITS();
            }
            else if (state->head != Z_NULL)
                state->head->extra = Z_NULL;
            state->mode = EXTRA;
        case EXTRA:
            if (state->flags & 0x0400) {
                copy = state->length;
                if (copy > have) copy = have;
                if (copy) {
                    if (state->head != Z_NULL &&
                        state->head->extra != Z_NULL) {
                        len = state->head->extra_len - state->length;
                        zmemcpy(state->head->extra + len, next,
                            len + copy > state->head->extra_max ?
                            state->head->extra_max - len : copy);
                    }
                    if (state->flags & 0x0200)
                        state->check = crc32(state->check, next, copy);
                    have -= copy;
                    next += copy;
                    state->length -= copy;
                }
                if (state->length) goto inf_leave;
            }
            state->length = 0;
            state->mode = NAME;
        case NAME:
            if (state->flags & 0x0800) {
                if (have == 0) goto inf_leave;
                copy = 0;
                do {
                    len = (unsigned)(next[copy++]);
                    if (state->head != Z_NULL &&
                        state->head->name != Z_NULL &&
                        state->length < state->head->name_max)
                        state->head->name[state->length++] = len;
                } while (len && copy < have);
                if (state->flags & 0x0200)
                    state->check = crc32(state->check, next, copy);
                have -= copy;
                next += copy;
                if (len) goto inf_leave;
            }
            else if (state->head != Z_NULL)
                state->head->name = Z_NULL;
            state->length = 0;
            state->mode = COMMENT;
        case COMMENT:
            if (state->flags & 0x1000) {
                if (have == 0) goto inf_leave;
                copy = 0;
                do {
                    len = (unsigned)(next[copy++]);
                    if (state->head != Z_NULL &&
                        state->head->comment != Z_NULL &&
                        state->length < state->head->comm_max)
                        state->head->comment[state->length++] = len;
                } while (len && copy < have);
                if (state->flags & 0x0200)
                    state->check = crc32(state->check, next, copy);
                have -= copy;
                next += copy;
                if (len) goto inf_leave;
            }
            else if (state->head != Z_NULL)
                state->head->comment = Z_NULL;
            state->mode = HCRC;
        case HCRC:
            if (state->flags & 0x0200) {
                NEEDBITS(16);
                if (hold != (state->check & 0xffff)) {
                    strm->msg = (char*)"header crc mismatch";
                    state->mode = BAD;
                    break;
                }
                INITBITS();
            }
            if (state->head != Z_NULL) {
                state->head->hcrc = (int)((state->flags >> 9) & 1);
                state->head->done = 1;
            }
            strm->adler = state->check = crc32(0L, Z_NULL, 0);
            state->mode = TYPE;
            break;
#endif
        case DICTID:
            NEEDBITS(32);
            strm->adler = state->check = REVERSE(hold);
            INITBITS();
            state->mode = DICT;
        case DICT:
            if (state->havedict == 0) {
                RESTORE();
                return Z_NEED_DICT;
            }
            strm->adler = state->check = adler32(0L, Z_NULL, 0);
            state->mode = TYPE;
        case TYPE:
            if (flush == Z_BLOCK) goto inf_leave;
        case TYPEDO:
            if (state->last) {
                BYTEBITS();
                state->mode = CHECK;
                break;
            }
            NEEDBITS(3);
            state->last = BITS(1);
            DROPBITS(1);
            switch (BITS(2)) {
            case 0:                             /* stored block */
                state->mode = STORED;
                break;
            case 1:                             /* fixed block */
                fixedtables(state);
                state->mode = LEN;              /* decode codes */
                break;
            case 2:                             /* dynamic block */
                state->mode = TABLE;
                break;
            case 3:
                strm->msg = (char*)"invalid block type";
                state->mode = BAD;
            }
            DROPBITS(2);
            break;
        case STORED:
            BYTEBITS();                         /* go to byte boundary */
            NEEDBITS(32);
            if ((hold & 0xffff) != ((hold >> 16) ^ 0xffff)) {
                strm->msg = (char*)"invalid stored block lengths";
                state->mode = BAD;
                break;
            }
            state->length = (unsigned)hold & 0xffff;
            INITBITS();
            state->mode = COPY;
        case COPY:
            copy = state->length;
            if (copy) {
                if (copy > have) copy = have;
                if (copy > left) copy = left;
                if (copy == 0) goto inf_leave;
                zmemcpy(put, next, copy);
                have -= copy;
                next += copy;
                left -= copy;
                put += copy;
                state->length -= copy;
                break;
            }
            state->mode = TYPE;
            break;
        case TABLE:
            NEEDBITS(14);
            state->nlen = BITS(5) + 257;
            DROPBITS(5);
            state->ndist = BITS(5) + 1;
            DROPBITS(5);
            state->ncode = BITS(4) + 4;
            DROPBITS(4);
#ifndef PKZIP_BUG_WORKAROUND
            if (state->nlen > 286 || state->ndist > 30) {
                strm->msg = (char*)"too many length or distance symbols";
                state->mode = BAD;
                break;
            }
#endif
            state->have = 0;
            state->mode = LENLENS;
        case LENLENS:
            while (state->have < state->ncode) {
                NEEDBITS(3);
                state->lens[order[state->have++]] = (unsigned short)BITS(3);
                DROPBITS(3);
            }
            while (state->have < 19)
                state->lens[order[state->have++]] = 0;
            state->next = state->codes;
            state->lencode = (code const*)(state->next);
            state->lenbits = 7;
            ret = inflate_table(CODES, state->lens, 19, &(state->next),
                &(state->lenbits), state->work);
            if (ret) {
                strm->msg = (char*)"invalid code lengths set";
                state->mode = BAD;
                break;
            }
            state->have = 0;
            state->mode = CODELENS;
        case CODELENS:
            while (state->have < state->nlen + state->ndist) {
                for (;;) {
                    _this = state->lencode[BITS(state->lenbits)];
                    if ((unsigned)(_this.bits) <= bits) break;
                    PULLBYTE();
                }
                if (_this.val < 16) {
                    NEEDBITS(_this.bits);
                    DROPBITS(_this.bits);
                    state->lens[state->have++] = _this.val;
                }
                else {
                    if (_this.val == 16) {
                        NEEDBITS(_this.bits + 2);
                        DROPBITS(_this.bits);
                        if (state->have == 0) {
                            strm->msg = (char*)"invalid bit length repeat";
                            state->mode = BAD;
                            break;
                        }
                        len = state->lens[state->have - 1];
                        copy = 3 + BITS(2);
                        DROPBITS(2);
                    }
                    else if (_this.val == 17) {
                        NEEDBITS(_this.bits + 3);
                        DROPBITS(_this.bits);
                        len = 0;
                        copy = 3 + BITS(3);
                        DROPBITS(3);
                    }
                    else {
                        NEEDBITS(_this.bits + 7);
                        DROPBITS(_this.bits);
                        len = 0;
                        copy = 11 + BITS(7);
                        DROPBITS(7);
                    }
                    if (state->have + copy > state->nlen + state->ndist) {
                        strm->msg = (char*)"invalid bit length repeat";
                        state->mode = BAD;
                        break;
                    }
                    while (copy--)
                        state->lens[state->have++] = (unsigned short)len;
                }
            }

            /* handle error breaks in while */
            if (state->mode == BAD) break;

            /* build code tables */
            state->next = state->codes;
            state->lencode = (code const*)(state->next);
            state->lenbits = 9;
            ret = inflate_table(LENS, state->lens, state->nlen, &(state->next),
                &(state->lenbits), state->work);
            if (ret) {
                strm->msg = (char*)"invalid literal/lengths set";
                state->mode = BAD;
                break;
            }
            state->distcode = (code const*)(state->next);
            state->distbits = 6;
            ret = inflate_table(DISTS, state->lens + state->nlen, state->ndist,
                &(state->next), &(state->distbits), state->work);
            if (ret) {
                strm->msg = (char*)"invalid distances set";
                state->mode = BAD;
                break;
            }
            state->mode = LEN;
        case LEN:
            if (have >= 6 && left >= 258) {
                RESTORE();
                inflate_fast(strm, out);
                LOAD();
                break;
            }
            for (;;) {
                _this = state->lencode[BITS(state->lenbits)];
                if ((unsigned)(_this.bits) <= bits) break;
                PULLBYTE();
            }
            if (_this.op && (_this.op & 0xf0) == 0) {
                last = _this;
                for (;;) {
                    _this = state->lencode[last.val +
                        (BITS(last.bits + last.op) >> last.bits)];
                    if ((unsigned)(last.bits + _this.bits) <= bits) break;
                    PULLBYTE();
                }
                DROPBITS(last.bits);
            }
            DROPBITS(_this.bits);
            state->length = (unsigned)_this.val;
            if ((int)(_this.op) == 0) {
                state->mode = LIT;
                break;
            }
            if (_this.op & 32) {
                state->mode = TYPE;
                break;
            }
            if (_this.op & 64) {
                strm->msg = (char*)"invalid literal/length code";
                state->mode = BAD;
                break;
            }
            state->extra = (unsigned)(_this.op) & 15;
            state->mode = LENEXT;
        case LENEXT:
            if (state->extra) {
                NEEDBITS(state->extra);
                state->length += BITS(state->extra);
                DROPBITS(state->extra);
            }
            state->mode = DIST;
        case DIST:
            for (;;) {
                _this = state->distcode[BITS(state->distbits)];
                if ((unsigned)(_this.bits) <= bits) break;
                PULLBYTE();
            }
            if ((_this.op & 0xf0) == 0) {
                last = _this;
                for (;;) {
                    _this = state->distcode[last.val +
                        (BITS(last.bits + last.op) >> last.bits)];
                    if ((unsigned)(last.bits + _this.bits) <= bits) break;
                    PULLBYTE();
                }
                DROPBITS(last.bits);
            }
            DROPBITS(_this.bits);
            if (_this.op & 64) {
                strm->msg = (char*)"invalid distance code";
                state->mode = BAD;
                break;
            }
            state->offset = (unsigned)_this.val;
            state->extra = (unsigned)(_this.op) & 15;
            state->mode = DISTEXT;
        case DISTEXT:
            if (state->extra) {
                NEEDBITS(state->extra);
                state->offset += BITS(state->extra);
                DROPBITS(state->extra);
            }
#ifdef INFLATE_STRICT
            if (state->offset > state->dmax) {
                strm->msg = (char*)"invalid distance too back";
                state->mode = BAD;
                break;
            }
#endif
            if (state->offset > state->whave + out - left) {
                strm->msg = (char*)"invalid distance too back";
                state->mode = BAD;
                break;
            }
            state->mode = MATCH;
        case MATCH:
            if (left == 0) goto inf_leave;
            copy = out - left;
            if (state->offset > copy) {         /* copy from window */
                copy = state->offset - copy;
                if (copy > state->write) {
                    copy -= state->write;
                    from = state->window + (state->wsize - copy);
                }
                else
                    from = state->window + (state->write - copy);
                if (copy > state->length) copy = state->length;
            }
            else {                              /* copy from output */
                from = put - state->offset;
                copy = state->length;
            }
            if (copy > left) copy = left;
            left -= copy;
            state->length -= copy;
            do {
                *put++ = *from++;
            } while (--copy);
            if (state->length == 0) state->mode = LEN;
            break;
        case LIT:
            if (left == 0) goto inf_leave;
            *put++ = (unsigned char)(state->length);
            left--;
            state->mode = LEN;
            break;
        case CHECK:
            if (state->wrap) {
                NEEDBITS(32);
                out -= left;
                strm->total_out += out;
                state->total += out;
                if (out)
                    strm->adler = state->check =
                    UPDATE(state->check, put - out, out);
                out = left;
                if ((
#ifdef GUNZIP
                    state->flags ? hold :
#endif
                    REVERSE(hold)) != state->check) {
                    strm->msg = (char*)"incorrect data check";
                    state->mode = BAD;
                    break;
                }
                INITBITS();
            }
#ifdef GUNZIP
            state->mode = LENGTH;
        case LENGTH:
            if (state->wrap && state->flags) {
                NEEDBITS(32);
                if (hold != (state->total & 0xffffffffUL)) {
                    strm->msg = (char*)"incorrect length check";
                    state->mode = BAD;
                    break;
                }
                INITBITS();
            }
#endif
            state->mode = DONE;
        case DONE:
            ret = Z_STREAM_END;
            goto inf_leave;
        case BAD:
            ret = Z_DATA_ERROR;
            goto inf_leave;
        case MEM:
            return Z_MEM_ERROR;
        case SYNC:
        default:
            return Z_STREAM_ERROR;
        }

    /*
       Return from inflate(), updating the total counts and the check value.
       If there was no progress during the inflate() call, return a buffer
       error.  Call updatewindow() to create and/or update the window state.
       Note: a memory error from inflate() is non-recoverable.
     */
inf_leave:
    RESTORE();
    if (state->wsize || (state->mode < CHECK && out != strm->avail_out))
        if (updatewindow(strm, out)) {
            state->mode = MEM;
            return Z_MEM_ERROR;
        }
    in -= strm->avail_in;
    out -= strm->avail_out;
    strm->total_in += in;
    strm->total_out += out;
    state->total += out;
    if (state->wrap && out)
        strm->adler = state->check =
        UPDATE(state->check, strm->next_out - out, out);
    strm->data_type = state->bits + (state->last ? 64 : 0) +
        (state->mode == TYPE ? 128 : 0);
    if (((in == 0 && out == 0) || flush == Z_FINISH) && ret == Z_OK)
        ret = Z_BUF_ERROR;
    return ret;
}

int inflateEnd(z_streamp strm)
{
    struct inflate_state* state;
    if (strm == Z_NULL || strm->state == Z_NULL)
        return Z_STREAM_ERROR;
    state = (struct inflate_state*)strm->state;
    if (state->window != Z_NULL) ZFREE(strm, state->window);
    ZFREE(strm, strm->state);
    strm->state = Z_NULL;
    return Z_OK;
}

int inflateSetDictionary(z_streamp strm, const Bytef* dictionary, uInt dictLength)
{
    struct inflate_state* state;
    unsigned long id;

    /* check state */
    if (strm == Z_NULL || strm->state == Z_NULL) return Z_STREAM_ERROR;
    state = (struct inflate_state*)strm->state;
    if (state->wrap != 0 && state->mode != DICT)
        return Z_STREAM_ERROR;

    /* check for correct dictionary id */
    if (state->mode == DICT) {
        id = adler32(0L, Z_NULL, 0);
        id = adler32(id, dictionary, dictLength);
        if (id != state->check)
            return Z_DATA_ERROR;
    }

    /* copy dictionary to window */
    if (updatewindow(strm, strm->avail_out)) {
        state->mode = MEM;
        return Z_MEM_ERROR;
    }
    if (dictLength > state->wsize) {
        zmemcpy(state->window, dictionary + dictLength - state->wsize,
            state->wsize);
        state->whave = state->wsize;
    }
    else {
        zmemcpy(state->window + state->wsize - dictLength, dictionary,
            dictLength);
        state->whave = dictLength;
    }
    state->havedict = 1;
    return Z_OK;
}

int inflateGetHeader(z_streamp strm, gz_headerp head)
{
    struct inflate_state* state;

    /* check state */
    if (strm == Z_NULL || strm->state == Z_NULL) return Z_STREAM_ERROR;
    state = (struct inflate_state*)strm->state;
    if ((state->wrap & 2) == 0) return Z_STREAM_ERROR;

    /* save header structure */
    state->head = head;
    head->done = 0;
    return Z_OK;
}

/*
   Search buf[0..len-1] for the pattern: 0, 0, 0xff, 0xff.  Return when found
   or when out of input.  When called, *have is the number of pattern bytes
   found in order so far, in 0..3.  On return *have is updated to the new
   state.  If on return *have equals four, then the pattern was found and the
   return value is how many bytes were read including the last byte of the
   pattern.  If *have is less than four, then the pattern has not been found
   yet and the return value is len.  In the latter case, syncsearch() can be
   called again with more data and the *have state.  *have is initialized to
   zero for the first call.
 */
unsigned syncsearch(unsigned* have, unsigned char* buf, unsigned len)
{
    unsigned got;
    unsigned next;

    got = *have;
    next = 0;
    while (next < len && got < 4) {
        if ((int)(buf[next]) == (got < 2 ? 0 : 0xff))
            got++;
        else if (buf[next])
            got = 0;
        else
            got = 4 - got;
        next++;
    }
    *have = got;
    return next;
}

int inflateSync(z_streamp strm)
{
    unsigned len;               /* number of bytes to look at or looked at */
    unsigned long in, out;      /* temporary to save total_in and total_out */
    unsigned char buf[4];       /* to restore bit buffer to byte string */
    struct inflate_state* state;

    /* check parameters */
    if (strm == Z_NULL || strm->state == Z_NULL) return Z_STREAM_ERROR;
    state = (struct inflate_state*)strm->state;
    if (strm->avail_in == 0 && state->bits < 8) return Z_BUF_ERROR;

    /* if first time, start search in bit buffer */
    if (state->mode != SYNC) {
        state->mode = SYNC;
        state->hold <<= state->bits & 7;
        state->bits -= state->bits & 7;
        len = 0;
        while (state->bits >= 8) {
            buf[len++] = (unsigned char)(state->hold);
            state->hold >>= 8;
            state->bits -= 8;
        }
        state->have = 0;
        syncsearch(&(state->have), buf, len);
    }

    /* search available input */
    len = syncsearch(&(state->have), strm->next_in, strm->avail_in);
    strm->avail_in -= len;
    strm->next_in += len;
    strm->total_in += len;

    /* return no joy or set up to restart inflate() on a new block */
    if (state->have != 4) return Z_DATA_ERROR;
    in = strm->total_in;  out = strm->total_out;
    inflateReset(strm);
    strm->total_in = in;  strm->total_out = out;
    state->mode = TYPE;
    return Z_OK;
}

/*
   Returns true if inflate is currently at the end of a block generated by
   Z_SYNC_FLUSH or Z_FULL_FLUSH. This function is used by one PPP
   implementation to provide an additional safety check. PPP uses
   Z_SYNC_FLUSH but removes the length bytes of the resulting empty stored
   block. When decompressing, PPP checks that at the end of input packet,
   inflate is waiting for these length bytes.
 */
int inflateSyncPoint(z_streamp strm)
{
    struct inflate_state* state;

    if (strm == Z_NULL || strm->state == Z_NULL) return Z_STREAM_ERROR;
    state = (struct inflate_state*)strm->state;
    return state->mode == STORED && state->bits == 0;
}

int inflateCopy(z_streamp dest, z_streamp source)
{
    struct inflate_state* state;
    struct inflate_state* copy;
    unsigned char* window;
    unsigned wsize;

    /* check input */
    if (dest == Z_NULL || source == Z_NULL || source->state == Z_NULL)
        return Z_STREAM_ERROR;
    state = (struct inflate_state*)source->state;

    /* allocate space */
    copy = (struct inflate_state*)
        ZALLOC(source, 1, sizeof(struct inflate_state));
    if (copy == Z_NULL) return Z_MEM_ERROR;
    window = Z_NULL;
    if (state->window != Z_NULL) {
        window = (unsigned char*)
            ZALLOC(source, 1U << state->wbits, sizeof(unsigned char));
        if (window == Z_NULL) {
            ZFREE(source, copy);
            return Z_MEM_ERROR;
        }
    }

    /* copy state */
    zmemcpy(dest, source, sizeof(z_stream));
    zmemcpy(copy, state, sizeof(struct inflate_state));
    if (state->lencode >= state->codes &&
        state->lencode <= state->codes + ENOUGH - 1) {
        copy->lencode = copy->codes + (state->lencode - state->codes);
        copy->distcode = copy->codes + (state->distcode - state->codes);
    }
    copy->next = copy->codes + (state->next - state->codes);
    if (window != Z_NULL) {
        wsize = 1U << state->wbits;
        zmemcpy(window, state->window, wsize);
    }
    copy->window = window;
    dest->state = (struct internal_state*)copy;
    return Z_OK;
}


//----------------------------------------------------------------------------------------------------
//
// 上面是zip解压
// 下面是读取png
//
//----------------------------------------------------------------------------------------------------

void pngMemcpy(void* p1, const void* p2, unsigned int len)
{
    char* c1 = (char*)p1;
    const char* c2 = (const char*)p2;
    for (unsigned int i = 0; i < len; i++)
        c1[i] = c2[i];
}
unsigned int pngReadBit(const char* p, int* seek, int* bit_locate, int bitSize)
{
    unsigned int r = 0;
    for (int i = 0; i < bitSize; i++)
    {
        int k = (p[*seek] >> (7 - (*bit_locate))) & 1;
        r |= k << (bitSize - i - 1);
        (*bit_locate)++;
        if (*bit_locate >= 8)
        {
            (*seek)++;
            *bit_locate = 0;
        }
    }
    return r;
}
CDymPng::CDymPng()
{
    m_tIHDR.Width = 0;
    m_tIHDR.Height = 0;
    m_tIHDR.BitDepth = 0;
    m_tIHDR.ColorType = 0;
    m_tIHDR.CompressionMethod = 0;
    m_tIHDR.FilterMethod = 0;
    m_tIHDR.InterlaceMethod = 0;
    m_pPixelData = 0;
    m_nOutputLocate = 0;
    m_nWritePixel = 0;
    m_nScanline = 0;
    m_pDataBuffer = 0;
    m_nDataBufferSize = 0;
    m_pDataBuffer2 = 0;
    m_nDataBufferSize2 = 0;
}
CDymPng::~CDymPng()
{
}
unsigned int CDymPng::ReadInt(const void* pData, int& seek)
{
    unsigned int a = ((const unsigned char*)pData)[seek++];
    unsigned int b = ((const unsigned char*)pData)[seek++];
    unsigned int c = ((const unsigned char*)pData)[seek++];
    unsigned int d = ((const unsigned char*)pData)[seek++];
    return (a << 24) | (b << 16) | (c << 8) | d;
}
unsigned char CDymPng::ReadChar(const void* pData, int& seek)
{
    return ((const unsigned char*)pData)[seek++];
}
#include <stdio.h>
int CDymPng::LoadFromFileA(const char* strFileName)
{
    FILE* p = NULL;
    p = fopen(strFileName, "rb");
    if (p)
    {
        fseek(p, 0, SEEK_END);
        unsigned int len = ftell(p);
        fseek(p, 0, SEEK_SET);
        char* pbuf = new char[len];
        fread(pbuf, len, 1, p);
        fclose(p);
        int tReturn = LoadFromMemory(pbuf, len);
        delete[] pbuf;
        return tReturn;
    }
    return 0;
}

void DecompressZipOutDataStream(void* pThis, void* pData, int size)
{
    ((CDymPng*)pThis)->_DecompressZipOutDataStream(pData, size);
}

void CDymPng::_FilterRow(unsigned char* pData, int start)
{
#define PNG_FILTER_VALUE_NONE  0
#define PNG_FILTER_VALUE_SUB   1
#define PNG_FILTER_VALUE_UP    2
#define PNG_FILTER_VALUE_AVG   3
#define PNG_FILTER_VALUE_PAETH 4
#define PNG_FILTER_VALUE_LAST  5

    unsigned int rowbytes = (unsigned int)m_tIHDR.Width;
    unsigned int pixel_depth = (unsigned int)m_tIHDR.BitDepth;
    if (m_tIHDR.ColorType == 6)
    {
        rowbytes *= 4;
        pixel_depth *= 4;
    }
    else if (m_tIHDR.ColorType == 2)
    {
        rowbytes *= 3;
        pixel_depth *= 3;
    }
    unsigned char* row = &pData[start + 1];
    unsigned char* prev_row = row - rowbytes - 1;

    switch (pData[start])
    {
    case PNG_FILTER_VALUE_NONE:
        break;
    case PNG_FILTER_VALUE_SUB:
    {
        unsigned int i;
        unsigned int istop = rowbytes;
        unsigned int bpp = (pixel_depth + 7) >> 3;
        unsigned char* rp = row + bpp;
        unsigned char* lp = row;

        for (i = bpp; i < istop; i++)
        {
            *rp = (unsigned char)(((int)(*rp) + (int)(*lp++)) & 0xff);
            rp++;
        }
        break;
    }
    case PNG_FILTER_VALUE_UP:
    {
        unsigned int i;
        unsigned int istop = rowbytes;
        unsigned char* rp = row;
        unsigned char* pp = prev_row;

        for (i = 0; i < istop; i++)
        {
            *rp = (unsigned char)(((int)(*rp) + (int)(*pp++)) & 0xff);
            rp++;
        }
        break;
    }
    case PNG_FILTER_VALUE_AVG:
    {
        unsigned int i;
        unsigned char* rp = row;
        unsigned char* pp = prev_row;
        unsigned char* lp = row;
        unsigned int bpp = (pixel_depth + 7) >> 3;
        unsigned int istop = rowbytes - bpp;

        for (i = 0; i < bpp; i++)
        {
            *rp = (unsigned char)(((int)(*rp) +
                ((int)(*pp++) / 2)) & 0xff);
            rp++;
        }

        for (i = 0; i < istop; i++)
        {
            *rp = (unsigned char)(((int)(*rp) +
                (int)(*pp++ + *lp++) / 2) & 0xff);
            rp++;
        }
        break;
    }
    case PNG_FILTER_VALUE_PAETH:
    {
        unsigned int i;
        unsigned char* rp = row;
        unsigned char* pp = prev_row;
        unsigned char* lp = row;
        unsigned char* cp = prev_row;
        unsigned int bpp = (pixel_depth + 7) >> 3;
        unsigned int istop = rowbytes - bpp;

        for (i = 0; i < bpp; i++)
        {
            *rp = (unsigned char)(((int)(*rp) + (int)(*pp++)) & 0xff);
            rp++;
        }

        for (i = 0; i < istop; i++)   /* Use leftover rp,pp */
        {
            int a, b, c, pa, pb, pc, p;

            a = *lp++;
            b = *pp++;
            c = *cp++;

            p = b - c;
            pc = a - c;

#ifdef PNG_USE_ABS
            pa = abs(p);
            pb = abs(pc);
            pc = abs(p + pc);
#else
            pa = p < 0 ? -p : p;
            pb = pc < 0 ? -pc : pc;
            pc = (p + pc) < 0 ? -(p + pc) : p + pc;
#endif

            /*
               if (pa <= pb && pa <= pc)
                  p = a;
               else if (pb <= pc)
                  p = b;
               else
                  p = c;
             */

            p = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;

            *rp = (unsigned char)(((int)(*rp) + p) & 0xff);
            rp++;
        }
        break;
    }
    default:
        //png_warning(png_ptr, "Ignoring bad adaptive filter type");
        *row = 0;
        break;
    }
}
void CDymPng::_DecompressZipOutDataStream(void* pData, int size)
{
    unsigned char* p = new unsigned char[m_nDataBufferSize + size];
    for (int i = 0; i < m_nDataBufferSize; i++)
        p[i] = m_pDataBuffer[i];
    for (int i = m_nDataBufferSize; i < m_nDataBufferSize + size; i++)
        p[i] = ((unsigned char*)pData)[i - m_nDataBufferSize];
    if (m_pDataBuffer) delete[] m_pDataBuffer;
    m_pDataBuffer = p;
    m_nDataBufferSize = m_nDataBufferSize + size;
}
void CDymPng::_DecompressZipOutDataStream2(void* pData, int size)
{
    m_nOutputLocate = 0;
    if (m_tIHDR.ColorType == 3)// 索引色
    {
        if (m_tIHDR.BitDepth == 4)
        {
            for (int i = 0; i < size; i++)
            {
                if (m_nScanline % ((m_tIHDR.Width + 1) / 2 + 1) == 0)
                {
                    if (((unsigned char*)pData)[m_nOutputLocate])
                        _FilterRow((unsigned char*)pData, m_nOutputLocate);
                    m_nScanline++;
                    m_nOutputLocate++;

                    if (m_tIHDR.Width % 2 == 1 && m_nWritePixel > 0)
                        m_nWritePixel--;
                    continue;
                }
                m_nScanline++;

                m_pPixelData[m_nWritePixel++] = m_dwPaletteColor[(((unsigned char*)pData)[m_nOutputLocate] & 0xF0) >> 4];
                m_pPixelData[m_nWritePixel++] = m_dwPaletteColor[((unsigned char*)pData)[m_nOutputLocate] & 0x0F];

                m_nOutputLocate++;
            }
        }
        else if (m_tIHDR.BitDepth == 8)
        {
            for (int i = 0; i < size; i++)
            {
                if (m_nScanline % (m_tIHDR.Width + 1) == 0)
                {
                    if (((unsigned char*)pData)[m_nOutputLocate])
                        _FilterRow((unsigned char*)pData, m_nOutputLocate);
                    m_nScanline++;
                    m_nOutputLocate++;
                    continue;
                }
                m_nScanline++;

                m_pPixelData[m_nWritePixel++] = m_dwPaletteColor[(((unsigned char*)pData)[m_nOutputLocate])];

                m_nOutputLocate++;
            }
        }
    }
    else if (m_tIHDR.ColorType == 6)// 带alpha真彩
    {
        if (m_tIHDR.BitDepth == 8)
        {
            for (int i = 0; i < size; i++)
            {
                if (m_nScanline % (m_tIHDR.Width * 4 + 1) == 0)
                {
                    if (((unsigned char*)pData)[m_nOutputLocate])
                        _FilterRow((unsigned char*)pData, m_nOutputLocate);
                    m_nScanline++;
                    m_nOutputLocate++;
                    continue;
                }
                m_nScanline++;

                unsigned int color = (unsigned int)((unsigned char*)pData)[m_nOutputLocate];
                if (m_nWritePixel % 4 == 0)
                    m_pPixelData[m_nWritePixel / 4] = color << 16;
                else if (m_nWritePixel % 4 == 1)
                    m_pPixelData[m_nWritePixel / 4] |= color << 8;
                else if (m_nWritePixel % 4 == 2)
                    m_pPixelData[m_nWritePixel / 4] |= color;
                else if (m_nWritePixel % 4 == 3)
                    m_pPixelData[m_nWritePixel / 4] |= color << 24;

                m_nWritePixel++;
                m_nOutputLocate++;
            }
        }
    }
    else if (m_tIHDR.ColorType == 2)// 真彩
    {
        if (m_tIHDR.BitDepth == 8)
        {
            for (int i = 0; i < size; i++)
            {
                if (m_nScanline % (m_tIHDR.Width * 3 + 1) == 0)
                {
                    if (((unsigned char*)pData)[m_nOutputLocate])
                        _FilterRow((unsigned char*)pData, m_nOutputLocate);
                    m_nScanline++;
                    m_nOutputLocate++;
                    continue;
                }
                m_nScanline++;

                unsigned int color = (unsigned int)((unsigned char*)pData)[m_nOutputLocate];
                if (m_nWritePixel % 3 == 0)
                    m_pPixelData[m_nWritePixel / 3] = color << 16;
                else if (m_nWritePixel % 3 == 1)
                    m_pPixelData[m_nWritePixel / 3] |= color << 8;
                else if (m_nWritePixel % 3 == 2)
                    m_pPixelData[m_nWritePixel / 3] |= color;

                m_nWritePixel++;
                m_nOutputLocate++;
            }
        }
    }
}
#define PNG_ROWBYTES(pixel_bits, width) \
    ((pixel_bits) >= 8 ? \
    ((width) * (((unsigned int)(pixel_bits)) >> 3)) : \
    (( ((width) * ((unsigned int)(pixel_bits))) + 7) >> 3) )

int CDymPng::LoadFromMemory(const void* pData, unsigned int datasize)
{
    if (!isPng(pData, datasize)) return 0;
    const char* p = (const char*)pData;
    m_nOutputLocate = 0;
    m_nWritePixel = 0;
    m_nScanline = 0;
    int seek = 0;
    m_pDataBuffer2 = NULL;
    m_nDataBufferSize2 = 0;

    // 读取文件头
    pngMemcpy(&m_tPngHeader, &p[seek], sizeof(m_tPngHeader));
    seek += sizeof(m_tPngHeader);

    while (seek < (int)datasize)
    {
        // 读取块大小
        unsigned int size = ReadInt(p, seek);
        // 读取块标识
        char ChunkType[4];
        ChunkType[0] = ReadChar(p, seek);
        ChunkType[1] = ReadChar(p, seek);
        ChunkType[2] = ReadChar(p, seek);
        ChunkType[3] = ReadChar(p, seek);

        int t = seek;
        // 解压IDAT数据
        if (m_pDataBuffer2 && (ChunkType[0] != 'I' || ChunkType[1] != 'D' || ChunkType[2] != 'A' || ChunkType[3] != 'T'))
        {
            /*unsigned char a1 = m_pDataBuffer2[0];// 0x78
            unsigned char a2 = m_pDataBuffer2[1];// 0xDA or 0xC9
            m_pDataBuffer = NULL;
            m_nDataBufferSize = 0;
            inflate( &m_pDataBuffer2[2] , m_nDataBufferSize2-2 , DecompressZipOutDataStream , this );
            if( m_pDataBuffer )
            {
                _DecompressZipOutDataStream2( m_pDataBuffer , m_nDataBufferSize );
                delete[] m_pDataBuffer;
            }
            m_pDataBuffer = NULL;
            m_nDataBufferSize = 0;
            delete[] m_pDataBuffer2;
            m_pDataBuffer2 = NULL;
            m_nDataBufferSize2 = 0;*/

            // 计算每一行所需要的字节
            int channels = 0;
            switch (m_tIHDR.ColorType)
            {
            case 0:// 灰度图像
            case 3:// 索引彩色图像
                channels = 1;
                break;
            case 2:// 真彩色图像
                channels = 3;
                break;
            case 4:// 带α通道数据的灰度图像
                channels = 2;
                break;
            case 6:// 带α通道数据的真彩色图像
                channels = 4;
                break;
            }
            int row_bytes = PNG_ROWBYTES((m_tIHDR.BitDepth * channels), m_tIHDR.Width) + 1;
            z_stream strm;
            ::zmemzero(&strm, sizeof(strm));
            strm.next_in = m_pDataBuffer2;
            strm.avail_in = m_nDataBufferSize2;
            strm.avail_out = row_bytes * m_tIHDR.Height;
            m_pDataBuffer = new unsigned char[row_bytes * m_tIHDR.Height];
            m_nDataBufferSize = row_bytes * m_tIHDR.Height;
            strm.next_out = m_pDataBuffer;
            ::inflateInit_(&strm, ZLIB_VERSION, sizeof(strm));
            int ret = inflate2(&strm, Z_PARTIAL_FLUSH);
            //_DecompressZipOutDataStream( pBuf , row_bytes*m_tIHDR.Height );

            if (m_pDataBuffer)
            {
                _DecompressZipOutDataStream2(m_pDataBuffer, m_nDataBufferSize);
                delete[] m_pDataBuffer;
            }
            m_pDataBuffer = NULL;
            m_nDataBufferSize = 0;
            delete[] m_pDataBuffer2;
            m_pDataBuffer2 = NULL;
            m_nDataBufferSize2 = 0;
        }
        // 文件头块
        if (ChunkType[0] == 'I' && ChunkType[1] == 'H' && ChunkType[2] == 'D' && ChunkType[3] == 'R')
        {
            int s = sizeof(tPng_IHDR);
            m_tIHDR.Width = ReadInt(p, t);
            m_tIHDR.Height = ReadInt(p, t);
            m_tIHDR.BitDepth = ReadChar(p, t);
            m_tIHDR.ColorType = ReadChar(p, t);
            m_tIHDR.CompressionMethod = ReadChar(p, t);
            m_tIHDR.FilterMethod = ReadChar(p, t);
            m_tIHDR.InterlaceMethod = ReadChar(p, t);
            m_pPixelData = new unsigned int[m_tIHDR.Width * m_tIHDR.Height];
        }
        // 调色板块
        else if (ChunkType[0] == 'P' && ChunkType[1] == 'L' && ChunkType[2] == 'T' && ChunkType[3] == 'E')
        {
            for (int i = 0; i < (int)size / 3; i++)
            {
                int r = (int)ReadChar(p, t);
                int g = (int)ReadChar(p, t);
                int b = (int)ReadChar(p, t);
                m_dwPaletteColor[i] = 0xFF000000 | (r << 16) | (g << 8) | b;
            }
        }
        // 透明数据
        else if (ChunkType[0] == 't' && ChunkType[1] == 'R' && ChunkType[2] == 'N' && ChunkType[3] == 'S')
        {
            for (int i = 0; i < (int)size; i++)
            {
                int a = (int)ReadChar(p, t);
                m_dwPaletteColor[i] = (a << 24) | (0x00FFFFFF & m_dwPaletteColor[i]);
            }
        }
        // 数据块
        else if (ChunkType[0] == 'I' && ChunkType[1] == 'D' && ChunkType[2] == 'A' && ChunkType[3] == 'T')
        {
            unsigned char* p2 = new unsigned char[m_nDataBufferSize2 + size];
            for (int i = 0; i < m_nDataBufferSize2; i++)
                p2[i] = m_pDataBuffer2[i];
            for (int i = m_nDataBufferSize2; i < m_nDataBufferSize2 + (int)size; i++)
                p2[i] = p[i - m_nDataBufferSize2 + t];
            if (m_pDataBuffer2) delete[] m_pDataBuffer2;
            m_pDataBuffer2 = p2;
            m_nDataBufferSize2 = m_nDataBufferSize2 + size;
        }
        // 结束符
        else if (ChunkType[0] == 'I' && ChunkType[1] == 'E' && ChunkType[2] == 'N' && ChunkType[3] == 'D')
        {
            return 1;
        }

        seek += size;
        unsigned int crc = (unsigned int)ReadInt(p, seek);
    }
    return 1;
}
bool CDymPng::isPng(const void* pData, unsigned int datasize)
{
    if (datasize >= 8)
    {
        const unsigned char* p = (const unsigned char*)pData;
        if (p[0] == 137 && p[1] == 80 && p[2] == 78 && p[3] == 71 && p[4] == 13 && p[5] == 10 && p[6] == 26 && p[7] == 10)
            return true;
    }
    return false;
}
int CDymPng::Width()
{
    return m_tIHDR.Width;
}
int CDymPng::Height()
{
    return m_tIHDR.Height;
}
unsigned int CDymPng::gifGetPixel(int x, int y, int frame)
{
    return m_pPixelData[y * m_tIHDR.Width + x];
}