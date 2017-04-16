#ifndef BREW_H
#define BREW_H

#include <vector>
#include <map>
#include <math.h>
#include <iostream>
#include <iomanip>

// Local includes
#include <Components.hpp>
#include <Constants.hpp>

class Brew
{
public:
   Brew(){};
   Brew(brewery brewery,
	std::vector<fermentable> fermentables, 
	std::vector<mash> mashes, 
	std::vector<hop> hops);
   Brew(brewery brewery,
	std::vector<fermentable> fermentables, 
	std::vector<mash> mashes, 
	std::vector<hop> hops,
	std::vector<yeast> yeasts);
   ~Brew(){};

   void setNote(std::string note) { m_recipeNote = note; }
   void setMetadata(std::map<std::string,std::string> m) { m_metadata = m; }

   double getStrikeWaterTemperature(mash m);
   double getStrikeWaterTemperature(mash mash, double temp);
   
   double getPreboilVolume();
   double getPostboilVolume();
   double getVolumeIntoFermenter();
   double getVolumeAtTime(double boilTime);

   double getIBU(hop h);
   double getTotalIBU();

   double getOechle(fermentable f, double volume);
   double getSG(double volume);
   double getPreboilSG();
   double getPostboilSG();
   double getBoilDuration();
   double getFGHigh();
   double getFGLow();

   double lovibondToSRM(double lovibond);
   double srmToLovibond(double srm);
   double getColorMoreyEBC();

   void print();
private:
   // class members
   brewery m_brewery;
   std::vector<fermentable> m_fermentables;
   std::vector<mash> m_mashes;
   std::vector<hop> m_hops;
   std::vector<yeast> m_yeasts;
   std::string m_recipeNote;
   std::map<std::string,std::string> m_metadata;

   // private function
   void calculateStrikeWaterTemperatures();
};

#endif
