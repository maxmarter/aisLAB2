#include "message.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

#include "user.h"


using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Message::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Message` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`from_user_id` INT NOT NULL,"
                        << "`to_user_id`INT NOT NULL,"
                        << "`message` VARCHAR(256) NOT NULL,"
                        << "FOREIGN KEY (`to_user_id`)  REFERENCES `User` (`id`),"
                        << "PRIMARY KEY (`id`));",
                now;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Message::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("from_user_id", _from_user_id);
        root->set("to_user_id", _to_user_id);
        root->set("message", _message);

        return root;
    }

    Message Message::fromJSON(const std::string &str)
    {
        Message Message;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        Message.id() = object->getValue<long>("id");
        Message.from_user_id() = object->getValue<long>("from_user_id");
        Message.to_user_id() = object->getValue<long>("to_user_id");
        Message.message() = object->getValue<std::string>("message");

        return Message;
    }

    std::optional<Message> Message::read_by_id(long user_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Message a;
            select << "SELECT id, fromUser_id, to_user_id, message FROM Message where from_user_id=?",
                into(a._id),
                into(a._from_user_id),
                into(a._to_user_id),
                into(a._message),
                
                use(user_id),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return a;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
        }
        return {};
    }
    // std::optional<Message> Message::read_by_id(long id)
    // {
    //     try
    //     {
    //         Poco::Data::Session session = database::Database::get().create_session();
    //         Poco::Data::Statement select(session);
    //         Message a;
    //         select << "SELECT id, first_name, last_name, email, title,login,password FROM Message where id=?",
    //             into(a._id),
    //             into(a._first_name),
    //             into(a._last_name),
    //             into(a._email),
    //             into(a._title),
    //             into(a._login),
    //             into(a._password),
    //             use(id),
    //             range(0, 1); //  iterate over result set one row at a time

    //         select.execute();
    //         Poco::Data::RecordSet rs(select);
    //         if (rs.moveFirst()) return a;
    //     }

    //     catch (Poco::Data::MySQL::ConnectionException &e)
    //     {
    //         std::cout << "connection:" << e.what() << std::endl;
    //     }
    //     catch (Poco::Data::MySQL::StatementException &e)
    //     {

    //         std::cout << "statement:" << e.what() << std::endl;
            
    //     }
    //     return {};
    // }

    std::vector<Message> Message::read_all_users_messages(long user_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Message> result;
            Message a;
            select << "SELECT id, from_user_id, to_user_id, message FROM Message WHERE to_user_id LIKE ?",
                into(a._id),
                into(a._from_user_id),
                into(a._to_user_id),
                into(a._message),
                use(user_id),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    void Message::send_message(std::string post, long to_user)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            
            insert << "INSERT INTO Message (id, from_user_id, to_user_id) VALUES(?, ?, ?, ?)",
                use(_id),
                use(_from_user_id),
                use(to_user),
                use(post),


            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    void Message::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Message (id, from_user_id, to_user_id) VALUES(?, ?, ?, ?)",
                use(_id),
                use(_from_user_id),
                use(_to_user_id),
                use(_message),

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    long Message::get_id() const
    {
        return _id;
    }

    long Message::get_from_user_id() const
    {
        return _from_user_id;
    }

    long Message::get_to_user_id() const
    {
        return _to_user_id;
    }

    const std::string &Message::get_message() const
    {
        return _message;
    }

    
    long &Message::id()
    {
        return _id;
    }

    long &Message::from_user_id()
    {
        return _from_user_id;
    }

    long &Message::to_user_id()
    {
        return _to_user_id;
    }

    std::string &Message::message()
    {
        return _message;
    }

}