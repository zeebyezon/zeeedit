// Created by David Bizon on 12/06/2025.
#pragma once

template <typename Impl, typename Tag>
class NumericType
{
    Impl value;

public:
    NumericType() : value(0) {}

    NumericType(const NumericType& other) = default;
    NumericType& operator=(const NumericType& other) = default;

    explicit NumericType(Impl v) : value(v) {}
    NumericType& operator=(Impl v) { value = v; return *this; }

    Impl getRawValue() const { return value; }

    NumericType operator+(Impl v) const { return NumericType(value + v); }
    NumericType operator-(Impl v) const { return NumericType(value - v); }
    NumericType operator*(Impl v) const { return NumericType(value * v); }
    NumericType operator/(Impl v) const { return NumericType(value / v); }
};

#define NUMERIC_TYPE(TypeName, ImplType)    using TypeName = NumericType<ImplType, struct TypeName##Tag>;
