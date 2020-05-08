#pragma once

// Leddevice includes
#include <leddevice/LedDevice.h>
#include "ProviderUdp.h"

// ssdp discover
#include <ssdp/SSDPDiscover.h>
#include <utils/Logger.h>

// STL includes
#include <cstring>
#include <cstdio>
#include <iostream>
#include <exception>
// Linux includes
#include <fcntl.h>
#include <sys/ioctl.h>

#include <stdint.h>


// Qt includes
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

const ushort LIFX_DEFAULT_PORT = 56700;
static const char LIFX_DEFAULT_HOST[] = "255.255.255.255";

///
/// Implementation of the LedDevice interface for sending to
/// Lifx devices via network by using the 'external control' protocol.
///
class LedDeviceLifx : public ProviderUdp
{
public:
	///
	/// Constructs the LedDevice for Lifx LightPanels (aka Aurora) or Canvas
	///
	/// following code shows all config options
	/// @code
	/// "device" :
	/// {
	///     "type"   : "lifx"
	///     "output" : "hostname or IP", // Optional. If empty, device is tried to be discovered
	///     "token"  : "Authentication Token",
	/// },
	///@endcode
	///
	/// @param deviceConfig json config for Lifx
	///
	explicit LedDeviceLifx(const QJsonObject &deviceConfig);

	///
	/// Destructor of the LedDevice; closes the tcp client
	///
	///virtual ~LedDeviceLifx() override; ?????

	/// Constructs leddevice
	static LedDevice* construct(const QJsonObject &deviceConfig);

	/// Switch the device on
	virtual int switchOn() override;

	/// Switch the device off
	virtual int switchOff() override;

protected:

	///
	/// Writes the led color values to the led-device
	///
	/// @param ledValues The color-value per led
	/// @return Zero on succes else negative
	///
	virtual int write(const std::vector<ColorRgb> & ledValues) override;

	///
	/// Initialise Lifx device's configuration and network address details
	///
	/// @param deviceConfig the json device config
	/// @return True if success
	///
	bool init(const QJsonObject &deviceConfig) override;

	///
	/// Get Lifx device details and configuration
	///
	/// @return True, if Lifx device capabilities fit configuration
	///
	bool initLeds();

	///
	/// Opens and initiatialises the output device
	///
	/// @return Zero on succes (i.e. device is ready and enabled) else negative
	///
	virtual int open() override;


	///uint16_t size;
	///uint16_t protocol:12;
	///uint8_t	addressable:1;
	///uint8_t	tagged:1;
	///uint8_t	origin:2;
	///uint32_t source;
	////* frame address */
	///uint8_t	target[8];
	///uint8_t	reserved[6];
	///uint8_t	res_required:1;
	///uint8_t	ack_required:1;
	///uint8_t	:6;
	///uint8_t	sequence;
	////* protocol header */
	///uint64_t :64;
	///uint16_t type;
	///uint16_t :16;

private:
	// QNetworkAccessManager object for sending requests.
	QNetworkAccessManager* _networkmanager;

	QString _hostname;
	QString _api_port;
	QString _auth_token;

	//Lifx device details
	QString _deviceModel;
	QString _deviceFirmwareVersion;
	ushort _extControlVersion;
	/// The number of panels with leds
	uint _panelLedCount;
	/// Array of the pannel ids.
	std::vector<uint> _panelIds;

	///
	/// Discover Lifx device via SSDP identifiers
	///
	/// @return True, if Lifx device was found
	///
	bool discoverDevice();

	///
	/// Change Lifx device to External Control (UDP) mode
	///
	/// @return Response from device
	///
	QJsonDocument changeToExternalControlMode();

	///
	/// Get command to switch Lifx device on or off
	///
	/// @param isOn True, if to switch on device
	/// @return Command to switch device on/off
	///
	QString getOnOffRequest (bool isOn ) const;

	///
	/// Get command as url
	///
	/// @param host Hostname or IP
	/// @param port IP-Port
	/// @param _auth_token Authorization token
	/// @param Endpoint command for request
	/// @return Url to execute endpoint/command
	///
	QString getUrl(QString host, QString port, QString auth_token, QString endpoint) const;

	///
	/// Execute GET request
	///
	/// @param url GET request for url
	/// @return Response from device
	///
	QJsonDocument getJson(QString url);

	///
	/// Execute PUT request
	///
	/// @param Url for PUT request
	/// @param json Command for request
	/// @return Response from device
	///
	QJsonDocument putJson(QString url, QString json);

	///
	/// Handle replys for GET and PUT requests
	///
	/// @param reply Network reply
	/// @return Response for request, if no error
	///
	QJsonDocument handleReply(QNetworkReply* const &reply );

	///
	/// convert vector to hex string
	///
	/// @param uint8_t vector
	/// @return vector as string of hex values
	std::string uint8_vector_to_hex_string( const std::vector<uint8_t>& buffer ) const;
};
