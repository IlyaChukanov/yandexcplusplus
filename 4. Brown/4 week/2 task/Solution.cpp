#include "Common.h"
#include <list>
#include <unordered_map>
#include <mutex>

using namespace std;

class LruCache : public ICache {
public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  ) {
    settings_ = {settings, settings.max_memory};
    books_unpacker_ = std::move(books_unpacker);
  }

  BookPtr GetBook(const string& book_name) override {
    // Если книга уже есть в кэше
    {
      std::lock_guard<std::mutex> a(mutex_);
      if (cache_elements_.count(book_name)) {
        auto point_to_cache = cache_elements_.at(book_name);
        cache_.splice(cache_.begin(), cache_, point_to_cache);
        return cache_.front().book;
      }
    }
    std::unique_ptr<IBook> book;
    size_t book_size;
    {
      std::lock_guard<std::mutex> a(mutex_);
      book = books_unpacker_->UnpackBook(book_name);
      book_size = book->GetContent().size();
    }
    // Если книга больше размера кэша

    if (book_size > settings_.default_settings_.max_memory) {
      return std::move(book);
    }
    // Если размер книги больше свободного места
    if (book_size > settings_.free_memory_) {
      ClearCache(book_size);
    }
    std::shared_ptr<IBook> result;
    {
      std::lock_guard<std::mutex> a(mutex_);
      cache_.push_front({book_name, std::move(book)});
      cache_elements_[book_name] = cache_.begin();
      settings_.free_memory_ -= book_size;
      result = cache_.front().book;
    }
    return result;
  }
 private:

  shared_ptr<IBooksUnpacker> books_unpacker_;

  struct SettingsState {
    Settings default_settings_;
    size_t free_memory_;
  } settings_;
  struct CacheItem {
    std::string name;
    std::shared_ptr<IBook> book;
  };

  std::unordered_map<std::string, std::list<CacheItem>::iterator> cache_elements_;
  std::list<CacheItem> cache_;
  std::mutex mutex_;

  void ClearCache(size_t need_free_space) {
    while (settings_.free_memory_ < need_free_space) {
      std::lock_guard<std::mutex> a(mutex_);
      auto deleted = cache_.back();
      cache_.pop_back();
      settings_.free_memory_ += deleted.book->GetContent().size();
      cache_elements_.erase(deleted.name);
    }
  }
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  return make_unique<LruCache>(books_unpacker, settings);
}
