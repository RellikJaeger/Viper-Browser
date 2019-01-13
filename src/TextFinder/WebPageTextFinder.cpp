#include "WebPageTextFinder.h"
#include "WebPage.h"

#include <functional>

WebPageTextFinder::WebPageTextFinder(QObject *parent) :
    ITextFinder(parent),
    m_page(nullptr),
    m_lastSearchTerm()
{
}

WebPageTextFinder::~WebPageTextFinder()
{
}

void WebPageTextFinder::stopSearching()
{
    m_searchTerm = QString();
    if (m_page != nullptr)
        m_page->findText(m_searchTerm);
}

void WebPageTextFinder::findNext()
{
    if (!m_page)
        return;

    auto flags = QFlags<WebPage::FindFlag>();

    if (m_matchCase)
        flags |= WebPage::FindCaseSensitively;

    m_page->findText(m_searchTerm, flags, std::bind(&WebPageTextFinder::onFindTextResult, this, true, std::placeholders::_1));
}

void WebPageTextFinder::findPrevious()
{
    if (!m_page)
        return;

    QFlags<WebPage::FindFlag> flags = WebPage::FindBackward;

    if (m_matchCase)
        flags |= WebPage::FindCaseSensitively;

    m_page->findText(m_searchTerm, flags, std::bind(&WebPageTextFinder::onFindTextResult, this, false, std::placeholders::_1));
}

void WebPageTextFinder::setHighlightAll(bool on)
{
    m_highlightAll = on;
}

void WebPageTextFinder::setMatchCase(bool on)
{
    m_matchCase = on;
}

void WebPageTextFinder::searchTermChanged(const QString &text)
{
    if (text.isEmpty() || m_lastSearchTerm.isEmpty() || !m_searchTerm.startsWith(text))
        m_numOccurrences = 0;

    m_lastSearchTerm = m_searchTerm;
    m_searchTerm = text;
}

void WebPageTextFinder::setWebPage(WebPage *page)
{
    if (m_page)
        stopSearching();

    m_page = page;
}

void WebPageTextFinder::onFindTextResult(bool isFindingNext, bool isFound)
{
    const bool isEmptySearch = m_searchTerm.isEmpty();
    if (!isFound || isEmptySearch || !m_page)
    {
        QString resultText = isEmptySearch ? QString() : tr("Phrase not found");
        emit showMatchResultText(resultText);
        return;
    }

    // Check if we need to search the page again to find the number of occurrences
    if (m_numOccurrences == 0)
    {
        m_occurrenceCounter = 0;
        m_page->toPlainText(std::bind(&WebPageTextFinder::onRetrieveWebPageText, this, isFindingNext, std::placeholders::_1));
    }
    else
        updatePositionTracker(isFindingNext);
}

void WebPageTextFinder::onRetrieveWebPageText(bool isFindingNext, const QString &text)
{
    Qt::CaseSensitivity caseSensitivity = m_matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive;
    m_numOccurrences = text.count(m_searchTerm, caseSensitivity);

    updatePositionTracker(isFindingNext);
}

void WebPageTextFinder::updatePositionTracker(bool isFindingNext)
{
    // Update position counter
    int positionModifier = isFindingNext ? 1 : -1;
    m_occurrenceCounter = (m_occurrenceCounter + positionModifier) % (m_numOccurrences + 1);
    if (m_occurrenceCounter == 0)
        m_occurrenceCounter = isFindingNext ? 1 : m_numOccurrences;

    // Send text representation to the UI
    QString resultText = tr("%1 of %2 matches").arg(m_occurrenceCounter).arg(m_numOccurrences);
    emit showMatchResultText(resultText);
}
