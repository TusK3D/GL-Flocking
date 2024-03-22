#include "Parser.h"

Parser::Parser()
{
}

Parser::Parser(int argc, char *argv[])
{
  if (argc <=1)
  {
    std::cout <<"Usage FileRead [filename] "<<std::endl;
  }
  // open the file to parse
  std::fstream fileIn;
  fileIn.open(argv[1],std::ios::in);
  if (!fileIn.is_open())
  {
    std::cout <<"File : "<<argv[1]<<" Not found Exiting "<<std::endl;
  }

  // this is the line we wish to parse
  std::string lineBuffer;
  // say which separators should be used in this
  // case Spaces, Tabs and return \ new line
  boost::char_separator<char> sep(" \t\r\n");

  // loop through the file
  while(!fileIn.eof())
  {
    // grab a line from the input
    getline(fileIn,lineBuffer,'\n');
    // make sure it's not an empty line
    if(lineBuffer.size() !=0)
    {
      // now tokenize the line
      tokenizer tokens(lineBuffer, sep);
      // and get the first token
      tokenizer::iterator  firstWord = tokens.begin();
      // now see if it's a valid one and call the correct function
      if (*firstWord == "Radius")
      {
        parseBoidRad(firstWord);
      }
      else
        if (*firstWord == "Velocity")
        {
          parseBoidVel(firstWord);
        }
      else
        if (*firstWord == "Accel")
        {
          parseBoidAccel(firstWord);
        }
      else
        if (*firstWord == "MaxSpeed")
        {
          parseBoidMaxSpeed(firstWord);
        }
      else
        if (*firstWord == "SeparationFactor")
        {
          parseBoidSepFactor(firstWord);
        }
      else
        if (*firstWord == "AlignmentFactor")
        {
          parseBoidAlignFactor(firstWord);
        }
      else
        if (*firstWord == "CohesionFactor")
        {
          parseBoidCohesionFactor(firstWord);
        }
      else
        if (*firstWord == "PredatorMaxSpeedFactor")
        {
          parsePredMaxSpeedFactor(firstWord);
        }
      else
        if (*firstWord == "Prey")
        {
          parseBoidPreyStatus(firstWord);
        }
      else
        if (*firstWord == "Shape")
          {
            parseBoidShape(firstWord);
          }
    }
  }


      fileIn.close();
}

void Parser::parseBoidRad(tokenizer::iterator &_firstWord)
{
  // skip first token
  ++_firstWord;
  // use lexical cast to convert to float then increment the itor
  m_rad=boost::lexical_cast<float>(*_firstWord++);
    // now print out values to prove it works
//  std::cout<<(boost::format("Radius [%0.2f]\n") % m_rad);
  std::cout<<"the radius is: "<<m_rad<<std::endl;
}

void Parser::parseBoidVel(tokenizer::iterator &_firstWord)
{
  // skip first token
  ++_firstWord;
  // use lexical cast to convert to float then increment the itor
  m_vel.m_x=boost::lexical_cast<float>(*_firstWord++);
  m_vel.m_y=boost::lexical_cast<float>(*_firstWord++);
  m_vel.m_z=boost::lexical_cast<float>(*_firstWord++);
    // now print out values to prove it works
//  std::cout<<(boost::format("Radius [%0.2f]\n") % m_rad);
  std::cout<<"the velocity is: "<<m_vel<<std::endl;
}

void Parser::parseBoidAccel(tokenizer::iterator &_firstWord)
{
  // skip first token
  ++_firstWord;
  // use lexical cast to convert to float then increment the itor
  m_accel.m_x=boost::lexical_cast<float>(*_firstWord++);
  m_accel.m_y=boost::lexical_cast<float>(*_firstWord++);
  m_accel.m_z= boost::lexical_cast<float>(*_firstWord++);
    // now print out values to prove it works
//  std::cout<<(boost::format("Radius [%0.2f]\n") % m_rad);
  std::cout<<"the acceleration is: "<<m_accel<<std::endl;
}

void Parser::parseBoidMaxSpeed(tokenizer::iterator &_firstWord)
{
  // skip first token
  ++_firstWord;
  // use lexical cast to convert to float then increment the itor
  m_maxSpeed=boost::lexical_cast<float>(*_firstWord++);
    // now print out values to prove it works
//  std::cout<<(boost::format("Radius [%0.2f]\n") % m_rad);
  std::cout<<"the maxspeed is: "<<m_maxSpeed<<std::endl;
}

void Parser::parseBoidSepFactor(tokenizer::iterator &_firstWord)
{
  // skip first token
  ++_firstWord;
  // use lexical cast to convert to float then increment the itor
  m_sepFactor=boost::lexical_cast<float>(*_firstWord++);
    // now print out values to prove it works
//  std::cout<<(boost::format("Radius [%0.2f]\n") % m_rad);
  std::cout<<"the sparation is: "<<m_sepFactor<<std::endl;
}

void Parser::parseBoidAlignFactor(tokenizer::iterator &_firstWord)
{
  // skip first token
  ++_firstWord;
  // use lexical cast to convert to float then increment the itor
  m_alignFactor=boost::lexical_cast<float>(*_firstWord++);
    // now print out values to prove it works
//  std::cout<<(boost::format("Radius [%0.2f]\n") % m_rad);
  std::cout<<"the alignment is: "<<m_alignFactor<<std::endl;
}

void Parser::parseBoidCohesionFactor(tokenizer::iterator &_firstWord)
{
  // skip first token
  ++_firstWord;
  // use lexical cast to convert to float then increment the itor
  m_cohesionFactor=boost::lexical_cast<float>(*_firstWord++);
    // now print out values to prove it works
//  std::cout<<(boost::format("Radius [%0.2f]\n") % m_rad);
  std::cout<<"the cohesion is: "<<m_cohesionFactor<<std::endl;
}

void Parser::parsePredMaxSpeedFactor(tokenizer::iterator &_firstWord)
{
  // skip first token
  ++_firstWord;
  // use lexical cast to convert to float then increment the itor
  m_predMaxSpeedFactor=boost::lexical_cast<float>(*_firstWord++);
    // now print out values to prove it works
//  std::cout<<(boost::format("Radius [%0.2f]\n") % m_rad);
  std::cout<<"the predator max speed is: "<<m_predMaxSpeedFactor<<std::endl;
}

void Parser::parseBoidPreyStatus(tokenizer::iterator &_firstWord)
{
  // skip first token
  ++_firstWord;
  // use lexical cast to convert to float then increment the itor
  m_preyStatus=boost::lexical_cast<float>(*_firstWord++);
    // now print out values to prove it works
//  std::cout<<(boost::format("Radius [%0.2f]\n") % m_rad);
  std::cout<<"the boidprey status is: "<<m_preyStatus<<std::endl;
}

void Parser::parseBoidShape(tokenizer::iterator &_firstWord)
{
  // skip first token
  ++_firstWord;
  // use lexical cast to convert to float then increment the itor
  m_boidShape=boost::lexical_cast<int>(*_firstWord++);
    // now print out values to prove it works
//  std::cout<<(boost::format("Radius [%0.2f]\n") % m_rad);
  std::cout<<"the boid shape is: "<<m_preyStatus<<std::endl;
}

