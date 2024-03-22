#ifndef PARSER_H
#define PARSER_H

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ngl/Vec3.h>

/// @file Parser.h
/// @brief brief a Parser class to load default values from a file
/// @author Tushar Kewlani
/// @version 1.0
/// @date 24/03/13
/// Revision History :
/// Initial Version 24/03/13 (Binary day ;-0 )
/// @class Parser
/// @brief imports the default values to be assigned as per need


class Parser
{

  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief default constructor
    //----------------------------------------------------------------------------------------------------------------------
    Parser();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief constructor to initialize the object based on the file passed in
    /// @param argc the arguments entered
    /// @param the string passed as the arguments
    //----------------------------------------------------------------------------------------------------------------------
    Parser(int argc,char *argv[]);

    /// @brief access the radius
    inline float getRadius(){return m_rad;}

    /// @brief access the radius
    inline ngl::Vec3 getVelocity(){return m_vel;}

    /// @brief access the radius
    inline ngl::Vec3 getAccel(){return m_accel;}

    /// @brief access the radius
    inline float getMaxSpeed(){return m_maxSpeed;}

    /// @brief access the radius
    inline float getSepFactor(){return m_sepFactor;}

    /// @brief access the radius
    inline float getAlignFactor(){return m_alignFactor;}

    /// @brief access the radius
    inline float getCohesionFactor(){return m_cohesionFactor;}

    /// @brief access the radius
    inline float getpredatorMaxSpeedFactor(){return m_predMaxSpeedFactor;}

    /// @brief access the radius
    inline float getPreyStatus(){return m_preyStatus;}

    /// @brief access the radius
    inline int getBoidShape(){return m_boidShape;}

  private:

    // create a typecast to tokenizer as it's quicker to write than the whole
    // line
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

    /// @brief finction to parse the radius of the Boid
    void parseBoidRad(
                        tokenizer::iterator &_firstWord);

    /// @brief finction to parse the radius of the Boid
    void parseBoidVel(
                       tokenizer::iterator &_firstWord);

    /// @brief finction to parse the radius of the Boid
    void parseBoidAccel(
                         tokenizer::iterator &_firstWord);

    /// @brief finction to parse the radius of the Boid
    void parseBoidMaxSpeed(
                            tokenizer::iterator &_firstWord);

    /// @brief finction to parse the radius of the Boid
    void parseBoidSepFactor(
                             tokenizer::iterator &_firstWord);

    /// @brief finction to parse the radius of the Boid
    void parseBoidAlignFactor(
                               tokenizer::iterator &_firstWord);

    /// @brief finction to parse the radius of the Boid
    void parseBoidCohesionFactor(
                                  tokenizer::iterator &_firstWord);

    /// @brief finction to parse the radius of the Boid
    void parsePredMaxSpeedFactor(
                                  tokenizer::iterator &_firstWord);

    /// @brief finction to parse the radius of the Boid
    void parseBoidPreyStatus(
                              tokenizer::iterator &_firstWord);

    /// @brief function to parse the shape of the Boid
    void parseBoidShape(
                         tokenizer::iterator &_firstWord);

    float m_rad;
    ngl::Vec3 m_vel;
    ngl::Vec3 m_accel;
    float m_maxSpeed;
    float m_sepFactor;
    float m_alignFactor;
    float m_cohesionFactor;
    float m_predMaxSpeedFactor;
    bool m_preyStatus;
    int m_boidShape;
};
#endif // PARSER_H
