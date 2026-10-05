#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

/* mac rd, rs1, rs2 : rd = rd + rs1 * rs2  (custom-0, R-type, funct3=0, funct7=0) */
static inline uint32_t mac_insn(uint32_t acc, uint32_t a, uint32_t b)
{
#if defined(__riscv)
    __asm__ volatile(".insn r 0x0b, 0, 0, %0, %1, %2"
                     : "+r"(acc)
                     : "r"(a), "r"(b));
    return acc;
#else
    return acc + a * b;
#endif
}

int32_t dot_ref(const int32_t *a, const int32_t *b, size_t n)
{
    uint32_t acc = 0;
    for (size_t i = 0; i < n; i++)
        acc = acc + (uint32_t)a[i] * (uint32_t)b[i];
    return (int32_t)acc;
}

int32_t dot_mac(const int32_t *a, const int32_t *b, size_t n)
{
    uint32_t acc = 0;
    for (size_t i = 0; i < n; i++)
        acc = mac_insn(acc, (uint32_t)a[i], (uint32_t)b[i]);
    return (int32_t)acc;
}

#ifndef __riscv
static int check(const char *name, const int32_t *a, const int32_t *b, size_t n)
{
    int32_t r = dot_ref(a, b, n), m = dot_mac(a, b, n);
    printf("%-14s n=%-3zu ref=%-12d mac=%-12d %s\n", name, n, r, m, r == m ? "OK" : "FAIL");
    return r == m;
}

int main(void)
{
    int32_t a1[] = {1, 2, 3, 4};
    int32_t b1[] = {5, 6, 7, 8};
    int32_t a2[] = {-3, 4, -5};
    int32_t b2[] = {7, -8, 9};
    int32_t a3[] = {0x7FFFFFFF, 0x7FFFFFFF};
    int32_t b3[] = {0x7FFFFFFF, 2};

    int ok = 1;
    ok &= check("basic", a1, b1, 4);
    ok &= check("signed", a2, b2, 3);
    ok &= check("overflow", a3, b3, 2);
    ok &= check("empty", a1, b1, 0);
    return ok ? 0 : 1;
}
#endif
