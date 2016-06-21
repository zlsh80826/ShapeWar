#ifndef POLYGONGROUP_H
#define POLYGONGROUP_H

#include <QGraphicsScene>
#include <QJsonArray>
#include <QVector>
/*!
 * this class provide a container of Polygon to operate specific functions
 */
template <class Polygon> class PolygonGroup {
private:
    int count;
    QVector<Polygon *> polygons;

public:
    PolygonGroup(int edgeCount, int count = 300) : count(count) {
        for (int i = 0; i < count; ++i)
            polygons.append(new Polygon(edgeCount));
    }

    /*!
     * \brief addToParent adda all polygon in this container to scene
     * \param parent
     */
    void addToParent(QGraphicsScene *parent) {
        for (int i = 0; i < count; ++i) {
            parent->addItem(polygons[i]);
            parent->addItem(polygons[i]->hpBar);
        }
    }

    /*!
     * \brief addToParentNoHPBar adda hp bars of all polygons in this container to scene
     * \param parent
     */
    void addToParentNoHPBar(QGraphicsScene *parent) {
        for (int i = 0; i < count; ++i) {
            parent->addItem(polygons[i]);
        }
    }

    /*!
     * \brief read call the corresponding read function to read Json object for all
     * polygons in this container
     * \param polygonInfo
     */
    void read(const QJsonArray &polygonInfo) {
        for (int i = 0; i < polygonInfo.size() and i < count; ++i) {
            polygons[i]->read(polygonInfo[i].toObject());
        }
    }
};

#endif // POLYGONGROUP_H
