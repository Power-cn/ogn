#include "Shared.h"
#include "BinaryStream.h"
#include "Variant.h"
#include <memory>
#include <string.h>
#include <stdio.h>
Variant::Variant(void):
mType(TypeNone)
{
	memset(&mValue, sizeof(mValue), 0);
}

Variant::Variant(const char* value)
{
	mType = TypeString;
	int len = strlen(value) + 1;
	mValue.value_str = new char[len];
	strcpy_s(mValue.value_str, len, value);
}

Variant::Variant(const BinaryStream& value)
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeMemory;
	mValue.value_bytes = new BinaryStream();
	mValue.value_bytes->WriteBytes(value.getPtr(), value.getWPostion());
}

Variant::Variant(int8* value, int32 length)
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeMemory;
	mValue.value_bytes = new BinaryStream();
	mValue.value_bytes->WriteBytes(value, length);
}

Variant::~Variant(void)
{
	if (mType == TypeMemory)
		delete mValue.value_bytes;
	reset();
}

Variant::Variant( const Variant& other )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = other.mType;
	copyValue(other.mValue, mValue, mType);
}

Variant::Variant( const bool value )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeBoolean;
	mValue.value_bool = value;
}

Variant::Variant( const int8 value )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeInt8;
	mValue.value_char = value;
}

Variant::Variant( const uint8 value )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeUint8;
	mValue.value_uchar = value;
}

Variant::Variant( const int16 value )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeInt16;
	mValue.value_short = value;
}

Variant::Variant( const uint16 value )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeUint16;
	mValue.value_ushort = value;
}


Variant::Variant( const int32 value )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeInt32;
	mValue.value_int = value;
}

Variant::Variant( const uint32 value )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeUint32;
	mValue.value_uint = value;
}

Variant::Variant( const int64 value )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeInt64;
	mValue.value_int64 = value;
}

Variant::Variant( const uint64 value )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeUint64;
	mValue.value_uint64 = value;
}

Variant::Variant( const float32 value )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeFloat32;
	mValue.value_float = value;
}

Variant::Variant( const float64 value )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeFloat64;
	mValue.value_double = value;
}

Variant::Variant(const std::string& value)
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypeString;
	int len = value.length() + 1;
	mValue.value_str = new char[len];
	strcpy_s(mValue.value_str, len, value.c_str());
}

Variant::Variant( const void* value )
{
	memset(&mValue, 0, sizeof(mValue));
	mType = TypePointer;
	mValue.value_pointer = (void*)value;
}

Variant& Variant::operator=( const Variant& value )
{
	reset();
	mType = value.mType;
	copyValue(value.mValue, mValue, mType);
	return *this;
}

Variant& Variant::operator=( const bool value )
{
	reset();

	mType = TypeBoolean;
	mValue.value_bool = value;

	return *this;
}

Variant& Variant::operator=( const int8 value )
{
	reset();
	mType = TypeInt8;
	mValue.value_char = value;

	return *this;
}

Variant& Variant::operator=( const uint8 value )
{
	reset();
	mType = TypeUint8;
	mValue.value_uchar = value;

	return *this;
}

Variant& Variant::operator=( const int16 value )
{
	reset();
	mType = TypeInt16;
	mValue.value_short = value;

	return *this;
}

Variant& Variant::operator=( const uint16 value )
{
	reset();
	mType = TypeUint16;
	mValue.value_ushort = value;

	return *this;
}

Variant& Variant::operator=( const int32 value )
{
	reset();
	mType = TypeInt32;
	mValue.value_int = value;

	return *this;
}

Variant& Variant::operator=( const uint32 value )
{
	reset();
	mType = TypeUint32;
	mValue.value_uint = value;

	return *this;
}

Variant& Variant::operator=( const int64 value )
{
	reset();
	mType = TypeInt64;
	mValue.value_int64 = value;

	return *this;
}

Variant& Variant::operator=( const uint64 value )
{
	reset();
	mType = TypeUint64;
	mValue.value_uint64 = value;

	return *this;
}

Variant& Variant::operator=( const float32 value )
{
	reset();
	mType = TypeFloat32;
	mValue.value_float = value;

	return *this;
}

Variant& Variant::operator=( const float64 value )
{
	reset();
	mType = TypeFloat64;
	mValue.value_double = value;

	return *this;
}

