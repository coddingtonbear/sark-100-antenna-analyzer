#ifndef VERSION_H
#define VERSION_H

/***************************************************************************
 *   Copyright (C) 2007 by Jeremy Burton   *
 *   jburton@39net-w04   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

//#include <QDomDocument>
//#include <QDomElement>
//#include <QDomNode>

#include "dom.h"

class Version
{
public:
    int major,minor,build;
    char subversion[64];

    Version() {major = minor = build = 0; subversion[0] = 0;}
    Version(int v1,int v2,int v3,const char *s = "") {major = v1; minor = v2; build = v3; strncpy(subversion,s,sizeof(subversion)); subversion[sizeof(subversion)-1]=0;}

    bool operator>(class Version v2) { return major>v2.major || (major==v2.major && (minor>v2.minor || (minor==v2.minor && (build>v2.build || (build==v2.build && strcmp(subversion,v2.subversion)>0))))); }
    bool operator<(class Version v2) { return major<v2.major || (major==v2.major && (minor<v2.minor || (minor==v2.minor && (build<v2.build || (build==v2.build && strcmp(subversion,v2.subversion)<0))))); }
    bool operator>=(class Version v2) { return *this==v2 || *this>v2; }
    bool operator<=(class Version v2) { return *this==v2 || *this<v2; }
    bool operator==(class Version v2) { return major==v2.major && minor==v2.minor && build==v2.build && strcmp(subversion,v2.subversion)==0; }

/*
    void toDom(QDomDocument &doc,QDomElement &parent)
    {
      QDomElement e = doc.createElement( "version" ), el;

      toDom_Text(doc,e,"major",major);
      toDom_Text(doc,e,"minor",minor);
      toDom_Text(doc,e,"build",build);
      toDom_Text(doc,e,"subversion",subversion);

      parent.appendChild(e);
    }

    void fromDom(QDomDocument &,QDomElement &element)
    {
      QDomNode n = element.firstChild();
      while(!n.isNull())
      {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if (!e.isNull())
        {
          if (e.tagName() == "major") { major=e.text().toInt(); }
          else if (e.tagName() == "minor") { minor=e.text().toInt(); }
          else if (e.tagName() == "build") { build=e.text().toInt(); }
          else if (e.tagName() == "subversion") { strncpy(subversion,e.text().toAscii().data(),sizeof(subversion)); subversion[sizeof(subversion)-1]=0; }
        }
        n = n.nextSibling();
      }
    }
*/
};

#endif // VERSION_H
