#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <monetary.h>
#include <errno.h>
#include <stdarg.h>

static ssize_t vstrfmon_l(char *s, size_t n, locale_t loc, const char *fmt, va_list ap)
{
	size_t l;
	double x;
	int left;
	int lp, rp, w, fw;
	char *s0=s;
	for (; n && *fmt; ) {
		if (*fmt != '%') {
		literal:
			*s++ = *fmt++;
			n--;
			continue;
		}
		fmt++;
		if (*fmt == '%') goto literal;

		left = 0;
		for (; ; fmt++) {
			switch (*fmt) {
			case '=':
				continue;
			case '^':
				continue;
			case '(':
			case '+':
				continue;
			case '!':
				continue;
			case '-':
				left = 1;
				continue;
			}
			break;
		}

		for (fw=0; isdigit(*fmt); fmt++)
			fw = 10*fw + (*fmt-'0');
		lp = 0;
		rp = 2;
		if (*fmt=='#') for (lp=0, fmt++; isdigit(*fmt); fmt++)
			lp = 10*lp + (*fmt-'0');
		if (*fmt=='.') for (rp=0, fmt++; isdigit(*fmt); fmt++)
			rp = 10*rp + (*fmt-'0');

		w = lp + 1 + rp;
		if (!left && fw>w) w = fw;

		x = va_arg(ap, double);
		l = snprintf(s, n, "%*.*f", w, rp, x);
		if (l >= n) {
			errno = E2BIG;
			return -1;
		}
		s += l;
		n -= l;
	}
	return s-s0;
}

ssize_t strfmon_l(char *restrict s, size_t n, locale_t loc, const char *restrict fmt, ...)
{
	va_list ap;
	ssize_t ret;

	va_start(ap, fmt);
	ret = vstrfmon_l(s, n, loc, fmt, ap);
	va_end(ap);

	return ret;
}


ssize_t strfmon(char *restrict s, size_t n, const char *restrict fmt, ...)
{
	va_list ap;
	ssize_t ret;

	va_start(ap, fmt);
	ret = vstrfmon_l(s, n, 0, fmt, ap);
	va_end(ap);

	return ret;
}
