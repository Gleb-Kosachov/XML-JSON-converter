#ifndef XML_JSON_CONVERTER_H
#define XML_JSON_CONVERTER_H

#include <QXmlStreamReader>
#include <QJsonDocument>
#include <QtXml/QDomDocument>

void XML_to_JSON(const QDomDocument &xml, QJsonDocument &json);
void JSON_to_XML(const QJsonValue &val, const QXmlStreamWriter &writer);

#endif // XML_JSON_CONVERTER_H
