#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>
#include <Constants.hpp>
#include <Scalar.hpp>

struct fermentable
{
   std::string name;
   double weight = 0.0;
   bool mash = true;
   double color = 0.0;
   double extract = 0.0;
};

struct hop
{
   std::string name;
   double alpha = 0.0;
   double weight = 0.0;
   double time = 0.0;
};

struct mash
{
   std::string name;
   Volume volume{SIUnit::liter(0.0)};
   Temperature temperature{SIUnit::C(0.0)};
   Time time{SIUnit::s(0.0)};
   Temperature mashInTemperature{SIUnit::C(0.0)};
};

struct yeast
{
   std::string name;
   Temperature temperature{SIUnit::C(0.0)};
   double time = 0.0;   
   double attenuationLow = 0.75;
   double attenuationHigh = 0.75;
};

struct brewery
{
   std::string name = "Default";
   double efficiency = 1.0;
   double boilEvaporationRate = 0.0;
   double mashDeadSpace = 0.0;
   double kettleDeadSpace = 0.0;
   double waterLostToMalt = 1.085; // liter/kg
   double waterLostToHops = 0.00835; // liter/gram
   double hopsUtilization = 1.0;
   SpecificHeatCapacity mashTunSpecificHeatCapacity{510.0}; // stainless steel
   // mash tun starting temperature
   Temperature mashTunTemperature{SIUnit::C(20.0)};
   Mass mashTunMass{SIUnit::kg(0.0)}; // kg, mash tun mass
   // grain starting temperature
   Temperature grainMashInTemperature{SIUnit::C(20.0)};
};

struct BrewSpecifications
{
   double OG = kConst::kDoubleUndefined;
   double FG = kConst::kDoubleUndefined;
   double preboilSG = kConst::kDoubleUndefined;
   double preboilVolume = kConst::kDoubleUndefined;
   double postboilVolume = kConst::kDoubleUndefined;
   double fermenterVolume = kConst::kDoubleUndefined;
   double color = kConst::kDoubleUndefined;
   double bitterness = kConst::kDoubleUndefined;
};

#endif
