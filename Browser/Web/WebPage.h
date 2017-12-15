#ifndef WEBPAGE_H
#define WEBPAGE_H

#include "UserScript.h"
#include <QString>
#include <QWebPage>

/**
 * @class WebPage
 * @brief Wrapper class for QT's QWebPage, used in order to
 *        handle cookies properly
 */
class WebPage : public QWebPage
{
    Q_OBJECT

public:
    /// WebPage constructor
    WebPage(QObject *parent = 0);

    /// Sets the network access manager to the browser's private browsing one
    void enablePrivateBrowsing();

    /// Sets the user agent to be sent with all requests
    static void setUserAgent(const QString &userAgent);

protected:
    /// Returns the user agent for a given url. If no user agent has been manually specified, the default agent will be sent
    QString userAgentForUrl(const QUrl &url) const override;

private slots:
    /// Attempts to handle unsupported network replies
    void onUnsupportedContent(QNetworkReply *reply);

    /// Called when a frame has started loading
    void onLoadStarted();

    /// Called when a frame is finished loading
    void onLoadFinished(bool ok);

    /// Injects any javascript into the frame, if applicable
    void injectUserJavaScript(QWebFrame *frame, ScriptInjectionTime injectionTime);

protected:
    /// Custom user agent string. Empty if default UA is used
    static QString m_userAgent;
};

#endif // WEBPAGE_H
