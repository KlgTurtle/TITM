#include "TLSState.h"
#include <WinSock2.h>
#include "ClientHello.h"
#include "ServerHello.h"
#include "ChangeCipherSpec.h"
#include "ServerCertificate.h"
#include "EncryptedHandshakeMessage.h"
#define DEBUG_PRINTS
#include "Common.h"

TLSState::TLSState() 
{
	m_CurrentMessageBuffer.clear();
}

void TLSState::Update(const std::vector<char>& DataBuffer, std::vector<std::shared_ptr<ITLSMessage>>& OutTLSMessages)
{
	OutTLSMessages.clear();

	m_CurrentMessageBuffer.insert(m_CurrentMessageBuffer.end(), DataBuffer.begin(), DataBuffer.end());

	// If we have a header and also a buffered message with the number specified in the header or more
	// it means we can parse a full message now.
	while (m_CurrentMessageBuffer.size() >= sizeof(TLSPlaintextHeader)) 
	{
		TLSPlaintextHeader* CurrentHeader = reinterpret_cast<TLSPlaintextHeader*>(m_CurrentMessageBuffer.data());
		unsigned short CurrHeaderLength = ntohs(CurrentHeader->length);
		if (m_CurrentMessageBuffer.size() >= CurrHeaderLength + sizeof(TLSPlaintextHeader))
		{
			std::shared_ptr<ITLSMessage> ReturnMsg = nullptr;
			switch (CurrentHeader->type)
			{
			case ContentType::handshake:
				
				if (IsEncryptedHandshakeMsg())
				{
					dbgprintf(GetTypeStr() + ": encrypted handshake\n");
					ReturnMsg = GetEncryptedHandshakeMsg();
				}
				else
				{
					dbgprintf(GetTypeStr() + ": handshake\n");
					ReturnMsg = GetHandshakeMsg();
				}
				break;
			case ContentType::change_cipher_spec:
				dbgprintf("change_cipher_spec\n");
				ReturnMsg = GetChangeCipherSpecMsg();
				break;
			case ContentType::application_data:
			//	dbgprintf("application_data\n");
				m_CurrentMessageBuffer.erase(m_CurrentMessageBuffer.begin(), m_CurrentMessageBuffer.begin() + CurrHeaderLength + sizeof(TLSPlaintextHeader));
				//ReturnMsg = GetApplicationDataMsg();
				break;
			case ContentType::alert:
				dbgprintf(GetTypeStr() + ": alert\n");
				m_CurrentMessageBuffer.erase(m_CurrentMessageBuffer.begin(), m_CurrentMessageBuffer.begin() + CurrHeaderLength + sizeof(TLSPlaintextHeader));
				//ReturnMsg = GetAlertMsg();
				break;
			default:
				m_CurrentMessageBuffer.clear();
				dbgprintf(GetTypeStr() + ": !!!UNKNOWN!!!\n");
				break;
			}	

			if (ReturnMsg != nullptr)
			{
				OutTLSMessages.push_back(ReturnMsg);
			}
		}
		else
		{
			break;
		}
	}
}


std::shared_ptr<ITLSMessage> TLSState::GetEncryptedHandshakeMsg()
{
	size_t Offset = 0;
	std::shared_ptr<ITLSMessage> RetTLSMessage = std::make_shared<EncryptedHandshakeMessage>(m_CurrentMessageBuffer, Offset);
	m_CurrentMessageBuffer.erase(m_CurrentMessageBuffer.begin(), m_CurrentMessageBuffer.begin() + Offset);
	return RetTLSMessage;
}

std::shared_ptr<ITLSMessage> TLSState::GetChangeCipherSpecMsg()
{
	size_t Offset = 0;

	std::shared_ptr<ITLSMessage> RetTLSMessage = std::make_shared<ChangeCipherSpec>(m_CurrentMessageBuffer, Offset);
	m_CurrentMessageBuffer.erase(m_CurrentMessageBuffer.begin(), m_CurrentMessageBuffer.begin() + Offset);
	return RetTLSMessage;
}

std::shared_ptr<ITLSMessage> TLSState::GetApplicationDataMsg()
{
	m_CurrentMessageBuffer.clear();
	return nullptr;
}

std::shared_ptr<ITLSMessage> TLSState::GetAlertMsg()
{
	m_CurrentMessageBuffer.clear();
	return nullptr;
}

bool TLSState::IsEncryptedHandshakeMsg()
{
	TLSPlaintextHeader* CurrentTLSHeader = reinterpret_cast<TLSPlaintextHeader*>(m_CurrentMessageBuffer.data());
	unsigned short TLSHeaderLength = ntohs(CurrentTLSHeader->length);
	unsigned int HSHeaderLength = GetHSHeaderLength();

	return (TLSHeaderLength <= m_CurrentMessageBuffer.size()) && 
		(HSHeaderLength + sizeof(HandshakeHeaderSerialized) != TLSHeaderLength);
}

unsigned int TLSState::GetHSHeaderLength()
{
	HandshakeHeaderSerialized* HSHeader = reinterpret_cast<HandshakeHeaderSerialized*>(&m_CurrentMessageBuffer[sizeof(TLSPlaintextHeader)]);

	std::vector<unsigned char> HSHeaderLengthBuf;
	HSHeaderLengthBuf.resize(sizeof(unsigned int));
	memset(HSHeaderLengthBuf.data(), 0x00, HSHeaderLengthBuf.size());
	memcpy(&HSHeaderLengthBuf[1], HSHeader->length, sizeof(HSHeader->length));

	unsigned int HSHeaderLength = ntohl(*reinterpret_cast<unsigned long*>(HSHeaderLengthBuf.data()));

	return HSHeaderLength;
}







