/*
 * Module: lbdata.hh
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */
#ifndef __LBDATA_HH__
#define __LBDATA_HH__

#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

class LBRule {
 public:
  typedef map<string, int> InterfaceDistColl;
  typedef map<string, int>::iterator InterfaceDistIter;

  typedef enum {ALL,ICMP,UDP,TCP} Protocol;

  LBRule() :
    _proto("all")
      {}

 public:
  string _proto;
  string _s_addr;
  string _s_net;
  string _s_port;

  string _d_addr;
  string _d_net;
  string _d_port;

  InterfaceDistColl _iface_dist_coll;
};


class LBHealthHistory {
public:
  LBHealthHistory(int buffer_size);

  //push in the ping response for this...
  int push(int rtt);


public:
  //results of health testing
  unsigned long _last_success;
  unsigned long _last_failure;

  unsigned long _failure_count;

  static int _buffer_size;
  vector<int> _resp_data;
  int _index;
};

class LBHealth {
 public:
  LBHealth() :
    _success_ct(0),
    _failure_ct(0),
    _ping_resp_time(0),
    _hresults(10),
    _is_active(true),
    _state_changed(true)
      {}

  void put(int rtt);

  bool 
  state_changed() const {return _state_changed;}

  unsigned long
  last_success() const {return _hresults._last_success;}

  unsigned long
  last_failure() const {return _hresults._last_failure;}

  unsigned long
  failure_count() const {return _failure_ct;}

  int _success_ct;
  int _failure_ct;
  string _ping_target;
  int _ping_resp_time;
  LBHealthHistory _hresults;
  bool _is_active;
  bool _state_changed;
};


class LBData {
 public:
  typedef map<int,LBRule> LBRuleColl;
  typedef map<int,LBRule>::iterator LBRuleIter;
  typedef map<int,LBRule>::const_iterator LBRuleConstIter;
  typedef map<string,LBHealth> InterfaceHealthColl;
  typedef map<string,LBHealth>::iterator InterfaceHealthIter;
  typedef map<string,LBHealth>::const_iterator InterfaceHealthConstIter;

  LBData() {}

  bool
  error() {return false;}

  bool
  is_active(const string &iface);

  bool
  state_changed();

  void
  reset_state_changed();

  void
  dump();

 public:
  string _filename;

  LBRuleColl _lb_rule_coll;
  InterfaceHealthColl _iface_health_coll;
};

#endif //__LBDATA_HH__
