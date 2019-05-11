#include "TLS.h"

std::string ClientHello::ToString()
{
	std::string TLSHeader = ITLSMessage::ToString();

	return "";
}

std::string ITLSMessage::ToString()
{
	std::string ContentTypeStr = "";
	switch (TLSHeader.type)
	{
	case ContentType::change_cipher_spec:
		ContentTypeStr = "Change Cipher Spec";
		break;
	case	ContentType::alert:
		ContentTypeStr = "Alert";
		break;
	case	ContentType::handshake:
		ContentTypeStr = "Handshake";
		break;
	case	ContentType::application_data:
		ContentTypeStr = "Application Data";
		break;
	default:
		ContentTypeStr = "*Unknown*";
		break;
	}

	std::string VersionStr = "";
	if (TLSHeader.version.major == 3 && TLSHeader.version.minor >= 1)
	{
		VersionStr = "TLSv1." + std::to_string(TLSHeader.version.minor - 1);
	}
	else
	{
		VersionStr += "SSLv" + std::to_string(TLSHeader.version.major) + ".0";
	}

	std::string LengthStr = std::to_string(TLSHeader.length);

	return
		"Content Type:" + ContentTypeStr + "\n"
		"Version:     " + VersionStr + "\n" +
		"Length:      " + LengthStr;
}
