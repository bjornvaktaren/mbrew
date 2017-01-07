#include "Brew.hpp"

Brew::Brew(brewery brewery,
	   std::vector<fermentable> fermentables, 
	   std::vector<mash> mashes, 
	   std::vector<hop> hops) : 
   m_brewery(brewery), 
   m_fermentables(fermentables), 
   m_mashes(mashes), 
   m_hops(hops)
{

}

Brew::Brew(brewery brewery,
	   std::vector<fermentable> fermentables, 
	   std::vector<mash> mashes, 
	   std::vector<hop> hops,
	   std::vector<yeast> yeasts) : 
   m_brewery(brewery), 
   m_fermentables(fermentables), 
   m_mashes(mashes), 
   m_hops(hops), 
   m_yeasts(yeasts)
{

}

double Brew::getPreboilVolume()
{
   double volume = 0.0;
   for ( auto m : m_mashes ) {
      volume += m.volume;
   }
   double maltMass = 0.0;
   for ( auto f : m_fermentables ) {
      if ( !f.mash ) continue;
      maltMass += f.weight;
   }
   return (volume - m_brewery.waterLostToMalt*maltMass*1e-3
	   - m_brewery.mashDeadSpace);
}

double Brew::getBoilDuration()
{
   double boilTime = 0.0;
   // look if total boil time is specified in mashes
   for ( auto m : m_mashes ) {
      if ( m.name.compare("Boil") == 0 || m.name.compare("boil") == 0 ||
	   m.name.compare("BOIL") == 0 ) {
	 boilTime = m.time;
      }
   }
   // if boil not specified in mashed, or if the longest hop addition is longer,
   // take from the longest hop addition
   for ( auto h : m_hops ) {
      if ( h.time > boilTime ) boilTime = h.time;
   }
   return boilTime;
}

double Brew::getPostboilVolume()
{
   return ( getPreboilVolume() 
	    - m_brewery.boilEvaporationRate*getBoilDuration()/60.0 );
}

double Brew::getVolumeAtTime(double boilTime)
{
   return getPreboilVolume() - m_brewery.boilEvaporationRate*boilTime/60.0;
}

double Brew::getVolumeIntoFermenter()
{
   double hopsMass = 0.0;
   for ( auto h : m_hops ) {
      hopsMass += h.weight;
   }
   return (getPostboilVolume() - m_brewery.kettleDeadSpace 
	   - m_brewery.waterLostToHops*hopsMass);
}

double Brew::getIBU(hop h)
{
   // Using the Tinseth formula.
   // IBU = milligrams per liter alpha acid, so
   // IBU = (milligrams_of_hops*alpha_percent/liters_of_wort)*utilization
   // utilization = 1.65*0.000125^(Gb-1) * (1 - exp(-0.04*T))/4.15
   // where Gb is the boil gravity and T is the boil time. (howtobrew.com)

   // calculate average gravity
   double gravity = 0.5*getSG( getVolumeAtTime( getBoilDuration() - h.time ) );
   gravity += 0.5*getSG( getPostboilVolume() );
   double utilization = ( 1.65 * pow(1.25e-4, gravity - 1.0) *
			  (1.0 - exp(-0.04 * h.time) ) / 4.15 );
   return ( m_brewery.hopsUtilization * 10.0 * h.weight * h.alpha * utilization
	    / getPostboilVolume() );
}

double Brew::getTotalIBU()
{
   double ibu = 0.0;
   for ( auto h : m_hops ) {
      ibu += getIBU(h);
   }
   return ibu;
}

double Brew::getOechle(fermentable f, double volume)
{
   double oechle = 0.0;
   oechle += 46.0*f.extract*f.weight*kConst::kkg2lbs*1e-5;
   return m_brewery.efficiency*oechle/volume*kConst::kGal2Litre;
}

double Brew::getPreboilSG()
{
   return getSG(getPreboilVolume());
}

double Brew::getPostboilSG()
{
   return getSG(getPostboilVolume());
}

double Brew::getSG(double volume)
{
   double oechle = 0.0;
   for ( auto f : m_fermentables ) {
      oechle += 46.0*f.extract*f.weight*kConst::kkg2lbs*1e-5;
   }
   return (1.0 + m_brewery.efficiency*oechle*1e-3/volume*kConst::kGal2Litre);
}

double Brew::getFGHigh()
{
   double attenuationLow = 1.0;
   for ( auto y : m_yeasts ) {
      if ( attenuationLow > y.attenuationLow ) {
	 attenuationLow = y.attenuationLow;
      }
   }
   return (1.0 + (getPostboilSG() - 1.0 )*(1.0 - attenuationLow));
}

double Brew::getFGLow()
{
   double attenuationHigh = 0.0;
   for ( auto y : m_yeasts ) {
      if ( attenuationHigh < y.attenuationHigh ) {
	 attenuationHigh = y.attenuationHigh;
      }
   }
   return (1.0 + (getPostboilSG() - 1.0 )*(1.0 - attenuationHigh));
}

double Brew::lovibondToSRM(double lovibond)
{
   return (1.3546*lovibond) - 0.76;
}

double Brew::srmToLovibond(double srm)
{
   return (srm + 0.76)/1.3546;
}

double Brew::getColorMoreyEBC()
{
   // Calculates beer color in EBC using Morey's equation
   // MCU = (grain_color_in_lov * grain_weight_in_lbs) / volume_gallons;
   // SRM_color = 1.4922 * MCU^0.6859
   double mcu = 0.0;
   for ( fermentable f : m_fermentables ) {
      if ( !f.mash ) continue;
      mcu += 1e-3*f.weight*kConst::kkg2lbs*f.color*kConst::kEBC2SRM;
   }
   mcu = mcu/(getPostboilVolume()/kConst::kGal2Litre);
   return 1.4922*pow(mcu,0.6859)/kConst::kEBC2SRM; // Morey's formula
}
