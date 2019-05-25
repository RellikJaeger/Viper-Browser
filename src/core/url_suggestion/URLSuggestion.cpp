#include "BookmarkNode.h"
#include "URLRecord.h"
#include "URLSuggestion.h"

URLSuggestion::URLSuggestion(const BookmarkNode *bookmark, const HistoryEntry &historyEntry) :
    Favicon(bookmark->getIcon()),
    Title(bookmark->getName()),
    URL(bookmark->getURL().toString()),
    LastVisit(historyEntry.LastVisit),
    URLTypedCount(historyEntry.URLTypedCount),
    VisitCount(historyEntry.NumVisits),
    IsHostMatch(false),
    IsBookmark(true)
{
}

URLSuggestion::URLSuggestion(const URLRecord &record, const QIcon &icon) :
    Favicon(icon),
    Title(record.getTitle()),
    URL(record.getUrl().toString()),
    LastVisit(record.getLastVisit()),
    URLTypedCount(record.getUrlTypedCount()),
    VisitCount(record.getNumVisits()),
    IsHostMatch(false),
    IsBookmark(false)
{
}
