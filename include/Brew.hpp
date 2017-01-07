#ifndef BREW_H
#define BREW_H

#include <vector>
#include <map>
#include <math.h>
#include <iostream>

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

   double getPreboilVolume();
   double getPostboilVolume();
   double getVolumeIntoFermenter();

   double getIBU(hop h, double gravity);

   double getOechle(fermentable f, double volume);
   double getSG(double volume);
   double getPreboilSG();
   double getPostboilSG();
   double getFGHigh();
   double getFGLow();

   double lovibondToSRM(double lovibond);
   double srmToLovibond(double srm);
   double getColorMoreyEBC(std::vector<fermentable> fermentables, 
			   double volume);
private:
   brewery m_brewery;
   std::vector<fermentable> m_fermentables;
   std::vector<mash> m_mashes;
   std::vector<hop> m_hops;
   std::vector<yeast> m_yeasts;
   std::string m_recipeNote;
   std::map<std::string,std::string> m_metadata;
};

#endif
