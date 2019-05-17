//#include <WinSock2.h>
//#include "SerializationHelper.h"
//
//static FieldType Deserialize(const std::vector<char>& Buffer, size_t& Offset)
//	{
//		const FieldType* FieldBE = reinterpret_cast<const FieldType*>(&Buffer[Offset]);
//		FieldType Field;
//		switch (sizeof(FieldType))
//		{
//			case sizeof(unsigned char) :
//				Field = *FieldBE;
//				break;
//			case sizeof(unsigned short) :
//				Field = ntohs(*FieldBE);
//				break;
//			case sizeof(unsigned int) :
//				Field = ntohl(*FieldBE);
//				break;
//			case sizeof(unsigned __int64) :
//				Field = ntohll(*FieldBE);
//				break;
//			default:
//				break;
//		}
//		
//		Offset += sizeof(FieldType);
//
//		return Field;
//	}
//	//static std::string DeserializeString(const std::vector<char>& Buffer, size_t& Offset, const size_t& Length);
//	template <class LengthType>
//	static std::vector<FieldType> DeserializeVec(const std::vector<char>& Buffer, size_t& Offset)
//	{
//		std::vector<FieldType> Result;
//
//
//		size_t VecSize = SerializationHelper<LengthType>::Deserialize(Buffer, Offset);
//		
//
//		for (size_t i = 0; i < VecSize / sizeof(FieldType); ++i)
//		{
//			FieldType Field = Deserialize(Buffer, Offset);
//			Result.push_back(Field);
//		}
//
//		return Result;
//	}
//
//unsigned short SerializationHelper::DeserializeUnsignedShort(const std::vector<char>& Buffer, size_t & Offset)
//{
//	const unsigned short* UShortBE = reinterpret_cast<const unsigned short*>(&Buffer[Offset]);
//	unsigned short UShort = ntohs(*UShortBE);
//	Offset += sizeof(UShort);
//
//	return UShort;
//}
//
//unsigned int SerializationHelper::DeserializeUnsignedInt(const std::vector<char>& Buffer, size_t & Offset)
//{
//	const unsigned int* UIntBE = reinterpret_cast<const unsigned int*>(&Buffer[Offset]);
//	unsigned int UInt = ntohl(*UIntBE);
//	Offset += sizeof(UInt);
//
//	return UInt;
//}
//
//unsigned char SerializationHelper::DeserializeUnsignedChar(const std::vector<char>& Buffer, size_t & Offset)
//{
//	unsigned char UChar = Buffer[Offset];
//	Offset += sizeof(UChar);
//	return UChar;
//}
//
//std::string SerializationHelper::DeserializeString(const std::vector<char>& Buffer, size_t & Offset, const size_t & Length)
//{
//	std::string Str(&Buffer[Offset], &Buffer[Offset + Length]);
//	Offset += Length;
//	return Str;
//}
//
//std::vector<unsigned short> SerializationHelper::DeserializeUShortVec(const std::vector<char>& Buffer, size_t & Offset, size_t LenLen)
//{
//	size_t VecSize;
//	std::vector<unsigned short> Result;
//
//	// This is the length of the length value that precedes the list.
//	switch (LenLen)
//	{
//	case sizeof(unsigned char) :
//		VecSize = DeserializeUnsignedChar(Buffer, Offset);
//		break;
//	case sizeof(unsigned short) :
//		VecSize = DeserializeUnsignedShort(Buffer, Offset);
//		break;
//	case sizeof(unsigned int) :
//		VecSize = DeserializeUnsignedInt(Buffer, Offset);
//		break;
//	default:
//		break;
//	}
//
//	for (size_t i = 0; i < VecSize / sizeof(unsigned short); ++i)
//	{
//		unsigned short UShort = DeserializeUnsignedShort(Buffer, Offset);
//		Result.push_back(UShort);
//	}
//
//	return Result;
//}
//
//void SerializationHelper::SerializeUnsignedShort(unsigned short UShort, std::vector<char>& Buffer, size_t & Offset)
//{
//	unsigned short UShortBE = htons(UShort);
//	memcpy(&Buffer[Offset], &UShortBE, sizeof(UShortBE));
//	Offset += sizeof(UShortBE);
//}
//
//void SerializationHelper::SerializeUnsignedInt(unsigned int UInt, std::vector<char>& Buffer, size_t & Offset)
//{
//	unsigned int UIntBE = htonl(UInt);
//	memcpy(&Buffer[Offset], &UIntBE, sizeof(UIntBE));
//	Offset += sizeof(UIntBE);
//}
//
//void SerializationHelper::SerializeUnsignedChar(unsigned char UChar, std::vector<char>& Buffer, size_t & Offset)
//{
//	Buffer[Offset] = UChar;
//	Offset += sizeof(UChar);
//}
//
//void SerializationHelper::SerializeString(const std::string & Str, std::vector<char>& Buffer, size_t & Offset)
//{
//	std::memcpy(&Buffer[Offset], Str.data(), Str.length());
//	Offset += Str.length();
//}
//
//void SerializationHelper::SerializeUShortVec(const std::vector<unsigned short>& UShortVec, size_t LenLen, std::vector<char>& Buffer, size_t & Offset)
//{
//	// This is the length of the length value that precedes the list.
//	switch (LenLen)
//	{
//	case sizeof(unsigned char) :
//		SerializeUnsignedChar(static_cast<unsigned char>(UShortVec.size()), Buffer, Offset);
//		break;
//	case sizeof(unsigned short) :
//		SerializeUnsignedShort(static_cast<unsigned short>(UShortVec.size()), Buffer, Offset);
//		break;
//	case sizeof(unsigned int) :
//		SerializeUnsignedInt(static_cast<unsigned int>(UShortVec.size()), Buffer, Offset);
//		break;
//	default:
//		break;
//	}
//
//	for (size_t i = 0; i < UShortVec.size() / sizeof(unsigned short); ++i)
//	{
//		SerializeUnsignedShort(UShortVec[i], Buffer, Offset);
//	}
//}
