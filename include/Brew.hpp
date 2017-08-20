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
   void setObservations(BrewSpecifications o) { m_observations = o; }

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

   std::string getMetadata(std::string key) { return m_metadata[key]; };

   double lovibondToSRM(double lovibond);
   double srmToLovibond(double srm);
   double getColorMoreyEBC();

   std::vector<fermentable> getFermentables() { return m_fermentables; };
   std::vector<mash> getMashes() { return m_mashes; };
   std::vector<hop> getHops() { return m_hops; };
   std::vector<yeast> getYeasts() { return m_yeasts; };
   std::string getNote() { return m_recipeNote; };
   BrewSpecifications getObservations() { return m_observations; };

   std::string getBreweryName() { return m_brewery.name; };
   double getBreweryEfficiency() { return m_brewery.efficiency; };
   double getBreweryBoilEvaporationRate()
      { return m_brewery.boilEvaporationRate; };
   double getBreweryMashDeadSpace() { return m_brewery.mashDeadSpace; };
   double getBreweryKettleDeadSpace() { return m_brewery.kettleDeadSpace; };
   double getBreweryWaterLostToMalt() { return m_brewery.waterLostToMalt; };
   double getBreweryWaterLostToHops() { return m_brewery.waterLostToHops; };
   double getBreweryHopsUtilization() { return m_brewery.hopsUtilization; };

   double getObservedOG() { return m_observations.OG; };
   double getObservedFG() { return m_observations.FG; };
   double getObservedPreboilSG() { return m_observations.preboilSG; };
   double getObservedPreboilVolume() { return m_observations.preboilVolume; };
   double getObservedPostboilVolume() { return m_observations.postboilVolume; };
   double getObservedFermenterVolume() {return m_observations.fermenterVolume;};
   double getObservedColor() { return m_observations.color; };
   double getObservedBitterness() { return m_observations.bitterness; };

   double getObservedEfficiency();

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
   BrewSpecifications m_observations;

   // private function
   void calculateStrikeWaterTemperatures();
};

#endif
