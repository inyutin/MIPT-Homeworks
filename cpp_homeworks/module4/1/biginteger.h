#include <vector>
#include <string>
#include <iostream>

class BigInteger {
public:
    BigInteger() : data(0), sign(0) {}
    ~BigInteger() = default;

    BigInteger(long long value);

    std::string toString() const;

    bool operator==(const BigInteger &) const;
    bool operator!=(const BigInteger &) const;
    bool operator<(const BigInteger &) const;
    bool operator>(const BigInteger &) const;
    bool operator<=(const BigInteger &) const;
    bool operator>=(const BigInteger &) const;

    BigInteger &operator+=(const BigInteger &);
    BigInteger &operator-=(const BigInteger &);
    BigInteger &operator*=(const BigInteger &);
    BigInteger &operator/=(const BigInteger &);
    BigInteger &operator%=(const BigInteger &);

    const BigInteger operator-() const;
    BigInteger &operator--();
    BigInteger &operator++();
    const BigInteger operator--(int);
    const BigInteger operator++(int);

    explicit operator bool() const;

    friend std::istream &operator>>(std::istream &is, BigInteger &integer);
    friend std::ostream &operator<<(std::ostream &os, const BigInteger &integer);

private:
    static const long long radix = 1000000;
    static const int radixPow = 6;
    std::vector<long long> data;
    signed char sign;

    BigInteger(std::vector<long long> &data, signed char signum) : data(data), sign(signum) {}

    inline long long mod(long long a, long long radix) const {
        return a >= 0 ? a % radix : (radix - (-a) % radix) % radix;
    }

    const BigInteger multiplyByInt(long long) const;

    std::pair<BigInteger, BigInteger> splitDigits(unsigned int) const;

    BigInteger shiftLeft(unsigned int) const;

    BigInteger shiftRight(unsigned int) const;

    std::pair<BigInteger, BigInteger> divMod(const BigInteger &) const;

    long long toLongLong() const;
};

BigInteger::BigInteger(long long value) : data(0) {
    sign = (value >= 0 ? 1 : -1);
    value = sign * value;
    while (value) {
        data.push_back(value % radix);
        value /= radix;
    }
    if (!data.size()) {
        data.resize(1);
    }
}

std::string BigInteger::toString() const {
    std::string s = (sign == -1 && *this != BigInteger(0) ? "-" : "");
    for (auto i = data.rbegin(); i != data.rend(); ++i) {
        if (i == data.rbegin()) {
            s += std::to_string(*i);
        } else
            for (long long j = radix / 10; j >= 1; j /= 10)
                s += std::to_string(*i / j % 10);
    }
    return s;
}

bool BigInteger::operator==(const BigInteger &other) const {
    return data == other.data && (sign == other.sign || data == std::vector<long long>(1, 0));
}
bool BigInteger::operator!=(const BigInteger &other) const {
    return !(other == *this);
}
bool BigInteger::operator<(const BigInteger &other) const {
    if (sign != other.sign) {
        return sign < other.sign;
    }
    if (data.size() != other.data.size())
        return data.size() * sign < other.data.size() * sign;
    for (auto i = data.rbegin(), j = other.data.rbegin(); i != data.rend(); ++i, ++j) {
        if (*i != *j)
            return *i * sign < *j * sign;
    }
    return false;
}
bool BigInteger::operator>(const BigInteger &other) const {
    return other < *this;
}
bool BigInteger::operator<=(const BigInteger &other) const {
    return !(other < *this);
}
bool BigInteger::operator>=(const BigInteger &other) const {
    return !(*this < other);
}

const BigInteger operator+(BigInteger left, const BigInteger &right) {
    return left += right;
}
const BigInteger operator-(BigInteger left, const BigInteger &right) {
    return left -= right;
}
const BigInteger operator*(BigInteger left, const BigInteger &right) {
    return left *= right;
}
const BigInteger operator/(BigInteger left, const BigInteger &right) {
    return left /= right;
}
const BigInteger operator%(BigInteger left, const BigInteger &right) {
    return left %= right;
}

BigInteger::operator bool() const {
    return data.size() > 1 || (data.size() == 1 && data[0] != 0);
}
long long BigInteger::toLongLong() const {
    long long ret = 0;
    for (auto i = data.rbegin(); i != data.rend(); ++i) {
        ret *= radix;
        ret += *i;
    }
    return ret * sign;
}
const BigInteger BigInteger::operator-() const {
    std::vector<long long> newData = data;
    return BigInteger(newData, -sign);
}


