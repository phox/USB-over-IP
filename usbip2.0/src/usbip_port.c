
#include "vhci_driver.h"
#include "usbip_common.h"

static int list_imported_devices(void)
{
	int i;
	struct usbip_imported_device *idev;
	int ret;

	ret = usbip_vhci_driver_open();
	if (ret < 0) {
		err("open vhci_driver");
		return -1;
	}

	printf("Imported USB devices\n");
	printf("====================\n");

	for (i = 0; i < vhci_driver->nports; i++) {
		idev = &vhci_driver->idev[i];

		if (usbip_vhci_imported_device_dump(idev) < 0)
			ret = -1;
	}

	usbip_vhci_driver_close();

	return ret;

}

int usbip_port_show(__attribute__((unused)) int argc,
		    __attribute__((unused)) char *argv[])
{
	int ret;

	ret = list_imported_devices();
	if (ret < 0)
		err("list imported devices");

	return ret;
}
