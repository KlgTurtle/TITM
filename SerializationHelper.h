#pragma once
#include <vector>
#include <string>
#include <WinSock2.h>
template <class FieldType>
class SerializationHelper
{
public:
	static FieldType Deserialize(const std::vector<char>& Buffer, size_t& Offset)
	{
		const FieldType* FieldBE = reinterpret_cast<const FieldType*>(&Buffer[Offset]);
		FieldType Field;
		switch (sizeof(FieldType))
		{
			case sizeof(unsigned char) :
				Field = *FieldBE;
				break;
			case sizeof(unsigned short) :
				Field = ntohs(*FieldBE);
				break;
			case sizeof(unsigned int) :
				Field = ntohl(*FieldBE);
				break;
			case sizeof(unsigned __int64) :
				Field = ntohll(*FieldBE);
				break;
			default:
				break;
		}
		
		Offset += sizeof(FieldType);

		return Field;
	}

	static void Serialize(FieldType Field, std::vector<char>& Buffer, size_t& Offset)
	{
		FieldType FieldBE;
		switch (sizeof(FieldType))
		{
		case sizeof(unsigned char) :
			FieldBE = Field;
			break;
		case sizeof(unsigned short) :
			FieldBE = htons(Field);
			break;
		case sizeof(unsigned int) :
			FieldBE = htonl(Field);
			break;
		case sizeof(unsigned __int64) :
			FieldBE = htonll(Field);
			break;
		default:
			break;
		}
		
		memcpy(&Buffer[Offset], &FieldBE, sizeof(FieldBE));
		Offset += sizeof(FieldBE);
	}
	
	template <class LengthType>
	static std::vector<FieldType> DeserializeVec(const std::vector<char>& Buffer, size_t& Offset)
	{
		std::vector<FieldType> Result;
		size_t VecSize = SerializationHelper<LengthType>::Deserialize(Buffer, Offset);
		
		for (size_t i = 0; i < VecSize / sizeof(FieldType); ++i)
		{
			FieldType Field = Deserialize(Buffer, Offset);
			Result.push_back(Field);
		}

		return Result;
	}

	template <class LengthType>
	static void SerializeVec(const std::vector<FieldType> Vec, std::vector<char>& Buffer, size_t& Offset)
	{
		Serialize(static_cast<LengthType>(Vec.size()), Buffer, Offset);

		for (size_t i = 0; i < Vec.size(); ++i)
		{
			Serialize(Vec[i], Buffer, Offset);
		}
	}
};

//class _SerializationHelper
//{
//public:
//	static unsigned short DeserializeUnsignedShort(const std::vector<char>& Buffer, size_t& Offset);
//	static unsigned int DeserializeUnsignedInt(const std::vector<char>& Buffer, size_t& Offset);
//	static unsigned char DeserializeUnsignedChar(const std::vector<char>& Buffer, size_t& Offset);
//	static std::string DeserializeString(const std::vector<char>& Buffer, size_t& Offset, const size_t& Length);
//	static std::vector<unsigned short> DeserializeUShortVec(const std::vector<char>& Buffer, size_t& Offset, size_t LenLen);
//
//	static void SerializeUnsignedShort(unsigned short UShort, std::vector<char>& Buffer, size_t& Offset);
//	static void SerializeUnsignedInt(unsigned int UInt, std::vector<char>& Buffer, size_t& Offset);
//	static void SerializeUnsignedChar(unsigned char UChar, std::vector<char>& Buffer, size_t& Offset);
//	static void SerializeString(const std::string& Str, std::vector<char>& Buffer, size_t& Offset);
//	static void SerializeUShortVec(const std::vector<unsigned short>& UShortVec, size_t LenLen, std::vector<char>& Buffer,
//		                           size_t& Offset);
//
//};