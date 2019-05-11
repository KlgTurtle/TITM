#include "TLSState.h"
#include <WinSock2.h>
TLSState::TLSState() 
{
	m_CurrentMessageBuffer.clear();
}

std::shared_ptr<ITLSMessage> TLSState::Update(const std::vector<char>& DataBuffer)
{


	std::shared_ptr<ITLSMessage> ReturnMsg = nullptr;

	/*if (m_CurrentMessageBuffer.size() < sizeof(TLSPlaintextHeader))
	{
		m_CurrentMessageBuffer.insert(m_CurrentMessageBuffer.end(), DataBuffer.begin(), DataBuffer.end());
		if (m_CurrentMessageBuffer.size() >= sizeof(TLSPlaintextHeader))
		{
			std::copy(reinterpret_cast<char*>(&m_CurrentHeader), m_CurrentMessageBuffer.data(), 
				sizeof(TLSPlaintextHeader));
		}


	}*/

	m_CurrentMessageBuffer.insert(m_CurrentMessageBuffer.end(), DataBuffer.begin(), DataBuffer.end());

	// If we have a header and also a buffered message with the number specified in the header or more
	// it means we can parse a full message now.
	if (m_CurrentMessageBuffer.size() >= sizeof(TLSPlaintextHeader)) 
		//&&
	//	m_CurrentMessageBuffer.size() >= m_CurrentHeader.length + sizeof(TLSPlaintextHeader))
	{
		TLSPlaintextHeader* CurrentHeader = reinterpret_cast<TLSPlaintextHeader*>(m_CurrentMessageBuffer.data());
		unsigned short CurrHeaderLength = ntohs(CurrentHeader->length);
		if (m_CurrentMessageBuffer.size() >= CurrHeaderLength + sizeof(TLSPlaintextHeader))
		{
			switch (CurrentHeader->type)
			{
			case ContentType::handshake:
				ReturnMsg = GetHandshakeMsg();
				break;
			case ContentType::change_cipher_spec:
				ReturnMsg = GetChangeCipherSpecMsg();
				break;
			case ContentType::application_data:
				ReturnMsg = GetApplicationDataMsg();
				break;
			case ContentType::alert:
				ReturnMsg = GetAlertMsg();
				break;
			}

			
		}
	}

	



	return ReturnMsg;
}

std::shared_ptr<ITLSMessage> TLSState::GetHandshakeMsg()
{
	std::shared_ptr<ITLSMessage> RetTLSMessage = nullptr;

	TLSPlaintextHeader* TLSHeader = reinterpret_cast<TLSPlaintextHeader*>(m_CurrentMessageBuffer.data());
	unsigned short TLSHeaderLen = ntohs(TLSHeader->length);
	HandshakeHeader* HSHeader = reinterpret_cast<HandshakeHeader*>(&m_CurrentMessageBuffer[sizeof(TLSPlaintextHeader)]);

	std::vector<unsigned char> HSHeaderLengthBuf;
	HSHeaderLengthBuf.resize(sizeof(unsigned int));
	memset(HSHeaderLengthBuf.data(), 0x00, HSHeaderLengthBuf.size());
	memcpy(&HSHeaderLengthBuf[1], HSHeader->length, sizeof(HSHeader->length));
	
	unsigned int HSHeaderLength = ntohl(*reinterpret_cast<unsigned long*>(HSHeaderLengthBuf.data()));
	//m_CurrentMessageBuffer.erase(m_CurrentMessageBuffer.begin(),
	//	m_CurrentMessageBuffer.begin() + CurrentHeader->length + sizeof(TLSPlaintextHeader));

	// Only proceed if we have the full handshake message. Otherwise wait for more bytes.
	// Note that this is because according to the RFC, a TLS message of any type (Handshake/Alert/Application/CCS)
	// MAY be split across several record layer messages!
	if (HSHeaderLength < m_CurrentMessageBuffer.size() - sizeof(TLSPlaintextHeader) - sizeof(HandshakeHeader))
	{
		return nullptr;
	}


	switch (HSHeader->msg_type)
	{
	case HandshakeType::hello_request:
		break;
	case HandshakeType::client_hello:
		RetTLSMessage = GetClientHello();
		break;
	case HandshakeType::server_hello:
		break;
	case HandshakeType::certificate:
		break;
	case HandshakeType::server_key_exchange:
		break;
	case HandshakeType::certificate_request:
		break;
	case HandshakeType::server_hello_done:
		break;
	case HandshakeType::certificate_verify:
		break;
	case HandshakeType::client_key_exchange:
		break;
	case HandshakeType::finished:
		break;
	default:
		break;
	}

	return nullptr;
}

std::shared_ptr<ITLSMessage> TLSState::GetChangeCipherSpecMsg()
{
	return nullptr;
}

std::shared_ptr<ITLSMessage> TLSState::GetApplicationDataMsg()
{
	return nullptr;
}

std::shared_ptr<ITLSMessage> TLSState::GetAlertMsg()
{
	return nullptr;
}

std::shared_ptr<ITLSMessage> TLSState::GetClientHello()
{
	std::shared_ptr<ClientHello> RetClientHello = std::make_shared<ClientHello>();
	size_t Offset = sizeof(TLSPlaintextHeader) + sizeof(HandshakeHeader);

	ClientHelloHeader* CHHeader = 
		reinterpret_cast<ClientHelloHeader*>(&m_CurrentMessageBuffer[Offset]);

	RetClientHello->Version = CHHeader->legacy_version;
	memcpy(RetClientHello->random, CHHeader->random, sizeof(CHHeader->random));
	RetClientHello->session_id_length = CHHeader->SessionIdLength;

	Offset += sizeof(ClientHelloHeader);

	RetClientHello->session_id.clear();
	RetClientHello->session_id.insert(RetClientHello->session_id.begin(), &m_CurrentMessageBuffer[Offset],
		&m_CurrentMessageBuffer[Offset + CHHeader->SessionIdLength]);

	Offset += CHHeader->SessionIdLength;

	unsigned short* CipherSuitesLengthBE = reinterpret_cast<unsigned short*>(&m_CurrentMessageBuffer[Offset]);
	unsigned short CipherSuitesLength = ntohs(*CipherSuitesLengthBE);


	return RetClientHello;
	
}



