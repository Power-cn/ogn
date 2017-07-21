#include "mysql.hpp"
#include <mysql.h>

DBConnector::DBConnector()
{
	mMysql = new MYSQL;
}

DBConnector::~DBConnector()
{
	delete mMysql;
}

bool DBConnector::connect(const std::string& host, const std::string& user, const std::string& password, const std::string& name, int16 port)
{
	mHost = host;
	mUser = user;
	mPassword = password;
	mName = name;
	mPort = port;

	//_threader = new Threader;
	//_threader->create(this, (Threader::ThreadCallBack)(&DBConnector::threaderRun));
	//_threader->create(this);
	mysql_init(mMysql);

#ifdef WIN64
	mysql_options4(mMysql, MYSQL_SET_CHARSET_NAME, "gb2312", "gb2312");
#else
	mysql_options(mMysql, MYSQL_SET_CHARSET_NAME, "gb2312");
#endif // WIN64

	my_bool reconnect = true;
	mysql_options(mMysql, MYSQL_OPT_RECONNECT, &reconnect);
	if (!mysql_real_connect(mMysql, mHost.c_str(), mUser.c_str(), mPassword.c_str(), mName.c_str(), mPort, NULL, 0))
	{
		const char* err = mysql_error(mMysql);
		return false;
	}

	//mysql_query(sql_account, "set character set'gbk'");
	//mysql_query(sql_account, "set names'gbk'");
	//mysql_set_character_set(sql_account, "set character set'gbk'");

	//if (!mysql_set_character_set(sql_account, "utf8"))
	//{
	//	LOG_DEBUG(LogSystem::csl_color_green, "<<<New client character set: [%s] ...", mysql_character_set_name(sql_account));
	//}

	//_threader = Threader::createThread(this, (Threader::ThreadCallBack)(&DBConnector::threaderRun));

	return true;
}

int8* DBConnector::Query(const DBRecord& query_record_, DBRecord* result_records_, uint32* result_count_, const int8* compare_record_names_, const int8* return_record_names_, int32 result_max_count /* = 0 */)
{
	//return doQuery((DBRecord&)query_record_, result_records_, result_count_, compare_record_names_, return_record_names_, result_max_count);
	QueryResult* result = new QueryResult;
	result->record = (DBRecord*)&query_record_;
	result->records = result_records_;
	result->queryCount = result_count_;
	result->resultCount = result_max_count;
	result->compareRecord = compare_record_names_ == NULL ? "" : compare_record_names_;
	result->resultRecord = return_record_names_ == NULL ? "" : return_record_names_;

	_mutex.lock();
	taskQueryQueue.push(result);		
	_mutex.unlock();
	return 0;
}

int8* DBConnector::Query(const DBRecord& query_record_, DBRecord* result_records, uint32* result_count_, const uint32 result_max_count /* = 0 */)
{
	//return doQuery((DBRecord&)query_record_, result_records, result_count_, result_max_count);
	QueryResult* result = new QueryResult;
	result->record = (DBRecord*)&query_record_;
	result->records = result_records;
	result->queryCount = result_count_;
	result->resultCount = result_max_count;
	_mutex.lock();
	taskQueryQueue.push(result);
	_mutex.unlock();
	return 0;
}

int8* DBConnector::Query(const DBRecord& query_record_, const int8* sql_name_, DBRecord* result_records_, uint32* result_count_, uint32 result_max_count /*= 0*/)
{
	//return doQuery((DBRecord&)query_record_, sql_name_, result_records_, result_count_, result_max_count);
	QueryResult* result = new QueryResult;
	result->record = (DBRecord*)&query_record_;
	result->records = result_records_;
	result->queryCount = result_count_;
	result->resultCount = result_max_count;
	result->cmd = sql_name_;
	_mutex.lock();
	taskQueryQueue.push(result);
	_mutex.unlock();
	return 0;
}

int8* DBConnector::Insert(const DBRecord& insert_record_, const int8* compare_record_names_ /* = 0 */, const int8* exclution_record_names_ /* = 0 */)
{
	//return doInsert((DBRecord&)insert_record_, compare_record_names_);
	InsertResult* result = new InsertResult;
	result->record = (DBRecord*)&insert_record_;
	result->compareRecord = compare_record_names_ == NULL ? "" : compare_record_names_;
	result->exclutionRecord = exclution_record_names_ == NULL ? "" : exclution_record_names_;
	_mutex.lock();
	taskInsertQueue.push(result);
	_mutex.unlock();
	return 0;
}

