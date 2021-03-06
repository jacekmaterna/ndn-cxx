/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2018 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 *
 * @author Davide Pesavento <davide.pesavento@lip6.fr>
 */

#ifndef NDN_NET_NETWORK_MONITOR_IMPL_NETLINK_HPP
#define NDN_NET_NETWORK_MONITOR_IMPL_NETLINK_HPP

#include "ndn-cxx-config.hpp"
#include "../network-monitor.hpp"

#ifndef NDN_CXX_HAVE_RTNETLINK
#error "This file should not be included ..."
#endif

#include <boost/asio/posix/stream_descriptor.hpp>

#include <array>
#include <map>

namespace ndn {
namespace net {

class NetlinkMessage;

class NetworkMonitorImplNetlink : public NetworkMonitorImpl
{
public:
  using Error = NetworkMonitor::Error;

  /** \brief initialize netlink socket and start enumerating interfaces
   */
  explicit
  NetworkMonitorImplNetlink(boost::asio::io_service& io);

  ~NetworkMonitorImplNetlink();

  uint32_t
  getCapabilities() const final
  {
    return NetworkMonitor::CAP_ENUM |
           NetworkMonitor::CAP_IF_ADD_REMOVE |
           NetworkMonitor::CAP_STATE_CHANGE |
           NetworkMonitor::CAP_MTU_CHANGE |
           NetworkMonitor::CAP_ADDR_ADD_REMOVE;
  }

  shared_ptr<const NetworkInterface>
  getNetworkInterface(const std::string& ifname) const final;

  std::vector<shared_ptr<const NetworkInterface>>
  listNetworkInterfaces() const final;

private:
  bool
  isEnumerating() const;

  void
  initSocket(int family);

  void
  joinGroup(int group);

  void
  sendDumpRequest(uint16_t nlmsgType);

  void
  asyncRead();

  void
  receiveMessage();

  void
  parseNetlinkMessage(const NetlinkMessage& nlmsg);

  void
  parseLinkMessage(const NetlinkMessage& nlmsg);

  void
  parseAddressMessage(const NetlinkMessage& nlmsg);

  void
  parseRouteMessage(const NetlinkMessage& nlmsg);

  void
  parseErrorMessage(const NetlinkMessage& nlmsg);

  static void
  updateInterfaceState(NetworkInterface& interface, uint8_t operState);

private:
  std::map<int, shared_ptr<NetworkInterface>> m_interfaces; ///< ifindex => interface
  std::array<uint8_t, 16384> m_buffer; ///< netlink messages received from the kernel
  shared_ptr<boost::asio::posix::stream_descriptor> m_socket; ///< the netlink socket
  uint32_t m_pid; ///< our port ID (unicast address for netlink sockets)
  uint32_t m_sequenceNo; ///< sequence number of the last netlink request sent to the kernel
  bool m_isEnumeratingLinks; ///< true if a dump of all links is in progress
  bool m_isEnumeratingAddresses; ///< true if a dump of all addresses is in progress
};

} // namespace net
} // namespace ndn

#endif // NDN_NET_NETWORK_MONITOR_IMPL_NETLINK_HPP
