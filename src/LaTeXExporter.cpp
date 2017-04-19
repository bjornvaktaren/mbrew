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
     << "\n"
     << "\\begin{document}\n"
     << "\\maketitle\n"
     << "\n"
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
     << "\\end{document}\n";
}

   
