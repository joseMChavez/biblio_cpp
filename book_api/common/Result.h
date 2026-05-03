#pragma once
#include <string>
#include <optional>

template<typename T>
class Result {
public:
    
    static Result<T> ok(T value) {
        Result<T> r;
        r.value_   = std::move(value);
        r.success_ = true;
        return r;
    }

    static Result<T> fail(std::string error) {
        Result<T> r;
        r.error_   = std::move(error);
        r.success_ = false;
        return r;
    }

    bool isOk()    const { return success_; }
    bool isFail()  const { return !success_; }

    const T&           value() const { return value_.value(); }
    const std::string& error() const { return error_; }

private:
    std::optional<T> value_;
    std::string      error_;
    bool             success_ = false;
};

// Especialización para void — operaciones sin retorno (remove, update)
template<>
class Result<void> {
public:
    static Result<void> ok() {
        Result<void> r;
        r.success_ = true;
        return r;
    }

    static Result<void> fail(std::string error) {
        Result<void> r;
        r.error_   = std::move(error);
        r.success_ = false;
        return r;
    }

    bool isOk()   const { return success_; }
    bool isFail() const { return !success_; }

    const std::string& error() const { return error_; }

private:
    std::string error_;
    bool        success_ = false;
};