int8* DBConnector::Update(const DBRecord& update_record_, const int8* compare_record_names_, uint32* update_rows_, const int8* update_record_names_)
{
	//return doUpdate((DBRecord&)update_record_, compare_record_names_, update_rows_, update_record_names_);
	UpdateResult* result = new UpdateResult;
	result->record = (DBRecord*)&update_record_;
	result->updateRows = update_rows_;

	result->compareRecord = compare_record_names_ == NULL ? "" : compare_record_names_;
	result->updateRecord = update_record_names_ == NULL ? "" : update_record_names_;
	_mutex.lock();
	taskUpdateQueue.push(result);
	_mutex.unlock();
	return 0;
}

int8* DBConnector::Update(const DBRecord& update_record_, const int8* sql_name_, uint32* update_rows_)
{
	UpdateResult* result = new UpdateResult;
	result->record = (DBRecord*)&update_record_;
	result->updateRows = update_rows_;
	result->cmd = sql_name_;
	_mutex.lock();
	taskUpdateQueue.push(result);
	_mutex.unlock();
	return 0;
}

int8* DBConnector::Delete(const DBRecord& delete_record_, const int8* compare_record_names_, uint32* update_rows_)
{
	DeleteResult* result = new DeleteResult;
	result->record = (DBRecord*)&delete_record_;

	result->compareRecord = compare_record_names_ == NULL ? "" : compare_record_names_;
	result->deleteRows = update_rows_;
	_mutex.lock();
	taskDeleteQueue.push(result);
	_mutex.unlock();
	return 0;
}

void DBConnector::process()
{

	if (taskQueryQueue.size() > 0)
	{
		_mutex.lock();
		QueryResult* query = taskQueryQueue.front();
		taskQueryQueue.pop();
		_mutex.unlock();

		int8* err = 0;

		if (query->cmd.length() > 0)
		{
			err = doQuery(*query->record, query->cmd, query->records, *query->queryCount, (uint32)query->resultCount);
		}
		else
		{
			if (query->compareRecord.length() > 0)
			{
				err = doQuery(*query->record, query->records, *query->queryCount, query->compareRecord, query->resultRecord, (uint32)query->resultCount);
			}
			else
			{
				err = doQuery(*query->record, query->records, *query->queryCount, (uint32)query->resultCount);
			}

		}

		if (err)
			query->err = err;


		_mutex.lock();
		completeQueryQueue.push(query);
		_mutex.unlock();
	}

	if (taskInsertQueue.size() > 0)
	{
		_mutex.lock();
		InsertResult* insert = taskInsertQueue.front();
		taskInsertQueue.pop();
		_mutex.unlock();

		int8* err = 0;
		if (insert->compareRecord.length() > 0)
		{
			err = doInsert(*insert->record, insert->compareRecord.c_str(), insert->exclutionRecord.c_str());

		}
		else
		{
			err = doInsert(*insert->record, "", insert->exclutionRecord.c_str());
		}
		if (err)
			insert->err = err;

		_mutex.lock();
		completeInsertQueue.push(insert);
		_mutex.unlock();
	}

	if (taskUpdateQueue.size() > 0)
	{
		_mutex.lock();
		UpdateResult* update = taskUpdateQueue.front();
		taskUpdateQueue.pop();
		_mutex.unlock();

		int8* err = 0;
		if (update->cmd.length() > 0)
		{
			err = doUpdate(*update->record, update->cmd.c_str(), *update->updateRows);
		}
		else
		{
			err = doUpdate(*update->record, (const int8*)update->compareRecord.c_str(), *update->updateRows, (const int8*)update->updateRecord.c_str());
		}
		if (err)
			update->err = err;

		_mutex.lock();
		completeUpdateQueue.push(update);
		_mutex.unlock();
	}

	if (taskDeleteQueue.size() > 0)
	{
		_mutex.lock();
		DeleteResult* deleteResult = taskDeleteQueue.front();
		taskDeleteQueue.pop();
		_mutex.unlock();

		int8* err = 0;
		err = doDelete(*deleteResult->record, deleteResult->compareRecord.c_str(), *deleteResult->deleteRows);
		if (err)
			deleteResult->err = err;

		_mutex.lock();
		completeDeleteQueue.push(deleteResult);
		_mutex.unlock();
	}


}

