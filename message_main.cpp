#include <optional>
#include "message_service/http_message_web_server.h"
//#include "other_server/http_other_server.h"

int main(int argc, char*argv[]) 
{
    MessageHTTPWebServer app;
    return app.run(argc, argv);
}