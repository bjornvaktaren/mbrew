#include "LaTeXExporter.hpp"

void LaTeXExporter::save(std::string file)
{
   std::ofstream f;
   f.open(file);
   f << m_preample 
     << "\\subject{Beer Recipe and Logbook}\n"
     << "\\title{" << m_brew->getMetadata("Name") << "}\n"
     << "\\subtitle{" << m_brew->getMetadata("Style") << "}\n"
     << "\\author{" << m_brew->getMetadata("Brewer") << "}\n"
     << "\\date{" << m_brew->getMetadata("Date") << "}\n"
     << '\n'
     << "\\begin{document}\n"
     << "\\maketitle\n"
     << '\n'
     << "\\section*{Specifications}\n"
     << "\\begin{tabular}{l c c}\n"
     << "\\toprule\n"
     << " &  Estimations  & Observed \\\\\n"
     << "\\midrule\n"
     << std::fixed << std::setprecision(0) 
     << "Color & " << m_brew->getColorMoreyEBC() << " EBC & \\\\\n"
     << std::setprecision(1)
     << "Preboil Volume & " << m_brew->getPreboilVolume() << " liter & \\\\\n"
     << "Postboil Volume & " << m_brew->getPostboilVolume() << " liter & \\\\\n"
     << "Fermenter Volume & " << m_brew->getVolumeIntoFermenter() 
     << " liter & \\\\\n" 
     << std::setprecision(3)
     << "Preboil SG & " << m_brew->getPreboilSG() << " & \\\\\n"
     << "OG & " << m_brew->getPostboilSG() << " & \\\\\n"
     << "FG & " << m_brew->getFGLow() << " to " 
     << m_brew->getFGHigh() << " & \\\\\n"
     << std::setprecision(0)
     << "Bitterness & " << m_brew->getTotalIBU() << " IBU & \\\\\n"
     << "\\bottomrule\n"
     << "\\end{tabular}\n"
     << '\n'
     << "\\section*{Fermentables}\n"
     << "\\begin{tabular}{l c c c c c}\n"
     << "\\toprule\n"
     << "Name &  Weight (g) & Added & Color (EBC) & Potential (\\%) & OG \\\\\n"
     << "\\midrule\n";
   double totalWeight = 0.0;
   for ( auto ferm : m_brew->getFermentables() ) {
      f << ferm.name << " & " << ferm.weight << " & " 
	<< ( ferm.mash ? "Mash" : "Boil" ) << " & " << ferm.color << " & "
	<< ferm.extract << " & " << 0.0 << " \\\\\n"; 
      totalWeight += ferm.weight;
   }
   f << "\\midrule\n"
     << "Total & " << totalWeight << " & & & & \\\\\n"
     << "\\bottomrule\n"
     << "\\end{tabular}\n"
     << '\n'
     << "\\section*{Hops}\n"
     << "\\begin{tabular}{l c c c c c}\n"
     << "\\toprule\n"
     << "Name & Alpha acid (\\%) & Weight (g) &"
     << " Time (min) & Bitterness (IBU)\\\\\n"
     << "\\midrule\n";
   totalWeight = 0.0;
   for ( auto h : m_brew->getHops() ) {
      f << h.name << " & " << std::setprecision(1) << std::fixed
	<< h.alpha << " & "  << h.weight << " & " << std::setprecision(0)
	<< h.time << " & " << std::setprecision(1) << m_brew->getIBU(h)
	<< " \\\\\n"; 
      totalWeight += h.weight;
   }
   f << "\\midrule\n"
     << "Total & & " << totalWeight << " & & " << m_brew->getTotalIBU()
     << " \\\\\n"
     << "\\bottomrule\n"
     << "\\end{tabular}\n"
     << '\n'
     << "\\section*{Yeast}\n"
     << "\\begin{tabular}{l c c c}\n"
     << "\\toprule\n"
     << "Name & Attenuation (\\%) & Temperature (C) & Time (days)  \\\\\n"
     << "\\midrule\n";
   double longestTime = 0.0;
   for ( auto y : m_brew->getYeasts() ) {
      f << y.name << " & " << std::fixed << std::setprecision(0)
	<< y.attenuationLow*100.0 << " to " << y.attenuationHigh*100.0 << " & "
	<< std::setprecision(1) << y.temperature << " & "
	<< std::setprecision(0) << y.time << " \\\\\n";
      if ( longestTime < y.time ) longestTime = y.time;
   }
   f << "\\midrule\n"
     << "Total & & & " << std::fixed << std::setprecision(0) << longestTime
     << " \\\\\n"
     << "\\bottomrule\n"
     << "\\end{tabular}\n"
     << '\n'
     << "\\section*{Mash}\n"
     << "\\begin{tabular}{l c c c c}\n"
     << "\\toprule\n"
     << "Name & Volume (liter) & Temperature (C) & Time (min) & "
     << "Strike water temp. (C) \\\\\n"
     << "\\midrule\n";
   for ( auto m : m_brew->getMashes() ) {
      std::stringstream ss1;
      ss1 << std::fixed << std::setprecision(1) << m.mashInTemperature;
      std::stringstream ss2;
      ss2 << std::fixed << std::setprecision(1) << m.volume;
      f << m.name << " & " << std::fixed << std::setprecision(1)
	<< ( m.volume < 0.01 ? "  " : ss2.str() ) 
	<< " & " << m.temperature << " & "
	<< std::setprecision(0) << m.time << " & "
	<< ( std::abs(m.mashInTemperature) > 100.0 ? "  " : ss1.str() )
	<<" \\\\\n";
   }
   f << "\\bottomrule\n"
     << "\\end{tabular}\n"
     << '\n'
     << "\\section*{Notes}\n"
     << m_brew->getNote()
     << '\n'
     << "\\end{document}\n";
}

   
