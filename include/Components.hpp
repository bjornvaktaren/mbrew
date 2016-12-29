#ifndef COMPONENTS_H
#define COMPONENTS_H

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
};

#endif
