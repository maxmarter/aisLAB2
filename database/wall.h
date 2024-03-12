#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Wall{
        private:
            long _id;
            long _user_id;
            std::string _post;
            long _post_id;
            
            

        public:

            static Wall fromJSON(const std::string & str);

            long             get_id() const;
            long get_user_id() const;
            const std::string &get_post() const;
            long get_post_id() const;

            long&        id();
            long& user_id();
            std::string &post();
            long& post_id();

            static void init();
            
            static std::optional<Wall> read_by_id(long id);
            // static std::optional<long> get_wall(long& user_id);
            static std::vector<Wall> show(long id);
            void add_new_post(long user_id, std::string post_content);
            // void add_post(std::string post);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif