#include "secondary_index.h"
#include <stdexcept>


void Database::inserter(const Record& record)
{
	storage_.push_front(record);
	id_to_record_[record.id] = storage_.begin();
	const auto timestamp_iter = timestamp_map_.insert({ record.timestamp, storage_.begin() });
	const auto karma_iter = karma_map_.insert({ record.karma, storage_.begin() });
	const auto user_iter = user_map_.insert({ record.user, storage_.begin() });

	id_to_iterators_.insert({ record.id, 
		Iterators{ timestamp_iter, karma_iter, user_iter } });
}


void Database::deleter(const std::unordered_map<std::string, ListIter>::iterator& iter)
{
	const Iterators iterators = id_to_iterators_.at(iter->first);
	timestamp_map_.erase(iterators.timestamp_iterator);
	karma_map_.erase(iterators.karma_iterator);
	user_map_.erase(iterators.user_iterator);

	const ListIter list_iter = iter->second;
	id_to_record_.erase(iter);
	storage_.erase(list_iter);
}


bool Database::Put(const Record& record)
{
	const auto finder = id_to_record_.find(record.id);

    if (finder == id_to_record_.end())
    {
		inserter(record);

        return true;
    }

    return false;
}


const Record* Database::GetById(const std::string& id) const
{
    try
    {
		return &(*id_to_record_.at(id));
    }
    catch (std::out_of_range&)
    {
        return nullptr;
    }
}


bool Database::Erase(const std::string& id)
{
	const auto finder = id_to_record_.find(id);

    if (finder == id_to_record_.end())
    {
        return false;
    }

	deleter(finder);
    return true;
}