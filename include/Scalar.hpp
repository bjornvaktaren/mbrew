#ifndef SCALAR_H
#define SCALAR_H

#include <ostream>
#include <array>
#include <sstream>

template <int meter, int kilogram, int second, int ampere, int kelvin,
	  int mole, int candela> class Scalar
{

public:
   
   Scalar<meter, kilogram, second, ampere, kelvin, mole, candela>() :
      m_value{0.0},
      m_exponents{{meter, kilogram, second, ampere, kelvin, mole, candela}}
   {
   };
   
   Scalar<meter, kilogram, second, ampere, kelvin, mole, candela>
   (const double &value) :
      m_value{value},
      m_exponents{{meter, kilogram, second, ampere, kelvin, mole, candela}}
   {
   };

   Scalar<meter, kilogram, second, ampere, kelvin, mole, candela>
   (const Scalar<meter, kilogram, second, ampere, kelvin, mole, candela> &s) :
      m_value{s.getValue()},
      m_exponents{{meter, kilogram, second, ampere, kelvin, mole, candela}}
   {
   };
   
   std::string printUnits() const
   {
      std::stringstream ss;
      for ( int i = 0; i < m_exponents.size(); ++i ) {
	 if ( m_exponents.at(i) != 0 ) {
	    ss <<  m_unitSymbol.at(i) << '^' << m_exponents.at(i) << ' ';
	 }
      }
      return ss.str();
   };

   double getValue() const { return m_value; };
   void setValue(double value) { m_value = value; };


   void
   operator=(const Scalar<meter, kilogram, second, ampere, kelvin, mole,
   	     candela> &scalar)
   {
      m_value = scalar.getValue();
   };

   // Define addition, subtraction, multiplication, and division operators.
   
   Scalar<meter, kilogram, second, ampere, kelvin, mole, candela>
   operator+(const Scalar<meter, kilogram, second, ampere, kelvin, mole,
	     candela>& other)
   {
      return Scalar<meter, kilogram, second, ampere, kelvin, mole, candela>
	 (this->getValue() + other.getValue());
   }
   void
   operator+=(const Scalar<meter, kilogram, second, ampere, kelvin, mole,
	     candela>& other)
   {
      this->setValue(this->getValue() + other.getValue());
   }
   
   Scalar<meter, kilogram, second, ampere, kelvin, mole, candela>
   operator-(const Scalar<meter, kilogram, second, ampere, kelvin, mole,
	     candela>& other)
   {
      return Scalar<meter, kilogram, second, ampere, kelvin, mole, candela>
	 (this->getValue() + other.getValue());
   }
   
   template <int meter2, int kilogram2, int second2, int ampere2, int kelvin2,
	     int mole2, int candela2>
   Scalar<meter+meter2, kilogram+kilogram2, second+second2, ampere+ampere2,
	  kelvin+kelvin2, mole+mole2, candela+candela2>
   operator*(const Scalar<meter2, kilogram2, second2, ampere2, kelvin2, mole2,
	     candela2>& other) 
   {
      return Scalar<meter+meter2, kilogram+kilogram2, second+second2,
		    ampere+ampere2, kelvin+kelvin2, mole+mole2,
		    candela+candela2>
	 (this->getValue()*other.getValue());
   }

   template <int meter2, int kilogram2, int second2, int ampere2, int kelvin2,
	     int mole2, int candela2>
   Scalar<meter-meter2, kilogram-kilogram2, second-second2, ampere-ampere2,
	  kelvin-kelvin2, mole-mole2, candela-candela2>
   operator/(const Scalar<meter2, kilogram2, second2, ampere2, kelvin2, mole2,
	     candela2>& other) 
   {
      return Scalar<meter-meter2, kilogram-kilogram2, second-second2,
		    ampere-ampere2, kelvin-kelvin2, mole-mole2,
		    candela-candela2>
	 (this->getValue()/other.getValue());
   }

protected:
   double m_value;
   const std::array<int,7> m_exponents;
   
private:
   const std::array<std::string,7> m_unitSymbol =
      {"m", "kg", "s", "A", "K", "mol", "cd"};
   
};

// Overload writing to the output stream
template <int meter, int kilogram, int second, int ampere, int kelvin,
	  int mole, int candela>
std::ostream& operator<<(std::ostream& out,
			 const Scalar<meter, kilogram, second, ampere,
			 kelvin, mole, candela> &scalar)
{
   out <<  scalar.getValue() << ' ' << scalar.printUnits();
   return out;
}

namespace SIUnit{
   // static methods to convert to SI units
   double C(double celsius)  { return celsius - 273.15; }
   double kg(double kilogram){ return kilogram; }
   double g(double gram)     { return 1e-3*gram; }
   double s(double second)   { return second; }
   double liter(double liter){ return 1e-3*liter; }
}

// Define common scalars

typedef Scalar<1,0,0,0,0,0,0> Length;
typedef Scalar<2,0,0,0,0,0,0> Area;
typedef Scalar<3,0,0,0,0,0,0> Volume;

typedef Scalar<0,1,0,0,0,0,0> Mass;

typedef Scalar<0,0,1,0,0,0,0> Time;
typedef Scalar<0,0,-1,0,0,0,0> Frequency;

typedef Scalar<0,0,0,1,0,0,0> Current;

typedef Scalar<0,0,0,0,1,0,0> Temperature;

typedef Scalar<-3,1,0,0,0,0,0> Density;
typedef Scalar<2,1,-2,0,-1,0,0> SpecificHeatCapacity;

#endif
