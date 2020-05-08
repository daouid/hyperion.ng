#pragma once

//including way too much , just copying all from other leddevice headers
#include <leddevice/LedDevice.h>
#include "ProviderUdp.h"
#include <ssdp/SSDPDiscover.h>
#include <utils/Logger.h>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <exception>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <QString>
#include <QStringList>
#include <QUdpSocket>
#include <QHostInfo>
#include <QHostAddress>
#include <QVector>
#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include <set>

// lifx device port
const ushort LIFX_DEFAULT_PORT = 56700;

// lifx device discovery:
/// The protocol documentation includes a section on workflows including how to do discovery. Essentially it works like this: Send a Device::GetService packet to the broadcast address (255.255.255.255) on port 56700
/// Each bulb will respond with a list of service they support as a Device::StateService 23 message. The only service currently documented is UDP which has its type field set to 1. When your application receives one of these messages it should store the sender address and the service port internally. This will constitute the list of LIFX devices that you have found. source: https://community.lifx.com/t/discovering-lifx-bulbs/265
///The basic premise is you create a UDP socket and then throw bytes at it. It’s recommended you only broadcast the GetService 2 to 255.255.255.255 and use the StateService replies to know what devices are on your network.

/// Then you “unicast” bytes to specific devices, which is the same as a broadcast, but instead of sending to 255.255.255.255 you send to the specific IP of the device you want to talk to. https://community.lifx.com/t/how-to-send-a-packet-to-a-lifx-bulb-using-the-lifx-lan-protocol/6502


static const char LIFX_DEFAULT_HOST[] = "255.255.255.255";

/// source of below: https://gist.github.com/smarthall/875301eb5a6b35378aec#file-lifx-h
/// mixed with vs hyperion's LedDeviceUdpArtNet.h
	
typedef union
{
	struct {
		/* frame */
		uint16_t	size;
		uint16_t	protocol:12;
		uint8_t		addressable:1;
		uint8_t		tagged:1;
		uint8_t		origin:2;
		uint32_t	source;
		/* frame address */
		uint8_t		target[8];
		uint8_t		reserved[6];
		uint8_t		res_required:1;
		uint8_t		ack_required:1;
		uint8_t		:6;
		uint8_t		sequence;
		/* protocol header */
		uint64_t	:64;
		uint16_t	type;
		uint16_t	:16;
		/* variable length payload follows */
		
	} __attribute__((packed));
	
} lifx_packet_t;


/// Implementation of the LedDevice interface for sending led colors via udp.
///
class LedDeviceLifx : public ProviderUdp
{
public:
	///
	/// Constructs the LedDevice for Lifx LightPanels
	///
	/// @param deviceConfig json config for Lifx
	///
	explicit LedDeviceLifx(const QJsonObject &deviceConfig);

	/// Constructs leddevice
	static LedDevice* construct(const QJsonObject &deviceConfig);
	
	///
	/// Sets configuration
	///
	/// @param deviceConfig the json device config
	/// @return true if success
	bool init(const QJsonObject &deviceConfig) override;

private:
	///
	/// Writes the led color values to the led-device
	///
	/// @param ledValues The color-value per led
	/// @return Zero on succes else negative
	///
	virtual int write(const std::vector<ColorRgb> & ledValues) override;
};
