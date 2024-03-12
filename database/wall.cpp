#include "wall.h"
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

    void Wall::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Wall` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`user_id` INT NOT NULL,"
                        << "`post` VARCHAR(256) NOT NULL,"
                        << "`post_id` INT NOT NULL,"
                        << "PRIMARY KEY (`id`),"
                        << "FOREIGN KEY (`user_id`)  REFERENCES `User` (`id`));",
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

    Poco::JSON::Object::Ptr Wall::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("user_id", _user_id);
        root->set("post", _post);
        root->set("post_id", _post_id);

        return root;
    }

    Wall Wall::fromJSON(const std::string &str)
    {
        Wall Wall;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        Wall.id() = object->getValue<long>("id");
        Wall.user_id() = object->getValue<long>("user_id");
        Wall.post() = object->getValue<std::string>("last_name");
        Wall.post_id() = object->getValue<long>("email");

        return Wall;
    }

    std::optional<Wall> Wall::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Wall a;
            select << "SELECT id, user_id, post, post_id FROM Wall where user_id=?",
                into(a._id),
                into(a._user_id),
                into(a._post),
                into(a._post_id),


                use(id),
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
    // std::optional<Wall> Wall::read_by_id(long id)
    // {
    //     try
    //     {
    //         Poco::Data::Session session = database::Database::get().create_session();
    //         Poco::Data::Statement select(session);
    //         Wall a;
    //         select << "SELECT id, first_name, last_name, email, title,login,password FROM Wall where id=?",
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

    std::vector<Wall> Wall::show(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Wall> result;
            Wall a;
            select << "SELECT id, user_id, post, post_id FROM Wall WHERE user_id LIKE &",
                into(a._id),
                into(a._user_id),
                into(a._post),
                into(a._post_id),
                use(id),
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

    void Wall::add_new_post(long user_id, std::string post_content)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            
            insert << "INSERT INTO Wall (user_id, post) VALUES(?, ?)",
                use(user_id),
                use(post_content);

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

    // void Wall::add_post(std::string post)
    // {
    //     try
    //     {
    //         Poco::Data::Session session = database::Database::get().create_session();
    //         Poco::Data::Statement insert(session);
            
    //         insert << "INSERT INTO Wall (id, post, post_id) VALUES(?, ?, ?)",
    //             use(_id),
    //             use(_post),
    //             use(_post_id);

    //         insert.execute();

    //         Poco::Data::Statement select(session);
    //         select << "SELECT LAST_INSERT_ID()",
    //             into(_id),
    //             range(0, 1); //  iterate over result set one row at a time

    //         if (!select.done())
    //         {
    //             select.execute();
    //         }
    //         std::cout << "inserted:" << _id << std::endl;
    //     }
    //     catch (Poco::Data::MySQL::ConnectionException &e)
    //     {
    //         std::cout << "connection:" << e.what() << std::endl;
    //         throw;
    //     }
    //     catch (Poco::Data::MySQL::StatementException &e)
    //     {

    //         std::cout << "statement:" << e.what() << std::endl;
    //         throw;
    //     }
    // }

    void Wall::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Wall (id, user_id, post, post_id) VALUES(?, ?, ?, ?)",
                use(_id),
                use(_user_id),
                use(_post),
                use(_post_id);

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

    long Wall::get_id() const
    {
        return _id;
    }

    long Wall::get_user_id() const
    {
        return _user_id;
    }

    const std::string &Wall::get_post() const
    {
        return _post;
    }

    long Wall::get_post_id() const
    {
        return _post_id;
    }

    
    long &Wall::id()
    {
        return _id;
    }

    long &Wall::user_id()
    {
        return _user_id;
    }

    std::string &Wall::post()
    {
        return _post;
    }

    long &Wall::post_id()
    {
        return _post_id;
    }

}