## LIFX DEVICE FORK
* Need someone to help me out with this.
https://hyperion-project.org/threads/lifx-support.2999/
https://community.lifx.com/t/lifx-device-addition-to-hyperion/6725


Lifx devices 
https://lan.developer.lifx.com/docs/lifx-products

Machine readable version of this table on GitHub
https://github.com/LIFX/products/blob/master/products.json

GetVersion message to retrieve a device's Vendor and Product IDs.
https://lan.developer.lifx.com/v2.0/docs/device-messages#section-getversion-32

Multizone
MultiZone lights are single devices that contain more than a single light source. The first such device is the LIFX Z Light Strip. To discover the MultiZone capability on your devices refer to the details on the LIFX Products page. We also provide a machine readable version of the product database for embedding inside your applications on Github.


https://lan.developer.lifx.com/docs/multizone-messages
This is the new API for LIFX Z/Beam and is only supported by the second generation that have a new enough firmware on them. The firmware version for a product that enables this API can always be found in the product definition found at https://github.com/LIFX/products


