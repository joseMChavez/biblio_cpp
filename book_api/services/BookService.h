#pragma once
#include "IBookService.h"
#include "repositories/IBookRepository.h"
#include <memory>

class BookService : public IBookService {
public:
 
    explicit BookService(std::shared_ptr<IBookRepository> repo)
        : repo_(std::move(repo)) {}

<<<<<<< HEAD
        Result<std::vector<Book>> getAll() override;
        Result<Book> getById(int id) override;
        Result<Book> create(const Book& libro) override;
        Result<bool> remove(int id) override;
        Result<bool> update(const Reader& item) override;
=======
    Result<std::vector<Book>> getAll() override;
    Result<Book> getById(int id) override;
    Result<Book> create(const Book& libro) override;
    Result<bool> remove(int id) override;
>>>>>>> de6f692cc521b603432362c4212f7a0d55aa9687

private:
    std::shared_ptr<IBookRepository> repo_;
};