#ifndef DB_HPP_INCLUDED
#define DB_HPP_INCLUDED

#include <pqxx/pqxx>

namespace polylib
{
  class DB
  {
  private:
    DB() = default;
    ~DB() = default;
    
    DB(const DB &) = delete;
    DB& operator=(const DB &) = delete;
    DB(DB &&) = delete;
    DB& operator=(DB &&) = delete;

  public:
    static pqxx::nontransaction * get()
    {
      static pqxx::connection db_connection("dbname=polylib");
      static pqxx::nontransaction db(db_connection);

      return &db;
    }
  };
}

#endif
