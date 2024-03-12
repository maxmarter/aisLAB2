#include <optional>
#include "wall_service/http_wall_web_server.h"
//#include "other_server/http_other_server.h"

int main(int argc, char*argv[]) 
{
    WallHTTPWebServer app;
    return app.run(argc, argv);
}