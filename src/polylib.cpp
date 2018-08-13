#include <polylib/polylib.hpp>
#include <polylib/base-types.hpp>
#include <yandex/base-types.hpp>
#include <vk/long-poll.hpp>

using namespace polylib;
using namespace yandex;
using namespace vk;
using namespace nlohmann;
using namespace std;

PolyLib::PolyLib(const MyDrive *drive, const Bot *bot):
  drive_(drive),
  bot_(bot)
{}

PolyLib::~PolyLib()
{
  delete drive_;
  delete bot_;
}

void PolyLib::runBot() const
{
  LongPoll long_poll(bot_);

  while (true)
  {
    handleUpdates(long_poll.getUpdates());
  }
}

void PolyLib::handleUpdates(const json &updates) const
{
  for (json update: updates)
  {
    if (update["type"] == "message_new")
    {
      json obj = update["object"];

      message_t message = {
        obj["id"].get<id_t>(),
        obj["from_id"].get<id_t>(),
        obj["text"].get<string>()
      };

      if (obj.find("payload") != obj.end())
      {
        message.payload = json::parse(obj["payload"].get<string>());
      }

      if (obj.find("attachments") != obj.end())
      {
        message.attachments = obj["attachments"];
      }

      handleMessage(message);
    }
  }
}

void PolyLib::handleMessage(const message_t &message) const
{
  payload_t payload(message.payload);
  transaction_t transaction(message.from_id);

  if (payload.action == "get_file")
  {
    transaction.open(payload.file_id);
  }

  if (transaction.is_pending)
  {
    if (payload.action == "cancel_transaction")
    {
      transaction.close();
    }
    else
    {
      if ((!message.attachments.empty()
        && message.attachments[0]["type"] == "money_transfer"
        && message.attachments[0]["amount"] == transaction.amount)
        || transaction.amount == 0 || message.text == "TEST")
      {
        bot_->sendMessage(
          message.from_id,
          "Благодарим Вас за покупку!\nСсылка на скачивание файла:\n" + drive_->getFileLink(transaction.file_id)
        );
        
        transaction.setCompleted();
      }
      else
      {
        keyboard_t keyboard({
        {
          {"action", "cancel_transaction"}
        }, "Отменить покупку", "negative"});

        bot_->sendMessage(
          message.from_id,
          "Чтобы получить доступ к этому файлу, отправьте денежый перевод на сумму " + to_string(transaction.amount) + " ₽",
          keyboard.json().dump()
        );

        return;
      }
    }
  }

  bot_->sendMessage(
    message.from_id,
    "Вы находитесь здесь: polylib" + (payload.path.empty() ? " (корневая папка)" : payload.path),
    getKeyboardByPath(payload.path, payload.offset).json().dump()
  );
}

keyboard_t PolyLib::getKeyboardByPath(const string &path, const size_t offset) const
{
  keyboard_t keyboard;

  const size_t limit = 5;

  if (!path.empty())
  {
    size_t found = path.find_last_of("/");

    keyboard.addButton({
    {
      {"action", "go_back"},
      {"path", path.substr(0, found)}
    }, "Назад", "positive"});
  }

  if (offset >= limit)
  {
    keyboard.addButton({
    {
      {"action", "go_up"},
      {"path", path},
      {"offset", offset - limit}
    }, "Вверх"});
  }

  for (resource_t resource: drive_->getResources(path, limit, offset))
  {
    if (resource.isFile())
    {
      keyboard.addButton({
      {
        {"action", "get_file"},
        {"file_id", resource.id}
      }, resource.name});
    }
    else
    {
      keyboard.addButton({
      {
        {"action", "get_dir"},
        {"path", path + "/" + resource.name}
      }, resource.name, "primary"});
    }
  }

  if ((offset + limit) < drive_->getResourcesCount(path))
  {
    keyboard.addButton({
    {
      {"action", "go_down"},
      {"path", path},
      {"offset", offset + limit}
    }, "Вниз"});
  }

  return keyboard;
}
