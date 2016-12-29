#ifndef COMPONENTS_H
#define COMPONENTS_H

struct fermentable
{
   std::string name;
   float weight = 0.0;
   bool mash = true;
   float color = 0.0;
   float extract = 0.0;
};

struct hop
{
   std::string name;
   float alpha = 0.0;
   float weight = 0.0;
   float time = 0.0;
};

struct mash
{
   std::string name;
   float volume = 0.0;
   float temperature = 0.0;
   float time = 0.0;   
};

struct yeast
{
   std::string name;
   float temperature = 0.0;
   float time = 0.0;   
};

#endif
