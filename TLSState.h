#pragma once
#include "TLS.h"
#include "ITLSMessage.h"
#include <vector>
#include <memory>
class TLSState
{
public:
	TLSState();
	virtual ~TLSState() {}

	virtual std::shared_ptr<ITLSMessage> Update(const std::vector<char>& DataBuffer);
	


protected:

	virtual std::shared_ptr<ITLSMessage> GetHandshakeMsg();
	virtual std::shared_ptr<ITLSMessage> GetChangeCipherSpecMsg();
	virtual std::shared_ptr<ITLSMessage> GetApplicationDataMsg();
	virtual std::shared_ptr<ITLSMessage> GetAlertMsg();

	//virtual std::shared_ptr<ITLSMessage> GetClientHello();

	

	//TLSPlaintextHeader m_CurrentHeader;
	//ITLSMessage        m_CurrentMessage;
	std::vector<char>  m_CurrentMessageBuffer;

};