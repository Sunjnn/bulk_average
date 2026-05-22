#include "bulk_average.hpp"

#include <limits>

namespace {

uint64_t ceil_div(uint64_t numerator, uint64_t denominator) noexcept {
    return numerator / denominator + (numerator % denominator != 0 ? 1 : 0);
}

} // namespace

namespace bulk_average {

BulkAverage &BulkAverage::operator+=(int64_t value) noexcept {
    ++_count;

    // Add (value - oldMean) to remainder, split to avoid overflow
    const int64_t oldMean = _mean;

    add_remainder(value);

    if (oldMean > 0) {
        sub_remainder(static_cast<uint64_t>(oldMean));
    } else if (oldMean < 0) {
        if (oldMean != std::numeric_limits<int64_t>::min()) {
            add_remainder(-oldMean);
        } else {
            add_remainder(-(oldMean + 1));
            add_remainder(1);
        }
    }

    return *this;
}

BulkAverage BulkAverage::operator+(int64_t value) const noexcept {
    BulkAverage result = *this;
    result += value;
    return result;
}

void BulkAverage::reset() noexcept {
    _mean = 0;
    _remainder = 0;
    _count = 0;
}

void BulkAverage::add_remainder(int64_t value) noexcept {
    if (value == 0) {
        return;
    }

    if (value > 0) {
        auto val = static_cast<uint64_t>(value);

        _mean += static_cast<int64_t>(val / _count);
        val %= _count;

        const uint64_t space = _count - _remainder;
        if (val >= space) {
            _mean += 1;
            _remainder = val - space;
        } else {
            _remainder += val;
        }

        return;
    }

    // value < 0: negate and delegate to sub_remainder
    // Special case: INT64_MIN cannot be negated directly
    if (value != std::numeric_limits<int64_t>::min()) {
        sub_remainder(static_cast<uint64_t>(-value));
    } else {
        sub_remainder(static_cast<uint64_t>(-(value + 1)));
        sub_remainder(1);
    }
}

void BulkAverage::sub_remainder(uint64_t value) noexcept {
    // Extract full _count units from value
    _mean -= static_cast<int64_t>(value / _count);
    value %= _count;

    if (value <= _remainder) {
        _remainder -= value;
    } else {
        // Borrow one more from _mean
        _mean -= 1;
        _remainder = _count - value + _remainder;
    }
}

} // namespace bulk_average
