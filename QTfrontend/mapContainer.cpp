/*
 * Hedgewars, a worms-like game
 * Copyright (c) 2006 Igor Ulyanov <iulyanov@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "mapContainer.h"

#include <QPushButton>
#include <QBuffer>
#include <QUuid>
#include <QBitmap>
#include <QPainter>
#include <QLinearGradient>
#include <QColor>
#include <QTextStream>

#include "hwconsts.h"

HWMapContainer::HWMapContainer(QWidget * parent) :
  QWidget(parent), mainLayout(this)
{
  imageButt=new QPushButton(this);
  imageButt->setFixedSize(256, 128);
  imageButt->setFlat(true);
  imageButt->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  mainLayout.addWidget(imageButt);
  connect(imageButt, SIGNAL(clicked()), this, SLOT(setRandomSeed()));
  setRandomSeed();

  chooseMap=new QComboBox(this);
  QDir tmpdir;
  tmpdir.cd(datadir->absolutePath());
  tmpdir.cd("Maps");
  tmpdir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
  QStringList mapList=tmpdir.entryList(QStringList("*"));
  mapList.push_front(QComboBox::tr("generated map..."));
  chooseMap->addItems(mapList);
  connect(chooseMap, SIGNAL(activated(int)), this, SLOT(mapChanged(int)));

  mainLayout.addWidget(chooseMap);
}

void HWMapContainer::setImage(const QImage newImage)
{
  QPixmap px(256, 128);
  QPixmap pxres(256, 128);
  QPainter p(&pxres);

  px.fill(Qt::yellow);
  QBitmap bm = QBitmap::fromImage(newImage);
  px.setMask(bm);

  QLinearGradient linearGrad(QPoint(128, 0), QPoint(128, 128));
  linearGrad.setColorAt(1, QColor(0, 0, 192));
  linearGrad.setColorAt(0, QColor(66, 115, 225));
  p.fillRect(QRect(0, 0, 256, 128), linearGrad);
  p.drawPixmap(QPoint(0, 0), px);

  imageButt->setIcon(pxres);
  imageButt->setIconSize(QSize(256, 128));
  chooseMap->setCurrentIndex(0);
}

void HWMapContainer::mapChanged(int index)
{
  if(!index) {
    changeImage();
    return;
  }

  QPixmap mapImage;
  if(!mapImage.load(datadir->absolutePath() + "/Maps/" + chooseMap->currentText() + "/map.png")) {
    changeImage();
    chooseMap->setCurrentIndex(0);
    return;
  }
  imageButt->setIcon(mapImage.scaled(256, 128));
  QFile mapCfgFile(datadir->absolutePath() + "/Maps/" + chooseMap->currentText() + "/map.cfg");
  if (mapCfgFile.open(QFile::ReadOnly)) {
    QTextStream input(&mapCfgFile);
    input >> theme;
    mapCfgFile.close();
  }
  emit mapChanged(chooseMap->currentText());
}

void HWMapContainer::changeImage()
{
  pMap = new HWMap();
  connect(pMap, SIGNAL(ImageReceived(const QImage)), this, SLOT(setImage(const QImage)));
  pMap->getImage(m_seed.toStdString());
  theme = (Themes->size() > 0) ? Themes->at(rand() % Themes->size()) : "steel";
}

QString HWMapContainer::getCurrentSeed() const
{
  return m_seed;
}

QString HWMapContainer::getCurrentMap() const
{
  if(!chooseMap->currentIndex()) return QString();
  return chooseMap->currentText();
}

QString HWMapContainer::getCurrentTheme() const
{
	return theme;
}

void HWMapContainer::resizeEvent ( QResizeEvent * event )
{
  //imageButt->setIconSize(imageButt->size());
}

void HWMapContainer::setSeed(const QString & seed)
{
	m_seed = seed;
	changeImage();
}

void HWMapContainer::setMap(const QString & map)
{

}

void HWMapContainer::setTheme(const QString & theme)
{
	this->theme = theme;
}

void HWMapContainer::setRandomSeed()
{
  m_seed = QUuid::createUuid().toString();
  emit seedChanged(m_seed);
  changeImage();
  emit themeChanged(theme);
}
