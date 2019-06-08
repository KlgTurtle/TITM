#pragma once
#include "TLSState.h"

class TLSStateServer : public TLSState
{
public:
	TLSStateServer();
	virtual ~TLSStateServer() {}

protected:
	virtual std::shared_ptr<ITLSMessage> GetHandshakeMsg();
};