#include "i8042.h"
#include "test4.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static short parse_short(char *str, int base);
static unsigned short parse_ushort(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char *argv[]) {
	sef_startup();
	/* DO NOT FORGET TO initialize service */
	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}


static void print_usage(char **argv)
{
  printf("Usage: one of the following:\n"
  			"\t service run %s -args \"packet <count>\" \n"
        "\t service run %s -args \"remote <period> <count>\" \n"
  			"\t service run %s -args \"async <idle_time>\" \n"
  			"\t service run %s -args \"gesture <length>\" \n",
        argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv) {
	unsigned short cnt, idle_time;
  unsigned long period;
	short length;

	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for test_packet()\n");
			return 1;
		}
		cnt = parse_ushort(argv[2], 10);
		if (cnt == USHRT_MAX)
			return 1;
		if (cnt < 0) {
			printf("mouse: counter must be positive\n");
			return 1;
		}
		printf("mouse::mouse_test_packet(%d)\n", cnt);
		return mouse_test_packet(cnt);


  } else if (strncmp(argv[1], "remote", strlen("remote")) == 0) {
		if (argc != 4) {
			printf("mouse: wrong no. of arguments for test_remote()\n");
			return 1;
		}
		period = parse_ulong(argv[2], 10);
		if (period == ULONG_MAX)
		return 1;
    if (period < 0) {
			printf("mouse: period must be positive\n");
			return 1;
		}
    cnt = parse_ushort(argv[3], 10);
		if (cnt == USHRT_MAX)
			return 1;
		if (cnt < 0) {
			printf("mouse: counter must be positive\n");
			return 1;
		}
		printf("mouse::mouse_test_remote(%d, %d)\n", period, cnt);
		return mouse_test_remote(period, cnt);


	} else if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for test_async()\n");
			return 1;
		}
		idle_time = parse_ushort(argv[2], 10);
		if (idle_time == USHRT_MAX)
			return 1;
		if (idle_time < 0) {
			printf("mouse: time must be positive\n");
			return 1;
		}
		printf("mouse::mouse_test_async(%d)\n", idle_time);
		return mouse_test_async(idle_time);


	} else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for test_config()\n");
			return 1;
		}
		length = parse_short(argv[2], 10);
    if (length == SHRT_MAX)
			return 1;
		if (length < 0) {
			printf("mouse: length must be positive\n");
			return 1;
		}
		printf("mouse::mouse_test_gesture(%d)\n", length);
		return mouse_test_gesture(length);


	} else {
		printf("mouse: %s - no valid function!\n", argv[1]);
		return 1;
	}
}


static unsigned long parse_ulong(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("kbd: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

static unsigned short parse_ushort(char *str, int base) {
	char *endptr;
	unsigned short val;

	/* Convert string to unsigned short */
	val = (unsigned short) strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == USHRT_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return USHRT_MAX;
	}

	if (endptr == str) {
		printf("kbd: parse_ushort: no digits were found in %s\n", str);
		return USHRT_MAX;
	}

	/* Successful conversion */
	return val;
}

static short parse_short(char *str, int base) {
	char *endptr;
	short val;

	/* Convert string to unsigned short */
	val = (short) strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == SHRT_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return SHRT_MAX;
	}

	if (endptr == str) {
		printf("kbd: parse_short: no digits were found in %s\n", str);
		return SHRT_MAX;
	}

	/* Successful conversion */
	return val;
}
