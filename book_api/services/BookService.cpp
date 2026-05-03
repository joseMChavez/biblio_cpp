#include "services/BookService.h"

Result<std::vector<Book>> BookService::getAll() {
    return repo_->getAll();
}

Result<Book> BookService::getById(int id) {
    return repo_->getById(id);
}

Result<Book> BookService::create(const Book& book) {
    return repo_->create(book);
}

Result<bool> BookService::remove(int id) {
    return repo_->remove(id);
 
}
Result<bool> BookService::update(const Reader& item) {
    return repo_->update(item);
 
}
