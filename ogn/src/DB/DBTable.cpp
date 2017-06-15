#include "stdafx.h"


IMPLEMENT_TABLE_BEGIN(DBAccount, "account")

IMPLEMENT_RECORD_INFO(DBAccount, id, Variant::TypeInt32, "NULL", KeyTypeAutoInc)
IMPLEMENT_RECORD_INFO(DBAccount, user, Variant::TypeString, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBAccount, password, Variant::TypeMemory, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBAccount, name, Variant::TypeString, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBAccount, email, Variant::TypeString, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBAccount, phone, Variant::TypeString, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBAccount, createTime, Variant::TypeInt32, "NULL", KeyTypeNone)

IMPLEMENT_TABLE_END(DBAccount, "account")

bool DBAccount::operator >> (BinaryStream& bytes)
{
	CHECK(__super::operator >>(bytes));
	CHECK(bytes << user);
	CHECK(bytes << password);
	CHECK(bytes << name);
	CHECK(bytes << email);
	CHECK(bytes << phone);
	CHECK(bytes << createTime);
	return true;
}

bool DBAccount::operator<<(BinaryStream& bytes)
{
	CHECK(__super::operator >>(bytes));
	CHECK(bytes >> user);
	CHECK(bytes >> password);
	CHECK(bytes >> name);
	CHECK(bytes >> email);
	CHECK(bytes >> phone);
	CHECK(bytes >> createTime);
	return true;
}


IMPLEMENT_TABLE_BEGIN(DBUser, "user")

IMPLEMENT_RECORD_INFO(DBUser, id, Variant::TypeInt32, "NULL", KeyTypeAutoInc)
IMPLEMENT_RECORD_INFO(DBUser, accountId, Variant::TypeInt32, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBUser, name, Variant::TypeString, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBUser, property, Variant::TypeMemory, "NULL", KeyTypeNone)
IMPLEMENT_RECORD_INFO(DBUser, createTime, Variant::TypeInt32, "NULL", KeyTypeNone)

IMPLEMENT_TABLE_END(DBUser, "user")

bool DBUser::operator >> (BinaryStream& bytes)
{
	CHECK(__super::operator >>(bytes));
	CHECK(bytes << accountId);
	CHECK(bytes << name);
	CHECK(bytes << property);
	CHECK(bytes << createTime);
	return true;
}

bool DBUser::operator<<(BinaryStream& bytes)
{
	CHECK(__super::operator <<(bytes));
	CHECK(bytes >> accountId);
	CHECK(bytes >> name);
	CHECK(bytes >> property);
	CHECK(bytes >> createTime);
	return true;
}
