#include "ITcpProxyManipulator.h"

ITcpProxyManipulator::ITcpProxyManipulator(TcpSocket SourceSocket, TcpSocket TargetSocket)
	: m_SourceSocket(SourceSocket), m_TargetSocket(TargetSocket)
{

}


