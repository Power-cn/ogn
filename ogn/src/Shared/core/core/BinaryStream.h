#pragma once

#include "Shared.h"
class Variant;
class Object;

struct StreamBuffer
{
	int8* buffer = 0;
	int32 length = 0;
};

class BinaryStream
{
public:
	BinaryStream(int32 count = BINARY_LENGTH);
	BinaryStream(void* ptr, int32 count, bool isDel = false);
	BinaryStream(const BinaryStream& other);
	~BinaryStream(void);
	BinaryStream& operator = (const BinaryStream& other);
	int32 getCount() const { return mCount; }
	int32 getWPostion() const { return mWritePosition; }
	int32 getRPostion() const { return mReadPosition; }
	void* getPtr() const { return mBytes; }

	void setWPostion(int32 pos) { mWritePosition = pos; }
	void setRPostion(int32 pos) { mReadPosition = pos; }
	void setResize(bool v) { mResize = v; }
	void Clear();
public:
	bool WriteBytes(const void* data, int32 count);
	bool PushBytes(int32 pos, const void* data, int32 count);

	bool operator << (const float32& value);
	bool operator << (const float64& value);
	bool operator << (const std::string& value);
	bool operator << (const char* value);
	bool operator << (const int8 value);
	bool operator << (const uint8 value);
	bool operator << (const int16 value);
	bool operator << (const uint16 value);
	bool operator << (const int32 value);
	bool operator << (const uint32 value);
	bool operator << (const int64 value);
	bool operator << (const uint64 value);
	bool operator << (const Variant& value);
	bool operator << (const Object& value);
	bool operator << (BinaryStream& value);
public:
	bool ReadBytes(void* data, int32 count);
	bool PopBytes(int32 pos, void* data, int32 count);
	bool operator >> (float32& value);
	bool operator >> (float64& value);
	bool operator >> (std::string& value);
	bool operator >> (int8& value);
	bool operator >> (uint8& value);
	bool operator >> (int16& value);
	bool operator >> (uint16& value);
	bool operator >> (int32& value);
	bool operator >> (uint32& value);
	bool operator >> (int64& value);
	bool operator >> (uint64& value);	
	bool operator >> (Variant& value);
	bool operator >> (Object& value);
	bool operator >> (BinaryStream& value);
protected:
	void resize(int32 size);
protected:
	char*			mBytes;
	int32			mCount;
	int32			mWritePosition;
	int32			mReadPosition;
	bool			mIsDel;
	bool			mResize;
};