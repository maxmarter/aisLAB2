#include "user_service/http_user_web_server.h"


int main(int argc, char*argv[]) 
{
    HTTPUSerWebServer app;
    return app.run(argc, argv);
}