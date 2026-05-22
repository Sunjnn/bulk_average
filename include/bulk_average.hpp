#ifndef BULK_AVERAGE_HPP
#define BULK_AVERAGE_HPP

#include <cstdint>
#include <ostream>

namespace bulk_average {

class BulkAverage {
public:
    BulkAverage &operator+=(int64_t value) noexcept;
    BulkAverage operator+(int64_t value) const noexcept;
    void reset() noexcept;

    int64_t mean() const noexcept { return _mean; }
    uint64_t remainder() const noexcept { return _remainder; }
    uint64_t count() const noexcept { return _count; }

    double to_double() const noexcept {
        if (_count == 0) {
            return 0.0;
        }
        return static_cast<double>(_mean) + (static_cast<double>(_remainder) / static_cast<double>(_count));
    }

private:
    void add_remainder(int64_t value) noexcept;
    void sub_remainder(uint64_t value) noexcept;

    // Invariant: true_sum == _mean * _count + _remainder, where 0 <= _remainder < _count
    int64_t _mean = 0;
    uint64_t _remainder = 0;
    uint64_t _count = 0;
};

inline std::ostream &operator<<(std::ostream &os, const BulkAverage &avg) {
    os << "BulkAverage{mean=" << avg.mean() << ", remainder=" << avg.remainder() << ", count=" << avg.count()
       << ", value=" << avg.to_double() << "}";
    return os;
}

} // namespace bulk_average

#endif // BULK_AVERAGE_HPP
