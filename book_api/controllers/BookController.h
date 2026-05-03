#pragma once
#include <drogon/HttpController.h>
#include "services/IBookService.h"
#include <memory>

using namespace drogon;

class BookController : public HttpController<BookController> {
public:
  static const bool isAutoCreation = false;
    explicit BookController(std::shared_ptr<IBookService> service)
        : service_(std::move(service)) {}

    METHOD_LIST_BEGIN
        ADD_METHOD_TO(BookController::getAll,   "/api/books",      Get);
        ADD_METHOD_TO(BookController::getById,  "/api/books/{id}", Get);
        ADD_METHOD_TO(BookController::create,   "/api/books",      Post);
<<<<<<< HEAD
        ADD_METHOD_TO(BookController::update,   "/api/books",   Put);
=======
>>>>>>> de6f692cc521b603432362c4212f7a0d55aa9687
        ADD_METHOD_TO(BookController::remove,   "/api/books/{id}", Delete);
    METHOD_LIST_END

    void getAll  (const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);
    void getById (const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb, int id);
    void create  (const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);
<<<<<<< HEAD
    void update  (const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb);
=======
>>>>>>> de6f692cc521b603432362c4212f7a0d55aa9687
    void remove  (const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb, int id);

private:
    std::shared_ptr<IBookService> service_;
  
    static std::mutex mutex_;
<<<<<<< HEAD
    
=======
>>>>>>> de6f692cc521b603432362c4212f7a0d55aa9687
      Json::Value libroToJson(const Book& libro) {
          Json::Value json;
          json["id"] = libro.id;
          json["title"] = libro.title;
          json["autor"] = libro.autor;
          json["year"] = libro.year;
          json["pages"] = libro.pages;
          json["read"] = libro.read;
          json["date_read"] = libro.date_read;
          return json;
      }

};