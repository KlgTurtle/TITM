#pragma once
#include "TLSState.h"

class TLSStateClient : public TLSState
{
public:
	TLSStateClient();
	virtual ~TLSStateClient() {}
protected:
	virtual std::shared_ptr<ITLSMessage> GetHandshakeMsg();
};