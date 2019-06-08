#include <memory>
#include "ClientHello.h"
#include "TLSStateClient.h"

TLSStateClient::TLSStateClient()
{
}

std::shared_ptr<ITLSMessage> TLSStateClient::GetHandshakeMsg()
{
	std::shared_ptr<ITLSMessage> RetTLSMessage = nullptr;
	HandshakeHeaderSerialized* HSHeader = reinterpret_cast<HandshakeHeaderSerialized*>(&m_CurrentMessageBuffer[sizeof(TLSPlaintextHeader)]);
	unsigned int HSHeaderLength = GetHSHeaderLength();

	// Only proceed if we have the full handshake message. Otherwise wait for more bytes.
	// Note that this is because according to the RFC, a TLS message of any type (Handshake/Alert/Application/CCS)
	// MAY be split across several record layer messages!
	if (HSHeaderLength > m_CurrentMessageBuffer.size() - sizeof(TLSPlaintextHeader) - sizeof(HandshakeHeaderSerialized))
	{
		return nullptr;
	}


	size_t Offset = 0;

	switch (HSHeader->msg_type)
	{
	case HandshakeType::client_hello:
		RetTLSMessage = std::make_shared<ClientHello>(m_CurrentMessageBuffer, Offset);
		break;
	case HandshakeType::certificate:
	//	break;
	case HandshakeType::certificate_verify:
		//break;
	case HandshakeType::client_key_exchange:
	//	break;
	case HandshakeType::finished:
	//	break;
	default:
		printf(std::string(std::to_string((int)HSHeader->msg_type) + "\n").c_str());
	//	break;
		break;
	}

	m_CurrentMessageBuffer.erase(m_CurrentMessageBuffer.begin(), m_CurrentMessageBuffer.begin() + HSHeaderLength + sizeof(TLSPlaintextHeader) + sizeof(HandshakeHeaderSerialized));

	return RetTLSMessage;
}
