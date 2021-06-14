#include "xml_json_converter.h"
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

void writeXml(QXmlStreamWriter &XmlWriter, QJsonValue &val)
{
    if (val.isObject())
    {
        QJsonObject obj = val.toObject();
        for (int i = 0; i < obj.length(); i++)
        {
            XmlWriter.writeStartElement(obj.keys()[i]);
            QJsonValue arg2 = QJsonValue(obj.value(obj.keys()[i]));
            writeXml(XmlWriter, arg2);
            XmlWriter.writeEndElement();
        }
    }
    else if(val.isArray())
    {
        QJsonArray arr = val.toArray();
        for (int i = 0; i < arr.size(); i++)
        {
            XmlWriter.writeStartElement("element");
            QJsonValue arg2 = arr[i];
            writeXml(XmlWriter, arg2);
            XmlWriter.writeEndElement();
        }
    }
    else
    {
        QString value;
        if (val.isBool()) value = val.toBool() ? "true" : "false";
        else if (val.isString()) value = val.toString();
        else if (val.isDouble()) value = std::to_string(val.toDouble()).c_str();
        else if (val.isUndefined()) return;
        else val = std::to_string(val.toInt()).c_str();
        XmlWriter.writeCharacters(value);
    }
}

void JSON_to_XML(QJsonValue &val, QXmlStreamWriter &writer)
{
    writer.writeStartDocument();
    writer.writeStartElement("root");
    writeXml(writer, val);
    writer.writeEndElement();
    writer.writeEndDocument();
}

QJsonValue Convert_Xml_to_JSON(QDomElement el, bool &array)
{
    QJsonObject obj;
    QDomNodeList list = el.childNodes();
    for (int i = 0; i < list.count(); i++)
    {
        QDomNode node = list.at(i);
        if (node.isElement())
        {
            bool a = false;
            QJsonValue object = Convert_Xml_to_JSON(node.toElement(), a);
            if (array)
            {
                QJsonArray arr = obj.begin().value().toArray();
                arr.append(object);
                obj.insert(node.nodeName(), arr);
            }
            else if (obj.contains(node.nodeName()))
            {
                array = true;
                QJsonArray arr;
                QJsonObject::iterator i = obj.find(node.nodeName());
                arr.append(i.value());
                obj.erase(i);
                arr.append(object);
                obj.insert(node.nodeName(), arr);
                continue;
            }
            else
            {
                obj.insert(node.nodeName(), object);
            }
        }
        else return QJsonValue(node.nodeValue());
    }
    if (array) return obj.begin().value();
    else return obj;
}

void XML_to_JSON(const QDomDocument &xml, QJsonDocument &json)
{
    bool array = false;
    QJsonValue JSON = Convert_Xml_to_JSON(xml.documentElement(), array);
    if (array) json = QJsonDocument(JSON.toArray());
    else json = QJsonDocument(JSON.toObject());
}