Variant& Variant::operator=(const std::string& value)
{
	reset();
	mType = TypeString;
	int len = value.length() + 1;
	mValue.value_str = new char[len];
	strcpy_s(mValue.value_str, len, value.c_str());

	return *this;
}

Variant& Variant::operator=( const void* value )
{
	reset();
	mType = TypePointer;
	mValue.value_pointer = (void*)value;

	return *this;
}

bool Variant::operator>( const Variant& other ) const
{
	return !(*this <= other);
}

bool Variant::operator>=( const Variant& other ) const
{
	if (mType != other.mType)
		return mType >= other.mType;

	switch (mType)
	{
	case TypeBoolean:
		return mValue.value_bool >= other.mValue.value_bool;
	case TypeInt8:
		return mValue.value_char >= other.mValue.value_char;
	case TypeUint8:
		return mValue.value_uchar >= other.mValue.value_uchar;
	case TypeInt16:
		return mValue.value_short >= other.mValue.value_short;
	case TypeUint16:
		return mValue.value_ushort >= other.mValue.value_ushort;
	case TypeInt32:
		return mValue.value_int >= other.mValue.value_int;
	case TypeUint32:
		return mValue.value_uint >= other.mValue.value_uint;
	case TypeInt64:
		return mValue.value_int64 >= other.mValue.value_int64;
	case TypeUint64:
		return mValue.value_uint64 >= other.mValue.value_uint64;
	case TypeFloat32:
		return mValue.value_float >= other.mValue.value_float;
	case TypeFloat64:
		return mValue.value_double >= other.mValue.value_double;
	case TypeString:
		return strcmp(mValue.value_str, other.mValue.value_str) >= 0;
	case TypeMemory:
		return memcmp(mValue.value_bytes->getPtr(), other.mValue.value_bytes->getPtr(), mValue.value_bytes->getWPostion() <= other.mValue.value_bytes->getWPostion() ? mValue.value_bytes->getWPostion() : other.mValue.value_bytes->getWPostion()) >= 0;
	case TypePointer:
		return mValue.value_pointer >= other.mValue.value_pointer;
	}

	printf("warn: type is None\n");
	return false;
}

bool Variant::operator<( const Variant& value ) const
{
	return !(*this >= value);
}

bool Variant::operator<=( const Variant& other ) const
{
	if (mType != other.mType)
		return mType <= other.mType;

	switch (mType)
	{
	case TypeBoolean:
		return mValue.value_bool <= other.mValue.value_bool;
	case TypeInt8:
		return mValue.value_char <= other.mValue.value_char;
	case TypeUint8:
		return mValue.value_uchar <= other.mValue.value_uchar;
	case TypeInt16:
		return mValue.value_short <= other.mValue.value_short;
	case TypeUint16:
		return mValue.value_ushort <= other.mValue.value_ushort;
	case TypeInt32:
		return mValue.value_int <= other.mValue.value_int;
	case TypeUint32:
		return mValue.value_uint <= other.mValue.value_uint;
	case TypeInt64:
		return mValue.value_int64 <= other.mValue.value_int64;
	case TypeUint64:
		return mValue.value_uint64 <= other.mValue.value_uint64;
	case TypeFloat32:
		return mValue.value_float <= other.mValue.value_float;
	case TypeFloat64:
		return mValue.value_double <= other.mValue.value_double;
	case TypeString:
		return strcmp(mValue.value_str, other.mValue.value_str) <= 0;
	case TypeMemory:
		return memcmp(mValue.value_bytes->getPtr(), other.mValue.value_bytes->getPtr(), mValue.value_bytes->getWPostion() <= other.mValue.value_bytes->getWPostion() ? mValue.value_bytes->getWPostion() : other.mValue.value_bytes->getWPostion()) <= 0;
	case TypePointer:
		return mValue.value_pointer <= other.mValue.value_pointer;
	}

	printf("warn: type is None\n");
	return false;
}

