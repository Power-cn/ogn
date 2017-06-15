#pragma once

// 数据字段类型;
enum KeyType
{
	KeyTypeNone,
	KeyTypeAutoInc,		// 自增长;
};

struct FieldDescriptor
{
	const int8*				field;
	const int8*				default;
	int32					offset;
	int8					typevalue;
	int8					typekey;
};

struct TableDescriptor
{
	FieldDescriptor*			records;
	int32						recordCount;
	const int8*					tableName;
	FieldDescriptor* getFieldDescriptor(const std::string& field) const;
};

class DBRecord : public EventDispatcher
{
	DECLARE_CLASS(DBRecord)
public:
	virtual const TableDescriptor* getDescriptor()  { static TableDescriptor desc; return &desc; }
	virtual bool operator >> (BinaryStream& bytes);
	virtual bool operator << (BinaryStream& bytes);
public:
	int32 id = 0;
};

class DBResult
{
public:
	DBRecord* record;
	std::string cmd;
	std::string compareRecord;
	std::string exclutionRecord;
	std::string err;
	DBResult();
	virtual ~DBResult();
};

class QueryResult : public DBResult
{
public:
	DBRecord* records;
	uint32* queryCount;
	uint32 resultCount;
	std::string resultRecord;
	QueryResult();
	virtual ~QueryResult();
};

class InsertResult : public DBResult
{
public:
	std::string resultRecord;
	InsertResult();
	virtual ~InsertResult();
};

class UpdateResult : public DBResult
{
public:
	uint32* updateRows;
	std::string updateRecord;
	UpdateResult();
	virtual ~UpdateResult();
};


class DeleteResult : public DBResult
{
public:
	uint32* deleteRows;
	DeleteResult();
	virtual ~DeleteResult();
};

#define DECLARE_TABLE(class_name)\
public:\
	const TableDescriptor* getDescriptor(); \

//#define IMPLEMENT_TABLE_BEGIN(class_name, table_name) \
//	FieldDescriptor class_name::s_##class_name##records = { \
//	
//#define IMPLEMENT_RECORD_INFO(class_name, member_name, value_type_, default_value, key_type_) \
//{ #member_name, default_value, sizeof(object_->member_name), offsetof(class_name, member_name), value_type_, key_type_ },
//
//
//#define IMPLEMENT_TABLE_END(class_name, table_name) \
//}; \
//	TableDescriptor class_name::s_##class_name##_descriptor = { \
//	class_name::s_##class_name##records, sizeof(class_name::s_##class_name##records) / sizeof(FieldDescriptor), table_name\
//}\

#define IMPLEMENT_TABLE_BEGIN(class_name, table_name) \
	const TableDescriptor* class_name::getDescriptor()\
{\
	class_name* object_ = 0; \
	static FieldDescriptor s_##class_name##records[] = {

#define IMPLEMENT_RECORD_INFO(class_name, member_name, value_type_, default_value, key_type_) \
{ #member_name, default_value, offsetof(class_name, member_name), value_type_, key_type_ },

#define IMPLEMENT_TABLE_END(class_name, table_name) \
}; \
	static TableDescriptor s_##class_name##_descriptor = { s_##class_name##records, sizeof(s_##class_name##records) / sizeof(FieldDescriptor), table_name }; \
	return &s_##class_name##_descriptor; \
}


#define SQL_CMD_COUNT			10240

#define cast_value(ptr, class_name)		*((class_name*)ptr)



void GetRecordValue(void* mysql, DBRecord& record, const FieldDescriptor& field, uint32 valuestrlength, int8* valuestr, uint32& valuesize);
void GetValueRecord(void* mysql, DBRecord& record, const FieldDescriptor& field, const int8* valuestr, uint32 valuesize);
bool GetQuerySqlCmd(void* mysql, char* sql_cmd, uint32& size, DBRecord& query_record, DBRecord* result_records, uint32 result_max_count, const std::string& compare_record_names, const std::string& return_record_names);
bool GetInsertSqlCmd(void* mysql, char* sql_cmd, size_t& size, DBRecord& insert_record, const std::string& compare_record_names);
bool GetUpdateSqlCmd(void* mysql, char* sql_cmd, int32& size, DBRecord& update_record, const std::string& compare_record_names, const std::string& update_record_names);
bool GetDeleteSqlCmd(void* mysql, char* sql_cmd, uint32& size, DBRecord& delete_record, const std::string& compare_record_names);
