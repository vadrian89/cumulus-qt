#include "Location.h"

Location::Location(QObject *parent) : QObject(parent) {}

Location::Location(QObject *parent, int locationId, QString locationCode, QString locationName)
    : QObject(parent), m_locationId(locationId), m_locationCode(locationCode), m_locationName(locationName) {}
