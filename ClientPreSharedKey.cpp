#include "ClientPreSharedKey.h"

ClientPreSharedKey::ClientPreSharedKey(const std::vector<char>& Buffer, size_t & Offset, bool bIsEmpty)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void ClientPreSharedKey::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
}

void ClientPreSharedKey::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
}
