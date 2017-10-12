
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>
#include <unistd.h>

#include "vhci_driver.h"
#include "usbip_common.h"
#include "usbip_network.h"
#include "usbip.h"

static const char usbip_detach_usage_string[] =
	"usbip detach <args>\n"
	"    -p, --port=<port>    " USBIP_VHCI_DRV_NAME
	" port the device is on\n";

void usbip_detach_usage(void)
{
	printf("usage: %s", usbip_detach_usage_string);
}

static int detach_port(char *port)
{
	int ret;
	uint8_t portnum;
	char path[PATH_MAX+1];

	unsigned int port_len = strlen(port);

	for (unsigned int i = 0; i < port_len; i++)
		if (!isdigit(port[i])) {
			err("invalid port %s", port);
			return -1;
		}

	/* check max port */

	portnum = atoi(port);

	/* remove the port state file */

	snprintf(path, PATH_MAX, VHCI_STATE_PATH"/port%d", portnum);

	remove(path);
	rmdir(VHCI_STATE_PATH);

	ret = usbip_vhci_driver_open();
	if (ret < 0) {
		err("open vhci_driver");
		return -1;
	}

	ret = usbip_vhci_detach_device(portnum);
	if (ret < 0)
		return -1;

	usbip_vhci_driver_close();

	return ret;
}

int usbip_detach(int argc, char *argv[])
{
	static const struct option opts[] = {
		{ "port", required_argument, NULL, 'p' },
		{ NULL, 0, NULL, 0 }
	};
	int opt;
	int ret = -1;

	for (;;) {
		opt = getopt_long(argc, argv, "p:", opts, NULL);

		if (opt == -1)
			break;

		switch (opt) {
		case 'p':
			ret = detach_port(optarg);
			goto out;
		default:
			goto err_out;
		}
	}

err_out:
	usbip_detach_usage();
out:
	return ret;
}
