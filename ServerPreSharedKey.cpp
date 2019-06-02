#include "ServerPreSharedKey.h"

ServerPreSharedKey::ServerPreSharedKey(const std::vector<char>& Buffer, size_t & Offset, bool bIsEmpty)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void ServerPreSharedKey::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::Serialize<unsigned short>(SelectedIdentity, Buffer, Offset);
}

void ServerPreSharedKey::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	SelectedIdentity = SerializationHelper::Deserialize<unsigned short>(Buffer, Offset);
}