int8* DBConnector::doQuery(const DBRecord& query_record, DBRecord* result_records, uint32& result_count, const uint32 result_max_count /*= 0*/)
{
	char sql_cmd_[SQL_CMD_COUNT] = { 0 };
	uint32 sql_size_ = 0;
	if (!GetQuerySqlCmd(mMysql, sql_cmd_, sql_size_, (DBRecord&)query_record, result_records, result_max_count, NULL, NULL))
	{
		return "";
	}
	return 0;
}

int8* DBConnector::doQuery(const DBRecord& query_record, DBRecord* result_records, uint32& result_count, const std::string& compare_record_names, const std::string& return_record_names, int32 result_max_count /* = 0 */)
{
	char sql_cmd_[SQL_CMD_COUNT] = { 0 };
	uint32 sql_size_ = 0;
	GetQuerySqlCmd(mMysql, sql_cmd_, sql_size_, (DBRecord&)query_record, result_records, result_max_count, compare_record_names.c_str(), return_record_names.c_str());
	if (int iError = mysql_real_query(mMysql, sql_cmd_, (unsigned long)strlen(sql_cmd_)))
	{
		return (int8*)mysql_error(mMysql);
	}

	MYSQL_RES* result_ = mysql_store_result(mMysql);
	if (result_ == NULL)
	{
		const int8* err = mysql_error(mMysql);
		if (err || strlen(err) == 0)
			return NULL;
		else
			return (int8*)err;
	}

	int32 num_rows_ = (int32)mysql_num_rows(result_);
	if (result_max_count && num_rows_ > result_max_count)
		num_rows_ = result_max_count;

	if (result_records)
	{
		const TableDescriptor& descriptor_ = *((DBRecord&)query_record).getDescriptor();
		MYSQL_ROW row_ = 0;
		for (int32 index_ = 0; index_ < num_rows_; ++index_)
		{
			if (!(row_ = mysql_fetch_row(result_)))
			{
				return "fetch result failed\n";
			}

			unsigned long* lengths = mysql_fetch_lengths(result_);

			size_t col_index_ = 0;
			MYSQL_FIELD* field = mysql_fetch_field(result_);
			while (field)
			{
				FieldDescriptor* record = descriptor_.getFieldDescriptor(field->name);
				if (record)
					GetValueRecord(mMysql, result_records[index_], (const FieldDescriptor&)*record, row_[col_index_], lengths[col_index_]);
				
				field = mysql_fetch_field(result_);
				col_index_++;
			}
		}
	}

	result_count = num_rows_;
	mysql_free_result(result_);

	return 0;
}

int8* DBConnector::doQuery(const DBRecord& query_record, const std::string& sql_name, DBRecord* result_records, uint32& result_count, uint32 result_max_count)
{
	if (int iError = mysql_real_query(mMysql, sql_name.c_str(), (unsigned long)sql_name.length()))
	{
		return (int8*)mysql_error(mMysql);
	}

	MYSQL_RES* result_ = mysql_store_result(mMysql);
	if (result_ == NULL)
	{
		const char* err = mysql_error(mMysql);
		if (err || strlen(err) == 0)
			return NULL;
		else
			return (int8*)err;
	}

	uint32 num_rows_ = (size_t)mysql_num_rows(result_);
	if (result_max_count && num_rows_ > result_max_count)
	{
		num_rows_ = result_max_count;
	}

	if (result_records)
	{
		const TableDescriptor& descriptor_ = *((DBRecord&)query_record).getDescriptor();
		MYSQL_ROW row_ = 0;
		for (uint32 index_ = 0; index_ < num_rows_; ++index_)
		{
			if (!(row_ = mysql_fetch_row(result_)))
			{
				return "fetch result failed\n";
			}
			unsigned long* lengths = mysql_fetch_lengths(result_);

			size_t col_index_ = 0;
			MYSQL_FIELD* field = mysql_fetch_field(result_);
			while (field)
			{
				FieldDescriptor* record = descriptor_.getFieldDescriptor(field->name);
				if (record)
					GetValueRecord(mMysql, result_records[index_], (const FieldDescriptor&)*record, row_[col_index_], lengths[col_index_]);

				field = mysql_fetch_field(result_);
				col_index_++;
			}
		}
	}

	result_count = num_rows_;

	mysql_free_result(result_);
	return 0;
}

