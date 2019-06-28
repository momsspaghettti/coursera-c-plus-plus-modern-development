#include "Common.h"
#include <utility>
#include <unordered_map>
#include <map>
#include <mutex>


class LruCache : public ICache
{
public:
	LruCache(std::shared_ptr<IBooksUnpacker> books_unpacker,
		const Settings& settings) : books_unpacker_(std::move(books_unpacker)), settings_(settings)
	{
		free_memory_ = settings_.max_memory;
		counter_ = -2'147'483'000;
	}

	BookPtr GetBook(const std::string& book_name) override
	{
		mutex_.lock();
		unordered_node node = name_to_book_.extract(book_name);
		BookPtr result;

        if (!node.empty())
        {
			result = get_existing_book(node);
        }
        else
        {
			result = get_new_book(book_name);
        }

		return [&]
		{
			mutex_.unlock();
			return result;
		}();
	}

private:
	std::shared_ptr<IBooksUnpacker> books_unpacker_;
	const Settings settings_;
	size_t free_memory_;
	int counter_;
	std::mutex mutex_ = std::mutex();

	std::map<int, std::string> priority_to_book_name_;
    std::unordered_map<std::string, std::pair<int, BookPtr>> name_to_book_;

	using unordered_node = std::unordered_map<std::string, std::pair<int, BookPtr>>::node_type;

    BookPtr get_existing_book(unordered_node& node)
    {
		BookPtr result = node.mapped().second;

		auto map_node = priority_to_book_name_.extract(node.mapped().first);
		map_node.key() = ++counter_;
		priority_to_book_name_.insert(std::move(map_node));

		node.mapped().first = counter_;
		name_to_book_.insert(std::move(node));

		return result;
    }

    BookPtr get_new_book(const std::string& book_name)
    {
		BookPtr new_book = books_unpacker_->UnpackBook(book_name);

		const size_t wanted_memory = new_book->GetContent().size();

        if (free_memory_ >= wanted_memory)
        {
			insert_new_book(new_book, wanted_memory);
        }
        else
        {
            if (free_cache_memory(wanted_memory))
            {
				insert_new_book(new_book, wanted_memory);
            }
        }

		return new_book;
    }

    void insert_new_book(const BookPtr& new_book, size_t wanted_memory)
    {
		free_memory_ -= wanted_memory;
		priority_to_book_name_.insert({ ++counter_, new_book->GetName() });
		name_to_book_.insert({ new_book->GetName(), std::make_pair(counter_, new_book) });
    }

    bool free_cache_memory(size_t wanted_memory)
    {
        while (!priority_to_book_name_.empty())
        {
			const auto begin_ = priority_to_book_name_.begin();

			const auto finder =  name_to_book_.find(begin_->second);
			free_memory_ += finder->second.second->GetContent().size();

			name_to_book_.erase(finder);
			priority_to_book_name_.erase(begin_);

            if (free_memory_ >= wanted_memory)
                break;
        }

		return free_memory_ >= wanted_memory;
    }
};


std::unique_ptr<ICache> MakeCache(std::shared_ptr<IBooksUnpacker> books_unpacker,
	const ICache::Settings& settings)
{
	return std::make_unique<LruCache>(std::move(books_unpacker), settings);
}