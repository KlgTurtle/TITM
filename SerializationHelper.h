#pragma once
#include <vector>
#include <string>
#include <WinSock2.h>

enum class UCharLength : unsigned char {};
enum class UShortLength : unsigned short {};
enum class UCharType : unsigned char {};

template<typename E>
constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type
{
	return static_cast<typename std::underlying_type<E>::type>(e);
}

class SerializationHelper
{
public:
	template <class FieldType>
	static FieldType Deserialize(const std::vector<char>& Buffer, size_t& Offset)
	{
		const auto* FieldBE = 
			reinterpret_cast<const std::underlying_type<FieldType>::type*>(&Buffer[Offset]);
		std::underlying_type<FieldType>::type Field;
		Field = Deserialize<std::underlying_type<FieldType>::type>(Buffer, Offset);
		
		return static_cast<FieldType>(Field);
	}

	template <>
	static unsigned char Deserialize<unsigned char>(const std::vector<char>& Buffer, size_t& Offset)
	{
		unsigned char Field = Buffer[Offset];
		Offset += sizeof(Field);
		return Field;
	}

	template <>
	static char Deserialize<char>(const std::vector<char>& Buffer, size_t& Offset)
	{
		char Field = Buffer[Offset];
		Offset += sizeof(Field);
		return Field;
	}

	template <>
	static unsigned short Deserialize<unsigned short>(const std::vector<char>& Buffer, size_t& Offset)
	{
		const unsigned short* FieldBE = reinterpret_cast<const unsigned short*>(&Buffer[Offset]);
		unsigned short Field = ntohs(*FieldBE);
		Offset += sizeof(Field);
		return Field;
	}

	template <>
	static unsigned int Deserialize<unsigned int>(const std::vector<char>& Buffer, size_t& Offset)
	{
		const unsigned int* FieldBE = reinterpret_cast<const unsigned int*>(&Buffer[Offset]);
		unsigned int Field = ntohl(*FieldBE);
		Offset += sizeof(Field);
		return Field;
	}

	template <>
	static unsigned __int64 Deserialize<unsigned __int64>(const std::vector<char>& Buffer, size_t& Offset)
	{
		const unsigned __int64* FieldBE = reinterpret_cast<const unsigned __int64*>(&Buffer[Offset]);
		unsigned __int64 Field = ntohll(*FieldBE);
		Offset += sizeof(Field);
		return Field;
	}

	static unsigned int Deserialize3BytesInt(const std::vector<char>& Buffer, size_t& Offset)
	{
		std::vector<unsigned char> WorkBuf;
		WorkBuf.resize(sizeof(unsigned int));
		WorkBuf[0] = 0x00;
		memcpy(&WorkBuf[1], &Buffer[Offset], sizeof(unsigned int) - 1);
		Offset += sizeof(unsigned int) - 1;
		
		unsigned int Result = ntohl(*reinterpret_cast<unsigned long*>(WorkBuf.data()));

		return Result;
	}

	template <class FieldType>
	static void Serialize(FieldType Field, std::vector<char>& Buffer, size_t& Offset)
	{
		auto FieldToSerialize = to_integral(Field);
		Serialize<typename std::underlying_type<FieldType>::type>(FieldToSerialize, Buffer, Offset);
	}

	template <>
	static void Serialize<unsigned char>(unsigned char Field, std::vector<char>& Buffer, size_t& Offset)
	{
		// size()==0 means we just want to get the updated offset back.
		if (Buffer.size() > 0)
		{
			Buffer[Offset] = Field;
		}
		Offset += sizeof(Field);
	}

	template <>
	static void Serialize<char>(char Field, std::vector<char>& Buffer, size_t& Offset)
	{
		// size()==0 means we just want to get the updated offset back.
		if (Buffer.size() > 0)
		{
			Buffer[Offset] = Field;
		}
		Offset += sizeof(Field);
	}

