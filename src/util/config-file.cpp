/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (C) 2014 Named Data Networking Project
 * See COPYING for copyright and distribution information.
 */


#include "config-file.hpp"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>

namespace ndn {

ConfigFile::ConfigFile()
  : m_path(findConfigFile())
{
  if (open())
    {
      parse();
      close();
    }
}

ConfigFile::~ConfigFile()
{
  if (m_input.is_open())
    {
      m_input.close();
    }
}

boost::filesystem::path
ConfigFile::findConfigFile()
{
  using namespace boost::filesystem;

  path home(std::getenv("HOME"));
  if (!home.empty())
    {
      home /= ".ndn/client.conf";
      if (exists(home))
        {
          return absolute(home);
        }
    }

#ifdef NDN_CPP_SYSCONFDIR
  path sysconfdir(NDN_CPP_SYSCONFDIR);
  sysconfdir /= "ndn/client.conf";

  if (exists(sysconfdir))
    {
      return absolute(sysconfdir);
    }
#endif // NDN_CPP_SYSCONFDIR

  path etc("/etc/ndn/client.conf");
  if (exists(etc))
    {
      return absolute(etc);
    }

  return path();
}



bool
ConfigFile::open()
{
  if (m_path.empty())
    {
      return false;
    }

  m_input.open(m_path.c_str());
  if (!m_input.good() || !m_input.is_open())
    {
      return false;
    }
  return true;
}

void
ConfigFile::close()
{
  if (m_input.is_open())
    {
      m_input.close();
    }
}


const ConfigFile::Parsed&
ConfigFile::parse()
{
  if (m_path.empty())
    {
      throw Error("Failed to locate configuration file for parsing");
    }
  else if (!m_input.is_open() && !open())
    {
      throw Error("Failed to open configuration file for parsing");
    }

  try
    {
      boost::property_tree::read_ini(m_input, m_config);
    }
  catch (boost::property_tree::ini_parser_error& error)
    {
      std::stringstream msg;
      msg << "Failed to parse configuration file";
      msg << " " << m_path;
      msg << " " << error.message() << " line " << error.line();
      throw Error(msg.str());
    }
  return m_config;
}

}