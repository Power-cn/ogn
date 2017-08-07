#pragma once

#define ACCOUNT_SIZE 32
#pragma pack(1)

class DBAccount :public DBRecord
{
public:
	virtual bool operator >> (BinaryStream& bytes);
	virtual bool operator << (BinaryStream& bytes);

public:
	std::string user;
	BinaryStream password;
	std::string name;
	std::string email;
	std::string phone;
	int32 createTime = 0;

	DECLARE_TABLE(DBAccount)
};

class DBUser : public DBRecord
{
public:
	virtual bool operator >> (BinaryStream& bytes);
	virtual bool operator << (BinaryStream& bytes);

public:
	int32 accountId = 0;
	std::string name;
	BinaryStream property;
	int32 createTime = 0;

	DECLARE_TABLE(DBUser)
};

#pragma pack()