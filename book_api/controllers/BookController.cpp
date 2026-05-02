#include "controllers/BookController.h"
  #include <drogon/drogon.h>
 
std::mutex BookController::mutex_;
  
void BookController::getAll(const HttpRequestPtr& req,
                              std::function<void(const HttpResponsePtr&)>&& cb) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto results = service_->getAll();
    if (results.isFail())
    {
        Json::Value err;
        err["error"] = results.error();
        auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k500InternalServerError);
        cb(resp);
        return;
    }
    
    Json::Value result(Json::arrayValue);
    for (const auto& l : results.value())
        result.append(libroToJson(l));

    cb(HttpResponse::newHttpJsonResponse(result));
}

 
void BookController::getById(const HttpRequestPtr& req,
                               std::function<void(const HttpResponsePtr&)>&& cb,
                               int id) {
    std::lock_guard<std::mutex> lock(mutex_);
   
  try
  {
        
        auto book = service_->getById(id);
        if (book.isFail())
        {
            LOG_INFO << "fallo " << id;
                Json::Value err;
                err["error"] = book.error();
                auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k404NotFound);
                cb(resp);
                LOG_ERROR << book.error();
             return;
             
        }
        auto libroJson = libroToJson(book.value());
      
        cb(HttpResponse::newHttpJsonResponse(libroJson));
  }
  catch(const std::exception& e)
  {
    Json::Value err;
        err["error"] = "Libro no encontrado";
        auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k404NotFound);
        cb(resp);
        return;
    LOG_ERROR << e.what() << '\n';
  }
  
  
}

 
void BookController::create(const HttpRequestPtr& req,
                              std::function<void(const HttpResponsePtr&)>&& cb) {
    auto body = req->getJsonObject();
    if (!body) {
        Json::Value err;
        err["error"] = "JSON inválido";
        auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k400BadRequest);
        cb(resp);
        return;
    }

    Book nuevo;
    nuevo.title = (*body)["title"].asString();
    nuevo.autor  = (*body)["autor"].asString();
    nuevo.year   = (*body)["year"].asInt();
    nuevo.pages  = (*body)["pages"].asInt();
    
    {
         auto re = service_->create(nuevo);
            if (re.isFail())
            {
                    Json::Value err;
                    err["error"] = re.error();
                    auto resp = HttpResponse::newHttpJsonResponse(err);
                    resp->setStatusCode(k500InternalServerError);
                    cb(resp);
                return;
    }

    auto resp = HttpResponse::newHttpJsonResponse(libroToJson(nuevo));
    resp->setStatusCode(k201Created);
    cb(resp);
    }
}
void BookController::remove(const HttpRequestPtr& req,
                              std::function<void(const HttpResponsePtr&)>&& cb,
                              int id) {
    std::lock_guard<std::mutex> lock(mutex_);
  
        auto libros = service_->remove(id);
        if (libros.isFail())
        {
                Json::Value err;
                err["error"] = libros.error();
                auto resp = HttpResponse::newHttpJsonResponse(err);
                resp->setStatusCode(k500InternalServerError);
                cb(resp);
            return;
        }
 
 
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k204NoContent);
    cb(resp);
  }

 
 
 