#ifndef POLYLIB_BASE_TYPES_HPP
#define POLYLIB_BASE_TYPES_HPP

#include "db.hpp"

#include <string>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>

namespace polylib
{
  struct transaction_t
  {
    id_t id;
    id_t user_id;
    std::string file_id;
    size_t amount;

    bool is_pending;

    transaction_t(const id_t &user_id):
      user_id(user_id)
    {
      get();
    }

    void get()
    {
      pqxx::result res = DB::get()->exec("SELECT id, file_id, amount FROM transactions WHERE user_id=" + std::to_string(user_id) + " AND status='pending'");

      if (res.size() == 1)
      {
        id = res[0]["id"].as<id_t>();
        file_id = res[0]["file_id"].as<std::string>();
        amount = res[0]["amount"].as<size_t>();
        is_pending = true;
      }
    }

    void open(const std::string &file_id)
    {
      pqxx::result res = DB::get()->exec("SELECT price FROM price_list WHERE file_id='" + file_id + "'");
      size_t price = res[0]["price"].as<size_t>();
      
      DB::get()->exec("INSERT INTO transactions(user_id, file_id, amount) "
        "VALUES ("
        + DB::get()->quote(user_id) + ","
        + DB::get()->quote(file_id) + ","
        + DB::get()->quote(price) + ")");

      get();
    }

    void setCompleted()
    {
      if (id != 0)
      {
        DB::get()->exec("UPDATE transactions SET status='completed' WHERE id=" + std::to_string(id));
        is_pending = false;
      }
    }

    void close()
    {
      if (id != 0)
      {
        DB::get()->exec("DELETE FROM transactions WHERE id=" + std::to_string(id));
        is_pending = false;
      }
    }
  };

  struct payload_t
  {
    std::string action;
    std::string file_id;
    std::string path;
    size_t offset;

    payload_t(const nlohmann::json &input)
    {
      if (!input.empty())
      {
        action = input["action"].get<std::string>();

        if (action == "get_file")
        {
          file_id = input["file_id"].get<std::string>();
        }
        else if (action == "go_back"
          || action == "go_up"
          || action == "get_dir"
          || action == "go_down")
        {
          path = input["path"].get<std::string>();
          
          if (input.find("offset") != input.end())
          {
            offset = input["offset"].get<size_t>();
          }
        }
      }
    }
  };
}

#endif
