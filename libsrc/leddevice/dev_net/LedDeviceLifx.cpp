// Local-Hyperion includes
#include "LedDeviceLifx.h"

LedDeviceLifx::LedDeviceLifx(const QJsonObject &deviceConfig)
	: ProviderUdp()
{
	_devConfig = deviceConfig;
	_deviceReady = false;
}

LedDevice* LedDeviceLifx::construct(const QJsonObject &deviceConfig)
{
	return new LedDeviceLifx(deviceConfig);
}

bool LedDeviceLifx::init(const QJsonObject &deviceConfig)
{
	_port = RAW_DEFAULT_PORT;
	bool isInitOK = ProviderUdp::init(deviceConfig);
	return isInitOK;
}

int LedDeviceLifx::write(const std::vector<ColorRgb> &ledValues)
{
	const uint8_t * dataPtr = reinterpret_cast<const uint8_t *>(ledValues.data());

	return writeBytes((unsigned)_ledRGBCount, dataPtr);
}
