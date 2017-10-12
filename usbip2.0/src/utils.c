
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "usbip_common.h"
#include "utils.h"
#include "sysfs_utils.h"

int modify_match_busid(char *busid, int add)
{
	char attr_name[] = "match_busid";
	char command[SYSFS_BUS_ID_SIZE + 4];
	char match_busid_attr_path[SYSFS_PATH_MAX];
	int rc;

	snprintf(match_busid_attr_path, sizeof(match_busid_attr_path),
		 "%s/%s/%s/%s/%s/%s", SYSFS_MNT_PATH, SYSFS_BUS_NAME,
		 SYSFS_BUS_TYPE, SYSFS_DRIVERS_NAME, USBIP_HOST_DRV_NAME,
		 attr_name);

	if (add)
		snprintf(command, SYSFS_BUS_ID_SIZE + 4, "add %s", busid);
	else
		snprintf(command, SYSFS_BUS_ID_SIZE + 4, "del %s", busid);

	rc = write_sysfs_attribute(match_busid_attr_path, command,
				   sizeof(command));
	if (rc < 0) {
		dbg("failed to write match_busid: %s", strerror(errno));
		return -1;
	}

	return 0;
}