int8* DBConnector::doInsert(const DBRecord& insert_record, const std::string& compare_record_names /* = "" */, const std::string& exclution_record_names /* = "" */)
{
	if (exclution_record_names.length() > 0)
	{
		uint32 count = 0;
		doQuery(insert_record, (DBRecord*)&insert_record, count, exclution_record_names, "");
		if (count > 0) return "ÒÑ´æÔÚ..";
	}

	char sql_cmd_[SQL_CMD_COUNT] = { 0 };
	size_t size_ = 0;

	if (!GetInsertSqlCmd(mMysql, sql_cmd_, size_, (DBRecord&)insert_record, compare_record_names.c_str()))
		return "error get insert sql cmd .";

	if (int err = mysql_real_query(mMysql, sql_cmd_, (unsigned long)strlen(sql_cmd_)))
		return (int8*)mysql_error(mMysql);

	DBRecord& record = (DBRecord&)insert_record;
	record.id = (int32)mysql_insert_id(mMysql);

	return 0;
}

int8* DBConnector::doUpdate(const DBRecord& update_record, const std::string& compare_record_names, uint32& update_rows, const std::string& update_record_names)
{
	char sql_cmd_[SQL_CMD_COUNT] = { 0 };
	int32 size_ = 0;

	if (!GetUpdateSqlCmd(mMysql, sql_cmd_, size_, (DBRecord&)update_record, compare_record_names.c_str(), update_record_names.c_str()))
		return "";

	if (int iError = mysql_real_query(mMysql, sql_cmd_, (unsigned long)strlen(sql_cmd_)))
		return (int8*)mysql_error(mMysql);

	update_rows = (int)mysql_affected_rows(mMysql);

	return 0;
}

int8* DBConnector::doUpdate(const DBRecord& update_record, const std::string& sql_name, uint32& update_rows)
{
	return 0;
}

int8* DBConnector::doDelete(const DBRecord& delete_record, const std::string& compare_record_names, uint32& update_rows)
{
	char sql_cmd_[SQL_CMD_COUNT] = { 0 };
	uint32 size_ = 0;

	if (!GetDeleteSqlCmd(mMysql, sql_cmd_, size_, (DBRecord&)delete_record, compare_record_names))
		return "";

	if (int iError = mysql_real_query(mMysql, sql_cmd_, (unsigned long)strlen(sql_cmd_)))
		return (int8*)mysql_error(mMysql);

	update_rows = (unsigned long)mysql_affected_rows(mMysql);

	return 0;
}

void DBConnector::update(float time, float delay)
{
	//process();
	if (completeQueryQueue.size() > 0)
	{
		_mutex.lock();
		QueryResult * result = completeQueryQueue.front();
		completeQueryQueue.pop();
		_mutex.unlock();

		DBEvent dbEvent;
		dbEvent.event = DBEvent::QUERY;
		dbEvent.result = result;
		result->record->dispatch(dbEvent);
		
		delete result;
	}

	if (completeInsertQueue.size() > 0)
	{
		_mutex.lock();
		InsertResult * result = completeInsertQueue.front();
		completeInsertQueue.pop();
		_mutex.unlock();

		DBEvent dbEvent;
		dbEvent.event = DBEvent::INSERT;
		dbEvent.result =result;
		result->record->dispatch(dbEvent);
		delete result;
	}

	if (completeUpdateQueue.size() > 0)
	{
		_mutex.lock();
		UpdateResult * result = completeUpdateQueue.front();
		completeUpdateQueue.pop();
		_mutex.unlock();

		DBEvent dbEvent;
		dbEvent.event = DBEvent::UPDATE;
		dbEvent.result = result;
		result->record->dispatch(dbEvent);
		delete result;
	}

	if (completeDeleteQueue.size() > 0)
	{
		_mutex.lock();
		DeleteResult * result = completeDeleteQueue.front();
		completeDeleteQueue.pop();
		_mutex.unlock();

		DBEvent dbEvent;
		dbEvent.event = DBEvent::REMOVE;
		dbEvent.result = result;
		result->record->dispatch(dbEvent);

		delete result;
	}
}

int32 DBConnector::threaderRun(Threader& theader)
{
	((DBConnector*)theader.m_thisObject)->process();
	Threader::sleep(1);
	return 0;
}

uint32 DBConnector::ThreadProcess(Threader* pThread)
{
	while (true)
	{
		process();
		Threader::sleep(1);
	}

	return 0;
}