bool Variant::operator==( const Variant& other ) const
{
	if (mType != other.mType)
		return false;

	switch (mType)
	{
	case TypeBoolean:
		return mValue.value_bool == other.mValue.value_bool;
	case TypeInt8:
		return mValue.value_char == other.mValue.value_char;
	case TypeInt16:
		return mValue.value_short == other.mValue.value_short;
	case TypeInt32:
		return mValue.value_int == other.mValue.value_int;
	case TypeInt64:
		return mValue.value_int64 == other.mValue.value_int64;
	case TypeUint8:
		return mValue.value_uchar == other.mValue.value_uchar;
	case TypeUint16:
		return mValue.value_ushort == other.mValue.value_ushort;
	case TypeUint32:
		return mValue.value_uint == other.mValue.value_uint;
	case TypeUint64:
		return mValue.value_uint64 == other.mValue.value_uint64;
	case TypeFloat32:
		return mValue.value_float == other.mValue.value_float;
	case TypeFloat64:
		return mValue.value_double == other.mValue.value_double;
	case TypeString:
		return strcmp(mValue.value_str, other.mValue.value_str) == 0;
	case TypeMemory:
		return memcmp(mValue.value_bytes->getPtr(), other.mValue.value_bytes->getPtr(), mValue.value_bytes->getWPostion() <= other.mValue.value_bytes->getWPostion() ? mValue.value_bytes->getWPostion() : other.mValue.value_bytes->getWPostion()) == 0;
	case TypePointer:
		return mValue.value_pointer == other.mValue.value_pointer;
	}

	printf("warn: type is None\n");
	return false;
}

bool Variant::operator!= (const Variant& other) const
{
	return !(*this == other);
}

Variant& Variant::operator=(const BinaryStream& value)
{
	reset();
	mType = TypeMemory;
	mValue.value_bytes = mValue.value_bytes ? mValue.value_bytes : new BinaryStream;
	mValue.value_bytes->WriteBytes(value.getPtr(), value.getWPostion());
	return *this;
}

Variant::operator int8()
{
	return valueInt8();
}

Variant::operator uint8()
{
	return valueUint8();
}

Variant::operator int16()
{
	return valueInt16();
}

Variant::operator uint16()
{
	return valueUint16();
}

Variant::operator int32()
{
	return valueInt32();
}

Variant::operator uint32()
{
	return valueUint32();
}

Variant::operator int64()
{
	return valueInt64();
}

Variant::operator uint64()
{
	return valueUint64();
}

Variant::operator float32()
{
	return valueFloat32();
}

Variant::operator float64()
{
	return valueFloat64();
}

Variant::operator char*()
{
	return (char*)valueString();
}

Variant::operator const char*()
{
	return valueString();
}

Variant::operator BinaryStream&()
{
	return (BinaryStream&)valueBytes();
}

const std::string Variant::toString() const
{
	char buffer[64] = {0};
	switch (mType)
	{
	case TypeBoolean:
		return mValue.value_bool ? "true" : "false";
	case TypeInt8:
		sprintf_s(buffer, sizeof(buffer), "%d", mValue.value_char);
		return buffer;
	case TypeInt16:
		sprintf_s(buffer, sizeof(buffer), "%d", mValue.value_short);
		return buffer;
	case TypeInt32:
		sprintf_s(buffer, sizeof(buffer), "%d", mValue.value_int);
		return buffer;
	case TypeInt64:
		sprintf_s(buffer, sizeof(buffer), "%I64d", mValue.value_int64);
		return buffer;
	case TypeUint8:
		sprintf_s(buffer, sizeof(buffer), "%d", mValue.value_uchar);
		return buffer;
	case TypeUint16:
		sprintf_s(buffer, sizeof(buffer), "%u", mValue.value_ushort);
		return buffer;
	case TypeUint32:
		sprintf_s(buffer, sizeof(buffer), "%u", mValue.value_uint);
		return buffer;
	case TypeUint64:
		sprintf_s(buffer, sizeof(buffer), "%I64u", mValue.value_uint64);
		return buffer;
	case TypeFloat32:
		sprintf_s(buffer, sizeof(buffer), "%04f", mValue.value_float);
		return buffer;
	case TypeFloat64:
		sprintf_s(buffer, sizeof(buffer), "%08f", mValue.value_double);
		return buffer;
	case TypeString:
		return mValue.value_str == NULL ? "" : mValue.value_str;
	case TypeMemory:
		return "Bytes";
	case TypePointer:
		sprintf_s(buffer, sizeof(buffer), "%p", mValue.value_pointer);
		return buffer;
	}

	return "";
}

