/* $Id: OAuthData.cpp 3761 2013-08-01 08:16:32Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "AuthCore.h"
#include "id/ChatId.h"
#include "oauth2/OAuthData.h"
#include "Settings.h"
#include "sglobal.h"

OAuthData::OAuthData(const QByteArray &provider)
  : htmlName(provider)
  , name(provider)
  , provider(provider)
  , type("oauth2")
{
  redirect = AuthCore::baseUrl() + "/oauth2/" + provider;
}


bool OAuthData::isValid() const
{
  if (provider.isEmpty() || id.isEmpty() || secret.isEmpty() || name.isEmpty() || htmlName.isEmpty())
    return false;

  return true;
}


bool OAuthData::read()
{
  Settings *settings = AuthCore::settings();
  id        = settings->value(provider + LS("/Id")).toByteArray();
  secret    = settings->value(provider + LS("/Secret")).toByteArray();
  publicKey = settings->value(provider + LS("/PublicKey")).toByteArray();

  return isValid();
}


QVariantList OAuthData::toList() const
{
  QVariantList list;
  list.append(provider);
  list.append(name);
  list.append(htmlName);
  list.append(toUrl("${STATE}"));
  return list;
}


QVariantMap OAuthData::toJSON(const QByteArray &state) const
{
  ChatId s(state);
  if (s.isNull())
    s.init(ChatId::MessageId);

  QVariantMap data;
  data[LS("name")]     = name;
  data[LS("htmlName")] = htmlName;
  data[LS("url")]      = toUrl(s.toBase32());
  data[LS("type")]     = type;
  return data;
}
