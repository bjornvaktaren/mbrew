#ifndef UNITS_H
#define UNITS_H

template <int meter, int kilogram, int second, int ampere, int kelvin,
	  int mole, int candela > class Unit
{
public:
   Unit() : m_exponents({meter,kilogram,second,ampere,kelvin,mole,candela}) {};
private:
   std::array<int,7> m_exponents;
};


template <int meter, int kilogram, int second, int ampere, int kelvin,
	  int mole, int candela> class Scalar
   : public Unit<meter, kilogram, second, ampere, kelvin, mole, candela>
{
public:
   Scalar() :
      m_exponents({meter,kilogram,second,ampere,kelvin,mole,candela}),
      m_value(0.0),
      m_scale(1.0)
   {};
   
private:
   std::array<int,7> m_exponents;
   double m_value;
   double m_scale;
};


class Liter : public Scalar<3, 0, 0, 0, 0, 0, 0>
{
public:
   Liter() :
      m_exponents({3, 0, 0, 0, 0, 0, 0}),
      m_value(0.0),
      m_scale(1e-3)
   {};
   
private:
   const std::array<int,7> m_exponents;
   double m_value;
   double m_scale;
};

// typedef long double Liter;
// constexpr Liter operator"" _l(long double liter)
// {
//    return Liter{liter};
// }


typedef long double Meter;
constexpr Meter operator"" _m(long double meter)
{
   return Meter{meter};
}


typedef long double Celsius;
constexpr Celsius operator"" _C(long double celsius)
{
   return Celsius{celsius};
}

#endif
