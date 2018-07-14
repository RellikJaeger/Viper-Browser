#ifndef WEBWIDGET_H
#define WEBWIDGET_H

#include <QPointer>
#include <QWidget>

class MainWindow;
class WebPage;
class WebView;

class QIcon;
class QUrl;
class QWebEngineHistory;

/**
 * @class WebWidget
 * @brief A widget that is used as the container of a \ref WebView and
 *        acts as a bridge between the browser and the web engine implementation
 */
class WebWidget : public QWidget
{
    friend class BrowserTabWidget;

    Q_OBJECT

public:
    /**
     * @brief Constructs the WebWidget
     * @param privateMode Set to true if the web view should be off-the-record, false if a regular web view
     * @param parent Pointer to the parent widget
     */
    explicit WebWidget(bool privateMode, QWidget *parent = nullptr);

    /// Returns the loading progress percentage value of the page as an integer in the range [0,100]
    int getProgress() const;

    /// Loads the resource of the blank page into the view
    void loadBlankPage();

    /// Returns true if the view's page is blank, with no resources being loaded
    bool isOnBlankPage() const;

    /// Returns the icon associated with the current page
    QIcon getIcon() const;

    /// Returns the URL of the icon associated with the current page
    QUrl getIconUrl() const;

    /**
     * @brief Returns the title of the web page being viewed
     *
     * Returns the title of the web page being viewed. If there is no title
     * for the page, and the path of the URL is not empty or the root path,
     * the path following the last '/' will be returned as the title. If only
     * the root path is available, the host will be returned as the title
     * @return The title of the web page being viewed
     */
    QString getTitle() const;

    /// Loads the specified url and displays it
    void load(const QUrl &url);

    /// Returns a pointer to the view's history
    QWebEngineHistory *history() const;

    /// Returns the current url
    QUrl url() const;

    /// Returns a pointer to the web page
    WebPage *page() const;

    /// Returns a pointer to the web view
    WebView *view() const;

    /// Event filter
    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    /// Reloads the current page
    void reload();

    /// Stops loading the current page
    void stop();

signals:
    /// Emitted when the web view should be closed
    void closeRequest();

    /// Emitted when the icon associated with the view is changed. The new icon is specified by icon.
    void iconChanged(const QIcon &icon);

    /// Emitted when the URL of the icon associated with the view is changed. The new URL is specified by url.
    void iconUrlChanged(const QUrl &url);

    /// Emitted when the inspector should be shown
    void inspectElement();

    /// Emitted when a link is hovered over by the user
    void linkHovered(const QString &url);

    /// Emitted when the current page has finished loading. If parameter 'ok' is true, the load was successful.
    void loadFinished(bool ok);

    /// Emitted when the current page has loaded by the given percent, as an integer in range [0,100]
    void loadProgress(int value);

    /// Emitted when the user requests to open a link in the current web page
    void openRequest(const QUrl &url);

    /// Called when the user requests to open a link in a new browser tab
    void openInNewTab(const QUrl &url);

    /// Called when the user requests to open a link in a new browser tab without switching tabs
    void openInNewBackgroundTab(const QUrl &url);

    /// Called when the user requests to open a link in a new browser window
    void openInNewWindowRequest(const QUrl &url, bool privateWindow);

    /// Emitted when the title of the current page has changed to the given string
    void titleChanged(const QString &title);

protected:
    /// Initializes the web view
    void setupView();

private slots:
    /// Shows the context menu on the web page
    void showContextMenuForView();

private:
    /// The actual web view widget being used to render and access web content
    WebView *m_view;

    /// Pointer to the widget's parent window
    MainWindow *m_mainWindow;

    /// True if the widget's view is on a private browsing setting, false if else
    bool m_privateMode;

    /// Global and relative positions of the requested context menu from the active web view
    QPoint m_contextMenuPosGlobal, m_contextMenuPosRelative;

    /// Pointer to the WebView's focus proxy
    QPointer<QWidget> m_viewFocusProxy;
};

#endif // WEBWIDGET_H
