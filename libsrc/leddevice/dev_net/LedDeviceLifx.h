#pragma once

//including way too much , just copying all from other leddevice headers
#include <leddevice/LedDevice.h>
#include <utils/Logger.h>
#include <ssdp/SSDPDiscover.h>
#include "ProviderUdp.h"

#include <cstring>
#include <cstdio>
#include <exception>
#include <set>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdint.h>

#include <sstream>
#include <iomanip>

#include <iostream>

#include <QEventLoop>
#include <QHostInfo>
#include <QHostAddress>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QUuid>
#include <QVector>



// lifx device discovery:
/// The protocol documentation includes a section on workflows including how to do discovery. Essentially it works like this: Send a Device::GetService packet to the broadcast address (255.255.255.255) on port 56700
/// Each bulb will respond with a list of service they support as a Device::StateService message. The only service currently documented is UDP which has its type field set to 1. When your application receives one of these messages it should store the sender address and the service port internally. This will constitute the list of LIFX devices that you have found. source: https://community.lifx.com/t/discovering-lifx-bulbs/265
///The basic premise is you create a UDP socket and then throw bytes at it. It’s recommended you only broadcast the GetService to 255.255.255.255 and use the StateService replies to know what devices are on your network.

/// Then you “unicast” bytes to specific devices, which is the same as a broadcast, but instead of sending to 255.255.255.255 you send to the specific IP of the device you want to talk to. https://community.lifx.com/t/how-to-send-a-packet-to-a-lifx-bulb-using-the-lifx-lan-protocol/6502

// lifx device port
const ushort LIFX_DEFAULT_PORT = 56700;
static const char LIFX_DEFAULT_HOST[] = "255.255.255.255";

class QUdpSocket;

/**
 * Implementation for the AtmoOrb
 *
 * To use set the device to "atmoorb".
 *
 * @author RickDB (github)
 */
class LedDeviceLifx : public LedDevice
{
	Q_OBJECT
public:

	///
	/// Constructs specific LedDevice
	///
	/// @param deviceConfig json device config
	///
	explicit LedDeviceLifx(const QJsonObject &deviceConfig);

	///
	/// Sets configuration
	///
	/// @param deviceConfig the json device config
	/// @return true if success
	bool init(const QJsonObject &deviceConfig) override;

	/// constructs leddevice
	static LedDevice* construct(const QJsonObject &deviceConfig);
	///
	/// Destructor of this device
	///
	virtual ~LedDeviceLifx() override;

protected:

	///
	/// Initialise device's network details
	///
	/// @return True if success
	bool initNetwork();

	///
	/// Opens and initiatialises the output device
	///
	/// @return Zero on succes (i.e. device is ready and enabled) else negative
	///
	virtual int open() override;

private:

	///
	/// Sends the given led-color values to the Orbs
	///
	/// @param ledValues The color-value per led
	/// @return Zero on success else negative
	///
	virtual int write(const std::vector <ColorRgb> &ledValues) override;

	///
	/// Set Orbcolor
	///
	/// @param orbId the orb id
	/// @param color which color to set
	/// @param commandType which type of command to send (off / smoothing / etc..)
	///
	void setColor(int orbId, const ColorRgb &color, int commandType);

	///
	/// Send Orb command
	///
	/// @param bytes the byte array containing command to send over multicast
	///
	void sendCommand(const QByteArray &bytes);

	/// QNetworkAccessManager object for sending requests.
	QNetworkAccessManager *_networkmanager;

	/// QUdpSocket object used to send data over
	QUdpSocket * _udpSocket;

	/// QHostAddress object of multicast group IP address
	QHostAddress _groupAddress;

	/// String containing multicast group IP address
	QString _multicastGroup;

	/// Multicast port to send data to
	quint16 _multiCastGroupPort;

	// Multicast status
	bool joinedMulticastgroup;

	/// use Orbs own (external) smoothing algorithm
	bool _useOrbSmoothing;

	/// Transition time between colors (not implemented)
	int _transitiontime;

	// Maximum allowed color difference, will skip Orb (external) smoothing once reached
	int _skipSmoothingDiff;

	/// Number of leds in Orb, used to determine buffer size
	int _numLeds;

	/// Array of the orb ids.
	QVector<int> _orbIds;

	// Last send color map
	QMap<int, int> lastColorRedMap;
	QMap<int, int> lastColorGreenMap;
	QMap<int, int> lastColorBlueMap;



	/// Array of the light ids.
	std::vector<unsigned int> _lightIds;
	/// Array to save the lamps.
	std::vector<LedDeviceLifx> _lights;

	unsigned int _lightsCount;


};

	


