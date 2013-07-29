/*
 * Copyright (C) by Krzesimir Nowak <krzesimir@endocode.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#include <QNetworkCookie>
#include <QWebFrame>
#include <QWebPage>

#include "mirall/creds/shibbolethcookiejar.h"
#include "mirall/creds/shibbolethwebview.h"

namespace Mirall
{

ShibbolethWebView::ShibbolethWebView(const QUrl& url, QWidget* parent)
  : QWebView(parent)
{
  ShibbolethCookieJar* jar = new ShibbolethCookieJar(this);
  QWebPage* page = new QWebPage(this);

  connect (jar, SIGNAL (newCookiesForUrl (QList<QNetworkCookie>, QUrl)),
           this, SLOT (onNewCookiesForUrl (QList<QNetworkCookie>, QUrl)));

  page->networkAccessManager()->setCookieJar(jar);
  page->mainFrame ()->load (url);
  this->setPage (page);
}

void ShibbolethWebView::onNewCookiesForUrl (const QList<QNetworkCookie>& cookieList, const QUrl& url)
{
  Q_FOREACH (const QNetworkCookie& cookie, cookieList) {
    if (cookie.name().startsWith ("_shibsession_")) {
      Q_EMIT shibbolethCookieReceived (cookie);

      return;
    }
  }
}

} // ns Mirall