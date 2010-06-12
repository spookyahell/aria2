/* <!-- copyright */
/*
 * aria2 - The high speed download utility
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/* copyright --> */
#ifndef _D_NETRC_H_
#define _D_NETRC_H_

#include "common.h"

#include <string>
#include <vector>
#include <iosfwd>

#include "SharedHandle.h"
#include "A2STR.h"

namespace aria2 {

class Authenticatable {
public:
  virtual ~Authenticatable() {}

  virtual bool match(const std::string& hostname) const = 0;
};

class Authenticator : public Authenticatable {
private:
  std::string _machine;
  std::string _login;
  std::string _password;
  std::string _account;
public:
  Authenticator() {}

  Authenticator(const std::string& machine,
                const std::string& login,
                const std::string& password,
                const std::string& account)
    :_machine(machine),
     _login(login),
     _password(password),
     _account(account) {}

  virtual ~Authenticator() {}

  virtual bool match(const std::string& hostname) const
  {
    return hostname == _machine;
  }

  const std::string& getMachine() const
  {
    return _machine;
  }

  void setMachine(const std::string& machine) { _machine = machine; }

  const std::string& getLogin() const
  {
    return _login;
  }

  void setLogin(const std::string& login) { _login = login; }

  const std::string& getPassword() const
  {
    return _password;
  }

  void setPassword(const std::string& password) { _password = password; }

  const std::string& getAccount() const
  {
    return _account;
  }

  void setAccount(const std::string& account) { _account = account; }
};

class DefaultAuthenticator : public Authenticator {
public:
  DefaultAuthenticator() {}

  DefaultAuthenticator(const std::string& login,
                       const std::string& password,
                       const std::string& account)
    :Authenticator(A2STR::NIL, login, password, account) {}

  virtual ~DefaultAuthenticator() {}

  virtual bool match(const std::string& hostname) const
  {
    return true;
  }
};

class Netrc {
private:
  std::vector<SharedHandle<Authenticator> > _authenticators;

  void storeAuthenticator(const SharedHandle<Authenticator>& authenticator);

  std::string getRequiredNextToken(std::ifstream& f) const;
  
  void skipMacdef(std::ifstream& f) const;
public:
  Netrc() {}

  void parse(const std::string& path);

  SharedHandle<Authenticator> findAuthenticator
  (const std::string& hostname) const;

  const std::vector<SharedHandle<Authenticator> >& getAuthenticators() const
  {
    return _authenticators;
  }

  void addAuthenticator(const SharedHandle<Authenticator>& authenticator)
  {
    _authenticators.push_back(authenticator);
  }

  static const std::string MACHINE;

  static const std::string DEFAULT;

  static const std::string LOGIN;

  static const std::string PASSWORD;

  static const std::string ACCOUNT;

  static const std::string MACDEF;
};

} // namespace aria2

#endif // _D_NETRC_H_
