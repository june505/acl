#include "acl_stdafx.hpp"
#include "acl_cpp/connpool/connect_client.hpp"
#include "acl_cpp/db/db_handle.hpp"
#include "acl_cpp/db/db_mysql.hpp"
#include "acl_cpp/db/mysql_pool.hpp"

namespace acl
{

mysql_pool::mysql_pool(const char* dbaddr, const char* dbname,
	const char* dbuser, const char* dbpass, size_t dblimit /* = 64 */,
	unsigned long dbflags /* = 0 */, bool auto_commit /* = true */,
	int conn_timeout /* = 60 */, int rw_timeout /* = 60 */)
: db_pool(dbaddr, dblimit)
{
	acl_assert(dbaddr && *dbaddr);
	acl_assert(dbname && *dbname);

	// ��ַ��ʽ��[dbname@]dbaddr
	const char* ptr = strchr(dbaddr, '@');
	if (ptr != NULL)
		ptr++;
	else
		ptr = dbaddr;
	acl_assert(*ptr);

	dbaddr_ = acl_mystrdup(ptr);
	dbname_ = acl_mystrdup(dbname);

	if (dbuser)
		dbuser_ = acl_mystrdup(dbuser);
	else
		dbuser_ = NULL;

	if (dbpass)
		dbpass_ = acl_mystrdup(dbpass);
	else
		dbpass_ = NULL;

	dbflags_ = dbflags;
	auto_commit_ = auto_commit;
	conn_timeout_ = conn_timeout;
	rw_timeout_ = rw_timeout;
}

mysql_pool::~mysql_pool()
{
	if (dbaddr_)
		acl_myfree(dbaddr_);
	if (dbname_)
		acl_myfree(dbname_);
	if (dbuser_)
		acl_myfree(dbuser_);
	if (dbpass_)
		acl_myfree(dbpass_);
}

connect_client* mysql_pool::create_connect()
{
	return NEW db_mysql(dbaddr_, dbname_, dbuser_,
		dbpass_, dbflags_, auto_commit_,
		conn_timeout_, rw_timeout_);
}

} // namespace acl