std::pair<BigInteger, BigInteger> BigInteger::splitDigits(unsigned int len) const {
    std::vector<long long> a1(1);
    std::vector<long long> a2(1);
    for (size_t i = 0; i < len && i < data.size(); ++i) {
        if (data[i]) {
            a1.resize(i + 1);
            a1[i] = data[i];
        }
    }
    for (int i = 0; i + len < data.size(); ++i)
        if (data[i + len]) {
            a2.resize(i + 1);
            a2[i] = data[i + len];
        }
    return std::make_pair(BigInteger(a1, 1), BigInteger(a2, 1));
}
BigInteger BigInteger::shiftLeft(unsigned int move) const {
    if (!*this)
        return 0;
    std::vector<long long> newData(data.size() + move, 0);
    for (size_t i = 0; i < data.size(); ++i)
        newData[i + move] = data[i];
    return BigInteger(newData, 1);
}
BigInteger BigInteger::shiftRight(unsigned int move) const {
    if (!*this)
        return 0;
    std::vector<long long> newData(data.size() - move, 0);
    for (size_t i = 0; i + move < data.size(); ++i)
        newData[i] = data[i + move];
    return BigInteger(newData, 1);
}
std::pair<BigInteger, BigInteger> BigInteger::divMod(const BigInteger &other) const {
    BigInteger mod = *this;
    mod.sign = 1;
    BigInteger div(0);
    while (mod >= other * other.sign) {
        unsigned int move = static_cast<unsigned int>(mod.data.size() - other.data.size() ?
                                                      mod.data.size() - other.data.size() - 1 : 0);
        BigInteger tmp = other.shiftLeft(move);
        BigInteger tmp1 = tmp.shiftLeft(1);
        if (tmp1 <= mod)
            tmp = tmp1;
        unsigned int shift = static_cast<unsigned int>(std::max((int) tmp.data.size() - 2, 0));
        long long a = mod.shiftRight(shift).toLongLong();
        long long b = tmp.shiftRight(shift).toLongLong();
        long long c = a / b;
        while (tmp.multiplyByInt(c) > mod) {
            c--;
        }
        mod -= tmp.multiplyByInt(c);
        div = div + BigInteger(c).shiftLeft(tmp.data.size() - other.data.size());
    }
    mod.sign = div.sign = sign * other.sign;
    return std::make_pair(div, mod);
}
const BigInteger BigInteger::multiplyByInt(long long other) const {
    if (!*this || !other)
        return 0;
    if (other == -1)
        return -*this;
    std::vector<long long> resData(1);
    signed char resSignum = (char) (this->sign * (other >= 0 ? 1 : -1));
    long long tmp = 0;
    for (size_t i = 0; i < data.size() || tmp; ++i) {
        if (i < data.size())
            tmp += data[i] * other;
        if (mod(tmp, radix)) {
            resData.resize(i + 1);
            resData[i] = mod(tmp, radix);
        }
        tmp -= mod(tmp, radix);
        tmp /= radix;
    }
    return BigInteger(resData, resSignum);
}

BigInteger &BigInteger::operator/=(const BigInteger &other) {
    return *this = divMod(other).first;
}
BigInteger &BigInteger::operator%=(const BigInteger &other) {
    return *this = divMod(other).second;
}
BigInteger &BigInteger::operator+=(const BigInteger &other) {
    signed char resSignum = (char) (*this >= -other ? 1 : -1);
    long long tmp = 0;
    size_t originalSize = data.size();
    size_t resSize = 1;
    for (size_t i = 0; i < originalSize || i < other.data.size() || tmp; ++i) {
        if (i < originalSize)
            tmp += data[i] * sign * resSignum;
        if (i < other.data.size())
            tmp += other.data[i] * other.sign * resSignum;
        if (i >= originalSize)
            data.resize(i + 1);
        data[i] = mod(tmp, radix);
        if (mod(tmp, radix))
            resSize = i + 1;
        tmp -= mod(tmp, radix);
        tmp /= radix;
    }
    data.resize(resSize);
    sign = resSignum;
    return *this;
}
BigInteger &BigInteger::operator-=(const BigInteger &other) {
    return *this += -other;
}
BigInteger &BigInteger::operator*=(const BigInteger &other) {
    if (!*this || !other)
        return *this = 0;

    signed char newSignum = sign * other.sign;
    if (other.data.size() == 1 && data.size() == 1) {
        long long res = data[0] * other.data[0];
        if (res >= radix) {
            std::vector<long long> newData = {res % radix, res / radix};
            return *this = BigInteger(newData, newSignum);
        } else {
            std::vector<long long> newData = {res};
            return *this = BigInteger(newData, newSignum);
        }
    }
    unsigned int len = std::max(data.size(), other.data.size()) / 2;
    std::pair<BigInteger, BigInteger> a = splitDigits(len);
    std::pair<BigInteger, BigInteger> b = other.splitDigits(len);
    BigInteger a1b1 = a.first * b.first;
    BigInteger a2b2 = a.second * b.second;
    a1b1.sign = 1;
    a2b2.sign = 1;
    BigInteger a1b2PlusA2b1 = (a.first + a.second) * (b.first + b.second) - a1b1 - a2b2;
    BigInteger res = a1b1 + a1b2PlusA2b1.shiftLeft(len) + a2b2.shiftLeft(2 * len);
    res.sign = newSignum;
    return *this = res;
}

BigInteger &BigInteger::operator--() {
    return *this -= 1;
}
BigInteger &BigInteger::operator++() {
    return *this += 1;
}
const BigInteger BigInteger::operator--(int) {
    BigInteger ret(*this);
    --*this;
    return ret;
}
const BigInteger BigInteger::operator++(int) {
    BigInteger ret(*this);
    ++*this;
    return ret;
}

std::istream &operator>>(std::istream &stream, BigInteger &integer) {
    integer = 0;
    std::string s;
    stream >> s;
    long long buffer = 0;
    for (size_t i = (s[0] == '-' ? 1 : 0); i < s.length(); ++i) {
        buffer *= 10;
        buffer += s[i] - '0';
        if ((s.length() - i - 1) % BigInteger::radixPow == 0) {
            integer = integer.shiftLeft(1);
            integer += buffer;
            buffer = 0;
        }
    }
    if (s[0] == '-')
        integer.sign = -1;
    return stream;
}
std::ostream &operator<<(std::ostream &stream, const BigInteger &integer) {
    stream << integer.toString();
    return stream;
}