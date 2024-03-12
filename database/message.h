#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Message{
        private:
            long _id;
            long _from_user_id;
            long _to_user_id;
            std::string _message;

            
            

        public:

            static Message fromJSON(const std::string & str);

            long             get_id() const;
            long get_from_user_id() const;
            long get_to_user_id() const;
            const std::string &get_message() const;

            long&        id();
            long& from_user_id();
            long& to_user_id();
            std::string &message();

            static void init();
            static std::optional<Message> read_by_id(long to_user_id);
            static std::vector<Message> read_all_users_messages(long to_user_id);
            void send_message(std::string post, long to_user_id);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif