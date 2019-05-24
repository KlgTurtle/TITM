#include "PskKeyExchangeModes.h"

PskKeyExchangeModes::PskKeyExchangeModes(const std::vector<char>& Buffer, size_t & Offset, bool bIsEmpty)
{
	if (!bIsEmpty)
	{
		Deserialize(Buffer, Offset);
	}
}

void PskKeyExchangeModes::Serialize(std::vector<char>& Buffer, size_t & Offset)
{
	SerializationHelper::SerializeVec<PskKeyExchangeMode, unsigned char>(PskKeyExchangeModesList, Buffer, Offset);
}

void PskKeyExchangeModes::Deserialize(const std::vector<char>& Buffer, size_t & Offset)
{
	PskKeyExchangeModesList.clear();

	SerializationHelper::DeserializeVec<PskKeyExchangeMode, unsigned char>(Buffer, Offset, PskKeyExchangeModesList);
}