const bool Variant::valueBool() const
{
	return mValue.value_bool;
}

const int8 Variant::valueInt8() const
{
	return mValue.value_char;
}

const uint8 Variant::valueUint8() const
{
	return mValue.value_uchar;
}

const int16 Variant::valueInt16() const
{
	return mValue.value_short;
}

const uint16 Variant::valueUint16() const
{
	return mValue.value_ushort;
}

const int32 Variant::valueInt32() const
{
	return mValue.value_int;
}

const uint32 Variant::valueUint32() const
{
	return mValue.value_uint;
}

const int64 Variant::valueInt64() const
{
	return mValue.value_int64;
}

const uint64 Variant::valueUint64() const
{
	return mValue.value_uint64;
}

const float32 Variant::valueFloat32() const
{
	return mValue.value_float;
}

const float64 Variant::valueFloat64() const
{
	return mValue.value_double;
}

const char* Variant::valueString() const
{
	if (mType == TypeString)
		return mValue.value_str == NULL ? "" : mValue.value_str;
	return "";
}

const void* Variant::valuePointer() const
{
	if (mType == TypePointer)
		return mValue.value_pointer;
	return NULL;
}

const BinaryStream& Variant::valueBytes()
{
	mType = mType == TypeNone ? TypeMemory : mType;
	mValue.value_bytes = mValue.value_bytes ? mValue.value_bytes : new BinaryStream;
	return *mValue.value_bytes;
}

void Variant::reset()
{
	if (mType == TypeString)
	{
		delete[] mValue.value_str;
		mValue.value_str = 0;
	}

	if (mType == TypeMemory)
	{
		if (mValue.value_bytes)
			mValue.value_bytes->Clear();
	}
}

void Variant::copyValue(const ValueUnion& src, ValueUnion& des, int8 type)
{
	switch (type)
	{
	case TypeBoolean:
		des.value_bool = src.value_bool;
		break;
	case TypeInt8:
		des.value_char = src.value_char;
		break;
	case TypeUint8:
		des.value_uchar = src.value_uchar;
		break;
	case TypeInt16:
		des.value_short = src.value_short;
		break;
	case TypeUint16:
		des.value_ushort = src.value_ushort;
		break;
	case TypeInt32:
		des.value_int = src.value_int;
		break;
	case TypeUint32:
		des.value_uint = src.value_uint;
		break;
	case TypeInt64:
		des.value_int64 = src.value_int64;
		break;
	case TypeUint64:
		des.value_uint64 = src.value_uint64;
		break;
	case TypeFloat32:
		des.value_float = src.value_float;
		break;
	case TypeFloat64:
		des.value_double = src.value_double;
		break;
	case TypeString:
		delete [] des.value_str;
		des.value_str = new char[strlen(src.value_str) + 1];
		strcpy_s(des.value_str, strlen(src.value_str) + 1, src.value_str);
		break;
	case TypePointer:
		break;
	case TypeDate:
		break;
	case TypeMemory:

		des.value_bytes = des.value_bytes ? des.value_bytes : new BinaryStream;
		des.value_bytes->Clear();
		if (src.value_bytes)
			des.value_bytes->WriteBytes(src.value_bytes->getPtr(), src.value_bytes->getWPostion());

		break;
	default:
		break;
	}
}

char Variant::StringToType(const char* str)
{
	if (strcmp(str, "bool") == 0)
		return TypeBoolean;
	else if (strcmp(str, "int8") == 0)
		return TypeInt8;
	else if (strcmp(str, "uint8") == 0)
		return TypeUint8;
	else if (strcmp(str, "int16") == 0)
		return TypeInt16;
	else if (strcmp(str, "uint16") == 0)
		return TypeUint16;
	else if (strcmp(str, "int32") == 0)
		return TypeInt32;
	else if (strcmp(str, "uint32") == 0)
		return TypeUint32;
	else if (strcmp(str, "int64") == 0)
		return TypeInt64;
	else if (strcmp(str, "uint64") == 0)
		return TypeUint64;
	else if (strcmp(str, "float32") == 0)
		return TypeFloat32;
	else if (strcmp(str, "float64") == 0)
		return TypeFloat64;
	else if (strcmp(str, "string") == 0)
		return TypeString;
	return TypeNone;
}
