## LIFX DEVICE FORK
* Need someone to help me out with this.
https://hyperion-project.org/threads/lifx-support.2999/
https://community.lifx.com/t/lifx-device-addition-to-hyperion/6725






**Multizone**
MultiZone lights are single devices that contain more than a single light source. The first such device is the LIFX Z Light Strip. To discover the MultiZone capability on your devices refer to the details on the LIFX Products page. We also provide a machine readable version of the product database for embedding inside your applications on Github.


https://lan.developer.lifx.com/docs/multizone-messages
This is the new API for LIFX Z/Beam and is only supported by the second generation that have a new enough firmware on them. The firmware version for a product that enables this API can always be found in the product definition found at https://github.com/LIFX/products



**Groups and Locations - https://lan.developer.lifx.com/docs/groups-and-locations

*The LIFX Application allows users to organise each bulb by giving it a group and a location. Most users use groups to represent a room, and location to represent a property (such as a house or office) but this does not need to be the case. These groups and locations are stored on the bulbs themselves, and are designed so that they can be modified or renamed as long as one of the bulbs in the affected group is powered on.

*Light (bulb/led strip) => Product (Device) => Group => Location*

Hierarchy:
- Location
	-Group
		-Device
		-Device
	-Group
		-Device


**GetVersion**
Get the hardware version. No payload is required. Causes the device to transmit a *StateVersion message*.Provides the hardware version of the device.

  |  
----- | ----
vendor: | vendor ID
product: | product ID
version: | hardware version

Field | Type
----- | ----
vendor | unsigned 32-bit integer
product | unsigned 32-bit integer
version | unsigned 32-bit integer

*Lifx devices* https://lan.developer.lifx.com/docs/lifx-products
*Machine readable version of this table on GitHub* https://github.com/LIFX/products/blob/master/products.json

**GetService**
Sent by a client to acquire responses from all devices on the local network. No payload is required. Causes the devices to transmit a *StateService message*. When using this message the Frame tagged field must be set to one (1).

If the Service is temporarily unavailable, then the port value will be 0.

Field | Type
----- | ----
service | unsigned 8-bit integer, maps to Service
port | unsigned 32-bit integer

**GetLabel**
Get device label. No payload is required. Causes the device to transmit a *StateLabel message*.

Field | Type
----- | ----
label | string, size: 32 bytes

**GetLocation**
Ask the bulb to return its location information.No payload is required. Causes the device to transmit a *StateLocation message*.

Field | Type
----- | ----
location | byte array, size: 16 bytes
label | string, size: 32 bytes
updated_at | unsigned 64-bit integer

**GetGroup**
Ask the bulb to return its group membership information. No payload is required.Causes the device to transmit a *StateGroup message*.

Field | Type
----- | ----
group | byte array, size: 16 bytes
label | string, size: 32 bytes
updated_at | unsigned 64-bit integer