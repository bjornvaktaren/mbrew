#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>
#include <Constants.hpp>
#include <Units.hpp>

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
   double volume = 0.0;
   Celsius temperature = 0.0_C;
   double time = 0.0;
   Celsius mashInTemperature = 0.0_C;
};

struct yeast
{
   std::string name;
   Celsius temperature = 0.0_C;
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
   double mashTunSpecificHeatCapacity = 510.0; // J/(kg*K), stainless steel
   Celsius mashTunTemperature = 20.0_C; // mash tun starting temperature
   double mashTunMass = 0.0; // kg, mash tun mass
   Celsius grainMashInTemperature = 20.0_C; // mash tun starting temperature
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
