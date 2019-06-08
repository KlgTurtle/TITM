#pragma once
#include "TLS.h"
#include "ITLSMessage.h"
#include <vector>
#include <memory>
class TLSState
{
public:
	TLSState();
	virtual void Update(const std::vector<char>& DataBuffer, std::vector<std::shared_ptr<ITLSMessage>>& OutTLSMessages);
	virtual ~TLSState() {}
	


protected:

	virtual std::shared_ptr<ITLSMessage> GetHandshakeMsg() = 0;
	virtual std::shared_ptr<ITLSMessage> GetChangeCipherSpecMsg();
	virtual std::shared_ptr<ITLSMessage> GetApplicationDataMsg();
	virtual std::shared_ptr<ITLSMessage> GetAlertMsg();

	virtual unsigned int GetHSHeaderLength();

	//virtual std::shared_ptr<ITLSMessage> GetClientHello();

	

	//TLSPlaintextHeader m_CurrentHeader;
	//ITLSMessage        m_CurrentMessage;
	std::vector<char>  m_CurrentMessageBuffer;

};

class TLSStateClient : public TLSState
{
public:
	TLSStateClient();
	virtual ~TLSStateClient() {}
protected:
	virtual std::shared_ptr<ITLSMessage> GetHandshakeMsg();
};

class TLSStateServer : public TLSState
{
public:
	TLSStateServer();
	virtual ~TLSStateServer() {}

protected:
	virtual std::shared_ptr<ITLSMessage> GetHandshakeMsg();
};
