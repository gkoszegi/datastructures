#pragma once

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <vector>

template<typename T>
class union_find
{
    public:

        static_assert(std::is_integral<T>::value, "union_find<T>: T must be integral");

        using value_type = T;
        using size_type = std::make_unsigned_t<T>;

        union_find(value_type n)
            : mSets(n)
            , mSize(n)
        {
            std::iota(mSets.begin(), mSets.end(), 0);
            std::fill(mSize.begin(), mSize.end(), 1);
        }

        value_type max_value() const
        {
            return static_cast<value_type>(mSets.size() - 1);
        }

        size_type size() const
        {
            return mSets.size();
        }

        bool join(value_type v1, value_type v2)
        {
            auto r1 = find(v1);
            auto r2 = find(v2);

            if (r1 == r2)
                return false;

            if (mSize[r1] < mSize[r2])
                std::swap(r1, r2);

            merge_into_left(r1, r2);

            return true;
        }

        value_type find(value_type value) const
        {
            if (static_cast<size_type>(value) >= size())
                throw std::out_of_range("union_find::find(): value out of range");

            value_type root = value;
            while (root != mSets[root])
                root = mSets[root];

            compress_path(value, root);
            return root;
        }

        size_type count_disjoint() const
        {
            size_type roots = 0;
            for (value_type value = 0; value <= max_value(); ++value)
            {
                if (mSets[value] == value)
                    ++roots;
            }
            return roots;
        }

    protected:

        void merge_into_left(value_type r1, value_type r2)
        {
            mSets[r2] = r1;
            mSize[r1] += mSize[r2];
        }

        void compress_path(value_type val, value_type root) const
        {
            while (val != root)
            {
                value_type parent = mSets[val];
                mSets[val] = root;
                mSize[parent] -= mSize[val];
                mSize[root] += mSize[val];
                val = parent;
            }
        }

        mutable std::vector<value_type> mSets;
        mutable std::vector<size_type> mSize;
};
