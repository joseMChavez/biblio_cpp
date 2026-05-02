#include <drogon/drogon.h>
#include "repositories/BookRepository.h"
#include "services/BookService.h"
#include "controllers/BookController.h"
int main() {
    //Set HTTP listener address and port
   auto repo       = std::make_shared<BookRepository>();
   auto service    = std::make_shared<BookService>(repo);
   auto controller = std::make_shared<BookController>(service);
    drogon::app().registerController(controller).addListener("0.0.0.0", 5555);
    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