	template <>
	static void Serialize<unsigned short>(unsigned short Field, std::vector<char>& Buffer, size_t& Offset)
	{
		// size()==0 means we just want to get the updated offset back.
		if (Buffer.size() > 0)
		{
			const unsigned short FieldBE = htons(Field);
			memcpy(&Buffer[Offset], &FieldBE, sizeof(FieldBE));
		}
		Offset += sizeof(Field);
	}

	template <>
	static void Serialize<unsigned int>(unsigned int Field, std::vector<char>& Buffer, size_t& Offset)
	{
		// size()==0 means we just want to get the updated offset back.
		if (Buffer.size() > 0)
		{
			const unsigned int FieldBE = htonl(Field);
			memcpy(&Buffer[Offset], &FieldBE, sizeof(FieldBE));
		}
		Offset += sizeof(Field);
	}

	template <>
	static void Serialize<unsigned __int64>(unsigned __int64 Field, std::vector<char>& Buffer, size_t& Offset)
	{
		// size()==0 means we just want to get the updated offset back.
		if (Buffer.size() > 0)
		{
			const unsigned __int64 FieldBE = htonll(Field);
			memcpy(&Buffer[Offset], &FieldBE, sizeof(FieldBE));
		}
		Offset += sizeof(Field);
	}

	static void Serialize3BytesInt(unsigned int Field, std::vector<char>& Buffer, size_t& Offset)
	{
		if (Buffer.size() > 0)
		{
			unsigned int FieldBE = htonl(Field);
			char* pFieldBE = reinterpret_cast<char*>(&FieldBE);
			memcpy(&Buffer[Offset], pFieldBE + 1, sizeof(unsigned int) - 1);
		}

		Offset += sizeof(unsigned int) - 1;
	}
	
	template <class FieldType, class LengthType>
	static void DeserializeVec(const std::vector<char>& Buffer, size_t& Offset, std::vector<FieldType>& OutVec)
	{
		OutVec.clear();
		LengthType VecSize = Deserialize<LengthType>(Buffer, Offset) / sizeof(FieldType);

		for (size_t i = 0; i < VecSize; ++i)
		{
			FieldType Field = Deserialize<FieldType>(Buffer, Offset);
			OutVec.push_back(Field);
		}
	}

	template <class FieldType>
	static void DeserializeFixedVec(const std::vector<char>& Buffer, size_t& Offset, std::vector<FieldType>& OutVec, size_t VecSize)
	{
		OutVec.clear();

		for (size_t i = 0; i < VecSize; ++i)
		{
			FieldType Field = Deserialize<FieldType>(Buffer, Offset);
			OutVec.push_back(Field);
		}
	}

	template <class FieldType, class LengthType>
	static void SerializeVec(const std::vector<FieldType>& Vec, std::vector<char>& Buffer, size_t& Offset)
	{
		LengthType VecSize = static_cast<LengthType>(Vec.size()) * sizeof(FieldType);
		Serialize<LengthType>(VecSize, Buffer, Offset);

		for (size_t i = 0; i < Vec.size(); ++i)
		{
			Serialize<FieldType>(Vec[i], Buffer, Offset);
		}
	}

	template <class FieldType>
	static void SerializeFixedVec(const std::vector<FieldType>& Vec, size_t VecSize, std::vector<char>& Buffer, size_t& Offset)
	{
		for (size_t i = 0; i < VecSize; ++i)
		{
			Serialize<FieldType>(Vec[i], Buffer, Offset);
		}
	}

	static void SerializeStruct(char* OpaqueStruct, size_t StructSize, std::vector<char>& Buffer, size_t& Offset)
	{
		if (Buffer.size() > 0)
		{
			memcpy(&Buffer[Offset], OpaqueStruct, StructSize);
		}

		Offset += StructSize;
	}

	static void DeserializeStruct(const std::vector<char>& Buffer, size_t& Offset, char* OpaqueStruct, size_t StructSize)
	{
		if (Buffer.size() > 0)
		{
			memcpy(OpaqueStruct, &Buffer[Offset], StructSize);
		}

		Offset += StructSize;
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

