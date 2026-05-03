#pragma once
#include "IBookService.h"
#include "repositories/IBookRepository.h"
#include <memory>

class BookService : public IBookService {
public:
 
    explicit BookService(std::shared_ptr<IBookRepository> repo)
        : repo_(std::move(repo)) {}

 
        Result<std::vector<Book>> getAll() override;
        Result<Book> getById(int id) override;
        Result<Book> create(const Book& libro) override;
        Result<bool> remove(int id) override;
        Result<bool> update(const Reader& item) override;
 
    Result<std::vector<Book>> getAll() override;
    Result<Book> getById(int id) override;
    Result<Book> create(const Book& libro) override;
    Result<bool> remove(int id) override;
 

private:
    std::shared_ptr<IBookRepository> repo_;
};
