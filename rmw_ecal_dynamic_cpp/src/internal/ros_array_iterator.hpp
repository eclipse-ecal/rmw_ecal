#pragma once

#include <iterator>
#include <type_traits>
#include <tuple>

#include <rmw/types.h>
#include <rmw/names_and_types.h>

namespace eCAL
{
namespace rmw
{
namespace RosArray
{
template <typename ROS_ARRAY_T>
class iterator
{
	using iterator_category = std::random_access_iterator_tag;
	using value_type = typename std::remove_pointer<decltype(ROS_ARRAY_T::data)>::type;
	using difference_type = std::ptrdiff_t;
	using reference = value_type &;
	using pointer = value_type *;

	pointer current_;

public:
	iterator(pointer start) : current_{start} {}
	iterator(const iterator &) = default;
	~iterator() = default;

	iterator &operator=(const iterator &rhs)
	{
		current_ = rhs.current_;
		return *this;
	}

	iterator &operator++()
	{
		current_++;
		return *this;
	}

	iterator operator++(int)
	{
		auto old = *this;
		current_++;
		return old;
	}

	iterator &operator--()
	{
		current_--;
		return *this;
	}

	iterator operator--(int)
	{
		auto old = *this;
		current_--;
		return old;
	}

	bool operator<(const iterator &other)
	{
		return current_ < other.current_;
	}

	bool operator>(const iterator &other)
	{
		return current_ > other.current_;
	}

	bool operator<=(const iterator &other)
	{
		return current_ <= other.current_;
	}

	bool operator>=(const iterator &other)
	{
		return current_ >= other.current_;
	}

	iterator operator+(difference_type offset)
	{
		return iterator{current_ + offset};
	}

	iterator &operator+=(difference_type offset)
	{
		current_ += offset;
		return *this;
	}

	iterator &operator-=(difference_type offset)
	{
		current_ -= offset;
		return *this;
	}

	iterator operator-(difference_type offset)
	{
		return iterator{current_ - offset};
	}

	difference_type operator-(iterator other)
	{
		return reinterpret_cast<difference_type>(other.current_ - current_);
	}

	bool operator==(const iterator &other)
	{
		return current_ == other.current_;
	}

	bool operator!=(const iterator &other)
	{
		return !(*this == other);
	}

	reference operator*() const
	{
		return *current_;
	}

	pointer operator->() const
	{
		return current_;
	}

	reference operator[](difference_type index) const
	{
		return *(current_[index]);
	}
};

template <>
class iterator<rmw_names_and_types_t>
{
	using iterator_category = std::random_access_iterator_tag;
	using value_type = std::tuple<char *&, rcutils_string_array_t &>;
	using difference_type = decltype(std::declval<decltype(rmw_names_and_types_t::names)>().size);
	using reference = value_type &;
	using pointer = value_type *;

	rmw_names_and_types_t &arr_;
	difference_type index_;

public:
	iterator(rmw_names_and_types_t &arr, difference_type index) : arr_{arr}, index_{index} {}
	iterator(const iterator &) = default;
	~iterator() = default;

	iterator &operator=(const iterator &rhs)
	{
		index_ = rhs.index_;
		return *this;
	}

	iterator &operator++()
	{
		index_++;
		return *this;
	}

	iterator operator++(int)
	{
		auto old = *this;
		index_++;
		return old;
	}

	iterator &operator--()
	{
		index_--;
		return *this;
	}

	iterator operator--(int)
	{
		auto old = *this;
		index_--;
		return old;
	}

	bool operator<(const iterator &other)
	{
		return index_ < other.index_;
	}

	bool operator>(const iterator &other)
	{
		return index_ > other.index_;
	}

	bool operator<=(const iterator &other)
	{
		return index_ <= other.index_;
	}

	bool operator>=(const iterator &other)
	{
		return index_ >= other.index_;
	}

	iterator operator+(difference_type offset)
	{
		return iterator{arr_, index_ + offset};
	}

	iterator &operator+=(difference_type offset)
	{
		index_ += offset;
		return *this;
	}

	iterator &operator-=(difference_type offset)
	{
		index_ -= offset;
		return *this;
	}

	iterator operator-(difference_type offset)
	{
		return iterator{arr_, index_ - offset};
	}

	difference_type operator-(iterator other)
	{
		return index_ - other.index_;
	}

	bool operator==(const iterator &other)
	{
		return index_ == other.index_;
	}

	bool operator!=(const iterator &other)
	{
		return !(*this == other);
	}

	value_type operator*() const
	{
		return std::forward_as_tuple(arr_.names.data[index_], arr_.types[index_]);
	}

	value_type operator->() const
	{
		return std::forward_as_tuple(arr_.names.data[index_], arr_.types[index_]);
	}

	value_type operator[](difference_type index) const
	{
		return std::forward_as_tuple(arr_.names.data[index], arr_.types[index]);
	}
};

template <typename ROS_ARRAY_T>
inline iterator<ROS_ARRAY_T> Begin(ROS_ARRAY_T &array)
{
	return iterator<ROS_ARRAY_T>{array.data};
}

template <typename ROS_ARRAY_T>
inline iterator<ROS_ARRAY_T> End(ROS_ARRAY_T &array)
{
	return iterator<ROS_ARRAY_T>{array.data + array.size};
}

template <>
inline iterator<rmw_names_and_types_t> Begin(rmw_names_and_types_t &array)
{
	return iterator<rmw_names_and_types_t>{array, 0};
}

template <>
inline iterator<rmw_names_and_types_t> End(rmw_names_and_types_t &array)
{
	return iterator<rmw_names_and_types_t>{array, array.names.size - 1};
}

} // namespace RosArrayIterator
} // namespace rmw
} // namespace eCAL