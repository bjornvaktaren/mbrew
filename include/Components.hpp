#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>

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
   double temperature = 0.0;
   double time = 0.0;   
};

struct yeast
{
   std::string name;
   double temperature = 0.0;
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
   double mashTunMass = 0.0;
   double hopsUtilization = 1.0;
   std::string mashTunMaterial;
};

#endif
